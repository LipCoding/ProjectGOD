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

	InitFadeIn();
	InitFadeOut();
	InitUVSprite();
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
	m_fUVSpriteStaticStartTime = -1.f;
	m_fUVSpriteStaticEndTime = -1.f;
	m_iUVSpriteStaticNum = -1;

	m_fUVSpriteTime = -1.f;

	m_editUVSpriteStartTime.SetWindowTextW(L"");
	m_editUVSpriteEndTime.SetWindowTextW(L"");
	m_editUVSpriteNum.SetWindowTextW(L"");
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

			m_checkFadeIn.SetCheck(1);
		}
		else
		{
			InitFadeIn();
			m_checkFadeIn.SetCheck(0);
		}

		/* Fade Out */
		CEffectAssist *pAssistFadeOut = pEffect->GetAssistFromType(CEffectAssist::ASSIST_FADE_OUT);

		if (pAssistFadeOut)
		{
			m_fFadeOutStaticStartTime = pAssistFadeOut->GetStartTime();
			m_fFadeOutStaticEndTime = pAssistFadeOut->GetEndTime();
			m_fFadeOutStaticDegree = pAssistFadeOut->GetDegree();

			m_checkFadeOut.SetCheck(1);
		}
		else
		{
			InitFadeOut();
			m_checkFadeOut.SetCheck(0);
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
			m_fUVSpriteStaticStartTime = pAssistUVAni->GetStartTime();
			m_fUVSpriteStaticEndTime = pAssistUVAni->GetEndTime();
			m_iUVSpriteStaticNum = pAssistUVAni->GetNum();

			m_checkUVSprite.SetCheck(1);
		}
		else
		{
			InitUVSprite();
			m_checkUVSprite.SetCheck(0);
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

void CEffectTab1::AddUVSprite(CEffect * pEffect)
{
	CString StartTime, EndTime, Num;

	m_editUVSpriteStartTime.GetWindowTextW(StartTime);
	m_editUVSpriteEndTime.GetWindowTextW(EndTime);
	m_editUVSpriteNum.GetWindowTextW(Num);

	if (StartTime == L"")
		m_fUVSpriteStaticStartTime = 0.f;
	else
		m_fUVSpriteStaticStartTime = (float)_wtof(StartTime);

	if (EndTime == L"")
		m_fUVSpriteStaticEndTime = 0.f;
	else
		m_fUVSpriteStaticEndTime = (float)_wtof(EndTime);

	if (Num == L"")
		m_iUVSpriteStaticNum = 0;
	else
		m_iUVSpriteStaticNum = (float)_wtoi(Num);

	pEffect->AddUVAnimation(m_fUVSpriteStaticStartTime, m_fUVSpriteStaticEndTime, m_iUVSpriteStaticNum, 1);
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
		AddUVSprite(pEffect);
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
