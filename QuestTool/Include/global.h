#pragma once
#define NO_WIN32_LEAN_AND_MEAN
#include "../resource.h"
#include "PGEngine.h"
#include <Commdlg.h>
#include <commctrl.h>
#include <ShlObj.h>
#pragma comment(lib, "shlwapi.lib")

#ifdef _WIN64
#ifdef _DEBUG
#pragma comment(lib, "Engine64_Debug")
#else
#pragma comment(lib, "Engine64")
#endif
#else
#ifdef _DEBUG
#pragma comment(lib, "Engine_Debug")
#else
#pragma comment(lib, "Engine")
#endif
#endif // _DEBUG



#define ADD_REWARD					1000
#define REMOVE_REWARD				1001
#define QUEST_SUBJECT				1002
#define QUEST_SUMMARY				1003
#define QUEST_CONTENTS				1004
#define REWARD_LIST					1005
#define REQUEST_LEVEL_MINIMUM		1006
#define REQUEST_LEVEL_MAXIMUM		1007
#define REWARD_GOLD					1008
#define REWARD_EXPERIENCE			1009
#define QUEST_LIST					1010
#define ADD_QUEST					1011
#define REMOVE_QUEST				1012

#define SAVE_QUEST					1100
#define LOAD_QUEST					1101

/*
����Ʈ ���� - QUEST_SUBJECT
����Ʈ ��� - QUEST_SUMMARY
����Ʈ ���� - QUEST_CONTENTS
������ - REWARD_LIST
������ư - REMOVE_REWARD
������ - REWARD_GOLD
�䱸 ���� - REQUEST_LEVEL
*/