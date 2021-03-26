/*
 * NAppGUI Cross-platform C SDK
 * © 2015-2021 Francisco Garcia Collado
 * All rights reserved
 * https://nappgui.com/en/legal/eula.html
 *
 * File: nowarn.hxx
 * https://nappgui.com/en/sewer/nowarn.html
 *
 */

/* This header will ignore ALL warnings of code file (.c .cpp .m etc) */
/* USE ONLY IN THIRD PARTY FILES, NOT IN OWN FILES */

#if defined (_MSC_VER)
#pragma warning(push, 0)
#pragma warning( disable : 4514 )
#pragma warning( disable : 4625 )
#pragma warning( disable : 4626 )
#endif

#if defined (__GNUC__)
#if defined(__APPLE__)

#if (__GNUC__ < 4) || (__GNUC__ == 4)

/*
#pragma GCC diagnostic ignored "-Wall"
#pragma GCC diagnostic ignored "-pedantic"
#pragma GCC diagnostic ignored "-Wformat"
#pragma GCC diagnostic ignored "-Wpointer-sign"
#pragma GCC diagnostic ignored "-Wbad-function-cast"
#pragma GCC diagnostic ignored "-Wtraditional"
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wmissing-prototypes"
#pragma GCC diagnostic ignored "-Wformat-nonliteral"
#pragma GCC diagnostic ignored "-Wformat-security"
#pragma GCC diagnostic ignored "-Wextra"
#pragma GCC diagnostic ignored "-Wpointer-arith"
#pragma GCC diagnostic ignored "-Wbad-function-cast"
#pragma GCC diagnostic ignored "-Wsign-compare"
#pragma GCC diagnostic ignored "-Waggregate-return"
#pragma GCC diagnostic ignored "-Wstrict-prototypes"
#pragma GCC diagnostic ignored "-Wold-style-definition"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#pragma GCC diagnostic ignored "-Wmissing-format-attribute"
#pragma GCC diagnostic ignored "-Wshadow"
*/

/* Disable QuickDraw support in old MacOSX */
#if (__GNUC__ == 4) && (__GNUC_MINOR__ == 2)
#define __QUICKDRAW__
#define __QUICKDRAWAPI__
#define __PALETTES__
#define __PICTUTILS__
#define __QUICKDRAWTYPES__
#define __QDOFFSCREEN__
#define __VIDEO__
#define __DISPLAYS__
#define __ATSUNICODEDRAWING__
#define __HISHAPE__
#define __ICONS__
#endif

#if (__GNUC__ == 4) && (__GNUC_MINOR__ > 2)
#pragma GCC diagnostic push
#endif
//#pragma GCC diagnostic warning "-w"

#endif
#endif
#endif

#if defined (__clang__)

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wall"
#pragma GCC diagnostic ignored "-Wshadow"
#pragma GCC diagnostic ignored "-Weverything"
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Wmissing-prototypes"
#pragma clang diagnostic ignored "-Wobjc-method-access"
//#pragma GCC diagnostic ignored "-Wstrict-prototypes"
//#pragma clang diagnostic ignored "-Wdeprecated-implementations"

#endif

