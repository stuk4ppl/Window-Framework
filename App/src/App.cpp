#include "../include/App.h"
#define OWN_NO_ERROR 0
#define OWN_GOT_ERROR 1

App::App()
{} 

App::~App()
{
	for (int i = 0; i < m_Wnd.size(); i++) {
		delete m_Wnd[i];
	}

	delete AppInstance;
}

App& App::GetAppInstance()
{
	if (AppInstance == nullptr) {
		AppInstance = new App();
	}
	return *AppInstance;
}

int App::Run()
{
	while (true) {
		if (const auto ExitCode = Window::ProcessMessages()) {
			return *ExitCode;
		}
		OnFrame();
	}
}

void App::OnFrame()
{
	for (int i = 0; i < m_Wnd.size(); i++) {
		m_Wnd.at(i)->GetGraphicsHandle().OnRender();
	}
}

void App::Init()
{
	m_Wnd.push_back(new Window(640, 580, L"Miu cho"));
}
