#pragma once

#include "GameObject/GameObject.h"

// CAnimMeshInfoTab 대화 상자
PG_USING

class CAnimMeshInfoTab : public CDialogEx
{
	DECLARE_DYNAMIC(CAnimMeshInfoTab)

public:
	CAnimMeshInfoTab(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CAnimMeshInfoTab();

	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_MESH_INFO };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	void SetEditObj(CGameObject* editObj) { m_pEditObj = editObj; }
	void SetArmObj(CGameObject* armObj) { m_pArmObj = armObj; }
	void SetMeshInfo();
public:
	afx_msg void OnRadioAnimTypeCheck(UINT id);
	afx_msg void OnBnClickedButtonModifyLocalInfo();
	afx_msg void OnBnClickedButtonCreateArmObj();
	afx_msg void OnBnClickedButtonArmUp();
	afx_msg void OnBnClickedButtonArmDown();
	afx_msg void OnBnClickedButtonSpeedModify();
	afx_msg void OnBnClickedButtonSaveArm();
	afx_msg void OnBnClickedButtonLoadArm();
	afx_msg void OnBnClickedButtonSaveMesh();
	afx_msg void OnBnClickedButtonLoadMesh();
	afx_msg void OnBnClickedButtonSaveAnimation();
	afx_msg void OnBnClickedButtonLoadAnimation();
	afx_msg void OnBnClickedButtonArmAttachBone();
	afx_msg void OnBnClickedButtonSaveLocalInfo();
	afx_msg void OnBnClickedButtonLoadLocalLoad();
	afx_msg void OnBnClickedButtonAdjustScaleArm();
private:
	float m_fScaleX;
	float m_fScaleY;
	float m_fScaleZ;
	float m_fRotationX;
	float m_fRotationY;
	float m_fRotationZ;
	float m_fPositionX;
	float m_fPositionY;
	float m_fPositionZ;
	float m_fPivot;

	CString		m_armMeshPath;
	CString		m_armObjName;
	CSliderCtrl m_sliderCtrlArmRotX;
	CSliderCtrl m_sliderCtrlArmRotY;
	CSliderCtrl m_sliderCtrlArmRotZ;
	float		m_fSpeed;

	int			m_iRadioAxisType = 0;
	CGameObject*	m_pEditObj = nullptr;
	CGameObject*	m_pArmObj = nullptr;
	float m_fArmScale;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};
