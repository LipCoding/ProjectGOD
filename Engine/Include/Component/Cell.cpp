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
}

CCell::CCell(const CCell & axisLine)
{
}

CCell::~CCell()
{
	SAFE_RELEASE(m_tVB.pBuffer);
	SAFE_DELETE_ARRAY(m_tVB.pVtxData);
	SAFE_RELEASE(m_tIB.pBuffer);
	SAFE_DELETE_ARRAY(m_tIB.pIdxData);
	SAFE_RELEASE(m_pShader);
}

bool CCell::Init(const vector<NAVIPOINT>& vecPoints)
{
	if (!SetPoints(vecPoints))
		return false;

	VERTEXCOLOR tLine[6] =
	{
		VERTEXCOLOR(m_points[0].x, m_points[0].y, m_points[0].z, 1.f, 0.f, 0.f, 1.f),
		VERTEXCOLOR(m_points[1].x, m_points[1].y, m_points[1].z, 1.f, 0.f, 0.f, 1.f),

		VERTEXCOLOR(m_points[1].x, m_points[1].y, m_points[1].z, 1.f, 0.f, 0.f, 1.f),
		VERTEXCOLOR(m_points[2].x, m_points[2].y, m_points[2].z, 1.f, 0.f, 0.f, 1.f),

		VERTEXCOLOR(m_points[2].x, m_points[2].y, m_points[2].z, 1.f, 0.f, 0.f, 1.f),
		VERTEXCOLOR(m_points[0].x, m_points[0].y, m_points[0].z, 1.f, 0.f, 0.f, 1.f),
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

int CCell::Update()
{
	return 0;
}

int CCell::LateUpdate()
{
	return 0;
}

void CCell::Render()
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

	//char*	pVtx = (char*)pVtxData;

	//// 정점의 Min, Max 값을 구한다.
	//for (int i = 0; i < iVtxCount; ++i)
	//{
	//	Vector3	vPos;
	//	memcpy(&vPos, pVtx + iVtxSize * i, sizeof(Vector3));

	//	if (vPos.x < m_vMin.x)
	//		m_vMin.x = vPos.x;

	//	if (vPos.y < m_vMin.y)
	//		m_vMin.y = vPos.y;

	//	if (vPos.z < m_vMin.z)
	//		m_vMin.z = vPos.z;

	//	if (vPos.x > m_vMax.x)
	//		m_vMax.x = vPos.x;

	//	if (vPos.y > m_vMax.y)
	//		m_vMax.y = vPos.y;

	//	if (vPos.z > m_vMax.z)
	//		m_vMax.z = vPos.z;
	//}

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
