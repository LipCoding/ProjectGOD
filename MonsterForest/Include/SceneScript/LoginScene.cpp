#include "LoginScene.h"
#include "Scene/Layer.h"
#include "GameObject/GameObject.h"
#include "Component/Renderer2D.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Component/Material.h"
#include "Component/Transform.h"
#include "Component/Animation2D.h"
#include "Component/Terrain2D.h"
#include "Component/Camera.h"
#include "Component/UIButton.h"
#include "Component/ColliderRect.h"
#include "Core/Input.h"
#include "Device.h"
#include "MainScene.h"
#include "Core/Scheduler.h"
#include "Core/NetworkManager.h"
#include "Core.h"
#include "Device.h"
#include "Component/Renderer2D.h"
#include "Component/Font.h"
#include "Component/ColliderRect.h"
#include "../Client.h"
#include "Component/UIPanel.h"
#include "../UserInterfaceManager.h"
LoginScene::LoginScene()
{
}

LoginScene::~LoginScene()
{
}

bool LoginScene::Init()
{
	{
		CLayer* pLayer = m_pScene->CreateLayer("UI+1", UI_LAYER + 1);
		SAFE_RELEASE(pLayer);
	}
	{
		CLayer* pLayer = m_pScene->CreateLayer("UI+2", UI_LAYER + 2);
		SAFE_RELEASE(pLayer);
	}
	/// < Background>
	{
		CLayer*	pUILayer = m_pScene->GetLayer("UI");

		CGameObject*	pLoginSceneBackgroundObj = CGameObject::CreateObject("LoginSceneBackground", pUILayer);

		CUIPanel*	pLoginSceneBackgroundUI = pLoginSceneBackgroundObj->AddComponent<CUIPanel>("LoginSceneBackground");
		pLoginSceneBackgroundUI->SetTitleArea(0, 0, 0, 0);
		SAFE_RELEASE(pLoginSceneBackgroundUI);
		
		CTransform*	pLoginSceneBackgroundTr = pLoginSceneBackgroundObj->GetTransform();

		pLoginSceneBackgroundTr->SetWorldScale(DEVICE_RESOLUTION.iWidth,
			DEVICE_RESOLUTION.iHeight, 1.f);

		SAFE_RELEASE(pLoginSceneBackgroundTr);

		CRenderer2D*	pRenderer = pLoginSceneBackgroundObj->AddComponent<CRenderer2D>("Renderer");

		pRenderer->SetShader(UI_SHADER);
		pRenderer->SetInputLayout("VertexTex");
		pRenderer->SetMesh("RectOrthoLTTex");
		pRenderer->SetRenderState(ALPHA_BLEND);

		/*pRenderer->CreateCBuffer("Button", 11, sizeof(BUTTONCBUFFER),
			SCT_VERTEX | SCT_PIXEL);*/

		CMaterial* pMaterial = pRenderer->CreateMaterial(SAMPLER_LINEAR,
			"LoginSceneBackground", 0, 0, L"LoginSceneBackground.png");

		SAFE_RELEASE(pMaterial);

		SAFE_RELEASE(pRenderer);
		SAFE_RELEASE(pUILayer);
	}
	/// < BaseUI >
	{
		CLayer*	pUILayer = m_pScene->GetLayer("UI");
		CGameObject*	pGameStartObj = CGameObject::CreateObject("MenuBaseUI", pUILayer);

		CUIPanel*	pGameStartUI = pGameStartObj->AddComponent<CUIPanel>("MenuBaseUI");
		pGameStartUI->EnableTitle(false);
		SAFE_RELEASE(pGameStartUI);

		CTransform*	pButtonTr = pGameStartObj->GetTransform();

		//pButtonTr->SetPivot(0.5f, 0.5f, 0.f);
		pButtonTr->SetWorldScale(260.f, 190.f, 1.f);
		pButtonTr->SetWorldPos(DEVICE_RESOLUTION.iWidth / 2.f - 115.f,
			DEVICE_RESOLUTION.iHeight / 2.f - 50.f, 0.f);

		SAFE_RELEASE(pButtonTr);

		CRenderer2D* pRenderer = pGameStartObj->FindComponentFromType<CRenderer2D>(CT_RENDERER2D);

		CMaterial* pMaterial = pRenderer->GetMaterial();

		pMaterial->SetDiffuseTexInfo("Linear", "BaseUI",
			0, 0, L"UserInterface\\UI_BASIC_BOX_BIG.png");

		SAFE_RELEASE(pMaterial);

		SAFE_RELEASE(pRenderer);

		CColliderRect* pRC = pGameStartObj->FindComponentFromType<CColliderRect>(CT_COLLIDER);

		pRC->SetRect(0, 0, 0, 0);

		SAFE_RELEASE(pRC);
		SAFE_RELEASE(pGameStartObj);

		SAFE_RELEASE(pUILayer);
	}

	/// < GameStart >
	{
		CLayer*	pUILayer = m_pScene->GetLayer("UI");
		CGameObject*	pGameStartObj = CGameObject::CreateObject("GaemStart", pUILayer);

		CUIButton*	pGameStartUI = pGameStartObj->AddComponent<CUIButton>("GaemStart");

		pGameStartUI->SetCallback(this, &LoginScene::gameConnectCallBack);

		SAFE_RELEASE(pGameStartUI);

		CTransform*	pButtonTr = pGameStartObj->GetTransform();

		//pButtonTr->SetPivot(0.5f, 0.5f, 0.f);
		pButtonTr->SetWorldScale(230.f, 50.f, 1.f);
		pButtonTr->SetWorldPos(DEVICE_RESOLUTION.iWidth / 2.f - 100.f,
			DEVICE_RESOLUTION.iHeight / 2.f - 35.f, 0.f);

		SAFE_RELEASE(pButtonTr);

		CRenderer2D* pRenderer = pGameStartObj->FindComponentFromType<CRenderer2D>(CT_RENDERER2D);

		CMaterial* pMaterial = pRenderer->GetMaterial();

		pMaterial->SetDiffuseTexInfo("Linear", "GaemStart",
			0, 0, L"UserInterface\\UI_MENU_BUTTON.png");

		SAFE_RELEASE(pMaterial);

		SAFE_RELEASE(pRenderer);

		CColliderRect* pRC = pGameStartObj->FindComponentFromType<CColliderRect>(CT_COLLIDER);

		pRC->SetRect(0, 0, 230.f, 50.f);

		SAFE_RELEASE(pRC);

		CFont* pGameStartFont = pGameStartObj->AddComponent<CFont>("GameStartFont");
		pGameStartFont->SetFont("돋움");
		pGameStartFont->SetText(L"게임 접속");
		pGameStartFont->SetArea(50, 0, 200, 39.f);

		SAFE_RELEASE(pGameStartFont);
		SAFE_RELEASE(pGameStartObj);

		SAFE_RELEASE(pUILayer);
	}

	/// < Create Account >
	{
		CLayer*	pUILayer = m_pScene->GetLayer("UI");
		CGameObject*	pGameStartObj = CGameObject::CreateObject("CreateAccount", pUILayer);

		CUIButton*	pGameStartUI = pGameStartObj->AddComponent<CUIButton>("CreateAccount");

		//pGameStartUI->SetCallback(this, &LoginScene::StartCallback);

		SAFE_RELEASE(pGameStartUI);

		CTransform*	pButtonTr = pGameStartObj->GetTransform();

		//pButtonTr->SetPivot(0.5f, 0.5f, 0.f);
		pButtonTr->SetWorldScale(230.f, 50.f, 1.f);
		pButtonTr->SetWorldPos(DEVICE_RESOLUTION.iWidth / 2.f - 100.f,
			DEVICE_RESOLUTION.iHeight / 2.f + 20.f, 0.f);

		SAFE_RELEASE(pButtonTr);

		CRenderer2D* pRenderer = pGameStartObj->FindComponentFromType<CRenderer2D>(CT_RENDERER2D);

		CMaterial* pMaterial = pRenderer->GetMaterial();

		pMaterial->SetDiffuseTexInfo("Linear", "CreateAccount",
			0, 0, L"UserInterface\\UI_MENU_BUTTON.png");

		SAFE_RELEASE(pMaterial);

		SAFE_RELEASE(pRenderer);

		CColliderRect* pRC = pGameStartObj->FindComponentFromType<CColliderRect>(CT_COLLIDER);

		pRC->SetRect(0, 0, 230.f, 50.f);

		SAFE_RELEASE(pRC);

		CFont* pGameStartFont = pGameStartObj->AddComponent<CFont>("CreateAccountFont");
		pGameStartFont->SetFont("돋움");
		pGameStartFont->SetText(L"계정 만들기");
		pGameStartFont->SetArea(35, 0, 200, 39.f);

		SAFE_RELEASE(pGameStartFont);
		SAFE_RELEASE(pGameStartObj);

		SAFE_RELEASE(pUILayer);
	}

	/// < Game Option >
	{
		CLayer*	pUILayer = m_pScene->GetLayer("UI");
		CGameObject*	pGameStartObj = CGameObject::CreateObject("GameOption", pUILayer);

		CUIButton*	pGameStartUI = pGameStartObj->AddComponent<CUIButton>("GameOption");

		//pGameStartUI->SetCallback(this, &LoginScene::StartCallback);

		SAFE_RELEASE(pGameStartUI);

		CTransform*	pButtonTr = pGameStartObj->GetTransform();

		//pButtonTr->SetPivot(0.5f, 0.5f, 0.f);
		pButtonTr->SetWorldScale(230.f, 50.f, 1.f);
		pButtonTr->SetWorldPos(DEVICE_RESOLUTION.iWidth / 2.f - 100.f,
			DEVICE_RESOLUTION.iHeight / 2.f + 75.f, 0.f);

		SAFE_RELEASE(pButtonTr);

		CRenderer2D* pRenderer = pGameStartObj->FindComponentFromType<CRenderer2D>(CT_RENDERER2D);

		CMaterial* pMaterial = pRenderer->GetMaterial();

		pMaterial->SetDiffuseTexInfo("Linear", "GameOption",
			0, 0, L"UserInterface\\UI_MENU_BUTTON.png");

		SAFE_RELEASE(pMaterial);

		SAFE_RELEASE(pRenderer);

		CColliderRect* pRC = pGameStartObj->FindComponentFromType<CColliderRect>(CT_COLLIDER);
		
		pRC->SetRect(0, 0, 230.f, 50.f);

		SAFE_RELEASE(pRC);

		CFont* pGameStartFont = pGameStartObj->AddComponent<CFont>("GameOptionFont");
		pGameStartFont->SetFont("돋움");
		pGameStartFont->SetText(L"게임 옵션");
		pGameStartFont->SetArea(45, 0, 200, 39.f);

		SAFE_RELEASE(pGameStartFont);
		SAFE_RELEASE(pGameStartObj);

		SAFE_RELEASE(pUILayer);
	}


	//{
	//	CLayer*	pUILayer = m_pScene->GetLayer("UI");

	//	CGameObject*	pButtonObj = CGameObject::CreateObject("Join", pUILayer);

	//	CUIButton*	pButton = pButtonObj->AddComponent<CUIButton>("Button");

	//	pButton->SetCallback(this, &LoginScene::login_JOIN_callback);

	//	SAFE_RELEASE(pButton);

	//	CTransform*	pButtonTr = pButtonObj->GetTransform();

	//	//pButtonTr->SetPivot(0.5f, 0.5f, 0.f);
	//	pButtonTr->SetWorldScale(200.f, 100.f, 1.f);
	//	pButtonTr->SetWorldPos(DEVICE_RESOLUTION.iWidth / 2.f + 50.f,
	//		DEVICE_RESOLUTION.iHeight / 2.f + 100.f, 0.f);

	//	SAFE_RELEASE(pButtonTr);

	//	CRenderer2D* pRenderer = pButtonObj->FindComponentFromType<CRenderer2D>(CT_RENDERER2D);

	//	CMaterial* pMaterial = pRenderer->GetMaterial();

	//	pMaterial->SetDiffuseTexInfo("Linear", "Join",
	//		0, 0, L"Join.bmp");

	//	SAFE_RELEASE(pMaterial);

	//	SAFE_RELEASE(pRenderer);

	//	CColliderRect* pRC = pButtonObj->FindComponentFromType<CColliderRect>(CT_COLLIDER);

	//	pRC->SetRect(0, 0, 200, 100);

	//	SAFE_RELEASE(pRC);

	//	SAFE_RELEASE(pButtonObj);

	//	SAFE_RELEASE(pUILayer);
	//}

	////GET_SINGLE(CScheduler)->AddSchedule<LoginScene>("GameStart",
	////	SO_ONCE, this, &LoginScene::ScheduleStartCallback,
	////	5.f, 0.f, 0);


	//{

	//	{
	//		CLayer*	pLayer = m_pScene->GetLayer("UI");
	//		this->pEditIDObject = CGameObject::CreateObject("UIIDText", pLayer);

	//		CUIButton*	pButton = pEditIDObject->AddComponent<CUIButton>("Login_ID_Edit");
	//		pButton->SetCallback(this, &LoginScene::login_ID_callback);
	//		SAFE_RELEASE(pButton);

	//		CTransform*	pButtonTr = pEditIDObject->GetTransform();

	//		//pButtonTr->SetPivot(0.5f, 0.5f, 0.f);
	//		pButtonTr->SetWorldScale(200.f, 50.f, 1.f);
	//		pButtonTr->SetWorldPos(DEVICE_RESOLUTION.iWidth / 2.f - 100.f,
	//			DEVICE_RESOLUTION.iHeight / 2.f - 120.f, 0.f);

	//		SAFE_RELEASE(pButtonTr);
	//		CRenderer2D* pRenderer = pEditIDObject->FindComponentFromType<CRenderer2D>(CT_RENDERER2D);
	//		CMaterial* pMaterial = pRenderer->GetMaterial();

	//		pMaterial->SetDiffuseTexInfo("Linear", "LoginIDEdit",
	//			0, 0, L"LoginEdit.bmp");

	//		SAFE_RELEASE(pMaterial);
	//		SAFE_RELEASE(pRenderer);

	//		CColliderRect* pRC = pEditIDObject->FindComponentFromType<CColliderRect>(CT_COLLIDER);

	//		pRC->SetRect(0, 0, 200, 50);

	//		SAFE_RELEASE(pRC);

	//		pUIEditText = pEditIDObject->AddComponent<CFont>("TextUI");
	//		pUIEditText->SetFont("나눔고딕");
	//		pEditIDString = L"채팅 테스트";
	//		pUIEditText->SetText(pEditIDString);
	//		pUIEditText->SetArea(0, 0, 200, 30.f);
	//	}


	//	{
	//		CLayer*	pLayer = m_pScene->GetLayer("UI");
	//		this->pEditPWObject = CGameObject::CreateObject("UIJoinText", pLayer);

	//		CUIButton*	pButton = pEditPWObject->AddComponent<CUIButton>("Login_Join_Edit");
	//		pButton->SetCallback(this, &LoginScene::login_PW_callback);
	//		SAFE_RELEASE(pButton);

	//		CTransform*	pButtonTr = pEditPWObject->GetTransform();
	//		//pButtonTr->SetPivot(0.5f, 0.5f, 0.f);
	//		pButtonTr->SetWorldScale(200.f, 50.f, 1.f);
	//		pButtonTr->SetWorldPos(DEVICE_RESOLUTION.iWidth / 2.f - 100.f,
	//			DEVICE_RESOLUTION.iHeight / 2.f, 0.f);

	//		SAFE_RELEASE(pButtonTr);
	//		CRenderer2D* pRenderer = pEditPWObject->FindComponentFromType<CRenderer2D>(CT_RENDERER2D);
	//		CMaterial* pMaterial = pRenderer->GetMaterial();

	//		pMaterial->SetDiffuseTexInfo("Linear", "LoginPWEdit",
	//			0, 0, L"LoginEdit.bmp");

	//		SAFE_RELEASE(pMaterial);
	//		SAFE_RELEASE(pRenderer);

	//		CColliderRect* pRC = pEditPWObject->FindComponentFromType<CColliderRect>(CT_COLLIDER);

	//		pRC->SetRect(0, 0, 200, 50);

	//		SAFE_RELEASE(pRC);

	//		pUIEditText_PW = pEditPWObject->AddComponent<CFont>("TextUI");
	//		pUIEditText_PW->SetFont("나눔고딕");
	//		pEditPWString = L"채팅 테스트";

	//		pUIEditText_PW->SetText(pEditPWString);
	//		pUIEditText_PW->SetArea(0, 0, 200, 30.f);

	//	}
	//}



	NetworkManager::getInstance()->initialize();

	return true;
}

int LoginScene::Update(float fTime)
{
	Vector3 pBaseLoginWindowUIPos;
	if (0 != UICont.count("LoginWindowUI"))
	{
		CTransform* pBaseLoginWindowUITr = UICont["LoginWindowUI"][0]->GetTransform();
		pBaseLoginWindowUIPos = pBaseLoginWindowUITr->GetWorldPos();
		SAFE_RELEASE(pBaseLoginWindowUITr);
		for (int index = 1; index < UICont["LoginWindowUI"].size(); ++index)
		{
			CTransform* pTr = UICont["LoginWindowUI"][index]->GetTransform();
			Vector3 pos = pBaseLoginWindowUIPos;
			pos = pos + pTr->getOffset();
			pTr->SetWorldPos(pos);
			SAFE_RELEASE(pTr);
		}
	}

	{
		wstring edit_view = L"";
		for (auto input : ID_inputCont)

		{
			//wchar_t temp[64];
			wchar_t temp2 = input;
			wstring appendString = L"";
			appendString = appendString + temp2;
			edit_view = edit_view + appendString;
		}


		this->pEditIDString = edit_view;
		if(nullptr != pUIEditText)
			this->pUIEditText->SetText(pEditIDString);
	}

	{
		wstring edit_view = L"";
		for (auto input : PW_inputCont)
		{
			//wchar_t temp[64];
			wchar_t temp2 = input;
			wstring appendString = L"";
			appendString = appendString + temp2;
			edit_view = edit_view + appendString;
		}
		this->pEditPWString = edit_view;

		if (nullptr != pUIEditText_PW)
			this->pUIEditText_PW->SetText(pEditPWString);
	}

	while (false == NetworkManager::getInstance()->getClientPacketQueue().empty())
	{
		void* packet = NetworkManager::getInstance()->fetchToClientPacket();

		if (nullptr == packet)
			continue;

		char packet_type = reinterpret_cast<char*>(packet)[2];

		switch (packet_type)
		{
		case SC_PACKET_CONNECT:
		{
			sc_packet_connect* pPacket = reinterpret_cast<sc_packet_connect*>(packet);
			int id = pPacket->id;

			if (false == NetworkManager::getInstance()->getIsConnected())
			{
				NetworkManager::getInstance()->enableConnected(true);
				NetworkManager::getInstance()->setMyClientID(id);
			}
		}
		break;
		case SC_PACKET_JOIN:
		{
			sc_packet_login* pPacket = reinterpret_cast<sc_packet_login*>(packet);
			if (pPacket->state == PACKETSTATE::PACKETSTATE_JOIN_SUCCESS)
			{
				MessageBox(NULL, L"가입에 성공하습니다.\n로그인 하십시오.", L"가입 성공", MB_OK);
			}
			else if (pPacket->state == PACKETSTATE::PACKETSTATE_JOIN_FAIL)
			{
				MessageBox(NULL, L"이미 해당 아이디가 존재합니다.", L"가입 실패", MB_OK);
			}

		}
		break;
		case SC_PACKET_LOGIN:
		{
			sc_packet_login* login_packet = reinterpret_cast<sc_packet_login*>(packet);

			// 로그인 성공시 씬 전환.
			if (PACKETSTATE::PACKETSTATE_LOGIN_SUCCESS == login_packet->state)
			{
				GET_SINGLE(CScheduler)->DeleteSchedule("GameStart");

				CScene*	pScene = GET_SINGLE(CSceneManager)->CreateNextScene("MainScene");

				pScene->CreateSceneScript<CMainScene>("MainScene", false);

				//SAFE_RELEASE(pMainScene);

				SAFE_RELEASE(pScene);
			}
			else if (PACKETSTATE::PACKETSTATE_LOGIN_ID_FAIL == login_packet->state)
			{
				MessageBox(NULL, L"아이디가 존재하지 않습니다.", L"로그인 실패", MB_OK);
			}
			else if (PACKETSTATE::PACKETSTATE_LOGIN_PW_FAIL == login_packet->state)
			{
				MessageBox(NULL, L"비밀번호가 틀립니다.", L"로그인 실패", MB_OK);
			}
		};
		}
		NetworkManager::getInstance()->popData();
		free(packet);
	}

	return 0;
}

void LoginScene::gameConnectCallBack(float time)
{
	Vector3 BaseUIPos;
	/// < BaseUI >
	{
		CLayer*	pUILayer = m_pScene->GetLayer("UI+1");
		UICont.insert(make_pair("LoginWindowUI", vector<CGameObject*>{}));

		CGameObject*	pLoginBaseUIObj = CGameObject::CreateObject("LoginBaseUI", pUILayer);
		UICont["LoginWindowUI"].push_back(pLoginBaseUIObj);

		CUIPanel*	pGameStartUI = pLoginBaseUIObj->AddComponent<CUIPanel>("LoginBaseUI");
		pGameStartUI->SetTitleArea(0, 0, 260, 30);
		SAFE_RELEASE(pGameStartUI);

		CTransform*	pLoginBaseUITr = pLoginBaseUIObj->GetTransform();

		//pButtonTr->SetPivot(0.5f, 0.5f, 0.f);
		pLoginBaseUITr->SetWorldScale(360.f, 180.f, 1.f);
		pLoginBaseUITr->SetWorldPos(DEVICE_RESOLUTION.iWidth / 2.f - 165.f,
			DEVICE_RESOLUTION.iHeight / 2.f - 50.f, 0.f);
		BaseUIPos = pLoginBaseUITr->GetWorldPos();
		SAFE_RELEASE(pLoginBaseUITr);

		CRenderer2D* pRenderer = pLoginBaseUIObj->FindComponentFromType<CRenderer2D>(CT_RENDERER2D);

		CMaterial* pMaterial = pRenderer->GetMaterial();

		pMaterial->SetDiffuseTexInfo("Linear", "LoginBaseUI",
			0, 0, L"UserInterface\\UI_BASIC_BOX_BIG.png");

		SAFE_RELEASE(pMaterial);

		SAFE_RELEASE(pRenderer);

		CColliderRect* pRC = pLoginBaseUIObj->FindComponentFromType<CColliderRect>(CT_COLLIDER);

		pRC->SetRect(0, 0, 360, 20);

		SAFE_RELEASE(pRC);
		SAFE_RELEASE(pLoginBaseUIObj);
		SAFE_RELEASE(pUILayer);
	}

		/// < LoginBaseUICloseButton >
	{
		CLayer*	pUILayer = m_pScene->GetLayer("UI+2");
		CGameObject*	pLoginBaseUICloseObj = CGameObject::CreateObject("LoginBaseUIClose", pUILayer);
		UICont["LoginWindowUI"].push_back(pLoginBaseUICloseObj);
		CUIButton*	pGameStartUI = pLoginBaseUICloseObj->AddComponent<CUIButton>("LoginBaseUIClose");
		pGameStartUI->SetCallback(this, &LoginScene::closeGameConnectUICallBack);
		//pGameStartUI->SetCallback(this, &LoginScene::StartCallback);

		SAFE_RELEASE(pGameStartUI);

		CTransform*	pLoginBaseUICloseTr = pLoginBaseUICloseObj->GetTransform();

		//pButtonTr->SetPivot(0.5f, 0.5f, 0.f);
		pLoginBaseUICloseTr->SetWorldScale(30.f, 30.f, 1.f);
		pLoginBaseUICloseTr->SetWorldPos(BaseUIPos.x + 330.f,
			BaseUIPos.y, 0.f);

		Vector3 offset = Vector3(330.f, 0.f, 0.f);
		pLoginBaseUICloseTr->setOffset(offset);

		SAFE_RELEASE(pLoginBaseUICloseTr);

		CRenderer2D* pRenderer = pLoginBaseUICloseObj->FindComponentFromType<CRenderer2D>(CT_RENDERER2D);

		CMaterial* pMaterial = pRenderer->GetMaterial();

		pMaterial->SetDiffuseTexInfo("Linear", "LoginBaseUIClose",
			0, 0, L"UserInterface\\UI_MENU_CHECK_X.png");

		SAFE_RELEASE(pMaterial);

		SAFE_RELEASE(pRenderer);

		CColliderRect* pRC = pLoginBaseUICloseObj->FindComponentFromType<CColliderRect>(CT_COLLIDER);

		pRC->SetRect(0, 0, 30.f, 30.f);

		SAFE_RELEASE(pRC);
		SAFE_RELEASE(pLoginBaseUICloseObj);

		SAFE_RELEASE(pUILayer);
	}

	/// < LoginID_Edit >
	{
		CLayer*	pUILayer = m_pScene->GetLayer("UI+2");
		CGameObject*	pLoginIDObj = CGameObject::CreateObject("LoginID_Edit", pUILayer);
		UICont["LoginWindowUI"].push_back(pLoginIDObj);
		CUIButton*	pGameStartUI = pLoginIDObj->AddComponent<CUIButton>("LoginID_Edit");
		pGameStartUI->SetCallback(this, &LoginScene::loginIDEditCallback);

		SAFE_RELEASE(pGameStartUI);

		CTransform*	pLoginIDTr = pLoginIDObj->GetTransform();

		//pButtonTr->SetPivot(0.5f, 0.5f, 0.f);
		pLoginIDTr->SetWorldScale(200.f, 50.f, 1.f);
		pLoginIDTr->SetWorldPos(BaseUIPos.x+130, BaseUIPos.y+20, 0.f);

		Vector3 offset = Vector3(130.f, 20.f, 0.f);
		pLoginIDTr->setOffset(offset);

		SAFE_RELEASE(pLoginIDTr);

		CRenderer2D* pRenderer = pLoginIDObj->FindComponentFromType<CRenderer2D>(CT_RENDERER2D);

		CMaterial* pMaterial = pRenderer->GetMaterial();

		pMaterial->SetDiffuseTexInfo("Linear", "LoginID_Edit",
			0, 0, L"UserInterface\\UI_BASIC_CIRCLE_LONG.png");

		SAFE_RELEASE(pMaterial);

		SAFE_RELEASE(pRenderer);

		CColliderRect* pRC = pLoginIDObj->FindComponentFromType<CColliderRect>(CT_COLLIDER);

		pRC->SetRect(0, 10, 200.f, 30.f);

		SAFE_RELEASE(pRC);

		CFont* pGameStartFont = pLoginIDObj->AddComponent<CFont>("IDFont");
		pGameStartFont->SetFont("돋움25");
		pGameStartFont->SetText(L"아이디:");
		pGameStartFont->SetArea(-85, 0, 200, 35.f);

		SAFE_RELEASE(pGameStartFont);

		CFont* IDFont = pLoginIDObj->AddComponent<CFont>("IDEditFont");
		IDFont->SetFont("나눔고딕");
		IDFont->SetText(L"");
		IDFont->SetArea(10, 0, 200, 35.f);
		pUIEditText = IDFont;
		//SAFE_RELEASE(IDFont);

		SAFE_RELEASE(pLoginIDObj);
		SAFE_RELEASE(pUILayer);
	}

	/// < LoginPW_Edit >
	{
		CLayer*	pUILayer = m_pScene->GetLayer("UI+2");
		CGameObject*	pLoginPWObj = CGameObject::CreateObject("LoginPW_Edit", pUILayer);
		UICont["LoginWindowUI"].push_back(pLoginPWObj);
		CUIButton*	pGameStartUI = pLoginPWObj->AddComponent<CUIButton>("LoginPW_Edit");
		pGameStartUI->SetCallback(this, &LoginScene::loginPWEditCallback);

		SAFE_RELEASE(pGameStartUI);

		CTransform*	pLoginPWTr = pLoginPWObj->GetTransform();

		//pButtonTr->SetPivot(0.5f, 0.5f, 0.f);
		pLoginPWTr->SetWorldScale(200.f, 50.f, 1.f);
		pLoginPWTr->SetWorldPos(BaseUIPos.x + 130, BaseUIPos.y + 70, 0.f);

		Vector3 offset = Vector3(130.f, 70.f, 0.f);
		pLoginPWTr->setOffset(offset);

		SAFE_RELEASE(pLoginPWTr);

		CRenderer2D* pRenderer = pLoginPWObj->FindComponentFromType<CRenderer2D>(CT_RENDERER2D);

		CMaterial* pMaterial = pRenderer->GetMaterial();

		pMaterial->SetDiffuseTexInfo("Linear", "LoginPW_Edit",
			0, 0, L"UserInterface\\UI_BASIC_CIRCLE_LONG.png");

		SAFE_RELEASE(pMaterial);

		SAFE_RELEASE(pRenderer);

		CColliderRect* pRC = pLoginPWObj->FindComponentFromType<CColliderRect>(CT_COLLIDER);

		pRC->SetRect(0, 10, 200.f, 30.f);

		SAFE_RELEASE(pRC);

		CFont* pIDFont = pLoginPWObj->AddComponent<CFont>("PWFont");
		pIDFont->SetFont("돋움25");
		pIDFont->SetText(L"패스워드:");
		pIDFont->SetArea(-110, 0, 200, 35.f);

		SAFE_RELEASE(pIDFont);

		CFont* PWFont = pLoginPWObj->AddComponent<CFont>("PWEditFont");
		PWFont->SetFont("나눔고딕");
		PWFont->SetText(L"");
		PWFont->SetArea(10, 0, 200, 35.f);
		pUIEditText_PW = PWFont;
		//SAFE_RELEASE(PWFont);

		SAFE_RELEASE(pLoginPWObj);

		SAFE_RELEASE(pUILayer);
	}

	/// < LoginButton >
	{
		CLayer*	pUILayer = m_pScene->GetLayer("UI+2");
		CGameObject*	pLoginButtonObj = CGameObject::CreateObject("LoginButton", pUILayer);
		UICont["LoginWindowUI"].push_back(pLoginButtonObj);
		CUIButton*	pLoginButtonUI = pLoginButtonObj->AddComponent<CUIButton>("LoginButton");
		pLoginButtonUI->SetCallback(this, &LoginScene::connectGameServer);

		SAFE_RELEASE(pLoginButtonUI);

		CTransform*	pLoginButtonTr = pLoginButtonObj->GetTransform();

		//pButtonTr->SetPivot(0.5f, 0.5f, 0.f);
		pLoginButtonTr->SetWorldScale(150.f, 50.f, 1.f);
		pLoginButtonTr->SetWorldPos(BaseUIPos.x + 30, BaseUIPos.y + 120, 0.f);

		Vector3 offset = Vector3(30.f, 120.f, 0.f);
		pLoginButtonTr->setOffset(offset);

		SAFE_RELEASE(pLoginButtonTr);

		CRenderer2D* pRenderer = pLoginButtonObj->FindComponentFromType<CRenderer2D>(CT_RENDERER2D);

		CMaterial* pMaterial = pRenderer->GetMaterial();

		pMaterial->SetDiffuseTexInfo("Linear", "LoginButton",
			0, 0, L"UserInterface\\UI_SYSTEM_BOX.png");

		SAFE_RELEASE(pMaterial);

		SAFE_RELEASE(pRenderer);

		CColliderRect* pRC = pLoginButtonObj->FindComponentFromType<CColliderRect>(CT_COLLIDER);

		pRC->SetRect(0, 0, 150.f, 50.f);

		SAFE_RELEASE(pRC);

		CFont* pLoginButtonFont = pLoginButtonObj->AddComponent<CFont>("IDFont");
		pLoginButtonFont->SetFont("돋움25");
		pLoginButtonFont->SetText(L"로그인");
		pLoginButtonFont->SetArea(30, 0, 200, 35.f);

		SAFE_RELEASE(pLoginButtonFont);

		SAFE_RELEASE(pLoginButtonObj);
		SAFE_RELEASE(pUILayer);
	}
}

void LoginScene::closeGameConnectUICallBack(float time)
{
	for (auto& UI : UICont["LoginWindowUI"])
	{
		UI->Die();
	}
	SAFE_RELEASE(pUIEditText);
	SAFE_RELEASE(pUIEditText_PW);
	pUIEditText = nullptr;
	pUIEditText_PW = nullptr;

	UICont.erase("LoginWindowUI");
}

void LoginScene::loginIDEditCallback(float fTime)
{
	this->id_write = true;
	this->pw_write = false;
}

void LoginScene::loginPWEditCallback(float fTime)
{
	this->id_write = false;
	this->pw_write = true;
}

void LoginScene::login_JOIN_callback(float fTime)
{
	cs_packet_login* packet = reinterpret_cast<cs_packet_login*>(NetworkManager::getInstance()->getSendBuffer());
	packet->id = NetworkManager::getInstance()->getMyClientID();
	packet->size = sizeof(cs_packet_login);
	packet->type = CS_PACKET_JOIN;
	wcscpy_s(packet->login_id, pEditIDString.c_str());
	packet->login_id[MAX_STR_SIZE] = 0;
	wcscpy_s(packet->login_pw, pEditPWString.c_str());
	packet->login_pw[MAX_STR_SIZE] = 0;

	NetworkManager::getInstance()->getSendWsaBuf().len = sizeof(cs_packet_login);

	DWORD iobyte;
	WSASend(NetworkManager::getInstance()->getSocket(), &NetworkManager::getInstance()->getSendWsaBuf(), 1, &iobyte, 0, NULL, NULL);
}

void LoginScene::connectGameServer(float fTime)
{
	cs_packet_login* packet = reinterpret_cast<cs_packet_login*>(NetworkManager::getInstance()->getSendBuffer());
	packet->id = NetworkManager::getInstance()->getMyClientID();
	packet->size = sizeof(cs_packet_login);
	packet->type = CS_PACKET_LOGIN;
	wcscpy_s(packet->login_id, pEditIDString.c_str());
	packet->login_id[MAX_STR_SIZE] = 0;
	wcscpy_s(packet->login_pw, pEditPWString.c_str());
	packet->login_pw[MAX_STR_SIZE] = 0;

	NetworkManager::getInstance()->getSendWsaBuf().len = sizeof(cs_packet_login);

	DWORD iobyte;
	int retval = WSASend(NetworkManager::getInstance()->getSocket(), &NetworkManager::getInstance()->getSendWsaBuf(), 1, &iobyte, 0, NULL, NULL);
	if (retval == SOCKET_ERROR)
	{
		MessageBoxA(NULL, "connectGameServer() SendError", "Error", MB_OK);
	}



	//GET_SINGLE(CScheduler)->DeleteSchedule("GameStart");

	//CScene*	pScene = GET_SINGLE(CSceneManager)->CreateNextScene("MainScene");

	//CMainScene*	pMainScene = pScene->CreateSceneScript<CMainScene>("MainScene");

	//SAFE_RELEASE(pMainScene);

	//SAFE_RELEASE(pScene);
}

void LoginScene::ScheduleStartCallback(float fTime)
{
	/*CScene*	pScene = GET_SINGLE(CSceneManager)->CreateNextScene("MainScene");

	CMainScene*	pMainScene = pScene->CreateSceneScript<CMainScene>("MainScene");

	SAFE_RELEASE(pMainScene);

	SAFE_RELEASE(pScene);*/
}
