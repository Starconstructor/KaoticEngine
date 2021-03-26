/*
 * NAppGUI Cross-platform C SDK
 * © 2015-2021 Francisco Garcia Collado
 * All rights reserved
 * https://nappgui.com/en/legal/eula.html
 *
 * File: ptr.h
 * https://nappgui.com/en/sewer/ptr.html
 *
 */

/* Safety pointer manipulation */ 

#include "sewer.hxx"

__EXTERN_C

void *ptr_dget_imp(void **dptr);

void *ptr_dget_no_null_imp(void **dptr);

void ptr_destroy_optional_imp(void **dptr, FPtr_destroy func_destroy);

void *ptr_copy_optional_imp(void *ptr, FPtr_copy func_copy);

__END_C

#if defined (__ASSERTS__)

#define ptr_get(ptr, type)\
    ((void)((ptr) == (type*)(ptr)),\
    cassert_no_null((ptr)),\
    *((type*)ptr))

#else

#define ptr_get(ptr, type)\
    ((void)((ptr) == (type*)(ptr)),\
    *(ptr))

#endif

#define ptr_dget(dptr, type)\
    ((void)((dptr) == (type**)(dptr)),\
	(type*)ptr_dget_imp((void**)(dptr)))

#define ptr_dget_no_null(dptr, type)\
    ((void)((dptr) == (type**)(dptr)),\
    (type*)ptr_dget_no_null_imp((void**)(dptr)))\

#define ptr_assign(dest, src)\
	if ((dest) != NULL) (*dest) = (src)

#define ptr_destopt(func_destroy, dptr, type)\
    ((void)((dptr) == (type**)(dptr)),\
    FUNC_CHECK_DESTROY(func_destroy, type),\
    ptr_destroy_optional_imp((void**)(dptr), (FPtr_destroy)func_destroy))

#define ptr_copyopt(func_copy, ptr, type)\
    ((void)((ptr) == (type*)(ptr)),\
    FUNC_CHECK_COPY(func_copy, type),\
    (type*)ptr_copy_optional_imp((void*)(ptr), (FPtr_copy)func_copy))
