#include "Timer.h"
#include "../Core.h"

PG_USING

CTimer::CTimer()
{
	SetTag("Timer");
	SetTypeName("CTimer");
	SetTypeID<CTimer>();
}

CTimer::~CTimer()
{
}

float CTimer::GetDeltaTime() const
{
	return m_fDeltaTime;
}

float CTimer::GetFPS() const
{
	return m_fFPS;
}

bool CTimer::Init()
{
	QueryPerformanceFrequency(&m_tSecond);

	m_fDeltaTime = 0.f;
	m_fFPS = 0.f;
	m_fFPSTime = 0.f;
	m_iFrame = 0;
	m_iFrameMax = 60;
	m_tTime.QuadPart = -1;

	return true;
}

void CTimer::Update()
{
	if (m_tTime.QuadPart == -1)
	{
		QueryPerformanceCounter(&m_tTime);
		return;
	}

	LARGE_INTEGER	tTime;
	QueryPerformanceCounter(&tTime);
	
	m_fDeltaTime = float(tTime.QuadPart - m_tTime.QuadPart) /
		(float)m_tSecond.QuadPart;

	// 프레임 맞추려고 강제로 세팅한 안좋은 코드.
	// 의도는 60프레임에 비슷한 시간값을 deltatime값과 비교해서
	// 만약 60프레임의 시간값보다 작다면 남은 시간만큼
	// 맞춰주기위해서 Sleep함수를 사용.
	// Sleep은 millisecond단위이기때문에 *1000을 해준다.
	// 하지만 Sleep은 Sleep(1)이 1ms만 쉬지는 않는다.
	// 프로세스가 대기로 빠졌다가 다시 코어에 올라가서 활성화되기까지 많은 과정들을 거쳐야 하기때문.
	// 이점 고려할것. 그렇기때문에 안좋은 코드.
	/*float duration_time = 0.016666f - m_fDeltaTime;
	if (duration_time > 0.f)
	{
		int sleep_time = duration_time * 1000;
		Sleep(sleep_time);
	}*/
	m_tTime = tTime;


	m_fFPSTime += m_fDeltaTime;
	++m_iFrame;

	if (m_iFrame == m_iFrameMax)
	{
		m_fFPS = m_iFrame / m_fFPSTime;

		m_fFPSTime = 0.f;
		m_iFrame = 0;

#ifdef _DEBUG
		//_cprintf("FPS : %.5f\n", m_fFPS);
#else
		//char	strFPS[128] = {};
		//sprintf_s(strFPS, "FPS : %.5f", m_fFPS);
		//SetWindowTextA(GET_SINGLE(CCore)->GetWindowHandle(), strFPS);
#endif // _

	}
}
