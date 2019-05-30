#pragma once

#include "../PGEngine.h"

PG_BEGIN

class PG_DLL CEffectAssist
{
public:
	enum ASSIST_TYPE
	{
		ASSIST_SCALE,
		ASSIST_ROT,
		ASSIST_FADE_IN,
		ASSIST_FADE_OUT,
		ASSIST_UV_ANI,
		ASSIST_UV_MOVE,
		ASSIST_END
	};

	enum EASE_TYPE
	{
		EASE_NONE,
		EASE_SINE_IN,
		EASE_SINE_OUT,
		EASE_QUAD_IN,
		EASE_QUAD_OUT,
		EASE_CUBIC_IN,
		EASE_CUBIC_OUT,
		EASE_QUART_IN,
		EASE_QUART_OUT,
		EASE_QUINT_IN,
		EASE_QUINT_OUT,
		EASE_EXPO_IN,
		EASE_EXPO_OUT,
		EASE_CIRC_IN,
		EASE_CIRC_OUT,
		EASE_BACK_IN,
		EASE_BACK_OUT,
		EASE_ELASTIC_IN,
		EASE_ELASTIC_OUT,
		EASE_BOUNCE_IN,
		EASE_BOUNCE_OUT,
		EASE_END
	};

public:
	CEffectAssist();
	~CEffectAssist();

public:
	void SetShareBuffer(SHARECBUFFER * sharebuffer) { m_pShareBuffer = sharebuffer; }
	/* Getter */
	ASSIST_TYPE GetType() { return m_AssistType; }
	EASE_TYPE GetEaseType() { return m_EaseType; }

	float GetStartTime() { return m_StartTime; }
	float GetEndTime() { return m_EndTime; }
	float GetTime() { return m_Time; }
	float GetPowerX() { return m_PowerX; }
	float GetPowerY() { return m_PowerY; }
	float GetPowerZ() { return m_PowerZ; }
		  
	float GetDegree() { return m_Degree; }

	int GetNum() { return m_Num; }
		
	int GetRepeat() { return m_Repeat; }

	float GetMoveUV_X() { return m_AniX; }
	float GetMoveUV_Y() { return m_AniY; }

	bool GetInifiniteCheck() { return m_InfiniteCheck; }

	/* Setter */
	void SetStartCheck(bool check) { m_StartCheck = check; }

	void SetStartTime(const float& start) { m_StartTime = start; }
	void SetEndTime(const float& end) { m_EndTime = end; }
	void SetPowerX(const float& x) { m_PowerX = x; }
	void SetPowerY(const float& y) { m_PowerY = y; }
	void SetPowerZ(const float& z) { m_PowerZ = z; }

	void SetDegree(const float& degree) { m_Degree = degree; }

	void SetNum(const int& num) { m_Num = num; }

	void SetRepeat(const int& repeat) { m_Repeat = repeat; }

	void SetMoveUV_X(const float& x) { m_AniX = x; }
	void SetMoveUV_Y(const float& y) { m_AniY = y; }

	void SetInfiniteCheck(bool check) { m_InfiniteCheck = check; }

public:
	void Init(class CGameObject *object, ASSIST_TYPE AssistType, EASE_TYPE easeType = EASE_END);
	void Update(class CGameObject *object, const float& deltaTime);
	
	void UpdateForTimeLimit(class CGameObject *object, const float& deltaTime);
	void UpdateForInfinite(class CGameObject *object, const float& deltaTime);

	void FirstStatusSet(class CGameObject *object);

private:
	void ReturnToFirstSet(class CGameObject *object);
	float Calc_Ease(EASE_TYPE type, const float& startValue, const float& variation, const float& duration);

private:
	/* Type */
	ASSIST_TYPE m_AssistType = ASSIST_END;
	EASE_TYPE   m_EaseType = EASE_END;

	/* Common */
	float m_StartTime = 0.f;
	float m_EndTime = 0.f;
	float m_Time = 0.f;

	/* Life Time */
	bool  m_StartCheck = false;
	float m_LifeTime = 0.f;

	/* Infinite */
	bool m_InfiniteCheck = false;

	/* For Pattern */
	float m_StartX = 0.f;
	float m_StartY = 0.f;
	float m_StartZ = 0.f;

	float m_PowerX = 0.f;
	float m_PowerY = 0.f;
	float m_PowerZ = 0.f;

	/* For Fade (In / Out) */
	float m_StartFadeIn = 0.f;
	float m_StartFadeOut = 0.f;

	float m_Degree = 0.f;

	/* For UV */
	class CAnimation2D *m_pAnimation = nullptr;

	int m_Repeat = 0;
	int m_Num = 0;

	/* Move */
	float m_AniX = 0.f;
	float m_AniY = 0.f;

	SHARECBUFFER *m_pShareBuffer = nullptr;
};

PG_END

