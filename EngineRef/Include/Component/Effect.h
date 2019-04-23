#pragma once
#include "Component.h"
#include "EffectAssist.h"

PG_BEGIN

class PG_DLL CEffect : 
	public CComponent
{
private:
	friend class CGameObject;

public:
	CEffect();
	CEffect(const CEffect& effect);
	~CEffect();

public:
	void SetMainStartTime(const float& start) { m_MainStartTime = start; }
	void SetMainEndTime(const float& end) { m_MainEndTime = end; }

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual CEffect* Clone();

public:
	/* Load Effect Mesh */
	bool LoadEffectMesh(const string& filePath, const string& fileName);
	bool LoadEffectLocalInfo(const string& filePath);
	bool CreateEffectCollider();

	/* Material */
	void SetEffectTexture(const string& name, const string& fullPath);

	/* Add Assist Effect */
	/* Pattern */
	void AddPatternScale(const float& start, const float& end,
						 const float& powX, const float& powY, const float& powZ, 
						 const int& repeat);
	void AddPatternRotation(
						 const float& start, const float& end,
						 const float& powX, const float& powY, const float& powZ,
						 const int& repeat);
	/* Fade In Out */
	void AddFadeIn(const float& start, const float& end, const float& degree);
	void AddFadeOut(const float& start, const float& end, const float& degree);

	/* UV */
	void AddUVAnimation(const float& start, const float& end,
						const int& width, const int& height, const int& repeat, const int& num);
	void AddUVMovement(const float& start, const float& end, const int& moveX, const int& moveY);

private:
	/* Container */
	vector<CEffectAssist*> m_vecAssist;

	/* Check */
	bool OperationCheck = false;

	/* Timer */
	float m_MainStartTime = 0.f;
	float m_MainEndTime = 0.f;
	float m_Timer = 0.f;

	/* Repeat */
	int m_Repeat = 0;

	/* Name */
	string EffectName = "";
	string MeshPath = "";
	string FileName = "";

	/* Class */
	class CRenderer *m_pRenderer = nullptr;

	/* Fade */
	float m_vAlpha = 1.f;

	/* Angle */
	float m_Angle = 0.f;

	/* Info */
	Vector3 m_vAngle;
	Vector3 m_vScale;
	Vector3 m_vPos;
};

PG_END
