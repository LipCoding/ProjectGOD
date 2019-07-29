#include "global.h"
#include "QuestToolCore.h"
#include "Core.h"
PG_USING

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	if (!QuestToolCore::getInstance()->initialize(hInstance))
	{
		return 0;
	}

	return 0;
}

