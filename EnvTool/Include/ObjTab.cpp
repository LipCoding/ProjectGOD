// ObjTab.cpp: 구현 파일
//

#include "stdafx.h"
#include "EnvTool.h"
#include "ObjTab.h"
#include "afxdialogex.h"


// CObjTab 대화 상자

IMPLEMENT_DYNAMIC(CObjTab, CDialogEx)

CObjTab::CObjTab(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG2, pParent)
{

}

CObjTab::~CObjTab()
{
}

void CObjTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CObjTab, CDialogEx)
END_MESSAGE_MAP()


// CObjTab 메시지 처리기
