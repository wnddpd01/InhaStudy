// Win32OmokClient.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Win32OmokClient.h"

#define MAX_LOADSTRING 100
#define BOARD_WIDTH 768
#define BOARD_HEIGHT 768
#define BOARD_WHITE_SPACE 42
#define BOARD_ONE_SPACE_LENGTH 38
#define CHATROOM_WIDTH 200

struct Stone
{
	POINTS point;
	StoneType stoneType;
};
// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
RECT crossPoint[19][19];
BitmapManager * bitMapManger;
// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

void SetCrossPoint();
void DrawBoard(HDC hdc);
void DrawChatRoom(HDC hdc);
void DrawStone(HDC hdc, const Stone& stone);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	// TODO: Place code here.

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_WIN32OMOKCLIENT, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIN32OMOKCLIENT));

	MSG msg;

	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WIN32OMOKCLIENT));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, BOARD_WIDTH + 10 + CHATROOM_WIDTH + 16, BOARD_HEIGHT + 39, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	using namespace std;
	static WSADATA wsaData;
	static SOCKET server;
	static SOCKADDR_IN addr = { 0 };
	static vector<char *> chatLog;
	static vector<Stone> stones;
	static HBRUSH charRectBrush = CreateSolidBrush(RGB(40, 170, 220));
	static MyPacket myPacket;
	switch (message)
	{
	case WM_CREATE:
	{
		bitMapManger = BitmapManager::getInstance();
		SetCrossPoint();
		WSAStartup(MAKEWORD(2, 2), &wsaData);
		server = socket(AF_INET, SOCK_STREAM, 0);
		if (server == INVALID_SOCKET)
		{
			MessageBox(NULL, _T("socket faield"), _T("Error"), MB_OK);
			return 0;
		}

		addr.sin_family = AF_INET;
		addr.sin_port = 20;
		addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
		if (connect(server, (LPSOCKADDR)&addr, sizeof(addr)) == SOCKET_ERROR)
		{
			MessageBox(NULL, _T("connect failed"), _T("Error"), MB_OK);
			return 0;
		}
		WSAAsyncSelect(server, hWnd, WM_ASYNC, FD_READ);
	}
	break;
	case WM_ASYNC:
		switch (lParam)
		{
		case FD_READ:
		{
			recv(server, (char*)myPacket.getPacketData(), sizeof(MyPacketData), NULL);
			if (myPacket.getPacketNumber() == PACKET_CHAT)
			{
				char *newChat = new char[strlen(myPacket.getMsg())];
				strcpy(newChat, myPacket.getMsg());
				chatLog.push_back(newChat);
			}
			else if (myPacket.getPacketNumber() == PACKET_POINT)
			{
				POINTS pts = myPacket.getPOINTS();
				recv(server, (char*)myPacket.getPacketData(), sizeof(MyPacketData), NULL);
				if (myPacket.getPacketNumber() != PACKET_STONE)
				{
					return 0;
				}
				StoneType stoneType = myPacket.getStoneType();
				Stone stone;
				stone.point = pts;
				stone.stoneType = stoneType;
				stones.push_back(stone);
				/*PAINTSTRUCT ps;
				HDC hdc = BeginPaint(hWnd, &ps);
				DrawStone(hdc, stone);
				EndPaint(hWnd, &ps);*/
			}
			InvalidateRect(hWnd, NULL, FALSE);
		}
		break;
		}
	break;
	case WM_ACTIVATE :
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code that uses hdc here...
		DrawBoard(hdc);
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code that uses hdc here...
		if (stones.size() > 0)
			DrawStone(hdc, stones.back());
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_LBUTTONUP:
	{
		POINT pt;
		pt.x = LOWORD(lParam);
		pt.y = HIWORD(lParam);
		myPacket.clear();
		for (size_t i = 0; i < 19; i++)
		{
			for (size_t j = 0; j < 19; j++)
			{
				if (PtInRect(&crossPoint[i][j], pt))
				{
					myPacket.setPacketNumber(PACKET_POINT);
					POINTS xy = { j, i };
					myPacket.setMsg(&xy);
					send(server, (char *)myPacket.getPacketData(), sizeof(MyPacketData), NULL);
					i = 19;
					break;
				}
			}
		}
	}
	break;
	case WM_DESTROY:
		closesocket(server);
		WSACleanup();
		PostQuitMessage(0);
	break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void SetCrossPoint()
{
	for (size_t i = 0; i < 19; i++)
	{
		for (size_t j = 0; j < 19; j++)
		{
			crossPoint[i][j] = { LONG(BOARD_WHITE_SPACE + BOARD_ONE_SPACE_LENGTH * j - 10)
								,LONG(BOARD_WHITE_SPACE + BOARD_ONE_SPACE_LENGTH * i - 10)
								,LONG(BOARD_WHITE_SPACE + BOARD_ONE_SPACE_LENGTH * j + 10)
								,LONG(BOARD_WHITE_SPACE + BOARD_ONE_SPACE_LENGTH * i + 10) };
		}
	}
}

void DrawBoard(HDC hdc)
{
	static HBRUSH boardBrush = CreateSolidBrush(RGB(250, 203, 125));
	static HBRUSH flowerPtBrush = CreateSolidBrush(RGB(0, 0, 0));
	HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, boardBrush);
	Rectangle(hdc, 0, 0, BOARD_WIDTH, BOARD_HEIGHT);
	SelectObject(hdc, flowerPtBrush);
	for (size_t i = 0; i < 19; i++)
	{
		MoveToEx(hdc, BOARD_WHITE_SPACE, BOARD_WHITE_SPACE + BOARD_ONE_SPACE_LENGTH * i, NULL);
		LineTo(hdc, 684 + BOARD_WHITE_SPACE, BOARD_WHITE_SPACE + BOARD_ONE_SPACE_LENGTH * i);
		MoveToEx(hdc, BOARD_WHITE_SPACE + BOARD_ONE_SPACE_LENGTH * i, BOARD_WHITE_SPACE, NULL);
		LineTo(hdc, BOARD_WHITE_SPACE + BOARD_ONE_SPACE_LENGTH * i, 684 + BOARD_WHITE_SPACE);
	}
	for (size_t i = 0; i < 3; i++)
	{
		Circle(hdc, BOARD_WHITE_SPACE + (BOARD_ONE_SPACE_LENGTH * 3 ), BOARD_WHITE_SPACE + (BOARD_ONE_SPACE_LENGTH * 3) + (BOARD_ONE_SPACE_LENGTH * 6) * i, 5);
		Circle(hdc, BOARD_WHITE_SPACE + (BOARD_ONE_SPACE_LENGTH * 9 ), BOARD_WHITE_SPACE + (BOARD_ONE_SPACE_LENGTH * 3) + (BOARD_ONE_SPACE_LENGTH * 6) * i, 5);
		Circle(hdc, BOARD_WHITE_SPACE + (BOARD_ONE_SPACE_LENGTH * 15), BOARD_WHITE_SPACE + (BOARD_ONE_SPACE_LENGTH * 3) + (BOARD_ONE_SPACE_LENGTH * 6) * i, 5);
	}
	SelectObject(hdc, boardBrush);

#ifdef DEBUG
	for (size_t i = 0; i < 19; i++)
	{
		for (size_t j = 0; j < 19; j++)
		{
			Rectangle(hdc, crossPoint[i][j].left, crossPoint[i][j].top, crossPoint[i][j].right, crossPoint[i][j].bottom);
		}
	}
#endif // DEBUG

}

void DrawStone(HDC hdc, const Stone & stone)
{
	static HBRUSH blackBrush = CreateSolidBrush(RGB(0, 0, 0));
	static HBRUSH whiteBrush = CreateSolidBrush(RGB(255, 255, 255));
	HBRUSH oldBrush;
	if (stone.stoneType == StoneType::Black)
		oldBrush = (HBRUSH)SelectObject(hdc, blackBrush);
	else if (stone.stoneType == StoneType::White)
		oldBrush = (HBRUSH)SelectObject(hdc, whiteBrush);

	Circle(hdc, stone.point.x * BOARD_ONE_SPACE_LENGTH + BOARD_WHITE_SPACE,
		stone.point.y * BOARD_ONE_SPACE_LENGTH + BOARD_WHITE_SPACE,
		10);

	SelectObject(hdc, oldBrush);
}

