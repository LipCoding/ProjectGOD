#include "QuestToolCore.h"
#include "Core.h"
#include "QuestScene.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "../resource.h"
#include "UserInterfaceManager.h"
#include "Quest.h"
#include "Core/PathManager.h"
/*
bool CCore::Init(HINSTANCE hInst, TCHAR * pTitle, TCHAR * pClass, int iIconID,
	UINT iWidth, UINT iHeight, bool bWindowMode, bool bOnMouseRenderer, bool bDirectInput)
*/
std::wstring strconv(const std::string& _src)
{
	USES_CONVERSION;
	return std::wstring(A2W(_src.c_str()));
};

std::string strconv(const std::wstring& _src)
{
	USES_CONVERSION;
	return std::string(W2A(_src.c_str()));
};

bool QuestToolCore::save(const string & save_file_name)
{
	UserInterfaceManager::getInstance()->save(save_file_name);

	return true;
}

bool QuestToolCore::load(const string & load_file_name)
{
	UserInterfaceManager::getInstance()->save(load_file_name);
	return true;
}

bool QuestToolCore::initialize(HINSTANCE instance)
{
	if (!GET_SINGLE(CCore)->Init(instance, (TCHAR*)L"Monster Forest", (TCHAR*)L"Monster Forest",
		IDI_ICON1, 1280, 960, true, false, false ))
	{
		DESTROY_SINGLE(CCore);
		return 0;
	}
	UserInterfaceManager::getInstance()->initialize();

	CreateWindow(L"static", L"퀘스트 제목", WS_CHILD | WS_VISIBLE,
		650, 10, 150, 25, GET_SINGLE(CCore)->GetWindowHandle(), (HMENU)-1, GET_SINGLE(CCore)->getWindowInstance(), NULL);

	edit_quest_subject_handle = CreateWindow(L"edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER |
		WS_CLIPCHILDREN,
		650, 40, 250, 25, GET_SINGLE(CCore)->GetWindowHandle(), (HMENU)QUEST_SUBJECT, GET_SINGLE(CCore)->getWindowInstance(), NULL);

	CreateWindow(L"static", L"퀘스트 요약", WS_CHILD | WS_VISIBLE,
		650, 70, 150, 25, GET_SINGLE(CCore)->GetWindowHandle(), (HMENU)-1, GET_SINGLE(CCore)->getWindowInstance(), NULL);

	edit_quest_summary_handle = CreateWindow(L"edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE |
		WS_CLIPCHILDREN,
		650, 100, 250, 60, GET_SINGLE(CCore)->GetWindowHandle(), (HMENU)QUEST_SUMMARY, GET_SINGLE(CCore)->getWindowInstance(), NULL);

	CreateWindow(L"static", L"퀘스트 내용", WS_CHILD | WS_VISIBLE,
		650, 170, 150, 25, GET_SINGLE(CCore)->GetWindowHandle(), (HMENU)-1, GET_SINGLE(CCore)->getWindowInstance(), NULL);

	edit_quest_contents_handle = CreateWindow(L"edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | WS_VSCROLL|
		WS_CLIPCHILDREN,
		650, 200, 250, 420, GET_SINGLE(CCore)->GetWindowHandle(), (HMENU)QUEST_CONTENTS, GET_SINGLE(CCore)->getWindowInstance(), NULL);

	CreateWindow(L"static", L"요구 레벨", WS_CHILD | WS_VISIBLE,
		910, 10, 100, 25, GET_SINGLE(CCore)->GetWindowHandle(), (HMENU)-1, GET_SINGLE(CCore)->getWindowInstance(), NULL);

	edit_minimum_level_handle = CreateWindow(L"edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER |
		WS_CLIPCHILDREN,
		1010, 10, 35, 25, GET_SINGLE(CCore)->GetWindowHandle(), (HMENU)REQUEST_LEVEL_MINIMUM, GET_SINGLE(CCore)->getWindowInstance(), NULL);

	CreateWindow(L"static", L"━", WS_CHILD | WS_VISIBLE,
		1050, 10, 25, 25, GET_SINGLE(CCore)->GetWindowHandle(), (HMENU)-1, GET_SINGLE(CCore)->getWindowInstance(), NULL);

	edit_maximum_level_handle = CreateWindow(L"edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER |
		WS_CLIPCHILDREN,
		1080, 10, 35, 25, GET_SINGLE(CCore)->GetWindowHandle(), (HMENU)REQUEST_LEVEL_MAXIMUM, GET_SINGLE(CCore)->getWindowInstance(), NULL);

	CreateWindow(L"static", L"보상 경험치", WS_CHILD | WS_VISIBLE,
		910, 45, 100, 25, GET_SINGLE(CCore)->GetWindowHandle(), (HMENU)-1, GET_SINGLE(CCore)->getWindowInstance(), NULL);

	edit_reward_experience_handle = CreateWindow(L"edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER |
		WS_CLIPCHILDREN,
		1020, 45, 150, 25, GET_SINGLE(CCore)->GetWindowHandle(), (HMENU)REWARD_EXPERIENCE, GET_SINGLE(CCore)->getWindowInstance(), NULL);

	CreateWindow(L"static", L"보상 골드", WS_CHILD | WS_VISIBLE,
		910, 80, 100, 25, GET_SINGLE(CCore)->GetWindowHandle(), (HMENU)-1, GET_SINGLE(CCore)->getWindowInstance(), NULL);

	edit_reward_gold_handle = CreateWindow(L"edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER |
		WS_CLIPCHILDREN,
		1020, 80, 150, 25, GET_SINGLE(CCore)->GetWindowHandle(), (HMENU)REWARD_GOLD, GET_SINGLE(CCore)->getWindowInstance(), NULL);


	CreateWindow(L"static", L"보상 목록", WS_CHILD | WS_VISIBLE,
		910, 170, 150, 25, GET_SINGLE(CCore)->GetWindowHandle(), (HMENU)-1, GET_SINGLE(CCore)->getWindowInstance(), NULL);

	listbox_reward_items_handle = CreateWindow(L"listbox", NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL |
		WS_BORDER | LBS_NOTIFY | WS_CLIPCHILDREN,
		910, 200, 250, 150, GET_SINGLE(CCore)->GetWindowHandle(), (HMENU)REWARD_LIST, GET_SINGLE(CCore)->getWindowInstance(), NULL);

	button_reward_items_add_handle = CreateWindow(L"button", L"추가", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_CLIPCHILDREN,
		910, 350, 120, 35, GET_SINGLE(CCore)->GetWindowHandle(), (HMENU)ADD_REWARD, GET_SINGLE(CCore)->getWindowInstance(), NULL);

	button_reward_items_remove_handle = CreateWindow(L"button", L"삭제", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_CLIPCHILDREN,
		1040, 350, 120, 35, GET_SINGLE(CCore)->GetWindowHandle(), (HMENU)REMOVE_REWARD, GET_SINGLE(CCore)->getWindowInstance(), NULL);

	CreateWindow(L"static", L"퀘스트 목록", WS_CHILD | WS_VISIBLE,
		910, 395, 150, 25, GET_SINGLE(CCore)->GetWindowHandle(), (HMENU)-1, GET_SINGLE(CCore)->getWindowInstance(), NULL);

	listbox_quest_lists = CreateWindow(L"listbox", NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL |
		WS_BORDER | LBS_NOTIFY | WS_CLIPCHILDREN,
		910, 420, 250, 170, GET_SINGLE(CCore)->GetWindowHandle(), (HMENU)QUEST_LIST, GET_SINGLE(CCore)->getWindowInstance(), NULL);

	button_quest_add_handle = CreateWindow(L"button", L"추가", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_CLIPCHILDREN,
		910, 585, 120, 35, GET_SINGLE(CCore)->GetWindowHandle(), (HMENU)ADD_QUEST, GET_SINGLE(CCore)->getWindowInstance(), NULL);

	button_quest_remove_handle = CreateWindow(L"button", L"삭제", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_CLIPCHILDREN,
		1040, 585, 120, 35, GET_SINGLE(CCore)->GetWindowHandle(), (HMENU)REMOVE_QUEST, GET_SINGLE(CCore)->getWindowInstance(), NULL);

	button_quest_save_handle = CreateWindow(L"button", L"저장", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_CLIPCHILDREN,
		910, 700, 120, 35, GET_SINGLE(CCore)->GetWindowHandle(), (HMENU)SAVE_QUEST, GET_SINGLE(CCore)->getWindowInstance(), NULL);

	button_quest_load_handle = CreateWindow(L"button", L"불러오기", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_CLIPCHILDREN,
		1040, 700, 120, 35, GET_SINGLE(CCore)->GetWindowHandle(), (HMENU)LOAD_QUEST, GET_SINGLE(CCore)->getWindowInstance(), NULL);

	CScene*	pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
	SAFE_RELEASE(pScene);


	GET_SINGLE(CCore)->setWindowProc(clientProc);
	int iRet = GET_SINGLE(CCore)->Run();

	DESTROY_SINGLE(CCore);
	/*
		quest_tool_hwnd = CreateWindowW(pClass, NULL, WS_CHILD | WS_VISIBLE | WS_BORDER |
		WS_CLIPCHILDREN,
		0, 0, 100, 100, hWnd, (HMENU)NULL, m_hInst, nullptr);
	*/

	return iRet;
}

LRESULT clientProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	TCHAR editStr[128];
	const auto& quest_tool_core = QuestToolCore::getInstance();
	static OPENFILENAMEA OFN;
	static char lpstrFile[MAX_PATH] = "";

	switch (msg)
	{
	case WM_CREATE:

		break;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {
			case ADD_REWARD:
			{
				wstring temp_path = GET_SINGLE(CPathManager)->FindPath(BASE_PATH);
				temp_path += L"//Texture/Item/ItemIcon/";
				string load_path = strconv(temp_path);
				memset(&OFN, 0, sizeof(OPENFILENAME));
				OFN.lStructSize = sizeof(OPENFILENAME);
				OFN.hwndOwner = hWnd;
				OFN.lpstrFilter = "All Files(*.*)\0*.*\0";
				OFN.lpstrFile = lpstrFile;
				OFN.nMaxFile = 256;
				OFN.lpstrInitialDir = load_path.c_str();
				if (GetOpenFileNameA(&OFN) != 0) {
					
				}
				MessageBox(NULL, L"추가 완료", L"보상 아이템목록", MB_OK);
			}
			break;

			case REMOVE_REWARD :
			{
				//switch (HIWORD(wParam)) {
				//	case LBN_SELCHANGE:
				//	{
				//		char tagName[128];
				//		int i = SendMessage(QuestToolCore::getInstance()->getListBoxQuestListsHandle(), LB_GETCURSEL, 0, 0);
				//		SendMessageA(QuestToolCore::getInstance()->getListBoxQuestListsHandle(), LB_DELETESTRING, i, 0);

				//		// 리스트에서 보상목록 삭제.
				//		MessageBox(NULL, L"삭제 완료", L"보상 아이템목록", MB_OK);
				//	}
				//	break;
				//}
			}
				break;

			case QUEST_SUBJECT :
			{
				switch (HIWORD(wParam)) {
					case EN_CHANGE:
					{
						char temp_edit_string[128];
						GetWindowTextA(QuestToolCore::getInstance()->getEditQuestSubjectHandle(), temp_edit_string, 128);
						QuestToolCore::getInstance()->setEditQuestSubject(string{ temp_edit_string });
						CGameObject* quest_ui_object = QuestToolCore::getInstance()->getQuestUIObject();

						wstring temp_edit_wstring;
						string temp_prev_convert_contents_string = string{ temp_edit_string };
						temp_edit_wstring = strconv(temp_prev_convert_contents_string);
						CFont* contents_text = quest_ui_object->FindComponentFromTag<CFont>("quest_ui_text_subject");
						contents_text->SetText(temp_edit_wstring);
						QuestToolCore::getInstance()->setEditQuestSubjectText(contents_text);
					}
					break;
				}
			}
				break;

			case QUEST_SUMMARY :
			{
				switch (HIWORD(wParam)) {
					case EN_CHANGE:
					{
						char temp_edit_string[128];
						GetWindowTextA(QuestToolCore::getInstance()->getEditQuestSummaryHandle(), temp_edit_string, 128);
						QuestToolCore::getInstance()->setEditQuestSummary(string{ temp_edit_string });
						CGameObject* quest_ui_object = QuestToolCore::getInstance()->getQuestUIObject();

						wstring temp_edit_wstring;
						string temp_prev_convert_contents_string = string{ temp_edit_string };
						temp_edit_wstring = strconv(temp_prev_convert_contents_string);
						CFont* contents_text = quest_ui_object->FindComponentFromTag<CFont>("quest_ui_text_summary");
						contents_text->SetText(temp_edit_wstring);
						QuestToolCore::getInstance()->setEditQuestSummaryText(contents_text);
					}
					break;
				}
			}
				break;

			case QUEST_CONTENTS :
			{
				switch (HIWORD(wParam)) {
				case EN_CHANGE:
				{
					char temp_edit_string[128];
					GetWindowTextA(QuestToolCore::getInstance()->getEditQuestContentsHandle(), temp_edit_string, 128);
					QuestToolCore::getInstance()->setEditQuestContents(string{ temp_edit_string });
					CGameObject* quest_ui_object = QuestToolCore::getInstance()->getQuestUIObject();

					wstring temp_edit_wstring;
					string temp_prev_convert_contents_string = string{ temp_edit_string };
					temp_edit_wstring = strconv(temp_prev_convert_contents_string);
					CFont* contents_text = quest_ui_object->FindComponentFromTag<CFont>("quest_ui_text_contents");
					contents_text->SetText(temp_edit_wstring);
					QuestToolCore::getInstance()->setEditQuestContentsText(contents_text);
				}
				break;
				}
			}
				break;

			case REWARD_LIST :
			{
				switch (HIWORD(wParam)) {
					case LBN_SELCHANGE:
					{
						// 모든 보상아이템 목록을 보여준다.

					}
					break;
				}
			}
			break;

			case REQUEST_LEVEL_MINIMUM :
			{
				switch (HIWORD(wParam)) {
				case EN_CHANGE:
				{
					char temp_edit_number[128];
					GetWindowTextA(QuestToolCore::getInstance()->getEditMinimumLevelHandle(), temp_edit_number, 128);
					int minimum_level = atoi(temp_edit_number);
					QuestToolCore::getInstance()->setEditMinimumLevel(minimum_level);

				}
				break;
				}
			}
			break;

			case REQUEST_LEVEL_MAXIMUM :
			{
				switch (HIWORD(wParam)) {
				case EN_CHANGE:
				{
					char temp_edit_number[128];
					GetWindowTextA(QuestToolCore::getInstance()->getEditMaximumLevelHandle(), temp_edit_number, 128);
					int maximum_level = atoi(temp_edit_number);
					QuestToolCore::getInstance()->setEditMaximumLevel(maximum_level);

				}
				break;
				}
			}
				break;
			case REWARD_EXPERIENCE :
			{
				switch (HIWORD(wParam)) {
				case EN_CHANGE:
				{
					char temp_edit_number[128];
					GetWindowTextA(QuestToolCore::getInstance()->getEditRewardExperienceHandle(), temp_edit_number, 128);
					QuestToolCore::getInstance()->setEditRewardExperience(atoi(temp_edit_number));
				}
				break;
				}
			}
			break;
			case REWARD_GOLD :
			{
				switch (HIWORD(wParam)) {
				case EN_CHANGE:
				{
					char temp_edit_number[128];
					GetWindowTextA(QuestToolCore::getInstance()->getEditRewardGoldHandle(), temp_edit_number, 128);
					QuestToolCore::getInstance()->setEditRewardGold(atoi(temp_edit_number));
				}
				break;
				}
			}
			break;

			case QUEST_LIST :
			{
				switch (HIWORD(wParam)) {
					case LBN_SELCHANGE:
					{
						char tagName[128];
						int i = SendMessage(QuestToolCore::getInstance()->getListBoxQuestListsHandle(), LB_GETCURSEL, 0, 0);
						SendMessageA(QuestToolCore::getInstance()->getListBoxQuestListsHandle(), LB_GETTEXT, i, (LPARAM)tagName);

						Quest* quest = UserInterfaceManager::getInstance()->findQuest(string{tagName});
						string minimum_level = to_string(quest->getMinimumLevel()).c_str();
						string maximum_level = to_string(quest->getMaximumLevel()).c_str();
						string reward_experience = to_string(quest->getRewardExperience()).c_str();
						string reward_gold = to_string(quest->getRewardGold()).c_str();

						SetWindowTextA(quest_tool_core->getEditQuestSubjectHandle(), (LPCSTR)quest->getQuestSubject().c_str());
						SetWindowTextA(quest_tool_core->getEditQuestSummaryHandle(), (LPCSTR)quest->getQuestSummary().c_str());
						SetWindowTextA(quest_tool_core->getEditQuestContentsHandle(), (LPCSTR)quest->getQuestContents().c_str());
						SetWindowTextA(quest_tool_core->getEditMinimumLevelHandle(), (LPCSTR)minimum_level.c_str());
						SetWindowTextA(quest_tool_core->getEditMaximumLevelHandle(), (LPCSTR)maximum_level.c_str());
						SetWindowTextA(quest_tool_core->getEditRewardExperienceHandle(), (LPCSTR)reward_experience.c_str());
						SetWindowTextA(quest_tool_core->getEditRewardGoldHandle(), (LPCSTR)reward_gold.c_str());
					}
					break;
				}
			}
			break;
			case ADD_QUEST :
			{
				UserInterfaceManager::getInstance()->addQuest(quest_tool_core->getEditQuestSubject(), quest_tool_core->getEditQuestSummary(), quest_tool_core->getEditQuestContents(),
					quest_tool_core->getEditMinimumLevel(), quest_tool_core->getEditMaximumLevel(), quest_tool_core->getEditRewardExperience(),
					quest_tool_core->getEditRewardGold(), quest_tool_core->getRewardItems());
			}
			break;	
			case REMOVE_QUEST :
			{
				switch (HIWORD(wParam)) {
				case LBN_SELCHANGE:
				{
					char tagName[128];
					int i = SendMessage(QuestToolCore::getInstance()->getListBoxQuestListsHandle(), LB_GETCURSEL, 0, 0);
					SendMessageA(QuestToolCore::getInstance()->getListBoxQuestListsHandle(), LB_DELETESTRING, i, 0);

					// 리스트에서 보상목록 삭제.
					MessageBox(NULL, L"삭제 완료", L"보상 아이템목록", MB_OK);
				}
				break;
				}
				MessageBox(NULL, L"삭제 완료", L"퀘스트 목록", MB_OK);
			}
			break;

			case SAVE_QUEST:
			{
				memset(&OFN, 0, sizeof(OPENFILENAME));
				OFN.lStructSize = sizeof(OPENFILENAME);
				OFN.hwndOwner = hWnd;
				OFN.lpstrFilter = "All Files(*.*)\0*.*\0";
				OFN.lpstrFile = lpstrFile;
				OFN.nMaxFile = 256;
				OFN.lpstrInitialDir = "c:\\";
				if (GetSaveFileNameA(&OFN) != 0) {
					quest_tool_core->save(string{ OFN.lpstrFile });
				}
			}
			break;

			case LOAD_QUEST:
			{
				memset(&OFN, 0, sizeof(OPENFILENAME));
				OFN.lStructSize = sizeof(OPENFILENAME);
				OFN.hwndOwner = hWnd;
				OFN.lpstrFilter = "All Files(*.*)\0*.*\0";
				OFN.lpstrFile = lpstrFile;
				OFN.nMaxFile = 256;
				OFN.lpstrInitialDir = "c:\\";
				if (GetOpenFileNameA(&OFN) != 0) {
					quest_tool_core->load(string{ OFN.lpstrFile });
				}
			}
			break;
		}
		break;
	}
	return CallWindowProc(GET_SINGLE(CCore)->getOldProc(), hWnd, msg, wParam, lParam);
}