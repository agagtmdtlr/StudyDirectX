#pragma once
class ServiceLoactor
{
private:
	static unordered_map<string,void*> services;

public:
	template<typename T>
	static void RegisterService(T* service)
	{
		
		services[typeid(T).name()] = service;
	}

	template<typename T>
	static T* GetService()
	{
		T* service = (T*)(services[typeid(T).name()]);
		return service;
	}
};

