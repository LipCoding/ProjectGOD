#include "QuestToolCore.h"
#include "Core.h"
#include "QuestScene.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "../resource.h"
#include "UserInterfaceManager.h"
#include "Quest.h"
#include "Core/PathManager.h"
#include "Resources/ResourcesManager.h"
#include "QuestResourcesManager.h"
#include "Component/UIButton.h"
#include "Component/Renderer2D.h"
#include "Component/Transform.h"
#include "Component/Material.h"
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
	UserInterfaceManager::getInstance()->load(load_file_name);
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
	QuestResourcesManager::getInstance()->initialize();
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

	edit_quest_contents_handle = CreateWindow(L"edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE |
		WS_CLIPCHILDREN,
		650, 200, 230, 420, GET_SINGLE(CCore)->GetWindowHandle(), (HMENU)QUEST_CONTENTS, GET_SINGLE(CCore)->getWindowInstance(), NULL);

	scroll_quest_contents_offset_handle = CreateWindow(TEXT("scrollbar"), NULL, WS_CHILD | WS_VISIBLE |
			SBS_VERT | WS_CLIPCHILDREN, 880, 200, 20, 420, GET_SINGLE(CCore)->GetWindowHandle(), (HMENU)QUEST_CONTENTS_SCROLL, GET_SINGLE(CCore)->getWindowInstance(), NULL);

	SetScrollRange(scroll_quest_contents_offset_handle, SB_CTL, 0, 10, TRUE);
	SetScrollPos(scroll_quest_contents_offset_handle, SB_CTL, 0, TRUE);

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
		910, 110, 150, 25, GET_SINGLE(CCore)->GetWindowHandle(), (HMENU)-1, GET_SINGLE(CCore)->getWindowInstance(), NULL);

	listbox_reward_items_handle = CreateWindow(L"listbox", NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL |
		WS_BORDER | LBS_NOTIFY | WS_CLIPCHILDREN,
		910, 140, 250, 150, GET_SINGLE(CCore)->GetWindowHandle(), (HMENU)REWARD_LIST, GET_SINGLE(CCore)->getWindowInstance(), NULL);

	button_reward_items_add_handle = CreateWindow(L"button", L"추가", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_CLIPCHILDREN,
		910, 290, 120, 35, GET_SINGLE(CCore)->GetWindowHandle(), (HMENU)ADD_REWARD, GET_SINGLE(CCore)->getWindowInstance(), NULL);

	button_reward_items_remove_handle = CreateWindow(L"button", L"삭제", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_CLIPCHILDREN,
		1040, 290, 120, 35, GET_SINGLE(CCore)->GetWindowHandle(), (HMENU)REMOVE_REWARD, GET_SINGLE(CCore)->getWindowInstance(), NULL);

	CreateWindow(L"static", L"퀘스트 목록", WS_CHILD | WS_VISIBLE,
		910, 335, 150, 25, GET_SINGLE(CCore)->GetWindowHandle(), (HMENU)-1, GET_SINGLE(CCore)->getWindowInstance(), NULL);

	listbox_quest_lists = CreateWindow(L"listbox", NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL |
		WS_BORDER | LBS_NOTIFY | WS_CLIPCHILDREN,
		910, 360, 250, 170, GET_SINGLE(CCore)->GetWindowHandle(), (HMENU)QUEST_LIST, GET_SINGLE(CCore)->getWindowInstance(), NULL);

	button_quest_add_handle = CreateWindow(L"button", L"추가", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_CLIPCHILDREN,
		910, 525, 120, 35, GET_SINGLE(CCore)->GetWindowHandle(), (HMENU)ADD_QUEST, GET_SINGLE(CCore)->getWindowInstance(), NULL);

	button_quest_remove_handle = CreateWindow(L"button", L"삭제", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_CLIPCHILDREN,
		1040, 525, 120, 35, GET_SINGLE(CCore)->GetWindowHandle(), (HMENU)REMOVE_QUEST, GET_SINGLE(CCore)->getWindowInstance(), NULL);

	button_quest_save_handle = CreateWindow(L"button", L"저장", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_CLIPCHILDREN,
		910, 580, 120, 35, GET_SINGLE(CCore)->GetWindowHandle(), (HMENU)SAVE_QUEST, GET_SINGLE(CCore)->getWindowInstance(), NULL);

	button_quest_load_handle = CreateWindow(L"button", L"불러오기", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_CLIPCHILDREN,
		1040, 580, 120, 35, GET_SINGLE(CCore)->GetWindowHandle(), (HMENU)LOAD_QUEST, GET_SINGLE(CCore)->getWindowInstance(), NULL);

	CreateWindow(L"static", L"퀘스트 목록", WS_CHILD | WS_VISIBLE,
		910, 335, 150, 25, GET_SINGLE(CCore)->GetWindowHandle(), (HMENU)-1, GET_SINGLE(CCore)->getWindowInstance(), NULL);

	CreateWindow(L"static", L"보상 아이템", WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
		10, 630, 610, 320, GET_SINGLE(CCore)->GetWindowHandle(), (HMENU)-1, GET_SINGLE(CCore)->getWindowInstance(), NULL);

	scroll_item_render_offset_handle = CreateWindow(L"scrollbar", NULL, WS_CHILD | WS_VISIBLE |
		SBS_VERT | WS_CLIPCHILDREN, 610, 630, 20, 320, GET_SINGLE(CCore)->GetWindowHandle(), (HMENU)ITEM_RENDER_OFFEST_SCROLL, GET_SINGLE(CCore)->getWindowInstance(), NULL);
	
	SetScrollRange(scroll_item_render_offset_handle, SB_CTL, 0, 10, TRUE);
	SetScrollPos(scroll_item_render_offset_handle, SB_CTL, 0, TRUE);

	combo_quest_type = CreateWindow(L"combobox", NULL, WS_CHILD | CBS_DROPDOWN | WS_VSCROLL | WS_VISIBLE | WS_CLIPCHILDREN,
		650, 630, 200, 300, GET_SINGLE(CCore)->GetWindowHandle(), (HMENU)QUEST_TYPE, GET_SINGLE(CCore)->getWindowInstance(), NULL);

	SendMessageA(combo_quest_type, CB_ADDSTRING, 0, (LPARAM)"0. 메인");
	SendMessageA(combo_quest_type, CB_ADDSTRING, 0, (LPARAM)"1. 일반");
	SendMessageA(combo_quest_type, CB_ADDSTRING, 0, (LPARAM)"2. 반복");

	SetWindowTextA(combo_quest_type, "0. 메인");

	CScene*	pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
	SAFE_RELEASE(pScene);


	GET_SINGLE(CCore)->setWindowProc(clientProc);

	CGameObject* quest_ui_object = QuestToolCore::getInstance()->getQuestUIObject();

	CFont* contents_text = quest_ui_object->FindComponentFromTag<CFont>("quest_ui_text_contents");
	contents_text->setMaxOffset(10);


	int iRet = GET_SINGLE(CCore)->Run();

	DESTROY_SINGLE(CCore);

	return iRet;
}

LRESULT clientProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	TCHAR editStr[128];
	const auto& quest_tool_core = QuestToolCore::getInstance();
	static OPENFILENAMEA OFN;
	static char lpstrFile[MAX_PATH] = "";
	static RECT clear_rect{ 10, 630, 610, 960 };
	static bool timer_start = false;

	static int offset = 0;
	static int mouse_x = 0, mouse_y = 0;

	if (timer_start == false)
	{
		SetTimer(hWnd, 1, 1000, NULL);
		timer_start = true;
	}

	switch (msg)
	{
		case WM_VSCROLL:
		{
			switch (LOWORD(wParam)) {
				
				case SB_LINEDOWN:
				{
					if (((HWND)lParam) == QuestToolCore::getInstance()->scroll_item_render_offset_handle)
					{
						offset = min(10, offset + 1);
					}
					else
					{
						CGameObject* quest_ui_object = QuestToolCore::getInstance()->getQuestUIObject();

						CFont* contents_text = quest_ui_object->FindComponentFromTag<CFont>("quest_ui_text_contents");
						contents_text->addOffset();
					}
				}
					break;
				case SB_LINEUP:
				{
					if (((HWND)lParam) == QuestToolCore::getInstance()->scroll_item_render_offset_handle)
					{
						offset = max(0, offset - 1);
					}
					else
					{
						CGameObject* quest_ui_object = QuestToolCore::getInstance()->getQuestUIObject();

						CFont* contents_text = quest_ui_object->FindComponentFromTag<CFont>("quest_ui_text_contents");
						contents_text->subOffset();
					}
				}
					break;
				case SB_THUMBTRACK:
					offset = HIWORD(wParam);
					break;
			}
			if (((HWND)lParam) == QuestToolCore::getInstance()->scroll_item_render_offset_handle)
			{
				string test = to_string(offset);
				OutputDebugStringA(test.c_str());
				InvalidateRect(hWnd, &clear_rect, true);
			}
			SetScrollPos((HWND)lParam, SB_CTL, offset, TRUE);
		}
		break;
	case WM_CREATE:
		
		break;

	case WM_TIMER:
	{
		switch (LOWORD(wParam))
		{
		case 1:
	
			HDC hdc = GetDC(hWnd);
			auto iter_begin = QuestResourcesManager::getInstance()->reward_items_texture.begin();
			auto iter_end = QuestResourcesManager::getInstance()->reward_items_texture.end();
			for (int i = 0, j = 0; iter_begin != iter_end; ++iter_begin, ++j)
			{
				CImage* image = (*iter_begin).second;
				if (j == 9)
				{
					++i;
					j = 0;
				}
				if(630 + ((i - offset) * 64) >= 630)
					image->StretchBlt(hdc, 10 + (j*64), 630 + ((i - offset)*64), 64, 64, 0, 0,  64, 64, SRCCOPY);
				
			}
			ReleaseDC(hWnd, hdc);
			break;
		}
	}
	break;
	
	case WM_LBUTTONDOWN:
	{
		if (quest_tool_core->getRewardItems().size() == 4)
			break;
		mouse_x = LOWORD(lParam);
		mouse_y = HIWORD(lParam);
		// 마우스 위치와 오프셋에 맞는 아이템을 추가한다.
		/// < 오프셋 고려해서 할것. >
		RECT reward_item_area{10, 630, 620, 950};
		if (PtInRect(&reward_item_area, POINT{ mouse_x, mouse_y }))
		{

			int index_x = (mouse_x - 10) / 64;
			int index_y = (mouse_y - 630) / 64;

			int index = ((index_y + offset) * 9) + index_x;
			if ((index < 0) || (index >= QuestResourcesManager::getInstance()->reward_items_texture.size()))
				break;

			if (quest_tool_core->getRewardItems().size() == 0)
			{
				CScene* pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
				CLayer*	pLayer = pScene->GetLayer("UI+1");

				CGameObject* ui_quest_object = CGameObject::CreateObject("reward_text", pLayer);
				ui_quest_object->AddComponent<CUIButton>("reward_text");

				CFont* contents_text_summary = ui_quest_object->AddComponent<CFont>("보상아이템");
				contents_text_summary->SetFont("맑은고딕25N");
				contents_text_summary->SetArea(250, 550, 750, 550.f);
				contents_text_summary->SetText(L"보상 아이템");

				//CTransform* pDropTableTr = ui_quest_object->GetTransform();
				//pDropTableTr->SetWorldScale(64.f, 64.f, 1.f);
				//pDropTableTr->SetWorldPos(300.f, 600.f, 1.f);
				

			}



			string item_name = QuestResourcesManager::getInstance()->reward_items_texture[index].first;
			quest_tool_core->getRewardItems().push_back( item_name );
			SendMessageA(QuestToolCore::getInstance()->getListBoxRewardItemsHandle(), LB_ADDSTRING, 0, (LPARAM)item_name.c_str());

			if (quest_tool_core->getRewardItems().size() == 1)
			{
				// 아이템 보상 테두리
				{
					CScene* pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
					CLayer*	pLayer = pScene->GetLayer("UI+1");

					CGameObject* ui_quest_object = CGameObject::CreateObject("item", pLayer);
					ui_quest_object->AddComponent<CUIButton>("ContentsScroll");

					CRenderer2D* pRenderer = ui_quest_object->FindComponentFromType<CRenderer2D>(CT_RENDERER2D);
					CMaterial* pMaterial = pRenderer->GetMaterial();

					CTransform* pDropTableTr = ui_quest_object->GetTransform();
					pDropTableTr->SetWorldScale(300.f, 70.f, 1.f);
					pDropTableTr->SetWorldPos(250.f, 600.f, 1.f);

					pMaterial->SetDiffuseTexInfo("Linear", "border",
						0, 0, L"UserInterface\\border.png");
				}

				// 아이템 보상 목록에 추가.
				{
					CScene* pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
					CLayer*	pLayer = pScene->GetLayer("UI+1");

					CGameObject* ui_quest_object = CGameObject::CreateObject("itemIcon", pLayer);
					ui_quest_object->AddComponent<CUIButton>("ContentsScroll");

					CRenderer2D* pRenderer = ui_quest_object->FindComponentFromType<CRenderer2D>(CT_RENDERER2D);
					CMaterial* pMaterial = pRenderer->GetMaterial();

					CTransform* pDropTableTr = ui_quest_object->GetTransform();
					pDropTableTr->SetWorldScale(64.f, 64.f, 1.f);
					pDropTableTr->SetWorldPos(250.f, 600.f, 1.f);
					wstring item_text = L"ItemIcon\\";

					item_text += strconv(item_name);
					pMaterial->SetDiffuseTexInfo("Linear", item_name.c_str(),
						0, 0, item_text.c_str());

					CFont* contents_text_summary = ui_quest_object->AddComponent<CFont>("보상아이템");
					contents_text_summary->SetFont("맑은고딕25N");
					contents_text_summary->SetArea(100, 0, 200, 20.f);
					wstring item_text_name = strconv(item_name);
					contents_text_summary->SetText(item_text_name);
				}
			}
			else if (quest_tool_core->getRewardItems().size() == 2)
			{
				/// <테스트>

							// 아이템 보상 테두리
				{
					CScene* pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
					CLayer*	pLayer = pScene->GetLayer("UI+1");

					CGameObject* ui_quest_object = CGameObject::CreateObject("item1", pLayer);
					ui_quest_object->AddComponent<CUIButton>("ContentsScroll");

					CRenderer2D* pRenderer = ui_quest_object->FindComponentFromType<CRenderer2D>(CT_RENDERER2D);
					CMaterial* pMaterial = pRenderer->GetMaterial();

					CTransform* pDropTableTr = ui_quest_object->GetTransform();
					pDropTableTr->SetWorldScale(300.f, 70.f, 1.f);
					pDropTableTr->SetWorldPos(580.f, 600.f, 1.f);

					pMaterial->SetDiffuseTexInfo("Linear", "border",
						0, 0, L"UserInterface\\border.png");
				}

				// 아이템 보상 목록에 추가.
				{
					CScene* pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
					CLayer*	pLayer = pScene->GetLayer("UI+1");

					CGameObject* ui_quest_object = CGameObject::CreateObject("QuestScroll2", pLayer);
					ui_quest_object->AddComponent<CUIButton>("ContentsScroll");

					CRenderer2D* pRenderer = ui_quest_object->FindComponentFromType<CRenderer2D>(CT_RENDERER2D);
					CMaterial* pMaterial = pRenderer->GetMaterial();

					CTransform* pDropTableTr = ui_quest_object->GetTransform();
					pDropTableTr->SetWorldScale(64.f, 64.f, 1.f);
					pDropTableTr->SetWorldPos(580.f, 600.f, 1.f);
					wstring item_text = L"ItemIcon\\";

					item_text += strconv(item_name);
					pMaterial->SetDiffuseTexInfo("Linear", item_name.c_str(),
						0, 0, item_text.c_str());

					CFont* contents_text_summary = ui_quest_object->AddComponent<CFont>("보상아이템");
					contents_text_summary->SetFont("맑은고딕25N");
					contents_text_summary->SetArea(100, 0, 200, 20.f);
					wstring item_text_name = strconv(item_name);
					contents_text_summary->SetText(item_text_name);
				}
			}
			else if (quest_tool_core->getRewardItems().size() == 3)
			{
				/// <테스트>

							// 아이템 보상 테두리
				{
					CScene* pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
					CLayer*	pLayer = pScene->GetLayer("UI+1");

					CGameObject* ui_quest_object = CGameObject::CreateObject("item1", pLayer);
					ui_quest_object->AddComponent<CUIButton>("ContentsScroll");

					CRenderer2D* pRenderer = ui_quest_object->FindComponentFromType<CRenderer2D>(CT_RENDERER2D);
					CMaterial* pMaterial = pRenderer->GetMaterial();

					CTransform* pDropTableTr = ui_quest_object->GetTransform();
					pDropTableTr->SetWorldScale(300.f, 70.f, 1.f);
					pDropTableTr->SetWorldPos(250.f, 700.f, 1.f);

					pMaterial->SetDiffuseTexInfo("Linear", "border",
						0, 0, L"UserInterface\\border.png");
				}

				// 아이템 보상 목록에 추가.
				{
					CScene* pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
					CLayer*	pLayer = pScene->GetLayer("UI+1");

					CGameObject* ui_quest_object = CGameObject::CreateObject("QuestScroll2", pLayer);
					ui_quest_object->AddComponent<CUIButton>("ContentsScroll");

					CRenderer2D* pRenderer = ui_quest_object->FindComponentFromType<CRenderer2D>(CT_RENDERER2D);
					CMaterial* pMaterial = pRenderer->GetMaterial();

					CTransform* pDropTableTr = ui_quest_object->GetTransform();
					pDropTableTr->SetWorldScale(64.f, 64.f, 1.f);
					pDropTableTr->SetWorldPos(250.f, 700.f, 1.f);
					wstring item_text = L"ItemIcon\\";

					item_text += strconv(item_name);
					pMaterial->SetDiffuseTexInfo("Linear", item_name.c_str(),
						0, 0, item_text.c_str());

					CFont* contents_text_summary = ui_quest_object->AddComponent<CFont>("보상아이템");
					contents_text_summary->SetFont("맑은고딕25N");
					contents_text_summary->SetArea(100, 0, 200, 20.f);
					wstring item_text_name = strconv(item_name);
					contents_text_summary->SetText(item_text_name);
				}
			}
			else if (quest_tool_core->getRewardItems().size() == 4)
			{
			/// <테스트>

						// 아이템 보상 테두리
				{
					CScene* pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
					CLayer*	pLayer = pScene->GetLayer("UI+1");

					CGameObject* ui_quest_object = CGameObject::CreateObject("item1", pLayer);
					ui_quest_object->AddComponent<CUIButton>("ContentsScroll");

					CRenderer2D* pRenderer = ui_quest_object->FindComponentFromType<CRenderer2D>(CT_RENDERER2D);
					CMaterial* pMaterial = pRenderer->GetMaterial();

					CTransform* pDropTableTr = ui_quest_object->GetTransform();
					pDropTableTr->SetWorldScale(300.f, 70.f, 1.f);
					pDropTableTr->SetWorldPos(580.f, 700.f, 1.f);

					pMaterial->SetDiffuseTexInfo("Linear", "border",
						0, 0, L"UserInterface\\border.png");
				}

				// 아이템 보상 목록에 추가.
				{
					CScene* pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
					CLayer*	pLayer = pScene->GetLayer("UI+1");

					CGameObject* ui_quest_object = CGameObject::CreateObject("QuestScroll2", pLayer);
					ui_quest_object->AddComponent<CUIButton>("ContentsScroll");

					CRenderer2D* pRenderer = ui_quest_object->FindComponentFromType<CRenderer2D>(CT_RENDERER2D);
					CMaterial* pMaterial = pRenderer->GetMaterial();

					CTransform* pDropTableTr = ui_quest_object->GetTransform();
					pDropTableTr->SetWorldScale(64.f, 64.f, 1.f);
					pDropTableTr->SetWorldPos(580.f, 700.f, 1.f);
					wstring item_text = L"ItemIcon\\";

					item_text += strconv(item_name);
					pMaterial->SetDiffuseTexInfo("Linear", item_name.c_str(),
						0, 0, item_text.c_str());

					CFont* contents_text_summary = ui_quest_object->AddComponent<CFont>("보상아이템");
					contents_text_summary->SetFont("맑은고딕25N");
					contents_text_summary->SetArea(100, 0, 200, 20.f);
					wstring item_text_name = strconv(item_name);
					contents_text_summary->SetText(item_text_name);
				}
			}
		}
	}
	break;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {
			case ADD_REWARD:
			{
				//wstring temp_path = GET_SINGLE(CPathManager)->FindPath(BASE_PATH);
				//temp_path += L"//Texture/Item/ItemIcon/";
				//string load_path = strconv(temp_path);
				//memset(&OFN, 0, sizeof(OPENFILENAME));
				//OFN.lStructSize = sizeof(OPENFILENAME);
				//OFN.hwndOwner = hWnd;
				//OFN.lpstrFilter = "All Files(*.*)\0*.*\0";
				//OFN.lpstrFile = lpstrFile;
				//OFN.nMaxFile = 256;
				//OFN.lpstrInitialDir = load_path.c_str();
				//if (GetOpenFileNameA(&OFN) != 0) {

				//}

				//char file_name[_MAX_FNAME];
				//char file_ext[_MAX_EXT];
				//_splitpath(lpstrFile, NULL, NULL, file_name, file_ext);

				//string middle_path = "\\ItemIcon";
				//string load_file_name = string{ file_name } +string{ file_ext };
				//GET_SINGLE(CResourcesManager)->LoadTexture(file_name, load_file_name.c_str(), TEXTURE_PATH);

				//auto finder = find(quest_tool_core->getRewardItems().begin(), quest_tool_core->getRewardItems().end(), string{ file_name });
				//if (finder == quest_tool_core->getRewardItems().end())
				//{
				//	quest_tool_core->getRewardItems().push_back(string{ file_name });
				//	SendMessageA(QuestToolCore::getInstance()->getListBoxRewardItemsHandle(), LB_ADDSTRING, 0, (LPARAM)file_name);

				//	//// 아이템 보상 목록에 추가.
				//	//{
				//	//	CScene* pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
				//	//	CLayer*	pLayer = pScene->GetLayer("UI+1");

				//	//	CGameObject* ui_quest_object = CGameObject::CreateObject("QuestScroll", pLayer);
				//	//	ui_quest_object->AddComponent<CUIButton>("ContentsScroll");

				//	//	CRenderer2D* pRenderer = ui_quest_object->FindComponentFromType<CRenderer2D>(CT_RENDERER2D);
				//	//	CMaterial* pMaterial = pRenderer->GetMaterial();

				//	//	CTransform* pDropTableTr = ui_quest_object->GetTransform();
				//	//	pDropTableTr->SetWorldScale(64.f, 64.f, 1.f);
				//	//	pDropTableTr->SetWorldPos(350.f, 780.f, 1.f);
				//	//	wstring item_text = L"ItemIcon";
				//	//	item_text += strconv( file_name );
				//	//	pMaterial->SetDiffuseTexInfo("Linear", "scroll",
				//	//		0, 0, item_text.c_str());
				//	//}

				//}
			}
			break;

			case REMOVE_REWARD:
			{
				switch (HIWORD(wParam)) {
				case LBN_SELCHANGE:
				{
					char reward_item_name[128];
					int i = SendMessage(QuestToolCore::getInstance()->getListBoxQuestListsHandle(), LB_GETCURSEL, 0, 0);
					SendMessageA(QuestToolCore::getInstance()->getListBoxQuestListsHandle(), LB_GETTEXT, i, (LPARAM)reward_item_name);
					SendMessageA(QuestToolCore::getInstance()->getListBoxQuestListsHandle(), LB_DELETESTRING, i, 0);



					// 리스트에서 보상목록 삭제.
					MessageBox(NULL, L"삭제 완료", L"보상 아이템목록", MB_OK);
				}
				break;
				}
			}
			break;

			case QUEST_SUBJECT:
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

			case QUEST_SUMMARY:
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

			case QUEST_CONTENTS:
			{
				switch (HIWORD(wParam)) {
				case EN_CHANGE:
				{
#pragma region temp1
					/*
					// 에디트박스로부터 문자열 받아옴.
					char temp_edit_string[1500];
					GetWindowTextA(QuestToolCore::getInstance()->getEditQuestContentsHandle(), temp_edit_string, 1500);

					// 컨텐츠 문자열 변경.
					QuestToolCore::getInstance()->setEditQuestContents(string{ temp_edit_string });
					
					CGameObject* quest_ui_object = QuestToolCore::getInstance()->getQuestUIObject();

					//컨버팅 받을 문자열
					wstring temp_edit_wstring;
					// 문자열 컨버팅
					string temp_prev_convert_contents_string = string{ temp_edit_string };
					temp_edit_wstring = strconv(temp_prev_convert_contents_string);

					
					// 폰트에 연결된 문자열 변경.
					CFont* contents_text = quest_ui_object->FindComponentFromTag<CFont>("quest_ui_text_contents");
					contents_text->SetText(temp_edit_wstring);
					QuestToolCore::getInstance()->setEditQuestContentsText(contents_text);
					*/
#pragma endregion
					char temp_edit_string[1500];
					GetWindowTextA(QuestToolCore::getInstance()->getEditQuestContentsHandle(), temp_edit_string, 1500);

					vector<string>& contents_strings = QuestToolCore::getInstance()->getContentsStrings();
					contents_strings.clear();
					string edit_string = temp_edit_string;
					
					string temp_string;
					int size = edit_string.size() - 25;
					if (size >= 0)
					{
						// 몫으로 구해서 단위를한다.
						// 25개 단위로 문자열을 채워넣는다.
						int lines = edit_string.size() / 25;
						_cprintf("%d", lines);

						for (int index = 0; index < lines; ++index)
						{
							// 총 3줄만 나오게 하는 작업.
							if (index > 2)
								break;

							temp_string.clear();
							for (int j = 0; j < 25; ++j)
							{
								temp_string += edit_string[(index*25)+j];
							}
							contents_strings.push_back(temp_string);
						}
						// 총 3줄만 나오게 하는 조건.
						if (lines < 3)
						{
							// 모듈연산으로 몫으로 계산한 라인 뒤부터남은 문자열을 채워넣는다.
							int words = edit_string.size() % 25;
							if (words != 0)
							{
								temp_string.clear();
								for (int index = 0; index < words; ++index)
								{

									temp_string += edit_string[(lines * 25) + index];
								}
								contents_strings.push_back(temp_string);
							}
						}
					}
					else
					{
						contents_strings.push_back(edit_string);
						temp_string.clear();
					}

					// 컨텐츠 문자열 변경.
					QuestToolCore::getInstance()->setEditQuestContents(string{ temp_edit_string });

					CGameObject* quest_ui_object = QuestToolCore::getInstance()->getQuestUIObject();

					//컨버팅 받을 문자열
					wstring temp_edit_wstring;
					
					string view_string;

					for (const string& string : contents_strings)
					{
						view_string += string + '\n';
					}

					temp_edit_wstring = strconv(view_string);
					// 폰트에 연결된 문자열 변경.
					CFont* contents_text = quest_ui_object->FindComponentFromTag<CFont>("quest_ui_text_contents");
					contents_text->SetText(temp_edit_wstring);
					QuestToolCore::getInstance()->setEditQuestContentsText(contents_text);
				}
				break;
				}
			}
			break;

			case REWARD_LIST:
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

			case REQUEST_LEVEL_MINIMUM:
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

			case REQUEST_LEVEL_MAXIMUM:
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
			case REWARD_EXPERIENCE:
			{
				switch (HIWORD(wParam)) {
				case EN_CHANGE:
				{
					char temp_edit_number[128];
					GetWindowTextA(QuestToolCore::getInstance()->getEditRewardExperienceHandle(), temp_edit_number, 128);
					QuestToolCore::getInstance()->setEditRewardExperience(atoi(temp_edit_number));
					CFont* exp_font_component = UserInterfaceManager::getInstance()->quest_exp_object->FindComponentFromTag<CFont>("EXP");
					wstring gold = L"경험치 ";
					gold += strconv(temp_edit_number);
					exp_font_component->SetText(gold);
				}
				break;
				}
			}
			break;
			case REWARD_GOLD:
			{
				switch (HIWORD(wParam)) {
				case EN_CHANGE:
				{
					char temp_edit_number[128];
					GetWindowTextA(QuestToolCore::getInstance()->getEditRewardGoldHandle(), temp_edit_number, 128);
					QuestToolCore::getInstance()->setEditRewardGold(atoi(temp_edit_number));
					CFont* gold_font_component = UserInterfaceManager::getInstance()->quest_gold_object->FindComponentFromTag<CFont>("GOLD");
					wstring exp = L"X ";
					exp += strconv(temp_edit_number);
					gold_font_component->SetText(exp);
				}
				break;
				}
			}
			break;

			case QUEST_LIST:
			{
				switch (HIWORD(wParam)) {
				case LBN_SELCHANGE:
				{
					char tagName[128];
					int i = SendMessage(QuestToolCore::getInstance()->getListBoxQuestListsHandle(), LB_GETCURSEL, 0, 0);
					SendMessageA(QuestToolCore::getInstance()->getListBoxQuestListsHandle(), LB_GETTEXT, i, (LPARAM)tagName);

					Quest* quest = UserInterfaceManager::getInstance()->findQuest(string{ tagName });
					string minimum_level = to_string(quest->getMinimumLevel()).c_str();
					string maximum_level = to_string(quest->getMaximumLevel()).c_str();
					string reward_experience = to_string(quest->getRewardExperience()).c_str();
					string reward_gold = to_string(quest->getRewardGold()).c_str();

					SetWindowTextA(quest_tool_core->getEditQuestSubjectHandle(), (LPCSTR)quest->getQuestSubject().c_str());
					SetWindowTextA(quest_tool_core->getEditQuestSummaryHandle(), (LPCSTR)quest->getQuestSummary().c_str());
					SetWindowTextA(quest_tool_core->getEditQuestContentsHandle(), (LPCSTR)quest->getQuestContents().c_str());
					wstring temp_text;
					CGameObject* quest_ui_object = QuestToolCore::getInstance()->getQuestUIObject();
					CFont* contents_text = quest_ui_object->FindComponentFromTag<CFont>("quest_ui_text_subject");
					temp_text = strconv(quest->getQuestSubject());
					contents_text->SetText(temp_text);

					contents_text = quest_ui_object->FindComponentFromTag<CFont>("quest_ui_text_summary");
					temp_text = strconv(quest->getQuestSummary());
					contents_text->SetText(temp_text);

					contents_text = quest_ui_object->FindComponentFromTag<CFont>("quest_ui_text_contents");
					temp_text = strconv(quest->getQuestContents());
					contents_text->SetText(temp_text);

					SetWindowTextA(quest_tool_core->getEditMinimumLevelHandle(), (LPCSTR)minimum_level.c_str());
					SetWindowTextA(quest_tool_core->getEditMaximumLevelHandle(), (LPCSTR)maximum_level.c_str());
					SetWindowTextA(quest_tool_core->getEditRewardExperienceHandle(), (LPCSTR)reward_experience.c_str());
					SetWindowTextA(quest_tool_core->getEditRewardGoldHandle(), (LPCSTR)reward_gold.c_str());

					{
						int count = SendMessage(QuestToolCore::getInstance()->getListBoxRewardItemsHandle(), LB_GETCOUNT, 0, 0);
						for (int i = count - 1; i >= 0; --i)
							SendMessageA(QuestToolCore::getInstance()->getListBoxRewardItemsHandle(), LB_DELETESTRING, i, 0);
					}
					for (const auto& item_name : quest->getRewardItems())
					{
						quest_tool_core->getRewardItems().push_back(item_name);
						SendMessageA(QuestToolCore::getInstance()->getListBoxRewardItemsHandle(), LB_ADDSTRING, 0, (LPARAM)item_name.c_str());
					}
				}
				break;
				}
			}
			break;
			case ADD_QUEST:
			{
				UserInterfaceManager::getInstance()->addQuest(quest_tool_core->getEditQuestSubject(), quest_tool_core->getEditQuestSummary(), quest_tool_core->getEditQuestContents(),
					quest_tool_core->getEditMinimumLevel(), quest_tool_core->getEditMaximumLevel(), quest_tool_core->getEditRewardExperience(),
					quest_tool_core->getEditRewardGold(), quest_tool_core->getRewardItems());

				quest_tool_core->getRewardItems().clear();
				SetWindowText(QuestToolCore::getInstance()->getEditQuestSubjectHandle(), L"");
				SetWindowText(QuestToolCore::getInstance()->getEditQuestSummaryHandle(), L"");
				SetWindowText(QuestToolCore::getInstance()->getEditQuestContentsHandle(), L"");
				SetWindowText(QuestToolCore::getInstance()->getEditMinimumLevelHandle(), L"");
				SetWindowText(QuestToolCore::getInstance()->getEditMaximumLevelHandle(), L"");
				SetWindowText(QuestToolCore::getInstance()->getEditRewardExperienceHandle(), L"");
				SetWindowText(QuestToolCore::getInstance()->getEditRewardGoldHandle(), L"");

				{
					QuestToolCore::getInstance()->setEditQuestSubject(string{ "" });
					CGameObject* quest_ui_object = QuestToolCore::getInstance()->getQuestUIObject();

					CFont* contents_text = quest_ui_object->FindComponentFromTag<CFont>("quest_ui_text_subject");
					contents_text->SetText(L"");
					QuestToolCore::getInstance()->setEditQuestSubjectText(contents_text);
				}

				{
					QuestToolCore::getInstance()->setEditQuestSummary(string{ "" });
					CGameObject* quest_ui_object = QuestToolCore::getInstance()->getQuestUIObject();
					CFont* contents_text = quest_ui_object->FindComponentFromTag<CFont>("quest_ui_text_summary");
					contents_text->SetText(L"");
					QuestToolCore::getInstance()->setEditQuestSummaryText(contents_text);
				}

				{
					QuestToolCore::getInstance()->setEditQuestContents(string{ "" });
					CGameObject* quest_ui_object = QuestToolCore::getInstance()->getQuestUIObject();

					CFont* contents_text = quest_ui_object->FindComponentFromTag<CFont>("quest_ui_text_contents");
					contents_text->SetText(L"");
					QuestToolCore::getInstance()->setEditQuestContentsText(contents_text);
				}

				int count = SendMessage(QuestToolCore::getInstance()->getListBoxRewardItemsHandle(), LB_GETCOUNT, 0, 0);
				for(int i = count-1; i >= 0; --i)
					SendMessageA(QuestToolCore::getInstance()->getListBoxRewardItemsHandle(), LB_DELETESTRING, i, 0);
			}
			break;
			case REMOVE_QUEST:
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

				int count = SendMessage(QuestToolCore::getInstance()->getListBoxQuestListsHandle(), LB_GETCOUNT, 0, 0);
				for (int i = count - 1; i >= 0; --i)
					SendMessageA(QuestToolCore::getInstance()->getListBoxQuestListsHandle(), LB_DELETESTRING, i, 0);

				for (const auto& quest : UserInterfaceManager::getInstance()->getQuests())
				{
					string quest_name = quest->getQuestSubject();
					SendMessageA(QuestToolCore::getInstance()->getListBoxQuestListsHandle(), LB_ADDSTRING, 0, (LPARAM)quest_name.c_str());
				}

			}
			break;
		}
		break;
	}
	return CallWindowProc(GET_SINGLE(CCore)->getOldProc(), hWnd, msg, wParam, lParam);
}