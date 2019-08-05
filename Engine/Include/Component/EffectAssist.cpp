#include "EffectAssist.h"
#include "../Ease.h"
#include "Transform.h"
#include "../GameObject/GameObject.h"
#include "../Component/Renderer.h"
#include "../Component/Animation2D.h"

PG_USING

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
	//CRenderer* pRenderer = object->FindComponentFromType<CRenderer>(CT_RENDERER);
	//pRenderer->CreateCBuffer("Share", 8, sizeof(SHARECBUFFER), SCT_PIXEL);
	//SAFE_RELEASE(pRenderer);
}

void CEffectAssist::Update(CGameObject * object, const float& deltaTime)
{
	if (m_StartCheck == false)
	{
		ReturnToFirstSet(object);
		return;
	}

	m_Time += deltaTime;

	if (false == m_InfiniteAssistCheck)
		UpdateForTimeLimit(object, deltaTime);
	else
		UpdateForInfinite(object, deltaTime);
}

void CEffectAssist::UpdateForTimeLimit(CGameObject * object, const float & deltaTime)
{
	// 페이드 인 시작하기 전에는 보이면 안되므로 렌더 비활성화
	if (m_AssistType == ASSIST_FADE_IN)
	{
		if (m_Time < m_StartTime)
		{
			object->SetRenderEnable(false);
		}
		else
		{
			object->SetRenderEnable(true);
		}
	}

	// 페이드 아웃 알파값이 1.f이상이면 바로 렌더 비활성화
	else if (m_AssistType == ASSIST_FADE_OUT &&
		m_FadeEndCheck)
	{
		object->SetRenderEnable(false);
	}

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
			object->SetRenderEnable(true);
			float fAlpha = Calc_Ease(m_EaseType, m_StartFadeIn, m_Degree, m_LifeTime);

			if (fAlpha > 1.f)
			{
				m_FadeEndCheck = true;
				fAlpha = 1.f;
			}

			m_pShareBuffer->fAlphaFadeOut = 1.f;
			m_pShareBuffer->fAlphaFadeIn = fAlpha;
			m_pShareBuffer->vColor = Vector4{ 0.f, 0.f, 0.f, 0.f };

			CRenderer* pRenderer = object->FindComponentFromType<CRenderer>(CT_RENDERER);
			pRenderer->UpdateCBuffer("Share", 8, sizeof(SHARECBUFFER), SCT_PIXEL, m_pShareBuffer);
			SAFE_RELEASE(pRenderer);

			break;
		}
		case ASSIST_FADE_OUT:
		{
			float fAlpha = Calc_Ease(m_EaseType, m_StartFadeOut, m_Degree, m_LifeTime);

			if (fAlpha > 1.f)
			{
				m_FadeEndCheck = true;
				fAlpha = 1.f;
			}

			m_pShareBuffer->fAlphaFadeIn = 0.f;
			m_pShareBuffer->fAlphaFadeOut = fAlpha;
			m_pShareBuffer->vColor = Vector4{ 0.f, 0.f, 0.f, 0.f };

			CRenderer* pRenderer = object->FindComponentFromType<CRenderer>(CT_RENDERER);
			pRenderer->UpdateCBuffer("Share", 8, sizeof(SHARECBUFFER), SCT_PIXEL, m_pShareBuffer);
			SAFE_RELEASE(pRenderer);

			break;
		}
		case ASSIST_UV_ANI:
		{
			CAnimation2D *pAnimation = object->FindComponentFromType<CAnimation2D>(CT_ANIMATION2D);
			if (pAnimation)
			{
				pAnimation->ChangeAnimation("Effect");
				SAFE_RELEASE(pAnimation);
			}
			break;
		}
		case ASSIST_UV_MOVE:
		{
			m_pShareBuffer->fMoveUV_X += m_AniX * deltaTime;
			m_pShareBuffer->fMoveUV_Y += m_AniY * deltaTime;
			CRenderer* pRenderer = object->FindComponentFromType<CRenderer>(CT_RENDERER);
			pRenderer->UpdateCBuffer("Share", 8, sizeof(SHARECBUFFER), SCT_PIXEL, m_pShareBuffer);
			SAFE_RELEASE(pRenderer);

			break;
		}
		case ASSIST_POS:
		{
			CTransform *pTr = object->GetTransform();
			Vector3 vPos;
			vPos.x = Calc_Ease(m_EaseType, m_StartX, m_PowerX, m_LifeTime);
			vPos.y = Calc_Ease(m_EaseType, m_StartY, m_PowerY, m_LifeTime);
			vPos.z = Calc_Ease(m_EaseType, m_StartZ, m_PowerZ, m_LifeTime);
			pTr->SetWorldPos(vPos);
			SAFE_RELEASE(pTr);
			break;
		}
		default:
			break;
		}
	}

	if (m_Time >= m_EndTime)
	{
		if (m_StartFromMain &&
			(m_AssistType == ASSIST_SCALE ||
			m_AssistType == ASSIST_ROT ||
			m_AssistType == ASSIST_POS) )
			return;

		ReturnToFirstSet(object);

		if (m_pShareBuffer)
		{
			/* Fade */
			m_pShareBuffer->fAlphaFadeIn = 0.f;
			m_pShareBuffer->fAlphaFadeOut = 0.f;
			m_pShareBuffer->vColor = Vector4{ 0.f, 0.f, 0.f, 0.f };
			m_pShareBuffer->fMoveUV_X = 0.f;
			m_pShareBuffer->fMoveUV_Y = 0.f;

			CRenderer* pRenderer = object->FindComponentFromType<CRenderer>(CT_RENDERER);
			pRenderer->UpdateCBuffer("Share", 8, sizeof(SHARECBUFFER), SCT_PIXEL, m_pShareBuffer);
			SAFE_RELEASE(pRenderer);
		}

		// 페이드 아웃 끝나면 바로 렌더 비활성화
		if (m_AssistType == ASSIST_FADE_OUT)
		{
			object->SetRenderEnable(false);
		}

		m_StartCheck = false;
	}
}

void CEffectAssist::UpdateForInfinite(CGameObject * object, const float & deltaTime)
{
	// 계속 지속되어야 하기 때문에
	// m_LifeTime += deltaTime / 2.f;

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

		if (fAlpha > 1.f)
			m_Time = 0.f;

		m_pShareBuffer->fAlphaFadeOut = 1.f;
		m_pShareBuffer->fAlphaFadeIn = fAlpha;
		m_pShareBuffer->vColor = Vector4{ 0.f, 0.f, 0.f, 0.f };

		CRenderer* pRenderer = object->FindComponentFromType<CRenderer>(CT_RENDERER);
		pRenderer->UpdateCBuffer("Share", 8, sizeof(SHARECBUFFER), SCT_PIXEL, m_pShareBuffer);
		SAFE_RELEASE(pRenderer);

		break;
	}
	case ASSIST_FADE_OUT:
	{
		float fAlpha = Calc_Ease(m_EaseType, m_StartFadeOut, m_Degree, m_LifeTime);

		if (fAlpha > 1.f)
			m_Time = 0.f;

		m_pShareBuffer->fAlphaFadeIn = 0.f;
		m_pShareBuffer->fAlphaFadeOut = fAlpha;
		m_pShareBuffer->vColor = Vector4{ 0.f, 0.f, 0.f, 0.f };

		CRenderer* pRenderer = object->FindComponentFromType<CRenderer>(CT_RENDERER);
		pRenderer->UpdateCBuffer("Share", 8, sizeof(SHARECBUFFER), SCT_PIXEL, m_pShareBuffer);
		SAFE_RELEASE(pRenderer);

		break;
	}
	case ASSIST_UV_ANI:
	{
		CAnimation2D *pAnimation = object->FindComponentFromType<CAnimation2D>(CT_ANIMATION2D);
		if (pAnimation)
		{
			pAnimation->ChangeAnimation("Effect");
			SAFE_RELEASE(pAnimation);
		}
		break;
	}
	case ASSIST_UV_MOVE:
	{
		m_pShareBuffer->fMoveUV_X += m_AniX * deltaTime;
		m_pShareBuffer->fMoveUV_Y += m_AniY * deltaTime;
		CRenderer* pRenderer = object->FindComponentFromType<CRenderer>(CT_RENDERER);
		pRenderer->UpdateCBuffer("Share", 8, sizeof(SHARECBUFFER), SCT_PIXEL, m_pShareBuffer);
		SAFE_RELEASE(pRenderer);

		break;
	}
	case ASSIST_POS:
	{
		CTransform *pTr = object->GetTransform();
		Vector3 vPos;
		vPos.x = Calc_Ease(m_EaseType, m_StartX, m_PowerX, m_LifeTime);
		vPos.y = Calc_Ease(m_EaseType, m_StartY, m_PowerY, m_LifeTime);
		vPos.z = Calc_Ease(m_EaseType, m_StartZ, m_PowerZ, m_LifeTime);
		pTr->SetWorldPos(vPos);
		SAFE_RELEASE(pTr);
		break;
	}
	default:
		break;
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
	case ASSIST_POS:
	{
		CTransform *pTr = object->GetTransform();
		Vector3 vPos = pTr->GetWorldPos();
		m_StartX = vPos.x;
		m_StartY = vPos.y;
		m_StartZ = vPos.z;
		SAFE_RELEASE(pTr);
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
		
		if (pTr->GetWorldScale() != Vector3(m_StartX, m_StartY, m_StartZ))
			pTr->SetWorldScale(m_StartX, m_StartY, m_StartZ);

		SAFE_RELEASE(pTr);
		break;
	}
	case ASSIST_ROT:
	{
		CTransform *pTr = object->GetTransform();

		if (pTr->GetWorldRot() != Vector3(XMConvertToRadians(m_StartX),
										  XMConvertToRadians(m_StartY),
										  XMConvertToRadians(m_StartZ)))
		{
			pTr->SetWorldRot(XMConvertToRadians(m_StartX),
				XMConvertToRadians(m_StartY),
				XMConvertToRadians(m_StartZ));
		}

		SAFE_RELEASE(pTr);
		break;
	}
	case ASSIST_FADE_IN:
	{
		m_FadeEndCheck = false;
		break;
	}
	case ASSIST_FADE_OUT:
	{
		m_FadeEndCheck = false;
		break;
	}
	case ASSIST_UV_ANI:
	{
		CAnimation2D *pAnimation = object->FindComponentFromType<CAnimation2D>(CT_ANIMATION2D);
		if (pAnimation)
		{
			pAnimation->ChangeAnimation("Default");	
			PANIMATIONCLIP2D pClip = pAnimation->GetClip("Effect");
			if (pClip)
			{
				switch (m_SpriteType)
				{
				case SPRITE_ATLAS:
					pClip->iFrameMaxX = m_Max_X / m_Width;
					pClip->iFrameMaxY = m_Max_Y / m_Height;
					break;
				case SPRITE_FRAME:
					pClip->iFrameMaxX = m_Num;
					pClip->iLengthX = m_Num;
					break;
				}

				pClip->fAnimLimitTime = m_LifeTime;
			}

			SAFE_RELEASE(pAnimation);
		}
		break;
	}
	case ASSIST_UV_MOVE:
	{
		break;
	}
	case ASSIST_POS:
	{
		CTransform *pTr = object->GetTransform();

		if (pTr->GetWorldPos() != Vector3(m_StartX, m_StartY, m_StartZ))
			pTr->SetWorldPos(m_StartX, m_StartY, m_StartZ);

		SAFE_RELEASE(pTr);
		break;
	}
	default:
		break;
	}

	if (true == m_InfiniteAssistCheck)
	{
		if (m_pShareBuffer)
		{
			/* Fade */
			m_pShareBuffer->fAlphaFadeIn = 0.f;
			m_pShareBuffer->fAlphaFadeOut = 0.f;
			m_pShareBuffer->vColor = Vector4{ 0.f, 0.f, 0.f, 0.f };
			m_pShareBuffer->fMoveUV_X = 0.f;
			m_pShareBuffer->fMoveUV_Y = 0.f;

			CRenderer* pRenderer = object->FindComponentFromType<CRenderer>(CT_RENDERER);
			pRenderer->UpdateCBuffer("Share", 8, sizeof(SHARECBUFFER), SCT_PIXEL, m_pShareBuffer);
			SAFE_RELEASE(pRenderer);
		}
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

