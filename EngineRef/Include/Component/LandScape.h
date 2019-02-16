#pragma once
#include "Component.h"
#include "../Resources/Mesh.h"
#include "../Component/DebugTree.h"

PG_BEGIN

const int MAX_TRIANGLES = 1000;

class PG_DLL CLandScape :
	public CComponent
{
private:
	struct NodeType
	{
		float fCenterX, fCenterZ, fWidth;
		int iTriCount;
		vector<VERTEXBUMP> vecVtx;
		vector<UINT> vecIndex;
		MESHCONTAINER* pMeshInfo;
		NodeType* pNodes[4];
	};

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
	const POINT& GetSize() { return POINT{ m_iNumX, m_iNumZ }; }

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

private:
	bool CreateQuadTree();
	void CreateQuadMesh(NodeType* node);
	void CalculateMeshDimensions(int vtxCount,
		float& centerX, float& centerZ,
		float& meshWidth);
	void CreateTreeNode(NodeType* node, float positionX, float positionZ,
		float width);
	int CountTriangles(float positionX, float positionZ, float width);
	bool IsTriangleContaind(int index, float positionX, float positionZ, float width);
	void ReleaseNode(NodeType* node);
	void RenderDebug(NodeType* node);
	/*void NodeCheck(float positionX, float positionZ);
	bool CollideCheck(float nodePosX, float nodePosZ, float width, float positionX, float positionZ);*/

private:
	bool m_bVisualCheck = false;
	int m_iTriCount, m_iDrawCount = 0;
	NodeType* m_pParentNode = nullptr;
	DebugTree* m_pDebugTree = nullptr;
	static int g_iQuadName;
};

PG_END
