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
#include "Component/Camera.h"
#include "Core/SoundManager.h"


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

	GET_SINGLE(CInput)->CreateKey("MoveLeft", 'A');
	GET_SINGLE(CInput)->CreateKey("MoveRight", 'D');

	GET_SINGLE(CInput)->CreateKey("Jump", VK_SPACE);

	GET_SINGLE(CInput)->CreateKey("ShieldOn", VK_F1);
	GET_SINGLE(CInput)->CreateKey("ShieldOff", VK_F2);
	GET_SINGLE(CInput)->CreateKey("ShieldRangeOn", VK_F3);
	GET_SINGLE(CInput)->CreateKey("ShieldRangeOff", VK_F4);

	GET_SINGLE(CInput)->CreateKey("Attack1", '1');
	GET_SINGLE(CInput)->CreateKey("Attack2", '2');
	GET_SINGLE(CInput)->CreateKey("Attack3", '3');

	/* Effect */
	// Common
	GET_SINGLE(CEffectManager)->AddEffect("Level_Up", "Effect\\Player\\Level_Up.bin");
	
	// Spell
	GET_SINGLE(CEffectManager)->AddEffect("Spell_Attack_ManySwings", 
		"Effect\\Player\\Tanker\\Spell_Attack_ManySwings.bin");
	GET_SINGLE(CEffectManager)->AddEffect("Spell_Attack_RollingBlade",
		"Effect\\Player\\Tanker\\Spell_Attack_RollingBlade.bin");
	GET_SINGLE(CEffectManager)->AddEffect("Spell_Buff_FireCircle",
		"Effect\\Player\\Tanker\\Spell_Buff_FireCircle.bin");
	GET_SINGLE(CEffectManager)->AddEffect("Holy_Light",
		"Effect\\Common\\Holy_Light.bin");

	return true;
}

void CPlayer_Test::Input(float fTime)
{
#ifdef _BOEM_

	Vector3 vDir = Vector3(0.f, 0.f, 0.f);
	Vector3 vDir_Z = m_pTransform->GetWorldAxis(AXIS_Z).Normalize();
	Vector3 vDir_X = m_pTransform->GetWorldAxis(AXIS_X).Normalize();

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
		vDir = vDir_Z.Normalize();

		m_pAnimation->ChangeClip("Run1");
	}

	if (KEYUP("MoveFront"))
	{
		m_pAnimation->ReturnDefaultClip();
	}

	if (KEYPUSH("MoveBack"))
	{
		vDir = (vDir - vDir_Z).Normalize();
		m_pAnimation->ChangeClip("RunBack1");
	}

	if (KEYUP("MoveBack"))
	{
		m_pAnimation->ReturnDefaultClip();
	}

	if (KEYPUSH("MoveLeft"))
	{
		vDir = (vDir - vDir_X).Normalize();
		m_pAnimation->ChangeClip("RunLeft");
	}

	if (KEYUP("MoveLeft"))
	{
		m_pAnimation->ReturnDefaultClip();
	}

	if (KEYPUSH("MoveRight"))
	{
		vDir = (vDir + vDir_X).Normalize();
		m_pAnimation->ChangeClip("RunRight");
	}

	if (KEYUP("MoveRight"))
	{
		m_pAnimation->ReturnDefaultClip();
	}

	if (KEYDOWN("Jump"))
	{
		//GET_SINGLE(SoundManager)->ChangeSound_Smooth("FieldBGM");
		m_pAnimation->ChangeClip("Jump");
	}

	if (KEYDOWN("Attack1"))
	{
		Vector3 vPos = Vector3(0.f, 0.f, 0.f);
		//Vector3 vLook = m_pTransform->GetWorldAxis(AXIS_Z).Normalize();
		//vPos += vLook * 1.75f;
		//vPos.y += 0.75f;
		//GET_SINGLE(CEffectManager)->OperateEffect("Hit", nullptr, vPos);
		GET_SINGLE(CEffectManager)->OperateEffect("Spell_Attack_RollingBlade", m_pGameObject, vPos);
		m_pAnimation->ChangeClip("Spell1");
	}

	if (KEYDOWN("Attack2"))
	{
		Vector3 vPos = Vector3(0.f, 0.f, 0.f);
		//Vector3 vLook = m_pTransform->GetWorldAxis(AXIS_Z).Normalize();
		//vPos += vLook * 1.75f;
		//vPos.y += 0.75f;
		//GET_SINGLE(CEffectManager)->OperateEffect("Critical", nullptr, vPos);
		GET_SINGLE(CEffectManager)->OperateEffect("Spell_Attack_ManySwings", m_pGameObject, vPos);
		m_pAnimation->ChangeClip("Spell2");
	}

	if (KEYDOWN("Attack3"))
	{
		Vector3 vPos = m_pTransform->GetWorldPos();
		GET_SINGLE(CEffectManager)->OperateEffect("Spell_Buff_FireCircle", m_pGameObject, Vector3(0.f, 0.f, 0.f),
			false, true);
		GET_SINGLE(CEffectManager)->OperateEffect("Holy_Light", m_pGameObject, vPos, true, false);
		m_pAnimation->ChangeClip("Spell");
	}

	if (KEYPUSH("MouseRButton"))
	{
		// Cam 방향으로 회전
		CCamera *pMainCam = m_pScene->GetMainCamera();
		CTransform *pCamTr = pMainCam->GetTransform();
		Vector3 vCamPos = pCamTr->GetWorldPos();
		Vector3 vPos = m_pTransform->GetWorldPos();
		vCamPos.y = 0.f;
		vPos.y = 0.f;

		Vector3 vCamAxisZ = (vPos - vCamPos).Normalize();

		m_pTransform->LookAt(m_pTransform->GetWorldPos() + vCamAxisZ * 1.5f);
	}

	//
	{
		Vector3 vPos = m_pTransform->GetWorldPos();
		Vector3 vDirection = vDir;

		vPos += vDir * m_fMoveSpeed * 2.f * fTime;

		if (GET_SINGLE(CNaviManager)->GetNaviCells() == nullptr)
		{
			float fPosY = GET_SINGLE(CQuadTreeManager)->GetY(vPos);
			vPos.y = fPosY;
		}
		else
		{
			if (GET_SINGLE(CNaviManager)->CheckPosition(vPos, &vDir))
			{
				float fPosY = GET_SINGLE(CQuadTreeManager)->GetY(vPos);
				vPos.y = fPosY;
			}
			else
			{
				Vector3 vOpposite = -vDirection;
				Vector3 vSlide = vDir * vOpposite.Dot(vDir);
				vSlide = (vDir + vSlide).Normalize();

				vPos += (vSlide * m_fMoveSpeed * 2.f * fTime);
			}
		}

		m_pTransform->SetWorldPos(vPos);
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
			//_cprintf("effect!\n");
		}
	}

	Vector3 vPos = m_pTransform->GetWorldPos();
	m_pScene->GetLightCamera()->SetLightCenterPosToObject(m_pGameObject);

	

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
