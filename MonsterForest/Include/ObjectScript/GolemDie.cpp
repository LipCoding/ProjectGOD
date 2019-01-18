#include "GolemDie.h"
#include "Golem.h"
#include "Component/Animation.h"


GolemDie::GolemDie()
{
}


GolemDie::~GolemDie()
{
}

void GolemDie::enter(CGameObject * pGameObject)
{
}

void GolemDie::execute(CGameObject * pGameObject)
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
	//case GOLEM_STATE::GOLEM_STATE_ATTACK:
	//	pState = pGolem->getState(GOLEM_STATE::GOLEM_STATE_ATTACK);
	//	if (nullptr != pState)
	//	{
	//		pGolem->setCurrentState(pState);
	//		pState->enter(pGameObject);
	//	}
	//	break;
	//case GOLEM_STATE::GOLEM_STATE_MOVE:
	//	pState = pGolem->getState(GOLEM_STATE::GOLEM_STATE_MOVE);
	//	if (nullptr != pState)
	//	{
	//		pGolem->setCurrentState(pState);
	//		pState->enter(pGameObject);
	//	}
	//	break;
	//case GOLEM_STATE::GOLEM_STATE_IDLE:
	//	pState = pGolem->getState(GOLEM_STATE::GOLEM_STATE_IDLE);
	//	if (nullptr != pState)
	//	{
	//		pGolem->setCurrentState(pState);
	//		pState->enter(pGameObject);
	//	}
	//	break;
	}

	size_t currentAnimation = pGolem->getCurrentAnimation();

	if (true == animation->IsAnimationEnd())
	{
		pGameObject->Die();
	}

	if (nextAnimation == GOLEM_STATE::GOLEM_STATE_NONE &&
		currentAnimation == GOLEM_STATE::GOLEM_STATE_DIE)
		return;

	//pGolem->changeAnimation();
	//pGolem->setCurrentAnimation(nextAnimation);
	//pGolem->setNextAnimation(GOLEM_STATE::GOLEM_STATE_NONE);
}

void GolemDie::exit(CGameObject * pGameObject)
{
}
