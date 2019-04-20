#pragma once
#include "../Obj.h"

PG_BEGIN

class PG_DLL CShader :
	public CObj
{
private:
	friend class CShaderManager;

private:
	CShader();
	~CShader();

private:
	ID3D11VertexShader*		m_pVS;
	ID3DBlob*	m_pVSBlob;

	ID3D11PixelShader*		m_pPS;
	ID3DBlob*	m_pPSBlob;

	ID3D11GeometryShader*	m_pGS;
	ID3DBlob*	m_pGSBlob;

	string		m_strKey;

	D3D11_SO_DECLARATION_ENTRY* pStreamDecl;
	UINT declCount;

public:
	string GetKey()	const;
	void* GetShaderByteCode();
	int GetShaderByteCodeLength();

public:
	void SetStreamDecl(D3D11_SO_DECLARATION_ENTRY* pStreamDecl, UINT count);

public:
	bool LoadShader(const string& strKey, TCHAR* pFileName,
		char* pEntry[ST_MAX],
		const string& strPathKey = SHADER_PATH, bool streamOut = false);
	bool LoadVertexShader(const string& strKey, TCHAR* pFileName,
		char* pEntry,
		const string& strPathKey = SHADER_PATH);
	bool LoadPixelShader(const string& strKey, TCHAR* pFileName,
		char* pEntry,
		const string& strPathKey = SHADER_PATH);
	bool LoadGeometryShader(const string& strKey, TCHAR* pFileName,
		char* pEntry,
		const string& strPathKey = SHADER_PATH, bool streamOut = false);

public:
	void SetShader();
};

PG_END
