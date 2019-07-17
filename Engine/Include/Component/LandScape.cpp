#include "LandScape.h"
#include "../GameObject/GameObject.h"
#include "Renderer.h"
#include "Material.h"
#include "../Resources/ResourcesManager.h"
#include "../Rendering/RenderManager.h"
#include "../Rendering/RenderState.h"
#include "../Resources/Mesh.h"
#include "../Core/PathManager.h"
#include "../Core/NavigationManager.h"
#include "../Core/Input.h"
#include "../Component/Mouse.h"
#include "../Component/ColliderRay.h"
#include "../Component/ColliderSphere.h"
#include "../Resources/Texture.h"

PG_USING

int CLandScape::number = 0;

CLandScape::CLandScape()
{
	SetTag("LandScape");
	SetTypeName("CLandScape");
	SetTypeID<CLandScape>();
	m_eType = CT_LANDSCAPE;
	m_iDetailLevel = 1;
	m_iSplatCount = 0;

	m_bCheckBrush = false;
	m_fRangeBrush = 1.f;
	m_vPosBrush = Vector3(0.f, 0.f, 0.f);
	m_vColorBrush = Vector4::Red;

	m_vecSplattingDiffuse.resize(4);
	m_vecSplattingNormal.resize(4);
	m_vecSplattingSpecular.resize(4);
	m_vecSplattingAlpha.resize(4);
}


CLandScape::CLandScape(const CLandScape & landscape)	:
	CComponent(landscape)
{
	m_iDetailLevel = landscape.m_iDetailLevel;
	m_iSplatCount = landscape.m_iSplatCount;
}

CLandScape::~CLandScape()
{
	for (auto &iter : m_listNode)
	{
		GET_SINGLE(CResourcesManager)->FindAndDeleteMesh(iter->strNodeName);
		iter->vecVtx.clear();
		iter->vecIndex.clear();
		SAFE_RELEASE(iter->pGameObject);

		for (int i = 0; i < 4; ++i)
		{
			if (nullptr != iter->pNodes[i])
			{
				delete iter->pNodes[i];
				iter->pNodes[i] = nullptr;
			}
		}
	}
	m_listNode.clear();

	for (auto &iter : m_listAllNodes)
	{
		/* 이미 등록된 메시를 지워준다. */
		GET_SINGLE(CResourcesManager)->FindAndDeleteMesh(iter->strNodeName);

		
		for (int i = 0; i < 4; ++i)
		{
			//if (nullptr != iter->pNodes[i])
			//{
			//	delete iter->pNodes[i];
			//	iter->pNodes[i] = nullptr;
			//}
		}
		iter->vecVtx.clear();
		iter->vecIndex.clear();
		SAFE_RELEASE(iter->pGameObject);
		
	}
	m_listAllNodes.clear();

	m_vecSplattingDiffuse.clear();
	m_vecSplattingNormal.clear();
	m_vecSplattingSpecular.clear();
	m_vecSplattingAlpha.clear();

	if (m_pParentNode)
	{
		ReleaseNode(m_pParentNode);
		delete m_pParentNode;
		m_pParentNode = nullptr;
	}


}

void CLandScape::SetDetailLevel(int iDetailLevel)
{
	m_iDetailLevel = iDetailLevel;
}

void CLandScape::SetSplatCount(int count)
{
	m_iSplatCount = count;
}

void CLandScape::SetDetailLevel_Splat(int index, int iDetailLevel)
{
	m_arrDetailLevel_Tex[index] = iDetailLevel;
}

void CLandScape::SetBrushCheck(bool check)
{
	m_bCheckBrush = check; 
}

void CLandScape::SetBrushCheck_Other(bool check)
{
	m_bCheckBrush_Other = check;
}

void CLandScape::SetTerrainSize(int x, int z)
{
	m_iNumX = x;
	m_iNumZ = z;
}

bool CLandScape::CreateLandScape(const string& strMeshKey, int iVtxCount, bool bBump, const string& strTexKey,
	const wchar_t* pFileName,
	const wchar_t* pNormalName, const wchar_t* pSpecularName,
	const char* pHeightMap, const string& strPathKey)
{
	CRenderer*	pRenderer = m_pGameObject->AddComponent<CRenderer>("LandScape");

	CMaterial*	pMaterial = pRenderer->CreateMaterial();

	pMaterial->SetDiffuseTexInfo("Linear", strTexKey, 0, 0, pFileName, strPathKey);
	pMaterial->SetNormalTexInfo("Linear", strTexKey + "_N", 1, 1,
		pNormalName, strPathKey);
	pMaterial->SetSpecularTexInfo("Linear", strTexKey + "_S", 2, 2,
		pSpecularName, strPathKey);

	SAFE_RELEASE(pMaterial);

	pRenderer->SetShader(LANDSCAPE_SHADER);
	pRenderer->SetInputLayout("Bump");

	// 높이맵을 읽어온다.
	const char* pPath = GET_SINGLE(CPathManager)->FindPathToMultiByte(strPathKey);
	string	strPath;

	if (pPath)
		strPath = pPath;
	strPath += pHeightMap;

	DWORD*	pPixel = NULL;
	int	iBitCount = 3;

	FILE*	pFile = NULL;

	fopen_s(&pFile, strPath.c_str(), "rb");

	if (pFile)
	{
		BITMAPFILEHEADER	fh;
		BITMAPINFOHEADER	ih;

		fread(&fh, sizeof(fh), 1, pFile);
		fread(&ih, sizeof(ih), 1, pFile);

		fseek(pFile, fh.bfOffBits, SEEK_SET);

		pPixel = new DWORD[ih.biWidth * ih.biHeight];

		fread(pPixel, sizeof(DWORD), ih.biWidth * ih.biHeight, pFile);

		// 상하가 뒤집어져서 저장되기 때문에 바꿔준다.
		DWORD*	pLine = new DWORD[ih.biWidth];
		for (int i = 0; i < ih.biHeight / 2; ++i)
		{
			memcpy(pLine, &pPixel[i * ih.biWidth], sizeof(DWORD) * ih.biWidth);
			memcpy(&pPixel[i * ih.biWidth],
				&pPixel[(ih.biWidth - 1 - i) * ih.biWidth],
				sizeof(DWORD) * ih.biWidth);
			memcpy(&pPixel[(ih.biWidth - 1 - i) * ih.biWidth],
				pLine, sizeof(DWORD) * ih.biWidth);
		}

		SAFE_DELETE_ARRAY(pLine);

		fclose(pFile);
	}

	m_iNumX = iVtxCount;
	m_iNumZ = iVtxCount;

	m_vecVtx.resize(iVtxCount * iVtxCount);

	for (int i = 0; i < m_iNumZ; ++i)
	{
		for (int j = 0; j < m_iNumX; ++j)
		{
			VERTEXBUMP	tVtx = {};

			tVtx.vPos = Vector3((float)j, 
				(pPixel[i * m_iNumX + j] & 0x000000ff) * 0.1f, (float)(m_iNumZ - 1) - i);
			m_vecPos.push_back(tVtx.vPos);

			tVtx.vNormal = Vector3(0.f, 1.f, 0.f);
			tVtx.vUV = Vector2((float)j / (float)(m_iNumX - 1),
				(float)i / (float)(m_iNumZ - 1));
			//tVtx.vUV = Vector2(j, i);

			m_vecVtx[i * m_iNumX + j] = tVtx;
		}
	}

	SAFE_DELETE_ARRAY(pPixel);
		
	m_vecIndex.resize((m_iNumX - 1) * (m_iNumZ - 1) * 2 * 3);

	int		iCount = 0;

	for (int i = 0; i < m_iNumZ - 1; ++i)
	{
		for (int j = 0; j < m_iNumX - 1; ++j)
		{
			// 좌상단 정점의 인덱스를 구해준다.
			int	idx = i * m_iNumX + j;
			// 우상단 삼각형 인덱스
			m_vecIndex[iCount++] = idx;
			m_vecIndex[iCount++] = idx + 1;
			m_vecIndex[iCount++] = idx + m_iNumX + 1;

			// 삼각형의 면법선을 구한다.
			Vector3	vEdge1 = m_vecPos[idx + 1] - m_vecPos[idx];
			Vector3	vEdge2 = m_vecPos[idx + m_iNumX + 1] - m_vecPos[idx];

			vEdge1 = vEdge1.Normalize();
			vEdge2 = vEdge2.Normalize();

			Vector3	vFaceNormal = vEdge1.Cross(vEdge2);

			m_vecFaceNormal.push_back(vFaceNormal.Normalize());

			// 좌하단 삼각형 인덱스
			m_vecIndex[iCount++] = idx;
			m_vecIndex[iCount++] = idx + m_iNumX + 1;
			m_vecIndex[iCount++] = idx + m_iNumX;

			// 삼각형의 면법선을 구한다.
			vEdge1 = m_vecPos[idx + m_iNumX + 1] - m_vecPos[idx];
			vEdge2 = m_vecPos[idx + m_iNumX] - m_vecPos[idx];

			vEdge1 = vEdge1.Normalize();
			vEdge2 = vEdge2.Normalize();

			vFaceNormal = vEdge1.Cross(vEdge2);

			m_vecFaceNormal.push_back(vFaceNormal.Normalize());
		}
	}

	ComputeNormal(m_vecVtx, m_vecIndex);
	ComputeTangent(m_vecVtx, m_vecIndex);

	CMesh*	pMesh = GET_SINGLE(CResourcesManager)->CreateMesh(strMeshKey,
		m_vecVtx.size(), sizeof(VERTEXBUMP), D3D11_USAGE_DEFAULT,
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, &m_vecVtx[0],
		m_vecIndex.size(), 4, D3D11_USAGE_DEFAULT,
		DXGI_FORMAT_R32_UINT, &m_vecIndex[0]);

	pMesh->SetShaderKey(LANDSCAPE_SHADER);
	pMesh->SetInputLayoutKey("Bump");
	pRenderer->SetMesh(pMesh);

	SAFE_RELEASE(pMesh);

	pRenderer->CreateCBuffer("LandScape", 12, sizeof(LANDSCAPECBUFFER),
		SCT_PIXEL);

	SAFE_RELEASE(pRenderer);

	// 내비게이션 관리자에 지형을 등록한다.
	GET_SINGLE(CNavigationManager)->AddLandScapeInfo(strMeshKey,
		m_iNumX, m_iNumZ, m_pScene, m_pTransform, &m_vecPos);

	return true;
}

bool CLandScape::CreateLandScape(const string & strMeshKey, int iSizeX, int iSizeZ, bool bBump,
	const string & strTexKey, const wchar_t * pFileName,
	const wchar_t * pNormalName, const wchar_t * pSpecularName,
	const char * pHeightMap, const string & strPathKey)
{
	CRenderer*	pRenderer = m_pGameObject->AddComponent<CRenderer>("LandScape");

	CMaterial*	pMaterial = pRenderer->CreateMaterial();

	pMaterial->SetDiffuseTexInfo("Linear", strTexKey, 0, 0, pFileName, strPathKey);
	pMaterial->SetNormalTexInfo("Linear", strTexKey + "_N", 1, 1,
		pNormalName, strPathKey);
	pMaterial->SetSpecularTexInfo("Linear", strTexKey + "_S", 2, 2,
		pSpecularName, strPathKey);

	SAFE_RELEASE(pMaterial);

	pRenderer->SetShader(LANDSCAPE_SHADER);
	pRenderer->SetInputLayout("Bump");

	m_iNumX = iSizeX + 1;
	m_iNumZ = iSizeZ + 1;

	m_vecVtx.resize(m_iNumX * m_iNumZ);

	// Vertex Pos
	for (int i = 0; i < m_iNumZ; ++i)
	{
		for (int j = 0; j < m_iNumX; ++j)
		{
			VERTEXBUMP	tVtx = {};

			tVtx.vPos = Vector3((float)j,
				0.f, (float)(m_iNumZ - 1) - i);
			m_vecPos.push_back(tVtx.vPos);

			tVtx.vNormal = Vector3(0.f, 1.f, 0.f);
			tVtx.vUV = Vector2((float)j / (float)(m_iNumX - 1),
				(float)i / (float)(m_iNumZ - 1));
			//tVtx.vUV = Vector2(j, i);

			m_vecVtx[i * m_iNumX + j] = tVtx;
		}
	}

	// Index Buffer
	m_vecIndex.resize((m_iNumX - 1) * (m_iNumZ - 1) * 2 * 3);

	int		iCount = 0;

	for (int i = 0; i < m_iNumZ - 1; ++i)
	{
		for (int j = 0; j < m_iNumX - 1; ++j)
		{
			// 좌상단 정점의 인덱스를 구해준다.
			int	idx = i * m_iNumX + j;
			// 우상단 삼각형 인덱스
			m_vecIndex[iCount++] = idx;
			m_vecIndex[iCount++] = idx + 1;
			m_vecIndex[iCount++] = idx + m_iNumX + 1;

			// 삼각형의 면법선을 구한다.
			Vector3	vEdge1 = m_vecPos[idx + 1] - m_vecPos[idx];
			Vector3	vEdge2 = m_vecPos[idx + m_iNumX + 1] - m_vecPos[idx];

			vEdge1 = vEdge1.Normalize();
			vEdge2 = vEdge2.Normalize();

			Vector3	vFaceNormal = vEdge1.Cross(vEdge2);

			m_vecFaceNormal.push_back(vFaceNormal.Normalize());

			// 좌하단 삼각형 인덱스
			m_vecIndex[iCount++] = idx;
			m_vecIndex[iCount++] = idx + m_iNumX + 1;
			m_vecIndex[iCount++] = idx + m_iNumX;

			// 삼각형의 면법선을 구한다.
			vEdge1 = m_vecPos[idx + m_iNumX + 1] - m_vecPos[idx];
			vEdge2 = m_vecPos[idx + m_iNumX] - m_vecPos[idx];

			vEdge1 = vEdge1.Normalize();
			vEdge2 = vEdge2.Normalize();

			vFaceNormal = vEdge1.Cross(vEdge2);

			m_vecFaceNormal.push_back(vFaceNormal.Normalize());
		}
	}

	ComputeNormal(m_vecVtx, m_vecIndex);
	ComputeTangent(m_vecVtx, m_vecIndex);



	CMesh*	pMesh = GET_SINGLE(CResourcesManager)->CreateMesh(strMeshKey,
		m_vecVtx.size(), sizeof(VERTEXBUMP), D3D11_USAGE_DEFAULT,
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, &m_vecVtx[0],
		m_vecIndex.size(), 4, D3D11_USAGE_DEFAULT,
		DXGI_FORMAT_R32_UINT, &m_vecIndex[0]);

	pMesh->SetShaderKey(LANDSCAPE_SHADER);
	pMesh->SetInputLayoutKey("Bump");
	pRenderer->SetMesh(pMesh);
	pRenderer->SetRenderState(CULLING_NONE);

	SAFE_RELEASE(pMesh);

	pRenderer->CreateCBuffer("LandScape", 12, sizeof(LANDSCAPECBUFFER),
		SCT_PIXEL);

	SAFE_RELEASE(pRenderer);

	// 내비게이션 관리자에 지형을 등록한다.
	GET_SINGLE(CNavigationManager)->AddLandScapeInfo(strMeshKey,
		m_iNumX, m_iNumZ, m_pScene, m_pTransform, &m_vecPos);

	return true;
}

bool CLandScape::CreateLandScapeQuadTree(const string & strMeshKey, 
	int iSizeX, int iSizeZ, bool bBump, const string & strTexKey, const wchar_t * pFileName, 
	const wchar_t * pNormalName, const wchar_t * pSpecularName, const char * pHeightMap, 
	const string & strPathKey)
{
	m_iNumX = iSizeX;
	m_iNumZ = iSizeZ;

	m_vecVtx.resize((m_iNumX + 1) * (m_iNumZ + 1));

	// Vertex Pos
	for (int i = 0; i <= m_iNumZ; ++i)
	{
		for (int j = 0; j <= m_iNumX; ++j)
		{
			VERTEXBUMP	tVtx = {};

			// before
			/*tVtx.vPos = Vector3((float)j,
				0.f, (float)(m_iNumZ) - i);*/
			tVtx.vPos = Vector3((float)j,
				0.f, (float)i);
			m_vecPos.push_back(tVtx.vPos);

			tVtx.vNormal = Vector3(0.f, 1.f, 0.f);
			tVtx.vUV = Vector2((float)(j) / (float)(m_iNumX),
				(float)(m_iNumZ - i) / (float)(m_iNumZ));
			//tVtx.vUV = Vector2(j, i);

			m_vecVtx[i * (m_iNumX + 1) + j] = tVtx;
		}
	}

	// Index Buffer
	m_vecIndex.resize((m_iNumX) * (m_iNumZ) * 2 * 3);

	int		iCount = 0;

	for (int i = 0; i < m_iNumZ; ++i)
	{
		for (int j = 0; j < m_iNumX; ++j)
		{
			// 좌상단 정점의 인덱스를 구해준다.
			int	idx = (i + 1) * (m_iNumX + 1) + j;

			// 좌상단 삼각형 인덱스
			m_vecIndex[iCount++] = idx;
			m_vecIndex[iCount++] = idx + 1;
			m_vecIndex[iCount++] = idx - (m_iNumX + 1);

			// 우하단 삼각형 인덱스
			m_vecIndex[iCount++] = idx - (m_iNumX + 1);
			m_vecIndex[iCount++] = idx + 1;
			m_vecIndex[iCount++] = idx - (m_iNumX + 1) + 1;

			// 삼각형의 면법선을 구하자
			// 좌상단
			Vector3	vEdge1 = m_vecPos[idx - (m_iNumX + 1)] - m_vecPos[idx];
			Vector3	vEdge2 = m_vecPos[idx - (m_iNumX + 1)] - m_vecPos[idx + 1];

			vEdge1 = vEdge1.Normalize();
			vEdge2 = vEdge2.Normalize();

			Vector3	vFaceNormal = vEdge1.Cross(vEdge2);

			m_vecFaceNormal.push_back(vFaceNormal.Normalize());

			// 우하단
			vEdge1 = m_vecPos[idx - (m_iNumX + 1)] - m_vecPos[idx + 1];
			vEdge2 = m_vecPos[idx - (m_iNumX + 1)] - m_vecPos[idx - (m_iNumX + 1) + 1];

			vEdge1 = vEdge1.Normalize();
			vEdge2 = vEdge2.Normalize();

			vFaceNormal = vEdge1.Cross(vEdge2);

			m_vecFaceNormal.push_back(vFaceNormal.Normalize());
		}
	}

	ComputeNormal(m_vecVtx, m_vecIndex);
	ComputeTangent(m_vecVtx, m_vecIndex);

	CreateQuadTree();

	// 컨테이너에 보관
	NodesToContainer();
	CreateTreeNodeToObject();

	// 머테리얼 등록
	for (auto& node : m_listAllNodes)
	{
		CRenderer* pRenderer = node->pGameObject->FindComponentFromTag<CRenderer>("Renderer");
		CMaterial* pMaterial = pRenderer->GetMaterial();

		// Splatting
		if (pFileName)
		{
			pMaterial->SetDiffuseTexInfo("Linear", "LandScape_D", 0, 0, pFileName, TEXTURE_PATH);
		}
		if (pNormalName)
		{
			pMaterial->SetNormalTexInfo("Linear", "LandScape_N", 1, 1, pNormalName, TEXTURE_PATH);
		}
		if (pSpecularName)
		{
			pMaterial->SetSpecularTexInfo("Linear", "LandScape_S", 2, 2, pSpecularName, TEXTURE_PATH);
		}

		SAFE_RELEASE(pMaterial);
		SAFE_RELEASE(pRenderer);
	}

	// 내비게이션 관리자에 지형을 등록한다.
	/*GET_SINGLE(CNavigationManager)->AddLandScapeInfo(strMeshKey,
		m_iNumX, m_iNumZ, m_pScene, m_pTransform, &m_vecVtx[]);*/

	return true;
}

bool CLandScape::SetDiffuseSplattingQuadTree(const string & strSmpKey, const string & strDifKey, const vector<wstring>* pvecPath, const string & strPathKey)
{
	for (auto& node : m_listAllNodes)
	{
		CRenderer*	pRenderer = node->pGameObject->FindComponentFromType<CRenderer>(CT_RENDERER);

		if (!pRenderer)
			return false;
		

		CMaterial*	pMaterial = pRenderer->GetMaterial();

		if (pMaterial)
		{
			pMaterial->AddMultiTexture(strSmpKey, strDifKey, 11, 11, 
				pvecPath, strPathKey);

			SAFE_RELEASE(pMaterial);
		}
		else
		{
			assert(false);
			return false;
		}

		SAFE_RELEASE(pRenderer);
	}

	//++m_iSplatCount;

	return true;
}

bool CLandScape::SetNormalSplattingQuadTree(const string & strSmpKey, const string & strDifKey, const vector<wstring>* pvecPath, const string & strPathKey)
{
	for (auto& node : m_listAllNodes)
	{
		CRenderer*	pRenderer = node->pGameObject->FindComponentFromType<CRenderer>(CT_RENDERER);

		if (!pRenderer)
			return false;


		CMaterial*	pMaterial = pRenderer->GetMaterial();

		if (pMaterial)
		{
			pMaterial->AddMultiTexture(strSmpKey, strDifKey, 12, 11,
				pvecPath, strPathKey);

			SAFE_RELEASE(pMaterial);
		}
		else
		{
			assert(false);
			return false;
		}

		SAFE_RELEASE(pRenderer);
	}

	//++m_iSplatCount;

	return true;
}

bool CLandScape::SetSpecularSplattingQuadTree(const string & strSmpKey, const string & strDifKey, const vector<wstring>* pvecPath, const string & strPathKey)
{
	for (auto& node : m_listAllNodes)
	{
		CRenderer*	pRenderer = node->pGameObject->FindComponentFromType<CRenderer>(CT_RENDERER);

		if (!pRenderer)
			return false;


		CMaterial*	pMaterial = pRenderer->GetMaterial();

		if (pMaterial)
		{
			pMaterial->AddMultiTexture(strSmpKey, strDifKey, 13, 11,
				pvecPath, strPathKey);

			SAFE_RELEASE(pMaterial);
		}
		else
		{
			assert(false);
			return false;
		}

		SAFE_RELEASE(pRenderer);
	}

	//++m_iSplatCount;

	return true;
}

bool CLandScape::SetSplattingAlphaQuadTree(const string & strSmpKey, const string & strDifKey, const vector<wstring>* pvecPath, const string & strPathKey)
{
	for (auto& node : m_listAllNodes)
	{
		CRenderer*	pRenderer = node->pGameObject->FindComponentFromType<CRenderer>(CT_RENDERER);

		if (!pRenderer)
			return false;


		CMaterial*	pMaterial = pRenderer->GetMaterial();

		if (pMaterial)
		{
			pMaterial->AddMultiTexture_Dynamic(strSmpKey, strDifKey, 14, 11,
				pvecPath, strPathKey);
			/*pMaterial->AddMultiTexture(strSmpKey, strDifKey, 14, 11,
				pvecPath, strPathKey);*/
		
			SAFE_RELEASE(pMaterial);
		}
		else
		{
			assert(false);
			return false;
		}

		SAFE_RELEASE(pRenderer);
	}

	++m_iSplatCount;

	return true;
}

bool CLandScape::SetDiffuseSplatting(const string & strSmpKey, const string & strDifKey,
	const vector<wstring>* pvecPath, const string & strPathKey)
{
	CRenderer*	pRenderer = FindComponentFromType<CRenderer>(CT_RENDERER);

	if (!pRenderer)
		return false;

	CMaterial*	pMaterial = pRenderer->GetMaterial();

	if (pMaterial)
	{
		pMaterial->AddMultiTexture(strSmpKey, strDifKey, 11, 11,
			pvecPath, strPathKey);

		SAFE_RELEASE(pMaterial);
	}
	else
	{
		assert(false);
		return false;
	}

	SAFE_RELEASE(pRenderer);

	++m_iSplatCount;

	return true;
}

bool CLandScape::SetNormalSplatting(const string & strSmpKey, const string & strDifKey,
	const vector<wstring>* pvecPath, const string & strPathKey)
{
	CRenderer*	pRenderer = FindComponentFromType<CRenderer>(CT_RENDERER);

	if (!pRenderer)
		return false;

	CMaterial*	pMaterial = pRenderer->GetMaterial();

	if (pMaterial)
	{
		pMaterial->AddMultiTexture(strSmpKey, strDifKey, 12, 11,
			pvecPath, strPathKey);

		SAFE_RELEASE(pMaterial);
	}

	else
	{
		assert(false);
		return false;
	}

	SAFE_RELEASE(pRenderer);

	++m_iSplatCount;

	return true;
}

bool CLandScape::SetSpecularSplatting(const string & strSmpKey, const string & strDifKey,
	const vector<wstring>* pvecPath, const string & strPathKey)
{
	CRenderer*	pRenderer = FindComponentFromType<CRenderer>(CT_RENDERER);

	if (!pRenderer)
		return false;

	CMaterial*	pMaterial = pRenderer->GetMaterial();

	if (pMaterial)
	{
		pMaterial->AddMultiTexture(strSmpKey, strDifKey, 13, 11,
			pvecPath, strPathKey);

		SAFE_RELEASE(pMaterial);
	}

	else
	{
		assert(false);
		return false;
	}

	SAFE_RELEASE(pRenderer);

	++m_iSplatCount;

	return true;
}

bool CLandScape::SetSplattingAlpha(const string & strSmpKey, 
	const string & strDifKey, int iRegTex, int iRegSmp, 
	const vector<wstring>* pvecPath, const string & strPathKey)
{
	CRenderer*	pRenderer = FindComponentFromType<CRenderer>(CT_RENDERER);

	if (!pRenderer)
		return false;

	CMaterial*	pMaterial = pRenderer->GetMaterial();

	if (pMaterial)
	{
		pMaterial->AddMultiTexture(strSmpKey, strDifKey, 14, 11,
			pvecPath, strPathKey);

		SAFE_RELEASE(pMaterial);
	}

	else
	{
		assert(false);
		return false;
	}

	SAFE_RELEASE(pRenderer);

	return true;
}

void CLandScape::SetMaterial_DNS_Default(const wchar_t * pFileName, const wchar_t * pNormalName, const wchar_t * pSpecularName)
{
	// 머테리얼 등록
	/*CTexture* pDiffuseTex = GET_SINGLE(CResourcesManager)->FindTexture("LandScape_D");
	CTexture* pNormalTex = GET_SINGLE(CResourcesManager)->FindTexture("LandScape_N");
	CTexture* pSpecularTex = GET_SINGLE(CResourcesManager)->FindTexture("LandScape_S");
	*/
	for (auto& node : m_listAllNodes)
	{
		CRenderer* pRenderer = node->pGameObject->FindComponentFromTag<CRenderer>("Renderer");
		CMaterial* pMaterial = pRenderer->GetMaterial();

		pMaterial->ResetTextureInfo();

		SAFE_RELEASE(pMaterial);
		SAFE_RELEASE(pRenderer);
	}


	GET_SINGLE(CResourcesManager)->FindAndDeleteTexture("LandScape_D");
	GET_SINGLE(CResourcesManager)->FindAndDeleteTexture("LandScape_N");
	GET_SINGLE(CResourcesManager)->FindAndDeleteTexture("LandScape_S");


	for (auto& node : m_listAllNodes)
	{
		CRenderer* pRenderer = node->pGameObject->FindComponentFromTag<CRenderer>("Renderer");
		CMaterial* pMaterial = pRenderer->GetMaterial();

		
		// Splatting
		if (pFileName)
		{
			// 지우고 해줘야하는듯
			pMaterial->SetDiffuseTexInfo("Linear", "LandScape_D", 0, 0, pFileName, TEXTURE_PATH);
		}
		if (pNormalName)
		{
			pMaterial->SetNormalTexInfo("Linear", "LandScape_N", 1, 1, pNormalName, TEXTURE_PATH);
		}
		if (pSpecularName)
		{
			pMaterial->SetSpecularTexInfo("Linear", "LandScape_S", 2, 2, pSpecularName, TEXTURE_PATH);
		}

		SAFE_RELEASE(pMaterial);
		SAFE_RELEASE(pRenderer);
	}
}

void CLandScape::SetMaterial_Splatting(vector<wstring>& vecDif, vector<wstring>& vecNormal, vector<wstring>& vecSpecular, vector<wstring>& vecAlpha,
	const string & strPathKey)
{
	// 터짐 방지
	if (*vecDif.begin()		  == L""||
		*vecNormal.begin()	  == L""||
		*vecSpecular.begin()  == L""||
		*vecAlpha.begin()	  == L"")
	{
		return;
	}

	for (auto& node : m_listAllNodes)
	{
		CRenderer* pRenderer = node->pGameObject->FindComponentFromTag<CRenderer>("Renderer");
		CMaterial* pMaterial = pRenderer->GetMaterial();

		pMaterial->ResetMultiTextureInfo();

		SAFE_RELEASE(pMaterial);
		SAFE_RELEASE(pRenderer);
	}

	GET_SINGLE(CResourcesManager)->FindAndDeleteTexture("SplatDif");
	GET_SINGLE(CResourcesManager)->FindAndDeleteTexture("SplatNormal");
	GET_SINGLE(CResourcesManager)->FindAndDeleteTexture("SplatSpecular");
	GET_SINGLE(CResourcesManager)->FindAndDeleteTexture("SplatAlpha");

	SetDiffuseSplattingQuadTree("Linear", "SplatDif", &vecDif);
	SetNormalSplattingQuadTree("Linear", "SplatNormal", &vecNormal);
	SetSpecularSplattingQuadTree("Linear", "SplatSpecular", &vecSpecular);
	SetSplattingAlphaQuadTree("Linear", "SplatAlpha", &vecAlpha, strPathKey);
}

bool CLandScape::Init()
{
	return true;
}

void CLandScape::Input(float fTime)
{
}

int CLandScape::Update(float fTime)
{
	return 0;
}

int CLandScape::LateUpdate(float fTime)
{
	UpdateNode();	

	return 0;
}

void CLandScape::Collision(float fTime)
{
}

void CLandScape::Render(float fTime)
{

}

CLandScape * CLandScape::Clone()
{
	return new CLandScape(*this);
}

void CLandScape::Save(FILE * pFile)
{
}

void CLandScape::Load(FILE * pFile)
{
}

void CLandScape::ComputeNormal(vector<VERTEXBUMP>& m_vecVtx, 
	const vector<UINT>& vecIdx)
{
	for (size_t i = 0; i < m_vecFaceNormal.size(); ++i)
	{
		int	idx0 = vecIdx[i * 3];
		int	idx1 = vecIdx[i * 3 + 1];
		int	idx2 = vecIdx[i * 3 + 2];

		m_vecVtx[idx0].vNormal += m_vecFaceNormal[i];
		m_vecVtx[idx1].vNormal += m_vecFaceNormal[i];
		m_vecVtx[idx2].vNormal += m_vecFaceNormal[i];
	}

	for (size_t i = 0; i < m_vecVtx.size(); ++i)
	{
		m_vecVtx[i].vNormal = m_vecVtx[i].vNormal.Normalize();
	}
}

void CLandScape::ComputeTangent(vector<VERTEXBUMP>& m_vecVtx, 
	const vector<UINT>& vecIdx)
{
	// 탄젠트 벡터 구함.
	for (size_t i = 0; i < m_vecFaceNormal.size(); ++i)
	{
		int	idx0 = vecIdx[i * 3];
		int	idx1 = vecIdx[i * 3 + 1];
		int	idx2 = vecIdx[i * 3 + 2];

		float	fVtx1[3], fVtx2[3];
		fVtx1[0] = m_vecVtx[idx1].vPos.x - m_vecVtx[idx0].vPos.x;
		fVtx1[1] = m_vecVtx[idx1].vPos.y - m_vecVtx[idx0].vPos.y;
		fVtx1[2] = m_vecVtx[idx1].vPos.z - m_vecVtx[idx0].vPos.z;

		fVtx2[0] = m_vecVtx[idx2].vPos.x - m_vecVtx[idx0].vPos.x;
		fVtx2[1] = m_vecVtx[idx2].vPos.y - m_vecVtx[idx0].vPos.y;
		fVtx2[2] = m_vecVtx[idx2].vPos.z - m_vecVtx[idx0].vPos.z;

		float	ftu[2], ftv[2];
		ftu[0] = m_vecVtx[idx1].vUV.x - m_vecVtx[idx0].vUV.x;
		ftv[0] = m_vecVtx[idx1].vUV.y - m_vecVtx[idx0].vUV.y;

		ftu[1] = m_vecVtx[idx2].vUV.x - m_vecVtx[idx0].vUV.x;
		ftv[1] = m_vecVtx[idx2].vUV.y - m_vecVtx[idx0].vUV.y;

		float	fDen = 1.f / (ftu[0] * ftv[1] - ftu[1] * ftv[0]);

		Vector3	vTangent;
		vTangent.x = (ftv[1] * fVtx1[0] - ftv[0] * fVtx2[0]) * fDen;
		vTangent.y = (ftv[1] * fVtx1[1] - ftv[0] * fVtx2[1]) * fDen;
		vTangent.z = (ftv[1] * fVtx1[2] - ftv[0] * fVtx2[2]) * fDen;

		vTangent = vTangent.Normalize();

		m_vecVtx[idx0].vTangent = vTangent;
		m_vecVtx[idx1].vTangent = vTangent;
		m_vecVtx[idx2].vTangent = vTangent;

		m_vecVtx[idx0].vBinormal = m_vecVtx[idx0].vNormal.Cross(vTangent).Normalize();
		m_vecVtx[idx1].vBinormal = m_vecVtx[idx1].vNormal.Cross(vTangent).Normalize();
		m_vecVtx[idx2].vBinormal = m_vecVtx[idx2].vNormal.Cross(vTangent).Normalize();
	}
}

bool CLandScape::CreateQuadTree()
{
	float fCenterX = 0.f;
	float fCenterZ = 0.f;
	float fWidth = 0.f;

	int vertexCount = m_vecVtx.size();
	int indexCount = m_vecIndex.size();

	m_iTriCount = indexCount;

	//Old
	//CalculateMeshDimensions(vertexCount, fCenterX, fCenterZ, fWidth);
	//New
	CalculateCorrectly(fCenterX, fCenterZ, fWidth);

	m_pParentNode = new QUADTREENODE;
	if (!m_pParentNode)
	{
		return false;
	}

	//Old
	//CreateTreeNode(m_pParentNode, fCenterX, fCenterZ, fWidth);
	//New
	CreateTreeNodeCorrectly(m_pParentNode, fCenterX, fCenterZ, fWidth);

	return true;
}

void CLandScape::CreateTreeNodeToObject()
{
	for (auto& node : m_listAllNodes)
	{
		//if (node->strNodeName == "Node0")
		{
			/* 이미 등록된 메시를 지워준다. */
			GET_SINGLE(CResourcesManager)->FindAndDeleteMesh(node->strNodeName);

			node->pGameObject = CGameObject::CreateObject(node->strNodeName, m_pLayer);

			// Mesh
			CRenderer*	pRenderer = node->pGameObject->AddComponent<CRenderer>("Renderer");
			CMaterial*	pMaterial = pRenderer->CreateMaterial();

			pRenderer->SetRenderState(LANDSCAPE_SHADER);
			pRenderer->SetInputLayout("Bump");
			// 추후에 고쳐야함
			// Splatting

			SAFE_RELEASE(pMaterial);

			// Mesh 설정
			CMesh*	pMesh = GET_SINGLE(CResourcesManager)->CreateMesh(
				node->MeshInfo, node->strNodeName,
				node->vecVtx.size(), sizeof(VERTEXBUMP), D3D11_USAGE_DYNAMIC,
				D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, &node->vecVtx[0],
				node->vecIndex.size(), 4, D3D11_USAGE_DYNAMIC,
				DXGI_FORMAT_R32_UINT, &node->vecIndex[0]);

			pMesh->SetShaderKey(LANDSCAPE_SHADER);
			pMesh->SetInputLayoutKey("Bump");
			pRenderer->SetMesh(pMesh);
			pRenderer->SetRenderState(CULLING_BACK);

			SAFE_RELEASE(pMesh);
			SAFE_RELEASE(pRenderer);

			CColliderAABB*	pCollider = node->pGameObject->AddComponent<CColliderAABB>("Collider");
			pCollider->SetAABB(node->vMin,
				node->vMax);

			SAFE_RELEASE(pCollider);
		}
	}
}

void CLandScape::CalculateMeshDimensions(int vtxCount, float & centerX, float & centerZ, float & meshWidth)
{
	centerX = 0.f;
	centerZ = 0.f;

	for (const auto iter : m_vecVtx)
	{
		centerX += iter.vPos.x;
		centerZ += iter.vPos.z;
	}

	centerX = centerX / (float)vtxCount;
	centerZ = centerZ / (float)vtxCount;

	float fMaxWidth = 0.f;
	float fMaxDepth = 0.f;

	float fMinWidth = fabsf(m_vecVtx[0].vPos.x - centerX);
	float fMinDepth = fabsf(m_vecVtx[0].vPos.z - centerZ);

	for (const auto iter : m_vecVtx)
	{
		float fWidth = fabsf(iter.vPos.x - centerX);
		float fDepth = fabsf(iter.vPos.z - centerZ);

		if (fWidth > fMaxWidth) { fMaxWidth = fWidth; }
		if (fDepth > fMaxDepth) { fMaxDepth = fDepth; }
		if (fWidth < fMinWidth) { fMinWidth = fWidth; }
		if (fDepth < fMinDepth) { fMinDepth = fDepth; }
	}

	float fMaxX = (float)max(fabs(fMinWidth), fabs(fMaxWidth));
	float fMaxZ = (float)max(fabs(fMinDepth), fabs(fMaxDepth));

	meshWidth = max(fMaxX, fMaxZ) * 2.f;
}

void CLandScape::CreateTreeNode(QUADTREENODE * node, float positionX, float positionZ, float width)
{
	node->fCenterX = positionX;
	node->fCenterZ = positionZ;
	node->fWidth = width;

	node->vMin = Vector3(0.f, 0.f, 0.f);
	node->vMax = Vector3(0.f, 0.f, 0.f);

	node->iSizeX = 0;
	node->iSizeZ = 0;

	node->iTriCount = 0;
	node->pGameObject = nullptr;

	for (int i = 0; i < 4; i++)
	{
		node->pNodes[i] = nullptr;
	}

	int iNumTriangles = CountTriangles(positionX, positionZ, width);

	if (iNumTriangles > MAX_TRIANGLES)
	{
		for (int i = 0; i < 4; i++)
		{
			// 새로운 자식 노드에 대한 위치 오프셋을 계산
			float fOffsetX = (((i % 2) < 1) ? -1.f : 1.f) * (width / 4.f);
			float fOffsetZ = (((i % 4) < 2) ? -1.f : 1.f) * (width / 4.f);

			// 새 노드에 삼각형이 있는지 확인한다.
			int iCount = CountTriangles((positionX + fOffsetX), (positionZ + fOffsetZ), (width / 2.f));

			if (iCount > 0)
			{
				// 이 새 노드가 있는 삼각형이 있는 경우 자식 노드로 만든다.
				node->pNodes[i] = new QUADTREENODE;

				// 이제 새 자식 노드에서 시작하는 트리를 확장한다.
				this->CreateTreeNode(node->pNodes[i], (positionX + fOffsetX), (positionZ + fOffsetZ), (width / 2.f));
			}
		}
		return;
	}

	// Tag 설정
	string nodeName = "Node";
	char str[64];

	string appendName = itoa(number, str, 10);
	nodeName = nodeName + appendName;

	node->strNodeName = nodeName;
	_cprintf("Node%d\n", number);

	number++;

	node->iTriCount = iNumTriangles;
	_cprintf("TriCount : %d\n", node->iTriCount);

	_cprintf("CenterX : %f, CenterZ : %f, width : %f\n", node->fCenterX, node->fCenterZ, node->fWidth);

	// Max, Min, Index, Vtx설정
	int iIndex = 0;
	int iVertexIndex = 0;

	// Index 를 돌며 min max를 구한다.
	for (int i = 0; i < m_iTriCount; i += 3)
	{
		if (this->IsTriangleContaind_Index_Other(i, positionX, positionZ, width))
		{
			int iIndexCount = i;

			node->vecIndex.push_back(m_vecIndex[iIndexCount]);
			iIndexCount++;
			iIndex++;
			node->vecIndex.push_back(m_vecIndex[iIndexCount]);
			iIndexCount++;
			iIndex++;
			node->vecIndex.push_back(m_vecIndex[iIndexCount]);
			iIndex++;
		}
	}
	
	// min
	node->vMin = m_vecVtx[node->vecIndex[2]].vPos;
	node->vMin.y = -50.f;
	// max
	node->vMax = m_vecVtx[node->vecIndex[iIndex - 1 - 1]].vPos;
	node->vMax.y = 50.f;
	
	node->iSizeZ = (int)node->vMax.z - (int)node->vMin.z;
	node->iSizeX = (int)node->vMax.x - (int)node->vMin.x;

	// 정점 할당 
	node->vecVtx.resize((node->iSizeZ + 1) * (node->iSizeX + 1));

	for (int i = 0; i <= m_iNumZ; i++)
	{
		for (int j = 0; j <= m_iNumX; j++)
		{
			int iOriginIdx = i * (m_iNumX + 1) + j;

			if (this->IsTriangleContaind_Vertex(iOriginIdx, node->vMin, node->vMax))
			{
				//node->vecVtx.push_back(m_vecVtx[iVertexIndex]);
				node->vecVtx[iVertexIndex] = m_vecVtx[iOriginIdx];
				iVertexIndex++;
			}
		}
	}

	// vMax.x, vMax.z를 바탕으로 새로운 인덱스를 구한다.
	node->vecIndex.clear();

	for (int i = 0; i < node->iSizeZ; i++)
	{
		for (int j = 0; j < node->iSizeX; j++)
		{
			// 좌상단 정점의 인덱스를 구해준다.
			int	idx = (i + 1) * (node->iSizeX + 1) + j;

			// 좌상단 삼각형 인덱스
			node->vecIndex.push_back(idx);
			node->vecIndex.push_back(idx + 1);
			node->vecIndex.push_back(idx - (node->iSizeX + 1));

			// 우하단 삼각형 인덱스
			node->vecIndex.push_back(idx - (node->iSizeX + 1));
			node->vecIndex.push_back(idx + 1);
			node->vecIndex.push_back(idx - (node->iSizeX + 1) + 1);
		}
	}

	_cprintf("MIN --> X : %f, Z : %f\n", node->vMin.x, node->vMin.z);
	_cprintf("MAX --> X : %f, Z : %f\n", node->vMax.x, node->vMax.z);

	return;
}

int CLandScape::CountTriangles(float positionX, float positionZ, float width)
{
	int count = 0;

	// 모든 인덱스를 돌며
	for (int i = 0; i < m_iTriCount; i += 3)
	{
		// 삼각형이 이 노드 안에 있으면 1씩 증가시킨다.
		if (this->IsTriangleContaind_Index(i, positionX, positionZ, width))
		{
			count++;
		}
	}

	return count;
}

bool CLandScape::IsTriangleContaind_Index(int index, float positionX, float positionZ, float width)
{
	float fRadius = width / 2.f;

	int iIndex = index;

	float fX1 = m_vecVtx[m_vecIndex[iIndex]].vPos.x;
	float fZ1 = m_vecVtx[m_vecIndex[iIndex]].vPos.z;
	iIndex++;

	float fX2 = m_vecVtx[m_vecIndex[iIndex]].vPos.x;
	float fZ2 = m_vecVtx[m_vecIndex[iIndex]].vPos.z;
	iIndex++;

	float fX3 = m_vecVtx[m_vecIndex[iIndex]].vPos.x;
	float fZ3 = m_vecVtx[m_vecIndex[iIndex]].vPos.z;

	float fMinimumX = min(fX1, min(fX2, fX3));
	if (fMinimumX > (positionX + fRadius))
	{
		return false;
	}

	float fMaximumX = max(fX1, max(fX2, fX3));
	if (fMaximumX < (positionX - fRadius))
	{
		return false;
	}

	float fMinimumZ = min(fZ1, min(fZ2, fZ3));
	if (fMinimumZ > (positionZ + fRadius))
	{
		return false;
	}

	float fMaximumZ = max(fZ1, max(fZ2, fZ3));
	if (fMaximumZ < (positionZ - fRadius))
	{
		return false;
	}

	return true;
}

bool CLandScape::IsTriangleContaind_Vertex(int vtxIndex, Vector3 min, Vector3 max)
{
	float fX = m_vecVtx[vtxIndex].vPos.x;
	float fZ = m_vecVtx[vtxIndex].vPos.z;

	if (fX > max.x)
	{
		return false;
	}

	if (fX < min.x)
	{
		return false;
	}

	if (fZ > max.z)
	{
		
		return false;
	}

	if (fZ < min.z)
	{
		return false;
	}

	return true;
}

void CLandScape::CalculateCorrectly(float & centerX, float & centerZ, float & meshWidth)
{
	centerX = (float)m_iNumX / 2;
	centerZ = (float)m_iNumZ / 2;

	meshWidth = (float)m_iNumX;
}

void CLandScape::CreateTreeNodeCorrectly(QUADTREENODE * node, float positionX, float positionZ, float width)
{
	node->fCenterX = positionX;
	node->fCenterZ = positionZ;
	node->fWidth = width;

	node->vMin = Vector3(0.f, 0.f, 0.f);
	node->vMax = Vector3(0.f, 0.f, 0.f);

	node->iSizeX = 0;
	node->iSizeZ = 0;

	node->iTriCount = 0;
	node->pGameObject = nullptr;

	for (int i = 0; i < 4; i++)
	{
		node->pNodes[i] = nullptr;
	}

	bool bCheck = RangeCheck(positionX, positionZ, width);

	if (!bCheck)
	{
		for (int i = 0; i < 4; i++)
		{
			// 새로운 자식 노드에 대한 위치 오프셋을 계산
			int fOffsetX = (((i % 2) < 1) ? -1 : 1) * ((int)width / 4);
			int fOffsetZ = (((i % 4) < 2) ? -1 : 1) * ((int)width / 4);

			
			/*bool bCheck_Other = RangeCheck(positionX + fOffsetX, positionZ + fOffsetZ, (int)width / 2);

			if (bCheck_Other)
			{
				
			}*/

			// 이 새 노드가 있는 삼각형이 있는 경우 자식 노드로 만든다.
			node->pNodes[i] = new QUADTREENODE;

			// 이제 새 자식 노드에서 시작하는 트리를 확장한다.
			this->CreateTreeNodeCorrectly(node->pNodes[i], (positionX + fOffsetX), (positionZ + fOffsetZ), ((int)width / 2));
		}
		return;
	}

	// Tag 설정
	string nodeName = "Node";
	char str[64];

	string appendName = itoa(number, str, 10);
	nodeName = nodeName + appendName;

	node->strNodeName = nodeName;
	_cprintf("Node%d\n", number);

	number++;

	node->iTriCount = MAX_RANGE * MAX_RANGE * 2;
	_cprintf("TriCount : %d\n", node->iTriCount);

	_cprintf("CenterX : %f, CenterZ : %f, width : %f\n", node->fCenterX, node->fCenterZ, node->fWidth);

	// Max, Min, Index, Vtx설정
	int iIndex = 0;
	int iVertexIndex = 0;

	int ij = 0;

	// Index 를 돌며 min max를 구한다.
	for (int i = 0; i < m_iTriCount; i += 3)
	{
		if (this->IsTriangleContaind_Index_Other(i, positionX, positionZ, width))
		{
			int iIndexCount = i;

			node->vecIndex.push_back(m_vecIndex[iIndexCount]);
			iIndexCount++;
			iIndex++;
			node->vecIndex.push_back(m_vecIndex[iIndexCount]);
			iIndexCount++;
			iIndex++;
			node->vecIndex.push_back(m_vecIndex[iIndexCount]);
			iIndex++;

			++ij;
		}
	}

	// min
	node->vMin = m_vecVtx[node->vecIndex[2]].vPos;
	node->vMin.y = -100.f;
	// max
	node->vMax = m_vecVtx[node->vecIndex[iIndex - 1 - 1]].vPos;
	node->vMax.y = 100.f;

	node->iSizeZ = (int)node->vMax.z - (int)node->vMin.z;
	node->iSizeX = (int)node->vMax.x - (int)node->vMin.x;

	// 정점 할당 
	node->vecVtx.resize((node->iSizeZ + 1) * (node->iSizeX + 1));

	for (int i = 0; i <= m_iNumZ; i++)
	{
		for (int j = 0; j <= m_iNumX; j++)
		{
			int iOriginIdx = i * (m_iNumX + 1) + j;

			if (this->IsTriangleContaind_Vertex_Other(iOriginIdx, node->vMin, node->vMax))
			{
				//node->vecVtx.push_back(m_vecVtx[iVertexIndex]);
				node->vecVtx[iVertexIndex] = m_vecVtx[iOriginIdx];
				iVertexIndex++;
			}
		}
	}

	// vMax.x, vMax.z를 바탕으로 새로운 인덱스를 구한다.
	node->vecIndex.clear();

	for (int i = 0; i < node->iSizeZ; i++)
	{
		for (int j = 0; j < node->iSizeX; j++)
		{
			// 좌상단 정점의 인덱스를 구해준다.
			int	idx = (i + 1) * (node->iSizeX + 1) + j;

			// 좌상단 삼각형 인덱스
			node->vecIndex.push_back(idx);
			node->vecIndex.push_back(idx + 1);
			node->vecIndex.push_back(idx - (node->iSizeX + 1));

			// 우하단 삼각형 인덱스
			node->vecIndex.push_back(idx - (node->iSizeX + 1));
			node->vecIndex.push_back(idx + 1);
			node->vecIndex.push_back(idx - (node->iSizeX + 1) + 1);
		}
	}

	_cprintf("MIN --> X : %f, Z : %f\n", node->vMin.x, node->vMin.z);
	_cprintf("MAX --> X : %f, Z : %f\n", node->vMax.x, node->vMax.z);

	return;
}

bool CLandScape::RangeCheck(float positionX, float positionZ, float width)
{
	if ((int)width > MAX_RANGE)
		return false;
	
	return true;
}

bool CLandScape::IsTriangleContaind_Index_Other(int index, float positionX, float positionZ, float width)
{
	int iRadius = (int)width / 2.f;

	int iIndex = index;

	int fX1 = (int)m_vecVtx[m_vecIndex[iIndex]].vPos.x;
	int fZ1 = (int)m_vecVtx[m_vecIndex[iIndex]].vPos.z;
	iIndex++;

	int fX2 = (int)m_vecVtx[m_vecIndex[iIndex]].vPos.x;
	int fZ2 = (int)m_vecVtx[m_vecIndex[iIndex]].vPos.z;
	iIndex++;

	int fX3 = (int)m_vecVtx[m_vecIndex[iIndex]].vPos.x;
	int fZ3 = (int)m_vecVtx[m_vecIndex[iIndex]].vPos.z;

	int fMinimumX = min(fX1, min(fX2, fX3));
	if (fMinimumX >= (positionX + iRadius))
	{
		return false;
	}

	int fMaximumX = max(fX1, max(fX2, fX3));
	if (fMaximumX <= (positionX - iRadius))
	{
		return false;
	}

	int fMinimumZ = min(fZ1, min(fZ2, fZ3));
	if (fMinimumZ >= (positionZ + iRadius))
	{
		return false;
	}

	int fMaximumZ = max(fZ1, max(fZ2, fZ3));
	if (fMaximumZ <= (positionZ - iRadius))
	{
		return false;
	}

	return true;
}

bool CLandScape::IsTriangleContaind_Vertex_Other(int vtxIndex, Vector3 min, Vector3 max)
{
	int fX = (int)m_vecVtx[vtxIndex].vPos.x;
	int fZ = (int)m_vecVtx[vtxIndex].vPos.z;

	if (fX > max.x)
	{
		return false;
	}

	if (fX < min.x)
	{
		return false;
	}

	if (fZ > max.z)
	{

		return false;
	}

	if (fZ < min.z)
	{
		return false;
	}

	return true;
}

list<QUADTREENODE*>* CLandScape::FindNode_ByMouse()
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	m_listNode.clear();

	NodeRayCollisionCheck();

	return &m_listNode;
}

list<QUADTREENODE*>* CLandScape::FindNode_ByRadius(float radius)
{
	m_listNode.clear();

	NodeRadiusCollisionCheck(radius);

	return &m_listNode;
}

void CLandScape::NodesToContainer()
{
	m_listAllNodes.clear();

	NodeAll(m_pParentNode);
}

void CLandScape::Load_Terrain(string fileName)
{
	string flexiblePath = GET_SINGLE(CPathManager)->FindPathToMultiByte(DATA_PATH);
	string filePath = "Terrain\\" + fileName + ".bin";

	ifstream mainFile;
	mainFile.open(flexiblePath + filePath, ios::in);

	if (!mainFile.is_open())
		return;

	// 노드 정보 불러오기
	string heightFileName;
	mainFile >> heightFileName;
	Load_QuadTree(flexiblePath + heightFileName);

	// 텍스쳐 정보 불러오기
	string textureFileName;
	mainFile >> textureFileName;
	Load_TextureName(flexiblePath + textureFileName);

	int iCount;

	mainFile >> iCount;

	for (int i = 0; i < iCount; i++)
	{
		string bmpFileName;
		wstring wbmpFileName;
		int iDetailLevel;

		mainFile >> bmpFileName;
		wbmpFileName.assign(bmpFileName.begin(), bmpFileName.end());

		m_vecSplattingAlpha[i] = wbmpFileName;

		mainFile >> iDetailLevel;

		SetDetailLevel_Splat(i, iDetailLevel);
	}

	int iDetailLeval_Default;
	mainFile >> iDetailLeval_Default;
	SetDetailLevel(iDetailLeval_Default);

	int iSizeX, iSizeZ;
	mainFile >> iSizeX >> iSizeZ;
	SetTerrainSize(iSizeX, iSizeZ);

	SetMaterial_DNS_Default(m_diffuseName.c_str(), m_normalName.c_str(), m_specularName.c_str());

	SetMaterial_Splatting(m_vecSplattingDiffuse,
		m_vecSplattingNormal,
		m_vecSplattingSpecular,
		m_vecSplattingAlpha, DATA_PATH);

	SetSplatCount(iCount);

	mainFile.close();

}

void CLandScape::Save_QuadTree(string fileName)
{
	// 파일을 오픈
	ofstream file;
	file.open(fileName + ".dat", ios::out | ios::trunc /*| ios::binary*/);

	// open 되었는지 검사
	if (!file.is_open())
		return;

	//
	for (const auto node : m_listAllNodes)
	{
		file << node->strNodeName << endl;
		file << node->iTriCount << endl;
		file << node->fCenterX << ' ' << node->fCenterZ << ' ' << node->fWidth << endl;
		file << node->vMin.x << ' ' << node->vMin.y << ' ' << node->vMin.z << endl;
		file << node->vMax.x << ' ' << node->vMax.y << ' ' << node->vMax.z << endl;
		file << node->iSizeX << ' ' << node->iSizeZ << endl;

		file << node->vecVtx.size() << endl;

		for (const auto iter : node->vecVtx)
		{
			// pos
			file << iter.vPos.x << ' ' << iter.vPos.y << ' ' << iter.vPos.z << endl;
			// UV
			file << iter.vUV.x << ' ' << iter.vUV.y << endl;
			// normal
			file << iter.vNormal.x << ' ' << iter.vNormal.y << ' ' << iter.vNormal.z << endl;
			// binormal
			file << iter.vBinormal.x << ' ' << iter.vBinormal.y << ' ' << iter.vBinormal.z << endl;
			// tangent
			file << iter.vTangent.x << ' ' << iter.vTangent.y << ' ' << iter.vTangent.z << endl;
		}

		file << node->vecIndex.size() << endl;

		for (const auto iter : node->vecIndex)
		{
			file << iter << endl;
		}
	}

	file.close();
}

void CLandScape::Load_QuadTree(string fileName)
{
	m_listAllNodes.clear();

	ifstream file;
	file.open(fileName + ".dat", ios::in);

	if (!file.is_open())
		return;


	while (!file.eof())
	{	
		QUADTREENODE* node = new QUADTREENODE;

		file >> node->strNodeName;

		// 임시 방편
		if (node->strNodeName == "")
			break;

		file >> node->iTriCount;
		file >> node->fCenterX >> node->fCenterZ >> node->fWidth;
		file >> node->vMin.x >> node->vMin.y >> node->vMin.z;
		file >> node->vMax.x >> node->vMax.y >> node->vMax.z;
		file >> node->iSizeX >> node->iSizeZ;

		int iVtxSize, iIdxSize = 0;
		file >> iVtxSize;

		for (int i = 0; i < iVtxSize; i++)
		{
			VERTEXBUMP tempVtx;
			// pos
			file >> tempVtx.vPos.x >> tempVtx.vPos.y >> tempVtx.vPos.z;
			// UV
			file >> tempVtx.vUV.x >> tempVtx.vUV.y;
			// normal
			file >> tempVtx.vNormal.x >> tempVtx.vNormal.y >> tempVtx.vNormal.z;
			// binormal
			file >> tempVtx.vBinormal.x >> tempVtx.vBinormal.y >> tempVtx.vBinormal.z;
			// tangent
			file >> tempVtx.vTangent.x >> tempVtx.vTangent.y >> tempVtx.vTangent.z;

			node->vecVtx.push_back(tempVtx);
		}

		file >> iIdxSize;

		for (int i = 0; i < iIdxSize; i++)
		{
			int iIdx;
			file >> iIdx;
			node->vecIndex.push_back(iIdx);
		}

		m_listAllNodes.push_back(node);
	}

	file.close();

	CreateTreeNodeToObject();
}

void CLandScape::Load_TextureName(string fileName)
{
	ifstream file;
	file.open(fileName + ".dat", ios::in);

	string diffuseName, normalName, specularName;

	file >> diffuseName;
	file >> normalName;
	file >> specularName;

	m_diffuseName.assign(diffuseName.begin(), diffuseName.end());
	m_normalName.assign(normalName.begin(), normalName.end());
	m_specularName.assign(specularName.begin(), specularName.end());

	int iCount = 0;

	file >> iCount;

	for (int i = 0; i < iCount; i++)
	{
		string diffuse, normal, specular;
		wstring wDiffuse, wNormal, wSpecular;

		file >> diffuse;
		file >> normal;
		file >> specular;

		wDiffuse.assign(diffuse.begin(), diffuse.end());
		wNormal.assign(normal.begin(), normal.end());
		wSpecular.assign(specular.begin(), specular.end());

		m_vecSplattingDiffuse[i] = wDiffuse;
		m_vecSplattingNormal[i] = wNormal;
		m_vecSplattingSpecular[i] = wSpecular;
	}

	file.close();
}

void CLandScape::UpdateNode()
{
	for (auto& node : m_listAllNodes)
	{
		if (node->pGameObject->GetCulling() == true)
		{
			continue;
		}

		CRenderer*	pRenderer = node->pGameObject->FindComponentFromType<CRenderer>(CT_RENDERER);

		if (pRenderer)
		{
			LANDSCAPECBUFFER	tBuffer = {};
			tBuffer.iDetailLevel = m_iDetailLevel;
			tBuffer.iSplatCount = m_iSplatCount;
			for (int i = 0; i < 4; i++)
			{
				tBuffer.arrDetailLevelTex[i] = m_arrDetailLevel_Tex[i];
			}

			// brush
			if (m_bCheckBrush)
			{
				tBuffer.fEmpty1 = 1.f;
				tBuffer.fRangeBrush = m_fRangeBrush;
				tBuffer.vPosBrush = m_vPosBrush;
				tBuffer.vColorBrush = m_vColorBrush;
			}
			else
			{
				tBuffer.fEmpty1 = 0.f;
			}

			// brush
			if (m_bCheckBrush_Other)
			{
				tBuffer.fEmpty2 = 1.f;
				tBuffer.vPosBrush_Other = m_vPosBrush_Other;
			}
			else
			{
				tBuffer.fEmpty2 = 0.f;
			}

			pRenderer->UpdateCBuffer("LandScape", 12, sizeof(LANDSCAPECBUFFER),
				SCT_PIXEL, &tBuffer);
			SAFE_RELEASE(pRenderer);
		}
	}
}

void CLandScape::NodeRayCollisionCheck()
{
	for (auto& node : m_listAllNodes)
	{
		if (node->pGameObject->GetCulling() == true)
		{
			continue;
		}

		CGameObject* pMouseObj = GET_SINGLE(CInput)->GetMouseObj();
		CMouse* pMouse = pMouseObj->FindComponentFromTag<CMouse>("Mouse");
		CColliderRay* pRay = pMouse->FindComponentFromTag<CColliderRay>("MouseRay");
		CColliderAABB* pAABB = node->pGameObject->FindComponentFromTag<CColliderAABB>("Collider");

		if (pRay->Collision(pAABB))
		{
			m_listNode.push_back(node);
			//_cprintf("Collide!\n");
		}

		SAFE_RELEASE(pRay);
		SAFE_RELEASE(pAABB);
		SAFE_RELEASE(pMouse);
		SAFE_RELEASE(pMouseObj);
	}

	return;
}

void CLandScape::NodeRadiusCollisionCheck(float radius)
{
	for (auto& node : m_listAllNodes)
	{
		if (node->pGameObject->GetCulling() == true)
		{
			continue;
		}

		for (const auto iter : node->vecVtx)
		{
			float fDistance = iter.vPos.Distance(m_vPosBrush);

			if (fDistance < radius)
			{
				m_listNode.push_back(node);
				break;
			}
		}
	}

	return;
}

void CLandScape::NodeSphereCollisionCheck(CGameObject * src)
{
	for (auto& node : m_listAllNodes)
	{
		if (node->pGameObject->GetCulling() == true)
		{
			continue;
		}

		CColliderSphere* pSphere = src->FindComponentFromTag<CColliderSphere>("Collider");
		CColliderAABB* pAABB = node->pGameObject->FindComponentFromTag<CColliderAABB>("Collider");

		if (pSphere->Collision(pAABB))
		{
			m_listNode.push_back(node);
			//_cprintf("Collide!\n");
		}

		SAFE_RELEASE(pSphere);
		SAFE_RELEASE(pAABB);
	}
	return;
}

void CLandScape::NodeAll(QUADTREENODE * node)
{
	int count = 0;

	for (int i = 0; i < 4; i++)
	{
		if (node->pNodes[i] != nullptr)
		{
			count++;
			NodeAll(node->pNodes[i]);
		}
	}

	if (count != 0)
	{
		return;
	}

	m_listAllNodes.push_back(node);

	return;
}

void CLandScape::ReleaseNode(QUADTREENODE* node)
{
	for (int i = 0; i < 4; i++)
	{
		if (node->pNodes[i])
		{
			ReleaseNode(node->pNodes[i]);
		}
	}

	if (node->pGameObject)
	{
		SAFE_RELEASE(node->pGameObject);
	}

	for (int i = 0; i < 4; i++)
	{
		if (node->pNodes[i])
		{
			delete node->pNodes[i];
			node->pNodes[i] = nullptr;
		}
	}
}