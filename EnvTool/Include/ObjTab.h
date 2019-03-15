﻿#pragma once

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
	static int g_iObjNumber;
private:
	vector<CGameObject*> m_vecObjects;
	//vector<string>       m_vecStringObjTypePath;
	CGameObject*		 m_pTempObject = nullptr;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnLbnSelchangeListObjectType();
public:
	void Process_ChangeTab();
	void Process_ShowTab();
private:
	CListBox m_listObjType;
	CListBox m_listObjList;	
};
