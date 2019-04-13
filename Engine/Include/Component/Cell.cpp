#include "Cell.h"

#include "../Resources/Mesh.h"
#include "../Resources/ResourcesManager.h"
#include "../Rendering/Shader.h"
#include "../Rendering/ShaderManager.h"
#include "Transform.h"
#include "../GameObject/GameObject.h"
#include "../Core/TimerManager.h"
#include "../Core/Timer.h"
#include "../Rendering/RenderManager.h"
#include "../Rendering/RenderState.h"
#include "../Scene/SceneManager.h"
#include "../Scene/Scene.h"
#include "../Scene/Layer.h"
#include "../Component/Camera.h"
#include "../Component/Renderer.h"
#include "../Device.h"

PG_USING

CCell::CCell()
{
	m_pShader = nullptr;
	m_pLayout = nullptr;
	m_bRenderCheck = true;
}

CCell::CCell(const CCell & axisLine)
{
}


CCell::~CCell()
{
	SAFE_RELEASE(m_pShader);
	SAFE_RELEASE(m_pMesh);
}

bool CCell::Init(const vector<Vector3>& vecPoints)
{
	SetPoints(vecPoints);

	m_pMesh = GET_SINGLE(CResourcesManager)->FindMesh("Line");
	m_pShader = GET_SINGLE(CShaderManager)->FindShader(STANDARD_COLOR_SHADER);
	m_pLayout = GET_SINGLE(CShaderManager)->FindInputLayout("VertexColor");

	return true;
}

int CCell::Update(float fTime)
{
	return 0;
}

int CCell::LateUpdate(float fTime)
{
	return 0;
}

void CCell::Render(float fTime)
{
	if (!m_bRenderCheck)
		return;
}

CCell * CCell::Clone()
{
	return nullptr;
}
