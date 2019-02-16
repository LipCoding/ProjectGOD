#include "DebugTree.h"
#include "../GameObject/GameObject.h"
#include "../Resources/ResourcesManager.h"
#include "../Resources/Mesh.h"
#include "../Component/Renderer.h"
#include "../Scene/SceneManager.h"
#include "../Scene/Scene.h"
#include "../Scene/Layer.h"
#include "../Component/Material.h"

PG_USING

int DebugTree::g_iDebugNum = 100000;

DebugTree::DebugTree()
{

}

DebugTree::DebugTree(const DebugTree &)
{
}


DebugTree::~DebugTree()
{
}

bool DebugTree::Initialize()
{
	CScene* pScene = GET_SINGLE(CSceneManager)->GetCurrentScene();
	CLayer* pLayer = pScene->GetLayer("Default");
	m_pGameObject = CGameObject::CreateObject("DebugQuadTree", pLayer);
	SAFE_RELEASE(pLayer);
	SAFE_RELEASE(pScene);

	return true;
}

bool DebugTree::MakeMesh(float posX, float posZ, float radian)
{
	m_posX = posX;
	m_posZ = posZ;
	m_radian = radian;

	return InitializeBuffer();
}

void DebugTree::Shutdown()
{
	SAFE_RELEASE(m_pGameObject);
}

void DebugTree::Render(ID3D11DeviceContext * deviceContext)
{
}

bool DebugTree::InitializeBuffer()
{
	int vertexCount = 8;
	int indexCount = vertexCount;

	// 정점 배열을 만듭니다.
	/*VertexType* vertices = new VertexType[vertexCount];
	if (!vertices)
	{
		return false;
	}*/

	vector<VERTEXCOLOR> vertices;
	vertices.resize(vertexCount);

	// 인덱스 배열을 만듭니다.
	/*unsigned long* indices = new unsigned long[indexCount];
	if (!indices)
	{
		return false;
	}*/

	vector<UINT> indices;
	indices.resize(vertexCount);

	// Line1
	// 왼쪽 아래
	vertices[0].vPos.x = m_posX - m_radian;
	vertices[0].vPos.y = 6.f;
	vertices[0].vPos.z = m_posZ - m_radian;
	
	// 왼쪽 위
	vertices[1].vPos.x = m_posX - m_radian;
	vertices[1].vPos.y = 6.f;
	vertices[1].vPos.z = m_posZ + m_radian;

	// Line2
	// 왼쪽 위
	vertices[2].vPos.x = m_posX - m_radian;
	vertices[2].vPos.y = 6.f;
	vertices[2].vPos.z = m_posZ + m_radian;

	// 오른쪽 위
	vertices[3].vPos.x = m_posX + m_radian;
	vertices[3].vPos.y = 6.f;
	vertices[3].vPos.z = m_posZ + m_radian;

	// Line3
	// 오른쪽 위
	vertices[4].vPos.x = m_posX + m_radian;
	vertices[4].vPos.y = 6.f;
	vertices[4].vPos.z = m_posZ + m_radian;

	// 오른쪽 아래
	vertices[5].vPos.x = m_posX + m_radian;
	vertices[5].vPos.y = 6.f;
	vertices[5].vPos.z = m_posZ - m_radian;

	// Line4
	// 오른쪽 아래
	vertices[6].vPos.x = m_posX + m_radian;
	vertices[6].vPos.y = 6.f;
	vertices[6].vPos.z = m_posZ - m_radian;

	// 왼쪽 아래
	vertices[7].vPos.x = m_posX - m_radian;
	vertices[7].vPos.y = 6.f;
	vertices[7].vPos.z = m_posZ - m_radian;

	for (int i = 0; i < 8; i++)
	{
		vertices[i].vColor = XMFLOAT4(1.f, 1.f, 1.f, 1.f);
		indices[i] = i;
	}

	CRenderer*	pRenderer = m_pGameObject->AddComponent<CRenderer>(to_string(g_iDebugNum));
	CMaterial*	pMaterial = pRenderer->CreateMaterial();

	SAFE_RELEASE(pMaterial);

	CMesh* pMesh = GET_SINGLE(CResourcesManager)->CreateMesh(to_string(g_iDebugNum),
		vertices.size(), sizeof(VERTEXCOLOR), D3D11_USAGE_DEFAULT, D3D11_PRIMITIVE_TOPOLOGY_LINELIST,
		&vertices[0], indices.size(), 4, D3D11_USAGE_DEFAULT,
		DXGI_FORMAT_R32_UINT, &indices[0]);

	pMesh->SetShaderKey(STANDARD_COLOR_SHADER);
	pRenderer->SetMesh(pMesh);
	pRenderer->SetRenderState(CULLING_NONE);
	 
	SAFE_RELEASE(pMesh);
	SAFE_RELEASE(pRenderer);

	++g_iDebugNum;

	//_cprintf("%d\n", g_iDebugNum);

	//// 정적 정점 버퍼의 구조체를 설정한다.
	//D3D11_BUFFER_DESC vertexBufferDesc;
	//vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	//vertexBufferDesc.ByteWidth = sizeof(VertexType) * vertexCount;
	//vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	//vertexBufferDesc.CPUAccessFlags = 0;
	//vertexBufferDesc.MiscFlags = 0;
	//vertexBufferDesc.StructureByteStride = 0;

	//// subresource 구조에 정점 데이터에 대한 포인터를 제공합니다.
	//D3D11_SUBRESOURCE_DATA vertexData;
	//vertexData.pSysMem = vertices;
	//vertexData.SysMemPitch = 0;
	//vertexData.SysMemSlicePitch = 0;

	//// 이제 정점 버퍼를 만듭니다.
	//if (FAILED(device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer)))
	//{
	//	return false;
	//}

	//// 정적 인덱스 버퍼의 구조체를 설정합니다.
	//D3D11_BUFFER_DESC indexBufferDesc;
	//indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	//indexBufferDesc.ByteWidth = sizeof(unsigned long) * indexCount;
	//indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	//indexBufferDesc.CPUAccessFlags = 0;
	//indexBufferDesc.MiscFlags = 0;
	//indexBufferDesc.StructureByteStride = 0;

	//// 하위 리소스 구조에 인덱스 데이터에 대한 포인터를 제공합니다.
	//D3D11_SUBRESOURCE_DATA indexData;
	//indexData.pSysMem = indices;
	//indexData.SysMemPitch = 0;
	//indexData.SysMemSlicePitch = 0;

	//// 인덱스 버퍼를 만듭니다.
	//if (FAILED(device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer)))
	//{
	//	return false;
	//}

	//// 이제 버퍼가 생성되고 로드된 배열을 해제하십시오.
	//delete[] vertices;
	//vertices = nullptr;

	//delete[] indices;
	//indices = nullptr;

	return true;
}