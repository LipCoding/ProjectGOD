#pragma once

#include "PGEngine.h"
#include "GameObject/GameObject.h"
#include "Component/Camera.h"
// CLightTab 대화 상자

PG_USING

class CLightTab : public CDialogEx
{
	DECLARE_DYNAMIC(CLightTab)

public:
	CLightTab(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CLightTab();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG4 };
#endif

public:
	void Process_ChangeTab();
	void Process_ShowTab();

private:
	CGameObject *m_pGlobalLight = nullptr;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
private:
	CSliderCtrl m_ctrSliderGlobLightPosX;
	CSliderCtrl m_ctrSliderGlobLightPosY;
	CSliderCtrl m_ctrSliderGlobLightPosZ;

	CEdit m_editGlobLightPosX;
	CEdit m_editGlobLightPosY;
	CEdit m_editGlobLightPosZ;

	CCamera *m_pLightCam = nullptr;
public:
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};
