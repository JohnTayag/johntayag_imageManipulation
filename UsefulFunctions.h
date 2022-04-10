//
// Created by floodd on 23/03/2022.
// DONT TOUCH

#include <windows.h>
#include <iostream>
#include <string.h>
#include <tchar.h>
#include "Image.h"
#include <map>

int createWindow(HINSTANCE &hInstance, int &nCmdShow, HMENU &hMenubar) ;
void dblBuffer(HWND hWnd, HDC hdc, LPRECT rcClientRect);
void processMenu(HWND hWnd, WPARAM wParam);
string saveFilename(const  char *filter = "All Files (*.ppm)\0*.*\0", HWND owner = NULL);
string openfilename(const  char *filter = "All Files (*.ppm)\0*.*\0", HWND owner = NULL);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void dblBuffer(HWND hWnd, HDC hdc, LPRECT rcClientRect, Image *image);
