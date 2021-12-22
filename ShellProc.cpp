#include "windClass.h"
#include <windows.h>
#include <d2d1.h>
#include <stdio.h>
/*
 - need to make buttons to control state and tool
 - just need to paint backgound




*/

void paint(HWND hwnd, struct StateInfoMain *pState){
                     PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd, &ps);

                     const COLORREF rgbRed   =  0x00FFFFFF;
			HBRUSH h = CreateSolidBrush(rgbRed);

			FillRect(hdc, &ps.rcPaint, h);
                     EndPaint(hwnd, &ps);
}

LRESULT CALLBACK ShellWindow::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{	
	struct StateInfoMain *pState;

       if(uMsg == WM_CREATE){
		CREATESTRUCT *pCreate = (CREATESTRUCT *)(lParam);
		pState = (struct StateInfoMain*)(pCreate->lpCreateParams);
		SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pState);

       }

       else{pState = (struct StateInfoMain *)GetWindowLongPtr(hwnd, GWLP_USERDATA);;}
	
	switch(uMsg){
		case WM_DESTROY:
			PostQuitMessage(0);
			return(0);
              case WM_PAINT:
                     paint(hwnd, pState);
              	return 0;

              case WM_LBUTTONDOWN:
                     printf("Clicked");
                     return 0;

	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

