#include "stdafx.h"
#include "BrushTool.h"

#include "Device.h"
#include "Resources/ResourcesManager.h"

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

void CBrushTool::SetPower(float power)
{
	m_fPower = power;
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

void CBrushTool::MovePixel(int texType, Vector3 mousePos, const float & fTime)
{
	CTexture* pTexture = GET_SINGLE(CResourcesManager)->FindTexture("SplatAlpha");
	UpdateTextureBuffer(texType, pTexture, mousePos, m_fPower * fTime);
	SAFE_RELEASE(pTexture);

	//switch (texType)
	//{
	//case 0:
	//{
	//	CTexture* pTexture = GET_SINGLE(CResourcesManager)->FindTexture("SplatAlpha");
	//	UpdateTextureBuffer(texType, pTexture, mousePos, m_fPower * fTime);
	//	SAFE_RELEASE(pTexture);
	//	break;
	//}
	//case 1:
	//{
	//	CTexture* pTexture = GET_SINGLE(CResourcesManager)->FindTexture("SplatAlpha1");
	//	UpdateTextureBuffer(texType, pTexture, mousePos, m_fPower * fTime);
	//	SAFE_RELEASE(pTexture);
	//	break;
	//}
	//case 2:
	//{
	//	CTexture* pTexture = GET_SINGLE(CResourcesManager)->FindTexture("SplatAlpha2");
	//	UpdateTextureBuffer(texType, pTexture, mousePos, m_fPower * fTime);
	//	SAFE_RELEASE(pTexture);
	//	break;
	//}
	//case 3:
	//{
	//	/*CTexture* pTexture = GET_SINGLE(CResourcesManager)->FindTexture("SplatAlpha");
	//	UpdateTextureBuffer(texType, pTexture, mousePos, m_fPower * fTime);
	//	SAFE_RELEASE(pTexture);
	//	break;*/
	//}
	//}
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

void CBrushTool::UpdateTextureBuffer(int texType, CTexture* pTexture, Vector3 mousePos, float power)
{
	CGameObject* pLandScapeObj = CGameObject::FindObject("LandScape");

	if (pLandScapeObj)
	{
		UINT Height = pTexture->GetTexDesc().Height;
		UINT Width = pTexture->GetTexDesc().Width;

		D3D11_MAPPED_SUBRESOURCE   tMap = {};
		vector<ID3D11Texture2D*>&  vecTex = pTexture->getVecTex();

		if (m_pArrPixel[texType] == nullptr)
		{
			m_pArrPixel[texType] = new UINT32[Height * Width];
			
			//for (size_t i = 0; i < vecTex.size(); ++i)
			//{
				//for (int iMipLevel = 0; iMipLevel < pTexture->GetTexDesc().MipLevels;
				//	++iMipLevel)
				//{
					CONTEXT->Map(vecTex[texType], 0, D3D11_MAP_READ,
						0, &tMap);
					m_pArrPixel_Byte = reinterpret_cast<BYTE*>(tMap.pData);
					CONTEXT->Unmap(vecTex[texType], 0);
				//}
			//}

			for (UINT i = 0; i < Height; ++i)
			{
				for (UINT j = 0; j < Width; ++j)
				{
					// UV값
					int pixel = i * tMap.RowPitch + (j * 4);
					int pixel2 = i * Width + (j * 1);

					// 강제형변환은 오른쪽부터 크기만큼 짤림
					// R
					BYTE r = (BYTE)(m_pArrPixel_Byte[pixel]);
					// G
					BYTE g = (BYTE)(m_pArrPixel_Byte[pixel] + 1);
					// B
					BYTE b = (BYTE)(m_pArrPixel_Byte[pixel] + 2);
					// A
					BYTE a = (BYTE)(m_pArrPixel_Byte[pixel] + 3);

					UINT32 tempR = (r << 8 * 3);
					UINT32 tempG = (g << 8 * 2);
					UINT32 tempB = (b << 8 * 1);
					UINT32 tempA = (a << 8 * 0);
					//
					(m_pArrPixel[texType])[pixel2] = tempR | tempG | tempB | tempA;
				}
			}
		}

		CLandScape* pLandScape = pLandScapeObj->FindComponentFromTag<CLandScape>("LandScape");

		POINT landSize = pLandScape->GetTerrainSize();

		float fVal = (float)Height / landSize.x;

		Vector3 vMouseUV;

		vMouseUV.x = mousePos.x / (float)landSize.x;
		vMouseUV.z = ((float)landSize.y - mousePos.z) / landSize.y;

		vMouseUV.x *= Width;
		vMouseUV.z *= Height;

		for (UINT i = 0; i < Height; ++i)
		{
			for (UINT j = 0; j < Width; ++j)
			{
				// UV값
				int pixel = i * Width + (j * 1);

				Vector3 vPixelPos;
				vPixelPos.x = (float)j;
				vPixelPos.y = 0.f;
				vPixelPos.z = (float)i;

				float distance = vMouseUV.Distance(vPixelPos);

				if (distance < m_fRange * fVal)
				{
					// 강제형변환은 오른쪽부터 크기만큼 짤림
					// R
					BYTE r = (BYTE)((m_pArrPixel[texType])[pixel] >> 8 * 3);
					// G
					BYTE g = (BYTE)((m_pArrPixel[texType])[pixel] >> 8 * 2);
					// B
					BYTE b = (BYTE)((m_pArrPixel[texType])[pixel] >> 8 * 1);
					// A
					BYTE a = (BYTE)((m_pArrPixel[texType])[pixel] >> 8 * 0);

					if (r < 255 || g < 255
						|| b < 255 || a < 255)
					{
						r += (BYTE)(power);
						g += (BYTE)(power);
						b += (BYTE)(power);
						a += (BYTE)(power);
					}

					if (r + (BYTE)(power) > 255 ||
						g + (BYTE)(power) > 255 ||
						b + (BYTE)(power) > 255 ||
						a + (BYTE)(power) > 255)
					{
						r = 255;
						g = 255;
						b = 255;
						a = 255;
					}

					UINT32 tempR = (r << 8 * 3);
					UINT32 tempG = (g << 8 * 2);
					UINT32 tempB = (b << 8 * 1);
					UINT32 tempA = (a << 8 * 0);
					//
					(m_pArrPixel[texType])[pixel] = tempR | tempG | tempB | tempA;
				}
			}
		}

		CONTEXT->UpdateSubresource(pTexture->GetTexArr(), texType, NULL, (m_pArrPixel[texType]), Width * 4, Width * Height * 4);

		/*for (size_t i = 0; i < vecTex.size(); ++i)
		{
			for (int iMipLevel = 0; iMipLevel < pTexture->GetTexDesc().MipLevels;
				++iMipLevel)
			{
				D3D11_MAPPED_SUBRESOURCE	tMap = {};

				CONTEXT->Map(vecTex[i], iMipLevel, D3D11_MAP_READ,
					0, &tMap);

				CONTEXT->UpdateSubresource(pTexture->GetTexArr(),
					D3D11CalcSubresource(iMipLevel, i, pTexture->GetTexDesc().MipLevels),
					NULL, tMap.pData, tMap.RowPitch, tMap.DepthPitch);

				CONTEXT->Unmap(vecTex[i], iMipLevel);
			}
		}*/

		SAFE_RELEASE(pLandScape);
		SAFE_RELEASE(pLandScapeObj);
	}

//	{
//	CGameObject* pLandScapeObj = CGameObject::FindObject("LandScape");
//
//	if (pLandScapeObj)
//	{
//		UINT Height = pTexture->GetTexDesc().Height;
//		UINT Width = pTexture->GetTexDesc().Width;
//
//		if (m_pArrPixel == nullptr)
//		{
//			vector<ID3D11Texture2D*>&  vecTex = pTexture->getVecTex();
//
//			for (size_t i = 0; i < vecTex.size(); ++i)
//			{
//				for (int iMipLevel = 0; iMipLevel < pTexture->GetTexDesc().MipLevels;
//					++iMipLevel)
//				{
//					D3D11_MAPPED_SUBRESOURCE   tMap = {};
//
//					CONTEXT->Map(vecTex[i], iMipLevel, D3D11_MAP_READ,
//						0, &tMap);
//					//m_pArrPixel = reinterpret_cast<UINT32*>(tMap.pData);
//					m_pArrPixel = reinterpret_cast<UINT32*>(tMap.pData);
//					//memcpy(m_arrPixel2, tMap.pData, Height * Width * 4);
//
//					CONTEXT->Unmap(vecTex[i], iMipLevel);
//				}
//			}
//		}
//
//		CLandScape* pLandScape = pLandScapeObj->FindComponentFromTag<CLandScape>("LandScape");
//
//		POINT landSize = pLandScape->GetTerrainSize();
//
//		float fVal = (float)Height / landSize.x;
//
//		Vector3 vMouseUV;
//
//		vMouseUV.x = mousePos.x / (float)landSize.x;
//		vMouseUV.z = ((float)landSize.y - mousePos.z) / landSize.y;
//
//		vMouseUV.x *= Width;
//		vMouseUV.z *= Height;
//
//		for (UINT i = 0; i < Height; ++i)
//		{
//			for (UINT j = 0; j < Width; ++j)
//			{
//				// UV값
//				int pixel = i * Width + (j * 1);
//
//				Vector3 vPixelPos;
//				vPixelPos.x = (float)j;
//				vPixelPos.y = 0.f;
//				vPixelPos.z = (float)i;
//
//				float distance = vMouseUV.Distance(vPixelPos);
//
//				if (distance < m_fRange * fVal)
//				{
//					// 강제형변환은 오른쪽부터 크기만큼 짤림
//					// R
//					BYTE r = (BYTE)(m_pArrPixel[pixel] >> 8 * 3);
//					// G
//					BYTE g = (BYTE)(m_pArrPixel[pixel] >> 8 * 2);
//					// B
//					BYTE b = (BYTE)(m_pArrPixel[pixel] >> 8 * 1);
//					// A
//					BYTE a = (BYTE)(m_pArrPixel[pixel] >> 8 * 0);
//
//					if (r < 255 || g < 255
//						|| b < 255 || a < 255)
//					{
//						r += (BYTE)(power);
//						g += (BYTE)(power);
//						b += (BYTE)(power);
//						a += (BYTE)(power);
//					}
//
//					if (r + (BYTE)(power) > 255 ||
//						g + (BYTE)(power) > 255 ||
//						b + (BYTE)(power) > 255 ||
//						a + (BYTE)(power) > 255)
//					{
//						r = 255;
//						g = 255;
//						b = 255;
//						a = 255;
//					}
//
//					UINT32 tempR = (r << 8 * 3);
//					UINT32 tempG = (g << 8 * 2);
//					UINT32 tempB = (b << 8 * 1);
//					UINT32 tempA = (a << 8 * 0);
//					//
//					m_pArrPixel[pixel] = tempR | tempG | tempB | tempA;
//				}
//			}
//		}
//
//		CONTEXT->UpdateSubresource(pTexture->GetTexArr(), 0, NULL, m_pArrPixel, Width * 4, Width * Height * 4);
//
//
//		SAFE_RELEASE(pLandScape);
//		SAFE_RELEASE(pLandScapeObj);
//	}
//}
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
