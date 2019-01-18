#include "Mouse.h"
#include "ColliderPoint.h"
#include "ColliderRay.h"
#include "../GameObject/GameObject.h"
#include "Transform.h"
#include "Animation2D.h"
#include "Renderer2D.h"
#include "Material.h"
#include "../Device.h"
#include "Camera.h"
#include "../Scene/Scene.h"

PG_USING

CMouse::CMouse()
{
	SetTag("Mouse");
	SetTypeName("CMouse");
	SetTypeID<CMouse>();
	m_eType = CT_MOUSE;
	m_bMouseShow = FALSE;
}

CMouse::CMouse(const CMouse & mouse)	:
	CComponent(mouse)
{
	m_bMouseShow = FALSE;
}

CMouse::~CMouse()
{
}

Vector3 CMouse::GetRayPos() const
{
	return m_vRayPos;
}

Vector3 CMouse::GetRayDir() const
{
	return m_vRayDir;
}

bool CMouse::Init()
{
	ShowCursor(FALSE);

	m_pTransform->SetWorldScale(32.f, 31.f, 1.f);

	CRenderer2D*	pRenderer = m_pGameObject->AddComponent<CRenderer2D>("MouseRenderer");

	pRenderer->SetMesh("RectOrthoLTTex");
	pRenderer->SetShader(UI_SHADER);
	pRenderer->SetInputLayout("VertexTex");
	pRenderer->SetRenderState(ALPHA_BLEND);

	vector<wstring>	vecPath;
	for (int i = 0; i < 1; ++i)
	{
		wchar_t	strPath[MAX_PATH] = {};
		wsprintf(strPath, L"Mouse/Default/%d.png", i);
		vecPath.push_back(strPath);
	}

	CMaterial*	pMaterial	= pRenderer->CreateMaterial(SAMPLER_LINEAR,
		"MouseDefault", 10, 0, &vecPath);

	SAFE_RELEASE(pMaterial);

	SAFE_RELEASE(pRenderer);

	CAnimation2D*	pAnimation = m_pGameObject->AddComponent<CAnimation2D>("MouseAnimation");

	pAnimation->CreateClip("MouseDefault", A2D_FRAME, A2DO_LOOP, 1, 1,
		1, 1, 0, 1.f, 0, 0.f, "MouseDefault", &vecPath);

	SAFE_RELEASE(pAnimation);

	// 포인트 충돌체 생성
	CColliderPoint*	pColl = m_pGameObject->AddComponent<CColliderPoint>("MousePoint");

	pColl->SetPos(m_pTransform->GetWorldPos().x,
		m_pTransform->GetWorldPos().y);

	SAFE_RELEASE(pColl);

	// Ray 충돌체 생성
	CColliderRay*	pCollRay = m_pGameObject->AddComponent<CColliderRay>("MouseRay");

	SAFE_RELEASE(pCollRay);

	return true;
}

void CMouse::Input(float fTime)
{
}

int CMouse::Update(float fTime)
{
	// 화면 밖을 나가는지 체크한다.
	Vector3	vPos = m_pTransform->GetWorldPos();
	if (vPos.x < 0 || vPos.x > DEVICE_RESOLUTION.iWidth ||
		vPos.y < 0 || vPos.y > DEVICE_RESOLUTION.iHeight)
	{
		if (!m_bMouseShow)
		{
			m_bMouseShow = TRUE;
			ShowCursor(TRUE);
		}
	}

	else
	{
		if (m_bMouseShow)
		{
			m_bMouseShow = FALSE;
			ShowCursor(FALSE);
		}
	}

	// 마우스 Ray를 만든다.
	// 카메라를 얻어온다.	
	CCamera*	pCamera = m_pScene->GetMainCamera();

	Matrix	matProj = pCamera->GetProjMatrix();
	Matrix	matView = pCamera->GetViewMatrix();

	SAFE_RELEASE(pCamera);

	D3D11_VIEWPORT	tVP = {};

	UINT	iVPCount = 1;
	CONTEXT->RSGetViewports(&iVPCount, &tVP);

	float	fHalfW = tVP.Width * 0.5f;
	float	fHalfH = tVP.Height * 0.5f;

	Vector3	vRayDir;

	// 뷰공간에서의 Ray를 구해준다.
	vRayDir.x = (vPos.x / fHalfW - 1.f) / matProj.m[0][0];
	vRayDir.y = (-(vPos.y / fHalfH) + 1.f) / matProj.m[1][1];
	vRayDir.z = 1.f;

	vRayDir = vRayDir.Normalize();

	// 월드공간으로 변환한다.
	// 뷰의 역행렬을 구해준다.
	matView	= XMMatrixInverse(&XMMatrixDeterminant(matView.mat),
		matView.mat);
	vRayDir = vRayDir.TransformNormal(matView.mat);

	Vector3	vRayPos;

	vRayPos = vRayPos.TransformCoord(matView.mat);

	// Ray 충돌체를 얻어온다.
	CColliderRay*	pRay = FindComponentFromTag<CColliderRay>("MouseRay");

	pRay->SetPos(vRayPos);
	pRay->SetDir(vRayDir);

	SAFE_RELEASE(pRay);

	m_vRayPos = vRayPos;
	m_vRayDir = vRayDir;

	return 0;
}

int CMouse::LateUpdate(float fTime)
{
	return 0;
}

void CMouse::Collision(float fTime)
{
}

void CMouse::Render(float fTime)
{
}

CMouse * CMouse::Clone()
{
	return new CMouse(*this);
}
