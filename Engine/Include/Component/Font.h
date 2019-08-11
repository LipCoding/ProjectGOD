#pragma once

#include "Component.h"

PG_BEGIN

class PG_DLL CFont :
	public CComponent
{
private:
	friend class CGameObject;
	size_t offset{0};
	size_t max_offset{0};
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
	void addOffset();
	void subOffset();
	void setOffset(size_t offset) { this->offset = offset; }
	void setMaxOffset(size_t max_offset) { this->max_offset = max_offset; }
	size_t getMaxOffset() { return max_offset; }
	size_t getOffset() { return offset; }
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