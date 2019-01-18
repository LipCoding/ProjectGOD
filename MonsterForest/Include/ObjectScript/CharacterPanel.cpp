#include "CharacterPanel.h"
#include "GameObject/GameObject.h"
#include "Scene/Scene.h"
#include "Component/Collider.h"
#include "Component/Camera.h"
#include "Device.h"
#include "Rendering/RenderManager.h"
#include "Rendering/RenderState.h"
#include "Rendering/RenderingTarget.h"
#include "Component/Renderer2D.h"
#include "Component/Renderer.h"
#include "Component/Material.h"

CCharacterPanel::CCharacterPanel()	:
	m_bAddResourceView(false)
{
}

CCharacterPanel::CCharacterPanel(const CCharacterPanel & panel)	:
	CScript(panel)
{
	m_bAddResourceView = false;
}

CCharacterPanel::~CCharacterPanel()
{
	SAFE_RELEASE(m_pCameraTransform);
	SAFE_RELEASE(m_pTargetCamera);
}

bool CCharacterPanel::Init()
{
	CRenderingTarget* pTarget = GET_SINGLE(CRenderManager)->CreateRenderTarget("CharacterTarget",
		DEVICE_RESOLUTION.iWidth, DEVICE_RESOLUTION.iHeight,
		DXGI_FORMAT_R8G8B8A8_UNORM, Vector4::Zero, DXGI_FORMAT_D24_UNORM_S8_UINT);

	pTarget->SetDebugEnable(true);
	pTarget->SetDebugInfo(Vector3(400.f, 0.f, 0.f), Vector3(100.f, 100.f, 1.f));

	SAFE_RELEASE(pTarget);

	CGameObject*	pCameraObj	= m_pScene->CreateCamera("CharTargetCamera",
		Vector3::Zero, PG_PI / 3.f, DEVICE_RESOLUTION.iWidth / (float)DEVICE_RESOLUTION.iHeight,
		0.03f, 5000.f);

	m_pTargetCamera = pCameraObj->FindComponentFromType<CCamera>(CT_CAMERA);

	m_pCameraTransform = pCameraObj->GetTransform();

	SAFE_RELEASE(pCameraObj);

	return true;
}

void CCharacterPanel::Input(float fTime)
{
}

int CCharacterPanel::Update(float fTime)
{
	return 0;
}

int CCharacterPanel::LateUpdate(float fTime)
{
	return 0;
}

void CCharacterPanel::Render(float fTime)
{
	CRenderingTarget* pTarget = GET_SINGLE(CRenderManager)->FindRenderTarget("CharacterTarget");

	if (!m_bAddResourceView)
	{
		m_bAddResourceView = true;
		CRenderer2D*	pRenderer = FindComponentFromType<CRenderer2D>(CT_RENDERER2D);

		CMaterial*	pMaterial = pRenderer->GetMaterial();

		pMaterial->AddMultiTexture(SAMPLER_LINEAR, pTarget->GetShaderResourceView(),
			11, 11);
		SAFE_RELEASE(pMaterial);

		pRenderer->CreateCBuffer("MultiTexture", 12, sizeof(MULTITEXTURECBUFFER),
			SCT_PIXEL);

		MULTITEXTURECBUFFER	tBuffer = {};
		tBuffer.vAreaStart = Vector2(0.1f, 0.f);
		tBuffer.vAreaEnd = Vector2(0.4f, 0.5f);

		pRenderer->UpdateCBuffer("MultiTexture", 12, sizeof(MULTITEXTURECBUFFER),
			SCT_PIXEL, &tBuffer);

		pRenderer->SetShader(UI_MULTITEXTURE_SHADER);

		SAFE_RELEASE(pRenderer);
	}

	// 타겟의 바로 앞 위치로 위치를 설정한다.
	Vector3	vTargetPos = m_pTargetTransform->GetWorldPos() + Vector3(0.f, 1.f, 0.f);
	m_pCameraTransform->SetWorldPos(m_pTargetTransform->GetWorldPos() +
		m_pTargetTransform->GetWorldAxis(AXIS_Z) * 2.f + Vector3(0.f, 1.f, 0.f));

	//m_pCameraTransform->LookAt(vTargetPos);
	//Vector3	vDir = vTargetPos - m_pCameraTransform->GetWorldPos();
	//vDir = vDir.Normalize();

	// 원본 축 벡터와 바라볼 벡터간 각도를 구해준다.
	// 이 각도를 이용해서 회전한다.
	//float	fAngle = vDir.Angle(Vector3::Axis[AXIS_Z]);

	//m_pCameraTransform->SetWorldRotY(fAngle);
	m_pCameraTransform->SetWorldAxis(m_pTargetTransform->GetWorldAxis(AXIS_X) * -1.f,
		AXIS_X);
	m_pCameraTransform->SetWorldAxis(m_pTargetTransform->GetWorldAxis(AXIS_Y),
		AXIS_Y);
	m_pCameraTransform->SetWorldAxis(m_pTargetTransform->GetWorldAxis(AXIS_Z) * -1.f,
		AXIS_Z);

	//m_pCameraTransform->ComputeWorldAxisRot();

	m_pTargetCamera->LateUpdate(fTime);

	m_pScene->ChangeMainCamera("CharTargetCamera");

	pTarget->SetTarget();
	pTarget->ClearTarget();

	//CRenderState*	pDepthDisable = GET_SINGLE(CRenderManager)->FindRenderState(DEPTH_DISABLE);

	//pDepthDisable->SetState();

	CRenderer*	pTargetRenderer = m_pTarget->FindComponentFromType<CRenderer>(CT_RENDERER);

	pTargetRenderer->RenderForward(fTime);

	SAFE_RELEASE(pTargetRenderer);

	//pDepthDisable->ResetState();

	pTarget->ResetTarget();

	SAFE_RELEASE(pTarget);

	m_pScene->ChangeMainCamera("MainCamera");
}

CCharacterPanel * CCharacterPanel::Clone()
{
	return new CCharacterPanel(*this);
}

void CCharacterPanel::OnCollisionEnter(CCollider * pSrc, CCollider * pDest, float fTime)
{
}

void CCharacterPanel::OnCollision(CCollider * pSrc, CCollider * pDest, float fTime)
{
}

void CCharacterPanel::OnCollisionLeave(CCollider * pSrc, CCollider * pDest, float fTime)
{
}
