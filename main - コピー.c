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

static LPBYTE p[10000000];
static HDC hMemDC;
static HBITMAP hBitmap;
static BITMAP bitmap;
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

void timer_handler(HWND hwnd)
{
	if (isProcessing)
	{
		return;
	}

	isProcessing = 1;

	SetBitmapBits(hBitmap, 10000000, p);

	InvalidateRect(hwnd, NULL, FALSE);

	isProcessing = 0;

	return;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	int id;

	switch (uMsg)
	{

	case WM_LBUTTONUP:
	{

		isMouseClicked = 0;
		break;
	}

	case WM_MOUSEMOVE:
	{

		if (!isMouseClicked)
		{
			break;
		}

		x = LOWORD(lParam);
		y = HIWORD(lParam);

		MOUSEMOVEPOINT p;
		p.x = x;
		p.y = y;
		p.time = clock();
		point[(current + 1) % 1000000] = p;
		current++;
		current = current % 1000000;

		InvalidateRect(hwnd, NULL, FALSE);

		POINT point;
		ZeroMemory(&point, sizeof(point));
		point.x = x;
		point.y = y;
		int result = ClientToScreen(hwnd, &point);

		MOUSEMOVEPOINT in_mmp;
		ZeroMemory(&in_mmp, sizeof(in_mmp));

		in_mmp.x = point.x;
		in_mmp.y = point.y;
		MOUSEMOVEPOINT out_mmp_array[64];
		int size = GetMouseMovePointsEx(sizeof(MOUSEMOVEPOINT), &in_mmp, &out_mmp_array, 64, GMMP_USE_DISPLAY_POINTS);

		LPBITMAPINFO lpbmi;

		for (int i = 0; i < size; i++)
		{
			POINT targetPoint;
			ZeroMemory(&targetPoint, sizeof(targetPoint));
			targetPoint.x = out_mmp_array[i].x;
			targetPoint.y = out_mmp_array[i].y;
			ScreenToClient(hwnd, &targetPoint);
		}

		int threadID;

		/*
		int threadResult = CreateThread(
			NULL,		   // default security attributes
			0,			   // use default stack size
			timer_handler, // thread function name
			hwnd,		   // argument to thread function
			0,			   // use default creation flags
			&threadID);	   // returns the thread identifier


		if (threadResult == NULL)
		{
			printf("Thread Creation Fails\n");
		}
		*/

		// InvalidateRect(hwnd, NULL, TRUE);

		last_x = x;
		last_y = y;
	}
	break;

	case WM_LBUTTONDOWN:
	{

		isMouseClicked = 1;

		x = LOWORD(lParam);
		y = HIWORD(lParam);

		MOUSEMOVEPOINT p;
		p.x = x;
		p.y = y;
		p.time = clock();
		point[(current + 1) % 1000000] = p;
		current++;
		current = current % 1000000;

		LPBITMAPINFO lpbmi;

		// GetDIBits(hMemDC, hBitmap, 0, 500, &p, lpbmi, DIB_RGB_COLORS);

/*
		int result = GetBitmapBits(hBitmap, 10000000, &p);

		int count = 0;
		for (int i = 0; i < bitmap.bmHeight; i++)
		{
			for (int j = 0; j < bitmap.bmWidth; j++)
			{

				if (((x - j) * (x - j) + (y - i) * (y - i)) < 100)
				{
					*(p + j + i * bitmap.bmWidth) = 0;
				}
				// printf("%x ", (p[j+i*bitmap.bmWidth]));
				count += 1;
			}
		}
		SetBitmapBits(hBitmap, 10000000, p);
		*/

		InvalidateRect(hwnd, NULL, FALSE);
	}
	break;

	case WM_CREATE:
	{
		hMemDC = CreateCompatibleDC(NULL);
		hBitmap = (HBITMAP)LoadImage(
			(HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
			BMPFILE, IMAGE_BITMAP,
			0, 0, LR_LOADFROMFILE);
		SelectObject(hMemDC, hBitmap);

		GetObject(hBitmap, sizeof(BITMAP), &bitmap);

		printf("%d\n", bitmap.bmHeight);
		printf("%d\n", bitmap.bmWidth);
	}
	break;

	case WM_DESTROY:
	{
		RECT rect;
		rect.bottom = bitmap.bmHeight;
		rect.left = 0;
		rect.right = bitmap.bmWidth;
		rect.top = 0;
		HDCToFile("C:\\Users\\gomur\\test.bmp", hMemDC, rect, 24);
		DeleteDC(hMemDC);
		DeleteObject(hBitmap);
		PostQuitMessage(0);
	}
	break;

	case WM_PAINT:
	{
		hdc = BeginPaint(hwnd, &ps);
		//  TextOut(hdc, x, y, pctKitty, lstrlen(pctKitty));

		for (int i = last + 1; i < (current - 1); i++)
		{
			if (abs(clock() - point[i].time) > 100)
			{
				continue;
			}
			pens[i] = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
			SelectObject(hMemDC, pens[i]);
			if (last == -1)
			{
				MoveToEx(hMemDC, 0, 0, NULL);
			}
			else
			{
				MoveToEx(hMemDC, point[i].x, point[i].y, NULL);
			}
			LineTo(hMemDC, point[i + 1].x, point[i + 1].y);
			last++;
			last = last % 1000000;
		}
		BitBlt(hdc, 0, 0, bitmap.bmWidth, bitmap.bmHeight, hMemDC, 0, 0, SRCCOPY);
		// BitBlt(hMemDC, 0, 0, bitmap.bmWidth, bitmap.bmHeight, hdc, 0, 0, SRCCOPY);

		EndPaint(hwnd, &ps);
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
