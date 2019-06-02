#include "Player_Test.h"
#include "..\Client.h"
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
#include "Core/NaviManager.h"
#include "Core/PathManager.h"
#include "Core/EffectManager.h"
#include "Component/Animation.h"
#include "Component/AnimationClip.h"
#include "Resources/ResourcesManager.h"
#include "Resources/Mesh.h"


CPlayer_Test::CPlayer_Test()
{
	SetTag("Player_Test");
	SetTypeName("CPlayer_Test");
	SetTypeID<CPlayer_Test>();
	m_bOnShield = false;
	m_bOnShieldRange = false;
	memset(m_pShield, 0, sizeof(CGameObject*) * 3);
	m_pNavigation = NULL;
}

CPlayer_Test::CPlayer_Test(const CPlayer_Test & player)
{
	m_fMoveSpeed = player.m_fMoveSpeed;
	m_fRotSpeed = player.m_fRotSpeed;
	m_bOnShield = false;
	m_bOnShieldRange = false;
	memset(m_pShield, 0, sizeof(CGameObject*) * 3);

	m_pNavigation = NULL;
}


CPlayer_Test::~CPlayer_Test()
{
	SAFE_RELEASE(m_pArm);
	SAFE_RELEASE(m_pNavigation);
	SAFE_RELEASE(m_pAnimation);
	for (int i = 0; i < 3; ++i)
	{
		SAFE_RELEASE(m_pShield[i]);
	}
}

bool CPlayer_Test::Init()
{
	m_fMoveSpeed = 5.f;
	m_fRotSpeed = PG_PI;
	m_fRecentRot = 0.f;

	CRenderer*	pRenderer = m_pGameObject->AddComponent<CRenderer>("PlayerRenderer");

	/* Mesh */
	pRenderer->SetMesh("Player", L"99.Dynamic_Mesh\\00.Player\\Tanker\\Tanker.msh");
	pRenderer->SetForwardShader();

	pRenderer->CreateCBuffer("Share", 8, sizeof(SHARECBUFFER), SCT_PIXEL);
	SHARECBUFFER tShareBuffer = {};

	tShareBuffer.vColor = Vector4{ 0.f, 0.f, 0.f, 1.f };
	tShareBuffer.fAlphaFadeIn = 0.f;
	tShareBuffer.vCameraPos = Vector3{ 0.f, 0.f, 0.f };

	pRenderer->UpdateCBuffer("Share", 8, sizeof(SHARECBUFFER), SCT_PIXEL, &tShareBuffer);

	string meshBasePath = GET_SINGLE(CPathManager)->FindPathToMultiByte(MESH_PATH);

	/* Animation */
	m_pAnimation = m_pGameObject->AddComponent<CAnimation>("PlayerAnimation");

	m_pAnimation->Load("99.Dynamic_Mesh\\00.Player\\Tanker\\Tanker.anm");
	m_pAnimation->SetDefaultClip("Idle2");
	m_pAnimation->ChangeClip("Idle2");

	/* Local */
	CTransform*	pTr = m_pGameObject->GetTransform();
	string transformPath = meshBasePath + "99.Dynamic_Mesh\\00.Player\\Tanker\\Tanker.dat";

	FILE* pFile_Player = nullptr;

	fopen_s(&pFile_Player, transformPath.c_str(), "rb");

	if (!pFile_Player)
		return false;

	pTr->Load_Local(pFile_Player);

	fclose(pFile_Player);

	/* Key */
	GET_SINGLE(CInput)->CreateKey("RotInvY", 'Q');
	GET_SINGLE(CInput)->CreateKey("RotY", 'E');

	GET_SINGLE(CInput)->CreateKey("RotInvZ", 'T');
	GET_SINGLE(CInput)->CreateKey("RotZ", 'G');

	GET_SINGLE(CInput)->CreateKey("ShieldOn", VK_F1);
	GET_SINGLE(CInput)->CreateKey("ShieldOff", VK_F2);
	GET_SINGLE(CInput)->CreateKey("ShieldRangeOn", VK_F3);
	GET_SINGLE(CInput)->CreateKey("ShieldRangeOff", VK_F4);

	GET_SINGLE(CInput)->CreateKey("Attack1", '1');
	GET_SINGLE(CInput)->CreateKey("Attack2", '2');
	GET_SINGLE(CInput)->CreateKey("Attack3", '3');

	/* Effect */
	/*GET_SINGLE(CEffectManager)->AddEffect("Hit", "Effect\\hit_test.bin");
	GET_SINGLE(CEffectManager)->AddEffect("Critical", "Effect\\critical_test.bin");
	GET_SINGLE(CEffectManager)->AddEffect("Rune", "Effect\\rune_test.bin");*/

	/*GET_SINGLE(CEffectManager)->AddEffect("Attack", "Effect\\Attack.bin");
	GET_SINGLE(CEffectManager)->AddEffect("Attack2", "Effect\\Attack2.bin");
	GET_SINGLE(CEffectManager)->AddEffect("Attack3", "Effect\\Attack3.bin");
	GET_SINGLE(CEffectManager)->AddEffect("Spell", "Effect\\Spell.bin");
	GET_SINGLE(CEffectManager)->AddEffect("Spell2", "Effect\\Spell2.bin");*/

	///* Effect */
	//GET_SINGLE(CEffectManager)->AddEffect("Attack", "Effect\\Attack.bin");
	//GET_SINGLE(CEffectManager)->AddEffect("Attack2", "Effect\\Attack2.bin");
	//GET_SINGLE(CEffectManager)->AddEffect("Attack3", "Effect\\Attack3.bin");
	////GET_SINGLE(CEffectManager)->AddEffect("Spell", "Effect\\Spell.bin");
	//GET_SINGLE(CEffectManager)->AddEffect("Spell1", "Effect\\Spell1.bin");
	//GET_SINGLE(CEffectManager)->AddEffect("Spell2", "Effect\\Spell2.bin");
	//GET_SINGLE(CEffectManager)->AddEffect("Spell3", "Effect\\Spell3.bin");
	//GET_SINGLE(CEffectManager)->AddEffect("Spell4", "Effect\\Spell4.bin");
	//GET_SINGLE(CEffectManager)->AddEffect("Level_Up", "Effect\\Level_Up.bin");
	return true;
}

void CPlayer_Test::Input(float fTime)
{
#ifdef _BOEM_
	if (!m_pArm)
	{
		CGameObject*	pCam = m_pScene->GetMainCameraObj();

		m_pArm = pCam->FindComponentFromType<CArm>(CT_ARM);

		SAFE_RELEASE(pCam);
	}

	if (KEYPUSH("RotY"))
	{
		m_pTransform->RotateWorldY(m_fRotSpeed, fTime);
		m_fRecentRot += m_fRotSpeed * fTime;
	}

	if (KEYPUSH("RotInvY"))
	{
		m_pTransform->RotateWorldY(-m_fRotSpeed, fTime);
		m_fRecentRot += -m_fRotSpeed * fTime;
	}

	if (KEYPUSH("MoveFront"))
	{
		Vector3 vPos = m_pTransform->GetWorldPos();
		Vector3 vdirection = m_pTransform->GetWorldAxis(AXIS_Z).Normalize();
		Vector3 vDir = m_pTransform->GetWorldAxis(AXIS_Z).Normalize();

		vPos += vDir * m_fMoveSpeed * 2.f * fTime;

		if (GET_SINGLE(CNaviManager)->GetNaviCells() == nullptr)
		{
			m_pTransform->MoveWorld(AXIS_Z, m_fMoveSpeed * 2.f, fTime);
			vPos = m_pTransform->GetWorldPos();
			float fPosY = GET_SINGLE(CQuadTreeManager)->GetY(vPos);
			vPos.y = fPosY;
		}
		else
		{
			if (GET_SINGLE(CNaviManager)->CheckPosition(vPos, &vDir))
			{
				m_pTransform->MoveWorld(AXIS_Z, m_fMoveSpeed * 2.f, fTime);
				vPos = m_pTransform->GetWorldPos();
				float fPosY = GET_SINGLE(CNaviManager)->GetY(vPos);
				vPos.y = fPosY;
			}
			else
			{
				Vector3 vOpposite = -vdirection;
				Vector3 vSlide = vDir * vOpposite.Dot(vDir);
				vSlide = vDir + vSlide;

				vPos = m_pTransform->GetWorldPos();
				vPos += (vSlide * m_fMoveSpeed * 2.f * fTime) / 2.f;

				//vPos = 
			}
		}

		m_pTransform->SetWorldPos(vPos);

		m_pAnimation->ChangeClip("Run1");
	}

	if (KEYUP("MoveFront"))
	{
		m_pAnimation->ReturnDefaultClip();
	}

	if (KEYPUSH("MoveBack"))
	{
		Vector3 vPos = m_pTransform->GetWorldPos();
		Vector3 vdirection = m_pTransform->GetWorldAxis(AXIS_Z).Normalize();
		Vector3 vDir = m_pTransform->GetWorldAxis(AXIS_Z).Normalize();

		vdirection = -vdirection;
		vDir = -vDir;
		vPos += vDir * -m_fMoveSpeed * 2.f * fTime;

		if (GET_SINGLE(CNaviManager)->GetNaviCells() == nullptr)
		{
			m_pTransform->MoveWorld(AXIS_Z, -m_fMoveSpeed * 2.f, fTime);
			vPos = m_pTransform->GetWorldPos();
			float fPosY = GET_SINGLE(CQuadTreeManager)->GetY(vPos);
			vPos.y = fPosY;
		}
		else
		{
			if (GET_SINGLE(CNaviManager)->CheckPosition(vPos, &vDir))
			{
				m_pTransform->MoveWorld(AXIS_Z, -m_fMoveSpeed * 2.f, fTime);
				vPos = m_pTransform->GetWorldPos();
				float fPosY = GET_SINGLE(CNaviManager)->GetY(vPos);
				vPos.y = fPosY;
			}
			else
			{
				Vector3 vOpposite = -vdirection;
				Vector3 vSlide = vDir * vOpposite.Dot(vDir);
				vSlide = vDir + vSlide;

				vPos = m_pTransform->GetWorldPos();
				vPos += (vSlide * m_fMoveSpeed * 2.f * fTime) / 2.f;

				//vPos = 
			}
		}

		m_pTransform->SetWorldPos(vPos);
		m_pAnimation->ChangeClip("Run1");
	}

	if (KEYUP("MoveBack"))
	{
		m_pAnimation->ReturnDefaultClip();
	}

	if (KEYDOWN("Attack1"))
	{
		Vector3 vPos = m_pTransform->GetWorldPos();
		//Vector3 vLook = m_pTransform->GetWorldAxis(AXIS_Z).Normalize();
		//vPos += vLook * 1.75f;
		//vPos.y += 0.75f;
		//GET_SINGLE(CEffectManager)->OperateEffect("Hit", nullptr, vPos);
		GET_SINGLE(CEffectManager)->OperateEffect("Spell1", nullptr, vPos);
		m_pAnimation->ChangeClip("Spell1");
	}

	if (KEYDOWN("Attack2"))
	{
		Vector3 vPos = m_pTransform->GetWorldPos();
		//Vector3 vLook = m_pTransform->GetWorldAxis(AXIS_Z).Normalize();
		//vPos += vLook * 1.75f;
		//vPos.y += 0.75f;
		//GET_SINGLE(CEffectManager)->OperateEffect("Critical", nullptr, vPos);
		GET_SINGLE(CEffectManager)->OperateEffect("Spell2", nullptr, vPos);
		m_pAnimation->ChangeClip("Spell2");
	}

	if (KEYDOWN("Attack3"))
	{
		Vector3 vPos = m_pTransform->GetWorldPos();
		GET_SINGLE(CEffectManager)->OperateEffect("Spell3", nullptr, vPos);
		m_pAnimation->ChangeClip("Spell");
	}
#endif
}

int CPlayer_Test::Update(float fTime)
{
	ANIMATION3DCLIP clipInfo = m_pAnimation->GetCurrentClip()->GetClipInfo();

	if("Attack1" == clipInfo.strName)
	{
		int iCurrentFrame = m_pAnimation->GetClipFrame();

		if (18 == iCurrentFrame)
		{
			Vector3 vPos = m_pTransform->GetWorldPos();
			Vector3 vLook = m_pTransform->GetWorldAxis(AXIS_Z).Normalize();
			vPos += vLook * 1.75f;
			vPos.y += 0.75f;
			GET_SINGLE(CEffectManager)->OperateEffect("Attack3", nullptr, vPos);
			_cprintf("effect!\n");
		}
	}


	return 0;
}

int CPlayer_Test::LateUpdate(float fTime)
{
	return 0;
}

CPlayer_Test * CPlayer_Test::Clone()
{
	return new CPlayer_Test(*this);
}

void CPlayer_Test::OnCollisionEnter(CCollider * pSrc, CCollider * pDest, float fTime)
{
}

void CPlayer_Test::OnCollision(CCollider * pSrc, CCollider * pDest, float fTime)
{
}

void CPlayer_Test::OnCollisionLeave(CCollider * pSrc, CCollider * pDest, float fTime)
{
}
