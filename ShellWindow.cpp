#include "windClass.h"
#include <windows.h>
#include <d2d1.h>
#include "helper.h"


ShellWindow::ShellWindow(HINSTANCE hInstance, TCHAR *name, int nameSize)
{
	appName = (TCHAR *)malloc(sizeof(TCHAR) * (nameSize + 1));
	cprStr(name, appName, nameSize);

	wndClass.hInstance = hInstance;
	wndClass.lpfnWndProc = WindowProc;
	wndClass.lpszClassName = appName;
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);	// cursor syle

	pState = (struct StateInfoMain *)malloc(sizeof(struct StateInfoMain));
}




int ShellWindow::createWnd(HINSTANCE hInstance, int nCmdShow)
{
	RegisterClass (&wndClass);

	hwnd = CreateWindowEx(
		0,			// extended style
		appName,		// class name
		appName,		// WindowName
		WS_OVERLAPPEDWINDOW,	// style
		0,0,1800,900,//x,y,w,hCW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT
		NULL,			// parent
		NULL,			// menu
		hInstance,		// instance handle
		pState			// state thing
	);

       ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	if(hwnd != NULL){return 1;}
	else{return 0;}
}



HWND ShellWindow::getHwnd(){return hwnd;}


