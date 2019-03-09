#pragma once

#include "Component.h"

PG_BEGIN

class PG_DLL CFont :
	public CComponent
{
private:
	friend class CGameObject;

private:
	CFont();
	CFont(const CFont& text);
	~CFont();

private:
	IDWriteTextFormat*			m_pFont;
	ID2D1SolidColorBrush*		m_pBrush;
	RECTINFO	m_tArea;
	wstring		m_strText;

public:
	void SetArea(float l, float t, float r, float b);
	void SetFont(const string& strFont);
	void SetBrush(const string& strBrush);
	void SetText(const wstring& strText);
	void AddText(const wstring& strText);

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CFont* Clone();
};

PG_END