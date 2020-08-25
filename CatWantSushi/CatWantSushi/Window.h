#pragma once
#include <Windows.h>

#define WM_SCENE_CHANGE WM_USER + 1
#define DEBUG

enum TimerID
{
	TimerRender = 0,
};

class Window
{
private :
protected:
	HWND m_window_hwnd_;
	HWND m_console_hwnd_;
	bool m_is_run_;
	UINT m_uint_window_width_;
	UINT m_uint_window_height_;
#pragma region GetterAndSetter
public:
	UINT m_uint_window_width() const
	{
		return m_uint_window_width_;
	}

	void set_m_uint_window_width(UINT m_uint_window_width)
	{
		m_uint_window_width_ = m_uint_window_width;
	}

	__declspec(property(get = m_uint_window_width, put = set_m_uint_window_width)) UINT MUintWindowWidth;

	UINT m_uint_window_height() const
	{
		return m_uint_window_height_;
	}

	void set_m_uint_window_height(UINT m_uint_window_height)
	{
		m_uint_window_height_ = m_uint_window_height;
	}

	__declspec(property(get = m_uint_window_height, put = set_m_uint_window_height)) UINT MUintWindowHeight;
#pragma endregion

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

