#include "stdafx.h"
#include <Windows.h>
#include "SystemClass.h"


int main()
{
	unique_ptr<SystemClass> system = make_unique<SystemClass>();

	system->Initialize();

	system->Run();

	system->Shutdown();

	return 0;
}
