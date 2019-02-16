#pragma once
#include "Component.h"

PG_BEGIN

class PG_DLL DebugTree
{
private:
	friend class CGameObject;

private:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT4 color;
	};

public:
	DebugTree();
	DebugTree(const DebugTree&);
	~DebugTree();
	
	bool Initialize();
	bool MakeMesh(float, float, float);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

private:
	bool InitializeBuffer();

private:
	float m_posX = 0.f;
	float m_posZ = 0.f;
	float m_radian = 0.f;
	static int g_iDebugNum;
	CGameObject* m_pGameObject = nullptr;
};

PG_END

