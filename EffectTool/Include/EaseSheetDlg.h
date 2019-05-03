#pragma once


// CEaseSheetDlg 대화 상자

class CEaseSheetDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CEaseSheetDlg)

public:
	CEaseSheetDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CEaseSheetDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG3 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

private:
	CString m_strBGImgPath;
	CImage  m_BGImg;

	int m_resizeX = 0;
	int m_resizeY = 0;
	int m_verticalCent = 0;
public:
	afx_msg void OnPaint();
};
