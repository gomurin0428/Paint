#define BMPFILE TEXT("test.bmp")

#ifndef UTILITY_H_INCLUDED
#define UTILITY_H_INCLUDED

#include "utility.h"

#endif

#include <windows.h>
#include <stdio.h>
#include <signal.h>
#include <windef.h>
#include <stdint.h>
#include <time.h>
#include "IComponent.h"
#include "Canvas.h"

static IComponent* canvas;
Canvas can;

static LPBYTE p[10000000];
static BOOL isMouseClicked = 0;
static LPCTSTR pctKitty = TEXT("Kitty on your lap");
static unsigned short int x = 0, y = 0;
static unsigned short int last_x = 0, last_y = 0;
static HDC hdc;
static PAINTSTRUCT ps;



static int count = 0;

static isProcessing = 0;

static HPEN pens[1000000];
static MOUSEMOVEPOINT point[1000000];
static int current = -1;
static int last = -1;

int HDCToFile(const char *FilePath, HDC Context, RECT Area, uint16_t BitsPerPixel)
{
	uint32_t Width = Area.right - Area.left;
	uint32_t Height = Area.bottom - Area.top;

	BITMAPINFO Info;
	BITMAPFILEHEADER Header;
	memset(&Info, 0, sizeof(Info));
	memset(&Header, 0, sizeof(Header));
	Info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	Info.bmiHeader.biWidth = Width;
	Info.bmiHeader.biHeight = Height;
	Info.bmiHeader.biPlanes = 1;
	Info.bmiHeader.biBitCount = BitsPerPixel;
	Info.bmiHeader.biCompression = BI_RGB;
	Info.bmiHeader.biSizeImage = Width * Height * (BitsPerPixel > 24 ? 4 : 3);
	Header.bfType = 0x4D42;
	Header.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

	char **Pixels;
	HDC MemDC = CreateCompatibleDC(Context);
	HBITMAP Section = CreateDIBSection(Context, &Info, DIB_RGB_COLORS, &Pixels, NULL, NULL);

	DeleteObject(SelectObject(MemDC, Section));
	BitBlt(MemDC, 0, 0, Width, Height, Context, Area.left, Area.top, SRCCOPY);
	DeleteDC(MemDC);

	FILE *stream;

	if ((stream = fopen(FilePath, "w")) == NULL)
	{
		printf("Could not open data file for reading¥n");
		return 0;
	}
	printf("test%ld\n", (((BitsPerPixel * Width + 31) & ~31) / 8) * Height);
	fwrite((char *)&Header, sizeof(Header), 1, stream);
	fwrite((char *)&Info.bmiHeader, sizeof(Info.bmiHeader), 1, stream);
	fwrite(Pixels, (((BitsPerPixel * Width + 31) & ~31) / 8) * Height, 1, stream);
	fflush(stream);
	fclose(stream);
	DeleteObject(Section);

	DeleteObject(Section);
	return 1;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	int id;

	switch (uMsg)
	{

	case WM_LBUTTONUP:
	{
		x = LOWORD(lParam);
		y = HIWORD(lParam);
		(*canvas).onMouseUp(canvas,(int)x,(int)y);
	}
	break;

	case WM_MOUSEMOVE:
	{

		x = LOWORD(lParam);
		y = HIWORD(lParam);

		(*canvas).onMouseMove(canvas,(int)x,(int)y);
	}
	break;

	case WM_LBUTTONDOWN:
	{
		x = LOWORD(lParam);
		y = HIWORD(lParam);
		(*canvas).onMouseClick(canvas,(int)x,(int)y);
	}
	break;

	case WM_CREATE:
	{
		canvas = &can;
		create_Canvas(canvas,hwnd,10,10,800,800,1,1);
	}
	break;

	case WM_DESTROY:
	{
		/*
		RECT rect;
		rect.bottom = bitmap.bmHeight;
		rect.left = 0;
		rect.right = bitmap.bmWidth;
		rect.top = 0;
		HDCToFile("C:\\Users\\gomur\\test.bmp", hMemDC, rect, 24);
		DeleteDC(hMemDC);
		DeleteObject(hBitmap);
		*/
		PostQuitMessage(0);
	}
	break;

	case WM_PAINT:
	{
		(*canvas).draw(canvas);
	}
	break;
	default:
		return (DefWindowProc(hwnd, uMsg, wParam, lParam));
	}

	return 0;
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hInstPrev, PSTR cmdline, int nCmdShow)
{

	const LPCWSTR CLASS_NAME = L"aaa";
	const LPCSTR TITLE_NAME = TEXT("tesatest");

	WNDCLASS wc;

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = CLASS_NAME;

	RegisterClass(&wc);

	HWND hwnd = CreateWindowEx(
		WS_EX_OVERLAPPEDWINDOW,
		CLASS_NAME,
		TITLE_NAME,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL);

	if (hwnd == NULL)
	{
		return 0;
	}

	ShowWindow(hwnd, nCmdShow);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}
