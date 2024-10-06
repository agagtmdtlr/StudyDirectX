#include "stdafx.h"
#include <Windows.h>
#include "System.h"


int main()
{
	unique_ptr<System> system = make_unique<System>();

	system->Initialize();

	system->Run();

	system->Shutdown();

	return 0;
}
