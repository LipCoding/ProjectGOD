#include "Actor.h"

Actor::Actor()
{
	SetTag("Actor");
	SetTypeName("Actor");
	SetTypeID<Actor>();
}

Actor::~Actor()
{
}

void Actor::OnCollisionEnter(CCollider * pSrc, CCollider * pDest, float fTime)
{
}

void Actor::OnCollision(CCollider * pSrc, CCollider * pDest, float fTime)
{
}

void Actor::OnCollisionLeave(CCollider * pSrc, CCollider * pDest, float fTime)
{
}
