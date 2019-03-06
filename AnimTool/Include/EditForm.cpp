// EditForm.cpp: 구현 파일
//

#include "stdafx.h"
#include "AnimTool.h"
#include "EditForm.h"

#include "MainFrm.h"
#include "AnimToolView.h"

#include "AnimSpeedTab.h"
#include "Core/PathManager.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "Scene/Layer.h"
#include "Component/Renderer.h"
#include "Component/Camera.h"
#include "Component/ThirdCamera.h"
#include "Component/Arm.h"
#include "Resources/Mesh.h"
#include "Component/Animation.h"
#include "Component/AnimationClip.h"

// CEditForm 대화 상자

IMPLEMENT_DYNCREATE(CEditForm, CFormView)

CEditForm::CEditForm()
	: CFormView(IDD_DIALOG_FORM)
{

}

CEditForm::~CEditForm()
{
	delete m_pAnimSpeedDlg;
	m_pAnimSpeedDlg = nullptr;

	SAFE_RELEASE(m_pEditObj);
}

BEGIN_MESSAGE_MAP(CEditForm, CView)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_ANIM, &CEditForm::OnTcnSelchangeTabAnim)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_MESH, &CEditForm::OnBnClickedButtonLoadMesh)
	ON_BN_CLICKED(IDC_BUTTON_ADD_CLIP, &CEditForm::OnBnClickedButtonAddClip)
	ON_BN_CLICKED(IDC_BUTTON_MODIFY_CLIP, &CEditForm::OnBnClickedButtonModifyClip)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_CLIP, &CEditForm::OnBnClickedButtonDeleteClip)
	ON_BN_CLICKED(IDC_BUTTON_CLIP_DEFAULT, &CEditForm::OnBnClickedButtonClipDefault)
	ON_LBN_SELCHANGE(IDC_LIST_CLIPS, &CEditForm::OnLbnSelchangeListClips)
END_MESSAGE_MAP()

// EditForm 진단

#ifdef _DEBUG
void CEditForm::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CEditForm::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CEditForm 메시지 처리기

void CEditForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_ANIM, m_Tab);
	DDX_Control(pDX, IDC_CHECK_PLAY, m_checkPlay);
	DDX_Control(pDX, IDC_LIST_CLIPS, m_listClips);
	DDX_Text(pDX, IDC_EDIT_CLIP_NAME, m_clipName);
	DDX_Text(pDX, IDC_EDIT_START_FRAME_CLIP, m_iStartFrame);
	DDX_Text(pDX, IDC_EDIT_END_FRAME_CLIP, m_iEndFrame);
	DDX_Radio(pDX, IDC_RADIO_CLIP_TYPE_1, (int&)m_iRadioAnimType);
}



void CEditForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	m_Tab.InsertItem(0, _T("Speed"));
	m_Tab.InsertItem(1, _T("Movement"));
	m_Tab.InsertItem(2, _T("Collider"));
	m_Tab.InsertItem(3, _T("OnOff"));
	m_Tab.InsertItem(4, _T("CamShake"));

	CRect rect;
	m_Tab.GetWindowRect(&rect);

	m_Tab.SetCurSel(0);

	m_pAnimSpeedDlg = new CAnimSpeedTab;
	m_pAnimSpeedDlg->Create(IDD_DIALOG_ANIM_SPEED, &m_Tab);
	m_pAnimSpeedDlg->MoveWindow(0, 20, rect.Width(), rect.Height());
	m_pAnimSpeedDlg->ShowWindow(SW_SHOW);

	// 메인 프레임을 받아온다.
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	m_pView = (CAnimToolView*)pMain->GetActiveView();
}


void CEditForm::OnTcnSelchangeTabAnim(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	int sel = m_Tab.GetCurSel();

	switch (sel)
	{
	case 0:
		m_pAnimSpeedDlg->ShowWindow(SW_SHOW);
		break;
	case 1:
		m_pAnimSpeedDlg->ShowWindow(SW_HIDE);
		break;
	case 2:
		break;
	case 3:
		break;
	case 4:
		break;
	}

	*pResult = 0;
}


void CEditForm::OnBnClickedButtonLoadMesh()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	static TCHAR BASED_CODE szFilter[] =
		_T("메쉬 파일(*.FBX) | *.fbx;|모든파일(*.*)|*.*||");
	CFileDialog dlg(TRUE, NULL, NULL, OFN_READONLY | OFN_OVERWRITEPROMPT, szFilter);

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

	CScene*	pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
	CLayer*	pLayer = pScene->GetLayer("Default");
	

	m_pEditObj = CGameObject::CreateObject("EditObj", pLayer);
	CTransform*	pTr = m_pEditObj->GetTransform();
	pTr->SetWorldPos(5.f, 0.f, 5.f);
	pTr->SetPivot(0.f, -0.5f, 0.f);
	SAFE_RELEASE(pTr);

	CRenderer* pRenderer = m_pEditObj->AddComponent<CRenderer>("Renderer");
	CT2CA pszConvertAnsiStringName(name);
	string tag(pszConvertAnsiStringName);
	
	pRenderer->SetMeshFromFullPath(tag, path);

	CAnimation *pAnimation = m_pEditObj->FindComponentFromType<CAnimation>(CT_ANIMATION);

	if (pAnimation)
	{
		const unordered_map<string, class CAnimationClip*>*	pClips =
			pAnimation->GetAllClip();

		unordered_map<string, class CAnimationClip*>::const_iterator	iter;
		unordered_map<string, class CAnimationClip*>::const_iterator	iterEnd = pClips->end();

		for (iter = pClips->begin(); iter != iterEnd; ++iter)
		{
			ANIMATION3DCLIP tInfo = iter->second->GetClipInfo();
			wstring	strName = CA2CT(tInfo.strName.c_str());

			m_listClips.AddString(strName.c_str());

			m_clipName = strName.c_str();
			m_iStartFrame = tInfo.iStartFrame;
			m_iEndFrame = tInfo.iEndFrame;
			m_iRadioAnimType = (int)tInfo.eOption;
		}

		CGameObject* pCameraObj = pScene->GetMainCameraObj();
		CThirdCamera* pThirdCam = pCameraObj->AddComponent<CThirdCamera>("ThirdCamera");

		CArm*	pArm = pCameraObj->AddComponent<CArm>("Arm");

		pArm->SetTarget(m_pEditObj);
		pArm->SetLookAtDist(Vector3(0.f, 1.f, 0.f));

		UpdateData(FALSE);

		SAFE_RELEASE(pArm);
		SAFE_RELEASE(pThirdCam);
		SAFE_RELEASE(pCameraObj);
		SAFE_RELEASE(pAnimation);
		SAFE_RELEASE(pRenderer);
		SAFE_RELEASE(pLayer);
		SAFE_RELEASE(pScene);
	}
}

void CEditForm::OnRadioAnimTypeCheck(UINT id)
{
	UpdateData(TRUE);

	switch (m_iRadioAnimType)
	{
	case 0:
		break;
	case 1:
		break;
	}
}

void CEditForm::OnBnClickedButtonAddClip()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!m_pEditObj)
	{
		AfxMessageBox(L"Error: You have to create Object first!");
		return;
	}

	static TCHAR BASED_CODE szFilter[] =
		_T("메쉬 파일(*.FBX) | *.fbx;|애니메이션 파일(*.ANM) | *.anm;|모든파일(*.*)|*.*||");
	CFileDialog dlg(TRUE, NULL, NULL, OFN_READONLY | OFN_OVERWRITEPROMPT, szFilter);

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
	CString ext = dlg.GetFileExt();

	char	strExt1[_MAX_EXT] = {};
	strcpy_s(strExt1, CT2CA(ext));

	_strupr_s(strExt1);

	CAnimation* pAnimation = m_pEditObj->FindComponentFromType<CAnimation>(CT_ANIMATION);

	if (!strcmp(strExt1, "FBX"))
	{
		pAnimation->AddClip(path);
		CAnimationClip* pClip = pAnimation->GetLastAddClip();

		ANIMATION3DCLIP tInfo = pClip->GetClipInfo();
		wstring wName = CA2CT(tInfo.strName.c_str());

		m_listClips.AddString(wName.c_str());

		m_clipName = wName.c_str();
		m_iStartFrame = tInfo.iStartFrame;
		m_iEndFrame = tInfo.iEndFrame;
		m_iRadioAnimType = (int)tInfo.eOption;

		SAFE_RELEASE(pClip);
	}
	else if (strcmp(strExt1, "ANM") == 0)
	{
		pAnimation->LoadFromFullPath(path);

		CAnimationClip*	pClip = pAnimation->GetLastAddClip();

		ANIMATION3DCLIP tInfo = pClip->GetClipInfo();
		wstring	wName = CA2CT(tInfo.strName.c_str());

		m_listClips.AddString(wName.c_str());

		m_clipName = wName.c_str();
		m_iStartFrame = tInfo.iStartFrame;
		m_iEndFrame = tInfo.iEndFrame;
		m_iRadioAnimType = (int)tInfo.eOption;

		SAFE_RELEASE(pClip);
	}

	SAFE_RELEASE(pAnimation);
}

void CEditForm::OnBnClickedButtonModifyClip()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int iPos = m_listClips.GetCurSel();

	if (iPos == -1)
	{
		AfxMessageBox(L"Error: You didn't select the clip!");
		return;
	}

	CString listText;
	m_listClips.GetText(iPos, listText);

	UpdateData(TRUE);

	string key = CT2CA(listText);
	string change = CT2CA(m_clipName);

	CAnimation* pAnimation = m_pEditObj->FindComponentFromType<CAnimation>(CT_ANIMATION);
	ANIMATION_OPTION	eOption = (ANIMATION_OPTION)m_iRadioAnimType;

	pAnimation->ModifyClip(key, change, eOption, m_iStartFrame, m_iEndFrame);

	SAFE_RELEASE(pAnimation);

	m_listClips.DeleteString(iPos);
	m_listClips.InsertString(iPos, m_clipName);
}


void CEditForm::OnBnClickedButtonDeleteClip()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CEditForm::OnBnClickedButtonClipDefault()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CEditForm::OnLbnSelchangeListClips()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int iPos = m_listClips.GetCurSel();

	if (iPos == -1)
	{
		AfxMessageBox(L"Error: You didn't select the clip!");
		return;
	}

	CString	strClip;
	m_listClips.GetText(iPos, strClip);

	string	strKey = CT2CA(strClip);

	CAnimation* pAnimation = m_pEditObj->FindComponentFromType<CAnimation>(COMPONENT_TYPE::CT_ANIMATION);
	CAnimationClip* pClip = pAnimation->FindClip(strKey);
	ANIMATION3DCLIP tInfo = pClip->GetClipInfo();

	m_clipName = CA2CT(tInfo.strName.c_str());
	m_iStartFrame = tInfo.iStartFrame;
	m_iEndFrame = tInfo.iEndFrame;
	m_iRadioAnimType = (int)tInfo.eOption;

	UpdateData(FALSE);

	// 모션 변경 
	pAnimation->ChangeClip(strKey);


	SAFE_RELEASE(pAnimation);

}
