#ifndef _CANVAS_H_
#define _CANVAS_H_

#include "IComponent.h"

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct Canvas
    {
        IComponent parent;
        HDC hDC;
        HBITMAP hBitmap;
        BITMAP bitmap;
        LPCWSTR fileName;
        int isMouseClicked;
        int lastX;
        int lastY;
        POINT points[100000];
        int start;
        int end;
    } Canvas;

    void create_Canvas(Canvas* component,HWND hwnd, int x, int y, int width, int height, int visible, int enabled);

#ifdef __cplusplus
}
#endif

#endif