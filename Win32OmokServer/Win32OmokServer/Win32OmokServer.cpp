// Win32OmokServer.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Win32OmokServer.h"

#define MAX_LOADSTRING 100

enum class PlayerType
{
	GameSpector = 0, GamePlayer_Black, GamePlayer_White
};

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
RECT RectClientCnt = { 300, 0, 400, 50 };
StoneType Board[19][19];

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.
	memset(Board, 0, sizeof(Board));
	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_WIN32OMOKSERVER, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIN32OMOKSERVER));

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
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WIN32OMOKSERVER));
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
		CW_USEDEFAULT, 0, 416, 439, nullptr, nullptr, hInstance, nullptr);

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
	//static vector<SOCKET> clientList;
	static map<SOCKET, PlayerType> clientList;
	static BYTE playerCnt = 0;
	static bool playerFull = false;
	static vector<string> logs;
	static SOCKADDR_IN addr = { 0 }, c_addr;
	static char msg[1024];
	static MyPacket myPacket;
	switch (message)
	{
	case WM_CREATE:
		WSAStartup(MAKEWORD(2, 2), &wsaData);
		server = socket(AF_INET, SOCK_STREAM, NULL);
		addr.sin_family = AF_INET;
		addr.sin_port = 20;
		addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
		if (bind(server, (LPSOCKADDR)& addr, sizeof(addr)) == -1)
		{
			MessageBox(NULL, _T("bind failed"), _T("Error"), MB_OK);
			return 0;
		}
		WSAAsyncSelect(server, hWnd, WM_ASYNC, FD_ACCEPT);
		if (listen(server, 5) == SOCKET_ERROR)
		{
			MessageBox(NULL, _T("listen failed"), _T("Error"), MB_OK);
			return 0;
		}
		break;
	case WM_ASYNC:
		switch (lParam)
		{
		case FD_ACCEPT:
		{
			int size = sizeof(c_addr);
			SOCKET newClient = accept(server, (LPSOCKADDR)&c_addr, &size);

			string log = to_string(newClient) + " user Connected";
			logs.push_back(log);

			myPacket.clear();
			sprintf(msg, "Your ID is %d", newClient);
			myPacket.setPacketNumber(PACKET_CHAT);
			myPacket.setMsg(msg);
			send(newClient, (char*)myPacket.getPacketData(), sizeof(MyPacketData), NULL);

			WSAAsyncSelect(newClient, hWnd, WM_ASYNC, FD_READ | FD_CLOSE);

			PlayerType newClientType; 
			myPacket.clear();
			myPacket.setPacketNumber(PACKET_CHAT);
			if (playerCnt == 0)
			{
				newClientType = PlayerType::GamePlayer_Black;
				sprintf(msg, "You Are Player And Your Stone is Black");
				myPacket.setMsg(msg);
				send(newClient, (char*)myPacket.getPacketData(), sizeof(MyPacketData), NULL);
			}
			else if (playerCnt == 1)
			{
				newClientType = PlayerType::GamePlayer_White;
				sprintf(msg, "You Are Player And Your Stone is White");
				myPacket.setMsg(msg);
				send(newClient, (char*)myPacket.getPacketData(), sizeof(MyPacketData), NULL);
			}
			else
				newClientType = PlayerType::GameSpector;
			clientList.insert(make_pair(newClient, newClientType));
			InvalidateRect(hWnd, NULL, TRUE);
		}
		break;
		case FD_READ:
		{
			recv(wParam, (char*)(myPacket.getPacketData()), sizeof(MyPacketData), 0);
			string strMsg(myPacket.getMsg());
			string log = to_string(wParam) + " send PaketNum : " + to_string(myPacket.getPacketNumber()) + " msg : "+ strMsg;
			logs.push_back(log);
			if (myPacket.getPacketNumber() == PACKET_POINT)
			{
				PlayerType sendPlayer = clientList.find(wParam)->second;
				if (sendPlayer == PlayerType::GamePlayer_Black || sendPlayer == PlayerType::GamePlayer_White)
				{
					for (auto it = clientList.begin(); it != clientList.end(); it++)
					{
						send((*it).first, (char*)myPacket.getPacketData(), sizeof(MyPacketData), NULL);
					}
					POINTS pt = myPacket.getPOINTS();
					myPacket.clear(); 
					char stoneType = (char)StoneType::Empty;
					if (sendPlayer == PlayerType::GamePlayer_Black)
					{
						Board[pt.y][pt.x] = StoneType::Black;
						stoneType = (char)StoneType::Black;
					}
					else
					{
						Board[pt.y][pt.x] = StoneType::White;
						stoneType = (char)StoneType::White;
					}
					myPacket.setMsg(&stoneType);
					myPacket.setPacketNumber(PACKET_STONE);
					
					for (auto it = clientList.begin(); it != clientList.end(); it++)
					{
						send((*it).first, (char*)myPacket.getPacketData(), sizeof(MyPacketData), NULL);
					}
				}
				else
				{

				}
			}
			InvalidateRect(hWnd, NULL, TRUE);
		}
		break;
		case FD_CLOSE:
		{
			for (auto it = clientList.begin(); it != clientList.end(); it++)
			{
				if ((*it).first == wParam)
				{
					string log = to_string(wParam) + " user Exit";
					logs.push_back(log);
					sprintf(msg, "%d %s", wParam, "user exit");
					closesocket((*it).first);
					clientList.erase(it);
					break;
				}
			}
			InvalidateRect(hWnd, NULL, TRUE);
		}
		break;
		}
		break;
	case WM_PAINT:
	{
		static char clientCntStr[124];
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code that uses hdc here...
		size_t logSize = logs.size();
		for (size_t i = 0; i < logSize; ++i)
		{
			TextOutA(hdc, 20, 370 - i * 20, logs[logSize - i - 1].c_str(), logs[logSize - i - 1].size());
		}
		sprintf(clientCntStr, "client count : %d", clientList.size());
		DrawTextA(hdc, clientCntStr, strlen(clientCntStr), &RectClientCnt, DT_SINGLELINE | DT_VCENTER);
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
