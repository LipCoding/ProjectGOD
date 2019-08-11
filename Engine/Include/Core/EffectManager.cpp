#include "EffectManager.h"

#include "../Scene/SceneManager.h"
#include "../Scene/Scene.h"
#include "../Scene/Layer.h"
#include "../Resources/Mesh.h"
#include "../Component/Transform.h"
#include "../Component/Renderer.h"
#include "../Component/Material.h"
#include "../Component/Billboard.h"
#include "../Core/PathManager.h"

PG_USING

DEFINITION_SINGLE(CEffectManager)

CEffectManager::CEffectManager()
{
}

CEffectManager::~CEffectManager()
{
}

void CEffectManager::AddEffect(const string & effectTag, const string & filePath)
{
	map<string, vector<PEFFECTDATA>>::iterator iter = m_mapEffects.find(effectTag);

	if (iter != m_mapEffects.end())
		return;

	vector<PEFFECTDATA> vecData;

	string dataPath = GET_SINGLE(CPathManager)->FindPathToMultiByte(DATA_PATH);
	string meshPath = GET_SINGLE(CPathManager)->FindPathToMultiByte(MESH_PATH);
	string texPath = GET_SINGLE(CPathManager)->FindPathToMultiByte(TEXTURE_PATH);

	ifstream file;
	file.open(dataPath + filePath, ios::in);

	if (!file.is_open())
		return;

	// Effect 갯수 
	int iCount = 0;
	file >> iCount;

	CScene* pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
	CLayer* pLayer = pScene->GetLayer("Effect");

	for (int i = 0; i < iCount; ++i)
	{
		// Create Object
		EFFECTDATA *pData = new EFFECTDATA;

		pData->strName = effectTag + to_string(m_iEffectNumber);
		pData->pObject = CGameObject::CreatePrototype(pData->strName, pScene);

		pData->pEffect = pData->pObject->AddComponent<CEffect>("Effect");

		// Mesh
		string meshName, meshFilePath;
		file >> meshName;
		meshFilePath = meshPath + meshName;

		if (!pData->pEffect->LoadEffectMesh(meshFilePath, meshName))
		{
			FreeEffectData(pData);
			SAFE_RELEASE(pLayer);
			SAFE_RELEASE(pScene);
			return;
		}

		// Local
		string localFilePath;
		file >> localFilePath;
		localFilePath = meshPath + localFilePath;

		if (!pData->pEffect->LoadEffectLocalInfo(localFilePath))
		{
			FreeEffectData(pData);
			SAFE_RELEASE(pLayer);
			SAFE_RELEASE(pScene);
			return;
		}

		// Texture
		string texName, texFilePath;
		file >> texName;
		texFilePath = texPath + texName + ".png";

		pData->pEffect->SetEffectTexture(texName, texFilePath);

		// scale, rot , (pos는 더해줘야 할듯)
		Vector3 worldPos, worldScale, worldRot;

		file >> worldPos.x >> worldPos.y >> worldPos.z;
		file >> worldScale.x >> worldScale.y >> worldScale.z;
		file >> worldRot.x >> worldRot.y >> worldRot.z;

		worldRot.x = XMConvertToRadians(worldRot.x);
		worldRot.y = XMConvertToRadians(worldRot.y);
		worldRot.z = XMConvertToRadians(worldRot.z);

		// Transform
		pData->pTr = pData->pObject->GetTransform();
		pData->pTr->SetWorldPos(worldPos.x, worldPos.y, worldPos.z);
		pData->pTr->SetWorldScale(worldScale.x, worldScale.y, worldScale.z);
		pData->pTr->SetWorldRot(worldRot.x, worldRot.y, worldRot.z);

		// Effect 정보
		float fMainStartTime, fMainEndTime = 0.f;
		file >> fMainStartTime;
		file >> fMainEndTime;

		pData->pEffect->SetMainStartTime(fMainStartTime);
		pData->pEffect->SetMainEndTime(fMainEndTime);

		int temp = 0;
		bool mainInfiniteCheck = false;
		file >> temp;

		if (1 == temp)
		{
			mainInfiniteCheck = true;
		}

		pData->pEffect->SetInfiniteMainCheck(mainInfiniteCheck);


		// Effect Assist 정보
		// Assist 갯수 
		int iAssistCount = 0;
		file >> iAssistCount;

		for (int j = 0; j < iAssistCount; ++j)
		{
			CEffectAssist::ASSIST_TYPE eType;
			CEffectAssist::EASE_TYPE eEaseType;

			int type = 0;
			file >> type;
			eType = (CEffectAssist::ASSIST_TYPE)type;

			file >> type;
			eEaseType = (CEffectAssist::EASE_TYPE)type;

			float fStart = 0.f;
			file >> fStart;

			float fEnd = 0.f;
			file >> fEnd;

			float fPowX, fPowY, fPowZ = 0.f;
			file >> fPowX >> fPowY >> fPowZ;

			float fDegree = 0.f;
			file >> fDegree;

			CEffectAssist::SPRITE_TYPE eSpriteType;
			file >> type;
			eSpriteType = (CEffectAssist::SPRITE_TYPE)type;

			int iNum = 0;
			file >> iNum;

			int iWidth, iHeight, iMaxX, iMaxY = 0;
			file >> iWidth;
			file >> iHeight;
			file >> iMaxX;
			file >> iMaxY;

			float fAniX, fAniY = 0.f;
			file >> fAniX;
			file >> fAniY;

			int temp_other = 0;
			bool assistInfiniteCheck = false;
			file >> temp_other;

			if (1 == temp_other)
			{
				assistInfiniteCheck = true;
			}

			switch (eType)
			{
			case CEffectAssist::ASSIST_SCALE:
				pData->pEffect->AddPatternScale(eEaseType, fStart, fEnd, fPowX, fPowY, fPowZ, 1);
				break;
			case CEffectAssist::ASSIST_ROT:
				pData->pEffect->AddPatternRotation(eEaseType, fStart, fEnd, fPowX, fPowY, fPowZ, 1);
				break;
			case CEffectAssist::ASSIST_POS:
				pData->pEffect->AddPatternPosition(eEaseType, fStart, fEnd, fPowX, fPowY, fPowZ, 1);
				break;
			case CEffectAssist::ASSIST_FADE_IN:
				pData->pEffect->AddFadeIn(fStart, fEnd, fDegree);
				break;
			case CEffectAssist::ASSIST_FADE_OUT:
				pData->pEffect->AddFadeOut(fStart, fEnd, fDegree);
				break;
			case CEffectAssist::ASSIST_UV_ANI:
				if (CEffectAssist::SPRITE_FRAME == eSpriteType)
					pData->pEffect->AddUVAnimation(fStart, fEnd, iNum, 1);
				else if (CEffectAssist::SPRITE_ATLAS == eSpriteType)
					pData->pEffect->AddUVAnimation(fStart, fEnd, iMaxX, iMaxY, iWidth, iHeight, 1);
				break;
			case CEffectAssist::ASSIST_UV_MOVE:
				pData->pEffect->AddUVMovement(fStart, fEnd, fAniX, fAniY);
				break;
			default:
				break;
			}

			pData->pEffect->SetInfiniteCheckAssistEffectFromType(eType, assistInfiniteCheck);
		}

		int iBillbordCheck = 0;
		file >> iBillbordCheck;

		if (iBillbordCheck)
		{
			pData->isBillboardCheck = true;
			//CBillboard *pBillboard = pData->pObject->AddComponent<CBillboard>("Billboard");
			//SAFE_RELEASE(pBillboard);
		}
		else
		{
			pData->isBillboardCheck = false;
		}

		vecData.push_back(pData);

		++m_iEffectNumber;
	}

	m_iEffectNumber = 0;

	file.close();

	SAFE_RELEASE(pLayer);
	SAFE_RELEASE(pScene);

	m_mapEffects.insert(make_pair(effectTag, vecData));
}

vector<PEFFECTDATA>* CEffectManager::FindEffect(const string & effectTag)
{
	map<string, vector<PEFFECTDATA>>::iterator iter = m_mapEffects.find(effectTag);
	
	if (iter == m_mapEffects.end())
		return nullptr;

	return &iter->second;
}

void CEffectManager::DeleteEffect(const string & effectTag)
{
	map<string, vector<PEFFECTDATA>>::iterator iter = m_mapEffects.find(effectTag);

	if (iter == m_mapEffects.end())
		return;

	for (auto& effect : iter->second)
	{
		FreeEffectData(effect);
	}

	iter->second.clear();

	m_mapEffects.erase(iter->first);
}

void CEffectManager::ClearAll()
{
	for (auto& iter : m_mapEffects)
	{
		for (auto& effect : iter.second)
		{
			FreeEffectData(effect);
		}
		iter.second.clear();
	}
	m_mapEffects.clear();
}

void CEffectManager::OperateEffect(const string & effectTag, CGameObject * pOperator, Vector3 vPos, bool followCheck, bool parentCheck)
{
	map<string, vector<PEFFECTDATA>>::iterator iter = m_mapEffects.find(effectTag);

	if (iter == m_mapEffects.end())
		return;

	CScene* pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
	CLayer* pLayer = pScene->GetLayer("Effect");

	for (auto& effect : iter->second)
	{
		CGameObject *pEffectObj = CGameObject::CreateClone(effect->strName, pLayer);

		if (effect->isBillboardCheck)
		{
			CBillboard *pBillboard = pEffectObj->AddComponent<CBillboard>("Billboard");
			SAFE_RELEASE(pBillboard);
		}

		CTransform *pEffectTr = pEffectObj->GetTransform();
		Vector3 vOriginPos = pEffectTr->GetWorldPos();
		
		CEffect *pEffect = pEffectObj->FindComponentFromType<CEffect>(CT_EFFECT);
		pEffect->SetErase(true);
		pEffect->SetOperationCheck(true);

		// Operator가 있으면 그 Oprator의 Look방향을 바라보면서 이펙트가 생성된다.
		if(pOperator)
		{
			pEffect->SetOperatorObject(pOperator);
			pEffect->SetFollowOperatorCheck(followCheck, vOriginPos);
			pEffect->SetOperatorParentCheck(parentCheck);
		}

		pEffectTr->SetWorldPos(vPos + vOriginPos);	
		SAFE_RELEASE(pEffectTr);

		SAFE_RELEASE(pEffect);
	}

	SAFE_RELEASE(pLayer);
	SAFE_RELEASE(pScene);
}

void CEffectManager::FreeEffectData(EFFECTDATA * effect)
{
	if (effect)
	{
		SAFE_RELEASE(effect->pEffect);
		SAFE_RELEASE(effect->pTr);

		if (effect->pObject)
			effect->pObject->Die();

		CGameObject::EraseObj(effect->pObject);
		SAFE_RELEASE(effect->pObject);
	}

	SAFE_DELETE(effect);
}
