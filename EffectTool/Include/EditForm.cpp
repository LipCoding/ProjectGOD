// EditForm.cpp: 구현 파일
//

#include "stdafx.h"
#include "EffectTool.h"
#include "EditForm.h"
#include "afxdialogex.h"

#include "MainFrm.h"
#include "EffectToolView.h"

// Tab
#include "EffectTab.h"
#include "EffectTab1.h"

// EditForm

#include "Core/PathManager.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "Scene/Layer.h"
#include "Component/EffectTexture.h"
#include "Component/ColliderSphere.h"
#include "Component/Renderer.h"
#include "Resources/Mesh.h"
#include "Component/Material.h"

// CEditForm 대화 상자

IMPLEMENT_DYNCREATE(CEditForm, CFormView)

CEditForm::CEditForm()
	: CFormView(IDD_DIALOG_FORM)
	, m_pView(nullptr)
	, m_eTabType(TAB_END)
{
}

CEditForm::~CEditForm()
{
	m_pView = nullptr;
	SAFE_DELETE(m_pEffectDlg)
	SAFE_DELETE(m_pEffect1Dlg)
	Safe_Delete_VecList(m_vecEffect);
}

BEGIN_MESSAGE_MAP(CEditForm, CView)
	ON_WM_VSCROLL()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_EFFECT, &CEditForm::OnTcnSelchangeTabEffect)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_TEXTURE, &CEditForm::OnBnClickedButtonLoadTexture)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_PARTICLE, &CEditForm::OnBnClickedButtonLoadParticle)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_MESH_TEXTURE, &CEditForm::OnBnClickedButtonLoadMeshTexture)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_MESH, &CEditForm::OnBnClickedButtonLoadMesh)
END_MESSAGE_MAP()


void CEditForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_EFFECT, m_Tab);
	DDX_Control(pDX, IDC_LIST_EFFECT_CONTAINER, m_listEffectList);
}

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


void CEditForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	
	// 스크롤바
	this->EnableScrollBarCtrl(SB_VERT, TRUE);
	SCROLLINFO si;
	si.fMask = SIF_PAGE | SIF_RANGE;
	si.nPage  = 10;
	si.nMin = 0;
	si.nMax = 500;
	this->SetScrollInfo(SB_VERT, &si, TRUE);

	m_eTabType = TAB_EFFECT;

	m_Tab.InsertItem(0, _T("Info/Pattern"));
	m_Tab.InsertItem(1, _T("Fade/UV"));
	m_Tab.InsertItem(2, _T("Particle"));

	CRect rect;
	m_Tab.GetWindowRect(&rect);

	m_Tab.SetCurSel(0);

	m_pEffectDlg = new CEffectTab;
	m_pEffectDlg->Create(IDD_DIALOG1, &m_Tab);
	m_pEffectDlg->MoveWindow(0, 20, rect.Width(), rect.Height());
	m_pEffectDlg->ShowWindow(SW_SHOW);

	m_pEffect1Dlg = new CEffectTab1;
	m_pEffect1Dlg->Create(IDD_DIALOG2, &m_Tab);
	m_pEffect1Dlg->MoveWindow(0, 20, rect.Width(), rect.Height());
	m_pEffect1Dlg->ShowWindow(SW_HIDE);

	// 메인 프레임을 받아온다.
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	m_pView = (CEnvToolView*)pMain->GetActiveView();

	m_eTabType = (TOOLTAB_TYPE)m_Tab.GetCurSel();
}

void CEditForm::UpdateForm()
{
	m_pEffectDlg->UpdateForm();
}

bool CEditForm::LoadEffectMesh(CGameObject * object, const CString & filePath, const CString& fileName)
{
	CString strTag = filePath + ".msh";

	if (!object)
		return false;

	CRenderer *pRenderer = object->AddComponent<CRenderer>("Renderer");

	pRenderer->SetMeshFromFullPath((string)CT2CA(fileName), strTag.GetString());

	CMesh *pMesh = pRenderer->GetMesh();

	if (pMesh == nullptr)
	{
		SAFE_RELEASE(pRenderer);
		return false;
	}

	SAFE_RELEASE(pMesh);
	SAFE_RELEASE(pRenderer);

	return true;
}

bool CEditForm::LoadEffectLocalInfo(CGameObject * object, const CString & filePath)
{
	CString strTag = filePath + ".dat";

	if (!object)
		return false;

	FILE* pFile = nullptr;

	char	strPath[MAX_PATH] = {};
	WideCharToMultiByte(CP_ACP, 0, strTag, -1,
		strPath, lstrlen(strTag), 0, 0);

	fopen_s(&pFile, strPath, "rb");

	if (!pFile)
		return false;

	CTransform* pTr = object->GetTransform();
	pTr->Load_Local(pFile);
	SAFE_RELEASE(pTr);

	//SetMeshInfo();
	fclose(pFile);

	return true;
}

void CEditForm::OnTcnSelchangeTabEffect(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	int sel = m_Tab.GetCurSel();

	switch (sel)
	{
	case 0:
		m_eTabType = TAB_EFFECT;

		m_pEffectDlg->ShowWindow(SW_SHOW);
		m_pEffect1Dlg->ShowWindow(SW_HIDE);
		
		break;
	case 1:
		m_eTabType = TAB_EFFECT1;

		m_pEffectDlg->ShowWindow(SW_HIDE);
		m_pEffect1Dlg->ShowWindow(SW_SHOW);

		break;
	}

	*pResult = 0;
}

/* Texture */
void CEditForm::OnBnClickedButtonLoadTexture()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	static TCHAR BASED_CODE szFilter[] =
		_T("이미지 파일(*.BMP,*.GIF,*.JPG, *.TGA, *.DDS) | *.BMP,*.GIF,*.JPG,*.TGA,*.DDS;*.bmp;*.gif;*.jpg;*.tga;*.dds|모든파일(*.*)|*.*||");
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);

	// 경로 지정
	wchar_t strPath[MAX_PATH] = {};
	wcscpy_s(strPath, MAX_PATH, GET_SINGLE(CPathManager)->FindPath(TEXTURE_PATH));

	CString originPath = strPath;

	wcscat_s(strPath, MAX_PATH, L"Effect_Texture\\");

	dlg.m_ofn.lpstrInitialDir = strPath;

	// do modal error 해결
	if (dlg.DoModal() != IDOK)
		return;

	CString path = dlg.GetPathName();
	CString name = dlg.GetFileTitle();

	EFFECTDATA *pData = new EFFECTDATA;

	pData->strName = "Effect_" + to_string(m_iEffectNumber) + "(Texture)";

	CScene *pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
	CLayer *pLayer = pScene->GetLayer("ParticleLayer");

	pData->pObject = CGameObject::CreateObject(pData->strName, pLayer);
	pData->pTr = pData->pObject->GetTransform();

	pData->pTr->SetWorldScale(2.f, 2.f, 2.f);
	pData->pTr->SetWorldPos(50.f / 2.f, 0.f, 50.f / 2.f);
	pData->pTr->SetWorldRot(0.f, 0.f, 0.f);

	/* add effect texture */
	pData->pEffect = pData->pObject->AddComponent<CEffectTexture>("Effect");
	pData->pEffect->SetEffect();

	/* set texture */
	pData->pEffect->SetTexture((string)CT2CA(path));

	/* Collider */
	CColliderSphere* pCollider = pData->pObject->AddComponent<CColliderSphere>("Collider");
	pCollider->SetSphere(Vector3(0.f, 0.f, 0.f), 5.f);
	pCollider->SetColliderRenderCheck(true);
	SAFE_RELEASE(pCollider);

	pData->eType = EFFECT_TYPE::EFT_TYPE_TEXTURE;
	m_vecEffect.push_back(pData);

	++m_iEffectNumber;

	SAFE_RELEASE(pLayer);
	SAFE_RELEASE(pScene);
}

/* Mesh */
void CEditForm::OnBnClickedButtonLoadMeshTexture()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	static TCHAR BASED_CODE szFilter[] =
		_T("이미지 파일(*.BMP,*.GIF,*.JPG, *.TGA, *.DDS) | *.BMP,*.GIF,*.JPG,*.TGA,*.DDS;*.bmp;*.gif;*.jpg;*.tga;*.dds|모든파일(*.*)|*.*||");
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);

	// 경로 지정
	wchar_t strPath[MAX_PATH] = {};
	wcscpy_s(strPath, MAX_PATH, GET_SINGLE(CPathManager)->FindPath(TEXTURE_PATH));

	CString originPath = strPath;

	wcscat_s(strPath, MAX_PATH, L"Effect_Mesh\\");

	dlg.m_ofn.lpstrInitialDir = strPath;

	// do modal error 해결
	if (dlg.DoModal() != IDOK)
		return;

	CString path = dlg.GetPathName();
	CString name = dlg.GetFileTitle();

	CRenderer *pRenderer = m_pEffect->FindComponentFromType<CRenderer>(CT_RENDERER);
	CMaterial *pMaterial = pRenderer->GetMaterial();

	pRenderer->SetRenderState(ALPHA_BLEND);
	//pRenderer->AlphaEnable(true);

	//pRenderer->SetRenderState(CULLING_NONE);

	string fullPath = CT2CA(path);

	pMaterial->SetDiffuseTexInfoFromFullPath(SAMPLER_LINEAR, (string)CT2CA(name), 0, 0,
		fullPath.c_str());

	SAFE_RELEASE(pMaterial);
	SAFE_RELEASE(pRenderer);
}

void CEditForm::OnBnClickedButtonLoadMesh()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	wchar_t	strFilter[] = L"MESHFile(*.msh)|*.msh|모든파일(*.*)|*.*|||";
	CFileDialog	dlg(TRUE, L".MSH", L"Mesh",
		OFN_HIDEREADONLY, strFilter);

	// 경로 지정
	wchar_t strPath[MAX_PATH] = {};
	wcscpy_s(strPath, MAX_PATH, GET_SINGLE(CPathManager)->FindPath(MESH_PATH));

	wcscat_s(strPath, MAX_PATH, L"Effect_Tool\\");

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

	CScene *pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
	CLayer *pLayer = pScene->GetLayer("Default");

	CGameObject *pObject = CGameObject::CreateObject("EffectObj", pLayer);

	CTransform *pTr = pObject->GetTransform();
	pTr->SetWorldPos(50.f / 2.f, 0.f, 50.f / 2.f);
	SAFE_RELEASE(pTr);

	if (!LoadEffectMesh(pObject, fullPath, name))
	{
		AfxMessageBox(L"Effect Mesh Create Fail!");

		SAFE_RELEASE(pObject);
		SAFE_RELEASE(pLayer);
		SAFE_RELEASE(pScene);
		return;
	}

	if (!LoadEffectLocalInfo(pObject, fullPath))
	{
		AfxMessageBox(L"Effect Mesh Local Information Create Fail!");
		SAFE_RELEASE(pObject);
		SAFE_RELEASE(pLayer);
		SAFE_RELEASE(pScene);
		return;
	}

	/* Collider */
	CRenderer *pRenderer = pObject->FindComponentFromType<CRenderer>(CT_RENDERER);
	CMesh *pMesh = pRenderer->GetMesh();

	pTr = pObject->GetTransform();

	Vector3 vMin, vMax, vCenter;

	vMin = (pMesh->GetMin()).TransformCoord(pTr->GetLocalMatrix().mat);
	vMax = (pMesh->GetMax()).TransformCoord(pTr->GetLocalMatrix().mat);
	vCenter = (pMesh->GetCenter()).TransformCoord(pTr->GetLocalMatrix().mat);

	float fRadius;

	fRadius = pMesh->GetRadius() * pTr->GetLocalScale().x;

	CColliderSphere* pCollider = pObject->AddComponent<CColliderSphere>("Collider");
	pCollider->SetSphere(vCenter, fRadius);
	pCollider->SetColliderRenderCheck(false);
	SAFE_RELEASE(pTr);
	SAFE_RELEASE(pCollider);

	//SAFE_RELEASE(pObject);
	m_pEffect = pObject;
	SAFE_RELEASE(pLayer);
	SAFE_RELEASE(pScene);
}

/* Particle */
void CEditForm::OnBnClickedButtonLoadParticle()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	static TCHAR BASED_CODE szFilter[] =
		_T("이미지 파일(*.BMP,*.GIF,*.JPG, *.TGA, *.DDS) | *.BMP,*.GIF,*.JPG,*.TGA,*.DDS;*.bmp;*.gif;*.jpg;*.tga;*.dds|모든파일(*.*)|*.*||");
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);

	// 경로 지정
	wchar_t strPath[MAX_PATH] = {};
	wcscpy_s(strPath, MAX_PATH, GET_SINGLE(CPathManager)->FindPath(TEXTURE_PATH));

	CString originPath = strPath;

	wcscat_s(strPath, MAX_PATH, L"Effect_Particle\\");

	dlg.m_ofn.lpstrInitialDir = strPath;

	// do modal error 해결
	if (dlg.DoModal() != IDOK)
		return;

	CString path = dlg.GetPathName();
	CString name = dlg.GetFileTitle();
}

