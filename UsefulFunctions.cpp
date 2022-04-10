//
// Created by floodd on 23/03/2022.
//DONT TOUCH
#include "UsefulFunctions.h"
static TCHAR szWindowClass[] = _T("DesktopApp");

// The string that appears in the application's title bar.
static TCHAR szTitle[] = _T("Image Editing Application");

// Stored instance handle for use in Win32 API calls such as FindResource
HINSTANCE hInst;

// Forward declarations of functions included in this code module:
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);



int createWindow(HINSTANCE &hInstance, int &nCmdShow, HMENU &hMenubar) {
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(wcex.hInstance, IDI_APPLICATION);
    wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, IDI_APPLICATION);

    if (!RegisterClassEx(&wcex))
    {
        MessageBox(NULL, _T("Call to RegisterClassEx failed!"),
                   _T("Windows Desktop Guided Tour"),0);

        return 1;
    }

    // Store instance handle in our global variable
    hInst = hInstance;

    // The parameters to CreateWindowEx explained:
// WS_EX_OVERLAPPEDWINDOW : An optional extended window style.
// szWindowClass: the name of the application
// szTitle: the text that appears in the title bar
// WS_OVERLAPPEDWINDOW: the type of window to create
// CW_USEDEFAULT, CW_USEDEFAULT: initial position (x, y)
// 500, 100: initial size (width, length)
// NULL: the parent of this window
// NULL: this application does not have a menu bar
// hInstance: the first parameter from WinMain
// NULL: not used in this application
    HWND hWnd = CreateWindowEx(
            WS_EX_OVERLAPPEDWINDOW ,
            szWindowClass,
            szTitle,
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT,
            300, 400,
            NULL,
            hMenubar,
            hInstance,
            NULL
    );


    if (!hWnd)
    {
        MessageBox(NULL, _T("Call to CreateWindow failed!"),
                   _T("Windows Desktop Guided Tour"),0);

        return 1;
    }

    // The parameters to ShowWindow explained:
// hWnd: the value returned from CreateWindow
// nCmdShow: the fourth parameter from WinMain
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    // Main message loop:
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int) msg.wParam;
}

string saveFilename(const char *filter , HWND owner) {
    OPENFILENAME ofn;
    char fileName[MAX_PATH] = "";
    ZeroMemory(&ofn, sizeof(ofn));

    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = owner;
    ofn.lpstrFilter = filter;
    ofn.lpstrFile = fileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_EXPLORER | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
    ofn.lpstrDefExt = ".ppm";

    string fileNameStr;

    if ( GetSaveFileName(&ofn) )
        fileNameStr = fileName;

    return fileNameStr;
}
string openfilename(const  char *filter, HWND owner) {
    OPENFILENAME ofn;
    char fileName[MAX_PATH] = "";
    ZeroMemory(&ofn, sizeof(ofn));

    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = owner;
    ofn.lpstrFilter = filter;
    ofn.lpstrFile = fileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
    ofn.lpstrDefExt = "ppm";

    string fileNameStr;

    if ( GetOpenFileName(&ofn) )
        fileNameStr = fileName;

    return fileNameStr;
}
void dblBuffer(HWND hWnd, HDC hdc, LPRECT rcClientRect, Image *image) {
    Rgb *pixels;
    HDC memDC = CreateCompatibleDC(hdc );

    // get window's client rectangle. We need this for bitmap creation.
    RECT rcClientRectangle;
    GetClientRect( hWnd, rcClientRect );

    // now we can create bitmap where we shall do our drawing
    HBITMAP bmp = CreateCompatibleBitmap( hdc,rcClientRect->right - rcClientRect->left,
                                          rcClientRect->bottom - rcClientRect->top );

    // we need to save original bitmap, and select it back when we are done,
// in order to avoid GDI leaks!
    HBITMAP oldBmp = (HBITMAP)SelectObject( memDC, bmp );


    pixels = image->getImage();
    int w = image->getWidth();
    for(int r = 0; r < image->getHeight(); r++)
    {
        for(int c = 0; c < w; c++)
        {
            Rgb col = pixels[(r*w)+c];
            COLORREF color = RGB((int)(col.r), (int)(col.g),(int)(col.b));
            // COLORREF color = RGB(255,0,0);
            //cout << col.r*255 <<endl;
            SetPixel(memDC,c,r,color);
        }
    }


    BitBlt( hdc, 0, 0, rcClientRect->right - rcClientRect->left,
            rcClientRect->bottom - rcClientRect->top, memDC, 0, 0, SRCCOPY );

    // all done, now we need to cleanup
    SelectObject( memDC, oldBmp ); // select back original bitmap
    DeleteObject( bmp ); // delete bitmap since it is no longer required
    DeleteDC( memDC );
}
