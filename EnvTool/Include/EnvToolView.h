
// EnvToolView.h: CEnvToolView 클래스의 인터페이스
//

#pragma once

#include "EnvToolDoc.h"

#include "GameObject/GameObject.h"
#include "Core/Timer.h"
#include "Component/Transform.h"

PG_USING

class CEnvToolView : public CView
{
protected: // serialization에서만 만들어집니다.
	CEnvToolView() noexcept;
	DECLARE_DYNCREATE(CEnvToolView)

// 특성입니다.
public:
	CEnvToolDoc* GetDocument() const;

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
	virtual ~CEnvToolView();
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
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

// 사용자 함수
public:
	void UpdateView();
	void UpdateInput(const float& fTime);
	void UpdateObject(const float& fTime);
	void SetMainCamera();
	void SetLightCamera();

private:
	//tab control
	//void UpdateTerrainTab
	void PickingProcess(TOOLTAB_TYPE type);
private:
	void SetTileColor(float x, float y);
// 사용자 변수
private:
	CGameObject	*m_pCamera = nullptr;
	CTransform	*m_pCamTr = nullptr;
	CTimer		*m_pTimer = nullptr;

	CGameObject	*m_pBrushObj = nullptr;
	Vector3		m_vPickPos;

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // EnvToolView.cpp의 디버그 버전
inline CEnvToolDoc* CEnvToolView::GetDocument() const
   { return reinterpret_cast<CEnvToolDoc*>(m_pDocument); }
#endif

