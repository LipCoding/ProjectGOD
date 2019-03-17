#pragma once

#include <mutex>

template<typename T>
class Singleton
{
private:
	static T* instance;
	static std::once_flag flag;
public:
	static T* getInstance();
	static void release();
};