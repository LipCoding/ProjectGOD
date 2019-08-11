// CEffectTab1.cpp: 구현 파일
//

#include "stdafx.h"
#include "EffectTool.h"
#include "EffectTab1.h"
#include "afxdialogex.h"

#include "EffectTab.h"

#include "../Component/EffectAssist.h"


// CEffectTab1 대화 상자

IMPLEMENT_DYNAMIC(CEffectTab1, CDialogEx)

CEffectTab1::CEffectTab1(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG2, pParent)
	, m_fFadeInStaticStartTime(0)
	, m_fFadeInStaticEndTime(0)
	, m_fFadeInStaticDegree(0)
	, m_fFadeInStaticTime(0)
	, m_fFadeOutStaticStartTime(0)
	, m_fFadeOutStaticEndTime(0)
	, m_fFadeOutStaticDegree(0)
	, m_fFadeOutStaticTime(0)
	, m_fUVSpriteStaticStartTime(0)
	, m_fUVSpriteStaticEndTime(0)
	, m_iUVSpriteStaticNum(0)
	, m_fUVSpriteTime(0)
	, m_fMoveUVStaticStartTime(0)
	, m_fMoveUVStaticEndTime(0)
	, m_fMoveUVStaticDirX(0)
	, m_fMoveUVStaticDirY(0)
	, m_fMoveUVStaticTime(0)
	, m_iUVSpriteStaticCountX(0)
	, m_iUVSpriteStaticCountY(0)
	, m_iUVSpriteStaticMax_X(0)
	, m_iUVSpriteStaticMax_Y(0)
	, m_iRadioSpriteType(-1)
{

}

CEffectTab1::~CEffectTab1()
{
}

void CEffectTab1::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_FADE_IN_STATIC_START_TIME, m_fFadeInStaticStartTime);
	DDX_Text(pDX, IDC_EDIT_FADE_IN_STATIC_END_TIME, m_fFadeInStaticEndTime);
	DDX_Text(pDX, IDC_EDIT_FADE_IN_STATIC_DEGREE, m_fFadeInStaticDegree);
	DDX_Text(pDX, IDC_EDIT_FADE_IN_STATIC_TIME, m_fFadeInStaticTime);
	DDX_Control(pDX, IDC_EDIT_FADE_IN_START_TIME, m_editFadeInStartTime);
	DDX_Control(pDX, IDC_EDIT_FADE_IN_END_TIME, m_editFadeInEndTime);
	DDX_Control(pDX, IDC_EDIT_FADE_IN_DEGREE, m_editFadeInDegree);
	DDX_Text(pDX, IDC_EDIT_FADE_OUT_STATIC_START_TIME, m_fFadeOutStaticStartTime);
	DDX_Text(pDX, IDC_EDIT_FADE_OUT_STATIC_END_TIME, m_fFadeOutStaticEndTime);
	DDX_Text(pDX, IDC_EDIT_FADE_OUT_STATIC_DEGREE, m_fFadeOutStaticDegree);
	DDX_Text(pDX, IDC_EDIT_FADE_OUT_STATIC_TIME, m_fFadeOutStaticTime);
	DDX_Control(pDX, IDC_EDIT_FADE_OUT_START_TIME, m_editFadeOutStartTime);
	DDX_Control(pDX, IDC_EDIT_FADE_OUT_END_TIME, m_editFadeOutEndTime);
	DDX_Control(pDX, IDC_EDIT_FADE_OUT_DEGREE, m_editFadeOutDegree);
	DDX_Control(pDX, IDC_CHECK_FADE_IN, m_checkFadeIn);
	DDX_Control(pDX, IDC_CHECK_FADE_OUT, m_checkFadeOut);
	DDX_Text(pDX, IDC_EDIT_UV_SPRITE_STATIC_START_TIME, m_fUVSpriteStaticStartTime);
	DDX_Text(pDX, IDC_EDIT_UV_SPRITE_STATIC_END_TIME, m_fUVSpriteStaticEndTime);
	DDX_Text(pDX, IDC_EDIT_UV_SPRITE_STATIC_NUM, m_iUVSpriteStaticNum);
	DDX_Control(pDX, IDC_EDIT_UV_SPRITE_START_TIME, m_editUVSpriteStartTime);
	DDX_Control(pDX, IDC_EDIT_UV_SPRITE_END_TIME, m_editUVSpriteEndTime);
	DDX_Control(pDX, IDC_EDIT_UV_SPRITE_NUM, m_editUVSpriteNum);
	DDX_Control(pDX, IDC_CHECK_UV_SPRITE, m_checkUVSprite);
	DDX_Text(pDX, IDC_EDIT_UV_SPRITE_STATIC_TIME, m_fUVSpriteTime);
	DDX_Text(pDX, IDC_EDIT_UV_MOVE_STATIC_START_TIME, m_fMoveUVStaticStartTime);
	DDX_Text(pDX, IDC_EDIT_UV_MOVE_STATIC_END_TIME, m_fMoveUVStaticEndTime);
	DDX_Text(pDX, IDC_EDIT_UV_MOVE_STATIC_MOVE_X, m_fMoveUVStaticDirX);
	DDX_Text(pDX, IDC_EDIT_UV_MOVE_STATIC_MOVE_Y, m_fMoveUVStaticDirY);
	DDX_Control(pDX, IDC_EDIT_UV_MOVE_START_TIME, m_editMoveUVStartTime);
	DDX_Control(pDX, IDC_EDIT_UV_MOVE_END_TIME, m_editMoveUVEndTime);
	DDX_Control(pDX, IDC_EDIT_UV_MOVE_DIR_X, m_editMoveUVDirX);
	DDX_Control(pDX, IDC_EDIT_UV_MOVE_DIR_Y, m_editMoveUVDirY);
	DDX_Text(pDX, IDC_EDIT_UV_MOVE_STATIC_TIME, m_fMoveUVStaticTime);
	DDX_Control(pDX, IDC_CHECK_UV, m_checkUVMove);
	DDX_Control(pDX, IDC_CHECK_INFINITE_FADE_IN, m_checkInfiniteFadeIn);
	DDX_Control(pDX, IDC_CHECK_INFINITE_FADE_OUT, m_checkInfiniteFadeOut);
	DDX_Control(pDX, IDC_CHECK_INFINITE_UV_MOVE, m_checkInfiniteUVMove);
	DDX_Radio(pDX, IDC_RADIO_SPRITE_TYPE_1, (int&)m_iRadioSpriteType);
	DDX_Text(pDX, IDC_EDIT_UV_SPRITE_STATIC_WIDTH, m_iUVSpriteStaticCountX);
	DDX_Text(pDX, IDC_EDIT_UV_SPRITE_STATIC_HEIGHT, m_iUVSpriteStaticCountY);
	DDX_Text(pDX, IDC_EDIT_UV_SPRITE_STATIC_MAX_X, m_iUVSpriteStaticMax_X);
	DDX_Text(pDX, IDC_EDIT_UV_SPRITE_STATIC_MAX_Y, m_iUVSpriteStaticMax_Y);
	DDX_Control(pDX, IDC_EDIT_UV_SPRITE_WIDTH, m_editUVSpriteCountX);
	DDX_Control(pDX, IDC_EDIT_UV_SPRITE_HEIGHT, m_editUVSpriteCountY);
	DDX_Control(pDX, IDC_EDIT_UV_SPRITE_MAX_X, m_editUVSpriteMax_X);
	DDX_Control(pDX, IDC_EDIT_UV_SPRITE_MAX_Y, m_editUVSpriteMax_Y);
	DDX_Control(pDX, IDC_CHECK_INFINITE_UV_SPRITE, m_checkInfiniteUVSprite);
}


BEGIN_MESSAGE_MAP(CEffectTab1, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_FADE_IN_INPUT, &CEffectTab1::OnBnClickedButtonFadeInInput)
	ON_BN_CLICKED(IDC_BUTTON_FADE_IN_PLAY, &CEffectTab1::OnBnClickedButtonFadeInPlay)
	ON_BN_CLICKED(IDC_BUTTON_FADE_IN_STOP, &CEffectTab1::OnBnClickedButtonFadeInStop)
	ON_BN_CLICKED(IDC_BUTTON_FADE_OUT_INPUT, &CEffectTab1::OnBnClickedButtonFadeOutInput)
	ON_BN_CLICKED(IDC_BUTTON_FADE_OUT_PLAY, &CEffectTab1::OnBnClickedButtonFadeOutPlay)
	ON_BN_CLICKED(IDC_BUTTON_FADE_OUT_STOP, &CEffectTab1::OnBnClickedButtonFadeOutStop)
	ON_BN_CLICKED(IDC_CHECK_FADE_IN, &CEffectTab1::OnBnClickedCheckFadeIn)
	ON_BN_CLICKED(IDC_CHECK_FADE_OUT, &CEffectTab1::OnBnClickedCheckFadeOut)
	ON_BN_CLICKED(IDC_BUTTON_UV_SPRITE_INPUT, &CEffectTab1::OnBnClickedButtonUvSpriteInput)
	ON_BN_CLICKED(IDC_BUTTON_UV_SPRITE_PLAY, &CEffectTab1::OnBnClickedButtonUvSpritePlay)
	ON_BN_CLICKED(IDC_BUTTON_UV_SPRITE_STOP, &CEffectTab1::OnBnClickedButtonUvSpriteStop)
	ON_BN_CLICKED(IDC_CHECK_UV_SPRITE, &CEffectTab1::OnBnClickedCheckUvSprite)
	ON_BN_CLICKED(IDC_BUTTON_UV_MOVE_INPUT, &CEffectTab1::OnBnClickedButtonUvMoveInput)
	ON_BN_CLICKED(IDC_BUTTON_UV_MOVE_PLAY, &CEffectTab1::OnBnClickedButtonUvMovePlay)
	ON_BN_CLICKED(IDC_BUTTON_UV_MOVE_STOP, &CEffectTab1::OnBnClickedButtonUvMoveStop)
	ON_BN_CLICKED(IDC_CHECK_UV, &CEffectTab1::OnBnClickedCheckUv)
	ON_BN_CLICKED(IDC_CHECK_INFINITE_FADE_IN, &CEffectTab1::OnBnClickedCheckInfiniteFadeIn)
	ON_BN_CLICKED(IDC_CHECK_INFINITE_FADE_OUT, &CEffectTab1::OnBnClickedCheckInfiniteFadeOut)
	ON_BN_CLICKED(IDC_CHECK_INFINITE_UV_MOVE, &CEffectTab1::OnBnClickedCheckInfiniteUvMove)
	ON_BN_CLICKED(IDC_RADIO_SPRITE_TYPE_1, &CEffectTab1::OnBnClickedRadioSpriteType1)
	ON_BN_CLICKED(IDC_RADIO_SPRITE_TYPE_2, &CEffectTab1::OnBnClickedRadioSpriteType2)
	ON_BN_CLICKED(IDC_CHECK_INFINITE_UV_SPRITE, &CEffectTab1::OnBnClickedCheckInfiniteUvSprite)
END_MESSAGE_MAP()


// CEffectTab1 메시지 처리기

void CEffectTab1::OnBnClickedButtonFadeInInput()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!m_pTargetObject)
		return;

	CEffect *pEffect = m_pTargetObject->FindComponentFromType<CEffect>(CT_EFFECT);
	if (!pEffect)
		return;

	if (m_checkFadeIn.GetCheck() == 0)
	{
		AfxMessageBox(L"Add fade in first!");
		return;
	}

	AddFadeIn(pEffect);

	UpdateFade();

	SAFE_RELEASE(pEffect);
}


void CEffectTab1::OnBnClickedButtonFadeInPlay()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!m_pTargetObject)
		return;

	CEffect *pEffect = m_pTargetObject->FindComponentFromType<CEffect>(CT_EFFECT);
	if (!pEffect)
		return;

	if (m_checkFadeIn.GetCheck() == 0)
	{
		AfxMessageBox(L"Add fade in first!");
		return;
	}

	pEffect->SetOperationCheckPart(CEffectAssist::ASSIST_FADE_IN, true);

	UpdateFade();

	SAFE_RELEASE(pEffect);
}


void CEffectTab1::OnBnClickedButtonFadeInStop()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!m_pTargetObject)
		return;

	CEffect *pEffect = m_pTargetObject->FindComponentFromType<CEffect>(CT_EFFECT);
	if (!pEffect)
		return;

	if (m_checkFadeIn.GetCheck() == 0)
	{
		AfxMessageBox(L"Add fade in first!");
		return;
	}

	pEffect->SetOperationCheckPart(CEffectAssist::ASSIST_FADE_IN, false);
	m_pTargetObject->SetRenderEnable(true);

	UpdateFade();

	SAFE_RELEASE(pEffect);
}


void CEffectTab1::OnBnClickedButtonFadeOutInput()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!m_pTargetObject)
		return;

	CEffect *pEffect = m_pTargetObject->FindComponentFromType<CEffect>(CT_EFFECT);
	if (!pEffect)
		return;

	if (m_checkFadeOut.GetCheck() == 0)
	{
		AfxMessageBox(L"Add fade out first!");
		return;
	}

	AddFadeOut(pEffect);

	UpdateFade();

	SAFE_RELEASE(pEffect);
}


void CEffectTab1::OnBnClickedButtonFadeOutPlay()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!m_pTargetObject)
		return;

	CEffect *pEffect = m_pTargetObject->FindComponentFromType<CEffect>(CT_EFFECT);
	if (!pEffect)
		return;

	if (m_checkFadeOut.GetCheck() == 0)
	{
		AfxMessageBox(L"Add fade out first!");
		return;
	}

	pEffect->SetOperationCheckPart(CEffectAssist::ASSIST_FADE_OUT, true);

	UpdateFade();

	SAFE_RELEASE(pEffect);
}


void CEffectTab1::OnBnClickedButtonFadeOutStop()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!m_pTargetObject)
		return;

	CEffect *pEffect = m_pTargetObject->FindComponentFromType<CEffect>(CT_EFFECT);
	if (!pEffect)
		return;

	if (m_checkFadeOut.GetCheck() == 0)
	{
		AfxMessageBox(L"Add fade out first!");
		return;
	}

	pEffect->SetOperationCheckPart(CEffectAssist::ASSIST_FADE_OUT, false);
	m_pTargetObject->SetRenderEnable(true);

	UpdateFade();

	SAFE_RELEASE(pEffect);
}


void CEffectTab1::OnBnClickedCheckFadeIn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!m_pTargetObject)
		return;

	CEffect *pEffect = m_pTargetObject->FindComponentFromType<CEffect>(CT_EFFECT);
	if (!pEffect)
		return;

	int check = m_checkFadeIn.GetCheck();

	/* Check를 푸는 동작 */
	if (check == 1)
	{
		AddFadeIn(pEffect);
		m_checkFadeIn.SetCheck(1);
	}
	/* Check를 하는 동작 */
	else
	{
		pEffect->DeleteAssistEffectFromType(CEffectAssist::ASSIST_FADE_IN);
		m_checkFadeIn.SetCheck(0);
	}

	UpdateFade();

	SAFE_RELEASE(pEffect);
}


void CEffectTab1::OnBnClickedCheckFadeOut()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!m_pTargetObject)
		return;

	CEffect *pEffect = m_pTargetObject->FindComponentFromType<CEffect>(CT_EFFECT);
	if (!pEffect)
		return;

	int check = m_checkFadeOut.GetCheck();

	/* Check를 푸는 동작 */
	if (check == 1)
	{
		AddFadeOut(pEffect);
		m_checkFadeOut.SetCheck(1);
	}
	/* Check를 하는 동작 */
	else
	{
		pEffect->DeleteAssistEffectFromType(CEffectAssist::ASSIST_FADE_OUT);
		m_checkFadeOut.SetCheck(0);
	}

	UpdateFade();

	SAFE_RELEASE(pEffect);
}

void CEffectTab1::UpdateForm()
{
	if (m_pTargetObject)
	{
		if (m_bFirstTargetCheck)
		{
			UpdateFade();
			UpdateUV();
			m_bFirstTargetCheck = false;
		}

		UpdateTime();
	}
	else
	{
		InitFormValue();
		m_bFirstTargetCheck = true;
	}

	UpdateData(FALSE);
}

void CEffectTab1::InitForm()
{
}

void CEffectTab1::InitFormValue()
{
	/* Check */
	m_checkFadeIn.SetCheck(0);
	m_checkFadeOut.SetCheck(0);
	m_checkUVSprite.SetCheck(0);
	m_checkUVMove.SetCheck(0);

	m_checkInfiniteFadeIn.SetCheck(0);
	m_checkInfiniteFadeOut.SetCheck(0);
	m_checkInfiniteUVMove.SetCheck(0);
	m_checkInfiniteUVSprite.SetCheck(0);

	InitFadeIn();
	InitFadeOut();
	InitUVSprite();
	InitUVMove();
}

void CEffectTab1::InitFadeIn()
{
	m_fFadeInStaticStartTime = -1.f;
	m_fFadeInStaticEndTime = -1.f;
	m_fFadeInStaticDegree = -1.f;

	m_fFadeInStaticTime = -1.f;

	m_editFadeInStartTime.SetWindowTextW(L"");
	m_editFadeInEndTime.SetWindowTextW(L"");
	m_editFadeInDegree.SetWindowTextW(L"");
}

void CEffectTab1::InitFadeOut()
{
	m_fFadeOutStaticStartTime = -1.f;
	m_fFadeOutStaticEndTime = -1.f;
	m_fFadeOutStaticDegree = -1.f;

	m_fFadeOutStaticTime = -1.f;

	m_editFadeOutStartTime.SetWindowTextW(L"");
	m_editFadeOutEndTime.SetWindowTextW(L"");
	m_editFadeOutDegree.SetWindowTextW(L"");
}

void CEffectTab1::InitUVSprite()
{
	m_iRadioSpriteType = -1;

	m_fUVSpriteStaticStartTime = -1.f;
	m_fUVSpriteStaticEndTime = -1.f;
	m_iUVSpriteStaticNum = -1;

	m_fUVSpriteTime = -1.f;

	m_iUVSpriteStaticCountX = -1;
	m_iUVSpriteStaticCountY = -1;
	m_iUVSpriteStaticMax_X = -1;
	m_iUVSpriteStaticMax_Y = -1;

	m_editUVSpriteStartTime.SetWindowTextW(L"");
	m_editUVSpriteEndTime.SetWindowTextW(L"");
	m_editUVSpriteNum.SetWindowTextW(L"");

	m_editUVSpriteCountX.SetWindowTextW(L"");
	m_editUVSpriteCountY.SetWindowTextW(L"");
	m_editUVSpriteMax_X.SetWindowTextW(L"");
	m_editUVSpriteMax_Y.SetWindowTextW(L"");
}

void CEffectTab1::InitUVMove()
{
	m_fMoveUVStaticStartTime = -1.f;
	m_fMoveUVStaticEndTime = -1.f;
	m_fMoveUVStaticDirX = -1.f;
	m_fMoveUVStaticDirY = -1.f;

	m_fMoveUVStaticTime = -1.f;

	m_editMoveUVStartTime.SetWindowTextW(L"");
	m_editMoveUVEndTime.SetWindowTextW(L"");
	m_editMoveUVDirX.SetWindowTextW(L"");
	m_editMoveUVDirY.SetWindowTextW(L"");
}

void CEffectTab1::UpdateFade()
{
	CEffect *pEffect = m_pTargetObject->FindComponentFromType<CEffect>(CT_EFFECT);

	if (pEffect)
	{
		/* Fade In */
		CEffectAssist *pAssistFadeIn = pEffect->GetAssistFromType(CEffectAssist::ASSIST_FADE_IN);

		if (pAssistFadeIn)
		{
			m_fFadeInStaticStartTime = pAssistFadeIn->GetStartTime();
			m_fFadeInStaticEndTime = pAssistFadeIn->GetEndTime();
			m_fFadeInStaticDegree = pAssistFadeIn->GetDegree();

			CString tempNum;
			tempNum.Format(_T("%.2f"), m_fFadeInStaticStartTime);
			m_editFadeInStartTime.SetWindowTextW(tempNum);
			tempNum.Format(_T("%.2f"), m_fFadeInStaticEndTime);
			m_editFadeInEndTime.SetWindowTextW(tempNum);
			tempNum.Format(_T("%.2f"), m_fFadeInStaticDegree);
			m_editFadeInDegree.SetWindowTextW(tempNum);

			m_checkFadeIn.SetCheck(1);

			if (pAssistFadeIn->GetInifiniteCheck() == true)
				m_checkInfiniteFadeIn.SetCheck(1);
			else
				m_checkInfiniteFadeIn.SetCheck(0);
			
		}
		else
		{
			InitFadeIn();
			m_checkFadeIn.SetCheck(0);
			m_checkInfiniteFadeIn.SetCheck(0);
		}

		/* Fade Out */
		CEffectAssist *pAssistFadeOut = pEffect->GetAssistFromType(CEffectAssist::ASSIST_FADE_OUT);

		if (pAssistFadeOut)
		{
			m_fFadeOutStaticStartTime = pAssistFadeOut->GetStartTime();
			m_fFadeOutStaticEndTime = pAssistFadeOut->GetEndTime();
			m_fFadeOutStaticDegree = pAssistFadeOut->GetDegree();

			CString tempNum;
			tempNum.Format(_T("%.2f"), m_fFadeOutStaticStartTime);
			m_editFadeOutStartTime.SetWindowTextW(tempNum);
			tempNum.Format(_T("%.2f"), m_fFadeOutStaticEndTime);
			m_editFadeOutEndTime.SetWindowTextW(tempNum);
			tempNum.Format(_T("%.2f"), m_fFadeOutStaticDegree);
			m_editFadeOutDegree.SetWindowTextW(tempNum);

			m_checkFadeOut.SetCheck(1);

			if (pAssistFadeOut->GetInifiniteCheck() == true)
				m_checkInfiniteFadeOut.SetCheck(1);
			else
				m_checkInfiniteFadeOut.SetCheck(0);
		}
		else
		{
			InitFadeOut();
			m_checkFadeOut.SetCheck(0);
			m_checkInfiniteFadeOut.SetCheck(0);
		}

		SAFE_RELEASE(pEffect);
	}
}

void CEffectTab1::UpdateUV()
{
	CEffect *pEffect = m_pTargetObject->FindComponentFromType<CEffect>(CT_EFFECT);

	if (pEffect)
	{
		/* UV Sprite */
		CEffectAssist *pAssistUVAni = pEffect->GetAssistFromType(CEffectAssist::ASSIST_UV_ANI);

		if (pAssistUVAni)
		{
			CEffectAssist::SPRITE_TYPE eType;

			eType = pAssistUVAni->GetSpriteType();

			if (CEffectAssist::SPRITE_ATLAS == eType)
			{
				m_iRadioSpriteType = 1;
			}
			else if(CEffectAssist::SPRITE_FRAME == eType)
			{
				m_iRadioSpriteType = 0;
			}

			CString tempNum;

			m_fUVSpriteStaticStartTime = pAssistUVAni->GetStartTime();
			m_fUVSpriteStaticEndTime = pAssistUVAni->GetEndTime();
			m_iUVSpriteStaticNum = pAssistUVAni->GetNum();

			tempNum.Format(_T("%.2f"), m_fUVSpriteStaticStartTime);
			m_editUVSpriteStartTime.SetWindowTextW(tempNum);
			tempNum.Format(_T("%.2f"), m_fUVSpriteStaticEndTime);
			m_editUVSpriteEndTime.SetWindowTextW(tempNum);
			tempNum.Format(_T("%d"), m_iUVSpriteStaticNum);
			m_editUVSpriteNum.SetWindowTextW(tempNum);

			m_iUVSpriteStaticCountX = pAssistUVAni->GetMaxX() / pAssistUVAni->GetWidth();
			m_iUVSpriteStaticCountY = pAssistUVAni->GetMaxY() / pAssistUVAni->GetHeight();
			m_iUVSpriteStaticMax_X = pAssistUVAni->GetMaxX();
			m_iUVSpriteStaticMax_Y = pAssistUVAni->GetMaxY();

			tempNum.Format(_T("%d"), m_iUVSpriteStaticCountX);
			m_editUVSpriteCountX.SetWindowTextW(tempNum);
			tempNum.Format(_T("%d"), m_iUVSpriteStaticCountY);
			m_editUVSpriteCountY.SetWindowTextW(tempNum);
			tempNum.Format(_T("%d"), m_iUVSpriteStaticMax_X);
			m_editUVSpriteMax_X.SetWindowTextW(tempNum);
			tempNum.Format(_T("%d"), m_iUVSpriteStaticMax_Y);
			m_editUVSpriteMax_Y.SetWindowTextW(tempNum);

			m_checkUVSprite.SetCheck(1);

			if (pAssistUVAni->GetInifiniteCheck() == true)
				m_checkInfiniteUVSprite.SetCheck(1);
			else
				m_checkInfiniteUVSprite.SetCheck(0);
		}
		else
		{
			InitUVSprite();
			m_checkUVSprite.SetCheck(0);
			m_checkInfiniteUVSprite.SetCheck(0);
		}

		/* UV Move */
		CEffectAssist *pAssistUVMove = pEffect->GetAssistFromType(CEffectAssist::ASSIST_UV_MOVE);

		if (pAssistUVMove)
		{
			m_fMoveUVStaticStartTime = pAssistUVMove->GetStartTime();
			m_fMoveUVStaticEndTime = pAssistUVMove->GetEndTime();
			m_fMoveUVStaticDirX = pAssistUVMove->GetMoveUV_X();
			m_fMoveUVStaticDirY = pAssistUVMove->GetMoveUV_Y();

			CString tempNum;
			tempNum.Format(_T("%.2f"), m_fMoveUVStaticStartTime);
			m_editMoveUVStartTime.SetWindowTextW(tempNum);
			tempNum.Format(_T("%.2f"), m_fMoveUVStaticEndTime);
			m_editMoveUVEndTime.SetWindowTextW(tempNum);
			tempNum.Format(_T("%.2f"), m_fMoveUVStaticDirX);
			m_editMoveUVDirX.SetWindowTextW(tempNum);
			tempNum.Format(_T("%.2f"), m_fMoveUVStaticDirY);
			m_editMoveUVDirY.SetWindowTextW(tempNum);

			m_checkUVMove.SetCheck(1);

			if (pAssistUVMove->GetInifiniteCheck() == true)
				m_checkInfiniteUVMove.SetCheck(1);
			else
				m_checkInfiniteUVMove.SetCheck(0);
			
		}
		else
		{
			InitUVMove();
			m_checkUVMove.SetCheck(0);
			m_checkInfiniteUVMove.SetCheck(0);
		}

		SAFE_RELEASE(pEffect);
	}
}

void CEffectTab1::UpdateTime()
{
	if (!m_pTargetObject)
		return;

	CEffect *pEffect = m_pTargetObject->FindComponentFromType<CEffect>(CT_EFFECT);
	if (!pEffect)
		return;

	CEffectAssist *pAssist = nullptr;

	/* Check 되어 있다면 */

	/* Fade */
	if (m_checkFadeIn.GetCheck() == 1)
	{
		pAssist = pEffect->GetAssistFromType(CEffectAssist::ASSIST_FADE_IN);

		if (pAssist)
			m_fFadeInStaticTime = pAssist->GetTime();
	}

	if (m_checkFadeOut.GetCheck() == 1)
	{
		pAssist = pEffect->GetAssistFromType(CEffectAssist::ASSIST_FADE_OUT);

		if (pAssist)
			m_fFadeOutStaticTime = pAssist->GetTime();
	}

	/* UV */
	if (m_checkUVSprite.GetCheck() == 1)
	{
		pAssist = pEffect->GetAssistFromType(CEffectAssist::ASSIST_UV_ANI);

		if (pAssist)
			m_fUVSpriteTime = pAssist->GetTime();
	}

	/* UV Move */
	if (m_checkUVMove.GetCheck() == 1)
	{
		pAssist = pEffect->GetAssistFromType(CEffectAssist::ASSIST_UV_MOVE);

		if (pAssist)
			m_fMoveUVStaticTime = pAssist->GetTime();
	}

	SAFE_RELEASE(pEffect);
}

void CEffectTab1::AddFadeIn(CEffect * pEffect)
{
	CString StartTime, EndTime, Degree;

	m_editFadeInStartTime.GetWindowTextW(StartTime);
	m_editFadeInEndTime.GetWindowTextW(EndTime);
	m_editFadeInDegree.GetWindowTextW(Degree);

	if (StartTime == L"")
		m_fFadeInStaticStartTime = 0.f;
	else
		m_fFadeInStaticStartTime = (float)_wtof(StartTime);

	if (EndTime == L"")
		m_fFadeInStaticEndTime = 0.f;
	else
		m_fFadeInStaticEndTime = (float)_wtof(EndTime);

	if (Degree == L"")
		m_fFadeInStaticDegree = 0.f;
	else
		m_fFadeInStaticDegree = (float)_wtof(Degree);

	pEffect->AddFadeIn(m_fFadeInStaticStartTime, m_fFadeInStaticEndTime, m_fFadeInStaticDegree);
}

void CEffectTab1::AddFadeOut(CEffect * pEffect)
{
	CString StartTime, EndTime, Degree;

	m_editFadeOutStartTime.GetWindowTextW(StartTime);
	m_editFadeOutEndTime.GetWindowTextW(EndTime);
	m_editFadeOutDegree.GetWindowTextW(Degree);

	if (StartTime == L"")
		m_fFadeOutStaticStartTime = 0.f;
	else
		m_fFadeOutStaticStartTime = (float)_wtof(StartTime);

	if (EndTime == L"")
		m_fFadeOutStaticEndTime = 0.f;
	else
		m_fFadeOutStaticEndTime = (float)_wtof(EndTime);

	if (Degree == L"")
		m_fFadeOutStaticDegree = 0.f;
	else
		m_fFadeOutStaticDegree = (float)_wtof(Degree);

	pEffect->AddFadeOut(m_fFadeOutStaticStartTime, m_fFadeOutStaticEndTime, m_fFadeOutStaticDegree);
}

bool CEffectTab1::AddUVSprite(CEffect * pEffect)
{
	CEffectAssist* pAssistUVMove = pEffect->GetAssistFromType(CEffectAssist::ASSIST_UV_MOVE);

	if (pAssistUVMove)
	{
		AfxMessageBox(L"You already have UV Move Assist!");
		return false;
	}

	CString StartTime, EndTime;

	m_editUVSpriteStartTime.GetWindowTextW(StartTime);
	m_editUVSpriteEndTime.GetWindowTextW(EndTime);

	if (StartTime == L"")
		m_fUVSpriteStaticStartTime = 0.f;
	else
		m_fUVSpriteStaticStartTime = (float)_wtof(StartTime);

	if (EndTime == L"")
		m_fUVSpriteStaticEndTime = 0.f;
	else
		m_fUVSpriteStaticEndTime = (float)_wtof(EndTime);

	switch (m_iRadioSpriteType)
	{
	case SPRITE_TYPE_FRAME:
	{
		CString Num;
		m_editUVSpriteNum.GetWindowTextW(Num);

		if (Num == L"")
			m_iUVSpriteStaticNum = 0;
		else
			m_iUVSpriteStaticNum = (int)_wtoi(Num);

		pEffect->AddUVAnimation(m_fUVSpriteStaticStartTime, m_fUVSpriteStaticEndTime, m_iUVSpriteStaticNum, 1);
		break;
	}
	case SPRITE_TYPE_ATLAS:
	{
		CString Count_X, Count_Y, Max_X, Max_Y;

		m_editUVSpriteCountX.GetWindowTextW(Count_X);
		m_editUVSpriteCountY.GetWindowTextW(Count_Y);
		m_editUVSpriteMax_X.GetWindowTextW(Max_X);
		m_editUVSpriteMax_Y.GetWindowTextW(Max_Y);

		if (Count_X == L"")
			m_iUVSpriteStaticCountX = 0;
		else
			m_iUVSpriteStaticCountX = (int)_wtoi(Count_X);

		if (Count_Y == L"")
			m_iUVSpriteStaticCountY = 0;
		else
			m_iUVSpriteStaticCountY = (int)_wtoi(Count_Y);

		if (Max_X == L"")
			m_iUVSpriteStaticMax_X = 0;
		else
			m_iUVSpriteStaticMax_X = (int)_wtoi(Max_X);

		if (Max_Y == L"")
			m_iUVSpriteStaticMax_Y = 0;
		else
			m_iUVSpriteStaticMax_Y = (int)_wtoi(Max_Y);

		int iEachSpriteSizeWidth = m_iUVSpriteStaticMax_X / m_iUVSpriteStaticCountX;
		int iEachSpriteSizeHeight = m_iUVSpriteStaticMax_Y / m_iUVSpriteStaticCountY;

		pEffect->AddUVAnimation(m_fUVSpriteStaticStartTime, m_fUVSpriteStaticEndTime,
								m_iUVSpriteStaticMax_X, m_iUVSpriteStaticMax_Y,
								iEachSpriteSizeWidth, iEachSpriteSizeHeight, 1);
		break;
	}
	default:
	{
		AfxMessageBox(L"Check Sprite type first!");
		return false;
	}
	}

	return true;
}

bool CEffectTab1::AddUVMove(CEffect * pEffect)
{
	CEffectAssist* pAssistUVAni = pEffect->GetAssistFromType(CEffectAssist::ASSIST_UV_ANI);

	if (pAssistUVAni)
	{
		AfxMessageBox(L"You already have UV Ani Assist!");
		return false;
	}

	CString StartTime, EndTime, DirX, DirY;

	m_editMoveUVStartTime.GetWindowTextW(StartTime);
	m_editMoveUVEndTime.GetWindowTextW(EndTime);
	m_editMoveUVDirX.GetWindowTextW(DirX);
	m_editMoveUVDirY.GetWindowTextW(DirY);

	if (StartTime == L"")
		m_fMoveUVStaticStartTime = 0.f;
	else
		m_fMoveUVStaticStartTime = (float)_wtof(StartTime);

	if (EndTime == L"")
		m_fMoveUVStaticEndTime = 0.f;
	else
		m_fMoveUVStaticEndTime = (float)_wtof(EndTime);

	if (DirX == L"")
		m_fMoveUVStaticDirX = 0.f;
	else
		m_fMoveUVStaticDirX = (float)_wtof(DirX);

	if (DirY == L"")
		m_fMoveUVStaticDirY = 0.f;
	else
		m_fMoveUVStaticDirY = (float)_wtof(DirY);

	pEffect->AddUVMovement(m_fMoveUVStaticStartTime, m_fMoveUVStaticEndTime, m_fMoveUVStaticDirX, m_fMoveUVStaticDirY);
	return true;
}


void CEffectTab1::OnBnClickedButtonUvSpriteInput()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!m_pTargetObject)
		return;

	CEffect *pEffect = m_pTargetObject->FindComponentFromType<CEffect>(CT_EFFECT);
	if (!pEffect)
		return;

	if (m_checkUVSprite.GetCheck() == 0)
	{
		AfxMessageBox(L"Add UV Sprite first!");
		return;
	}

	AddUVSprite(pEffect);

	UpdateUV();

	SAFE_RELEASE(pEffect);
}


void CEffectTab1::OnBnClickedButtonUvSpritePlay()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!m_pTargetObject)
		return;

	CEffect *pEffect = m_pTargetObject->FindComponentFromType<CEffect>(CT_EFFECT);
	if (!pEffect)
		return;

	if (m_checkUVSprite.GetCheck() == 0)
	{
		AfxMessageBox(L"Add UV Sprite first!");
		return;
	}

	pEffect->SetOperationCheckPart(CEffectAssist::ASSIST_UV_ANI, true);

	UpdateUV();

	SAFE_RELEASE(pEffect);
}


void CEffectTab1::OnBnClickedButtonUvSpriteStop()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!m_pTargetObject)
		return;

	CEffect *pEffect = m_pTargetObject->FindComponentFromType<CEffect>(CT_EFFECT);
	if (!pEffect)
		return;

	if (m_checkUVSprite.GetCheck() == 0)
	{
		AfxMessageBox(L"Add UV Sprite first!");
		return;
	}

	pEffect->SetOperationCheckPart(CEffectAssist::ASSIST_UV_ANI, false);

	UpdateUV();

	SAFE_RELEASE(pEffect);
}


void CEffectTab1::OnBnClickedCheckUvSprite()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!m_pTargetObject)
		return;

	CEffect *pEffect = m_pTargetObject->FindComponentFromType<CEffect>(CT_EFFECT);
	if (!pEffect)
		return;

	int check = m_checkUVSprite.GetCheck();


	if (check == 1)
	{
		if(false == AddUVSprite(pEffect))
			m_checkUVSprite.SetCheck(0);
		else
			m_checkUVSprite.SetCheck(1);
	}

	else
	{
		pEffect->DeleteAssistEffectFromType(CEffectAssist::ASSIST_UV_ANI);
		m_checkUVSprite.SetCheck(0);
	}

	UpdateUV();

	SAFE_RELEASE(pEffect);
}


void CEffectTab1::OnBnClickedButtonUvMoveInput()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!m_pTargetObject)
		return;

	CEffect *pEffect = m_pTargetObject->FindComponentFromType<CEffect>(CT_EFFECT);
	if (!pEffect)
		return;

	if (m_checkUVMove.GetCheck() == 0)
	{
		AfxMessageBox(L"Add UV Movement first!");
		return;
	}

	AddUVMove(pEffect);

	UpdateUV();

	SAFE_RELEASE(pEffect);
}


void CEffectTab1::OnBnClickedButtonUvMovePlay()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!m_pTargetObject)
		return;

	CEffect *pEffect = m_pTargetObject->FindComponentFromType<CEffect>(CT_EFFECT);
	if (!pEffect)
		return;

	if (m_checkUVMove.GetCheck() == 0)
	{
		AfxMessageBox(L"Add UV Movement first!");
		return;
	}

	pEffect->SetOperationCheckPart(CEffectAssist::ASSIST_UV_MOVE, true);

	UpdateUV();

	SAFE_RELEASE(pEffect);
}


void CEffectTab1::OnBnClickedButtonUvMoveStop()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!m_pTargetObject)
		return;

	CEffect *pEffect = m_pTargetObject->FindComponentFromType<CEffect>(CT_EFFECT);
	if (!pEffect)
		return;

	if (m_checkUVMove.GetCheck() == 0)
	{
		AfxMessageBox(L"Add UV Movement first!");
		return;
	}

	pEffect->SetOperationCheckPart(CEffectAssist::ASSIST_UV_MOVE, false);

	UpdateUV();

	SAFE_RELEASE(pEffect);
}


void CEffectTab1::OnBnClickedCheckUv()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!m_pTargetObject)
		return;

	CEffect *pEffect = m_pTargetObject->FindComponentFromType<CEffect>(CT_EFFECT);
	if (!pEffect)
		return;

	int check = m_checkUVMove.GetCheck();


	if (check == 1)
	{
		if(false == AddUVMove(pEffect))
			m_checkUVMove.SetCheck(0);
		else
			m_checkUVMove.SetCheck(1);
	}

	else
	{
		pEffect->DeleteAssistEffectFromType(CEffectAssist::ASSIST_UV_MOVE);
		m_checkUVMove.SetCheck(0);
	}

	UpdateUV();

	SAFE_RELEASE(pEffect);
}


BOOL CEffectTab1::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE ||
			pMsg->wParam == VK_RETURN)
			return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CEffectTab1::OnBnClickedCheckInfiniteFadeIn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!m_pTargetObject)
		return;

	CEffect *pEffect = m_pTargetObject->FindComponentFromType<CEffect>(CT_EFFECT);
	if (!pEffect)
		return;

	int check = m_checkFadeIn.GetCheck();

	/* 설정되어 있지 않았다면 */
	if (0 == check)
	{
		m_checkInfiniteFadeIn.SetCheck(0);
		return;
	}

	check = m_checkInfiniteFadeIn.GetCheck();

	/* Check를 하는 동작 */
	if (check == 1)
	{
		pEffect->SetInfiniteCheckAssistEffectFromType(CEffectAssist::ASSIST_FADE_IN, true);
		m_checkInfiniteFadeIn.SetCheck(1);
	}
	/* Check를 푸는 동작 */
	else
	{
		pEffect->SetInfiniteCheckAssistEffectFromType(CEffectAssist::ASSIST_FADE_IN, false);
		m_checkInfiniteFadeIn.SetCheck(0);
	}

	UpdateFade();

	SAFE_RELEASE(pEffect);
}


void CEffectTab1::OnBnClickedCheckInfiniteFadeOut()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!m_pTargetObject)
		return;

	CEffect *pEffect = m_pTargetObject->FindComponentFromType<CEffect>(CT_EFFECT);
	if (!pEffect)
		return;

	int check = m_checkFadeOut.GetCheck();

	/* 설정되어 있지 않았다면 */
	if (0 == check)
	{
		m_checkInfiniteFadeOut.SetCheck(0);
		return;
	}

	check = m_checkInfiniteFadeOut.GetCheck();

	/* Check를 하는 동작 */
	if (check == 1)
	{
		pEffect->SetInfiniteCheckAssistEffectFromType(CEffectAssist::ASSIST_FADE_OUT, true);
		m_checkInfiniteFadeOut.SetCheck(1);
	}
	/* Check를 푸는 동작 */
	else
	{
		pEffect->SetInfiniteCheckAssistEffectFromType(CEffectAssist::ASSIST_FADE_OUT, false);
		m_checkInfiniteFadeOut.SetCheck(0);
	}

	UpdateFade();

	SAFE_RELEASE(pEffect);
}


void CEffectTab1::OnBnClickedCheckInfiniteUvMove()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!m_pTargetObject)
		return;

	CEffect *pEffect = m_pTargetObject->FindComponentFromType<CEffect>(CT_EFFECT);
	if (!pEffect)
		return;

	int check = m_checkUVMove.GetCheck();

	/* 설정되어 있지 않았다면 */
	if (0 == check)
	{
		m_checkInfiniteUVMove.SetCheck(0);
		return;
	}

	check = m_checkInfiniteUVMove.GetCheck();

	/* Check를 하는 동작 */
	if (check == 1)
	{
		pEffect->SetInfiniteCheckAssistEffectFromType(CEffectAssist::ASSIST_UV_MOVE, true);
		m_checkInfiniteUVMove.SetCheck(1);
	}
	/* Check를 푸는 동작 */
	else
	{
		pEffect->SetInfiniteCheckAssistEffectFromType(CEffectAssist::ASSIST_UV_MOVE, false);
		m_checkInfiniteUVMove.SetCheck(0);
	}

	UpdateUV();

	SAFE_RELEASE(pEffect);
}


void CEffectTab1::OnBnClickedRadioSpriteType1()
{
	CEffect *pEffect = m_pTargetObject->FindComponentFromType<CEffect>(CT_EFFECT);

	if (pEffect)
	{
		/* UV Sprite */
		CEffectAssist *pAssistUVAni = pEffect->GetAssistFromType(CEffectAssist::ASSIST_UV_ANI);

		if (pAssistUVAni && CEffectAssist::SPRITE_ATLAS == pAssistUVAni->GetSpriteType())
		{
			return;
		}
	}

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_iRadioSpriteType = 0;
}


void CEffectTab1::OnBnClickedRadioSpriteType2()
{
	CEffect *pEffect = m_pTargetObject->FindComponentFromType<CEffect>(CT_EFFECT);

	if (pEffect)
	{
		/* UV Sprite */
		CEffectAssist *pAssistUVAni = pEffect->GetAssistFromType(CEffectAssist::ASSIST_UV_ANI);

		if (pAssistUVAni && CEffectAssist::SPRITE_FRAME == pAssistUVAni->GetSpriteType())
		{
			return;
		}
	}

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_iRadioSpriteType = 1;
}


void CEffectTab1::OnBnClickedCheckInfiniteUvSprite()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!m_pTargetObject)
		return;

	CEffect *pEffect = m_pTargetObject->FindComponentFromType<CEffect>(CT_EFFECT);
	if (!pEffect)
		return;

	int check = m_checkUVSprite.GetCheck();

	/* 설정되어 있지 않았다면 */
	if (0 == check)
	{
		m_checkInfiniteUVSprite.SetCheck(0);
		return;
	}

	check = m_checkInfiniteUVSprite.GetCheck();

	/* Check를 하는 동작 */
	if (check == 1)
	{
		pEffect->SetInfiniteCheckAssistEffectFromType(CEffectAssist::ASSIST_UV_ANI, true);
		m_checkInfiniteUVSprite.SetCheck(1);
	}
	/* Check를 푸는 동작 */
	else
	{
		pEffect->SetInfiniteCheckAssistEffectFromType(CEffectAssist::ASSIST_UV_ANI, false);
		m_checkInfiniteUVSprite.SetCheck(0);
	}

	UpdateUV();

	SAFE_RELEASE(pEffect);
}
