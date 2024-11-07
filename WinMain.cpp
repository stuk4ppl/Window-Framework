#include <Windows.h>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include "Framework\include\Window.h"
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
	ShowConsole();
	//Cai tien them tu khoa inline, noexcept cho tung ham, truyen tham so duoi dang reference or pointer
	Window Wnd(640, 480, L"Hello");

	MSG msg;
	INT GetResult = 0;
	while ((GetResult = GetMessage(&msg, nullptr, 0,0)) > 0) { // Kiem tra xem return value cua GetMessages tra ve lon hon 0 ko, neu lon hon thi vong lap tiep tuc thuc thi
		// Filter dung de loc 1 khoang value cua thong diep nao do, vd WM_MOUSEFIRST va WM_MOUSELAST
		TranslateMessage(&msg); // Dich cac ki tu tren ban phim thanh thong diep WM_CHAR
		DispatchMessage(&msg); // Gui thong diep den Window Proceduce
	}

	if (GetResult == 0) { // If GetMessages return WM_QUIT
		return msg.wParam;
	}
	else if (GetResult == -1) { // If GetMessages got error
		DWORD error = GetLastError();
		return error;
	}
	else { // If GetMessages return normally
		return 0;
	}

}