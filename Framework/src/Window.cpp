#include "../include/Window.h"
#include <iostream>

const wchar_t* Window::WindowClass::GetName() {
	static WindowClass wndClass;
	return m_WndClassName;
}

Window::WindowClass::WindowClass() : m_hInstance(GetModuleHandle(nullptr)) {
	m_WndClassEx = { 0 };
	m_WndClassEx.cbSize = sizeof(m_WndClassEx); // Class Byte Size
	m_WndClassEx.style = CS_OWNDC;
	m_WndClassEx.lpfnWndProc = Window::WndProc;
	m_WndClassEx.cbClsExtra = 0;
	m_WndClassEx.cbWndExtra = 0;
	m_WndClassEx.hInstance = m_hInstance;
	m_WndClassEx.hIcon = NULL;
	m_WndClassEx.hCursor = NULL;
	m_WndClassEx.hbrBackground = NULL;
	m_WndClassEx.lpszClassName = m_WndClassName;
	m_WndClassEx.hIconSm = NULL;
	RegisterClassEx(&m_WndClassEx); // Day la back-end ( khoi tao va cau hinh cua so )
}

Window::WindowClass::~WindowClass() {
	UnregisterClass(m_WndClassName, m_hInstance);
}

Window::Window(int width, int height, const wchar_t* wndname) {
	m_hWnd = CreateWindowEx(
		0, 
		WindowClass::GetName(),
		wndname,
		WS_CAPTION | WS_SYSMENU | WS_POPUP | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,   // day la front-end ( hien thi UI )
		200,
		200,
		width,
		height,
		NULL,
		NULL,
		nullptr,
		nullptr
	);
	ShowWindow(m_hWnd, SW_SHOW); // SW_SHOW la cach ma cua so se duoc hien thi
}

Window::~Window() {
	DestroyWindow(m_hWnd);
}

LRESULT Window::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) { // Thu tuc giong nhu nguoi dung tuong tac voi cua so => tao ra thong diep
	// Va ham nay de xu li cac thong diep do, vi du hanh dong tat cua so se goi ham PostQuitMessages
	// Thoat chuong trinh va tra ve code la 69

	switch (uMsg) {
		case WM_CLOSE:// Xu li su kien dong cua so
		{
			PostQuitMessage(69);
			break;
		}

		case WM_KEYDOWN: // Xu li su kien nhan phim tu nguoi dung, hanh vi cua cua so sau khi nhan phim
		{
			break;
		}

		case WM_CHAR:
		{
			static std::wstring title;

			if (wParam == VK_BACK) {
				if (GetWindowTextLength(hWnd) == 0) break;
				title.pop_back();
			}
			else {
				title.push_back((wchar_t)wParam);
			}

			SetWindowText(hWnd, title.c_str());
			break;
		}
		case WM_LBUTTONDOWN:
		{
			const POINTS pt = MAKEPOINTS(lParam);
			std::wstringstream ss;
			ss << "(" << pt.x << "," << pt.y << ")";
			SetWindowText(hWnd, ss.str().c_str());

			break;
		}
		default:
		{
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
			break;
		}
	}
}