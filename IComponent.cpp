#include "IComponent.h"

#ifdef __cplusplus
extern "C"
{
#endif

    

    void create_IComponent(IComponent* component, HWND hwnd,int x, int y, int width, int height, int visible, int enabled, void (*draw)(IComponent*), void (*onMouseMove)(IComponent*,int, int), void (*onMouseClick)(IComponent*,int, int), void (*onMouseUp)(IComponent*,int, int))
    {
        (*component).hwnd = hwnd;
        (*component).x = x;
        (*component).y = y;
        (*component).width = width;
        (*component).height = height;
        (*component).visible = visible;
        (*component).enabled = enabled;
        (*component).draw = draw;
        (*component).onMouseMove = onMouseMove;
        (*component).onMouseClick = onMouseClick;
        (*component).onMouseUp = onMouseUp;
    }

#ifdef __cplusplus
}
#endif