#pragma once
#include "UI.h"

PG_BEGIN

enum PG_DLL BUTTON_STATE
{
	BS_NONE,
	BS_MOUSEON,
	BS_CLICK
};

class PG_DLL CUIButton :
	public CUI
{
protected:
	friend class CGameObject;

public:
	CUIButton();
	CUIButton(const CUIButton& button);
	~CUIButton();

protected:
	Vector4		m_vNormalCol;
	Vector4		m_vMouseOnCol;
	Vector4		m_vClickCol;
	Vector3     offset;
	BUTTONCBUFFER	m_tCBuffer;
	function<void(float)>	m_ButtonCallback;
	bool		m_bCallback;
	BUTTON_STATE	m_eState;
	float UILength;
	float lengthRatio;
	float alpha = 1.f;
	bool isalpha = false;

public:
	void enableAlpha(bool isalpha) { this->isalpha = isalpha; }

public:
	void setUILength(float UILength) { this->UILength = UILength; }
	float getUILength() { return UILength; }

	void setLengthRatio(float lengthRatio) { this->lengthRatio = lengthRatio; }
	float getLengthRatio() { return lengthRatio; }

public:
	template <typename T>
	void SetCallback(T* pObj, void(T::*pFunc)(float))
	{
		m_ButtonCallback = bind(pFunc, pObj, placeholders::_1);
		m_bCallback = true;
	}

	void SetCallback(void(*pFunc)(float));

	void SetNormalColor(const Vector4& vColor);
	void SetMouseOnColor(const Vector4& vColor);
	void SetClickColor(const Vector4& vColor);

public:
	void setOffset(const Vector3& offset) { this->offset = offset; }
	Vector3 getOffset() { return this->offset; }

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CUIButton* Clone();
	virtual void OnCollisionEnter(class CCollider* pSrc, class CCollider* pDest,
		float fTime);
	virtual void OnCollision(class CCollider* pSrc, class CCollider* pDest,
		float fTime);
	virtual void OnCollisionLeave(class CCollider* pSrc, class CCollider* pDest,
		float fTime);
};

PG_END
