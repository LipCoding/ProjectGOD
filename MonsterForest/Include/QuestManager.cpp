#include "QuestManager.h"
#include "Core/PathManager.h"
#include "Scene/Layer.h"
#include "GameObject/GameObject.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Component/Renderer2D.h"
#include "Component/ColliderRect.h"
#include "Component/Material.h"
#include "Component/Transform.h"

#include "Component/Font.h"

QuestManager::QuestManager()
{
}


QuestManager::~QuestManager()
{
}

bool QuestManager::initialize()
{
	wstring temp_path = GET_SINGLE(CPathManager)->FindPath(BASE_PATH);
	temp_path += L"Quest/*.*";

	string itemIcon_folder_path = strconv(temp_path);
	vector<string> filesInForder;
	WIN32_FIND_DATAA fd;
	HANDLE handle = ::FindFirstFileA(itemIcon_folder_path.c_str(), &fd);

	if (INVALID_HANDLE_VALUE == handle)
		return false;

	do
	{
		if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			filesInForder.push_back(fd.cFileName);
		}
	} while (::FindNextFileA(handle, &fd));

	::FindClose(handle);

	for (const auto& file_name : filesInForder)
	{
		Quest* new_quest = new Quest;
		// 각각의 퀘스트들의 경로이다. 이 경로대로 파일을 오픈한다.
		wstring file_path = GET_SINGLE(CPathManager)->FindPath(BASE_PATH);
		file_path += L"Quest\\" + strconv(file_name);

		ifstream quest_file(file_path);
		new_quest->load(quest_file);
		quest_file.close();

		quests.insert(make_pair(file_name, new_quest));
	}
	


	CScene* pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
	pScene->CreateLayer("UI_QUEST", UI_LAYER + 10);
	SAFE_RELEASE(pScene);
	{

		CScene* pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
		CLayer*	pLayer = pScene->GetLayer("UI_QUEST");

		pQuestUIObject = CGameObject::CreateObject("QuestUIObject", pLayer);
		pQuestUIObject->Enable(false);

		pQuestUIComponent = pQuestUIObject->AddComponent<QuestBaseUI>("QuestUIObject");
		pQuestUIComponent->initialize();
		CRenderer2D* pRenderer = pQuestUIObject->FindComponentFromType<CRenderer2D>(CT_RENDERER2D);
		CMaterial* pMaterial = pRenderer->GetMaterial();

		pMaterial->SetDiffuseTexInfo("Linear", "UI_DropTable",
			0, 0, L"UserInterface/UI_INVEN_BOX_3.png");

		CTransform* pDropTableTr = pQuestUIObject->GetTransform();
		pDropTableTr->SetWorldScale(1000.f, 600.f, 1.f);
		pDropTableTr->SetWorldPos(100.f, 100.f, 1.f);

		SAFE_RELEASE(pDropTableTr);
		SAFE_RELEASE(pMaterial);
		SAFE_RELEASE(pRenderer);
		SAFE_RELEASE(pLayer);
		SAFE_RELEASE(pScene);
	}

	return true;
}


Quest * QuestManager::findQuest(const string & quest_name)
{
	const auto& finder = quests.find(quest_name);

	if (finder != quests.end())
	{
		return finder->second;
	}

	return nullptr;
}

void QuestManager::enableShow(bool ui_show)
{
	this->ui_show = ui_show;
	this->pQuestUIObject->Enable(ui_show);
	pQuestUIComponent->enableShow(ui_show);
}
