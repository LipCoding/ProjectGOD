// ObjTab.cpp: 구현 파일
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
#include "Resources/Mesh.h"


// CObjTab 대화 상자
int CObjTab::g_iObjNumber = 0;

IMPLEMENT_DYNAMIC(CObjTab, CDialogEx)

CObjTab::CObjTab(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG2, pParent)
{

}

CObjTab::~CObjTab()
{
}

void CObjTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_OBJECT_TYPE, m_listObjType);
	DDX_Control(pDX, IDC_LIST_OBJECTS, m_listObjList);
}


BEGIN_MESSAGE_MAP(CObjTab, CDialogEx)
	ON_LBN_SELCHANGE(IDC_LIST_OBJECT_TYPE, &CObjTab::OnLbnSelchangeListObjectType)
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
