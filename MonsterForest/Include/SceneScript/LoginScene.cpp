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

LoginScene::LoginScene()
{
}

LoginScene::~LoginScene()
{
}

bool LoginScene::Init()
{
	CLayer*	pUILayer = m_pScene->GetLayer("UI");

	CGameObject*	pBackPanel = CGameObject::CreateObject("StartSceneBack", pUILayer);

	CTransform*	pBackTr = pBackPanel->GetTransform();

	pBackTr->SetWorldScale(DEVICE_RESOLUTION.iWidth,
		DEVICE_RESOLUTION.iHeight, 1.f);

	SAFE_RELEASE(pBackTr);

	CRenderer2D*	pRenderer = pBackPanel->AddComponent<CRenderer2D>("Renderer");

	pRenderer->SetShader(UI_SHADER);
	pRenderer->SetInputLayout("VertexTex");
	pRenderer->SetMesh("RectOrthoLTTex");
	pRenderer->SetRenderState(ALPHA_BLEND);

	/*pRenderer->CreateCBuffer("Button", 11, sizeof(BUTTONCBUFFER),
		SCT_VERTEX | SCT_PIXEL);*/

	CMaterial* pMaterial = pRenderer->CreateMaterial(SAMPLER_LINEAR,
		"Yasuo", 0, 0, L"Panel.jpg");

	SAFE_RELEASE(pMaterial);

	SAFE_RELEASE(pRenderer);

	SAFE_RELEASE(pBackPanel);

	CGameObject*	pButtonObj = CGameObject::CreateObject("Button", pUILayer);

	CUIButton*	pButton = pButtonObj->AddComponent<CUIButton>("Button");

	pButton->SetCallback(this, &LoginScene::StartCallback);

	SAFE_RELEASE(pButton);

	CTransform*	pButtonTr = pButtonObj->GetTransform();

	//pButtonTr->SetPivot(0.5f, 0.5f, 0.f);
	pButtonTr->SetWorldScale(200.f, 100.f, 1.f);
	pButtonTr->SetWorldPos(DEVICE_RESOLUTION.iWidth / 2.f - 200.f,
		DEVICE_RESOLUTION.iHeight / 2.f + 100.f, 0.f);

	SAFE_RELEASE(pButtonTr);

	pRenderer = pButtonObj->FindComponentFromType<CRenderer2D>(CT_RENDERER2D);

	pMaterial = pRenderer->GetMaterial();

	pMaterial->SetDiffuseTexInfo("Linear", "StartButton",
		0, 0, L"GameStart.bmp");

	SAFE_RELEASE(pMaterial);

	SAFE_RELEASE(pRenderer);

	CColliderRect* pRC = pButtonObj->FindComponentFromType<CColliderRect>(CT_COLLIDER);

	pRC->SetRect(0, 0, 200, 100);

	SAFE_RELEASE(pRC);

	SAFE_RELEASE(pButtonObj);

	SAFE_RELEASE(pUILayer);



	{
		CLayer*	pUILayer = m_pScene->GetLayer("UI");

		CGameObject*	pButtonObj = CGameObject::CreateObject("Join", pUILayer);

		CUIButton*	pButton = pButtonObj->AddComponent<CUIButton>("Button");

		pButton->SetCallback(this, &LoginScene::login_JOIN_callback);

		SAFE_RELEASE(pButton);

		CTransform*	pButtonTr = pButtonObj->GetTransform();

		//pButtonTr->SetPivot(0.5f, 0.5f, 0.f);
		pButtonTr->SetWorldScale(200.f, 100.f, 1.f);
		pButtonTr->SetWorldPos(DEVICE_RESOLUTION.iWidth / 2.f + 50.f,
			DEVICE_RESOLUTION.iHeight / 2.f + 100.f, 0.f);

		SAFE_RELEASE(pButtonTr);

		pRenderer = pButtonObj->FindComponentFromType<CRenderer2D>(CT_RENDERER2D);

		pMaterial = pRenderer->GetMaterial();

		pMaterial->SetDiffuseTexInfo("Linear", "Join",
			0, 0, L"Join.bmp");

		SAFE_RELEASE(pMaterial);

		SAFE_RELEASE(pRenderer);

		CColliderRect* pRC = pButtonObj->FindComponentFromType<CColliderRect>(CT_COLLIDER);

		pRC->SetRect(0, 0, 200, 100);

		SAFE_RELEASE(pRC);

		SAFE_RELEASE(pButtonObj);

		SAFE_RELEASE(pUILayer);
	}

	//GET_SINGLE(CScheduler)->AddSchedule<LoginScene>("GameStart",
	//	SO_ONCE, this, &LoginScene::ScheduleStartCallback,
	//	5.f, 0.f, 0);


	{

		{
			CLayer*	pLayer = m_pScene->GetLayer("UI");
			this->pEditIDObject = CGameObject::CreateObject("UIIDText", pLayer);

			CUIButton*	pButton = pEditIDObject->AddComponent<CUIButton>("Login_ID_Edit");
			pButton->SetCallback(this, &LoginScene::login_ID_callback);
			SAFE_RELEASE(pButton);

			CTransform*	pButtonTr = pEditIDObject->GetTransform();

			//pButtonTr->SetPivot(0.5f, 0.5f, 0.f);
			pButtonTr->SetWorldScale(200.f, 50.f, 1.f);
			pButtonTr->SetWorldPos(DEVICE_RESOLUTION.iWidth / 2.f - 100.f,
				DEVICE_RESOLUTION.iHeight / 2.f - 120.f, 0.f);

			SAFE_RELEASE(pButtonTr);
			CRenderer2D* pRenderer = pEditIDObject->FindComponentFromType<CRenderer2D>(CT_RENDERER2D);
			CMaterial* pMaterial = pRenderer->GetMaterial();

			pMaterial->SetDiffuseTexInfo("Linear", "LoginIDEdit",
				0, 0, L"LoginEdit.bmp");

			SAFE_RELEASE(pMaterial);
			SAFE_RELEASE(pRenderer);

			CColliderRect* pRC = pEditIDObject->FindComponentFromType<CColliderRect>(CT_COLLIDER);

			pRC->SetRect(0, 0, 200, 50);

			SAFE_RELEASE(pRC);

			pUIEditText = pEditIDObject->AddComponent<CFont>("TextUI");
			pUIEditText->SetFont("나눔고딕");
			pEditIDString = L"채팅 테스트";
			pUIEditText->SetText(pEditIDString);
			pUIEditText->SetArea(0, 0, 200, 30.f);
		}


		{
			CLayer*	pLayer = m_pScene->GetLayer("UI");
			this->pEditPWObject = CGameObject::CreateObject("UIJoinText", pLayer);

			CUIButton*	pButton = pEditPWObject->AddComponent<CUIButton>("Login_Join_Edit");
			pButton->SetCallback(this, &LoginScene::login_PW_callback);
			SAFE_RELEASE(pButton);

			CTransform*	pButtonTr = pEditPWObject->GetTransform();
			//pButtonTr->SetPivot(0.5f, 0.5f, 0.f);
			pButtonTr->SetWorldScale(200.f, 50.f, 1.f);
			pButtonTr->SetWorldPos(DEVICE_RESOLUTION.iWidth / 2.f - 100.f,
				DEVICE_RESOLUTION.iHeight / 2.f, 0.f);

			SAFE_RELEASE(pButtonTr);
			CRenderer2D* pRenderer = pEditPWObject->FindComponentFromType<CRenderer2D>(CT_RENDERER2D);
			CMaterial* pMaterial = pRenderer->GetMaterial();

			pMaterial->SetDiffuseTexInfo("Linear", "LoginPWEdit",
				0, 0, L"LoginEdit.bmp");

			SAFE_RELEASE(pMaterial);
			SAFE_RELEASE(pRenderer);

			CColliderRect* pRC = pEditPWObject->FindComponentFromType<CColliderRect>(CT_COLLIDER);

			pRC->SetRect(0, 0, 200, 50);

			SAFE_RELEASE(pRC);

			pUIEditText_PW = pEditPWObject->AddComponent<CFont>("TextUI");
			pUIEditText_PW->SetFont("나눔고딕");
			pEditPWString = L"채팅 테스트";

			pUIEditText_PW->SetText(pEditPWString);
			pUIEditText_PW->SetArea(0, 0, 200, 30.f);

		}
	}



	NetworkManager::getInstance()->initialize();

	return true;
}

int LoginScene::Update(float fTime)
{
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

				pScene->CreateSceneScript<CMainScene>("MainScene");

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

void LoginScene::login_ID_callback(float fTime)
{
	this->id_write = true;
	this->pw_write = false;


}

void LoginScene::login_PW_callback(float fTime)
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

void LoginScene::StartCallback(float fTime)
{

	///// < id를 읽어온다. >
	//wstring login_id = L"";
	//{
	//	const auto& login_id_cont = this->edit_id->getInputCont();

	//	for (auto input : login_id_cont)
	//	{
	//		wchar_t temp[64];
	//		wchar_t temp2 = input;
	//		wstring appendString = L"";
	//		appendString = appendString + temp2;
	//		login_id = login_id + appendString;
	//	}
	//}

	///// < pw를 읽어온다. >
	//wstring login_pw = L"";
	//{
	//	const auto& login_pw_cont = this->edit_pw->getInputCont();

	//	for (auto input : login_pw_cont)
	//	{
	//		wchar_t temp[64];
	//		wchar_t temp2 = input;
	//		wstring appendString = L"";
	//		appendString = appendString + temp2;
	//		login_pw = login_pw + appendString;
	//	}
	//}


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
	WSASend(NetworkManager::getInstance()->getSocket(), &NetworkManager::getInstance()->getSendWsaBuf(), 1, &iobyte, 0, NULL, NULL);




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
