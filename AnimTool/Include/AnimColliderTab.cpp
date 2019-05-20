// AnimColliderTab.cpp: 구현 파일
//

#include "stdafx.h"
#include "AnimTool.h"
#include "AnimColliderTab.h"
#include "afxdialogex.h"


// CAnimColliderTab 대화 상자

IMPLEMENT_DYNAMIC(CAnimColliderTab, CDialogEx)

CAnimColliderTab::CAnimColliderTab(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_COLLIDER, pParent)
	, m_fScale(0)
	, m_fSpeed(0)
{

}

CAnimColliderTab::~CAnimColliderTab()
{
}

void CAnimColliderTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SCALE_COLLIDER, m_fScale);
	DDX_Text(pDX, IDC_EDIT_COLLIDER_SPEED, m_fSpeed);
	DDX_Radio(pDX, IDC_RADIO_COLLIDER_AXIS_1, (int&)m_iRadioAxisType);
	DDX_Control(pDX, IDC_LIST_COLLIDER, m_listCollider);
}


BEGIN_MESSAGE_MAP(CAnimColliderTab, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_CREATE_COLLIDER_WORLDMATRIX, &CAnimColliderTab::OnBnClickedButtonCreateColliderWorldmatrix)
	ON_BN_CLICKED(IDC_BUTTON_CREATE_COLLIDER_ATTACH_TO_BONE, &CAnimColliderTab::OnBnClickedButtonCreateColliderAttachToBone)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_COLLIDER, &CAnimColliderTab::OnBnClickedButtonDeleteCollider)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR_ALL_COLLIDER, &CAnimColliderTab::OnBnClickedButtonClearAllCollider)
	ON_BN_CLICKED(IDC_BUTTON_ADJUST_SCALE_COLLIDER, &CAnimColliderTab::OnBnClickedButtonAdjustScaleCollider)
	ON_BN_CLICKED(IDC_BUTTON_COLLIDER_UP, &CAnimColliderTab::OnBnClickedButtonColliderUp)
	ON_BN_CLICKED(IDC_BUTTON_COLLIDER_DOWN, &CAnimColliderTab::OnBnClickedButtonColliderDown)
	ON_BN_CLICKED(IDC_BUTTON_SPEED_MODIFY_COLLIDER, &CAnimColliderTab::OnBnClickedButtonSpeedModifyCollider)
END_MESSAGE_MAP()


// CAnimColliderTab 메시지 처리기


BOOL CAnimColliderTab::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CDialogEx::PreTranslateMessage(pMsg);
}


BOOL CAnimColliderTab::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CAnimColliderTab::OnBnClickedButtonCreateColliderWorldmatrix()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CAnimColliderTab::OnBnClickedButtonCreateColliderAttachToBone()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CAnimColliderTab::OnBnClickedButtonDeleteCollider()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CAnimColliderTab::OnBnClickedButtonClearAllCollider()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CAnimColliderTab::OnBnClickedButtonAdjustScaleCollider()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CAnimColliderTab::OnBnClickedButtonColliderUp()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CAnimColliderTab::OnBnClickedButtonColliderDown()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CAnimColliderTab::OnBnClickedButtonSpeedModifyCollider()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CAnimColliderTab::OnRadioColliderTypeCheck(UINT id)
{
	UpdateData(TRUE);

	switch (m_iRadioAxisType)
	{
	case 0:
		break;
	case 1:
		break;
	case 2:
		break;
	}
}