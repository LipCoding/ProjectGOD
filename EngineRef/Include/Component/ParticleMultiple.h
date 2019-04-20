#pragma once
#include "Component.h"

PG_BEGIN

class PG_DLL ParticleMultiple :
	public CComponent
{
	friend class CGameObject;
public:
	ParticleMultiple();
	ParticleMultiple(const ParticleMultiple& particleSystem);
	~ParticleMultiple();

private:
	ID3D11Buffer* pInitVertexBuffer;
	ID3D11Buffer* pStreamVertexBuffer;
	ID3D11Buffer* pDrawVertexBuffer;

	int maxParticles;
	bool firstRun;
	class CShader* pStreamShader;
	class CShader* pShader;
	class CShader* pLightShader;
	ID3D11InputLayout* pInputLayout;
	class CTexture* pTexture;
	class CRenderState* pDepthDisable;
	class CRenderState* pAlphaBlend;
	class CRenderState* pDepthWriteDisable;
	float particleTime;
	bool isLight;
	float lightRange;

public:
	bool getParticleLight() const;

public:
	void setLightRange(float range);
	void setShader(const string& strKey);
	void setStreamShader(const string& strKey);
	void setInputLayout(const string& strKey);
	void setParticleInfo(int maxParticles = MAX_PARTICLES);
	void setParticleTexture(const string& strKey, TCHAR* pFileName,
		const string& strPathKey = TEXTURE_PATH);
	void setParticleLight(bool particleLight);

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual ParticleMultiple* Clone();

public:
	void renderLight(float time);
};

PG_END

