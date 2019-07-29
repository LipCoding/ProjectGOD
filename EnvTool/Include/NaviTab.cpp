// NaviTab.cpp: 구현 파일
//

#include "stdafx.h"
#include "EnvTool.h"
#include "NaviTab.h"
#include "afxdialogex.h"

#include "GameObject/GameObject.h"
#include "Scene/Scene.h"
#include "Scene/Layer.h"
#include "Scene/SceneManager.h"
#include "Component/Transform.h"
#include "Component/Picking.h"

#include "BrushTool.h"

#include "Core/NaviManager.h"
#include "Core/NaviMesh.h"

#include "Core/PathManager.h"
#include "Core/Input.h"

PG_USING

// CNaviTab 대화 상자

IMPLEMENT_DYNAMIC(CNaviTab, CDialogEx)

CNaviTab::CNaviTab(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG3, pParent)
{

}

CNaviTab::~CNaviTab()
{
}

void CNaviTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_NAVI_POINT, m_listNaviPoint);
	DDX_Control(pDX, IDC_LIST_NAVI_CELL, m_listNaviCell);
	DDX_Control(pDX, IDC_CHECK_NAVI, m_checkBoxNaviOn);
}


BEGIN_MESSAGE_MAP(CNaviTab, CDialogEx)
	ON_BN_CLICKED(IDC_CHECK_NAVI, &CNaviTab::OnBnClickedCheckNavi)
	ON_BN_CLICKED(IDC_BUTTON_NAVI_UNDO, &CNaviTab::OnBnClickedButtonNaviUndo)
	ON_BN_CLICKED(IDC_BUTTON_NAVI_SAVE, &CNaviTab::OnBnClickedButtonNaviSave)
	ON_BN_CLICKED(IDC_BUTTON_NAVI_LOAD, &CNaviTab::OnBnClickedButtonNaviLoad)
	ON_BN_CLICKED(IDC_BUTTON_NAVI_DELETE, &CNaviTab::OnBnClickedButtonNaviDelete)
	ON_BN_CLICKED(IDC_BUTTON_NAVI_CLEARALL, &CNaviTab::OnBnClickedButtonNaviClearall)
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()


// CNaviTab 메시지 처리기


BOOL CNaviTab::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	GET_SINGLE(CNaviManager)->CreateNaviMesh("test");
	GET_SINGLE(CNaviManager)->SetRenderCheck(true);

	m_checkBoxNaviOn.SetCheck(1);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CNaviTab::Set_Brush_Size(const float & size)
{
	m_BrushSize += size;

	CGameObject* pBrushObj = CGameObject::FindObject("Brush");
	CBrushTool* pBrushTool = pBrushObj->FindComponentFromTag<CBrushTool>("BrushTool");
	pBrushTool->SetBrushInformation(m_BrushSize);

	SAFE_RELEASE(pBrushTool);
	SAFE_RELEASE(pBrushObj);
}

void CNaviTab::Process_ChangeTab()
{
	CGameObject* pBrushObj = CGameObject::FindObject("Brush");
	CBrushTool* pBrushTool = pBrushObj->FindComponentFromTag<CBrushTool>("BrushTool");

	pBrushTool->SetBrushCheck(false);
	pBrushTool->SetBrushCheck_Other(false);
	// 한번 Update위한 함수(이렇게 해야 Brush모양이 사라짐)
	pBrushTool->SetBrushInformation(Vector3{ 0.f, 0.f, 0.f });
	pBrushTool->SetBrushInformation_Other(Vector3{ 0.f, 0.f, 0.f });

	SAFE_RELEASE(pBrushTool);
	SAFE_RELEASE(pBrushObj);
}

void CNaviTab::Process_ShowTab()
{
	CGameObject* pBrushObj = CGameObject::FindObject("Brush");
	CBrushTool* pBrushTool = pBrushObj->FindComponentFromTag<CBrushTool>("BrushTool");

	pBrushTool->SetBrushCheck(true);
	pBrushTool->SetBrushCheck_Other(true);
	pBrushTool->SetBrushInformation(m_BrushSize);

	SAFE_RELEASE(pBrushTool);
	SAFE_RELEASE(pBrushObj);
}

void CNaviTab::UpdateForm()
{

}

void CNaviTab::Add_Point(const Vector3 & vPoint)
{
	// Vector
	Vector3 vP = vPoint;
	Find_NearPoint(vP);

	// listBox
	m_vecNaviPoint.push_back(vP);

	if (m_vecNaviPoint.size() == 3)
	{	
		// Clock, ReverseClock Check And Modify To Clock
		Check_Direction();
		GET_SINGLE(CNaviManager)->AddCell(m_vecNaviPoint);

		m_vecNaviPoint.clear();
		m_listNaviPoint.ResetContent();
		m_listNaviCell.AddString(L"Cell_" + CString(to_string(m_NumCell).c_str()));
		m_NumCell++;
	}	
	else
	{
		m_listNaviPoint.AddString(L"Point");
	}
}

void CNaviTab::Find_NearPoint(Vector3 & vPoint)
{
	if (GET_SINGLE(CNaviManager)->IsCellEmpty())
		return;

	float fDist = 0.f;

	for (const auto& cell : *GET_SINGLE(CNaviManager)->GetNaviCells())
	{
		for (int i = 0; i < 3; ++i)
		{
			fDist = fabs((cell->Get_Point(i) - vPoint).Length());

			if (fDist <= m_BrushSize)
			{
				vPoint = cell->Get_Point(i);
				return;
			}
		}
	}
}

void CNaviTab::Check_Direction()
{
	if (m_vecNaviPoint.size() != 3)
		return;

	Vector3 vDirZeroToOne = (m_vecNaviPoint[1] - m_vecNaviPoint[0]).Normalize();
	Vector3 vDirZeroToTwo = (m_vecNaviPoint[2] - m_vecNaviPoint[0]).Normalize();

	CScene* pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
	CCamera* pCamera = pScene->GetMainCamera();
	CTransform* pCamTr = pCamera->GetTransform();

	Vector3 vCamDir = pCamTr->GetWorldAxis(AXIS_Z).Normalize();

	Vector3 vCross = vDirZeroToOne.Cross(vDirZeroToTwo);

	float fDot = vCamDir.Dot(vCross);

	// 시계 반대방향
	if (fDot > 0.f)
	{
		Vector3 vTemp = m_vecNaviPoint[1];

		m_vecNaviPoint[1] = m_vecNaviPoint[2];
		m_vecNaviPoint[2] = vTemp;
	}

	SAFE_RELEASE(pCamTr);
	SAFE_RELEASE(pCamera);
	SAFE_RELEASE(pScene);

	return;

	// 대안코드 (이해 불가)
	/*auto& iter = m_vecNaviPoint.begin();

	Vector3 vCenterPoint = Vector3{ ((*iter).vPosition.x + (*(iter + 1)).vPosition.x + (*(iter + 2)).vPosition.x) / 3,
									((*iter).vPosition.y + (*(iter + 1)).vPosition.y + (*(iter + 2)).vPosition.y) / 3,
									((*iter).vPosition.z + (*(iter + 1)).vPosition.z + (*(iter + 2)).vPosition.z) / 3 };

	Vector3 vDirection[3];
	vDirection[0] = (*(iter + 1)).vPosition - (*iter).vPosition;
	vDirection[1] = (*(iter + 2)).vPosition - (*(iter + 1)).vPosition;
	vDirection[2] = (*iter).vPosition - (*(iter + 2)).vPosition;

	Vector3 vNormal[3];
	vNormal[0] = Vector3{ -vDirection[0].z, vDirection[0].y, vDirection[0].x };
	vNormal[1] = Vector3{ -vDirection[1].z, vDirection[1].y, vDirection[1].x };
	vNormal[2] = Vector3{ -vDirection[2].z, vDirection[2].y, vDirection[2].x };

	for (int i = 0; i < 3; ++i)
		vNormal[i] = vNormal[i].Normalize();

	for (int i = 0; i < 3; ++i)
	{
		Vector3 vTemp = (vCenterPoint - (*(iter + i)).vPosition).Normalize();

		float fDotProduct = vTemp.Dot(vNormal[i]);

		if (fDotProduct > 0.f)
		{
			Vector3 vTmp = (*(iter + 1)).vPosition;

			(*(iter + 1)).vPosition = (*(iter + 2)).vPosition;
			(*(iter + 2)).vPosition = vTmp;

			return;
		}
	}*/
}

void CNaviTab::Pick_NaviMeshCell(const Vector3 & vPos)
{
	if (!GET_SINGLE(CNaviManager)->GetNaviCells())
		return;

	m_pSelectCell = nullptr;

	for (const auto& cell : *GET_SINGLE(CNaviManager)->GetNaviCells())
	{
		UINT iCount = 0;

		for (int i = 0; i < 3; ++i)
		{
			Vector3 vTemp = (vPos - cell->Get_Point(i)).Normalize();

			float fDot = vTemp.Dot(cell->Get_Normal(i));

			// In
			if (fDot < 0.f)
				++iCount;

			if (iCount == 3)
			{
				m_pSelectCell = cell;	
			}
		}
	}

	CGameObject* pBrushObj = CGameObject::FindObject("Brush");
	CBrushTool* pBrushTool = pBrushObj->FindComponentFromTag<CBrushTool>("BrushTool");

	if (m_pSelectCell)
	{
		pBrushTool->SetBrushCheck_Other(true);
		pBrushTool->SetBrushInformation_Other(vPos);
	}
	else
	{
		pBrushTool->SetBrushCheck_Other(false);
		pBrushTool->SetBrushInformation_Other(vPos);
	}

	SAFE_RELEASE(pBrushTool);
	SAFE_RELEASE(pBrushObj);
}

void CNaviTab::OnBnClickedCheckNavi()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int check = m_checkBoxNaviOn.GetCheck();

	if (check == 1)
	{
		GET_SINGLE(CNaviManager)->SetRenderCheck(true);
	}
	else
	{
		GET_SINGLE(CNaviManager)->SetRenderCheck(false);
	}
}


void CNaviTab::OnBnClickedButtonNaviUndo()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (GET_SINGLE(CNaviManager)->IsCellEmpty())
		return;

	GET_SINGLE(CNaviManager)->UndoCell();

	m_listNaviCell.DeleteString(m_listNaviCell.GetCount() - 1);
	--m_NumCell;
}


void CNaviTab::OnBnClickedButtonNaviSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	static TCHAR BASED_CODE szFilter[] =
		_T("데이터 파일(*.bin) | *.bin;|모든파일(*.*)|*.*||");
	CFileDialog dlg(FALSE, NULL, NULL, OFN_OVERWRITEPROMPT, szFilter);

	// 경로 지정
	wchar_t strPath[MAX_PATH] = {};
	wchar_t strDir[MAX_PATH] = {};
	wcscpy_s(strPath, MAX_PATH, GET_SINGLE(CPathManager)->FindPath(DATA_PATH));
	wcscpy_s(strDir, MAX_PATH, GET_SINGLE(CPathManager)->FindPath(DATA_PATH));
	wcscat_s(strPath, MAX_PATH, L"Navi\\");

	CString originPath = strPath;

	dlg.m_ofn.lpstrInitialDir = strPath;

	// do modal error 해결
	if (dlg.DoModal() != IDOK)
		return;

	CString path = dlg.GetPathName();
	CString fileName = dlg.GetFileName();
	CString flexiblePath = L"Navi\\";

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

	CT2CA pszConvertAnsiStringPathName(path);
	string strFilePath(pszConvertAnsiStringPathName);
	CT2CA pszConvertAnsiStringFileName(fileName);
	string strFileName(pszConvertAnsiStringFileName);

	ofstream mainFile;
	mainFile.open(strFilePath + strFileName + ".bin", ios::out | ios::trunc);
	
	const vector<CCell*>* pVecCellInfo = GET_SINGLE(CNaviManager)->GetNaviCells();

	// Cell 크기
	UINT iCellSize = (UINT)pVecCellInfo->size();
	mainFile << iCellSize << endl;

	// Cell을 순회돌며
	// vPosition을 저장한다.
	for (const auto& cell : *pVecCellInfo)
	{
		mainFile << cell->Get_Point(0).x << ' ' << cell->Get_Point(0).y << ' ' << cell->Get_Point(0).z << endl;
		mainFile << cell->Get_Point(1).x << ' ' << cell->Get_Point(1).y << ' ' << cell->Get_Point(1).z << endl;
		mainFile << cell->Get_Point(2).x << ' ' << cell->Get_Point(2).y << ' ' << cell->Get_Point(2).z << endl;
	}

	mainFile.close();
}


void CNaviTab::OnBnClickedButtonNaviLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	static TCHAR BASED_CODE szFilter[] =
		_T("데이터 파일(*.bin) | *.bin;|모든파일(*.*)|*.*||");
	CFileDialog dlg(TRUE, NULL, NULL, OFN_READONLY | OFN_OVERWRITEPROMPT, szFilter);

	// 경로 지정
	wchar_t strPath[MAX_PATH] = {};
	wcscpy_s(strPath, MAX_PATH, GET_SINGLE(CPathManager)->FindPath(DATA_PATH));
	wcscat_s(strPath, MAX_PATH, L"Navi\\");

	CString originPath = strPath;

	dlg.m_ofn.lpstrInitialDir = strPath;

	// do modal error 해결
	if (dlg.DoModal() != IDOK)
		return;

	CString path = dlg.GetPathName();
	CString fileName = dlg.GetFileName();

	// 확장자명 제거(만약 확장자명이 세이브 이름으로 들어갈시)
	for (int i = lstrlen(fileName) - 1; i >= 0; i--)
	{
		if (fileName[i] == '.')
		{
			fileName.Delete(i, lstrlen(fileName) - 1);
			break;
		}
	}

	CT2CA pszConvertAnsiStringName(fileName);
	string strFileName(pszConvertAnsiStringName);

	// Cell 다 지우고
	OnBnClickedButtonNaviClearall();

	// 불러오기
	if (!GET_SINGLE(CNaviManager)->CreateNaviMesh(strFileName))
		return;

	UINT iCellSize = (UINT)GET_SINGLE(CNaviManager)->GetNaviCells()->size();

	for (UINT i = 0; i < iCellSize; ++i)
	{
		m_listNaviCell.AddString(L"Cell_" + (CString)to_string(m_NumCell).c_str());
		++m_NumCell;
	}
	
}


void CNaviTab::OnBnClickedButtonNaviDelete()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (GET_SINGLE(CNaviManager)->IsCellEmpty() ||
		m_pSelectCell == nullptr)
		return;

	GET_SINGLE(CNaviManager)->EraseCell(m_pSelectCell);

	m_listNaviCell.DeleteString(m_listNaviCell.GetCount() - 1);
	--m_NumCell;
}


void CNaviTab::OnBnClickedButtonNaviClearall()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (GET_SINGLE(CNaviManager)->IsCellEmpty())
		return;

	GET_SINGLE(CNaviManager)->FreeNaviMesh();
	m_listNaviCell.ResetContent();
	m_NumCell = 0;
}


BOOL CNaviTab::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}


BOOL CNaviTab::PreTranslateMessage(MSG* pMsg)
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
