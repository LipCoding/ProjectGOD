#include "Gun.h"
#include "Component/Transform.h"

CGun::CGun()
{
	SetTag("Gun");
	SetTypeName("CGun");
	SetTypeID<CGun>();
}

CGun::CGun(const CGun & gun)	:
	CScript(gun)
{
}

CGun::~CGun()
{
}

bool CGun::Init()
{
	memset(m_fOffsetAngle, 0, sizeof(float) * AXIS_END);

	return true;
}

void CGun::Input(float fTime)
{
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		m_fOffsetAngle[AXIS_Y] -= PG_PI * fTime;
	}

	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		m_fOffsetAngle[AXIS_Y] += PG_PI * fTime;
	}

	if (GetAsyncKeyState(VK_UP) & 0x8000)
	{
		m_fOffsetAngle[AXIS_X] -= PG_PI * fTime;
	}

	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	{
		m_fOffsetAngle[AXIS_X] += PG_PI * fTime;
	}

	if (GetAsyncKeyState(VK_NEXT) & 0x8000)
	{
		m_fOffsetAngle[AXIS_Z] -= PG_PI * fTime;
	}

	if (GetAsyncKeyState(VK_PRIOR) & 0x8000)
	{
		m_fOffsetAngle[AXIS_Z] += PG_PI * fTime;
	}

	if (GetAsyncKeyState('1') & 0x8000)
	{
		m_vOffset.x -= 1.f * fTime;
	}

	if (GetAsyncKeyState('2') & 0x8000)
	{
		m_vOffset.x += 1.f * fTime;
	}

	if (GetAsyncKeyState('3') & 0x8000)
	{
		m_vOffset.y -= 1.f * fTime;
	}

	if (GetAsyncKeyState('4') & 0x8000)
	{
		m_vOffset.y += 1.f * fTime;
	}

	if (GetAsyncKeyState('5') & 0x8000)
	{
		m_vOffset.z -= 1.f * fTime;
	}

	if (GetAsyncKeyState('6') & 0x8000)
	{
		m_vOffset.z += 1.f * fTime;
	}

	m_pTransform->SetLocalPos(m_vOffset);

	for (int i = 0; i < AXIS_END; ++i)
	{
		m_pTransform->SetLocalRot(m_fOffsetAngle);
	}
}

int CGun::Update(float fTime)
{
	return 0;
}

int CGun::LateUpdate(float fTime)
{
	return 0;
}

CGun * CGun::Clone()
{
	return new CGun(*this);
}
