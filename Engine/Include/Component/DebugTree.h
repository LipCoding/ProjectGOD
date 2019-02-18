#pragma once
#include "../Resources/Mesh.h"
#include "../Rendering/Shader.h"
#include "../Scene/Scene.h"

PG_BEGIN

class PG_DLL DebugTree
{
private:
	friend class CGameObject;

public:
	DebugTree();
	DebugTree(const DebugTree& debugTree);
	~DebugTree();
	
	bool Init();
	void Update(float fTime);
	void Render(float fTime);

	void SetPosition(float posX, float posZ, float width);
	void Release();

private:
	Vector3				m_vLength;
	Vector3				m_vPos;

	CScene*				m_pScene = nullptr;
	CMesh*				m_pMesh = nullptr;
	CShader*			m_pShader = nullptr;
	ID3D11InputLayout*  m_pLayout = nullptr;
	TRANSFORMCBUFFER    m_tTransform;
};

PG_END

