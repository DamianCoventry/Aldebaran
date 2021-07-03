#include "stdafx.h"
#define INITGUID
#define WIN32_LEAN_AND_MEAN

//--------------------------------------------------------------------------------
// defines
//--------------------------------------------------------------------------------

#define DEBUGGING               0
#define SPLASH_WAITTHRESHOLD    240 // approx 4 secs

//--------------------------------------------------------------------------------
// includes
//--------------------------------------------------------------------------------
#include <windows.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#include "sys.h"
#include "game.h"
#include "resource.h"

//--------------------------------------------------------------------------------
// private data
//--------------------------------------------------------------------------------
static const char *m_winclass = "mainwndclass";
static int m_active = 0;

//--------------------------------------------------------------------------------
// private prototypes
//--------------------------------------------------------------------------------

static LRESULT CALLBACK WinProc( HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam );
static BOOL CALLBACK DialogProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
 
//--------------------------------------------------------------------------------
// main game loop
//--------------------------------------------------------------------------------

int WINAPI WinMain( HINSTANCE hinstance, HINSTANCE hprevinstance, LPSTR lpcmdline, int ncmdshow )
{
    MSG msg;
    HDC hdc;
    WNDCLASSEX wc;
    int Done;
    int i, Exit;
    int SplashCounter;
    int SplashSound;
    //int Fading;
    obj_t SplashImage;
    char Buf[ 256 ];

    GetCurrentDirectory( 300, g_apppath );

    sys_logcreate( "log.txt" );

    ShowCursor( FALSE );

    g_hinst = hinstance;

    wc.cbSize           = sizeof( WNDCLASSEX );
    wc.style            = CS_OWNDC;
    wc.lpfnWndProc      = WinProc;
    wc.cbClsExtra       = 0;
    wc.cbWndExtra       = 0;
    wc.hInstance        = g_hinst;
    wc.hIcon            = LoadIcon( g_hinst, MAKEINTRESOURCE( IDI_ASTEROIDS ) );
    wc.hCursor          = LoadCursor( g_hinst, IDC_ARROW );
    wc.hbrBackground    = (HBRUSH)GetStockObject( BLACK_BRUSH );
    wc.lpszMenuName     = NULL;
    wc.lpszClassName    = m_winclass;
    wc.hIconSm          = NULL;

    RegisterClassEx( &wc );

#if DEBUGGING
    g_hwnd = CreateWindowEx( 0, m_winclass, "Main Window", WS_POPUP, 0, 0
                           , SCREEN_WIDTH, SCREEN_HEIGHT
                           , (HWND)NULL, (HMENU)NULL, g_hinst, NULL );
#else
    g_hwnd = CreateWindowEx( 0, m_winclass, "Main Window", WS_POPUP, 0, 0
                           , GetSystemMetrics( SM_CXSCREEN ), GetSystemMetrics( SM_CYSCREEN )
                           , (HWND)NULL, (HMENU)NULL, g_hinst, NULL );
#endif

    ShowWindow( g_hwnd, SW_SHOWDEFAULT); 
    UpdateWindow( g_hwnd );

    srand( (unsigned int)time( NULL ) );

    hdc = GetDC( g_hwnd );
    if( hdc ) {
        SetBkMode( hdc, OPAQUE );
        SetBkColor( hdc, 0 );
        SetTextColor( hdc, RGB( 255, 255, 255 ) );

        TextOut( hdc
               , GetSystemMetrics( SM_CXSCREEN ) - 128
               , GetSystemMetrics( SM_CYSCREEN ) - 32
               , "initialising..."
               , static_cast<int>(strlen( "initialising..." )) );

        ReleaseDC( g_hwnd, hdc );
    }

#if DEBUGGING
    if( !sys_init( 0 ) ) {
#else
    if( !sys_init( 1 ) ) {
#endif
        sys_shutdown();
        ShowWindow( g_hwnd, SW_HIDE );
        ShowCursor( TRUE );
        ClipCursor( NULL );
        sys_logwrite( "WinMain: %s\n", g_syserror );
        MessageBox( g_hwnd, g_syserror, NULL, MB_OK | MB_ICONEXCLAMATION );
        return 0;
    }

    Exit = 0;

    sys_logwrite( "WinMain() checking for splash screen files...\n" );
    Done = 0;
    for( i = 0; !Done; i++ ) {
        sprintf_s( Buf, 256, "Splash/Splash%d.bmp", i + 1 );

        if( game_createobject( &SplashImage, 640, 480, Buf ) ) {
            SplashImage.x = SCREEN_HALFWIDTH_f;
            SplashImage.y = SCREEN_HALFHEIGHT_f;
            game_setframe( &SplashImage, 0 );

            sprintf_s( Buf, 256, "Splash/Splash%d.wav", i + 1 );
            SplashSound = sys_createsound( Buf );

            SplashCounter   = 0;
            //Fading          = 0;

            for( ; ; ) {
                if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) ) {
                    if( WM_QUIT == msg.message ) {
                        Done = Exit = 1;
                        break;
                    }
                    TranslateMessage( &msg );
                    DispatchMessage( &msg );
                }
                else if( m_active ) {
                    sys_begin();

                    if( sys_keydown( DIK_ESCAPE ) || sys_keydown( DIK_SPACE ) || sys_keydown( DIK_RETURN ) ||
                        sys_mousebuttondown( 0 ) || sys_mousebuttondown( 1 ) ) {

                        //if( !Fading ) {
                        //    Fading = 1;
                        //}
                        //else {
                            sys_end();
                            break;
                        //}
                    }

                    //if( Fading ) {
                    //    if( !sys_fadetocolor( SplashImage.dds
                    //                        , SCREEN_WIDTH, SCREEN_HEIGHT
                    //                        , -3, 0, 0, 0 ) ) {
                    //        break;
                    //    }
                    //}
                    //else {
                        if( ++SplashCounter > SPLASH_WAITTHRESHOLD && !sys_issoundplaying( SplashSound ) ) {
                            //Fading = 1;
                            break;
                        }
                    //}

                    RECT dest;
                    dest.left = long(SCREEN_HALFWIDTH_f) - SplashImage.halfwidth;
                    dest.top = long(SCREEN_HALFHEIGHT_f) - SplashImage.halfheight;
                    dest.right = dest.left + SplashImage.width;
                    dest.bottom = dest.top + SplashImage.height;
                    g_backbuffer->Blt(&dest, SplashImage.dds, NULL, DDBLT_WAIT, NULL);

                    // just to make sure the bitmap is onscreen before
                    // we play the wav
                    if( 1 == SplashCounter ) {
                        sys_playsound( SplashSound, 0 );
                    }

                    sys_draw();
                    sys_end();

                    Sleep( 15 );

                }
                else {
                    WaitMessage();
                }
            }

            sys_stopsound( SplashSound );
            sys_deletesound( SplashSound );
            game_deleteobject( &SplashImage );
        }
        else {
            Done = 1;
        }
    }

    sys_logwrite( "WinMain() starting game initialisation...\n" );
    if( !Exit ) {
        if( !game_init() ) {
            game_shutdown();
            sys_shutdown();
            ShowWindow( g_hwnd, SW_HIDE );
            ShowCursor( TRUE );
            ClipCursor( NULL );
            sys_logwrite( "WinMain: %s\n", g_gameerror );
            MessageBox( g_hwnd, g_gameerror, NULL, MB_OK | MB_ICONEXCLAMATION );
            return 0;
        }

        for( ; ; ) {
            if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) ) {
                if( WM_QUIT == msg.message ) {
                    break;
                }
                TranslateMessage( &msg );
                DispatchMessage( &msg );
            }
            else if( m_active ) {
                sys_begin();
                game_main();
                sys_draw();
                sys_end();
                //Sleep( 15 );
            }
            else {
                WaitMessage();
            }
        }

        game_shutdown();
    }

    sys_shutdown();
    ShowCursor( TRUE );
    ClipCursor( NULL );

#if DEMO_VERSION
    DialogBoxParam( hinstance, MAKEINTRESOURCE( IDD_DIALOG1 ), NULL, DialogProc, 0 );
#endif

    return static_cast<int>(msg.wParam);
}

//--------------------------------------------------------------------------------
// window procedure
//--------------------------------------------------------------------------------

static LRESULT CALLBACK WinProc( HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam )
{
    switch( umsg ) {
        case WM_ACTIVATE :
            if( LOWORD( wparam ) == WA_INACTIVE ) {
                m_active = 0;
                sys_unacquire();
                sys_pausemp3( -1 );
                ClipCursor( NULL );
            }
            else {
                m_active = 1;
                sys_acquire();
                sys_playmp3( -1 );
                ClipCursor( &g_drawrect );
            }
            break;

        case WM_SETCURSOR :
            SetCursor( NULL );
            break;

        case WM_PAINT : {
            PAINTSTRUCT ps;
            BeginPaint( hwnd, &ps );
            EndPaint( hwnd, &ps );
            break;
        }

        case WM_CLOSE :
            PostQuitMessage( 0 );
            break;

        case WM_SYSCOMMAND :
            switch( 0xFFF0 & wparam ) {
                case SC_KEYMENU :           // ignore these commands
                case SC_MONITORPOWER :
                case SC_MOUSEMENU :
                case SC_TASKLIST :
                case SC_SCREENSAVE :
                    return 0;

                default :
                    return DefWindowProc( hwnd, umsg, wparam, lparam );
            }
            break;

        // dshow events
        case WM_GRAPHEVENT :
            sys_ongraphevent();
            break;

        default:
            return DefWindowProc( hwnd, umsg, wparam, lparam );
    }

    return 0;
}

static BOOL CALLBACK DialogProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    switch( uMsg )
    {
        case WM_INITDIALOG :
            return TRUE;

        case WM_CLOSE :
            EndDialog( hWnd, IDOK );
            return TRUE;

        case WM_COMMAND :
            switch( LOWORD( wParam ) )
            {
                case IDOK :
                    EndDialog( hWnd, IDOK );
                    return TRUE;
            }
            break;
    }

    return FALSE;
}

//--------------------------------------------------------------------------------
// eof
//--------------------------------------------------------------------------------
