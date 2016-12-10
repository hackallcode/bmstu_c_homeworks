// Clicks History.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Clicks History.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
WCHAR S_NOT_SAVED[MAX_LOADSTRING];
WCHAR S_SAVE_QSTN[MAX_LOADSTRING];
WCHAR S_NOT_OPENED[MAX_LOADSTRING];
WCHAR S_NOT_CLEARED[MAX_LOADSTRING];
WCHAR S_NOT_FOUND[MAX_LOADSTRING];
WCHAR S_NOT_ADDED[MAX_LOADSTRING];
WCHAR S_ARE_YOU_SURE[MAX_LOADSTRING];
WCHAR S_ERROR[MAX_LOADSTRING];
WCHAR S_EXT[MAX_LOADSTRING];
Tree * mouseEvents = NULL;
int lastEventID = -1;
int textSize = 20;
int top = 10;
int left = 10;
COLORREF textColor = RGB(200, 200, 200);
COLORREF coordColor = RGB(86, 156, 214);
COLORREF bkColor = RGB(64, 64, 64);
int PosV = 0; // Текущие позиции ползунков
int RangeV = 0; // Максимальные значения диапазонов
bool capture = false; // Перехватывать клики или нет
bool saved = true; // Сохранен файл или нет
bool newFile = true; // Сохранять в новый файл или нет
OPENFILENAME ofn;
TCHAR fileName[256] = L""; // Для имени файла
HMENU hMn;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
int					notSavedErr(HWND hWnd);
int					saveQstnDlg(HWND hWnd);
int					notOpenedErr(HWND hWnd);
int					clrErr(HWND hWnd);
int					notFoundErr(HWND hWnd);
int					notAddedErr(HWND hWnd);
bool				saveMouseEvents(HWND hWnd);
bool				saveMouseEventsDlg(HWND hWnd);
bool				openMouseEvents(HWND hWnd);
int					printMouseEvents(HWND hWnd, HDC hdc);

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
    LoadStringW(hInstance, IDC_CLICKSHISTORY, szWindowClass, MAX_LOADSTRING);
	LoadStringW(hInstance, IDS_NOT_SAVED, S_NOT_SAVED, MAX_LOADSTRING);
	LoadStringW(hInstance, IDS_SAVE_QSTN, S_SAVE_QSTN, MAX_LOADSTRING);
	LoadStringW(hInstance, IDS_NOT_OPENED, S_NOT_OPENED, MAX_LOADSTRING);
	LoadStringW(hInstance, IDS_NOT_CLEARED, S_NOT_CLEARED, MAX_LOADSTRING);
	LoadStringW(hInstance, IDS_NOT_FOUND, S_NOT_FOUND, MAX_LOADSTRING);
	LoadStringW(hInstance, IDS_NOT_ADDED, S_NOT_ADDED, MAX_LOADSTRING);
	LoadStringW(hInstance, IDS_ARE_YOU_SURE, S_ARE_YOU_SURE, MAX_LOADSTRING);
	LoadStringW(hInstance, IDS_ERROR, S_ERROR, MAX_LOADSTRING);
	LoadStringW(hInstance, IDS_EXT, S_EXT, MAX_LOADSTRING);

    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CLICKSHISTORY));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CLICKSHISTORY));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)GetStockObject(DKGRAY_BRUSH);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_CLICKSHISTORY);
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

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW | WS_VSCROLL,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
   hMn = GetMenu(hWnd);

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
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
			case ID_FILE_NEW:
				if (!saveMouseEventsDlg(hWnd)) return 0;
				if (removeTree(&mouseEvents) != SUCCESS) {
					clrErr(hWnd);
					return 0;
				};
				lastEventID = -1;
				saved = true;
				newFile = true;
				capture = false;
				EnableMenuItem(hMn, ID_CAPTURE_START, MF_ENABLED);
				EnableMenuItem(hMn, ID_CAPTURE_STOP, MF_GRAYED);
				EnableMenuItem(hMn, ID_CAPTURE_RESTART, MF_GRAYED);
				InvalidateRect(hWnd, NULL, TRUE);
				break;
			case ID_FILE_SAVE:
				if (saveMouseEvents(hWnd)) saved = true;
				break;
			case ID_FILE_OPEN:
				if (!saveMouseEventsDlg(hWnd)) return 0;
				if (!openMouseEvents(hWnd)) return 0;
				saved = true;
				newFile = false;
				break;
            case IDM_EXIT:
				if (!saveMouseEventsDlg(hWnd)) return 0;
                DestroyWindow(hWnd);
                break;
			case ID_CAPTURE_START:
				capture = true;
				EnableMenuItem(hMn, ID_CAPTURE_START, MF_GRAYED);
				EnableMenuItem(hMn, ID_CAPTURE_STOP, MF_ENABLED);
				EnableMenuItem(hMn, ID_CAPTURE_RESTART, MF_ENABLED);
				break;
			case ID_CAPTURE_STOP:
				capture = false;
				EnableMenuItem(hMn, ID_CAPTURE_START, MF_ENABLED);
				EnableMenuItem(hMn, ID_CAPTURE_STOP, MF_GRAYED);
				if (lastEventID < 0)
					EnableMenuItem(hMn, ID_CAPTURE_RESTART, MF_GRAYED);
				else
					EnableMenuItem(hMn, ID_CAPTURE_RESTART, MF_ENABLED);
				break;
			case ID_CAPTURE_RESTART:
				capture = false;
				if (!saveMouseEventsDlg(hWnd)) return 0;
				if (removeTree(&mouseEvents) != SUCCESS) {
					clrErr(hWnd);
					return 0;
				}
				lastEventID = -1;
				saved = true;
				newFile = true;
				capture = true;
				EnableMenuItem(hMn, ID_CAPTURE_START, MF_GRAYED);
				EnableMenuItem(hMn, ID_CAPTURE_STOP, MF_ENABLED);
				EnableMenuItem(hMn, ID_CAPTURE_RESTART, MF_ENABLED);
				InvalidateRect(hWnd, NULL, TRUE);
				break;
			case IDM_ABOUT:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
				break;
			default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
			printMouseEvents(hWnd, hdc);
            EndPaint(hWnd, &ps);
        }
        break;
	case WM_VSCROLL:// Сообщение от верт ПП
		switch (LOWORD(wParam))
		{
		case SB_LINEDOWN: // Стрелка вниз
			if (PosV < RangeV) PosV += textSize + top;
			if (PosV > RangeV) PosV = RangeV;
			break;
		case SB_LINEUP: // Стрелка вверх
			if (PosV > 0) PosV -= textSize + top;
			if (PosV < 0) PosV = 0;
			break;
		case SB_THUMBTRACK: case SB_THUMBPOSITION:
			//  Перемещение ползунка с полощью мыши
			PosV = HIWORD(wParam); // Определяем текущую позицию ползунка

		}
		SetScrollPos(hWnd, SB_VERT, PosV, TRUE); // Устанавливаем новое положение
		InvalidateRect(hWnd, NULL, TRUE); // Перерисовать окно
		return 0;
	case WM_LBUTTONDOWN: case WM_LBUTTONUP: case WM_LBUTTONDBLCLK:
	case WM_RBUTTONDOWN: case WM_RBUTTONUP: case WM_RBUTTONDBLCLK:
	case WM_MBUTTONDOWN: case WM_MBUTTONUP: case WM_MBUTTONDBLCLK:
		if (!capture) break;
		int data[3];
		switch (message)
		{
		case WM_LBUTTONDOWN:
			data[0] = 1;
			break;
		case WM_LBUTTONUP:
			data[0] = 2;
			break;
		case WM_LBUTTONDBLCLK:
			data[0] = 3;
			break;
		case WM_RBUTTONDOWN:
			data[0] = 4;
			break;
		case WM_RBUTTONUP:
			data[0] = 5;
			break;
		case WM_RBUTTONDBLCLK:
			data[0] = 6;
			break;
		case WM_MBUTTONDOWN:
			data[0] = 7;
			break;
		case WM_MBUTTONUP:
			data[0] = 8;
			break;
		case WM_MBUTTONDBLCLK:
			data[0] = 9;
			break;
		default:
			data[0] = 0;
			break;
		}
		data[1] = LOWORD(lParam); // X
		data[2] = HIWORD(lParam); // Y
		lastEventID++;
		if (addChildTree(&mouseEvents, lastEventID, data) != SUCCESS) {
			notAddedErr(hWnd);
			return 0;
		}
		saved = false;
		InvalidateRect(hWnd, NULL, TRUE);
		break;
    case WM_DESTROY:
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

////////////////////////
//   User Functions   //
////////////////////////

// If file cannot be saved
int notSavedErr(HWND hWnd) {
	return MessageBox(hWnd, S_NOT_SAVED, S_ERROR, MB_OK | MB_ICONERROR);
}

// Save file or no
int saveQstnDlg(HWND hWnd) {
	return MessageBox(hWnd, S_SAVE_QSTN, S_ARE_YOU_SURE, MB_YESNOCANCEL | MB_ICONQUESTION);
}

// If file cannot be opened
int notOpenedErr(HWND hWnd) {
	return MessageBox(hWnd, S_NOT_OPENED, S_ERROR, MB_OK | MB_ICONERROR);
}

// If cannor clear mouse events
int clrErr(HWND hWnd) {
	return MessageBox(hWnd, S_NOT_CLEARED, S_ERROR, MB_OK | MB_ICONERROR);
}

// If mouse event not found 
int notFoundErr(HWND hWnd) {
	return MessageBox(hWnd, S_NOT_FOUND, S_ERROR, MB_OK | MB_ICONERROR);
}

// If cannot add mouse event
int notAddedErr(HWND hWnd) {
	return MessageBox(hWnd, S_NOT_ADDED, S_ERROR, MB_OK | MB_ICONERROR);
}

// Save File Function
bool saveMouseEvents(HWND hWnd) {
	if (newFile) {
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = hWnd; // родительское окно 
		ofn.lpstrFilter = S_EXT;
		ofn.lpstrFile = fileName;  // Сюда будет записано полное имя файла
		ofn.nMaxFile = 255;
		ofn.lpstrFileTitle = NULL;//
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = NULL;
		ofn.Flags = OFN_SHOWHELP | OFN_OVERWRITEPROMPT;
		ofn.lpstrTitle = NULL;
		ofn.lpstrDefExt = L"mef"; // Строка расширение по умолчанию
		if (GetSaveFileName(&ofn)) {
			if (saveTree(mouseEvents, fileName) != SUCCESS) {
				notSavedErr(hWnd);
				return false;
			}
		}
		else {
			return false;
		}
	}
	else {
		if (saveTree(mouseEvents, fileName) != SUCCESS) {
			notSavedErr(hWnd);
			return false;
		}
	}
	return true;
}

// Save Question Function
bool saveMouseEventsDlg(HWND hWnd) {
	if (!saved) {
		int MBResult = saveQstnDlg(hWnd);
		switch (MBResult)
		{
		case IDYES:
			if (!saveMouseEvents(hWnd)) {
				return false;
			}
			break;
		case IDNO:
			break;
		default:
			return false;
			break;
		}
	}
	return true;
}

// Open File Function
bool openMouseEvents(HWND hWnd) {
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFile = fileName;
	// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
	// use the contents of szFile to initialize itself.
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = 255;
	ofn.lpstrFilter = S_EXT;
	ofn.nFilterIndex = 0;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	ofn.lpstrDefExt = L"mef"; // Строка расширение по умолчанию

	if (GetOpenFileName(&ofn)) {
		if (removeTree(&mouseEvents) != SUCCESS) {
			clrErr(hWnd);
			return false;
		}
		if (openTree(&mouseEvents, fileName) != SUCCESS) {
			notOpenedErr(hWnd);
			return false;
		}
		lastEventID = countChildTrees(mouseEvents) - 1;
		InvalidateRect(hWnd, NULL, TRUE);
	}
	else {
		return false;
	}
	return true;
}

// Print mouse events on screen
int printMouseEvents(HWND hWnd, HDC hdc) {
	RECT Rect;
	int n = countChildTrees(mouseEvents);
	long y = (textSize + top) * n + top;

	// Определяем размер рабочей области окна
	GetClientRect(hWnd, &Rect);
	// Устанавливаем диапазоны ПП
	RangeV = y - Rect.bottom;
	if (RangeV > 0)
		SetScrollRange(hWnd, SB_VERT, 0, RangeV, TRUE);
	else
		SetScrollRange(hWnd, SB_VERT, 0, 0, TRUE); // ПП не нужна

												   // Установить параметры текста
	SetBkColor(hdc, bkColor);
	HFONT hFont = CreateFont(textSize, 0, 0, 0, 0, 0, 0, 0,
		DEFAULT_CHARSET,
		0, 0, 0, 0,
		L"Arial"
	);
	SelectObject(hdc, hFont);

	y = 0;
	for (int i = 0; i < n; i++) {
		char buf[100];
		char type[32];
		Tree * temp = NULL;
		if (findChildTree(mouseEvents, i, &temp) != SUCCESS) {
			notFoundErr(hWnd);
			return 0;
		}
		switch (temp->data[0])
		{
		case 1: strcpy_s(type, 32, "Left Button Down");
			break;
		case 2: strcpy_s(type, 32, "Left Button Up");
			break;
		case 3: strcpy_s(type, 32, "Left Button Double Click");
			break;
		case 4: strcpy_s(type, 32, "Right Button Down");
			break;
		case 5: strcpy_s(type, 32, "Right Button Up");
			break;
		case 6: strcpy_s(type, 32, "Right Button Double Click");
			break;
		case 7: strcpy_s(type, 32, "Medium Button Down");
			break;
		case 8: strcpy_s(type, 32, "Medium Button Up");
			break;
		case 9: strcpy_s(type, 32, "Medium Button Double Click");
			break;
		default:
			strcpy_s(type, 32, "Uknown Event");
			break;
		}
		SetTextColor(hdc, textColor);
		sprintf_s(buf, 99, "Coordinates: X =         , Y =         . Event: %s", type);
		TextOutA(hdc, left, y + top - PosV, buf, strlen(buf));
		SetTextColor(hdc, coordColor);
		sprintf_s(buf, 99, "%4d", temp->data[1]);
		TextOutA(hdc, left + 128, y + top - PosV, buf, strlen(buf));
		sprintf_s(buf, 99, "%4d", temp->data[2]);
		TextOutA(hdc, left + 209, y + top - PosV, buf, strlen(buf));

		y += top + textSize;
	}

}