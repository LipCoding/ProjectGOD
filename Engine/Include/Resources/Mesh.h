#pragma once
#include "../Obj.h"

PG_BEGIN

typedef struct _tagVertexBuffer
{
	ID3D11Buffer*	pBuffer;
	UINT			iVtxCount;
	UINT			iVtxSize;
	D3D11_USAGE		eUsage;
	D3D11_PRIMITIVE_TOPOLOGY	ePrimitive;
	void*			pVtxData;
}VERTEXBUFFER, *PVERTEXBUFFER;

typedef struct _tagIndexBuffer
{
	ID3D11Buffer*	pBuffer;
	UINT			iIdxCount;
	UINT			iIdxSize;
	D3D11_USAGE		eUsage;
	DXGI_FORMAT		eFmt;
	void*			pIdxData;
}INDEXBUFFER, *PINDEXBUFFER;

typedef struct _tagMeshContainer
{
	VERTEXBUFFER	tVB;
	vector<INDEXBUFFER>	vecIB;
	vector<class CMaterial*>	vecMaterial;

	_tagMeshContainer()
	{
	}
}MESHCONTAINER, *PMESHCONTAINER;

class PG_DLL CMesh :
	public CObj
{
private:
	friend class CResourcesManager;

private:
	CMesh();
	~CMesh();

private:
	vector<PMESHCONTAINER>	m_vecMeshContainer;
	string			m_strKey;
	Vector3			m_vLength;
	Vector3			m_vMin;
	Vector3			m_vMax;
	Vector3			m_vCenter;
	float			m_fRadius;
	class CAnimation*	m_pAnimation;
	string			m_strShaderKey;
	string			m_strShadowShaderKey;
	string			m_strInputLayoutKey;

public:
	void SetShaderKey(const string& strShaderKey);
	void SetInputLayoutKey(const string& strInputLayoutKey);
	size_t GetContainerCount()	const;
	size_t GetSubsetCount(int iContainer = 0)	const;
	string GetShaderKey()	const;
	string GetShadowShaderKey()	const;
	string GetInputLayoutKey()	const;
	Vector3 GetLength()	const;
	Vector3	GetCenter()	const;
	float GetRadius()	const;
	string GetKey()	const;
	void SetKey(const string& strKey);
	class CMaterial* CloneMaterial(int iContainer = 0, int iSubset = 0);
	class CAnimation* CloneAnimation()	const;
	void GetMeshPosition(vector<Vector3>* vecPos, int iContainer = 0);
	void GetMeshIndex(vector<int>* vecIndex, int iContainer = 0, int iSubset = 0);
	void CreateNaviMeshInfo(class CNavigationMesh* pNaviMesh, const Vector3& vScale,
		const Vector3& vLandPos);

public:
	bool CreateMesh(UINT iVtxCount, UINT iVtxSize, D3D11_USAGE eVtxUsage,
		D3D11_PRIMITIVE_TOPOLOGY ePrimitive, void* pVtxData,
		UINT iIdxCount = 0, UINT iIdxSize = 0,
		D3D11_USAGE eIdxUsage = D3D11_USAGE_DEFAULT,
		DXGI_FORMAT eFmt = DXGI_FORMAT_R32_UINT,
		void* pIdxData = NULL);

	bool CreateMesh(MESHCONTAINER* pOut, 
		UINT iVtxCount, UINT iVtxSize, D3D11_USAGE eVtxUsage,
		D3D11_PRIMITIVE_TOPOLOGY ePrimitive, void* pVtxData,
		UINT iIdxCount = 0, UINT iIdxSize = 0,
		D3D11_USAGE eIdxUsage = D3D11_USAGE_DEFAULT,
		DXGI_FORMAT eFmt = DXGI_FORMAT_R32_UINT,
		void* pIdxData = NULL);

	bool LoadMesh(const string& strKey, const char* pFullPath);

private:
	bool CreateVertexBuffer(UINT iVtxCount, UINT iVtxSize, D3D11_USAGE eVtxUsage,
		D3D11_PRIMITIVE_TOPOLOGY ePrimitive, void* pVtxData);
	bool CreateIndexBuffer(UINT iIdxCount = 0, UINT iIdxSize = 0,
		D3D11_USAGE eIdxUsage = D3D11_USAGE_DEFAULT,
		DXGI_FORMAT eFmt = DXGI_FORMAT_R32_UINT,
		void* pIdxData = NULL);

public:
	void Render(float fTime);
	void Render(float fTime, int iContainer, int iSubset);
	bool CreateSphere(float radius, UINT numSubdivisions);
	void Subdivide(vector<XMFLOAT3>& vecPos, vector<UINT>& vecIndex);
	float AngleFromXY(float x, float y);

private:
	bool ConvertFbx(class CFbxLoader* pLoader);

public:
	bool Save(const wchar_t* pFileName, const string& strPathKey = MESH_PATH);
	bool Save(const char* pFileName, const string& strPathKey = MESH_PATH);
	bool SaveFromFullPath(const wchar_t* pFullPath);
	bool SaveFromFullPath(const char* pFullPath);
	bool Load(const wchar_t* pFileName, const string& strPathKey = MESH_PATH);
	bool Load(const char* pFileName, const string& strPathKey = MESH_PATH);
	bool LoadFromFullPath(const wchar_t* pFullPath);
	bool LoadFromFullPath(const char* pFullPath);
};

PG_END
