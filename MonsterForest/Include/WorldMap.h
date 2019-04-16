#pragma once

#include "Component/Script.h"
#include "Component/UIPanel.h"

PG_USING

// 월드맵
// 지역들을 UI로 이미지를 나눠서 세팅
// 지역이미지 UI를 클릭하면 세부지역에 대한 이미지가나옴.

class WorldMap
{
	// 전체 월드맵을 보여줌
	// 월드맵의 각 지역별로 클릭하면 해당 지역이 확대되어 보임.
	// 해당 지역에 각 요소에 MouseOn 이벤트.
	// MouseOn인 경우 PopUp창을 띄워서 해당요소를 설명.
public:
	WorldMap();
	~WorldMap();
};

