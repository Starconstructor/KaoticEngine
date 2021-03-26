/*
 * NAppGUI Cross-platform C SDK
 * © 2015-2021 Francisco Garcia Collado
 * All rights reserved
 * https://nappgui.com/en/legal/eula.html
 *
 * File: view.h
 * https://nappgui.com/en/gui/view.html
 *
 */

/* Custom view */

#include "gui.hxx"

__EXTERN_C

View *view_create(void);

View *view_scroll(void);

void view_data_imp(View *view, void **data, FPtr_destroy func_destroy_data);

void *view_get_data_imp(const View *view);

void view_size(View *view, const S2Df size);

void view_OnDraw(View *view, Listener *listener);
    
void view_OnSize(View *view, Listener *listener);

void view_OnEnter(View *view, Listener *listener);

void view_OnExit(View *view, Listener *listener);

void view_OnMove(View *view, Listener *listener);

void view_OnDown(View *view, Listener *listener);

void view_OnUp(View *view, Listener *listener);

void view_OnClick(View *view, Listener *listener);

void view_OnDrag(View *view, Listener *listener);

void view_OnWheel(View *view, Listener *listener);

void view_OnKeyDown(View *view, Listener *listener);

void view_OnKeyUp(View *view, Listener *listener);

void view_OnFocus(View *view, Listener *listener);

void view_keybuf(View *view, KeyBuf *buffer);

void view_get_size(const View *view, S2Df *size);

void view_content_size(View *view, const S2Df size, const S2Df line);

void view_scroll_x(View *view, const real32_t pos);

void view_scroll_y(View *view, const real32_t pos);

void view_scroll_size(const View *view, real32_t *width, real32_t *height);

void view_viewport(const View *view, V2Df *pos, S2Df *size);

void view_point_scale(const View *view, real32_t *scale);

void view_update(View *view);

void *view_native(View *view);

__END_C

#define view_data(view, data, func_destroy_data, type)\
    (\
        (void)((type**)data == data),\
        FUNC_CHECK_DESTROY(func_destroy_data, type),\
        view_data_imp(view, (void**)data, (FPtr_destroy)func_destroy_data)\
    )

#define view_get_data(view, type)\
    (type*)view_get_data_imp(view)
