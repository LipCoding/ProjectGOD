#include "DebugTree.h"

PG_USING

DebugTree::DebugTree()
{
}

DebugTree::DebugTree(const DebugTree &)
{
}


DebugTree::~DebugTree()
{
}

bool DebugTree::Initialize(ID3D11Device * device, float posX, float posZ, float radian)
{
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = nullptr;
	}

	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = nullptr;
	}

	m_posX = posX;
	m_posZ = posZ;
	m_radian = radian;

	return InitializeBuffer(device);
}

void DebugTree::Shutdown()
{
	// 인덱스 버퍼를 해제합니다.
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// 버텍스 버퍼를 해제합니다.
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}
}

void DebugTree::Render(ID3D11DeviceContext * deviceContext)
{
	// 정점 버퍼 보폭 및 오프셋을 설정합니다.
	unsigned int stride = sizeof(VertexType);
	unsigned int offset = 0;

	// 렌더링 할 수 있도록 입력 어셈블러에서 정점 버퍼를 활성으로 설정합니다.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// 렌더링 할 수 있도록 입력 어셈블러에서 인덱스 버퍼를 활성으로 설정합니다.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// 이 버텍스 버퍼에서 렌더링되어야하는 프리미티브의 타입을 설정한다.이 경우 라인리스트이다.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

}

bool DebugTree::InitializeBuffer(ID3D11Device * device)
{
	int vertexCount = 8;
	int indexCount = vertexCount;

	// 정점 배열을 만듭니다.
	VertexType* vertices = new VertexType[vertexCount];
	if (!vertices)
	{
		return false;
	}

	// 인덱스 배열을 만듭니다.
	unsigned long* indices = new unsigned long[indexCount];
	if (!indices)
	{
		return false;
	}

	// Line1
	// 왼쪽 아래
	vertices[0].position.x = m_posX - m_radian;
	vertices[0].position.y = 6.f;
	vertices[0].position.z = m_posZ - m_radian;
	
	// 왼쪽 위
	vertices[1].position.x = m_posX - m_radian;
	vertices[1].position.y = 6.f;
	vertices[1].position.z = m_posZ + m_radian;

	// Line2
	// 왼쪽 위
	vertices[2].position.x = m_posX - m_radian;
	vertices[2].position.y = 6.f;
	vertices[2].position.z = m_posZ + m_radian;

	// 오른쪽 위
	vertices[3].position.x = m_posX + m_radian;
	vertices[3].position.y = 6.f;
	vertices[3].position.z = m_posZ + m_radian;

	// Line3
	// 오른쪽 위
	vertices[4].position.x = m_posX + m_radian;
	vertices[4].position.y = 6.f;
	vertices[4].position.z = m_posZ + m_radian;

	// 오른쪽 아래
	vertices[5].position.x = m_posX + m_radian;
	vertices[5].position.y = 6.f;
	vertices[5].position.z = m_posZ - m_radian;

	// Line4
	// 오른쪽 아래
	vertices[6].position.x = m_posX + m_radian;
	vertices[6].position.y = 6.f;
	vertices[6].position.z = m_posZ - m_radian;

	// 왼쪽 아래
	vertices[7].position.x = m_posX - m_radian;
	vertices[7].position.y = 6.f;
	vertices[7].position.z = m_posZ - m_radian;

	for (int i = 0; i < 8; i++)
	{
		vertices[i].color = XMFLOAT4(1.f, 1.f, 1.f, 1.f);
		indices[i] = i;
	}

	// 정적 정점 버퍼의 구조체를 설정한다.
	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// subresource 구조에 정점 데이터에 대한 포인터를 제공합니다.
	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// 이제 정점 버퍼를 만듭니다.
	if (FAILED(device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer)))
	{
		return false;
	}

	// 정적 인덱스 버퍼의 구조체를 설정합니다.
	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// 하위 리소스 구조에 인덱스 데이터에 대한 포인터를 제공합니다.
	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// 인덱스 버퍼를 만듭니다.
	if (FAILED(device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer)))
	{
		return false;
	}

	// 이제 버퍼가 생성되고 로드된 배열을 해제하십시오.
	delete[] vertices;
	vertices = nullptr;

	delete[] indices;
	indices = nullptr;

	return true;
}
