// EffectTab.cpp: 구현 파일
//

#include "stdafx.h"
#include "EffectTool.h"
#include "EffectTab.h"
#include "afxdialogex.h"

#include "MainFrm.h"
#include "EaseSheetDlg.h"

#include "Core/PathManager.h"
#include "Resources/Mesh.h"
#include "Component/Animation.h"
#include "Component/AnimationClip.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "Scene/Layer.h"
#include "Component/Transform.h"
#include "Component/Renderer.h"
#include "Component/Camera.h"
#include "Component/ColliderAABB.h"
#include "Component/ColliderSphere.h"


#include "Component/EffectAssist.h"
// CEffectTab 대화 상자

IMPLEMENT_DYNAMIC(CEffectTab, CDialogEx)

CEffectTab::CEffectTab(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
	, m_fInfoStaticPosX(0)
	, m_fInfoStaticPosY(0)
	, m_fInfoStaticPosZ(0)
	, m_fInfoStaticScaleX(0)
	, m_fInfoStaticScaleY(0)
	, m_fInfoStaticScaleZ(0)
	, m_iInfoStaticRotX(0)
	, m_iInfoStaticRotY(0)
	, m_iInfoStaticRotZ(0)
	, m_fPatternStaticScaleX(0)
	, m_fPatternStaticScaleY(0)
	, m_fPatternStaticScaleZ(0)
	, m_fPatternStaticScaleStartTime(0)
	, m_fPatternStaticScaleEndTime(0)
	, m_iPatternStaticScaleRepeat(0)
	, m_fPatternStaticScaleTime(0)
	, m_fPatternStaticRotX(0)
	, m_fPatternStaticRotY(0)
	, m_fPatternStaticRotZ(0)
	, m_fPatternStaticRotStartTime(0)
	, m_fPatternStaticRotEndTime(0)
	, m_iPatternStaticRepeat(0)
	, m_fPatternStaticRotTime(0)
{

}

CEffectTab::~CEffectTab()
{
	SAFE_DELETE(m_pEaseSheetDlg);
}

void CEffectTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_BONE_LIST, m_comboBoxBoneInfo);
	DDX_Control(pDX, IDC_COMBO_ANIM_LIST, m_comboBoxAnimInfo);
	DDX_Text(pDX, IDC_EDIT_POS_INFO_STATIC_X, m_fInfoStaticPosX);
	DDX_Text(pDX, IDC_EDIT_POS_INFO_STATIC_Y, m_fInfoStaticPosY);
	DDX_Text(pDX, IDC_EDIT_POS_INFO_STATIC_Z, m_fInfoStaticPosZ);
	DDX_Text(pDX, IDC_EDIT_SCALE_INFO_STATIC_X, m_fInfoStaticScaleX);
	DDX_Text(pDX, IDC_EDIT_SCALE_INFO_STATIC_Y, m_fInfoStaticScaleY);
	DDX_Text(pDX, IDC_EDIT_SCALE_INFO_STATIC_Z, m_fInfoStaticScaleZ);
	DDX_Text(pDX, IDC_EDIT_ROT_INFO_STATIC_X, m_iInfoStaticRotX);
	DDX_Text(pDX, IDC_EDIT_ROT_INFO_STATIC_Y, m_iInfoStaticRotY);
	DDX_Text(pDX, IDC_EDIT_ROT_INFO_STATIC_Z, m_iInfoStaticRotZ);
	DDX_Control(pDX, IDC_EDIT_POS_INFO_X, m_editInfoPosX);
	DDX_Control(pDX, IDC_EDIT_POS_INFO_Y, m_editInfoPosY);
	DDX_Control(pDX, IDC_EDIT_POS_INFO_Z, m_editInfoPosZ);
	DDX_Control(pDX, IDC_EDIT_SCALE_INFO_X, m_editInfoScaleX);
	DDX_Control(pDX, IDC_EDIT_SCALE_INFO_Y, m_editInfoScaleY);
	DDX_Control(pDX, IDC_EDIT_SCALE_INFO_Z, m_editInfoScaleZ);
	DDX_Control(pDX, IDC_EDIT_ROT_INFO_X, m_editInfoRotX);
	DDX_Control(pDX, IDC_EDIT_ROT_INFO_Y, m_editInfoRotY);
	DDX_Control(pDX, IDC_EDIT_ROT_INFO_Z, m_editInfoRotZ);
	DDX_Control(pDX, IDC_COMBO_VIEWSHEET, m_comboEaseSheet_Scale);
	DDX_Control(pDX, IDC_COMBO_VIEWSHEET1, m_comboEaseSheet_Rot);
	DDX_Text(pDX, IDC_EDIT_PATTERN_STATIC_SCALE_X, m_fPatternStaticScaleX);
	DDX_Text(pDX, IDC_EDIT_PATTERN_STATIC_SCALE_Y_, m_fPatternStaticScaleY);
	DDX_Text(pDX, IDC_EDIT_PATTERN_STATIC_SCALE_Z, m_fPatternStaticScaleZ);
	DDX_Text(pDX, IDC_EDIT_PATTERN_STATIC_SCALE_START_TIME, m_fPatternStaticScaleStartTime);
	DDX_Text(pDX, IDC_EDIT_PATTERN_STATIC_SCALE_END_TIME, m_fPatternStaticScaleEndTime);
	DDX_Text(pDX, IDC_EDIT_PATTERN_STATIC_SCALE_REPEAT, m_iPatternStaticScaleRepeat);
	DDX_Control(pDX, IDC_EDIT_PATTERN_SCALE_START_TIME, m_editPatternScaleStartTime);
	DDX_Control(pDX, IDC_EDIT_PATTERN_SCALE_END_TIME, m_editPatternScaleEndTime);
	DDX_Control(pDX, IDC_EDIT_PATTERN_SCALE_REPEAT, m_editPatternScaleRepeat);
	DDX_Control(pDX, IDC_EDIT_PATTERN_SCALE_X, m_editPatternScaleX);
	DDX_Control(pDX, IDC_EDIT_PATTERN_SCALE_Y, m_editPatternScaleY);
	DDX_Control(pDX, IDC_EDIT_PATTERN_SCALE_Z, m_editPatternScaleZ);
	DDX_Text(pDX, IDC_EDIT_PATTERN_STATIC_SCALE_TIME, m_fPatternStaticScaleTime);
	DDX_Control(pDX, IDC_CHECK_SCAILING, m_checkSclaling);
	DDX_Text(pDX, IDC_EDIT_PATTERN_STATIC_ROT_X, m_fPatternStaticRotX);
	DDX_Text(pDX, IDC_EDIT_PATTERN_STATIC_ROT_Y, m_fPatternStaticRotY);
	DDX_Text(pDX, IDC_EDIT_PATTERN_STATIC_ROT_Z, m_fPatternStaticRotZ);
	DDX_Text(pDX, IDC_EDIT_PATTERN_STATIC_ROT_START_TIME, m_fPatternStaticRotStartTime);
	DDX_Text(pDX, IDC_EDIT_PATTERN_STATIC_ROT_END_TIME, m_fPatternStaticRotEndTime);
	DDX_Text(pDX, IDC_EDIT_PATTERN_STATIC_ROT_REPEAT, m_iPatternStaticRepeat);
	DDX_Text(pDX, IDC_EDIT_PATTERN_STATIC_ROT_TIME, m_fPatternStaticRotTime);
	DDX_Control(pDX, IDC_EDIT_PATTERN_ROT_X, m_editPatternRotX);
	DDX_Control(pDX, IDC_EDIT_PATTERN_ROT_Y, m_editPatternRotY);
	DDX_Control(pDX, IDC_EDIT_PATTERN_ROT_Z, m_editPatternRotZ);
	DDX_Control(pDX, IDC_EDIT_PATTERN_ROT_START_TIME, m_editPatternRotStartTime);
	DDX_Control(pDX, IDC_EDIT_PATTERN_ROT_END_TIME, m_editPatternRotEndTime);
	DDX_Control(pDX, IDC_EDIT_PATTERN_ROT_REPEAT, m_editPatternRotRepeat);
	DDX_Control(pDX, IDC_CHECK_ROTATING, m_checkRotating);
}

BEGIN_MESSAGE_MAP(CEffectTab, CDialogEx)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BUTTON_LOAD_TARGET_OBJ, &CEffectTab::OnBnClickedButtonLoadTargetObj)
	ON_CBN_SELCHANGE(IDC_COMBO_BONE_LIST, &CEffectTab::OnCbnSelchangeComboBoneList)
	ON_CBN_SELCHANGE(IDC_COMBO_ANIM_LIST, &CEffectTab::OnCbnSelchangeComboAnimList)
	ON_BN_CLICKED(IDC_BUTTON_INPUT_INFO, &CEffectTab::OnBnClickedButtonInputInfo)
	ON_BN_CLICKED(IDC_BUTTON_INFO_POSX_UP, &CEffectTab::OnBnClickedButtonInfoPosxUp)
	ON_BN_CLICKED(IDC_BUTTON_INFO_POSX_DOWN, &CEffectTab::OnBnClickedButtonInfoPosxDown)
	ON_BN_CLICKED(IDC_BUTTON_INFO_POSY_UP, &CEffectTab::OnBnClickedButtonInfoPosyUp)
	ON_BN_CLICKED(IDC_BUTTON_INFO_POSY_DOWN, &CEffectTab::OnBnClickedButtonInfoPosyDown)
	ON_BN_CLICKED(IDC_BUTTON_INFO_POSZ_UP, &CEffectTab::OnBnClickedButtonInfoPoszUp)
	ON_BN_CLICKED(IDC_BUTTON_INFO_POSZ_DOWN, &CEffectTab::OnBnClickedButtonInfoPoszDown)
	ON_BN_CLICKED(IDC_BUTTON_INFO_SCALEX_UP, &CEffectTab::OnBnClickedButtonInfoScalexUp)
	ON_BN_CLICKED(IDC_BUTTON_INFO_SCALEX_DOWN, &CEffectTab::OnBnClickedButtonInfoScalexDown)
	ON_BN_CLICKED(IDC_BUTTON_INFO_SCALEY_UP, &CEffectTab::OnBnClickedButtonInfoScaleyUp)
	ON_BN_CLICKED(IDC_BUTTON_INFO_SCALEY_DOWN, &CEffectTab::OnBnClickedButtonInfoScaleyDown)
	ON_BN_CLICKED(IDC_BUTTON_INFO_SCALEZ_UP, &CEffectTab::OnBnClickedButtonInfoScalezUp)
	ON_BN_CLICKED(IDC_BUTTON_INFO_SCALEZ_DOWN, &CEffectTab::OnBnClickedButtonInfoScalezDown)
	ON_BN_CLICKED(IDC_BUTTON_INFO_ROTX_UP, &CEffectTab::OnBnClickedButtonInfoRotxUp)
	ON_BN_CLICKED(IDC_BUTTON_INFO_ROTX_DOWN, &CEffectTab::OnBnClickedButtonInfoRotxDown)
	ON_BN_CLICKED(IDC_BUTTON_INFO_ROTY_UP, &CEffectTab::OnBnClickedButtonInfoRotyUp)
	ON_BN_CLICKED(IDC_BUTTON_INFO_ROTY_DOWN, &CEffectTab::OnBnClickedButtonInfoRotyDown)
	ON_BN_CLICKED(IDC_BUTTON_INFO_ROTZ_UP, &CEffectTab::OnBnClickedButtonInfoRotzUp)
	ON_BN_CLICKED(IDC_BUTTON_INFO_ROTZ_DOWN, &CEffectTab::OnBnClickedButtonInfoRotzDown)
	ON_BN_CLICKED(IDC_BUTTON_VIEWSHEET, &CEffectTab::OnBnClickedButtonViewsheet)
	ON_BN_CLICKED(IDC_BUTTON_VIEWSHEET2, &CEffectTab::OnBnClickedButtonViewsheet2)
	ON_BN_CLICKED(IDC_CHECK_SCAILING, &CEffectTab::OnBnClickedCheckScailing)
	ON_BN_CLICKED(IDC_BUTTON_PATTERN_SCALE_INPUT, &CEffectTab::OnBnClickedButtonPatternScaleInput)
	ON_BN_CLICKED(IDC_BUTTON_PATTERN_SCALE_PLAY, &CEffectTab::OnBnClickedButtonPatternScalePlay)
	ON_BN_CLICKED(IDC_BUTTON_PATTERN_SCALE_STOP, &CEffectTab::OnBnClickedButtonPatternScaleStop)
	ON_BN_CLICKED(IDC_BUTTON_PATTERN_ROT_INPUT, &CEffectTab::OnBnClickedButtonPatternRotInput)
	ON_BN_CLICKED(IDC_BUTTON_PATTERN_ROT_PLAY, &CEffectTab::OnBnClickedButtonPatternRotPlay)
	ON_BN_CLICKED(IDC_BUTTON_PATTERN_ROT_STOP, &CEffectTab::OnBnClickedButtonPatternRotStop)
	ON_BN_CLICKED(IDC_CHECK_ROTATING, &CEffectTab::OnBnClickedCheckRotating)
END_MESSAGE_MAP()

// CEffectTab 메시지 처리기

BOOL CEffectTab::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	/* Static Value */
	InitFormValue();

	/* Ease Sheet */
	InitComboBox();

	/* Check */
	m_checkSclaling.SetCheck(0);
	m_checkRotating.SetCheck(0);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CEffectTab::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CEffectTab::InitForm()
{
	m_comboBoxBoneInfo.ResetContent();
	m_comboBoxAnimInfo.ResetContent();
}

void CEffectTab::InitFormValue()
{
	/* Sheet */
	m_comboEaseSheet_Scale.SetCurSel(0);
	m_comboEaseSheet_Rot.SetCurSel(0);

	/* Check */
	m_checkSclaling.SetCheck(0);
	m_checkRotating.SetCheck(0);

	InitFormInfo();
	InitFormPatternScale();
	InitFormPatternRot();
}

void CEffectTab::InitFormInfo()
{
	/* Info */
	m_fInfoStaticPosX = -1.f;
	m_fInfoStaticPosY = -1.f;
	m_fInfoStaticPosZ = -1.f;

	m_fInfoStaticScaleX = -1.f;
	m_fInfoStaticScaleY = -1.f;
	m_fInfoStaticScaleZ = -1.f;

	m_iInfoStaticRotX = -1;
	m_iInfoStaticRotY = -1;
	m_iInfoStaticRotZ = -1;

	m_editInfoPosX.SetWindowTextW(L"");
	m_editInfoPosY.SetWindowTextW(L"");
	m_editInfoPosZ.SetWindowTextW(L"");

	m_editInfoScaleX.SetWindowTextW(L"");
	m_editInfoScaleY.SetWindowTextW(L"");
	m_editInfoScaleZ.SetWindowTextW(L"");

	m_editInfoRotX.SetWindowTextW(L"");
	m_editInfoRotY.SetWindowTextW(L"");
	m_editInfoRotZ.SetWindowTextW(L"");
}

void CEffectTab::InitFormPatternScale()
{
	/* Pattern */
	m_fPatternStaticScaleX = -1.f;
	m_fPatternStaticScaleY = -1.f;
	m_fPatternStaticScaleZ = -1.f;

	m_fPatternStaticScaleStartTime = -1.f;
	m_fPatternStaticScaleEndTime = -1.f;
	m_iPatternStaticScaleRepeat = -1;

	m_fPatternStaticScaleTime = -1.f;

	m_editPatternScaleX.SetWindowTextW(L"");
	m_editPatternScaleY.SetWindowTextW(L"");
	m_editPatternScaleZ.SetWindowTextW(L"");

	m_editPatternScaleStartTime.SetWindowTextW(L"");
	m_editPatternScaleEndTime.SetWindowTextW(L"");
	m_editPatternScaleRepeat.SetWindowTextW(L"");
}

void CEffectTab::InitFormPatternRot()
{
	/* Pattern */
	m_fPatternStaticRotX = -1.f;
	m_fPatternStaticRotY = -1.f;
	m_fPatternStaticRotZ = -1.f;

	m_fPatternStaticRotStartTime = -1.f;
	m_fPatternStaticRotEndTime = -1.f;
	m_iPatternStaticRepeat = -1;

	m_fPatternStaticRotTime = -1.f;

	m_editPatternRotX.SetWindowTextW(L"");
	m_editPatternRotY.SetWindowTextW(L"");
	m_editPatternRotZ.SetWindowTextW(L"");

	m_editPatternRotStartTime.SetWindowTextW(L"");
	m_editPatternRotEndTime.SetWindowTextW(L"");
	m_editPatternRotRepeat.SetWindowTextW(L"");
}

void CEffectTab::InitComboBox()
{
	m_comboEaseSheet_Scale.AddString(L"None");
	m_comboEaseSheet_Scale.AddString(L"EaseInSine");
	m_comboEaseSheet_Scale.AddString(L"EaseOutSine");
	m_comboEaseSheet_Scale.AddString(L"EaseInQuad");
	m_comboEaseSheet_Scale.AddString(L"EaseOutQuad");
	m_comboEaseSheet_Scale.AddString(L"EaseInCubic");
	m_comboEaseSheet_Scale.AddString(L"EaseOutCubic");
	m_comboEaseSheet_Scale.AddString(L"EaseInQuart");
	m_comboEaseSheet_Scale.AddString(L"EaseOutQuart");
	m_comboEaseSheet_Scale.AddString(L"EaseInQuint");
	m_comboEaseSheet_Scale.AddString(L"EaseOutQuint");
	m_comboEaseSheet_Scale.AddString(L"EaseInExpo");
	m_comboEaseSheet_Scale.AddString(L"EaseOutExpo");
	m_comboEaseSheet_Scale.AddString(L"EaseInCirc");
	m_comboEaseSheet_Scale.AddString(L"EaseOutCirc");
	m_comboEaseSheet_Scale.AddString(L"EaseInBack");
	m_comboEaseSheet_Scale.AddString(L"EaseOutBack");
	m_comboEaseSheet_Scale.AddString(L"EaseInElastic");
	m_comboEaseSheet_Scale.AddString(L"EaseOutElastic");
	m_comboEaseSheet_Scale.AddString(L"EaseInBounce");
	m_comboEaseSheet_Scale.AddString(L"EaseOutBounce");

	m_comboEaseSheet_Rot.AddString(L"None");
	m_comboEaseSheet_Rot.AddString(L"EaseInSine");
	m_comboEaseSheet_Rot.AddString(L"EaseOutSine");
	m_comboEaseSheet_Rot.AddString(L"EaseInQuad");
	m_comboEaseSheet_Rot.AddString(L"EaseOutQuad");
	m_comboEaseSheet_Rot.AddString(L"EaseInCubic");
	m_comboEaseSheet_Rot.AddString(L"EaseOutCubic");
	m_comboEaseSheet_Rot.AddString(L"EaseInQuart");
	m_comboEaseSheet_Rot.AddString(L"EaseOutQuart");
	m_comboEaseSheet_Rot.AddString(L"EaseInQuint");
	m_comboEaseSheet_Rot.AddString(L"EaseOutQuint");
	m_comboEaseSheet_Rot.AddString(L"EaseInExpo");
	m_comboEaseSheet_Rot.AddString(L"EaseOutExpo");
	m_comboEaseSheet_Rot.AddString(L"EaseInCirc");
	m_comboEaseSheet_Rot.AddString(L"EaseOutCirc");
	m_comboEaseSheet_Rot.AddString(L"EaseInBack");
	m_comboEaseSheet_Rot.AddString(L"EaseOutBack");
	m_comboEaseSheet_Rot.AddString(L"EaseInElastic");
	m_comboEaseSheet_Rot.AddString(L"EaseOutElastic");
	m_comboEaseSheet_Rot.AddString(L"EaseInBounce");
	m_comboEaseSheet_Rot.AddString(L"EaseOutBounce");

	m_comboEaseSheet_Scale.SetCurSel(0);
	m_comboEaseSheet_Rot.SetCurSel(0);
}

void CEffectTab::UpdateInfo()
{
	CTransform *pTr = m_pTargetObject->GetTransform();

	Vector3 pScale = pTr->GetWorldScale();
	Vector3 pPos = pTr->GetWorldPos();
	Vector3 pRot = pTr->GetWorldRot();

	pPos.x -= 50.f / 2.f;
	pPos.z -= 50.f / 2.f;

	m_fInfoStaticPosX = pPos.x;
	m_fInfoStaticPosY = pPos.y;
	m_fInfoStaticPosZ = pPos.z;

	m_fInfoStaticScaleX = pScale.x;
	m_fInfoStaticScaleY = pScale.y;
	m_fInfoStaticScaleZ = pScale.z;

	m_iInfoStaticRotX = (int)round(XMConvertToDegrees(pRot.x));
	m_iInfoStaticRotY = (int)round(XMConvertToDegrees(pRot.y));
	m_iInfoStaticRotZ = (int)round(XMConvertToDegrees(pRot.z));

	SAFE_RELEASE(pTr);
}

void CEffectTab::UpdateMain()
{
}

void CEffectTab::UpdatePattern()
{
	CEffect *pEffect = m_pTargetObject->FindComponentFromType<CEffect>(CT_EFFECT);
	
	if (pEffect)
	{
		/* Scale */
		CEffectAssist *pAssistScale = pEffect->GetAssistFromType(CEffectAssist::ASSIST_SCALE);

		if (pAssistScale)
		{
			m_fPatternStaticScaleX = pAssistScale->GetPowerX();
			m_fPatternStaticScaleY = pAssistScale->GetPowerY();
			m_fPatternStaticScaleZ = pAssistScale->GetPowerZ();

			m_fPatternStaticScaleStartTime = pAssistScale->GetStartTime();
			m_fPatternStaticScaleEndTime = pAssistScale->GetEndTime();
			m_iPatternStaticScaleRepeat = pAssistScale->GetRepeat();

			m_comboEaseSheet_Scale.SetCurSel(pAssistScale->GetEaseType());
			m_checkSclaling.SetCheck(1);
		}
		else
		{
			InitFormPatternScale();
			m_comboEaseSheet_Scale.SetCurSel(0);
			m_checkSclaling.SetCheck(0);
		}

		/* Rotate */
		CEffectAssist *pAssistRot = pEffect->GetAssistFromType(CEffectAssist::ASSIST_ROT);

		if (pAssistRot)
		{
			m_fPatternStaticRotX = pAssistRot->GetPowerX();
			m_fPatternStaticRotY = pAssistRot->GetPowerY();
			m_fPatternStaticRotZ = pAssistRot->GetPowerZ();

			m_fPatternStaticRotStartTime = pAssistRot->GetStartTime();
			m_fPatternStaticRotEndTime = pAssistRot->GetEndTime();
			m_iPatternStaticRepeat = pAssistRot->GetRepeat();

			m_comboEaseSheet_Rot.SetCurSel(pAssistRot->GetEaseType());
			m_checkRotating.SetCheck(1);
		}
		else
		{
			InitFormPatternRot();
			m_comboEaseSheet_Rot.SetCurSel(0);
			m_checkRotating.SetCheck(0);
		}

		SAFE_RELEASE(pEffect);
	}
}

void CEffectTab::UpdateTime()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!m_pTargetObject)
		return;

	CEffect *pEffect = m_pTargetObject->FindComponentFromType<CEffect>(CT_EFFECT);
	if (!pEffect)
		return;

	CEffectAssist *pAssist = nullptr;


	/* Check 되어 있다면 */
	
	/* Pattern */
	if (m_checkSclaling.GetCheck() == 1)
	{
		pAssist = pEffect->GetAssistFromType(CEffectAssist::ASSIST_SCALE);

		if(pAssist)
			m_fPatternStaticScaleTime = pAssist->GetTime();
	}

	if (m_checkRotating.GetCheck() == 1)
	{
		pAssist = pEffect->GetAssistFromType(CEffectAssist::ASSIST_ROT);

		if (pAssist)
			m_fPatternStaticRotTime = pAssist->GetTime();
	}
}

bool CEffectTab::LoadTargetMesh(const CString & filePath, const CString& fileName)
{
	CString strTag = filePath + ".msh";

	/* 이미 가지고 있다면 지운다. */
	if (m_pObjectMesh)
	{
		CAnimation *pAnimation = m_pObjectMesh->FindComponentFromType<CAnimation>(CT_ANIMATION);
		if (pAnimation)
		{
			SAFE_RELEASE(pAnimation);
		}

		DeleteTargetMesh();
		InitForm();
	}

	CScene *pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
	CLayer *pLayer = pScene->GetLayer("Default");

	m_pObjectMesh = CGameObject::CreateObject("TargetObj", pLayer);
	CTransform *pTr = m_pObjectMesh->GetTransform();
	pTr->SetWorldPos(50.f / 2.f, 0.f, 50.f / 2.f);
	SAFE_RELEASE(pTr);

	CRenderer *pRenderer = m_pObjectMesh->AddComponent<CRenderer>("Renderer");

	pRenderer->SetMeshFromFullPath((string)CT2CA(fileName), strTag.GetString());

	CMesh *pMesh = pRenderer->GetMesh();

	if (pMesh == nullptr)
	{
		SAFE_RELEASE(pRenderer);
		SAFE_RELEASE(pScene);
		SAFE_RELEASE(pLayer);
		return false;
	}

	SAFE_RELEASE(pMesh);
	SAFE_RELEASE(pRenderer);
	SAFE_RELEASE(pLayer);
	SAFE_RELEASE(pScene);

	return true;
}

bool CEffectTab::LoadTargetAnimation(const CString & filePath)
{
	CString strTag = filePath + ".anm";

	/* 이미 가지고 있다면 지운다. */
	if (m_pObjectMesh)
	{
		CAnimation *pAnimationCheck = m_pObjectMesh->FindComponentFromType<CAnimation>(CT_ANIMATION);
		
		if (pAnimationCheck)
		{
			AfxMessageBox(L"Error : Your object already have Animation!");
			SAFE_RELEASE(pAnimationCheck);
			return false;
		}
	}

	CAnimation*	pAnimation = m_pObjectMesh->AddComponent<CAnimation>("Animation");

	if (!pAnimation->LoadFromFullPath(strTag))
		return false;

	const unordered_map<string, class CAnimationClip*>*	pClips =
		pAnimation->GetAllClip();


	unordered_map<string, class CAnimationClip*>::const_iterator	iter;
	unordered_map<string, class CAnimationClip*>::const_iterator	iterEnd = pClips->end();

	for (iter = pClips->begin(); iter != iterEnd; ++iter)
	{
		ANIMATION3DCLIP tInfo = iter->second->GetClipInfo();
		wstring	strName = CA2CT(tInfo.strName.c_str());

		/* Effect Tool 작업을 위해 모든 Clip을 반복으로 바꾼다. */
		iter->second->SetClipInfo(tInfo.strName, AO_LOOP, 24, 
			tInfo.iStartFrame, tInfo.iEndFrame, tInfo.fStartTime, tInfo.fEndTime);

		/* Add Info to Form */
		m_comboBoxAnimInfo.AddString(strName.c_str());
	}
	
	m_comboBoxAnimInfo.SetCurSel(0);

	/* Bone */
	const vector<PBONE> boneInfo = pAnimation->GetBoneVector();

	for (const auto iter : boneInfo)
	{
		CString boneName = CA2CT(iter->strName.c_str());
		m_comboBoxBoneInfo.AddString(boneName);
	}

	UpdateData(FALSE);

	SAFE_RELEASE(pAnimation);
	return true;
}

bool CEffectTab::LoadTargetLocalInfo(const CString & filePath)
{
	CString strTag = filePath + ".dat";

	if (m_pObjectMesh)
	{
		FILE* pFile = nullptr;

		char	strPath[MAX_PATH] = {};
		WideCharToMultiByte(CP_ACP, 0, strTag, -1,
			strPath, lstrlen(strTag), 0, 0);

		fopen_s(&pFile, strPath, "rb");

		if (!pFile)
			return false;

		CTransform* pTr = m_pObjectMesh->GetTransform();
		pTr->Load_Local(pFile);
		SAFE_RELEASE(pTr);

		//SetMeshInfo();
		fclose(pFile);
	}
	else
	{
		return false;
	}

	return true;
}

void CEffectTab::DeleteTargetMesh()
{
	if (m_pObjectMesh)
		m_pObjectMesh->Die();

	CGameObject::EraseObj(m_pObjectMesh);
	SAFE_RELEASE(m_pObjectMesh);
	
	m_pTargetObject = nullptr;
	m_pBoneMatrix = nullptr;
	m_boneNameAttachTo = "";
}

void CEffectTab::OnBnClickedButtonLoadTargetObj()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	wchar_t	strFilter[] = L"MESHFile(*.msh)|*.msh|모든파일(*.*)|*.*|||";
	CFileDialog	dlg(TRUE, L".MSH", L"Mesh",
		OFN_HIDEREADONLY, strFilter);

	// 경로 지정
	wchar_t strPath[MAX_PATH] = {};
	wcscpy_s(strPath, MAX_PATH, GET_SINGLE(CPathManager)->FindPath(MESH_PATH));

	CString originPath = strPath;

	dlg.m_ofn.lpstrInitialDir = strPath;

	// do modal error 해결
	if (dlg.DoModal() != IDOK)
		return;

	CString path = dlg.GetPathName();
	CString name = dlg.GetFileTitle();

	// 파일 이름 제거
	for (int i = lstrlen(path) - 1; i >= 0; --i)
	{
		if (path[i] == '\\')
		{
			path.Delete(i + 1, lstrlen(path) - 1);
			break;
		}
	}

	CString fullPath = path + name;

	/*string ConvertPath = CT2CA(path.GetString());
	string ConvertName = CT2CA(name.GetString());
	string ConvertFullPath = ConvertPath + ConvertName;*/

	if (!LoadTargetMesh(fullPath, name))
	{
		AfxMessageBox(L"Target Mesh Create Fail!");
		DeleteTargetMesh();
		return;
	}

	if (!LoadTargetLocalInfo(fullPath))
	{
		AfxMessageBox(L"Mesh Local Information Create Fail!");
		DeleteTargetMesh();
		return;
	}

	/* Option between dynamic and static */
	LoadTargetAnimation(fullPath);


	/* Collider */
	CRenderer *pRenderer = m_pObjectMesh->FindComponentFromType<CRenderer>(CT_RENDERER);
	CMesh *pMesh = pRenderer->GetMesh();
	
	CTransform *pTr = m_pObjectMesh->GetTransform();
	
	Vector3 vMin, vMax, vCenter;

	vMin = (pMesh->GetMin()).TransformCoord(pTr->GetLocalMatrix().mat);
	vMax = (pMesh->GetMax()).TransformCoord(pTr->GetLocalMatrix().mat);
	vCenter = (pMesh->GetCenter()).TransformCoord(pTr->GetLocalMatrix().mat);

	float fRadius;

	fRadius = pMesh->GetRadius() * pTr->GetLocalScale().x;

	CColliderSphere* pCollider = m_pObjectMesh->AddComponent<CColliderSphere>("Collider");
	pCollider->SetSphere(vCenter, fRadius);
	pCollider->SetColliderRenderCheck(true);
	SAFE_RELEASE(pCollider);

	/*CColliderAABB* pCollider = m_pObjectMesh->AddComponent<CColliderAABB>("Collider");
	pCollider->SetAABB(vMin * 1.5f, vMax * 1.5f);
	pCollider->SetColliderRenderCheck(true);
	SAFE_RELEASE(pCollider);*/

	SAFE_RELEASE(pTr);
	SAFE_RELEASE(pMesh);
	SAFE_RELEASE(pRenderer);
}

void CEffectTab::OnCbnSelchangeComboBoneList()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!m_pObjectMesh)
		return;

	int iPos = m_comboBoxBoneInfo.GetCurSel();

	if (iPos == -1)
		return;

	CString boneName;
	m_comboBoxBoneInfo.GetLBText(iPos, boneName);

	string findName = CT2CA(boneName);
	CAnimation *pAnimation = m_pObjectMesh->FindComponentFromType<CAnimation>(CT_ANIMATION);
	PBONE pBone = pAnimation->FindBone(findName);

	/* 추가 구현 */
	m_pBoneMatrix = pBone->matBone;

	SAFE_RELEASE(pAnimation);
}

void CEffectTab::OnCbnSelchangeComboAnimList()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int iPos = m_comboBoxAnimInfo.GetCurSel();

	if (iPos == -1)
		return;

	CString strClip;
	m_comboBoxAnimInfo.GetLBText(iPos, strClip);

	string strKey = CT2CA(strClip);

	CAnimation *pAnimation = m_pObjectMesh->FindComponentFromType<CAnimation>(CT_ANIMATION);
	CAnimationClip *pClip = pAnimation->FindClip(strKey);
	UpdateData(FALSE);

	pAnimation->ChangeClip(strKey);

	SAFE_RELEASE(pAnimation);
}

void CEffectTab::UpdateForm()
{
	if (m_pTargetObject)
	{
		/* Info */
		UpdateInfo();

		if (m_bFirstTargetCheck)
		{
			/* MainTime */
			UpdateMain();

			/* Pattern */
			UpdatePattern();

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

#pragma region setInfo
void CEffectTab::SetInfoPos()
{
	CTransform *pTr = m_pTargetObject->GetTransform();
	
	Vector3 vNewPos;
	CString X, Y, Z;
	m_editInfoPosX.GetWindowTextW(X);
	m_editInfoPosY.GetWindowTextW(Y);
	m_editInfoPosZ.GetWindowTextW(Z);


	if (X == L"")
	{
		vNewPos.x = m_fInfoStaticPosX;
	}
	else
	{
		vNewPos.x = (float)_wtof(X);
	}

	if (Y == L"")
	{
		vNewPos.y = m_fInfoStaticPosY;
	}
	else
	{
		vNewPos.y = (float)_wtof(Y);
	}

	if (Z == L"")
	{
		vNewPos.z = m_fInfoStaticPosZ;
	}
	else
	{
		vNewPos.z = (float)_wtof(Z);
	}
	
	vNewPos.x += 50.f / 2.f;
	vNewPos.z += 50.f / 2.f;
	
	pTr->SetWorldPos(vNewPos);

	SAFE_RELEASE(pTr);
}

void CEffectTab::SetInfoScale()
{
	CTransform *pTr = m_pTargetObject->GetTransform();

	Vector3 vNewScale;
	CString X, Y, Z;
	m_editInfoScaleX.GetWindowTextW(X);
	m_editInfoScaleY.GetWindowTextW(Y);
	m_editInfoScaleZ.GetWindowTextW(Z);

	if (X == L"")
	{
		vNewScale.x = m_fInfoStaticScaleX;
	}
	else
	{
		vNewScale.x = (float)_wtof(X);
	}

	if (Y == L"")
	{
		vNewScale.y = m_fInfoStaticScaleY;
	}
	else
	{
		vNewScale.y = (float)_wtof(Y);
	}

	if (Z == L"")
	{
		vNewScale.z = m_fInfoStaticScaleZ;
	}
	else
	{
		vNewScale.z = (float)_wtof(Z);
	}

	pTr->SetWorldScale(vNewScale);

	SAFE_RELEASE(pTr);
}

void CEffectTab::SetInfoRot()
{
	CTransform *pTr = m_pTargetObject->GetTransform();

	Vector3 vNewRot;
	CString X, Y, Z;
	m_editInfoRotX.GetWindowTextW(X);
	m_editInfoRotY.GetWindowTextW(Y);
	m_editInfoRotZ.GetWindowTextW(Z);

	if (X == L"")
	{
		vNewRot.x = (float)m_iInfoStaticRotX;
	}
	else
	{
		vNewRot.x = (float)_wtof(X);
	}

	if (Y == L"")
	{
		vNewRot.y = (float)m_iInfoStaticRotY;
	}
	else
	{
		vNewRot.y = (float)_wtof(Y);
	}

	if (Z == L"")
	{
		vNewRot.z = (float)m_iInfoStaticRotZ;
	}
	else
	{
		vNewRot.z = (float)_wtof(Z);
	}

	vNewRot.x = XMConvertToRadians(vNewRot.x);
	vNewRot.y = XMConvertToRadians(vNewRot.y);
	vNewRot.z = XMConvertToRadians(vNewRot.z);

	pTr->SetWorldRot(vNewRot);

	SAFE_RELEASE(pTr);
}
#pragma endregion

void CEffectTab::AddPatternScale(CEffect *pEffect)
{
	CString StartTime, EndTime, Repeat, X, Y, Z;

	m_editPatternScaleX.GetWindowTextW(X);
	m_editPatternScaleY.GetWindowTextW(Y);
	m_editPatternScaleZ.GetWindowTextW(Z);

	m_editPatternScaleStartTime.GetWindowTextW(StartTime);
	m_editPatternScaleEndTime.GetWindowTextW(EndTime);
	m_editPatternScaleRepeat.GetWindowTextW(Repeat);

	/* Init Edit */
	if (X == L"")
		m_fInfoStaticScaleX = 0.f;
	else
		m_fInfoStaticScaleX = (float)_wtof(X);
	
	if (Y == L"")
		m_fInfoStaticScaleY = 0.f;
	else
		m_fInfoStaticScaleY = (float)_wtof(Y);

	if (Z == L"")
		m_fInfoStaticScaleZ = 0.f;
	else
		m_fInfoStaticScaleZ = (float)_wtof(Z);

	if (StartTime == L"")
		m_fPatternStaticScaleStartTime = 0.f;
	else
		m_fPatternStaticScaleStartTime = (float)_wtof(StartTime);

	if (EndTime == L"")
		m_fPatternStaticScaleEndTime = 0.f;
	else
		m_fPatternStaticScaleEndTime = (float)_wtof(EndTime);

	if (Repeat == L"")
		m_iPatternStaticScaleRepeat = 0;
	else
		m_iPatternStaticScaleRepeat = _wtoi(Repeat);

		
	pEffect->AddPatternScale(m_comboEaseSheet_Scale.GetCurSel(),
		m_fPatternStaticScaleStartTime, m_fPatternStaticScaleEndTime,
		m_fInfoStaticScaleX, m_fInfoStaticScaleY, m_fInfoStaticScaleZ,
		m_iPatternStaticScaleRepeat);
}

void CEffectTab::AddPatternRot(CEffect * pEffect)
{
	CString StartTime, EndTime, Repeat, X, Y, Z;

	m_editPatternRotX.GetWindowTextW(X);
	m_editPatternRotY.GetWindowTextW(Y);
	m_editPatternRotZ.GetWindowTextW(Z);

	m_editPatternRotStartTime.GetWindowTextW(StartTime);
	m_editPatternRotEndTime.GetWindowTextW(EndTime);
	m_editPatternRotRepeat.GetWindowTextW(Repeat);

	/* Init Edit */
	if (X == L"")
		m_fPatternStaticRotX = 0.f;
	else
		m_fPatternStaticRotX = (float)_wtof(X);

	if (Y == L"")
		m_fPatternStaticRotY = 0.f;
	else
		m_fPatternStaticRotY = (float)_wtof(Y);

	if (Z == L"")
		m_fPatternStaticRotZ = 0.f;
	else
		m_fPatternStaticRotZ = (float)_wtof(Z);

	if (StartTime == L"")
		m_fPatternStaticRotStartTime = 0.f;
	else
		m_fPatternStaticRotStartTime = (float)_wtof(StartTime);

	if (EndTime == L"")
		m_fPatternStaticRotEndTime = 0.f;
	else
		m_fPatternStaticRotEndTime = (float)_wtof(EndTime);

	if (Repeat == L"")
		m_iPatternStaticRepeat = 0;
	else
		m_iPatternStaticRepeat = _wtoi(Repeat);

	pEffect->AddPatternRotation(m_comboEaseSheet_Rot.GetCurSel(),
		m_fPatternStaticRotStartTime, m_fPatternStaticRotEndTime,
		m_fPatternStaticRotX, m_fPatternStaticRotY, m_fPatternStaticRotZ,
		m_iPatternStaticRepeat);
}

void CEffectTab::OnBnClickedButtonInputInfo()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	if (m_pTargetObject)
	{
		SetInfoPos();
		SetInfoScale();
		SetInfoRot();
	}
}

#pragma region modifybutton_pos
void CEffectTab::OnBnClickedButtonInfoPosxUp()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!m_pTargetObject)
		return;
	m_fInfoStaticPosX += 0.1f;

	/* 오차 범위 */
	if (fabsf(m_fInfoStaticPosX) <= FLT_EPSILON)
		m_fInfoStaticPosX = 0.f;

	m_editInfoPosX.SetWindowTextW(L"");
	SetInfoPos();	
}


void CEffectTab::OnBnClickedButtonInfoPosxDown()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!m_pTargetObject)
		return;
	m_fInfoStaticPosX -= 0.1f;

	/* 오차 범위 */
	if (fabsf(m_fInfoStaticPosX) <= FLT_EPSILON)
		m_fInfoStaticPosX = 0.f;

	m_editInfoPosX.SetWindowTextW(L"");
	SetInfoPos();	
}


void CEffectTab::OnBnClickedButtonInfoPosyUp()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!m_pTargetObject)
		return;
	m_fInfoStaticPosY += 0.1f;

	/* 오차 범위 */
	if (fabsf(m_fInfoStaticPosY) <= FLT_EPSILON)
		m_fInfoStaticPosY = 0.f;

	m_editInfoPosY.SetWindowTextW(L"");
	SetInfoPos();	
}


void CEffectTab::OnBnClickedButtonInfoPosyDown()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!m_pTargetObject)
		return;
	m_fInfoStaticPosY -= 0.1f;

	/* 오차 범위 */
	if (fabsf(m_fInfoStaticPosY) <= FLT_EPSILON)
		m_fInfoStaticPosY = 0.f;

	m_editInfoPosY.SetWindowTextW(L"");
	SetInfoPos();
}


void CEffectTab::OnBnClickedButtonInfoPoszUp()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!m_pTargetObject)
		return;
	m_fInfoStaticPosZ += 0.1f;

	/* 오차 범위 */
	if (fabsf(m_fInfoStaticPosZ) <= FLT_EPSILON)
		m_fInfoStaticPosZ = 0.f;

	m_editInfoPosZ.SetWindowTextW(L"");
	SetInfoPos();
}


void CEffectTab::OnBnClickedButtonInfoPoszDown()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!m_pTargetObject)
		return;
	m_fInfoStaticPosZ -= 0.1f;

	/* 오차 범위 */
	if (fabsf(m_fInfoStaticPosZ) <= FLT_EPSILON)
		m_fInfoStaticPosZ = 0.f;

	m_editInfoPosZ.SetWindowTextW(L"");
	SetInfoPos();
}
#pragma endregion

#pragma region modifybutton_scale
void CEffectTab::OnBnClickedButtonInfoScalexUp()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!m_pTargetObject)
		return;

	m_fInfoStaticScaleX += 0.1f;

	/* 오차 범위 */
	if (fabsf(m_fInfoStaticScaleX) <= FLT_EPSILON)
		m_fInfoStaticScaleX = 0.f;

	m_editInfoScaleX.SetWindowTextW(L"");
	SetInfoScale();
}


void CEffectTab::OnBnClickedButtonInfoScalexDown()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!m_pTargetObject)
		return;

	m_fInfoStaticScaleX -= 0.1f;

	/* 오차 범위 */
	if (fabsf(m_fInfoStaticScaleX) <= FLT_EPSILON)
		m_fInfoStaticScaleX = 0.f;

	m_editInfoScaleX.SetWindowTextW(L"");
	SetInfoScale();	
}


void CEffectTab::OnBnClickedButtonInfoScaleyUp()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!m_pTargetObject)
		return;

	m_fInfoStaticScaleY += 0.1f;

	/* 오차 범위 */
	if (fabsf(m_fInfoStaticScaleY) <= FLT_EPSILON)
		m_fInfoStaticScaleY = 0.f;

	m_editInfoScaleY.SetWindowTextW(L"");
	SetInfoScale();
}


void CEffectTab::OnBnClickedButtonInfoScaleyDown()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!m_pTargetObject)
		return;

	m_fInfoStaticScaleY -= 0.1f;

	/* 오차 범위 */
	if (fabsf(m_fInfoStaticScaleY) <= FLT_EPSILON)
		m_fInfoStaticScaleY = 0.f;

	m_editInfoScaleY.SetWindowTextW(L"");
	SetInfoScale();
}


void CEffectTab::OnBnClickedButtonInfoScalezUp()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!m_pTargetObject)
		return;

	m_fInfoStaticScaleZ += 0.1f;

	/* 오차 범위 */
	if (fabsf(m_fInfoStaticScaleZ) <= FLT_EPSILON)
		m_fInfoStaticScaleZ = 0.f;

	m_editInfoScaleZ.SetWindowTextW(L"");
	SetInfoScale();
}


void CEffectTab::OnBnClickedButtonInfoScalezDown()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!m_pTargetObject)
		return;

	m_fInfoStaticScaleZ -= 0.1f;

	/* 오차 범위 */
	if (fabsf(m_fInfoStaticScaleZ) <= FLT_EPSILON)
		m_fInfoStaticScaleZ = 0.f;

	m_editInfoScaleZ.SetWindowTextW(L"");
	SetInfoScale();
}
#pragma endregion

#pragma region modifybutton_rot
void CEffectTab::OnBnClickedButtonInfoRotxUp()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!m_pTargetObject)
		return;

	m_iInfoStaticRotX += 5;
	m_editInfoRotX.SetWindowTextW(L"");

	SetInfoRot();
}


void CEffectTab::OnBnClickedButtonInfoRotxDown()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!m_pTargetObject)
		return;

	m_iInfoStaticRotX -= 5;
	m_editInfoRotX.SetWindowTextW(L"");
	
	SetInfoRot();
}


void CEffectTab::OnBnClickedButtonInfoRotyUp()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!m_pTargetObject)
		return;

	m_iInfoStaticRotY += 5;
	m_editInfoRotY.SetWindowTextW(L"");

	SetInfoRot();
}


void CEffectTab::OnBnClickedButtonInfoRotyDown()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!m_pTargetObject)
		return;

	m_iInfoStaticRotY -= 5;
	m_editInfoRotY.SetWindowTextW(L"");

	SetInfoRot();
}


void CEffectTab::OnBnClickedButtonInfoRotzUp()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!m_pTargetObject)
		return;

	m_iInfoStaticRotZ += 5;
	m_editInfoRotZ.SetWindowTextW(L"");

	SetInfoRot();
}


void CEffectTab::OnBnClickedButtonInfoRotzDown()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!m_pTargetObject)
		return;

	m_iInfoStaticRotZ -= 5;
	m_editInfoRotZ.SetWindowTextW(L"");

	SetInfoRot();
}
#pragma endregion

void CEffectTab::OnBnClickedButtonViewsheet()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_pEaseSheetDlg == nullptr)
	{
		m_pEaseSheetDlg = new CEaseSheetDlg;
		m_pEaseSheetDlg->Create(IDD_DIALOG3);
	}
	m_pEaseSheetDlg->ShowWindow(SW_SHOW);
}

void CEffectTab::OnBnClickedButtonViewsheet2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnBnClickedButtonViewsheet();
}

void CEffectTab::OnBnClickedCheckScailing()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!m_pTargetObject)
		return;

	CEffect *pEffect = m_pTargetObject->FindComponentFromType<CEffect>(CT_EFFECT);
	if (!pEffect)
		return;

	int check = m_checkSclaling.GetCheck();

	/* Check를 푸는 동작 */
	if(check == 1)
	{
		AddPatternScale(pEffect);
		m_checkSclaling.SetCheck(1);
	}
	/* Check를 하는 동작 */
	else
	{
		pEffect->DeleteAssistEffectFromType(CEffectAssist::ASSIST_SCALE);
		m_checkSclaling.SetCheck(0);
	}

	/* Pattern */
	UpdatePattern();

	SAFE_RELEASE(pEffect);
}

void CEffectTab::OnBnClickedCheckRotating()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!m_pTargetObject)
		return;

	CEffect *pEffect = m_pTargetObject->FindComponentFromType<CEffect>(CT_EFFECT);
	if (!pEffect)
		return;

	int check = m_checkRotating.GetCheck();

	/* Check를 푸는 동작 */
	if (check == 1)
	{
		AddPatternRot(pEffect);
		m_checkRotating.SetCheck(1);
	}
	/* Check를 하는 동작 */
	else
	{
		pEffect->DeleteAssistEffectFromType(CEffectAssist::ASSIST_ROT);
		m_checkRotating.SetCheck(0);
	}

	/* Pattern */
	UpdatePattern();

	SAFE_RELEASE(pEffect);
}

void CEffectTab::OnBnClickedButtonPatternScaleInput()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!m_pTargetObject)
		return;

	CEffect *pEffect = m_pTargetObject->FindComponentFromType<CEffect>(CT_EFFECT);
	if (!pEffect)
		return;

	if (m_checkSclaling.GetCheck() == 0)
	{
		AfxMessageBox(L"Add scale pattern first!");
		return;
	}

	AddPatternScale(pEffect);

	/* Pattern */
	UpdatePattern();

	SAFE_RELEASE(pEffect);
}

void CEffectTab::OnBnClickedButtonPatternScalePlay()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!m_pTargetObject)
		return;

	CEffect *pEffect = m_pTargetObject->FindComponentFromType<CEffect>(CT_EFFECT);
	if (!pEffect)
		return;

	if (m_checkSclaling.GetCheck() == 0)
	{
		AfxMessageBox(L"Add scale pattern first!");
		return;
	}

	pEffect->SetOperationCheckPart(CEffectAssist::ASSIST_SCALE, true);

	/* Pattern */
	UpdatePattern();

	SAFE_RELEASE(pEffect);
}

void CEffectTab::OnBnClickedButtonPatternScaleStop()
{
	if (!m_pTargetObject)
		return;

	CEffect *pEffect = m_pTargetObject->FindComponentFromType<CEffect>(CT_EFFECT);
	if (!pEffect)
		return;

	if (m_checkSclaling.GetCheck() == 0)
	{
		AfxMessageBox(L"Add scale pattern first!");
		return;
	}

	pEffect->SetOperationCheckPart(CEffectAssist::ASSIST_SCALE, false);

	/* Pattern */
	UpdatePattern();

	SAFE_RELEASE(pEffect);
}

void CEffectTab::OnBnClickedButtonPatternRotInput()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!m_pTargetObject)
		return;

	CEffect *pEffect = m_pTargetObject->FindComponentFromType<CEffect>(CT_EFFECT);
	if (!pEffect)
		return;

	if (m_checkRotating.GetCheck() == 0)
	{
		AfxMessageBox(L"Add rot pattern first!");
		return;
	}

	AddPatternRot(pEffect);

	/* Pattern */
	UpdatePattern();

	SAFE_RELEASE(pEffect);
}

void CEffectTab::OnBnClickedButtonPatternRotPlay()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!m_pTargetObject)
		return;

	CEffect *pEffect = m_pTargetObject->FindComponentFromType<CEffect>(CT_EFFECT);
	if (!pEffect)
		return;

	if (m_checkRotating.GetCheck() == 0)
	{
		AfxMessageBox(L"Add rot pattern first!");
		return;
	}

	pEffect->SetOperationCheckPart(CEffectAssist::ASSIST_ROT, true);

	/* Pattern */
	UpdatePattern();

	SAFE_RELEASE(pEffect);
}

void CEffectTab::OnBnClickedButtonPatternRotStop()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!m_pTargetObject)
		return;

	CEffect *pEffect = m_pTargetObject->FindComponentFromType<CEffect>(CT_EFFECT);
	if (!pEffect)
		return;

	if (m_checkRotating.GetCheck() == 0)
	{
		AfxMessageBox(L"Add rot pattern first!");
		return;
	}

	pEffect->SetOperationCheckPart(CEffectAssist::ASSIST_ROT, false);

	/* Pattern */
	UpdatePattern();

	SAFE_RELEASE(pEffect);
}
