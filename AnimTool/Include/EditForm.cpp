// EditForm.cpp: 구현 파일
//

#include "stdafx.h"
#include "AnimTool.h"
#include "EditForm.h"

#include "MainFrm.h"
#include "AnimToolView.h"

#include "AnimMeshInfoTab.h"
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
#include "Component/AxisLine.h"
#include "Resources/ResourcesManager.h"

// CEditForm 대화 상자

IMPLEMENT_DYNCREATE(CEditForm, CFormView)

CEditForm::CEditForm()
	: CFormView(IDD_DIALOG_FORM)
{

}

CEditForm::~CEditForm()
{
	delete m_pAnimMeshInfoDlg;
	m_pAnimMeshInfoDlg = nullptr;
	m_pBoneMatrix = nullptr;
	SAFE_RELEASE(m_pEditObj);
	SAFE_RELEASE(m_pArmObj);
}

BEGIN_MESSAGE_MAP(CEditForm, CView)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_ANIM, &CEditForm::OnTcnSelchangeTabAnim)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_MESH, &CEditForm::OnBnClickedButtonLoadMesh)
	ON_BN_CLICKED(IDC_BUTTON_ADD_CLIP, &CEditForm::OnBnClickedButtonAddClip)
	ON_BN_CLICKED(IDC_BUTTON_MODIFY_CLIP, &CEditForm::OnBnClickedButtonModifyClip)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_CLIP, &CEditForm::OnBnClickedButtonDeleteClip)
	ON_BN_CLICKED(IDC_BUTTON_CLIP_DEFAULT, &CEditForm::OnBnClickedButtonClipDefault)
	ON_LBN_SELCHANGE(IDC_LIST_CLIPS, &CEditForm::OnLbnSelchangeListClips)
	ON_BN_CLICKED(IDC_BUTTON_PLAY_PAUSE, &CEditForm::OnBnClickedButtonPlay)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CEditForm::OnBnClickedButtonStop)
	ON_WM_HSCROLL()
	ON_CBN_SELCHANGE(IDC_COMBO_BONE_INFO, &CEditForm::OnCbnSelchangeComboBoneInfo)
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
	DDX_Control(pDX, IDC_SLIDER_CLIP_INFO, m_ctrlSliderClipFrame);
	DDX_Control(pDX, IDC_EDIT_POS_CLIP, m_editFramePosition);
	DDX_Control(pDX, IDC_COMBO_BONE_INFO, m_comboBoxBoneInfo);
}



void CEditForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	m_Tab.InsertItem(0, _T("ObjInfo"));
	m_Tab.InsertItem(1, _T("Movement"));
	m_Tab.InsertItem(2, _T("Collider"));
	m_Tab.InsertItem(3, _T("OnOff"));
	m_Tab.InsertItem(4, _T("CamShake"));

	CRect rect;
	m_Tab.GetWindowRect(&rect);

	m_Tab.SetCurSel(0);

	m_pAnimMeshInfoDlg = new CAnimMeshInfoTab;
	m_pAnimMeshInfoDlg->Create(IDD_DIALOG_MESH_INFO, &m_Tab);
	m_pAnimMeshInfoDlg->MoveWindow(0, 20, rect.Width(), rect.Height());
	m_pAnimMeshInfoDlg->ShowWindow(SW_SHOW);

	// 메인 프레임을 받아온다.
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	m_pView = (CAnimToolView*)pMain->GetActiveView();
}

void CEditForm::MeshLoadFromMeshInfoTab(CString path, CString name)
{
	if (m_pEditObj)
	{
		CAnimation* pAnimation = m_pEditObj->FindComponentFromType<CAnimation>(CT_ANIMATION);
		if (pAnimation)
		{
			SAFE_RELEASE(pAnimation);
			DeleteEditObject();
			InitForm();
		}
	}

 	if (!m_pEditObj)
	{
		CScene*	pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
		CLayer*	pLayer = pScene->GetLayer("Default");

		m_pEditObj = CGameObject::CreateObject("EditObj",
			pLayer);
		CTransform*	pTr = m_pEditObj->GetTransform();
		pTr->SetWorldPos(5.f, 0.f, 5.f);
		SAFE_RELEASE(pTr);

		CRenderer*	pRenderer = m_pEditObj->AddComponent<CRenderer>("Renderer");

		string	strTag = CT2CA(name.GetString());

		pRenderer->SetMeshFromFullPath(strTag,
			path.GetString());

		SAFE_RELEASE(pRenderer);
		SAFE_RELEASE(pScene);
		SAFE_RELEASE(pLayer);
	}
	else
	{
		CTransform*	pTr = m_pEditObj->GetTransform();
		pTr->SetWorldPos(5.f, 0.f, 5.f);
		SAFE_RELEASE(pTr);

		CRenderer* pRenderer = m_pEditObj->AddComponent<CRenderer>("Renderer");
		string tag = CT2CA(name);

		pRenderer->SetMeshFromFullPath(tag, path);

		SAFE_RELEASE(pRenderer);
	}

	CScene*	pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
	CLayer*	pLayer = pScene->GetLayer("Default");

	CThirdCamera* pThirdCam = nullptr;
	CArm* pArm = nullptr;

	CGameObject* pCameraObj = pScene->GetMainCameraObj();
	pThirdCam = pCameraObj->FindComponentFromTag<CThirdCamera>("ThirdCamera");
	if (pThirdCam)
	{
		pArm = pCameraObj->FindComponentFromTag<CArm>("Arm");
	}
	else
	{
		pThirdCam = pCameraObj->AddComponent<CThirdCamera>("ThirdCamera");
		pArm = pCameraObj->AddComponent<CArm>("Arm");
	}
	

	pArm->SetTarget(m_pEditObj);
	pArm->SetLookAtDist(Vector3(0.f, 1.f, 0.f));

	m_pAnimMeshInfoDlg->SetEditObj(m_pEditObj);
	m_pAnimMeshInfoDlg->SetMeshInfo();
	
	SAFE_RELEASE(pArm);
	SAFE_RELEASE(pThirdCam);
	SAFE_RELEASE(pCameraObj);
	SAFE_RELEASE(pScene);
	SAFE_RELEASE(pLayer);
}

void CEditForm::ArmMeshLoadFromMeshInfoTab(CString path, CString name)
{
	if (m_pArmObj)
	{
		// 지우고 다시 만들기?
		m_pArmObj->Die();
		CGameObject::EraseObj(m_pArmObj);
		SAFE_RELEASE(m_pArmObj);
	}

	if (!m_pArmObj)
	{
		CScene*	pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
		CLayer*	pLayer = pScene->GetLayer("Default");

		m_pArmObj = CGameObject::CreateObject("ArmObj",
			pLayer);
		
		CTransform*	pTr = m_pArmObj->GetTransform();
		SAFE_RELEASE(pTr);

		CRenderer*	pRenderer = m_pArmObj->AddComponent<CRenderer>("Renderer");

		string	strTag = CT2CA(name.GetString());

		pRenderer->SetMeshFromFullPath(strTag,
			path.GetString());

		SAFE_RELEASE(pRenderer);
		SAFE_RELEASE(pScene);
		SAFE_RELEASE(pLayer);
	}
	

	m_pAnimMeshInfoDlg->SetArmObj(m_pArmObj);
}

void CEditForm::AnimationLoadFromMeshInfoTab(CString path, CString name)
{
	CAnimation*	pAnimationCheck = m_pEditObj->FindComponentFromTag<CAnimation>("Animation");

	if (pAnimationCheck)
	{
		AfxMessageBox(L"Error : Your object already have Animation!");
		SAFE_RELEASE(pAnimationCheck);
		return;
	}

	if (!m_pEditObj)
	{
		CScene*	pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
		CLayer*	pLayer = pScene->GetLayer("Default");

		m_pEditObj = CGameObject::CreateObject("EditObj", pLayer);

		CTransform*	pTr = m_pEditObj->GetTransform();
		SAFE_RELEASE(pTr);

		CAnimation*	pAnimation = m_pEditObj->AddComponent<CAnimation>("Animation");
		pAnimation->LoadFromFullPath(path);

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

		m_pAnimMeshInfoDlg->SetEditObj(m_pEditObj);

		SAFE_RELEASE(pAnimation);
		SAFE_RELEASE(pLayer);
		SAFE_RELEASE(pScene);
	}
	else
	{
		CAnimation*	pAnimation = m_pEditObj->AddComponent<CAnimation>("Animation");

		pAnimation->LoadFromFullPath(path);

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

		SAFE_RELEASE(pAnimation);
	}
	
	// AxisLine
	CAxisLine* pAxisLine = m_pEditObj->AddComponent<CAxisLine>("AxisLine");
	SAFE_RELEASE(pAxisLine);

	CAnimation*	pAnimation = m_pEditObj->FindComponentFromType<CAnimation>(CT_ANIMATION);
	// Bone Info
	const vector<PBONE> boneInfo = pAnimation->GetBoneVector();

	for (const auto iter : boneInfo)
	{
		CString boneName = CA2CT(iter->strName.c_str());
		m_comboBoxBoneInfo.AddString(boneName);
	}

	UpdateData(FALSE);

	SAFE_RELEASE(pAnimation);

	m_listClips.SetCurSel(m_iPos);
}

void CEditForm::SetBoneMatrix(const string& bone)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!m_pEditObj)
		return;

	if (bone == "")
	{
		int iPos = m_comboBoxBoneInfo.GetCurSel();

		if (iPos == -1)
		{
			AfxMessageBox(L"Error : Select Bone First!");
			return;
		}

		CString boneName;
		m_comboBoxBoneInfo.GetLBText(iPos, boneName);

		string findName = CT2CA(boneName);
		CAnimation* pAnimation = m_pEditObj->FindComponentFromType<CAnimation>(CT_ANIMATION);
		PBONE pBone = pAnimation->FindBone(findName);

		m_boneNameAttachTo = pBone->strName;
		m_pBoneMatrix = pBone->matBone;
		
		SAFE_RELEASE(pAnimation);
	}
	else
	{
		CAnimation* pAnimation = m_pEditObj->FindComponentFromType<CAnimation>(CT_ANIMATION);
		PBONE pBone = pAnimation->FindBone(bone);

		m_boneNameAttachTo = pBone->strName;
		m_pBoneMatrix = pBone->matBone;

		SAFE_RELEASE(pAnimation);
	}
}

const string & CEditForm::GetBoneName()
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_boneNameAttachTo;
}

void CEditForm::UpdateForm(const float & fTime)
{
	if (!m_pEditObj)
		return;

	CAnimation*	pAnimation = m_pEditObj->FindComponentFromType<CAnimation>(CT_ANIMATION);

	if (!pAnimation)
		return;

	CAnimationClip* pClip = pAnimation->GetCurrentClip();
	ANIMATION3DCLIP clipInfo = pClip->GetClipInfo();

	CString num;

	// Range
	int iCurrentFrameNum = pAnimation->GetClipFrame();
	num.Format(_T("%d"), iCurrentFrameNum);
	m_editFramePosition.SetWindowTextW(num);

	m_ctrlSliderClipFrame.SetRange(clipInfo.iStartFrame, clipInfo.iEndFrame);
	m_ctrlSliderClipFrame.SetTicFreq(5);
	m_ctrlSliderClipFrame.SetLineSize(1);

	if (!m_bStopCheck)
		m_ctrlSliderClipFrame.SetPos(iCurrentFrameNum);

	CTransform* pTr = m_pEditObj->GetTransform();

	CAxisLine* pAxisLine = m_pEditObj->FindComponentFromTag<CAxisLine>("AxisLine");
	
	if (pAxisLine)
	{
		pAxisLine->SetObjMatrix(pTr->GetLocalMatrix() * pTr->GetWorldMatrix());
		SAFE_RELEASE(pAxisLine);
	}

	if (m_pArmObj && m_pBoneMatrix)
	{
		CTransform* pArmTr = m_pArmObj->GetTransform();
		pArmTr->SetParentMatrix(*m_pBoneMatrix * pTr->GetLocalMatrix() * pTr->GetWorldMatrix());
		SAFE_RELEASE(pArmTr);
	}

	SAFE_RELEASE(pTr);
	SAFE_RELEASE(pAnimation);
}

void CEditForm::DeleteEditObject()
{
	if(m_pEditObj)
		m_pEditObj->Die();
	if (m_pArmObj)
		m_pArmObj->Die();

	CGameObject::EraseObj(m_pEditObj);
	CGameObject::EraseObj(m_pArmObj);
	SAFE_RELEASE(m_pEditObj);
	SAFE_RELEASE(m_pArmObj);
	m_pBoneMatrix = nullptr;
}

void CEditForm::InitForm()
{
	m_iStartFrame = 0;
	m_iEndFrame = 0;
	m_listClips.ResetContent();
	m_comboBoxBoneInfo.ResetContent();
	m_boneNameAttachTo = "";
	m_bStopCheck = false;
}

void CEditForm::OnTcnSelchangeTabAnim(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	int sel = m_Tab.GetCurSel();

	switch (sel)
	{
	case 0:
		m_pAnimMeshInfoDlg->ShowWindow(SW_SHOW);
		break;
	case 1:
		m_pAnimMeshInfoDlg->ShowWindow(SW_HIDE);
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

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_pEditObj)
	{
		DeleteEditObject();
		InitForm();

		//AfxMessageBox(L"Error : Already have Edit Obj!");
		//return;
	}

	CString path = dlg.GetPathName();
	CString name = dlg.GetFileTitle();

	CScene*	pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
	CLayer*	pLayer = pScene->GetLayer("Default");
	

	m_pEditObj = CGameObject::CreateObject("EditObj", pLayer);
	

	CTransform*	pTr = m_pEditObj->GetTransform();
	pTr->SetWorldPos(10.f, 0.f, 10.f);

	CRenderer* pRenderer = m_pEditObj->AddComponent<CRenderer>("Renderer");
	CT2CA pszConvertAnsiStringName(name);
	string tag(pszConvertAnsiStringName);
	
	pRenderer->SetMeshFromFullPath(tag, path);

	// Axis Line
	CAxisLine* pAxisLine = m_pEditObj->AddComponent<CAxisLine>("AxisLine");
	pAxisLine->SetObjMatrix(pTr->GetWorldMatrix());
	SAFE_RELEASE(pTr);
	SAFE_RELEASE(pAxisLine);

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

		// Bone Info
		const vector<PBONE> boneInfo = pAnimation->GetBoneVector();

		for (const auto iter : boneInfo)
		{
			CString boneName = CA2CT(iter->strName.c_str());
			m_comboBoxBoneInfo.AddString(boneName);
		}

		UpdateData(FALSE);

		SAFE_RELEASE(pAnimation);
		SAFE_RELEASE(pRenderer);
	}

	CThirdCamera* pThirdCam = nullptr;
	CArm* pArm = nullptr;

	CGameObject* pCameraObj = pScene->GetMainCameraObj();
	pThirdCam = pCameraObj->FindComponentFromTag<CThirdCamera>("ThirdCamera");
	if (pThirdCam)
	{
		pArm = pCameraObj->FindComponentFromTag<CArm>("Arm");
	}
	else
	{
		pThirdCam = pCameraObj->AddComponent<CThirdCamera>("ThirdCamera");
		pArm = pCameraObj->AddComponent<CArm>("Arm");
	}

	pArm->SetTarget(m_pEditObj);
	pArm->SetLookAtDist(Vector3(0.f, 1.f, 0.f));

	// Tab
	m_pAnimMeshInfoDlg->SetEditObj(m_pEditObj);
	m_pAnimMeshInfoDlg->SetMeshInfo();

	SAFE_RELEASE(pArm);
	SAFE_RELEASE(pThirdCam);
	SAFE_RELEASE(pCameraObj);
	SAFE_RELEASE(pLayer);
	SAFE_RELEASE(pScene);
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
	case 2:
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

	m_listClips.SetCurSel(m_iPos);

	SAFE_RELEASE(pAnimation);
}

void CEditForm::OnBnClickedButtonModifyClip()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_iPos = m_listClips.GetCurSel();

	if (m_iPos == -1)
	{
		AfxMessageBox(L"Error: You didn't select the clip!");
		m_iPos = 0;
		return;
	}

	CString listText;
	m_listClips.GetText(m_iPos, listText);

	UpdateData(TRUE);

	string key = CT2CA(listText);
	string change = CT2CA(m_clipName);

	CAnimation* pAnimation = m_pEditObj->FindComponentFromType<CAnimation>(CT_ANIMATION);
	ANIMATION_OPTION	eOption = (ANIMATION_OPTION)m_iRadioAnimType;

	pAnimation->ModifyClip(key, change, eOption, m_iStartFrame, m_iEndFrame);

	SAFE_RELEASE(pAnimation);

	m_listClips.DeleteString(m_iPos);
	m_listClips.InsertString(m_iPos, m_clipName);
	m_listClips.SetCurSel(m_iPos);
}

void CEditForm::OnBnClickedButtonDeleteClip()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!m_pEditObj)
	{
		AfxMessageBox(L"Error: You have to create Object first!");
		return;
	}

	// 애니메이션이 하나밖에 없는경우.
	// 그냥 Edit객체를 지워버린다.
	if (m_listClips.GetCount() == 1)
	{
		DeleteEditObject();
		InitForm();
		return;
	}

	m_iPos = m_listClips.GetCurSel();

	if (m_iPos == -1)
	{
		AfxMessageBox(L"Error: You didn't select the clip!");
		m_iPos = 0;
		return;
	}

	CString listText;
	m_listClips.GetText(m_iPos, listText);

	string key = CT2CA(listText);

	CAnimation* pAnimation = m_pEditObj->FindComponentFromType<CAnimation>(CT_ANIMATION);
	pAnimation->DeleteClip(key);
	SAFE_RELEASE(pAnimation);
	m_listClips.DeleteString(m_iPos);
}

void CEditForm::OnBnClickedButtonClipDefault()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!m_pEditObj)
	{
		AfxMessageBox(L"Error: You have to create Object first!");
		return;
	}

	int iPos = m_listClips.GetCurSel();

	if (iPos == -1)
	{
		AfxMessageBox(L"Error: You didn't select the clip!");
		return;
	}

	CString listText;
	m_listClips.GetText(iPos, listText);

	string key = CT2CA(listText);

	CAnimation* pAnimation = m_pEditObj->FindComponentFromType<CAnimation>(CT_ANIMATION);
	pAnimation->SetDefaultClip(key);
	SAFE_RELEASE(pAnimation);
}

void CEditForm::OnLbnSelchangeListClips()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_iPos = m_listClips.GetCurSel();

	if (m_iPos == -1)
	{
		AfxMessageBox(L"Error: You didn't select the clip!");
		m_iPos = 0;
		return;
	}

	CString	strClip;
	m_listClips.GetText(m_iPos, strClip);

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

	// 정지되어 있다면 Update를 위해 강제 Play
	if(m_bStopCheck)
		OnBnClickedButtonPlay();
}

void CEditForm::OnBnClickedButtonPlay()
{
	if (!m_pEditObj)
		return;
	
	CAnimation* pAnimation = m_pEditObj->FindComponentFromType<CAnimation>(COMPONENT_TYPE::CT_ANIMATION);

	if (!pAnimation)
		return;

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_bStopCheck)
		m_bStopCheck = false;

	pAnimation->SetStopCheck(m_bStopCheck);

	SAFE_RELEASE(pAnimation);
}

void CEditForm::OnBnClickedButtonStop()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!m_pEditObj)
		return;

	CAnimation* pAnimation = m_pEditObj->FindComponentFromType<CAnimation>(COMPONENT_TYPE::CT_ANIMATION);

	if (!pAnimation)
		return;

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!m_bStopCheck)
		m_bStopCheck = true;

	pAnimation->SetStopCheck(m_bStopCheck);

	SAFE_RELEASE(pAnimation);
}

void CEditForm::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (!m_bStopCheck)
		return;

	if (pScrollBar)
	{
		if (pScrollBar == (CScrollBar*)&m_ctrlSliderClipFrame)
		{
			int iPos = m_ctrlSliderClipFrame.GetPos();
			
			if (!m_pEditObj)
				return;

			CAnimation* pAnimation = m_pEditObj->FindComponentFromType<CAnimation>(COMPONENT_TYPE::CT_ANIMATION);

			if (!pAnimation)
				return;

			pAnimation->SetClipFrame(iPos);

			SAFE_RELEASE(pAnimation);
		}
	}

	CFormView::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CEditForm::OnCbnSelchangeComboBoneInfo()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!m_pEditObj)
		return;

	int iPos = m_comboBoxBoneInfo.GetCurSel();

	if (iPos == -1)
		return;

	CString boneName;
	m_comboBoxBoneInfo.GetLBText(iPos, boneName);

	string findName = CT2CA(boneName);
	CAnimation* pAnimation = m_pEditObj->FindComponentFromType<CAnimation>(CT_ANIMATION);
	PBONE pBone = pAnimation->FindBone(findName);

	CAxisLine* pAxis = m_pEditObj->FindComponentFromTag<CAxisLine>("AxisLine");
	if (pAxis)
	{
		pAxis->SetRenderCheck(true);
		pAxis->SetBoneMatrix(pBone->matBone);
		SAFE_RELEASE(pAxis);
	}
}
