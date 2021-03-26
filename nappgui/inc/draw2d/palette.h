/*
 * NAppGUI Cross-platform C SDK
 * © 2015-2021 Francisco Garcia Collado
 * All rights reserved
 * https://nappgui.com/en/legal/eula.html
 *
 * File: palette.h
 * https://nappgui.com/en/draw2d/palette.html
 *
 */

/* Color palette */

#include "draw2d.hxx"

__EXTERN_C

Palette *palette_create(const uint32_t size);

Palette *palette_optimal(const uint32_t width, const uint32_t height, const pixformat_t format, const byte_t *data, const uint32_t size);

Palette *palette_cga2(const bool_t mode, const bool_t intense);

Palette *palette_ega4(void);

Palette *palette_rgb8(void);

Palette *palette_gray1(void);

Palette *palette_gray2(void);

Palette *palette_gray4(void);

Palette *palette_gray8(void);

Palette *palette_binary(const color_t zero, const color_t one);

void palette_destroy(Palette **palette);

uint32_t palette_size(const Palette *palette);

color_t *palette_colors(Palette *palette);

const color_t *palette_ccolors(const Palette *palette);

__END_C
