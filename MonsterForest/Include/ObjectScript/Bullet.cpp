#include "Bullet.h"
#include "Component/Renderer2D.h"
#include "Component/Transform.h"
#include "GameObject/GameObject.h"

CBullet::CBullet()	:
	m_pTarget(NULL)
{
	SetTag("Bullet");
	SetTypeName("CBullet");
	SetTypeID<CBullet>();
}

CBullet::CBullet(const CBullet & bullet)	:
	CScript(bullet)
{
	m_fSpeed = bullet.m_fSpeed;
	m_fDist = bullet.m_fDist;

	m_pTarget = NULL;
}

CBullet::~CBullet()
{
	SAFE_RELEASE(m_pTarget);
}

void CBullet::SetTarget(const string & strTag)
{
	const list<CGameObject*>* pList = CGameObject::FindObjects(strTag);

	if (pList->empty())
		return;

	if (pList->size() == 1)
	{
		m_pTarget = *pList->begin();
		m_pTarget->AddRef();
		return;
	}

	list<CGameObject*>::const_iterator	iter;
	list<CGameObject*>::const_iterator	iterEnd = pList->end();

	float	fPrevDist = -1.f;
	float	fDist = 0.f;

	for (iter = pList->begin(); iter != iterEnd; ++iter)
	{
		CTransform*	pTr = (*iter)->GetTransform();

		fDist = pTr->GetWorldPos().Distance(m_pTransform->GetWorldPos());

		if (fPrevDist == -1)
		{
			fPrevDist = fDist;
			m_pTarget = *iter;
		}

		else if (fPrevDist > fDist)
		{
			fPrevDist = fDist;
			m_pTarget = *iter;
		}

		SAFE_RELEASE(pTr);
	}

	if (m_pTarget)
		m_pTarget->AddRef();
}

bool CBullet::Init()
{
	CRenderer2D*	pRenderer = m_pGameObject->AddComponent<CRenderer2D>("BulletRenderer");

	pRenderer->SetShader(STANDARD_COLOR_SHADER);
	pRenderer->SetInputLayout("VertexColor");
	pRenderer->SetMesh("RectOrthoLTColor");

	SAFE_RELEASE(pRenderer);

	m_pTransform->SetWorldScale(50.f, 50.f, 1.f);

	m_fSpeed = 600.f;
	m_fDist = 300.f;

	return true;
}

void CBullet::Input(float fTime)
{
}

int CBullet::Update(float fTime)
{
	if (m_pTarget)
	{
		m_pTransform->LookAt(m_pTarget);
		/*CTransform*	pTransform = m_pTarget->GetTransform();

		Vector3	vDir = pTransform->GetWorldPos() - m_pTransform->GetWorldPos();
		vDir = vDir.Normalize();

		m_pTransform->MoveWorld(vDir, 50.f, fTime);

		SAFE_RELEASE(pTransform);*/
	}

	m_pTransform->MoveWorld(AXIS_X, m_fSpeed, fTime);

	m_fDist -= m_fSpeed * fTime;

	if (m_fDist <= 0.f)
	{
		m_pGameObject->Die();
	}

	return 0;
}

int CBullet::LateUpdate(float fTime)
{
	return 0;
}

CBullet * CBullet::Clone()
{
	return new CBullet(*this);
}
