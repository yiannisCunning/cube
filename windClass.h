#pragma once

#include <windows.h>
#include <d2d1.h>
#include "things.h"


//*******************************************************************************************//

//***********************************  Shell  ***********************************************//

//*******************************************************************************************//


struct StateInfoMain {

       RECT sqr;
};


// Main window
class ShellWindow
{
       TCHAR *appName;                 // name shown on window
	WNDCLASS wndClass = {};            // window class
	HWND hwnd;                         // handle to window
       struct StateInfoMain *pState;      // state info

public:
       // constructor
       ShellWindow(HINSTANCE hInstance, TCHAR *name, int nameSize);

       // creates window and registers it, will return 1 if successful
       int createWnd(HINSTANCE hInstance, int nCmdShow);

       // window procedure function
       static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

       // returns handle to window
       HWND getHwnd();
};



//*******************************************************************************************//

//********************************  Canvas  *************************************************//

//*******************************************************************************************//


struct StateInfo {
       ID2D1Factory                *pFactory;
       ID2D1HwndRenderTarget       *pRenderTarget;
       ID2D1SolidColorBrush        *pBrush;

       ID2D1Bitmap                 *bitmap;
       D2D1_BITMAP_PROPERTIES      prop;
       D2D1_SIZE_U                 size;
       int width;
       int height;
       RECT sqr;

       int x;
       int y;

       camera *cam;
       D2D1_RECT_U rectangle1;
       cube *rube;
       float theta;
};



class ChildWindow
{
       TCHAR appName[10];
	WNDCLASS wndClass = {};
	HWND hwnd;
public:
       struct StateInfo *pState;

       // constructor
       ChildWindow(HINSTANCE hInstance);

       // deconstuctor
       ~ChildWindow();

       int registerWnd();
       int createWnd(HWND mainWnd, HINSTANCE hInstance, int nCmdShow);
       static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
       HWND getHwnd();
       void changelens(float *w);

};




//*******************************************************************************************//

//*******************************************************************************************//

//*******************************************************************************************//