#ifndef _ICOMPONENT_H_
#define _ICOMPONENT_H_

#include <windows.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct IComponent
    {
        HWND hwnd;
        int x;
        int y;
        int width;
        int height;
        int visible;
        int enabled;
        void (*draw)(struct IComponent* coponent);
        void (*onMouseMove)(struct IComponent* component,int x, int y);
        void (*onMouseClick)(struct IComponent* component,int x, int y);
        void (*onMouseUp)(struct IComponent* component,int x, int y);
        void (*onDestroy)(struct IComponent* component);
    } IComponent;

    void create_IComponent(IComponent* component, HWND hwnd,int x, int y, int width, int height, int visible, int enabled, void (*draw)(IComponent*), void (*onMouseMove)(IComponent*,int, int), void (*onMouseClick)(IComponent*,int, int), void (*onMouseUp)(IComponent*,int, int));

#ifdef __cplusplus
}
#endif

#endif