// TerrainTab.cpp: 구현 파일
//

#include "stdafx.h"
#include "EnvTool.h"
#include "TerrainTab.h"
#include "afxdialogex.h"


// CTerrainTab 대화 상자

IMPLEMENT_DYNAMIC(CTerrainTab, CDialogEx)

CTerrainTab::CTerrainTab(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

CTerrainTab::~CTerrainTab()
{
}

void CTerrainTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_TERRAIN_SIZEX, m_editTerrainSizeX);
	DDX_Control(pDX, IDC_EDIT_TERRIAN_SIZEY, m_editTerrainSizeY);
}


BEGIN_MESSAGE_MAP(CTerrainTab, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_ADJ_SIZE, &CTerrainTab::OnBnClickedButtonAdjSize)
END_MESSAGE_MAP()


// CTerrainTab 메시지 처리기


void CTerrainTab::OnBnClickedButtonAdjSize()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CString sizeX, sizeY;

	// 에디트 컨트롤을 가져온다.
	m_editTerrainSizeX.GetWindowTextA(sizeX);
	m_editTerrainSizeY.GetWindowTextA(sizeY);

	// string을 숫자로 변환
}
