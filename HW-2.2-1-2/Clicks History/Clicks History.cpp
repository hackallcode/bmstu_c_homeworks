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
WCHAR S_EXT_MEH[MAX_LOADSTRING];
WCHAR S_LV_NOT_CREATED[MAX_LOADSTRING];
WCHAR S_LEFT_CLICK[MAX_LOADSTRING];
WCHAR S_LEFT_DBLCLICK[MAX_LOADSTRING];
WCHAR S_RIGHT_CLICK[MAX_LOADSTRING];
WCHAR S_RIGHT_DBLCLICK[MAX_LOADSTRING];
WCHAR S_ALL_FILES[MAX_LOADSTRING];
Tree * mouseEvents = NULL;
int lastEventID = 0;
COLORREF textColor = RGB(200, 200, 200);
COLORREF coordColor = RGB(86, 156, 214);
COLORREF bkColor = RGB(64, 64, 64);
bool capture = false; // Перехватывать клики или нет
bool saved = true; // Сохранен файл или нет
bool newFile = true; // Сохранять в новый файл или нет
OPENFILENAME ofn;
TCHAR fileName[256] = L""; // Для имени файла
HWND hListView = NULL;
TCHAR * header[4] = { L"#", L"Type", L"X", L"Y" };

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Find(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
int					notCreatedLV(HWND hWnd);
int					notSavedErr(HWND hWnd);
int					saveQstnDlg(HWND hWnd);
int					notOpenedErr(HWND hWnd);
int					clrErr(HWND hWnd);
int					notFoundErr(HWND hWnd);
int					notAddedErr(HWND hWnd);
HWND				CreateListView(HWND hWndParent, UINT uId);
BOOL WINAPI			AddListViewItems(HWND hWndLV, TCHAR id[21], TCHAR * type, TCHAR coords[2][21]);
BOOL				clearAllItems(HWND hWnd);
bool				saveMouseEvents(HWND hWnd);
bool				saveMouseEventsDlg(HWND hWnd);
bool				openMouseEvents(HWND hWnd);

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
	LoadStringW(hInstance, IDS_EXT_MEH, S_EXT_MEH, MAX_LOADSTRING);
	LoadStringW(hInstance, IDS_LV_NOT_CREATED, S_LV_NOT_CREATED, MAX_LOADSTRING);
	LoadStringW(hInstance, IDS_LEFT_CLICK, S_LEFT_CLICK, MAX_LOADSTRING);
	LoadStringW(hInstance, IDS_LEFT_DBLCLICK, S_LEFT_DBLCLICK, MAX_LOADSTRING);
	LoadStringW(hInstance, IDS_RIGHT_CLICK, S_RIGHT_CLICK, MAX_LOADSTRING);
	LoadStringW(hInstance, IDS_RIGHT_DBLCLICK, S_RIGHT_DBLCLICK, MAX_LOADSTRING);
	LoadStringW(hInstance, IDS_ALL_FILES, S_ALL_FILES, MAX_LOADSTRING);

	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
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
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CLICKSHISTORY));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(DKGRAY_BRUSH);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_CLICKSHISTORY);
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

	// ListView create
	if ((hListView = CreateListView(hWnd, IDC_MOUSE_EVENTS_LV)) == NULL)
		notCreatedLV(hWnd);
	else
		ShowWindow(hListView, SW_SHOWDEFAULT);

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
	case WM_NOTIFY:
		LPNMHDR lpnmHdr;
		lpnmHdr = (LPNMHDR)lParam;
		if (lpnmHdr->idFrom == IDC_MOUSE_EVENTS_LV) {
			if (!capture) break;
			int data[3];
			TCHAR id[21];
			TCHAR * type;
			TCHAR coords[2][21];
			switch (lpnmHdr->code)
			{
			case NM_CLICK:
				data[0] = 1;
				type = S_LEFT_CLICK;
				break;
			case NM_DBLCLK:
				data[0] = 3;
				type = S_LEFT_DBLCLICK;
				break;
			case NM_RCLICK:
				data[0] = 4;
				type = S_RIGHT_CLICK;
				break;
			case NM_RDBLCLK:
				data[0] = 6;
				type = S_RIGHT_DBLCLICK;
				break;
			default:
				return 0;
				break;
			}
			POINT pt;
			GetCursorPos(&pt);
			ScreenToClient(hWnd, &pt);

			data[1] = pt.x; // X
			data[2] = pt.y; // Y
			lastEventID++;
			if (addChildTree(&mouseEvents, lastEventID, data) != SUCCESS) {
				notAddedErr(hWnd);
				return 0;
			}
			_itow_s(lastEventID, id, 21, 10);
			_itow_s(data[1], coords[0], 21, 10);
			_itow_s(data[2], coords[1], 21, 10);
			AddListViewItems(hListView, id, type, coords);
			saved = false;
		}
		break;
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case ID_FILE_NEW:
		{
			if (!saveMouseEventsDlg(hWnd)) return 0;
			clearAllItems(hWnd);
			saved = true;
			newFile = true;
			capture = false;
			HMENU hMn = GetMenu(hWnd);
			EnableMenuItem(hMn, ID_CAPTURE_START, MF_ENABLED);
			EnableMenuItem(hMn, ID_CAPTURE_STOP, MF_GRAYED);
			EnableMenuItem(hMn, ID_CAPTURE_RESTART, MF_GRAYED);
		}
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
		case ID_EDIT_FIND:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_FIND
			), hWnd, Find);
			break;
		case ID_EDIT_DELETEALL:
			clearAllItems(hWnd);
			break;
		case ID_CAPTURE_START:
		{
			capture = true;
			HMENU hMn = GetMenu(hWnd);
			EnableMenuItem(hMn, ID_CAPTURE_START, MF_GRAYED);
			EnableMenuItem(hMn, ID_CAPTURE_STOP, MF_ENABLED);
			EnableMenuItem(hMn, ID_CAPTURE_RESTART, MF_ENABLED);
		}
		break;
		case ID_CAPTURE_STOP:
		{
			capture = false;
			HMENU hMn = GetMenu(hWnd);
			EnableMenuItem(hMn, ID_CAPTURE_START, MF_ENABLED);
			EnableMenuItem(hMn, ID_CAPTURE_STOP, MF_GRAYED);
			if (lastEventID < 0)
				EnableMenuItem(hMn, ID_CAPTURE_RESTART, MF_GRAYED);
			else
				EnableMenuItem(hMn, ID_CAPTURE_RESTART, MF_ENABLED);
		}
		break;
		case ID_CAPTURE_RESTART:
		{
			capture = false;
			if (!saveMouseEventsDlg(hWnd)) return 0;
			clearAllItems(hWnd);
			saved = true;
			newFile = true;
			capture = true;
			HMENU hMn = GetMenu(hWnd);
			EnableMenuItem(hMn, ID_CAPTURE_START, MF_GRAYED);
			EnableMenuItem(hMn, ID_CAPTURE_STOP, MF_ENABLED);
			EnableMenuItem(hMn, ID_CAPTURE_RESTART, MF_ENABLED);
		}
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
		RECT rcl;
		GetClientRect(hWnd, &rcl);
		SetWindowPos(hListView, NULL, 0, 0, rcl.right - rcl.left, rcl.bottom - rcl.top, SWP_SHOWWINDOW);
		ShowWindow(hListView, SW_SHOWDEFAULT);
		EndPaint(hWnd, &ps);
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK Find(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
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

// If ListView cannot be created
int notCreatedLV(HWND hWnd) {
	return MessageBox(hWnd, S_LV_NOT_CREATED, S_ERROR, MB_OK | MB_ICONERROR);
}

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

// Function to create ListView
HWND CreateListView(HWND hWndParent, UINT uId) {
	INITCOMMONCONTROLSEX icex;
	icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icex.dwICC = ICC_LISTVIEW_CLASSES;
	InitCommonControlsEx(&icex);

	RECT rcl;
	GetClientRect(hWndParent, &rcl);

	HWND hWndLV = CreateWindow(WC_LISTVIEW, L"",
		WS_CHILD | LVS_REPORT,
		0, 0, rcl.right - rcl.left, rcl.bottom - rcl.top,
		hWndParent, (HMENU)uId, GetModuleHandle(NULL), NULL);

	// Чтобы определялись строка (item) и столбец (subitem) обязательно устанавливаем
	// расширенный стиль LVS_EX_FULLROWSELECT.
	ListView_SetExtendedListViewStyleEx(hWndLV, 0, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	LVCOLUMN lvc;
	lvc.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_FMT;
	lvc.cchTextMax = 32;

	lvc.pszText = header[0];
	lvc.cx = 20;
	lvc.fmt = LVCFMT_CENTER;
	ListView_InsertColumn(hWndLV, 0, &lvc);

	lvc.pszText = header[1];
	lvc.cx = (rcl.right - rcl.left) / 2 - 40;
	lvc.fmt = LVCFMT_LEFT;
	ListView_InsertColumn(hWndLV, 1, &lvc);

	lvc.pszText = header[2];
	lvc.cx = (rcl.right - rcl.left) / 4;
	lvc.fmt = LVCFMT_CENTER;
	ListView_InsertColumn(hWndLV, 2, &lvc);

	lvc.pszText = header[3];
	lvc.cx = (rcl.right - rcl.left) / 4;
	lvc.fmt = LVCFMT_CENTER;
	ListView_InsertColumn(hWndLV, 3, &lvc);

	return (hWndLV);
}

// Add items in ListView
BOOL WINAPI AddListViewItems(HWND hWndLV, TCHAR id[21], TCHAR * type, TCHAR coords[2][21]) {
	int iLastIndex = ListView_GetItemCount(hWndLV);

	LVITEM lvi;
	lvi.mask = LVIF_TEXT;
	lvi.cchTextMax = 32;
	lvi.iItem = iLastIndex;
	lvi.pszText = type;
	lvi.iSubItem = 0;

	if (ListView_InsertItem(hWndLV, &lvi) == -1)
		return FALSE;
	ListView_SetItemText(hWndLV, iLastIndex, 0, id);
	ListView_SetItemText(hWndLV, iLastIndex, 1, type);
	for (int i = 0; i < 2; i++)
		ListView_SetItemText(hWndLV, iLastIndex, i + 2, coords[i]);

	return TRUE;
}

// Clear All
BOOL clearAllItems(HWND hWnd) {
	if (removeTree(&mouseEvents) != SUCCESS) {
		clrErr(hWnd);
		return 0;
	}
	lastEventID = 0;
	ListView_DeleteAllItems(hListView);
}

// Save File Function
bool saveMouseEvents(HWND hWnd) {
	if (newFile) {
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = hWnd; // родительское окно 
		ofn.lpstrFilter = L"Mouse Events History (*.meh)\0*.meh\0All Files\0*.*";
		ofn.lpstrFile = fileName;  // Сюда будет записано полное имя файла
		ofn.nMaxFile = 255;
		ofn.lpstrFileTitle = NULL;//
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = NULL;
		ofn.Flags = OFN_SHOWHELP | OFN_OVERWRITEPROMPT;
		ofn.lpstrTitle = NULL;
		ofn.lpstrDefExt = L"meh"; // Строка расширение по умолчанию
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
	ofn.lpstrFilter = L"Mouse Events History (*.meh)\0*.meh\0All Files\0*.*";
	ofn.nFilterIndex = 0;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	ofn.lpstrDefExt = L"meh"; // Строка расширение по умолчанию

	if (GetOpenFileName(&ofn)) {
		clearAllItems(hWnd);

		if (openTree(&mouseEvents, fileName) != SUCCESS) {
			notOpenedErr(hWnd);
			return false;
		}
		lastEventID = countChildTrees(mouseEvents) - 1;

		for (int i = 0; i < lastEventID + 1; i++) {
			Tree * mouseEvent = NULL;
			if (findChildTree(mouseEvents, i, &mouseEvent) != SUCCESS) {
				notFoundErr(hWnd);
			}
			TCHAR id[21];
			TCHAR * type;
			TCHAR coords[2][21];
			switch (mouseEvent->data[0])
			{
			case 1:
				type = S_LEFT_CLICK;
				break;
			case 3:
				type = S_LEFT_DBLCLICK;
				break;
			case 4:
				type = S_RIGHT_CLICK;
				break;
			case 6:
				type = S_RIGHT_DBLCLICK;
				break;
			default:
				type = L"Unknown type";
				break;
			}
			_itow_s(mouseEvent->key, id, 21, 10);
			_itow_s(mouseEvent->data[1], coords[0], 21, 10);
			_itow_s(mouseEvent->data[2], coords[1], 21, 10);
			AddListViewItems(hListView, id, type, coords);
		}
	}
	else {
		return false;
	}
	return true;
}