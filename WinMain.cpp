#include <Windows.h>
#include <iostream>

#include "Misc\include\AntiException.h"
#include "App\include\App.h"
#pragma warning(disable : 28251)

void ShowConsole() {
	AllocConsole(); // For debug
	FILE* f;
	freopen_s(&f, "CONOUT$", "w", stdout);
}

int __stdcall WinMain(HINSTANCE hInstance,
					  HINSTANCE hPrevInstance,
					  LPSTR lpCmdParameter,
					  int nCmdShow)
{
	// Hoc them ve macro, hoc ban chat cua macro
	ShowConsole();
	try {
		App::GetAppInstance().Init();
		DWORD ExitCode = App::GetAppInstance().Run();
		return ExitCode;
		
	}
	catch (const Window::Exception& e) {
		MessageBoxA(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
		return e.GetErrorCode();
	}
	catch (const std::exception& e) {
		MessageBoxA(nullptr, e.what(), "Return value 2: Standart Exception", MB_OK | MB_ICONEXCLAMATION);
		return 2;
	}
	catch (...) {
		MessageBoxA(nullptr, "No Detail Exception", "Return value 1: Unknow Exception", MB_OK | MB_ICONEXCLAMATION);
		return 1;
	}
	return 0;
}