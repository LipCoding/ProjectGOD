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
	Vector3 vScale = m_pObjTr->GetWorldScale();
	Vector3 vCamPos = m_pCameraTr->GetWorldPos();

	double angle;
	float rotation;

	/* x (동작 불가) */
	/*angle = atan2(vPos.z - vCamPos.z, vPos.y - vCamPos.y) * (180.0 / PG_PI);
	rotation = (float)XMConvertToRadians(angle);

	m_pObjTr->SetWorldRotX(rotation);*/

	/* y */
	angle = atan2(vPos.x - vCamPos.x, vPos.z - vCamPos.z) * (180.0 / PG_PI);
	rotation = (float)XMConvertToRadians(angle);

	m_pObjTr->SetWorldRotY(rotation);

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
