// EaseSheetDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "EffectTool.h"
#include "EaseSheetDlg.h"
#include "afxdialogex.h"


// CEaseSheetDlg 대화 상자

IMPLEMENT_DYNAMIC(CEaseSheetDlg, CDialogEx)

CEaseSheetDlg::CEaseSheetDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG3, pParent)
{

}

CEaseSheetDlg::~CEaseSheetDlg()
{
}

void CEaseSheetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CEaseSheetDlg, CDialogEx)
	ON_WM_CREATE()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CEaseSheetDlg 메시지 처리기


int CEaseSheetDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	m_strBGImgPath = L".\\Resource\\Ease.png";

	return 0;
}


void CEaseSheetDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.

	m_BGImg.Destroy();
	m_BGImg.Load(m_strBGImgPath);

	if (m_resizeX <= 0 || m_resizeY <= 0)
	{
		m_resizeX = m_BGImg.GetWidth();
		m_resizeY = m_BGImg.GetHeight();
		m_verticalCent = 0;
	}

	if (m_BGImg == NULL)
	{
		AfxMessageBox(L"Load PNG Error!");
		return;
	}

	::SetStretchBltMode(dc.m_hDC, COLORONCOLOR);
	m_BGImg.StretchBlt(dc.m_hDC, 0, m_verticalCent, m_resizeX, m_resizeY, SRCCOPY);
}
