/*
 * NAppGUI Cross-platform C SDK
 * © 2015-2021 Francisco Garcia Collado
 * All rights reserved
 * https://nappgui.com/en/legal/eula.html
 *
 * File: obb2d.h
 * https://nappgui.com/en/geom2d/obb2d.html
 *
 */

/* Oriented Box 2d */

#include "geom2d.hxx"

__EXTERN_C

OBB2Df* obb2d_createf(const V2Df *center, const real32_t width, const real32_t height, const real32_t angle);

OBB2Dd* obb2d_created(const V2Dd *center, const real64_t width, const real64_t height, const real64_t angle);

OBB2Df* obb2d_from_linef(const V2Df *p0, const V2Df *p1, const real32_t thickness);

OBB2Dd* obb2d_from_lined(const V2Dd *p0, const V2Dd *p1, const real64_t thickness);

OBB2Df* obb2d_copyf(const OBB2Df *obb);

OBB2Dd* obb2d_copyd(const OBB2Dd *obb);

void obb2d_destroyf(OBB2Df **obb);

void obb2d_destroyd(OBB2Dd **obb);

void obb2d_updatef(OBB2Df *obb, const V2Df *center, const real32_t width, const real32_t height, const real32_t angle);

void obb2d_updated(OBB2Dd *obb, const V2Dd *center, const real64_t width, const real64_t height, const real64_t angle);

void obb2d_movef(OBB2Df *obb, const real32_t offset_x, const real32_t offset_y);

void obb2d_moved(OBB2Dd *obb, const real64_t offset_x, const real64_t offset_y);

void obb2d_transf(OBB2Df *obb, const T2Df *t2d);

void obb2d_transd(OBB2Dd *obb, const T2Dd *t2d);

const V2Df *obb2d_cornersf(const OBB2Df *obb);

const V2Dd *obb2d_cornersd(const OBB2Dd *obb);

V2Df obb2d_centerf(const OBB2Df *obb);

V2Dd obb2d_centerd(const OBB2Dd *obb);

real32_t obb2d_widthf(const OBB2Df *obb);

real64_t obb2d_widthd(const OBB2Dd *obb);

real32_t obb2d_heightf(const OBB2Df *obb);

real64_t obb2d_heightd(const OBB2Dd *obb);

real32_t obb2d_anglef(const OBB2Df *obb);

real64_t obb2d_angled(const OBB2Dd *obb);

Box2Df obb2d_boxf(const OBB2Df *obb);

Box2Dd obb2d_boxd(const OBB2Dd *obb);

__END_C

