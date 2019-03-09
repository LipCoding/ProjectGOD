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

PG_USING

CAxisLine::CAxisLine()
{
	SetTypeID<CAxisLine>();
	m_eType = CT_AXIS;
	m_pMesh = nullptr;
	m_pShader = nullptr;
	m_pLayout = nullptr;
}

CAxisLine::CAxisLine(const CAxisLine & axisLine)
{
	m_pMesh = axisLine.m_pMesh;
	m_pShader = axisLine.m_pShader;
	m_pLayout = axisLine.m_pLayout;

	if (m_pMesh)
		m_pMesh->AddRef();

	if (m_pShader)
		m_pShader->AddRef();
}


CAxisLine::~CAxisLine()
{
	SAFE_RELEASE(m_pMesh);
	SAFE_RELEASE(m_pShader);
}

bool CAxisLine::Init()
{
	m_pMesh = GET_SINGLE(CResourcesManager)->FindMesh("Line");
	m_pShader = GET_SINGLE(CShaderManager)->FindShader(STANDARD_COLOR_SHADER);
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
	//Matrix	matScale, matTrans, matRot;

	//matScale.mat = XMMatrixScaling(2.f, 2.f, 2.f);
	//matTrans.mat = XMMatrixTranslation(5.f, 1.f, 5.f);

	//CCamera*	pCamera = m_pScene->GetMainCamera();

	//m_tTransform.matWorld = matScale *
	//	matRot * matTrans;

	//m_tTransform.matView = pCamera->GetViewMatrix();
	//m_tTransform.matProj = pCamera->GetProjMatrix();
	//m_tTransform.matWV = m_tTransform.matWorld * m_tTransform.matView;
	//m_tTransform.matWVP = m_tTransform.matWV * m_tTransform.matProj;
	//m_tTransform.matWP = m_tTransform.matWorld * m_tTransform.matProj;
	//m_tTransform.vPivot = Vector3(0.f, 0.f, 0.f);
	//m_tTransform.vLength = Vector3::One;
	//m_tTransform.vLength.z = 0.f;

	//m_tTransform.matWorld = XMMatrixTranspose(m_tTransform.matWorld.mat);
	//m_tTransform.matView = XMMatrixTranspose(m_tTransform.matView.mat);
	//m_tTransform.matProj = XMMatrixTranspose(m_tTransform.matProj.mat);
	//m_tTransform.matWV = XMMatrixTranspose(m_tTransform.matWV.mat);
	//m_tTransform.matWVP = XMMatrixTranspose(m_tTransform.matWVP.mat);
	//m_tTransform.matWP = XMMatrixTranspose(m_tTransform.matWP.mat);

	//SAFE_RELEASE(pCamera);

	//GET_SINGLE(CShaderManager)->UpdateCBuffer("Transform",
	//	&m_tTransform, SCT_VERTEX | SCT_PIXEL);
	//
	//m_pShader->SetShader();
	//GET_SINGLE(CShaderManager)->SetInputLayout("VertexColor");
	//m_pMesh->Render(fTime);
}

void CAxisLine::AxisRender(float fTime)
{
	Matrix	matScale, matTrans, matRot;

	matScale.mat = XMMatrixScaling(0.5f, 0.5f, 0.5f);
	matTrans.mat = XMMatrixTranslation(5.f, 1.f, 5.f);

	CCamera*	pCamera = m_pScene->GetMainCamera();

	m_tTransform.matWorld = matScale *
		matRot * matTrans;

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

	SAFE_RELEASE(pCamera);

	GET_SINGLE(CShaderManager)->UpdateCBuffer("Transform",
		&m_tTransform, SCT_VERTEX | SCT_PIXEL);

	m_pShader->SetShader();
	GET_SINGLE(CShaderManager)->SetInputLayout("VertexColor");
	m_pMesh->Render(fTime);
}

CAxisLine * CAxisLine::Clone()
{
	return nullptr;
}
