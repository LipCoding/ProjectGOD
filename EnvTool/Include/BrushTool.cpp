#include "stdafx.h"
#include "BrushTool.h"

#include "Device.h"

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

void CBrushTool::SetSpeed(float Speed)
{
	// up
	if (m_fUpSpeed < 0.f)
		m_fUpSpeed = -Speed;
	
	// down
	if (m_fUpSpeed >= 0.f)
		m_fUpSpeed = Speed;
}

void CBrushTool::SetSpeedDir(bool check)
{
	// up
	if (check)
	{
		if (m_fUpSpeed < 0.f)
			m_fUpSpeed *= -1.f;
	}
	// down
	else
	{
		if (m_fUpSpeed > 0.f)
			m_fUpSpeed *= -1.f;
	}
}

void CBrushTool::MoveHeight(list<QUADTREENODE*>* list, Vector3 mousePos, const float & fTime)
{
	for (auto& node : *list)
	{
		int iIndex = 0;

		for (auto& iter : node->vecVtx)
		{
			float fDistance = iter.vPos.Distance(mousePos);

			if (fDistance < m_fRange)
			{
				switch (m_eUpType)
				{
				case UP_TYPE_FLAT:
					iter.vPos.y += m_fUpSpeed * fTime;
					break;
				case UP_TYPE_ROUND:
					iter.vPos.y += (m_fUpSpeed * (1.f - fDistance / m_fRange)) * fTime;
					break;
				case UP_TYPE_SMOOTH:
					float fGap = 0.f;
					int iScrIndex = 0;
					float fAverValue = 0.2f;
					float GAP = 4.f;

					int iNumSizeX = int(node->vMax.x - node->vMin.x) + 1;

					if (iter.vPos.x < node->vMax.x &&
						iter.vPos.x > node->vMin.x)
					{
						/*Left*/
						if (0 < (iIndex - 1) &&
							node->vecVtx[iIndex - 1].vPos.x >= node->vMin.x)
						{
							iScrIndex = iIndex - 1;
							fGap = abs(node->vecVtx[iIndex].vPos.y - node->vecVtx[iScrIndex].vPos.y);

							if (fGap > fAverValue)
							{
								fGap /= GAP;

								if (node->vecVtx[iIndex].vPos.y > node->vecVtx[iScrIndex].vPos.y)
								{
									node->vecVtx[iIndex].vPos.y -= fGap * fTime * 3.f;
									node->vecVtx[iScrIndex].vPos.y += fGap * fTime* 3.f;
								}
								else
								{
									node->vecVtx[iIndex].vPos.y += fGap * fTime* 3.f;
									node->vecVtx[iScrIndex].vPos.y -= fGap * fTime* 3.f;
								}
							}
						}

						/*Right*/
						if (node->vecVtx.size() > (iIndex + 1) &&
							node->vecVtx[iIndex + 1].vPos.x <= node->vMax.x)
						{
							iScrIndex = iIndex + 1;
							fGap = abs(node->vecVtx[iIndex].vPos.y - node->vecVtx[iScrIndex].vPos.y);

							if (fGap > fAverValue)
							{
								fGap /= GAP;

								if (node->vecVtx[iIndex].vPos.y > node->vecVtx[iScrIndex].vPos.y)
								{
									node->vecVtx[iIndex].vPos.y -= fGap * fTime* 3.f;
									node->vecVtx[iScrIndex].vPos.y += fGap * fTime* 3.f;
								}
								else
								{
									node->vecVtx[iIndex].vPos.y += fGap * fTime* 3.f;
									node->vecVtx[iScrIndex].vPos.y -= fGap * fTime* 3.f;
								}
							}
						}
					}

					if (iter.vPos.z < node->vMax.z &&
						iter.vPos.z > node->vMin.z)
					{
						/*Up*/
						if (node->vecVtx.size() > (iIndex + iNumSizeX) &&
							node->vecVtx[iIndex + iNumSizeX].vPos.z <= node->vMax.z)
						{
							iScrIndex = iIndex + iNumSizeX;
							fGap = abs(node->vecVtx[iIndex].vPos.y - node->vecVtx[iScrIndex].vPos.y);

							if (fGap > fAverValue)
							{
								fGap /= GAP;

								if (node->vecVtx[iIndex].vPos.y > node->vecVtx[iScrIndex].vPos.y)
								{
									node->vecVtx[iIndex].vPos.y -= fGap * fTime* 3.f;
									node->vecVtx[iScrIndex].vPos.y += fGap * fTime* 3.f;
								}
								else
								{
									node->vecVtx[iIndex].vPos.y += fGap * fTime* 3.f;
									node->vecVtx[iScrIndex].vPos.y -= fGap * fTime* 3.f;
								}
							}
						}

						/*Down*/
						if (0 < (iIndex - iNumSizeX) &&
							node->vecVtx[iIndex - iNumSizeX].vPos.z >= node->vMin.z)
						{
							iScrIndex = iIndex - iNumSizeX;
							fGap = abs(node->vecVtx[iIndex].vPos.y - node->vecVtx[iScrIndex].vPos.y);

							if (fGap > fAverValue)
							{
								fGap /= GAP;

								if (node->vecVtx[iIndex].vPos.y > node->vecVtx[iScrIndex].vPos.y)
								{
									node->vecVtx[iIndex].vPos.y -= fGap * fTime* 3.f;
									node->vecVtx[iScrIndex].vPos.y += fGap * fTime* 3.f;
								}
								else
								{
									node->vecVtx[iIndex].vPos.y += fGap * fTime* 3.f;
									node->vecVtx[iScrIndex].vPos.y -= fGap * fTime* 3.f;
								}
							}
						}
					}
					break;
				}

				UpdateVtxBuffer(&node->MeshInfo, node->vecVtx);
			}
			iIndex++;
		}
	}
}

void CBrushTool::ResetHeight()
{
	CGameObject* pLandScapeObj = CGameObject::FindObject("LandScape");

	if (pLandScapeObj)
	{
		CLandScape* pLandScape = pLandScapeObj->FindComponentFromTag<CLandScape>("LandScape");

		list<QUADTREENODE*>* listNode = pLandScape->FindNode_All();

		for (auto& node : *listNode)
		{
			for (auto& vtx : node->vecVtx)
			{
				vtx.vPos.y = 0.f;
			}

			UpdateVtxBuffer(&node->MeshInfo, node->vecVtx);
		}

		SAFE_RELEASE(pLandScape);
		SAFE_RELEASE(pLandScapeObj);
	}
}

void CBrushTool::UpdateVtxBuffer(MESHCONTAINER * info, vector<VERTEXBUMP>& vtx)
{
	D3D11_MAPPED_SUBRESOURCE	tMap = {};

	CONTEXT->Map(info->tVB.pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &tMap);
	void* dataPtr;
	dataPtr = (void*)tMap.pData;
	memcpy(dataPtr, &vtx[0], vtx.size() * sizeof(VERTEXBUMP));
	CONTEXT->Unmap(info->tVB.pBuffer, 0);
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
