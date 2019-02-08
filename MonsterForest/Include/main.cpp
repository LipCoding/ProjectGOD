
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

	CMainScene*	pStartScene = pScene->CreateSceneScript<CMainScene>("MainScene");

	SAFE_RELEASE(pStartScene);

	SAFE_RELEASE(pScene);


	int iRet = GET_SINGLE(CCore)->Run();

	DESTROY_SINGLE(CCore);

	return iRet;
}
