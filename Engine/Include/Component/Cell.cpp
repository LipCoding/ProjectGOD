#include "Cell.h"

#include "../Resources/Mesh.h"
#include "../Resources/ResourcesManager.h"
#include "../Rendering/Shader.h"
#include "../Rendering/ShaderManager.h"
#include "Transform.h"
#include "../GameObject/GameObject.h"
#include "../Core/TimerManager.h"
#include "../Core/Timer.h"
#include "../Rendering/RenderManager.h"
#include "../Rendering/RenderState.h"
#include "../Scene/SceneManager.h"
#include "../Scene/Scene.h"
#include "../Scene/Layer.h"
#include "../Component/Camera.h"
#include "../Component/Renderer.h"
#include "../Device.h"

PG_USING

CCell::CCell()
{
	m_pShader = nullptr;
	ZeroMemory(m_vPoints, sizeof(Vector3) * 3);
	m_tVB.pBuffer = nullptr;
	m_tIB.pBuffer = nullptr;
	m_tVB.pVtxData = nullptr;
	m_tIB.pIdxData = nullptr;
}

CCell::CCell(const CCell & axisLine)
{
}

CCell::~CCell()
{
	if(m_tVB.pBuffer)
		SAFE_RELEASE(m_tVB.pBuffer);
	if (m_tVB.pVtxData)
		SAFE_DELETE_ARRAY(m_tVB.pVtxData);
	if (m_tIB.pBuffer)
		SAFE_RELEASE(m_tIB.pBuffer);
	if (m_tIB.pIdxData)
		SAFE_DELETE_ARRAY(m_tIB.pIdxData);
	SAFE_RELEASE(m_pShader);
}

bool CCell::Check_Position(const Vector3 & vPos, Vector3 * vDir, int * iIdx, int * iOutCount)
{
	for (size_t i = 0; i < DIR_END; ++i)
	{
		Vector3 vP0 = vPos;
		vP0.y = 0.f;

		Vector3 vP1 = m_vPoints[i];
		vP1.y = 0.f;

		Vector3 v = (vP0 - vP1).Normalize();

		float fCos = v.Dot(m_vNormal[i]);


		if (fCos > 0.f)
		{
			if (nullptr == m_pNeighbor[i])
			{
				*vDir = -m_vNormal[i];

				return false;
			}
			else
			{
				++*iOutCount;

				if (*iOutCount >= 25)
					return false;

				bool isIn = true;
				int iCount = *iOutCount;

				isIn = m_pNeighbor[i]->Check_Position(vPos, vDir, iIdx, iOutCount);

				if (true == isIn && iCount == *iOutCount)
					*iIdx = m_pNeighbor[i]->Get_Index();

				return isIn;
			}
		}
	}

	return true;
}

const bool CCell::Check_Inclusion(const Vector3 & vPos)
{
	for (UINT i = 0; i < 3; i++)
	{
		Vector3 vTemp = (vPos - m_vPoints[i]).Normalize();

		float fDot = vTemp.Dot(m_vNormal[i]);

		if (fDot > 0.f)
			return false;
	}

	return true;
}

void CCell::Compute_Neighbor(CCell * pTargetCell)
{
	ULONG dwTargetPoint = 0;
	bool bIsNeighbor = false;

	if (m_pNeighbor[NEIGHBOR_AB] == nullptr)
	{
		for (int i = POINT_A; i < POINT_END; ++i)
		{
			if (m_vPoints[POINT_A] == pTargetCell->Get_Point(i))
			{
				dwTargetPoint |= 1 << i;
				bIsNeighbor = true;
				continue;
			}
			if (m_vPoints[POINT_B] == pTargetCell->Get_Point(i) && bIsNeighbor)
			{
				m_pNeighbor[NEIGHBOR_AB] = pTargetCell;

				if (dwTargetPoint & DW_A && POINT_B == i)
					pTargetCell->m_pNeighbor[NEIGHBOR_AB] = this;
				else if (dwTargetPoint & DW_A && POINT_C == i)
					pTargetCell->m_pNeighbor[NEIGHBOR_CA] = this;
				else if (dwTargetPoint & DW_B)
					pTargetCell->m_pNeighbor[NEIGHBOR_BC] = this;
			}
		}
	}

	if (m_pNeighbor[NEIGHBOR_BC] == nullptr)
	{
		for (int i = POINT_A; i < POINT_END; ++i)
		{
			if (m_vPoints[POINT_B] == pTargetCell->Get_Point(i))
			{
				dwTargetPoint |= 1 << i;
				bIsNeighbor = true;
				continue;
			}
			if (m_vPoints[POINT_C] == pTargetCell->Get_Point(i) && bIsNeighbor)
			{
				m_pNeighbor[NEIGHBOR_BC] = pTargetCell;

				if (dwTargetPoint & DW_A && POINT_B == i)
					pTargetCell->m_pNeighbor[NEIGHBOR_AB] = this;
				else if (dwTargetPoint & DW_A && POINT_C == i)
					pTargetCell->m_pNeighbor[NEIGHBOR_CA] = this;
				else if (dwTargetPoint & DW_B)
					pTargetCell->m_pNeighbor[NEIGHBOR_BC] = this;
			}
		}
	}

	if (m_pNeighbor[NEIGHBOR_CA] == nullptr)
	{
		for (int i = POINT_A; i < POINT_END; ++i)
		{
			if (m_vPoints[POINT_C] == pTargetCell->Get_Point(i))
			{
				dwTargetPoint |= 1 << i;
				bIsNeighbor = true;
				continue;
			}
			if (m_vPoints[POINT_A] == pTargetCell->Get_Point(i) && bIsNeighbor)
			{
				m_pNeighbor[NEIGHBOR_CA] = pTargetCell;

				if (dwTargetPoint & DW_A && POINT_B == i)
					pTargetCell->m_pNeighbor[NEIGHBOR_AB] = this;
				else if (dwTargetPoint & DW_A && POINT_C == i)
					pTargetCell->m_pNeighbor[NEIGHBOR_CA] = this;
				else if (dwTargetPoint & DW_B)
					pTargetCell->m_pNeighbor[NEIGHBOR_BC] = this;
			}
		}
	}
}

bool CCell::Compute_NeighborCell(const Vector3 & pPoint1, const Vector3 & pPoint2, CCell * pCell)
{
	if (m_vPoints[POINT_A] == pPoint1)
	{
		if (m_vPoints[POINT_B] == pPoint2)
		{
			m_pNeighbor[NEIGHBOR_AB] = pCell;
			return true;
		}
		else if (m_vPoints[POINT_C] == pPoint2)
		{
			m_pNeighbor[NEIGHBOR_CA] = pCell;
			return true;
		}
	}

	if (m_vPoints[POINT_B] == pPoint1)
	{
		if (m_vPoints[POINT_A] == pPoint2)
		{
			m_pNeighbor[NEIGHBOR_AB] = pCell;
			return true;
		}
		else if (m_vPoints[POINT_C] == pPoint2)
		{
			m_pNeighbor[NEIGHBOR_BC] = pCell;
			return true;
		}
	}

	if (m_vPoints[POINT_C] == pPoint1)
	{
		if (m_vPoints[POINT_A] == pPoint2)
		{
			m_pNeighbor[NEIGHBOR_CA] = pCell;
			return true;
		}
		else if (m_vPoints[POINT_B] == pPoint2)
		{
			m_pNeighbor[NEIGHBOR_BC] = pCell;
			return true;
		}
	}

	return false;
}

bool CCell::InitCell(const vector<NAVIPOINT>& vecPoints, CCell::CELL_OPT eCellOpt)
{
	ZeroMemory(m_pNeighbor, sizeof(CCell*)*DIR_END);
	ZeroMemory(m_vPoints, sizeof(Vector3)*POINT_END);
	ZeroMemory(m_vDir, sizeof(Vector3)*DIR_END);
	ZeroMemory(m_vNormal, sizeof(Vector3)*DIR_END);
	ZeroMemory(m_vEdgeCenter, sizeof(Vector3)*DIR_END);

	if (vecPoints.size() != 3)
		return false;

	m_eOption = eCellOpt;
	Calc_Point(vecPoints[0].vPosition, vecPoints[1].vPosition, vecPoints[2].vPosition);

	VERTEXCOLOR tLine[6] =
	{
		VERTEXCOLOR(m_vPoints[0].x, m_vPoints[0].y, m_vPoints[0].z, 1.f, 0.f, 0.f, 1.f),
		VERTEXCOLOR(m_vPoints[1].x, m_vPoints[1].y, m_vPoints[1].z, 1.f, 0.f, 0.f, 1.f),

		VERTEXCOLOR(m_vPoints[1].x, m_vPoints[1].y, m_vPoints[1].z, 1.f, 0.f, 0.f, 1.f),
		VERTEXCOLOR(m_vPoints[2].x, m_vPoints[2].y, m_vPoints[2].z, 1.f, 0.f, 0.f, 1.f),

		VERTEXCOLOR(m_vPoints[2].x, m_vPoints[2].y, m_vPoints[2].z, 1.f, 0.f, 0.f, 1.f),
		VERTEXCOLOR(m_vPoints[0].x, m_vPoints[0].y, m_vPoints[0].z, 1.f, 0.f, 0.f, 1.f),
	};

	UINT iLineIndex[6] = { 0, 1, 2, 3, 4, 5 };

	if (!CreateVertexBuffer(6, sizeof(VERTEXCOLOR), 
		D3D11_USAGE_DEFAULT, 
		D3D11_PRIMITIVE_TOPOLOGY_LINELIST,
		tLine
		))
		return false;

	if (!CreateIndexBuffer(6, sizeof(UINT), 
		D3D11_USAGE_DEFAULT, 
		DXGI_FORMAT_R32_UINT, 
		iLineIndex))
		return false;

	m_pShader = GET_SINGLE(CShaderManager)->FindShader(STANDARD_COLOR_SHADER);
	
	

	return true;
}

bool CCell::InitCell(const Vector3 * vecPoints, CCell::CELL_OPT eCellOpt)
{
	ZeroMemory(m_pNeighbor, sizeof(CCell*)*DIR_END);
	ZeroMemory(m_vPoints, sizeof(Vector3)*POINT_END);
	ZeroMemory(m_vDir, sizeof(Vector3)*DIR_END);
	ZeroMemory(m_vNormal, sizeof(Vector3)*DIR_END);
	ZeroMemory(m_vEdgeCenter, sizeof(Vector3)*DIR_END);

	if (vecPoints == nullptr)
		return false;

	m_eOption = eCellOpt;
	Calc_Point(vecPoints[0], vecPoints[1], vecPoints[2]);

	VERTEXCOLOR tLine[6] =
	{
		VERTEXCOLOR(m_vPoints[0].x, m_vPoints[0].y, m_vPoints[0].z, 1.f, 0.f, 0.f, 1.f),
		VERTEXCOLOR(m_vPoints[1].x, m_vPoints[1].y, m_vPoints[1].z, 1.f, 0.f, 0.f, 1.f),

		VERTEXCOLOR(m_vPoints[1].x, m_vPoints[1].y, m_vPoints[1].z, 1.f, 0.f, 0.f, 1.f),
		VERTEXCOLOR(m_vPoints[2].x, m_vPoints[2].y, m_vPoints[2].z, 1.f, 0.f, 0.f, 1.f),

		VERTEXCOLOR(m_vPoints[2].x, m_vPoints[2].y, m_vPoints[2].z, 1.f, 0.f, 0.f, 1.f),
		VERTEXCOLOR(m_vPoints[0].x, m_vPoints[0].y, m_vPoints[0].z, 1.f, 0.f, 0.f, 1.f),
	};

	UINT iLineIndex[6] = { 0, 1, 2, 3, 4, 5 };

	if (!CreateVertexBuffer(6, sizeof(VERTEXCOLOR),
		D3D11_USAGE_DEFAULT,
		D3D11_PRIMITIVE_TOPOLOGY_LINELIST,
		tLine
	))
		return false;

	if (!CreateIndexBuffer(6, sizeof(UINT),
		D3D11_USAGE_DEFAULT,
		DXGI_FORMAT_R32_UINT,
		iLineIndex))
		return false;

	m_pShader = GET_SINGLE(CShaderManager)->FindShader(STANDARD_COLOR_SHADER);



	return true;
}

int CCell::UpdateCell()
{
	return 0;
}

int CCell::LateUpdateCell()
{
	return 0;
}

void CCell::RenderCell()
{
	CScene		*pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
	CCamera*	pCamera = pScene->GetMainCamera();

	Matrix matScale;

	matScale.mat = XMMatrixScaling(1.f, 1.f, 1.f);

	m_tTransform.matWorld = matScale;

	m_tTransform.matView = pCamera->GetViewMatrix();
	m_tTransform.matProj = pCamera->GetProjMatrix();
	m_tTransform.matWV = m_tTransform.matWorld * m_tTransform.matView;
	m_tTransform.matWVP = m_tTransform.matWV * m_tTransform.matProj;
	m_tTransform.matWP = m_tTransform.matWorld * m_tTransform.matProj;
	m_tTransform.vPivot = Vector3(0.f, 0.f, 0.f);
	m_tTransform.vLength = Vector3::One;
	m_tTransform.vLength.z = 0.f;

	m_tTransform.matWorld = XMMatrixTranspose(m_tTransform.matWorld.mat);
	m_tTransform.matView = XMMatrixTranspose(m_tTransform.matView.mat);
	m_tTransform.matProj = XMMatrixTranspose(m_tTransform.matProj.mat);
	m_tTransform.matWV = XMMatrixTranspose(m_tTransform.matWV.mat);
	m_tTransform.matWVP = XMMatrixTranspose(m_tTransform.matWVP.mat);
	m_tTransform.matWP = XMMatrixTranspose(m_tTransform.matWP.mat);

	GET_SINGLE(CShaderManager)->UpdateCBuffer("Transform",
		&m_tTransform, SCT_VERTEX | SCT_PIXEL);
	m_pShader->SetShader();

	GET_SINGLE(CShaderManager)->SetInputLayout("VertexColor");

	//// Mesh Render
	UINT iOffset = 0;
	UINT iSize = m_tVB.iVtxSize;
	CONTEXT->IASetVertexBuffers(0, 1, &m_tVB.pBuffer, &iSize, &iOffset);
	CONTEXT->IASetPrimitiveTopology(m_tVB.ePrimitive);

	CONTEXT->IASetIndexBuffer(m_tIB.pBuffer,
		m_tIB.eFmt, 0);
	CONTEXT->DrawIndexed(m_tIB.iIdxCount, 0, 0);

	SAFE_RELEASE(pCamera);
	SAFE_RELEASE(pScene);
}

CCell * CCell::Clone()
{
	return nullptr;
}

bool CCell::CreateVertexBuffer(UINT iVtxCount, UINT iVtxSize, D3D11_USAGE eVtxUsage, D3D11_PRIMITIVE_TOPOLOGY ePrimitive, void * pVtxData)
{
	m_tVB.pBuffer = NULL;
	m_tVB.iVtxCount = iVtxCount;
	m_tVB.iVtxSize = iVtxSize;
	m_tVB.eUsage = eVtxUsage;
	m_tVB.ePrimitive = ePrimitive;
	m_tVB.pVtxData = new char[iVtxCount * iVtxSize];

	memcpy(m_tVB.pVtxData, pVtxData, iVtxCount * iVtxSize);

	D3D11_BUFFER_DESC	tDesc = {};
	tDesc.ByteWidth = iVtxCount * iVtxSize;
	tDesc.Usage = eVtxUsage;
	tDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	if (eVtxUsage == D3D11_USAGE_DYNAMIC)
		tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA	tData = {};
	tData.pSysMem = pVtxData;

	if (FAILED(DEVICE->CreateBuffer(&tDesc, &tData, &m_tVB.pBuffer)))
		return false;

	return true;
}

bool CCell::CreateIndexBuffer(UINT iIdxCount, UINT iIdxSize, D3D11_USAGE eIdxUsage, DXGI_FORMAT eFmt, void * pIdxData)
{
	if (iIdxCount == 0)
		return true;

	m_tIB.pBuffer = NULL;
	m_tIB.iIdxCount = iIdxCount;
	m_tIB.iIdxSize = iIdxSize;
	m_tIB.eUsage = eIdxUsage;
	m_tIB.eFmt = eFmt;
	m_tIB.pIdxData = new char[iIdxCount * iIdxSize];

	memcpy(m_tIB.pIdxData, pIdxData, iIdxCount * iIdxSize);

	D3D11_BUFFER_DESC	tDesc = {};
	tDesc.ByteWidth = iIdxCount * iIdxSize;
	tDesc.Usage = eIdxUsage;
	tDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	if (eIdxUsage == D3D11_USAGE_DYNAMIC)
		tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA	tData = {};
	tData.pSysMem = pIdxData;

	if (FAILED(DEVICE->CreateBuffer(&tDesc, &tData, &m_tIB.pBuffer)))
		return false;

	return true;
}

void CCell::Calc_Point(Vector3 P0, Vector3 P1, Vector3 P2)
{
	/* Point */
	m_vPoints[POINT_A] = P0;
	//m_vPoints[POINT_A].y -= 0.2f;

	m_vPoints[POINT_B] = P1;
	//m_vPoints[POINT_B].y -= 0.2f;

	m_vPoints[POINT_C] = P2;
	//m_vPoints[POINT_C].y -= 0.2f;

	/* Direction */
	m_vDir[DIR_AB] = m_vPoints[POINT_B] - m_vPoints[POINT_A];
	m_vDir[DIR_BC] = m_vPoints[POINT_C] - m_vPoints[POINT_B];
	m_vDir[DIR_CA] = m_vPoints[POINT_A] - m_vPoints[POINT_C];

	m_vEdgeCenter[DIR_AB] = (m_vPoints[POINT_B] + m_vPoints[POINT_A]) / 2.f;
	m_vEdgeCenter[DIR_BC] = (m_vPoints[POINT_C] + m_vPoints[POINT_B]) / 2.f;
	m_vEdgeCenter[DIR_CA] = (m_vPoints[POINT_A] + m_vPoints[POINT_C]) / 2.f;
	
	/* Normal */
	for (size_t i = 0; i < DIR_END; ++i)
	{
		m_vNormal[i] = Vector3(-m_vDir[i].z, m_vDir[i].y, m_vDir[i].x).Normalize();
	}

	/* Center */
	m_vCenter = Vector3{ (m_vPoints[POINT_A].x + m_vPoints[POINT_B].x + m_vPoints[POINT_C].x) / 3,
						(m_vPoints[POINT_A].y + m_vPoints[POINT_B].y + m_vPoints[POINT_C].y) / 3,
						(m_vPoints[POINT_A].z + m_vPoints[POINT_B].z + m_vPoints[POINT_C].z) / 3 };
}
