// AnimSpeedTab.cpp: 구현 파일
//

#include "stdafx.h"
#include "AnimTool.h"
#include "AnimSpeedTab.h"
#include "afxdialogex.h"


// CAnimSpeedTab 대화 상자

IMPLEMENT_DYNAMIC(CAnimSpeedTab, CDialogEx)

CAnimSpeedTab::CAnimSpeedTab(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_ANIM_SPEED, pParent)
{

}

CAnimSpeedTab::~CAnimSpeedTab()
{
}

void CAnimSpeedTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAnimSpeedTab, CDialogEx)
END_MESSAGE_MAP()


// CAnimSpeedTab 메시지 처리기
