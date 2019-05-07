#pragma once

#include "Component/Script.h"
#include "Component/UIPanel.h"
#include "Component/UIButton.h"

PG_USING

class PGMessageBox :
	public CUIPanel
{

	CUIButton* pConfirm;
	CUIButton* pCancel;
public:
	PGMessageBox();
	~PGMessageBox();

public:
	void initialize();
	int Update(float fTime);

public:
	void enableRender(bool show);
};

