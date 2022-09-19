// Clicks History.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Clicks History.h"

#define MAX_LOADSTRING 100
#define COLUMNS_NUMBER 4

// Global Variables:
HINSTANCE hInst;													// Current instance
HWND hWnd;															// Main Window Handle
HMENU hMn;															// Menu Handle
HWND hListView = NULL;												// ListView Handle
TREE * mouseEvents = NULL;											// Main tree with data
TREE * ME_types = NULL;												// Tree for types search
TREE * ME_X = NULL;													// Tree for X search
TREE * ME_Y = NULL;													// Tree for Y search
INT lastItemID = 0;													// ID of last item (0 - Default)
enum filterParam { ID, TYPE, X, Y };									// Param for filter
TCHAR fileName[256] = L"";											// Name of opened file

BOOL isCapture = FALSE;												// Capture in the moment or no
BOOL isSaved = TRUE;												// File saved or no
BOOL isNewFile = TRUE;												// Save in new file or no

TCHAR szTitle[MAX_LOADSTRING];									    // The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];					            // The main window class name
TCHAR S_EXT[MAX_LOADSTRING];										// Const string IDS_EXT
TCHAR * S_DEFAULT_EXT = L"meh";										// Default extension
TCHAR S_ARE_YOU_SURE[MAX_LOADSTRING];								// Const string IDS_ARE_YOU_SURE
TCHAR S_SAVE_QSTN[MAX_LOADSTRING];									// Const string IDS_SAVE_QSTN
TCHAR S_ERROR[MAX_LOADSTRING];										// Const string IDS_ERROR
TCHAR S_NOT_SAVED[MAX_LOADSTRING];									// Const string IDS_NOT_SAVED
TCHAR S_NOT_OPENED[MAX_LOADSTRING];									// Const string IDS_NOT_OPENED
TCHAR S_NOT_CLEARED[MAX_LOADSTRING];								// Const string IDS_NOT_CLEARED
TCHAR S_NOT_FOUND[MAX_LOADSTRING];									// Const string IDS_NOT_FOUND
TCHAR S_NOT_ADDED[MAX_LOADSTRING];									// Const string IDS_NOT_ADDED
TCHAR S_LV_NOT_CREATED[MAX_LOADSTRING];								// Const string IDS_LV_NOT_CREATED
TCHAR S_NO_MEMORY[MAX_LOADSTRING];									// Const string IDS_NO_MEMORY
TCHAR S_TYPE[MAX_LOADSTRING];										// Const string IDS_TYPE
TCHAR S_LEFT_DOWN[MAX_LOADSTRING];									// Const string IDS_LEFT_DOWN
TCHAR S_LEFT_UP[MAX_LOADSTRING];									// Const string IDS_LEFT_UP
TCHAR S_LEFT_DBLCLICK[MAX_LOADSTRING];								// Const string IDS_LEFT_DBLCLICK
TCHAR S_RIGHT_DOWN[MAX_LOADSTRING];									// Const string IDS_RIGHT_DOWN
TCHAR S_RIGHT_UP[MAX_LOADSTRING];									// Const string IDS_RIGHT_UP
TCHAR S_RIGHT_DBLCLICK[MAX_LOADSTRING];								// Const string IDS_RIGHT_DBLCLICK
TCHAR S_MIDDLE_DOWN[MAX_LOADSTRING];								// Const string IDS_MIDDLE_DOWN
TCHAR S_MIDDLE_UP[MAX_LOADSTRING];									// Const string IDS_MIDDLE_UP
TCHAR S_MIDDLE_DBLCLICK[MAX_LOADSTRING];							// Const string IDS_MIDDLE_DBLCLICK
TCHAR S_UNKNOWN_TYPE[MAX_LOADSTRING];								// Const string IDS_UNKNOWN_TYPE

COLORREF LV_textColor = RGB(200, 200, 200);							// Color of text
COLORREF LV_numbersColor = RGB(86, 156, 214);						// Colot of numbers
COLORREF LV_bkColor = RGB(30, 30, 30);								// Color of background
TCHAR * LV_headers[COLUMNS_NUMBER] = { L"#", S_TYPE, L"X", L"Y" };	// Columns names
double LV_widthCoef[COLUMNS_NUMBER] = { 0.02, 0.48, 0.25, 0.25 };	// Part of full width for each column
int LV_maxLengths[COLUMNS_NUMBER] = { 32, 64, 32, 32 };				// Max count of chars for each column
int LV_columnsFmt[COLUMNS_NUMBER] = {								// LVCOLUMN.Fmt for each columns
	LVCFMT_CENTER,
	LVCFMT_LEFT,
	LVCFMT_CENTER,
	LVCFMT_CENTER
};

// Forward declarations of functions included in this code module:
int					notCreatedLV();
int					noMemory();
int					notSavedErr();
int					saveQstnDlg();
int					notOpenedErr();
int					clrErr();
int					notFoundErr();
int					notAddedErr();
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
HWND				CreateListView(HWND hWndParent, UINT uId);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Find(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
BOOL				startCapture();
BOOL				stopCapture();
BOOL				restartCapture();
BOOL				addItem(int item[3]);
BOOL				addItemIndexes(DATA * data);
BOOL WINAPI			addListViewItem(ME_data * data);
BOOL				filterItems(filterParam param, int value);
BOOL				removeAllItems();
BOOL				showItems(DATA * data);
BOOL				saveMouseEventsDlg();
BOOL				saveMouseEvents(BOOL isNew);
BOOL				openMouseEvents();

/////////////////////////
//   Error Functions   //
/////////////////////////

// If ListView cannot be created
int notCreatedLV() {
	return MessageBox(hWnd, S_LV_NOT_CREATED, S_ERROR, MB_OK | MB_ICONERROR);
}

// If ListView cannot be created
int noMemory() {
	return MessageBox(hWnd, S_NO_MEMORY, S_ERROR, MB_OK | MB_ICONERROR);
}

// If file cannot be saved
int notSavedErr() {
	return MessageBox(hWnd, S_NOT_SAVED, S_ERROR, MB_OK | MB_ICONERROR);
}

// Save file or no
int saveQstnDlg() {
	return MessageBox(hWnd, S_SAVE_QSTN, S_ARE_YOU_SURE, MB_YESNOCANCEL | MB_ICONQUESTION);
}

// If file cannot be opened
int notOpenedErr() {
	return MessageBox(hWnd, S_NOT_OPENED, S_ERROR, MB_OK | MB_ICONERROR);
}

// If cannor clear mouse events
int clrErr() {
	return MessageBox(hWnd, S_NOT_CLEARED, S_ERROR, MB_OK | MB_ICONERROR);
}

// If mouse event not found 
int notFoundErr() {
	return MessageBox(hWnd, S_NOT_FOUND, S_ERROR, MB_OK | MB_ICONERROR);
}

// If cannot add mouse event
int notAddedErr() {
	return MessageBox(hWnd, S_NOT_ADDED, S_ERROR, MB_OK | MB_ICONERROR);
}

int tcharcmp(TCHAR * str1, TCHAR * str2) {
#ifdef _UNICODE
	return wcscmp(str1, str2);
#else
	return strcmp(str1, str2);
#endif
}

////////////////////////////////////
//   Windows Creation Functions   //
////////////////////////////////////

//
//   FUNCTION: wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
//
//   PURPOSE: Main function
//
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
	LoadStringW(hInstance, IDS_EXT, S_EXT, MAX_LOADSTRING);
	{ // For multistring support
		INT strLen = wcslen(S_EXT);
		for (INT i = 0; i < strLen; i++) if (S_EXT[i] == '~') S_EXT[i] = '\0';
	}
	LoadStringW(hInstance, IDS_ARE_YOU_SURE, S_ARE_YOU_SURE, MAX_LOADSTRING);
	LoadStringW(hInstance, IDS_SAVE_QSTN, S_SAVE_QSTN, MAX_LOADSTRING);
	LoadStringW(hInstance, IDS_ERROR, S_ERROR, MAX_LOADSTRING);
	LoadStringW(hInstance, IDS_NOT_SAVED, S_NOT_SAVED, MAX_LOADSTRING);
	LoadStringW(hInstance, IDS_NOT_OPENED, S_NOT_OPENED, MAX_LOADSTRING);
	LoadStringW(hInstance, IDS_NOT_CLEARED, S_NOT_CLEARED, MAX_LOADSTRING);
	LoadStringW(hInstance, IDS_NOT_FOUND, S_NOT_FOUND, MAX_LOADSTRING);
	LoadStringW(hInstance, IDS_NOT_ADDED, S_NOT_ADDED, MAX_LOADSTRING);
	LoadStringW(hInstance, IDS_LV_NOT_CREATED, S_LV_NOT_CREATED, MAX_LOADSTRING);
	LoadStringW(hInstance, IDS_NO_MEMORY, S_NO_MEMORY, MAX_LOADSTRING);
	LoadStringW(hInstance, IDS_TYPE, S_TYPE, MAX_LOADSTRING);
	LoadStringW(hInstance, IDS_LEFT_DOWN, S_LEFT_DOWN, MAX_LOADSTRING);
	LoadStringW(hInstance, IDS_LEFT_UP, S_LEFT_UP, MAX_LOADSTRING);
	LoadStringW(hInstance, IDS_LEFT_DBLCLICK, S_LEFT_DBLCLICK, MAX_LOADSTRING);
	LoadStringW(hInstance, IDS_RIGHT_DOWN, S_RIGHT_DOWN, MAX_LOADSTRING);
	LoadStringW(hInstance, IDS_RIGHT_UP, S_RIGHT_UP, MAX_LOADSTRING);
	LoadStringW(hInstance, IDS_RIGHT_DBLCLICK, S_RIGHT_DBLCLICK, MAX_LOADSTRING);
	LoadStringW(hInstance, IDS_MIDDLE_DOWN, S_MIDDLE_DOWN, MAX_LOADSTRING);
	LoadStringW(hInstance, IDS_MIDDLE_UP, S_MIDDLE_UP, MAX_LOADSTRING);
	LoadStringW(hInstance, IDS_MIDDLE_DBLCLICK, S_MIDDLE_DBLCLICK, MAX_LOADSTRING);
	LoadStringW(hInstance, IDS_UNKNOWN_TYPE, S_UNKNOWN_TYPE, MAX_LOADSTRING);

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
//   FUNCTION: MyRegisterClass()
//
//   PURPOSE: Registers the window class.
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

	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// Set menu holder
	hMn = GetMenu(hWnd);

	// ListView create
	if ((hListView = CreateListView(hWnd, IDC_LISTVIEW)) == NULL)
		notCreatedLV();
	else
		ShowWindow(hListView, SW_SHOWDEFAULT);

	return TRUE;
}

//
//   FUNCTION: CreateListView(HWND hWndParent, UINT uId)
//
//   PURPOSE: Creates ListView in selected window
//
HWND CreateListView(HWND hWndParent, UINT uId) {
	INITCOMMONCONTROLSEX icex;
	icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icex.dwICC = ICC_LISTVIEW_CLASSES;
	InitCommonControlsEx(&icex);

	RECT rcl;
	GetClientRect(hWndParent, &rcl);

	HWND hWndLV = CreateWindow(WC_LISTVIEW, L"LISTVIEW",
		WS_CHILD | LVS_REPORT,
		0, 0, rcl.right - rcl.left, rcl.bottom - rcl.top,
		hWndParent, (HMENU)uId, GetModuleHandle(NULL), NULL);

	ListView_SetExtendedListViewStyleEx(hWndLV, 0, LVS_EX_FULLROWSELECT);
	ListView_SetBkColor(hWndLV, LV_bkColor);
	ListView_SetTextBkColor(hWndLV, LV_bkColor);
	ListView_SetTextColor(hWndLV, LV_textColor);

	LVCOLUMN lvc;
	lvc.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_FMT;

	for (int i = 0; i < COLUMNS_NUMBER; i++) {
		lvc.cchTextMax = LV_maxLengths[i];
		lvc.pszText = LV_headers[i];
		lvc.cx = (rcl.right - rcl.left - 20) * LV_widthCoef[i];
		lvc.fmt = LV_columnsFmt[i];
		ListView_InsertColumn(hWndLV, i, &lvc);
	}

	return (hWndLV);
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
		if (lpnmHdr->idFrom == IDC_LISTVIEW) {
			if (isCapture == FALSE) break;
			int item[3];

			switch (lpnmHdr->code)
			{
			case NM_CLICK:
				item[0] = 1;
				break;
			case NM_DBLCLK:
				item[0] = 3;
				break;
			case NM_RCLICK:
				item[0] = 4;
				break;
			case NM_RDBLCLK:
				item[0] = 6;
				break;
			default:
				return 0;
				break;
			}
			POINT pt;
			GetCursorPos(&pt);
			ScreenToClient(hWnd, &pt);
			item[1] = pt.x; // X
			item[2] = pt.y; // Y
			addItem(item);

			isSaved = false;
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
			if (saveMouseEventsDlg()) {
				stopCapture();
				removeAllItems();
				isNewFile = TRUE;
				isSaved = TRUE;
			}
		}
		break;
		case ID_FILE_SAVE:
			saveMouseEvents(isNewFile);
			break;
		case ID_FILE_SAVEAS:
			saveMouseEvents(TRUE);
			break;
		case ID_FILE_OPEN:
			if (saveMouseEventsDlg()) {
				openMouseEvents();
			}
			break;
		case IDM_EXIT:
			if (saveMouseEventsDlg()) {
				DestroyWindow(hWnd);
			}
			break;
		case ID_EDIT_FIND:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_FIND), hWnd, Find);
			break;
		case ID_EDIT_SHOWALL:
			ListView_DeleteAllItems(hListView);
			for (int i = 1; i <= lastItemID; i++) {
				TREE * mouseEvent = NULL;
				if (findChildTree(mouseEvents, i, &mouseEvent) != SUCCESS) {
					notFoundErr();
				}
				showItems(&mouseEvent->data);
			}
			break;
		case ID_CAPTURE_START:
			startCapture();
			break;
		case ID_CAPTURE_STOP:
			stopCapture();
			break;
		case ID_CAPTURE_RESTART:
			restartCapture();
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

// Message handler for find box.
INT_PTR CALLBACK Find(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
	{
		HWND hCombo = GetDlgItem(hDlg, IDC_COMBO);
		SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)"#");
		SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)S_TYPE);
		SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)"X");
		SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)"Y");
		hCombo = GetDlgItem(hDlg, IDC_TYPESCOMBO);
		SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)S_LEFT_DOWN);
		//SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)S_LEFT_UP);
		SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)S_LEFT_DBLCLICK);
		SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)S_RIGHT_DOWN);
		//SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)S_RIGHT_UP);
		SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)S_RIGHT_DBLCLICK);
		//SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)S_MIDDLE_DOWN);
		//SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)S_MIDDLE_UP);
		//SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)S_MIDDLE_DBLCLICK);
	}
	return (INT_PTR)TRUE;
	case WM_COMMAND:
		if (LOWORD(wParam) == IDC_COMBO) {
			TCHAR buf[64];
			GetDlgItemText(hDlg, IDC_COMBO, buf, 64);
			if (tcharcmp(buf, S_TYPE) == 0) {
				ShowWindow(GetDlgItem(hDlg, IDC_EDIT), SW_HIDE);
				ShowWindow(GetDlgItem(hDlg, IDC_TYPESCOMBO), SW_SHOW);
			}
			else {
				ShowWindow(GetDlgItem(hDlg, IDC_TYPESCOMBO), SW_HIDE);
				ShowWindow(GetDlgItem(hDlg, IDC_EDIT), SW_SHOW);
			}
			break;
		}
		if (LOWORD(wParam) == IDCANCEL) {
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		if (LOWORD(wParam) == IDOK) {
			TCHAR buf[64];
			GetDlgItemText(hDlg, IDC_COMBO, buf, 64);
			filterParam type;
			if (tcharcmp(buf, L"#") == 0)
				type = ID;
			else if (tcharcmp(buf, S_TYPE) == 0)
				type = TYPE;
			else if (tcharcmp(buf, L"X") == 0)
				type = X;
			else if (tcharcmp(buf, L"Y") == 0)
				type = Y;
			int value;
			if (type == TYPE) {
				GetDlgItemText(hDlg, IDC_TYPESCOMBO, buf, 64);
				if (tcharcmp(buf, S_LEFT_DOWN) == 0)
					value = 1;
				else if (tcharcmp(buf, S_LEFT_UP) == 0)
					value = 2;
				else if(tcharcmp(buf, S_LEFT_DBLCLICK) == 0)
					value = 3;
				else if(tcharcmp(buf, S_RIGHT_DOWN) == 0)
					value = 4;
				else if(tcharcmp(buf, S_RIGHT_UP) == 0)
					value = 5;
				else if(tcharcmp(buf, S_RIGHT_DBLCLICK) == 0)
					value = 6;
				else if(tcharcmp(buf, S_MIDDLE_DOWN) == 0)
					value = 7;
				else if(tcharcmp(buf, S_MIDDLE_UP) == 0)
					value = 8;
				else if(tcharcmp(buf, S_MIDDLE_DBLCLICK) == 0)
					value = 9;
			}
			else {
				GetDlgItemText(hDlg, IDC_EDIT, buf, 64);
#ifdef _UNICODE
				value = _wtoi(buf);
#else
				value = _atoi64(buf);
#endif
			}
			filterItems(type, value);
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

//
BOOL startCapture() {
	isCapture = TRUE;
	EnableMenuItem(hMn, ID_CAPTURE_START, MF_GRAYED);
	EnableMenuItem(hMn, ID_CAPTURE_STOP, MF_ENABLED);
	EnableMenuItem(hMn, ID_CAPTURE_RESTART, MF_ENABLED);
	return TRUE;
}

//
BOOL stopCapture() {
	isCapture = FALSE;
	EnableMenuItem(hMn, ID_CAPTURE_START, MF_ENABLED);
	EnableMenuItem(hMn, ID_CAPTURE_STOP, MF_GRAYED);
	if (lastItemID < 1)
		EnableMenuItem(hMn, ID_CAPTURE_RESTART, MF_GRAYED);
	else
		EnableMenuItem(hMn, ID_CAPTURE_RESTART, MF_ENABLED);
	return TRUE;
}

//
BOOL restartCapture() {
	BOOL temp = isCapture;
	isCapture = FALSE;
	if (removeAllItems() == FALSE) {
		isCapture = temp;
		return FALSE;
	}

	isCapture = TRUE;
	EnableMenuItem(hMn, ID_CAPTURE_START, MF_GRAYED);
	EnableMenuItem(hMn, ID_CAPTURE_STOP, MF_ENABLED);
	EnableMenuItem(hMn, ID_CAPTURE_RESTART, MF_ENABLED);
}

//
BOOL addItem(int item[3]) {
	DATA data;
	data.dataNumber = 1;
	data.data = (ME_data *)malloc(sizeof(ME_data));
	if (data.data == NULL) {
		noMemory();
		return FALSE;
	}
	data.data[0].id = ++lastItemID;
	data.data[0].type = item[0];
	data.data[0].x = item[1];
	data.data[0].y = item[2];

	if (addChildTree(&mouseEvents, data.data[0].id, &data) != SUCCESS) {
		notAddedErr();
		return FALSE;
	}

	addItemIndexes(&data);

	if (addListViewItem(&data.data[0]) != TRUE) {
		notAddedErr();
		return FALSE;
	}

	isSaved = FALSE;

	return TRUE;
}

//
BOOL addItemIndexes(DATA * data) {
	if (addChildTree(&ME_types, data->data[0].type, data) != SUCCESS) {
		notAddedErr();
		return FALSE;
	}
	if (addChildTree(&ME_X, data->data[0].x, data) != SUCCESS) {
		notAddedErr();
		return FALSE;
	}
	if (addChildTree(&ME_Y, data->data[0].y, data) != SUCCESS) {
		notAddedErr();
		return FALSE;
	}
}

//
BOOL WINAPI addListViewItem(ME_data * data) {
	TCHAR id[32];
	TCHAR x[32];
	TCHAR y[32];
#ifdef _UNICODE
	_itow_s(data->id, id, 32, 10);
	_itow_s(data->x, x, 32, 10);
	_itow_s(data->y, y, 32, 10);
#else
	_itoa_s(data->id, id, 32, 10);
	_itoa_s(data->x, x, 32, 10);
	_itoa_s(data->y, y, 32, 10);
#endif

	TCHAR * type;
	switch (data->type)
	{
	case 1:
		type = S_LEFT_DOWN;
		break;
	case 2:
		type = S_LEFT_UP;
		break;
	case 3:
		type = S_LEFT_DBLCLICK;
		break;
	case 4:
		type = S_RIGHT_DOWN;
		break;
	case 5:
		type = S_RIGHT_UP;
		break;
	case 6:
		type = S_RIGHT_DBLCLICK;
		break;
	case 7:
		type = S_MIDDLE_DOWN;
		break;
	case 8:
		type = S_MIDDLE_UP;
		break;
	case 9:
		type = S_MIDDLE_DBLCLICK;
		break;
	default:
		type = S_UNKNOWN_TYPE;
		break;
	}

	int iLastIndex = ListView_GetItemCount(hListView);

	LVITEM lvi;
	lvi.mask = LVIF_TEXT;
	lvi.iItem = iLastIndex;
	lvi.pszText = type;
	lvi.iSubItem = 0;

	if (ListView_InsertItem(hListView, &lvi) == -1)
		return FALSE;
	lvi.cchTextMax = LV_maxLengths[0];
	ListView_SetItemText(hListView, iLastIndex, 0, id);
	lvi.cchTextMax = LV_maxLengths[1];
	ListView_SetItemText(hListView, iLastIndex, 1, type);
	lvi.cchTextMax = LV_maxLengths[2];
	ListView_SetItemText(hListView, iLastIndex, 2, x);
	lvi.cchTextMax = LV_maxLengths[3];
	ListView_SetItemText(hListView, iLastIndex, 3, y);

	return TRUE;
}

//
BOOL filterItems(filterParam param, int value) {
	ListView_DeleteAllItems(hListView);
	TREE * resultTree = NULL;
	TREE * searchTree = NULL;
	switch (param)
	{
	case ID:
		searchTree = mouseEvents;
		break;
	case TYPE:
		searchTree = ME_types;
		break;
	case X:
		searchTree = ME_X;
		break;
	case Y:
		searchTree = ME_Y;
		break;
	default:
		return FALSE;
	}
	if (findChildTree(searchTree, value, &resultTree) != SUCCESS) {
		notFoundErr();
		return FALSE;
	}
	for (int i = 0; i < resultTree->data.dataNumber; i++) {
		addListViewItem(&resultTree->data.data[i]);
	}
	return TRUE;
}

// Clear All
BOOL removeAllItems() {
	if (removeTree(&mouseEvents) != SUCCESS) {
		clrErr();
		return FALSE;
	}
	if (removeTree(&ME_types) != SUCCESS) {
		clrErr();
		return FALSE;
	}
	if (removeTree(&ME_X) != SUCCESS) {
		clrErr();
		return FALSE;
	}
	if (removeTree(&ME_Y) != SUCCESS) {
		clrErr();
		return FALSE;
	}
	ListView_DeleteAllItems(hListView);

	lastItemID = 0;
	isSaved = FALSE;

	return TRUE;
}

//
BOOL showItems(DATA * data) {
	for (int i = 0; i < data->dataNumber; i++) {
		addListViewItem(&data->data[i]);
	}
	return TRUE;
}

// Save Question Function
BOOL saveMouseEventsDlg() {
	if (isSaved == FALSE) {
		int MBResult = saveQstnDlg();
		switch (MBResult)
		{
		case IDYES:
			if (!saveMouseEvents(isNewFile)) {
				return FALSE;
			}
			return TRUE;
		case IDNO:
			return TRUE;
		default:
			return FALSE;
		}
	}
}

// Save File Function
BOOL saveMouseEvents(BOOL isNew) {
	if (isNew) {
		OPENFILENAME ofn;
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = hWnd; // родительское окно 
		ofn.lpstrFilter = S_EXT;
		ofn.lpstrFile = fileName;  // Сюда будет записано полное имя файла
		ofn.nMaxFile = 255;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = NULL;
		ofn.Flags = OFN_SHOWHELP | OFN_OVERWRITEPROMPT;
		ofn.lpstrTitle = NULL;
		ofn.lpstrDefExt = S_DEFAULT_EXT; // Строка расширение по умолчанию
		if (GetSaveFileName(&ofn)) {
			if (saveTree(mouseEvents, fileName) != SUCCESS) {
				notSavedErr();
				return FALSE;
			}
			isSaved = TRUE;
			return TRUE;
		}
		else {
			return FALSE;
		}
	}
	else {
		if (saveTree(mouseEvents, fileName) != SUCCESS) {
			notSavedErr();
			return FALSE;
		}
		isSaved = TRUE;
		return TRUE;
	}
}

// Open File Function
BOOL openMouseEvents() {
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFile = fileName;
	ofn.lpstrFile[0] = '\0'; // Not use name of last file
	ofn.nMaxFile = 255;
	ofn.lpstrFilter = S_EXT;
	ofn.nFilterIndex = 0;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	ofn.lpstrDefExt = S_DEFAULT_EXT; // Строка расширение по умолчанию

	if (GetOpenFileName(&ofn)) {
		removeAllItems();

		if (openTree(&mouseEvents, fileName) != SUCCESS) {
			notOpenedErr();
			return FALSE;
		}
		lastItemID = countChildTrees(mouseEvents);

		for (int i = 1; i <= lastItemID; i++) {
			TREE * mouseEvent = NULL;
			if (findChildTree(mouseEvents, i, &mouseEvent) != SUCCESS) {
				notFoundErr();
			}
			addItemIndexes(&mouseEvent->data);
			showItems(&mouseEvent->data);
		}

		isNewFile = FALSE;
		isSaved = TRUE;

		return TRUE;
	}
	else {
		return FALSE;
	}
}