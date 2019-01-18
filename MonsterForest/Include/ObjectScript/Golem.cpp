#include "Golem.h"
#include "GameObject/GameObject.h"
#include "Scene/Scene.h"
#include "Component/Collider.h"
#include "Component/Camera.h"
#include "Device.h"
#include "Core/Input.h"
#include "GolemAttack.h"
#include "GolemIdle.h"
#include "GolemMove.h"
#include "GolemDie.h"
#include "Component/ParticleSingle.h"
#include "Component/Animation2D.h"
#include "Component/Navigation3D.h"
#include "../ObjectScript/Effect.h"
Golem::Golem()	:
	m_pTarget(NULL),
	m_pNavigation(NULL)
{
	SetTag("BigGoblin");
	SetTypeName("Golem");
	SetTypeID<Golem>();
}

Golem::Golem(const Golem & goblin)	:
	CScript(goblin)
{
	m_pTarget = NULL;
	m_pAnimation = NULL;
	m_pNavigation = NULL;

	GolemAttack* pAttackState = new GolemAttack;
	GolemIdle* pIdleState = new GolemIdle;
	GolemMove* pMoveState = new GolemMove;
	GolemDie* pDieState = new GolemDie;

	stateMap.insert(make_pair(GOLEM_STATE::GOLEM_STATE_ATTACK, GOLEMSTATE{ "ATTACK",pAttackState }));
	stateMap.insert(make_pair(GOLEM_STATE::GOLEM_STATE_IDLE, GOLEMSTATE{ "IDLE", pIdleState }));
	stateMap.insert(make_pair(GOLEM_STATE::GOLEM_STATE_MOVE, GOLEMSTATE{ "MOVE", pMoveState }));
	stateMap.insert(make_pair(GOLEM_STATE::GOLEM_STATE_DIE, GOLEMSTATE{ "DIE", pDieState }));

	pState = getState(GOLEM_STATE::GOLEM_STATE_IDLE);
	currentAnimation = GOLEM_STATE::GOLEM_STATE_IDLE;
	nextAnimation = GOLEM_STATE_NONE;
}

Golem::~Golem()
{
	SAFE_RELEASE(m_pNavigation);
	SAFE_RELEASE(m_pAnimation);
	SAFE_RELEASE(m_pTarget);
}

GolemState * Golem::getCurrentState()
{
	return this->pState;
}

GolemState * Golem::getState(int stateID)
{
	const auto& finder = stateMap.find(stateID);

	if (finder == stateMap.end())
		return nullptr;

	return finder->second.second;
}

size_t Golem::getCurrentAnimation()
{
	return currentAnimation;
}

size_t Golem::getNextAnimation()
{
	return nextAnimation;
}

void Golem::changeAnimation()
{
	m_pAnimation->ChangeClip(stateMap[nextAnimation].first);
}

void Golem::setCurrentAnimation(size_t currentAnimation)
{
	this->currentAnimation = currentAnimation;
}

void Golem::setNextAnimation(size_t nextAnimation)
{
	this->nextAnimation = nextAnimation;
}

void Golem::setCurrentState(GolemState* pState)
{
	this->pState = pState;
}

void Golem::SetTarget(const string & strTag)
{
	CGameObject*	pTargetObj = CGameObject::FindObject(strTag);

	if (pTargetObj)
	{
		m_pTarget = pTargetObj->GetTransform();
		SAFE_RELEASE(pTargetObj);
	}
}

CAnimation * Golem::GetAnimation()
{
	return m_pAnimation;
}

bool Golem::Init()
{
	m_pTransform->SetOriginAxis(AXIS_Y);
	m_pTransform->SetLocalRotY(-PG_PI);
	//m_pTransform->SetWorldScale(3.f, 3.f, 3.f);

	m_pAnimation = m_pGameObject->AddComponent<CAnimation>("GolemAnimation");

	m_pNavigation = m_pGameObject->AddComponent<CNavigation3D>("Navigation");

	GolemAttack* pAttackState = new GolemAttack;
	GolemIdle* pIdleState = new GolemIdle;
	GolemMove* pMoveState = new GolemMove;
	GolemDie* pDieState = new GolemDie;

	stateMap.insert(make_pair(GOLEM_STATE::GOLEM_STATE_ATTACK, GOLEMSTATE{ "ATTACK",pAttackState }));
	stateMap.insert(make_pair(GOLEM_STATE::GOLEM_STATE_IDLE, GOLEMSTATE{"IDLE", pIdleState}));
	stateMap.insert(make_pair(GOLEM_STATE::GOLEM_STATE_MOVE, GOLEMSTATE{"MOVE", pMoveState}));
	stateMap.insert(make_pair(GOLEM_STATE::GOLEM_STATE_DIE, GOLEMSTATE{ "DIE", pDieState }));

	pState = getState(GOLEM_STATE::GOLEM_STATE_IDLE);
	currentAnimation = GOLEM_STATE::GOLEM_STATE_IDLE;
	nextAnimation = GOLEM_STATE_NONE;
	return true;
}

void Golem::Input(float fTime)
{
	if (!m_pAnimation)
		m_pAnimation = FindComponentFromType<CAnimation>(CT_ANIMATION);

	if (!m_pNavigation)
		m_pNavigation = FindComponentFromType<CNavigation3D>(CT_NAVIGATION3D);
}

int Golem::Update(float fTime)
{
	if (nullptr != pState)
		pState->execute(m_pGameObject);

	if (m_pTarget)
	{
		Vector3 vTarget = m_pTarget->GetWorldPos();
		//vTarget.y = 0.f;
		Vector3 vMonster = m_pTransform->GetWorldPos();
		if(returnToInitialPos == false)
		{
			if ((initial_pos - vMonster).Length() > 30.f)
				returnToInitialPos = true;
			else if ((vTarget - vMonster).Length() <= 30.f)
			{
				if ((vTarget - vMonster).Length() < 2.5f)
				{
					if(typeid(GolemDie) != typeid(pState)) 
						nextAnimation = GOLEM_STATE::GOLEM_STATE_ATTACK;
					return 0;
				}
				else
				{
					if (typeid(GolemDie) != typeid(pState))
						nextAnimation = GOLEM_STATE::GOLEM_STATE_MOVE;
					//m_pAnimation->ChangeClip("ATTACK");
				}

				//vTarget.y = 0.f;
				//vMonster.y = 0.f;

				Vector3 vDir = (vTarget - vMonster).Normalize();
				//vDir.y = 0.f;
				Vector3 vDir_Mine = m_pTransform->GetWorldAxis(AXIS_Z).Normalize();
				//vDir_Mine.y = 0.f;
				float angle = (vDir_Mine.Dot(vDir));
				if (angle >= 180.f)
					angle = 180.f - angle;
				float Radian = (float)acos(angle);

				if (isnan(Radian))
				{
					Radian = 0.f;
				}

				m_pTransform->RotateWorldY(Radian);
				m_pTransform->MoveWorld(AXIS_Z, 3.f * 2.f, fTime);
			}
		}
		else
		{
			if ((initial_pos - vMonster).Length() < 2.5f)
			{
				if (typeid(GolemDie) != typeid(pState))
					nextAnimation = GOLEM_STATE::GOLEM_STATE_IDLE;
				returnToInitialPos = false;
			}
			else
			{
				if (typeid(GolemDie) != typeid(pState))
					nextAnimation = GOLEM_STATE::GOLEM_STATE_MOVE;
				Vector3 vDir = (initial_pos - vMonster).Normalize();
				vDir.y = 0.f;
				Vector3 vDir_Mine = m_pTransform->GetWorldAxis(AXIS_Z).Normalize();
				vDir_Mine.y = 0.f;
				float angle = (vDir_Mine.Dot(vDir));
				if (angle >= 180.f)
					angle = 180.f - angle;
				float Radian = (float)acos(angle);

				if (isnan(Radian))
				{
					Radian = 0.f;
				}

				m_pTransform->RotateWorldY(Radian);
				m_pTransform->MoveWorld(AXIS_Z, 3.f * 2.f, fTime);
			}
		}
	}

	return 0;
}

int Golem::LateUpdate(float fTime)
{
	return 0;
}

Golem * Golem::Clone()
{
	return new Golem(*this);
}

void Golem::OnCollisionEnter(CCollider * pSrc, CCollider * pDest, float fTime)
{
}

void Golem::OnCollision(CCollider * pSrc, CCollider * pDest, float fTime)
{
	if (pDest->GetTag() == "PlayerBody")
	{
		if (m_bEffectCheck)
		{
			m_fEffectTime += fTime;
			if (m_fEffectTime > 1.85f)
			{
#pragma region Effect
				char str[64];
				static int count = 0;
				itoa(count, str, 10);
				string appendTag = str;
				string objectTag = "ParticleObj" + appendTag;
				// 클래스 따로 만드는게 나을듯
				CLayer*	pParticleLayer = m_pScene->GetLayer("ParticleLayer");

				CGameObject*	pParticleObj = CGameObject::CreateObject(
					objectTag, pParticleLayer);

				CTransform*	pParticleTr = pParticleObj->GetTransform();
				Vector3 monster_pos = m_pTransform->GetWorldPos();
				Vector3 effect_dir = (m_pTarget->GetTransform()->GetWorldPos() - monster_pos).Normalize();

				monster_pos += effect_dir * 1.25f;

				monster_pos.y += 2.4f;
				pParticleTr->SetWorldPos(monster_pos);

				SAFE_RELEASE(pParticleTr);

				Effect*	pEffect = pParticleObj->AddComponent<Effect>("ParticleObj");

				pEffect->SetGameObject(pParticleObj);

				CParticleSingle*	pParticleSingle = pParticleObj->AddComponent<CParticleSingle>("ParticleSingle");

				pParticleSingle->SetSize(0.f, 0.f);
				pEffect->SetParticleSingle(pParticleSingle);

				SAFE_RELEASE(pParticleSingle);

				CRenderer*	pParticleRenderer = pParticleObj->FindComponentFromType<CRenderer>(CT_RENDERER);

				pEffect->Init_Other();
				SAFE_RELEASE(pEffect);
				/*pMaterial = pParticleRenderer->GetMaterial();

				pMaterial->SetDiffuseTex()

				SAFE_RELEASE(pMaterial);*/

				pParticleRenderer->CreateCBuffer("Animation2D", 10, sizeof(ANIMATION2DBUFFER),
					SCT_VERTEX | SCT_PIXEL);

				CAnimation2D*	pParticleAnimation = pParticleObj->AddComponent<CAnimation2D>("ParticleAnimation");

				pParticleAnimation->SetRenderer2DEnable(false);

				vector<wstring>	vecExplosion;

				for (int i = 1; i <= 1; ++i)
				{
					wchar_t	strPath[MAX_PATH] = {};
					wsprintf(strPath, L"Hit/Hit%d.png", i);

					vecExplosion.push_back(strPath);
				}

				pParticleAnimation->CreateClip("Explosion", A2D_FRAME, A2DO_LOOP,
					1, 1, 1, 1, 0, 0.2f, 0, 0.f, "Explosion",
					&vecExplosion);


				SAFE_RELEASE(pParticleAnimation);

				pParticleRenderer->SetRenderState(ALPHA_BLEND);

				SAFE_RELEASE(pParticleRenderer);

				SAFE_RELEASE(pParticleObj);

#pragma endregion
				m_fEffectTime = 0.f;
				m_bEffectCheck = false;
			}
		}
		if (KEYDOWN("Attack") && m_bEffectCheck == false)
		{

			hearth_point -= 10;
			m_bEffectCheck = true;

			if (hearth_point < 0)
			{
				nextAnimation = GOLEM_STATE::GOLEM_STATE_DIE;
				{

				}
			}
		}
	}

	if (pDest->GetTag() == "MouseRay" && GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{

		// 이 오브젝트의 좌표를 뷰 공간으로 변환한다.
		CCamera*	pCam = m_pScene->GetMainCamera();

		Matrix	matView = pCam->GetViewMatrix();
		Matrix	matProj = pCam->GetProjMatrix();
		Matrix	matVP = matView * matProj;
		
		Vector3	vPos1 = m_pTransform->GetWorldPos();
		Vector4	vPos = Vector4(vPos1.x, vPos1.y, vPos1.z, 1.f);
		Vector4	vConvertPos = XMVector4Transform(vPos.Convert(), matVP.mat);

		// 투영공간의 w는 뷰공간의 z다.
		float	fViewZ = vConvertPos.w;

		RESOLUTION	tRS = DEVICE_RESOLUTION;

		POINT	ptMouse = GET_SINGLE(CInput)->GetMousePos();
		POINT	ptMouseMove = GET_SINGLE(CInput)->GetMouseMove();

		Vector2	vMouse(ptMouse.x, ptMouse.y);
		Vector2	vMouseMove(ptMouseMove.x, ptMouseMove.y);

		Vector2	vResult = vMouseMove;

		// 결과 벡터를 3차원 공간으로 변환한다.
		Vector3	vResultWorld;
		vResultWorld.x = vResult.x / (tRS.iWidth / 2.f);
		vResultWorld.y = vResult.y / -(tRS.iHeight / 2.f);
		vResultWorld.z = vConvertPos.z / vConvertPos.w;

		// 뷰공간으로 변환한다.
		Matrix	matInvProj	= XMMatrixInverse(&XMMatrixDeterminant(matProj.mat),
			matProj.mat);
		vResultWorld = vResultWorld.TransformCoord(matInvProj.mat);

		float	fDist = vResultWorld.Length();

		vResultWorld.z = 0.f;

		Matrix	matInvView;
		matInvView = XMMatrixInverse(&XMMatrixDeterminant(matView.mat), matView.mat);

		vResultWorld = vResultWorld.TransformNormal(matInvView.mat);

		m_pTransform->SetWorldPos(vPos1 + vResultWorld);

		SAFE_RELEASE(pCam);
	}
}

void Golem::OnCollisionLeave(CCollider * pSrc, CCollider * pDest, float fTime)
{
	int a = 10;
}
