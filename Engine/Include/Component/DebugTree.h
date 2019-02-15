#pragma once
#include "Component.h"

PG_BEGIN

class PG_DLL DebugTree
{
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
	
	bool Initialize(ID3D11Device*, float, float, float);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

private:
	bool InitializeBuffer(ID3D11Device*);

private:
	float m_posX = 0.f;
	float m_posZ = 0.f;
	float m_radian = 0.f;
	ID3D11Buffer* m_vertexBuffer = nullptr;
	ID3D11Buffer* m_indexBuffer = nullptr;
};

PG_END

