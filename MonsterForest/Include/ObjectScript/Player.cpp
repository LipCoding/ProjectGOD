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

CPlayer::CPlayer(const CPlayer & player)	:
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
	SAFE_RELEASE(m_pThirdCamera);
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
	//m_pTransform->SetLocalRotY(-PG_PI / 2.f);
	//m_pTransform->SetLocalRotX(PG_PI);
	//m_pTransform->SetLocalScale(2.f, 2.f, 2.f);
	//m_pTransform->SetLocalRotY(PG_PI);
	//m_pTransform->SetPivot(0.f, -0.55f, 0.f);
	//m_pTransform->SetLocalPos(0.f, 0.5f, 0.f);
	//m_pTransform->SetOriginAxis(AXIS_Y);
	//m_pTransform->SetWorldScale(0.05f, 0.05f, 0.05f);
	//m_pTransform->SetWorldScale(0.05f, 0.05f, 0.05f);
	//m_pTransform->SetWorldScale(0.0005f, 0.0005f, 0.0005f);
	m_fMoveSpeed = 5.f;
	m_fRotSpeed = PG_PI;
	m_fRecentRot = 0.f;

	m_pAnimation = m_pGameObject->AddComponent<CAnimation>("PlayerAnimation");

	m_pAnimation->Load("99.Dynamic_Mesh\\00.Player\\Player.anm");
	m_pAnimation->SetDefaultClip("Idle");

	//m_pAnimation->AddSocket("RightHand", "Gun");
	//m_pAnimation->AddSocket("mixamorig:RightHand", "Sword");

	// Sword 추가

	//m_pNavigation = m_pGameObject->AddComponent<CNavigation3D>("Navigation");


	////무기
	//CGameObject*    pSwordObj = CGameObject::CreateObject("Sword", m_pLayer);
	//CTransform*     pSwordTr = pSwordObj->GetTransform();

	//pSwordTr->SetPivot(0.f, 0.f, 0.f);
	//pSwordTr->SetLocalScale(Vector3(0.05f, 0.05f, 0.05f));

	////pSwordTr->SetWorldPos(Vector3(66.f, 0.f, 170.f));

	//SAFE_RELEASE(pSwordTr);

	//CRenderer* pSwordRenderer = pSwordObj->AddComponent<CRenderer>("SwordRenderer");
	////CNavigation3D* pSwordNavigation = pSwordObj->AddComponent<CNavigation3D>("Navigation");

	//pSwordRenderer->SetMesh("Sword", L"Sword.FBX");

	//m_pAnimation->SetSocketTarget("mixamorig:RightHand", "Sword", pSwordObj);

	//CGun*	pGun = pSwordObj->AddComponent<CGun>("Sword");

	//SAFE_RELEASE(pGun);

	//SAFE_RELEASE(pSwordRenderer);
	////SAFE_RELEASE(pSwordNavigation);
	//SAFE_RELEASE(pSwordObj);
	// 무기
	//CGameObject*	pGunObj = CGameObject::CreateObject("Gun", m_pLayer);

	//CTransform* pGunTr = pGunObj->GetTransform();

	//pGunTr->SetPivot(0.f, 0.f, 0.f);
	//pGunTr->SetWorldScale(0.007f, 0.007f, 0.007f);
	////pGunTr->SetLocalRotY(PG_PI);

	//SAFE_RELEASE(pGunTr);

	//CRenderer* pGunRenderer = pGunObj->AddComponent<CRenderer>("GunRenderer");

	//pGunRenderer->SetMesh("AM110M", L"AM110M.msh");

	//SAFE_RELEASE(pGunRenderer);

	//CGun*	pGun = pGunObj->AddComponent<CGun>("Gun");

	//SAFE_RELEASE(pGun);

	//m_pAnimation->SetSocketTarget("RightHand", "Gun", pGunObj);

	//SAFE_RELEASE(pGunObj);

	/*m_pAnimation->ModifyClip("Take 001", "Walk", AO_ONCE_RETURN,
		0, 100);
	m_pAnimation->AddClip("Idle", AO_LOOP, 101, 200);
	m_pAnimation->AddClip("Attack", AO_ONCE_RETURN, 201, 300);
	m_pAnimation->AddClip("Die", AO_ONCE_RETURN, 301, 400);
	m_pAnimation->SetDefaultClip("Idle");
	m_pAnimation->SetCurrentClip("Walk");
	m_pAnimation->ChangeClip("Idle");*/

	GET_SINGLE(CInput)->CreateKey("RotInvY", 'Q');
	GET_SINGLE(CInput)->CreateKey("RotY", 'E');

	GET_SINGLE(CInput)->CreateKey("RotInvZ", 'T');
	GET_SINGLE(CInput)->CreateKey("RotZ", 'G');

	GET_SINGLE(CInput)->CreateKey("ShieldOn", VK_F1);
	GET_SINGLE(CInput)->CreateKey("ShieldOff", VK_F2);
	GET_SINGLE(CInput)->CreateKey("ShieldRangeOn", VK_F3);
	GET_SINGLE(CInput)->CreateKey("ShieldRangeOff", VK_F4);


	// Child 추가
	/*CGameObject*	pChild = CGameObject::CreateObject("Child");

	CTransform*	pTr = pChild->GetTransform();

	pTr->SetParentModifyType(PMT_POS | PMT_ROT);
	pTr->SetWorldPos(1.f, 0.f, 0.f);
	pTr->SetWorldScale(0.5f, 0.5f, 0.5f);

	SAFE_RELEASE(pTr);

	CRenderer*	pRenderer = pChild->AddComponent<CRenderer>("ChildRenderer");

	pRenderer->SetMesh("Pyramid");
	pRenderer->SetShader(STANDARD_COLOR_NORMAL_SHADER);
	pRenderer->SetInputLayout("VertexColorNormal");

	CMaterial* pMaterial = pRenderer->CreateMaterial();

	SAFE_RELEASE(pMaterial);

	SAFE_RELEASE(pRenderer);

	CColliderAABB*	pAABB = pChild->AddComponent<CColliderAABB>("ChildBody");

	pAABB->SetAABB(Vector3(-0.25f, -0.25f, -0.25f), Vector3(0.25f, 0.25f, 0.25f));

	SAFE_RELEASE(pAABB);

	m_pGameObject->AddChild(pChild);

	CGameObject*	pChildChild = CGameObject::CreateObject("ChildChild");

	pTr = pChildChild->GetTransform();

	pTr->SetParentModifyType(PMT_POS | PMT_ROT);
	pTr->SetWorldPos(1.f, 0.f, 0.f);
	pTr->SetWorldScale(0.5f, 0.5f, 0.5f);

	SAFE_RELEASE(pTr);

	pRenderer = pChildChild->AddComponent<CRenderer>("ChildRenderer");

	pRenderer->SetMesh("Pyramid");
	pRenderer->SetShader(STANDARD_COLOR_NORMAL_SHADER);
	pRenderer->SetInputLayout("VertexColorNormal");

	pMaterial = pRenderer->CreateMaterial();

	SAFE_RELEASE(pMaterial);

	SAFE_RELEASE(pRenderer);

	pAABB = pChildChild->AddComponent<CColliderAABB>("ChildChildBody");

	pAABB->SetAABB(Vector3(-0.25f, -0.25f, -0.25f), Vector3(0.25f, 0.25f, 0.25f));

	SAFE_RELEASE(pAABB);

	pChild->AddChild(pChildChild);

	SAFE_RELEASE(pChildChild);

	SAFE_RELEASE(pChild);*/

	m_pArm = NULL;
	m_pThirdCamera = NULL;

	return true;
}

void CPlayer::Input(float fTime)
{
	if (!m_pArm)
	{
		CGameObject*	pCam = m_pScene->GetMainCameraObj();

		m_pArm = pCam->FindComponentFromType<CArm>(CT_ARM);

		SAFE_RELEASE(pCam);
	}

	if (!m_pThirdCamera)
	{
		CGameObject*	pCam = m_pScene->GetMainCameraObj();

		m_pThirdCamera = pCam->FindComponentFromType<CThirdCamera>(CT_THIRDCAMERA);

		SAFE_RELEASE(pCam);
	}

	if (KEYPUSH("RotY"))
	{
		m_pTransform->RotateWorldY(m_fRotSpeed, fTime);
		m_fRecentRot += m_fRotSpeed * fTime;
		m_pArm->RotationY(m_fRotSpeed * fTime);
	}

	if (KEYPUSH("RotInvY"))
	{
		m_pTransform->RotateWorldY(-m_fRotSpeed, fTime);
		m_fRecentRot += -m_fRotSpeed * fTime;
		m_pArm->RotationY(-m_fRotSpeed * fTime);
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
		
		m_pTransform->MoveWorld(AXIS_Z, m_fMoveSpeed * 2.f, fTime);
		Vector3 vPos = m_pTransform->GetWorldPos();

		float fPosY = GET_SINGLE(CQuadTreeManager)->GetY(vPos);
		vPos.y = fPosY;

		m_pTransform->SetWorldPos(vPos);

		CTransform* pLightTransform = m_pScene->GetLightCameraTr();
		Vector3 LightPos = m_pTransform->GetWorldPos();
		LightPos = LightPos + Vector3{ -15, 15, -15 };
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
		m_pTransform->MoveWorld(AXIS_Z, -m_fMoveSpeed, fTime);

		Vector3 vPos = m_pTransform->GetWorldPos();

		float fPosY = GET_SINGLE(CQuadTreeManager)->GetY(vPos);
		vPos.y = fPosY;

		m_pTransform->SetWorldPos(vPos);

		CTransform* pLightTransform = m_pScene->GetLightCameraTr();
		Vector3 LightPos = m_pTransform->GetWorldPos();
		LightPos = LightPos + Vector3{ -15, 15, -15 };
		pLightTransform->SetWorldPos(LightPos);
		m_pAnimation->ChangeClip("Run");
	}

	if (KEYUP("MoveBack"))
	{
		m_pAnimation->ReturnDefaultClip();
		//m_pAnimation->ChangeClip("Idle");
	}

	if (KEYDOWN("ShieldOn"))
	{
		m_bOnShield = true;

		if (!m_pShield[0])
		{
			float	fAngle = PG_PI * 0.666666f;
			for (int i = 0; i < 3; ++i)
			{
				m_pShield[i] = CGameObject::CreateObject("Shield", m_pLayer);

				CTransform*	pShieldTr = m_pShield[i]->GetTransform();

				pShieldTr->SetWorldScale(0.3f, 0.3f, 0.3f);

				SAFE_RELEASE(pShieldTr);

				CRenderer*	pRenderer = m_pShield[i]->AddComponent<CRenderer>("ShieldRenderer");

				pRenderer->SetMesh("Pyramid");
				pRenderer->SetShader(STANDARD_COLOR_NORMAL_SHADER);
				pRenderer->SetInputLayout("VertexColorNormal");

				CMaterial* pMaterial = pRenderer->CreateMaterial();

				SAFE_RELEASE(pMaterial);

				SAFE_RELEASE(pRenderer);

				CColliderAABB*	pAABB = m_pShield[i]->AddComponent<CColliderAABB>("ShieldBody");

				pAABB->SetAABB(Vector3(-0.25f, -0.25f, -0.25f), Vector3(0.25f, 0.25f, 0.25f));

				SAFE_RELEASE(pAABB);

				CArm*	pArm = m_pShield[i]->AddComponent<CArm>("ShieldArm");

				pArm->SetTarget(m_pTransform);
				pArm->SetDist(Vector3(0.f, 0.f, -2.f));
				pArm->SetLookAtDist(Vector3(0.f, 0.f, 0.f));
				pArm->SetMinDist(2.f);
				pArm->SetMaxDist(5.f);
				pArm->RotationY(i * fAngle);

				SAFE_RELEASE(pArm);

				CShield*	pShield = m_pShield[i]->AddComponent<CShield>("Shield");

				SAFE_RELEASE(pShield);
			}
		}

		else
		{
			for (int i = 0; i < 3; ++i)
			{
				m_pShield[i]->Enable(true);
			}
		}
	}

	if (KEYDOWN("ShieldOff"))
	{
		m_bOnShield = false;

		if (m_pShield[0])
		{
			for (int i = 0; i < 3; ++i)
			{
				m_pShield[i]->Enable(false);
			}
		}
	}

	if (KEYDOWN("ShieldRangeOn") && !m_bOnShieldRange)
	{
		if (m_pShield[0] && m_pShield[0]->GetEnable())
		{
			m_bOnShieldRange = true;
			for (int i = 0; i < 3; ++i)
			{
				//m_pShield[i]->Enable(false);
				CArm*	pArm = m_pShield[i]->FindComponentFromType<CArm>(CT_ARM);

				CTransform*	pTransform = m_pShield[i]->GetTransform();

				pArm->Zoom(pTransform->GetWorldAxis(AXIS_Z), -3.f);

				SAFE_RELEASE(pTransform);

				SAFE_RELEASE(pArm);
			}
		}
	}

	if (KEYDOWN("ShieldRangeOff") && m_bOnShieldRange)
	{
		if (m_pShield[0] && m_pShield[0]->GetEnable())
		{
			m_bOnShieldRange = false;
			for (int i = 0; i < 3; ++i)
			{
				//m_pShield[i]->Enable(false);
				CArm*	pArm = m_pShield[i]->FindComponentFromType<CArm>(CT_ARM);

				CTransform*	pTransform = m_pShield[i]->GetTransform();

				pArm->Zoom(pTransform->GetWorldAxis(AXIS_Z), 3.f);

				SAFE_RELEASE(pTransform);

				SAFE_RELEASE(pArm);
			}
		}
	}

	if (KEYDOWN("Attack"))
	{
		//m_pAnimation->ChangeClip("Fire1");
		m_pAnimation->ChangeClip("Attack_S");

//#pragma region Effect
//		char str[64];
//		static int count = 0;
//		itoa(count, str, 10);
//		string appendTag = str;
//		string objectTag = "ParticleObj" + appendTag;
//		// 클래스 따로 만드는게 나을듯
//		CLayer*	pParticleLayer = m_pScene->GetLayer("ParticleLayer");
//
//
//		CGameObject*	pParticleObj = CGameObject::CreateObject(
//			objectTag, pParticleLayer);
//
//		CTransform*	pParticleTr = pParticleObj->GetTransform();
//		/*Vector3 monster_pos = m_pTransform->GetWorldPos();
//		Vector3 effect_dir = (m_pTarget->GetTransform()->GetWorldPos() - monster_pos).Normalize();
//
//		monster_pos += effect_dir * 0.6f;
//
//		monster_pos.y += 1.f;*/
//		pParticleTr->SetWorldPos(m_pTransform->GetWorldPos());
//
//		SAFE_RELEASE(pParticleTr);
//
//		Effect*	pEffect = pParticleObj->AddComponent<Effect>("ParticleObj");
//
//		pEffect->SetGameObject(pParticleObj);
//
//		CParticleSingle*	pParticleSingle = pParticleObj->AddComponent<CParticleSingle>("ParticleSingle");
//
//		pParticleSingle->SetSize(0.f, 0.f);
//		pEffect->SetParticleSingle(pParticleSingle);
//
//		SAFE_RELEASE(pParticleSingle);
//
//		CRenderer*	pParticleRenderer = pParticleObj->FindComponentFromType<CRenderer>(CT_RENDERER);
//
//		pEffect->Init_Other();
//		SAFE_RELEASE(pEffect);
//		/*pMaterial = pParticleRenderer->GetMaterial();
//
//		pMaterial->SetDiffuseTex()
//
//		SAFE_RELEASE(pMaterial);*/
//
//		pParticleRenderer->CreateCBuffer("Animation2D", 10, sizeof(ANIMATION2DBUFFER),
//			SCT_VERTEX | SCT_PIXEL);
//
//		CAnimation2D*	pParticleAnimation = pParticleObj->AddComponent<CAnimation2D>("ParticleAnimation");
//
//		pParticleAnimation->SetRenderer2DEnable(false);
//
//		vector<wstring>	vecExplosion;
//
//		for (int i = 1; i <= 1; ++i)
//		{
//			wchar_t	strPath[MAX_PATH] = {};
//			wsprintf(strPath, L"Hit/Hit%d.png", i);
//
//			vecExplosion.push_back(strPath);
//		}
//
//		pParticleAnimation->CreateClip("Explosion", A2D_FRAME, A2DO_LOOP,
//			1, 1, 1, 1, 0, 0.2f, 0, 0.f, "Explosion",
//			&vecExplosion);
//
//
//		SAFE_RELEASE(pParticleAnimation);
//
//		pParticleRenderer->SetRenderState(ALPHA_BLEND);
//
//		SAFE_RELEASE(pParticleRenderer);
//
//		SAFE_RELEASE(pParticleObj);
//
//#pragma endregion
	}

	if (KEYDOWN("MouseRButton"))
	{
		//Vector3	vRayPos = GET_SINGLE(CInput)->GetRayPos();
		//Vector3 vRayDir = GET_SINGLE(CInput)->GetRayDir();
		//m_pNavigation->FindNavigation(m_pTransform->GetWorldPos(),
		//	vRayPos, vRayDir);
		//MessageBox(nullptr, L"aa", L"aa", MB_OK);
	}

	if (GetAsyncKeyState(VK_F8) & 0x8000)
	{
		m_pNavigation->FindNavigation(Vector3(0.5f, 0.f, 127.7f), Vector3(5.f, 0.f, 122.7f));
	}
}

int CPlayer::Update(float fTime)
{
	//_cprintf("x :  %f     z :  %f     angle :  %f\n", m_pTransform->GetWorldPos().x, m_pTransform->GetWorldPos().z, m_fRecentRot);

	// Camera Update
	if (m_pThirdCamera)
	{
		CGameObject* pPlayerObj = CGameObject::FindObject("PlayerCharacter");
		CRenderer* pRenderer = pPlayerObj->FindComponentFromTag<CRenderer>("PlayerRenderer");

		SHARECBUFFER tShareBuffer = {};
		tShareBuffer.vCameraPos = m_pThirdCamera->GetTransform()->GetWorldAxis(AXIS_Z);
		//_cprintf("x : %f, y : %f, z : %f\n", tShareBuffer.vCameraPos.x, tShareBuffer.vCameraPos.y, tShareBuffer.vCameraPos.z);
		tShareBuffer.vColor = Vector4{1.f, 0.f, 0.f, 1.f};
		tShareBuffer.fEmpty = 1.f;
		pRenderer->UpdateCBuffer("Share", 8, sizeof(SHARECBUFFER), SCT_VERTEX, &tShareBuffer);
		

		SAFE_RELEASE(pRenderer);
		SAFE_RELEASE(pPlayerObj);
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
