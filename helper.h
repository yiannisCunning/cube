#pragma once

#include <windows.h>
#include <d2d1.h>
#include "windClass.h"



void cprStr(TCHAR *original, TCHAR *dest, int size);





// paint functions:

// Paint the window hwnd with backgound of col
void paintBackgroundGDI(HWND hwnd, COLORREF col);