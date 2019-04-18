// CEffectTab1.cpp: 구현 파일
//

#include "stdafx.h"
#include "EffectTool.h"
#include "EffectTab1.h"
#include "afxdialogex.h"


// CEffectTab1 대화 상자

IMPLEMENT_DYNAMIC(CEffectTab1, CDialogEx)

CEffectTab1::CEffectTab1(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG2, pParent)
{

}

CEffectTab1::~CEffectTab1()
{
}

void CEffectTab1::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CEffectTab1, CDialogEx)
END_MESSAGE_MAP()


// CEffectTab1 메시지 처리기
