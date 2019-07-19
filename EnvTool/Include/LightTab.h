#pragma once

#include "PGEngine.h"
#include "GameObject/GameObject.h"
#include "Component/Camera.h"
#include "Component/Light.h"

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
	void SetAllByMemberValue();
	void SetPointLightPosXZ(const Vector3& pos);
	void SetPointLightHeight(const float& height);

public:
	void Process_ChangeTab();
	void Process_ShowTab();

	void AddPointLight();
	void UndoPointLight();

private:
	vector<CGameObject*> m_vecObjects;

	CGameObject *m_pGlobalLight = nullptr;
	CGameObject *m_pPointLight = nullptr;

	CCamera *m_pLightCam = nullptr;
	CLight *m_pLight = nullptr;
	CLight *m_pLight_point = nullptr;

	float m_fRange = 425.f;
	Vector4 m_vLightColor = Vector4(1.f, 1.f, 1.f, 1.f);
	Vector3 m_vCamPos = Vector3(-512.f / 8.f, 400.f, -512.f / 8.f);
	Vector3 m_vCamLookPos = Vector3{ 512.f / 2.f, 0.f, 512.f / 2.f };

	float m_fPointRange = 10.f;
	Vector4 m_vPointLightColor = Vector4(1.f, 1.f, 1.f, 1.f);
	float m_fHeight = 0.f;
	int m_iPointNum = 0;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
private:
	CSliderCtrl m_ctrSliderGlobLightPosX;
	CSliderCtrl m_ctrSliderGlobLightPosY;
	CSliderCtrl m_ctrSliderGlobLightPosZ;

	CEdit m_editGlobLightRange;

	CEdit m_editGlobLightR;
	CEdit m_editGlobLightG;
	CEdit m_editGlobLightB;

	CEdit m_editGlobLightPosX;
	CEdit m_editGlobLightPosY;
	CEdit m_editGlobLightPosZ;

	CSliderCtrl m_ctrSliderGlobLightLookPosX;
	CSliderCtrl m_ctrSliderGlobLightLookPosY;
	CSliderCtrl m_ctrSliderGlobLightLookPosZ;

	CEdit m_editGlobLightLookPosX;
	CEdit m_editGlobLightLookPosY;
	CEdit m_editGlobLightLookPosZ;

	CListBox m_listPointLightList;

	CEdit m_editPointLightRange;
	CEdit m_editPointLightR;
	CEdit m_editPointLightG;
	CEdit m_editPointLightB;

public:
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedButtonGloblightRangeAdj();
	afx_msg void OnBnClickedButtonGloblightColorAdj();
	afx_msg void OnBnClickedButtonGloblightReset();
	afx_msg void OnBnClickedButtonGloblightSave();
	afx_msg void OnBnClickedButtonGloblightLoad();
	afx_msg void OnBnClickedButtonPointRangeAdj();
	afx_msg void OnBnClickedEditPointColorAdj();
};
