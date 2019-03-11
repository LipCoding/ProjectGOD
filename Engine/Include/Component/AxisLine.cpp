#include "AxisLine.h"
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

CAxisLine::CAxisLine()
{
	SetTypeID<CAxisLine>();
	m_eType = CT_AXIS;
	m_pMeshSphere = nullptr;
	m_pShaderSphere = nullptr;
	m_pLayout = nullptr;
	m_bRednerCheck = false;

	for (int i = 0; i < 3; i++)
	{
		m_pFont[i] = GET_SINGLE(CDevice)->FindTextFormat("¹ÙÅÁ");
		m_pBrush[i] = GET_SINGLE(CDevice)->FindTextBrush("Black");
	}

	m_strText[0] = L"X";
	m_strText[1] = L"Y";
	m_strText[2] = L"Z";

	m_vecPosFont[0] = Vector3{ 0.25f, 0.f, 0.f };
	m_vecPosFont[1] = Vector3{ 0.f, 0.25f, 0.f };
	m_vecPosFont[2] = Vector3{ 0.f, 0.f, 0.25f };
}

CAxisLine::CAxisLine(const CAxisLine & axisLine)
{
	m_pMeshSphere = axisLine.m_pMeshSphere;
	m_pShaderSphere = axisLine.m_pShaderSphere;
	m_pLayout = axisLine.m_pLayout;

	if (m_pMeshSphere)
		m_pMeshSphere->AddRef();

	if (m_pShaderSphere)
		m_pShaderSphere->AddRef();

	if (m_pMeshLine)
		m_pMeshLine->AddRef();
}


CAxisLine::~CAxisLine()
{
	SAFE_RELEASE(m_pMeshSphere);
	SAFE_RELEASE(m_pShaderSphere);
	SAFE_RELEASE(m_pMeshLine);
}

void CAxisLine::SetObjMatrix(Matrix matrix)
{
	m_matObjWorld = matrix;
}

void CAxisLine::SetBoneMatrix(Matrix* matrix)
{
	m_matBoneWorld = matrix;
}

void CAxisLine::SetRenderCheck(bool check)
{
	m_bRednerCheck = check;
}

bool CAxisLine::Init()
{
	m_pMeshSphere = GET_SINGLE(CResourcesManager)->FindMesh("Sphere");
	m_pMeshLine = GET_SINGLE(CResourcesManager)->FindMesh("Line");
	m_pShaderSphere = GET_SINGLE(CShaderManager)->FindShader(STANDARD_COLOR_SHADER);
	m_pLayout = GET_SINGLE(CShaderManager)->FindInputLayout("VertexColor");


	return true;
}

void CAxisLine::Input(float fTime)
{
}

int CAxisLine::Update(float fTime)
{
	return 0;
}

int CAxisLine::LateUpdate(float fTime)
{
	return 0;
}

void CAxisLine::Render(float fTime)
{
	
}

void CAxisLine::AxisRender(float fTime)
{
	if (!m_bRednerCheck)
		return;

	CCamera*	pCamera = m_pScene->GetMainCamera();

	Matrix matScale;
	
	matScale.mat = XMMatrixScaling(0.03f, 0.03f, 0.03f);
	m_tTransform.matWorld = matScale * *m_matBoneWorld * m_matObjWorld;

	/*m_tTransform.matWorld = m_matObjWorld * *m_matBoneWorld;*/

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
	m_pShaderSphere->SetShader();

	GET_SINGLE(CShaderManager)->SetInputLayout("VertexColor");
	
	m_pMeshSphere->Render(fTime);
	m_pMeshLine->Render(fTime);

	SAFE_RELEASE(pCamera);
}

CAxisLine * CAxisLine::Clone()
{
	return nullptr;
}

void CAxisLine::RenderFont()
{
	for (int i = 0; i < 3; i++)
	{
		
	}
}
