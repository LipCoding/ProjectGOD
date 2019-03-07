﻿// AnimSpeedTab.cpp: 구현 파일
//

#include "stdafx.h"
#include "AnimTool.h"
#include "AnimMeshInfoTab.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "EditForm.h"

#include "Component/Transform.h"
#include "Core/PathManager.h"
#include "Component/Renderer.h"
#include "Resources/Mesh.h"
#include "Component/Animation.h"
// CAnimMeshInfoTab 대화 상자

IMPLEMENT_DYNAMIC(CAnimMeshInfoTab, CDialogEx)

CAnimMeshInfoTab::CAnimMeshInfoTab(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_MESH_INFO, pParent)
	, m_fScaleX(0.f)
	, m_fScaleY(0.f)
	, m_fScaleZ(0.f)
	, m_fRotationX(0.f)
	, m_fRotationY(0.f)
	, m_fRotationZ(0.f)
	, m_fPositionX(0)
	, m_fPositionY(0)
	, m_fPositionZ(0)
	, m_armObjName(_T(""))
	, m_fSpeed(0)
	, m_fPivot(0)
{

}

CAnimMeshInfoTab::~CAnimMeshInfoTab()
{
	m_pEditObj = nullptr;
}

void CAnimMeshInfoTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SCALE_X, m_fScaleX);
	DDX_Text(pDX, IDC_EDIT_SCALE_Y, m_fScaleY);
	DDX_Text(pDX, IDC_EDIT_SCALE_Z, m_fScaleZ);
	DDX_Text(pDX, IDC_EDIT_ROTATION_X, m_fRotationX);
	DDX_Text(pDX, IDC_EDIT_ROTATION_Y, m_fRotationY);
	DDX_Text(pDX, IDC_EDIT_ROTATION_Z, m_fRotationZ);
	DDX_Text(pDX, IDC_EDIT_POSITION_X, m_fPositionX);
	DDX_Text(pDX, IDC_EDIT_POSITION_Y, m_fPositionY);
	DDX_Text(pDX, IDC_EDIT_POSITION_Z, m_fPositionZ);
	DDX_Text(pDX, IDC_EDIT_NAME_ARM, m_armObjName);
	DDX_Control(pDX, IDC_SLIDER_ARM_ROT_X, m_sliderCtrlArmRotX);
	DDX_Control(pDX, IDC_SLIDER_ARM_ROT_Y, m_sliderCtrlArmRotY);
	DDX_Control(pDX, IDC_SLIDER_ARM_ROT_Z, m_sliderCtrlArmRotZ);
	DDX_Text(pDX, IDC_EDIT_ARM_SPEED, m_fSpeed);
	DDX_Radio(pDX, IDC_RADIO_ARM_AXIS_1, (int&)m_iRadioAxisType);
	DDX_Text(pDX, IDC_EDIT_MESH_PIVOT, m_fPivot);
}


BEGIN_MESSAGE_MAP(CAnimMeshInfoTab, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_MODIFY_LOCAL_INFO, &CAnimMeshInfoTab::OnBnClickedButtonModifyLocalInfo)
	ON_BN_CLICKED(IDC_BUTTON_CREATE_ARM_OBJ, &CAnimMeshInfoTab::OnBnClickedButtonCreateArmObj)
	ON_BN_CLICKED(IDC_BUTTON_ARM_UP, &CAnimMeshInfoTab::OnBnClickedButtonArmUp)
	ON_BN_CLICKED(IDC_BUTTON_ARM_DOWN, &CAnimMeshInfoTab::OnBnClickedButtonArmDown)
	ON_BN_CLICKED(IDC_BUTTON_SPEED_MODIFY, &CAnimMeshInfoTab::OnBnClickedButtonSpeedModify)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_ARM, &CAnimMeshInfoTab::OnBnClickedButtonSaveArm)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_ARM, &CAnimMeshInfoTab::OnBnClickedButtonLoadArm)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_MESH, &CAnimMeshInfoTab::OnBnClickedButtonSaveMesh)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_MESH, &CAnimMeshInfoTab::OnBnClickedButtonLoadMesh)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_ANIMATION, &CAnimMeshInfoTab::OnBnClickedButtonSaveAnimation)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_ANIMATION, &CAnimMeshInfoTab::OnBnClickedButtonLoadAnimation)
	ON_BN_CLICKED(IDC_BUTTON_ARM_ATTACH_BONE, &CAnimMeshInfoTab::OnBnClickedButtonArmAttachBone)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_LOCAL_INFO, &CAnimMeshInfoTab::OnBnClickedButtonSaveLocalInfo)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_LOCAL_INFO, &CAnimMeshInfoTab::OnBnClickedButtonLoadLocalLoad)
END_MESSAGE_MAP()


// CAnimMeshInfoTab 메시지 처리기


void CAnimMeshInfoTab::SetMeshInfo()
{
	if (!m_pEditObj)
		return;

	CTransform* pTr = m_pEditObj->GetTransform();

	m_fScaleX = pTr->GetLocalScale().x;
	m_fScaleY = pTr->GetLocalScale().y;
	m_fScaleZ = pTr->GetLocalScale().z;

	m_fRotationX = XMConvertToDegrees(pTr->GetLocalRot().x);
	m_fRotationY = XMConvertToDegrees(pTr->GetLocalRot().y);
	m_fRotationZ = XMConvertToDegrees(pTr->GetLocalRot().z);

	m_fPositionX = pTr->GetLocalPos().x;
	m_fPositionY = pTr->GetLocalPos().y;
	m_fPositionZ = pTr->GetLocalPos().z;

	m_fPivot = pTr->GetPivot().y;

	UpdateData(FALSE);

	SAFE_RELEASE(pTr);
}

void CAnimMeshInfoTab::OnBnClickedButtonModifyLocalInfo()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!m_pEditObj)
		return;

	UpdateData(TRUE);

	CTransform* pTr = m_pEditObj->GetTransform();

	pTr->SetLocalScale(m_fScaleX, m_fScaleY, m_fScaleZ);
	
	// 라디안값으로 변경 필요
	pTr->SetLocalRot(
		XMConvertToRadians(m_fRotationX),
		XMConvertToRadians(m_fRotationY),
		XMConvertToRadians(m_fRotationZ));
	pTr->SetLocalPos(m_fPositionX, m_fPositionY, m_fPositionZ);
	pTr->SetPivot(0.f, m_fPivot, 0.f);

	SAFE_RELEASE(pTr);
}


void CAnimMeshInfoTab::OnBnClickedButtonCreateArmObj()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CAnimMeshInfoTab::OnBnClickedButtonArmAttachBone()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CAnimMeshInfoTab::OnRadioAnimTypeCheck(UINT id)
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


void CAnimMeshInfoTab::OnBnClickedButtonArmUp()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CAnimMeshInfoTab::OnBnClickedButtonArmDown()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CAnimMeshInfoTab::OnBnClickedButtonSpeedModify()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CAnimMeshInfoTab::OnBnClickedButtonSaveArm()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CAnimMeshInfoTab::OnBnClickedButtonLoadArm()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CAnimMeshInfoTab::OnBnClickedButtonSaveMesh()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!m_pEditObj)
	{
		AfxMessageBox(L"No Mesh to save!");
		return;
	}

	wchar_t	strFilter[] = L"MESHFile(*.msh)|*.msh|모든파일(*.*)|*.*|||";
	CFileDialog	dlg(FALSE, L".MSH", L"Mesh",
		OFN_OVERWRITEPROMPT, strFilter);

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

	CRenderer* pRenderer = m_pEditObj->FindComponentFromType<CRenderer>(CT_RENDERER);
	CMesh*	   pMesh = pRenderer->GetMesh();
	pMesh->SaveFromFullPath(path);

	SAFE_RELEASE(pMesh);
	SAFE_RELEASE(pRenderer);
}


void CAnimMeshInfoTab::OnBnClickedButtonLoadMesh()
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

	((CMainFrame*)AfxGetMainWnd())->GetEdit()->MeshLoadFromMeshInfoTab(path, name);
}


void CAnimMeshInfoTab::OnBnClickedButtonSaveAnimation()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!m_pEditObj)
	{
		AfxMessageBox(L"No Animation to save!");
		return;
	}

	wchar_t	strFilter[] = L"AnimationFile(*.anm)|*.anm|모든파일(*.*)|*.*|||";
	CFileDialog	dlg(FALSE, L".ANM", L"Animation",
		OFN_OVERWRITEPROMPT, strFilter);

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

	CAnimation*	pAnimation = m_pEditObj->FindComponentFromType<CAnimation>(CT_ANIMATION);
	pAnimation->SaveFromFullPath(path);
	SAFE_RELEASE(pAnimation);
}


void CAnimMeshInfoTab::OnBnClickedButtonLoadAnimation()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	wchar_t	strFilter[] = L"AnimationFile(*.anm)|*.anm|모든파일(*.*)|*.*|||";
	CFileDialog	dlg(TRUE, L".ANM", L"Animation",
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

	((CMainFrame*)AfxGetMainWnd())->GetEdit()->AnimationLoadFromMeshInfoTab(path, name);
}


void CAnimMeshInfoTab::OnBnClickedButtonSaveLocalInfo()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!m_pEditObj)
	{
		AfxMessageBox(L"No Mesh to save!");
		return;
	}

	wchar_t	strFilter[] = L"DATAFile(*.dat)|*.dat|모든파일(*.*)|*.*|||";
	CFileDialog	dlg(FALSE, L".DAT", L"Data",
		OFN_OVERWRITEPROMPT, strFilter);

	// 경로 지정
	wchar_t wstrPath[MAX_PATH] = {};
	wcscpy_s(wstrPath, MAX_PATH, GET_SINGLE(CPathManager)->FindPath(MESH_PATH));

	CString originPath = wstrPath;

	dlg.m_ofn.lpstrInitialDir = wstrPath;

	// do modal error 해결
	if (dlg.DoModal() != IDOK)
		return;

	CString path = dlg.GetPathName();
	CString name = dlg.GetFileTitle();
	
	FILE* pFile = nullptr;

	char	strPath[MAX_PATH] = {};
	WideCharToMultiByte(CP_ACP, 0, path, -1,
		strPath, lstrlen(path), 0, 0);

	fopen_s(&pFile, strPath, "wb");
	
	if (!pFile)
		return;

	CTransform* pTr = m_pEditObj->GetTransform();
	pTr->Save_Local(pFile);

	SAFE_RELEASE(pTr);

	fclose(pFile);
}


void CAnimMeshInfoTab::OnBnClickedButtonLoadLocalLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	wchar_t	strFilter[] = L"DATAFile(*.dat)|*.dat|모든파일(*.*)|*.*|||";
	CFileDialog	dlg(TRUE, L".DAT", L"Data",
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

	if (m_pEditObj)
	{
		FILE* pFile = nullptr;

		char	strPath[MAX_PATH] = {};
		WideCharToMultiByte(CP_ACP, 0, path, -1,
			strPath, lstrlen(path), 0, 0);

		fopen_s(&pFile, strPath, "rb");

		if (!pFile)
			return;

		CTransform* pTr = m_pEditObj->GetTransform();
		pTr->Load_Local(pFile);
		SAFE_RELEASE(pTr);

		SetMeshInfo();

		fclose(pFile);
	}
}
