#include "Effect.h"
#include "Component/Transform.h"
#include "Core/Input.h"

Effect::Effect()
{
	SetTag("Effect");
	SetTypeName("Effect");
	SetTypeID<Effect>();
}

Effect::Effect(const Effect & effect) :
	CScript(effect)
{
}

Effect::~Effect()
{
	SAFE_RELEASE(m_pParticleSingle);
	SAFE_RELEASE(m_pRenderer);
	SAFE_RELEASE(m_pObject);
}

void Effect::Init_Other()
{
	m_pParticleSingle->SetAlpha(1.f);
	int tmp = 1.f;
	rand() % 2 ? (tmp *= -1.f) : (tmp *= 1.f);
	m_pParticleSingle->SetAlpha(1.f);
	m_pParticleSingle->SetAngle(tmp * (180.f / ((rand() % 5) + 4.f)));
	/*PRENDERERCBUFFER cbuffer = m_pParticleSingle->GetRenderer()->FindCBuffer(11);
	float& alpha = static_cast<PARTICLECBUFFER*>(cbuffer->pData)->vAlpha;
	alpha = 1.f;
	m_pParticleSingle->GetRenderer()->UpdateCBuffer("Particle", 11, sizeof(PARTICLECBUFFER),
	SCT_GEOMETRY | SCT_PIXEL, cbuffer->pData);*/
}

bool Effect::Init()
{
	GET_SINGLE(CInput)->CreateKey("ReEffect", VK_SPACE);
	return true;
}

void Effect::Input(float fTime)
{
	if (KEYPUSH("ReEffect") && m_key_check == false)
	{
		int tmp = 1.f;
		rand() % 2 ? (tmp *= -1.f) : (tmp *= 1.f);
		m_pParticleSingle->SetAlpha(1.f);
		m_pParticleSingle->SetAngle(tmp * (180.f / ((rand() % 30) + 4.f)));
		m_acc_scale_y = m_acc_scale_y_origin;
		m_speed_scale_y = m_speed_scale_y_origin;
		m_acc_alpha = m_acc_alpha_origin;
		m_speed_alpha = m_speed_alpha_origin;
		m_pParticleSingle->SetSize(0.95f, 0.95f);
		m_key_check = true;
		m_limit_check = false;
	}

	if (KEYUP("ReEffect"))
	{
		m_key_check = false;
	}
}

int Effect::Update(float fTime)
{
	Vector2 size = m_pParticleSingle->GetSize();
	if (size.y > m_limit_size)
	{
		m_limit_check = true;
	}
	m_speed_scale_y += m_acc_scale_y * fTime;
	if (m_limit_check == false)
	{
		size.x += (m_speed_scale_y / 10.f) * fTime;
		size.y += m_speed_scale_y * fTime;
	}
	else
	{
		size.x -= (m_speed_scale_y / 10.f) * fTime;
		size.y -= m_speed_scale_y * fTime;
	}
	m_pParticleSingle->SetSize(size);

	float tAlpha = m_pParticleSingle->GetAlpha();

	if (tAlpha <= 0.f)
	{
		m_pParticleSingle->SetSize(0.f, 0.f);
		//°´Ã¼Áö¿ò
		//m_bAlive = false;
	}

	m_speed_alpha += m_acc_alpha * fTime;
	tAlpha -= m_speed_alpha * fTime;
	m_pParticleSingle->SetAlpha(tAlpha);

	return 0;
}

int Effect::LateUpdate(float fTime)
{
	return 0;
}

Effect * Effect::Clone()
{
	return nullptr;
}
