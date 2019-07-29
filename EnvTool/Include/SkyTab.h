#pragma once

#include "PGEngine.h"

// CSkyTab 대화 상자
PG_USING

class CSkyTab : public CDialogEx
{
	DECLARE_DYNAMIC(CSkyTab)

public:
	CSkyTab(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CSkyTab();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG5 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CComboBox m_comboSkyBoxList;
	afx_msg void OnCbnSelchangeComboSkytype();
	afx_msg void OnBnClickedButtonSkySave();
	afx_msg void OnBnClickedButtonSkyLoad();
};
