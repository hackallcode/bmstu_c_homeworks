// LW-8-8.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "LW-8-8.h"

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
Round r;
BOOL isPainting = FALSE;
HPEN bPen;
HBRUSH hBrush;

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
    LoadStringW(hInstance, IDC_LW88, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LW88));

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

    return (int) msg.wParam;
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

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LW88));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LW88);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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
		bPen = CreatePen(PS_DASHDOTDOT, 1, RGB(255, 0, 255));
		hBrush = CreateHatchBrush(HS_DIAGCROSS, RGB(255, 255, 0));
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
	case WM_RBUTTONDOWN:
		r.left = r.right = LOWORD(lParam);
		r.top = r.bottom = HIWORD(lParam);
		isPainting = TRUE;
		break;
	case WM_MOUSEMOVE:
		if (wParam & MK_RBUTTON) {
			HDC hdc = GetDC(hWnd);

			SelectObject(hdc, GetStockObject(WHITE_PEN));
			SelectObject(hdc, GetStockObject(WHITE_BRUSH));
			Ellipse(hdc, r.left, r.top, r.right, r.bottom);

			r.right = LOWORD(lParam);
			if (r.right - r.left > 50) r.right = r.left + 50;
			else if (r.left - r.right > 50) r.right = r.left - 50;
			r.bottom = HIWORD(lParam);
			if (r.bottom - r.top > 50) r.bottom = r.top + 50;
			else if (r.top - r.bottom > 50) r.bottom = r.top - 50;
			InvalidateRect(hWnd, NULL, FALSE);

			ReleaseDC(hWnd, hdc);
		}
		break;
	case WM_RBUTTONUP:
		addRound(r.left, r.top, r.right, r.bottom);
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
			if (isPainting) Ellipse(hdc, r.left, r.top, r.right, r.bottom);
			EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
		DeleteObject(bPen);
		DeleteObject(hBrush);
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
	RoundList * last = NULL;
	if (rounds == NULL) {
		rounds = (RoundList *)malloc(sizeof(RoundList));
		if (rounds == NULL) return FALSE;
		last = rounds;
	}
	else {
		last = rounds;
		while (last->next != NULL) last = last->next;
		last->next = (RoundList *)malloc(sizeof(RoundList));
		if (last->next == NULL) return FALSE;
		last = last->next;
	}
	last->r.left = left;
	last->r.top = top;
	last->r.right = right;
	last->r.bottom = bottom;
	last->next = NULL;
	return TRUE;
}