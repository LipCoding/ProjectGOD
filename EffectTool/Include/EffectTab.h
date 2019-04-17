#pragma once


// CEffectTab 대화 상자

class CEffectTab : public CDialogEx
{
	DECLARE_DYNAMIC(CEffectTab)

public:
	CEffectTab(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CEffectTab();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()


public:
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};
