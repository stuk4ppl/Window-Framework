#pragma once
#include <Windows.h>
#include <string>
#include <optional>

#include "../../D3DGraphic/include/Graphics.h"
#include "../../Misc/include/AntiException.h"

class Window
{
public:
	class Exception : public AntiException {
	public:
		Exception(int line, const char* file, HRESULT hr);
		const char* what() const override;
	public:
		virtual const char* GetType() const;
		static std::string TranslateErrorCode(HRESULT hr);
		HRESULT GetErrorCode() const;
		std::string GetErrorString() const;
	private:
		HRESULT m_Hr;
	};
private:
	class WindowClass {
	public: // interface of windowclass
		static const wchar_t* GetName();

		static HINSTANCE GetInstance();
	private:
		WindowClass();

		~WindowClass();

	private:
		static WindowClass WndClass;

	private:
		inline static const wchar_t* m_WndClassName = L"WndClass";

		WNDCLASSEX m_WndClassEx;

		HINSTANCE m_hInstance;
	};

private: // Window Proceduce
	static LRESULT CALLBACK WndProcSetup(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	static LRESULT CALLBACK WndProcAdapter(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	inline Window(const Window&) = delete;

	inline Window& operator=(const Window&) = delete;

private: // Member Variables
	HWND m_hWnd;

	Graphics* m_Graphics;

public:// Main interface of Window class
	Window(int, int, const wchar_t*);

	Graphics& GetGraphicsHandle() const;

	static std::optional<int> ProcessMessages();

	~Window();
};

#define OWN_WND_EXCEPT(hr) do { throw Window::Exception(__LINE__, __FILE__, hr); } while(0)

#define OWN_WND_FAILED_EXCEPT(hr) do { if (FAILED(hr)) throw Window::Exception(__LINE__, __FILE__, hr); } while(0)

#define OWN_WND_SUCCEEDED_EXCEPT(hr) do { if (SUCCEEDED(hr)) throw Window::Exception(__LINE__, __FILE__, hr); } while(0)

#define OWN_WND_LASTERROR_EXCEPT() do { (throw Window::Exception(__LINE__, __FILE__, GetLastError())); } while(0)


