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
	int   m_iSizeX;
	int   m_iSizeZ;

	UINT  m_iRadio;
	UINT  m_iRadio1;
	UINT  m_iRadio2;
	UINT  m_iRadio_Texture;



public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonAdjSize();
	afx_msg void OnBnClickedCheckBrush();
//	afx_msg void OnNMReleasedcaptureSliderRange(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnRadioCheck(UINT id);
	afx_msg void OnRadioCheck1(UINT id);
	afx_msg void OnRadioCheck2(UINT id);
	afx_msg void OnRadioCheck_Texture(UINT id);
	afx_msg void OnBnClickedButtonHeightReset();
	afx_msg void OnBnClickedButtonTexLoad();
private:
	CEdit m_editTerrainSizeX;
	CEdit m_editTerrainSizeZ;
	CButton m_checkBoxHeightCheck;
	CSliderCtrl m_ctrSliderBrushRange;
	CEdit m_editBrushRange;
	CSliderCtrl m_ctrSliderHeightPower;
	CEdit m_editHeightPower;
	CSliderCtrl m_ctrSliderDetail_Default;

	CEdit m_editDetail_Default;
	CEdit m_editDetail_Tex1;
	CEdit m_editDetail_Tex2;
	CEdit m_editDetail_Tex3;
	CEdit m_editDetail_Tex4;

};
