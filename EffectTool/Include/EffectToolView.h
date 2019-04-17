
// EffectToolView.h: CEffectToolView 클래스의 인터페이스
//

#pragma once

#include "EffectToolDoc.h"
#include "GameObject/GameObject.h"
#include "Core/Timer.h"
#include "Component/Transform.h"

PG_USING


class CEffectToolView : public CView
{
protected: // serialization에서만 만들어집니다.
	CEffectToolView() noexcept;
	DECLARE_DYNCREATE(CEffectToolView)

// 특성입니다.
public:
	CEffectToolDoc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CEffectToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();

	// 사용자 함수
public:
	void UpdateView();
	void UpdateInput(const float& fTime);
	void UpdateObject(const float& fTime);
	void UpdateForm(const float& fTime);

	// 사용자 변수
private:
	CGameObject	*m_pCamera = nullptr;
	CTransform	*m_pCamTr = nullptr;
	CTimer		*m_pTimer = nullptr;

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};

#ifndef _DEBUG  // EffectToolView.cpp의 디버그 버전
inline CEffectToolDoc* CEffectToolView::GetDocument() const
   { return reinterpret_cast<CEffectToolDoc*>(m_pDocument); }
#endif

