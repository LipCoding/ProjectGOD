#pragma once

#include "PGEngine.h"

PG_USING

class CNaviTab : public CDialogEx
{
	DECLARE_DYNAMIC(CNaviTab)

public:
	CNaviTab(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CNaviTab();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG3 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

public:
	void Process_ChangeTab();
	void Process_ShowTab();
	void UpdateForm();

public:
	void Add_Point(const Vector3& vPoint);

private:
	vector<NAVIPOINT> m_vecNaviPoint;
	CListBox m_listNaviPoint;
	CListBox m_listNaviCell;
	CButton m_checkBoxNaviOn;

	int m_NumCell = 0;

public:
	afx_msg void OnBnClickedCheckNavi();
};
