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
	// �߰�
	void AddEffect(const string& effectTag, const string& filePath);

	// �˻�
	vector<PEFFECTDATA>* FindEffect(const string& effectTag);

	// ����, ��ü����
	void DeleteEffect(const string& effectTag);
	void ClearAll();
	
	// ����
	void OperateEffect(const string& effectTag, CGameObject *pOperator, Vector3 vPos);

private:
	void FreeEffectData(EFFECTDATA* effect);

private:
	map<string, vector<PEFFECTDATA>> m_mapEffects;
	int m_iEffectNumber = 0;

	DECLARE_SINGLE(CEffectManager)
};

PG_END