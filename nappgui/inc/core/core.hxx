/*
 * NAppGUI Cross-platform C SDK
 * © 2015-2021 Francisco Garcia Collado
 * All rights reserved
 * https://nappgui.com/en/legal/eula.html
 *
 * File: core.hxx
 * https://nappgui.com/en/core/core.html
 *
 */

/* Core library */

#ifndef __CORE_HXX__
#define __CORE_HXX__

#include "osbs.hxx"

typedef enum _core_event_t
{
    ekEASSERT = 0x100,
    ekEFILE,
    ekEENTRY,
    ekEEXIT
} core_event_t;

typedef enum _sstate_t
{
    ekSTOK,
    ekSTEND,
    ekSTCORRUPT,
    ekSTBROKEN
} sstate_t;

typedef enum _key_t
{
    ekKEY_UNDEF             = 0,
    ekKEY_A                 = 1,
    ekKEY_S                 = 2,
    ekKEY_D                 = 3,
    ekKEY_F                 = 4,
    ekKEY_H                 = 5,
    ekKEY_G                 = 6,
    ekKEY_Z                 = 7,
    ekKEY_X                 = 8,
    ekKEY_C                 = 9,
    ekKEY_V                 = 10,
    ekKEY_BSLASH            = 11,
    ekKEY_B                 = 12,
    ekKEY_Q                 = 13,
    ekKEY_W                 = 14,
    ekKEY_E                 = 15,
    ekKEY_R                 = 16,
    ekKEY_Y                 = 17,
    ekKEY_T                 = 18,
    ekKEY_1                 = 19,
    ekKEY_2                 = 20,
    ekKEY_3                 = 21,
    ekKEY_4                 = 22,
    ekKEY_6                 = 23,
    ekKEY_5                 = 24,
    ekKEY_9                 = 25,
    ekKEY_7                 = 26,
    ekKEY_8                 = 27,
    ekKEY_0                 = 28,
    ekKEY_RCURLY            = 29,
    ekKEY_O                 = 30,
    ekKEY_U                 = 31,
    ekKEY_LCURLY            = 32,
    ekKEY_I                 = 33,
    ekKEY_P                 = 34,
    ekKEY_RETURN            = 35,
    ekKEY_L                 = 36,
    ekKEY_J                 = 37,
    ekKEY_SEMICOLON         = 38,
    ekKEY_K                 = 39,
    ekKEY_QUEST             = 40,
    ekKEY_COMMA             = 41,
    ekKEY_MINUS             = 42,
    ekKEY_N                 = 43,
    ekKEY_M                 = 44,
    ekKEY_PERIOD            = 45,
    ekKEY_TAB               = 46,
    ekKEY_SPACE             = 47,
    ekKEY_GTLT              = 48,
    ekKEY_BACK              = 49,
    ekKEY_ESCAPE            = 50,
    ekKEY_F17               = 51,
    ekKEY_NUMDECIMAL        = 52,
    ekKEY_NUMMULT           = 53,
    ekKEY_NUMADD            = 54,
    ekKEY_NUMLOCK           = 55,
    ekKEY_NUMDIV            = 56,
    ekKEY_NUMRET            = 57,
    ekKEY_NUMMINUS          = 58,
    ekKEY_F18               = 59,
    ekKEY_F19               = 60,
    ekKEY_NUMEQUAL          = 61,
    ekKEY_NUM0              = 62,
    ekKEY_NUM1              = 63,
    ekKEY_NUM2              = 64,
    ekKEY_NUM3              = 65,
    ekKEY_NUM4              = 66,
    ekKEY_NUM5              = 67,
    ekKEY_NUM6              = 68,
    ekKEY_NUM7              = 69,
    ekKEY_NUM8              = 70,
    ekKEY_NUM9              = 71,
    ekKEY_F5                = 72,
    ekKEY_F6                = 73,
    ekKEY_F7                = 74,
    ekKEY_F3                = 75,
    ekKEY_F8                = 76,
    ekKEY_F9                = 77,
    ekKEY_F11               = 78,
    ekKEY_F13               = 79,
    ekKEY_F16               = 80,
    ekKEY_F14               = 81,
    ekKEY_F10               = 82,
    ekKEY_F12               = 83,
    ekKEY_F15               = 84,
    ekKEY_PAGEUP            = 85,
    ekKEY_HOME              = 86,
    ekKEY_SUPR              = 87,
    ekKEY_F4                = 88,
    ekKEY_PAGEDOWN          = 89,
    ekKEY_F2                = 90,
    ekKEY_END               = 91,
    ekKEY_F1                = 92,
    ekKEY_LEFT              = 93,
    ekKEY_RIGHT             = 94,
    ekKEY_DOWN              = 95,
    ekKEY_UP                = 96,
    ekKEY_LSHIFT            = 97,
    ekKEY_RSHIFT            = 98,
    ekKEY_LCTRL             = 99,
    ekKEY_RCTRL             = 100,
    ekKEY_LALT              = 101,
    ekKEY_RALT              = 102,
    ekKEY_INSERT            = 103,
    ekKEY_EXCLAM            = 104,
    ekKEY_MENU              = 105,
    ekKEY_LWIN              = 106,
    ekKEY_RWIN              = 107,
    ekKEY_CAPS              = 108,
    ekKEY_TILDE             = 109,
    ekKEY_GRAVE             = 110,
    ekKEY_PLUS              = 111
} vkey_t;

#define kKEY_MAX    112

typedef enum _mkey_t
{
    ekMKEY_NONE     = 0,
    ekMKEY_SHIFT    = 1,
    ekMKEY_CONTROL  = 1 << 1,
    ekMKEY_ALT      = 1 << 2,
    ekMKEY_COMMAND  = 1 << 3
} mkey_t;

typedef enum _lextoken_t
{
    ekTSLCOM = 1,
    ekTMLCOM,
    ekTSPACE,
    ekTEOL,

    ekTLESS,        /* < */
    ekTGREAT,       /* > */
    ekTCOMMA,       /* , */				
    ekTPERIOD,      /* . */
    ekTSCOLON,      /* ; */
    ekTCOLON,       /* : */

    ekTOPENPAR,     /* ( */
    ekTCLOSPAR,     /* ) */
    ekTOPENBRAC,    /* [ */
    ekTCLOSBRAC,    /* ] */
    ekTOPENCURL,    /* { */
    ekTCLOSCURL,    /* } */
    
    ekTPLUS,        /* + */
    ekTMINUS,       /* - */
    ekTASTERK,      /* * */
    ekTEQUALS,      /* = */

    ekTDOLLAR,      /* $ */
    ekTPERCENT,     /* % */
    ekTPOUND,       /* # */
    ekTAMPER,       /* & */

    ekTAPOST,       /* ' */
    ekTQUOTE,       /* " */
    ekTCIRCUM,      /* ^ */
    ekTTILDE,       /* ~ */
    ekTEXCLA,       /* ! */
    ekTQUEST,       /* ? */
    ekTVLINE,       /* | */
    ekTSLASH,       /* / */
    ekTBSLASH,      /* \ */
    ekTAT,          /* @ */

    ekTINTEGER,
    ekTOCTAL,
    ekTHEX,
    ekTREAL,
    ekTSTRING,
    ekTIDENT,

    ekTUNDEF,
    ekTCORRUP,
    ekTEOF,

    ekTRESERVED

} lextoken_t;

typedef struct _array_t Array;
typedef struct _buffer_t Buffer;
typedef struct _keybuf_t KeyBuf;
typedef struct _clock_t Clock;
typedef struct _event_t Event;
typedef struct _lexscn_t LexScn;
typedef struct _listener_t Listener;
typedef struct _rbtree_t RBTree;
typedef const char_t* ResId;
typedef struct _respack ResPack;
typedef struct _regex RegEx;
typedef struct _stream_t Stream;
typedef struct _string_t String;
typedef struct _direntry_t DirEntry;
typedef struct _evfiledir_t EvFileDir;

#define HEAPARR         "::arr"
#define ARRST           "ArrSt::"
#define ARRPT           "ArrPt::"
#define ArrPt(type)     struct Arr##Pt##type
#define ArrSt(type)     struct Arr##St##type
#define SetPt(type)     struct Set##Pt##type
#define SetSt(type)     struct Set##St##type
#define DictPt(type)    struct Dict##Pt##type
#define DictSt(type)    struct Dict##St##type
#define HashPt(type)    struct Hash##Pt##type
#define HashSt(type)    struct Hash##St##type
#define TreePt(type)    struct Tree##Pt##type
#define NodePt(type)    struct Node##Pt##type
#define TreeSt(type)    struct Tree##St##type
#define NodeSt(type)    struct Node##St##type

typedef void(*FPtr_remove)(void *obj);
#define FUNC_CHECK_REMOVE(func, type)\
    (void)((void(*)(type*))func == func)

typedef void(*FPtr_event_handler)(void *obj, Event *event);
#define FUNC_CHECK_EVENT_HANDLER(func, type)\
    (void)((void(*)(type*, Event*))func == func)

typedef void*(*FPtr_read)(Stream *stream);
#define FUNC_CHECK_READ(func, type)\
    (void)((type*(*)(Stream*))func == func)

typedef void(*FPtr_read_init)(Stream *stream, void *obj);
#define FUNC_CHECK_READ_INIT(func, type)\
    (void)((void(*)(Stream*, type*))func == func)

typedef void(*FPtr_write)(Stream *stream, const void *obj);
#define FUNC_CHECK_WRITE(func, type)\
    (void)((void(*)(Stream*, const type*))func == func)

struct _direntry_t
{
    String *name;
    file_type_t type;
    uint64_t size;
    Date date;
};

struct _evfiledir_t
{
    const char_t *filename;
    const char_t *pathname;
    uint32_t depth;
};

ArrSt(bool_t);
ArrSt(int8_t);
ArrSt(int16_t);
ArrSt(int32_t);
ArrSt(int64_t);
ArrSt(uint8_t);
ArrSt(uint16_t);
ArrSt(uint32_t);
ArrSt(uint64_t);
ArrSt(real32_t);
ArrSt(real64_t);
SetSt(bool_t);
SetSt(int8_t);
SetSt(int16_t);
SetSt(int32_t);
SetSt(int64_t);
SetSt(uint8_t);
SetSt(uint16_t);
SetSt(uint32_t);
SetSt(uint64_t);
SetSt(real32_t);
SetSt(real64_t);
ArrPt(String);
ArrPt(ResPack);
ArrPtDecl(String);
DeclType(DirEntry);

#ifdef  __cplusplus

struct IListener
{
protected:
    Listener *listen(IListener *obj, void(IListener::*handler)(Event*));
};

typedef void(IListener::*EventHandler)(Event*);

#endif

#endif
