#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING

#include "../include/Window.h"
#include <iostream>
#include <string>
#include <sstream>

Window::WindowClass Window::WindowClass::WndClass; // dinh nghia WndClass

const wchar_t* Window::WindowClass::GetName() {
	return m_WndClassName;
}

HINSTANCE Window::WindowClass::GetInstance()
{
	return WndClass.m_hInstance;
}

Window::WindowClass::WindowClass() : m_hInstance(GetModuleHandle(nullptr)) {
	m_WndClassEx = { 0 };
	m_WndClassEx.cbSize = sizeof(m_WndClassEx); // Class Byte Size
	m_WndClassEx.style = CS_OWNDC;
	m_WndClassEx.lpfnWndProc = Window::WndProcSetup;
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
		WindowClass::GetInstance(),
		this
	);

	if (m_hWnd == nullptr) {
		OWN_WND_LASTERROR_EXCEPT();
	}

	m_Graphics = new Graphics(m_hWnd);
	ShowWindow(m_hWnd, SW_SHOW); // SW_SHOW la cach ma cua so se duoc hien thi
}

Graphics& Window::GetGraphicsHandle() const
{
	return *m_Graphics;
}

std::optional<int> Window::ProcessMessages()
{
	static MSG msg;
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
		if (msg.message == WM_QUIT) {
			return GetLastError();
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return {};
}

Window::~Window() {
	delete m_Graphics;
	DestroyWindow(m_hWnd);
}

LRESULT Window::WndProcSetup(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_NCCREATE) {
		CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
		Window* pWnd = reinterpret_cast<Window*>(pCreate->lpCreateParams);
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::WndProcAdapter));
		return pWnd->WndProc(hWnd, uMsg, wParam, lParam);
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

LRESULT Window::WndProcAdapter(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	Window* pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	if (pWnd) {
		return pWnd->WndProc(hWnd, uMsg, wParam, lParam);
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

LRESULT Window::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) { // Thu tuc giong nhu nguoi dung tuong tac voi cua so => tao ra thong diep
	// Va ham nay de xu li cac thong diep do, vi du hanh dong tat cua so se goi ham PostQuitMessages
	// Thoat chuong trinh va tra ve code la 69
	static std::wstring title;

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

			if (wParam == 'r') {
				m_Graphics->ClearBufferWithColors(1.0f, 0.0f, 0.0f, 1.0f);
			}

			if (wParam == 'g') {
				m_Graphics->ClearBufferWithColors(0.0f, 1.0f, 0.0f, 1.0f);
			}

			if (wParam == 'b') {
				m_Graphics->ClearBufferWithColors(1.0f, 0.0f, 1.0f, 1.0f);
			}

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
			title = L" ";

			break;
		}
		default:
		{
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
			break;
		}
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

Window::Exception::Exception(int line, const char* file, HRESULT hr) :
	AntiException(line, file), m_Hr(hr)
{}

const char* Window::Exception::what() const
{
	std::stringstream ss;
	ss << "[ERROR CODE] " << GetErrorCode() << std::endl
		<< "[DESCRIPTION] " << GetErrorString() << std::endl
		<< GetDescription();

	m_WhatBuffer = ss.str();
	return m_WhatBuffer.c_str();
}

const char* Window::Exception::GetType() const
{
	return "You Got An Exception: ";
}

std::string Window::Exception::TranslateErrorCode(HRESULT hr)
{
	char* pMsgBuf = nullptr;
	DWORD MsgLen = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPSTR>(&pMsgBuf), 0, nullptr
	);
	if (MsgLen == 0) {
		return "Invalid Error Code!";
	}
	std::string ErrorString = pMsgBuf;
	LocalFree(pMsgBuf);
	return ErrorString;
}

HRESULT Window::Exception::GetErrorCode() const
{
	return m_Hr;
}

std::string Window::Exception::GetErrorString() const
{
	return TranslateErrorCode(m_Hr);
}
