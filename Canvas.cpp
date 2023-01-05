#include "Canvas.h"
#include <windows.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C"
{
#endif

    void canvasOnDraw(IComponent *component)
    {
        Canvas *canvas = (Canvas *)component;
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(canvas->parent.hwnd, &ps);
        BitBlt(hdc, 0, 0, canvas->bitmap.bmWidth, canvas->bitmap.bmHeight, canvas->hDC, 0, 0, SRCCOPY);
        EndPaint(canvas->parent.hwnd, &ps);
    }

    void canvasOnMouseClick(IComponent *component, int x, int y)
    {
        Canvas *canvas = (Canvas *)component;
        canvas->isMouseClicked = TRUE;
    }

    void canvasOnMouseMove(IComponent *component, int x, int y)
    {
        Canvas *canvas = (Canvas *)component;

        InvalidateRect(component->hwnd, NULL, FALSE);

        canvas->lastX = x;
        canvas->lastY = y;
    }
    void canvasOnMouseUp(IComponent *component, int x, int y)
    {
        Canvas *canvas = (Canvas *)component;
        canvas->isMouseClicked = FALSE;
    }

    void create_Canvas(Canvas *component, HWND hwnd, int x, int y, int width, int height, int visible, int enabled)
    {
        IComponent tmp;
        memset(&tmp, 0, sizeof(IComponent));
        IComponent *tmpComponent = &tmp;
        create_IComponent(tmpComponent, hwnd, x, y, width, height, visible, enabled, canvasOnDraw, canvasOnMouseMove, canvasOnMouseClick, canvasOnMouseUp);
        (*component).parent = *tmpComponent;

        (*component).hDC = CreateCompatibleDC(NULL);
        if((*component).hDC==NULL){
            printf("CreateCompatibleDC FAILS\n");
        }
        (*component).hBitmap = (HBITMAP)LoadImage(
            (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
            TEXT("test.bmp"), IMAGE_BITMAP,
            0, 0, LR_LOADFROMFILE);
        if((*component).hBitmap==NULL){
            printf("LoadImageFails\n");
        }

        SelectObject((*component).hDC, (*component).hBitmap);
        GetObject((*component).hBitmap, sizeof(BITMAP), (BITMAP*) &((*component).bitmap));
    }

#ifdef __cplusplus
}
#endif