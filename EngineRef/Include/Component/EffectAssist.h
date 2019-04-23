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

public:
	CEffectAssist();
	~CEffectAssist();

public:
	/* Getter */
	


	/* Setter */
	void SetStartTime(const float& start) { m_StartTime = start; }
	void SetEndTime(const float& end) { m_EndTime = end; }
	void SetPowerX(const float& x) { m_PowerX = x; }
	void SetPowerY(const float& y) { m_PowerY = y; }
	void SetPowerZ(const float& z) { m_PowerZ = z; }

	void SetDegree(const float& degree) { m_Degree = degree; }

	void SetWidth(const int& width) { m_Width = width; }
	void SetHeight(const int& height) { m_Height = height; }
	void SetNum(const int& num) { m_Num = num; }

	void SetRepeat(const int& repeat) { m_Repeat = repeat; }

public:
	void Init(ASSIST_TYPE type);
	void Update(class CGameObject *object, const float& time);

private:
	/* Type */
	ASSIST_TYPE m_AssistType = ASSIST_END;


	/* Common */
	float m_StartTime = 0.f;
	float m_EndTime = 0.f;
	float m_Time = 0.f;

	/* For Pattern */
	float m_PowerX = 0.f;
	float m_PowerY = 0.f;
	float m_PowerZ = 0.f;

	/* For Fade (In / Out) */
	float m_Degree = 0.f;

	/* For UV */
	int m_Width = 0;
	int m_Height = 0;
	int m_Num = 0;

	int m_AniX = 0;
	int m_AniY = 0;

	int m_Repeat = 0;
};

PG_END

