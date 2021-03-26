/*
 * NAppGUI Cross-platform C SDK
 * © 2015-2021 Francisco Garcia Collado
 * All rights reserved
 * https://nappgui.com/en/legal/eula.html
 *
 * File: t2d.hpp
 * https://nappgui.com/en/geom2d/t2d.html
 *
 */

/* 2D affine transformations */

#include "v2d.hpp"

template<typename real>
struct T2D
{
    static void (*move)(T2D<real> *dest, const T2D<real> *src, const real x, const real y);

    static void (*rotate)(T2D<real> *dest, const T2D<real> *src, const real angle);

    static void (*scale)(T2D<real> *dest, const T2D<real> *src, const real sx, const real sy);

    static void (*invfast)(T2D<real> *dest, const T2D<real> *src);

    static void (*inverse)(T2D<real> *dest, const T2D<real> *src);

    static void (*mult)(T2D<real> *dest, const T2D<real> *src1, const T2D<real> *src2);

    static void (*vmult)(V2D<real> *dest, const T2D<real> *t2d, const V2D<real> *src);

    static void (*vmultn)(V2D<real> *dest, const T2D<real> *t2d, const V2D<real> *src, const uint32_t n);

    static void (*decompose)(const T2D<real> *t2d, V2D<real> *pos, real *angle, V2D<real> *sc);

    static const T2D<real> *kIDENT;

    V2D<real> i;
    V2D<real> j;
    V2D<real> p;
};
