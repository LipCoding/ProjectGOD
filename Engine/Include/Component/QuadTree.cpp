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
	//쿼드 트리 데이터를 재귀적으로 해제
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

	// 지형 정점 배열의 정점 수를 가져온다.
	int vertexCount = m_vecVtx.size();
	
	// 정점 리스트의 총 삼각형 수를 저장
	m_iTriCount = vertexCount / 3;

	// 중심 x, z와 메쉬의 너비를 계산
	this->CalculateMeshDimensions(vertexCount, fCenterX, fCenterZ, fWidth);

	// 쿼드 트리의 부모 노드를 만든다.
	m_pParentNode = new NodeType;
	if (!m_pParentNode)
	{
		return false;
	}

	// 정점 목록 데이터와 메쉬 차원을 기반으로 쿼드 트리를 재귀적으로 빌드한다.
	this->CreateTreeNode(m_pParentNode, fCenterX, fCenterZ, fWidth);

	// 쿼드 트리가 각 노드에 정점을 갖기 때문에 정점 목록을 놓는다.
	if (!m_vecVtx.empty())
	{
		m_vecVtx.clear();
	}

	return true;
}

void CQuadTree::CalculateMeshDimensions(int vtxCount, float & centerX, float & centerZ, float & meshWidth)
{
	// 메쉬의 중심 위치를 0으로 초기화
	centerX = 0.f;
	centerZ = 0.f;

	// 메쉬의 모든 정점을 합친다.
	for (const auto iter : m_vecVtx)
	{
		centerX += iter.vPos.x;
		centerZ += iter.vPos.z;
	}

	// 메쉬의 중간 점을 찾기 위해 정점의 수로 나눈다.
	centerX = centerX / (float)vtxCount;
	centerZ = centerZ / (float)vtxCount;

	// 메쉬의 최대 및 최소 크기를 초기화 한다.
	float fMaxWidth = 0.f;
	float fMaxDepth = 0.f;

	float fMinWidth = fabsf(m_vecVtx[0].vPos.x - centerX);
	float fMinDepth = fabsf(m_vecVtx[0].vPos.z - centerZ);
	
	// 모든 정점을 살펴보고 메쉬의 최대 너비와 최소 너비와 깊이를 찾는다.
	for (const auto iter : m_vecVtx)
	{
		float fWidth = fabsf(iter.vPos.x - centerX);
		float fDepth = fabsf(iter.vPos.z - centerZ);

		if (fWidth > fMaxWidth) { fMaxWidth = fWidth; }
		if (fDepth > fMaxDepth) { fMaxDepth = fDepth; }
		if (fWidth < fMinWidth) { fMinWidth = fWidth; }
		if (fDepth < fMinDepth) { fMinDepth = fDepth; }
	}

	// 최소와 최대 깊이와 너비 사이의 절대 최대값을 찾는다.
	float fMaxX = (float)max(fabs(fMaxWidth), fabs(fMaxWidth));
	float fMaxZ = (float)max(fabs(fMaxDepth), fabs(fMaxDepth));

	// 메쉬의 최대 직경을 계산
	meshWidth = max(fMaxX, fMaxZ) * 2.f;
}

void CQuadTree::CreateTreeNode(NodeType * node, float positionX, float positionZ, float width)
{
	// 노드의 위치와 크기를 저장
	node->fPositionX = positionX;
	node->fPositionZ = positionZ;
	node->fWidth = width;

	// 노드의 삼각형 수를 0으로 초기화
	node->iTriCount = 0;

	// 정점 및 인덱스 버퍼를 null로 초기화
	node->vertexBuffer = nullptr;
	node->indexBuffer = nullptr;

	// 이 노드의 자식 노드를 null로 초기화
	node->nodes[0] = nullptr;
	node->nodes[1] = nullptr;
	node->nodes[2] = nullptr;
	node->nodes[3] = nullptr;

	// 이 노드 안에 있는 삼각형 수를 센다.
	int iNumTriangles = this->CountTriangles(positionX, positionZ, width);

	// 1. 이 노드에 삼각형이 없으면 비어있는 상태이므로 return
	if (iNumTriangles == 0)
	{
		return;
	}

	// 2. 이 노드에 너무 많은 삼각형이 있는 경우 4 개의 동일한 크기의 
	// 더 작은 트리 노드로 분할한다.
	if (iNumTriangles > MAX_TRIANGLES)
	{
		for (int i = 0; i < 4; i++)
		{
			// 새로운 자식 노드에 대한 위치 오프셋을 계산
			float fOffsetX = (((i % 2) < 1) ? -1.f : 1.f) * (width / 4.f);
			float fOffsetZ = (((i % 4) < 2) ? -1.f : 1.f) * (width / 4.f);

			// 새 노드에 삼각형이 있는지 확인한다.
			int iCount = this->CountTriangles((positionX + fOffsetX), (positionZ + fOffsetZ), (width / 2.f));
			if (iCount > 0)
			{
				// 이 새 노드가 있는 삼각형이 있는 경우 자식 노드로 만든다.
				node->nodes[i] = new NodeType;

				// 이제 새 자식 노드에서 시작하는 트리를 확장한다.
				this->CreateTreeNode(node->nodes[i], (positionX + fOffsetX), (positionZ + fOffsetZ), (width / 2.f));
			}
		}
		return;
	}

	// 3. 이 노드가 비어 있지 않고 그 노드의 삼각형 수가 최대 값보다 작으면
	// 이 노드는 트리의 맨 아래에 있으므로 저장할 삼각형 목록을 만든다.
	node->iTriCount = iNumTriangles;

	// 정점의 수를 계산
	int iVertexCount = iNumTriangles * 3;

	// 정점 배열을 만든다.()
	node->vecVtx.resize(iVertexCount);
	node->vecIndex.resize(iVertexCount);

	// 정점 배열을 만든다.
	VERTEXBUMP* vertices = new VERTEXBUMP[iVertexCount];
	UINT* indices = new UINT[iVertexCount];

	int iIndex = 0;
	int iVertexIndex = 0;

	for (int i = 0; i < m_iTriCount; i++)
	{
		// 삼각형이 이 노드 안에 있으면 꼭지점 배열에 추가한다.
		if (this->IsTriangleCountaiend(i, positionX, positionZ, width))
		{
			// 지형 버텍스 목록에 인덱스를 계산한다.
			iVertexIndex = i * 3;

			// 정점 목록에서 이 삼각형의 세 꼭지점을 가져온다.
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

	// 정점 버퍼 구조체 설정
	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VERTEXBUMP) * iVertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// subresource 구조에 정점 데이터에 대한 포인터를 제공
	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	DEVICE->CreateBuffer(&vertexBufferDesc, &vertexData, &node->vertexBuffer);

	// 인덱스 버퍼의 설명을 설정
	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(UINT) * iVertexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// 하위 리소스 구조에 인덱스 데이터에 대한 포인터 제공
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
	// 카운트를 0으로 초기화 한다.
	int count = 0;

	// 전체 메쉬의 모든 삼각형을 살펴보고 어떤 노드가
	// 이 노드 안에 있어야 하는지 확인한다.
	for (int i = 0; i < m_iTriCount; i++)
	{
		// 삼각형이 이 노드 안에 있으면 1씩 증가시킨다.
		if (this->IsTriangleCountaiend(i, positionX, positionZ, width))
		{
			count++;
		}
	}

	return count;
}

bool CQuadTree::IsTriangleCountaiend(int index, float positionX, float positionZ, float width)
{
	// 이 노드의 반경을 계산한다.
	float fRadius = width / 2.f;

	// 인덱스를 정점 목록으로 가져온다.
	int iVertexIndex = index * 3;

	// 정점 목록에서 이 삼각형의 세 꼭지점을 가져온다.
	float fX1 = m_vecVtx[iVertexIndex].vPos.x;
	float fZ1 = m_vecVtx[iVertexIndex].vPos.z;
	iVertexIndex++;

	float fX2 = m_vecVtx[iVertexIndex].vPos.x;
	float fZ2 = m_vecVtx[iVertexIndex].vPos.z;
	iVertexIndex++;

	float fX3 = m_vecVtx[iVertexIndex].vPos.x;
	float fZ3 = m_vecVtx[iVertexIndex].vPos.z;

	// 삼각형의 x좌표의 최소값이 노드 안에 있는지 확인
	float fMinimumX = min(fX1, min(fX2, fX3));
	if (fMinimumX > (positionX + fRadius))
	{
		return false;
	}

	// 삼각형의 x좌표의 최대값이 노드 안에 있는지 확인
	float fMaximumX = max(fX1, max(fX2, fX3));
	if (fMaximumX < (positionX - fRadius))
	{
		return false;
	}

	// 삼각형의 z좌표의 최소값이 노드 안에 있는지 확인
	float fMinimumZ = min(fZ1, min(fZ2, fZ3));
	if (fMinimumX > (positionZ + fRadius))
	{
		return false;
	}

	// 삼각형의 z좌표의 최대값이 노드 안에 있는지 확인
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

	// 자식 노드가 있는 경우
	if (count != 0)
	{
		return;
	}

	m_pDebugTree->Initialize(DEVICE, node->fPositionX, node->fPositionZ, node->fWidth / 2.f);
	m_pDebugTree->Render(CONTEXT);
	
}

void CQuadTree::NodeCheck(NodeType * node, float positionX, float positionZ)
{
	// 아닌경우
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

	// 자식 노드가 있는 경우
	if (count != 0)
	{
		return;
	}

	// 없는 경우

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
