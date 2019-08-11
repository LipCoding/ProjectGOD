#include "Font.h"
#include "../Device.h"
#include "Transform.h"

PG_USING

CFont::CFont()
{
	SetTag("Text");
	SetTypeName("CText");
	SetTypeID<CFont>();
	m_eType = CT_UI;

	m_pFont = GET_SINGLE(CDevice)->FindTextFormat("����");
	m_pBrush = GET_SINGLE(CDevice)->FindTextBrush("Black");
}

CFont::CFont(const CFont & text) :
	CComponent(text)
{
	*this = text;
}

CFont::~CFont()
{
}

void CFont::SetArea(float l, float t, float r, float b)
{
	m_tArea = RECTINFO(l, t, r, b);
}

void CFont::SetFont(const string & strFont)
{
	m_pFont = GET_SINGLE(CDevice)->FindTextFormat(strFont);
	// m_pFont->
}

void CFont::SetBrush(const string & strBrush)
{
	m_pBrush = GET_SINGLE(CDevice)->FindTextBrush(strBrush);
}

void CFont::SetText(const wstring & strText)
{
	m_strText = strText;
}

void CFont::AddText(const wstring & strText)
{
	m_strText += strText;
}

bool CFont::Init()
{
	return true;
}

void CFont::Input(float fTime)
{
}

int CFont::Update(float fTime)
{
	return 0;
}

int CFont::LateUpdate(float fTime)
{
	return 0;
}

void CFont::Collision(float fTime)
{
}

void CFont::Render(float fTime)
{
	D2D_RECT_F tArea;

	tArea.left = m_tArea.l + m_pTransform->GetWorldPos().x;
	tArea.right = m_tArea.r + m_pTransform->GetWorldPos().x;
	tArea.top = m_tArea.t + m_pTransform->GetWorldPos().y + (offset*50);
	tArea.bottom = m_tArea.b + m_pTransform->GetWorldPos().y + (offset*50);

	GET_SINGLE(CDevice)->Get2DRenderTarget()->BeginDraw();
	int length = m_strText.length();
	GET_SINGLE(CDevice)->Get2DRenderTarget()->DrawTextW(m_strText.c_str(), m_strText.length(),
		m_pFont, tArea, m_pBrush);
	GET_SINGLE(CDevice)->Get2DRenderTarget()->EndDraw();
}

CFont * CFont::Clone()
{
	return new CFont(*this);
}

void CFont::addOffset()
{
	if (offset < max_offset)
		this->offset++;
}

void CFont::subOffset()
{
	if (offset > 0)
		this->offset--;
}