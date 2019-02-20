#include "stdafx.h"
#include "BrushTool.h"
#include "Component/LandScape.h"

CBrushTool::CBrushTool()
{
	m_vColor = Vector4::Red;
}

CBrushTool::CBrushTool(const CBrushTool & brushtool)
{
}

CBrushTool::~CBrushTool()
{
}

void CBrushTool::SetBrushCheck(bool check)
{
	m_bBrushCheck = check;
}

void CBrushTool::SetBrushInformation(float range)
{
	CGameObject* pLandScapeObj = CGameObject::FindObject("LandScape");

	if (pLandScapeObj)
	{
		CLandScape* pLandScape = pLandScapeObj->FindComponentFromTag<CLandScape>("LandScape");

		m_fRange = range;

		pLandScape->SetBrushInformation(range);

		SAFE_RELEASE(pLandScape);
		SAFE_RELEASE(pLandScapeObj);
	}
}

void CBrushTool::SetBrushInformation(Vector4 color)
{
	CGameObject* pLandScapeObj = CGameObject::FindObject("LandScape");

	if (pLandScapeObj)
	{
		CLandScape* pLandScape = pLandScapeObj->FindComponentFromTag<CLandScape>("LandScape");

		m_vColor = color;

		pLandScape->SetBrushInformation(color);

		SAFE_RELEASE(pLandScape);
		SAFE_RELEASE(pLandScapeObj);
	}
}

void CBrushTool::SetBrushInformation(Vector3 mousePos)
{
	CGameObject* pLandScapeObj = CGameObject::FindObject("LandScape");

	if (pLandScapeObj)
	{
		CLandScape* pLandScape = pLandScapeObj->FindComponentFromTag<CLandScape>("LandScape");

		pLandScape->SetBrushCheck(m_bBrushCheck);
		pLandScape->SetBrushInformation(mousePos);

		SAFE_RELEASE(pLandScape);
		SAFE_RELEASE(pLandScapeObj);
	}
}

void CBrushTool::MoveHeight(vector<VERTEXBUMP>* pVtx, Vector3 mousePos, const float & fTime)
{
	for (auto& iter : *pVtx)
	{
		float fDistance = iter.vPos.Distance(mousePos);

		if (fDistance < m_fRange)
		{
			//iter.vPos.y -= 1.f;
			iter.vPos.y += m_fUpSpeed * fTime;
		}
	}


}

bool CBrushTool::Init()
{
	return true;
}

void CBrushTool::Input(float fTime)
{
}

int CBrushTool::Update(float fTime)
{
	return 0;
}

int CBrushTool::LateUpdate(float fTime)
{
	return 0;
}

CScript * CBrushTool::Clone()
{
	return nullptr;
}
