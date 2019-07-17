#pragma once
#include "Component/Script.h"
#include "GameObject/GameObject.h"
#include "Component/LandScape.h"
#include "Resources/Texture.h"

PG_USING


class CBrushTool :
	public CScript
{
public:
	//엡실론 (나중에 Math에 추가예정)
	bool sameFloat(const float& a, const float& b)
	{
		if (fabsf(a - b) < FLT_EPSILON)
			return true;
		else
			return false;
	}

public:
	CBrushTool();
	CBrushTool(const CBrushTool& brushtool);
	~CBrushTool();

public:
	float GetBrushRange() { return m_fRange; }
	bool  GetBrushCheck() { return m_bBrushCheck; }
	bool  GetHeightCheck() { return m_bHeightCheck; }
	bool  GetSplattingCheck() { return m_bSplattingCheck; }

	void SetBrushCheck(bool check) { m_bBrushCheck = check; }
	void SetBrushCheck_Other(bool check) { m_bBrushCheck_Other = check; }
	void SetHeightCheck(bool check) { m_bHeightCheck = check; }
	void SetSlattingCheck(bool check) { m_bSplattingCheck = check; }
	void SetBrushInformation(float range);
	void SetBrushInformation(Vector4 color);
	void SetBrushInformation(Vector3 mousePos);
	void SetBrushInformation_Other(Vector3 pos);
	
	void SetSpeed(float Speed);
	void SetSpeedDir(bool check);
	void SetPower(float power);
	void SetUpType(HEIGHTUP_TYPE type) { m_eUpType = type; }
	void SetTexType(int type) { m_iTexType = type; }
	void SetArrPixel(int type);
public:
	void MoveHeight(list<QUADTREENODE*>* list, Vector3 mousePos, const float& fTime);
	void MoveHeightZero(list<QUADTREENODE*>* list, Vector3 mousePos);
	void MovePixel(Vector3 mousePos, const float& fTime, bool erase = false);
	void ResetHeight();

	void SettingOriginPixelToTexture(int texType);

	void Save_AlphaSplat_Bitmap(string fileName, int idx);
	void HBITMAP2BMP(HBITMAP hBitmap, string fileName);
private:
	void FindNeighborNodes(QUADTREENODE* &up, QUADTREENODE* &down, QUADTREENODE* &left, QUADTREENODE* &right,
		QUADTREENODE* &node, POINT landScapeSize, Vector3 &myNodePos, list<QUADTREENODE*>* &list);
	void SmoothOutLine(QUADTREENODE* &neigborNode, QUADTREENODE* &node, int currentIndex, float iterPos,
		float limitPos, float iterLimitPosX, float iterLimitPosZ, float deltaTime, char typeCheck);
	void SmoothInLine(QUADTREENODE* &node, int srcIndex, int index, float deltaTime);
private:
	void UpdateVtxBuffer(MESHCONTAINER* info, vector<VERTEXBUMP>& vtx);
	void UpdateTextureBuffer(int texType, CTexture* pTexture, Vector3 mousePos, float power, bool erase);
private:
	bool  m_bBrushCheck = false;
	bool  m_bBrushCheck_Other = false;
	bool  m_bHeightCheck = true;
	bool  m_bSplattingCheck = false;
	float m_fRange = 1.f;
	float m_fUpSpeed = 10.f;
	float m_fPower = 50.f;
	HEIGHTUP_TYPE   m_eUpType = UP_TYPE_FLAT;
	int		m_iTexType = 0;
	Vector4 m_vColor = Vector4::Red;

	UINT32* m_pArrPixel[4] = { nullptr };
	BYTE* m_pArrPixel_Byte = nullptr;

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual CScript* Clone();
};

