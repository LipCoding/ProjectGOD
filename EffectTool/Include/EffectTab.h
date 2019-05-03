#pragma once

#include "GameObject/GameObject.h"
#include "Component/Effect.h"

PG_USING

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
	/* Getter, Setter */
	void SetTargetObject(CGameObject *target)
	{
		m_pTargetObject = target;
		m_bFirstTargetCheck = true;
	}

	CGameObject* GetTargetObject()
	{
		return m_pTargetObject;
	}

	bool GetFirstCheck()
	{
		return m_bFirstTargetCheck;
	}

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	/* Load Target Resource */
	bool LoadTargetMesh(const CString& filePath, const CString& fileName);
	bool LoadTargetAnimation(const CString& filePath);
	bool LoadTargetLocalInfo(const CString& filePath);

	void DeleteTargetMesh();

public:
	afx_msg void OnBnClickedButtonLoadTargetObj();
	afx_msg void OnCbnSelchangeComboBoneList();
	afx_msg void OnCbnSelchangeComboAnimList();

public:
	void UpdateForm();

private:
	/* */
	void InitForm();

	/* Total */
	void InitFormValue();
	/* Main */
	void InitMainTimer();
	/* Info */
	void InitFormInfo();
	/* Pattern */
	void InitFormPatternScale();
	void InitFormPatternRot();
	/* Sheet */
	void InitComboBox();

	void UpdateInfo();
	void UpdateMainTimer();
	void UpdatePattern();
	void UpdateTime();

private:
	void SetInfoPos();
	void SetInfoScale();
	void SetInfoRot();

	void AddPatternScale(class CEffect *pEffect);
	void AddPatternRot(class CEffect *pEffect);

	void SetMainTimer(class CEffect *pEffect);
private:
	/* Main Time */
	float m_fMainStaticStartTime;
	float m_fMainStaticEndTime;
	int m_iMainRepeat;
	
	float m_fMainStaticTime;

	CEdit m_editMainStartTime;
	CEdit m_editMainEndTime;
	CEdit m_editMainRepeat;

	/* Info */
	/* Static */
	float m_fInfoStaticPosX;
	float m_fInfoStaticPosY;
	float m_fInfoStaticPosZ;

	float m_fInfoStaticScaleX;
	float m_fInfoStaticScaleY;
	float m_fInfoStaticScaleZ;

	int m_iInfoStaticRotX;
	int m_iInfoStaticRotY;
	int m_iInfoStaticRotZ;

	/* Input */
	CEdit m_editInfoPosX;
	CEdit m_editInfoPosY;
	CEdit m_editInfoPosZ;

	CEdit m_editInfoScaleX;
	CEdit m_editInfoScaleY;
	CEdit m_editInfoScaleZ;

	CEdit m_editInfoRotX;
	CEdit m_editInfoRotY;
	CEdit m_editInfoRotZ;

	/* Pattern */

	/* Scale */
	/* Static */
	float m_fPatternStaticScaleX;
	float m_fPatternStaticScaleY;
	float m_fPatternStaticScaleZ;

	float m_fPatternStaticScaleStartTime;
	float m_fPatternStaticScaleEndTime;
	int m_iPatternStaticScaleRepeat;

	float m_fPatternStaticScaleTime;

	/* Input */
	CEdit m_editPatternScaleX;
	CEdit m_editPatternScaleY;
	CEdit m_editPatternScaleZ;

	CEdit m_editPatternScaleStartTime;
	CEdit m_editPatternScaleEndTime;
	CEdit m_editPatternScaleRepeat;

	/* Rot */
	/* Static */
	float m_fPatternStaticRotX;
	float m_fPatternStaticRotY;
	float m_fPatternStaticRotZ;

	float m_fPatternStaticRotStartTime;
	float m_fPatternStaticRotEndTime;
	int m_iPatternStaticRepeat;

	float m_fPatternStaticRotTime;

	/* Input */
	CEdit m_editPatternRotX;
	CEdit m_editPatternRotY;
	CEdit m_editPatternRotZ;

	CEdit m_editPatternRotStartTime;
	CEdit m_editPatternRotEndTime;
	CEdit m_editPatternRotRepeat;

private:
	class CEaseSheetDlg *m_pEaseSheetDlg = nullptr;

	CGameObject *m_pObjectMesh = nullptr;
	Matrix		*m_pBoneMatrix = nullptr;

	string		m_boneNameAttachTo = "";

	CGameObject *m_pTargetObject = nullptr;

	CComboBox m_comboBoxBoneInfo;
	CComboBox m_comboBoxAnimInfo;
	CComboBox m_comboEaseSheet_Scale;
	CComboBox m_comboEaseSheet_Rot;

	CButton m_checkSclaling;
	CButton m_checkRotating;
	CButton m_checkPartOrAll;

	bool m_bFirstTargetCheck = true;

public:
	afx_msg void OnBnClickedButtonInputInfo();
	afx_msg void OnBnClickedButtonInfoPosxUp();
	afx_msg void OnBnClickedButtonInfoPosxDown();
	afx_msg void OnBnClickedButtonInfoPosyUp();
	afx_msg void OnBnClickedButtonInfoPosyDown();
	afx_msg void OnBnClickedButtonInfoPoszUp();
	afx_msg void OnBnClickedButtonInfoPoszDown();
	afx_msg void OnBnClickedButtonInfoScalexUp();
	afx_msg void OnBnClickedButtonInfoScalexDown();
	afx_msg void OnBnClickedButtonInfoScaleyUp();
	afx_msg void OnBnClickedButtonInfoScaleyDown();
	afx_msg void OnBnClickedButtonInfoScalezUp();
	afx_msg void OnBnClickedButtonInfoScalezDown();
	afx_msg void OnBnClickedButtonInfoRotxUp();
	afx_msg void OnBnClickedButtonInfoRotxDown();
	afx_msg void OnBnClickedButtonInfoRotyUp();
	afx_msg void OnBnClickedButtonInfoRotyDown();
	afx_msg void OnBnClickedButtonInfoRotzUp();
	afx_msg void OnBnClickedButtonInfoRotzDown();
	afx_msg void OnBnClickedButtonViewsheet();
	afx_msg void OnBnClickedButtonViewsheet2();
	afx_msg void OnBnClickedCheckScailing();
	afx_msg void OnBnClickedButtonPatternScaleInput();
	afx_msg void OnBnClickedButtonPatternScalePlay();
	afx_msg void OnBnClickedButtonPatternScaleStop();
	afx_msg void OnBnClickedButtonPatternRotInput();
	afx_msg void OnBnClickedButtonPatternRotPlay();
	afx_msg void OnBnClickedButtonPatternRotStop();
	afx_msg void OnBnClickedCheckRotating();
	afx_msg void OnBnClickedButtonInfoMain();
	afx_msg void OnBnClickedButtonMainPlay();
	afx_msg void OnBnClickedButtonMainStop();	
};
