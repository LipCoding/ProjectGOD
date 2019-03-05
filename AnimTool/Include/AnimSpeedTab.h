#pragma once


// CAnimSpeedTab 대화 상자

class CAnimSpeedTab : public CDialogEx
{
	DECLARE_DYNAMIC(CAnimSpeedTab)

public:
	CAnimSpeedTab(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CAnimSpeedTab();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ANIM_SPEED };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
