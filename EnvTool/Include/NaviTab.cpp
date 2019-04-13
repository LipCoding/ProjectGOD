// NaviTab.cpp: 구현 파일
//

#include "stdafx.h"
#include "EnvTool.h"
#include "NaviTab.h"
#include "afxdialogex.h"


// CNaviTab 대화 상자

IMPLEMENT_DYNAMIC(CNaviTab, CDialogEx)

CNaviTab::CNaviTab(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG3, pParent)
{

}

CNaviTab::~CNaviTab()
{
}

void CNaviTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CNaviTab, CDialogEx)
END_MESSAGE_MAP()


// CNaviTab 메시지 처리기


BOOL CNaviTab::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CNaviTab::Process_ChangeTab()
{
}

void CNaviTab::Process_ShowTab()
{
}

void CNaviTab::UpdateForm()
{
}
