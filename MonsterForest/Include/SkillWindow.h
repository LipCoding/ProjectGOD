#pragma once

#include "Component/Script.h"
#include "Component/UIPanel.h"

PG_USING

class SkillWindow
{
	// 스킬 아이콘을 갖고있음.
public:
	SkillWindow();
	~SkillWindow();

public:
	// 현재 캐릭터 스킬정보를 세팅.
	bool initialize();
};

