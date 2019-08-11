#include "UserInterfaceManager.h"
#include "Quest.h"
#include "Core.h"
#include <ShlObj.h>
#include <Shlwapi.h>
#include "Scene/Layer.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Component/Renderer2D.h"
//#include "Component/ColliderRect.h"
#include "QuestToolCore.h"
#include "Component/Material.h"
#include "Component/Transform.h"
#include "Component/UIPanel.h"
#include "Component/UIButton.h"
//#include "Core/TimerManager.h"
//#include "Core\Timer.h"
//#include "Core/Input.h"
//#include "Component/ThirdCamera.h"
//#include "Component/Arm.h"
//#include "Core/QuadTreeManager.h"
UserInterfaceManager::UserInterfaceManager()
{
}


UserInterfaceManager::~UserInterfaceManager()
{
}

bool UserInterfaceManager::initialize()
{
	CScene* pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
	{
		pScene->CreateLayer("UI+1", UI_LAYER + 1);
	}
#pragma region Contents
	{
		CLayer*	pLayer = pScene->GetLayer("UI");
		
		CGameObject* ui_quest_object = CGameObject::CreateObject("DropTableUI", pLayer);
		ui_quest_object->AddComponent<CUIPanel>("Quest");

		CRenderer2D* pRenderer = ui_quest_object->FindComponentFromType<CRenderer2D>(CT_RENDERER2D);
		CMaterial* pMaterial = pRenderer->GetMaterial();

		pMaterial->SetDiffuseTexInfo("Linear", "UI_DropTable",
			0, 0, L"UserInterface/UI_BASIC_BOX_BIG.png");
		CFont* contents_text_subject = ui_quest_object->AddComponent<CFont>("quest_ui_text_subject");
		contents_text_subject->SetFont("¸¼Àº°íµñ35N");
		contents_text_subject->SetArea(50, 50, 750, 100.f);

		CFont* contents_text_summary = ui_quest_object->AddComponent<CFont>("quest_ui_text_summary");
		contents_text_summary->SetFont("¸¼Àº°íµñ35N");
		contents_text_summary->SetArea(50, 150, 750, 300.f);

		CFont* contents_text_contents = ui_quest_object->AddComponent<CFont>("quest_ui_text_contents");
		contents_text_contents->SetFont("¸¼Àº°íµñN");
		contents_text_contents->SetArea(50, 350, 680, 400.f);

		CTransform* pDropTableTr = ui_quest_object->GetTransform();
		pDropTableTr->SetWorldScale(800.f, 800.f, 1.f);
		pDropTableTr->SetWorldPos(200.f, 100.f, 1.f);

		SAFE_RELEASE(pDropTableTr);
		SAFE_RELEASE(pLayer);
		SAFE_RELEASE(pMaterial);
		SAFE_RELEASE(pRenderer);


		QuestToolCore::getInstance()->setQuestUIObject(ui_quest_object);
	}
	{
		CLayer*	pLayer = pScene->GetLayer("UI+1");

		scroll_object = CGameObject::CreateObject("QuestScroll", pLayer);
		scroll_object->AddComponent<CUIButton>("ContentsScroll");

		CRenderer2D* pRenderer = scroll_object->FindComponentFromType<CRenderer2D>(CT_RENDERER2D);
		CMaterial* pMaterial = pRenderer->GetMaterial();
		
		CTransform* pDropTableTr = scroll_object->GetTransform();
		pDropTableTr->SetWorldScale(50.f, 400.f, 1.f);
		pDropTableTr->SetWorldPos(900.f, 450.f, 1.f);

		pMaterial->SetDiffuseTexInfo("Linear", "scroll",
			0, 0, L"UserInterface/UI_BASIC_SCROLLBAR.png");
	}

	{
		CLayer*	pLayer = pScene->GetLayer("UI+1");

		quest_gold_object = CGameObject::CreateObject("GoldIcon", pLayer);
		quest_gold_object->AddComponent<CUIButton>("GoldIcon");

		CRenderer2D* pRenderer = quest_gold_object->FindComponentFromType<CRenderer2D>(CT_RENDERER2D);
		CMaterial* pMaterial = pRenderer->GetMaterial();

		CTransform* pDropTableTr = quest_gold_object->GetTransform();
		pDropTableTr->SetWorldScale(64.f, 64.f, 1.f);
		pDropTableTr->SetWorldPos(300.f, 780.f, 1.f);

		pMaterial->SetDiffuseTexInfo("Linear", "Gold",
			0, 0, L"UserInterface/Gold.png");
		
		CFont* contents_text_summary = quest_gold_object->AddComponent<CFont>("GOLD");
		contents_text_summary->SetFont("¸¼Àº°íµñ25N");
		contents_text_summary->SetArea(100, 0, 250, 50.f);
		contents_text_summary->SetText(L"X 0");

	}

	{
		CLayer*	pLayer = pScene->GetLayer("UI+1");

		quest_exp_object = CGameObject::CreateObject("Quest_EXP", pLayer);
		quest_exp_object->AddComponent<CUIButton>("GoldIcon");

		CTransform* pDropTableTr = quest_exp_object->GetTransform();
		pDropTableTr->SetWorldScale(64.f, 64.f, 1.f);
		pDropTableTr->SetWorldPos(550.f, 780.f, 1.f);

		CFont* contents_text_summary = quest_exp_object->AddComponent<CFont>("EXP");
		contents_text_summary->SetFont("¸¼Àº°íµñ25N");
		contents_text_summary->SetArea(100, 0, 450, 50.f);
		contents_text_summary->SetText(L"°æÇèÄ¡ 0");

	}
	/*
						char reward_item_name[128];
					int i = SendMessage(QuestToolCore::getInstance()->getListBoxQuestListsHandle(), LB_GETCURSEL, 0, 0);
					SendMessageA(QuestToolCore::getInstance()->getListBoxQuestListsHandle(), LB_GETTEXT, i, (LPARAM)reward_item_name);
	*/
	//char reward_item_name[256];
	//int count = SendMessage(QuestToolCore::getInstance()->getListBoxRewardItemsHandle(), LB_GETCOUNT, 0, 0);
	//vector<string> listbox_item_names;
	//for (int i = 0; i < count; ++i)
	//{
	//	SendMessageA(QuestToolCore::getInstance()->getListBoxRewardItemsHandle(), LB_GETTEXT, i, (LPARAM)reward_item_name);
	//	listbox_item_names.push_back(string{ reward_item_name });
	//}

#pragma endregion
	SAFE_RELEASE(pScene);
	return true;
}

bool UserInterfaceManager::addQuest(const string & quest_subject, const string & quest_summary, const string & quest_contents, int minimum_level, int maximum_level,
	int reward_experience, int reward_gold, const vector<string>& reward_items)
{
	Quest* new_quest = new Quest;
	new_quest->initialize(quest_subject, quest_summary, quest_contents, minimum_level, maximum_level, reward_experience, reward_gold, reward_items);
	quests.emplace_back(new_quest);

	
	SendMessageA(QuestToolCore::getInstance()->getListBoxQuestListsHandle(), LB_ADDSTRING, 0, (LPARAM)quest_subject.c_str());
	return true;
}

bool UserInterfaceManager::removeQuest(const string & quest_subject)
{
	auto finder = remove_if(quests.begin(), quests.end(),
		[quest_subject](Quest* quest)
		{
			return quest->getQuestSubject() == quest_subject;
		}
	);

	if (quests.end() != finder)
		quests.erase(quests.end(), finder);

	return true;
}

Quest* UserInterfaceManager::findQuest(const string & quest_subject)
{
	auto finder = find_if(quests.begin(), quests.end(), 
		[quest_subject](Quest* quest)
	{
		return quest->getQuestSubject() == quest_subject;
	});

	if (quests.end() != finder)
		return *finder;

	return nullptr;
}

bool UserInterfaceManager::save(const string & save_file_name)
{
	ofstream save_file(save_file_name);
	save_file << quests.size() << " ";
	for (Quest* quest : quests)
	{
		quest->save(save_file);
	}
	return true;
}

bool UserInterfaceManager::load(const string & load_file_name)
{
	quests.clear();

	ifstream load_file(load_file_name);

	int count;
	load_file >> count;

	for (int i = 0; i < count; ++i)
	{
		// »õ Äù½ºÆ® ÇÒ´ç.
		Quest* load_quest = new Quest;

		// ·ÎµåÇÑ Äù½ºÆ® Ãß°¡.
		load_quest->load(load_file);
		quests.push_back(load_quest);
	}

	return true;
}

void UserInterfaceManager::GetFindFileList(char* pszDirectory, const char* pszFilter, std::vector <std::string> *vtList, int iLevel)

{

	if (iLevel == 0)

		return;



	char buf[MAX_PATH];

	WIN32_FIND_DATAA FindFileData;



	bool bAddFile = true;

	bool bAddFolder = false;

	if (pszFilter[1] == 0)

	{

		if (pszFilter[0] == '\\')

		{

			bAddFile = false;

			bAddFolder = true;

		}

		else if (pszFilter[0] == '*')

		{

			bAddFolder = true;

		}

	}



	strcpy_s(buf, MAX_PATH, pszDirectory);

	strcat_s(buf, MAX_PATH, "\\");

	if (bAddFolder == true)

		strcat_s(buf, MAX_PATH, "*.*");

	else

		strcat_s(buf, MAX_PATH, pszFilter);



	HANDLE hHandle = FindFirstFileA(buf, &FindFileData);



	for (; hHandle != INVALID_HANDLE_VALUE; )

	{

		if ((FindFileData.cFileName[0] != '.' || strlen(FindFileData.cFileName) > 2) &&

			(0 != _stricmp(FindFileData.cFileName, "Replay"))) //  Replay Æú´õ´Â Áö¿ìÁö ¾Ê´Â´Ù

		{

			strcpy_s(buf, MAX_PATH, pszDirectory);

			strcat_s(buf, MAX_PATH, "\\");

			strcat_s(buf, MAX_PATH, FindFileData.cFileName);



			if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)

			{

				if (bAddFolder)

					vtList->push_back(buf);

				GetFindFileList(buf, pszFilter, vtList, iLevel - 1);

			}

			else

			{

				if (bAddFile)

					vtList->push_back(buf);

			}



		}



		if (!FindNextFileA(hHandle, &FindFileData))

			break;

	}

	FindClose(hHandle);

}
