#pragma once
#include "afxwin.h"



// EditForm 보기

class CEditForm : public CFormView
{
	DECLARE_DYNCREATE(CEditForm)

protected:
	CEditForm();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CEditForm();

public:
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
public:
	CTabCtrl m_Tab;
	class CTerrainTab	*m_pTerrainDlg = nullptr;
	class CObjTab		*m_pObjDlg = nullptr;
//	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	virtual void OnInitialUpdate();
};


