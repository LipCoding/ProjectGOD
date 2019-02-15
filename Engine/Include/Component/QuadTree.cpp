#include "QuadTree.h"
#include "../Device.h"

PG_USING

CQuadTree::CQuadTree()
	:m_bVisualCheck(false),
	 m_iTriCount(0),
	 m_iDrawCount(0),
	 m_pParentNode(nullptr),
	 m_pDebugTree(nullptr)
{
}

CQuadTree::CQuadTree(const CQuadTree & quadTree)
{
}

CQuadTree::~CQuadTree()
{
	//���� Ʈ�� �����͸� ��������� ����
	if (m_pParentNode)
	{
		ReleaseNode(m_pParentNode);
		delete m_pParentNode;
		m_pParentNode = nullptr;
	}

	delete m_pDebugTree;
	m_pDebugTree = nullptr;
}

bool CQuadTree::CreateQuadTree(CLandScape * terrain)
{
	float fCenterX = 0.f;
	float fCenterZ = 0.f;
	float fWidth = 0.f;

	m_vecVtx = terrain->getVecVtx();

	// ���� ���� �迭�� ���� ���� �����´�.
	int vertexCount = m_vecVtx.size();
	
	// ���� ����Ʈ�� �� �ﰢ�� ���� ����
	m_iTriCount = vertexCount / 3;

	// �߽� x, z�� �޽��� �ʺ� ���
	this->CalculateMeshDimensions(vertexCount, fCenterX, fCenterZ, fWidth);

	// ���� Ʈ���� �θ� ��带 �����.
	m_pParentNode = new NodeType;
	if (!m_pParentNode)
	{
		return false;
	}

	// ���� ��� �����Ϳ� �޽� ������ ������� ���� Ʈ���� ��������� �����Ѵ�.
	this->CreateTreeNode(m_pParentNode, fCenterX, fCenterZ, fWidth);

	// ���� Ʈ���� �� ��忡 ������ ���� ������ ���� ����� ���´�.
	if (!m_vecVtx.empty())
	{
		m_vecVtx.clear();
	}

	return true;
}

void CQuadTree::CalculateMeshDimensions(int vtxCount, float & centerX, float & centerZ, float & meshWidth)
{
	// �޽��� �߽� ��ġ�� 0���� �ʱ�ȭ
	centerX = 0.f;
	centerZ = 0.f;

	// �޽��� ��� ������ ��ģ��.
	for (const auto iter : m_vecVtx)
	{
		centerX += iter.vPos.x;
		centerZ += iter.vPos.z;
	}

	// �޽��� �߰� ���� ã�� ���� ������ ���� ������.
	centerX = centerX / (float)vtxCount;
	centerZ = centerZ / (float)vtxCount;

	// �޽��� �ִ� �� �ּ� ũ�⸦ �ʱ�ȭ �Ѵ�.
	float fMaxWidth = 0.f;
	float fMaxDepth = 0.f;

	float fMinWidth = fabsf(m_vecVtx[0].vPos.x - centerX);
	float fMinDepth = fabsf(m_vecVtx[0].vPos.z - centerZ);
	
	// ��� ������ ���캸�� �޽��� �ִ� �ʺ�� �ּ� �ʺ�� ���̸� ã�´�.
	for (const auto iter : m_vecVtx)
	{
		float fWidth = fabsf(iter.vPos.x - centerX);
		float fDepth = fabsf(iter.vPos.z - centerZ);

		if (fWidth > fMaxWidth) { fMaxWidth = fWidth; }
		if (fDepth > fMaxDepth) { fMaxDepth = fDepth; }
		if (fWidth < fMinWidth) { fMinWidth = fWidth; }
		if (fDepth < fMinDepth) { fMinDepth = fDepth; }
	}

	// �ּҿ� �ִ� ���̿� �ʺ� ������ ���� �ִ밪�� ã�´�.
	float fMaxX = (float)max(fabs(fMaxWidth), fabs(fMaxWidth));
	float fMaxZ = (float)max(fabs(fMaxDepth), fabs(fMaxDepth));

	// �޽��� �ִ� ������ ���
	meshWidth = max(fMaxX, fMaxZ) * 2.f;
}

void CQuadTree::CreateTreeNode(NodeType * node, float positionX, float positionZ, float width)
{
	// ����� ��ġ�� ũ�⸦ ����
	node->fPositionX = positionX;
	node->fPositionZ = positionZ;
	node->fWidth = width;

	// ����� �ﰢ�� ���� 0���� �ʱ�ȭ
	node->iTriCount = 0;

	// ���� �� �ε��� ���۸� null�� �ʱ�ȭ
	node->vertexBuffer = nullptr;
	node->indexBuffer = nullptr;

	// �� ����� �ڽ� ��带 null�� �ʱ�ȭ
	node->nodes[0] = nullptr;
	node->nodes[1] = nullptr;
	node->nodes[2] = nullptr;
	node->nodes[3] = nullptr;

	// �� ��� �ȿ� �ִ� �ﰢ�� ���� ����.
	int iNumTriangles = this->CountTriangles(positionX, positionZ, width);

	// 1. �� ��忡 �ﰢ���� ������ ����ִ� �����̹Ƿ� return
	if (iNumTriangles == 0)
	{
		return;
	}

	// 2. �� ��忡 �ʹ� ���� �ﰢ���� �ִ� ��� 4 ���� ������ ũ���� 
	// �� ���� Ʈ�� ���� �����Ѵ�.
	if (iNumTriangles > MAX_TRIANGLES)
	{
		for (int i = 0; i < 4; i++)
		{
			// ���ο� �ڽ� ��忡 ���� ��ġ �������� ���
			float fOffsetX = (((i % 2) < 1) ? -1.f : 1.f) * (width / 4.f);
			float fOffsetZ = (((i % 4) < 2) ? -1.f : 1.f) * (width / 4.f);

			// �� ��忡 �ﰢ���� �ִ��� Ȯ���Ѵ�.
			int iCount = this->CountTriangles((positionX + fOffsetX), (positionZ + fOffsetZ), (width / 2.f));
			if (iCount > 0)
			{
				// �� �� ��尡 �ִ� �ﰢ���� �ִ� ��� �ڽ� ���� �����.
				node->nodes[i] = new NodeType;

				// ���� �� �ڽ� ��忡�� �����ϴ� Ʈ���� Ȯ���Ѵ�.
				this->CreateTreeNode(node->nodes[i], (positionX + fOffsetX), (positionZ + fOffsetZ), (width / 2.f));
			}
		}
		return;
	}

	// 3. �� ��尡 ��� ���� �ʰ� �� ����� �ﰢ�� ���� �ִ� ������ ������
	// �� ���� Ʈ���� �� �Ʒ��� �����Ƿ� ������ �ﰢ�� ����� �����.
	node->iTriCount = iNumTriangles;

	// ������ ���� ���
	int iVertexCount = iNumTriangles * 3;

	// ���� �迭�� �����.()
	node->vecVtx.resize(iVertexCount);
	node->vecIndex.resize(iVertexCount);

	// ���� �迭�� �����.
	VERTEXBUMP* vertices = new VERTEXBUMP[iVertexCount];
	UINT* indices = new UINT[iVertexCount];

	int iIndex = 0;
	int iVertexIndex = 0;

	for (int i = 0; i < m_iTriCount; i++)
	{
		// �ﰢ���� �� ��� �ȿ� ������ ������ �迭�� �߰��Ѵ�.
		if (this->IsTriangleCountaiend(i, positionX, positionZ, width))
		{
			// ���� ���ؽ� ��Ͽ� �ε����� ����Ѵ�.
			iVertexIndex = i * 3;

			// ���� ��Ͽ��� �� �ﰢ���� �� �������� �����´�.
			node->vecVtx[iIndex] = m_vecVtx[iVertexIndex];
			node->vecIndex[iIndex] = iIndex;
			vertices[iIndex] = m_vecVtx[iVertexIndex];
			indices[iIndex] = iIndex;
			iIndex++;

			iVertexIndex++;
			node->vecVtx[iIndex] = m_vecVtx[iVertexIndex];
			node->vecIndex[iIndex] = iIndex;
			vertices[iIndex] = m_vecVtx[iVertexIndex];
			indices[iIndex] = iIndex;
			iIndex++;

			iVertexIndex++;
			node->vecVtx[iIndex] = m_vecVtx[iVertexIndex];
			node->vecIndex[iIndex] = iIndex;
			vertices[iIndex] = m_vecVtx[iVertexIndex];
			indices[iIndex] = iIndex;
			iIndex++;
		}
	}

	// ���� ���� ����ü ����
	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VERTEXBUMP) * iVertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// subresource ������ ���� �����Ϳ� ���� �����͸� ����
	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	DEVICE->CreateBuffer(&vertexBufferDesc, &vertexData, &node->vertexBuffer);

	// �ε��� ������ ������ ����
	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(UINT) * iVertexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// ���� ���ҽ� ������ �ε��� �����Ϳ� ���� ������ ����
	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	DEVICE->CreateBuffer(&indexBufferDesc, &indexData, &node->indexBuffer);

	delete[] vertices;
	vertices = nullptr;

	delete[] indices;
	indices = nullptr;
}

int CQuadTree::CountTriangles(float positionX, float positionZ, float width)
{
	// ī��Ʈ�� 0���� �ʱ�ȭ �Ѵ�.
	int count = 0;

	// ��ü �޽��� ��� �ﰢ���� ���캸�� � ��尡
	// �� ��� �ȿ� �־�� �ϴ��� Ȯ���Ѵ�.
	for (int i = 0; i < m_iTriCount; i++)
	{
		// �ﰢ���� �� ��� �ȿ� ������ 1�� ������Ų��.
		if (this->IsTriangleCountaiend(i, positionX, positionZ, width))
		{
			count++;
		}
	}

	return count;
}

bool CQuadTree::IsTriangleCountaiend(int index, float positionX, float positionZ, float width)
{
	// �� ����� �ݰ��� ����Ѵ�.
	float fRadius = width / 2.f;

	// �ε����� ���� ������� �����´�.
	int iVertexIndex = index * 3;

	// ���� ��Ͽ��� �� �ﰢ���� �� �������� �����´�.
	float fX1 = m_vecVtx[iVertexIndex].vPos.x;
	float fZ1 = m_vecVtx[iVertexIndex].vPos.z;
	iVertexIndex++;

	float fX2 = m_vecVtx[iVertexIndex].vPos.x;
	float fZ2 = m_vecVtx[iVertexIndex].vPos.z;
	iVertexIndex++;

	float fX3 = m_vecVtx[iVertexIndex].vPos.x;
	float fZ3 = m_vecVtx[iVertexIndex].vPos.z;

	// �ﰢ���� x��ǥ�� �ּҰ��� ��� �ȿ� �ִ��� Ȯ��
	float fMinimumX = min(fX1, min(fX2, fX3));
	if (fMinimumX > (positionX + fRadius))
	{
		return false;
	}

	// �ﰢ���� x��ǥ�� �ִ밪�� ��� �ȿ� �ִ��� Ȯ��
	float fMaximumX = max(fX1, max(fX2, fX3));
	if (fMaximumX < (positionX - fRadius))
	{
		return false;
	}

	// �ﰢ���� z��ǥ�� �ּҰ��� ��� �ȿ� �ִ��� Ȯ��
	float fMinimumZ = min(fZ1, min(fZ2, fZ3));
	if (fMinimumX > (positionZ + fRadius))
	{
		return false;
	}

	// �ﰢ���� z��ǥ�� �ִ밪�� ��� �ȿ� �ִ��� Ȯ��
	float fMaximumZ = max(fZ1, max(fZ2, fZ3));
	if (fMaximumZ < (positionZ - fRadius))
	{
		return false;
	}

	return true;
}

void CQuadTree::ReleaseNode(NodeType* node)
{
	for (int i = 0; i < 4; i++)
	{
		if (node->nodes[i])
		{
			ReleaseNode(node->nodes[i]);
		}
	}

	if (node->vertexBuffer)
	{
		node->vertexBuffer->Release();
		node->vertexBuffer = nullptr;
	}

	if (node->indexBuffer)
	{
		node->indexBuffer->Release();
		node->indexBuffer = nullptr;
	}

	for (int i = 0; i < 4; i++)
	{
		if (node->nodes[i])
		{
			delete node->nodes[i];
			node->nodes[i] = nullptr;
		}
	}
}

void CQuadTree::RenderDebug(NodeType * node)
{
	int count = 0;
	for (int i = 0; i < 4; i++)
	{
		if (node->nodes[i] != 0)
		{
			count++;
			RenderDebug(node->nodes[i]);
		}
	}

	// �ڽ� ��尡 �ִ� ���
	if (count != 0)
	{
		return;
	}

	m_pDebugTree->Initialize(DEVICE, node->fPositionX, node->fPositionZ, node->fWidth / 2.f);
	m_pDebugTree->Render(CONTEXT);
	
}

void CQuadTree::NodeCheck(NodeType * node, float positionX, float positionZ)
{
	// �ƴѰ��
	if (!this->CollideCheck(node->fPositionX, node->fPositionZ, node->fWidth,
		positionX, positionZ))
	{
		return;
	}

	int count = 0;
	for (int i = 0; i < 4; i++)
	{
		if (node->nodes[i] != 0)
		{
			count++;
			NodeCheck(node->nodes[i], positionX, positionZ);
		}
	}

	// �ڽ� ��尡 �ִ� ���
	if (count != 0)
	{
		return;
	}

	// ���� ���

}

bool CQuadTree::CollideCheck(float nodePosX, float nodePosZ, float width, float positionX, float positionZ)
{
	float fMinX, fMaxX, fMinZ, fMaxZ, fRadius;

	fRadius = width / 2.f;

	fMinX = nodePosX - width;
	fMaxX = nodePosX + width;
	fMinZ = nodePosZ - width;
	fMaxZ = nodePosZ + width;

	if (positionX < fMinX || positionX > fMaxX)
		return false;

	if (positionZ < fMinZ || positionZ > fMaxZ)
		return false;

	return true;
}

bool CQuadTree::Init()
{
	m_pDebugTree = new DebugTree;

	return true;
}

void CQuadTree::Input(float fTime)
{
}

int CQuadTree::Update(float fTime)
{
	RenderDebug(m_pParentNode);

	return 0;
}

int CQuadTree::LateUpdate(float fTime)
{
	return 0;
}

void CQuadTree::Collision(float fTime)
{
}

void CQuadTree::Render(float fTime)
{
	
}

CQuadTree * CQuadTree::Clone()
{
	return nullptr;
}
