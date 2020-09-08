#include "Window.h"

#include <iostream>

#include "SceneManager.h"
#include "StartScene.h"
#include "GameOptionManager.h"
#include "GlobalValue.h"
Window* window = NULL;

auto gameOptionManager = GameOptionManager::GetInstance();
SceneManager& scene_manager = *(SceneManager::getInstance());
UINT game_loop_start = 0;
void InputHandle()
{
	for (pair<shortCut, UCHAR> short_cut : gameOptionManager->shortCutKeyList)
	{
		if (GetKeyState(short_cut.second) & 0x8000)
		{
			scene_manager.keyboard_input_handle(short_cut.second);
		}
	}
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static UINT loop_start = 0;
	PAINTSTRUCT ps;
	switch (msg)
	{
	case WM_CREATE:
	{
		window->on_create();
		SetTimer(hwnd, TimerGameLoop, 16, NULL);
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
		BeginPaint(hwnd, &ps);
		scene_manager.render(&ps);
		EndPaint(hwnd, &ps);
		cout << "time : " << GetTickCount() - loop_start << '\n';
		loop_start = GetTickCount();
		break;
	}
	case WM_TIMER:
	{
		switch (wParam)
		{
		case TimerGameLoop:
		{
			loop_start = GetTickCount();
			InputHandle();
			scene_manager.update();
			InvalidateRect(hwnd, NULL, FALSE);
			UpdateWindow(hwnd);
			break;
		}
		default:
			break;
		}
		break;
	}
	case WM_SCENE_CHANGE:
	{
		scene_manager.scene_change(wParam);
		break;
	}
	case WM_LBUTTONUP:
	case WM_MOUSEMOVE:
	{
		POINT pt = { LOWORD(lParam), HIWORD(lParam) };
		scene_manager.mouse_event_handle(msg, pt);
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

	m_window_hwnd_ = CreateWindowEx(WS_EX_APPWINDOW, L"CWSWindowClass", L"CatWantSushi", WS_POPUP, GetSystemMetrics(SM_CXSCREEN) / 2 - gameOptionManager->GameWidth / 2, GetSystemMetrics(SM_CYSCREEN) / 2 - gameOptionManager->GameHeight / 2, gameOptionManager->GameWidth, gameOptionManager->GameHeight, NULL, NULL, NULL, NULL);

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
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	window->on_update();
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

}


void Window::on_destroy()
{
	m_is_run_ = false;
}

Window::~Window()
{
}
