#pragma once

#include "../PGEngine.h"

#include "../GameObject/GameObject.h"
#include "../Component/Effect.h"

PG_BEGIN

typedef struct _tagEffectData
{
	string			strName;
	CGameObject		*pObject;
	CEffect			*pEffect;
	CTransform		*pTr;
	bool			isBillboardCheck;
}EFFECTDATA, *PEFFECTDATA;

class PG_DLL CEffectManager
{
public:
	// 추가
	void AddEffect(const string& effectTag, const string& filePath);

	// 검색
	vector<PEFFECTDATA>* FindEffect(const string& effectTag);

	// 삭제, 전체삭제
	void DeleteEffect(const string& effectTag);
	void ClearAll();
	
	// 실행
	void OperateEffect(const string& effectTag, CGameObject *pOperator, Vector3 vPos);

private:
	void FreeEffectData(EFFECTDATA* effect);

private:
	map<string, vector<PEFFECTDATA>> m_mapEffects;
	int m_iEffectNumber = 0;

	DECLARE_SINGLE(CEffectManager)
};

PG_END