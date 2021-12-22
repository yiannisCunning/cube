#include <windows.h>
#include <d2d1.h>
#include "windClass.h"
#include "helper.h"


void cprStr(TCHAR *original, TCHAR *dest, int n){
       for(int i = 0; i < n; i++){
              *(dest + i) = *(original + i);
       }
       *(original + n) = NULL;
}



void paintBackgroundGDI(HWND hwnd, COLORREF col){

}