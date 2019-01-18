#include "GolemAttack.h"
#include "Golem.h"
#include "Component/Animation.h"
GolemAttack::GolemAttack()
{
	setTypeID<GolemAttack>();
}


GolemAttack::~GolemAttack()
{
}

void GolemAttack::enter(CGameObject * pGameObject)
{
	string tag = pGameObject->GetTag();
	const auto& pGolem = pGameObject->FindComponentFromTag<Golem>(tag);

	//플레이어 포인터 유효성 체크.
	if (nullptr == pGolem)
		return;
}

void GolemAttack::execute(CGameObject * pGameObject)
{
	string tag = pGameObject->GetTag();
	const auto& pGolem = pGameObject->FindComponentFromTag<Golem>(tag);
	const auto& animation = pGameObject->FindComponentFromType<CAnimation>(CT_ANIMATION);

	if (nullptr == pGolem)
		return;

	size_t nextAnimation = pGolem->getNextAnimation();
	GolemState* pState{ nullptr };

	switch (nextAnimation)
	{
	case GOLEM_STATE::GOLEM_STATE_IDLE:
		pState = pGolem->getState(GOLEM_STATE::GOLEM_STATE_IDLE);
		if (nullptr != pState)
		{
			pGolem->setCurrentState(pState);
			pState->enter(pGameObject);
		}
		break;
	case GOLEM_STATE::GOLEM_STATE_MOVE:
		pState = pGolem->getState(GOLEM_STATE::GOLEM_STATE_MOVE);
		if (nullptr != pState)
		{
			pGolem->setCurrentState(pState);
			pState->enter(pGameObject);
		}
		break;
	case GOLEM_STATE::GOLEM_STATE_DIE:
		pState = pGolem->getState(GOLEM_STATE::GOLEM_STATE_DIE);
		if (nullptr != pState)
		{
			pGolem->setCurrentState(pState);
			pState->enter(pGameObject);
		}
		break;
	}

	size_t currentAnimation = pGolem->getCurrentAnimation();

	if (true == animation->IsAnimationEnd())
	{
		nextAnimation = GOLEM_STATE::GOLEM_STATE_IDLE;
	}

	if (nextAnimation == GOLEM_STATE::GOLEM_STATE_NONE &&
		currentAnimation == GOLEM_STATE::GOLEM_STATE_ATTACK)
		return;

	pGolem->changeAnimation();
	pGolem->setCurrentAnimation(nextAnimation);
	pGolem->setNextAnimation(GOLEM_STATE::GOLEM_STATE_NONE);
}

void GolemAttack::exit(CGameObject * pGameObject)
{

}
