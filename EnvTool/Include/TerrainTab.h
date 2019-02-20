#pragma once


// CTerrainTab 대화 상자

class CTerrainTab : public CDialogEx
{
	DECLARE_DYNAMIC(CTerrainTab)

public:
	CTerrainTab(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CTerrainTab();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
private:
	CEdit m_editTerrainSizeX;
	int   m_iSizeX;
	CEdit m_editTerrainSizeZ;
	int   m_iSizeZ;

public:
	afx_msg void OnBnClickedButtonAdjSize();
	afx_msg void OnBnClickedCheckBrush();
	virtual BOOL OnInitDialog();
//	afx_msg void OnNMReleasedcaptureSliderRange(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	
	CButton m_checkBoxHeightCheck;
	CSliderCtrl m_ctrSliderBrushRange;
	CEdit m_editBrushRange;
};
