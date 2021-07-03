#ifndef _game_h_
#define _game_h_

//--------------------------------------------------------------------------------
// includes
//--------------------------------------------------------------------------------

#include <stdio.h>
#include <math.h>
#include <time.h>
#include <io.h>

#include "sys.h"


//--------------------------------------------------------------------------------
// defines
//--------------------------------------------------------------------------------

#define DEMO_VERSION            0

#define MAX_WAYPOINTS           10


//--------------------------------------------------------------------------------
// globals
//--------------------------------------------------------------------------------

extern char g_gameerror[ 1024 ];


//--------------------------------------------------------------------------------
// structures
//--------------------------------------------------------------------------------

typedef enum state_e {
      STATE_PREBIRTH = 0
    , STATE_BIRTH
    , STATE_ALIVE
    , STATE_DYING
    , STATE_DEAD
    , STATE_AUTOSHIELD
    , STATE_PAUSED
    , STATE_NMEWAITING
    , STATE_NMESTARTWAIT
    , STATE_NMEACTIVE
    , STATE_NMEDEAD
    , STATE_NMETURNING
    , STATE_NMEMOVING
    , STATE_NMEWPWAIT
    , STATE_LINE
    , STATE_CIRCLE
}
state_t;

typedef struct options_s {
    int soundlevel;
    int musiclevel;
    int mouseoffset;
    float mousespeed;
    int physicstype;
    int playerthrusting;
    int playershottrails;
    int playershotexpl;
    int playerexpl;
    int nmeshottrails;
    int nmeshotexpl;
    int nmeexpl;
}
options_t;

typedef enum gun_e {
      GUN_NORMAL
    , GUN_MULTISHOT
    , GUN_CHARGEUP
}
gun_t;

typedef struct statesliderbox_s {
    float minx;
    float maxx;
    float offsetx;
    float offsety;
}
statesliderbox_t;

typedef struct stateplayer_s {
    int direction;
    int cooldown;
    int shield;
    int autoshield;
    int autocounter;
    int autostate;
    int score;
    int livesleft;
    int counter;
    int shieldhealth;
    int cfcounteractive;
    int cfcount;
    int ammo;
    int charge;
    int isguncharging;
    gun_t gun;
    gun_t gunpushed;
    int ammopushed;
    int chargepushed;
}
stateplayer_t;

typedef struct stateshot_s {
    int charge;
    int index;
}
stateshot_t;

typedef struct statepickup_s {
    int framecounter;
    int statecounter;
    int statethreshold;
    int lingercounter;
    int lingerthreshold;
    int exitpending;
}
statepickup_t;

typedef struct waypoint_s {
    float x, y;
    int changestate;
    int counter;
}
waypoint_t;

typedef struct statenme_s {
    state_t statewaypoint;
    int counter;
    int threshold;
    int cooldown;
    int canshoot;
    int currentwaypoint;
    int waypointcount;
    waypoint_t waypoints[ MAX_WAYPOINTS ];
}
statenme_t;

typedef struct staterisingscore_s {
    char string[ 16 ];
    int counter;
    int largefont;
}
staterisingscore_t;

typedef struct statecursor_s {
    int counter;
    int change;
    int direction;
}
statecursor_t;

typedef struct stateasteroid_s {
    int direction;
    int collection;
    int type;
    int framecounter;
    int framethreshold;
    int counter;
    int threshold;
    int health;
    int bombcounter;
    int bombthreshold;
    int bombactive;
    int hasentered;
}
stateasteroid_t;

typedef struct obj_s {
    RECT src;
    RECT dst;

    state_t state;
    state_t savedstate;

    int points;

    int direction;
    int bmpwidth;
    int width, height;
    int bbwidth, bbheight;
    int halfwidth, halfheight;
    int clone;

    float mass;

    float x, y;
    float spd;
    float spdx, spdy;

    union {
        stateplayer_t stateplayer;
        stateasteroid_t stateasteroid;
        statecursor_t statecursor;
        staterisingscore_t staterisingscore;
        statenme_t statenme;
        statepickup_t statepickup;
        stateshot_t stateshot;
        statesliderbox_t statesliderbox;
    };

    LPDIRECTDRAWSURFACE7 dds;
}
obj_t;


//--------------------------------------------------------------------------------
// public prototypes
//--------------------------------------------------------------------------------
int game_init( void );
void game_shutdown( void );
void game_main( void );

int game_createobject( obj_t *obj, int w, int h, const char *filename );
void game_deleteobject( obj_t *obj );
int game_setframe( obj_t *obj, int frame );

#endif