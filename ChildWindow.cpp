#include "windClass.h"
#include <windows.h>
#include <Windowsx.h>
#include <d2d1.h>
#include <stdio.h>
#include <math.h>
#include "things.h"
#include "vectorFun.h"

static inline struct StateInfo* GetAppState(HWND hwnd)
{
    LONG_PTR ptr = GetWindowLongPtr(hwnd, GWLP_USERDATA);
    struct StateInfo *pState =(struct StateInfo *)(ptr);
    return pState;
}

//constructor
ChildWindow::ChildWindow(HINSTANCE hInstance)
{

       appName[0] = 'c';
       appName[1] = NULL;


	wndClass.style        = (CS_HREDRAW | CS_VREDRAW);
	wndClass.lpfnWndProc = WindowProc;
	wndClass.cbClsExtra   = 0;
	wndClass.cbWndExtra = sizeof(long);
	wndClass.hInstance    = hInstance;
	wndClass.hIcon        = LoadIcon (NULL, IDI_APPLICATION);
	wndClass.hCursor  = LoadCursor (NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = appName;


       pState = (struct StateInfo *)malloc(sizeof(struct StateInfo));
       HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &(pState->pFactory));
       pState->height = 700;
       pState->width = 700;


       
       camera *c = new camera(650, 650);
       c->clear_screen();

       
	float v[3] = {19.82999999,9.38472823,0};

       float ra = sqrt(20*20 + 10*10);
       setVector(v, ra*cos(0), ra*sin(0), 10);

	c->setlens(v);
	c->drawAxis();
       pState->cam = c;

       pState->rectangle1 = D2D1::RectU(0, 0, pState->cam->width, pState->cam->height);


       cube *r = new cube();
       r->color_all_faces();
       pState->rube = r;
       pState->cam->draw_cube(*r);
       pState->theta = 0;
}

//Destructor
ChildWindow::~ChildWindow()
{

}




// register window
int ChildWindow::registerWnd()
{
       return RegisterClass (&wndClass);      
}


// create and show window
int ChildWindow::createWnd(HWND mainWnd, HINSTANCE hInstance, int nCmdShow)
{
       RECT rc;
       GetClientRect(mainWnd, &rc);
       int w = rc.right - 20;
       int h = rc.bottom - 110;      

	hwnd = CreateWindow(
              appName,        
              TEXT("Hello World"), 
              WS_CHILD | WS_VISIBLE,  //| WS_MINIMIZEBOX | WS_MAXIMIZEBOX| WS_SYSMENU | WS_THICKFRAME| WS_CAPTION
              10,//CW_USEDEFAULT,      
              100,//CW_USEDEFAULT,      
              w,        
              h,        
              mainWnd,               
              NULL,               
              hInstance,
              pState);

       ShowWindow(hwnd, nCmdShow);
       UpdateWindow(hwnd);


       return 1;
}


HWND ChildWindow::getHwnd()
{
       return hwnd;
}


void ChildWindow::changelens(float *w)
{
       
       pState->cam->setlens(w);
       pState->cam->clear_screen();
       pState->cam->drawAxis();
       pState->cam->draw_cube(*pState->rube);
       GetClientRect(hwnd, &(pState->sqr));
       InvalidateRect(hwnd, &(pState->sqr), FALSE);
}








//***************************************************************************************//
// Non class functions //
template <class T> void SafeRelease(T **ppT)
{
    if (*ppT)
    {
        (*ppT)->Release();
        *ppT = NULL;
    }
}


static void CreateGraphicsResources(HWND hwnd, struct StateInfo *pState){
       RECT rc;
       GetClientRect(hwnd, &rc);
       //GetUpdateRect(hwnd, &rc, FALSE);
       D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

       pState->pFactory->CreateHwndRenderTarget(
            D2D1::RenderTargetProperties(),
            D2D1::HwndRenderTargetProperties(hwnd, size),
            &(pState->pRenderTarget));
       
       pState->pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0.0f,0.0f,0.0f,0.0f), &(pState->pBrush));

       pState->size = D2D1::SizeU(pState->width, pState->height);
       pState->prop = D2D1::BitmapProperties(D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM,D2D1_ALPHA_MODE_IGNORE), 96.0f, 96.0f);
       HRESULT hr = pState->pRenderTarget->CreateBitmap(pState->size, pState->prop, &(pState->bitmap));

}


static void ReleaseGraphicsResources(HWND hwnd, struct StateInfo *pState){
       SafeRelease(&(pState->pRenderTarget));
       SafeRelease(&(pState->pBrush));
       SafeRelease(&(pState->bitmap));
}











LRESULT CALLBACK ChildWindow::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
       struct StateInfo *pState;
       HRESULT hr;


       if(uMsg == WM_CREATE){
		CREATESTRUCT *pCreate = (CREATESTRUCT *)(lParam);
		pState = (struct StateInfo*)(pCreate->lpCreateParams);
		SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pState);
              pState->x = -1;
              pState->y = -1;

              CreateGraphicsResources(hwnd, pState);

       }

       else{pState = GetAppState(hwnd);}


       switch(uMsg){
              case WM_DESTROY:
			PostQuitMessage(0);
                     ReleaseGraphicsResources(hwnd, pState);
                     return 0;
              case WM_PAINT:

                     pState->pRenderTarget->BeginDraw();

                     pState->pRenderTarget->Clear( D2D1::ColorF(D2D1::ColorF::DimGray) );

                     pState->bitmap->CopyFromMemory(&(pState->rectangle1), pState->cam->screen, 4 * pState->cam->width);
                     pState->pRenderTarget->DrawBitmap(pState->bitmap, D2D1::RectF(5, pState->height + 5, pState->width + 5, 5));


                     hr = pState->pRenderTarget->EndDraw();
                     return 0;

       }
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}






