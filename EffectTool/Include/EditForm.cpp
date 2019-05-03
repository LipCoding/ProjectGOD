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
#include "Component/ColliderSphere.h"
#include "Component/Renderer.h"
#include "Resources/Mesh.h"
#include "Component/Material.h"
#include "Component/Billboard.h"

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
	ON_BN_CLICKED(IDC_BUTTON_LOAD_PARTICLE, &CEditForm::OnBnClickedButtonLoadParticle)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_MESH_TEXTURE, &CEditForm::OnBnClickedButtonLoadMeshTexture)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_MESH, &CEditForm::OnBnClickedButtonLoadMesh)
	ON_BN_CLICKED(IDC_CHECK_BILLBOARD, &CEditForm::OnBnClickedCheckBillboard)
	ON_LBN_SELCHANGE(IDC_LIST_EFFECT_CONTAINER, &CEditForm::OnLbnSelchangeListEffectContainer)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CEditForm::OnBnClickedButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR_ALL, &CEditForm::OnBnClickedButtonClearAll)
END_MESSAGE_MAP()


void CEditForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_EFFECT, m_Tab);
	DDX_Control(pDX, IDC_LIST_EFFECT_CONTAINER, m_listEffectList);
	DDX_Control(pDX, IDC_CHECK_BILLBOARD, m_checkBillBoard);
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

	m_checkBillBoard.SetCheck(0);
}

void CEditForm::UpdateForm()
{
	m_pEffectDlg->UpdateForm();
	m_pEffect1Dlg->UpdateForm();
}

void CEditForm::UpdateTarget(class CGameObject* object)
{
	/* Billboard */
	CBillboard *pBillboard = object->FindComponentFromTag<CBillboard>("Billboard");
	if (pBillboard)
	{
		if(pBillboard->GetOperateBillboard())
			m_checkBillBoard.SetCheck(1);
		else
			m_checkBillBoard.SetCheck(0);
		SAFE_RELEASE(pBillboard);
	}
	else
	{
		m_checkBillBoard.SetCheck(0);
	}
}

void CEditForm::FreeEffectData(EFFECTDATA* effect)
{
	if (effect)
	{
		SAFE_RELEASE(effect->pEffect);
		SAFE_RELEASE(effect->pTr);

		if (effect->pObject)
			effect->pObject->Die();

		CGameObject::EraseObj(effect->pObject);
		SAFE_RELEASE(effect->pObject);
	}

	SAFE_DELETE(effect);
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

/* Mesh */
void CEditForm::OnBnClickedButtonLoadMeshTexture()
{
	if (!m_pCurEffect)
	{
		AfxMessageBox(L"You didn't choose mesh. Create Mesh! OR Click the Mesh Correctly!");
		return;
	}

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	static TCHAR BASED_CODE szFilter[] =
		_T("이미지 파일(*.BMP,*.GIF,*.JPG, *.TGA, *.DDS, *.PNG) | *.BMP,*.GIF,*.JPG,*.TGA,*.DDS;*.bmp;*.gif;*.jpg;*.tga;*.dds;*.png|모든파일(*.*)|*.*||");
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

	m_pCurEffect->pEffect->SetEffectTexture((string)CT2CA(name), (string)CT2CA(path));
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

	EFFECTDATA *pData = new EFFECTDATA;

	// Create Object
	pData->strName = "EffectObj_" + to_string(m_iEffectNumber);
	pData->pObject = CGameObject::CreateObject(pData->strName, pLayer);

	// Transform
	pData->pTr = pData->pObject->GetTransform();
	pData->pTr->SetWorldPos(50.f / 2.f, 0.f, 50.f / 2.f);
	pData->pEffect = pData->pObject->AddComponent<CEffect>("Effect");

	if (!pData->pEffect->LoadEffectMesh( (string)CT2CA(fullPath), (string)CT2CA(name) ))
	{
		AfxMessageBox(L"Effect Mesh Create Fail!");
		FreeEffectData(pData);
		SAFE_RELEASE(pLayer);
		SAFE_RELEASE(pScene);
		return;
	}

	if (!pData->pEffect->LoadEffectLocalInfo( (string)CT2CA(fullPath) ))
	{
		AfxMessageBox(L"Effect Mesh Local Information Create Fail!");
		FreeEffectData(pData);
		SAFE_RELEASE(pLayer);
		SAFE_RELEASE(pScene);
		return;
	}

	if (!pData->pEffect->CreateEffectCollider())
	{
		AfxMessageBox(L"Collider Create Fail!");
		FreeEffectData(pData);
		SAFE_RELEASE(pLayer);
		SAFE_RELEASE(pScene);
		return;
	}
	
	m_vecEffect.push_back(pData);

	/* list box 추가 */
	CString EffectName = (CString)pData->strName.c_str();
	m_listEffectList.AddString(EffectName);

	++m_iEffectNumber;

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

void CEditForm::OnBnClickedCheckBillboard()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int check = m_checkBillBoard.GetCheck();
	
	CGameObject *pTarget = m_pEffectDlg->GetTargetObject();


	if (check == 1)
	{
		if (pTarget)
		{
			CBillboard *pBillboard = pTarget->FindComponentFromTag<CBillboard>("Billboard");
			if (pBillboard)
			{
				pBillboard->SetOperateBillboard(true);
				SAFE_RELEASE(pBillboard);
			}
			else
			{
				pBillboard = pTarget->AddComponent<CBillboard>("Billboard");
				SAFE_RELEASE(pBillboard);
			}
			m_checkBillBoard.SetCheck(1);
		}
	}
	else
	{
		if (pTarget)
		{
			CBillboard *pBillboard = pTarget->FindComponentFromTag<CBillboard>("Billboard");
			if (pBillboard)
			{
				pBillboard->SetOperateBillboard(false);
				SAFE_RELEASE(pBillboard);
			}
			m_checkBillBoard.SetCheck(0);
		}
	}
}


void CEditForm::OnLbnSelchangeListEffectContainer()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	/* 선택이 안된 Object의 충돌체를 끈다. */
	((CMainFrame*)AfxGetMainWnd())->GetView()->OnRButtonDown(0, CPoint(0, 0));

	int pos = m_listEffectList.GetCurSel();

	CString effectName;
	m_listEffectList.GetText(pos, effectName);

	for (auto& effect : m_vecEffect)
	{
		/* 원하는 Effect를 찾았으니 */
		if (effect->strName == (string)CT2CA(effectName))
		{
			CColliderSphere *pColl = effect->pObject->FindComponentFromType<CColliderSphere>(CT_COLLIDER);
			pColl->SetColliderRenderCheck(true);

			SetTargetEffect(effect);
			((CMainFrame*)AfxGetMainWnd())->GetView()->SetCollideObject(effect->pObject);
			((CMainFrame*)AfxGetMainWnd())->GetEdit()->GetEffectTab()->SetTargetObject(effect->pObject);
			((CMainFrame*)AfxGetMainWnd())->GetEdit()->GetEffect1Tab()->SetTargetObject(effect->pObject);
			((CMainFrame*)AfxGetMainWnd())->GetEdit()->UpdateTarget(effect->pObject);
			break;
		}
	}
}


void CEditForm::OnBnClickedButtonDelete()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int pos = m_listEffectList.GetCurSel();

	if (pos == -1)
	{
		AfxMessageBox(L"Select list first!");
		return;
	}

	CString wEffectName;
	m_listEffectList.GetText(pos, wEffectName);
	string effectName = (string)CT2CA(wEffectName);

	for (auto& effect_iter : m_vecEffect)
	{
		if (effect_iter->strName == effectName)
		{
			auto& erase_iter = find(m_vecEffect.begin(), m_vecEffect.end(), effect_iter);
			FreeEffectData(effect_iter);
			m_vecEffect.erase(erase_iter);
			break;
		}
	}

	m_listEffectList.DeleteString(pos);

	/* Target 초기화 */
	((CMainFrame*)AfxGetMainWnd())->GetView()->OnRButtonDown(0, CPoint(0, 0));
}


void CEditForm::OnBnClickedButtonClearAll()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	for (auto& effect : m_vecEffect)
	{
		FreeEffectData(effect);
	}
	m_vecEffect.clear();

	m_listEffectList.ResetContent();
	m_iEffectNumber = 0;

	/* Target 초기화 */
	((CMainFrame*)AfxGetMainWnd())->GetView()->OnRButtonDown(0, CPoint(0, 0));
}
