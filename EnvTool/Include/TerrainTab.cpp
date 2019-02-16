// TerrainTab.cpp: 구현 파일
//

#include "stdafx.h"
#include "EnvTool.h"
#include "TerrainTab.h"
#include "afxdialogex.h"

#include "GameObject/GameObject.h"
#include "Scene/Scene.h"
#include "Scene/Layer.h"
#include "Scene/SceneManager.h"
#include "Component/Transform.h"
#include "Component/Renderer2D.h"
#include "Component/Material.h"
#include "Component/Terrain2D.h"
#include "Component/LandScape.h"
#include "Component/Picking.h"
#include "Component/QuadTree.h"

PG_USING

// CTerrainTab 대화 상자

IMPLEMENT_DYNAMIC(CTerrainTab, CDialogEx)

CTerrainTab::CTerrainTab(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
	, m_iSizeX(0)
	, m_iSizeZ(0)
{

}

CTerrainTab::~CTerrainTab()
{
}

void CTerrainTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_TERRAIN_SIZEX, m_editTerrainSizeX);
	DDX_Control(pDX, IDC_EDIT_TERRIAN_SIZEY, m_editTerrainSizeZ);
}


BEGIN_MESSAGE_MAP(CTerrainTab, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_ADJ_SIZE, &CTerrainTab::OnBnClickedButtonAdjSize)
END_MESSAGE_MAP()


// CTerrainTab 메시지 처리기


void CTerrainTab::OnBnClickedButtonAdjSize()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CString sizeX, sizeZ;

	// 에디트 컨트롤을 가져온다.
	m_editTerrainSizeX.GetWindowTextA(sizeX);
	m_editTerrainSizeZ.GetWindowTextA(sizeZ);

	// string을 숫자로 변환
	m_iSizeX = _ttoi(sizeX);
	m_iSizeZ = _ttoi(sizeZ);

	// terrain 생성
	CScene* pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();

	CLayer* pLayer = pScene->GetLayer("Default");
	CGameObject* pLandScapeObj = CGameObject::CreateObject("LandScape", pLayer);
	CTransform* pLandTr = pLandScapeObj->GetTransform();
	SAFE_RELEASE(pLandTr);

	CLandScape* pLandScape = pLandScapeObj->AddComponent<CLandScape>("LandScape");
	pLandScape->CreateLandScapeQuadTree("LandScapeMesh", m_iSizeX, m_iSizeZ, false, "LandScape",
		L"LandScape/GRASS_00+SAND.dds",
		L"LandScape/GRASS_00+SAND_NRM.png",
		L"LandScape/GRASS_00+SAND_SPEC.png");

	/*pLandScape->CreateLandScape("LandScapeMesh", m_iSizeX, m_iSizeZ, false, "LandScape",
		L"LandScape/GRASS_00+SAND.dds",
		L"LandScape/GRASS_00+SAND_NRM.png",
		L"LandScape/GRASS_00+SAND_SPEC.png");*/

	CPicking* pPicking = pLandScapeObj->AddComponent<CPicking>("Picking");

	SAFE_RELEASE(pPicking);
	SAFE_RELEASE(pLandScape);

	SAFE_RELEASE(pLayer);
	SAFE_RELEASE(pScene);

	_cprintf("Terrain Create Success!\n");
}
