#pragma once
#include "Component.h"
#include "../Component/LandScape.h"
#include "../Component/DebugTree.h"

PG_BEGIN

const int MAX_TRIANGLES2 = 10000;

class PG_DLL CQuadTree :
	public CComponent
{
private:
	struct NodeType
	{
		float fPositionX, fPositionZ, fWidth;
		int iTriCount;
		ID3D11Buffer *vertexBuffer, *indexBuffer;
		vector<VERTEXBUMP> vecVtx;
		vector<UINT> vecIndex;
		NodeType* nodes[4];
	};

private:
	friend class CGameObject;

public:
	CQuadTree();
	CQuadTree(const CQuadTree& quadTree);
	~CQuadTree();

public:
	bool CreateQuadTree(CLandScape* terrain);

private:
	void CalculateMeshDimensions(int vtxCount, 
		float& centerX, float& centerZ, 
		float& meshWidth);
	void CreateTreeNode(NodeType* node, 
		float positionX, float positionZ,
		float width);
	int CountTriangles(float positionX, float positionZ, float width);
	bool IsTriangleCountaiend(int index, float positionX, float positionZ, float width);
	void ReleaseNode(NodeType* node);
	
	void RenderDebug(NodeType* node);
	void NodeCheck(NodeType* node, float positionX, float positionZ);
	bool CollideCheck(float nodePosX, float nodePosZ, float width, float positionX, float positionZ);

private:
	bool m_bVisualCheck = false;
	int m_iTriCount, m_iDrawCount = 0;
	vector<VERTEXBUMP> m_vecVtx;
	vector<UINT> m_vecIndex;
	NodeType* m_pParentNode = nullptr;
	DebugTree* m_pDebugTree = nullptr;
public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CQuadTree* Clone();
};

PG_END