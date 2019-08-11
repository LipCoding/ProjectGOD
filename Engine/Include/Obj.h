#pragma once

#include "PGEngine.h"

PG_BEGIN

class PG_DLL CObj
{
protected:
	CObj();
	virtual ~CObj();

protected:
	
	string				m_strTag;
	string				m_strTypeName;
	const type_info*	m_pTypeID;
	bool				m_bEnable;
	bool				m_bRenderEnable;
	bool				m_bAlive;

public:
	int					m_iRefCount;
	string GetTag()	const;
	string GetTypeName()	const;
	const type_info* GetTypeID()	const;
	bool GetEnable()	const;
	bool GetAlive()	const;
	bool GetRenderEnable() const;
	
public:
	void SetTag(const string& strTag);
	void SetTypeName(const string& strTypeName);
	template <typename T>
	void SetTypeID()
	{
		m_pTypeID = &typeid(T);
	}

	void Enable(bool bEnable);
	void SetRenderEnable(bool bRenderEnable);
	void Die();

public:
	int Release();
	void AddRef();
};

PG_END
