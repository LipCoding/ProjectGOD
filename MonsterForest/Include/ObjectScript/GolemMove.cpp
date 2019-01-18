#include "GolemMove.h"
#include "Golem.h"
#include "Component/Animation.h"


GolemMove::GolemMove()
{
	setTypeID<GolemMove>();
}


GolemMove::~GolemMove()
{
}

void GolemMove::enter(CGameObject * pGameObject)
{
}

void GolemMove::execute(CGameObject * pGameObject)
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
	case GOLEM_STATE::GOLEM_STATE_ATTACK:
		pState = pGolem->getState(GOLEM_STATE::GOLEM_STATE_ATTACK);
		if (nullptr != pState)
		{
			pGolem->setCurrentState(pState);
			pState->enter(pGameObject);
		}
		break;
	case GOLEM_STATE::GOLEM_STATE_IDLE:
		pState = pGolem->getState(GOLEM_STATE::GOLEM_STATE_IDLE);
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
		currentAnimation == GOLEM_STATE::GOLEM_STATE_MOVE)
		return;

	pGolem->changeAnimation();
	pGolem->setCurrentAnimation(nextAnimation);
	pGolem->setNextAnimation(GOLEM_STATE::GOLEM_STATE_NONE);
}

void GolemMove::exit(CGameObject * pGameObject)
{
}
