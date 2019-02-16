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

PG_USING

int CLandScape::g_iQuadName = 0;

CLandScape::CLandScape()
{
	SetTag("LandScape");
	SetTypeName("CLandScape");
	SetTypeID<CLandScape>();
	m_eType = CT_LANDSCAPE;
	m_iDetailLevel = 37;
	m_iSplatCount = 0;
}


CLandScape::CLandScape(const CLandScape & landscape)	:
	CComponent(landscape)
{
	m_iDetailLevel = landscape.m_iDetailLevel;
	m_iSplatCount = landscape.m_iSplatCount;
}

CLandScape::~CLandScape()
{
}

void CLandScape::SetDetailLevel(int iDetailLevel)
{
	m_iDetailLevel = iDetailLevel;
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

bool CLandScape::CreateLandScapeQuadTree(const string & strMeshKey, int iSizeX, int iSizeZ, bool bBump, const string & strTexKey, const wchar_t * pFileName, const wchar_t * pNormalName, const wchar_t * pSpecularName, const char * pHeightMap, const string & strPathKey)
{
	m_iNumX = iSizeX;
	m_iNumZ = iSizeZ;

	m_vecVtx.resize((m_iNumX) * (m_iNumZ) * 2 * 3);
	m_vecIndex.resize((m_iNumX) * (m_iNumZ) * 2 * 3);
	int index = 0;

	for (int i = 0; i < m_iNumZ; ++i)
	{
		for (int j = 0; j < m_iNumX; ++j)
		{
			int left    = j;									    // 왼쪽
 			int right   = (j + 1);									// 오른쪽
			int top     = (i + 1);					// 위
			int bottom  = i;						// 아래

			// 왼쪽 위.

			m_vecVtx[index].vPos = XMFLOAT3(left, 0.f, top);
			m_vecVtx[index].vNormal = Vector3(0.f, 1.f, 0.f);
			m_vecVtx[index].vUV = Vector2((float)left / (float)(m_iNumX),
							(float)top / (float)(m_iNumZ));
			m_vecIndex[index] = index;
			index++;

			// 오른쪽 위.
			
			m_vecVtx[index].vPos = XMFLOAT3(right, 0.f, top);
			m_vecVtx[index].vNormal = Vector3(0.f, 1.f, 0.f);
			m_vecVtx[index].vUV = Vector2((float)right / (float)(m_iNumX),
				(float)top / (float)(m_iNumZ));
			m_vecIndex[index] = index;
			index++;

			// 왼쪽 아래.
			
			m_vecVtx[index].vPos = XMFLOAT3(left, 0.f, bottom);
			m_vecVtx[index].vNormal = Vector3(0.f, 1.f, 0.f);
			m_vecVtx[index].vUV = Vector2((float)left / (float)(m_iNumX),
				(float)bottom / (float)(m_iNumZ));
			m_vecIndex[index] = index;
			index++;

			// 왼쪽 아래.
			
			m_vecVtx[index].vPos = XMFLOAT3(left, 0.f, bottom);
			m_vecVtx[index].vNormal = Vector3(0.f, 1.f, 0.f);
			m_vecVtx[index].vUV = Vector2((float)left / (float)(m_iNumX),
				(float)bottom / (float)(m_iNumZ));
			m_vecIndex[index] = index;
			index++;

			// 오른쪽 위.
			
			m_vecVtx[index].vPos = XMFLOAT3(right, 0.f, top);
			m_vecVtx[index].vNormal = Vector3(0.f, 1.f, 0.f);
			m_vecVtx[index].vUV = Vector2((float)right / (float)(m_iNumX),
				(float)top / (float)(m_iNumZ));
			m_vecIndex[index] = index;
			index++;

			// 오른쪽 아래.
			
			m_vecVtx[index].vPos = XMFLOAT3(right, 0.f, bottom);
			m_vecVtx[index].vNormal = Vector3(0.f, 1.f, 0.f);
			m_vecVtx[index].vUV = Vector2((float)right / (float)(m_iNumX),
				(float)bottom / (float)(m_iNumZ));
			m_vecIndex[index] = index;
			index++;
		}
	}

	for (int i = 0; i < m_iNumZ; ++i)
	{
		for (int j = 0; j < m_iNumX; ++j)
		{
			//좌하단
			int	idx = i * (m_iNumX + 1) + j;

			//우하단
			Vector3	 vEdge1 = m_vecVtx[idx + (m_iNumX + 1) + 1].vPos - m_vecVtx[idx].vPos;
			Vector3	 vEdge2 = m_vecVtx[idx + 1].vPos - m_vecVtx[idx].vPos;

			vEdge1 = vEdge1.Normalize();
			vEdge2 = vEdge2.Normalize();

			Vector3	vFaceNormal = vEdge1.Cross(vEdge2);

			m_vecFaceNormal.push_back(vFaceNormal.Normalize());

			//좌상단
			vEdge1 = m_vecVtx[idx + (m_iNumX + 1) + 1].vPos - m_vecVtx[idx].vPos;
			vEdge2 = m_vecVtx[idx + (m_iNumX + 1)].vPos - m_vecVtx[idx].vPos;

		

			vEdge1 = vEdge1.Normalize();
			vEdge2 = vEdge2.Normalize();

			vFaceNormal = vEdge1.Cross(vEdge2);

			m_vecFaceNormal.push_back(vFaceNormal.Normalize());
		}
	}

	// before
	/*for (int i = 0; i < m_iNumZ; ++i)
	{
		for (int j = 0; j < m_iNumX; ++j)
		{
			// 좌상단 정점의 인덱스를 구해준다.
			int	idx = i * (m_iNumX + 1) + j;

			// 우상단 삼각형 인덱스
			m_vecIndex[iCount++] = idx;
			m_vecIndex[iCount++] = idx + 1;
			m_vecIndex[iCount++] = idx + (m_iNumX + 1) + 1;

			// 좌하단 삼각형 인덱스
			m_vecIndex[iCount++] = idx;
			m_vecIndex[iCount++] = idx + (m_iNumX + 1) + 1;
			m_vecIndex[iCount++] = idx + (m_iNumX + 1);

			// 삼각형의 면법선을 구하자
			// 우상단
			Vector3	vEdge1 = m_vecPos[idx + 1] - m_vecPos[idx];
			Vector3	vEdge2 = m_vecPos[idx + (m_iNumX + 1) + 1] - m_vecPos[idx];

			vEdge1 = vEdge1.Normalize();
			vEdge2 = vEdge2.Normalize();

			Vector3	vFaceNormal = vEdge1.Cross(vEdge2);

			m_vecFaceNormal.push_back(vFaceNormal.Normalize());

			// 좌하단
			vEdge1 = m_vecPos[idx + (m_iNumX + 1) + 1] - m_vecPos[idx];
			vEdge2 = m_vecPos[idx + (m_iNumX + 1)] - m_vecPos[idx];

			vEdge1 = vEdge1.Normalize();
			vEdge2 = vEdge2.Normalize();

			vFaceNormal = vEdge1.Cross(vEdge2);

			m_vecFaceNormal.push_back(vFaceNormal.Normalize());

		}
	}*/

	/*ComputeNormal(m_vecVtx, m_vecIndex);
	ComputeTangent(m_vecVtx, m_vecIndex);

	CRenderer*	pRenderer = m_pGameObject->AddComponent<CRenderer>("LandScape");
	CMaterial*	pMaterial = pRenderer->CreateMaterial();

	// 추후에 고쳐야함
	pMaterial->SetDiffuseTexInfo("Linear", "LandScape", 0, 0, L"LandScape/GRASS_00+SAND.dds", TEXTURE_PATH);
	pMaterial->SetNormalTexInfo("Linear", "LandScape_N", 1, 1,
		L"LandScape/GRASS_00+SAND_NRM.png", TEXTURE_PATH);
	pMaterial->SetSpecularTexInfo("Linear", "LandScape_S", 2, 2,
		L"LandScape/GRASS_00+SAND_SPEC.png", TEXTURE_PATH);

	SAFE_RELEASE(pMaterial);

	// Mesh 설정
	CMesh*	pMesh = GET_SINGLE(CResourcesManager)->CreateMesh(
		"LandScape",
		m_vecVtx.size(), sizeof(VERTEXBUMP), D3D11_USAGE_DEFAULT,
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, &m_vecVtx[0],
		m_vecIndex.size(), 4, D3D11_USAGE_DEFAULT,
		DXGI_FORMAT_R32_UINT, &m_vecIndex[0]);

	pMesh->SetShaderKey(LANDSCAPE_SHADER);
	pMesh->SetInputLayoutKey("Bump");
	pRenderer->SetMesh(pMesh);
	pRenderer->SetRenderState(CULLING_NONE);

	SAFE_RELEASE(pMesh);
	SAFE_RELEASE(pRenderer);*/

	for (size_t i = 0; i < m_vecFaceNormal.size(); ++i)
	{
		int	idx0 = m_vecIndex[i * 3];
		int	idx1 = m_vecIndex[i * 3 + 1];
		int	idx2 = m_vecIndex[i * 3 + 2];
	
		m_vecVtx[idx0].vNormal += m_vecFaceNormal[i];
		m_vecVtx[idx1].vNormal += m_vecFaceNormal[i];
		m_vecVtx[idx2].vNormal += m_vecFaceNormal[i];
	}
	
	for (size_t i = 0; i < m_vecVtx.size(); ++i)
	{
		m_vecVtx[i].vNormal = m_vecVtx[i].vNormal.Normalize();
	}
	
	// 탄젠트 벡터 구함.
	for (size_t i = 0; i < m_vecFaceNormal.size(); ++i)
	{
		int	idx0 = m_vecIndex[i * 3];
		int	idx1 = m_vecIndex[i * 3 + 1];
		int	idx2 = m_vecIndex[i * 3 + 2];
	
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
	
	CreateQuadTree();

	// 내비게이션 관리자에 지형을 등록한다.
	/*GET_SINGLE(CNavigationManager)->AddLandScapeInfo(strMeshKey,
		m_iNumX, m_iNumZ, m_pScene, m_pTransform, &m_vecVtx[]);*/

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

bool CLandScape::Init()
{
	m_pDebugTree = new DebugTree;
	m_pDebugTree->Initialize();

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
	CRenderer*	pRenderer = FindComponentFromType<CRenderer>(CT_RENDERER);

	if (pRenderer)
	{
		LANDSCAPECBUFFER	tBuffer = {};
		tBuffer.iDetailLevel = m_iDetailLevel;
		tBuffer.iSplatCount = m_iSplatCount;
		pRenderer->UpdateCBuffer("LandScape", 12, sizeof(LANDSCAPECBUFFER),
			SCT_PIXEL, &tBuffer);
		SAFE_RELEASE(pRenderer);
	}


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

	m_iTriCount = vertexCount / 3;

	CalculateMeshDimensions(vertexCount, fCenterX, fCenterZ, fWidth);

	m_pParentNode = new NodeType;
	if (!m_pParentNode)
	{
		return false;
	}

	CreateTreeNode(m_pParentNode, fCenterX, fCenterZ, fWidth);
	CreateQuadMesh(m_pParentNode);

	return true;
}

void CLandScape::CreateQuadMesh(NodeType * node)
{
	int count = 0;
	for (int i = 0; i < 4; i++)
	{
		if (node->pNodes[i] != 0)
		{
			count++;
			CreateQuadMesh(node->pNodes[i]);
		}
	}

	if (count != 0)
	{
		return;
	}

	string rendererName = "Renderer";
	static int number = 0;
	char str[64];

	string appendName = itoa(number, str, 10);
	rendererName = rendererName + appendName;
	_cprintf(rendererName.c_str());
	number++;

	//if (rendererName == "Renderer0")
	{
		CRenderer*	pRenderer = m_pGameObject->AddComponent<CRenderer>(rendererName);
		CMaterial*	pMaterial = pRenderer->CreateMaterial();

		// 추후에 고쳐야함
		pMaterial->SetDiffuseTexInfo("Linear", "LandScape", 0, 0, L"LandScape/GRASS_00+SAND.dds", TEXTURE_PATH);
		pMaterial->SetNormalTexInfo("Linear", "LandScape_N", 1, 1,
			L"LandScape/GRASS_00+SAND_NRM.png", TEXTURE_PATH);
		pMaterial->SetSpecularTexInfo("Linear", "LandScape_S", 2, 2,
			L"LandScape/GRASS_00+SAND_SPEC.png", TEXTURE_PATH);

		SAFE_RELEASE(pMaterial);

		// for debug
		/*vector<VERTEXCOLOR> vecVtx;
		vector<UINT> vecIndex;

		vecVtx.resize(node->vecVtx.size());
		vecIndex.resize(node->vecIndex.size());

		int iCount = 0;

		for (auto iter : node->vecVtx)
		{
			vecVtx[iCount].vPos = iter.vPos;
			vecVtx[iCount].vColor = Vector4(1.f, 1.f, 1.f, 1.f);
			iCount++;
		}

		iCount = 0;

		for (auto iter : node->vecIndex)
		{
			vecIndex[iCount] = iter;
			iCount++;
		}

		CMesh*	pMesh = GET_SINGLE(CResourcesManager)->CreateMesh(
			node->pMeshInfo, to_string(g_iQuadName),
			vecVtx.size(), sizeof(VERTEXCOLOR), D3D11_USAGE_DEFAULT,
			D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, &vecVtx[0],
			vecIndex.size(), 4, D3D11_USAGE_DEFAULT,
			DXGI_FORMAT_R32_UINT, &vecIndex[0]);

		pMesh->SetShaderKey(STANDARD_COLOR_SHADER);
		pRenderer->SetMesh(pMesh);
		pRenderer->SetRenderState(WIRE_FRAME);*/

		// Mesh 설정
		CMesh*	pMesh = GET_SINGLE(CResourcesManager)->CreateMesh(
			node->pMeshInfo, rendererName,
			node->vecVtx.size(), sizeof(VERTEXBUMP), D3D11_USAGE_DEFAULT,
			D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, &node->vecVtx[0],
			node->vecIndex.size(), 4, D3D11_USAGE_DEFAULT,
			DXGI_FORMAT_R32_UINT, &node->vecIndex[0]);

		pMesh->SetShaderKey(LANDSCAPE_SHADER);
		pMesh->SetInputLayoutKey("Bump");
		pRenderer->SetMesh(pMesh);
		pRenderer->SetRenderState(CULLING_NONE);

		SAFE_RELEASE(pMesh);
		SAFE_RELEASE(pRenderer);

		m_pDebugTree->MakeMesh(node->fCenterX, node->fCenterZ, node->fWidth / 2.f);

		//g_iQuadName++;
		//cprintf("%d\n", g_iQuadName);
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

void CLandScape::CreateTreeNode(NodeType* node, float positionX, float positionZ, float width)
{
	node->fCenterX = positionX;
	node->fCenterZ = positionZ;
	node->fWidth = width;

	node->iTriCount = 0;

	node->pMeshInfo = nullptr;

	node->pNodes[0] = nullptr;
	node->pNodes[1] = nullptr;
	node->pNodes[2] = nullptr;
	node->pNodes[3] = nullptr;

	int iNumTriangles = CountTriangles(positionX, positionZ, width);

	if (iNumTriangles == 0)
	{
		return;
	}

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
				node->pNodes[i] = new NodeType;

				// 이제 새 자식 노드에서 시작하는 트리를 확장한다.
				this->CreateTreeNode(node->pNodes[i], (positionX + fOffsetX), (positionZ + fOffsetZ), (width / 2.f));
			}
		}
		return;
	}

	node->iTriCount = iNumTriangles;

	int iVertexCount = iNumTriangles * 3;
	
	vector<VERTEXBUMP> vecVtx;
	vector<UINT> vecIndex;

	vecVtx.resize(iVertexCount);
	vecIndex.resize(iVertexCount);

	int iIndex = 0;
	int iVertexIndex = 0;

	for (int i = 0; i < m_iTriCount; i++)
	{
		// 삼각형이 이 노드 안에 있으면 꼭지점 배열에 추가한다.
		if (this->IsTriangleContaind(i, positionX, positionZ, width))
		{
			// 지형 버텍스 목록에 인덱스를 계산한다.
			iVertexIndex = i * 3;

			// 정점 목록에서 이 삼각형의 세 꼭지점을 가져온다.
			vecVtx[iIndex] = m_vecVtx[iVertexIndex];
			vecIndex[iIndex] = iIndex;
			iIndex++;

			iVertexIndex++;
			vecVtx[iIndex] = m_vecVtx[iVertexIndex];
			vecIndex[iIndex] = iIndex;
			iIndex++;

			iVertexIndex++;
			vecVtx[iIndex] = m_vecVtx[iVertexIndex];
			vecIndex[iIndex] = iIndex;
			iIndex++;
		}
		else
		{
			int a = 10;
		}
	}

	node->vecVtx = vecVtx;
	node->vecIndex = vecIndex;
}

int CLandScape::CountTriangles(float positionX, float positionZ, float width)
{
	int count = 0;

	for (int i = 0; i < m_iTriCount; i++)
	{
		// 삼각형이 이 노드 안에 있으면 1씩 증가시킨다.
		if (IsTriangleContaind(i, positionX, positionZ, width))
		{
			count++;
		}
	}

	return count;
}

bool CLandScape::IsTriangleContaind(int index, float positionX, float positionZ, float width)
{
	float fRadius = width / 2.f;

	int iVertexIndex = index * 3;

	float fX1 = m_vecVtx[iVertexIndex].vPos.x;
	float fZ1 = m_vecVtx[iVertexIndex].vPos.z;
	iVertexIndex++;

	float fX2 = m_vecVtx[iVertexIndex].vPos.x;
	float fZ2 = m_vecVtx[iVertexIndex].vPos.z;
	iVertexIndex++;

	float fX3 = m_vecVtx[iVertexIndex].vPos.x;
	float fZ3 = m_vecVtx[iVertexIndex].vPos.z;

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

void CLandScape::ReleaseNode(NodeType* node)
{
	for (int i = 0; i < 4; i++)
	{
		if (node->pNodes[i])
		{
			ReleaseNode(node->pNodes[i]);
		}
	}

	if (node->pMeshInfo)
	{
		node->pMeshInfo = nullptr;
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

void CLandScape::RenderDebug(NodeType* node)
{
}
