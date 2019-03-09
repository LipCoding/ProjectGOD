
#pragma once

#include "PGMacro.h"

PG_BEGIN

// Shader Type
enum SHADER_TYPE
{
	ST_VERTEX,
	ST_PIXEL,
	ST_GEOMETRY,
	ST_MAX
};

// Component Type
enum COMPONENT_TYPE
{
	CT_TRANSFORM,
	CT_RENDERER2D,
	CT_RENDERER,
	CT_SCRIPT,
	CT_CAMERA,
	CT_MATERIAL,
	CT_ANIMATION2D,
	CT_COLLIDER,
	CT_TERRAIN2D,
	CT_TILE,
	CT_MOUSE,
	CT_UI,
	CT_NAVIGATION,
	CT_NAVIGATION3D,
	CT_LIGHT,
	CT_THIRDCAMERA,
	CT_ARM,
	CT_LANDSCAPE,
	CT_PARTICLE_SINGLE,
	CT_ANIMATION,
	CT_DECAL,
	CT_AXIS,
	CT_END
};

// Light Type
enum LIGHT_TYPE
{
	LT_DIR,
	LT_POINT,
	LT_SPOT
};

// Collider Type
enum COLLIDER_TYPE
{
	CT_RECT,
	CT_SPHERE,
	CT_POINT,
	CT_PIXEL,
	CT_OBB2D,
	CT_AABB,
	CT_OBB,
	CT_RAY
};

// View Type
enum VIEW_TYPE
{
	VT_NORMAL,
	VT_UI
};

enum AXIS
{
	AXIS_NONE = -1,
	AXIS_X,
	AXIS_Y,
	AXIS_Z,
	AXIS_END
};

// Shader Constant Type
enum SHADER_CONSTANT_TYPE
{
	SCT_VERTEX = 0x1,
	SCT_PIXEL = 0x2,
	SCT_GEOMETRY = 0x4
};

// Camera Type
enum CAMERA_TYPE
{
	CT_PERSPECTIVE,
	CT_ORTHO
};

// RenderState Type
enum RENDERSTATE_TYPE
{
	RST_BLEND,
	RST_RASTERIZER,
	RST_DEPTH,
	RST_END
};

// Animation2D Type
enum ANIMATION2D_TYPE
{
	A2D_NONE = -1,
	A2D_ATLAS,
	A2D_FRAME,
	A2D_END
};

// Animation2D Option
enum ANIMATION2D_OPTION
{
	A2DO_LOOP,
	A2DO_ONCE_RETURN,
	A2DO_ONCE_DESTROY,
	A2DO_COUNT_RETURN,
	A2DO_COUNT_DESTROY,
	A2DO_TIME_RETURN,
	A2DO_TIME_DESTROY,
	A2DO_END
};

// Animation Type
enum ANIMATION_TYPE
{
	AT_NONE = -1,
	AT_ATLAS,
	AT_FRAME,
	AT_END
};

// Animation Option
enum ANIMATION_OPTION
{
	AO_LOOP,
	AO_ONCE_RETURN,
	AO_ONCE_DESTROY,
	AO_COUNT_RETURN,
	AO_COUNT_DESTROY,
	AO_TIME_RETURN,
	AO_TIME_DESTROY,
	AO_END
};

// Terrain Tile Type
enum TERRAIN2D_TILE_TYPE
{
	TILE_NONE,
	TILE_ISOMETRIC,
	TILE_RECT,
	TILE_END
};

// Terrain Tile Option
enum TERRAIN2D_TILE_OPTION
{
	TTO_NONE,
	TTO_NOMOVE
};

// Scene Change
enum SCENE_CHANGE
{
	SC_NONE,
	SC_CHANGE
};

// Parent Modify Type
enum PARENT_MODIFY_TYPE
{
	PMT_NONE,
	PMT_SCALE = 0x1,
	PMT_ROT = 0x2,
	PMT_POS = 0x4,
	PMT_ALL = PMT_SCALE | PMT_ROT | PMT_POS
};


PG_END
