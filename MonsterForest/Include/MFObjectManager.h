#pragma once

#include "GameObject/GameObject.h"
#include "NetworkManager.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Scene/Layer.h"
#include "Component/ColliderSphere.h"
#include "Component/Renderer.h"
#include "Component/Transform.h"
#include "ObjectScript/Player.h"
#include "ObjectScript/Golem.h"
#include "UserInterfaceManager.h"
#include "ObjectScript/Player.h"
#include "ObjectScript/Golem.h"

PG_USING

class MFObjectManager :
	public Singleton<MFObjectManager>
{
public:
	MFObjectManager();
	MFObjectManager(const MFObjectManager&);
	MFObjectManager(MFObjectManager&&);
	void operator=(const MFObjectManager&);
	void operator=(MFObjectManager&&);
public:
	bool initialize();
public:
	template<typename ActorType>
	ActorType* CreateActor(const string& layer_name, const string& object_name, const string& prefab_name, bool my_player = false);
private:
	void protoTypeSetting();
	void staticObjectSetting();
};

template<typename ActorType>
inline ActorType* MFObjectManager::CreateActor(const string& layer_name, const string& object_name, const string& prototype_name, bool my_player)
{
	
	CLayer*	default_layer = GET_SINGLE(CSceneManager)->GetCurrentScene()->GetLayer(layer_name);
	
	CGameObject* new_object = CGameObject::FindObject(object_name);
	
	/// < 임시 코드 / 서버쪽 확인하기 >
	if (nullptr != new_object)
		return new_object->FindComponentFromTypeName<ActorType>("Actor");

	new_object = CGameObject::CreateClone(prototype_name, default_layer);
	new_object->SetTag(object_name);

	CColliderSphere* collider_component = new_object->AddComponent<CColliderSphere>("collider");
	collider_component->SetSphere(Vector3(0.f, 1.f, 0.f), 1.f);
	SAFE_RELEASE(collider_component);
	
	ActorType*	player_component = new_object->AddComponent<Actor, ActorType>("Actor");
	SAFE_RELEASE(default_layer);

	return player_component;
}