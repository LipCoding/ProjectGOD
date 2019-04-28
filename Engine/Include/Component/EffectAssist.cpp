#include "EffectAssist.h"
#include "../Ease.h"
#include "Transform.h"
#include "../GameObject/GameObject.h"
#include "../Component/Renderer.h"

PG_USING

SHARECBUFFER CEffectAssist::g_tShareBuffer{};

CEffectAssist::CEffectAssist()
{
}


CEffectAssist::~CEffectAssist()
{
}

void CEffectAssist::Init(CGameObject *object, ASSIST_TYPE AssistType, EASE_TYPE easeType)
{
	m_AssistType = AssistType;
	m_EaseType = easeType;

	FirstStatusSet(object);

	CRenderer* pRenderer = object->FindComponentFromType<CRenderer>(CT_RENDERER);
	pRenderer->CreateCBuffer("Share", 8, sizeof(SHARECBUFFER), SCT_PIXEL);
	SAFE_RELEASE(pRenderer);
}

void CEffectAssist::Update(CGameObject * object, const float& deltaTime)
{
	if (m_StartCheck == false)
	{
		ReturnToFirstSet(object);
		return;
	}

	m_Time += deltaTime;

	if (m_Time >= m_StartTime)
	{
		switch (m_AssistType)
		{
		case ASSIST_SCALE:
		{
			CTransform *pTr = object->GetTransform();
			Vector3 vScale;
			vScale.x = Calc_Ease(m_EaseType, m_StartX, m_PowerX, m_LifeTime);
			vScale.y = Calc_Ease(m_EaseType, m_StartY, m_PowerY, m_LifeTime);
			vScale.z = Calc_Ease(m_EaseType, m_StartZ, m_PowerZ, m_LifeTime);
			pTr->SetWorldScale(vScale);
			SAFE_RELEASE(pTr);
			break;
		}
		case ASSIST_ROT:
		{
			CTransform *pTr = object->GetTransform();
			Vector3 vRot;
			vRot.x = XMConvertToRadians(Calc_Ease(m_EaseType, m_StartX, m_PowerX, m_LifeTime));
			vRot.y = XMConvertToRadians(Calc_Ease(m_EaseType, m_StartY, m_PowerY, m_LifeTime));
			vRot.z = XMConvertToRadians(Calc_Ease(m_EaseType, m_StartZ, m_PowerZ, m_LifeTime));
			pTr->SetWorldRot(vRot);
			SAFE_RELEASE(pTr);
			break;
		}
		case ASSIST_FADE_IN:
		{
			float fAlpha = Calc_Ease(m_EaseType, m_StartFadeIn, m_Degree, m_LifeTime);

			if (fAlpha >= 1.f)
				fAlpha = 1.f;

			g_tShareBuffer.fAlphaFadeOut = 1.f;
			g_tShareBuffer.fAlphaFadeIn = fAlpha;
			g_tShareBuffer.vColor = Vector4{ 0.f, 0.f, 0.f, 0.f };

			CRenderer* pRenderer = object->FindComponentFromType<CRenderer>(CT_RENDERER);
			pRenderer->UpdateCBuffer("Share", 8, sizeof(SHARECBUFFER), SCT_PIXEL, &g_tShareBuffer);
			SAFE_RELEASE(pRenderer);

			break;
		}
		case ASSIST_FADE_OUT:
		{
			g_tShareBuffer.fAlphaFadeIn = 0.f;
			g_tShareBuffer.fAlphaFadeOut = Calc_Ease(m_EaseType, m_StartFadeOut, m_Degree, m_LifeTime);
			g_tShareBuffer.vColor = Vector4{ 0.f, 0.f, 0.f, 0.f };

			CRenderer* pRenderer = object->FindComponentFromType<CRenderer>(CT_RENDERER);
			pRenderer->UpdateCBuffer("Share", 8, sizeof(SHARECBUFFER), SCT_PIXEL, &g_tShareBuffer);
			SAFE_RELEASE(pRenderer);

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

	if (m_Time >= m_EndTime)
	{
		ReturnToFirstSet(object);

		/* Fade */
		g_tShareBuffer.fAlphaFadeIn = 0.f;
		g_tShareBuffer.fAlphaFadeOut = 0.f;
		g_tShareBuffer.vColor = Vector4{ 0.f, 0.f, 0.f, 0.f };

		CRenderer* pRenderer = object->FindComponentFromType<CRenderer>(CT_RENDERER);
		pRenderer->UpdateCBuffer("Share", 8, sizeof(SHARECBUFFER), SCT_PIXEL, &g_tShareBuffer);
		SAFE_RELEASE(pRenderer);

		m_StartCheck = false;
	}
}

void CEffectAssist::FirstStatusSet(CGameObject *object)
{
	m_LifeTime = m_EndTime - m_StartTime;
	m_Time = 0.f;

	switch (m_AssistType)
	{
	case ASSIST_SCALE:
	{
		CTransform *pTr = object->GetTransform();
		Vector3 vScale = pTr->GetWorldScale();
		m_StartX = vScale.x;
		m_StartY = vScale.y;
		m_StartZ = vScale.z;
		SAFE_RELEASE(pTr);
		break;
	}
	case ASSIST_ROT:
	{
		CTransform *pTr = object->GetTransform();
		Vector3 vRot = pTr->GetWorldRot();
		m_StartX = XMConvertToDegrees(vRot.x);
		m_StartY = XMConvertToDegrees(vRot.y);
		m_StartZ = XMConvertToDegrees(vRot.z);
		SAFE_RELEASE(pTr);
		break;
	}
	case ASSIST_FADE_IN:
	{
		m_StartFadeIn = 0.f;
		break;
	}
	case ASSIST_FADE_OUT:
	{
		m_StartFadeOut = 0.f;
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

void CEffectAssist::ReturnToFirstSet(CGameObject * object)
{
	m_Time = 0.f;
	m_LifeTime = m_EndTime - m_StartTime;

	switch (m_AssistType)
	{
	case ASSIST_SCALE:
	{
		CTransform *pTr = object->GetTransform();
		pTr->SetWorldScale(m_StartX, m_StartY, m_StartZ);
		SAFE_RELEASE(pTr);
		break;
	}
	case ASSIST_ROT:
	{
		CTransform *pTr = object->GetTransform();
		pTr->SetWorldRot(XMConvertToRadians(m_StartX), XMConvertToRadians(m_StartY), XMConvertToRadians(m_StartZ));
		SAFE_RELEASE(pTr);
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

float CEffectAssist::Calc_Ease(EASE_TYPE type, const float & startValue, const float & variation, const float & duration)
{
	float fTime = m_Time - m_StartTime;

	switch (type)
	{
		case EASE_NONE			:
			return CEase_None::easeNone(fTime, startValue, variation, duration);
		case EASE_SINE_IN		:
			return CEase_Sine::easeIn(fTime, startValue, variation, duration);
		case EASE_SINE_OUT		:
			return CEase_Sine::easeOut(fTime, startValue, variation, duration);
		case EASE_QUAD_IN		:
			return CEase_Quad::easeIn(fTime, startValue, variation, duration);
		case EASE_QUAD_OUT		:
			return CEase_Quad::easeOut(fTime, startValue, variation, duration);
		case EASE_CUBIC_IN		:
			return CEase_Cubic::easeIn(fTime, startValue, variation, duration);
		case EASE_CUBIC_OUT		:
			return CEase_Cubic::easeOut(fTime, startValue, variation, duration);
		case EASE_QUART_IN		:
			return CEase_Quart::easeIn(fTime, startValue, variation, duration);
		case EASE_QUART_OUT		:
			return CEase_Quart::easeOut(fTime, startValue, variation, duration);
		case EASE_QUINT_IN		:
			return CEase_Quint::easeIn(fTime, startValue, variation, duration);
		case EASE_QUINT_OUT		:
			return CEase_Quint::easeOut(fTime, startValue, variation, duration);
		case EASE_EXPO_IN		:
			return CEase_Expo::easeIn(fTime, startValue, variation, duration);
		case EASE_EXPO_OUT		:
			return CEase_Expo::easeOut(fTime, startValue, variation, duration);
		case EASE_CIRC_IN		:
			return CEase_Circ::easeIn(fTime, startValue, variation, duration);
		case EASE_CIRC_OUT		:
			return CEase_Circ::easeOut(fTime, startValue, variation, duration);
		case EASE_BACK_IN		:
			return CEase_Back::easeIn(fTime, startValue, variation, duration);
		case EASE_BACK_OUT		:
			return CEase_Back::easeOut(fTime, startValue, variation, duration);
		case EASE_ELASTIC_IN	:
			return CEase_Elastic::easeIn(fTime, startValue, variation, duration);
		case EASE_ELASTIC_OUT	:
			return CEase_Elastic::easeOut(fTime, startValue, variation, duration);
		case EASE_BOUNCE_IN		:
			return CEase_Bounce::easeIn(fTime, startValue, variation, duration);
		case EASE_BOUNCE_OUT	:
			return CEase_Bounce::easeOut(fTime, startValue, variation, duration);
		default:
			return 0.f;
	}
}

