#pragma once
#include "Component.h"
#include "../Resources/Mesh.h"
#include "../Component/DebugTree.h"
#include "../Component/ColliderAABB.h"

PG_BEGIN

const int MAX_TRIANGLES = 10000;

typedef struct _tagNodeQuadTree
{
	string strNodeName;
	float fCenterX, fCenterZ, fWidth;
	Vector3 fMin, fMax;
	int iTriCount;
	vector<VERTEXBUMP> vecVtx;
	vector<UINT> vecIndex;
	CGameObject* pGameObject;
	DebugTree* pDebugTree;
	// 안쓸수도 있음
	MESHCONTAINER* pMeshInfo;
	//
	_tagNodeQuadTree* pNodes[4];
}QUADTREENODE;

class PG_DLL CLandScape :
	public CComponent
{
private:
	friend class CGameObject;

private:
	CLandScape();
	CLandScape(const CLandScape& landscape);
	~CLandScape();

private:
	int		m_iNumX;
	int		m_iNumZ;
	vector<Vector3>	m_vecPos;
	vector<Vector3>	m_vecFaceNormal;

	vector<VERTEXBUMP> m_vecVtx;
	vector<UINT> m_vecIndex;

	int		m_iDetailLevel;
	int		m_iSplatCount;

public:
	vector<VERTEXBUMP>& getVecVtx() { return m_vecVtx; }
	vector<UINT>& getVecIndex() { return m_vecIndex; }

public:
	void SetDetailLevel(int iDetailLevel);
	bool CreateLandScape(const string& strMeshKey, int iVtxCount, bool bBump, 
		const string& strTexKey, const wchar_t* pFileName, 
		const wchar_t* pNormalName, const wchar_t* pSpecularName,
		const char* pHeightMap = NULL,
		const string& strPathKey = TEXTURE_PATH);
	bool CreateLandScape(const string& strMeshKey, int iSizeX, int iSizeZ, bool bBump,
		const string& strTexKey, const wchar_t* pFileName,
		const wchar_t* pNormalName, const wchar_t* pSpecularName,
		const char* pHeightMap = NULL,
		const string& strPathKey = TEXTURE_PATH);
	bool CreateLandScapeQuadTree(const string& strMeshKey, int iSizeX, int iSizeZ, bool bBump,
		const string& strTexKey, const wchar_t* pFileName,
		const wchar_t* pNormalName, const wchar_t* pSpecularName,
		const char* pHeightMap = NULL,
		const string& strPathKey = TEXTURE_PATH);
	bool SetDiffuseSplatting(const string& strSmpKey,
		const string& strDifKey,
		const vector<wstring>* pvecPath = NULL,
		const string& strPathKey = TEXTURE_PATH);
	bool SetNormalSplatting(const string& strSmpKey,
		const string& strDifKey,
		const vector<wstring>* pvecPath = NULL,
		const string& strPathKey = TEXTURE_PATH);
	bool SetSpecularSplatting(const string& strSmpKey,
		const string& strDifKey,
		const vector<wstring>* pvecPath = NULL,
		const string& strPathKey = TEXTURE_PATH);
	bool SetSplattingAlpha(const string& strSmpKey,
		const string& strDifKey, int iRegTex = 0, int iRegSmp = 0,
		const vector<wstring>* pvecPath = NULL,
		const string& strPathKey = TEXTURE_PATH);

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CLandScape* Clone();
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);

private:
	void ComputeNormal(vector<VERTEXBUMP>& m_vecVtx, const vector<UINT>& vecIdx);
	void ComputeTangent(vector<VERTEXBUMP>& m_vecVtx, const vector<UINT>& vecIdx);

public:
	list<QUADTREENODE*>* FindNode_ByMouse();

private:
	// QuadTree
	bool CreateQuadTree();
	void CreateTreeNodeToObject(QUADTREENODE* node);
	void CreateTreeNode(QUADTREENODE* node, float positionX, float positionZ,
		float width);
	void CalculateMeshDimensions(int vtxCount,
		float& centerX, float& centerZ,
		float& meshWidth);
	int CountTriangles(float positionX, float positionZ, float width);
	bool CountTrianglesMax_For_Speed(int& out,  float positionX, float positionZ, float width);
	bool CountTriangles_For_Speed(float positionX, float positionZ, float width);
	bool IsTriangleContaind(int index, float positionX, float positionZ, float width);
	
	// Node Circulation
	void UpdateNode(QUADTREENODE* node);
	void RenderDebug(QUADTREENODE* node, float fTime);
	void NodeRayCollisionCheck(QUADTREENODE* node);
	void ReleaseNode(QUADTREENODE* node);

private:
	bool m_bVisualCheck = false;
	int m_iTriCount, m_iDrawCount = 0;
	int m_iTriStack = 0;
	int m_iDebugStack = 0;
	QUADTREENODE* m_pParentNode = nullptr;
	static int number;

	list<QUADTREENODE*> m_listNode;
};

PG_END
