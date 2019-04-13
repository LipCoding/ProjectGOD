#pragma once

#include "PGEngine.h"

PG_USING

// CNaviTab 대화 상자
typedef struct _tagNaviPoint
{
	Vector3 vPosition;
	//Type

	_tagNaviPoint(float x, float y, float z)
	{ 
	vPosition.x = x;
	vPosition.y = y;
	vPosition.z = z;
	}

	_tagNaviPoint(Vector3 vPos)
	{
		vPosition.x = vPos.x;
		vPosition.y = vPos.y;
		vPosition.z = vPos.z;
	}
}NAVIPOINT;

typedef struct _tagNaviCell
{
	vector<NAVIPOINT> vecPoints;
	//Type

	_tagNaviCell(vector<NAVIPOINT>& points)
	{
		vecPoints = move(points);
	}
}NAVICELL;

class CNaviTab : public CDialogEx
{
	DECLARE_DYNAMIC(CNaviTab)

public:
	CNaviTab(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CNaviTab();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG3 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

public:
	void Process_ChangeTab();
	void Process_ShowTab();
	void UpdateForm();

public:
	void Add_Point(const Vector3& vPoint);

private:
	vector<NAVIPOINT> m_vecNaviPoint;
	vector<NAVICELL>  m_vecNaviCell;
	CListBox m_listNaviPoint;
	CListBox m_listNaviCell;

	int m_NumCell = 0;
};
