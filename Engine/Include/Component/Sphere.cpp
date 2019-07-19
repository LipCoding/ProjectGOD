#include "Sphere.h"
#include "../Resources/Mesh.h"
#include "../Resources/ResourcesManager.h"
#include "../Rendering/Shader.h"
#include "../Rendering/ShaderManager.h"
#include "Transform.h"
#include "../GameObject/GameObject.h"
#include "../Rendering/RenderManager.h"
#include "../Rendering/RenderState.h"
#include "../Scene/SceneManager.h"
#include "../Scene/Scene.h"
#include "../Scene/Layer.h"
#include "../Component/Camera.h"
#include "../Component/Renderer.h"

PG_USING

CSphere::CSphere()
{
	//m_eType = CT_SPHERE;
	m_pMeshSphere = nullptr;
	m_pLayout = nullptr;
	m_bRenderCheck = false;
	m_vSize = Vector3(1.f, 1.f, 1.f);
}

CSphere::CSphere(const CSphere & sphere)
{
	//
}

CSphere::~CSphere()
{
	SAFE_RELEASE(m_pMeshSphere);
	SAFE_RELEASE(m_pShaderSphere);
}

void CSphere::SetObjMatrix(Matrix *matrix)
{
	m_matObjWorld = matrix;
}

void CSphere::SetRenderCheck(bool check)
{
	m_bRenderCheck = check;
}

void CSphere::SetSize(const Vector3 & size)
{
	m_vSize = size;
}

bool CSphere::Init()
{
	m_pMeshSphere = GET_SINGLE(CResourcesManager)->FindMesh("Sphere");
	m_pShaderSphere = GET_SINGLE(CShaderManager)->FindShader(STANDARD_COLOR_SHADER);
	m_pLayout = GET_SINGLE(CShaderManager)->FindInputLayout("VertexColor");

	return true;
}

void CSphere::Input(float fTime)
{
}

int CSphere::Update(float fTime)
{
	return 0;
}

int CSphere::LateUpdate(float fTime)
{
	return 0;
}

void CSphere::Render(float fTime)
{
	SphereRender(fTime);
}

void CSphere::SphereRender(float fTime)
{
	if (!m_bRenderCheck)
		return;

	CCamera*	pCamera = m_pScene->GetMainCamera();

	Matrix matScale;

	matScale.mat = XMMatrixScaling(m_vSize.x, m_vSize.x, m_vSize.x);
	m_tTransform.matWorld = matScale * *m_matObjWorld;

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

	SAFE_RELEASE(pCamera);
}

CSphere * CSphere::Clone()
{
	return nullptr;
}
