#include "..\Client.h"
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
#include "Core/NaviManager.h"
#include "../TargetPlayerUI.h"
#include "../UserInterfaceManager.h"
#include "Component/Animation.h"
#include "Component/AnimationClip.h"
#include "Core/EffectManager.h"
#include "../NetworkManager.h"
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

	m_pAnimation->Load("99.Dynamic_Mesh\\00.Player\\Tanker\\Tanker.anm");
	m_pAnimation->SetDefaultClip("Idle1");
#ifdef _BOEM_
	GET_SINGLE(CInput)->CreateKey("RotInvY", 'Q');
	GET_SINGLE(CInput)->CreateKey("RotY", 'E');

	GET_SINGLE(CInput)->CreateKey("RotInvZ", 'T');
	GET_SINGLE(CInput)->CreateKey("RotZ", 'G');

	GET_SINGLE(CInput)->CreateKey("ShieldOn", VK_F1);
	GET_SINGLE(CInput)->CreateKey("ShieldOff", VK_F2);
	GET_SINGLE(CInput)->CreateKey("ShieldRangeOn", VK_F3);
	GET_SINGLE(CInput)->CreateKey("ShieldRangeOff", VK_F4);
#endif
	return true;
}

void CPlayer::Input(float fTime)
{
#ifdef _BOEM_
	if (!m_pArm)
	{
		CGameObject*	pCam = m_pScene->GetMainCameraObj();

		m_pArm = pCam->FindComponentFromType<CArm>(CT_ARM);

		SAFE_RELEASE(pCam);
	}

	/*if (!m_pThirdCamera)
	{
		CGameObject*	pCam = m_pScene->GetMainCameraObj();

		m_pThirdCamera = pCam->FindComponentFromType<CThirdCamera>(CT_THIRDCAMERA);

		SAFE_RELEASE(pCam);
	}*/

	if (KEYPUSH("RotY"))
	{
		m_pTransform->RotateWorldY(m_fRotSpeed, fTime);
		m_fRecentRot += m_fRotSpeed * fTime;
		//m_pArm->RotationY(m_fRotSpeed * fTime);
	}

	if (KEYPUSH("RotInvY"))
	{
		m_pTransform->RotateWorldY(-m_fRotSpeed, fTime);
		m_fRecentRot += -m_fRotSpeed * fTime;
		//m_pArm->RotationY(-m_fRotSpeed * fTime);
	}

	/*if (KEYPUSH("RotX"))
	{
		m_pTransform->RotateWorldX(m_fRotSpeed, fTime);
	}

	if (KEYPUSH("RotInvX"))
	{
		m_pTransform->RotateWorldX(-m_fRotSpeed, fTime);
	}

	if (KEYPUSH("RotZ"))
	{
		m_pTransform->RotateWorldZ(m_fRotSpeed, fTime);
	}

	if (KEYPUSH("RotInvZ"))
	{
		m_pTransform->RotateWorldZ(-m_fRotSpeed, fTime);
	}*/

	if (KEYPUSH("MoveFront"))
	{
		/*
		Vector3	vPos = m_pTarget->GetWorldPos() + m_vLookAtDist + m_vDist;

		m_pTransform->SetWorldPos(vPos);
		*/
	
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

		CTransform* pLightTransform = m_pScene->GetLightCameraTr();
		Vector3 LightPos = m_pTransform->GetWorldPos();
		LightPos = LightPos + Vector3{ -15, 30, -15 };
		//LightPos = Vector3{ 256.f / 2.f, 100.f, 256.f / 2.f };
		pLightTransform->SetWorldPos(LightPos);
		//pLightTransform->MoveWorld(Vector3{ 1, 0, 1 }, m_fMoveSpeed * 2.f, fTime);
		m_pAnimation->ChangeClip("Run");
	}

	if (KEYUP("MoveFront"))
	{
		m_pAnimation->ReturnDefaultClip();
		//m_pAnimation->ChangeClip("Idle");
	}

	if (KEYPUSH("MoveBack"))
	{
		//CTransform* pLightTransform = m_pScene->GetLightCameraTr();
		//pLightTransform->MoveWorld(Vector3{ -1, 0, -1 }, m_fMoveSpeed * 2.f, fTime);

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

		CTransform* pLightTransform = m_pScene->GetLightCameraTr();
		Vector3 LightPos = m_pTransform->GetWorldPos();
		LightPos = LightPos + Vector3{ -15, 30, -15 };
		//LightPos = Vector3{ 256.f / 2.f, 100.f, 256.f / 2.f };
		pLightTransform->SetWorldPos(LightPos);
		//pLightTransform->MoveWorld(Vector3{ 1, 0, 1 }, m_fMoveSpeed * 2.f, fTime);
		m_pAnimation->ChangeClip("Run");
	}

	if (KEYUP("MoveBack"))
	{
		m_pAnimation->ReturnDefaultClip();
		//m_pAnimation->ChangeClip("Idle");
	}

	//if (KEYDOWN("ShieldOn"))
	//{
	//	m_bOnShield = true;

	//	if (!m_pShield[0])
	//	{
	//		float	fAngle = PG_PI * 0.666666f;
	//		for (int i = 0; i < 3; ++i)
	//		{
	//			m_pShield[i] = CGameObject::CreateObject("Shield", m_pLayer);

	//			CTransform*	pShieldTr = m_pShield[i]->GetTransform();

	//			pShieldTr->SetWorldScale(0.3f, 0.3f, 0.3f);

	//			SAFE_RELEASE(pShieldTr);

	//			CRenderer*	pRenderer = m_pShield[i]->AddComponent<CRenderer>("ShieldRenderer");

	//			pRenderer->SetMesh("Pyramid");
	//			pRenderer->SetShader(STANDARD_COLOR_NORMAL_SHADER);
	//			pRenderer->SetInputLayout("VertexColorNormal");

	//			CMaterial* pMaterial = pRenderer->CreateMaterial();

	//			SAFE_RELEASE(pMaterial);

	//			SAFE_RELEASE(pRenderer);

	//			CColliderAABB*	pAABB = m_pShield[i]->AddComponent<CColliderAABB>("ShieldBody");

	//			pAABB->SetAABB(Vector3(-0.25f, -0.25f, -0.25f), Vector3(0.25f, 0.25f, 0.25f));

	//			SAFE_RELEASE(pAABB);

	//			CArm*	pArm = m_pShield[i]->AddComponent<CArm>("ShieldArm");

	//			pArm->SetTarget(m_pTransform);
	//			pArm->SetDist(Vector3(0.f, 0.f, -2.f));
	//			pArm->SetLookAtDist(Vector3(0.f, 0.f, 0.f));
	//			pArm->SetMinDist(2.f);
	//			pArm->SetMaxDist(5.f);
	//			pArm->RotationY(i * fAngle);

	//			SAFE_RELEASE(pArm);

	//			CShield*	pShield = m_pShield[i]->AddComponent<CShield>("Shield");

	//			SAFE_RELEASE(pShield);
	//		}
	//	}

	//	else
	//	{
	//		for (int i = 0; i < 3; ++i)
	//		{
	//			m_pShield[i]->Enable(true);
	//		}
	//	}
	//}

	//if (KEYDOWN("ShieldOff"))
	//{
	//	m_bOnShield = false;
#endif
}

int CPlayer::Update(float fTime)
{

	//_cprintf("x :  %f     z :  %f     angle :  %f\n", m_pTransform->GetWorldPos().x, m_pTransform->GetWorldPos().z, m_fRecentRot);
	int id = NetworkManager::getInstance()->getMyClientID();
	string playerTag = "Player" + to_string(id);

	if (this->m_pGameObject->GetTag() == playerTag)
	{
		//ANIMATION3DCLIP clipInfo = m_pAnimation->GetCurrentClip()->GetClipInfo();

		//if ("Attack1" == clipInfo.strName)
		//{
		//	int iCurrentFrame = m_pAnimation->GetClipFrame();

		//	if (18 == iCurrentFrame)
		//	{
		//		Vector3 vPos = m_pTransform->GetWorldPos();
		//		Vector3 vLook = m_pTransform->GetWorldAxis(AXIS_Z).Normalize();
		//		vPos += vLook * 1.75f;
		//		vPos.y += 0.75f;
		//		GET_SINGLE(CEffectManager)->OperateEffect("Attack3", nullptr, vPos);
		//		_cprintf("effect!\n");
		//	}
		//}
		
		ANIMATION3DCLIP clipInfo = m_pAnimation->GetCurrentClip()->GetClipInfo();

		if ("Attack1" == clipInfo.strName)
		{
			int iCurrentFrame = m_pAnimation->GetClipFrame();

			//if (18 == iCurrentFrame)
			if(16 == iCurrentFrame)
			{
				cs_packet_attack_skill_player* packet = reinterpret_cast<cs_packet_attack_skill_player*>(NetworkManager::getInstance()->getSendBuffer());
				packet->size = sizeof(cs_packet_attack_skill_player);
				packet->type = CS_PACKET_ATTACK_SKILL1_EFFECT;
				packet->playerID = id;
				wstring effectName = L"Attack3";
				wcscpy_s(packet->effect_name, effectName.c_str());
				DWORD iobyte;
				NetworkManager::getInstance()->getSendWsaBuf().len = sizeof(cs_packet_attack_skill_player);
				int ret = WSASend(NetworkManager::getInstance()->getSocket(), &NetworkManager::getInstance()->getSendWsaBuf(), 1, &iobyte, 0, NULL, NULL);
			}
		}

	}
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

void CPlayer::addExp(int exp)
{
	this->EXP += exp;
	// 인터페이스에 보여준다.

}

void CPlayer::OnCollisionEnter(CCollider * pSrc, CCollider * pDest, float fTime)
{

}

void CPlayer::OnCollision(CCollider * pSrc, CCollider * pDest, float fTime)
{
	if (KEYDOWN("MouseRButton"))
	{
		if ((pSrc->GetTag() == "MouseRay" || pDest->GetTag() == "MouseRay") && GetAsyncKeyState(VK_RBUTTON) & 0x8000)
		{
			// UI를 띄운다.
			TargetPlayerUI* pTargetPlayerUI = GET_SINGLE(UserInterfaceManager)->getTargetPlayerUI();
			CTransform* pTargetPlayerUITransform = pTargetPlayerUI->GetTransform();
			POINT pos = GET_SINGLE(CInput)->GetMousePos();
			pTargetPlayerUITransform->SetWorldPos(Vector3(pos.x, pos.y, 0));
			pTargetPlayerUI->enableRender(true);

			string tag = m_pGameObject->GetTag();
			tag.erase(0, 6);
			int id = atoi(tag.c_str());
			pTargetPlayerUI->setPlayerNumber(id);
			SAFE_RELEASE(pTargetPlayerUITransform);
		}
	}
}

void CPlayer::OnCollisionLeave(CCollider * pSrc, CCollider * pDest, float fTime)
{
	
}
