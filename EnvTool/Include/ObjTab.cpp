﻿// ObjTab.cpp: 구현 파일
//

#include "stdafx.h"
#include "EnvTool.h"
#include "ObjTab.h"
#include "afxdialogex.h"
#include "Core/PathManager.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "Scene/Layer.h"
#include "Component/Transform.h"
#include "Component/Renderer.h"
#include "Resources/ResourcesManager.h"
#include "Resources/Mesh.h"
#include "Rendering/ShaderManager.h"


// CObjTab 대화 상자
int CObjTab::g_iObjNumber = 0;

IMPLEMENT_DYNAMIC(CObjTab, CDialogEx)

CObjTab::CObjTab(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG2, pParent)
{

}

CObjTab::~CObjTab()
{
	for (auto& iter : m_vecObjects)
	{
		SAFE_RELEASE(iter);
	}
	m_vecObjects.clear();
}

void CObjTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_OBJECT_TYPE, m_listObjType);
	DDX_Control(pDX, IDC_LIST_OBJECTS, m_listObjList);
}


BEGIN_MESSAGE_MAP(CObjTab, CDialogEx)
	ON_LBN_SELCHANGE(IDC_LIST_OBJECT_TYPE, &CObjTab::OnLbnSelchangeListObjectType)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_ALLOBJ, &CObjTab::OnBnClickedButtonDeleteAllobj)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_OBJ, &CObjTab::OnBnClickedButtonDeleteObj)
	ON_LBN_SELCHANGE(IDC_LIST_OBJECTS, &CObjTab::OnLbnSelchangeListObjects)
	ON_BN_CLICKED(IDC_BUTTON_OBJECT_SAVE, &CObjTab::OnBnClickedButtonObjectSave)
	ON_BN_CLICKED(IDC_BUTTON_OBJECT_LOAD, &CObjTab::OnBnClickedButtonObjectLoad)
END_MESSAGE_MAP()


// CObjTab 메시지 처리기


BOOL CObjTab::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	// ObjPath를 가지고 온다.
	string MeshPath;
	MeshPath = GET_SINGLE(CPathManager)->FindPathToMultiByte(MESH_PATH);
	string DataPath;
	DataPath = GET_SINGLE(CPathManager)->FindPathToMultiByte(DATA_PATH);

	ifstream file;

	file.open(DataPath + "Object\\ObjType.txt", ios::in);

	if (!file.is_open())
		return FALSE;

	while (!file.eof())
	{
		string filePath;
		file >> filePath;
		// Path를 listBox 담는다.
		//m_vecStringObjTypePath.push_back(filePath);
		m_listObjType.AddString((CString)filePath.c_str());
	}


	UpdateData(FALSE);
	// Mesh를 Load
	// 한꺼번에 
	// 나중에

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CObjTab::OnLbnSelchangeListObjectType()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	Vector3 vOldPos = { 0.f, 0.f, 0.f };

	if (m_pTempObject)
	{
		CTransform* pTr = m_pTempObject->GetTransform();
		vOldPos = pTr->GetWorldPos();
		m_pTempObject->Die();
		CGameObject::EraseObj(m_pTempObject);
		SAFE_RELEASE(pTr);
		SAFE_RELEASE(m_pTempObject);
	}

	CString meshPath, meshRestPath;;
	meshPath = GET_SINGLE(CPathManager)->FindPath(MESH_PATH);

	int iPos = m_listObjType.GetCurSel();

	m_listObjType.GetText(iPos, meshRestPath);

	CScene *pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
	CLayer *pLayer = pScene->GetLayer("Default");
	m_pTempObject = CGameObject::CreateObject("TempObj", pLayer);

	// Local Transform Data
	CString localDataPath;

	localDataPath = meshPath + meshRestPath + L".dat";

	FILE* pFile = nullptr;
	
	char strPath[MAX_PATH] = {};
	WideCharToMultiByte(CP_ACP, 0, localDataPath, -1,
		strPath, lstrlen(localDataPath), 0, 0);

	fopen_s(&pFile, strPath, "rb");

	if (!pFile)
		return;

	CTransform* pTr = m_pTempObject->GetTransform();
	pTr->Load_Local(pFile);
	pTr->SetWorldPos(vOldPos);
	SAFE_RELEASE(pTr);

	fclose(pFile);

	// Mesh Data
	CString meshDataPath;

	meshDataPath = meshPath + meshRestPath + L".msh";

	CRenderer* pRenderer = m_pTempObject->AddComponent<CRenderer>("Renderer");
	CT2CA pszConvertAnsiStringName(meshRestPath);
	string tag(pszConvertAnsiStringName);

	pRenderer->SetMeshFromFullPath(tag, meshDataPath);

	m_currentTypePath = meshRestPath;

	SAFE_RELEASE(pRenderer);
	SAFE_RELEASE(pScene);
	SAFE_RELEASE(pLayer);
}

void CObjTab::Process_ChangeTab()
{
	if (m_pTempObject)
	{
		m_pTempObject->Enable(false);
	}
}

void CObjTab::Process_ShowTab()
{
	if (m_pTempObject)
	{
		m_pTempObject->Enable(true);
	}
}

void CObjTab::UpdateForm()
{
	/*for (auto& iter : m_vecObjects)
	{
		CRenderer* pRenderer = iter->FindComponentFromType<CRenderer>(CT_RENDERER);

		SHARECBUFFER tShareBuffer = {};

		tShareBuffer.vColor = Vector4{ 1.f, 0.f, 0.f, 1.f };

		pRenderer->UpdateCBuffer("Share", 8, sizeof(SHARECBUFFER), SCT_PIXEL, &tShareBuffer);

		SAFE_RELEASE(pRenderer);
	}*/
}

void CObjTab::AddObject()
{
	CScene *pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
	CLayer *pLayer = pScene->GetLayer("Default");

	CString objName;
	objName.Format(_T("%d"), g_iObjNumber);
	
	objName = L"Object_" + objName;
	m_listObjList.AddString(objName);

	CGameObject *pObj = CGameObject::CreateObject(string(CT2CA(objName)), pLayer);
	
	SAFE_RELEASE(pLayer);
	SAFE_RELEASE(pScene);

	// Transform
	CTransform* pOriginTr = m_pTempObject->GetTransform();
	CTransform* pTr = pObj->GetTransform();

	// Local
	pTr->SetLocalScale(pOriginTr->GetLocalScale());
	pTr->SetLocalRot(pOriginTr->GetLocalRot());
	pTr->SetLocalPos(pOriginTr->GetLocalPos());

	// World
	pTr->SetWorldScale(pOriginTr->GetWorldScale());
	pTr->SetWorldRot(pOriginTr->GetWorldRot());
	pTr->SetWorldPos(pOriginTr->GetWorldPos());

	SAFE_RELEASE(pTr);
	SAFE_RELEASE(pOriginTr);

	// Mesh
	CRenderer* pOriginRenderer = m_pTempObject->FindComponentFromTag<CRenderer>("Renderer");
	CRenderer* pRenderer = pOriginRenderer->Clone();
	pObj->AddComponent(pRenderer);
	
	SAFE_RELEASE(pRenderer);
	SAFE_RELEASE(pOriginRenderer);

	// CBuffer
	pRenderer = pObj->FindComponentFromType<CRenderer>(CT_RENDERER);
	pRenderer->CreateCBuffer("Share", 8, sizeof(SHARECBUFFER), SCT_PIXEL);

	SHARECBUFFER tShareBuffer = {};

	tShareBuffer.vColor = Vector4{ 0.f, 0.f, 0.f, 1.f };

	pRenderer->UpdateCBuffer("Share", 8, sizeof(SHARECBUFFER), SCT_PIXEL, &tShareBuffer);

	SAFE_RELEASE(pRenderer);

	m_vecObjects.push_back(pObj);
	m_vecStringObjTypePath.push_back(string(CT2CA(m_currentTypePath)));
	g_iObjNumber++;
}

void CObjTab::Undo()
{
	if (!m_vecObjects.empty())
	{
		auto& iter_end = m_vecObjects.end();

		--iter_end;

		(*iter_end)->Die();
		CGameObject::EraseObj((*iter_end));
		SAFE_RELEASE((*iter_end));

		m_vecObjects.pop_back();
		m_vecStringObjTypePath.pop_back();

		m_listObjList.DeleteString(m_listObjList.GetCount() - 1);
		g_iObjNumber--;
	}
}


void CObjTab::OnBnClickedButtonDeleteAllobj()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	for (auto& iter : m_vecObjects)
	{
		iter->Die();
		CGameObject::EraseObj(iter);
		SAFE_RELEASE(iter);
	}
	m_vecObjects.clear();
	m_listObjList.ResetContent();
	m_vecStringObjTypePath.clear();
	g_iObjNumber = 0;
}


void CObjTab::OnBnClickedButtonDeleteObj()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int iPos = m_listObjList.GetCurSel();

	if (iPos == -1)
	{
		AfxMessageBox(L"Error : Select Object First!");
		return;
	}

	// 마지막값
	if(iPos == m_listObjList.GetCount() - 1)
	{
		Undo();

		if (!m_vecObjects.empty())
		{
			m_listObjList.SetCurSel(iPos - 1);

			CRenderer* pRenderer = m_vecObjects[iPos - 1]->FindComponentFromType<CRenderer>(CT_RENDERER);

			SHARECBUFFER tShareBuffer = {};
			tShareBuffer.vColor = Vector4{ 1.f, 0.f, 0.f, 1.f };
			pRenderer->UpdateCBuffer("Share", 8, sizeof(SHARECBUFFER), SCT_PIXEL, &tShareBuffer);

			SAFE_RELEASE(pRenderer);
		}
	}

	// 아닌경우
	else
	{
		m_vecObjects[iPos]->Die();
		CGameObject::EraseObj(m_vecObjects[iPos]);
		SAFE_RELEASE(m_vecObjects[iPos]);
		m_vecObjects.erase(m_vecObjects.begin() + iPos);
		m_vecStringObjTypePath.erase(m_vecStringObjTypePath.begin() + iPos);

		m_listObjList.DeleteString(iPos);
		m_listObjList.SetCurSel(iPos + 1);

		if (!m_vecObjects.empty())
		{
			CRenderer* pRenderer = m_vecObjects[iPos + 1]->FindComponentFromType<CRenderer>(CT_RENDERER);

			SHARECBUFFER tShareBuffer = {};
			tShareBuffer.vColor = Vector4{ 1.f, 0.f, 0.f, 1.f };
			pRenderer->UpdateCBuffer("Share", 8, sizeof(SHARECBUFFER), SCT_PIXEL, &tShareBuffer);

			SAFE_RELEASE(pRenderer);
		}
	}
}


void CObjTab::OnLbnSelchangeListObjects()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	for (auto& iter : m_vecObjects)
	{
		CRenderer* pRenderer = iter->FindComponentFromType<CRenderer>(CT_RENDERER);

		SHARECBUFFER tShareBuffer = {};

		tShareBuffer.vColor = Vector4{ 0.f, 0.f, 0.f, 1.f };

		pRenderer->UpdateCBuffer("Share", 8, sizeof(SHARECBUFFER), SCT_PIXEL, &tShareBuffer);

		SAFE_RELEASE(pRenderer);
	}

	int iPos = m_listObjList.GetCurSel();

	CRenderer* pRenderer = m_vecObjects[iPos]->FindComponentFromType<CRenderer>(CT_RENDERER);

	SHARECBUFFER tShareBuffer = {};
	tShareBuffer.vColor = Vector4{ 1.f, 0.f, 0.f, 1.f };
	pRenderer->UpdateCBuffer("Share", 8, sizeof(SHARECBUFFER), SCT_PIXEL, &tShareBuffer);

	SAFE_RELEASE(pRenderer);

}


void CObjTab::OnBnClickedButtonObjectSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_vecObjects.empty())
	{
		AfxMessageBox(L"Error : No object to save! Create object first!");
		return;
	}

	static TCHAR BASED_CODE szFilter[] =
		_T("데이터 파일(*.bin) | *.bin;|모든파일(*.*)|*.*||");
	CFileDialog dlg(FALSE, NULL, NULL, OFN_OVERWRITEPROMPT, szFilter);

	// 경로 지정
	wchar_t strPath[MAX_PATH] = {};
	wchar_t strDir[MAX_PATH] = {};
	wcscpy_s(strPath, MAX_PATH, GET_SINGLE(CPathManager)->FindPath(DATA_PATH));
	wcscpy_s(strDir, MAX_PATH, GET_SINGLE(CPathManager)->FindPath(DATA_PATH));
	wcscat_s(strPath, MAX_PATH, L"Object\\");

	CString originPath = strPath;

	dlg.m_ofn.lpstrInitialDir = strPath;

	// do modal error 해결
	if (dlg.DoModal() != IDOK)
		return;

	CString path = dlg.GetPathName();
	CString fileName = dlg.GetFileName();
	
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

	ofstream file;

	file.open(path + fileName + L".bin", ios::out);

	// 총 오브젝트의 갯수
	file << m_vecObjects.size() << endl;

	int iIndex = 0;
	for (const auto& iter : m_vecObjects)
	{
		// -> 파일경로 (Tag)
		file << m_vecStringObjTypePath[iIndex] << endl;

		// -> Transform(World)
		CTransform* pTr = iter->GetTransform();
		// Scale
		Vector3 vScale = pTr->GetWorldScale();
		file << vScale.x << ' ' << vScale.y << ' ' << vScale.z << endl;
		// Rotation
		Vector3 vRotation = pTr->GetWorldRot();
		file << vRotation.x << ' ' << vRotation.y << ' ' << vRotation.z << endl;
		// Position
		Vector3 vPosition = pTr->GetWorldPos();
		file << vPosition.x << ' ' << vPosition.y << ' ' << vPosition.z << endl;
		SAFE_RELEASE(pTr);

		iIndex++;
	}

	file.close();
}


void CObjTab::OnBnClickedButtonObjectLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	static TCHAR BASED_CODE szFilter[] =
		_T("데이터 파일(*.bin) | *.bin;|모든파일(*.*)|*.*||");
	CFileDialog dlg(TRUE, NULL, NULL, OFN_READONLY | OFN_OVERWRITEPROMPT, szFilter);

	// 경로 지정
	wchar_t strPath[MAX_PATH] = {};
	wcscpy_s(strPath, MAX_PATH, GET_SINGLE(CPathManager)->FindPath(DATA_PATH));
	wcscat_s(strPath, MAX_PATH, L"Object\\");

	CString originPath = strPath;

	dlg.m_ofn.lpstrInitialDir = strPath;

	// do modal error 해결
	if (dlg.DoModal() != IDOK)
		return;
	
	CString filePath = dlg.GetPathName();

	// 전체삭제
	OnBnClickedButtonDeleteAllobj();

	ifstream file;
	file.open(filePath, ios::in);

	if (!file.is_open())
		return;

	// 전체 갯수
	int iObjSize = 0;
	file >> iObjSize;

	m_vecObjects.resize(iObjSize);
	m_vecStringObjTypePath.resize(iObjSize);

	CScene *pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
	CLayer *pLayer = pScene->GetLayer("Default");

	for (int i = 0; i < iObjSize; i++)
	{
		CString objName;
		objName.Format(_T("%d"), g_iObjNumber);

		objName = L"Object_" + objName;
		m_listObjList.AddString(objName);

		CGameObject *pObj = CGameObject::CreateObject(string(CT2CA(objName)), pLayer);
		// Mesh Path
		// 파일경로
		string ObjTag;
		file >> ObjTag;
		// Mesh
		string meshPath, meshRestPath;
		meshPath = GET_SINGLE(CPathManager)->FindPathToMultiByte(MESH_PATH);
		meshRestPath = ObjTag;

		string meshDataPath;
		meshDataPath = meshPath + meshRestPath + ".msh";

		CRenderer* pRenderer = pObj->AddComponent<CRenderer>("Renderer");

		wstring wMeshDataPath;
		wMeshDataPath.assign(meshDataPath.begin(), meshDataPath.end());
		pRenderer->SetMeshFromFullPath(ObjTag, wMeshDataPath.c_str());

		SAFE_RELEASE(pRenderer);
		
		// Transform
		// Local Transform Data
		string localDataPath;

		localDataPath = meshPath + meshRestPath + ".dat";

		FILE* pFile = nullptr;

		fopen_s(&pFile, localDataPath.c_str(), "rb");

		if (!pFile)
			return;

		CTransform* pTr = pObj->GetTransform();
		pTr->Load_Local(pFile);
		fclose(pFile);
		
		// World Transform Data
		Vector3 vScale, vRotation, vPos;
		file >> vScale.x >> vScale.y >> vScale.z;
		file >> vRotation.x >> vRotation.y >> vRotation.z;
		file >> vPos.x >> vPos.y >> vPos.z;

		pTr->SetWorldScale(vScale);
		pTr->SetWorldRot(vRotation);
		pTr->SetWorldPos(vPos);

		SAFE_RELEASE(pTr);
		// Vector 추가
		m_vecObjects[i] = pObj;
		m_vecStringObjTypePath[i] = meshRestPath;
		g_iObjNumber++;
	}

	file.close();

	SAFE_RELEASE(pLayer);
	SAFE_RELEASE(pScene);
	
}
