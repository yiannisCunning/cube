#include <Windows.h>
#include <stdio.h>
#include <d2d1.h>
#include <math.h>
#include "WindClass.h"
#include "things.h"
#include "cube.h"
#include "vectorFun.h"

// -lstdc++
// gcc ShellWindow.cpp ChildWindow.cpp shell.cpp -ld2d1 -lgdi32 -lstdc++ -o run
// gcc *.cpp -ld2d1 -lgdi32 -lstdc++ -o run

// Windows initialzation
//
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){


	static TCHAR name[] = TEXT ("WindowThing");
	int size = 11;

	ShellWindow shell(hInstance, name, size);
	int h = shell.createWnd(hInstance, nCmdShow);


	ChildWindow canvas(hInstance);
	canvas.registerWnd();
	canvas.createWnd(shell.getHwnd(), hInstance, nCmdShow);






	if (h == 0){return 0;}	
	int i = 0;
	double theta = 0;
	float v[3] = {0,0,0};
	float r = sqrt(20*20 + 10*10);
	
	MSG msg = {};
	while(GetMessage(&msg, NULL, 0, 0)){
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		if(i == 1){
			i = 0;
			theta = theta += 0.01;
			setVector(v, r*cos(theta),r*sin(theta),r*cos(theta));
			canvas.changelens(v);
		}
		i++;
	}
	
    	return 0;
}



/// tools 
// input shape with dimensions at pos - sqr at 5,5 to x,y