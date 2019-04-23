#pragma once

#include "GameObject/GameObject.h"

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
	}

	CGameObject* GetTargetObject()
	{
		return m_pTargetObject;
	}

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	void InitForm();

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
	void SetInfoPos();
	void SetInfoScale();
	void SetInfoRot();

private:
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

private:
	class CEaseSheetDlg *m_pEaseSheetDlg = nullptr;

	CGameObject *m_pObjectMesh = nullptr;
	CGameObject *m_pTargetObject = nullptr;
	Matrix		*m_pBoneMatrix = nullptr;

	string		m_boneNameAttachTo = "";

	CComboBox m_comboBoxBoneInfo;
	CComboBox m_comboBoxAnimInfo;
	CComboBox m_comboEaseSheet_Scale;
	CComboBox m_comboEaseSheet_Rot;
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
};
