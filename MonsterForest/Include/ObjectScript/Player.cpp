#include "Player.h"
#include "Component/Transform.h"
#include "Core/Input.h"
#include "Component/Renderer.h"
#include "Component/ColliderAABB.h"
#include "Component/Material.h"
#include "Shield.h"
#include "Component/Arm.h"
#include "Gun.h"
#include "Scene/Scene.h"
#include "../ObjectScript/Effect.h"
#include "Core/QuadTreeManager.h"

CPlayer::CPlayer()
{
	SetTag("Player");
	SetTypeName("CPlayer");
	SetTypeID<CPlayer>();
	m_bOnShield = false;
	m_bOnShieldRange = false;
	memset(m_pShield, 0, sizeof(CGameObject*) * 3);
	m_pNavigation = NULL;
}

CPlayer::CPlayer(const CPlayer & player) :
	CScript(player)
{
	m_fMoveSpeed = player.m_fMoveSpeed;
	m_fRotSpeed = player.m_fRotSpeed;
	m_bOnShield = false;
	m_bOnShieldRange = false;
	memset(m_pShield, 0, sizeof(CGameObject*) * 3);

	m_pNavigation = NULL;
}

CPlayer::~CPlayer()
{
	SAFE_RELEASE(m_pArm);
	SAFE_RELEASE(m_pNavigation);
	SAFE_RELEASE(m_pAnimation);
	for (int i = 0; i < 3; ++i)
	{
		SAFE_RELEASE(m_pShield[i]);
	}
}

bool CPlayer::Init()
{
	m_fMoveSpeed = 5.f;
	m_fRotSpeed = PG_PI;
	m_fRecentRot = 0.f;

	m_pAnimation = m_pGameObject->AddComponent<CAnimation>("PlayerAnimation");

	m_pAnimation->Load("99.Dynamic_Mesh\\00.Player\\Player.anm");
	m_pAnimation->SetDefaultClip("Idle");

	GET_SINGLE(CInput)->CreateKey("RotInvX", 'Q');
	GET_SINGLE(CInput)->CreateKey("RotX", 'E');

	GET_SINGLE(CInput)->CreateKey("RotInvZ", 'T');
	GET_SINGLE(CInput)->CreateKey("RotZ", 'G');

	GET_SINGLE(CInput)->CreateKey("ShieldOn", VK_F1);
	GET_SINGLE(CInput)->CreateKey("ShieldOff", VK_F2);
	GET_SINGLE(CInput)->CreateKey("ShieldRangeOn", VK_F3);
	GET_SINGLE(CInput)->CreateKey("ShieldRangeOff", VK_F4);

	return true;
}

void CPlayer::Input(float fTime)
{

}

int CPlayer::Update(float fTime)
{

	//_cprintf("x :  %f     z :  %f     angle :  %f\n", m_pTransform->GetWorldPos().x, m_pTransform->GetWorldPos().z, m_fRecentRot);

	return 0;
}

int CPlayer::LateUpdate(float fTime)
{
	return 0;
}

CPlayer * CPlayer::Clone()
{
	return new CPlayer(*this);
}

void CPlayer::OnCollisionEnter(CCollider * pSrc, CCollider * pDest, float fTime)
{

}

void CPlayer::OnCollision(CCollider * pSrc, CCollider * pDest, float fTime)
{

}

void CPlayer::OnCollisionLeave(CCollider * pSrc, CCollider * pDest, float fTime)
{

}
