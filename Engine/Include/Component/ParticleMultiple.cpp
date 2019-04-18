#include "ParticleMultiple.h"
#include "../Rendering/ShaderManager.h"
#include "../Rendering/Shader.h"
#include "../Rendering/RenderManager.h"
#include "../Resources/Texture.h"
#include "../Rendering/DepthStencilState.h"
#include "../Rendering/RenderState.h"
#include "../Resources/ResourcesManager.h"
#include "../Device.h"
#include "../Component/Transform.h"
#include "../Scene/SceneManager.h"
#include "../Component/Camera.h"
#include "../Scene/Scene.h"

PG_USING

ParticleMultiple::ParticleMultiple() :
	pInitVertexBuffer(NULL),
	pStreamVertexBuffer(NULL),
	pDrawVertexBuffer(NULL),
	pShader(NULL),
	pStreamShader(NULL),
	pTexture(NULL),
	pInputLayout(NULL),
	pAlphaBlend(NULL),
	pLightShader(NULL),
	pDepthWriteDisable(NULL),
	maxParticles(0),
	particleTime(0.f),
	lightRange(1.5f),
	firstRun(true),
	isLight(false)
{
	SetTag("ParticleMultiple");
	SetTypeName("ParticleMultiple");
	SetTypeID<ParticleMultiple>();
	this->m_eType = CT_PARTICLE_MULTIPLE;

	setShader(PARTICLE_SHADER);
	setStreamShader(PARTICLE_STREAMOUT_SHADER);
	setInputLayout("Particle");

	//pLightShader = GET_SINGLE(CShaderManager)->FindShader(PARTICLE_LIGHT_SHADER);
	pDepthDisable = GET_SINGLE(CRenderManager)->FindRenderState(DEPTH_DISABLE);
	pAlphaBlend = GET_SINGLE(CRenderManager)->FindRenderState(ALPHA_BLEND);
}

ParticleMultiple::ParticleMultiple(const ParticleMultiple & ParticleMultiple) :
	CComponent(ParticleMultiple)
{
	*this = ParticleMultiple;

	particleTime = 0.f;
	firstRun = true;
	setParticleInfo(ParticleMultiple.maxParticles);

	pShader = ParticleMultiple.pShader;

	if (pShader)
		pShader->AddRef();

	if (pStreamShader)
		pStreamShader->AddRef();

	if (pTexture)
		pTexture->AddRef();

	if (pDepthDisable)
		pDepthDisable->AddRef();

	if (pAlphaBlend)
		pAlphaBlend->AddRef();

	if (pLightShader)
		pLightShader->AddRef();

	if (pDepthWriteDisable)
		pDepthWriteDisable->AddRef();

	pInputLayout = ParticleMultiple.pInputLayout;
}


ParticleMultiple::~ParticleMultiple()
{
	SAFE_RELEASE(pDepthWriteDisable);
	SAFE_RELEASE(pLightShader);
	SAFE_RELEASE(pAlphaBlend);
	SAFE_RELEASE(pDepthDisable);
	SAFE_RELEASE(pTexture);
	SAFE_RELEASE(pStreamShader);
	SAFE_RELEASE(pShader);

	SAFE_RELEASE(pInitVertexBuffer);
	SAFE_RELEASE(pStreamVertexBuffer);
	SAFE_RELEASE(pDrawVertexBuffer);

}

bool ParticleMultiple::getParticleLight() const
{
	return isLight;
}

void ParticleMultiple::setLightRange(float range)
{
	this->lightRange = range;
}

void ParticleMultiple::setShader(const string & strKey)
{
	SAFE_RELEASE(pShader);
	pShader = GET_SINGLE(CShaderManager)->FindShader(strKey);

}

void ParticleMultiple::setStreamShader(const string & strKey)
{
	SAFE_RELEASE(pStreamShader);
	pStreamShader = GET_SINGLE(CShaderManager)->FindShader(strKey);
}

void ParticleMultiple::setInputLayout(const string & strKey)
{
	pInputLayout = GET_SINGLE(CShaderManager)->FindInputLayout(strKey);
}

void ParticleMultiple::setParticleInfo(int maxParticles)
{
	this->maxParticles = maxParticles;

	D3D11_BUFFER_DESC desc = {};

	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof(VERTEXPARTICLEMULTIPLE);
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	VERTEXPARTICLEMULTIPLE vertex = {};
	vertex.createTime = 0.05f;
	vertex.lifeTime = 2.5f;
	vertex.size = Vector2(0.3f, 0.3f);
	vertex.lightRange = 1.5f;
	vertex.type = 0;

	D3D11_SUBRESOURCE_DATA data = {};
	data.pSysMem = &vertex;

	if (FAILED(DEVICE->CreateBuffer(&desc, &data, &pInitVertexBuffer)))
	{
		MessageBoxA(NULL, "CreateBuffer Error", "Error", MB_OK);
		return;
	}
	desc.ByteWidth = sizeof(VERTEXPARTICLEMULTIPLE) * maxParticles;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_STREAM_OUTPUT;

	if (FAILED(DEVICE->CreateBuffer(&desc, NULL, &pStreamVertexBuffer)))
	{
		MessageBoxA(NULL, "CreateBuffer Error", "Error", MB_OK);
		return;
	}
	if (FAILED(DEVICE->CreateBuffer(&desc, NULL, &pDrawVertexBuffer)))
	{
		MessageBoxA(NULL, "CreateBuffer Error", "Error", MB_OK);
		return;
	}
}

void ParticleMultiple::setParticleTexture(const string & strKey, TCHAR * pFileName, const string & strPathKey)
{
	SAFE_RELEASE(pTexture);
	pTexture = GET_SINGLE(CResourcesManager)->LoadTexture(strKey, pFileName, strPathKey);
}

void ParticleMultiple::setParticleLight(bool particleLight)
{
	isLight = particleLight;

	SAFE_RELEASE(pDepthWriteDisable);

	if (particleLight)
	{
		pDepthWriteDisable = GET_SINGLE(CRenderManager)->FindRenderState(DEPTH_DISABLE);
	}
}

bool ParticleMultiple::Init()
{
	firstRun = true;
	return true;
}

void ParticleMultiple::Input(float fTime)
{
}

int ParticleMultiple::Update(float fTime)
{
	return 0;
}

int ParticleMultiple::LateUpdate(float fTime)
{
	return 0;
}

void ParticleMultiple::Collision(float fTime)
{
}

void ParticleMultiple::Render(float fTime)
{
	PARTICLEMULTIPLECBUFFER particleCBuffer = {};

	particleCBuffer.pos = m_pTransform->GetWorldPos();
	particleCBuffer.deltaTime = fTime;

	CCamera* pCamera = m_pScene->GetMainCamera();
	CTransform* pCameraTr = pCamera->GetTransform();

	particleCBuffer.vCamAxisY = pCameraTr->GetWorldAxis(AXIS_Y);
	particleCBuffer.vCamAxisX = pCameraTr->GetWorldAxis(AXIS_X) * -1.f;
	particleCBuffer.createTime = 0.05f;
	particleCBuffer.speed = 3.f;
	particleCBuffer.gameTime = particleTime;
	particleCBuffer.createDir = Vector3((rand() % 10001 - 5000) / 5000.f,
		(rand() % 10001 - 5000) / 5000.f,
		(rand() % 10001 - 5000) / 5000.f).Normalize();


	TRANSFORMCBUFFER transformCBuffer;
	transformCBuffer.matVP = pCamera->GetViewMatrix() * pCamera->GetProjMatrix();

	transformCBuffer.matVP = XMMatrixTranspose(transformCBuffer.matVP.mat);

	GET_SINGLE(CShaderManager)->UpdateCBuffer("Transform", &transformCBuffer, SCT_GEOMETRY);


	SAFE_RELEASE(pCameraTr);
	SAFE_RELEASE(pCamera);

	GET_SINGLE(CShaderManager)->UpdateCBuffer("ParticleMultiple", &particleCBuffer,
		SCT_GEOMETRY);

	CONTEXT->IASetInputLayout(pInputLayout);
	CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	UINT stride = sizeof(VERTEXPARTICLEMULTIPLE);
	UINT offset = 0;

	if (!isLight)
	{
		particleTime += fTime;

		// 스트림 버퍼에 출력한다.(쉐이더에서 새로운 파티클들을 생성시켜준다.)
		// 스트림버퍼 출력시에는 깊이를 끈다.
		pDepthDisable->SetState();

		if (firstRun)
		{
			CONTEXT->IASetVertexBuffers(0, 1, &pInitVertexBuffer, &stride, &offset);
		}
		else
			CONTEXT->IASetVertexBuffers(0, 1, &pDrawVertexBuffer, &stride, &offset);

		pStreamShader->SetShader();

		CONTEXT->SOSetTargets(1, &pStreamVertexBuffer, &offset);

		if (firstRun)
		{
			CONTEXT->Draw(1, 0);
			firstRun = false;
		}
		else
			CONTEXT->DrawAuto();

		ID3D11Buffer* pBuffer = NULL;

		CONTEXT->SOSetTargets(1, &pBuffer, &offset);
		pDepthDisable->ResetState();

		// 버퍼 ping-pong
		swap(pDrawVertexBuffer, pStreamVertexBuffer);

	}

	pShader->SetShader();

	pAlphaBlend->SetState();

	if (pTexture)
		pTexture->SetTexture(0, SCT_PIXEL);

	CONTEXT->IASetInputLayout(pInputLayout);
	CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	CONTEXT->IASetVertexBuffers(0, 1, &pDrawVertexBuffer, &stride, &offset);

	CONTEXT->DrawAuto();

	pAlphaBlend->ResetState();

}

ParticleMultiple * ParticleMultiple::Clone()
{
	return new ParticleMultiple(*this);
}

void ParticleMultiple::renderLight(float time)
{
	//PARTICLECBUFFER particleCBuffer = {};

	//particleCBuffer.pos = m_pTransform->GetWorldPos();
	//particleCBuffer.deltaTime = time;

	//CCamera* pCamera = m_pScene->GetMainCamera();
	//CTransform* pCameraTr = pCamera->GetTransform();

	//particleCBuffer.vCamAxisY = pCameraTr->GetWorldAxis(AXIS_Y);
	//particleCBuffer.vCamAxisX = pCameraTr->GetWorldAxis(AXIS_X) * -1.f;
	//particleCBuffer.createTime = 0.1f;
	//particleCBuffer.speed = 5.f;
	//particleCBuffer.gameTime = particleTime;
	//particleCBuffer.gameTime = particleTime;
	//particleCBuffer.createDir = Vector3((rand() % 10001 - 5000) / 5000.f,
	//	(rand() % 10001 - 5000) / 5000.f,
	//	(rand() % 10001 - 5000) / 5000.f).Normalize();

	//TRANSFORMCBUFFER transformCBuffer;
	//transformCBuffer.matView = pCamera->GetViewMatrix();
	//transformCBuffer.matVP = pCamera->GetViewMatrix() * pCamera->GetProjMatrix();
	//transformCBuffer.matProj = pCamera->GetProjMatrix();
	//transformCBuffer.matInvProj = XMMatrixInverse(&XMMatrixDeterminant(transformCBuffer.matProj.mat),
	//	transformCBuffer.matProj.mat);

	//transformCBuffer.matView = XMMatrixTranspose(transformCBuffer.matView.mat);
	//transformCBuffer.matProj = XMMatrixTranspose(transformCBuffer.matProj.mat);
	//transformCBuffer.matInvProj = XMMatrixTranspose(transformCBuffer.matInvProj.mat);
	//transformCBuffer.matVP = XMMatrixTranspose(transformCBuffer.matVP.mat);

	//GET_SINGLE(CShaderManager)->UpdateCBuffer("Transform", &transformCBuffer, SCT_GEOMETRY);

	//SAFE_RELEASE(pCameraTr);
	//SAFE_RELEASE(pCamera);

	//GET_SINGLE(CShaderManager)->UpdateCBuffer("ParticleCBuffer",
	//	&particleCBuffer, SCT_GEOMETRY);

	//CONTEXT->IASetInputLayout(pInputLayout);
	//CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	//UINT stride = sizeof(VERTEXPARTICLESYSTEM);
	//UINT offset = 0;

	//particleTime += time;

	//// 스트림 버퍼에 출려한다.(쉐이더에서 새로운 파티클을 생성시켜준다.)
	//// 스트림 버퍼 출력시에는 깊이를 끈다.
	//pDepthDisable->SetState();

	//if (firstRun)
	//	CONTEXT->IASetVertexBuffers(0, 1, &pInitVertexBuffer, &stride, &offset);
	//else
	//	CONTEXT->IASetVertexBuffers(0, 1, &pDrawVertexBuffer, &stride, &offset);

	//pStreamShader->SetShader();

	//// StreamBuffer를 지정한다.
	//CONTEXT->SOSetTargets(1, &pStreamVertexBuffer, &offset);

	//if (firstRun)
	//{
	//	CONTEXT->Draw(1, 0);
	//	firstRun = false;
	//}
	//else
	//	CONTEXT->DrawAuto();

	//pDepthDisable->ResetState();

	//ID3D11Buffer* pBuffer = NULL;

	//// 버퍼 ping-pong
	//swap(pDrawVertexBuffer, pStreamVertexBuffer);
	//pLightShader->SetShader();
	//pDepthDisable->SetState();

	//CONTEXT->IASetInputLayout(pInputLayout);
	//CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	//CONTEXT->IASetVertexBuffers(0, 1, &pDrawVertexBuffer, &stride, &offset);

	//CONTEXT->DrawAuto();

	//pDepthWriteDisable->ResetState();
}
