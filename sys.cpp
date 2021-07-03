#include "stdafx.h"
//--------------------------------------------------------------------------------
// includes
//--------------------------------------------------------------------------------
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <strmif.h>
#include <uuids.h>
#include <control.h>
#include <evcode.h>

#include "sys.h"

#pragma comment(linker,"\"/manifestdependency:type='win32' \
    name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
    processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

//--------------------------------------------------------------------------------
// defines
//--------------------------------------------------------------------------------

#define BITMAP_ID       0x4d42

#define MAX_CHARS       128
#define MAX_TRACKS      16

//--------------------------------------------------------------------------------
// structures
//--------------------------------------------------------------------------------
typedef struct bmp_s {
    BITMAPFILEHEADER    fileheader;
    BITMAPINFOHEADER    infoheader;
    PALETTEENTRY        pal[ 256 ];
    unsigned char *     buffer;
}
bmp_t;

typedef struct wav_s {
    WAVEFORMATEX fmt;
    int size;
    unsigned char *buffer;
}
wav_t;

typedef struct snd_s
{
    int crntsnd;
    LPDIRECTSOUNDBUFFER dsbs[ MAX_DUPSOUNDS ];
}
snd_t;

typedef struct fnt_s {
    LPDIRECTDRAWSURFACE7 dds;
    int totalwidth;
    int advancewidths[ MAX_CHARS ];
}
fnt_t;

typedef struct track_s {
    IGraphBuilder *graphbuilder;
    IMediaControl *mediacontrol;
    IMediaEventEx *mediaevent;
    IMediaPosition *mediaposition;
    IBasicAudio *basicaudio;
}
track_t;

//--------------------------------------------------------------------------------
// globals
//--------------------------------------------------------------------------------

// windows
HINSTANCE g_hinst;
HWND g_hwnd;
char g_apppath[ 300 ];
char g_syserror[ 1024 ];

// direct draw
RECT g_drawrect;
void (*sys_draw)( void );
//int (*sys_fadetocolor)( LPDIRECTDRAWSURFACE7 dds, int width, int height, int speed, unsigned char dstred, unsigned char dstgreen, unsigned char dstblue );
LPDIRECTDRAW7 g_dd;
LPDIRECTDRAWSURFACE7 g_frontbuffer;
LPDIRECTDRAWSURFACE7 g_backbuffer;
LPDIRECTDRAWCLIPPER g_frontclipper;
LPDIRECTDRAWCLIPPER g_backclipper;
DDPIXELFORMAT g_pixelfmt;
DDSURFACEDESC2 g_ddsd;
DDSCAPS2 g_ddscaps;

// direct input
DIMOUSESTATE g_mousestate;
DIMOUSESTATE g_oldmousestate;
char g_keys[ 256 ];
char g_oldkeys[ 256 ];
IDirectInput8* m_di;
IDirectInputDevice8* m_keyboard;
IDirectInputDevice8* m_mouse;
char m_keychars[ 256 ];

// direct sound
LPDIRECTSOUND g_ds;

//--------------------------------------------------------------------------------
// private data
//--------------------------------------------------------------------------------

// direct sound
static int m_issoundon;
static LPDIRECTSOUNDBUFFER m_primary;
static snd_t m_sounds[ MAX_SOUNDS ];

// font
static fnt_t m_fonts[ MAX_FONTS ];

// logging
static char m_filename[ 300 ];

// mp3
static int m_ismp3on;
static int m_currenttrack;
static track_t m_tracks[ MAX_TRACKS ];

//--------------------------------------------------------------------------------
// private prototypes
//--------------------------------------------------------------------------------
static int sys_ddinit( int fullscreen );
static int sys_ddinitfullscreen( void );
static int sys_ddinitwindowed( void );
static void sys_ddshutdown( void );

static int sys_diinit( void );
static void sys_dishutdown( void );
static char sys_keytochar( int key );
static void sys_buildkeychartable( void );

static int sys_dsinit( void );
static void sys_dsshutdown( void );

static int sys_createclipper( void );
static void sys_deleteclipper( void );

static int sys_bmpload( const char *filename, bmp_t *bmp );
static void sys_bmpunload( bmp_t *bmp );
static int sys_bmp24to32( bmp_t *srcbmp, bmp_t *dstbmp );
//static int sys_bmp24to16( bmp_t *srcbmp, bmp_t *dstbmp );
//static int sys_bmp16to24( bmp_t *srcbmp, bmp_t *dstbmp );
static int sys_bmpflip( bmp_t *bmp );
static int sys_bmpsave( const char *Filename, int Width, int Height, int BPP, unsigned char *Pixels );

static int sys_wavload( const char *filename, wav_t *wav );
static void sys_wavunload( wav_t *wav );

static void sys_drawfullscreen( void );
static void sys_drawwindowed( void );

//static int sys_fadetocolor555( LPDIRECTDRAWSURFACE7 dds, int width, int height, int speed, unsigned char dstred, unsigned char dstgreen, unsigned char dstblue );
//static int sys_fadetocolor565( LPDIRECTDRAWSURFACE7 dds, int width, int height, int speed, unsigned char dstred, unsigned char dstgreen, unsigned char dstblue );

const char *sys_getmmioerr( UINT err );

//--------------------------------------------------------------------------------
// public functions
//--------------------------------------------------------------------------------
int sys_init( int fullscreen )
{
    if( FAILED( CoInitialize( NULL ) ) ) {
        sys_logwrite( "sys_init: the call to CoInitialize() failed\n" );
        return 0;
    }

    sys_logwrite( "sys_init: starting dx\n" );

    if( !sys_ddinit( fullscreen ) ) {
        return 0;
    }

    if( !sys_diinit() ) {
        return 0;
    }

    if( !sys_dsinit() ) {
        sys_logwrite( "sys_init: no dsound support\n" );
        return 1;
    }

    m_ismp3on = 1;

    sys_logwrite( "sys_init: start done\n" );
    return 1;
}

void sys_shutdown( void )
{
    sys_logwrite( "sys_shutdown: stopping dx\n" );
    sys_dsshutdown();
    sys_dishutdown();
    sys_ddshutdown();
    sys_logwrite( "sys_shutdown: stop done\n" );

    ShowWindow( g_hwnd, SW_HIDE );

    CoUninitialize();
}

void sys_drawfullscreen( void )
{
    HRESULT hret;
    for( ; ; ) {
        hret = g_frontbuffer->Flip( NULL, 0L );
        if (hret == DD_OK )
            break;
        if (hret == DDERR_SURFACELOST )
        {
            hret = g_frontbuffer->Restore();
            if (hret != DD_OK )
                break;
        }
        if (hret != DDERR_WASSTILLDRAWING )
            break;
    }
}

void sys_drawwindowed( void )
{
    g_frontbuffer->Blt( &g_drawrect
                      , g_backbuffer
                      , NULL
                      , DDBLT_WAIT
                      , NULL );
}

void sys_text( int fgcolor, int bgcolor, int x, int y, const char *string )
{
    HDC h;

    if( !string || !g_backbuffer ) {
        return;
    }

    if( DD_OK != g_backbuffer->GetDC( &h ) ) {
        return;
    }

    SetBkMode( h, OPAQUE );
    SetBkColor( h, bgcolor );
    SetTextColor( h, fgcolor );

    TextOut( h, x, y, string, static_cast<int>(strlen( string )) );

    g_backbuffer->ReleaseDC( h );
}

void sys_acquire( void )
{
    HRESULT hres;

    if( m_keyboard ) {
        sys_logwrite( "sys_acquire: acquiring keyboard\n" );
        if( DI_OK != ( hres = m_keyboard->Acquire() ) ) {
            sys_logwrite( "sys_acquire: keyboard acquire failed [%s]\n", sys_getdierr( hres ) );
        }
    }
    if( m_mouse ) {
        sys_logwrite( "sys_acquire: acquiring mouse\n" );
        if( DI_OK != ( hres = m_mouse->Acquire() ) ) {
            sys_logwrite( "sys_acquire: mouse acquire failed [%s]\n", sys_getdierr( hres ) );
        }
    }
}

//int sys_fadetocolor555( LPDIRECTDRAWSURFACE7 dds
//                      , int width, int height, int speed
//                      , unsigned char dstred, unsigned char dstgreen, unsigned char dstblue )
//{
//    DDSURFACEDESC2 ddsd;
//    unsigned short *words;
//    unsigned short rawcolor;
//    unsigned char r, g, b;
//    int actualwidth;
//    int x, y;
//    int done;
//    int offset;
//    int absspeed;
//
//    memset( &ddsd, 0, sizeof( DDSURFACEDESC2 ) );
//    ddsd.dwSize = sizeof( DDSURFACEDESC2 );
//
//    if( FAILED( dds->Lock( NULL, &ddsd, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT, NULL ) ) ) {
//        return 0;
//    }
//
//    words       = (unsigned short *)ddsd.lpSurface;
//    actualwidth = ddsd.lPitch / 2;
//    absspeed    = abs( speed );
//    done        = 0;
//
//    for( y = 0; y < height; y++ ) {
//        for( x = 0; x < width; x++ ) {
//
//            // calc this only once per loop
//            offset = y * actualwidth + x;
//
//            rawcolor = words[ offset ];
//
//            r = sys_getred555( rawcolor );
//            g = sys_getgreen555( rawcolor );
//            b = sys_getblue555( rawcolor );
//
//            if( r >= dstred - absspeed && r <= dstred + absspeed ) {
//                r = dstred;
//            }
//            else {
//                r += speed;
//                done = 1;
//            }
//            if( g >= dstgreen - absspeed && g <= dstgreen + absspeed ) {
//                g = dstgreen;
//            }
//            else {
//                g += speed;
//                done = 1;
//            }
//            if( b >= dstblue - absspeed && b <= dstblue + absspeed ) {
//                b = dstblue;
//            }
//            else {
//                b += speed;
//                done = 1;
//            }
//
//            words[ offset ] = sys_rgb555( r, g, b );
//        }
//    }
//
//    if( FAILED( dds->Unlock( NULL ) ) ) {
//        return 0;
//    }
//
//    return done;
//}

//int sys_fadetocolor565( LPDIRECTDRAWSURFACE7 dds
//                      , int width, int height, int speed
//                      , unsigned char dstred, unsigned char dstgreen, unsigned char dstblue )
//{
//    DDSURFACEDESC2 ddsd;
//    unsigned short *words;
//    unsigned short rawcolor;
//    unsigned char r, g, b;
//    int actualwidth;
//    int x, y;
//    int done;
//    int offset;
//    int absspeed;
//
//    memset( &ddsd, 0, sizeof( DDSURFACEDESC2 ) );
//    ddsd.dwSize = sizeof( DDSURFACEDESC2 );
//
//    if( FAILED( dds->Lock( NULL, &ddsd, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT, NULL ) ) ) {
//        return 0;
//    }
//
//    words       = (unsigned short *)ddsd.lpSurface;
//    actualwidth = ddsd.lPitch / 2;
//    absspeed    = abs( speed );
//    done        = 0;
//
//    for( y = 0; y < height; y++ ) {
//        for( x = 0; x < width; x++ ) {
//
//            // calc this only once per loop
//            offset = y * actualwidth + x;
//
//            rawcolor = words[ offset ];
//
//            r = sys_getred565( rawcolor );
//            g = sys_getgreen565( rawcolor );
//            b = sys_getblue565( rawcolor );
//
//            if( r >= dstred - absspeed && r <= dstred + absspeed ) {
//                r = (unsigned char)dstred;
//            }
//            else {
//                r += (unsigned char)speed;
//                done = 1;
//            }
//            if( g >= dstgreen - absspeed && g <= dstgreen + absspeed ) {
//                g = (unsigned char)dstgreen;
//            }
//            else {
//                g += (unsigned char)speed;
//                done = 1;
//            }
//            if( b >= dstblue - absspeed && b <= dstblue + absspeed ) {
//                b = (unsigned char)dstblue;
//            }
//            else {
//                b += (unsigned char)speed;
//                done = 1;
//            }
//
//            words[ offset ] = sys_rgb565( r, g, b );
//        }
//    }
//
//    if( FAILED( dds->Unlock( NULL ) ) ) {
//        return 0;
//    }
//
//    return done;
//}

void sys_unacquire( void )
{
    HRESULT hres;

    if( m_keyboard ) {
        sys_logwrite( "sys_unacquire: unacquiring keyboard\n" );
        if( DI_OK != ( hres = m_keyboard->Unacquire() ) ) {
            sys_logwrite( "sys_acquire: keyboard acquire failed [%s]\n", sys_getdierr( hres ) );
        }
    }
    if( m_mouse ) {
        sys_logwrite( "sys_unacquire: unacquiring mouse\n" );
        if( DI_OK != ( hres = m_mouse->Unacquire() ) ) {
            sys_logwrite( "sys_acquire: mouse acquire failed [%s]\n", sys_getdierr( hres ) );
        }
    }
}

void sys_begin( void )
{
    if( DIERR_INPUTLOST == m_keyboard->GetDeviceState( sizeof( g_keys ), g_keys ) ) {
        m_keyboard->Acquire();
    }

    if( DIERR_INPUTLOST == m_mouse->GetDeviceState( sizeof( DIMOUSESTATE ), &g_mousestate ) ) {
        m_mouse->Acquire();
    }
}

void sys_end( void )
{
    memcpy( g_oldkeys, g_keys, sizeof( char ) * 256 );
    memcpy( &g_oldmousestate, &g_mousestate, sizeof( DIMOUSESTATE ) );
}

int sys_getalphanumerickey( char *buffer, int len )
{
    int i, keypressed;
    char str[ 2 ];

    if( (int)strlen( buffer ) >= len - 1 )  {
        return 0;
    }

    str[ 1 ]   = 0;
    keypressed = 0;

    for( i = 0; i < 256; i++ ) {
        if( sys_keydown( i ) ) {
            str[ 0 ] = sys_keytochar( i );
            if( str[ 0 ] ) {
                strcat_s( buffer, len, str );
            }
            keypressed = 1;
        }
    }

    return keypressed;
}

int sys_createsurface( const char *filename, LPDIRECTDRAWSURFACE7 *dds, int *bmpwidth )
{
    DDSURFACEDESC2 ddsd;
    DDCOLORKEY ckey;
    bmp_t srcbmp;
    bmp_t dstbmp;
    int y;
    DWORD color;
    unsigned char *src;
    unsigned char *dst;
    HRESULT hres;

    if( NULL == filename || NULL == dds || NULL == bmpwidth ) {
        strcpy_s( g_syserror, 1024, "sys_createsurface: was passed a null pointer" );
        return 0;
    }

    if( !sys_bmpload( filename, &srcbmp ) ) {
        return 0;
    }

    if( !sys_bmp24to32( &srcbmp, &dstbmp ) ) {
        return 0;
    }

    (*bmpwidth) = srcbmp.infoheader.biWidth;

    memset( &ddsd, 0, sizeof( DDSURFACEDESC2 ) );
    ddsd.dwSize         = sizeof( DDSURFACEDESC2 );
    ddsd.dwFlags        = DDSD_WIDTH | DDSD_HEIGHT | DDSD_CAPS;
    ddsd.dwWidth        = srcbmp.infoheader.biWidth;
    ddsd.dwHeight       = srcbmp.infoheader.biHeight;
    ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;

    if( FAILED( ( hres = g_dd->CreateSurface( &ddsd, dds, NULL ) ) ) ) {
        sprintf_s( g_syserror, 1024, "sys_createsurface: failed to create an offscreen surface [%s]", sys_getdderr( hres ) );
        return 0;
    }

    memset( &ddsd, 0, sizeof( DDSURFACEDESC2 ) );
    ddsd.dwSize = sizeof( DDSURFACEDESC2 );

    // grab the top left pixel, we'll use this as the transparent color
    color = RGB(srcbmp.buffer[0], srcbmp.buffer[1], srcbmp.buffer[2]);

    ckey.dwColorSpaceHighValue  = color;                   // color keys are always set, but they
    ckey.dwColorSpaceLowValue   = color;                   // won't be used unless the client sets
                                                           // a sprite as transparent

    if( FAILED( ( hres = (*dds)->SetColorKey( DDCKEY_SRCBLT, &ckey ) ) ) ) {
        sprintf_s( g_syserror, 1024, "sys_createsurface: failed to set the color key [%s]", sys_getdderr( hres ) );
        return 0;
    }

    if( FAILED( ( hres = (*dds)->Lock( NULL, &ddsd, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT, NULL ) ) ) ) {
        sprintf_s( g_syserror, 1024, "sys_createsurface: failed to lock the surface [%s]", sys_getdderr( hres ) );
        return 0;
    }

    src = dstbmp.buffer;
    dst = (unsigned char *)ddsd.lpSurface;

    for( y = 0; y < dstbmp.infoheader.biHeight; y++ ) {
        memcpy( dst, src, dstbmp.infoheader.biWidth * ( dstbmp.infoheader.biBitCount >> 3 ) );

        src += dstbmp.infoheader.biWidth * ( dstbmp.infoheader.biBitCount >> 3 );
        dst += ddsd.lPitch;
    }

    if( FAILED( ( hres = (*dds)->Unlock( NULL ) ) ) ) {
        sprintf_s( g_syserror, 1024, "sys_createsurface: failed to unlock the surface [%s]", sys_getdderr( hres ) );
        return 0;
    }

    sys_bmpunload( &srcbmp );
    sys_logwrite( "sys_createsurface: created a [%dx%d] surface from the file [%s]\n"
                , dstbmp.infoheader.biWidth, dstbmp.infoheader.biHeight, filename );
    return 1;
}

int sys_createsurface( LPDIRECTDRAWSURFACE7 *dds, int width, int height, DWORD transcolor )
{
    DDSURFACEDESC2 ddsd;
    DDCOLORKEY ckey;
    HRESULT hres;

    if( NULL == dds ) {
        strcpy_s( g_syserror, 1024, "sys_createsurface: was passed a null pointer" );
        return 0;
    }

    memset( &ddsd, 0, sizeof( DDSURFACEDESC2 ) );
    ddsd.dwSize         = sizeof( DDSURFACEDESC2 );
    ddsd.dwFlags        = DDSD_WIDTH | DDSD_HEIGHT | DDSD_CAPS;
    ddsd.dwWidth        = width;
    ddsd.dwHeight       = height;
    ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;

    if( FAILED( ( hres = g_dd->CreateSurface( &ddsd, dds, NULL ) ) ) ) {
        sprintf_s( g_syserror, 1024, "sys_createsurface: failed to create an offscreen surface[%s]", sys_getdderr( hres ) );
        return 0;
    }

    memset( &ddsd, 0, sizeof( DDSURFACEDESC2 ) );
    ddsd.dwSize = sizeof( DDSURFACEDESC2 );

    ckey.dwColorSpaceHighValue  = transcolor;              // color keys are always set, but they
    ckey.dwColorSpaceLowValue   = transcolor;              // won't be used unless the client sets
                                                           // a sprite as transparent

    if( FAILED( ( hres = (*dds)->SetColorKey( DDCKEY_SRCBLT, &ckey ) ) ) ) {
        sprintf_s( g_syserror, 1024, "sys_createsurface: failed to set the color key [%s]", sys_getdderr( hres ) );
        return 0;
    }

    sys_clearsurface( (*dds), 0 );

    sys_logwrite( "sys_createsurface: created a [%dx%d] surface\n", width, height );
    return 1;
}

void sys_deletesurface( LPDIRECTDRAWSURFACE7 dds )
{
    if( dds ) {
        sys_logwrite( "sys_deletesurface: released a surface\n" );
        dds->Release();
    }
}

void sys_clearsurface( LPDIRECTDRAWSURFACE7 dds, DWORD color )
{
    DDBLTFX ddbltfx;

    if( !dds ) {
        return;
    }

    memset( &ddbltfx, 0, sizeof( DDBLTFX ) );
    ddbltfx.dwSize = sizeof( DDBLTFX );
    ddbltfx.dwFillColor = color; 

    dds->Blt( NULL, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &ddbltfx );
}

int sys_logcreate( const char *filename )
{
    time_t now;
    struct tm tmstruct;
    char buf[ 128 ];
    FILE *file;
    errno_t Error;

    if( NULL == filename ) {
        return 0;
    }

    strcpy_s( m_filename, filename );

    Error = fopen_s( &file, m_filename, "wt" );
    if( Error != 0 ) {
        return 0;
    }

    // write the current date and time into a buffer
    time( &now );
    localtime_s( &tmstruct, &now );
    strftime( buf, 128, "%A, %d %B %Y, %I:%M:%S", &tmstruct );

    // write the buffer to the log file
    fprintf( file, "log file opened [%s]\n", buf );
    fclose( file );

    return 1;
}

int sys_logwrite( const char *string, ... )
{
    FILE *file;
    errno_t Error;
    va_list list;

    if( NULL == string || 0 == m_filename[ 0 ] ) {
        return 0;
    }

    Error = fopen_s( &file, m_filename, "at" );
    if( Error != 0 ) {
        return 0;
    }

    va_start( list, string );
    vfprintf( file, string, list );
    va_end( list );

    fclose( file );
    return 1;
}

int sys_createsound( const char *filename )
{
    int i, idx;
    wav_t wav;
    DSBUFFERDESC dsbd;
    unsigned char *audiobuf1;
    unsigned char *audiobuf2;
    unsigned long audiolen1;
    unsigned long audiolen2;
    HRESULT hres;

    if( !g_ds ) {
        strcpy_s( g_syserror, 1024, "sys_createsound: the main dsound object is not instantiated" );
        return 0;
    }

    if( NULL == filename ) {
        strcpy_s( g_syserror, 1024, "sys_createsound: was passed a null pointer" );
        return -1;
    }

    for( idx = 0; idx < MAX_SOUNDS; idx++ ) {
        if( NULL == m_sounds[ idx ].dsbs[ 0 ] ) {
            break;
        }
    }

    if( idx >= MAX_SOUNDS ) {
        strcpy_s( g_syserror, 1024, "sys_createsound: no free sound indexes" );
        return -1;
    }

    if( !sys_wavload( filename, &wav ) ) {
        return -1;
    }

    if( SOUND_BPS       != wav.fmt.wBitsPerSample ||
        SOUND_SPS       != (int)wav.fmt.nSamplesPerSec ||
        SOUND_CHANNELS  != wav.fmt.nChannels ) {
        sys_wavunload( &wav );
        sprintf_s( g_syserror, 1024, "sys_createsound: the sound [%s] is not in the required format of [%d %d %d]", filename, SOUND_BPS, SOUND_SPS, SOUND_CHANNELS );
        return -1;
    }

    // setup a buffer desc struct
    memset( (DSBUFFERDESC *)&dsbd, 0, sizeof( DSBUFFERDESC ) );

    dsbd.dwSize         = sizeof( DSBUFFERDESC );
    dsbd.dwFlags        = DSBCAPS_LOCSOFTWARE | DSBCAPS_STATIC |
                          DSBCAPS_CTRLFREQUENCY | DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME;
    dsbd.dwBufferBytes  = wav.size;
    dsbd.lpwfxFormat    = &wav.fmt;

    // create the buffer
    if( DS_OK != ( hres = g_ds->CreateSoundBuffer( &dsbd, &m_sounds[ idx ].dsbs[ 0 ], NULL ) ) ) {
        sprintf_s( g_syserror, 1024, "sys_createsound: failed to create a dsound buffer [%s]", sys_getdserr( hres ) );
        sys_wavunload( &wav );
        return -1;
    }

    audiobuf1 = audiobuf2 = NULL;
    audiolen1 = audiolen2 = 0;

    // write the bytes to the buffer
    if( DS_OK != ( hres = m_sounds[ idx ].dsbs[ 0 ]->Lock( 0
                                                , wav.size
                                                , (void **)&audiobuf1
                                                , &audiolen1
                                                , (void **)&audiobuf2
                                                , &audiolen2
                                                , DSBLOCK_FROMWRITECURSOR ) ) ) {
        m_sounds[ idx ].dsbs[ 0 ]->Release();
        sprintf_s( g_syserror, 1024, "sys_createsound: failed to lock the dsnd buffer [%s]", sys_getdserr( hres ) );
        sys_wavunload( &wav );
        return -1;
    }

    memcpy( audiobuf1, wav.buffer, audiolen1 );
    memcpy( audiobuf2, ( wav.buffer + audiolen1 ), audiolen2 );

    if( DS_OK != ( hres = m_sounds[ idx ].dsbs[ 0 ]->Unlock( audiobuf1, audiolen1, audiobuf2, audiolen2 ) ) ) {
        m_sounds[ idx ].dsbs[ 0 ]->Release();
        sprintf_s( g_syserror, 1024, "sys_createsound: failed to unlock the dsnd buffer [%s]", sys_getdserr( hres ) );
        sys_wavunload( &wav );
        return -1;
    }

    sys_wavunload( &wav );

    sys_logwrite( "sys_createsound: created a [%d,%d,%d] sound buffer from the file [%s]\n"
                , wav.fmt.wBitsPerSample, wav.fmt.nSamplesPerSec, wav.fmt.nChannels, filename );

    for( i = 1; i < MAX_DUPSOUNDS; i++ ) {
        hres = g_ds->DuplicateSoundBuffer( m_sounds[ idx ].dsbs[ 0 ], &m_sounds[ idx ].dsbs[ i ] );
        if( NULL == m_sounds[ idx ].dsbs[ i ] ) {
            sprintf_s( g_syserror, 1024, "sys_createsound: failed to duplicate snd buffer [%s]", sys_getdserr( hres ) );
            return -1;
        }
    }

    sys_logwrite( "sys_createsound: created [%d] duplicates of the sound [%s]\n", MAX_DUPSOUNDS, filename );
    return idx;
}

void sys_deletesound( int handle )
{
    int i, deleted;

    if( handle < 0 || handle >= MAX_SOUNDS ) {
        return;
    }

    deleted = 0;
    for( i = 1; i < MAX_DUPSOUNDS; i++ ) {
        if( m_sounds[ handle ].dsbs[ i ] ) {
            m_sounds[ handle ].dsbs[ i ]->Release();
            m_sounds[ handle ].dsbs[ i ] = NULL;
            deleted = 1;
        }
    }
    if( deleted ) {
        sys_logwrite( "sys_deletesound: released duplicated sound buffers\n" );
    }

    if( m_sounds[ handle ].dsbs[ 0 ] ) {
        m_sounds[ handle ].dsbs[ 0 ]->Release();
        m_sounds[ handle ].dsbs[ 0 ] = NULL;
        sys_logwrite( "sys_deletesound: released a sound buffer\n" );
    }
}

int sys_playsound( int handle, int pan )
{
    if( !g_ds || !m_issoundon ) {
        return 0;
    }

    if( handle < 0 || handle >= MAX_SOUNDS ) {
        return 0;
    }

    m_sounds[ handle ].dsbs[ m_sounds[ handle ].crntsnd ]->SetPan( (LONG)pan );
    m_sounds[ handle ].dsbs[ m_sounds[ handle ].crntsnd ]->Play( 0, 0, 0 );

    if( ++m_sounds[ handle ].crntsnd >= MAX_DUPSOUNDS ) {
        m_sounds[ handle ].crntsnd = 0;
    }

    return 1;
}

void sys_stopsound( int handle )
{
    int i;

    if( !g_ds ) {
        return;
    }

    if( handle < 0 || handle >= MAX_SOUNDS ) {
        return;
    }

    for( i = 0; i < MAX_DUPSOUNDS; i++ ) {
        m_sounds[ handle ].dsbs[ i ]->Stop();
    }
}

void set_resetsoundpos( int handle )
{
    int i;

    if( !g_ds ) {
        return;
    }

    if( handle < 0 || handle >= MAX_SOUNDS ) {
        return;
    }

    for( i = 0; i < MAX_DUPSOUNDS; i++ ) {
        m_sounds[ handle ].dsbs[ i ]->SetCurrentPosition( 0 );
    }
}

void sys_setsoundvolume( int handle, int volume )
{
    int i;

    if( !g_ds ) {
        return;
    }

    if( handle < 0 || handle >= MAX_SOUNDS ) {
        return;
    }

    for( i = 0; i < MAX_DUPSOUNDS; i++ ) {
        m_sounds[ handle ].dsbs[ i ]->SetVolume( volume );
    }
}

void sys_soundon( void )
{
    m_issoundon = 1;
}

void sys_soundoff( void )
{
    m_issoundon = 0;
}

int sys_issoundplaying( int handle )
{
    int i;
    DWORD Status;

    if( !g_ds ) {
        return 0;
    }

    if( handle < 0 || handle >= MAX_SOUNDS ) {
        return 0;
    }

    for( i = 0; i < MAX_DUPSOUNDS; i++ ) {
        m_sounds[ handle ].dsbs[ i ]->GetStatus( &Status );
        if( Status & DSBSTATUS_PLAYING ) {
            return 1;
        }
    }

    return 0;
}

int font_create( const char *fontface, int size, int fontweight, int color )
{
    int font;
    int i, x;
    char buf[ 8 ];
    HFONT hfont;
    HFONT holdfont;
    HDC hdc;

    for( font = 0; font < MAX_FONTS; font++ ) {
        if( NULL == m_fonts[ font ].dds ) {
            break;
        }
    }

    if( font >= MAX_FONTS ) {
        return -1;      // no free font indexes
    }

    if( !sys_createsurface( &m_fonts[ font ].dds, MAX_CHARSIZE * MAX_CHARS, MAX_CHARSIZE, 0 ) ) {
        return -1;
    }

    // get a dc to the surface
    if( DD_OK != m_fonts[ font ].dds->GetDC( &hdc ) ) {
        sprintf_s( g_syserror, 1024, "font_create: failed to get a dc to the font surface [%d]", GetLastError() );
        return -1;
    }

    if( NULL == fontface ) {
        // use the system font
        hfont = (HFONT)GetStockObject( ANSI_VAR_FONT );
        if( NULL == hfont ) {
            hfont = (HFONT)GetStockObject( SYSTEM_FONT );
            if( NULL == hfont ) {
                strcpy_s( g_syserror, 1024, "font_create: failed to get the system font" );
                return -1;
            }
        }
    }
    else {
        // create the font
        hfont = CreateFont( -MulDiv( size, GetDeviceCaps( hdc, LOGPIXELSY ), 72 )
                          , 0, 0, 0, fontweight, FALSE, FALSE, FALSE, ANSI_CHARSET
                          , OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS
                          , DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS
                          , fontface );
        if( NULL == hfont ) {
            m_fonts[ font ].dds->ReleaseDC( hdc );
            sprintf_s( g_syserror, 1024, "font_create: failed to create the font [%s] [%d], do you have this font?", fontface, GetLastError() );
            return -1;
        }
    }

    // select the font into the dc
    holdfont = (HFONT)SelectObject( hdc, hfont );
    if( NULL == holdfont ) {
        m_fonts[ font ].dds->ReleaseDC( hdc );
        sprintf_s( g_syserror, 1024, "font_create: failed to select the font [%s] into the device context [%d], do you have this font?", fontface, GetLastError() );
        return -1;
    }

    // get the advance widths of all the characters
    if( !GetCharWidth32( hdc, 0, MAX_CHARS - 1, m_fonts[ font ].advancewidths ) ) {
        if( !GetCharWidth( hdc, 0, MAX_CHARS - 1, m_fonts[ font ].advancewidths ) ) {
            SelectObject( hdc, holdfont );
            m_fonts[ font ].dds->ReleaseDC( hdc );
            DeleteObject( hfont );
            sprintf_s( g_syserror, 1024, "font_create: failed to get the advance widths for the font [%s] [%d]", fontface, GetLastError() );
            return -1;
        }
    }

    SetBkMode( hdc, OPAQUE );
    SetBkColor( hdc, 0 );
    SetTextColor( hdc, color );

    // blit all the chars to the surface
    for( x = 0, i = 0; i < MAX_CHARSIZE * MAX_CHARS; i++, x += MAX_CHARSIZE ) {
        sprintf_s( buf, 8, "%c", i );
        TextOut( hdc, x, 0, buf, 1 );
    }

    // set the members of the struct
    m_fonts[ font ].totalwidth = MAX_CHARSIZE * MAX_CHARS;

    // clean up
    SelectObject( hdc, holdfont );
    m_fonts[ font ].dds->ReleaseDC( hdc );
    DeleteObject( hfont );

    sys_logwrite( "font_create: created a font using font face [%s], point size [%d]\n", fontface, size );
    return font;
}

void font_delete( int font )
{
    if( font < 0 || font >= MAX_FONTS ) {
        return;
    }

    if( m_fonts[ font ].dds ) {
        sys_deletesurface( m_fonts[ font ].dds );
        m_fonts[ font ].dds = NULL;
        sys_logwrite( "font_delete: deleted a font\n" );
    }
}

int font_draw( int font, LPDIRECTDRAWSURFACE7 dds, const char *string, int x, int y )
{
    int i, len;
    RECT src, dst;

    if( font < 0 || font >= MAX_FONTS ||
        NULL == string || NULL == dds ) {
        return 0;
    }

    len = static_cast<int>(strlen( string ));

    src.left   = 0;
    src.right  = src.left + MAX_CHARDRAWSIZE;
    src.top    = 0;
    src.bottom = src.top  + MAX_CHARDRAWSIZE;
    dst.left   = x;
    dst.right  = dst.left + MAX_CHARDRAWSIZE;
    dst.top    = y;
    dst.bottom = dst.top  + MAX_CHARDRAWSIZE;

    for( i = 0; i < len; i++ ) {
        src.left   = MAX_CHARSIZE * string[ i ];
        src.right  = src.left + MAX_CHARDRAWSIZE;

        dds->Blt( &dst, m_fonts[ font ].dds, &src
                , DDBLT_KEYSRC | DDBLT_WAIT, NULL );

        dst.left  += m_fonts[ font ].advancewidths[ string[ i ] ];
        dst.right  = dst.left + MAX_CHARDRAWSIZE;
    }

    return 1;
}

int font_length( int font, const char *string )
{
    int i, len;
    int total;

    if( NULL == string || font < 0 || font >= MAX_FONTS ) {
        return -1;
    }

    len   = static_cast<int>(strlen( string ));
    total = 0;

    for( i = 0; i < len; i++ ) {
        total += m_fonts[ font ].advancewidths[ string[ i ] ];
    }

    return total;
}

int font_build( int font, LPDIRECTDRAWSURFACE7 *dds, int *width, const char *string, ... )
{
    int len;
    va_list list;
    char *newstring;

    if( font < 0 || font >= MAX_FONTS ||
        NULL == string || NULL == dds ) {
        return 0;
    }

    newstring = (char *)malloc( sizeof( char ) * strlen( string ) + 1 );
    if( NULL == newstring ) {
        strcpy_s( g_syserror, 1024, "font_build: failed to allocate memory" );
        return 0;
    }

    len = font_length( font, string );

    // round the length up to the nearest 32 byte boundary
    len += 32;
    //len &= 0xffffffe0;

    if( width ) {
        (*width) = len;
    }

    if( !sys_createsurface( dds, len, MAX_CHARSIZE, 0 ) ) {
        free( newstring );
        return 0;
    }

    va_start( list, string );
    vsprintf_s( newstring, strlen( string )+1, string, list );
    va_end( list );

    font_draw( font, (*dds), newstring, 0, 0 );

    free( newstring );
    sys_logwrite( "font_build: built a font surface from a string\n" );
    return 1;
}

const char *sys_getdderr( HRESULT hres )
{
    switch( hres ) {
        case DD_OK : return "The request completed successfully.";
        case DDERR_ALREADYINITIALIZED : return "The object has already been initialized.";
        case DDERR_BLTFASTCANTCLIP : return "A DirectDrawClipper object is attached to a source surface that has passed into a call to the IDirectDrawSurface7::BltFast method.";
        case DDERR_CANNOTATTACHSURFACE : return "A surface cannot be attached to another requested surface.";
        case DDERR_CANNOTDETACHSURFACE : return "A surface cannot be detached from another requested surface.";
        case DDERR_CANTCREATEDC : return "Windows cannot create any more device contexts (DCs), or a DC has requested a palette-indexed surface when the surface had no palette and the display mode was not palette-indexed (in this case DirectDraw cannot select a proper palette into the DC).";
        case DDERR_CANTDUPLICATE : return "Primary and 3-D surfaces, or surfaces that are implicitly created, cannot be duplicated.";
        case DDERR_CANTLOCKSURFACE : return "Access to this surface is refused because an attempt was made to lock the primary surface without DCI support.";
        case DDERR_CANTPAGELOCK : return "An attempt to page-lock a surface failed. Page lock does not work on a display-memory surface or an emulated primary surface.";
        case DDERR_CANTPAGEUNLOCK : return "An attempt to page-unlock a surface failed. Page unlock does not work on a display-memory surface or an emulated primary surface.";
        case DDERR_CLIPPERISUSINGHWND : return "An attempt was made to set a clip list for a DirectDrawClipper object that is already monitoring a window handle.";
        case DDERR_COLORKEYNOTSET : return "No source color key is specified for this operation.";
        case DDERR_CURRENTLYNOTAVAIL : return "No support is currently available.";
        case DDERR_DDSCAPSCOMPLEXREQUIRED : return "New for DirectX 7.0. The surface requires the DDSCAPS_COMPLEX flag.";
        case DDERR_DCALREADYCREATED : return "A device context (DC) has already been returned for this surface. Only one DC can be retrieved for each surface.";
        case DDERR_DEVICEDOESNTOWNSURFACE : return "Surfaces created by one DirectDraw device cannot be used directly by another DirectDraw device.";
        case DDERR_DIRECTDRAWALREADYCREATED : return "A DirectDraw object representing this driver has already been created for this process.";
        case DDERR_EXCEPTION : return "An exception was encountered while performing the requested operation.";
        case DDERR_EXCLUSIVEMODEALREADYSET : return "An attempt was made to set the cooperative level when it was already set to exclusive.";
        case DDERR_EXPIRED : return "The data has expired and is therefore no longer valid.";
        case DDERR_GENERIC : return "There is an undefined error condition.";
        case DDERR_HEIGHTALIGN : return "The height of the provided rectangle is not a multiple of the required alignment.";
        case DDERR_HWNDALREADYSET : return "The DirectDraw cooperative-level window handle has already been set. It cannot be reset while the process has surfaces or palettes created.";
        case DDERR_HWNDSUBCLASSED : return "DirectDraw is prevented from restoring state because the DirectDraw cooperative-level window handle has been subclassed.";
        case DDERR_IMPLICITLYCREATED : return "The surface cannot be restored because it is an implicitly created surface.";
        case DDERR_INCOMPATIBLEPRIMARY : return "The primary surface creation request does not match the existing primary surface.";
        case DDERR_INVALIDCAPS : return "One or more of the capability bits passed to the callback function are incorrect.";
        case DDERR_INVALIDCLIPLIST : return "DirectDraw does not support the provided clip list.";
        case DDERR_INVALIDDIRECTDRAWGUID : return "The globally unique identifier (GUID) passed to the DirectDrawCreate function is not a valid DirectDraw driver identifier.";
        case DDERR_INVALIDMODE : return "DirectDraw does not support the requested mode.";
        case DDERR_INVALIDOBJECT : return "DirectDraw received a pointer that was an invalid DirectDraw object.";
        case DDERR_INVALIDPARAMS : return "One or more of the parameters passed to the method are incorrect.";
        case DDERR_INVALIDPIXELFORMAT : return "The pixel format was invalid as specified.";
        case DDERR_INVALIDPOSITION : return "The position of the overlay on the destination is no longer legal.";
        case DDERR_INVALIDRECT : return "The provided rectangle was invalid.";
        case DDERR_INVALIDSTREAM : return "The specified stream contains invalid data.";
        case DDERR_INVALIDSURFACETYPE : return "The surface was of the wrong type.";
        case DDERR_LOCKEDSURFACES : return "One or more surfaces are locked, causing the failure of the requested operation.";
        case DDERR_MOREDATA : return "There is more data available than the specified buffer size can hold.";
        case DDERR_NEWMODE : return "New for DirectX 7.0. When IDirectDraw7::StartModeTest is called with the DDSMT_ISTESTREQUIRED flag, it may return this value to denote that some or all of the resolutions can and should be tested. IDirectDraw7::EvaluateMode returns this value to indicate that the test has switched to a new display mode.";
        case DDERR_NO3D : return "No 3-D hardware or emulation is present.";
        case DDERR_NOALPHAHW : return "No alpha-acceleration hardware is present or available, causing the failure of the requested operation.";
        case DDERR_NOBLTHW : return "No blitter hardware is present.";
        case DDERR_NOCLIPLIST : return "No clip list is available.";
        case DDERR_NOCLIPPERATTACHED : return "No DirectDrawClipper object is attached to the surface object.";
        case DDERR_NOCOLORCONVHW : return "No color-conversion hardware is present or available.";
        case DDERR_NOCOLORKEY : return "The surface does not currently have a color key.";
        case DDERR_NOCOLORKEYHW : return "There is no hardware support for the destination color key.";
        case DDERR_NOCOOPERATIVELEVELSET : return "A create function was called without the IDirectDraw7::SetCooperativeLevel method.";
        case DDERR_NODC : return "No device context (DC) has ever been created for this surface.";
        case DDERR_NODDROPSHW : return "No DirectDraw raster-operation (ROP) hardware is available.";
        case DDERR_NODIRECTDRAWHW : return "Hardware-only DirectDraw object creation is not possible; the driver does not support any hardware.";
        case DDERR_NODIRECTDRAWSUPPORT : return "DirectDraw support is not possible with the current display driver.";
        case DDERR_NODRIVERSUPPORT : return "New for DirectX 7.0. Testing cannot proceed because the display adapter driver does not enumerate refresh rates.";
        case DDERR_NOEMULATION : return "Software emulation is not available.";
        case DDERR_NOEXCLUSIVEMODE : return "The operation requires the application to have exclusive mode, but the application does not have exclusive mode.";
        case DDERR_NOFLIPHW : return "Flipping visible surfaces is not supported.";
        case DDERR_NOFOCUSWINDOW : return "An attempt was made to create or set a device window without first setting the focus window.";
        case DDERR_NOGDI : return "No GDI is present.";
        case DDERR_NOHWND : return "Clipper notification requires a window handle, or no window handle has been previously set as the cooperative level window handle.";
        case DDERR_NOMIPMAPHW : return "No mipmap-capable texture mapping hardware is present or available.";
        case DDERR_NOMIRRORHW : return "No mirroring hardware is present or available.";
        case DDERR_NOMONITORINFORMATION : return "New for DirectX 7.0. Testing cannot proceed because the monitor has no associated EDID data.";
        case DDERR_NONONLOCALVIDMEM : return "An attempt was made to allocate nonlocal video memory from a device that does not support nonlocal video memory.";
        case DDERR_NOOPTIMIZEHW : return "The device does not support optimized surfaces.";
        case DDERR_NOOVERLAYDEST : return "The IDirectDrawSurface7::GetOverlayPosition method is called on an overlay that the IDirectDrawSurface7::UpdateOverlay method has not been called on to establish as a destination.";
        case DDERR_NOOVERLAYHW : return "No overlay hardware is present or available.";
        case DDERR_NOPALETTEATTACHED : return "No palette object is attached to this surface.";
        case DDERR_NOPALETTEHW : return "There is no hardware support for 16- or 256-color palettes.";
        case DDERR_NORASTEROPHW : return "No appropriate raster-operation hardware is present or available.";
        case DDERR_NOROTATIONHW : return "No rotation hardware is present or available.";
        case DDERR_NOSTEREOHARDWARE : return "There is no stereo hardware present or available.";
        case DDERR_NOSTRETCHHW : return "There is no hardware support for stretching.";
        case DDERR_NOSURFACELEFT : return "There is no hardware present that supports stereo surfaces.";
        case DDERR_NOT4BITCOLOR : return "The DirectDrawSurface object is not using a 4-bit color palette, and the requested operation requires a 4-bit color palette.";
        case DDERR_NOT4BITCOLORINDEX : return "The DirectDrawSurface object is not using a 4-bit color index palette, and the requested operation requires a 4-bit color index palette.";
        case DDERR_NOT8BITCOLOR : return "The DirectDrawSurface object is not using an 8-bit color palette, and the requested operation requires an 8-bit color palette.";
        case DDERR_NOTAOVERLAYSURFACE : return "An overlay component is called for a nonoverlay surface.";
        case DDERR_NOTEXTUREHW : return "The operation cannot be carried out because no texture-mapping hardware is present or available.";
        case DDERR_NOTFLIPPABLE : return "An attempt was made to flip a surface that cannot be flipped.";
        case DDERR_NOTFOUND : return "The requested item was not found.";
        case DDERR_NOTINITIALIZED : return "An attempt was made to call an interface method of a DirectDraw object created by CoCreateInstance before the object was initialized.";
        case DDERR_NOTLOADED : return "The surface is an optimized surface, but it has not yet been allocated any memory.";
        case DDERR_NOTLOCKED : return "An attempt was made to unlock a surface that was not locked.";
        case DDERR_NOTPAGELOCKED : return "An attempt was made to page-unlock a surface with no outstanding page locks.";
        case DDERR_NOTPALETTIZED : return "The surface being used is not a palette-based surface.";
        case DDERR_NOVSYNCHW : return "There is no hardware support for vertical blank–synchronized operations.";
        case DDERR_NOZBUFFERHW : return "The operation to create a z-buffer in display memory or to perform a blit, using a z-buffer cannot be carried out because there is no hardware support for z-buffers.";
        case DDERR_NOZOVERLAYHW : return "The overlay surfaces cannot be z-layered, based on the z-order because the hardware does not support z-ordering of overlays.";
        case DDERR_OUTOFCAPS : return "The hardware needed for the requested operation has already been allocated.";
        case DDERR_OUTOFMEMORY : return "DirectDraw does not have enough memory to perform the operation.";
        case DDERR_OUTOFVIDEOMEMORY : return "DirectDraw does not have enough display memory to perform the operation.";
        case DDERR_OVERLAPPINGRECTS : return "The source and destination rectangles are on the same surface and overlap each other.";
        case DDERR_OVERLAYCANTCLIP : return "The hardware does not support clipped overlays.";
        case DDERR_OVERLAYCOLORKEYONLYONEACTIVE : return "An attempt was made to have more than one color key active on an overlay.";
        case DDERR_OVERLAYNOTVISIBLE : return "The IDirectDrawSurface7::GetOverlayPosition method was called on a hidden overlay.";
        case DDERR_PALETTEBUSY : return "Access to this palette is refused because the palette is locked by another thread.";
        case DDERR_PRIMARYSURFACEALREADYEXISTS : return "This process has already created a primary surface.";
        case DDERR_REGIONTOOSMALL : return "The region passed to the IDirectDrawClipper::GetClipList method is too small.";
        case DDERR_SURFACEALREADYATTACHED : return "An attempt was made to attach a surface to another surface to which it is already attached.";
        case DDERR_SURFACEALREADYDEPENDENT : return "An attempt was made to make a surface a dependency of another surface on which it is already dependent.";
        case DDERR_SURFACEBUSY : return "Access to the surface is refused because the surface is locked by another thread.";
        case DDERR_SURFACEISOBSCURED : return "Access to the surface is refused because the surface is obscured.";
        case DDERR_SURFACELOST : return "Access to the surface is refused because the surface memory is gone. Call the IDirectDrawSurface7::Restore method on this surface to restore the memory associated with it.";
        case DDERR_SURFACENOTATTACHED : return "The requested surface is not attached.";
        case DDERR_TESTFINISHED : return "New for DirectX 7.0. When returned by the IDirectDraw7::StartModeTest method, this value means that no test could be initiated because all the resolutions chosen for testing already have refresh rate information in the registry. When returned by IDirectDraw7::EvaluateMode, the value means that DirectDraw has completed a refresh rate test.";
        case DDERR_TOOBIGHEIGHT : return "The height requested by DirectDraw is too large.";
        case DDERR_TOOBIGSIZE : return "The size requested by DirectDraw is too large. However, the individual height and width are valid sizes.";
        case DDERR_TOOBIGWIDTH : return "The width requested by DirectDraw is too large.";
        case DDERR_UNSUPPORTED : return "The operation is not supported.";
        case DDERR_UNSUPPORTEDFORMAT : return "The pixel format requested is not supported by DirectDraw.";
        case DDERR_UNSUPPORTEDMASK : return "The bitmask in the pixel format requested is not supported by DirectDraw.";
        case DDERR_UNSUPPORTEDMODE : return "The display is currently in an unsupported mode.";
        case DDERR_VERTICALBLANKINPROGRESS : return "A vertical blank is in progress.";
        case DDERR_VIDEONOTACTIVE : return "The video port is not active.";
        case DDERR_WASSTILLDRAWING : return "The previous blit operation that is transferring information to or from this surface is incomplete.";
        case DDERR_WRONGMODE : return "This surface cannot be restored because it was created in a different mode.";
        case DDERR_XALIGN : return "The provided rectangle was not horizontally aligned on a required boundary.";
    }
    return "not found";
}

const char *sys_getdserr( HRESULT hres )
{
    switch( hres ) {
        case DS_OK : return "The request completed successfully.";
        case DSERR_ACCESSDENIED : return "The request failed because access was denied.";
        case DSERR_ALLOCATED : return "The request failed because resources, such as a priority level, were already in use by another caller.";
        case DSERR_ALREADYINITIALIZED : return "The object is already initialized.";
        case DSERR_BADFORMAT : return "The specified wave format is not supported.";
        case DSERR_BUFFERLOST : return "The buffer memory has been lost and must be restored.";
        case DSERR_CONTROLUNAVAIL : return "The buffer control (volume, pan, and so on) requested by the caller is not available.";
        case DSERR_GENERIC : return "An undetermined error occurred inside the DirectSound subsystem.";
        //case DSERR_HWUNAVAIL : return "The DirectSound hardware device is unavailable.";
        case DSERR_INVALIDCALL : return "This function is not valid for the current state of this object.";
        case DSERR_INVALIDPARAM : return "An invalid parameter was passed to the returning function.";
        case DSERR_NOAGGREGATION : return "The object does not support aggregation.";
        case DSERR_NODRIVER : return "No sound driver is available for use.";
        case DSERR_NOINTERFACE : return "The requested COM interface is not available.";
        case DSERR_OTHERAPPHASPRIO : return "Another application has a higher priority level, preventing this call from succeeding";
        case DSERR_OUTOFMEMORY : return "The DirectSound subsystem could not allocate sufficient memory to complete the caller's request.";
        case DSERR_PRIOLEVELNEEDED : return "The caller does not have the priority level required for the function to succeed.";
        case DSERR_UNINITIALIZED : return "The IDirectSound::Initialize method has not been called or has not been called successfully before other methods were called.";
        case DSERR_UNSUPPORTED : return "The function called is not supported at this time.";
    }
    return "not found";
}

const char *sys_getdierr( HRESULT hres )
{
    switch( hres ) {
        case DI_BUFFEROVERFLOW : return "The device buffer overflowed and some input was lost. This value is equal to the S_FALSE standard COM return value.";
        case DI_DOWNLOADSKIPPED : return "The parameters of the effect were successfully updated, but the effect could not be downloaded because the associated device was not acquired in exclusive mode.";
        case DI_EFFECTRESTARTED : return "The effect was stopped, the parameters were updated, and the effect was restarted.";
        //case DI_NOEFFECT : return "The operation had no effect. This value is equal to the S_FALSE standard COM return value.";
        //case DI_NOTATTACHED : return "The device exists but is not currently attached. This value is equal to the S_FALSE standard COM return value.";
        case DI_OK : return "The operation completed successfully. This value is equal to the S_OK standard COM return value.";
        case DI_POLLEDDEVICE : return "The device is a polled device. As a result, device buffering does not collect any data and event notifications is not signaled until the IDirectInputDevice8::Poll method is called.";
        //case DI_PROPNOEFFECT : return "The change in device properties had no effect. This value is equal to the S_FALSE standard COM return value.";
        case DI_TRUNCATED : return "The parameters of the effect were successfully updated, but some of them were beyond the capabilities of the device and were truncated to the nearest supported value.";
        case DI_TRUNCATEDANDRESTARTED : return "Equal to DI_EFFECTRESTARTED | DI_TRUNCATED.";
        case DIERR_ACQUIRED : return "The operation cannot be performed while the device is acquired.";
        case DIERR_ALREADYINITIALIZED : return "This object is already initialized";
        case DIERR_BADDRIVERVER : return "The object could not be created due to an incompatible driver version or mismatched or incomplete driver components.";
        case DIERR_BETADIRECTINPUTVERSION : return "The application was written for an unsupported prerelease version of DirectInput.";
        case DIERR_DEVICEFULL : return "The device is full.";
        case DIERR_DEVICENOTREG : return "The device or device instance is not registered with DirectInput. This value is equal to the REGDB_E_CLASSNOTREG standard COM return value.";
        case DIERR_EFFECTPLAYING : return "The parameters were updated in memory but were not downloaded to the device because the device does not support updating an effect while it is still playing.";
        case DIERR_HASEFFECTS : return "The device cannot be reinitialized because there are still effects attached to it.";
        case DIERR_GENERIC : return "An undetermined error occurred inside the DirectInput subsystem. This value is equal to the E_FAIL standard COM return value.";
        case DIERR_HANDLEEXISTS : return "The device already has an event notification associated with it. This value is equal to the E_ACCESSDENIED standard COM return value.";
        case DIERR_INCOMPLETEEFFECT : return "The effect could not be downloaded because essential information is missing. For example, no axes have been associated with the effect, or no type-specific information has been supplied.";
        case DIERR_INPUTLOST : return "Access to the input device has been lost. It must be reacquired.";
        case DIERR_INVALIDPARAM : return "An invalid parameter was passed to the returning function, or the object was not in a state that permitted the function to be called. This value is equal to the E_INVALIDARG standard COM return value.";
        case DIERR_MOREDATA : return "Not all the requested information fit into the buffer.";
        case DIERR_NOAGGREGATION : return "This object does not support aggregation.";
        case DIERR_NOINTERFACE : return "The specified interface is not supported by the object. This value is equal to the E_NOINTERFACE standard COM return value.";
        case DIERR_NOTACQUIRED : return "The operation cannot be performed unless the device is acquired.";
        case DIERR_NOTBUFFERED : return "The device is not buffered. Set the DIPROP_BUFFERSIZE property to enable buffering.";
        case DIERR_NOTDOWNLOADED : return "The effect is not downloaded.";
        case DIERR_NOTEXCLUSIVEACQUIRED : return "The operation cannot be performed unless the device is acquired in DISCL_EXCLUSIVE mode.";
        case DIERR_NOTFOUND : return "The requested object does not exist.";
        case DIERR_NOTINITIALIZED : return "This object has not been initialized.";
        //case DIERR_OBJECTNOTFOUND : return "The requested object does not exist.";
        case DIERR_OLDDIRECTINPUTVERSION : return "The application requires a newer version of DirectInput.";
        //case DIERR_OTHERAPPHASPRIO : return "Another application has a higher priority level, preventing this call from succeeding. This value is equal to the E_ACCESSDENIED standard COM return value. This error can be returned when an application has only foreground access to a device but is attempting to acquire the device while in the background.";
        case DIERR_OUTOFMEMORY : return "The DirectInput subsystem could not allocate sufficient memory to complete the call. This value is equal to the E_OUTOFMEMORY standard COM return value.";
        //case DIERR_READONLY : return "The specified property cannot be changed. This value is equal to the E_ACCESSDENIED standard COM return value.";
        case DIERR_REPORTFULL : return "More information was requested to be sent than can be sent to the device.";
        case DIERR_UNPLUGGED : return "The operation could not be completed because the device is not plugged in.";
        case DIERR_UNSUPPORTED : return "The function called is not supported at this time. This value is equal to the E_NOTIMPL standard COM return value.";
        case E_HANDLE : return "The HWND parameter is not a valid top-level window that belongs to the process.";
        case E_PENDING : return "Data is not yet available.";
    }
    return "not found";
}

int sys_createmp3( const char *filename )
{
    int i;
    WCHAR wfilename[ 300 ];

    if( NULL == filename ) {
        strcpy_s( g_syserror, 1024, "sys_createmp3: was passed a null pointer" );
        return -1;
    }

    for( i = 0; i < MAX_TRACKS; i++ ) {
        if( NULL == m_tracks[ i ].graphbuilder ) {
            break;
        }
    }

    if( i >= MAX_TRACKS ) {
        strcpy_s( g_syserror, 1024, "sys_createmp3: no free handles" );
        return -1;
    }

    sys_logwrite( "sys_createmp3: creating music track\n" );

    // create the objects
    if( FAILED( CoCreateInstance( CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER
                                , IID_IGraphBuilder, (void**)&m_tracks[ i ].graphbuilder ) ) ) {
        sprintf_s( g_syserror, 1024, "sys_createmp3: failed to create the graph builder object" );
        m_tracks[ i ].graphbuilder = NULL;
        return -1;
    }

    m_tracks[ i ].graphbuilder->QueryInterface( IID_IMediaControl,  (void **)&m_tracks[ i ].mediacontrol );
    m_tracks[ i ].graphbuilder->QueryInterface( IID_IMediaEvent,    (void **)&m_tracks[ i ].mediaevent );
    m_tracks[ i ].graphbuilder->QueryInterface( IID_IMediaPosition, (void **)&m_tracks[ i ].mediaposition );
    m_tracks[ i ].graphbuilder->QueryInterface( IID_IBasicAudio,    (void **)&m_tracks[ i ].basicaudio );

    m_tracks[ i ].mediaevent->SetNotifyWindow( (OAHWND)g_hwnd, WM_GRAPHEVENT, 0 );
    m_tracks[ i ].mediaevent->SetNotifyFlags( 0 );      // turn on notifications

    // load the file
    MultiByteToWideChar( CP_ACP, 0, filename, -1, wfilename, static_cast<int>(strlen( filename ) + 1) );
    wfilename[ strlen( filename ) ] = 0;

    if( FAILED( m_tracks[ i ].graphbuilder->RenderFile( wfilename, NULL ) ) ) {
        sprintf_s( g_syserror, 1024, "sys_createmp3: failed to create the mp3 [%s]", filename );
        sys_logwrite( "sys_createmp3: failed to create the mp3 [%s]\n", filename );
        return -1;
    }

    m_tracks[ i ].mediaposition->put_CurrentPosition( 0 );

    sys_logwrite( "sys_createmp3: create done\n" );

    return i;
}

void sys_deletemp3( int handle )
{
    if( handle < 0 || handle >= MAX_TRACKS ) {
        return;
    }

    if( m_tracks[ handle ].graphbuilder ) {
        sys_logwrite( "sys_deletemp3: deleting music track\n" );

        if( m_tracks[ handle ].basicaudio ) {
            m_tracks[ handle ].basicaudio->Release();
            m_tracks[ handle ].basicaudio = NULL;
        }

        if( m_tracks[ handle ].mediacontrol ) {
            m_tracks[ handle ].mediacontrol->Stop();
            m_tracks[ handle ].mediacontrol->Release();
            m_tracks[ handle ].mediacontrol = NULL;
        }

        if( m_tracks[ handle ].mediaevent ) {
            m_tracks[ handle ].mediaevent->Release();
            m_tracks[ handle ].mediaevent = NULL;
        }

        if( m_tracks[ handle ].mediaposition ) {
            m_tracks[ handle ].mediaposition->Release();
            m_tracks[ handle ].mediaposition = NULL;
        }

        m_tracks[ handle ].graphbuilder->Release();
        m_tracks[ handle ].graphbuilder = NULL;

        sys_logwrite( "sys_deletemp3: delete done\n" );
    }
}

void sys_playmp3( int handle )
{
    if( -1 == handle && m_ismp3on ) {
        if( m_currenttrack >= 0 && m_currenttrack < MAX_TRACKS ) {
            if( m_tracks[ m_currenttrack ].mediacontrol ) {
                sys_logwrite( "sys_playmp3() playing current track [%d]\n", m_currenttrack );
                m_tracks[ m_currenttrack ].mediacontrol->Run();
            }
            return;
        }
    }

    if( handle < 0 || handle >= MAX_TRACKS ) {
        return;
    }

    if( !m_ismp3on ) {
        return;
    }

    m_tracks[ handle ].mediacontrol->Run();
    m_currenttrack = handle;
}

void sys_stopmp3( int handle )
{
    if( handle < 0 || handle >= MAX_TRACKS ) {
        return;
    }

    m_tracks[ handle ].mediacontrol->Stop();
    m_tracks[ handle ].mediaposition->put_CurrentPosition( 0 );
}

void sys_pausemp3( int handle )
{
    if( -1 == handle && m_ismp3on ) {
        if( m_currenttrack >= 0 && m_currenttrack < MAX_TRACKS ) {
            if( m_tracks[ m_currenttrack ].mediacontrol ) {
                sys_logwrite( "sys_pausemp3() pausing current track [%d]\n", m_currenttrack );
                m_tracks[ m_currenttrack ].mediacontrol->Pause();
            }
            return;
        }
    }

    if( handle < 0 || handle >= MAX_TRACKS ) {
        return;
    }

    m_tracks[ handle ].mediacontrol->Pause();
}

void sys_setmp3volume( int handle, int volume )
{
    if( handle < 0 || handle >= MAX_TRACKS ) {
        return;
    }

    sys_logwrite( "sys_setmp3volume() setting mp3 [%d] volume to [%d]\n", handle, volume );

    if( FAILED( m_tracks[ handle ].basicaudio->put_Volume( volume ) ) ) {
        sys_logwrite( "sys_setmp3volume() put_Volume() returned failure\n" );
    }
}

void sys_mp3on( void )
{
    m_ismp3on = 1;
}

void sys_mp3off( void )
{
    m_ismp3on = 0;
}

void sys_ongraphevent( void )
{
    long eventcode;
    LONG_PTR param1;
    LONG_PTR param2;

    while( E_ABORT != m_tracks[ m_currenttrack ].mediaevent->GetEvent( &eventcode, &param1, &param2, 0 ) ) {
        switch( eventcode ) {
            case EC_COMPLETE:            
                // here when media is completely done playing
                m_tracks[ m_currenttrack ].mediaposition->put_CurrentPosition( 0 );
                break;

            default:
                break;
        }	
        m_tracks[ m_currenttrack ].mediaevent->FreeEventParams( eventcode, param1, param2 );
    }
}

//--------------------------------------------------------------------------------
// private functions
//--------------------------------------------------------------------------------
static int sys_ddinit( int fullscreen )
{
    HRESULT hres;

    hres = DirectDrawCreateEx( NULL, (void **)&g_dd, IID_IDirectDraw7, NULL );
    if( FAILED( hres ) ) {
        sprintf_s( g_syserror, 1024, "sys_ddinit: failed to create the main ddraw object [%s]", sys_getdderr( hres ) );
        return 0;
    }

    if( fullscreen ) {
        if( !sys_ddinitfullscreen() ) {
            return 0;
        }
    }
    else {
        if( !sys_ddinitwindowed() ) { 
            return 0;
        }
    }

    //sys_fadetocolor = ( 0x03E0 == g_pixelfmt.dwGBitMask ? sys_fadetocolor555 : sys_fadetocolor565 );

    return 1;
}

static int sys_ddinitfullscreen( void )
{
    HRESULT hres;

    sys_logwrite( "sys_ddinitfullscreen: starting ddraw fullscreen mode\n" );

    if( FAILED( ( hres = g_dd->SetCooperativeLevel( g_hwnd, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN | DDSCL_ALLOWREBOOT ) ) ) ) {
        sprintf_s( g_syserror, 1024, "sys_ddinitfullscreen: failed to set the co-op level [%s]", sys_getdderr( hres ) );
        return 0;
    }

    if( FAILED( ( hres = g_dd->SetDisplayMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, 0, 0 ) ) ) ) {
        sprintf_s( g_syserror, 1024, "sys_ddinitfullscreen: failed to set the display mode [%dx%d], [%d] bit color, [%s]", SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, sys_getdderr( hres ) );
        return 0;
    }

    sys_logwrite( "sys_ddinitfullscreen: display mode [%dx%d], [%d] bit color\n", SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP );

    memset( &g_ddsd, 0, sizeof( DDSURFACEDESC2 ) );
    g_ddsd.dwSize             = sizeof( DDSURFACEDESC2 );
    g_ddsd.dwFlags            = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
    g_ddsd.ddsCaps.dwCaps     = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX;
    g_ddsd.dwBackBufferCount  = 1;

    if( FAILED( ( hres = g_dd->CreateSurface( &g_ddsd, &g_frontbuffer, NULL ) ) ) ) {
        sprintf_s( g_syserror, 1024, "sys_ddinitfullscreen: failed to create the front buffer [%s]", sys_getdderr( hres ) );
        return 0;
    }

    memset( &g_ddscaps, 0, sizeof( DDSCAPS2 ) );
    g_ddscaps.dwCaps = DDSCAPS_BACKBUFFER;

    if( FAILED( ( hres = g_frontbuffer->GetAttachedSurface( &g_ddscaps, &g_backbuffer ) ) ) ) {
        sprintf_s( g_syserror, 1024, "sys_ddinit: failed to get the back buffer [%s]", sys_getdderr( hres ) );
        return 0;
    }

    if( !sys_createclipper() ) {
        return 0;
    }

    memset( &g_pixelfmt, 0, sizeof( DDPIXELFORMAT ) );
    g_pixelfmt.dwSize = sizeof( DDPIXELFORMAT );
    g_frontbuffer->GetPixelFormat( &g_pixelfmt );

    GetClientRect(g_hwnd, &g_drawrect);
    ClipCursor(&g_drawrect);

    sys_logwrite( "sys_ddinitfullscreen: rgb bit count [%d]\n", g_pixelfmt.dwRGBBitCount );
    sys_logwrite( "sys_ddinitfullscreen: red mask [0x%08x], green mask [0x%08x], blue mask [0x%08x]\n", g_pixelfmt.dwRBitMask, g_pixelfmt.dwGBitMask, g_pixelfmt.dwBBitMask );

    sys_draw = sys_drawfullscreen;

    sys_logwrite( "sys_ddinitfullscreen: start done\n" );
    return 1;
}

static int sys_ddinitwindowed( void )
{
    HRESULT hres;

    sys_logwrite( "sys_ddinitwindowed: starting ddraw windowed mode\n" );

    if( FAILED( ( hres = g_dd->SetCooperativeLevel( g_hwnd, DDSCL_NORMAL ) ) ) ) {
        sprintf_s( g_syserror, 1024, "sys_ddinitwindowed: failed to set the co-op level [%s]", sys_getdderr( hres ) );
        return 0;
    }

    GetClientRect( g_hwnd, &g_drawrect );
    ClipCursor( &g_drawrect );
    ClientToScreen( g_hwnd, (POINT *)&g_drawrect.left );
    ClientToScreen( g_hwnd, (POINT *)&g_drawrect.right );

    memset( &g_ddsd, 0, sizeof( DDSURFACEDESC2 ) );
    g_ddsd.dwSize         = sizeof( DDSURFACEDESC2 );
    g_ddsd.dwFlags        = DDSD_CAPS;
    g_ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

    if( FAILED( ( hres = g_dd->CreateSurface( &g_ddsd, &g_frontbuffer, NULL ) ) ) ) {
        sprintf_s( g_syserror, 1024, "sys_ddinitwindowed: failed create the front buffer [%s]", sys_getdderr( hres ) );
        return 0;
    }

    memset( &g_ddsd, 0, sizeof( DDSURFACEDESC2 ) );
    g_ddsd.dwSize         = sizeof( DDSURFACEDESC2 );
    g_ddsd.dwFlags        = DDSD_WIDTH | DDSD_HEIGHT | DDSD_CAPS;
    g_ddsd.dwWidth        = SCREEN_WIDTH;
    g_ddsd.dwHeight       = SCREEN_HEIGHT;
    g_ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_VIDEOMEMORY;

    if( FAILED( ( hres = g_dd->CreateSurface( &g_ddsd, &g_backbuffer, NULL ) ) ) ) {
        g_ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
        if( FAILED( ( hres = g_dd->CreateSurface( &g_ddsd, &g_backbuffer, NULL ) ) ) ) {
            sprintf_s( g_syserror, 1024, "sys_ddinitwindowed: failed to create the back buffer [%s]", sys_getdderr( hres ) );
            return 0;
        }
    }

    if( FAILED( ( hres = g_dd->CreateClipper( 0, &g_frontclipper, NULL ) ) ) ) {
        sprintf_s( g_syserror, 1024, "sys_ddinitwindowed: failed to create the front clipper [%s]", sys_getdderr( hres ) );
        return 0;
    }

    if( !sys_createclipper() ) {
        return 0;
    }

    // associate the clipper with the window
    if( FAILED( hres = g_frontclipper->SetHWnd( 0, g_hwnd ) ) ) {
        sprintf_s( g_syserror, 1024, "sys_ddinitwindowed: failed to set the clipper on the hwnd [%s]", sys_getdderr( hres ) );
        return 0;
    }

    if( FAILED( hres = g_frontbuffer->SetClipper( g_frontclipper ) ) ) {
        sprintf_s( g_syserror, 1024, "sys_ddinitwindowed: failed to set the clipper on the front buffer [%s]", sys_getdderr( hres ) );
        return 0;
    }

    if( FAILED( hres = g_backbuffer->SetClipper( g_backclipper ) ) ) {
        sprintf_s( g_syserror, 1024, "sys_ddinitwindowed: failed to set the clipper on the back buffer [%s]", sys_getdderr( hres ) );
        return 0;
    }

    g_frontclipper->Release();
    g_frontclipper = NULL;

    memset( &g_pixelfmt, 0, sizeof( DDPIXELFORMAT ) );
    g_pixelfmt.dwSize = sizeof( DDPIXELFORMAT );
    g_frontbuffer->GetPixelFormat( &g_pixelfmt );

    sys_draw = sys_drawwindowed;

    sys_logwrite( "sys_ddinitwindowed: start done\n" );
    return 1;
}

static void sys_ddshutdown( void )
{
    if( g_dd ) {
        sys_logwrite( "sys_ddshutdown: stopping ddraw\n" );
        sys_deleteclipper();

        if( g_backbuffer ) {
            g_backbuffer->Release();
            g_backbuffer = NULL;
        }

        if( g_frontbuffer ) {
            g_frontbuffer->Release();
            g_frontbuffer = NULL;
        }

        g_dd->Release();
        g_dd = NULL;
        sys_logwrite( "sys_ddshutdown: stop done\n" );
    }
}

static int sys_diinit( void )
{
    HRESULT hres;

    sys_logwrite( "sys_diinit: starting dinput\n" );

    sys_buildkeychartable();

    hres = CoCreateInstance(CLSID_DirectInput8, NULL, CLSCTX_INPROC, IID_IDirectInput8, reinterpret_cast<void**>(&m_di));
    if(FAILED(hres)) {
        sprintf_s( g_syserror, 1024, "sys_diinit: failed to create a dinput object [%s]", sys_getdierr( hres ) );
        return 0;
    }

    hres = m_di->Initialize(g_hinst, DIRECTINPUT_VERSION);
    if(FAILED(hres)) {
        sprintf_s( g_syserror, 1024, "sys_diinit: failed to create a dinput object [%s]", sys_getdierr( hres ) );
        return 0;
    }

    if( FAILED( hres = m_di->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL))) {
        sprintf_s( g_syserror, 1024, "sys_diinit: failed to create the keyboard device [%s]", sys_getdierr( hres ) );
        return 0;
    }

    if( FAILED( hres = m_keyboard->SetDataFormat( &c_dfDIKeyboard ) ) ) {
        sprintf_s( g_syserror, 1024, "sys_diinit: failed to set the keyboard data format [%s]", sys_getdierr( hres ) );
        return 0;
    }
    sys_logwrite( "sys_diinit: captured the keyboard\n" );

    if( FAILED( hres = m_keyboard->SetCooperativeLevel( g_hwnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND ) ) ) {
        sprintf_s( g_syserror, 1024, "sys_diinit: failed to set the keyboard co-op level [%s]", sys_getdierr( hres ) );
        return 0;
    }

    if( FAILED( hres = m_di->CreateDevice( GUID_SysMouse, &m_mouse, NULL ) ) ) {
        sprintf_s( g_syserror, 1024, "sys_diinit: failed to create the mouse device [%s]", sys_getdierr( hres ) );
        return 0;
    }

    if( FAILED( hres = m_mouse->SetDataFormat( &c_dfDIMouse ) ) ) {
        sprintf_s( g_syserror, 1024, "sys_diinit: failed to set the mouse data format [%s]", sys_getdierr( hres ) );
        return 0;
    }

    if( FAILED( hres = m_mouse->SetCooperativeLevel( g_hwnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND ) ) ) {
        sprintf_s( g_syserror, 1024, "sys_diinit: failed to set the mouse co-op level [%s]", sys_getdierr( hres ) );
        return 0;
    }
    sys_logwrite( "sys_diinit: captured the mouse\n" );

    memset( &g_mousestate, 0, sizeof( DIMOUSESTATE ) );
    memset( &g_oldmousestate, 0, sizeof( DIMOUSESTATE ) );
    memset( g_keys, 0, sizeof( char ) * 256 );
    memset( g_oldkeys, 0, sizeof( char ) * 256 );

    sys_acquire();
    sys_logwrite( "sys_diinit: start done\n" );
    return 1;
}

static void sys_dishutdown( void )
{
    if( m_di ) {
        sys_logwrite( "sys_dishutdown: stopping dinput\n" );
        if( m_keyboard ) {
            sys_logwrite( "sys_dishutdown: releasing keyboard\n" );
            m_keyboard->Unacquire();
            m_keyboard->Release();
            m_keyboard = NULL;
        }

        if( m_mouse ) {
            sys_logwrite( "sys_dishutdown: releasing mouse\n" );
            m_mouse->Unacquire();
            m_mouse->Release();
            m_mouse = NULL;
        }

        m_di->Release();
        m_di = NULL;
        sys_logwrite( "sys_dishutdown: stop done\n" );
    }
}

static int sys_dsinit( void )
{
    DSBUFFERDESC dsbd;
    WAVEFORMATEX wfx;
    HRESULT hres;

    sys_logwrite( "sys_dsinit: starting dsound\n" );

    m_issoundon = 0;

    if( FAILED( hres = DirectSoundCreate( NULL, &g_ds, NULL ) ) ) {
        sprintf_s( g_syserror, 1024, "sys_dsinit: failed to create the dsound object [%s]", sys_getdserr( hres ) );
        g_ds = NULL;
        return 0;
    }

    if( FAILED( hres = g_ds->SetCooperativeLevel( g_hwnd, DSSCL_PRIORITY ) ) ) {
        sprintf_s( g_syserror, 1024, "sys_dsinit: failed to set the co-op level [%s]", sys_getdserr( hres ) );
        g_ds->Release();
        g_ds = NULL;
        return 0;
    }

    memset( &dsbd, 0, sizeof( DSBUFFERDESC ) );
    dsbd.dwSize  = sizeof( DSBUFFERDESC );
    dsbd.dwFlags = DSBCAPS_PRIMARYBUFFER;
       
    if( FAILED( hres = g_ds->CreateSoundBuffer( &dsbd, &m_primary, NULL ) ) ) {
        sprintf_s( g_syserror, 1024, "sys_dsinit: failed to create the primary sound buffer [%s]", sys_getdserr( hres ) );
        g_ds->Release();
        g_ds = NULL;
        return 0;
    }

    memset( &wfx, 0, sizeof( WAVEFORMATEX ) ); 
    wfx.wFormatTag      = WAVE_FORMAT_PCM; 
    wfx.nChannels       = SOUND_CHANNELS; 
    wfx.nSamplesPerSec  = SOUND_SPS; 
    wfx.wBitsPerSample  = SOUND_BPS; 
    wfx.nBlockAlign     = ( wfx.wBitsPerSample / 8 ) * wfx.nChannels;
    wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;

    if( FAILED( hres = m_primary->SetFormat( &wfx ) ) ) {
        sprintf_s( g_syserror, 1024, "sys_dsinit: failed to set the primary buffer's format [%s]", sys_getdserr( hres ) );
        m_primary->Release();
        m_primary = NULL;
        g_ds->Release();
        g_ds = NULL;
        return 0;
    }
    sys_logwrite( "sys_dsinit: set format [%d,%d,%d]\n", SOUND_BPS, SOUND_SPS, SOUND_CHANNELS );

    m_issoundon = 1;
    sys_logwrite( "sys_dsinit: start done\n" );
    return 1;
}

static void sys_dsshutdown( void )
{
    if( g_ds ) {
        sys_logwrite( "sys_dsshutdown: stopping dsound\n" );
        if( m_primary ) {
            m_primary->Release();
            m_primary = NULL;
        }

        g_ds->Release();
        g_ds = NULL;
        sys_logwrite( "sys_dsshutdown: stop done\n" );
    }
}

static int sys_createclipper( void )
{
    LPRGNDATA rdata;
    RECT Rect;
    HRESULT hres;

    Rect.left   = 0;
    Rect.right  = SCREEN_WIDTH;
    Rect.top    = 0;
    Rect.bottom = SCREEN_HEIGHT;

    sys_logwrite( "sys_createclipper: creating ddraw clipper\n" );

    if( NULL == g_dd ) {
        sprintf_s( g_syserror, 1024, "sys_createclipper: ddraw isn't stared yet" );
        return 0;
    }

    if( FAILED( hres = g_dd->CreateClipper( 0, &g_backclipper, NULL ) ) ) {
        sprintf_s( g_syserror, 1024, "sys_createclipper: failed to create clipper [%s]", sys_getdderr( hres ) );
        return 0;
    }

    rdata = (LPRGNDATA)malloc( sizeof( RGNDATAHEADER ) + sizeof( RECT ) );
    if( NULL == rdata ) {
        strcpy_s( g_syserror, 1024, "sys_createclipper: failed to alloc memory" );
        return 0;
    }

    memcpy( (RECT *)rdata->Buffer, (RECT *)&Rect, sizeof( RECT ) );

    rdata->rdh.dwSize           = sizeof( RGNDATAHEADER );
    rdata->rdh.iType            = RDH_RECTANGLES;
    rdata->rdh.nCount           = 1;
    rdata->rdh.nRgnSize         = sizeof( RECT );

    rdata->rdh.rcBound.left     = 0;
    rdata->rdh.rcBound.right    = SCREEN_WIDTH;
    rdata->rdh.rcBound.top      = 0;
    rdata->rdh.rcBound.bottom   = SCREEN_HEIGHT;

    if( FAILED( hres = g_backclipper->SetClipList( rdata, 0 ) ) ) {
        free( rdata );
        sprintf_s( g_syserror, 1024, "sys_createclipper: failed to set the clip list [%s]", sys_getdderr( hres ) );
        return 0;
    }

    if( FAILED( hres = g_backbuffer->SetClipper( g_backclipper ) ) ) {
        free( rdata );
        sprintf_s( g_syserror, 1024, "sys_createclipper: failed to set the clipper onto the back buffer [%s]", sys_getdderr( hres ) );
        return 0;
    }

    free( rdata );
    sys_logwrite( "sys_createclipper: create done\n" );
    return 1;
}

static void sys_deleteclipper( void )
{
    sys_logwrite( "sys_deleteclipper: deleting ddraw clipper\n" );

    if( g_backclipper ) {
        g_backclipper->Release();
        g_backclipper = NULL;
    }
    if( g_frontclipper ) {
        g_frontclipper->Release();
        g_frontclipper = NULL;
    }

    sys_logwrite( "sys_deleteclipper: delete done\n" );
}

static int sys_bmpload( const char *filename, bmp_t *bmp )
{
    HANDLE file;
    DWORD bytesread;
    DWORD size;
    int i;
    int temp;

    if( NULL == filename || NULL == bmp ) {
        strcpy_s( g_syserror, 1024, "sys_bmpload: was passed a null pointer" );
        return 0;
    }

    file = CreateFile( filename, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
    if( INVALID_HANDLE_VALUE == file ) {
        sprintf_s( g_syserror, 1024, "sys_bmpload: failed to open the file [%s] [%d]", filename, GetLastError() );
        return 0;
    }

    if( !ReadFile( file, &bmp->fileheader, sizeof( BITMAPFILEHEADER ), &bytesread, NULL ) || sizeof( BITMAPFILEHEADER ) != bytesread ) {
        CloseHandle( file );
        sprintf_s( g_syserror, 1024, "sys_bmpload: failed to read the bitmap file header from the file [%s] [%d]", filename, GetLastError() );
        return 0;
    }

    if( BITMAP_ID != bmp->fileheader.bfType ) {
        sprintf_s( g_syserror, 1024, "sys_bmpload: the file [%s] isn't a valid bitmap file", filename );
        return 0;
    }

    if( !ReadFile( file, &bmp->infoheader, sizeof( BITMAPINFOHEADER ), &bytesread, NULL ) || sizeof( BITMAPINFOHEADER ) != bytesread ) {
        CloseHandle( file );
        sprintf_s( g_syserror, 1024, "sys_bmpload: failed to read the bitmap info header from the file [%s] [%d]", filename, GetLastError() );
        return 0;
    }

    if( BI_RGB != bmp->infoheader.biCompression ) {
        CloseHandle( file );
        sprintf_s( g_syserror, 1024, "sys_bmpload: the file [%s] uses an unsupported compression scheme", filename );
        return 0;
    }

    switch( bmp->infoheader.biBitCount ) {
        case 8 :
            if( !ReadFile( file, bmp->pal, sizeof( PALETTEENTRY ) * 256, &bytesread, NULL ) || sizeof( PALETTEENTRY ) * 256 != (int)bytesread ) {
                CloseHandle( file );
                sprintf_s( g_syserror, 1024, "sys_bmpload: failed to read the palette from the file [%s] [%d]", filename, GetLastError() );
                return 0;
            }

            // the palette is stored as BGR, reverse it and also set the flags
            for( i = 0; i < 256; i++ ) {
                temp                   = bmp->pal[ i ].peRed;
                bmp->pal[ i ].peRed    = bmp->pal[ i ].peBlue;
                bmp->pal[ i ].peBlue   = temp;
                bmp->pal[ i ].peFlags  = PC_NOCOLLAPSE;
            }

            // fall thru

        case 16 :
        case 24 :
        case 32 :
            if( bmp->infoheader.biHeight < 0 ) {
                size = bmp->infoheader.biWidth * -bmp->infoheader.biHeight * ( bmp->infoheader.biBitCount >> 3 );
            }
            else {
                size = bmp->infoheader.biWidth * bmp->infoheader.biHeight * ( bmp->infoheader.biBitCount >> 3 );
            }

            SetFilePointer( file, -(LONG)size, NULL, FILE_END );

            bmp->buffer = (unsigned char *)malloc( sizeof( unsigned char ) * size );
            if( NULL == bmp->buffer ) {
                CloseHandle( file );
                strcpy_s( g_syserror, 1024, "sys_bmpload: failed to allocate memory" );
                return 0;
            }

            if( !ReadFile( file, bmp->buffer, size, &bytesread, NULL ) || size != (int)bytesread ) {
                free( bmp->buffer );
                CloseHandle( file );
                sprintf_s( g_syserror, 1024, "sys_bmpload: failed to read the pixels from the file [%s] [%d]", filename, GetLastError() );
                return 0;
            }

            if( bmp->infoheader.biHeight > 0 ) {
                if( !sys_bmpflip( bmp ) ) {
                    free( bmp->buffer );
                    CloseHandle( file );
                    return 0; // err msg already set
                }
            }

            break;

        default :
            CloseHandle( file );
            sprintf_s( g_syserror, 1024, "sys_bmpload: the file [%s] uses an unsupported bit depth, only 8, 16, 24 and 32 bit bitmaps supported", filename );
            return 0;

    }

    CloseHandle( file );

    return 1;
}

static void sys_bmpunload( bmp_t *bmp )
{
    if( bmp && bmp->buffer ) {
        free( bmp->buffer );
        bmp->buffer = NULL;
    }
}

static int sys_bmp24to32( bmp_t *srcbmp, bmp_t *dstbmp )
{
    int x, y;
    unsigned char r, g, b;
    unsigned int srcoffset;
    unsigned int dstoffset;
    unsigned char *dst;

    memcpy( &dstbmp->infoheader, &srcbmp->infoheader, sizeof( BITMAPINFOHEADER ) );
    dstbmp->infoheader.biBitCount = 32;
    dstbmp->infoheader.biSizeImage = dstbmp->infoheader.biWidth * dstbmp->infoheader.biHeight * (dstbmp->infoheader.biBitCount >> 3);

    dstbmp->buffer = (unsigned char *)malloc( sizeof( char ) * dstbmp->infoheader.biSizeImage);
    if( NULL == dstbmp->buffer ) {
        strcpy_s( g_syserror, 1024, "sys_bmp24to32: failed to allocate memory" );
        return 0;
    }

    dst = dstbmp->buffer;

    for( y = 0; y < dstbmp->infoheader.biHeight; y++ ) {
        for( x = 0; x < dstbmp->infoheader.biWidth; x++ ) {

            srcoffset = ((y * srcbmp->infoheader.biWidth) + x) * 3;

            r = srcbmp->buffer[srcoffset + 0];       // this data came from a bmp file, therefore
            g = srcbmp->buffer[srcoffset + 1];       // it's in BGR format, NOT RGB
            b = srcbmp->buffer[srcoffset + 2];

            dstoffset = ((y * dstbmp->infoheader.biWidth) + x) * 4;

            dst[dstoffset + 0] = r;
            dst[dstoffset + 1] = g;
            dst[dstoffset + 2] = b;
            dst[dstoffset + 3] = 255;
        }
    }

    return 1;
}

//static int sys_bmp24to16( bmp_t *srcbmp, bmp_t *dstbmp )
//{
//    int x, y;
//    int r, g, b;
//    int srcoffset;
//    unsigned short *dst;
//
//    memcpy( &dstbmp->infoheader, &srcbmp->infoheader, sizeof( BITMAPINFOHEADER ) );
//    dstbmp->infoheader.biBitCount = 16;
//
//    dstbmp->buffer = (unsigned char *)malloc( sizeof( char ) * ( dstbmp->infoheader.biWidth * dstbmp->infoheader.biHeight * ( dstbmp->infoheader.biBitCount >> 3 ) ) );
//    if( NULL == dstbmp->buffer ) {
//        strcpy_s( g_syserror, 1024, "sys_bmp24to16: failed to allocate memory" );
//        return 0;
//    }
//
//    dst = (unsigned short *)dstbmp->buffer;
//
//    for( y = 0; y < dstbmp->infoheader.biHeight; y++ ) {
//        for( x = 0; x < dstbmp->infoheader.biWidth; x++ ) {
//
//            srcoffset = ( y * srcbmp->infoheader.biWidth ) + x;
//
//            b = srcbmp->buffer[ srcoffset*3 + 0 ];       // this data came from a bmp file, therefore
//            g = srcbmp->buffer[ srcoffset*3 + 1 ];       // it's in BGR format, NOT RGB
//            r = srcbmp->buffer[ srcoffset*3 + 2 ];
//
//            dst[ srcoffset ] = ( 0x03E0 == g_pixelfmt.dwGBitMask ?
//                                           sys_rgb555( r>>3, g>>3, b>>3 ) :
//                                           sys_rgb565( r>>3, g>>2, b>>3 ) );
//        }
//    }
//
//    return 1;
//}

//static int sys_bmp16to24( int Width, int Height, unsigned char *SrcBuffer, unsigned char **DstBuffer )
//{
//    int X, Y;
//    int Offset;
//    short Word;
//    unsigned char *Dst;
//
//    (*DstBuffer) = (unsigned char *)malloc( Width * Height * 3 );
//    if( NULL == (*DstBuffer) )
//        return 0;
//
//    Dst = (*DstBuffer);
//
//    for( Y = 0; Y < Height; Y++ )
//    {
//        for( X = 0; X < Width; X++ )
//        {
//            Offset = ( Y * Width ) + X;
//
//            Word  = (unsigned char)SrcBuffer[ Offset * 2 + 0 ];
//            Word |= (unsigned char)SrcBuffer[ Offset * 2 + 1 ] << 8;
//
//            if( 0x03E0 == g_pixelfmt.dwGBitMask )
//            {
//                Dst[ Offset * 3 + 2 ] = sys_getred555( Word ) << 3;
//                Dst[ Offset * 3 + 1 ] = sys_getgreen555( Word ) << 3;
//                Dst[ Offset * 3 + 0 ] = sys_getblue555( Word ) << 3;
//            }
//            else
//            {
//                Dst[ Offset * 3 + 2 ] = sys_getred565( Word ) << 3;
//                Dst[ Offset * 3 + 1 ] = sys_getgreen565( Word ) << 2;
//                Dst[ Offset * 3 + 0 ] = sys_getblue565( Word ) << 3;
//            }
//        }
//    }
//
//    return 1;
//}

static int sys_bmpflip( bmp_t *bmp )
{
    unsigned char *dest;
    long bytewidth;
    long height;

    if( NULL == bmp || NULL == bmp->buffer ) {
        strcpy_s( g_syserror, 1024, "sys_bmpflip: was passed a null pointer" );
        return 0;
    }

    bytewidth = bmp->infoheader.biWidth * ( bmp->infoheader.biBitCount >> 3 );

    if( bmp->infoheader.biHeight < 0 ) {
        dest = (unsigned char *)malloc( sizeof( unsigned char ) * -bmp->infoheader.biHeight * bytewidth );
        height = -bmp->infoheader.biHeight;
    }
    else {
        dest = (unsigned char *)malloc( sizeof( unsigned char ) * bmp->infoheader.biHeight * bytewidth );
        height = bmp->infoheader.biHeight;
    }

    if( NULL == dest ) {
        strcpy_s( g_syserror, 1024, "sys_bmpflip: failed to allocate memory" );
        return 0;
    }

    for( int row = 0; row < height; row++ ) {
        memcpy( &dest[ ( ( height - 1 ) - row ) * bytewidth ]
              , &bmp->buffer[ row * bytewidth ]
              , bytewidth );
    }

    free( bmp->buffer );
    bmp->buffer = dest;

    return 1;
}

static int sys_wavload( const char *filename, wav_t *wav )
{
    HMMIO hwav;
    MMCKINFO parent;
    MMCKINFO child;
    MMIOINFO mmioinfo;
    MMRESULT mmres;

    unsigned char *audiobuf1 = NULL;
    unsigned char *audiobuf2 = NULL;

    unsigned long audiolen1 = 0;
    unsigned long audiolen2 = 0;

    if( NULL == filename || NULL == wav ) {
        strcpy_s( g_syserror, 1024, "sys_wavload: was passed a null pointer" );
        return 0;
    }

    memset( &mmioinfo, 0, sizeof( MMIOINFO ) );

    parent.ckid         = (FOURCC)0;
    parent.cksize       = 0;
    parent.fccType      = (FOURCC)0;
    parent.dwFlags      = 0;
    parent.dwDataOffset = 0;

    child = parent;

    hwav = mmioOpen( (char *)filename, &mmioinfo, MMIO_READ | MMIO_ALLOCBUF );
    if( !hwav ) {
        sprintf_s( g_syserror, 1024, "sys_wavload: failed to open/find the wave file [%s] [%s]", filename, sys_getmmioerr( mmioinfo.wErrorRet ) );
        return 0;
    }

    parent.fccType = mmioFOURCC( 'W', 'A', 'V', 'E' );

    if( MMSYSERR_NOERROR != ( mmres = mmioDescend( hwav, &parent, NULL, MMIO_FINDRIFF ) ) ) {
        mmioClose( hwav, 0 );
        sprintf_s( g_syserror, 1024, "sys_wavload: the file [%s] is not a valid wave file [%s]", filename, sys_getmmioerr( mmres ) );
        return 0;
    }

    child.ckid = mmioFOURCC( 'f', 'm', 't', ' ' );

    if( MMSYSERR_NOERROR != ( mmres = mmioDescend( hwav, &child, &parent, 0 ) ) ) {
        mmioClose( hwav, 0 );
        sprintf_s( g_syserror, 1024, "sys_wavload: the file [%s] is not a valid wave file [%s]", filename, sys_getmmioerr( mmres ) );
        return 0;
    }

    if( sizeof( WAVEFORMATEX ) != mmioRead( hwav, (char *)&wav->fmt, sizeof( WAVEFORMATEX ) ) ) {
        mmioClose( hwav, 0 );
        sprintf_s( g_syserror, 1024, "sys_wavload: failed to read the header info from the file [%s]", filename );
        return 0;
    }

    if( WAVE_FORMAT_PCM != wav->fmt.wFormatTag ) {
        mmioClose( hwav, 0 );
        sprintf_s( g_syserror, 1024, "sys_wavload: unrecognized PCM encoding in the file [%s]", filename );
        return 0;
    }

    if( MMSYSERR_NOERROR != ( mmres = mmioAscend( hwav, &child, 0 ) ) ) {
        mmioClose( hwav, 0 );
        sprintf_s( g_syserror, 1024, "sys_wavload: the file [%s] is not a valid wave file [%s]", filename, sys_getmmioerr( mmres ) );
        return 0;
    }

    child.ckid = mmioFOURCC( 'd', 'a', 't', 'a' );

    if( MMSYSERR_NOERROR != ( mmres = mmioDescend( hwav, &child, &parent, MMIO_FINDCHUNK ) ) ) {
        mmioClose( hwav, 0 );
        sprintf_s( g_syserror, 1024, "sys_wavload: the file [%s] is not a valid wave file [%s]", filename, sys_getmmioerr( mmres ) );
        return 0;
    }

    wav->size = child.cksize;

    wav->buffer = (unsigned char *)malloc( sizeof( unsigned char ) * child.cksize );
    if( NULL == wav->buffer ) {
        mmioClose( hwav, 0 );
        strcpy_s( g_syserror, 1024, "sys_wavload: mem alloc failed" );
        return 0;
    }

    if( -1 == mmioRead( hwav, (char *)wav->buffer, child.cksize ) ) {
        mmioClose( hwav, 0 );
        sprintf_s( g_syserror, 1024, "sys_wavload: failed to read the sound bytes from the file [%s]", filename );
        return 0;
    }

    mmioClose( hwav, 0 );

    return 1;
}

static void sys_wavunload( wav_t *wav )
{
    if( wav && wav->buffer ) {
        free( wav->buffer );
        wav->buffer = NULL;
    }
}

static char sys_keytochar( int key )
{
    if( key >= 0 && key < 256 ) {
        return m_keychars[ key ];
    }
    return 0;
}

static void sys_buildkeychartable( void )
{
    memset( m_keychars, 0, sizeof( char ) * 256 );

    m_keychars[ DIK_1 ] = '1';
    m_keychars[ DIK_2 ] = '2';
    m_keychars[ DIK_3 ] = '3';
    m_keychars[ DIK_4 ] = '4';
    m_keychars[ DIK_5 ] = '5';
    m_keychars[ DIK_6 ] = '6';
    m_keychars[ DIK_7 ] = '7';
    m_keychars[ DIK_8 ] = '8';
    m_keychars[ DIK_9 ] = '9';
    m_keychars[ DIK_0 ] = '0';
    m_keychars[ DIK_MINUS ] = '-';
    m_keychars[ DIK_EQUALS ] = '=';
    m_keychars[ DIK_Q ] = 'Q';
    m_keychars[ DIK_W ] = 'W';
    m_keychars[ DIK_E ] = 'E';
    m_keychars[ DIK_R ] = 'R';
    m_keychars[ DIK_T ] = 'T';
    m_keychars[ DIK_Y ] = 'Y';
    m_keychars[ DIK_U ] = 'U';
    m_keychars[ DIK_I ] = 'I';
    m_keychars[ DIK_O ] = 'O';
    m_keychars[ DIK_P ] = 'P';
    m_keychars[ DIK_LBRACKET ] = '[';
    m_keychars[ DIK_RBRACKET ] = ']';
    m_keychars[ DIK_A ] = 'A';
    m_keychars[ DIK_S ] = 'S';
    m_keychars[ DIK_D ] = 'D';
    m_keychars[ DIK_F ] = 'F';
    m_keychars[ DIK_G ] = 'G';
    m_keychars[ DIK_H ] = 'H';
    m_keychars[ DIK_J ] = 'J';
    m_keychars[ DIK_K ] = 'K';
    m_keychars[ DIK_L ] = 'L';
    m_keychars[ DIK_SEMICOLON ] = ';';
    m_keychars[ DIK_APOSTROPHE ] = '\'';
    m_keychars[ DIK_GRAVE ] = '^';
    m_keychars[ DIK_BACKSLASH ] = '\\';
    m_keychars[ DIK_Z ] = 'Z';
    m_keychars[ DIK_X ] = 'X';
    m_keychars[ DIK_C ] = 'C';
    m_keychars[ DIK_V ] = 'V';
    m_keychars[ DIK_B ] = 'B';
    m_keychars[ DIK_N ] = 'N';
    m_keychars[ DIK_M ] = 'M';
    m_keychars[ DIK_COMMA ] = ',';
    m_keychars[ DIK_PERIOD ] = '.';
    m_keychars[ DIK_SLASH ] = '/';
    m_keychars[ DIK_MULTIPLY ] = '*';
    m_keychars[ DIK_SPACE ] = ' ';
    m_keychars[ DIK_NUMPAD7 ] = '7';
    m_keychars[ DIK_NUMPAD8 ] = '8';
    m_keychars[ DIK_NUMPAD9 ] = '9';
    m_keychars[ DIK_SUBTRACT ] = '-';
    m_keychars[ DIK_NUMPAD4 ] = '4';
    m_keychars[ DIK_NUMPAD5 ] = '5';
    m_keychars[ DIK_NUMPAD6 ] = '6';
    m_keychars[ DIK_ADD ] = '+';
    m_keychars[ DIK_NUMPAD1 ] = '1';
    m_keychars[ DIK_NUMPAD2 ] = '2';
    m_keychars[ DIK_NUMPAD3 ] = '3';
    m_keychars[ DIK_NUMPAD0 ] = '0';
    m_keychars[ DIK_DECIMAL ] = '.';
    m_keychars[ DIK_NUMPADEQUALS ] = '=';
    m_keychars[ DIK_COLON ] = '"';
    m_keychars[ DIK_UNDERLINE ] = '_';
    m_keychars[ DIK_DIVIDE ] = '/';
}

const char *sys_getmmioerr( UINT err )
{
    switch( err ) {
        case MMIOERR_ACCESSDENIED : "The file is protected and cannot be opened.";
        case MMIOERR_INVALIDFILE : "Another failure condition occurred. This is the default error for an open-file failure.";
        case MMIOERR_NETWORKERROR : "The network is not responding to the request to open a remote file.";
        case MMIOERR_PATHNOTFOUND : "The directory specification is incorrect.";
        case MMIOERR_SHARINGVIOLATION : "The file is being used by another application and is unavailable.";
        case MMIOERR_TOOMANYOPENFILES : "The number of files simultaneously open is at a maximum level. The system has run out of available file handles.";
        case MMIOERR_CHUNKNOTFOUND : "The end of the file (or the end of the parent chunk, if given) was reached before the desired chunk was found.";
        case MMIOERR_CANNOTSEEK : "There was an error while seeking to the end of the chunk.";
        case MMIOERR_CANNOTWRITE : "The contents of the buffer could not be written to disk.";
    }
    return "not found";
}

static int sys_bmpsave( const char *Filename, int Width, int Height, int BPP, unsigned char *Pixels )
{
    FILE *File;
    errno_t Error;
    BITMAPFILEHEADER FileHeader;
    BITMAPINFO Info;

    if( NULL == Filename || NULL == Pixels )
        return 0;

    Error = fopen_s( &File, Filename, "wb" );
    if( Error != 0 )
        return 0;

    ZeroMemory( &FileHeader, sizeof( BITMAPFILEHEADER ) );
    FileHeader.bfType    = BITMAP_ID;
    FileHeader.bfSize    = sizeof( BITMAPFILEHEADER ) + sizeof( BITMAPINFO ) + ( Width * Height * ( BPP >> 3 ) );
    FileHeader.bfOffBits = sizeof( BITMAPFILEHEADER ) + sizeof( BITMAPINFO );

    ZeroMemory( &Info, sizeof( BITMAPINFO ) );
    Info.bmiHeader.biSize           = sizeof( BITMAPINFOHEADER );
    Info.bmiHeader.biWidth          = Width;
    Info.bmiHeader.biHeight         = Height;
    Info.bmiHeader.biPlanes         = 1;
    Info.bmiHeader.biBitCount       = BPP;
    Info.bmiHeader.biCompression    = BI_RGB;

    fwrite( &FileHeader, sizeof( BITMAPFILEHEADER ), 1, File );
    fwrite( &Info, sizeof( BITMAPINFO ), 1, File );
    fwrite( Pixels, sizeof( unsigned char ), Width * Height * ( BPP >> 3 ), File );

    fclose( File );

    return 1;
}

//void sys_screenshot( void )
//{
//    DDSURFACEDESC2 ddsd;
//    unsigned char *Buffer16, *Buffer24;
//    unsigned char *Src, *Dst;
//    int i;
//    char Filename[ 256 ];
//    FILE *File;
//    errno_t Error;
//
//    CreateDirectory( "ScreenShots", NULL );
//
//    // find the next available screenshot filename
//    for( i = 0; ; i++ )
//    {
//        sprintf_s( Filename, 256, "ScreenShots/ScreeShot%04d.bmp", i );
//        Error = fopen_s( &File, Filename, "rb" );
//        if( Error == 0 )
//            fclose( File );
//        else
//            break;
//    }
//
//    // get a pointer to the display pixels
//    memset( &ddsd, 0, sizeof( DDSURFACEDESC2 ) );
//    ddsd.dwSize = sizeof( DDSURFACEDESC2 );
//
//    if( FAILED( g_frontbuffer->Lock( NULL, &ddsd, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT, NULL ) ) ) {
//        return;
//    }
//
//    // store all the pixels in a buffer guaranteed to store the pixels contiguously
//    Buffer16 = (unsigned char *)malloc( sizeof( unsigned char ) * SCREEN_WIDTH * SCREEN_HEIGHT * ( SCREEN_BPP >> 3 ) );
//    if( NULL == Buffer16 )
//        return;
//
//    Src = (unsigned char *)ddsd.lpSurface;
//    Dst = Buffer16;
//
//    for( i = 0; i < SCREEN_HEIGHT; i++ )
//    {
//        memcpy( Dst, Src, SCREEN_WIDTH * ( SCREEN_BPP >> 3 ) );
//
//        Src += ddsd.lPitch;
//        Dst += SCREEN_WIDTH * ( SCREEN_BPP >> 3 );
//    }
//
//    sys_bmp16to24( SCREEN_WIDTH, SCREEN_HEIGHT, Buffer16, &Buffer24 );
//    sys_bmpsave( Filename, SCREEN_WIDTH, SCREEN_HEIGHT, 24, Buffer24 );
//
//    free( Buffer24 );
//    free( Buffer16 );
//    g_frontbuffer->Unlock( NULL );
//}

//--------------------------------------------------------------------------------
// eof
//--------------------------------------------------------------------------------
