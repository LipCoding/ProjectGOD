#pragma once
class Quest
{
	// 우측에 퀘스트 제목과 내용을 표시한다.

	// 퀘스트 창을 누르면 퀘스트 목록이 나온다.

	// 퀘스트 목록을 누르면 퀘스트내용이 나온다.
public:
	Quest();
	~Quest();

public:
	// 퀘스트의 내용을 파싱해서 담아둔다.
	bool initialize();
};

