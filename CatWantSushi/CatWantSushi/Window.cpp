#include "Window.h"

#include <iostream>
#include <thread>


#include "SceneManager.h"
#include "StartScene.h"
#include "GameOptionManager.h"
#include "GlobalValue.h"

Window* window = NULL;

extern GameOptionManager * game_option_manager;
extern SceneManager* scene_manager;

void InputHandle()
{
	for (pair<shortCut, UCHAR> short_cut : game_option_manager->shortCutKeyList)
	{
		if (GetKeyState(short_cut.second) & 0x8000)
		{
			scene_manager->keyboard_input_handle(short_cut.second);
		}
	}
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	switch (msg)
	{
	case WM_CREATE:
	{
		window->on_create();
		//SetTimer(hwnd, TimerGameLoop, 1000/gameOptionManager->Frame, NULL);
		break;
	}
	case WM_DESTROY:
	{
		window->on_destroy();
		PostQuitMessage(0);
		break;
	}
	case WM_PAINT:
	{
		/*if (window->DrawCalled == TRUE)
		{
			BeginPaint(hwnd, &ps);
			scene_manager->render(&ps);
			EndPaint(hwnd, &ps);
			window->DrawCalled = FALSE;
		}*/
		break;
	}
	
	case WM_SCENE_CHANGE:
	{
		scene_manager->scene_change(wParam);
		break;
	}
	case WM_LBUTTONUP:
	case WM_MOUSEMOVE:
	{
		POINT pt = { LOWORD(lParam), HIWORD(lParam) };
		scene_manager->mouse_event_handle(msg, pt);
		break;
	}
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return NULL;
}

void Window::init_wndclassex(WNDCLASSEX& wc)
{
	wc.cbClsExtra = NULL;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.cbWndExtra = NULL;
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = NULL;
	wc.lpszClassName = L"CWSWindowClass";
	wc.lpszMenuName = L"";
	wc.style = NULL;
	wc.lpfnWndProc = &WndProc;
}

Window::Window()
{
}

bool Window::init()
{
	WNDCLASSEX wc;
	init_wndclassex(wc);
	if (!RegisterClassEx(&wc))
		return false;

	if (!window)
		window = this;

	m_window_hwnd_ = CreateWindowEx(WS_EX_APPWINDOW, L"CWSWindowClass", L"CatWantSushi", WS_POPUP, GetSystemMetrics(SM_CXSCREEN) / 2 - game_option_manager->GameWidth / 2, GetSystemMetrics(SM_CYSCREEN) / 2 - game_option_manager->GameHeight / 2, game_option_manager->GameWidth, game_option_manager->GameHeight, NULL, NULL, NULL, NULL);

	if (!m_window_hwnd_)
		return false;


#ifdef DEBUG__
	m_console_hwnd_ = GetConsoleWindow();
	ShowWindow(m_console_hwnd_, SW_SHOW);
#endif
	ShowWindow(m_window_hwnd_, SW_SHOW);
	UpdateWindow(m_window_hwnd_);

	m_is_run_ = true;
	return true;
}

bool Window::broadcast()
{
	MSG msg;
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0)
	{
		//TranslateMessage(&msg);
		DispatchMessage(&msg);
		std::this_thread::yield();
	}
	return true;
}

bool Window::release()
{
	if (!DestroyWindow(m_window_hwnd_))
		return false;
	return true;
}

bool Window::is_run()
{
	return m_is_run_;
}

void Window::on_create()
{

}

void Window::on_update()
{
	//static UINT lastUpdateTime = GetTickCount();
	//if (GetTickCount() - lastUpdateTime < 16)
	//	return;
	//lastUpdateTime = GetTickCount();
	DWORD dwElapsedTicks = 0;
	DWORD dwLastTicks = 0;
	DWORD dwInterval = 1000 / game_option_manager->Frame;
	UINT frameCnt = 0;
	dwLastTicks = GetTickCount();
	PAINTSTRUCT ps;
	ps.hdc = GetDC(m_window_hwnd_);
	ps.fErase = FALSE;
	GetClientRect(m_window_hwnd_, &ps.rcPaint);
	while (m_is_run_)
	{
		InputHandle();
		scene_manager->update();
		scene_manager->render(&ps);
		/*InvalidateRect(m_window_hwnd_, NULL, FALSE);
		UpdateWindow(m_window_hwnd_);
		draw_called = TRUE;
		while (draw_called == TRUE)
			Sleep(1);*/
		dwElapsedTicks += (GetTickCount() - dwLastTicks);
		if (dwElapsedTicks < dwInterval)
		{
			//Sleep(dwInterval - dwElapsedTicks);
			std::this_thread::sleep_for(std::chrono::milliseconds(dwInterval - dwElapsedTicks));
			dwElapsedTicks = 0;
		}
		else 
		{
			dwElapsedTicks -= dwInterval;
		}
		dwLastTicks = GetTickCount();
	}
	ReleaseDC(m_window_hwnd_, ps.hdc);
}


void Window::on_destroy()
{
	m_is_run_ = false;
}

Window::~Window()
{
}
