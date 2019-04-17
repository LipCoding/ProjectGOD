#pragma once
#include "afxwin.h"

// CEditForm 대화 상자

class CEditForm : public CFormView
{
	DECLARE_DYNCREATE(CEditForm)

public:
	CEditForm();   // 표준 생성자입니다.
	virtual ~CEditForm();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_FORM };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:
	class CEnvToolView  *m_pView = nullptr;
	
	class CEffectTab *m_pEffectDlg = nullptr;
	TOOLTAB_TYPE m_eTabType = TAB_END;

public:
	virtual void OnInitialUpdate();

private:
	CTabCtrl m_Tab;
	CListBox m_listEffectList;
};
