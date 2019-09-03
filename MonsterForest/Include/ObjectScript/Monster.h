#pragma once

#include "Component/Script.h"
#include "Component/Transform.h"
#include "Component/Animation.h"
#include "Component/Navigation3D.h"
#include "Actor.h"
PG_USING

class Monster :
	public Actor
{
protected:
	OBJECT_SET_TYPE objectSetType = OBJECT_SET_TYPE::NONE;
	bool dieState = false;
	int experience = 0;
public:
	Monster();
	~Monster();

public:
	virtual void damaged(int damage);
	virtual void attack(const string& target_tag);
	virtual void attack_animation(const string& target_tag, const wstring& name) {};
	virtual void settingStatus(int current_hp, int current_mp, int level, int exp) = 0;
	virtual void rotate(float x, float y, float z);
	virtual void move(float x, float y, float z, bool isBackMove = false);
	virtual void changeAnimation(const string& clip_name);
	virtual void worldscale(float x, float y, float z);
public:
	void setExperience(int experience) { this->experience = experience; }
	virtual void setDieState(bool state);
	void setObjectSetType(OBJECT_SET_TYPE type) { objectSetType = type; }
	OBJECT_SET_TYPE getObjectSetType() { return objectSetType; }

public:
	virtual void OnCollisionEnter(class CCollider* pSrc, class CCollider* pDest,
		float fTime);
	virtual void OnCollision(class CCollider* pSrc, class CCollider* pDest,
		float fTime);
	virtual void OnCollisionLeave(class CCollider* pSrc, class CCollider* pDest,
		float fTime);
};

