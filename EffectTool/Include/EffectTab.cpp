// EffectTab.cpp: 구현 파일
//

#include "stdafx.h"
#include "EffectTool.h"
#include "EffectTab.h"
#include "afxdialogex.h"

#include "MainFrm.h"

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
	, m_fInfoStaticRotX(0)
	, m_fInfoStaticRotY(0)
	, m_fInfoStaticRotZ(0)
{

}

CEffectTab::~CEffectTab()
{
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
	DDX_Text(pDX, IDC_EDIT_ROT_INFO_STATIC_X, m_fInfoStaticRotX);
	DDX_Text(pDX, IDC_EDIT_ROT_INFO_STATIC_Y, m_fInfoStaticRotY);
	DDX_Text(pDX, IDC_EDIT_ROT_INFO_STATIC_Z, m_fInfoStaticRotZ);
	DDX_Control(pDX, IDC_EDIT_POS_INFO_X, m_editInfoPosX);
	DDX_Control(pDX, IDC_EDIT_POS_INFO_Y, m_editInfoPosY);
	DDX_Control(pDX, IDC_EDIT_POS_INFO_Z, m_editInfoPosZ);
	DDX_Control(pDX, IDC_EDIT_SCALE_INFO_X, m_editInfoScaleX);
	DDX_Control(pDX, IDC_EDIT_SCALE_INFO_Y, m_editInfoScaleY);
	DDX_Control(pDX, IDC_EDIT_SCALE_INFO_Z, m_editInfoScaleZ);
	DDX_Control(pDX, IDC_EDIT_ROT_INFO_X, m_editInfoRotX);
	DDX_Control(pDX, IDC_EDIT_ROT_INFO_Y, m_editInfoRotY);
	DDX_Control(pDX, IDC_EDIT_ROT_INFO_Z, m_editInfoRotZ);
}


BEGIN_MESSAGE_MAP(CEffectTab, CDialogEx)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BUTTON_LOAD_TARGET_OBJ, &CEffectTab::OnBnClickedButtonLoadTargetObj)
	ON_CBN_SELCHANGE(IDC_COMBO_BONE_LIST, &CEffectTab::OnCbnSelchangeComboBoneList)
	ON_CBN_SELCHANGE(IDC_COMBO_ANIM_LIST, &CEffectTab::OnCbnSelchangeComboAnimList)
	ON_BN_CLICKED(IDC_BUTTON_INPUT_INFO, &CEffectTab::OnBnClickedButtonInputInfo)
END_MESSAGE_MAP()


// CEffectTab 메시지 처리기


BOOL CEffectTab::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_fInfoStaticPosX = -1.f;
	m_fInfoStaticPosY = -1.f;
	m_fInfoStaticPosZ = -1.f;

	m_fInfoStaticScaleX = -1.f;
	m_fInfoStaticScaleY = -1.f;
	m_fInfoStaticScaleZ = -1.f;

	m_fInfoStaticRotX = -1.f;
	m_fInfoStaticRotY = -1.f;
	m_fInfoStaticRotZ = -1.f;

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
}

bool CEffectTab::LoadTargetMesh(const CString & filePath)
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

	pRenderer->SetMeshFromFullPath("TargetMesh", strTag.GetString());

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
	
	m_pBoneMatrix = nullptr;
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

	if (!LoadTargetMesh(fullPath))
	{
		AfxMessageBox(L"Target Mesh Create Fail!");
		return;
	}

	if (!LoadTargetLocalInfo(fullPath))
	{
		AfxMessageBox(L"Mesh Local Information Create Fail!");
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
	pCollider->SetColliderRenderCheck(false);
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

		m_fInfoStaticRotX = XMConvertToDegrees(pRot.x);
		m_fInfoStaticRotY = XMConvertToDegrees(pRot.y);
		m_fInfoStaticRotZ = XMConvertToDegrees(pRot.z);

		SAFE_RELEASE(pTr);
	}
	else
	{
		m_fInfoStaticPosX = -1.f;
		m_fInfoStaticPosY = -1.f;
		m_fInfoStaticPosZ = -1.f;

		m_fInfoStaticScaleX = -1.f;
		m_fInfoStaticScaleY = -1.f;
		m_fInfoStaticScaleZ = -1.f;

		m_fInfoStaticRotX = -1.f;
		m_fInfoStaticRotY = -1.f;
		m_fInfoStaticRotZ = -1.f;
		

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

	UpdateData(FALSE);
}

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
		vNewRot.x = m_fInfoStaticRotX;
	}
	else
	{
		vNewRot.x = (float)_wtof(X);
	}

	if (Y == L"")
	{
		vNewRot.y = m_fInfoStaticRotY;
	}
	else
	{
		vNewRot.y = (float)_wtof(Y);
	}

	if (Z == L"")
	{
		vNewRot.z = m_fInfoStaticRotZ;
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
