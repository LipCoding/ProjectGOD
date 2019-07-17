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

void CBrushTool::SetBrushInformation_Other(Vector3 pos)
{
	CGameObject* pLandScapeObj = CGameObject::FindObject("LandScape");

	if (pLandScapeObj)
	{
		CLandScape* pLandScape = pLandScapeObj->FindComponentFromTag<CLandScape>("LandScape");

		pLandScape->SetBrushCheck_Other(m_bBrushCheck_Other);
		pLandScape->SetBrushInformation_Other(pos);

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

void CBrushTool::SetArrPixel(int type)
{
	CTexture* pTexture = GET_SINGLE(CResourcesManager)->FindTexture("SplatAlpha");
	if (pTexture == nullptr)
		return;

	vector<ID3D11Texture2D*>&  vecTex = pTexture->getVecTex();

	UINT Height = pTexture->GetTexDesc().Height;
	UINT Width = pTexture->GetTexDesc().Width;

	D3D11_MAPPED_SUBRESOURCE   tMap = {};

	if (m_pArrPixel[type] == nullptr)
	{
		m_pArrPixel[type] = new UINT32[Height * Width];

		CONTEXT->Map(vecTex[type], 0, D3D11_MAP_READ,
			0, &tMap);
		m_pArrPixel_Byte = reinterpret_cast<BYTE*>(tMap.pData);
		CONTEXT->Unmap(vecTex[type], 0);

		for (UINT i = 0; i < Height; ++i)
		{
			for (UINT j = 0; j < Width; ++j)
			{
				// UV값
				int pixel = i * tMap.RowPitch + (j * 4);
				int pixel2 = i * Width + (j * 1);

				// R
				BYTE* a = &(m_pArrPixel_Byte[pixel]);
				// G
				BYTE* b = (&m_pArrPixel_Byte[pixel] + 1);
				// B
				BYTE* g = (&m_pArrPixel_Byte[pixel] + 2);
				// A
				BYTE* r = (&m_pArrPixel_Byte[pixel] + 3);

				UINT32 tempR = (*r << 8 * 3);
				UINT32 tempG = (*g << 8 * 2);
				UINT32 tempB = (*b << 8 * 1);
				UINT32 tempA = (*a << 8 * 0);
				//
				(m_pArrPixel[type])[pixel2] = tempR | tempG | tempB | tempA;
			}
		}
	}

	SAFE_RELEASE(pTexture);
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
				{
					float fGap = 0.f;
					int iScrIndex = 0;
					float fAverValue = 0.2f;
					float GAP = 4.f;

					CGameObject* pLandScapeObj = CGameObject::FindObject("LandScape");
					CLandScape* pLandScape = pLandScapeObj->FindComponentFromTag<CLandScape>("LandScape");
					POINT LandScapeSize = pLandScape->GetTerrainSize();
					{
						Vector3 myNodePos = { node->fCenterX, 0.f, node->fCenterZ };
						//주변 노드 찾기
						QUADTREENODE* pUpNode = nullptr;
						QUADTREENODE* pDownNode = nullptr;
						QUADTREENODE* pLeftNode = nullptr;
						QUADTREENODE* pRightNode = nullptr;

						FindNeighborNodes(pUpNode, pDownNode, pLeftNode, pRightNode,
							node, LandScapeSize, myNodePos, list);

						int iNumSizeX = int(node->vMax.x - node->vMin.x) + 1;

						// 노드 내부 정점 변화
						//X
						if (iter.vPos.x < node->vMax.x &&
							iter.vPos.x > node->vMin.x)
						{
							/*Left*/
							if (0 < (iIndex - 1) && node->vecVtx[iIndex - 1].vPos.x >= node->vMin.x)
								SmoothInLine(node, iIndex - 1, iIndex, fTime);
							

							/*Right*/
							if (node->vecVtx.size() > (iIndex + 1) && node->vecVtx[iIndex + 1].vPos.x <= node->vMax.x)
								SmoothInLine(node, iIndex + 1, iIndex, fTime);
						}
						//Z
						if (iter.vPos.z < node->vMax.z &&
							iter.vPos.z > node->vMin.z)
						{
							/*Up*/
							if (node->vecVtx.size() > (iIndex + iNumSizeX) && node->vecVtx[iIndex + iNumSizeX].vPos.z <= node->vMax.z)
								SmoothInLine(node, iIndex + iNumSizeX, iIndex, fTime);

							/*Down*/
							if (0 < (iIndex - iNumSizeX) && node->vecVtx[iIndex - iNumSizeX].vPos.z >= node->vMin.z)
								SmoothInLine(node, iIndex - iNumSizeX, iIndex, fTime);
						}

						// 노드 외곽  정점 변화
						// 상 - 하 좌 우
						SmoothOutLine(pUpNode, node, iIndex, iter.vPos.z, node->vMax.z,
							iter.vPos.x, iter.vPos.z, fTime, 'Z');
						// 우 - 상 하 좌
						SmoothOutLine(pRightNode, node, iIndex, iter.vPos.x, node->vMax.x,
							iter.vPos.x, iter.vPos.z, fTime, 'X');

						// 좌 - 상 하 우
						SmoothOutLine(pLeftNode, node, iIndex, iter.vPos.x, node->vMin.x,
							iter.vPos.x - 1, iter.vPos.z, fTime, 'X');
						// 하 - 상 좌 우
						SmoothOutLine(pDownNode, node, iIndex, iter.vPos.z, node->vMin.z,
							iter.vPos.x, iter.vPos.z - 1, fTime, 'Z');
					}
					SAFE_RELEASE(pLandScape);
					SAFE_RELEASE(pLandScapeObj);
					break;
				}
				//
				}

				UpdateVtxBuffer(&node->MeshInfo, node->vecVtx);
			}
			iIndex++;
		}
	}
}

void CBrushTool::MoveHeightZero(list<QUADTREENODE*>* list, Vector3 mousePos)
{
	for (auto& node : *list)
	{
		int iIndex = 0;

		for (auto& iter : node->vecVtx)
		{
			float fDistance = iter.vPos.Distance(mousePos);

			if (fDistance < m_fRange)
			{
				iter.vPos.y = 0 ;
				UpdateVtxBuffer(&node->MeshInfo, node->vecVtx);
			}
			iIndex++;
		}
	}
}

void CBrushTool::MovePixel(Vector3 mousePos, const float & fTime, bool erase)
{
	CTexture* pTexture = GET_SINGLE(CResourcesManager)->FindTexture("SplatAlpha");
	if (pTexture == nullptr)
		return;

	UpdateTextureBuffer(m_iTexType, pTexture, mousePos, m_fPower * fTime, erase);
	SAFE_RELEASE(pTexture);
}

void CBrushTool::ResetHeight()
{
	CGameObject* pLandScapeObj = CGameObject::FindObject("LandScape");

	if (pLandScapeObj)
	{
		CLandScape* pLandScape = pLandScapeObj->FindComponentFromTag<CLandScape>("LandScape");

		list<QUADTREENODE*>* listNode = pLandScape->GetAllNodes();

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

void CBrushTool::SettingOriginPixelToTexture(int texType)
{
	CTexture* pTexture = GET_SINGLE(CResourcesManager)->FindTexture("SplatAlpha");
	if (pTexture == nullptr)
		return;

	vector<ID3D11Texture2D*>&  vecTex = pTexture->getVecTex();

	if (vecTex.size() == 0 ||
		vecTex.size() < texType + 1)
	{
		return;
	}

	UINT Height = pTexture->GetTexDesc().Height;
	UINT Width = pTexture->GetTexDesc().Width;

	CONTEXT->UpdateSubresource(pTexture->GetTexArr(), texType, NULL, (m_pArrPixel[texType]), Width * 4, Width * Height * 4);
	SAFE_RELEASE(pTexture);
}

void CBrushTool::Save_AlphaSplat_Bitmap(string fileName, int idx)
{
	CTexture* pTexture = GET_SINGLE(CResourcesManager)->FindTexture("SplatAlpha");
	if (pTexture == nullptr || m_pArrPixel[idx] == nullptr)
		return;

	fileName += ".bmp";

	// 이미지 크기
	UINT height = pTexture->GetTexDesc().Height;
	UINT width = pTexture->GetTexDesc().Width;

	HBITMAP my_bmp = CreateBitmap(width, height, 1, GetDeviceCaps(GetDC(NULL), BITSPIXEL), m_pArrPixel[idx]);

	HBITMAP2BMP(my_bmp, fileName);

	SAFE_RELEASE(pTexture);
}

void CBrushTool::HBITMAP2BMP(HBITMAP hBitmap, string fileName)
{
	// DC를 얻어옴
	HDC hdc = GetDC(NULL);

	// bitmap info 를 얻어옴
	BITMAPINFO bmpInfo;
	ZeroMemory(&bmpInfo, sizeof(BITMAPINFO));
	bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	GetDIBits(hdc, hBitmap, 0, 0, NULL, &bmpInfo, DIB_RGB_COLORS);
	if (bmpInfo.bmiHeader.biSizeImage <= 0)
		bmpInfo.bmiHeader.biSizeImage = bmpInfo.bmiHeader.biWidth*abs(bmpInfo.bmiHeader.biHeight)*(bmpInfo.bmiHeader.biBitCount + 7) / 8;

	// 실제 image 내용을 얻어오기
	LPVOID pBuf = NULL;
	if ((pBuf = malloc(bmpInfo.bmiHeader.biSizeImage)) == NULL)
	{
		MessageBox(NULL, L"Unable to Allocate Bitmap Memory", L"Error", MB_OK | MB_ICONERROR);
		return;
	}

	bmpInfo.bmiHeader.biCompression = BI_RGB;
	GetDIBits(hdc, hBitmap, 0, bmpInfo.bmiHeader.biHeight, pBuf, &bmpInfo, DIB_RGB_COLORS);

	// bitmap file header 만들기
	BITMAPFILEHEADER bmpFileHeader;
	bmpFileHeader.bfReserved1 = 0;
	bmpFileHeader.bfReserved2 = 0;
	bmpFileHeader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + bmpInfo.bmiHeader.biSizeImage;
	bmpFileHeader.bfType = 'MB';
	bmpFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

	// 파일을 열고 쓰기
	FILE* fp = fopen(fileName.c_str(), "wb");
	if (fp == NULL)
	{
		MessageBox(NULL, L"Unable to Create Bitmap File", L"Error", MB_OK | MB_ICONERROR);
		return;
	}
	fwrite(&bmpFileHeader, sizeof(BITMAPFILEHEADER), 1, fp);
	fwrite(&bmpInfo.bmiHeader, sizeof(BITMAPINFOHEADER), 1, fp);
	fwrite(pBuf, bmpInfo.bmiHeader.biSizeImage, 1, fp);

	if (hdc) ReleaseDC(NULL, hdc);
	if (pBuf) free(pBuf);
	if (fp) fclose(fp);
}

void CBrushTool::FindNeighborNodes(QUADTREENODE *& up, QUADTREENODE *& down, QUADTREENODE *& left, QUADTREENODE *& right, QUADTREENODE *& node, POINT landScapeSize, Vector3 & myNodePos, list<QUADTREENODE*>*& list)
{
	// 상
	float minDistance = 1000000.f;
	// 노드의 Max가 맨 위쪽인 경우는 상 노드가 없음
	if (node->vMax.z < (float)landScapeSize.y)
	{
		for (const auto searchNode : *list)
		{
			Vector3 otherNodePos = { searchNode->fCenterX, 0.f, searchNode->fCenterZ };

			float fDistance = myNodePos.Distance(otherNodePos);

			if (otherNodePos.z > myNodePos.z &&
				fDistance < minDistance)
			{
				minDistance = fDistance;
				up = searchNode;
			}
		}
	}
	// 하
	minDistance = 1000000.f;
	// 노드의 Min이 맨 밑인 경우는 하 노드가 없음
	if (node->vMin.z > 0.f)
	{
		for (const auto searchNode : *list)
		{
			Vector3 otherNodePos = { searchNode->fCenterX, 0.f, searchNode->fCenterZ };

			float fDistance = myNodePos.Distance(otherNodePos);

			if (otherNodePos.z < myNodePos.z &&
				fDistance < minDistance)
			{
				minDistance = fDistance;
				down = searchNode;
			}
		}
	}
	// 좌
	minDistance = 1000000.f;
	// 노드의 Min이 맨 왼쪽인 경우는 좌 노드가 없음
	if (node->vMin.x > 0.f)
	{
		for (const auto searchNode : *list)
		{
			Vector3 otherNodePos = { searchNode->fCenterX, 0.f, searchNode->fCenterZ };

			float fDistance = myNodePos.Distance(otherNodePos);

			if (otherNodePos.x < myNodePos.x &&
				fDistance < minDistance)
			{
				minDistance = fDistance;
				left = searchNode;
			}
		}
	}
	// 우
	minDistance = 1000000.f;
	// 노드의 Max가 맨 오른쪽인 같은 경우는 좌 노드가 없음
	if (node->vMax.x < landScapeSize.x)
	{
		for (const auto searchNode : *list)
		{
			Vector3 otherNodePos = { searchNode->fCenterX, 0.f, searchNode->fCenterZ };

			float fDistance = myNodePos.Distance(otherNodePos);

			if (otherNodePos.x > myNodePos.x &&
				fDistance < minDistance)
			{
				minDistance = fDistance;
				right = searchNode;
			}
		}
	}
}

void CBrushTool::SmoothOutLine(QUADTREENODE *& neigborNode, QUADTREENODE *& node, int currentIndex, float iterPos, float limitPos, float iterLimitPosX, float iterLimitPosZ, float deltaTime, char typeCheck)
{
	if (!neigborNode)
		return;

	float iterLimitPosX_Other = iterLimitPosX;
	float iterLimitPosZ_Other = iterLimitPosZ;

	if (typeCheck == 'X')
	{
		iterLimitPosX_Other += 1.f;
	}
	else if (typeCheck == 'Z')
	{
		iterLimitPosZ_Other += 1.f;
	}
	else
		return;

	float fGap = 0.f;
	float fAverValue = 0.2f;
	float GAP = 4.f;

	//if (iterPos == limitPos)
	if (sameFloat(iterPos, limitPos))
	{

		Vector3* pos_otherNode = nullptr;
		Vector3* pos_otherNode_samePos = nullptr;
		for (auto& iter_otherNode : neigborNode->vecVtx)
		{
			/*if (iter_otherNode.vPos.z == iterLimitPosZ &&
				iter_otherNode.vPos.x == iterLimitPosX)*/
			if(sameFloat(iter_otherNode.vPos.z, iterLimitPosZ) &&
				sameFloat(iter_otherNode.vPos.x, iterLimitPosX))
			{
				pos_otherNode_samePos = &iter_otherNode.vPos;
			}

			if (sameFloat(iter_otherNode.vPos.z, iterLimitPosZ_Other) &&
				sameFloat(iter_otherNode.vPos.x, iterLimitPosX_Other))
			{
				pos_otherNode = &iter_otherNode.vPos;
				break;
			}
		}

		if (pos_otherNode)
		{
			fGap = abs(node->vecVtx[currentIndex].vPos.y - (*pos_otherNode).y);

			if (fGap > fAverValue)
			{
				fGap /= GAP;

				if (node->vecVtx[currentIndex].vPos.y > (*pos_otherNode).y)
				{
					node->vecVtx[currentIndex].vPos.y -= fGap * deltaTime* 3.f;
				}
				else
				{
					node->vecVtx[currentIndex].vPos.y += fGap * deltaTime* 3.f;		
				}			
			}

			
		}

		if (pos_otherNode_samePos)
			(*pos_otherNode_samePos).y = node->vecVtx[currentIndex].vPos.y;
	}
}

void CBrushTool::SmoothInLine(QUADTREENODE *& node, int srcIndex, int index, float deltaTime)
{
	float fGap = 0.f;
	int iScrIndex = 0;
	float fAverValue = 0.2f;
	float GAP = 4.f;

	iScrIndex = srcIndex;
	fGap = abs(node->vecVtx[index].vPos.y - node->vecVtx[iScrIndex].vPos.y);

	if (fGap > fAverValue)
	{
		fGap /= GAP;

		if (node->vecVtx[index].vPos.y > node->vecVtx[iScrIndex].vPos.y)
		{
			node->vecVtx[index].vPos.y -= fGap * deltaTime * 3.f;
			node->vecVtx[iScrIndex].vPos.y += fGap * deltaTime* 3.f;
		}
		else
		{
			node->vecVtx[index].vPos.y += fGap * deltaTime* 3.f;
			node->vecVtx[iScrIndex].vPos.y -= fGap * deltaTime* 3.f;
		}
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

void CBrushTool::UpdateTextureBuffer(int texType, CTexture* pTexture, Vector3 mousePos, float power, bool erase)
{
	CGameObject* pLandScapeObj = CGameObject::FindObject("LandScape");

	if (pLandScapeObj)
	{
		vector<ID3D11Texture2D*>&  vecTex = pTexture->getVecTex();

		if (vecTex.size() == 0 ||
			vecTex.size() < texType + 1)
		{
			SAFE_RELEASE(pLandScapeObj);
			return;
		}

		UINT Height = pTexture->GetTexDesc().Height;
		UINT Width = pTexture->GetTexDesc().Width;

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

					if (erase)
					{
						if (r - (BYTE)(power) < 0)
							r = 0;
						if (g - (BYTE)(power) < 0)
							g = 0;
						if (b - (BYTE)(power) < 0)
							b = 0;
						if (a - (BYTE)(power) < 0)
							a = 0;

						if (r > 0)
							r -= (BYTE)(power);
						if (g > 0)
							g -= (BYTE)(power);
						if (b > 0)
							b -= (BYTE)(power);
						if (a > 0)
							a -= (BYTE)(power);
					}
					else
					{
						if (r + (BYTE)(power) > 255)
							r = 255;
						if (g + (BYTE)(power) > 255)
							g = 255;
						if (b + (BYTE)(power) > 255)
							b = 255;
						if (a + (BYTE)(power) > 255)
							a = 255;

						if (r < 255)
							r += (BYTE)(power);
						if (g < 255)
							g += (BYTE)(power);
						if (b < 255)
							b += (BYTE)(power);
						if (a < 255)
							a += (BYTE)(power);
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
