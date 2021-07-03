#ifndef _sys_h_
#define _sys_h_

//--------------------------------------------------------------------------------
// includes
//--------------------------------------------------------------------------------
#define DIRECTINPUT_VERSION 0x0800

#include <windows.h>
#include <mmsystem.h>
#include <ddraw.h>
#include <dinput.h>
#include <dsound.h>

//--------------------------------------------------------------------------------
// defines
//--------------------------------------------------------------------------------
#define SCREEN_WIDTH        1024
#define SCREEN_HEIGHT       768
#define SCREEN_BPP          32

#define SCREEN_WIDTH_f      1024.0f
#define SCREEN_HEIGHT_f     768.0f

#define SCREEN_HALFWIDTH_f  512.0f
#define SCREEN_HALFHEIGHT_f 384.0f

#define SOUND_BPS           8
#define SOUND_SPS           11025
#define SOUND_CHANNELS      1

#define MAX_SOUNDS          16
#define MAX_DUPSOUNDS       8

#define MAX_FONTS           8
#define MAX_CHARSIZE        32
#define MAX_CHARDRAWSIZE    24

// define a custom window message for graph events
#define WM_GRAPHEVENT	    WM_USER


//--------------------------------------------------------------------------------
// macros
//--------------------------------------------------------------------------------
#define sys_keydown( key )       ( 0 != g_keys[ key ] && 0 == g_oldkeys[ key ] )
#define sys_keyup( key )         ( 0 == g_keys[ key ] && 0 != g_oldkeys[ key ] )
#define sys_keydownnow( key )    ( g_keys[ key ] & 0x80 )

#define sys_mousebuttondown( mousebutton )       ( 0 != g_mousestate.rgbButtons[ mousebutton ] && 0 == g_oldmousestate.rgbButtons[ mousebutton ] )
#define sys_mousebuttonup( mousebutton )         ( 0 == g_mousestate.rgbButtons[ mousebutton ] && 0 != g_oldmousestate.rgbButtons[ mousebutton ] )
#define sys_mousebuttondownnow( mousebutton )    ( g_mousestate.rgbButtons[ mousebutton ] )

//#define sys_rgb555( r, g, b )       (WORD)(((b)&31) + (((g)&31) << 5) + (((r)&31) << 10))
//#define sys_rgb565( r, g, b )       (WORD)(((b)&31) + (((g)&63) << 5) + (((r)&31) << 11))

//--------------------------------------------------------------------------------
// globals
//--------------------------------------------------------------------------------

// windows
extern HINSTANCE g_hinst;
extern HWND g_hwnd;
extern char g_syserror[ 1024 ];
extern char g_apppath[ 300 ];

// direct draw
extern RECT g_drawrect;
extern LPDIRECTDRAW7 g_dd;
extern LPDIRECTDRAWSURFACE7 g_frontbuffer;
extern LPDIRECTDRAWSURFACE7 g_backbuffer;
extern LPDIRECTDRAWCLIPPER g_frontclipper;
extern LPDIRECTDRAWCLIPPER g_backclipper;
extern DDPIXELFORMAT g_pixelfmt;

// direct input
extern DIMOUSESTATE g_mousestate;
extern DIMOUSESTATE g_oldmousestate;
extern char g_keys[ 256 ];
extern char g_oldkeys[ 256 ];

// direct sound
extern LPDIRECTSOUND g_ds;

//--------------------------------------------------------------------------------
// public prototypes
//--------------------------------------------------------------------------------

// general
int sys_init( int fullscreen );
void sys_shutdown( void );

// direct draw
extern void (*sys_draw)( void );
//extern int (*sys_fadetocolor)( LPDIRECTDRAWSURFACE7 dds, int width, int height, int speed, unsigned char dstred, unsigned char dstgreen, unsigned char dstblue );
void sys_text( int fgcolor, int bgcolor, int x, int y, const char *string );
int sys_createsurface( const char *filename, LPDIRECTDRAWSURFACE7 *dds, int *bmpwidth );
int sys_createsurface( LPDIRECTDRAWSURFACE7 *dds, int width, int height, DWORD transcolor );
void sys_deletesurface( LPDIRECTDRAWSURFACE7 dds );
void sys_clearsurface( LPDIRECTDRAWSURFACE7 dds, DWORD color );
const char *sys_getdderr( HRESULT hres );
//void sys_screenshot( void );

// direct input
void sys_acquire( void );
void sys_unacquire( void );
void sys_begin( void );
void sys_end( void );
int sys_getalphanumerickey( char *buffer, int len );
const char *sys_getdierr( HRESULT hres );

// direct sound
int sys_createsound( const char *filename );
void sys_deletesound( int handle );
int sys_playsound( int handle, int pan );
void sys_stopsound( int handle );
void set_resetsoundpos( int handle );
void sys_setsoundvolume( int handle, int volume );
void sys_soundon( void );
void sys_soundoff( void );
int sys_issoundplaying( int handle );
const char *sys_getdserr( HRESULT hres );

// direct show
void sys_ongraphevent( void );
int sys_createmp3( const char *filename );
void sys_deletemp3( int handle );
void sys_playmp3( int handle );
void sys_stopmp3( int handle );
void sys_pausemp3( int handle );
void sys_setmp3volume( int handle, int volume );
void sys_mp3on( void );
void sys_mp3off( void );

// logging
int sys_logcreate( const char *filename );
int sys_logwrite( const char *string, ... );

// font
int font_create( const char *fontface, int size, int fontweight, int color );
void font_delete( int font );
int font_draw( int font, LPDIRECTDRAWSURFACE7 dds, const char *string, int x, int y );
int font_length( int font, const char *string );
int font_build( int font, LPDIRECTDRAWSURFACE7 *dds, int *width, const char *string, ... );

#endif