#include "UserInterfaceManager.h"
#include "Quest.h"
#include "Core.h"
#include <ShlObj.h>
#include <Shlwapi.h>
#include "Scene/Layer.h"
#include "GameObject/GameObject.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Component/Renderer2D.h"
//#include "Component/ColliderRect.h"
#include "QuestToolCore.h"
#include "Component/Material.h"
#include "Component/Transform.h"
#include "Component/UIPanel.h"
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
#pragma region Contents
	{
		CScene* pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
		CLayer*	pLayer = pScene->GetLayer("UI");

		CGameObject* ui_quest_object = CGameObject::CreateObject("DropTableUI", pLayer);
		ui_quest_object->AddComponent<CUIPanel>("Quest");

		CRenderer2D* pRenderer = ui_quest_object->FindComponentFromType<CRenderer2D>(CT_RENDERER2D);
		CMaterial* pMaterial = pRenderer->GetMaterial();

		pMaterial->SetDiffuseTexInfo("Linear", "UI_DropTable",
			0, 0, L"UserInterface/UI_BASIC_BOX_BIG.png");
		CFont* contents_text_subject = ui_quest_object->AddComponent<CFont>("quest_ui_text_subject");
		contents_text_subject->SetFont("맑은고딕35");
		contents_text_subject->SetArea(50, 0, 750, 100.f);


		CFont* contents_text_summary = ui_quest_object->AddComponent<CFont>("quest_ui_text_summary");
		contents_text_summary->SetFont("맑은고딕35");
		contents_text_summary->SetArea(50, 0, 750, 200.f);

		CFont* contents_text_contents = ui_quest_object->AddComponent<CFont>("quest_ui_text_contents");
		contents_text_contents->SetFont("맑은고딕35");
		contents_text_contents->SetArea(50, 0, 750, 600.f);

		CTransform* pDropTableTr = ui_quest_object->GetTransform();
		pDropTableTr->SetWorldScale(800.f, 800.f, 1.f);
		pDropTableTr->SetWorldPos(200.f, 100.f, 1.f);

		SAFE_RELEASE(pDropTableTr);
		SAFE_RELEASE(pLayer);
		SAFE_RELEASE(pMaterial);
		SAFE_RELEASE(pRenderer);
		SAFE_RELEASE(pScene);

		QuestToolCore::getInstance()->setQuestUIObject(ui_quest_object);
	}
#pragma endregion


	//char* pszFolderPath = new char[_MAX_PATH];
	//LPITEMIDLIST pidl;
	//BROWSEINFO bi;

	//bi.hwndOwner = GET_SINGLE(CCore)->GetWindowHandle();
	//bi.pidlRoot = NULL;
	//bi.pszDisplayName = NULL;
	//bi.lpszTitle = L"폴더를 선택해 주세요";
	//bi.ulFlags = BIF_NEWDIALOGSTYLE | BIF_EDITBOX | BIF_RETURNONLYFSDIRS;
	//bi.lpfn = NULL;
	//bi.lParam = 0;

	//pidl = SHBrowseForFolder(&bi);
	//if (pidl == NULL)
	//	return false;

	//SHGetPathFromIDListA(pidl, pszFolderPath);
	////SetWindowTextA(folderNameEdit, (LPCSTR)pszFolderPath);

	//vector<string> fileList;
	//GetFindFileList(pszFolderPath, "*.*", &fileList, -1);

	//{
	//	// 리소스 매니저에 등록한다.
	//	for (const auto& fileName : fileList)
	//	{
	//		string path = fileName;
	//		char fileNameKey[128];
	//		strcpy(fileNameKey, path.c_str());
	//		fileNameKey[path.length()] = '\0';
	//		PathStripPathA(fileNameKey);
	//		//tileNameCont.push_back(move(string{ fileNameKey }));
	//		//ResourcesManager::getInstance()->loadTexture(path, fileNameKey, 1, 1);

	//	}
	//}

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
	for (Quest* quest : quests)
	{
		quest->save(save_file);
	}
	return true;
}

bool UserInterfaceManager::load(const string & load_file_name)
{
	ifstream load_file(load_file_name);
	// 새 퀘스트 할당.
	Quest* load_quest;

	// 로드한 퀘스트 추가.
	load_quest->load(load_file);

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

			(0 != _stricmp(FindFileData.cFileName, "Replay"))) //  Replay 폴더는 지우지 않는다

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
