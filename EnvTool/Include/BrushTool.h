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
	CBrushTool();
	CBrushTool(const CBrushTool& brushtool);
	~CBrushTool();

public:
	float GetBrushRange() { return m_fRange; }
	bool  GetBrushCheck() { return m_bBrushCheck; }
	bool  GetHeightCheck() { return m_bHeightCheck; }
	bool  GetSplattingCheck() { return m_bSplattingCheck; }

	void SetBrushCheck(bool check) { m_bBrushCheck = check; }
	void SetHeightCheck(bool check) { m_bHeightCheck = check; }
	void SetSlattingCheck(bool check) { m_bSplattingCheck = check; }
	void SetBrushInformation(float range);
	void SetBrushInformation(Vector4 color);
	void SetBrushInformation(Vector3 mousePos);
	
	void SetSpeed(float Speed);
	void SetSpeedDir(bool check);
	void SetUpType(HEIGHTUP_TYPE type) { m_eUpType = type; }

public:
	void MoveHeight(list<QUADTREENODE*>* list, Vector3 mousePos, const float& fTime);
	void MovePixel(list<QUADTREENODE*>* list, Vector3 mousePos, const float& fTime);
	void ResetHeight();

private:
	void UpdateVtxBuffer(MESHCONTAINER* info, vector<VERTEXBUMP>& vtx);
	void UpdateTextureBuffer(CTexture* pTexture ,Vector3 mousePos, float power);
	void UpdateTextureBuffer_2(CTexture* pTexture, Vector3 mousePos, float power);
private:
	bool  m_bBrushCheck = false;
	bool  m_bHeightCheck = true;
	bool  m_bSplattingCheck = false;
	float m_fRange = 1.f;
	float m_fUpSpeed = 10.f;
	float m_fPower = 1.f;
	HEIGHTUP_TYPE   m_eUpType = UP_TYPE_FLAT;
	Vector4 m_vColor = Vector4::Red;

	BYTE m_arrPixel[129 * 129] = {0};
	UINT32 m_arrPixel2[129 * 129] = { 0 };
	
public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual CScript* Clone();
};

