#pragma once

#include "Component/Script.h"
#include "Component/UIPanel.h"

PG_USING

// �̴ϸ��� ���� �ʿ��� ���� ��ġ �ֺ��� ���ؼ� Ȯ���ؼ� �������ϰų�
// �ش� �ʿ� �ش��ϴ� �̹����� Ȯ���ؼ� �׸���.
// �ֺ� ������Ʈ �������� ��ġ�� �븻�������ؼ� ��ġ�� ��´�.
// �ش��ϴ� ��ġ�� �÷��̾� �����ܰ� �ֺ�������Ʈ �������� �׸���.

class MiniMap :
	public CUIPanel
{
	// �ֺ� �� ������ ���ֵ��� ǥ��.
	

public:
	MiniMap();
	~MiniMap();

public:
	bool initialize();
};

