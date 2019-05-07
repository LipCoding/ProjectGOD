#include "Sword.h"
#include "../ObjectScript/Player.h"
#include "Resources/Mesh.h"
#include "Core/PathManager.h"
#include "Component/Renderer.h"
#include "Component/Transform.h"

CSword::CSword()
{
	SetTag("Sword");
	SetTypeName("CSword");
	SetTypeID<CSword>();
}

CSword::CSword(const CSword & gun)
{
}


CSword::~CSword()
{
	SAFE_RELEASE(m_pParentTr);
	m_pBoneMatrix = nullptr;
}

bool CSword::Init()
{
	// 따로 Class 필요
	FILE* pFile = nullptr;

	string meshBasePath = GET_SINGLE(CPathManager)->FindPathToMultiByte(MESH_PATH);
	string swordPath = meshBasePath + "00.test\\Attach_Sword_Player_Tanker.dat";

	fopen_s(&pFile, swordPath.c_str(), "rb");

	if (!pFile)
		return false;

	// 메쉬 파일 경로, 메쉬 이름
	wstring wstrMeshPath = GET_SINGLE(CPathManager)->FindPath(MESH_PATH);

	wchar_t wstrArmPath[MAX_PATH] = {};
	size_t pathSize;
	fread(&pathSize, sizeof(size_t), 1, pFile);
	fread(&wstrArmPath, sizeof(wchar_t), pathSize, pFile);
	wchar_t wstrArmName[MAX_PATH] = {};
	size_t nameSize;
	fread(&nameSize, sizeof(size_t), 1, pFile);
	fread(&wstrArmName, sizeof(wchar_t), nameSize, pFile);

	CRenderer *pRenderer = m_pGameObject->AddComponent<CRenderer>("SwordRenderer");

	char strArmName[MAX_PATH] = {};
	sprintf(strArmName, "%ws", wstrArmName);

	pRenderer->SetMeshFromFullPath(strArmName, (wstrMeshPath + wstrArmPath).c_str());

	// 뼈정보
	char strBoneName[MAX_PATH] = {};
	size_t boneSize;
	fread(&boneSize, sizeof(size_t), 1, pFile);
	fread(&strBoneName, sizeof(char), boneSize, pFile);

	CGameObject *pPlayerObj = CGameObject::FindObject("PlayerCharacter");
	CAnimation* pAnimation = pPlayerObj->FindComponentFromType<CAnimation>(CT_ANIMATION);
	PBONE pBone = pAnimation->FindBone(strBoneName);

	m_pBoneMatrix = pBone->matBone;

	m_pParentTr = pPlayerObj->GetTransform();

	CTransform *pSwordTr = m_pGameObject->GetTransform();
	pSwordTr->Load_Local(pFile);

	Vector3 vecCurrentRot = m_pParentTr->GetLocalRot();

	SAFE_RELEASE(pSwordTr);
	SAFE_RELEASE(pAnimation);
	SAFE_RELEASE(pPlayerObj);
	SAFE_RELEASE(pRenderer);

	fclose(pFile);

	return true;
}

void CSword::Input(float fTime)
{
}

int CSword::Update(float fTime)
{
	CTransform* pArmTr = m_pGameObject->GetTransform();
	pArmTr->SetParentMatrix(*m_pBoneMatrix * m_pParentTr->GetLocalMatrix() * m_pParentTr->GetWorldMatrix());
	SAFE_RELEASE(pArmTr);

	return 0;
}

int CSword::LateUpdate(float fTime)
{
	return 0;
}

CSword * CSword::Clone()
{
	return new CSword(*this);
}
