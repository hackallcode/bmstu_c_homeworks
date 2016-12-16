// LW-8-12.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "LW-8-7.h"

#define MAX_LOADSTRING 100

struct Round {
	int left, top, right, bottom;
};

struct RoundList {
	Round r;
	RoundList * next;
};

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
RoundList * rounds = NULL;
int left, top, right, bottom;
BOOL isPainting = FALSE;
HPEN bPen;			
HBRUSH hBrush;						
HPEN bkPen;								
HBRUSH bkBrush;							

												// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
BOOL				addRound(int left, int top, int right, int bottom);

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
	LoadStringW(hInstance, IDC_LW87, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LW87));

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
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LW87));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_LW87);
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
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

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
	switch (message)
	{
	case WM_CREATE:
		bPen = CreatePen(PS_DASHDOT, 1, RGB(255, 0, 0));
		hBrush = CreateHatchBrush(HS_CROSS, RGB(0, 0, 255));
		bkPen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
		bkBrush = CreateSolidBrush(RGB(255, 255, 255));
		break;
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_LBUTTONDOWN:
		left = right = LOWORD(lParam);
		top = bottom = HIWORD(lParam);
		isPainting = TRUE;
		break;
	case WM_MOUSEMOVE:
		if (wParam & MK_LBUTTON)
		{
			HDC hdc = GetDC(hWnd);

			SelectObject(hdc, bkPen);
			SelectObject(hdc, bkBrush);
			Ellipse(hdc, left, top, right, bottom);

			right = LOWORD(lParam);
			if (right - left > 50) right = left + 50;
			else if (left - right > 50) right = left - 50;
			bottom = HIWORD(lParam);
			if (bottom - top > 50) bottom = top + 50;
			else if (top - bottom > 50) bottom = top - 50;
			InvalidateRect(hWnd, NULL, FALSE);

			ReleaseDC(hWnd, hdc);
		}
		break;
	case WM_LBUTTONUP:
		addRound(left, top, right, bottom);
		isPainting = FALSE;
		break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		SelectObject(hdc, bPen);
		SelectObject(hdc, hBrush);
		RoundList * current = rounds;
		while (current != NULL)
		{
			Ellipse(hdc, current->r.left, current->r.top, current->r.right, current->r.bottom);
			current = current->next;
		}
		if (isPainting) Ellipse(hdc, left, top, right, bottom);
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		DeleteObject(bPen);
		DeleteObject(hBrush);
		DeleteObject(bkPen);
		DeleteObject(bkBrush);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

BOOL addRound(int left, int top, int right, int bottom) {
	if (rounds == NULL) {
		rounds = (RoundList *)malloc(sizeof(RoundList));
		rounds->r.left = left;
		rounds->r.top = top;
		rounds->r.right = right;
		rounds->r.bottom = bottom;
		rounds->next = NULL;
		return TRUE;
	}
	else {
		RoundList * last = rounds;
		while (last->next != NULL) last = last->next;
		last->next = (RoundList *)malloc(sizeof(RoundList));
		last->next->r.left = left;
		last->next->r.top = top;
		last->next->r.right = right;
		last->next->r.bottom = bottom;
		last->next->next = NULL;
		return TRUE;
	}
}