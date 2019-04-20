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

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	void InitForm();

	/* Load Target Resource */
	bool LoadTargetMesh(const CString& filePath);
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

	float m_fInfoStaticRotX;
	float m_fInfoStaticRotY;
	float m_fInfoStaticRotZ;

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
	CGameObject *m_pObjectMesh = nullptr;
	CGameObject *m_pTargetObject = nullptr;
	Matrix		*m_pBoneMatrix = nullptr;

	string		m_boneNameAttachTo = "";

	CComboBox m_comboBoxBoneInfo;
	CComboBox m_comboBoxAnimInfo;
public:
	afx_msg void OnBnClickedButtonInputInfo();
};
