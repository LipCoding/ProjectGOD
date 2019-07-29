// SkyTab.cpp: 구현 파일
//

#include "stdafx.h"
#include "EnvTool.h"
#include "SkyTab.h"
#include "afxdialogex.h"

#include "Core/PathManager.h"
#include "GameObject/GameObject.h"
#include "Resources/ResourcesManager.h"
#include "Component/Renderer.h"
#include "Component/Material.h"

// CSkyTab 대화 상자

IMPLEMENT_DYNAMIC(CSkyTab, CDialogEx)

CSkyTab::CSkyTab(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG5, pParent)
{

}

CSkyTab::~CSkyTab()
{
}

void CSkyTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_SKYTYPE, m_comboSkyBoxList);
}


BEGIN_MESSAGE_MAP(CSkyTab, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO_SKYTYPE, &CSkyTab::OnCbnSelchangeComboSkytype)
	ON_BN_CLICKED(IDC_BUTTON_SKY_SAVE, &CSkyTab::OnBnClickedButtonSkySave)
	ON_BN_CLICKED(IDC_BUTTON_SKY_LOAD, &CSkyTab::OnBnClickedButtonSkyLoad)
END_MESSAGE_MAP()


// CSkyTab 메시지 처리기


BOOL CSkyTab::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	string TexturePath;
	TexturePath = GET_SINGLE(CPathManager)->FindPathToMultiByte(TEXTURE_PATH);
	string DataPath;
	DataPath = GET_SINGLE(CPathManager)->FindPathToMultiByte(DATA_PATH);

	ifstream file;

	file.open(DataPath + "Sky\\SkyType.txt", ios::in);

	if (!file.is_open())
		return FALSE;

	while (!file.eof())
	{
		string filePath;
		file >> filePath;
		m_comboSkyBoxList.AddString((CString)filePath.c_str());
	}

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CSkyTab::OnCbnSelchangeComboSkytype()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString TexturePath;
	TexturePath = GET_SINGLE(CPathManager)->FindPath(TEXTURE_PATH);

	int iPos = m_comboSkyBoxList.GetCurSel();

	CString TextureSubPath;

	m_comboSkyBoxList.GetLBText(iPos, TextureSubPath);

	// SkyBox
	CGameObject* pSky = CGameObject::FindObject("Sky");
	if (pSky)
	{
		CRenderer*   pRenderer = pSky->FindComponentFromTag<CRenderer>("SkyRenderer");
		CMaterial*   pMaterial = pRenderer->GetMaterial();

		pMaterial->ResetTextureInfo();
		GET_SINGLE(CResourcesManager)->FindAndDeleteTexture("Sky");
		pMaterial->SetDiffuseTexInfo(SAMPLER_LINEAR, "Sky", 0, 0, TextureSubPath);

		SAFE_RELEASE(pMaterial);
		SAFE_RELEASE(pRenderer);
		SAFE_RELEASE(pSky);
	}
}


void CSkyTab::OnBnClickedButtonSkySave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CSkyTab::OnBnClickedButtonSkyLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
