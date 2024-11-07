#pragma once
#include <Windows.h>
#include <string>
#include <sstream>

class Window
{
private:
	class WindowClass {
	public: // interface
		static const wchar_t* GetName();
	private:
		WindowClass();

		~WindowClass();

		inline static const wchar_t* m_WndClassName = L"WndClass";

		WNDCLASSEX m_WndClassEx;

		HINSTANCE m_hInstance;
	};

private:
	HWND m_hWnd; 

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	Window(const Window&) = delete;

	Window& operator=(const Window&) = delete;

public:// Main interface
	Window(int, int, const wchar_t*);

	~Window();
};

