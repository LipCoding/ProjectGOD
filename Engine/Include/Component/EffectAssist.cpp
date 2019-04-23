#include "EffectAssist.h"

PG_USING

CEffectAssist::CEffectAssist()
{
}


CEffectAssist::~CEffectAssist()
{
}

void CEffectAssist::Init(ASSIST_TYPE type)
{
	m_AssistType = type;
}

void CEffectAssist::Update(CGameObject * object, const float& time)
{
	switch (m_AssistType)
	{

	case ASSIST_SCALE:
	{
		break;
	}
	case ASSIST_ROT:
	{
		break;
	}
	case ASSIST_FADE_IN:
	{
		break;
	}
	case ASSIST_FADE_OUT:
	{
		break;
	}
	case ASSIST_UV_ANI:
	{
		break;
	}
	case ASSIST_UV_MOVE:
	{
		break;
	}
	default:
		break;
	}
}
