#pragma once

#include "GameObject/GameObject.h"

// CAnimColliderTab 대화 상자
PG_USING

class CAnimColliderTab : public CDialogEx
{
	DECLARE_DYNAMIC(CAnimColliderTab)

public:
	CAnimColliderTab(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CAnimColliderTab();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_COLLIDER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	void SetEditObj(CGameObject* editObj) { m_pEditObj = editObj; }
	void SetColliderObj(CGameObject* colliderObj) { m_pColliderObj = colliderObj; }

private:
	int	 m_iRadioAxisType = 0;
	CGameObject*	m_pEditObj = nullptr;
	CGameObject*	m_pColliderObj = nullptr;

	int  m_iColliderNumber = 0;

	float m_fScale;
	float m_fSpeed;

	CListBox m_listCollider;
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedButtonCreateColliderWorldmatrix();
	afx_msg void OnBnClickedButtonCreateColliderAttachToBone();
	afx_msg void OnBnClickedButtonDeleteCollider();
	afx_msg void OnBnClickedButtonClearAllCollider();
	afx_msg void OnBnClickedButtonAdjustScaleCollider();
	afx_msg void OnBnClickedButtonColliderUp();
	afx_msg void OnBnClickedButtonColliderDown();
	afx_msg void OnBnClickedButtonSpeedModifyCollider();
	void OnRadioColliderTypeCheck(UINT id);
};
