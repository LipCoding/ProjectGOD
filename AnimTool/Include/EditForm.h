#pragma once
#include "afxwin.h"

#include "GameObject/GameObject.h"

// CEditForm 대화 상자
PG_USING

class CEditForm : public CFormView
{
	DECLARE_DYNCREATE(CEditForm)

public:
	CEditForm();   // 표준 생성자입니다.
	virtual ~CEditForm();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_FORM };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
public:
	void MeshLoadFromMeshInfoTab(CString path, CString name);
	void ArmMeshLoadFromMeshInfoTab(CString path, CString name);
	void AnimationLoadFromMeshInfoTab(CString path, CString name);
	
	void SetBoneMatrix();
	
	
	void UpdateForm(const float& fTime);

private:	
	CButton m_checkPlay;
	class CAnimToolView		*m_pView = nullptr;
	CSliderCtrl m_ctrlSliderClipFrame;
	CString m_clipName;
	int		m_iStartFrame = 0;
	int		m_iEndFrame = 0;
	CEdit	m_editFramePosition;
	CListBox m_listClips;
	int		 m_iPos = 0;
	int		 m_iRadioAnimType = 0;
	/// Tab 
	CTabCtrl m_Tab;
	// MeshInfo
	class CAnimMeshInfoTab		*m_pAnimMeshInfoDlg = nullptr;
	CGameObject *m_pEditObj = nullptr;
	CGameObject *m_pArmObj = nullptr;
	Matrix *m_pBoneMatrix = nullptr;
	// Movement

	// Collider

	// OnOff

	// CamShake

	bool m_bStopCheck = false;
	CComboBox m_comboBoxBoneInfo;

public:
	afx_msg void OnTcnSelchangeTabAnim(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonLoadMesh();
	afx_msg void OnRadioAnimTypeCheck(UINT id);

	afx_msg void OnBnClickedButtonAddClip();
	afx_msg void OnBnClickedButtonModifyClip();
	afx_msg void OnBnClickedButtonDeleteClip();
	afx_msg void OnBnClickedButtonClipDefault();
	afx_msg void OnLbnSelchangeListClips();

	afx_msg void OnBnClickedButtonPlay();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnCbnSelchangeComboBoneInfo();
};
