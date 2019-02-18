#include "DebugTree.h"
#include "../GameObject/GameObject.h"
#include "../Resources/ResourcesManager.h"
#include "../Rendering/ShaderManager.h"
#include "../Scene/SceneManager.h"
#include "../Component/Camera.h"

PG_USING

DebugTree::DebugTree()
	:
	m_pScene(nullptr),
	m_pMesh(nullptr),
	m_pShader(nullptr),
	m_pLayout(nullptr)
{

}

DebugTree::DebugTree(const DebugTree & debugTree)
{
}


DebugTree::~DebugTree()
{
}

bool DebugTree::Init()
{
	m_pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();

	m_pMesh = GET_SINGLE(CResourcesManager)->FindMesh("AABB");
	m_pShader = GET_SINGLE(CShaderManager)->FindShader(COLLIDER_SHADER);
	m_pLayout = GET_SINGLE(CShaderManager)->FindInputLayout("VertexColor");

	return true;
}

void DebugTree::Update(float fTime)
{
	Matrix	matScale, matTrans, matRot;

	matScale.mat = XMMatrixScaling(m_vLength.x, m_vLength.y, m_vLength.z);
	matTrans.mat = XMMatrixTranslation(m_vPos.x, m_vPos.y, m_vPos.z);

	CCamera* pCamera = m_pScene->GetMainCamera();

	m_tTransform.matWorld = matScale * matRot * matTrans;

	m_tTransform.matView = pCamera->GetViewMatrix();
	m_tTransform.matProj = pCamera->GetProjMatrix();
	m_tTransform.matWV = m_tTransform.matWorld * m_tTransform.matView;
	m_tTransform.matWVP = m_tTransform.matWV * m_tTransform.matProj;
	m_tTransform.matWP = m_tTransform.matWorld * m_tTransform.matProj;
	m_tTransform.vPivot = Vector3(0.5f, 0.5f, 0.5f);
	m_tTransform.vLength = Vector3::One;
	m_tTransform.vLength.z = 0.f;

	m_tTransform.matWorld = XMMatrixTranspose(m_tTransform.matWorld.mat);
	m_tTransform.matView = XMMatrixTranspose(m_tTransform.matView.mat);
	m_tTransform.matProj = XMMatrixTranspose(m_tTransform.matProj.mat);
	m_tTransform.matWV = XMMatrixTranspose(m_tTransform.matWV.mat);
	m_tTransform.matWVP = XMMatrixTranspose(m_tTransform.matWVP.mat);
	m_tTransform.matWP = XMMatrixTranspose(m_tTransform.matWP.mat);

	SAFE_RELEASE(pCamera);

	
}

void DebugTree::Render(float fTime)
{
	GET_SINGLE(CShaderManager)->UpdateCBuffer("Transform",
		&m_tTransform, SCT_VERTEX | SCT_PIXEL);
	GET_SINGLE(CShaderManager)->UpdateCBuffer("ColliderColor",
		&Vector4::Green, SCT_VERTEX);

	m_pShader->SetShader();
	GET_SINGLE(CShaderManager)->SetInputLayout("VertexColor");
	m_pMesh->Render(fTime);
}

void DebugTree::SetPosition(float posX, float posZ, float width)
{
	Vector3 vMin = Vector3{ posX - width / 2.f, 10.f, posZ - width / 2.f };
	Vector3 vMax = Vector3{ posX + width / 2.f, 10.f, posZ + width / 2.f };

	m_vLength = vMax - vMin;
	m_vPos = (vMax + vMin) / 2.f;
}

void DebugTree::Release()
{
	SAFE_RELEASE(m_pLayout);
	SAFE_RELEASE(m_pShader);
	SAFE_RELEASE(m_pMesh);
	SAFE_RELEASE(m_pScene);
}

