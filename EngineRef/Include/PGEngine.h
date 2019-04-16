
#pragma once

#include <WinSock2.h>
#include <Windows.h>
#include <unordered_map>
#include <map>
#include <queue>
#include <list>
#include <vector>
#include <stack>
#include <string>
#include <algorithm>
#include <conio.h>
#include <functional>
#include <fstream>
#include "PGTypes.h"
#include "Protocol.h"
#include "fmod/fmod.hpp"

#include <d2d1.h> // 글자 출력
#include <dwrite.h> // 글자 출력
#include <fstream>
#pragma comment(lib, "DirectXTex")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#pragma comment(lib, "ws2_32")
//----- 폰트엔진
#pragma comment(lib, "d2d1")
#pragma comment(lib, "dwrite")
#pragma comment(lib,"msimg32")
#pragma comment(lib,"fmod64_vc.lib")
using namespace FMOD;

#define	PG_PI	3.141592f

#define WIN32_LEAN_AND_MEAN
#define INITGUID
#define	WM_SOCKET				WM_USER + 1

// Path Key
#define	BASE_PATH	"BasePath"
#define	SHADER_PATH	"ShaderPath"
#define	TEXTURE_PATH	"TexturePath"
#define	DATA_PATH		"DataPath"
#define	SOUND_PATH		"SoundPath"
#define	MESH_PATH		"MeshPath"


// Sampler Key
#define	SAMPLER_POINT	"Point"
#define	SAMPLER_LINEAR	"Linear"


// Shader Key
#define SHADOWMAP_SHADER "ShadowMapShader"
#define SHADOWSTATICOBJECT_SHADER "ShadowStaticObjectShader"
#define SHADOWCHARACTER_SHADER "ShadowCharacterShader"
#define	STANDARD_COLOR_SHADER	"StandardColorShader"
#define	STANDARD_COLOR_FORWARD_SHADER	"StandardColorForwardShader"
#define	STANDARD_COLOR_NORMAL_SHADER	"StandardColorNormalShader"
#define	STANDARD_TEX_SHADER		"StandardTexShader"
#define	STANDARD_TEX_NORMAL_SHADER		"StandardTexNormalShader"
#define	STANDARD_BUMP_SHADER		"StandardBumpShader"
#define	STANDARD_TEX_NORMAL_ANIM_SHADER		"StandardTexNormalAnimShader"
#define	STANDARD_BUMP_ANIM_SHADER		"StandardBumpAnimShader"
#define	STANDARD_BUMP_ANIM_FORWARD_SHADER	"StandardBumpAnimForwardShader"
#define	STANDARD_TEX_NORMAL_ANIM_FORWARD_SHADER		"StandardTexNormalAnimForwardShader"
#define	STANDARD_SHADOWMAP_TEXNORMAL_SHADER					"StandardShadowMapTexNormalShader"
#define	STANDARD_SHADOWMAP_TEXNORMALANIM_SHADER				"StandardShadowMapTexNormalAnimShader"
#define	STANDARD_SHADOWMAP_BUMP_SHADER						"StandardShadowMapTexBumpShader"
#define	STANDARD_SHADOWMAP_BUMPANIM_SHADER					"StandardShadowMapTexBumpAnimShader"
#define	STANDARD_SHADOWMAP_COLOR_SHADER					"StandardShadowMapColorShader"
#define	STANDARD_SHADOWMAP_COLORNORMAL_SHADER					"StandardShadowMapColorNormalShader"

#define	LIGHT_ACC_SHADER		"LightAccShader"
#define	LIGHT_BLEND_SHADER		"LightBlendShader"
#define	DEFERRED_SHADER			"DeferredShader"

#define	LANDSCAPE_SHADER		"LandScapeShader"

#define	COLLIDER_SHADER			"ColliderShader"

#define	UI_SHADER				"UIShader"
#define	UI_MULTITEXTURE_SHADER	"UIMultiTextureShader"
#define	UI_COLOR_SHADER			"UIColorShader"
#define UI_HEARTH_BAR_SHADER "UIHearthBarShader"

#define	SKY_SHADER				"SkyShader"

#define	EFFECT_SHADER			"EffectShader"
#define	DECAL_SHADER			"DecalShader"

// RenderState Key
#define	ALPHA_BLEND	"AlphaBlend"
#define	ONE_BLEND	"OneBlend"

#define	DEPTH_DISABLE	"DepthDisable"
#define	DEPTH_LESS_EQUAL	"DepthLessEqual"

#define	WIRE_FRAME		"WireFrame"
#define CULLING_BACK    "CullBack"
#define	CULLING_NONE	"CullNone"

#define UI_LAYER 1000



template <typename T>
void Safe_Release_VecList(T& p)
{
	T::iterator	iter = p.begin();
	T::iterator	iterEnd = p.end();

	while (iter != iterEnd)
	{
		SAFE_RELEASE((*iter));
		++iter;
	}

	p.clear();
}

template <typename T>
void Safe_Release_Map(T& p)
{
	T::iterator	iter = p.begin();
	T::iterator	iterEnd = p.end();

	while (iter != iterEnd)
	{
		SAFE_RELEASE(iter->second);
		++iter;
	}

	p.clear();
}

template <typename T>
void Safe_Delete_VecList(T& p)
{
	T::iterator	iter = p.begin();
	T::iterator	iterEnd = p.end();

	while (iter != iterEnd)
	{
		SAFE_DELETE((*iter));
		++iter;
	}

	p.clear();
}

template <typename T>
void Safe_Delete_Map(T& p)
{
	T::iterator	iter = p.begin();
	T::iterator	iterEnd = p.end();

	while (iter != iterEnd)
	{
		SAFE_DELETE(iter->second);
		++iter;
	}

	p.clear();
}
