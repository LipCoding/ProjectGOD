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
	// �ε��� ���۸� �����մϴ�.
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// ���ؽ� ���۸� �����մϴ�.
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}
}

void DebugTree::Render(ID3D11DeviceContext * deviceContext)
{
	// ���� ���� ���� �� �������� �����մϴ�.
	unsigned int stride = sizeof(VertexType);
	unsigned int offset = 0;

	// ������ �� �� �ֵ��� �Է� ��������� ���� ���۸� Ȱ������ �����մϴ�.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// ������ �� �� �ֵ��� �Է� ��������� �ε��� ���۸� Ȱ������ �����մϴ�.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// �� ���ؽ� ���ۿ��� �������Ǿ���ϴ� ������Ƽ���� Ÿ���� �����Ѵ�.�� ��� ���θ���Ʈ�̴�.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

}

bool DebugTree::InitializeBuffer(ID3D11Device * device)
{
	int vertexCount = 8;
	int indexCount = vertexCount;

	// ���� �迭�� ����ϴ�.
	VertexType* vertices = new VertexType[vertexCount];
	if (!vertices)
	{
		return false;
	}

	// �ε��� �迭�� ����ϴ�.
	unsigned long* indices = new unsigned long[indexCount];
	if (!indices)
	{
		return false;
	}

	// Line1
	// ���� �Ʒ�
	vertices[0].position.x = m_posX - m_radian;
	vertices[0].position.y = 6.f;
	vertices[0].position.z = m_posZ - m_radian;
	
	// ���� ��
	vertices[1].position.x = m_posX - m_radian;
	vertices[1].position.y = 6.f;
	vertices[1].position.z = m_posZ + m_radian;

	// Line2
	// ���� ��
	vertices[2].position.x = m_posX - m_radian;
	vertices[2].position.y = 6.f;
	vertices[2].position.z = m_posZ + m_radian;

	// ������ ��
	vertices[3].position.x = m_posX + m_radian;
	vertices[3].position.y = 6.f;
	vertices[3].position.z = m_posZ + m_radian;

	// Line3
	// ������ ��
	vertices[4].position.x = m_posX + m_radian;
	vertices[4].position.y = 6.f;
	vertices[4].position.z = m_posZ + m_radian;

	// ������ �Ʒ�
	vertices[5].position.x = m_posX + m_radian;
	vertices[5].position.y = 6.f;
	vertices[5].position.z = m_posZ - m_radian;

	// Line4
	// ������ �Ʒ�
	vertices[6].position.x = m_posX + m_radian;
	vertices[6].position.y = 6.f;
	vertices[6].position.z = m_posZ - m_radian;

	// ���� �Ʒ�
	vertices[7].position.x = m_posX - m_radian;
	vertices[7].position.y = 6.f;
	vertices[7].position.z = m_posZ - m_radian;

	for (int i = 0; i < 8; i++)
	{
		vertices[i].color = XMFLOAT4(1.f, 1.f, 1.f, 1.f);
		indices[i] = i;
	}

	// ���� ���� ������ ����ü�� �����Ѵ�.
	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// subresource ������ ���� �����Ϳ� ���� �����͸� �����մϴ�.
	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// ���� ���� ���۸� ����ϴ�.
	if (FAILED(device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer)))
	{
		return false;
	}

	// ���� �ε��� ������ ����ü�� �����մϴ�.
	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// ���� ���ҽ� ������ �ε��� �����Ϳ� ���� �����͸� �����մϴ�.
	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// �ε��� ���۸� ����ϴ�.
	if (FAILED(device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer)))
	{
		return false;
	}

	// ���� ���۰� �����ǰ� �ε�� �迭�� �����Ͻʽÿ�.
	delete[] vertices;
	vertices = nullptr;

	delete[] indices;
	indices = nullptr;

	return true;
}
