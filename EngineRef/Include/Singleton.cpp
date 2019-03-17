#include "Singleton.h"

template<typename T>
T* Singleton<T>::instance = nullptr;

template<typename T>
std::once_flag  Singleton<T>::flag;

template<typename T>
T * Singleton<T>::getInstance()
{
	std::call_once(flag, []()
	{
		instance = new T();
	});
	return instance;
}

template<typename T>
void Singleton<T>::release()
{
	if (nullptr != instance)
		delete instance;
}