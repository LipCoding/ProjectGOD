#include "Billboard.h"
#include "../Component/Transform.h"
#include "../Component/Camera.h"
#include "../Scene/SceneManager.h"
#include "../Scene/Scene.h"
#include "../GameObject/GameObject.h"

PG_USING

CBillboard::CBillboard()
{
	SetTag("Billboard");
	SetTypeName("CBillboard");
	SetTypeID<CBillboard>();
}

CBillboard::CBillboard(const CBillboard & billboard)
{
	m_pCam = billboard.m_pCam;
	billboard.m_pCam->AddRef();

	m_pObjTr = billboard.m_pObjTr;
	billboard.m_pObjTr->AddRef();

	m_pCameraTr = billboard.m_pCameraTr;
	billboard.m_pCameraTr->AddRef();

	m_isOperate = billboard.m_isOperate;
}

CBillboard::~CBillboard()
{
	SAFE_RELEASE(m_pObjTr);
	SAFE_RELEASE(m_pCam);
	SAFE_RELEASE(m_pCameraTr);
}

bool CBillboard::Init()
{
	CScene *pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
	m_pCam = pScene->GetMainCamera();
	m_pCameraTr = m_pCam->GetTransform();
	SAFE_RELEASE(pScene);

	m_pObjTr = m_pGameObject->GetTransform();

	m_vOriginRot = m_pObjTr->GetWorldRot();
	m_vOriginRot.x = XMConvertToDegrees(m_vOriginRot.x);
	m_vOriginRot.y = XMConvertToDegrees(m_vOriginRot.y);
	m_vOriginRot.z = XMConvertToDegrees(m_vOriginRot.z);

	return true;
}

void CBillboard::Input(float fTime)
{
}

int CBillboard::Update(float fTime)
{
	if (!m_isOperate)
		return 0;

	Vector3 vPos = m_pObjTr->GetWorldPos();
	Vector3 vCamPos = m_pCameraTr->GetWorldPos();

	double angle;
	float rotationX = 0.f, rotationY = 0.f;

	/* x (동작 불가) */
	angle = atan2(vPos.y - vCamPos.y, vPos.z - vCamPos.z) * (180.0 / PG_PI);
	rotationX = (float)XMConvertToRadians(angle);

	//m_pObjTr->SetWorldRotX(rotationX);

	/* y */
	angle = atan2(vPos.x - vCamPos.x, vPos.z - vCamPos.z) * (180.0 / PG_PI);
	rotationY = (float)XMConvertToRadians(angle);

	m_pObjTr->SetWorldRotY(rotationY);

	//m_pObjTr->SetWorldRot(rotationX, rotationY, 0.f);

	return 0;
}

int CBillboard::LateUpdate(float fTime)
{
	return 0;
}

void CBillboard::Collision(float fTime)
{
}

void CBillboard::Render(float fTime)
{
}

CBillboard * CBillboard::Clone()
{
	return new CBillboard(*this);
}
