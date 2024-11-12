#pragma once
#include <vector>
#include <Windows.h>

#include "../../Framework/include/Window.h"
class App
{
private: // Member Variable
	std::vector<Window*> m_Wnd;

private: // All stuff for singleton
	inline static App* AppInstance = nullptr;
	App();
	~App();

private: // Private Method
	void OnFrame();

private: // delete some shit
	App(const App& other) = delete;
	App& operator=(const App& other) = delete;

public: // Public Method
	static App& GetAppInstance();
	void Init();
	int Run();
};

