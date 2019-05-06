#pragma once
#include "afxwin.h"
#include "GameObject/GameObject.h"
#include "Component/Effect.h"

// CEditForm 대화 상자

PG_USING

class CEditForm : public CFormView
{
	DECLARE_DYNCREATE(CEditForm)
public:
	typedef struct _tagEffectData
	{
		string			strName;
		CGameObject		*pObject;
		CEffect			*pEffect;
		CTransform		*pTr;
	}EFFECTDATA;

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
	/* Getter, Setter */
	class CEffectTab* GetEffectTab()
	{
		return m_pEffectDlg;
	}
	class CEffectTab1* GetEffect1Tab()
	{
		return m_pEffect1Dlg;
	}


	vector<EFFECTDATA*> *GetEffects()
	{
		return &m_vecEffect;
	}
	void SetTargetEffect(EFFECTDATA *curEffect)
	{
		m_pCurEffect = curEffect;
	}

private:
	vector<EFFECTDATA*> m_vecEffect;
	EFFECTDATA* m_pCurEffect = nullptr;

	class CEnvToolView  *m_pView = nullptr;
	class CEffectTab	*m_pEffectDlg = nullptr;
	class CEffectTab1	*m_pEffect1Dlg = nullptr;
	TOOLTAB_TYPE		m_eTabType = TAB_END;

	int m_iEffectNumber = 0;

public:
	virtual void OnInitialUpdate();
	void UpdateForm();
	void UpdateTarget(class CGameObject* object);
	
	void CloneTarget();

private:
	void FreeEffectData(EFFECTDATA* effect);
private:
	CTabCtrl m_Tab;
	CListBox m_listEffectList;
	CButton m_checkBillBoard;
public:
	afx_msg void OnTcnSelchangeTabEffect(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonLoadParticle();
	afx_msg void OnBnClickedButtonLoadMeshTexture();
	afx_msg void OnBnClickedButtonLoadMesh();
	afx_msg void OnBnClickedCheckBillboard();
	afx_msg void OnLbnSelchangeListEffectContainer();
	afx_msg void OnBnClickedButtonDelete();
	afx_msg void OnBnClickedButtonClearAll();
	afx_msg void OnBnClickedButtonEffectSave();
	afx_msg void OnBnClickedButtonEffectLoad();
};
