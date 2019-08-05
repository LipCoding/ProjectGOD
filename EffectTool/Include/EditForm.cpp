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
	ON_BN_CLICKED(IDC_BUTTON_EFFECT_SAVE, &CEditForm::OnBnClickedButtonEffectSave)
	ON_BN_CLICKED(IDC_BUTTON_EFFECT_LOAd, &CEditForm::OnBnClickedButtonEffectLoad)
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

void CEditForm::CloneTarget()
{
	if (!m_pCurEffect)
	{
		AfxMessageBox(L"Choose Effect first!");
		return;
	}

	EFFECTDATA *cloneEffect = new EFFECTDATA;

	CScene *pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
	CLayer *pLayer = pScene->GetLayer("Effect");

	cloneEffect->strName = "EffectObj_" + to_string(m_iEffectNumber);

	/* Clone */
	CColliderSphere *pCollider = m_pCurEffect->pObject->FindComponentFromType<CColliderSphere>(CT_COLLIDER);
	pCollider->AddRef();
	SAFE_RELEASE(pCollider);

	CGameObject*	pClone = m_pCurEffect->pObject->Clone();
	pClone->SetTag(cloneEffect->strName);
	if (pLayer)
		pLayer->AddObject(pClone);
	CGameObject::AddObjList(pClone);

	cloneEffect->pObject = pClone;

	//cloneEffect->pObject->SetTag(cloneEffect->strName);

	cloneEffect->pEffect = cloneEffect->pObject->FindComponentFromTag<CEffect>("Effect");
	
	cloneEffect->pTr = cloneEffect->pObject->GetTransform();

	m_vecEffect.push_back(cloneEffect);

	/* list box 추가 */
	CString EffectName = (CString)cloneEffect->strName.c_str();
	m_listEffectList.AddString(EffectName);

	++m_iEffectNumber;
	SAFE_RELEASE(pLayer);
	SAFE_RELEASE(pScene);
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

	wcscat_s(strPath, MAX_PATH, L"Effect_Texture\\");

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
	CFileDialog	dlg(TRUE, L"*.msh", L"Mesh",
		OFN_HIDEREADONLY, strFilter);

	// 경로 지정
	wchar_t strPath[MAX_PATH] = {};
	wcscpy_s(strPath, MAX_PATH, GET_SINGLE(CPathManager)->FindPath(MESH_PATH));

	wcscat_s(strPath, MAX_PATH, L"97.Effect_Mesh\\");

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
	CLayer *pLayer = pScene->GetLayer("Effect");

	EFFECTDATA *pData = new EFFECTDATA;

	// Create Object
	pData->strName = "EffectObj_" + to_string(m_iEffectNumber);
	pData->pObject = CGameObject::CreateObject(pData->strName, pLayer);

	// Transform
	pData->pTr = pData->pObject->GetTransform();
	pData->pTr->SetWorldPos(50.f / 2.f, 0.f, 50.f / 2.f);
	pData->pEffect = pData->pObject->AddComponent<CEffect>("Effect");
	pData->pEffect->SetEffectName(pData->strName);

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
			if (pColl)
			{
				pColl->SetColliderRenderCheck(true);
				SAFE_RELEASE(pColl);
			}

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


void CEditForm::OnBnClickedButtonEffectSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	static TCHAR BASED_CODE szFilter[] =
		_T("데이터 파일(*.bin) | *.bin;|모든파일(*.*)|*.*||");
	CFileDialog dlg(FALSE, NULL, NULL, OFN_OVERWRITEPROMPT, szFilter);

	// 경로 지정
	wchar_t strPath[MAX_PATH] = {};
	wcscpy_s(strPath, MAX_PATH, GET_SINGLE(CPathManager)->FindPath(DATA_PATH));
	wcscat_s(strPath, MAX_PATH, L"Effect\\");

	CString originPath = strPath;

	dlg.m_ofn.lpstrInitialDir = strPath;

	// do modal error 해결
	if (dlg.DoModal() != IDOK)
		return;

	CString path = dlg.GetPathName();
	CString fileName = dlg.GetFileName();
	CString flexiblePath = L"Effect\\";

	// 파일 이름 제거
	for (int i = lstrlen(path) - 1; i >= 0; i--)
	{
		if (path[i] == '\\')
		{
			path.Delete(i + 1, lstrlen(path) - 1);
			break;
		}
	}

	// 확장자명 제거(만약 확장자명이 세이브 이름으로 들어갈시)
	for (int i = lstrlen(fileName) - 1; i >= 0; i--)
	{
		if (fileName[i] == '.')
		{
			fileName.Delete(i, lstrlen(fileName) - 1);
			break;
		}
	}

	string strFilePath = (string)CT2CA(path);
	string strFileName = (string)CT2CA(fileName);

	ofstream mainFile;
	mainFile.open(strFilePath + strFileName + ".bin", ios::out | ios::trunc);

	// Effect 갯수 저장
	int iCount = (int)m_vecEffect.size();
	mainFile << iCount << endl;

	for (int i = 0; i < iCount; ++i)
	{
		// Mesh
		mainFile << m_vecEffect[i]->pEffect->GetMeshPath() << endl;
		// Local
		mainFile << m_vecEffect[i]->pEffect->GetLocalPath() << endl;
		// Texture
		mainFile << m_vecEffect[i]->pEffect->GetTexturePath() << endl;

		// World 정보
		// scale, rot , (pos는 더해줘야 할듯)
		CTransform *pTr = m_vecEffect[i]->pObject->GetTransform();
		Vector3 scale = pTr->GetWorldScale();
		Vector3 rot = pTr->GetWorldRot();
		Vector3 pos = pTr->GetWorldPos();

		pos.x -= 50.f / 2.f;
		pos.z -= 50.f / 2.f;

		mainFile << pos.x << ' ' << pos.y << ' ' << pos.z << endl;
		mainFile << scale.x << ' ' << scale.y << ' ' << scale.z << endl;
		mainFile << XMConvertToDegrees(rot.x) << ' '
				 << XMConvertToDegrees(rot.y) << ' '
				 << XMConvertToDegrees(rot.z) << endl;	

		SAFE_RELEASE(pTr);

		// Effect 정보
		mainFile << m_vecEffect[i]->pEffect->GetMainStartTime() << endl;
		mainFile << m_vecEffect[i]->pEffect->GetMainEndTime() << endl;

		mainFile << (int)m_vecEffect[i]->pEffect->GetInfiniteMainCheck() << endl;

		// Effect Assist 정보

		// Assist 갯수 저장
		vector<CEffectAssist*>* pVecAssist = m_vecEffect[i]->pEffect->GetAssists();

		int iAssistCount = (int)pVecAssist->size();
		mainFile << iAssistCount << endl;
		//type, easetype, start, end, power, degree, num, ani
		for (int j = 0; j < iAssistCount; ++j)
		{
			//type
			/*if ((int)(*pVecAssist)[j]->GetType() >= 2)
			{
				if ((int)(*pVecAssist)[j]->GetType() == CEffectAssist::ASSIST_POS)
					mainFile << 6 << endl;
				else
					mainFile << (int)(*pVecAssist)[j]->GetType() - 1 << endl;
			}	
			else*/
			mainFile << (int)(*pVecAssist)[j]->GetType() << endl;

			//easetype
			mainFile << (int)(*pVecAssist)[j]->GetEaseType() << endl;
			//start
			mainFile << (*pVecAssist)[j]->GetStartTime() << endl;
			//end
			mainFile << (*pVecAssist)[j]->GetEndTime() << endl;
			//power
			mainFile << (*pVecAssist)[j]->GetPowerX() << ' '
					 << (*pVecAssist)[j]->GetPowerY() << ' '
					 << (*pVecAssist)[j]->GetPowerZ() << endl;
			//degree
			mainFile << (*pVecAssist)[j]->GetDegree() << endl;

			//num (sprite)
			mainFile << (int)(*pVecAssist)[j]->GetSpriteType() << endl;
			mainFile << (*pVecAssist)[j]->GetNum() << endl;
			mainFile << (*pVecAssist)[j]->GetWidth() << endl;
			mainFile << (*pVecAssist)[j]->GetHeight() << endl;
			mainFile << (*pVecAssist)[j]->GetMaxX() << endl;
			mainFile << (*pVecAssist)[j]->GetMaxY() << endl;
			//ani
			mainFile << (*pVecAssist)[j]->GetMoveUV_X() << endl;
			mainFile << (*pVecAssist)[j]->GetMoveUV_Y() << endl;
			//infinite
			mainFile << (int)(*pVecAssist)[j]->GetInifiniteCheck() << endl;
		}

		int iBillbordCheck = 0;
		CBillboard *pBillboard = m_vecEffect[i]->pObject->FindComponentFromTag<CBillboard>("Billboard");

		if (pBillboard)
		{
			if (pBillboard->GetOperateBillboard())
				iBillbordCheck = 1;
			else
				iBillbordCheck = 0;

			SAFE_RELEASE(pBillboard);
		}
		else
		{
			iBillbordCheck = 0;
		}
		
		mainFile << iBillbordCheck << endl;
	}

	mainFile.close();
}


void CEditForm::OnBnClickedButtonEffectLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	static TCHAR BASED_CODE szFilter[] =
		_T("데이터 파일(*.bin) | *.bin;|모든파일(*.*)|*.*||");
	CFileDialog dlg(TRUE, NULL, NULL, OFN_READONLY | OFN_OVERWRITEPROMPT, szFilter);

	// 경로 지정
	wchar_t strPath[MAX_PATH] = {};
	wcscpy_s(strPath, MAX_PATH, GET_SINGLE(CPathManager)->FindPath(DATA_PATH));
	wcscat_s(strPath, MAX_PATH, L"Effect\\");

	CString originPath = strPath;

	dlg.m_ofn.lpstrInitialDir = strPath;

	// do modal error 해결
	if (dlg.DoModal() != IDOK)
		return;

	CString path = dlg.GetPathName();
	CString fileName = dlg.GetFileName();
	CString flexiblePath = L"Effect\\";

	// 확장자명 제거(만약 확장자명이 세이브 이름으로 들어갈시)
	for (int i = lstrlen(path) - 1; i >= 0; i--)
	{
		if (path[i] == '.')
		{
			fileName.Delete(i, lstrlen(path) - 1);
			break;
		}
	}

	string strFilePath = (string)CT2CA(path);
	string strFileName = (string)CT2CA(fileName);

	ifstream mainFile;
	mainFile.open(strFilePath, ios::in);
	
	if (!mainFile.is_open())
		return;

	// Effect 갯수 
	int iCount = 0;
	mainFile >> iCount;

	CScene* pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
	CLayer* pLayer = pScene->GetLayer("Effect");

	for (int i = 0; i < iCount; ++i)
	{
		// Create Object
		EFFECTDATA *pData = new EFFECTDATA;

		pData->strName = "EffectObj_" + to_string(m_iEffectNumber);
		pData->pObject = CGameObject::CreateObject(pData->strName, pLayer);

		pData->pEffect = pData->pObject->AddComponent<CEffect>("Effect");

		// Effect 정보
		string meshPath = GET_SINGLE(CPathManager)->FindPathToMultiByte(MESH_PATH);
		string texPath = GET_SINGLE(CPathManager)->FindPathToMultiByte(TEXTURE_PATH);

		// Mesh
		string meshName, meshFilePath;
		mainFile >> meshName;
		meshFilePath = meshPath + meshName;

		if (!pData->pEffect->LoadEffectMesh(meshFilePath, meshName))
		{
			AfxMessageBox(L"Effect Mesh Create Fail!");
			FreeEffectData(pData);
			SAFE_RELEASE(pLayer);
			SAFE_RELEASE(pScene);
			return;
		}

		// Local
		string localFilePath;
		mainFile >> localFilePath;
		localFilePath = meshPath + localFilePath;

		if (!pData->pEffect->LoadEffectLocalInfo(localFilePath))
		{
			AfxMessageBox(L"Effect Mesh Local Information Create Fail!");
			FreeEffectData(pData);
			SAFE_RELEASE(pLayer);
			SAFE_RELEASE(pScene);
			return;
		}

		// Texture
		string texName, texFilePath;
		mainFile >> texName;
		texFilePath = texPath + texName + ".png";

		pData->pEffect->SetEffectTexture(texName, texFilePath);

		// scale, rot , (pos는 더해줘야 할듯)
		Vector3 worldPos, worldScale, worldRot;

		mainFile >> worldPos.x >> worldPos.y >> worldPos.z;
		mainFile >> worldScale.x >> worldScale.y >> worldScale.z;
		mainFile >> worldRot.x >> worldRot.y >> worldRot.z;
	
		worldRot.x = XMConvertToRadians(worldRot.x);
		worldRot.y = XMConvertToRadians(worldRot.y);
		worldRot.z = XMConvertToRadians(worldRot.z);

		// Transform
		pData->pTr = pData->pObject->GetTransform();
		pData->pTr->SetWorldPos(50.f / 2.f + worldPos.x, 0.f + worldPos.y, 50.f / 2.f + worldPos.z);
		pData->pTr->SetWorldScale(worldScale.x, worldScale.y, worldScale.z);
		pData->pTr->SetWorldRot(worldRot.x, worldRot.y, worldRot.z);

		// Collider
		if (!pData->pEffect->CreateEffectCollider())
		{
			AfxMessageBox(L"Collider Create Fail!");
			FreeEffectData(pData);
			SAFE_RELEASE(pLayer);
			SAFE_RELEASE(pScene);
			return;
		}

		// Effect 정보
		float fMainStartTime, fMainEndTime = 0.f;
		mainFile >> fMainStartTime;
		mainFile >> fMainEndTime;

		pData->pEffect->SetMainStartTime(fMainStartTime);
		pData->pEffect->SetMainEndTime(fMainEndTime);

		int temp = 0;
		bool mainInfiniteCheck = false;
		mainFile >> temp;

		if (0 != temp)
			mainInfiniteCheck = true;

		pData->pEffect->SetInfiniteMainCheck(mainInfiniteCheck);

		// Effect Assist 정보
		// Assist 갯수 
		int iAssistCount = 0;
		mainFile >> iAssistCount;
	
		for (int j = 0; j < iAssistCount; ++j)
		{
			CEffectAssist::ASSIST_TYPE eType;
			CEffectAssist::EASE_TYPE eEaseType;
			
			int type = 0;
			mainFile >> type;
			eType = (CEffectAssist::ASSIST_TYPE)type;

			mainFile >> type;
			eEaseType = (CEffectAssist::EASE_TYPE)type;

			float fStart = 0.f;
			mainFile >> fStart;

			float fEnd = 0.f;
			mainFile >> fEnd;

			float fPowX, fPowY, fPowZ = 0.f;
			mainFile >> fPowX >> fPowY >> fPowZ;

			float fDegree = 0.f;
			mainFile >> fDegree;

			CEffectAssist::SPRITE_TYPE eSpriteType;
			mainFile >> type;
			eSpriteType = (CEffectAssist::SPRITE_TYPE)type;

			int iNum = 0;
			mainFile >> iNum;
			
			int iWidth, iHeight, iMaxX, iMaxY = 0;
			mainFile >> iWidth;
			mainFile >> iHeight;
			mainFile >> iMaxX;
			mainFile >> iMaxY;

			float fAniX, fAniY = 0.f;
			mainFile >> fAniX;
			mainFile >> fAniY;

			int temp_other = 0;
			bool assistInfiniteCheck = false;
			mainFile >> temp_other;

			if (0 != temp_other)
				assistInfiniteCheck = true;

			switch (eType)
			{
			case CEffectAssist::ASSIST_SCALE:
				pData->pEffect->AddPatternScale(eEaseType, fStart, fEnd, fPowX, fPowY, fPowZ, 1);
				break;
			case CEffectAssist::ASSIST_ROT:
				pData->pEffect->AddPatternRotation(eEaseType, fStart, fEnd, fPowX, fPowY, fPowZ, 1);
				break;
			case CEffectAssist::ASSIST_FADE_IN:
				pData->pEffect->AddFadeIn(fStart, fEnd, fDegree);
				break;
			case CEffectAssist::ASSIST_FADE_OUT:
				pData->pEffect->AddFadeOut(fStart, fEnd, fDegree);
				break;
			case CEffectAssist::ASSIST_UV_ANI:
				if(CEffectAssist::SPRITE_FRAME == eSpriteType)
					pData->pEffect->AddUVAnimation(fStart, fEnd, iNum, 1);
				else if(CEffectAssist::SPRITE_ATLAS == eSpriteType)
					pData->pEffect->AddUVAnimation(fStart, fEnd, iMaxX, iMaxY, iWidth, iHeight, 1);
				break;
			case CEffectAssist::ASSIST_UV_MOVE:
				pData->pEffect->AddUVMovement(fStart, fEnd, fAniX, fAniY);
				break;
			case CEffectAssist::ASSIST_POS:
				pData->pEffect->AddPatternPosition(eEaseType, fStart, fEnd, fPowX, fPowY, fPowZ, 1);
				break;
			default:
				break;
			}

			pData->pEffect->SetInfiniteCheckAssistEffectFromType(eType, assistInfiniteCheck);
		}

		int iBillbordCheck = 0;
		mainFile >> iBillbordCheck;
		
		if (iBillbordCheck)
		{
			CBillboard *pBillboard = pData->pObject->AddComponent<CBillboard>("Billboard");
			SAFE_RELEASE(pBillboard);
		}

		m_vecEffect.push_back(pData);

		/* list box 추가 */
		CString EffectName = (CString)pData->strName.c_str();
		m_listEffectList.AddString(EffectName);

		++m_iEffectNumber;
	}

	mainFile.close();

	SAFE_RELEASE(pLayer);
	SAFE_RELEASE(pScene);
}


BOOL CEditForm::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE ||
			pMsg->wParam == VK_RETURN)
			return TRUE;
	}

	return CFormView::PreTranslateMessage(pMsg);
}
