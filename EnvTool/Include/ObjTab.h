#pragma once

#include "GameObject/GameObject.h"

PG_USING
// CObjTab 대화 상자


class CObjTab : public CDialogEx
{
	DECLARE_DYNAMIC(CObjTab)

public:
	CObjTab(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CObjTab();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	string typeName;
	static int g_iObjNumber;
	int currentType = -1;
private:
	vector<CGameObject*> m_vecObjects;
	vector<string>       m_vecStringObjTypePath;
	CGameObject*		 m_pTempObject = nullptr;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnLbnSelchangeListObjectType();
public:
	void Process_ChangeTab();
	void Process_ShowTab();
	void UpdateForm();
public:
	void AddObject();
	void Undo();
private:
	CListBox m_listObjType;
	CListBox m_listObjList;	
	CString  m_currentTypePath;
public:
	afx_msg void OnBnClickedButtonDeleteAllobj();
	afx_msg void OnBnClickedButtonDeleteObj();
	afx_msg void OnLbnSelchangeListObjects();
	afx_msg void OnBnClickedButtonObjectSave();
	afx_msg void OnBnClickedButtonObjectLoad();
};
