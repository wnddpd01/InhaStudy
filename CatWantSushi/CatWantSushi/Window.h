#pragma once
#include <Windows.h>

#define DEBUG__

enum TimerID
{
	TimerGameLoop = 0
};

class Window
{
private :
	BOOL draw_called;
public:
	BOOL get_draw_called() const
	{
		return draw_called;
	}

	void set_draw_called(BOOL draw_called)
	{
		this->draw_called = draw_called;
	}

	__declspec(property(get = get_draw_called, put = set_draw_called)) BOOL DrawCalled;
protected:
	virtual void init_wndclassex(WNDCLASSEX &wc);
public:
	HWND m_console_hwnd_;
	HWND m_window_hwnd_;
	bool m_is_run_;
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

