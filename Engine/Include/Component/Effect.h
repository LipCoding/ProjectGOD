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
	vector<CEffectAssist*>* GetAssists() { return &m_vecAssist; }
	CEffectAssist* GetAssistFromType(CEffectAssist::ASSIST_TYPE type);

	float GetMainStartTime() { return m_MainStartTime; }
	float GetMainEndTime() { return m_MainEndTime; }
	float GetMainTime() { return m_Timer; }

	int GetMainRepeat() { return m_Repeat; }

	const string& GetMeshPath() { return MeshPath; }
	const string& GetLocalPath() { return LocalPath; }
	const string& GetTexturePath() { return TexturePath; }

	void SetOperationCheck(bool check);
	void SetErase(bool check);
	void SetOperationCheckPart(CEffectAssist::ASSIST_TYPE type, bool check);

	void SetMainStartTime(const float& start) { m_MainStartTime = start; }
	void SetMainEndTime(const float& end) { m_MainEndTime = end; }
	void SetRepeat(const int& repeat) { m_Repeat = repeat; }


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
	void AddPatternScale(const int& easeType,
						 const float& start, const float& end,
						 const float& powX, const float& powY, const float& powZ, 
						 const int& repeat);
	void AddPatternRotation(const int& easeType,
						 const float& start, const float& end,
						 const float& powX, const float& powY, const float& powZ,
						 const int& repeat);

	/* Fade In Out */
	void AddFadeIn(const float& start, const float& end, const float& degree);
	void AddFadeOut(const float& start, const float& end, const float& degree);

	/* UV */
	void AddUVAnimation(const float& start, const float& end, const int& num, const int& repeat);
	void AddUVMovement(const float& start, const float& end, const float& moveX, const float& moveY);

	/* Delete Assist Effect */
	void DeleteAssistEffectFromType(CEffectAssist::ASSIST_TYPE type);

private:
	/* Container */
	vector<CEffectAssist*> m_vecAssist;

	/* Check */
	bool m_OperationCheck = false;
	bool m_EraseCheck = false;

	/* Timer */
	float m_MainStartTime = 0.f;
	float m_MainEndTime = 0.f;
	float m_Timer = 0.f;

	/* Repeat */
	int m_Repeat = 0;

	/* Name */
	string EffectName = "";
	string FileName = "";

	string TextureFullPath = "";

	string MeshPath = "";
	string LocalPath = "";
	string TexturePath = "";


	/* Class */
	class CRenderer *m_pRenderer = nullptr;

	SHARECBUFFER     m_tshareBuffer = {};
};

PG_END
