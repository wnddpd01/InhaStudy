#pragma once
#include <Windows.h>

#define WM_SCENE_CHANGE WM_USER + 1
#define DEBUG

enum TimerID
{
	TimerRender = 0
};

class Window
{
private :
protected:
	HWND m_window_hwnd_;
	HWND m_console_hwnd_;
	bool m_is_run_;
protected:
	virtual void init_wndclassex(WNDCLASSEX &wc);
public:
	Window();

	bool init();
	bool broadcast();
	bool release();
	bool is_run();

	virtual void on_create();
	virtual void on_update();
	virtual void on_destroy();
	
	virtual ~Window();
};

