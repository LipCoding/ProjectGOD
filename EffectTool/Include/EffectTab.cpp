// EffectTab.cpp: 구현 파일
//

#include "stdafx.h"
#include "EffectTool.h"
#include "EffectTab.h"
#include "afxdialogex.h"


// CEffectTab 대화 상자

IMPLEMENT_DYNAMIC(CEffectTab, CDialogEx)

CEffectTab::CEffectTab(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

CEffectTab::~CEffectTab()
{
}

void CEffectTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CEffectTab, CDialogEx)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()


// CEffectTab 메시지 처리기


BOOL CEffectTab::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CEffectTab::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}
