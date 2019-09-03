#include "QuestBaseUI.h"
#include "Core/PathManager.h"
#include "Scene/Layer.h"
#include "GameObject/GameObject.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Component/Renderer2D.h"
#include "Component/ColliderRect.h"
#include "Component/Material.h"
#include "Component/Transform.h"
#include "QuestBaseUI.h"
#include "Component/Font.h"
#include "QuestManager.h"
#include "QuestItem.h"

QuestBaseUI::QuestBaseUI()
{
}


QuestBaseUI::~QuestBaseUI()
{
}

bool QuestBaseUI::initialize()
{
	{

		CScene* pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
		CLayer*	pLayer = pScene->GetLayer("UI_QUEST+1");

		QuestUIBackGroundObject = CGameObject::CreateObject("QuestListBackGround", pLayer);
		QuestUIBackGroundObject->Enable(false);
		CUIPanel* quest_ui_component = QuestUIBackGroundObject->AddComponent<CUIPanel>("QuestListBackGround");
		//quest_ui_component->initialize();

		CRenderer2D* pRenderer = QuestUIBackGroundObject->FindComponentFromType<CRenderer2D>(CT_RENDERER2D);
		CMaterial* pMaterial = pRenderer->GetMaterial();

		pMaterial->SetDiffuseTexInfo("Linear", "QuestListBackGround",
			0, 0, L"UserInterface/UI_INVEN_BOX_3.png");

		CTransform* pDropTableTr = QuestUIBackGroundObject->GetTransform();
		pDropTableTr->SetWorldScale(450, 500.f, 1.f);
		pDropTableTr->SetWorldPos(150.f, 150.f, 1.f);

		SAFE_RELEASE(pDropTableTr);
		SAFE_RELEASE(pMaterial);
		SAFE_RELEASE(pRenderer);
		SAFE_RELEASE(pLayer);
		SAFE_RELEASE(pScene);
	}

	{

		CScene* pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
		CLayer*	pLayer = pScene->GetLayer("UI_QUEST+1");

		quest_view_object = CGameObject::CreateObject("QuestView", pLayer);
		quest_view_object->Enable(false);
		CUIPanel* quest_ui_component = quest_view_object->AddComponent<CUIPanel>("QuestViewBackGround");
		//quest_ui_component->initialize();

		CRenderer2D* pRenderer = quest_view_object->FindComponentFromType<CRenderer2D>(CT_RENDERER2D);
		CMaterial* pMaterial = pRenderer->GetMaterial();

		pMaterial->SetDiffuseTexInfo("Linear", "QuestListBackGround",
			0, 0, L"UserInterface/UI_INVEN_BOX_3.png");

		CTransform* pDropTableTr = quest_view_object->GetTransform();
		pDropTableTr->SetWorldScale(430, 500.f, 1.f);
		pDropTableTr->SetWorldPos(620.f, 150.f, 1.f);

		CFont* contents_text_subject = quest_view_object->AddComponent<CFont>("quest_ui_text_subject");
		contents_text_subject->SetFont("맑은고딕25N");
		contents_text_subject->SetArea(50, 20, 750, 100.f);
		contents_text_subject->SetText(L"폰트테스트");

		CFont* contents_text_summary = quest_view_object->AddComponent<CFont>("quest_ui_text_summary");
		contents_text_summary->SetFont("맑은고딕20N");
		contents_text_summary->SetArea(50, 70, 750, 300.f);
		contents_text_summary->SetText(L"폰트테스트");

		CFont* contents_text_contents = quest_view_object->AddComponent<CFont>("quest_ui_text_contents");
		contents_text_contents->SetFont("맑은고딕N");
		contents_text_contents->SetArea(50, 150, 680, 400.f);
		contents_text_contents->SetText(L"폰트테스트");

		SAFE_RELEASE(pDropTableTr);
		SAFE_RELEASE(pMaterial);
		SAFE_RELEASE(pRenderer);
		SAFE_RELEASE(pLayer);
		SAFE_RELEASE(pScene);
	}
	CScene* pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
	CLayer*	pLayer = pScene->GetLayer("UI_QUEST+2");

	reward_item_text_object = CGameObject::CreateObject("reward_text", pLayer);
	reward_item_text_object->AddComponent<CUIButton>("reward_text");
	reward_item_text_object->Enable(false); 
	CFont* contents_text_summary = reward_item_text_object->AddComponent<CFont>("보상아이템");
	contents_text_summary->SetFont("맑은고딕20N");
	contents_text_summary->SetArea(650, 450, 850, 450.f);
	contents_text_summary->SetText(L"보상 아이템");

	//for(int i = 0; i<8; ++i)
	//{
	//	CScene* pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
	//	CLayer*	pLayer = pScene->GetLayer("UI_QUEST+2");

	//	CGameObject* QuestItemObject = CGameObject::CreateObject("QuestItem", pLayer);

	//	CUIPanel* quest_ui_component = QuestItemObject->AddComponent<CUIPanel>("QuestItem");
	//	//quest_ui_component->initialize();

	//	CRenderer2D* pRenderer = QuestItemObject->FindComponentFromType<CRenderer2D>(CT_RENDERER2D);
	//	CMaterial* pMaterial = pRenderer->GetMaterial();

	//	pMaterial->SetDiffuseTexInfo("Linear", "QuestItem",
	//		0, 0, L"UserInterface/UI_INVEN_BOX_1.png");
	//	int offset = quest_list.size();

	//	CTransform* pDropTableTr = QuestItemObject->GetTransform();
	//	pDropTableTr->SetWorldScale(400, 50.f, 1.f);
	//	pDropTableTr->SetWorldPos(175.f, 180.f + (offset*55), 1.f);

	//	SAFE_RELEASE(pDropTableTr);
	//	SAFE_RELEASE(pMaterial);
	//	SAFE_RELEASE(pRenderer);
	//	//SAFE_RELEASE(QuestUIObject);
	//	SAFE_RELEASE(pLayer);
	//	SAFE_RELEASE(pScene);
	//	quest_list.push_back(QuestItemObject);
	//}

	 //#######
	/*Quest* quest = QuestManager::getInstance()->findQuest("The Archiereus Of Flame");
	current_quest = quest;
	{
		CScene* pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
		CLayer*	pLayer = pScene->GetLayer("UI_QUEST+2");

		CGameObject* QuestItemObject = CGameObject::CreateObject("QuestItem", pLayer);

		QuestItem* quest_item_component = QuestItemObject->AddComponent<QuestItem>("QuestItem");
		quest_item_component->setQuest(quest);
		quest_item_component->setQuestBaseUI(this);

		CRenderer2D* pRenderer = QuestItemObject->FindComponentFromType<CRenderer2D>(CT_RENDERER2D);
		CMaterial* pMaterial = pRenderer->GetMaterial();

		pMaterial->SetDiffuseTexInfo("Linear", "QuestItem",
			0, 0, L"UserInterface/UI_INVEN_BOX_1.png");
		int offset = quest_list.size();

		CTransform* pDropTableTr = QuestItemObject->GetTransform();
		pDropTableTr->SetWorldScale(400, 50.f, 1.f);
		pDropTableTr->SetWorldPos(175.f, 180.f + (offset * 55), 1.f);
		wstring temp_text = strconv(quest->getQuestSubject());
		CFont* quest_item_text = QuestItemObject->AddComponent<CFont>("quest_ui_text_contents");
		quest_item_text->SetFont("맑은고딕N");
		quest_item_text->SetArea(50, 15.f, 680, 400.f);
		quest_item_text->SetText(temp_text);

		CColliderRect* pRC = QuestItemObject->FindComponentFromType<CColliderRect>(CT_COLLIDER);
		pRC->SetTag("quest_slot");
		pRC->SetRect(0, 0, 400.f, 50.f);

		SAFE_RELEASE(pRC);

		SAFE_RELEASE(pDropTableTr);
		SAFE_RELEASE(pMaterial);
		SAFE_RELEASE(pRenderer);
		SAFE_RELEASE(pLayer);
		SAFE_RELEASE(pScene);
		quest_list.push_back(QuestItemObject);
	}*/

	//updateQuestView();
	return true;
}

void QuestBaseUI::updateQuestView()
{

	// 폰트에 연결된 텍스트 변경.
	if (current_quest != nullptr)
	{
		{
			CFont* contents_text = this->quest_view_object->FindComponentFromTag<CFont>("quest_ui_text_subject");
			wstring quest_subject_text = strconv(current_quest->getQuestSubject());
			contents_text->SetText(quest_subject_text);
		}
		//
		{
			CFont* contents_text = quest_view_object->FindComponentFromTag<CFont>("quest_ui_text_summary");
			wstring quest_summary_text = strconv(current_quest->getQuestSummary());
			contents_text->SetText(quest_summary_text);
		}

		{
			CFont* contents_text = quest_view_object->FindComponentFromTag<CFont>("quest_ui_text_contents");
			wstring quest_summary_text = strconv(current_quest->getQuestContents());
			contents_text->SetText(quest_summary_text);
		}
	}

	for (REWARD_ITEM& reward_item : reward_list)
	{
		reward_item.first->Die();
		reward_item.second->Die();
	}
	reward_list.clear();
	reward_list.resize(0);
	// 첫번째 아이템 보상 표시
	for (int i = 0; i < current_quest->getRewardItems().size(); ++i)
	{
		float x;
		float y;
		if (i == 0)
		{
			x = 650;
			y = 500;

		}
		else if (i == 1)
		{
			x = 850;
			y = 500;
		}
		else if (i == 2)
		{
			x = 650;
			y = 550;
		}
		else if (i == 3)
		{
			x = 850;
			y = 550;
		}
		// 아이템 보상 테두리
		CGameObject* item_reward_border = nullptr;
		{
			CScene* pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
			CLayer*	pLayer = pScene->GetLayer("UI_QUEST+2");

			item_reward_border = CGameObject::CreateObject("item", pLayer);
			item_reward_border->AddComponent<CUIButton>("ContentsScroll");

			CRenderer2D* pRenderer = item_reward_border->FindComponentFromType<CRenderer2D>(CT_RENDERER2D);
			CMaterial* pMaterial = pRenderer->GetMaterial();

			CTransform* pDropTableTr = item_reward_border->GetTransform();
			pDropTableTr->SetWorldScale(150, 35.f, 1.f);
			pDropTableTr->SetWorldPos(x, y, 1.f);

			pMaterial->SetDiffuseTexInfo("Linear", "border",
				0, 0, L"UserInterface\\border.png");
		}

		// 아이템 보상 목록에 추가.
		CGameObject* reward_item_object = nullptr;
		{
			CScene* pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
			CLayer*	pLayer = pScene->GetLayer("UI_QUEST+2");

			reward_item_object = CGameObject::CreateObject("itemIcon", pLayer);
			reward_item_object->AddComponent<CUIButton>("ContentsScroll");

			CRenderer2D* pRenderer = reward_item_object->FindComponentFromType<CRenderer2D>(CT_RENDERER2D);
			CMaterial* pMaterial = pRenderer->GetMaterial();

			CTransform* pDropTableTr = reward_item_object->GetTransform();
			pDropTableTr->SetWorldScale(32.f, 32.f, 1.f);
			pDropTableTr->SetWorldPos(x, y, 1.f);
			wstring item_text = L"ItemIcon\\";

			item_text += strconv(current_quest->getRewardItems()[0]);
			pMaterial->SetDiffuseTexInfo("Linear", current_quest->getRewardItems()[0].c_str(),
				0, 0, item_text.c_str());

			CFont* contents_text_summary = reward_item_object->AddComponent<CFont>("보상아이템");
			contents_text_summary->SetFont("맑은고딕N");
			contents_text_summary->SetArea(50, 0, 200, 20.f);
			wstring item_text_name = strconv(current_quest->getRewardItems()[0]);
			contents_text_summary->SetText(item_text_name);

		}
		reward_list.emplace_back(REWARD_ITEM{item_reward_border, reward_item_object});
	}
}

void QuestBaseUI::enableShow(bool ui_show)
{
	this->ui_show = ui_show;
	m_pGameObject->Enable(ui_show);
	quest_view_object->Enable(ui_show);

	reward_item_text_object->Enable(ui_show);

	QuestUIBackGroundObject->Enable(ui_show);
	for (const auto& quest_item_object : quest_list)
		quest_item_object->Enable(ui_show);
	for (REWARD_ITEM& reward_item : reward_list)
	{
		reward_item.first->Enable(ui_show);
		reward_item.second->Enable(ui_show);
	}
}
