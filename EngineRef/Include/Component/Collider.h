#pragma once
#include "Component.h"

PG_BEGIN

class PG_DLL CCollider :
	public CComponent
{
protected:
	CCollider();
	CCollider(const CCollider& coll);
	virtual ~CCollider() = 0;

protected:
	COLLIDER_TYPE	m_eCollType;
	list<CCollider*>	m_CollList;
	Vector3			m_vPrevPos;
	Vector3			m_vMove;
	string			m_strGroup;
	Vector3			m_vCollMin;
	Vector3			m_vCollMax;
	Vector3			m_vCenter;
	float			m_fRadius;
	vector<int>		m_vecSectionIndex;

	class CMesh*		m_pMesh;
	class CShader*		m_pShader;
	ID3D11InputLayout*	m_pLayout;

	TRANSFORMCBUFFER	m_tTransform;
	VIEW_TYPE			m_eViewType;
	class CRenderState*	m_pWireFrame;

	bool			m_bColliderRenderCheck;
public:
	Vector4				m_vColor;

public:
	Vector4 getColor() { return this->m_vColor; }
	void setColor(const Vector4 color) { this->m_vColor = color; }
public:
	Vector3 GetColliderMin()	const;
	Vector3 GetColliderMax()	const;
	Vector3 GetColliderCenter();
	float GetColliderRadius();
	COLLIDER_TYPE GetColliderType()	const;
	void AddCollList(CCollider* pCollider);
	bool CheckCollList(CCollider* pCollider);
	bool CheckCollList(const string& strTag);
	void EraseCollList(CCollider* pCollider);
	void SetViewType(VIEW_TYPE eType);
	string GetCollisionGroup()	const;
	void SetCollisionGroup(const string& strGroup);
	void AddSectionIndex(int iIndex);
	void ClearSectionIndex();
	void CheckCollisionSection(float fTime);
	void SetColliderRenderCheck(bool check);
public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual int Update(float fTime);
	virtual int LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void Render(float fTime);
	virtual void ColliderRender(float fTime);
	virtual CCollider* Clone() = 0;
	virtual void OnCollisionEnter(class CCollider* pSrc, class CCollider* pDest,
		float fTime);
	virtual void OnCollision(class CCollider* pSrc, class CCollider* pDest,
		float fTime);
	virtual void OnCollisionLeave(class CCollider* pSrc, class CCollider* pDest,
		float fTime);

public:
	virtual bool Collision(CCollider* pDest) = 0;

protected:
	bool CollisionRectToRect(const RECTINFO& rc1, const RECTINFO& rc2);
	bool CollisionRectToPoint(const RECTINFO& rc, const Vector2& point);
	bool CollisionObb2DToObb2D(const OBB2DINFO& tObb1, const OBB2DINFO& tObb2);
	bool CollisionAABBToAABB(const AABB& src, const AABB& dest);
	bool CollisionRayToAABB(PRAY tRay, const AABB& dest);
	bool CollisionRayToSphere(PRAY tRay, const SPHERE& tSphere);
	bool CollisionSphereToSphere(const SPHERE& tSrc, const SPHERE& tDest);
	bool CollisionSphereToAABB(const AABB& tDest, const SPHERE & tSrc);
};

PG_END
