

/*
简单的全局对象结构
2008 8 21
*/
#pragma once

template <typename T>
class CSingleton
{
public:
	static T* getInstance()
	{
		static T instance;
		return &instance;
	}
};


