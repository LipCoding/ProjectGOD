
#include "Core.h"
#include "Client.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "SceneScript\MainScene.h"
#include "SceneScript\StartScene.h"

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

	CStartScene*	pStartScene = pScene->CreateSceneScript<CStartScene>("MainScene");

	SAFE_RELEASE(pStartScene);

	SAFE_RELEASE(pScene);


	int iRet = GET_SINGLE(CCore)->Run();

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
			if (true == static_cast<CStartScene*>(pStartScene)->id_write)
			{
				if (false == pStartScene->ID_inputCont.empty())
					pStartScene->ID_inputCont.pop_back();
			}

			if (true == static_cast<CStartScene*>(pStartScene)->pw_write)
			{
				if (false == pStartScene->PW_inputCont.empty())
					pStartScene->PW_inputCont.pop_back();
			}
		}
		break;

		default:
		{
			if (true == static_cast<CStartScene*>(pStartScene)->id_write)
			{
				pStartScene->ID_inputCont.push_back(wParam);
			}

			if (true == static_cast<CStartScene*>(pStartScene)->pw_write)
			{
				pStartScene->PW_inputCont.push_back(wParam);
			}
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