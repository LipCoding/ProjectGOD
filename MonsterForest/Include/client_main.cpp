
#include "Core.h"
#include "Client.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "SceneScript\MainScene.h"
#include "SceneScript\LoginScene.h"
#include "SceneScript\TestScene.h"
#include "UserInterfaceManager.h"
#include "Chatting.h"
PG_USING

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	if (!GET_SINGLE(CCore)->Init(hInstance, L"Monster Forest", L"Monster Forest",
		IDI_ICON1, 1280, 720, true, true, false))
	{
		DESTROY_SINGLE(CCore);
		return 0;
	}

	CScene*	pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();

	//CMainScene*	pLoginScene = pScene->CreateSceneScript<CMainScene>("MainScene");
	

	pLoginScene = pScene->CreateSceneScript<LoginScene>("LoginScene");
	//CTestScene * pTestScene = pScene->CreateSceneScript<CTestScene>("TestScene");



	SAFE_RELEASE(pScene);

	GET_SINGLE(CCore)->setWindowProc(clientProc);
	int iRet = GET_SINGLE(CCore)->Run();
	SAFE_RELEASE(pLoginScene);
	DESTROY_SINGLE(CCore);

	return iRet;
}


LRESULT clientProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	TCHAR editStr[128];

	switch (msg)
	{
	case WM_CREATE:
		break;
	case WM_CHAR:
		switch (wParam)
		{
		case VK_BACK:
		{
			if (true == static_cast<LoginScene*>(pLoginScene)->id_write)
			{
				if (false == pLoginScene->ID_inputCont.empty())
					pLoginScene->ID_inputCont.pop_back();
			}

			if (true == static_cast<LoginScene*>(pLoginScene)->pw_write)
			{
				if (false == pLoginScene->PW_inputCont.empty())
					pLoginScene->PW_inputCont.pop_back();
			}
			CScene* pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
			if (pScene->GetTag() == "MainScene")
			{
				CMainScene* pSceneScript = static_cast<CMainScene*>(pScene->m_vecSceneScript[0]);
				if (pSceneScript != NULL)
				{
					Chatting* pChatting = GET_SINGLE(UserInterfaceManager)->getChatting();
					if (true == pChatting->isChatWrite())
					{
						if (false == pChatting->getChatCont().empty())
							pChatting->getChatCont().pop_back();
					}
				}
			}
			SAFE_RELEASE(pScene);
		}
		break;

		default:
		{
			if (true == static_cast<LoginScene*>(pLoginScene)->id_write)
			{
				pLoginScene->ID_inputCont.push_back(wParam);
			}

			if (true == static_cast<LoginScene*>(pLoginScene)->pw_write)
			{
				pLoginScene->PW_inputCont.push_back(wParam);
			}

			CScene* pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
			if (pScene->GetTag() == "MainScene")
			{
				CMainScene* pSceneScript = static_cast<CMainScene*>(pScene->m_vecSceneScript[0]);
				if (pSceneScript != NULL)
				{
					Chatting* pChatting = GET_SINGLE(UserInterfaceManager)->getChatting();
					if (true == pChatting->isChatWrite())
					{
						if (pChatting->getChatCont().size() < 30)
							pChatting->getChatCont().push_back(wParam);
					}
				}
			}
			char a = 0;
			/*	const auto& sceneScript = SceneManager::getInstance()->getCurrentScene()->getSceneScript();
				if ("LoginScene" == sceneScript->getTag())
				{
					if (EDITTYPE::EDIT_FOCUS == static_pointer_cast<LoginScene>(sceneScript)->edit_id->getEditType())
						static_pointer_cast<LoginScene>(sceneScript)->edit_id->inputKey(wParam);
					if (EDITTYPE::EDIT_FOCUS == static_pointer_cast<LoginScene>(sceneScript)->edit_pw->getEditType())
						static_pointer_cast<LoginScene>(sceneScript)->edit_pw->inputKey(wParam);
				}
				else
				{
					if (wParam == VK_RETURN)
					{
					}
					else if (EDITTYPE::EDIT_FOCUS == static_pointer_cast<MainScene>(sceneScript)->pChatEdit->getEditType())
						static_pointer_cast<MainScene>(sceneScript)->pChatEdit->inputKey(wParam);
				}*/
		}
		break;
		}

		break;
	}
	return CallWindowProc(GET_SINGLE(CCore)->getOldProc(), hWnd, msg, wParam, lParam);
}