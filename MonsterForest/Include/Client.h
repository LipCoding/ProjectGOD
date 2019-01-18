
#pragma once

#include "resource.h"
#include "PGEngine.h"

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

