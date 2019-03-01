#pragma once
#include "Component.h"
#include "../Resources/Mesh.h"
#include "../Component/ColliderAABB.h"

PG_BEGIN

const int MAX_TRIANGLES = 5000;

typedef struct _tagNodeQuadTree
{
	string strNodeName;
	int iTriCount;
	float fCenterX, fCenterZ, fWidth;
	Vector3 vMin, vMax;
	int iSizeX, iSizeZ;
	vector<VERTEXBUMP> vecVtx;
	vector<UINT> vecIndex;
	CGameObject* pGameObject;
	MESHCONTAINER MeshInfo;
	//DWORD*		 pPixel[4];
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
	int     m_arrDetailLevel_Tex[4] = { 1, 1, 1, 1 };
	int		m_iSplatCount;

	// Brush
	bool m_bCheckBrush = false;
	Vector3 m_vPosBrush;
	Vector4 m_vColorBrush;
	float   m_fRangeBrush = 1.f;

public:
	vector<VERTEXBUMP>& getVecVtx() { return m_vecVtx; }
	vector<UINT>& getVecIndex() { return m_vecIndex; }

	POINT GetTerrainSize() { return POINT{ (LONG)m_iNumX, (LONG)m_iNumZ }; }
	int   GetDetailLevel() { return m_iDetailLevel; }
	int*  GetDetailLevel_Splat() { return m_arrDetailLevel_Tex; }
public:
	void SetDetailLevel(int iDetailLevel);
	void SetSplatCount(int count);
	void SetDetailLevel_Splat(int index, int iDetailLevel);
	void SetBrushCheck(bool check);
	void SetBrushInformation(float range) { m_fRangeBrush = range; }
	void SetBrushInformation(Vector4 color) { m_vColorBrush = color; }
	void SetBrushInformation(Vector3 pos) { m_vPosBrush = pos; }
	void SetTerrainSize(int x, int z);
public:
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
	bool CreateLandScapeQuadTree(const string& strMeshKey, int iSizeX, int iSizeZ, bool bBump,
		const string& strTexKey, const wchar_t* pFileName,
		const wchar_t* pNormalName, const wchar_t* pSpecularName,
		const char* pHeightMap = NULL,
		const string& strPathKey = TEXTURE_PATH);
	bool SetDiffuseSplattingQuadTree(const string& strSmpKey,
		const string& strDifKey,
		const vector<wstring>* pvecPath = NULL,
		const string& strPathKey = TEXTURE_PATH);
	bool SetNormalSplattingQuadTree(const string& strSmpKey,
		const string& strDifKey,
		const vector<wstring>* pvecPath = NULL,
		const string& strPathKey = TEXTURE_PATH);
	bool SetSpecularSplattingQuadTree(const string& strSmpKey,
		const string& strDifKey,
		const vector<wstring>* pvecPath = NULL,
		const string& strPathKey = TEXTURE_PATH);
	bool SetSplattingAlphaQuadTree(const string& strSmpKey,
		const string& strDifKey,
		const vector<wstring>* pvecPath = NULL,
		const string& strPathKey = TEXTURE_PATH);

	void SetMaterial_DNS_Default(const wchar_t* pFileName,
		const wchar_t* pNormalName,
		const wchar_t* pSpecularName);
	void SetMaterial_Splatting(
		vector<wstring>& vecDif,
		vector<wstring>& vecNormal,
		vector<wstring>& vecSpecular,
		vector<wstring>& vecAlpha,
		const string & strPathKey = TEXTURE_PATH);

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

public:
	list<QUADTREENODE*>* FindNode_ByMouse();
	list<QUADTREENODE*>* FindNode_ByRadius(float radius);

	list<QUADTREENODE*>* GetAllNodes() { return &m_listAllNodes; };

	void NodesToContainer();

	void Save_QuadTree(string fileName);
	void Load_QuadTree(string fileName);
private:
	// QuadTree
	bool CreateQuadTree();
	void CreateTreeNodeToObject();
	void CreateTreeNode(QUADTREENODE* node, float positionX, float positionZ,
		float width);
	void CalculateMeshDimensions(int vtxCount,
		float& centerX, float& centerZ,
		float& meshWidth);
	int CountTriangles(float positionX, float positionZ, float width);
	bool IsTriangleContaind_Index(int index, float positionX, float positionZ, float width);
	bool IsTriangleContaind_Vertex(int vtxIndex, Vector3 min, Vector3 max);

	// Node Circulation
	void UpdateNode();
	void NodeRayCollisionCheck();
	void NodeRadiusCollisionCheck(float radius);
	void NodeSphereCollisionCheck(CGameObject* src);
	void NodeAll(QUADTREENODE* node);
	void ReleaseNode(QUADTREENODE* node);

private:
	void ComputeNormal(vector<VERTEXBUMP>& m_vecVtx, const vector<UINT>& vecIdx);
	void ComputeTangent(vector<VERTEXBUMP>& m_vecVtx, const vector<UINT>& vecIdx);

private:
	// Node
	bool m_bVisualCheck = false;
	int m_iTriCount, m_iDrawCount = 0;
	int m_iDebugStack = 0;
	QUADTREENODE* m_pParentNode = nullptr;
	static int number;
	list<QUADTREENODE*> m_listNode;
	list<QUADTREENODE*> m_listAllNodes;
	// 

};

PG_END
