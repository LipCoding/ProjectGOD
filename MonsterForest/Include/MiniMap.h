#pragma once

#include "Component/Script.h"
#include "Component/UIPanel.h"

PG_USING

// 미니맵을 현재 맵에서 나의 위치 주변에 대해서 확대해서 렌더링하거나
// 해당 맵에 해당하는 이미지를 확대해서 그린다.
// 주변 오브젝트 정보들의 위치를 노말라이즈해서 위치를 잡는다.
// 해당하는 위치에 플레이어 아이콘과 주변오브젝트 아이콘을 그린다.

class MiniMap :
	public CUIPanel
{
	// 주변 맵 지형과 유닛들을 표시.
	

public:
	MiniMap();
	~MiniMap();

public:
	bool initialize();
};

