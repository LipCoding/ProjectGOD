#pragma once

#include "Component/Script.h"

PG_USING

class Actor :
	public CScript
{
protected:
	wstring characterName;

	int currentHP = 0;
	int currentMP = 0;

	int maxHP = 0;
	int maxMP = 0;
	int level = 1;
	int attackDamage = 0;
	int EXP = 0;
	int maxEXP = 100;
public:
	Actor();
	~Actor();

public:
	virtual void attack(const string& target_tag) = 0;
	virtual void damaged(int damage) = 0;
	virtual void settingStatus(int current_hp, int current_mp, int level, int exp) = 0;
	virtual void rotate(float x, float y, float z) = 0;
	virtual void move(float x, float y, float z, bool isBackMove = false) = 0;
	virtual void worldscale(float x, float y, float z) = 0;
	virtual void changeAnimation(const string& clip_name) = 0;

public:
	virtual void OnCollisionEnter(class CCollider* pSrc, class CCollider* pDest,
		float fTime);
	virtual void OnCollision(class CCollider* pSrc, class CCollider* pDest,
		float fTime);
	virtual void OnCollisionLeave(class CCollider* pSrc, class CCollider* pDest,
		float fTime);
public:
	int getLevel() { return level; }
	int getCurrentHP() { return currentHP; }
	int getCurrentMP() { return currentMP; }
	int getMaxHP() { return maxHP; }
	int getMaxMP() { return maxMP; }
	int getAttackDamage() { return attackDamage; }
	int getEXP() { return EXP; }
	int getMaxEXP() { return maxEXP; }
public:
	void setEXP(int exp) { this->EXP = exp; }
	void setMaxEXP(int maxEXP) { this->maxEXP = maxEXP; }
	void setLevel(int level) { this->level = level; }
	void setCharacterName(const wstring& characterName) { this->characterName = characterName; }
	void setCurrentHP(int currentHP) { this->currentHP = currentHP; }
	void setCurrentMP(int currentMP) { this->currentMP = currentMP; }
	void setMaxHP(int maxHP) { this->maxHP = maxHP; }
	void setMaxMP(int maxMP) { this->maxMP = maxMP; }
	void setAttackDamage(int attackDamage) { this->attackDamage = attackDamage; }
};

