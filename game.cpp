#include "stdafx.h"
//--------------------------------------------------------------------------------
// includes
//--------------------------------------------------------------------------------
#include "game.h"

//--------------------------------------------------------------------------------
// defines
//--------------------------------------------------------------------------------

// general
#define MAX_LEVELS              10
#define MAX_ROUNDS              5
#define MAX_OVERALLLEVELS       50              // MAX_LEVELS * MAX_ROUNDS
#define MAX_GAMESOUNDS          13
#define MAX_GAMETRACKS          10
#define MAX_PTLESINCLSTR        64
#define FILE_HIGHSCORES         "scr/highscores.scr"
#define PI_OVER_180             0.0174532f
#define _180_OVER_PI            57.29578f
#define WORST_LEVELTIME         180             // 3 minutes (180 secs)
#define ACCURACY_MULTIPLIER     2
#define SHIELDDMG_MULTIPLIER    2
#define COLLISION_ANGLENOISE    10
#define COLLISION_SPEEDNOSEMAX  50
#define COLLISION_SPEEDNOSEMIN  -150

// highscores
#define MAX_HIGHSCORES          5
#define MAX_SCORENAMELEN        16

// level title/game over bitmap
#define MAX_FRAMETHRESHOLD      150
#define MAX_GAMEOVERTHRESHOLD   240

// asteroids
#define MAX_ASTS                5
#define MAX_ASTCOLS             2
#define MAX_ASTPIECES           7
#define MIN_ASTFRAMEROT         3
#define MAX_ASTFRAMEROT         6
#define ASTEROID_SPEED          0.01f
#define ASTEROID_SPEEDMIN       0.5f
#define ASTEROID_SPEEDMAX       3
#define ASTTYPE_PIECE0          0
#define ASTTYPE_PIECE1          1
#define ASTTYPE_PIECE2          2
#define ASTTYPE_PIECE3          3
#define ASTTYPE_PIECE4          4
#define ASTTYPE_PIECE5          5
#define ASTTYPE_PIECE6          6
#define ASTCOL_DIRT             0
#define ASTCOL_ROCK             1

// shots
#define MAX_SHOTS               3
#define SHOT_SPEED              8.0f
#define SHOT_MASS               0.25f
#define MAX_CHARGEUPSHOTS       5
#define MAX_CHARGEUPSIZES       3

// nme shots
#define MAX_NMESHOTS            5
#define NMESHOT_SPEED           8.0f
#define NMESHOT_MASS            0.25f

// shot trails
#define MAX_SHOTTRAILS          12
#define MAX_SHOTTRAILPTLES      32
#define SHOTTRAIL_MINRED        192
#define SHOTTRAIL_MAXRED        255
#define SHOTTRAIL_MINGREEN      192
#define SHOTTRAIL_MAXGREEN      255
#define SHOTTRAIL_MINBLUE       48
#define SHOTTRAIL_MAXBLUE       128
#define SHOTTRAIL_DESTHUE       16
#define SHOTTRAIL_INC           -3
#define SHOTTRAIL_ADD           1

// charge up trail & explosion
#define CHARGEUPTRAIL_MINRED        180
#define CHARGEUPTRAIL_MAXRED        240
#define CHARGEUPTRAIL_MINGREEN      16
#define CHARGEUPTRAIL_MAXGREEN      32
#define CHARGEUPTRAIL_MINBLUE       180
#define CHARGEUPTRAIL_MAXBLUE       240
#define CHARGEUPTRAIL_DESTHUE       16
#define CHARGEUPTRAIL_INC           -3
#define CHARGEUPSPIT_INC            -4
#define CHARGEUPTRAIL_ADD           1
#define CHARGEUPSPIT_ADD            2

// nme explosions
#define NMEEXP_MINRED           180
#define NMEEXP_MAXRED           220
#define NMEEXP_MINGREEN         224
#define NMEEXP_MAXGREEN         255
#define NMEEXP_MINBLUE          224
#define NMEEXP_MAXBLUE          255
#define NMEEXP_DESTHUE          16
#define NMEEXP_INC              -3

// nme shot trails
#define MAX_NMESHOTTRAILS       10
#define NMESHOTTRAIL_MINRED     16
#define NMESHOTTRAIL_MAXRED     48
#define NMESHOTTRAIL_MINGREEN   212
#define NMESHOTTRAIL_MAXGREEN   255
#define NMESHOTTRAIL_MINBLUE    212
#define NMESHOTTRAIL_MAXBLUE    255
#define NMESHOTTRAIL_DESTHUE    16
#define NMESHOTTRAIL_INC        -3

// player thrust trail
#define PLYRTHRUSTTRAILF_MINRED     16
#define PLYRTHRUSTTRAILF_MAXRED     32
#define PLYRTHRUSTTRAILF_MINGREEN   224
#define PLYRTHRUSTTRAILF_MAXGREEN   255
#define PLYRTHRUSTTRAILF_MINBLUE    150
#define PLYRTHRUSTTRAILF_MAXBLUE    176
#define PLYRTHRUSTTRAILF_DESTHUE    16
#define PLYRTHRUSTTRAILF_INC        -10
#define PLYRTHRUSTTRAILF_ADD        2

#define PLYRTHRUSTTRAILB_MINRED     16
#define PLYRTHRUSTTRAILB_MAXRED     32
#define PLYRTHRUSTTRAILB_MINGREEN   176
#define PLYRTHRUSTTRAILB_MAXGREEN   150
#define PLYRTHRUSTTRAILB_MINBLUE    96
#define PLYRTHRUSTTRAILB_MAXBLUE    112
#define PLYRTHRUSTTRAILB_DESTHUE    16
#define PLYRTHRUSTTRAILB_INC        -10
#define PLYRTHRUSTTRAILB_ADD        2

// shot richochets
#define MAX_SHOTRICHOCHETS          10
#define MIN_SHOTRICHOCHETSPEED      0.1f
#define MAX_SHOTRICHOCHETSPEED      20
#define MAX_SHOTRICHOCHETPTLES      64
#define MAX_SHOTRICHOCHETSPREADANG  30
#define SHOTRICHOCHET_MINRED        192
#define SHOTRICHOCHET_MAXRED        255
#define SHOTRICHOCHET_MINGREEN      80
#define SHOTRICHOCHET_MAXGREEN      160
#define SHOTRICHOCHET_MINBLUE       48
#define SHOTRICHOCHET_MAXBLUE       64
#define SHOTRICHOCHET_DESTHUE       16
#define SHOTRICHOCHET_INC           -10
#define SHOTRICHOCHET_ADD           4

// nme shot richochets
#define MAX_NMESHOTRICHOCHETS          10
#define NMESHOTRICHOCHET_MINRED        192
#define NMESHOTRICHOCHET_MAXRED        255
#define NMESHOTRICHOCHET_MINGREEN      80
#define NMESHOTRICHOCHET_MAXGREEN      160
#define NMESHOTRICHOCHET_MINBLUE       48
#define NMESHOTRICHOCHET_MAXBLUE       64
#define NMESHOTRICHOCHET_DESTHUE       16
#define NMESHOTRICHOCHET_INC           -5

// player colliding
#define PLYRCOLLIDE_MINRED          16
#define PLYRCOLLIDE_MAXRED          64
#define PLYRCOLLIDE_MINGREEN        128
#define PLYRCOLLIDE_MAXGREEN        255
#define PLYRCOLLIDE_MINBLUE         16
#define PLYRCOLLIDE_MAXBLUE         64
#define PLYRCOLLIDE_DESTHUE         16
#define PLYRCOLLIDE_INC             -2
#define PLYRCOLLIDE_CFTHRESHOLD     10

// the player explosion
#define PLYREXP_MINRED              60
#define PLYREXP_MAXRED              80
#define PLYREXP_MINGREEN            90
#define PLYREXP_MAXGREEN            110
#define PLYREXP_MINBLUE             200
#define PLYREXP_MAXBLUE             255
#define PLYREXP_INC                 -2

// starfield
#define MAX_LAYER1SPEED          6250
#define MIN_LAYER1SPEED           625
#define MAX_LAYER2SPEED         12500
#define MIN_LAYER2SPEED          1250
#define MAX_LAYER3SPEED         25000
#define MIN_LAYER3SPEED          2500
#define MAX_LAYER4SPEED         50000
#define MIN_LAYER4SPEED          5000
#define LAYERSPEEDDIVISOR       10000.0f

#define STARFIELD_COL1MINRED    240
#define STARFIELD_COL1MAXRED    224
#define STARFIELD_COL1MINGREEN  240
#define STARFIELD_COL1MAXGREEN  224
#define STARFIELD_COL1MINBLUE   240
#define STARFIELD_COL1MAXBLUE   224
#define STARFIELD_COL2MINRED    20
#define STARFIELD_COL2MAXRED    40
#define STARFIELD_COL2MINGREEN  206
#define STARFIELD_COL2MAXGREEN  226
#define STARFIELD_COL2MINBLUE   234
#define STARFIELD_COL2MAXBLUE   254
#define STARFIELD_COL3MINRED    234
#define STARFIELD_COL3MAXRED    254
#define STARFIELD_COL3MINGREEN  150
#define STARFIELD_COL3MAXGREEN  170
#define STARFIELD_COL3MINBLUE   20
#define STARFIELD_COL3MAXBLUE   40
#define STARFIELD_ANGDEVIATION  5
#define STARFIELD_LAYERS        4
#define STARFIELD_PTLESINLAYER  ( MAX_PTLESINCLSTR / STARFIELD_LAYERS )

// ship debris
#define MAX_DEBRIS              4
#define MAX_ACTIVEDEBRIS        5
#define DEBRIS_MASS             0.25f

// smallest debris
#define MAX_SMALLESTTYPES       4
#define MAX_SMALLESTDEBRIS      16

// player
#define PLAYER_ACCEL            0.1f
#define PLAYER_MAXSPEED         5.0f
#define PLAYER_SHIELDOFFSET     1152
#define PLAYER_FRAMEINC         10
#define PLAYER_TURNINC          6
#define PLAYER_MAXFRAMEIDX      35
#define PLAYER_GUNCOOLDOWN      25
#define PLAYER_AUTOSHIELDMAX    12
#define PLAYER_AUTOCOUNTERMAX   10
#define PLAYER_LIVES            3
#define PLAYER_MAXLIVES         5
#define PLAYER_PAUSETHRESHOLD   150             // appox 2.5 seconds
#define PLAYER_MASS             0.85f

// sounds
#define SND_ASTEXPL0            0
#define SND_ASTEXPL1            1
#define SND_ASTHIT              2
#define SND_PLYREXPL            3
#define SND_PLYRSHLDDN          4
#define SND_PLYRSHLDUP          5
#define SND_PLYRSHT             6
#define SND_CLICK               7
#define SND_CHEAT               8
#define SND_BOUNCE              9
#define SND_PLYRSHT2            10
#define SND_PLYRCHGUP           11
#define SND_APPLAUSE            12

// marquee
#define MAX_MARQUEELINES        80
#define MARQUEE_LINESIZE        32
#define MARQUEE_THRESHOLD       2

// points
#define POINTS_ASTBASE          3
#define POINTS_ASTCHIPBASE      1
#define POINTS_DEBRISBASE       2
#define POINTS_NMESHIPBASE      4

// mouse cursor
#define MOUSE_ROTATESPEED       6
#define MOUSE_CHANGETHRESHOLD   60

// end of level statistics
#define STATS_THRESHOLD         600

// enter name cursor
#define MAX_BLINKTHRESHOLD      15

// rising score
#define RISINGSCORE_SPEED       -0.25f
#define RISINGSCORE_THRESHOLD   120

// enemy ships
#define MAX_NMESHIPS            10
#define WAYPOINT_BOXSIZE        16
#define NME_TURNSPEED           7
#define NME_STARTWAITBASE       200
#define NME_ACCEL               0.15f
#define NME_BASESPEED           1.5f
#define NME_INCSPEED            0.25f
#define NME_SPACING             64
#define NME_FOVBASE             5
#define NME_GUNCOOLDOWN         20
#define NME_WAITTHRESHOLD       100
#define NME_WPWAITTHRESHOLD     180

// pickups
#define MAX_STATEFRAMETHRESHOLD 5
#define MIN_STATETHRESHOLD      180
#define MAX_STATETHRESHOLD      360
#define MAX_LINGERBASE          100
#define MAX_PICKUPS             6

#define PICKUP_TURNSPEED        5
#define PICKUP_SPEED            2.0f

#define BOMBREDUCE_MINTHRESHOLD     5
#define BOMBREDUCE_MAXTHRESHOLD     30

#define BOMBSCATTER_MAXSHOTS        20
#define BOMBSCATTER_THRESHOLD       7

// guns
#define MULTISHOT_ANGLESPREAD       15
#define MULTISHOT_SHOTCOUNT         3
#define MULTISHOT_MAXAMMO           30

// volume
#define MP3_VOLUMEMULTIPLIER        -20
#define SND_VOLUMEMULTIPLIER        -10

// help pages
#define MAX_HELPPAGES               8

// portrait
#define PORTRAIT_THRESHOLD          5
#define PORTRAIT_FRAMESEED          8

// options pages
#define MAX_OPTPAGES    4

// options
#define FILE_OPTIONS                "opt/options.opt"
#define DEFAULT_SOUNDLEVEL          100
#define DEFAULT_MUSICLEVEL          100
#define DEFAULT_MOUSESPEED          1.0f
#define DEFAULT_PHYSICSTYPE         1
#define DEFAULT_PLAYERTHRUSTING     1
#define DEFAULT_PLAYERSHOTTRAILS    1
#define DEFAULT_PLAYERSHOTEXPL      1
#define DEFAULT_PLAYEREXPL          1
#define DEFAULT_NMESHOTTRAILS       1
#define DEFAULT_NMESHOTEXPL         1
#define DEFAULT_NMEEXPL             1

#define ISBONUSROUND()              ( m_round >= ( MAX_ROUNDS - 1 ) )

//--------------------------------------------------------------------------------
// structures
//--------------------------------------------------------------------------------
typedef enum pickup_e {
      PICKUP_GUNMULTISHOT
    , PICKUP_GUNCHARGEUP
    , PICKUP_BOMBREDUCE
    , PICKUP_BOMBSCATTER
    , PICKUP_BONUSEXTRALIFE
    , PICKUP_BONUSFULLSHIELD
}
pickup_t;

typedef enum helppage_e {
      PAGE_GAMEPLAY = 0
    , PAGE_PLAYER
    , PAGE_ENEMY
    , PAGE_ASTEROIDS
    , PAGE_PICKUPS
    , PAGE_GUNS
    , PAGE_BOMBS
    , PAGE_BONUSES
}
helppage_t;

typedef struct point_s {
    float x1, y1;
    float x2, y2;
}
point_t;

typedef struct objcol_s {
    int activepieces;
    obj_t astpieces[ MAX_ASTPIECES ][ MAX_ASTS ];
}
objcol_t;

typedef struct ptle_s {
    state_t state;
    float x, y;
    float spdx, spdy;
    float accelx, accely;
    unsigned char r, g, b;
    unsigned char incr, incg, incb;
    unsigned char dstr, dstg, dstb;
}
ptle_t;

typedef struct clstr_s {
    state_t state;
    state_t savedstate;
    obj_t *obj;

    int addrate;
    int recyclecount;
    int ptlecount;

    int minr, maxr;
    int ming, maxg;
    int minb, maxb;

    ptle_t ptles[ MAX_PTLESINCLSTR ];
}
clstr_t;

typedef struct marqueeline_s {
    LPDIRECTDRAWSURFACE7 dds;
    int length;
}
marqueeline_t;

typedef struct marquee_s {
    state_t state;
    int count;
    int offset;
    int counter;
    marqueeline_t lines[ MAX_MARQUEELINES ];
}
marquee_t;

typedef struct tablerow_s {
    char garbage1[ 12 ];
    char name[ MAX_SCORENAMELEN ];
    char garbage2[ 7 ];
    time_t date;
    char garbage3[ 6 ];
    int level;
    char garbage4[ 7 ];
    int score;
}
tablerow_t;

typedef struct highscoretable_s {
    tablerow_t rows[ MAX_HIGHSCORES ];
}
highscoretable_t;

//--------------------------------------------------------------------------------
// globals
//--------------------------------------------------------------------------------
char g_gameerror[ 1024 ];

//--------------------------------------------------------------------------------
// private data
//--------------------------------------------------------------------------------
static const char *m_version = "v1.21";

static point_t m_fpoints[ 36 ] = {
      {  7.0f,  9.0f,  7.0f, 22.0f }, {  8.0f,  8.0f,  6.0f, 21.0f }, { 10.0f,  7.0f,  5.0f, 19.0f }
    , { 12.0f,  5.0f,  5.0f, 17.0f }, { 14.0f,  5.0f,  5.0f, 15.0f }, { 15.0f,  5.0f,  5.0f, 14.0f }
    , { 17.0f,  5.0f,  6.0f, 10.0f }, { 19.0f,  5.0f,  6.0f, 10.0f }, { 21.0f,  6.0f,  8.0f,  8.0f }
    , { 22.0f,  7.0f,  9.0f,  7.0f }, { 23.0f,  8.0f, 11.0f,  6.0f }, { 25.0f, 10.0f, 12.0f,  5.0f }
    , { 25.0f, 11.0f, 14.0f,  5.0f }, { 26.0f, 14.0f, 16.0f,  5.0f }, { 26.0f, 15.0f, 17.0f,  5.0f }
    , { 26.0f, 17.0f, 19.0f,  5.0f }, { 26.0f, 19.0f, 21.0f,  6.0f }, { 25.0f, 21.0f, 23.0f,  8.0f }
    , { 24.0f, 22.0f, 24.0f,  9.0f }, { 23.0f, 23.0f, 25.0f, 11.0f }, { 21.0f, 25.0f, 26.0f, 12.0f }
    , { 20.0f, 25.0f, 26.0f, 14.0f }, { 18.0f, 25.0f, 26.0f, 16.0f }, { 16.0f, 27.0f, 26.0f, 17.0f }
    , { 14.0f, 26.0f, 26.0f, 19.0f }, { 12.0f, 26.0f, 24.0f, 21.0f }, { 11.0f, 25.0f, 23.0f, 23.0f }
    , {  9.0f, 24.0f, 22.0f, 24.0f }, {  8.0f, 23.0f, 21.0f, 25.0f }, {  6.0f, 24.0f, 18.0f, 25.0f }
    , {  6.0f, 19.0f, 17.0f, 26.0f }, {  5.0f, 17.0f, 15.0f, 26.0f }, {  5.0f, 16.0f, 14.0f, 25.0f }
    , {  6.0f, 14.0f, 12.0f, 25.0f }, {  6.0f, 12.0f, 11.0f, 25.0f }, {  6.0f, 10.0f,  8.0f, 23.0f }
};

static point_t m_bpoints[ 36 ] = {
      { 17.0f,  9.0f, 17.0f, 22.0f }, { 18.0f,  9.0f, 15.0f, 22.0f }, { 19.0f, 10.0f, 14.0f, 22.0f }
    , { 20.0f, 10.0f, 13.0f, 22.0f }, { 20.0f, 11.0f, 12.0f, 21.0f }, { 21.0f, 12.0f, 11.0f, 20.0f }
    , { 21.0f, 13.0f, 10.0f, 20.0f }, { 22.0f, 14.0f, 10.0f, 19.0f }, { 22.0f, 15.0f,  9.0f, 18.0f }
    , { 22.0f, 17.0f,  9.0f, 17.0f }, { 22.0f, 18.0f,  9.0f, 15.0f }, { 21.0f, 19.0f,  9.0f, 14.0f }
    , { 21.0f, 20.0f, 10.0f, 12.0f }, { 20.0f, 20.0f, 10.0f, 12.0f }, { 19.0f, 21.0f, 11.0f, 11.0f }
    , { 18.0f, 21.0f, 11.0f, 10.0f }, { 17.0f, 22.0f, 12.0f, 10.0f }, { 16.0f, 22.0f, 13.0f,  9.0f }
    , { 14.0f, 22.0f, 14.0f,  9.0f }, { 13.0f, 22.0f, 16.0f,  9.0f }, { 12.0f, 22.0f, 17.0f,  9.0f }
    , { 11.0f, 21.0f, 18.0f, 10.0f }, { 11.0f, 20.0f, 19.0f, 10.0f }, { 10.0f, 19.0f, 20.0f, 11.0f }
    , { 10.0f, 18.0f, 20.0f, 12.0f }, {  9.0f, 17.0f, 21.0f, 12.0f }, {  9.0f, 16.0f, 22.0f, 13.0f }
    , {  9.0f, 14.0f, 22.0f, 14.0f }, {  9.0f, 13.0f, 22.0f, 16.0f }, { 10.0f, 12.0f, 22.0f, 17.0f }
    , { 10.0f, 11.0f, 21.0f, 18.0f }, { 11.0f, 11.0f, 21.0f, 19.0f }, { 12.0f, 10.0f, 20.0f, 20.0f }
    , { 13.0f,  9.0f, 20.0f, 21.0f }, { 15.0f, 10.0f, 19.0f, 21.0f }, { 16.0f, 10.0f, 18.0f, 22.0f }
};

static obj_t m_nmeships[ MAX_NMESHIPS ];

static obj_t m_lblDirectional;
static obj_t m_lblSliding;

static obj_t m_butoptpages[ MAX_OPTPAGES ];
static obj_t m_menuoptions2;

static options_t m_options;

static int m_waitforplayer;
static int m_playerready;
static int m_currentpage;
static obj_t m_helppages[ MAX_HELPPAGES ];

static obj_t m_risingscore;

static obj_t m_portraitplyr;
static obj_t m_portraitnme;

static int m_lowshieldcounter;
static int m_lowshield;

static int m_menuactive;
static int m_gameactive;
static int m_popupactive;
static int m_newgamepending;
static state_t m_hudstate;
static state_t m_hudsavedstate;

static obj_t m_lblsound;
static obj_t m_lblmusic;
static obj_t m_lblmouse;
static obj_t m_lblshipphysics;
static obj_t m_lblplyrshottrails;
static obj_t m_lblplyrshotexpl;
static obj_t m_lblplyrexpl;
static obj_t m_lblnmeshottrails;
static obj_t m_lblnmeshotexpl;
static obj_t m_lblnmeexpl;
static obj_t m_lblthrusting;

static char m_name[ MAX_SCORENAMELEN ];

static int m_cheatgod;
static int m_cheatnocollide;
static int m_cheatfinishscreen;
static int m_cheatlevelwarp;
static int m_cheatroundwarp;

static time_t m_timemark;
static time_t m_timeelapsed;
static int m_timebonus;
static int m_accuracybonus;
static int m_accuracy;
static int m_statsshotsfired;
static int m_statsshotshit;
static int m_statsshotsmissed;

static obj_t *lastfocus;

static obj_t m_scorerows[ MAX_HIGHSCORES ];
static highscoretable_t m_highscoretable;

static int m_fonttitle;
static int m_fontbody;
static int m_fontbodyred;
static int m_fontscores;
static int m_fontdamage;

static obj_t m_smallestast0[ MAX_SMALLESTTYPES ][ MAX_SMALLESTDEBRIS ];
static obj_t m_smallestast1[ MAX_SMALLESTTYPES ][ MAX_SMALLESTDEBRIS ];
static obj_t m_smallestplyr[ MAX_SMALLESTTYPES ][ MAX_SMALLESTDEBRIS ];
static obj_t m_smallestnme[ MAX_SMALLESTTYPES ][ MAX_SMALLESTDEBRIS ];

float m_astmass[] = { 1.0f, 0.75f, 0.50f, 0.25f };
float m_chargeupmass[] = { 1.0f, 0.75f, 0.50f };
int m_chargeupsize[] = { 32, 24, 16 };

static obj_t m_leveltitles[ MAX_LEVELS ];
static obj_t m_roundtitles[ MAX_ROUNDS ];

static obj_t m_entername;
static obj_t m_enternamecursor;

static obj_t m_title;
static obj_t m_menubar;
static obj_t m_menugame;
static obj_t m_menuscores;
static obj_t m_menuscores2;
static obj_t m_menuhelp;
static obj_t m_menuhelp2;
static obj_t m_menuabout;
static obj_t m_menuoptions;
static obj_t m_mousecursor;
static obj_t *m_mousecapture;

static obj_t m_butgame;
static obj_t m_butscores;
static obj_t m_butoptions;
static obj_t m_buthelp;
static obj_t m_butexit;
static obj_t m_butnew;
static obj_t m_butresume;
static obj_t m_butstop;
static obj_t m_butviewhs;
static obj_t m_butreset;
static obj_t m_butviewhp;
static obj_t m_butviewopt;
static obj_t m_butabout;
static obj_t m_butback;
static obj_t m_butnext;
static obj_t m_butprev;

static obj_t m_finishscreen;
static obj_t m_gameover;

static int m_level;
static int m_round;

static obj_t m_player;
static obj_t m_stats;
static obj_t m_shots[ MAX_SHOTS * 3 ];          // x3 because of multishot pickup
static obj_t m_chargeupshots[ MAX_CHARGEUPSIZES ][ MAX_CHARGEUPSHOTS ];
static obj_t m_scattershots[ BOMBSCATTER_MAXSHOTS ];
static obj_t m_nmeshots[ MAX_NMESHOTS ];
static int m_didanyleave;
static objcol_t m_astcols[ 2 ];
static int m_sounds[ MAX_GAMESOUNDS ];
static int m_menutrack;
static int m_creditstrack;
static int m_leveltrackidx;
static int m_trackcount;
static int m_tracks[ MAX_GAMETRACKS ];
static obj_t m_debris[ MAX_DEBRIS ][ MAX_ACTIVEDEBRIS ];
static obj_t m_nmedebris[ MAX_DEBRIS ][ MAX_ACTIVEDEBRIS ];

static obj_t m_sldrsound;
static obj_t m_sldrboxsound;
static obj_t m_sldrmusic;
static obj_t m_sldrboxmusic;
static obj_t m_sldrmouse;
static obj_t m_sldrboxmouse;

static obj_t m_drpshipphysics;
static obj_t m_drpshipphysicssel;
static int m_drpshipdropped;
static obj_t m_lstshipphysics;
static obj_t m_butsliding;
static obj_t m_butdirectional;

static obj_t m_chkplyrthrusting;
static obj_t m_chkplyrshottrails;
static obj_t m_chkplyrshotexpl;
static obj_t m_chkplyrexpl;
static obj_t m_chknmeshottrails;
static obj_t m_chknmeshotexpl;
static obj_t m_chknmeexpl;

static obj_t *m_proppagecontrols[] = {
      &m_sldrsound, &m_sldrboxsound, &m_sldrmusic, &m_sldrboxmusic, &m_sldrmouse, &m_sldrboxmouse
    , &m_drpshipphysics, &m_lstshipphysics
    , &m_chkplyrthrusting, &m_chkplyrshottrails, &m_chkplyrshotexpl
    , &m_chkplyrexpl, &m_chknmeshottrails, &m_chknmeshotexpl, &m_chknmeexpl
    , &m_lblDirectional, &m_lblSliding
    , NULL
};

static obj_t *m_buttons[] = {
      &m_butgame,     &m_butscores, &m_butoptions, &m_buthelp, &m_butexit   // menubar buttons
    , &m_butnew,      &m_butresume, &m_butstop                              // game menu buttons
    , &m_butviewhs,   &m_butreset                                           // scores menu buttons
    , &m_butviewopt                                                         // options
    , &m_butviewhp,   &m_butabout                                           // help menu buttons
    , &m_butback,     &m_butnext,   &m_butprev                              // other buttons

    , &m_sldrboxsound, &m_sldrboxmusic, &m_sldrboxmouse, &m_drpshipphysics
    , &m_butsliding, &m_butdirectional

    , NULL
};

static obj_t *m_multistate[] = {
      &m_butoptpages[ 0 ], &m_butoptpages[ 1 ], &m_butoptpages[ 2 ], &m_butoptpages[ 3 ]
    , &m_chkplyrthrusting, &m_chkplyrshottrails, &m_chkplyrshotexpl
    , &m_chkplyrexpl, &m_chknmeshottrails, &m_chknmeshotexpl, &m_chknmeexpl
    , NULL
};

static obj_t *m_currentpickup;
static obj_t m_pickups[ MAX_PICKUPS ];

static clstr_t m_plyrtrail1;
static clstr_t m_plyrtrail2;

static clstr_t m_plyrexp;
static clstr_t m_shottrails[ MAX_SHOTTRAILS * 3 ];
static clstr_t m_shotrichochets[ MAX_SHOTRICHOCHETS ];
static clstr_t m_chargeupspit;
static clstr_t m_chargeupshotrichochets[ MAX_SHOTRICHOCHETS ];
static clstr_t m_chargeupshottrails[ MAX_SHOTTRAILS ];
static clstr_t m_nmeexp[ MAX_SHOTRICHOCHETS ];
static clstr_t m_nmeshottrails[ MAX_NMESHOTTRAILS ];
static clstr_t m_nmeshotrichochets[ MAX_NMESHOTRICHOCHETS ];
static clstr_t m_plyrcollide[ MAX_SHOTRICHOCHETS ];
static clstr_t m_starfield;

static state_t m_statenme;
static float m_nmemaxspeed;
static int m_nmecounter;
static int m_nmethreshold;
static int m_nmeactiveships;
static int m_nmefov;
static int m_nmecooldown;
static int m_nmeshipcount;

static state_t m_nmestatethink;
static state_t m_nmepushstatethink;

static marquee_t m_marquee;

static const char *m_soundfilenames[] = {
      "wav/astexpl0.wav"
    , "wav/astexpl1.wav"
    , "wav/asthit.wav"
    , "wav/plyrexpl.wav"
    , "wav/plyrshlddn.wav"
    , "wav/plyrshldup.wav"
    , "wav/plyrsht.wav"
    , "wav/click.wav"
    , "wav/cheat.wav"
    , "wav/astbnce.wav"
    , "wav/plyrsht2.wav"
    , "wav/plyrchgup.wav"
    , "wav/applause.wav"
    , "wav/thrusting.wav"
};

static int m_astsizelookup[] = {
    64, 48, 48, 32, 32, 32, 32
};

static int m_aststartlookup1[] = { 1, 1, 2, 2, 2, 3, 3, 3, 3, 3 };
static int m_aststartlookup2[] = { 1, 1, 1, 2, 2, 2, 3, 3, 3, 3 };
static int m_aststartlookup3[] = { 0, 1, 1, 1, 2, 2, 2, 3, 3, 3 };
static int m_aststartlookup4[] = { 0, 0, 1, 1, 1, 2, 2, 2, 3, 3 };
static int m_aststartlookup5[] = { 0, 0, 0, 1, 1, 1, 2, 2, 2, 3 };

static int *m_aststarts[] = {
      m_aststartlookup1, m_aststartlookup2, m_aststartlookup3
    , m_aststartlookup4, m_aststartlookup5, NULL
};

//--------------------------------------------------------------------------------
// private prototypes
//--------------------------------------------------------------------------------
static int game_cloneobject( obj_t *dstobj, obj_t *srcobj );

static void options_read( const char *filename, options_t *options );
static void options_write( const char *filename, options_t *options );
static void options_default( options_t *options );

static int game_rand( int min, int max );

static int game_incframe( obj_t *obj );
static int game_decframe( obj_t *obj );
static int game_maptoscreen( obj_t *obj );

static int game_objcollide( obj_t *src, obj_t *dst );
static int game_pointinbox( int ptx, int pty, int bxx, int bxy, int bw, int bh );

static void game_resolveshot( obj_t *src, obj_t *dst );

static int game_getangle( float x1, float y1, float x2, float y2 );
static int game_anglediff( int angle1, int angle2 );

static void portrait_think( obj_t *obj );
static void portrait_thinkpickup( obj_t *obj );
static void portrait_draw( obj_t *obj );
static void portrait_deactivateall( void );
static void portrait_activate( void );

static void player_think( void );
static void player_move( void );
static int player_collide( int flag );
static void player_draw( void );
static void player_awardpickup( void );
static void player_lookupframe( obj_t *obj );
static void player_fireshot( void );
static void player_reset( void );
static void player_explode( obj_t *obj );

static void shot_think( obj_t *obj );
static void shot_move( obj_t *obj );
static void shot_collide( obj_t *obj );
static void shot_draw( obj_t *obj );

static void nmeshot_collide( obj_t *obj );

static void chargeupshot_think( obj_t *obj );
static void chargeupshot_collide( obj_t *obj );
static int chargeupshot_damage( obj_t *shot, obj_t *obj );

static void asteroid_think( obj_t *obj );
static void asteroid_move( obj_t *obj );
static void asteroid_collide( obj_t *obj, int subtract );
static void asteroid_draw( obj_t *obj );

static void debris_spawn( obj_t *src, int type );
static void debris_think( obj_t *obj );

static void clstr_begindraw( unsigned char **buffer, long *numBytesPerScanline );
static void clstr_enddraw( void );
static void clstr_draw( clstr_t *clstr, unsigned char *buffer, long numBytesPerScanline );
static void clstr_move( clstr_t *clstr );
static void clstr_collide( clstr_t *clstr );

static void shottrail_init( clstr_t *clstr, int addrate
                          , int minr, int maxr
                          , int ming, int maxg
                          , int minb, int maxb
                          , int desthue, int inc );

static void shottrail_think( clstr_t *clstr );
static void shottrail_move( clstr_t *clstr );
static void shottrail_add( clstr_t *clstr );
static void shottrail_setactive( clstr_t *clstr, obj_t *obj, int minr, int maxr, int ming, int maxg, int minb, int maxb, int desthue, int inc );

static void plyrtrail_think( clstr_t *clstr );
static void plyrtrail_add1( clstr_t *clstr, obj_t *obj, int flag
                          , int minr, int maxr
                          , int ming, int maxg
                          , int minb, int maxb );
static void plyrtrail_add2( clstr_t *clstr, obj_t *obj, int flag
                          , int minr, int maxr
                          , int ming, int maxg
                          , int minb, int maxb );

static void shotrichochet_init( clstr_t *clstr
                              , int angle, float x, float y
                              , int minr, int maxr
                              , int ming, int maxg
                              , int minb, int maxb
                              , int desthue, int inc );

static void shotrichochet_think( clstr_t *clstr );
static void shotrichochet_add( clstr_t *clstr );
static void shotrichochet_setactive( clstr_t *clstr
                                   , int angle, float x, float y
                                   , int minr, int maxr
                                   , int ming, int maxg
                                   , int minb, int maxb
                                   , int desthue, int inc );

static void spitparticles_think( clstr_t *clstr );

static int game_start( void );
static int game_new( void );
static void game_stats( void );
static int game_pause( void );
static int game_resume( void );
static int game_stop( void );

static int help_view( void );
static int options_view( void );
static int options_viewpage( int page );
static int about_view( void );
static int menu_view( void );

static void menu_drawhelppage0( void );
static void menu_drawhelppage1( void );
static void menu_drawhelppage2( void );
static void menu_drawhelppage3( void );
static void menu_drawhelppage4( void );
static void menu_drawhelppage5( void );
static void menu_drawhelppage6( void );
static void menu_drawhelppage7( void );

static void (*func_drawhelppages[ MAX_HELPPAGES ])( void ) = {
      menu_drawhelppage0, menu_drawhelppage1, menu_drawhelppage2
    , menu_drawhelppage3, menu_drawhelppage4, menu_drawhelppage5
    , menu_drawhelppage6, menu_drawhelppage7
};

static void menu_draw( obj_t *obj );
static void button_draw( obj_t *obj );
static void button_draw2( obj_t *obj );
static void button_execute( obj_t *obj, int popup );

static void sldr_move( obj_t *obj );

static void cursor_think( void );
static void cursor_move( void );
static void cursor_collide( void );
static void cursor_draw( void );

static void starfield_init( int direction );
static void starfield_think( void );
static void starfield_collide( void );

static int marquee_init( void );
static void marquee_shutdown( void );
//static void marquee_move( void );
static void marquee_draw( void );

static int scores_read( const char *filename );
static int scores_write( const char *filename );
static int scores_insert( const char *name, int level, int score );
static int scores_isnewscore( int score );
static int scores_view( void );
static int scores_reset( void );
static int scores_build( void );
static int scores_iscorruptfile( void );

static void finishscreen_activate( void );
static void finishscreen_draw( void );

static void leveltitle_think( void );
static void leveltitle_render( void );

static void gameover_init( void );
static void gameover_think( void );
static void gameover_draw( void );

static void stats_think( void );
static void stats_draw( void );

static int entername_init( void );
static int entername_think( void );
static int entername_draw( void );

static void reflect_headingup( obj_t *obj );
static void reflect_headingdown( obj_t *obj );
static void reflect_headingleft( obj_t *obj );
static void reflect_headingright( obj_t *obj );
static void reflect_movingobj( obj_t *obj );
static void reflect_unjam( obj_t *obj );

static void risingscore_init( obj_t *obj, int largefont, float x, float y );
static void risingscore_think( obj_t *obj );
static void risingscore_move( obj_t *obj );

static void nme_explode( obj_t *nme, obj_t *obj );
static void nme_fireshot( obj_t *obj );
static void nme_think( obj_t *obj );
static void nme_move( obj_t *obj );
static void nme_collide( obj_t *obj );
static void nme_draw( obj_t *obj );

static void nme_mgrinit( void );
static void nme_mgrreset( void );
static void nme_mgrthink( void );
static void nme_mgrsetwaypoints( obj_t *obj, int *type );

static void pickup_init( float x, float y, int type );
static int pickup_choose( void );
static void pickup_change( void );
static void pickup_think( obj_t *obj );
static void pickup_move( obj_t *obj );
static void pickup_collide( obj_t *obj );
static void pickup_draw( obj_t *obj );

static int bombreduce_canreduce( void );

static void bombscatter_init( void );
static void bombscatter_think( obj_t *obj );

static void bonus_levelone( void );
static void bonus_leveltwo( void );
static void bonus_levelthree( void );
static void bonus_levelfour( void );
static void bonus_levelfive( void );

static void (*func_bonuslevels[ MAX_LEVELS ])( void ) = {
      bonus_levelone, bonus_leveltwo,   bonus_levelthree, bonus_levelfour, bonus_levelfive
    , bonus_levelone, bonus_leveltwo,   bonus_levelthree, bonus_levelfour, bonus_levelfive
};

//--------------------------------------------------------------------------------
// public functions
//--------------------------------------------------------------------------------
int game_init( void )
{
    int i;
    float f;
    int astcol, piece;
    char buf[ 128 ];

    sys_clearsurface( g_backbuffer, 0 );
    sys_text( RGB( 255, 255, 255 ), 0, SCREEN_WIDTH - 128, SCREEN_HEIGHT - 32, "loading bitmaps..." );
    sys_draw();

    sys_logwrite( "game_init: loading game bitmap files\n" );

    // the menu objects
    if( !game_createobject( &m_title, 256, 256, "bmp/title.bmp" ) ) {
        return 0;
    }
    game_setframe( &m_title, 0 );
    m_title.x = SCREEN_HALFWIDTH_f;
    m_title.y = SCREEN_HALFHEIGHT_f - 50.0f;

    if( !game_createobject( &m_menubar, 320, 32, "bmp/menubar.bmp" ) ) {
        return 0;
    }
    game_setframe( &m_menubar, 0 );
    m_menubar.x = SCREEN_HALFWIDTH_f;
    m_menubar.y = m_title.y + m_title.halfheight + 20.0f;

    if( !game_createobject( &m_menugame, 64, 96, "bmp/menugame.bmp" ) ) {
        return 0;
    }
    game_setframe( &m_menugame, 0 );
    m_menugame.x = (m_menubar.x - m_menubar.halfwidth) + m_menugame.halfwidth;
    m_menugame.y = (m_menubar.y + m_menubar.halfheight) + m_menugame.halfheight;

    if( !game_createobject( &m_menuscores, 64, 64, "bmp/menuscores.bmp" ) ) {
        return 0;
    }
    game_setframe( &m_menuscores, 0 );
    m_menuscores.x = (m_menugame.x + m_menugame.halfwidth) + m_menuscores.halfwidth;
    m_menuscores.y = (m_menubar.y + m_menubar.halfheight) + m_menuscores.halfheight;

    if( !game_createobject( &m_menuscores2, 416, 256, "bmp/menuscores2.bmp" ) ) {
        return 0;
    }
    game_setframe( &m_menuscores2, 0 );
    m_menuscores2.x = SCREEN_HALFWIDTH_f;
    m_menuscores2.y = SCREEN_HALFHEIGHT_f;

    if( !game_createobject( &m_menuoptions, 64, 32, "bmp/menuoptions.bmp" ) ) {
        return 0;
    }
    game_setframe( &m_menuoptions, 0 );
    m_menuoptions.x = (m_menuscores.x + m_menuscores.halfwidth) + m_menuoptions.halfwidth;
    m_menuoptions.y = (m_menubar.y + m_menubar.halfheight) + m_menuoptions.halfheight;

    if( !game_createobject( &m_menuhelp, 64, 64, "bmp/menuhelp.bmp" ) ) {
        return 0;
    }
    game_setframe( &m_menuhelp, 0 );
    m_menuhelp.x = (m_menuoptions.x + m_menuoptions.halfwidth) + m_menuhelp.halfwidth;
    m_menuhelp.y = (m_menubar.y + m_menubar.halfheight) + m_menuhelp.halfheight;

    if( !game_createobject( &m_menuabout, 64, 32, "bmp/menuabout.bmp" ) ) {
        return 0;
    }
    game_setframe( &m_menuabout, 0 );
    m_menuabout.x = (m_menuscores2.x + m_menuscores2.halfwidth) - m_menuabout.halfwidth;
    m_menuabout.y = (m_menuscores2.y + m_menuscores2.halfheight) + m_menuabout.halfheight + 10.0f;

    f = SCREEN_HALFWIDTH_f - 95.0f;

    for( i = 0; i < MAX_OPTPAGES; i++, f += 63.0f ) {
        sprintf_s( buf, 128, "bmp/butoptpage%d.bmp", i );
        if( !game_createobject( &m_butoptpages[ i ], 64, 32, buf ) ) {
            return 0;
        }
        m_butoptpages[ i ].state = STATE_PREBIRTH;
        game_setframe( &m_butoptpages[ i ], 0 );
        m_butoptpages[ i ].x = f;
        m_butoptpages[ i ].y = SCREEN_HALFHEIGHT_f - 137.0f;
    }

    if( !game_createobject( &m_menuoptions2, 256, 256, "bmp/menuoptions2.bmp" ) ) {
        return 0;
    }
    game_setframe( &m_menuoptions2, 0 );
    m_menuoptions2.x = SCREEN_HALFWIDTH_f;
    m_menuoptions2.y = SCREEN_HALFHEIGHT_f;

    if( !game_createobject( &m_mousecursor, 16, 16, "bmp/mse_crsr.bmp" ) ) {
        return 0;
    }
    game_setframe( &m_mousecursor, 0 );
    m_mousecursor.x = SCREEN_HALFWIDTH_f;
    m_mousecursor.y = SCREEN_HALFHEIGHT_f;

    // the buttons
    if( !game_createobject( &m_butgame, 64, 32, "bmp/butgame.bmp" ) ) {
        return 0;
    }
    game_setframe( &m_butgame, 0 );
    m_butgame.x = m_menugame.x;
    m_butgame.y = m_menubar.y;

    if( !game_createobject( &m_butscores, 64, 32, "bmp/butscores.bmp" ) ) {
        return 0;
    }
    game_setframe( &m_butscores, 0 );
    m_butscores.x = m_menuscores.x;
    m_butscores.y = m_menubar.y;

    if( !game_createobject( &m_butoptions, 64, 32, "bmp/butoptions.bmp" ) ) {
        return 0;
    }
    game_setframe( &m_butoptions, 0 );
    m_butoptions.x = m_menuoptions.x;
    m_butoptions.y = m_menubar.y;

    if( !game_createobject( &m_buthelp, 64, 32, "bmp/buthelp.bmp" ) ) {
        return 0;
    }
    game_setframe( &m_buthelp, 0 );
    m_buthelp.x = m_menuhelp.x;
    m_buthelp.y = m_menubar.y;

    if( !game_createobject( &m_butexit, 64, 32, "bmp/butexit.bmp" ) ) {
        return 0;
    }
    game_setframe( &m_butexit, 0 );
    m_butexit.x = (m_menuhelp.x + m_menuhelp.halfwidth) + m_butexit.halfwidth;
    m_butexit.y = m_menubar.y;

    if( !game_createobject( &m_butnew, 64, 32, "bmp/butnew.bmp" ) ) {
        return 0;
    }
    game_setframe( &m_butnew, 0 );
    m_butnew.x = m_menugame.x;
    m_butnew.y = m_menugame.y - m_butnew.height;

    if( !game_createobject( &m_butresume, 64, 32, "bmp/butresume.bmp" ) ) {
        return 0;
    }
    game_setframe( &m_butresume, 0 );
    m_butresume.x = m_menugame.x;
    m_butresume.y = m_menugame.y;

    if( !game_createobject( &m_butstop, 64, 32, "bmp/butstop.bmp" ) ) {
        return 0;
    }
    game_setframe( &m_butstop, 0 );
    m_butstop.x = m_menugame.x;
    m_butstop.y = m_menugame.y + m_butresume.height;

    if( !game_createobject( &m_butviewhs, 64, 32, "bmp/butviewhs.bmp" ) ) {
        return 0;
    }
    game_setframe( &m_butviewhs, 0 );
    m_butviewhs.x = m_menuscores.x;
    m_butviewhs.y = m_menuscores.y - m_butviewhs.halfheight;

    if( !game_createobject( &m_butreset, 64, 32, "bmp/butreset.bmp" ) ) {
        return 0;
    }
    game_setframe( &m_butreset, 0 );
    m_butreset.x = m_menuscores.x;
    m_butreset.y = m_menuscores.y + m_butviewhs.halfheight;

    if( !game_createobject( &m_butviewopt, 64, 32, "bmp/butviewopt.bmp" ) ) {
        return 0;
    }
    game_setframe( &m_butviewopt, 0 );
    m_butviewopt.x = m_menuoptions.x;
    m_butviewopt.y = m_menuoptions.y;

    if( !game_createobject( &m_butviewhp, 64, 32, "bmp/butviewhelp.bmp" ) ) {
        return 0;
    }
    game_setframe( &m_butviewhp, 0 );
    m_butviewhp.x = m_menuhelp.x;
    m_butviewhp.y = m_menuhelp.y - m_butviewhp.halfheight;

    if( !game_createobject( &m_butabout, 64, 32, "bmp/butviewab.bmp" ) ) {
        return 0;
    }
    game_setframe( &m_butabout, 0 );
    m_butabout.x = m_menuhelp.x;
    m_butabout.y = m_menuhelp.y + m_butabout.halfheight;

    if( !game_createobject( &m_butback, 64, 32, "bmp/butback.bmp" ) ) {
        return 0;
    }
    game_setframe( &m_butback, 0 );
    m_butback.x = (m_menuscores2.x + m_menuscores2.halfwidth) - m_butback.halfwidth;
    m_butback.y = (m_menuscores2.y + m_menuscores2.halfheight) + m_butback.halfheight + 10.0f;

    if( !game_createobject( &m_butnext, 64, 32, "bmp/butnext.bmp" ) ) {
        return 0;
    }
    game_setframe( &m_butnext, 0 );
    m_butnext.x = m_butback.x - m_butnext.width;
    m_butnext.y = m_butback.y;

    if( !game_createobject( &m_butprev, 64, 32, "bmp/butprev.bmp" ) ) {
        return 0;
    }
    game_setframe( &m_butprev, 0 );
    m_butprev.x = m_butnext.x - m_butprev.width;
    m_butprev.y = m_butback.y;

    if (!game_createobject(&m_menuhelp2, 192, 32, "bmp/menuhelp2.bmp")) {
        return 0;
    }
    game_setframe(&m_menuhelp2, 0);
    m_menuhelp2.x = m_butnext.x;
    m_menuhelp2.y = m_butnext.y;

#if !DEMO_VERSION
    // the finishing screen
    if( !game_createobject( &m_finishscreen, SCREEN_WIDTH, SCREEN_HEIGHT, "bmp/finish.bmp" ) ) {
        return 0;
    }
    game_setframe( &m_finishscreen, 0 );
    m_finishscreen.x = SCREEN_WIDTH  / 2.0f;
    m_finishscreen.y = SCREEN_HEIGHT / 2.0f;
    m_finishscreen.dst.left   = 0;
    m_finishscreen.dst.right  = SCREEN_WIDTH;
    m_finishscreen.dst.top    = 0;
    m_finishscreen.dst.bottom = SCREEN_HEIGHT;
#endif

    // the gameover bitmap
    if( !game_createobject( &m_gameover, 256, 256, "bmp/gameover.bmp" ) ) {
        return 0;
    }
    game_setframe( &m_gameover, 0 );
    m_gameover.x = SCREEN_WIDTH  / 2.0f;
    m_gameover.y = SCREEN_HEIGHT / 2.0f;
    m_gameover.dst.left   = 192;
    m_gameover.dst.right  = 448;
    m_gameover.dst.top    = 112;
    m_gameover.dst.bottom = 368;

    // the 'enter your name' dialog
    if( !game_createobject( &m_entername, 192, 64, "bmp/entername.bmp" ) ) {
        return 0;
    }
    game_setframe( &m_entername, 0 );
    m_entername.x = SCREEN_WIDTH / 2.0f;
    m_entername.y = 425;

    if( !sys_createsurface( &m_enternamecursor.dds, 8, 16, 0 ) ) {
        strcpy_s( g_gameerror, 1024, g_syserror );
        return 0;
    }
    m_enternamecursor.width      = 8;
    m_enternamecursor.height     = 16;
    m_enternamecursor.halfwidth  = m_enternamecursor.width  / 2;
    m_enternamecursor.halfheight = m_enternamecursor.height / 2;
    m_enternamecursor.bmpwidth   = 8;
    game_setframe( &m_enternamecursor, 0 );
    sys_clearsurface(m_enternamecursor.dds, RGB(240, 240, 240));

#if DEMO_VERSION
    if( !game_createobject( &m_leveltitles[ 0 ], 192, 192, "bmp/level1.bmp" ) ) {
        return 0;
    }
    game_setframe( &m_leveltitles[ 0 ], 0 );
    m_leveltitles[ 0 ].x = SCREEN_WIDTH_f  / 2.0f;
    m_leveltitles[ 0 ].y = SCREEN_HEIGHT_f / 2.0f;
    if( !game_createobject( &m_leveltitles[ 6 ], 192, 192, "bmp/level7.bmp" ) ) {
        return 0;
    }
    game_setframe( &m_leveltitles[ 6 ], 0 );
    m_leveltitles[ 6 ].x = SCREEN_WIDTH_f  / 2.0f;
    m_leveltitles[ 6 ].y = SCREEN_HEIGHT_f / 2.0f;
#else
    // the level titles
    for( i = 0; i < MAX_LEVELS; i++ ) {
        sprintf_s( buf, 128, "bmp/level%d.bmp", i + 1 );
        if( !game_createobject( &m_leveltitles[ i ], 192, 192, buf ) ) {
            return 0;
        }
        game_setframe( &m_leveltitles[ i ], 0 );
        m_leveltitles[ i ].x = SCREEN_WIDTH_f  / 2.0f;
        m_leveltitles[ i ].y = SCREEN_HEIGHT_f / 2.0f;
    }
#endif
    // the round titles
    for( i = 0; i < MAX_ROUNDS; i++ ) {
        sprintf_s( buf, 128, "bmp/round%d.bmp", i + 1 );
        if( !game_createobject( &m_roundtitles[ i ], 192, 80, buf ) ) {
            return 0;
        }
        game_setframe( &m_roundtitles[ i ], 0 );
        m_roundtitles[ i ].x = SCREEN_WIDTH_f  / 2.0f;
        m_roundtitles[ i ].y = SCREEN_HEIGHT_f / 2.0f + 56.0f;
    }

    // portraits
    if( !game_createobject( &m_portraitplyr, 64, 48, "bmp/plyr_prt.bmp" ) ) {
        return 0;
    }
    m_portraitplyr.state = STATE_DEAD;
    m_portraitplyr.stateasteroid.framecounter = rand() % PORTRAIT_FRAMESEED;
    game_setframe( &m_portraitplyr, 0 );

    if( !game_createobject( &m_portraitnme, 64, 48, "bmp/nme_prt.bmp" ) ) {
        return 0;
    }
    m_portraitnme.state = STATE_DEAD;
    m_portraitnme.stateasteroid.framecounter = rand() % PORTRAIT_FRAMESEED;
    game_setframe( &m_portraitnme, 0 );


    // the player's ship
    if( !game_createobject( &m_player, 32, 32, "bmp/plyr.bmp" ) ) {
        return 0;
    }
    m_player.state                  = STATE_DEAD;
    m_player.stateplayer.livesleft  = PLAYER_LIVES - 1;
    m_player.mass                   = PLAYER_MASS;

    // the player's shots
    if( !game_createobject( &m_shots[ 0 ], 16, 16, "bmp/shot.bmp" ) ) {
        return 0;
    }
    m_shots[ 0 ].mass = SHOT_MASS;
    game_setframe( &m_shots[ 0 ], 0 );

    for( i = 1; i < MAX_SHOTS * 3; i++ ) {              // x3 coz of multishot
        game_cloneobject( &m_shots[ i ], &m_shots[ 0 ] );
    }

    // the scatterbomb shots
    if( !game_createobject( &m_scattershots[ 0 ], 16, 16, "bmp/shot.bmp" ) ) {
        return 0;
    }
    m_scattershots[ 0 ].mass = SHOT_MASS;
    game_setframe( &m_scattershots[ 0 ], 0 );

    for( i = 1; i < BOMBSCATTER_MAXSHOTS; i++ ) {
        game_cloneobject( &m_scattershots[ i ], &m_scattershots[ 0 ] );
    }

    // charge up shots
    for( astcol = 0; astcol < MAX_CHARGEUPSIZES; astcol++ ) {
        sprintf_s( buf, 128, "bmp/chargeup_pce%d.bmp", astcol );

        if( !game_createobject( &m_chargeupshots[ astcol ][ 0 ]
                              , m_chargeupsize[ astcol ]
                              , m_chargeupsize[ astcol ]
                              , buf ) ) {
            return 0;
        }
        m_chargeupshots[ astcol ][ 0 ].mass = m_chargeupmass[ astcol ];
        game_setframe( &m_chargeupshots[ astcol ][ 0 ], 0 );

        for( i = 1; i < MAX_CHARGEUPSHOTS; i++ ) {
            game_cloneobject( &m_chargeupshots[ astcol ][ i ], &m_chargeupshots[ astcol ][ 0 ] );
        }
    }

    // the enemy's shots
    if( !game_createobject( &m_nmeshots[ 0 ], 16, 16, "bmp/nmeshot.bmp" ) ) {
        return 0;
    }
    m_nmeshots[ 0 ].mass = NMESHOT_MASS;
    game_setframe( &m_nmeshots[ 0 ], 0 );

    for( i = 1; i < MAX_NMESHOTS; i++ ) {
        game_cloneobject( &m_nmeshots[ i ], &m_nmeshots[ 0 ] );
    }

    // the end of level stats dialog
    if( !game_createobject( &m_stats, 192, 192, "bmp/stats.bmp" ) ) {
        return 0;
    }
    game_setframe( &m_stats, 0 );
    m_stats.state = STATE_DEAD;
    m_stats.x = SCREEN_HALFWIDTH_f;
    m_stats.y = SCREEN_HALFHEIGHT_f;

    // the asteroid pieces
    for( astcol = 0; astcol < MAX_ASTCOLS; astcol++ ) {
        for( piece = 0; piece < MAX_ASTPIECES; piece++ ) {

            sprintf_s( buf, 128, "bmp/ast%d_pce%d.bmp", astcol, piece );

            if( !game_createobject( &m_astcols[ astcol ].astpieces[ piece ][ 0 ], m_astsizelookup[ piece ], m_astsizelookup[ piece ], buf ) ) {
                return 0;
            }
            m_astcols[ astcol ].astpieces[ piece ][ 0 ].mass = m_astmass[ piece / 2 ];
            game_setframe( &m_astcols[ astcol ].astpieces[ piece ][ 0 ], 0 );

            m_astcols[ astcol ].astpieces[ piece ][ 0 ].stateasteroid.type       = piece;
            m_astcols[ astcol ].astpieces[ piece ][ 0 ].stateasteroid.collection = astcol;

            for( i = 1; i < MAX_ASTS; i++ ) {
                game_cloneobject( &m_astcols[ astcol ].astpieces[ piece ][ i ]
                                , &m_astcols[ astcol ].astpieces[ piece ][ 0 ] );
            }
        }
    }

    // load all the debris
    for( astcol = 0; astcol < MAX_DEBRIS; astcol++ ) {
        sprintf_s( buf, 128, "bmp/plyr_pce%d.bmp", astcol );

        if( !game_createobject( &m_debris[ astcol ][ 0 ], 16, 16, buf ) ) { 
            return 0;
        }
        m_debris[ astcol ][ 0 ].mass = DEBRIS_MASS;
        game_setframe( &m_debris[ astcol ][ 0 ], 0 );

        for( i = 1; i < MAX_ACTIVEDEBRIS; i++ ) {
            game_cloneobject( &m_debris[ astcol ][ i ], &m_debris[ astcol ][ 0 ] );
        }
    }

    for( astcol = 0; astcol < MAX_DEBRIS; astcol++ ) {
        sprintf_s( buf, 128, "bmp/nme_pce%d.bmp", astcol );

        if( !game_createobject( &m_nmedebris[ astcol ][ 0 ], 16, 16, buf ) ) { 
            return 0;
        }
        m_nmedebris[ astcol ][ 0 ].mass = DEBRIS_MASS;
        game_setframe( &m_nmedebris[ astcol ][ 0 ], 0 );

        for( i = 1; i < MAX_ACTIVEDEBRIS; i++ ) {
            game_cloneobject( &m_nmedebris[ astcol ][ i ], &m_nmedebris[ astcol ][ 0 ] );
        }
    }

    for( piece = 0; piece < MAX_SMALLESTTYPES; piece++ ) {
        sprintf_s( buf, 128, "bmp/ast0_debris%d.bmp", piece + 1 );

        if( !game_createobject( &m_smallestast0[ piece ][ 0 ], 16, 16, buf ) ) { 
            return 0;
        }
        m_smallestast0[ piece ][ 0 ].mass = DEBRIS_MASS;
        game_setframe( &m_smallestast0[ piece ][ 0 ], 0 );

        for( i = 1; i < MAX_SMALLESTDEBRIS; i++ ) {
            game_cloneobject( &m_smallestast0[ piece ][ i ], &m_smallestast0[ piece ][ 0 ] );
        }
    }

    for( piece = 0; piece < MAX_SMALLESTTYPES; piece++ ) {
        sprintf_s( buf, 128, "bmp/ast1_debris%d.bmp", piece + 1 );

        if( !game_createobject( &m_smallestast1[ piece ][ 0 ], 16, 16, buf ) ) { 
            return 0;
        }
        m_smallestast1[ piece ][ 0 ].mass = DEBRIS_MASS;
        game_setframe( &m_smallestast1[ piece ][ 0 ], 0 );

        for( i = 1; i < MAX_SMALLESTDEBRIS; i++ ) {
            game_cloneobject( &m_smallestast1[ piece ][ i ], &m_smallestast1[ piece ][ 0 ] );
        }
    }

    for( piece = 0; piece < MAX_SMALLESTTYPES; piece++ ) {
        sprintf_s( buf, 128, "bmp/plyr_debris%d.bmp", piece + 1 );

        if( !game_createobject( &m_smallestplyr[ piece ][ 0 ], 16, 16, buf ) ) { 
            return 0;
        }
        m_smallestplyr[ piece ][ 0 ].mass = DEBRIS_MASS;
        game_setframe( &m_smallestplyr[ piece ][ 0 ], 0 );

        for( i = 1; i < MAX_SMALLESTDEBRIS; i++ ) {
            game_cloneobject( &m_smallestplyr[ piece ][ i ], &m_smallestplyr[ piece ][ 0 ] );
        }
    }

    for( piece = 0; piece < MAX_SMALLESTTYPES; piece++ ) {
        sprintf_s( buf, 128, "bmp/nme_debris%d.bmp", piece + 1 );

        if( !game_createobject( &m_smallestnme[ piece ][ 0 ], 16, 16, buf ) ) { 
            return 0;
        }
        m_smallestnme[ piece ][ 0 ].mass = DEBRIS_MASS;
        game_setframe( &m_smallestnme[ piece ][ 0 ], 0 );

        for( i = 1; i < MAX_SMALLESTDEBRIS; i++ ) {
            game_cloneobject( &m_smallestnme[ piece ][ i ], &m_smallestnme[ piece ][ 0 ] );
        }
    }

    // the nme ships
    if( !game_createobject( &m_nmeships[ 0 ], 32, 32, "bmp/nme.bmp" ) ) {
        return 0;
    }
    m_nmeships[ 0 ].state = STATE_NMEWAITING;
    m_nmeships[ 0 ].mass  = PLAYER_MASS;
    game_setframe( &m_nmeships[ 0 ], 0 );

    for( i = 1; i < MAX_NMESHIPS; i++ ) {
        game_cloneobject( &m_nmeships[ i ], &m_nmeships[ 0 ] );
    }

    // pickups
    m_currentpickup = NULL;
    if( !game_createobject( &m_pickups[ PICKUP_BOMBREDUCE ], 16, 24, "bmp/pck_bombreduce.bmp" ) ) {
        return 0;
    }
    m_pickups[ PICKUP_BOMBREDUCE ].state = STATE_DEAD;
    game_setframe( &m_pickups[ PICKUP_BOMBREDUCE ], 0 );

    if( !game_createobject( &m_pickups[ PICKUP_BOMBSCATTER ], 16, 24, "bmp/pck_bombscatter.bmp" ) ) {
        return 0;
    }
    m_pickups[ PICKUP_BOMBSCATTER ].state = STATE_DEAD;
    game_setframe( &m_pickups[ PICKUP_BOMBSCATTER ], 0 );

    if( !game_createobject( &m_pickups[ PICKUP_GUNMULTISHOT ], 16, 24, "bmp/pck_gunmultishot.bmp" ) ) {
        return 0;
    }
    m_pickups[ PICKUP_GUNMULTISHOT ].state = STATE_DEAD;
    game_setframe( &m_pickups[ PICKUP_GUNMULTISHOT ], 0 );

    if( !game_createobject( &m_pickups[ PICKUP_GUNCHARGEUP ], 16, 24, "bmp/pck_gunchargeup.bmp" ) ) {
        return 0;
    }
    m_pickups[ PICKUP_GUNCHARGEUP ].state = STATE_DEAD;
    game_setframe( &m_pickups[ PICKUP_GUNCHARGEUP ], 0 );

    if( !game_createobject( &m_pickups[ PICKUP_BONUSEXTRALIFE ], 16, 24, "bmp/pck_bonusextralife.bmp" ) ) {
        return 0;
    }
    m_pickups[ PICKUP_BONUSEXTRALIFE ].state = STATE_DEAD;
    game_setframe( &m_pickups[ PICKUP_BONUSEXTRALIFE ], 0 );

    if( !game_createobject( &m_pickups[ PICKUP_BONUSFULLSHIELD ], 16, 24, "bmp/pck_bonusfullshield.bmp" ) ) {
        return 0;
    }
    m_pickups[ PICKUP_BONUSFULLSHIELD ].state = STATE_DEAD;
    game_setframe( &m_pickups[ PICKUP_BONUSFULLSHIELD ], 0 );

    for( i = 0; i < MAX_HELPPAGES; i++ ) {
        sprintf_s( buf, 128, "bmp/help_p%d.bmp", i );

        if( !game_createobject( &m_helppages[ i ], 416, 256, buf ) ) {
            return 0;
        }

        m_helppages[ i ].state = STATE_DEAD;
        m_helppages[ i ].x = SCREEN_HALFWIDTH_f;
        m_helppages[ i ].y = SCREEN_HALFHEIGHT_f;

        game_setframe( &m_helppages[ i ], 0 );
    }

    if( !game_createobject( &m_lblDirectional, 224, 80, "bmp/lblDirectional.bmp" ) ) {
        return 0;
    }
    m_lblDirectional.state   = STATE_DEAD;
    m_lblDirectional.x       = SCREEN_HALFWIDTH_f;
    m_lblDirectional.y       = SCREEN_HALFHEIGHT_f + 64.0f;
    game_setframe( &m_lblDirectional, 0 );

    if( !game_createobject( &m_lblSliding, 224, 80, "bmp/lblSliding.bmp" ) ) {
        return 0;
    }
    m_lblSliding.state   = STATE_DEAD;
    m_lblSliding.x       = SCREEN_HALFWIDTH_f;
    m_lblSliding.y       = SCREEN_HALFHEIGHT_f + 64.0f;
    game_setframe( &m_lblSliding, 0 );

    // the options property sheet
    if( !game_createobject( &m_sldrsound, 128, 32, "bmp/sldrgeneral.bmp" ) ) {
        return 0;
    }
    m_sldrsound.state   = STATE_DEAD;
    m_sldrsound.x       = SCREEN_HALFWIDTH_f;
    m_sldrsound.y       = SCREEN_HALFHEIGHT_f - 64.0f;
    game_setframe( &m_sldrsound, 0 );

    if( !game_createobject( &m_sldrboxsound, 16, 16, "bmp/sldrbutton.bmp" ) ) {
        return 0;
    }
    m_sldrboxsound.state = STATE_DEAD;
    m_sldrboxsound.x     = SCREEN_HALFWIDTH_f;
    m_sldrboxsound.y     = SCREEN_HALFHEIGHT_f - 64.0f;
    game_setframe( &m_sldrboxsound, 0 );
    m_sldrboxsound.statesliderbox.minx = m_sldrboxsound.x - 50.0f;
    m_sldrboxsound.statesliderbox.maxx = m_sldrboxsound.x + 50.0f;

    if( !game_cloneobject( &m_sldrmusic, &m_sldrsound ) ) {
        return 0;
    }
    m_sldrsound.x       = SCREEN_HALFWIDTH_f;
    m_sldrsound.y       = SCREEN_HALFHEIGHT_f;

    if( !game_cloneobject( &m_sldrboxmusic, &m_sldrboxsound ) ) {
        return 0;
    }
    m_sldrboxmusic.x     = SCREEN_HALFWIDTH_f;
    m_sldrboxmusic.y     = SCREEN_HALFHEIGHT_f;
    m_sldrboxmusic.statesliderbox.minx = m_sldrboxmusic.x - 50.0f;
    m_sldrboxmusic.statesliderbox.maxx = m_sldrboxmusic.x + 50.0f;

    if( !game_cloneobject( &m_sldrmouse, &m_sldrsound ) ) {
        return 0;
    }
    m_sldrmouse.x       = SCREEN_HALFWIDTH_f;
    m_sldrmouse.y       = SCREEN_HALFHEIGHT_f;

    if( !game_cloneobject( &m_sldrboxmouse, &m_sldrboxsound ) ) {
        return 0;
    }
    m_sldrboxmouse.x     = SCREEN_HALFWIDTH_f;
    m_sldrboxmouse.y     = SCREEN_HALFHEIGHT_f;
    m_sldrboxmouse.statesliderbox.minx = m_sldrboxmouse.x - 50.0f;
    m_sldrboxmouse.statesliderbox.maxx = m_sldrboxmouse.x + 50.0f;

    if( !game_createobject( &m_drpshipphysics, 128, 24, "bmp/drpphysics.bmp" ) ) {
        return 0;
    }
    m_drpshipphysics.state = STATE_DEAD;
    m_drpshipphysics.x     = SCREEN_HALFWIDTH_f;
    m_drpshipphysics.y     = SCREEN_HALFHEIGHT_f - 64.0f;
    game_setframe( &m_drpshipphysics, 0 );

    if( !game_createobject( &m_drpshipphysicssel, 128, 24, "bmp/drpphysicssel.bmp" ) ) {
        return 0;
    }
    m_drpshipphysicssel.state = STATE_DEAD;
    m_drpshipphysicssel.x     = SCREEN_HALFWIDTH_f;
    m_drpshipphysicssel.y     = SCREEN_HALFHEIGHT_f - 64.0f;
    game_setframe( &m_drpshipphysicssel, 0 );

    if( !game_createobject( &m_lstshipphysics, 128, 64, "bmp/lstgeneralwide.bmp" ) ) {
        return 0;
    }
    m_lstshipphysics.state = STATE_DEAD;
    m_lstshipphysics.x     = SCREEN_HALFWIDTH_f;
    m_lstshipphysics.y     = SCREEN_HALFHEIGHT_f - 20.0f;
    game_setframe( &m_lstshipphysics, 0 );

    if( !game_createobject( &m_butsliding, 128, 32, "bmp/butsliding.bmp" ) ) {
        return 0;
    }
    m_butsliding.state = STATE_PREBIRTH;
    m_butsliding.x     = SCREEN_HALFWIDTH_f;
    m_butsliding.y     = SCREEN_HALFHEIGHT_f - 36.0f;
    game_setframe( &m_butsliding, 0 );

    if( !game_createobject( &m_butdirectional, 128, 32, "bmp/butdirectional.bmp" ) ) {
        return 0;
    }
    m_butdirectional.state = STATE_PREBIRTH;
    m_butdirectional.x     = SCREEN_HALFWIDTH_f;
    m_butdirectional.y     = SCREEN_HALFHEIGHT_f - 4;
    game_setframe( &m_butdirectional, 0 );

    if( !game_createobject( &m_chkplyrthrusting, 16, 16, "bmp/chkgeneral.bmp" ) ) {
        return 0;
    }
    m_chkplyrthrusting.state = STATE_PREBIRTH;
    m_chkplyrthrusting.x     = SCREEN_HALFWIDTH_f - 80.0f;
    m_chkplyrthrusting.y     = SCREEN_HALFHEIGHT_f - 96.0f;
    game_setframe( &m_chkplyrthrusting, 0 );

    if( !game_cloneobject( &m_chkplyrshottrails, &m_chkplyrthrusting ) ) {
        return 0;
    }
    m_chkplyrshottrails.x     = SCREEN_HALFWIDTH_f - 80.0f;
    m_chkplyrshottrails.y     = SCREEN_HALFHEIGHT_f - 64.0f;

    if( !game_cloneobject( &m_chkplyrshotexpl, &m_chkplyrthrusting ) ) {
        return 0;
    }
    m_chkplyrshotexpl.x     = SCREEN_HALFWIDTH_f - 80.0f;
    m_chkplyrshotexpl.y     = SCREEN_HALFHEIGHT_f - 32.0f;

    if( !game_cloneobject( &m_chkplyrexpl, &m_chkplyrthrusting ) ) {
        return 0;
    }
    m_chkplyrexpl.x     = SCREEN_HALFWIDTH_f - 80.0f;
    m_chkplyrexpl.y     = SCREEN_HALFHEIGHT_f;

    if( !game_cloneobject( &m_chknmeshottrails, &m_chkplyrthrusting ) ) {
        return 0;
    }
    m_chknmeshottrails.x     = SCREEN_HALFWIDTH_f - 80.0f;
    m_chknmeshottrails.y     = SCREEN_HALFHEIGHT_f + 32.0f;

    if( !game_cloneobject( &m_chknmeshotexpl, &m_chkplyrthrusting ) ) {
        return 0;
    }
    m_chknmeshotexpl.x     = SCREEN_HALFWIDTH_f - 80.0f;
    m_chknmeshotexpl.y     = SCREEN_HALFHEIGHT_f + 64.0f;

    if( !game_cloneobject( &m_chknmeexpl, &m_chkplyrthrusting ) ) {
        return 0;
    }
    m_chknmeexpl.x     = SCREEN_HALFWIDTH_f - 80.0f;
    m_chknmeexpl.y     = SCREEN_HALFHEIGHT_f + 96.0f;

    sys_clearsurface( g_backbuffer, 0 );
    sys_text(RGB(255, 255, 255), 0, SCREEN_WIDTH - 128, SCREEN_HEIGHT - 32, "loading waves...");
    sys_draw();

    sys_logwrite( "game_init: loading game wave files\n" );

    // load all the sounds
    if( g_ds ) {
        for( i = 0; i < MAX_GAMESOUNDS; i++ ) {
            m_sounds[ i ] = sys_createsound( m_soundfilenames[ i ] );
            if( -1 == m_sounds[ i ] ) {
                strcpy_s( g_gameerror, 1024, g_syserror );
                return 0;
            }
        }
    }
    else {
        sys_logwrite( "game_init: sound support not found\n" );
    }

    for( i = 0; i < MAX_SHOTTRAILS * 3; i++ ) {
        shottrail_init( &m_shottrails[ i ], SHOTTRAIL_ADD
                      , SHOTTRAIL_MINRED,   SHOTTRAIL_MAXRED
                      , SHOTTRAIL_MINGREEN, SHOTTRAIL_MAXGREEN
                      , SHOTTRAIL_MINBLUE,  SHOTTRAIL_MAXBLUE
                      , SHOTTRAIL_DESTHUE,  SHOTTRAIL_INC );
    }

    for( i = 0; i < MAX_SHOTTRAILS; i++ ) {
        shottrail_init( &m_chargeupshottrails[ i ], SHOTTRAIL_ADD
                      , CHARGEUPTRAIL_MINRED,   CHARGEUPTRAIL_MAXRED
                      , CHARGEUPTRAIL_MINGREEN, CHARGEUPTRAIL_MAXGREEN
                      , CHARGEUPTRAIL_MINBLUE,  CHARGEUPTRAIL_MAXBLUE
                      , CHARGEUPTRAIL_DESTHUE,  CHARGEUPTRAIL_INC );
    }


    for( i = 0; i < MAX_SHOTRICHOCHETS; i++ ) {
        shotrichochet_init( &m_shotrichochets[ i ]
                          , 0, 0.0f, 0.0f
                          , SHOTTRAIL_MINRED,   SHOTTRAIL_MAXRED
                          , SHOTTRAIL_MINGREEN, SHOTTRAIL_MAXGREEN
                          , SHOTTRAIL_MINBLUE,  SHOTTRAIL_MAXBLUE
                          , SHOTTRAIL_DESTHUE,  SHOTTRAIL_INC );

        shotrichochet_init( &m_chargeupshotrichochets[ i ]
                          , 0, 0.0f, 0.0f
                          , CHARGEUPTRAIL_MINRED,   CHARGEUPTRAIL_MAXRED
                          , CHARGEUPTRAIL_MINGREEN, CHARGEUPTRAIL_MAXGREEN
                          , CHARGEUPTRAIL_MINBLUE,  CHARGEUPTRAIL_MAXBLUE
                          , CHARGEUPTRAIL_DESTHUE,  CHARGEUPTRAIL_INC );

        shotrichochet_init( &m_nmeexp[ i ]
                          , 0, 0.0f, 0.0f
                          , NMEEXP_MINRED,  NMEEXP_MAXRED
                          , NMEEXP_MAXRED,  NMEEXP_MAXGREEN
                          , NMEEXP_MINBLUE, NMEEXP_MAXBLUE
                          , NMEEXP_DESTHUE, NMEEXP_INC );

        shotrichochet_init( &m_plyrcollide[ i ]
                          , 0, 0.0f, 0.0f
                          , PLYRCOLLIDE_MINRED,  PLYRCOLLIDE_MAXRED
                          , PLYRCOLLIDE_MAXRED,  PLYRCOLLIDE_MAXGREEN
                          , PLYRCOLLIDE_MINBLUE, PLYRCOLLIDE_MAXBLUE
                          , PLYRCOLLIDE_DESTHUE, PLYRCOLLIDE_INC );
    }

    shotrichochet_init( &m_plyrtrail1
                      , 0, 0.0f, 0.0f
                      , PLYRTHRUSTTRAILF_MINRED,   PLYRTHRUSTTRAILF_MAXRED
                      , PLYRTHRUSTTRAILF_MINGREEN, PLYRTHRUSTTRAILF_MAXGREEN
                      , PLYRTHRUSTTRAILF_MINBLUE,  PLYRTHRUSTTRAILF_MAXBLUE
                      , PLYRTHRUSTTRAILF_DESTHUE,  PLYRTHRUSTTRAILF_INC );
    shotrichochet_init( &m_plyrtrail2
                      , 0, 0.0f, 0.0f
                      , PLYRTHRUSTTRAILF_MINRED,   PLYRTHRUSTTRAILF_MAXRED
                      , PLYRTHRUSTTRAILF_MINGREEN, PLYRTHRUSTTRAILF_MAXGREEN
                      , PLYRTHRUSTTRAILF_MINBLUE,  PLYRTHRUSTTRAILF_MAXBLUE
                      , PLYRTHRUSTTRAILF_DESTHUE,  PLYRTHRUSTTRAILF_INC );
    m_plyrtrail1.addrate = PLYRTHRUSTTRAILF_ADD;
    m_plyrtrail2.addrate = PLYRTHRUSTTRAILF_ADD;

    shotrichochet_init( &m_plyrexp
                      , 0, 0.0f, 0.0f
                      , PLYREXP_MINRED,    PLYREXP_MAXRED
                      , PLYREXP_MINGREEN,  PLYREXP_MAXGREEN
                      , PLYREXP_MINBLUE,   PLYREXP_MAXBLUE
                      , SHOTTRAIL_DESTHUE, PLYREXP_INC );

    for( i = 0; i < MAX_NMESHOTTRAILS; i++ ) {
        shottrail_init( &m_nmeshottrails[ i ], SHOTTRAIL_ADD
                      , NMESHOTTRAIL_MINRED,   NMESHOTTRAIL_MAXRED
                      , NMESHOTTRAIL_MINGREEN, NMESHOTTRAIL_MAXGREEN
                      , NMESHOTTRAIL_MINBLUE,  NMESHOTTRAIL_MAXBLUE
                      , NMESHOTTRAIL_DESTHUE,  NMESHOTTRAIL_INC );
    }

    for( i = 0; i < MAX_NMESHOTRICHOCHETS; i++ ) {
        shotrichochet_init( &m_nmeshotrichochets[ i ]
                          , 0, 0.0f, 0.0f
                          , NMESHOTTRAIL_MINRED,   NMESHOTTRAIL_MAXRED
                          , NMESHOTTRAIL_MINGREEN, NMESHOTTRAIL_MAXGREEN
                          , NMESHOTTRAIL_MINBLUE,  NMESHOTTRAIL_MAXBLUE
                          , NMESHOTTRAIL_DESTHUE,  NMESHOTTRAIL_INC );
    }

    starfield_init( rand() % 360 );

    for( i = 0; i < MAX_HIGHSCORES; i++ ) {
        if( !sys_createsurface( &m_scorerows[ i ].dds, 384, 32, 0 ) ) {
            return 0;
        }
    }

    sys_clearsurface( g_backbuffer, 0 );
    sys_text( RGB( 255, 255, 255 ), 0, SCREEN_WIDTH - 128, SCREEN_HEIGHT - 32, "building fonts..." );
    sys_draw();

    sys_logwrite( "game_init: building game fonts\n" );

    // create the fonts
    m_fonttitle = font_create( "Tahoma", 12, FW_BOLD, RGB( 32, 96, 32 ) );
    if( -1 == m_fonttitle ) {
        m_fonttitle = font_create( NULL, 12, FW_BOLD, RGB( 32, 96, 32 ) );
        if( -1 == m_fonttitle ) {
            strcpy_s( g_gameerror, 1024, g_syserror );
            return 0;
        }
    }

    m_fontbody = font_create( "Arial", 8, FW_NORMAL, RGB( 128, 128, 160 ) );
    if( -1 == m_fontbody ) {
        m_fontbody = font_create( NULL, 8, FW_NORMAL, RGB( 128, 128, 160 ) );
        if( -1 == m_fontbody ) {
            strcpy_s( g_gameerror, 1024, g_syserror );
            return 0;
        }
    }

    m_fontbodyred = font_create( "Arial", 8, FW_NORMAL, RGB( 200, 0, 0 ) );
    if( -1 == m_fontbodyred ) {
        m_fontbodyred = font_create( NULL, 8, FW_NORMAL, RGB( 200, 0, 0 ) );
        if( -1 == m_fontbodyred ) {
            strcpy_s( g_gameerror, 1024, g_syserror );
            return 0;
        }
    }

    m_fontscores = font_create( "Arial", 8, FW_NORMAL, RGB( 240, 240, 240 ) );
    if( -1 == m_fontscores ) {
        m_fontscores = font_create( NULL, 8, FW_NORMAL, RGB( 240, 240, 240 ) );
        if( -1 == m_fontscores ) {
            strcpy_s( g_gameerror, 1024, g_syserror );
            return 0;
        }
    }

    m_fontdamage = font_create( "Arial", 6, FW_NORMAL, RGB( 250, 250, 250 ) );
    if( -1 == m_fontdamage ) {
        m_fontdamage = font_create( NULL, 6, FW_NORMAL, RGB( 250, 250, 250 ) );
        if( -1 == m_fontdamage ) {
            strcpy_s( g_gameerror, 1024, g_syserror );
            return 0;
        }
    }

    if( !marquee_init() ) {
        return 0;
    }

    // options page labels
    if( !font_build( m_fontscores, &m_lblsound.dds, &m_lblsound.width, "Sound:" ) ) {
        strcpy_s( g_gameerror, 1024, g_syserror );
        return 0;
    }
    m_lblsound.height = 32;
    m_lblsound.bmpwidth = m_lblsound.width;
    m_lblsound.halfwidth = m_lblsound.width / 2;
    m_lblsound.halfheight = m_lblsound.height / 2;
    game_setframe( &m_lblsound, 0 );
    m_lblsound.x = ( SCREEN_HALFWIDTH_f - 50.0f ) + ( m_lblsound.width / 2 );
    m_lblsound.y = SCREEN_HALFHEIGHT_f - 72.0f;
    game_maptoscreen( &m_lblsound );

    if( !font_build( m_fontscores, &m_lblmusic.dds, &m_lblmusic.width, "Music:" ) ) {
        strcpy_s( g_gameerror, 1024, g_syserror );
        return 0;
    }
    m_lblmusic.height = 32;
    m_lblmusic.bmpwidth = m_lblmusic.width;
    m_lblmusic.halfwidth = m_lblmusic.width / 2;
    m_lblmusic.halfheight = m_lblmusic.height / 2;
    game_setframe( &m_lblmusic, 0 );
    m_lblmusic.x = ( SCREEN_HALFWIDTH_f - 50.0f ) + ( m_lblmusic.width / 2 );
    m_lblmusic.y = SCREEN_HALFHEIGHT_f - 8.0f;
    game_maptoscreen( &m_lblmusic );

    if( !font_build( m_fontscores, &m_lblmouse.dds, &m_lblmouse.width, "Speed:" ) ) {
        strcpy_s( g_gameerror, 1024, g_syserror );
        return 0;
    }
    m_lblmouse.height = 32;
    m_lblmouse.bmpwidth = m_lblmouse.width;
    m_lblmouse.halfwidth = m_lblmouse.width / 2;
    m_lblmouse.halfheight = m_lblmouse.height / 2;
    game_setframe( &m_lblmouse, 0 );
    m_lblmouse.x = ( SCREEN_HALFWIDTH_f - 50.0f ) + ( m_lblmouse.width / 2 );
    m_lblmouse.y = SCREEN_HALFHEIGHT_f - 8.0f;
    game_maptoscreen( &m_lblmouse );

    if( !font_build( m_fontscores, &m_lblshipphysics.dds, &m_lblshipphysics.width, "Player ship:" ) ) {
        strcpy_s( g_gameerror, 1024, g_syserror );
        return 0;
    }
    m_lblshipphysics.height = 32;
    m_lblshipphysics.bmpwidth = m_lblshipphysics.width;
    m_lblshipphysics.halfwidth = m_lblshipphysics.width / 2;
    m_lblshipphysics.halfheight = m_lblshipphysics.height / 2;
    game_setframe( &m_lblshipphysics, 0 );
    m_lblshipphysics.x = ( SCREEN_HALFWIDTH_f - 64.0f ) + ( m_lblshipphysics.width / 2 );
    m_lblshipphysics.y = SCREEN_HALFHEIGHT_f - 80.0f;
    game_maptoscreen( &m_lblshipphysics );

    if( !font_build( m_fontscores, &m_lblthrusting.dds, &m_lblthrusting.width, "Player thrust trail" ) ) {
        strcpy_s( g_gameerror, 1024, g_syserror );
        return 0;
    }
    m_lblthrusting.height = 32;
    m_lblthrusting.bmpwidth = m_lblthrusting.width;
    m_lblthrusting.halfwidth = m_lblthrusting.width / 2;
    m_lblthrusting.halfheight = m_lblthrusting.height / 2;
    game_setframe( &m_lblthrusting, 0 );
    m_lblthrusting.x = ( SCREEN_HALFWIDTH_f - 64.0f ) + ( m_lblthrusting.width / 2 );
    m_lblthrusting.y = SCREEN_HALFHEIGHT_f - 88.0f;
    game_maptoscreen( &m_lblthrusting );

    if( !font_build( m_fontscores, &m_lblplyrshottrails.dds, &m_lblplyrshottrails.width, "Player shot trails" ) ) {
        strcpy_s( g_gameerror, 1024, g_syserror );
        return 0;
    }
    m_lblplyrshottrails.height = 32;
    m_lblplyrshottrails.bmpwidth = m_lblplyrshottrails.width;
    m_lblplyrshottrails.halfwidth = m_lblplyrshottrails.width / 2;
    m_lblplyrshottrails.halfheight = m_lblplyrshottrails.height / 2;
    game_setframe( &m_lblplyrshottrails, 0 );
    m_lblplyrshottrails.x = ( SCREEN_HALFWIDTH_f - 64.0f ) + ( m_lblplyrshottrails.width / 2 );
    m_lblplyrshottrails.y = SCREEN_HALFHEIGHT_f - 56.0f;
    game_maptoscreen( &m_lblplyrshottrails );

    if( !font_build( m_fontscores, &m_lblplyrshotexpl.dds, &m_lblplyrshotexpl.width, "Player shot explosion" ) ) {
        strcpy_s( g_gameerror, 1024, g_syserror );
        return 0;
    }
    m_lblplyrshotexpl.height = 32;
    m_lblplyrshotexpl.bmpwidth = m_lblplyrshotexpl.width;
    m_lblplyrshotexpl.halfwidth = m_lblplyrshotexpl.width / 2;
    m_lblplyrshotexpl.halfheight = m_lblplyrshotexpl.height / 2;
    game_setframe( &m_lblplyrshotexpl, 0 );
    m_lblplyrshotexpl.x = ( SCREEN_HALFWIDTH_f - 64.0f ) + ( m_lblplyrshotexpl.width / 2 );
    m_lblplyrshotexpl.y = SCREEN_HALFHEIGHT_f - 24.0f;
    game_maptoscreen( &m_lblplyrshotexpl );

    if( !font_build( m_fontscores, &m_lblplyrexpl.dds, &m_lblplyrexpl.width, "Player explosion" ) ) {
        strcpy_s( g_gameerror, 1024, g_syserror );
        return 0;
    }
    m_lblplyrexpl.height = 32;
    m_lblplyrexpl.bmpwidth = m_lblplyrexpl.width;
    m_lblplyrexpl.halfwidth = m_lblplyrexpl.width / 2;
    m_lblplyrexpl.halfheight = m_lblplyrexpl.height / 2;
    game_setframe( &m_lblplyrexpl, 0 );
    m_lblplyrexpl.x = ( SCREEN_HALFWIDTH_f - 64.0f ) + ( m_lblplyrexpl.width / 2 );
    m_lblplyrexpl.y = SCREEN_HALFHEIGHT_f + 8.0f;
    game_maptoscreen( &m_lblplyrexpl );

    if( !font_build( m_fontscores, &m_lblnmeshottrails.dds, &m_lblnmeshottrails.width, "Enemy shot trails" ) ) {
        strcpy_s( g_gameerror, 1024, g_syserror );
        return 0;
    }
    m_lblnmeshottrails.height = 32;
    m_lblnmeshottrails.bmpwidth = m_lblnmeshottrails.width;
    m_lblnmeshottrails.halfwidth = m_lblnmeshottrails.width / 2;
    m_lblnmeshottrails.halfheight = m_lblnmeshottrails.height / 2;
    game_setframe( &m_lblnmeshottrails, 0 );
    m_lblnmeshottrails.x = ( SCREEN_HALFWIDTH_f - 64.0f ) + ( m_lblnmeshottrails.width / 2 );
    m_lblnmeshottrails.y = SCREEN_HALFHEIGHT_f + 40.0f;
    game_maptoscreen( &m_lblnmeshottrails );

    if( !font_build( m_fontscores, &m_lblnmeshotexpl.dds, &m_lblnmeshotexpl.width, "Enemy shot explosion" ) ) {
        strcpy_s( g_gameerror, 1024, g_syserror );
        return 0;
    }
    m_lblnmeshotexpl.height = 32;
    m_lblnmeshotexpl.bmpwidth = m_lblnmeshotexpl.width;
    m_lblnmeshotexpl.halfwidth = m_lblnmeshotexpl.width / 2;
    m_lblnmeshotexpl.halfheight = m_lblnmeshotexpl.height / 2;
    game_setframe( &m_lblnmeshotexpl, 0 );
    m_lblnmeshotexpl.x = ( SCREEN_HALFWIDTH_f - 64.0f ) + ( m_lblnmeshotexpl.width / 2 );
    m_lblnmeshotexpl.y = SCREEN_HALFHEIGHT_f + 72.0f;
    game_maptoscreen( &m_lblnmeshotexpl );

    if( !font_build( m_fontscores, &m_lblnmeexpl.dds, &m_lblnmeexpl.width, "Enemy explosion" ) ) {
        strcpy_s( g_gameerror, 1024, g_syserror );
        return 0;
    }
    m_lblnmeexpl.height = 32;
    m_lblnmeexpl.bmpwidth = m_lblnmeexpl.width;
    m_lblnmeexpl.halfwidth = m_lblnmeexpl.width / 2;
    m_lblnmeexpl.halfheight = m_lblnmeexpl.height / 2;
    game_setframe( &m_lblnmeexpl, 0 );
    m_lblnmeexpl.x = ( SCREEN_HALFWIDTH_f - 64.0f ) + ( m_lblnmeexpl.width / 2 );
    m_lblnmeexpl.y = SCREEN_HALFHEIGHT_f + 102.0f;
    game_maptoscreen( &m_lblnmeexpl );

    sys_logwrite( "game_init: reading game high scores\n" );

    if( !scores_read( FILE_HIGHSCORES ) ) {
        scores_reset();
    }

    sys_clearsurface( g_backbuffer, 0 );
    sys_text( RGB( 255, 255, 255 ), 0, SCREEN_WIDTH - 128, SCREEN_HEIGHT - 32, "loading music..." );
    sys_draw();

    m_trackcount    = 0;
    m_leveltrackidx = 0;

    m_menutrack     = sys_createmp3( "mp3/menu.mp3" );
    m_creditstrack  = sys_createmp3( "mp3/credits.mp3" );

    for( i = 0; i < MAX_GAMETRACKS; i++ ) {
        sprintf_s( buf, 128, "mp3/game%d.mp3", i );
        m_tracks[ i ] = sys_createmp3( buf );
        if( -1 != m_tracks[ i ] ) {
            m_trackcount++;
        }
    }

    if( m_trackcount ) {
        m_leveltrackidx = rand() % m_trackcount;
    }

    options_read( FILE_OPTIONS, &m_options );

    if( m_options.soundlevel > 0 ) {
        sys_soundon();
        for( i = 0; i < MAX_GAMESOUNDS; i++ ) {
            sys_setsoundvolume( m_sounds[ i ], SND_VOLUMEMULTIPLIER * ( 100 - m_options.soundlevel ) );
        }
        sys_playsound( m_sounds[ SND_CHEAT ], 0 );
    }
    else {
        sys_soundoff();
    }

    if( m_options.musiclevel > 0 ) {
        sys_mp3on();
        sys_setmp3volume( m_menutrack, MP3_VOLUMEMULTIPLIER * ( 100 - m_options.musiclevel ) );
        sys_setmp3volume( m_creditstrack, MP3_VOLUMEMULTIPLIER * ( 100 - m_options.musiclevel ) );
        for( i = 0; i < MAX_GAMETRACKS; i++ ) {
            sys_setmp3volume( m_tracks[ i ], MP3_VOLUMEMULTIPLIER * ( 100 - m_options.musiclevel ) );
        }
    }
    else {
        sys_mp3off();
    }

    m_gameactive = 0;
    menu_view();

    sys_logwrite( "game_init: entering game loop\n" );

    return 1;
}

void game_shutdown( void )
{
    int piece, astcol, i;

    sys_logwrite( "game_shutdown: exited game loop\n" );

    options_write( FILE_OPTIONS, &m_options );

    sys_logwrite( "game_shutdown: writing game high scores\n" );
    scores_write( FILE_HIGHSCORES );

    sys_logwrite( "game_shutdown: deleting music\n" );
    for( i = 0; i < MAX_GAMETRACKS; i++ ) {
        sys_deletemp3( m_tracks[ i ] );
    }
    sys_deletemp3( m_menutrack );
    sys_deletemp3( m_creditstrack );

    sys_logwrite( "game_shutdown: deleting game fonts\n" );

    font_delete( m_fontdamage );
    font_delete( m_fonttitle );
    font_delete( m_fontbodyred );
    font_delete( m_fontbody );
    font_delete( m_fontscores );

    sys_logwrite( "game_shutdown: deleting game waves\n" );

    if( g_ds ) {
        for( i = 0; i < MAX_GAMESOUNDS; i++ ) {
            sys_deletesound( m_sounds[ i ] );
        }
    }

    game_deleteobject( &m_lblSliding );
    game_deleteobject( &m_lblDirectional );
    game_deleteobject( &m_lblsound );
    game_deleteobject( &m_lblmusic );
    game_deleteobject( &m_lblmouse );
    game_deleteobject( &m_lblshipphysics );
    game_deleteobject( &m_lblplyrshottrails );
    game_deleteobject( &m_lblplyrshotexpl );
    game_deleteobject( &m_lblplyrexpl );
    game_deleteobject( &m_lblnmeshottrails );
    game_deleteobject( &m_lblnmeshotexpl );
    game_deleteobject( &m_lblnmeexpl );
    game_deleteobject( &m_lblthrusting );

    sys_logwrite( "game_shutdown: deleting game bitmaps\n" );

    game_deleteobject( &m_sldrsound );
    game_deleteobject( &m_sldrboxsound );
    game_deleteobject( &m_sldrmusic );
    game_deleteobject( &m_sldrboxmusic );
    game_deleteobject( &m_sldrmouse );
    game_deleteobject( &m_sldrboxmouse );
    game_deleteobject( &m_drpshipphysicssel );
    game_deleteobject( &m_drpshipphysics );
    game_deleteobject( &m_lstshipphysics );
    game_deleteobject( &m_butsliding );
    game_deleteobject( &m_butdirectional );
    game_deleteobject( &m_chkplyrthrusting );
    game_deleteobject( &m_chkplyrshottrails );
    game_deleteobject( &m_chkplyrshotexpl );
    game_deleteobject( &m_chkplyrexpl );
    game_deleteobject( &m_chknmeshottrails );
    game_deleteobject( &m_chknmeshotexpl );
    game_deleteobject( &m_chknmeexpl );

    for( i = 0; i < MAX_HELPPAGES; i++ ) {
        sys_deletesurface( m_helppages[ i ].dds );
    }

    for( i = 0; i < MAX_PICKUPS; i++ ) {
        sys_deletesurface( m_pickups[ i ].dds );
    }

    for( i = 0; i < MAX_HIGHSCORES; i++ ) {
        sys_deletesurface( m_scorerows[ i ].dds );
    }

    sys_deletesurface( m_enternamecursor.dds );
    marquee_shutdown();

    for( piece = 0; piece < MAX_SMALLESTTYPES; piece++ ) {
        for( i = 0; i < MAX_SMALLESTDEBRIS; i++ ) {
            game_deleteobject( &m_smallestast0[ piece ][ i ] );
            game_deleteobject( &m_smallestast1[ piece ][ i ] );
            game_deleteobject( &m_smallestplyr[ piece ][ i ] );
            game_deleteobject( &m_smallestnme[ piece ][ i ] );
        }
    }

    for( astcol = 0; astcol < MAX_DEBRIS; astcol++ ) {
        for( i = 0; i < MAX_ACTIVEDEBRIS; i++ ) {
            game_deleteobject( &m_debris[ astcol ][ i ] );
        }
    }

    for( astcol = 0; astcol < MAX_DEBRIS; astcol++ ) {
        for( i = 0; i < MAX_ACTIVEDEBRIS; i++ ) {
            game_deleteobject( &m_nmedebris[ astcol ][ i ] );
        }
    }

    for( astcol = 0; astcol < MAX_ASTCOLS; astcol++ ) {
        for( piece = 0; piece < MAX_ASTPIECES; piece++ ) {
            for( i = 0; i < MAX_ASTS; i++ ) {
                game_deleteobject( &m_astcols[ astcol ].astpieces[ piece ][ i ] );
            }
        }
    }

    for( i = 0; i < MAX_SHOTS * 3; i++ ) {              // x3 coz of multishot
        game_deleteobject( &m_shots[ i ] );
    }

    for( i = 0; i < BOMBSCATTER_MAXSHOTS; i++ ) {
        game_deleteobject( &m_scattershots[ i ] );
    }

    for( astcol = 0; astcol < MAX_CHARGEUPSIZES; astcol++ ) {
        for( i = 0; i < MAX_CHARGEUPSHOTS; i++ ) {
            game_deleteobject( &m_chargeupshots[ astcol ][ i ] );
        }
    }

    for( i = 0; i < MAX_NMESHOTS; i++ ) {
        game_deleteobject( &m_nmeshots[ i ] );
    }

    for( i = 0; i < MAX_LEVELS; i++ ) {
        game_deleteobject( &m_leveltitles[ i ] );
    }

    for( i = 0; i < MAX_ROUNDS; i++ ) {
        game_deleteobject( &m_roundtitles[ i ] );
    }

    for( i = 0; i < MAX_NMESHIPS; i++ ) {
        game_deleteobject( &m_nmeships[ i ] );
    }

    for( i = 0; i < MAX_OPTPAGES; i++ ) {
        game_deleteobject( &m_butoptpages[ i ] );
    }

    game_deleteobject( &m_portraitnme );
    game_deleteobject( &m_portraitplyr );
    game_deleteobject( &m_stats );
    game_deleteobject( &m_player );
    game_deleteobject( &m_entername );
    game_deleteobject( &m_gameover );
    game_deleteobject( &m_finishscreen );

    game_deleteobject( &m_menuoptions2 );
    game_deleteobject( &m_menubar );
    game_deleteobject( &m_menugame );
    game_deleteobject( &m_menuscores );
    game_deleteobject( &m_menuscores2 );
    game_deleteobject( &m_menuhelp );
    game_deleteobject( &m_menuhelp2 );
    game_deleteobject( &m_menuabout );
    game_deleteobject( &m_mousecursor );
    game_deleteobject( &m_butgame );
    game_deleteobject( &m_butscores );
    game_deleteobject( &m_butoptions );
    game_deleteobject( &m_buthelp );
    game_deleteobject( &m_butexit );
    game_deleteobject( &m_butnew );
    game_deleteobject( &m_butresume );
    game_deleteobject( &m_butstop );
    game_deleteobject( &m_butviewhs );
    game_deleteobject( &m_butreset );
    game_deleteobject( &m_butviewhp );
    game_deleteobject( &m_butviewopt );
    game_deleteobject( &m_butabout );
    game_deleteobject( &m_butnext );
    game_deleteobject( &m_butprev );
    game_deleteobject( &m_butback );

    game_deleteobject( &m_title );
}

void game_main( void )
{
    int astcol, piece, i;
    unsigned char *buffer;
    long numBytesPerScanline;
    char buf[ 128 ];

#if !DEMO_VERSION
    // check for cheat code activation
    if( sys_keydownnow( DIK_PGDN ) && sys_keydown( DIK_W ) ) {
        sys_playsound( m_sounds[ SND_CHEAT ], 0 );
        sys_playsound( m_sounds[ SND_CHEAT ], 0 );
        m_cheatlevelwarp = !m_cheatlevelwarp;
    }

    if( sys_keydownnow( DIK_PGDN ) && sys_keydown( DIK_R ) ) {
        sys_playsound( m_sounds[ SND_CHEAT ], 0 );
        m_cheatroundwarp = !m_cheatroundwarp;
    }

    if( sys_keydownnow( DIK_PGDN ) && sys_keydown( DIK_G ) ) {
        sys_playsound( m_sounds[ SND_CHEAT ], 0 );
        m_cheatgod = !m_cheatgod;
    }

    if( sys_keydownnow( DIK_PGDN ) && sys_keydown( DIK_F ) ) {
        sys_playsound( m_sounds[ SND_CHEAT ], 0 );
        m_cheatfinishscreen = !m_cheatfinishscreen;
    }

    if( sys_keydownnow( DIK_PGDN ) && sys_keydown( DIK_S ) && m_gameactive ) {
        sys_playsound( m_sounds[ SND_CHEAT ], 0 );
        m_player.stateplayer.score += 1000;
    }

    if( sys_keydownnow( DIK_PGDN ) && sys_keydown( DIK_H ) ) {
        sys_playsound( m_sounds[ SND_CHEAT ], 0 );
        m_player.stateplayer.shieldhealth = 100;
    }

    if( sys_keydownnow( DIK_PGDN ) && sys_keydown( DIK_COMMA ) ) {
        sys_playsound( m_sounds[ SND_CHEAT ], 0 );
        m_player.stateplayer.gun  = GUN_MULTISHOT;
        m_player.stateplayer.ammo = MULTISHOT_MAXAMMO;
    }

    if( sys_keydownnow( DIK_PGDN ) && sys_keydown( DIK_PERIOD ) ) {
        sys_playsound( m_sounds[ SND_CHEAT ], 0 );
        m_player.stateplayer.gun  = GUN_CHARGEUP;
        m_player.stateplayer.ammo = 100;
    }

    if( sys_keydownnow( DIK_PGDN ) && sys_keydown( DIK_N ) ) {
        sys_playsound( m_sounds[ SND_CHEAT ], 0 );
        m_cheatnocollide = !m_cheatnocollide;
    }

    if( m_cheatlevelwarp ) {
        // level warp keys
        for( i = 0; i < 9; i++ ) {
            if( sys_keydown( DIK_1 + i ) ) {
                m_level = i;
                m_round = 0;
                m_player.stateplayer.livesleft = PLAYER_LIVES - 1;
                m_player.stateplayer.score     = 0;
                game_new();
                break;
            }
        }
        if( sys_keydown( DIK_0 ) ) {
            m_level = 9;
            m_round = 0;
            m_player.stateplayer.livesleft = PLAYER_LIVES - 1;
            m_player.stateplayer.score     = 0;
            game_new();
        }
    }

    if( m_cheatroundwarp ) {
        // level warp keys
        for( i = 0; i < 5; i++ ) {
            if( sys_keydown( DIK_1 + i ) ) {
                m_round = i;
                m_player.stateplayer.livesleft = PLAYER_LIVES - 1;
                m_player.stateplayer.score     = 0;
                game_new();
                break;
            }
        }
    }

    if( m_cheatfinishscreen ) {
        if( sys_keydown( DIK_F10 ) ) {
            finishscreen_activate();
        }
    }
#endif

    sys_clearsurface( g_backbuffer, 0 );

    if( m_menuactive ) {
        // check the menu bar keys
        if( STATE_ALIVE == m_menubar.state ) {
            if( sys_keydown( DIK_G ) ) {
                button_execute( &m_butgame, 0 );
            }
            if( sys_keydown( DIK_C ) ) {
                button_execute( &m_butscores, 0 );
            }
            if( sys_keydown( DIK_O ) ) {
                button_execute( &m_butoptions, 0 );
            }
            if( sys_keydown( DIK_H ) ) {
                button_execute( &m_buthelp, 0 );
            }
            if( sys_keydown( DIK_X ) ) {
                button_execute( &m_butexit, 0 );
            }
        }

        // menu buttons based on the active menu
        if( STATE_ALIVE == m_butnew.state ) {
            if( sys_keydown( DIK_N ) ) {
                button_execute( &m_butnew, 0 );
            }
            if( sys_keydown( DIK_R ) && STATE_ALIVE == m_butresume.state ) {
                button_execute( &m_butresume, 0 );
            }
            if( sys_keydown( DIK_S ) && STATE_ALIVE == m_butstop.state ) {
                button_execute( &m_butstop, 0 );
            }
        }
        else if( STATE_ALIVE == m_butviewhs.state ) {
            if( sys_keydown( DIK_V ) ) {
                button_execute( &m_butviewhs, 0 );
            }
            if( sys_keydown( DIK_R ) ) {
                button_execute( &m_butreset, 0 );
            }
        }
        else if( STATE_ALIVE == m_butviewhp.state ) {
            if( sys_keydown( DIK_V ) ) {
                button_execute( &m_butviewhp, 0 );
            }
            if( sys_keydown( DIK_A ) ) {
                button_execute( &m_butabout, 0 );
            }
        }
        else if( STATE_ALIVE == m_menuscores2.state ) {
            if( sys_keydown( DIK_B ) && STATE_ALIVE == m_butback.state ) {
                button_execute( &m_butback, 0 );
            }
        }
        else if( STATE_ALIVE == m_menuoptions.state ) {
            if( sys_keydown( DIK_V ) ) {
                button_execute( &m_butviewopt, 0 );
            }
        }
        else if( STATE_ALIVE == m_menuhelp2.state ) {
            if( sys_keydown( DIK_P ) && STATE_ALIVE == m_butprev.state ) {
                button_execute( &m_butprev, 0 );
            }
            if( sys_keydown( DIK_N ) && STATE_ALIVE == m_butnext.state ) {
                button_execute( &m_butnext, 0 );
            }
            if( sys_keydown( DIK_B ) ) {
                button_execute( &m_butback, 0 );
            }
        }
        else if( STATE_ALIVE == m_butabout.state ) {
            if( sys_keydown( DIK_V ) ) {
                button_execute( &m_butabout, 0 );
            }
        }
        else if( STATE_PREBIRTH != m_butoptpages[ 0 ].state  ) {
            if( sys_keydown( DIK_A ) ) {
                button_execute( &m_butoptpages[ 0 ], 0 );
            }
            if( sys_keydown( DIK_H ) ) {
                button_execute( &m_butoptpages[ 1 ], 0 );
            }
            if( sys_keydown( DIK_P ) ) {
                button_execute( &m_butoptpages[ 2 ], 0 );
            }
            if( sys_keydown( DIK_M ) ) {
                button_execute( &m_butoptpages[ 3 ], 0 );
            }
            if( sys_keydown( DIK_B ) ) {
                button_execute( &m_butback, 0 );
            }
        }
        else if( STATE_ALIVE == m_menuabout.state  ) {      // this one must be last since 
            if( sys_keydown( DIK_B ) ) {                    // it's reused many times
                button_execute( &m_butback, 0 );
            }
        }

        if( sys_mousebuttonup( 0 ) || sys_mousebuttonup( 2 ) ) {
            if( m_mousecapture == &m_sldrboxsound ) {
                if( m_options.soundlevel > 0 ) {
                    sys_soundon();
                    for( i = 0; i < MAX_GAMESOUNDS; i++ ) {
                        sys_setsoundvolume( m_sounds[ i ], SND_VOLUMEMULTIPLIER * ( 100 - m_options.soundlevel ) );
                    }
                    sys_playsound( m_sounds[ SND_CHEAT ], 0 );
                }
                else {
                    sys_soundoff();
                }
            }
            else if( m_mousecapture == &m_sldrboxmusic ) {
                if( m_options.musiclevel > 0 ) {
                    sys_mp3on();
                    sys_setmp3volume( m_menutrack, MP3_VOLUMEMULTIPLIER * ( 100 - m_options.musiclevel ) );
                    sys_setmp3volume( m_creditstrack, MP3_VOLUMEMULTIPLIER * ( 100 - m_options.musiclevel ) );
                    for( i = 0; i < MAX_GAMETRACKS; i++ ) {
                        sys_setmp3volume( m_tracks[ i ], MP3_VOLUMEMULTIPLIER * ( 100 - m_options.musiclevel ) );
                    }
                    sys_playmp3( m_menutrack );
                }
                else {
                    sys_stopmp3( m_tracks[ m_leveltrackidx ] );
                    sys_stopmp3( m_menutrack );
                    sys_mp3off();
                }
            }
            else if( m_mousecapture == &m_sldrboxmouse ) {
                // this will set the mouse speed from 0.25 -> 2.0
                m_options.mousespeed = 0.25f + ( m_options.mouseoffset * 0.0175f );
            }
            m_mousecapture = NULL;
        }

        starfield_think();
        cursor_think();
        portrait_think( &m_portraitplyr );
        portrait_think( &m_portraitnme );
        portrait_think( &m_astcols[ 0 ].astpieces[ ASTTYPE_PIECE0 ][ 0 ] );
        portrait_think( &m_astcols[ 1 ].astpieces[ ASTTYPE_PIECE0 ][ 0 ] );
        for( i = 0; i < MAX_PICKUPS; i++ ) {
            portrait_thinkpickup( &m_pickups[ i ] );
        }
        entername_think();

        sldr_move( &m_sldrboxsound );
        sldr_move( &m_sldrboxmusic );
        sldr_move( &m_sldrboxmouse );

        clstr_move( &m_starfield );
        cursor_move();
        //marquee_move();

        starfield_collide();
        if( NULL == m_mousecapture ) {
            cursor_collide();
        }

        clstr_begindraw( &buffer, &numBytesPerScanline );
        clstr_draw( &m_starfield, buffer, numBytesPerScanline );
        clstr_enddraw();
        asteroid_draw( &m_title );
        menu_draw( &m_menubar );
        menu_draw( &m_menugame );
        menu_draw( &m_menuscores );
        menu_draw( &m_menuscores2 );
        menu_draw( &m_menuhelp );
        menu_draw( &m_menuoptions );
        menu_draw( &m_menuoptions2 );
        menu_draw( &m_menuabout );
        menu_draw( &m_menuhelp2 );
        func_drawhelppages[ m_currentpage ]();
        portrait_draw( &m_portraitplyr );
        portrait_draw( &m_portraitnme );
        portrait_draw( &m_astcols[ 0 ].astpieces[ ASTTYPE_PIECE0 ][ 0 ] );
        portrait_draw( &m_astcols[ 1 ].astpieces[ ASTTYPE_PIECE0 ][ 0 ] );
        for( i = 0; i < MAX_PICKUPS; i++ ) {
            portrait_draw( &m_pickups[ i ] );
        }
        entername_draw();
        for( i = 0; i < MAX_HIGHSCORES; i++ ) {
            asteroid_draw( &m_scorerows[ i ] );
        }
        marquee_draw();

        button_draw( &m_sldrmusic ); button_draw( &m_sldrsound ); button_draw( &m_sldrmouse );
        for( i = 0; m_buttons[ i ]; i++ ) {
            button_draw( m_buttons[ i ] );
        }

        for( i = 0; m_multistate[ i ]; i++ ) {
            button_draw2( m_multistate[ i ] );
        }

        button_draw( &m_lblDirectional );
        button_draw( &m_lblSliding );

        button_draw( &m_drpshipphysicssel );
        button_draw( &m_butsliding ); button_draw( &m_butdirectional );

        if( STATE_ALIVE == m_sldrsound.state ) {
            g_backbuffer->Blt( &m_lblsound.dst, m_lblsound.dds, &m_lblsound.src, DDBLT_KEYSRC | DDBLT_WAIT, NULL );
            g_backbuffer->Blt( &m_lblmusic.dst, m_lblmusic.dds, &m_lblmusic.src, DDBLT_KEYSRC | DDBLT_WAIT, NULL );
        }
        else if( STATE_ALIVE == m_drpshipphysics.state ) {
            g_backbuffer->Blt( &m_lblshipphysics.dst, m_lblshipphysics.dds, &m_lblshipphysics.src, DDBLT_KEYSRC | DDBLT_WAIT, NULL );
        }
        else if( STATE_PREBIRTH != m_chkplyrshottrails.state ) {
            g_backbuffer->Blt( &m_lblplyrshottrails.dst, m_lblplyrshottrails.dds, &m_lblplyrshottrails.src, DDBLT_KEYSRC | DDBLT_WAIT, NULL );
            g_backbuffer->Blt( &m_lblplyrshotexpl.dst, m_lblplyrshotexpl.dds, &m_lblplyrshotexpl.src, DDBLT_KEYSRC | DDBLT_WAIT, NULL );
            g_backbuffer->Blt( &m_lblplyrexpl.dst, m_lblplyrexpl.dds, &m_lblplyrexpl.src, DDBLT_KEYSRC | DDBLT_WAIT, NULL );
            g_backbuffer->Blt( &m_lblnmeshottrails.dst, m_lblnmeshottrails.dds, &m_lblnmeshottrails.src, DDBLT_KEYSRC | DDBLT_WAIT, NULL );
            g_backbuffer->Blt( &m_lblnmeshotexpl.dst, m_lblnmeshotexpl.dds, &m_lblnmeshotexpl.src, DDBLT_KEYSRC | DDBLT_WAIT, NULL );
            g_backbuffer->Blt( &m_lblnmeexpl.dst, m_lblnmeexpl.dds, &m_lblnmeexpl.src, DDBLT_KEYSRC | DDBLT_WAIT, NULL );
            g_backbuffer->Blt( &m_lblthrusting.dst, m_lblthrusting.dds, &m_lblthrusting.src, DDBLT_KEYSRC | DDBLT_WAIT, NULL );
        }
        else if( STATE_ALIVE == m_sldrmouse.state ) {
            g_backbuffer->Blt( &m_lblmouse.dst, m_lblmouse.dds, &m_lblmouse.src, DDBLT_KEYSRC | DDBLT_WAIT, NULL );
        }

        if( m_drpshipdropped ) {
            button_draw( &m_lstshipphysics );
            button_draw( &m_butsliding );
            button_draw( &m_butdirectional );
        }

        font_draw(m_fontbody, g_backbuffer, m_version, SCREEN_WIDTH - 30, SCREEN_HEIGHT - 20);

#if DEMO_VERSION
        font_draw( m_fontbody, g_backbuffer, "demo version", 550, 0 );
#endif
        cursor_draw();
    }
    else {
        if( sys_keydown( DIK_ESCAPE ) && STATE_ALIVE != m_stats.state ) {
            if( STATE_ALIVE == m_player.state ) {
                m_player.state = STATE_AUTOSHIELD;
            }
            game_pause();
            menu_view();
            if( !m_gameactive ) {
                entername_init();
            }
        }

        if( STATE_ALIVE == m_stats.state ) {
            // allow the user to press to dismiss the end of level stats dialog early
            if( sys_keydown( DIK_ESCAPE ) || sys_keydown( DIK_SPACE ) || sys_keydown( DIK_RETURN ) ) {
                m_stats.stateasteroid.framecounter = STATS_THRESHOLD + 1;
            }
        }

        // ai
        for( piece = 0; piece < MAX_SMALLESTTYPES; piece++ ) {
            for( i = 0; i < MAX_SMALLESTDEBRIS; i++ ) {
                debris_think( &m_smallestplyr[ piece ][ i ] );
                debris_think( &m_smallestnme[ piece ][ i ] );
                debris_think( &m_smallestast0[ piece ][ i ] );
                debris_think( &m_smallestast1[ piece ][ i ] );
            }
        }

        for( i = 0; i < MAX_NMESHIPS; i++ ) {
            nme_think( &m_nmeships[ i ] );
        }
        if( m_playerready ) {
            nme_mgrthink();
        }
        risingscore_think( &m_risingscore );
        leveltitle_think();
        gameover_think();
        stats_think();
        player_think();
        starfield_think();
        if( m_currentpickup ) {
            pickup_think( m_currentpickup );
        }
        for( i = 0; i < MAX_SHOTS * 3; i++ ) {          // x3 coz of multishot
            shot_think( &m_shots[ i ] );
        }
        for( i = 0; i < BOMBSCATTER_MAXSHOTS; i++ ) {
            bombscatter_think( &m_scattershots[ i ] );
        }
        for( astcol = 0; astcol < MAX_CHARGEUPSIZES; astcol++ ) {
            for( i = 0; i < MAX_CHARGEUPSHOTS; i++ ) {
                chargeupshot_think( &m_chargeupshots[ astcol ][ i ] );
            }
        }
        for( i = 0; i < MAX_NMESHOTS; i++ ) {
            shot_think( &m_nmeshots[ i ] );
        }
        for( i = 0; i < MAX_SHOTTRAILS; i++ ) {
            shottrail_think( &m_shottrails[ i ] );
            shottrail_think( &m_chargeupshottrails[ i ] );
        }
        for( i = 0; i < MAX_NMESHOTTRAILS; i++ ) {
            shottrail_think( &m_nmeshottrails[ i ] );
        }
        for( i = 0; i < MAX_SHOTRICHOCHETS; i++ ) {
            shotrichochet_think( &m_shotrichochets[ i ] );
            shotrichochet_think( &m_plyrcollide[ i ] );
            shotrichochet_think( &m_nmeexp[ i ] );
            shotrichochet_think( &m_chargeupshotrichochets[ i ] );
        }
        for( i = 0; i < MAX_NMESHOTRICHOCHETS; i++ ) {
            shotrichochet_think( &m_nmeshotrichochets[ i ] );
        }
        for( astcol = 0; astcol < MAX_DEBRIS; astcol++ ) {
            for( i = 0; i < MAX_ACTIVEDEBRIS; i++ ) {
                asteroid_think( &m_debris[ astcol ][ i ] );
                asteroid_think( &m_nmedebris[ astcol ][ i ] );
            }
        }
        plyrtrail_think( &m_plyrtrail1 );
        plyrtrail_think( &m_plyrtrail2 );
        shotrichochet_think( &m_plyrexp );
        spitparticles_think( &m_chargeupspit );
        for( astcol = 0; astcol < MAX_ASTCOLS; astcol++ ) {
            for( piece = 0; piece < MAX_ASTPIECES; piece++ ) {
                for( i = 0; i < MAX_ASTS; i++ ) {
                    asteroid_think( &m_astcols[ astcol ].astpieces[ piece ][ i ] );
                }
            }
        }

        // movement
        for( piece = 0; piece < MAX_SMALLESTTYPES; piece++ ) {
            for( i = 0; i < MAX_SMALLESTDEBRIS; i++ ) {
                asteroid_move( &m_smallestplyr[ piece ][ i ] );
                asteroid_move( &m_smallestnme[ piece ][ i ] );
                asteroid_move( &m_smallestast0[ piece ][ i ] );
                asteroid_move( &m_smallestast1[ piece ][ i ] );
            }
        }
        for( i = 0; i < MAX_NMESHIPS; i++ ) {
            nme_move( &m_nmeships[ i ] );
        }
        if( m_currentpickup ) {
            pickup_move( m_currentpickup );
        }
        risingscore_move( &m_risingscore );
        player_move();
        clstr_move( &m_starfield );
        for( i = 0; i < MAX_SHOTS * 3; i++ ) {              // x3 coz of multishot
            shot_move( &m_shots[ i ] );
        }
        for( i = 0; i < BOMBSCATTER_MAXSHOTS; i++ ) {
            shot_move( &m_scattershots[ i ] );
        }
        for( astcol = 0; astcol < MAX_CHARGEUPSIZES; astcol++ ) {
            for( i = 0; i < MAX_CHARGEUPSHOTS; i++ ) {
                shot_move( &m_chargeupshots[ astcol ][ i ] );
            }
        }
        for( i = 0; i < MAX_NMESHOTS; i++ ) {
            shot_move( &m_nmeshots[ i ] );
        }
        for( i = 0; i < MAX_SHOTTRAILS; i++ ) {
            shottrail_move( &m_shottrails[ i ] );
            shottrail_move( &m_chargeupshottrails[ i ] );
        }
        for( i = 0; i < MAX_NMESHOTTRAILS; i++ ) {
            shottrail_move( &m_nmeshottrails[ i ] );
        }
        for( i = 0; i < MAX_SHOTRICHOCHETS; i++ ) {
            clstr_move( &m_shotrichochets[ i ] );
            clstr_move( &m_plyrcollide[ i ] );
            clstr_move( &m_nmeexp[ i ] );
            clstr_move( &m_chargeupshotrichochets[ i ] );
        }
        for( i = 0; i < MAX_NMESHOTRICHOCHETS; i++ ) {
            clstr_move( &m_nmeshotrichochets[ i ] );
        }
        clstr_move( &m_plyrtrail1 );
        clstr_move( &m_plyrtrail2 );
        clstr_move( &m_plyrexp );
        clstr_move( &m_chargeupspit );
        for( astcol = 0; astcol < MAX_DEBRIS; astcol++ ) {
            for( i = 0; i < MAX_ACTIVEDEBRIS; i++ ) {
                asteroid_move( &m_debris[ astcol ][ i ] );
                asteroid_move( &m_nmedebris[ astcol ][ i ] );
            }
        }
        for( astcol = 0; astcol < MAX_ASTCOLS; astcol++ ) {
            for( piece = 0; piece < MAX_ASTPIECES; piece++ ) {
                for( i = 0; i < MAX_ASTS; i++ ) {
                    asteroid_move( &m_astcols[ astcol ].astpieces[ piece ][ i ] );
                }
            }
        }

        // collisions
        for( piece = 0; piece < MAX_SMALLESTTYPES; piece++ ) {
            for( i = 0; i < MAX_SMALLESTDEBRIS; i++ ) {
                asteroid_collide( &m_smallestplyr[ piece ][ i ], 0 );
                asteroid_collide( &m_smallestnme[ piece ][ i ], 0 );
                asteroid_collide( &m_smallestast0[ piece ][ i ], 0 );
                asteroid_collide( &m_smallestast1[ piece ][ i ], 0 );
            }
        }
        for( i = 0; i < MAX_NMESHIPS; i++ ) {
            nme_collide( &m_nmeships[ i ] );
        }
        if( m_currentpickup ) {
            pickup_collide( m_currentpickup );
        }
        player_collide( 0 );
        starfield_collide();
        for( i = 0; i < MAX_SHOTS * 3; i++ ) {              // x3 coz of multishot
            shot_collide( &m_shots[ i ] );
        }
        for( i = 0; i < BOMBSCATTER_MAXSHOTS; i++ ) {
            shot_collide( &m_scattershots[ i ] );
        }
        for( astcol = 0; astcol < MAX_CHARGEUPSIZES; astcol++ ) {
            for( i = 0; i < MAX_CHARGEUPSHOTS; i++ ) {
                chargeupshot_collide( &m_chargeupshots[ astcol ][ i ] );
            }
        }
        for( i = 0; i < MAX_NMESHOTS; i++ ) {
            nmeshot_collide( &m_nmeshots[ i ] );
        }
        /*for( i = 0; i < MAX_SHOTTRAILS; i++ ) {       // don't collide with anything... yet...
            clstr_collide( &m_shottrails[ i ] );
        }
        for( i = 0; i < MAX_SHOTRICHOCHETS; i++ ) {
            clstr_collide( &m_shotrichochets[ i ] );
        }*/
        for( astcol = 0; astcol < MAX_DEBRIS; astcol++ ) {
            for( i = 0; i < MAX_ACTIVEDEBRIS; i++ ) {
                asteroid_collide( &m_debris[ astcol ][ i ], 0 );
                asteroid_collide( &m_nmedebris[ astcol ][ i ], 0 );
            }
        }

        for( astcol = 0; astcol < MAX_ASTCOLS; astcol++ ) {
            for( piece = 0; piece < MAX_ASTPIECES; piece++ ) {
                for( i = 0; i < MAX_ASTS; i++ ) {
                    asteroid_collide( &m_astcols[ astcol ].astpieces[ piece ][ i ], 1 );
                }
            }
        }

        // drawing
        clstr_begindraw( &buffer, &numBytesPerScanline );
        clstr_draw( &m_starfield, buffer, numBytesPerScanline );
        clstr_enddraw();

        for( i = 0; i < MAX_SHOTS * 3; i++ ) {              // x3 coz of multishot
            shot_draw( &m_shots[ i ] );
        }
        for( i = 0; i < BOMBSCATTER_MAXSHOTS; i++ ) {
            shot_draw( &m_scattershots[ i ] );
        }
        for( astcol = 0; astcol < MAX_CHARGEUPSIZES; astcol++ ) {
            for( i = 0; i < MAX_CHARGEUPSHOTS; i++ ) {
                shot_draw( &m_chargeupshots[ astcol ][ i ] );
            }
        }
        for( i = 0; i < MAX_NMESHOTS; i++ ) {
            shot_draw( &m_nmeshots[ i ] );
        }
        for( astcol = 0; astcol < MAX_DEBRIS; astcol++ ) {
            for( i = 0; i < MAX_ACTIVEDEBRIS; i++ ) {
                asteroid_draw( &m_debris[ astcol ][ i ] );
                asteroid_draw( &m_nmedebris[ astcol ][ i ] );
            }
        }
        for( astcol = 0; astcol < MAX_ASTCOLS; astcol++ ) {
            for( piece = 0; piece < MAX_ASTPIECES; piece++ ) {
                for( i = 0; i < MAX_ASTS; i++ ) {
                    asteroid_draw( &m_astcols[ astcol ].astpieces[ piece ][ i ] );
                }
            }
        }
        for( piece = 0; piece < MAX_SMALLESTTYPES; piece++ ) {
            for( i = 0; i < MAX_SMALLESTDEBRIS; i++ ) {
                asteroid_draw( &m_smallestplyr[ piece ][ i ] );
                asteroid_draw( &m_smallestnme[ piece ][ i ] );
                asteroid_draw( &m_smallestast0[ piece ][ i ] );
                asteroid_draw( &m_smallestast1[ piece ][ i ] );
            }
        }
        for( i = 0; i < MAX_NMESHIPS; i++ ) {
            nme_draw( &m_nmeships[ i ] );
        }
        if( m_currentpickup ) {
            pickup_draw( m_currentpickup );
        }
        clstr_begindraw( &buffer, &numBytesPerScanline );
        if( buffer && numBytesPerScanline ) {
            for( i = 0; i < MAX_SHOTTRAILS; i++ ) {
                clstr_draw( &m_shottrails[ i ], buffer, numBytesPerScanline );
            }
            for( i = 0; i < MAX_NMESHOTTRAILS; i++ ) {
                clstr_draw( &m_nmeshottrails[ i ], buffer, numBytesPerScanline );
            }
            for( i = 0; i < MAX_SHOTRICHOCHETS; i++ ) {
                clstr_draw( &m_shotrichochets[ i ], buffer, numBytesPerScanline );
                clstr_draw( &m_plyrcollide[ i ], buffer, numBytesPerScanline );
                clstr_draw( &m_nmeexp[ i ], buffer, numBytesPerScanline );
                clstr_draw( &m_chargeupshottrails[ i ], buffer, numBytesPerScanline );
                clstr_draw( &m_chargeupshotrichochets[ i ], buffer, numBytesPerScanline );
            }
            for( i = 0; i < MAX_NMESHOTRICHOCHETS; i++ ) {
                clstr_draw( &m_nmeshotrichochets[ i ], buffer, numBytesPerScanline );
            }
            clstr_draw( &m_plyrtrail1, buffer, numBytesPerScanline );
            clstr_draw( &m_plyrtrail2, buffer, numBytesPerScanline );
            clstr_draw( &m_plyrexp, buffer, numBytesPerScanline );
            clstr_draw( &m_chargeupspit, buffer, numBytesPerScanline );
        }
        clstr_enddraw();

        player_draw();

        if( m_risingscore.state == STATE_ALIVE ) {
            font_draw( m_risingscore.staterisingscore.largefont ? m_fontscores : m_fontdamage, g_backbuffer
                     , m_risingscore.staterisingscore.string
                     , (int)m_risingscore.x, (int)m_risingscore.y );
        }

        gameover_draw();
        leveltitle_render();
        stats_draw();
        finishscreen_draw();

        if( STATE_ALIVE == m_hudstate ) {
            sprintf_s( buf, 128, "score: %d", m_player.stateplayer.score );
            font_draw(m_fontbody, g_backbuffer, buf, 0, SCREEN_HEIGHT - 15);

            sprintf_s( buf, 128, "lives: %d", m_player.stateplayer.livesleft );
            font_draw( m_fontbody, g_backbuffer, buf, 100, SCREEN_HEIGHT - 15);

            sprintf_s( buf, 128, "level: %d, %d", m_level + 1, m_round + 1 );
            font_draw( m_fontbody, g_backbuffer, buf, 200, SCREEN_HEIGHT - 15);

            int elapsed = static_cast<int>(m_timeelapsed + (time(NULL) - m_timemark));
            sprintf_s( buf, 128, "time: %d", elapsed);
            font_draw( m_fontbody, g_backbuffer, buf, 300, SCREEN_HEIGHT - 15);

            sprintf_s( buf, 128, "shield: %d%%", m_player.stateplayer.shieldhealth );

            if( m_player.stateplayer.shieldhealth < 25 ) {
                if( m_player.stateplayer.shieldhealth < 10 ) {
                    if( ++m_lowshieldcounter > 10 ) {
                        m_lowshieldcounter = 0;
                        m_lowshield = !m_lowshield;
                    }
                }
                else {
                    m_lowshield = 1;
                }
            }
            else {
                m_lowshield = 1;
            }

            if( m_lowshield ) {
                font_draw( m_player.stateplayer.shieldhealth < 25 ? m_fontbodyred : m_fontbody, g_backbuffer, buf, 400, SCREEN_HEIGHT - 15);
            }

            if( GUN_MULTISHOT == m_player.stateplayer.gun ) {
                if( -1 == m_player.stateplayer.ammo ) {
                    strcpy_s( buf, "shots: unlimited" );
                }
                else {
                    sprintf_s( buf, 128, "shots: %d", m_player.stateplayer.ammo );
                }
                font_draw( m_fontbody, g_backbuffer, buf, 500, SCREEN_HEIGHT - 15);
            }
            else if( GUN_CHARGEUP == m_player.stateplayer.gun ) {
                sprintf_s( buf, 128, "charge: %d%%", m_player.stateplayer.ammo );
                font_draw( m_fontbody, g_backbuffer, buf, 500, SCREEN_HEIGHT - 15);
            }
        }
#if DEMO_VERSION
        font_draw( m_fontbody, g_backbuffer, "demo version", 550, 0 );
#endif
    }

    //if( sys_keydown( DIK_F8 ) ) {
    //    sys_screenshot();
    //}
}

int game_createobject( obj_t *obj, int w, int h, const char *filename )
{
    if( NULL == obj || NULL == filename ) {
        strcpy_s( g_gameerror, 1024, "game_createobject: was passed a null pointer" );
        return 0;
    }

    memset( obj, 0, sizeof( obj_t ) );

    obj->width          = w;
    obj->height         = h;
    obj->halfwidth      = w / 2;
    obj->halfheight     = h / 2;
    obj->bbwidth        = (int)( (float)obj->halfwidth  * 0.75f );
    obj->bbheight       = (int)( (float)obj->halfheight * 0.75f );

    if( !sys_createsurface( filename, &obj->dds, &obj->bmpwidth ) ) {
        strcpy_s( g_gameerror, 1024, g_syserror );
        return 0;
    }

    return 1;
}

void game_deleteobject( obj_t *obj )
{
    if( NULL == obj ) {
        strcpy_s( g_gameerror, 1024, "game_deleteobject: was passed a null pointer" );
        return;
    }

    if( !obj->clone ) {
        sys_deletesurface( obj->dds );
    }

    memset( obj, 0, sizeof( obj_t ) );
}

int game_setframe( obj_t *obj, int frame )
{
    long offset;

    if( NULL == obj ) {
        strcpy_s( g_gameerror, 1024, "game_setframe: was passed a null pointer" );
        return 0;
    }

    offset = frame * obj->width;
    if( offset >= obj->bmpwidth ) {
        sprintf_s( g_gameerror, 1024, "game_setframe: was passed an invalid frame index of [%d]", frame );
        return 0;
    }

    obj->src.left  = offset;
    obj->src.right = obj->src.left + obj->width;

    obj->src.top    = 0;
    obj->src.bottom = obj->height;

    return 1;
}

//--------------------------------------------------------------------------------
// private functions
//--------------------------------------------------------------------------------

static int game_cloneobject( obj_t *dstobj, obj_t *srcobj )
{
    if( NULL == srcobj || NULL == dstobj ) {
        strcpy_s( g_gameerror, 1024, "game_cloneobject: was passed a null pointer" );
        return 0;
    }

    memcpy( dstobj, srcobj, sizeof( obj_t ) );

    dstobj->clone = 1;
    return 1;
}

static int game_incframe( obj_t *obj )
{
    if( NULL == obj ) {
        strcpy_s( g_gameerror, 1024, "game_incframe: was passed a null pointer" );
        return 0;
    }

    obj->src.left += obj->width;
    if( obj->src.left >= obj->bmpwidth ) {
        obj->src.left = 0;
    }

    obj->src.right = obj->src.left + obj->width;
    return 1;
}

static int game_decframe( obj_t *obj )
{
    if( NULL == obj ) {
        strcpy_s( g_gameerror, 1024, "game_decframe: was passed a null pointer" );
        return 0;
    }

    obj->src.left -= obj->width;
    if( obj->src.left < 0 ) {
        obj->src.left = obj->bmpwidth - obj->width;
    }

    obj->src.right = obj->src.left + obj->width;
    return 1;
}

static int game_objcollide( obj_t *src, obj_t *dst )
{
    if( ( STATE_ALIVE == src->state || STATE_NMEWPWAIT == src->state ) &&
        ( STATE_ALIVE == dst->state || STATE_NMEWPWAIT == dst->state ) ) {
        if( game_pointinbox( (int)src->x, (int)src->y
                           , (int)dst->x, (int)dst->y
                           , dst->bbwidth, dst->bbheight ) ) {
            src->state = STATE_DYING;
            dst->state = STATE_DYING;
            return 1;
        }

        if( game_pointinbox( (int)src->x - (int)src->bbwidth, (int)src->y - (int)src->bbheight
                           , (int)dst->x, (int)dst->y
                           , dst->bbwidth, dst->bbheight ) ) {
            src->state = STATE_DYING;
            dst->state = STATE_DYING;
            return 1;
        }

        if( game_pointinbox( (int)src->x - (int)src->bbwidth, (int)src->y + (int)src->bbheight
                           , (int)dst->x, (int)dst->y
                           , dst->bbwidth, dst->bbheight ) ) {
            src->state = STATE_DYING;
            dst->state = STATE_DYING;
            return 1;
        }

        if( game_pointinbox( (int)src->x + (int)src->bbwidth, (int)src->y - (int)src->bbheight
                           , (int)dst->x, (int)dst->y
                           , dst->bbwidth, dst->bbheight ) ) {
            src->state = STATE_DYING;
            dst->state = STATE_DYING;
            return 1;
        }

        if( game_pointinbox( (int)src->x + (int)src->bbwidth, (int)src->y + (int)src->bbheight
                           , (int)dst->x, (int)dst->y
                           , dst->bbwidth, dst->bbheight ) ) {
            src->state = STATE_DYING;
            dst->state = STATE_DYING;
            return 1;
        }
    }

    return 0;
}

static int game_maptoscreen( obj_t *obj )
{
    if( NULL == obj ) {
        strcpy_s( g_gameerror, 1024, "game_maptoscreen: was passed a null pointer" );
        return 0;
    }

    obj->dst.left = (long)( (float)obj->x - (float)obj->halfwidth  );
    obj->dst.top  = (long)( (float)obj->y - (float)obj->halfheight );

    obj->dst.right  = obj->dst.left + obj->width;
    obj->dst.bottom = obj->dst.top  + obj->height;

    return 1;
}

static void player_think( void )
{
    if( m_player.stateplayer.cfcounteractive ) {
        if( ++m_player.stateplayer.cfcount > PLYRCOLLIDE_CFTHRESHOLD ) {
            m_player.stateplayer.cfcount = 0;
            m_player.stateplayer.cfcounteractive = 0;
        }
    }

    if( !sys_mousebuttondownnow( 0 ) && !sys_keydownnow( DIK_LCONTROL ) && !sys_keydownnow( DIK_RCONTROL ) ) {
        m_chargeupspit.obj = NULL;
    }

    switch( m_player.state ) {
        case STATE_AUTOSHIELD :
            if( ++m_player.stateplayer.autocounter >= PLAYER_AUTOCOUNTERMAX ) {
                m_player.stateplayer.autocounter = 0;
                m_player.stateplayer.autostate   = !m_player.stateplayer.autostate;
                if( ++m_player.stateplayer.autoshield >= PLAYER_AUTOSHIELDMAX ) {
                    m_player.stateplayer.autoshield = 0;
                    m_player.stateplayer.autostate  = 0;
                    m_player.state                  = STATE_ALIVE;
                    m_playerready                   = 1;

                    if( player_collide( 1 ) ) {
                        m_player.state                  = STATE_AUTOSHIELD;
                        m_player.stateplayer.autoshield = 1;
                    }
                }
            }
            // fall thru

        case STATE_ALIVE :
            if( !m_player.stateplayer.isguncharging ) {
                if( sys_mousebuttondownnow( 1 ) || sys_keydownnow( DIK_LSHIFT ) || sys_keydownnow( DIK_RSHIFT ) ) {
                    if( m_player.stateplayer.shieldhealth > 0 ) {
                        if( !m_player.stateplayer.shield && STATE_AUTOSHIELD != m_player.state ) {
                            sys_playsound( m_sounds[ SND_PLYRSHLDUP ], 0 );
                        }
                        m_player.stateplayer.shield = 1;
                    }
                }
                else if( STATE_AUTOSHIELD != m_player.state ){
                    if( m_player.stateplayer.shield ) {
                        sys_playsound( m_sounds[ SND_PLYRSHLDDN ], 0 );
                    }
                    m_player.stateplayer.shield = 0;

                    if( m_player.stateplayer.gun == GUN_CHARGEUP && ( sys_mousebuttondownnow( 0 ) || sys_keydownnow( DIK_LCONTROL ) || sys_keydownnow( DIK_RCONTROL ) ) ) {
                        // begin charge up
                        m_player.stateplayer.isguncharging = 1;
                        sys_playsound( m_sounds[ SND_PLYRCHGUP ], 0 );

                        shotrichochet_init( &m_chargeupspit
                                          , -1, m_player.x, m_player.y
                                          , CHARGEUPTRAIL_MINRED,   CHARGEUPTRAIL_MAXRED
                                          , CHARGEUPTRAIL_MINGREEN, CHARGEUPTRAIL_MAXGREEN
                                          , CHARGEUPTRAIL_MINBLUE,  CHARGEUPTRAIL_MAXBLUE
                                          , CHARGEUPTRAIL_DESTHUE,  CHARGEUPSPIT_INC );

                        m_chargeupspit.obj      = &m_player;
                        m_chargeupspit.state    = STATE_ALIVE;
                        m_chargeupspit.addrate  = CHARGEUPSPIT_ADD;
                    }
                }
            }

            if( sys_keydownnow( DIK_UP ) ) {
                if( m_options.playerthrusting ) {
                    plyrtrail_add1( &m_plyrtrail1, &m_player, 0
                          , PLYRTHRUSTTRAILF_MINRED,   PLYRTHRUSTTRAILF_MAXRED
                          , PLYRTHRUSTTRAILF_MINGREEN, PLYRTHRUSTTRAILF_MAXGREEN
                          , PLYRTHRUSTTRAILF_MINBLUE,  PLYRTHRUSTTRAILF_MAXBLUE );

                    plyrtrail_add1( &m_plyrtrail2, &m_player, 1
                          , PLYRTHRUSTTRAILF_MINRED,   PLYRTHRUSTTRAILF_MAXRED
                          , PLYRTHRUSTTRAILF_MINGREEN, PLYRTHRUSTTRAILF_MAXGREEN
                          , PLYRTHRUSTTRAILF_MINBLUE,  PLYRTHRUSTTRAILF_MAXBLUE );
                }

                if( !m_options.physicstype ) {
                    m_player.spd += PLAYER_ACCEL;
                    if( m_player.spd >= PLAYER_MAXSPEED ) {
                        m_player.spd = PLAYER_MAXSPEED;
                    }
                }
                else {
                    m_player.spdx += ( PLAYER_ACCEL * (float)cos( PI_OVER_180 * m_player.direction ) );
                    if( m_player.spdx >= PLAYER_MAXSPEED ) {
                        m_player.spdx = ( PLAYER_MAXSPEED * (float)cos( PI_OVER_180 * m_player.direction ) );
                    }

                    m_player.spdy += ( PLAYER_ACCEL * (float)sin( PI_OVER_180 * m_player.direction ) );
                    if( m_player.spdy >= PLAYER_MAXSPEED ) {
                        m_player.spdy = ( PLAYER_MAXSPEED * (float)sin( PI_OVER_180 * m_player.direction ) );
                    }
                }
            }

            if( sys_keydownnow( DIK_DOWN ) ) {
                if( m_options.playerthrusting ) {
                    plyrtrail_add2( &m_plyrtrail1, &m_player, 0
                          , PLYRTHRUSTTRAILB_MINRED,   PLYRTHRUSTTRAILB_MAXRED
                          , PLYRTHRUSTTRAILB_MINGREEN, PLYRTHRUSTTRAILB_MAXGREEN
                          , PLYRTHRUSTTRAILB_MINBLUE,  PLYRTHRUSTTRAILB_MAXBLUE );

                    plyrtrail_add2( &m_plyrtrail2, &m_player, 1
                          , PLYRTHRUSTTRAILB_MINRED,   PLYRTHRUSTTRAILB_MAXRED
                          , PLYRTHRUSTTRAILB_MINGREEN, PLYRTHRUSTTRAILB_MAXGREEN
                          , PLYRTHRUSTTRAILB_MINBLUE,  PLYRTHRUSTTRAILB_MAXBLUE );
                }

                if( !m_options.physicstype ) {
                    m_player.spd -= PLAYER_ACCEL;
                    if( m_player.spd <= -PLAYER_MAXSPEED ) {
                        m_player.spd = -PLAYER_MAXSPEED;
                    }
                }
                else {
                    m_player.spdx -= ( PLAYER_ACCEL * (float)cos( PI_OVER_180 * m_player.direction ) );
                    if( m_player.spdx <= -PLAYER_MAXSPEED ) {
                        m_player.spdx = ( -PLAYER_MAXSPEED * (float)cos( PI_OVER_180 * m_player.direction ) );
                    }

                    m_player.spdy -= ( PLAYER_ACCEL * (float)sin( PI_OVER_180 * m_player.direction ) );
                    if( m_player.spdy <= -PLAYER_MAXSPEED ) {
                        m_player.spdy = ( -PLAYER_MAXSPEED * (float)sin( PI_OVER_180 * m_player.direction ) );
                    }
                }
            }

            if( sys_keydownnow( DIK_LEFT ) ) {
                m_player.direction -= PLAYER_TURNINC;
                if( m_player.direction < 0 ) {
                    m_player.direction += 360;
                }
            }
            else if( g_mousestate.lX < 0 ) {
                m_player.direction += (int)( (float)g_mousestate.lX * m_options.mousespeed );
                if( m_player.direction < 0 ) {
                    m_player.direction += 360;
                }
            }

            if( sys_keydownnow( DIK_RIGHT ) ) {
                m_player.direction += PLAYER_TURNINC;
                if( m_player.direction >= 360 ) {
                    m_player.direction -= 360;
                }
            }
            else if( g_mousestate.lX > 0 ) {
                m_player.direction += (int)( (float)g_mousestate.lX * m_options.mousespeed );
                if( m_player.direction >= 360 ) {
                    m_player.direction -= 360;
                }
            }

            if( m_player.state != STATE_AUTOSHIELD ) {
                if( m_player.stateplayer.gun == GUN_CHARGEUP ) {
                    if( m_player.stateplayer.cooldown >= PLAYER_GUNCOOLDOWN && !m_player.stateplayer.shield ) {
                        if( sys_mousebuttondown( 0 ) ) {
                            // begin charge up
                            m_player.stateplayer.isguncharging = 1;
                            sys_playsound( m_sounds[ SND_PLYRCHGUP ], 0 );

                            shotrichochet_init( &m_chargeupspit
                                              , -1, m_player.x, m_player.y
                                              , CHARGEUPTRAIL_MINRED,   CHARGEUPTRAIL_MAXRED
                                              , CHARGEUPTRAIL_MINGREEN, CHARGEUPTRAIL_MAXGREEN
                                              , CHARGEUPTRAIL_MINBLUE,  CHARGEUPTRAIL_MAXBLUE
                                              , CHARGEUPTRAIL_DESTHUE,  CHARGEUPSPIT_INC );

                            m_chargeupspit.obj      = &m_player;
                            m_chargeupspit.state    = STATE_ALIVE;
                            m_chargeupspit.addrate  = CHARGEUPSPIT_ADD;
                        }
                        else if( sys_mousebuttonup( 0 ) ) {
                            // fire the charged shot
                            player_fireshot();
                            m_player.stateplayer.isguncharging = 0;
                            m_player.stateplayer.charge        = 0;
                        }
                    }
                }
                else if( sys_mousebuttondownnow( 0 ) || sys_keydownnow( DIK_LCONTROL ) || sys_keydownnow( DIK_RCONTROL ) ) {
                    player_fireshot();
                }
            }

            if( sys_keydownnow( DIK_SPACE ) || sys_keydownnow( DIK_RETURN ) ) {
                m_player.spd  = 0.0f;
                m_player.spdx = 0.0f;
                m_player.spdy = 0.0f;
            }

            if( ++m_player.stateplayer.cooldown >= PLAYER_GUNCOOLDOWN ) {
                m_player.stateplayer.cooldown = PLAYER_GUNCOOLDOWN;
            }

            if( m_player.stateplayer.isguncharging ) {

                // shake the player around and spit particles
                m_player.x += (float)( ( (int)rand() % (int)3 )- (int)1 );
                m_player.y += (float)( ( (int)rand() % (int)3 )- (int)1 );

                m_player.stateplayer.charge++;
                if( --m_player.stateplayer.ammo <= 0 ) {
                    m_player.stateplayer.ammo          = 0;
                    m_player.stateplayer.isguncharging = 0;

                    player_fireshot();

                    m_player.stateplayer.gun    = GUN_NORMAL;
                    m_player.stateplayer.charge = 0;
                }
            }

            player_lookupframe( &m_player );
            break;

        case STATE_DYING :
            if( ++m_player.stateplayer.counter > PLAYER_PAUSETHRESHOLD ) {
                m_player.stateplayer.counter = 0;

                // subtract a life - unless we're cheating ;)
                if( !m_cheatgod && !ISBONUSROUND() ) {
                    if( --m_player.stateplayer.livesleft < 0 ) {
                        // game over - no lives left
                        m_player.state = STATE_DEAD;
                        game_stats();
                        //gameover_init();
                    }
                    else {
                        player_reset();
                    }
                }
                else {
                    player_reset();
                }
            }
            break;
    }
}

static void player_move( void )
{
    switch( m_player.state ) {
        case STATE_AUTOSHIELD :
        case STATE_ALIVE :
            if( !m_options.physicstype ) {
                m_player.x += ( m_player.spd * (float)cos( PI_OVER_180 * m_player.direction ) );
                m_player.y += ( m_player.spd * (float)sin( PI_OVER_180 * m_player.direction ) );
            }
            else {
                m_player.x += m_player.spdx;
                m_player.y += m_player.spdy;
            }
            break;

    }
}

static int player_collide( int flag )
{
    int astcol, piece, i;
    int points;
    obj_t *dstobj;

    switch( m_player.state ) {
        case STATE_AUTOSHIELD :
            if( m_player.x < 0.0f ) {
                m_player.x = 0.0f;
                if( !m_options.physicstype ) {
                    reflect_headingleft( &m_player );
                }
                else {
                    sys_playsound( m_sounds[ SND_BOUNCE ], 0 );
                    m_player.spdx = -m_player.spdx;
                }
            }
            else if( m_player.x > SCREEN_WIDTH_f ) {
                m_player.x = SCREEN_WIDTH_f;
                if( !m_options.physicstype ) {
                    reflect_headingright( &m_player );
                }
                else {
                    sys_playsound( m_sounds[ SND_BOUNCE ], 0 );
                    m_player.spdx = -m_player.spdx;
                }
            }

            if( m_player.y < 0.0f ) {
                m_player.y = 0.0f;
                if( !m_options.physicstype ) {
                    reflect_headingup( &m_player );
                }
                else {
                    sys_playsound( m_sounds[ SND_BOUNCE ], 0 );
                    m_player.spdy = -m_player.spdy;
                }
            }
            else if( m_player.y > SCREEN_HEIGHT_f ) {
                m_player.y = SCREEN_HEIGHT_f;
                if( !m_options.physicstype ) {
                    reflect_headingdown( &m_player );
                }
                else {
                    sys_playsound( m_sounds[ SND_BOUNCE ], 0 );
                    m_player.spdy = -m_player.spdy;
                }
            }
            break;

        case STATE_ALIVE :
            if( m_player.x < 0.0f ) {
                m_player.x = 0.0f;
                if( !m_options.physicstype ) {
                    reflect_headingleft( &m_player );
                }
                else {
                    sys_playsound( m_sounds[ SND_BOUNCE ], 0 );
                    m_player.spdx = -m_player.spdx;
                }
            }
            else if( m_player.x > SCREEN_WIDTH_f ) {
                m_player.x = SCREEN_WIDTH_f;
                if( !m_options.physicstype ) {
                    reflect_headingright( &m_player );
                }
                else {
                    sys_playsound( m_sounds[ SND_BOUNCE ], 0 );
                    m_player.spdx = -m_player.spdx;
                }
            }

            if( m_player.y < 0.0f ) {
                m_player.y = 0.0f;
                if( !m_options.physicstype ) {
                    reflect_headingup( &m_player );
                }
                else {
                    sys_playsound( m_sounds[ SND_BOUNCE ], 0 );
                    m_player.spdy = -m_player.spdy;
                }
            }
            else if( m_player.y > SCREEN_HEIGHT_f ) {
                m_player.y = SCREEN_HEIGHT_f;
                if( !m_options.physicstype ) {
                    reflect_headingdown( &m_player );
                }
                else {
                    sys_playsound( m_sounds[ SND_BOUNCE ], 0 );
                    m_player.spdy = -m_player.spdy;
                }
            }

            // collide with the current pickup
            if( m_currentpickup ) {

                // the game_objcollide() function only compares objects whose
                // state is STATE_ALIVE.  pickup objects use STATE_LINE and STATE_CIRCLE.

                m_currentpickup->savedstate = m_currentpickup->state;
                m_currentpickup->state = STATE_ALIVE;

                if( game_objcollide( &m_player, m_currentpickup ) ) {
                    player_awardpickup();
                    m_player.state = STATE_ALIVE;
                    m_currentpickup->state = STATE_DEAD;
                    m_currentpickup = NULL;
                    nme_mgrreset();

                    if( m_player.stateplayer.isguncharging ) {
                        player_fireshot();
                    }

                    return 1;
                }

                m_currentpickup->state = m_currentpickup->savedstate;
            }

            if( !m_cheatnocollide ) {
                // collide with the enemy's shots
                for( i = 0; i < MAX_NMESHOTS; i++ ) {
                    if( game_objcollide( &m_player, &m_nmeshots[ i ] ) ) {
                        if( !flag ) {
                            if( m_player.stateplayer.shield ) {
                                // bounce off
                                m_player.state = STATE_ALIVE;
                                m_nmeshots[ i ].state = STATE_DEAD;
                                reflect_movingobj( &m_nmeshots[ i ] );
                            }
                            else {
                                m_player.stateplayer.gun = GUN_NORMAL;
                                player_explode( &m_nmeshots[ i ] );
                                debris_spawn( &m_player, 0 );
                                if( m_options.playerexpl ) {
                                    shotrichochet_setactive( &m_plyrexp
                                                           , -1, m_player.x, m_player.y
                                                           , PLYREXP_MINRED,    PLYREXP_MAXRED
                                                           , PLYREXP_MINGREEN,  PLYREXP_MAXGREEN
                                                           , PLYREXP_MINBLUE,   PLYREXP_MAXBLUE
                                                           , SHOTTRAIL_DESTHUE, PLYREXP_INC );
                                }
                            }
                        }
                        else {
                            m_nmeshots[ i ].state = STATE_ALIVE;
                            m_player.state = STATE_AUTOSHIELD;
                        }
                        return 1;
                    }
                }

                // collide with the enemy's ships
                for( i = 0; i < MAX_NMESHIPS; i++ ) {
                    if( game_objcollide( &m_player, &m_nmeships[ i ] ) ) {
                        if( !flag ) {
                            if( m_player.stateplayer.shield ) {
                                // bounce off
                                m_player.state = STATE_ALIVE;
                                m_nmeships[ i ].state = STATE_DEAD;
                                reflect_movingobj( &m_nmeships[ i ] );
                            }
                            else {
                                m_player.stateplayer.gun = GUN_NORMAL;
                                player_explode( &m_nmeships[ i ] );
                                debris_spawn( &m_player, 0 );
                                debris_spawn( &m_player, 3 );
                                if( m_options.playerexpl ) {
                                    shotrichochet_setactive( &m_plyrexp
                                                           , -1, m_player.x, m_player.y
                                                           , PLYREXP_MINRED,    PLYREXP_MAXRED
                                                           , PLYREXP_MINGREEN,  PLYREXP_MAXGREEN
                                                           , PLYREXP_MINBLUE,   PLYREXP_MAXBLUE
                                                           , SHOTTRAIL_DESTHUE, PLYREXP_INC );
                                }
                            }
                            if( ISBONUSROUND() ) {
                                if( --m_nmeshipcount <= 0 ) {
                                    points = ( m_nmeships[ i ].points * ( ( m_level + 1 ) * 50 ) );
                                    sys_playsound( m_sounds[ SND_APPLAUSE ], 0 );
                                }
                                else {
                                    points = ( m_nmeships[ i ].points * ( ( m_level + 1 ) * 10 ) );
                                }
                                m_player.stateplayer.score += points;
                                sprintf_s( m_risingscore.staterisingscore.string, 16, "%d", points );
                                risingscore_init( &m_risingscore, m_nmeshipcount <= 0, m_nmeships[ i ].x, m_nmeships[ i ].y );
                                nme_explode( &m_nmeships[ i ], &m_player );
                            }
                            else {
                                nme_explode( &m_nmeships[ i ], &m_player );
                                if( --m_nmeshipcount <= 0 ) {
                                    pickup_init( m_nmeships[ i ].x, m_nmeships[ i ].y, -1 );
                                }
                            }
                        }
                        else {
                            m_nmeships[ i ].state = STATE_ALIVE;
                            m_player.state = STATE_AUTOSHIELD;
                        }
                        return 1;
                    }
                }

                // collide with debris
                for( astcol = 0; astcol < MAX_DEBRIS; astcol++ ) {
                    for( i = 0; i < MAX_ACTIVEDEBRIS; i++ ) {
                        if( game_objcollide( &m_player, &m_debris[ astcol ][ i ] ) ) {
                            if( !flag ) {
                                if( m_player.stateplayer.shield ) {
                                    // bounce off
                                    m_player.state = STATE_ALIVE;
                                    m_debris[ astcol ][ i ].state = STATE_ALIVE;
                                    reflect_movingobj( &m_debris[ astcol ][ i ] );
                                }
                                else {
                                    // award points for destroying the debris
                                    m_player.stateplayer.score += m_debris[ astcol ][ i ].points;
                                    m_player.stateplayer.gun = GUN_NORMAL;
                                    player_explode( &m_debris[ astcol ][ i ] );
                                    debris_spawn( &m_player, 0 );
                                    debris_spawn( &m_debris[ astcol ][ i ], 0 );
                                    if( m_options.playerexpl ) {
                                        shotrichochet_setactive( &m_plyrexp
                                                               , -1, m_player.x, m_player.y
                                                               , PLYREXP_MINRED,    PLYREXP_MAXRED
                                                               , PLYREXP_MINGREEN,  PLYREXP_MAXGREEN
                                                               , PLYREXP_MINBLUE,   PLYREXP_MAXBLUE
                                                               , SHOTTRAIL_DESTHUE, PLYREXP_INC );
                                    }
                                }
                            }
                            else {
                                m_debris[ astcol ][ i ].state = STATE_ALIVE;
                                m_player.state = STATE_AUTOSHIELD;
                            }
                            return 1;
                        }
                        if( game_objcollide( &m_player, &m_nmedebris[ astcol ][ i ] ) ) {
                            if( !flag ) {
                                if( m_player.stateplayer.shield ) {
                                    // bounce off
                                    m_player.state = STATE_ALIVE;
                                    m_nmedebris[ astcol ][ i ].state = STATE_ALIVE;
                                    reflect_movingobj( &m_nmedebris[ astcol ][ i ] );
                                }
                                else {
                                    // award points for destroying the debris
                                    m_player.stateplayer.score += m_nmedebris[ astcol ][ i ].points;
                                    m_nmedebris[ astcol ][ i ].state = STATE_PREBIRTH;
                                    m_player.stateplayer.gun = GUN_NORMAL;
                                    player_explode( &m_nmedebris[ astcol ][ i ] );
                                    debris_spawn( &m_player, 0 );
                                    debris_spawn( &m_nmedebris[ astcol ][ i ], 3 );
                                    if( m_options.playerexpl ) {
                                        shotrichochet_setactive( &m_plyrexp
                                                               , -1, m_player.x, m_player.y
                                                               , PLYREXP_MINRED,    PLYREXP_MAXRED
                                                               , PLYREXP_MINGREEN,  PLYREXP_MAXGREEN
                                                               , PLYREXP_MINBLUE,   PLYREXP_MAXBLUE
                                                               , SHOTTRAIL_DESTHUE, PLYREXP_INC );
                                    }
                                }
                            }
                            else {
                                m_nmedebris[ astcol ][ i ].state = STATE_ALIVE;
                                m_player.state = STATE_AUTOSHIELD;
                            }
                            return 1;
                        }
                    }
                }

                // collide with debris
                for( piece = 0; piece < MAX_SMALLESTTYPES; piece++ ) {
                    for( i = 0; i < MAX_SMALLESTDEBRIS; i++ ) {
                        if( game_objcollide( &m_player, &m_smallestplyr[ piece ][ i ] ) ) {
                            if( !flag ) {
                                if( m_player.stateplayer.shield ) {
                                    // bounce off
                                    m_player.state = STATE_ALIVE;
                                    m_smallestplyr[ piece ][ i ].state = STATE_ALIVE;
                                    reflect_movingobj( &m_smallestplyr[ piece ][ i ] );
                                }
                                else {
                                    // award points for destroying the debris
                                    m_player.stateplayer.score += m_smallestplyr[ piece ][ i ].points;
                                    m_player.stateplayer.gun = GUN_NORMAL;
                                    player_explode( &m_smallestplyr[ piece ][ i ] );
                                    debris_spawn( &m_player, 0 );
                                    if( m_options.playerexpl ) {
                                        shotrichochet_setactive( &m_plyrexp
                                                               , -1, m_player.x, m_player.y
                                                               , PLYREXP_MINRED,    PLYREXP_MAXRED
                                                               , PLYREXP_MINGREEN,  PLYREXP_MAXGREEN
                                                               , PLYREXP_MINBLUE,   PLYREXP_MAXBLUE
                                                               , SHOTTRAIL_DESTHUE, PLYREXP_INC );
                                    }
                                }
                            }
                            else {
                                m_smallestplyr[ piece ][ i ].state = STATE_ALIVE;
                                m_player.state = STATE_AUTOSHIELD;
                            }
                            return 1;
                        }
                        else if( game_objcollide( &m_player, &m_smallestnme[ piece ][ i ] ) ) {
                            if( !flag ) {
                                if( m_player.stateplayer.shield ) {
                                    // bounce off
                                    m_player.state = STATE_ALIVE;
                                    m_smallestnme[ piece ][ i ].state = STATE_ALIVE;
                                    reflect_movingobj( &m_smallestnme[ piece ][ i ] );
                                }
                                else {
                                    // award points for destroying the debris
                                    m_player.stateplayer.score += m_smallestnme[ piece ][ i ].points;
                                    m_player.stateplayer.gun = GUN_NORMAL;
                                    player_explode( &m_smallestnme[ piece ][ i ] );
                                    debris_spawn( &m_player, 3 );
                                    if( m_options.playerexpl ) {
                                        shotrichochet_setactive( &m_plyrexp
                                                               , -1, m_player.x, m_player.y
                                                               , PLYREXP_MINRED,    PLYREXP_MAXRED
                                                               , PLYREXP_MINGREEN,  PLYREXP_MAXGREEN
                                                               , PLYREXP_MINBLUE,   PLYREXP_MAXBLUE
                                                               , SHOTTRAIL_DESTHUE, PLYREXP_INC );
                                    }
                                }
                            }
                            else {
                                m_smallestnme[ piece ][ i ].state = STATE_ALIVE;
                                m_player.state = STATE_AUTOSHIELD;
                            }
                            return 1;
                        }
                        else if( game_objcollide( &m_player, &m_smallestast0[ piece ][ i ] ) ) {
                            if( !flag ) {
                                if( m_player.stateplayer.shield ) {
                                    // bounce off
                                    m_player.state = STATE_ALIVE;
                                    m_smallestast0[ piece ][ i ].state = STATE_ALIVE;
                                    reflect_movingobj( &m_smallestast0[ piece ][ i ] );
                                }
                                else {
                                    // award points for destroying the debris
                                    m_player.stateplayer.score += m_smallestast0[ piece ][ i ].points;
                                    m_player.stateplayer.gun = GUN_NORMAL;
                                    player_explode( &m_smallestast0[ piece ][ i ] );
                                    debris_spawn( &m_player, 0 );
                                    if( m_options.playerexpl ) {
                                        shotrichochet_setactive( &m_plyrexp
                                                               , -1, m_player.x, m_player.y
                                                               , PLYREXP_MINRED,    PLYREXP_MAXRED
                                                               , PLYREXP_MINGREEN,  PLYREXP_MAXGREEN
                                                               , PLYREXP_MINBLUE,   PLYREXP_MAXBLUE
                                                               , SHOTTRAIL_DESTHUE, PLYREXP_INC );
                                    }
                                }
                            }
                            else {
                                m_smallestast0[ piece ][ i ].state = STATE_ALIVE;
                                m_player.state = STATE_AUTOSHIELD;
                            }
                            return 1;
                        }
                        else if( game_objcollide( &m_player, &m_smallestast1[ piece ][ i ] ) ) {
                            if( !flag ) {
                                if( m_player.stateplayer.shield ) {
                                    // bounce off
                                    m_player.state = STATE_ALIVE;
                                    m_smallestast1[ piece ][ i ].state = STATE_ALIVE;
                                    reflect_movingobj( &m_smallestast1[ piece ][ i ] );
                                }
                                else {
                                    // award points for destroying the debris
                                    m_player.stateplayer.score += m_smallestast1[ piece ][ i ].points;
                                    m_player.stateplayer.gun = GUN_NORMAL;
                                    player_explode( &m_smallestast1[ piece ][ i ] );
                                    debris_spawn( &m_player, 0 );
                                    if( m_options.playerexpl ) {
                                        shotrichochet_setactive( &m_plyrexp
                                                               , -1, m_player.x, m_player.y
                                                               , PLYREXP_MINRED,    PLYREXP_MAXRED
                                                               , PLYREXP_MINGREEN,  PLYREXP_MAXGREEN
                                                               , PLYREXP_MINBLUE,   PLYREXP_MAXBLUE
                                                               , SHOTTRAIL_DESTHUE, PLYREXP_INC );
                                    }
                                }
                            }
                            else {
                                m_smallestast1[ piece ][ i ].state = STATE_ALIVE;
                                m_player.state = STATE_AUTOSHIELD;
                            }
                            return 1;
                        }
                    }
                }

                // collide with asteroids
                for( astcol = 0; astcol < MAX_ASTCOLS; astcol++ ) {
                    for( piece = 0; piece < MAX_ASTPIECES; piece++ ) {
                        for( i = 0; i < MAX_ASTS; i++ ) {
                            dstobj = &m_astcols[ astcol ].astpieces[ piece ][ i ];
                            if( game_objcollide( &m_player, dstobj ) ) {
                                if( !flag ) {
                                    if( m_player.stateplayer.shield ) {
                                        // bounce off
                                        m_player.state = STATE_ALIVE;
                                        dstobj->state = STATE_ALIVE;
                                        reflect_movingobj( dstobj );
                                    }
                                    else {
                                        game_resolveshot( &m_player, dstobj );
                                        m_player.stateplayer.gun = GUN_NORMAL;
                                        player_explode( dstobj );
                                        debris_spawn( &m_player, 0 );
                                        debris_spawn( dstobj, astcol == ASTCOL_DIRT ? 1 : 2 );
                                        if( m_options.playerexpl ) {
                                            shotrichochet_setactive( &m_plyrexp
                                                                   , -1, m_player.x, m_player.y
                                                                   , PLYREXP_MINRED,    PLYREXP_MAXRED
                                                                   , PLYREXP_MINGREEN,  PLYREXP_MAXGREEN
                                                                   , PLYREXP_MINBLUE,   PLYREXP_MAXBLUE
                                                                   , SHOTTRAIL_DESTHUE, PLYREXP_INC );
                                        }
                                    }
                                }
                                else {
                                    dstobj->state = STATE_ALIVE;
                                    m_player.state = STATE_AUTOSHIELD;
                                }
                                return 1;
                            }
                        }
                    }
                }
            }

            break;

        default :
            break;
    }

    return 0;
}

static void player_draw( void )
{
    switch( m_player.state ) {
        case STATE_AUTOSHIELD :
            game_maptoscreen( &m_player );
            if( m_player.stateplayer.autostate ) {
                g_backbuffer->Blt( &m_player.dst
                                 , m_player.dds
                                 , &m_player.src
                                 , DDBLT_KEYSRC | DDBLT_WAIT
                                 , NULL );
            }
            break;

        case STATE_ALIVE :
            game_maptoscreen( &m_player );

            if( m_player.stateplayer.shield || STATE_AUTOSHIELD == m_player.state ) {
                m_player.src.left  += PLAYER_SHIELDOFFSET;
                m_player.src.right += PLAYER_SHIELDOFFSET;

                g_backbuffer->Blt( &m_player.dst
                                 , m_player.dds
                                 , &m_player.src
                                 , DDBLT_KEYSRC | DDBLT_WAIT
                                 , NULL );

                m_player.src.left  -= PLAYER_SHIELDOFFSET;
                m_player.src.right -= PLAYER_SHIELDOFFSET;
            }
            else {
                g_backbuffer->Blt( &m_player.dst
                                 , m_player.dds
                                 , &m_player.src
                                 , DDBLT_KEYSRC | DDBLT_WAIT
                                 , NULL );
            }
            break;

        default :
            break;
    }
}

static void player_lookupframe( obj_t *obj )
{
    int frame;

    frame = obj->direction / PLAYER_FRAMEINC;

    if( (obj->direction % PLAYER_FRAMEINC) >= (PLAYER_FRAMEINC / 2) ) {
        frame++;
    }

    game_setframe( obj, frame > PLAYER_MAXFRAMEIDX ? 0 : frame );
}

static void player_fireshot( void )
{
    int i, shot;
    int idx, dir;

    if( m_player.stateplayer.cooldown < PLAYER_GUNCOOLDOWN ||
        m_player.stateplayer.shield ||
        m_player.state == STATE_AUTOSHIELD ) {
        return;
    }

    switch( m_player.stateplayer.gun ) {
        case GUN_NORMAL :
            for( i = 0; i < MAX_SHOTS; i++ ) {
                if( STATE_ALIVE != m_shots[ i ].state ) {
                    break;
                }
            }

            if( i >= MAX_SHOTS ) {
                return;
            }

            m_shots[ i ].state  = STATE_ALIVE;
            m_shots[ i ].x      = m_player.x;
            m_shots[ i ].y      = m_player.y;
            m_shots[ i ].spd    = SHOT_SPEED;
            m_shots[ i ].spdx   = m_shots[ i ].spd * (float)cos( PI_OVER_180 * m_player.direction );
            m_shots[ i ].spdy   = m_shots[ i ].spd * (float)sin( PI_OVER_180 * m_player.direction );

            m_shots[ i ].direction = m_player.direction;

            m_player.stateplayer.cooldown = 0;

            if( m_options.playershottrails ) {
                shottrail_setactive( m_shottrails, &m_shots[ i ]
                                   , SHOTTRAIL_MINRED,   SHOTTRAIL_MAXRED
                                   , SHOTTRAIL_MINGREEN, SHOTTRAIL_MAXGREEN
                                   , SHOTTRAIL_MINBLUE,  SHOTTRAIL_MAXBLUE
                                   , SHOTTRAIL_DESTHUE,  SHOTTRAIL_INC );
            }
            sys_playsound( m_sounds[ SND_PLYRSHT ], 0 );

            m_statsshotsfired++;
            break;

        case GUN_MULTISHOT :
            for( i = 0; i < MAX_SHOTS; i++ ) {
                if( STATE_ALIVE != m_shots[ i ].state ) {
                    break;
                }
            }

            if( i >= MAX_SHOTS ) {
                return;
            }

            dir = m_player.direction - MULTISHOT_ANGLESPREAD;

            for( shot = 0; shot < MULTISHOT_SHOTCOUNT; shot++ ) {
                idx = i + ( shot * MULTISHOT_SHOTCOUNT );

                m_shots[ idx ].state  = STATE_ALIVE;
                m_shots[ idx ].x      = m_player.x;
                m_shots[ idx ].y      = m_player.y;
                m_shots[ idx ].spd    = SHOT_SPEED;
                m_shots[ idx ].spdx   = m_shots[ idx ].spd * (float)cos( PI_OVER_180 * dir );
                m_shots[ idx ].spdy   = m_shots[ idx ].spd * (float)sin( PI_OVER_180 * dir );

                m_shots[ idx ].direction = m_player.direction;

                m_player.stateplayer.cooldown = 0;

                if( m_options.playershottrails ) {
                    shottrail_setactive( m_shottrails, &m_shots[ idx ]
                                       , SHOTTRAIL_MINRED,   SHOTTRAIL_MAXRED
                                       , SHOTTRAIL_MINGREEN, SHOTTRAIL_MAXGREEN
                                       , SHOTTRAIL_MINBLUE,  SHOTTRAIL_MAXBLUE
                                       , SHOTTRAIL_DESTHUE,  SHOTTRAIL_INC );
                }
                m_statsshotsfired++;
                dir += MULTISHOT_ANGLESPREAD;
            }

            sys_playsound( m_sounds[ SND_PLYRSHT ], 0 );

            // unlimited ammo in a bonus round
            if( !ISBONUSROUND() ) {
                if( --m_player.stateplayer.ammo <= 0 ) {
                    m_player.stateplayer.ammo = 0;
                    m_player.stateplayer.gun  = GUN_NORMAL;
                }
            }
            break;

        case GUN_CHARGEUP :
            sys_stopsound( m_sounds[ SND_PLYRCHGUP ] );
            set_resetsoundpos( m_sounds[ SND_PLYRCHGUP ] );

            for( i = 0; i < MAX_CHARGEUPSHOTS; i++ ) {
                if( STATE_ALIVE != m_chargeupshots[ 0 ][ i ].state ) {
                    break;
                }
            }

            if( i >= MAX_CHARGEUPSHOTS ) {
                return;
            }

            idx = 2 - ( m_player.stateplayer.charge / 34 );
            if( idx > 2 ) {
                idx = 2;
            }

            m_chargeupshots[ idx ][ i ].state  = STATE_ALIVE;
            m_chargeupshots[ idx ][ i ].x      = m_player.x;
            m_chargeupshots[ idx ][ i ].y      = m_player.y;
            m_chargeupshots[ idx ][ i ].spd    = SHOT_SPEED;
            m_chargeupshots[ idx ][ i ].spdx   = m_chargeupshots[ idx ][ i ].spd * (float)cos( PI_OVER_180 * m_player.direction );
            m_chargeupshots[ idx ][ i ].spdy   = m_chargeupshots[ idx ][ i ].spd * (float)sin( PI_OVER_180 * m_player.direction );

            m_chargeupshots[ idx ][ i ].direction = m_player.direction;

            m_chargeupshots[ idx ][ i ].stateshot.charge = m_player.stateplayer.charge;
            m_chargeupshots[ idx ][ i ].stateshot.index  = i;

            m_player.stateplayer.cooldown = 0;

            m_chargeupspit.obj = NULL;

            if( m_options.playershottrails ) {
                shottrail_setactive( m_chargeupshottrails, &m_chargeupshots[ idx ][ i ]
                                   , CHARGEUPTRAIL_MINRED,   CHARGEUPTRAIL_MAXRED
                                   , CHARGEUPTRAIL_MINGREEN, CHARGEUPTRAIL_MAXGREEN
                                   , CHARGEUPTRAIL_MINBLUE,  CHARGEUPTRAIL_MAXBLUE
                                   , CHARGEUPTRAIL_DESTHUE,  CHARGEUPTRAIL_INC );
            }

            sys_playsound( m_sounds[ SND_PLYRSHT2 ], 0 );

            m_statsshotsfired++;
            break;
    }
}

static void player_reset( void )
{
    game_setframe( &m_player, 0 );

    m_player.state  = STATE_AUTOSHIELD;
    m_player.x      = SCREEN_WIDTH_f / 2;
    m_player.y      = SCREEN_HEIGHT_f / 2;
    m_player.spd    = 0.0f;
    m_player.spdx   = 0.0f;
    m_player.spdy   = 0.0f;

    m_player.stateplayer.autoshield     = 0;
    m_player.stateplayer.autostate      = 0;
    m_player.stateplayer.isguncharging  = 0;
    m_player.stateplayer.shieldhealth   = 100;
    m_player.stateplayer.autocounter    = ISBONUSROUND() ? PLAYER_AUTOCOUNTERMAX / 2 : 0;

    m_player.direction  = 270;
    m_player.stateplayer.cooldown   = PLAYER_GUNCOOLDOWN;
}

static void player_explode( obj_t *obj )
{
    int i, j;
    int direction;

    m_chargeupspit.obj = NULL;

    m_player.stateplayer.isguncharging = 0;
    m_player.stateplayer.charge        = 0;

    for( i = 0; i < MAX_ACTIVEDEBRIS; i++ ) {
        if( STATE_ALIVE != m_debris[ 0 ][ i ].state ) {
            break;
        }
    }

    if( i >= MAX_ACTIVEDEBRIS ) {
        // just use the first one
        i = 0;
    }

    if( fabs( obj->spd ) * obj->mass > fabs( m_player.spd ) * m_player.mass ) {
        direction = obj->direction;
    }
    else {
        if( m_player.spd < 0 ) {
            direction = m_player.direction - 180;
            if( direction < 0 ) {
                direction += 360;
            }
        }
        else {
            direction = m_player.direction;
        }
    }

    for( j = 0; j < MAX_DEBRIS; j++ ) {
        m_debris[ j ][ i ].state    = STATE_ALIVE;
        m_debris[ j ][ i ].x        = m_player.x;
        m_debris[ j ][ i ].y        = m_player.y;

        m_debris[ j ][ i ].direction = direction + game_rand( -20, 20 );

        //m_debris[ j ][ i ].spd = (float)( (int)rand() % (int)ASTEROID_SPEEDMAX ) + ASTEROID_SPEEDMIN;
        m_debris[ j ][ i ].spd = (float)( (float)ASTEROID_SPEED * ( ( ( (int)m_level + (int)1 ) * (int)10 ) + ( (int)m_round + (int)1 ) ) ) + ( (float)game_rand( 5, 20 ) / 100.0f );

        m_debris[ j ][ i ].spdx = m_debris[ j ][ i ].spd * (float)cos( PI_OVER_180 * m_debris[ j ][ i ].direction );
        m_debris[ j ][ i ].spdy = m_debris[ j ][ i ].spd * (float)sin( PI_OVER_180 * m_debris[ j ][ i ].direction );

        game_setframe( &m_debris[ j ][ i ], 0 );
    }

    sys_playsound( m_sounds[ SND_PLYREXPL ], 0 );
}

static void player_awardpickup( void )
{
    int astcol;
    int piece;
    int i;

    sys_playsound( m_sounds[ SND_CHEAT ], 0 );

    m_chargeupspit.obj = NULL;

    if( m_currentpickup == &m_pickups[ PICKUP_BOMBREDUCE ] )
    {
        // set each asteroid to blow up in the near future
        for( astcol = 0; astcol < MAX_ASTCOLS; astcol++ ) {
            for( piece = 0; piece < MAX_ASTPIECES; piece++ ) {
                for( i = 0; i < MAX_ASTS; i++ ) {
                    if( STATE_ALIVE    == m_astcols[ astcol ].astpieces[ piece ][ i ].state &&
                        ASTTYPE_PIECE2 >= m_astcols[ astcol ].astpieces[ piece ][ i ].stateasteroid.type ) {
                        m_astcols[ astcol ].astpieces[ piece ][ i ].stateasteroid.bombactive    = 1;
                        m_astcols[ astcol ].astpieces[ piece ][ i ].stateasteroid.bombcounter   = 0;
                        m_astcols[ astcol ].astpieces[ piece ][ i ].stateasteroid.bombthreshold = game_rand( BOMBREDUCE_MINTHRESHOLD, BOMBREDUCE_MAXTHRESHOLD );
                    }
                }
            }
        }
    }
    else if( m_currentpickup == &m_pickups[ PICKUP_BOMBSCATTER ] ) {
        bombscatter_init();
    }
    else if( m_currentpickup == &m_pickups[ PICKUP_GUNMULTISHOT ] ) {
        m_player.stateplayer.gun  = GUN_MULTISHOT;
        m_player.stateplayer.ammo = MULTISHOT_MAXAMMO;
    }
    else if( m_currentpickup == &m_pickups[ PICKUP_GUNCHARGEUP ] ) {
        m_player.stateplayer.gun  = GUN_CHARGEUP;
        m_player.stateplayer.ammo = 100;
    }
    else if( m_currentpickup == &m_pickups[ PICKUP_BONUSEXTRALIFE ] ) {
        if( ++m_player.stateplayer.livesleft > PLAYER_MAXLIVES ) {
            m_player.stateplayer.livesleft = PLAYER_MAXLIVES;
        }
    }
    else if( m_currentpickup == &m_pickups[ PICKUP_BONUSFULLSHIELD ] ) {
        m_player.stateplayer.shieldhealth = 100;
    }
}

static void shot_think( obj_t *obj )
{
    switch( obj->state ) {
        case STATE_ALIVE :
            game_incframe( obj );
            break;

        case STATE_DYING :
            obj->state = STATE_PREBIRTH;
            break;
    }
}

static void shot_move( obj_t *obj )
{
    if( STATE_ALIVE == obj->state ) {
        obj->x += obj->spdx;
        obj->y += obj->spdy;
    }
}

static void shot_collide( obj_t *obj )
{
    int i, astcol, piece;
    int points;
    obj_t *dstobj;

    if( STATE_ALIVE == obj->state ) {
        if( obj->x >= SCREEN_WIDTH_f  + obj->width  || obj->x <= -obj->width ||
            obj->y >= SCREEN_HEIGHT_f + obj->height || obj->y <= -obj->height ) {
            obj->state = STATE_PREBIRTH;
            m_statsshotsmissed++;
            return;
        }

        // collide with the current pickup
        if( m_currentpickup ) {

            // the game_objcollide() function only compares objects whose
            // state is STATE_ALIVE.  pickup objects use STATE_LINE and STATE_CIRCLE.

            m_currentpickup->savedstate = m_currentpickup->state;
            m_currentpickup->state = STATE_ALIVE;

            if( game_objcollide( obj, m_currentpickup ) ) {
                m_currentpickup->state = STATE_DEAD;
                m_statsshotshit++;
                pickup_change();
                sys_playsound( m_sounds[ SND_ASTHIT ], 0 );
                if( m_options.playershotexpl ) {
                    shotrichochet_setactive( m_shotrichochets
                                           , -1, obj->x, obj->y
                                           , SHOTTRAIL_MINRED,   SHOTTRAIL_MAXRED
                                           , SHOTTRAIL_MINGREEN, SHOTTRAIL_MAXGREEN
                                           , SHOTTRAIL_MINBLUE,  SHOTTRAIL_MAXBLUE
                                           , SHOTTRAIL_DESTHUE,  SHOTTRAIL_INC );
                }
                return;
            }

            m_currentpickup->state = m_currentpickup->savedstate;
        }

        // collide with the enemy's ships
        for( i = 0; i < MAX_NMESHIPS; i++ ) {
            if( game_objcollide( obj, &m_nmeships[ i ] ) ) {
                // award points for destroying the nme ship
                sys_playsound( m_sounds[ 0 == rand() % 2 ? SND_ASTEXPL0 : SND_ASTEXPL1 ], 0 );
                m_statsshotshit++;
                if( m_options.playershotexpl ) {
                    shotrichochet_setactive( m_shotrichochets
                                           , -1, obj->x, obj->y
                                           , SHOTTRAIL_MINRED,   SHOTTRAIL_MAXRED
                                           , SHOTTRAIL_MINGREEN, SHOTTRAIL_MAXGREEN
                                           , SHOTTRAIL_MINBLUE,  SHOTTRAIL_MAXBLUE
                                           , SHOTTRAIL_DESTHUE,  SHOTTRAIL_INC );
                }
                nme_explode( &m_nmeships[ i ], obj );
                if( ISBONUSROUND() ) {
                    if( --m_nmeshipcount <= 0 ) {
                        points = ( m_nmeships[ i ].points * ( ( m_level + 1 ) * 50 ) );
                        sys_playsound( m_sounds[ SND_APPLAUSE ], 0 );
                    }
                    else {
                        points = ( m_nmeships[ i ].points * ( ( m_level + 1 ) * 10 ) );
                    }
                    m_player.stateplayer.score += points;
                    sprintf_s( m_risingscore.staterisingscore.string, 16, "%d", points );
                    risingscore_init( &m_risingscore, m_nmeshipcount <= 0, obj->x, obj->y );
                }
                else {
                    m_player.stateplayer.score += m_nmeships[ i ].points;
                    if( --m_nmeshipcount <= 0 ) {
                        pickup_init( obj->x, obj->y, -1 );
                    }
                }
                return;
            }
        }

        // collide with player debris
        for( astcol = 0; astcol < MAX_DEBRIS; astcol++ ) {
            for( i = 0; i < MAX_ACTIVEDEBRIS; i++ ) {
                if( game_objcollide( obj, &m_debris[ astcol ][ i ] ) ) {
                    // award points for destroying the debris
                    m_player.stateplayer.score += m_debris[ astcol ][ i ].points;
                    debris_spawn( obj, 0 );
                    sys_playsound( m_sounds[ 0 == rand() % 2 ? SND_ASTEXPL0 : SND_ASTEXPL1 ], 0 );
                    m_statsshotshit++;
                    if( m_options.playershotexpl ) {
                        shotrichochet_setactive( m_shotrichochets
                                               , -1, obj->x, obj->y
                                               , SHOTTRAIL_MINRED,   SHOTTRAIL_MAXRED
                                               , SHOTTRAIL_MINGREEN, SHOTTRAIL_MAXGREEN
                                               , SHOTTRAIL_MINBLUE,  SHOTTRAIL_MAXBLUE
                                               , SHOTTRAIL_DESTHUE,  SHOTTRAIL_INC );
                    }
                    return;
                }
                if( game_objcollide( obj, &m_nmedebris[ astcol ][ i ] ) ) {
                    // award points for destroying the debris
                    m_player.stateplayer.score += m_nmedebris[ astcol ][ i ].points;
                    m_nmedebris[ astcol ][ i ].state = STATE_PREBIRTH;
                    debris_spawn( obj, 3 );
                    sys_playsound( m_sounds[ 0 == rand() % 2 ? SND_ASTEXPL0 : SND_ASTEXPL1 ], 0 );
                    m_statsshotshit++;
                    if( m_options.playershotexpl ) {
                        shotrichochet_setactive( m_shotrichochets
                                               , -1, obj->x, obj->y
                                               , SHOTTRAIL_MINRED,   SHOTTRAIL_MAXRED
                                               , SHOTTRAIL_MINGREEN, SHOTTRAIL_MAXGREEN
                                               , SHOTTRAIL_MINBLUE,  SHOTTRAIL_MAXBLUE
                                               , SHOTTRAIL_DESTHUE,  SHOTTRAIL_INC );
                    }
                    return;
                }
            }
        }

        // collide with the smallest debris
        for( piece = 0; piece < MAX_SMALLESTTYPES; piece++ ) {
            for( i = 0; i < MAX_SMALLESTDEBRIS; i++ ) {
                if( game_objcollide( obj, &m_smallestplyr[ piece ][ i ] ) ) {
                    // award points for destroying the debris
                    m_player.stateplayer.score += m_smallestplyr[ piece ][ i ].points;
                    //debris_think( &m_smallestplyr[ piece ][ i ] );
                    sys_playsound( m_sounds[ 0 == rand() % 2 ? SND_ASTEXPL0 : SND_ASTEXPL1 ], 0 );
                    m_statsshotshit++;
                    if( m_options.playershotexpl ) {
                        shotrichochet_setactive( m_shotrichochets
                                               , -1, obj->x, obj->y
                                               , SHOTTRAIL_MINRED,   SHOTTRAIL_MAXRED
                                               , SHOTTRAIL_MINGREEN, SHOTTRAIL_MAXGREEN
                                               , SHOTTRAIL_MINBLUE,  SHOTTRAIL_MAXBLUE
                                               , SHOTTRAIL_DESTHUE,  SHOTTRAIL_INC );
                    }
                    return;
                }
                else if( game_objcollide( obj, &m_smallestnme[ piece ][ i ] ) ) {
                    // award points for destroying the debris
                    m_player.stateplayer.score += m_smallestnme[ piece ][ i ].points;
                    //debris_think( &m_smallestnme[ piece ][ i ] );
                    sys_playsound( m_sounds[ 0 == rand() % 2 ? SND_ASTEXPL0 : SND_ASTEXPL1 ], 0 );
                    m_statsshotshit++;
                    if( m_options.playershotexpl ) {
                        shotrichochet_setactive( m_shotrichochets
                                               , -1, obj->x, obj->y
                                               , SHOTTRAIL_MINRED,   SHOTTRAIL_MAXRED
                                               , SHOTTRAIL_MINGREEN, SHOTTRAIL_MAXGREEN
                                               , SHOTTRAIL_MINBLUE,  SHOTTRAIL_MAXBLUE
                                               , SHOTTRAIL_DESTHUE,  SHOTTRAIL_INC );
                    }
                    return;
                }
                else if( game_objcollide( obj, &m_smallestast0[ piece ][ i ] ) ) {
                    // award points for destroying the debris
                    m_player.stateplayer.score += m_smallestast0[ piece ][ i ].points;
                    //debris_think( &m_smallestast0[ piece ][ i ] );
                    sys_playsound( m_sounds[ 0 == rand() % 2 ? SND_ASTEXPL0 : SND_ASTEXPL1 ], 0 );
                    m_statsshotshit++;
                    if( m_options.playershotexpl ) {
                        shotrichochet_setactive( m_shotrichochets
                                               , -1, obj->x, obj->y
                                               , SHOTTRAIL_MINRED,   SHOTTRAIL_MAXRED
                                               , SHOTTRAIL_MINGREEN, SHOTTRAIL_MAXGREEN
                                               , SHOTTRAIL_MINBLUE,  SHOTTRAIL_MAXBLUE
                                               , SHOTTRAIL_DESTHUE,  SHOTTRAIL_INC );
                    }
                    return;
                }
                else if( game_objcollide( obj, &m_smallestast1[ piece ][ i ] ) ) {
                    // award points for destroying the debris
                    m_player.stateplayer.score += m_smallestast1[ piece ][ i ].points;
                    //debris_think( &m_smallestast1[ piece ][ i ] );
                    sys_playsound( m_sounds[ 0 == rand() % 2 ? SND_ASTEXPL0 : SND_ASTEXPL1 ], 0 );
                    m_statsshotshit++;
                    if( m_options.playershotexpl ) {
                        shotrichochet_setactive( m_shotrichochets
                                               , -1, obj->x, obj->y
                                               , SHOTTRAIL_MINRED,   SHOTTRAIL_MAXRED
                                               , SHOTTRAIL_MINGREEN, SHOTTRAIL_MAXGREEN
                                               , SHOTTRAIL_MINBLUE,  SHOTTRAIL_MAXBLUE
                                               , SHOTTRAIL_DESTHUE,  SHOTTRAIL_INC );
                    }
                    return;
                }
            }
        }

        // collide with asteroids
        for( astcol = 0; astcol < MAX_ASTCOLS; astcol++ ) {
            for( piece = 0; piece < MAX_ASTPIECES; piece++ ) {
                for( i = 0; i < MAX_ASTS; i++ ) {
                    dstobj = &m_astcols[ astcol ].astpieces[ piece ][ i ];
                    if( game_objcollide( obj, dstobj ) ) {
                        m_statsshotshit++;
                        debris_spawn( obj, astcol == ASTCOL_DIRT ? 1 : 2 );
                        if( --dstobj->stateasteroid.health < 0 ) {
                            // then asteroid has no health left - kill it
                            dstobj->stateasteroid.health = 0;
                            game_resolveshot( obj, dstobj );
                            if( m_options.playershotexpl ) {
                                shotrichochet_setactive( m_shotrichochets
                                                       , -1, obj->x, obj->y
                                                       , SHOTTRAIL_MINRED,   SHOTTRAIL_MAXRED
                                                       , SHOTTRAIL_MINGREEN, SHOTTRAIL_MAXGREEN
                                                       , SHOTTRAIL_MINBLUE,  SHOTTRAIL_MAXBLUE
                                                       , SHOTTRAIL_DESTHUE,  SHOTTRAIL_INC );
                            }
                        }
                        else {
                            // the func game_objcollide() changed the obj's
                            // state, its health hasn't run out yet so put
                            // its state back to alive
                            dstobj->state = STATE_ALIVE;
                            sys_playsound( m_sounds[ SND_ASTHIT ], 0 );
                            if( m_options.playershotexpl ) {
                                shotrichochet_setactive( m_shotrichochets
                                                       , -1, obj->x, obj->y
                                                       , SHOTTRAIL_MINRED,   SHOTTRAIL_MAXRED
                                                       , SHOTTRAIL_MINGREEN, SHOTTRAIL_MAXGREEN
                                                       , SHOTTRAIL_MINBLUE,  SHOTTRAIL_MAXBLUE
                                                       , SHOTTRAIL_DESTHUE,  SHOTTRAIL_INC );
                            }

                            // award points for shooting the asteroid
                            m_player.stateplayer.score += ( m_level + 1 ) * POINTS_ASTCHIPBASE;
                        }
                        return;
                    }
                }
            }
        }
    }

}

static void chargeupshot_think( obj_t *obj )
{
    switch( obj->state ) {
        case STATE_ALIVE :
            game_incframe( obj );
            break;

        case STATE_DYING :
            obj->state = STATE_PREBIRTH;
            break;
    }
}

static void chargeupshot_collide( obj_t *obj )
{
    int i, astcol, piece;
    obj_t *dstobj;

    if( STATE_ALIVE == obj->state ) {
        if( obj->x >= SCREEN_WIDTH_f  + obj->width  || obj->x <= -obj->width ||
            obj->y >= SCREEN_HEIGHT_f + obj->height || obj->y <= -obj->height ) {
            obj->state = STATE_PREBIRTH;
            m_statsshotsmissed++;
            return;
        }

        // collide with the current pickup
        if( m_currentpickup ) {

            // the game_objcollide() function only compares objects whose
            // state is STATE_ALIVE.  pickup objects use STATE_LINE and STATE_CIRCLE.

            m_currentpickup->savedstate = m_currentpickup->state;
            m_currentpickup->state = STATE_ALIVE;

            if( game_objcollide( obj, m_currentpickup ) ) {
                m_currentpickup->state = STATE_DEAD;
                m_statsshotshit++;
                pickup_change();
                sys_playsound( m_sounds[ SND_ASTHIT ], 0 );
                if( m_options.playershotexpl ) {
                    shotrichochet_setactive( m_chargeupshotrichochets
                                           , -1, obj->x, obj->y
                                           , CHARGEUPTRAIL_MINRED,   CHARGEUPTRAIL_MAXRED
                                           , CHARGEUPTRAIL_MINGREEN, CHARGEUPTRAIL_MAXGREEN
                                           , CHARGEUPTRAIL_MINBLUE,  CHARGEUPTRAIL_MAXBLUE
                                           , CHARGEUPTRAIL_DESTHUE,  CHARGEUPTRAIL_INC );
                }
                return;
            }

            m_currentpickup->state = m_currentpickup->savedstate;
        }

        // collide with the enemy's ships
        for( i = 0; i < MAX_NMESHIPS; i++ ) {
            if( game_objcollide( obj, &m_nmeships[ i ] ) ) {

                chargeupshot_damage( obj, &m_nmeships[ i ] );

                // award points for destroying the nme ship
                m_player.stateplayer.score += m_nmeships[ i ].points;
                sys_playsound( m_sounds[ 0 == rand() % 2 ? SND_ASTEXPL0 : SND_ASTEXPL1 ], 0 );
                m_statsshotshit++;
                if( m_options.playershotexpl ) {
                    shotrichochet_setactive( m_chargeupshotrichochets
                                           , -1, obj->x, obj->y
                                           , CHARGEUPTRAIL_MINRED,   CHARGEUPTRAIL_MAXRED
                                           , CHARGEUPTRAIL_MINGREEN, CHARGEUPTRAIL_MAXGREEN
                                           , CHARGEUPTRAIL_MINBLUE,  CHARGEUPTRAIL_MAXBLUE
                                           , CHARGEUPTRAIL_DESTHUE,  CHARGEUPTRAIL_INC );
                }
                nme_explode( &m_nmeships[ i ], obj );
                if( --m_nmeshipcount <= 0 ) {
                    pickup_init( obj->x, obj->y, -1 );
                }
                return;
            }
        }

        // collide with player debris
        for( astcol = 0; astcol < MAX_DEBRIS; astcol++ ) {
            for( i = 0; i < MAX_ACTIVEDEBRIS; i++ ) {
                if( game_objcollide( obj, &m_debris[ astcol ][ i ] ) ) {
                    chargeupshot_damage( obj, &m_debris[ astcol ][ i ] );
                    // award points for destroying the debris
                    m_player.stateplayer.score += m_debris[ astcol ][ i ].points;
                    debris_spawn( obj, 0 );
                    sys_playsound( m_sounds[ 0 == rand() % 2 ? SND_ASTEXPL0 : SND_ASTEXPL1 ], 0 );
                    m_statsshotshit++;
                    if( m_options.playershotexpl ) {
                        shotrichochet_setactive( m_chargeupshotrichochets
                                               , -1, obj->x, obj->y
                                               , CHARGEUPTRAIL_MINRED,   CHARGEUPTRAIL_MAXRED
                                               , CHARGEUPTRAIL_MINGREEN, CHARGEUPTRAIL_MAXGREEN
                                               , CHARGEUPTRAIL_MINBLUE,  CHARGEUPTRAIL_MAXBLUE
                                               , CHARGEUPTRAIL_DESTHUE,  CHARGEUPTRAIL_INC );
                    }
                    return;
                }
                if( game_objcollide( obj, &m_nmedebris[ astcol ][ i ] ) ) {
                    chargeupshot_damage( obj, &m_nmedebris[ astcol ][ i ] );
                    m_nmedebris[ astcol ][ i ].state = STATE_PREBIRTH;
                    // award points for destroying the debris
                    m_player.stateplayer.score += m_nmedebris[ astcol ][ i ].points;
                    debris_spawn( obj, 3 );
                    sys_playsound( m_sounds[ 0 == rand() % 2 ? SND_ASTEXPL0 : SND_ASTEXPL1 ], 0 );
                    m_statsshotshit++;
                    if( m_options.playershotexpl ) {
                        shotrichochet_setactive( m_chargeupshotrichochets
                                               , -1, obj->x, obj->y
                                               , CHARGEUPTRAIL_MINRED,   CHARGEUPTRAIL_MAXRED
                                               , CHARGEUPTRAIL_MINGREEN, CHARGEUPTRAIL_MAXGREEN
                                               , CHARGEUPTRAIL_MINBLUE,  CHARGEUPTRAIL_MAXBLUE
                                               , CHARGEUPTRAIL_DESTHUE,  CHARGEUPTRAIL_INC );
                    }
                    return;
                }
            }
        }

        // collide with the smallest debris
        for( piece = 0; piece < MAX_SMALLESTTYPES; piece++ ) {
            for( i = 0; i < MAX_SMALLESTDEBRIS; i++ ) {
                if( game_objcollide( obj, &m_smallestplyr[ piece ][ i ] ) ) {
                    chargeupshot_damage( obj, &m_smallestplyr[ piece ][ i ] );
                    // award points for destroying the debris
                    m_player.stateplayer.score += m_smallestplyr[ piece ][ i ].points;
                    //debris_think( &m_smallestplyr[ piece ][ i ] );
                    sys_playsound( m_sounds[ 0 == rand() % 2 ? SND_ASTEXPL0 : SND_ASTEXPL1 ], 0 );
                    m_statsshotshit++;
                    if( m_options.playershotexpl ) {
                        shotrichochet_setactive( m_chargeupshotrichochets
                                               , -1, obj->x, obj->y
                                               , CHARGEUPTRAIL_MINRED,   CHARGEUPTRAIL_MAXRED
                                               , CHARGEUPTRAIL_MINGREEN, CHARGEUPTRAIL_MAXGREEN
                                               , CHARGEUPTRAIL_MINBLUE,  CHARGEUPTRAIL_MAXBLUE
                                               , CHARGEUPTRAIL_DESTHUE,  CHARGEUPTRAIL_INC );
                    }
                    return;
                }
                else if( game_objcollide( obj, &m_smallestnme[ piece ][ i ] ) ) {
                    chargeupshot_damage( obj, &m_smallestnme[ piece ][ i ] );
                    // award points for destroying the debris
                    m_player.stateplayer.score += m_smallestnme[ piece ][ i ].points;
                    //debris_think( &m_smallestnme[ piece ][ i ] );
                    sys_playsound( m_sounds[ 0 == rand() % 2 ? SND_ASTEXPL0 : SND_ASTEXPL1 ], 0 );
                    m_statsshotshit++;
                    if( m_options.playershotexpl ) {
                        shotrichochet_setactive( m_chargeupshotrichochets
                                               , -1, obj->x, obj->y
                                               , CHARGEUPTRAIL_MINRED,   CHARGEUPTRAIL_MAXRED
                                               , CHARGEUPTRAIL_MINGREEN, CHARGEUPTRAIL_MAXGREEN
                                               , CHARGEUPTRAIL_MINBLUE,  CHARGEUPTRAIL_MAXBLUE
                                               , CHARGEUPTRAIL_DESTHUE,  CHARGEUPTRAIL_INC );
                    }
                    return;
                }
                else if( game_objcollide( obj, &m_smallestast0[ piece ][ i ] ) ) {
                    chargeupshot_damage( obj, &m_smallestast0[ piece ][ i ] );
                    // award points for destroying the debris
                    m_player.stateplayer.score += m_smallestast0[ piece ][ i ].points;
                    //debris_think( &m_smallestast0[ piece ][ i ] );
                    sys_playsound( m_sounds[ 0 == rand() % 2 ? SND_ASTEXPL0 : SND_ASTEXPL1 ], 0 );
                    m_statsshotshit++;
                    if( m_options.playershotexpl ) {
                        shotrichochet_setactive( m_chargeupshotrichochets
                                               , -1, obj->x, obj->y
                                               , CHARGEUPTRAIL_MINRED,   CHARGEUPTRAIL_MAXRED
                                               , CHARGEUPTRAIL_MINGREEN, CHARGEUPTRAIL_MAXGREEN
                                               , CHARGEUPTRAIL_MINBLUE,  CHARGEUPTRAIL_MAXBLUE
                                               , CHARGEUPTRAIL_DESTHUE,  CHARGEUPTRAIL_INC );
                    }
                    return;
                }
                else if( game_objcollide( obj, &m_smallestast1[ piece ][ i ] ) ) {
                    chargeupshot_damage( obj, &m_smallestast1[ piece ][ i ] );
                    // award points for destroying the debris
                    m_player.stateplayer.score += m_smallestast1[ piece ][ i ].points;
                    //debris_think( &m_smallestast1[ piece ][ i ] );
                    sys_playsound( m_sounds[ 0 == rand() % 2 ? SND_ASTEXPL0 : SND_ASTEXPL1 ], 0 );
                    m_statsshotshit++;
                    if( m_options.playershotexpl ) {
                        shotrichochet_setactive( m_chargeupshotrichochets
                                               , -1, obj->x, obj->y
                                               , CHARGEUPTRAIL_MINRED,   CHARGEUPTRAIL_MAXRED
                                               , CHARGEUPTRAIL_MINGREEN, CHARGEUPTRAIL_MAXGREEN
                                               , CHARGEUPTRAIL_MINBLUE,  CHARGEUPTRAIL_MAXBLUE
                                               , CHARGEUPTRAIL_DESTHUE,  CHARGEUPTRAIL_INC );
                    }
                    return;
                }
            }
        }

        // collide with asteroids
        for( astcol = 0; astcol < MAX_ASTCOLS; astcol++ ) {
            for( piece = 0; piece < MAX_ASTPIECES; piece++ ) {
                for( i = 0; i < MAX_ASTS; i++ ) {
                    dstobj = &m_astcols[ astcol ].astpieces[ piece ][ i ];
                    if( game_objcollide( obj, dstobj ) ) {
                        m_statsshotshit++;
                        if( obj->stateshot.charge >= 33 ) {
                            chargeupshot_damage( obj, dstobj );
                            debris_spawn( obj, astcol == ASTCOL_DIRT ? 1 : 2 );
                            dstobj->stateasteroid.health = 0;
                            game_resolveshot( obj, dstobj );
                            if( m_options.playershotexpl ) {
                                shotrichochet_setactive( m_chargeupshotrichochets
                                                       , -1, obj->x, obj->y
                                                       , CHARGEUPTRAIL_MINRED,   CHARGEUPTRAIL_MAXRED
                                                       , CHARGEUPTRAIL_MINGREEN, CHARGEUPTRAIL_MAXGREEN
                                                       , CHARGEUPTRAIL_MINBLUE,  CHARGEUPTRAIL_MAXBLUE
                                                       , CHARGEUPTRAIL_DESTHUE,  CHARGEUPTRAIL_INC );
                            }
                        }
                        else {
                            chargeupshot_damage( obj, dstobj );
                            debris_spawn( obj, astcol == ASTCOL_DIRT ? 1 : 2 );
                            if( --dstobj->stateasteroid.health < 0 ) {
                                // then asteroid has no health left - kill it
                                dstobj->stateasteroid.health = 0;
                                game_resolveshot( obj, dstobj );
                                if( m_options.playershotexpl ) {
                                    shotrichochet_setactive( m_chargeupshotrichochets
                                                           , -1, obj->x, obj->y
                                                           , CHARGEUPTRAIL_MINRED,   CHARGEUPTRAIL_MAXRED
                                                           , CHARGEUPTRAIL_MINGREEN, CHARGEUPTRAIL_MAXGREEN
                                                           , CHARGEUPTRAIL_MINBLUE,  CHARGEUPTRAIL_MAXBLUE
                                                           , CHARGEUPTRAIL_DESTHUE,  CHARGEUPTRAIL_INC );
                                }
                            }
                            else {
                                // the func game_objcollide() changed the obj's
                                // state, its health hasn't run out yet so put
                                // its state back to alive
                                dstobj->state = STATE_ALIVE;
                                sys_playsound( m_sounds[ SND_ASTHIT ], 0 );
                                if( m_options.playershotexpl ) {
                                    shotrichochet_setactive( m_chargeupshotrichochets
                                                           , -1, obj->x, obj->y
                                                           , CHARGEUPTRAIL_MINRED,   CHARGEUPTRAIL_MAXRED
                                                           , CHARGEUPTRAIL_MINGREEN, CHARGEUPTRAIL_MAXGREEN
                                                           , CHARGEUPTRAIL_MINBLUE,  CHARGEUPTRAIL_MAXBLUE
                                                           , CHARGEUPTRAIL_DESTHUE,  CHARGEUPTRAIL_INC );
                                }

                                // award points for shooting the asteroid
                                m_player.stateplayer.score += ( m_level + 1 ) * POINTS_ASTCHIPBASE;
                            }
                        }
                        return;
                    }
                }
            }
        }
    }
}

static int chargeupshot_damage( obj_t *shot, obj_t *obj )
{
    int dmg, idx;

    // use the momentum to damage the charge up shot
    dmg = (int)( (float)obj->mass * (float)fabs( obj->spd ) );

    shot->stateshot.charge -= ( 2 * dmg );
    if( shot->stateshot.charge <= 0 ) {
        shot->state = STATE_PREBIRTH;
        // start the charge up explosion
        return 1; // charge up shot dead
    }

    idx = 2 - ( shot->stateshot.charge / 34 );
    if( idx > 2 ) {
        idx = 2;
    }

    shot->state = STATE_PREBIRTH;

    m_chargeupshots[ idx ][ shot->stateshot.index ].state   = STATE_ALIVE;
    m_chargeupshots[ idx ][ shot->stateshot.index ].x       = shot->x;
    m_chargeupshots[ idx ][ shot->stateshot.index ].y       = shot->y;
    m_chargeupshots[ idx ][ shot->stateshot.index ].spd     = shot->spd;
    m_chargeupshots[ idx ][ shot->stateshot.index ].spdx    = shot->spdx;
    m_chargeupshots[ idx ][ shot->stateshot.index ].spdy    = shot->spdy;

    m_chargeupshots[ idx ][ shot->stateshot.index ].stateshot.charge = shot->stateshot.charge;
    m_chargeupshots[ idx ][ shot->stateshot.index ].stateshot.index  = shot->stateshot.index;

    return 0; // charge up shot not dead
}

static void nmeshot_collide( obj_t *obj )
{
    int i, astcol, piece;

    if( STATE_ALIVE == obj->state ) {
        if( obj->x >= SCREEN_WIDTH_f  + obj->width  || obj->x <= -obj->width ||
            obj->y >= SCREEN_HEIGHT_f + obj->height || obj->y <= -obj->height ) {
            obj->state = STATE_PREBIRTH;
            return;
        }

        // collide with debris
        for( astcol = 0; astcol < MAX_DEBRIS; astcol++ ) {
            for( i = 0; i < MAX_ACTIVEDEBRIS; i++ ) {
                if( game_objcollide( obj, &m_debris[ astcol ][ i ] ) ) {
                    debris_spawn( obj, 0 );
                    sys_playsound( m_sounds[ 0 == rand() % 2 ? SND_ASTEXPL0 : SND_ASTEXPL1 ], 0 );
                    if( m_options.nmeshotexpl ) {
                        shotrichochet_setactive( m_nmeshotrichochets
                                               , -1, obj->x, obj->y
                                               , NMESHOTTRAIL_MINRED,   NMESHOTTRAIL_MAXRED
                                               , NMESHOTTRAIL_MINGREEN, NMESHOTTRAIL_MAXGREEN
                                               , NMESHOTTRAIL_MINBLUE,  NMESHOTTRAIL_MAXBLUE
                                               , NMESHOTTRAIL_DESTHUE,  NMESHOTTRAIL_INC );
                    }
                    return;
                }
                if( game_objcollide( obj, &m_nmedebris[ astcol ][ i ] ) ) {
                    m_nmedebris[ astcol ][ i ].state = STATE_PREBIRTH;
                    debris_spawn( obj, 3 );
                    sys_playsound( m_sounds[ 0 == rand() % 2 ? SND_ASTEXPL0 : SND_ASTEXPL1 ], 0 );
                    if( m_options.nmeshotexpl ) {
                        shotrichochet_setactive( m_nmeshotrichochets
                                               , -1, obj->x, obj->y
                                               , NMESHOTTRAIL_MINRED,   NMESHOTTRAIL_MAXRED
                                               , NMESHOTTRAIL_MINGREEN, NMESHOTTRAIL_MAXGREEN
                                               , NMESHOTTRAIL_MINBLUE,  NMESHOTTRAIL_MAXBLUE
                                               , NMESHOTTRAIL_DESTHUE,  NMESHOTTRAIL_INC );
                    }
                    return;
                }
            }
        }

        // collide with the smallest debris
        for( piece = 0; piece < MAX_SMALLESTTYPES; piece++ ) {
            for( i = 0; i < MAX_SMALLESTDEBRIS; i++ ) {
                if( game_objcollide( obj, &m_smallestplyr[ piece ][ i ] ) ) {
                    sys_playsound( m_sounds[ 0 == rand() % 2 ? SND_ASTEXPL0 : SND_ASTEXPL1 ], 0 );
                    if( m_options.nmeshotexpl ) {
                        shotrichochet_setactive( m_nmeshotrichochets
                                               , -1, obj->x, obj->y
                                               , NMESHOTTRAIL_MINRED,   NMESHOTTRAIL_MAXRED
                                               , NMESHOTTRAIL_MINGREEN, NMESHOTTRAIL_MAXGREEN
                                               , NMESHOTTRAIL_MINBLUE,  NMESHOTTRAIL_MAXBLUE
                                               , NMESHOTTRAIL_DESTHUE,  NMESHOTTRAIL_INC );
                    }
                    return;
                }
            }
        }
    }
}

static void shot_draw( obj_t *obj )
{
    if( STATE_ALIVE == obj->state ) {
        game_maptoscreen( obj );

        g_backbuffer->Blt( &obj->dst
                         , obj->dds
                         , &obj->src
                         , DDBLT_KEYSRC | DDBLT_WAIT
                         , NULL );
    }
}

static void asteroid_think( obj_t *obj )
{
    switch( obj->state ) {
        case STATE_ALIVE :
            if( ++obj->stateasteroid.framecounter >= obj->stateasteroid.framethreshold ) {
                obj->stateasteroid.framecounter = 0;
                game_incframe( obj );
            }
            if( obj->stateasteroid.bombactive ) {
                if( ++obj->stateasteroid.bombcounter > obj->stateasteroid.bombthreshold ) {
                    obj->stateasteroid.bombcounter = 0;
                    obj->state = STATE_PREBIRTH;
                    // blow up the asteroid
                    game_resolveshot( obj, obj );
                }
            }
            break;

        case STATE_DYING :
            obj->state = STATE_PREBIRTH;
            break;
    }
}

static void asteroid_move( obj_t *obj )
{
    if( STATE_ALIVE == obj->state ) {
        obj->x += obj->spdx;
        obj->y += obj->spdy;
    }
}

static void asteroid_collide( obj_t *obj, int subtract )
{
    if( STATE_ALIVE == obj->state ) {
        if( ISBONUSROUND() && m_playerready ) {
            if( obj->stateasteroid.hasentered ) {
                if( obj->x < -obj->width  || obj->x > SCREEN_WIDTH_f  + obj->width  ||
                    obj->y < -obj->height || obj->y > SCREEN_HEIGHT_f + obj->height ) {
                    obj->state = STATE_PREBIRTH;
                    if( subtract ) {
                        m_didanyleave = 1;
                        m_astcols[ obj->stateasteroid.collection ].activepieces--;

                        if( !m_astcols[ ASTCOL_ROCK ].activepieces && !m_astcols[ ASTCOL_DIRT ].activepieces ) {
                            m_player.state = STATE_DEAD;
                            // move to the next level
                            m_round = 0;
                            m_level++;
                            game_stats();
                        }
                    }
                }
            }
            else {
                if( obj->x >= -obj->width  && obj->x <= SCREEN_WIDTH_f  + obj->width  &&
                    obj->y >= -obj->height && obj->y <= SCREEN_HEIGHT_f + obj->height ) {
                    obj->stateasteroid.hasentered = 1;
                }
            }
        }
        else {
            if( obj->x <= -obj->width ) {
                obj->x = SCREEN_WIDTH_f + (float)obj->width;
            }
            else if( obj->x >= SCREEN_WIDTH_f + obj->width  ) {
                obj->x = -(float)obj->width;
            }

            if( obj->y <= -obj->height ) {
                obj->y = SCREEN_HEIGHT_f + (float)obj->height;
            }
            else if( obj->y >= SCREEN_HEIGHT_f + obj->height  ) {
                obj->y = -(float)obj->height;
            }
        }
    }
}

static void asteroid_draw( obj_t *obj )
{
    if( STATE_ALIVE == obj->state ) {
        game_maptoscreen( obj );

        g_backbuffer->Blt( &obj->dst
                         , obj->dds
                         , &obj->src
                         , DDBLT_KEYSRC | DDBLT_WAIT
                         , NULL );
    }
}

static int game_new( void )
{
    game_pause();

    // if the previous round wasn't a bonus round
    if( 0 != m_round ) {
        m_player.stateplayer.gunpushed      = m_player.stateplayer.gun;
        m_player.stateplayer.ammopushed     = m_player.stateplayer.ammo;
        m_player.stateplayer.chargepushed   = m_player.stateplayer.charge;
    }

    m_menuactive = 0;
    m_gameactive = 1;
    m_hudstate   = STATE_DEAD;

    m_currentpickup = NULL;

    // make the starfield move in another direction
    starfield_init( rand() % 360 );

    m_leveltitles[ m_level ].state                      = STATE_ALIVE;
    m_leveltitles[ m_level ].stateasteroid.framecounter = 0;

    m_roundtitles[ m_round ].state                      = STATE_ALIVE;
    m_roundtitles[ m_round ].stateasteroid.framecounter = 0;

    m_player.stateplayer.shieldhealth = 100;

    sys_stopmp3( m_tracks[ m_leveltrackidx ] );
    sys_stopmp3( m_menutrack );

    if( ++m_leveltrackidx >= m_trackcount ) {
        m_leveltrackidx = 0;
    }

    sys_playmp3( m_tracks[ m_leveltrackidx ] );

    return 1;
}

static void game_stats( void )
{
    // award points based on how quickly the user cleared
    // all asteroids in the level
    m_timeelapsed += ( time( NULL ) - m_timemark );

    m_timebonus = 0;
    if( m_timeelapsed < WORST_LEVELTIME ) {
        m_timebonus = ( ( WORST_LEVELTIME - (int)m_timeelapsed ) * ( m_level + 1 ) );
        m_player.stateplayer.score += m_timebonus;
    }
    // else no bonus, the player's time is
    // too shitty

    // award points based on how accurate the player's shots were
    if( 0 == m_statsshotsfired ) {
        m_accuracy = 0;
    }
    else {
        m_accuracy = ( m_statsshotshit * 100 ) / m_statsshotsfired;
    }

    m_accuracybonus = m_accuracy * ACCURACY_MULTIPLIER;
    m_player.stateplayer.score += m_accuracybonus;

    m_menuactive = 0;
    m_gameactive = 1;

    m_player.state = STATE_DEAD;
    m_hudstate     = STATE_DEAD;
    m_stats.state  = STATE_ALIVE;

    m_stats.stateasteroid.framecounter = 0;
}

static int game_start( void )
{
    int i, astcol, piece;
    int sml, med, lge;
    int health, idx, loopstate;
    int **aststart;

    m_nmestatethink = STATE_NMEWAITING;

    m_menuactive  = 0;
    m_popupactive = 0;
    m_hudstate    = STATE_ALIVE;
    m_timeelapsed = 0;

    // turn off the finish screen (incase it's active)
    m_finishscreen.state = STATE_DEAD;

    // setup the player
    player_reset();

    // setup the player's shots
    for( i = 0; i < MAX_SHOTS * 3; i++ ) {              // x3 coz of multishot
        m_shots[ i ].state = STATE_PREBIRTH;
    }

    for( i = 0; i < BOMBSCATTER_MAXSHOTS; i++ ) {
        m_scattershots[ i ].state = STATE_PREBIRTH;
    }

    for( astcol = 0; astcol < MAX_CHARGEUPSIZES; astcol++ ) {
        for( i = 0; i < MAX_CHARGEUPSHOTS; i++ ) {
            m_chargeupshots[ astcol ][ i ].state = STATE_PREBIRTH;
        }
    }

    // setup the nme ships
    for( i = 0; i < MAX_NMESHIPS; i++ ) {
        m_nmeships[ i ].state  = STATE_PREBIRTH;
        m_nmeships[ i ].points = ( m_level + 1 ) * POINTS_NMESHIPBASE;
    }

    // setup the debris
    for( astcol = 0; astcol < MAX_DEBRIS; astcol++ ) {
        for( i = 0; i < MAX_ACTIVEDEBRIS; i++ ) {
            // player debris
            m_debris[ astcol ][ i ].state  = STATE_PREBIRTH;
            m_debris[ astcol ][ i ].points = ( m_level + 1 ) * POINTS_DEBRISBASE;

            m_debris[ astcol ][ i ].stateasteroid.framecounter   = 0;
            m_debris[ astcol ][ i ].stateasteroid.framethreshold = ( rand() % MAX_ASTFRAMEROT ) + MIN_ASTFRAMEROT;

            // nme ship debris
            m_nmedebris[ astcol ][ i ].state  = STATE_PREBIRTH;
            m_nmedebris[ astcol ][ i ].points = ( m_level + 1 ) * POINTS_DEBRISBASE;

            m_nmedebris[ astcol ][ i ].stateasteroid.framecounter   = 0;
            m_nmedebris[ astcol ][ i ].stateasteroid.framethreshold = ( rand() % MAX_ASTFRAMEROT ) + MIN_ASTFRAMEROT;
        }
    }

    for( piece = 0; piece < MAX_SMALLESTTYPES; piece++ ) {
        for( i = 0; i < MAX_SMALLESTDEBRIS; i++ ) {
            m_smallestplyr[ piece ][ i ].points = ( m_level + 1 ) * POINTS_DEBRISBASE;
            m_smallestnme[ piece ][ i ].points  = ( m_level + 1 ) * POINTS_DEBRISBASE;
            m_smallestast0[ piece ][ i ].points = ( m_level + 1 ) * POINTS_DEBRISBASE;
            m_smallestast1[ piece ][ i ].points = ( m_level + 1 ) * POINTS_DEBRISBASE;
        }
    }

    nme_mgrinit();

    if( ISBONUSROUND() ) {
        m_didanyleave = 0;
        func_bonuslevels[ m_level ]();
    }
    else {
        m_player.stateplayer.gun    = m_player.stateplayer.gunpushed;
        m_player.stateplayer.ammo   = m_player.stateplayer.ammopushed;
        m_player.stateplayer.charge = m_player.stateplayer.chargepushed;

        // setup the asteroid collections
        m_astcols[ ASTCOL_DIRT ].activepieces = 0;
        m_astcols[ ASTCOL_ROCK ].activepieces = 0;

        for( astcol = 0; astcol < MAX_ASTCOLS; astcol++ ) {
            for( piece = 0; piece < MAX_ASTPIECES; piece++ ) {
                for( i = 0; i < MAX_ASTS; i++ ) {
                    m_astcols[ astcol ].astpieces[ piece ][ i ].stateasteroid.framecounter   = 0;
                    m_astcols[ astcol ].astpieces[ piece ][ i ].stateasteroid.framethreshold = ( rand() % MAX_ASTFRAMEROT ) + MIN_ASTFRAMEROT;

                    m_astcols[ astcol ].astpieces[ piece ][ i ].stateasteroid.bombactive     = 0;

                    // make the asteroids take more shots to kill as the levels increase
                    //health = ( m_level == MAX_LEVELS - 1 ? ( MAX_LEVELS / 2 ) : ( m_level / 3 ) );

                    health = m_level;
                    if( health >= 5 ) {
                        health -= 5;
                    }

                    if( ASTCOL_ROCK == astcol ) {
                        health = ( health + 1 ) * 2;
                    }

                    m_astcols[ astcol ].astpieces[ piece ][ i ].stateasteroid.health = health;

                    m_astcols[ astcol ].astpieces[ piece ][ i ].state  = STATE_PREBIRTH;
                    m_astcols[ astcol ].astpieces[ piece ][ i ].points = ( m_level + 1 ) * POINTS_ASTBASE;
                }
            }
        }

        sml     = med = lge = 0;
        astcol  = ASTCOL_DIRT;
        idx     = m_level;

        loopstate = 0;

        if( idx >= ( MAX_LEVELS / 2 ) ) {
            idx -= ( MAX_LEVELS / 2 );
            loopstate = 1;
        }

        for( ; ; ) {
            for( aststart = m_aststarts; (*aststart); aststart++ ) {
                switch( (*aststart)[ idx + m_round ] ) {
                    case 0 :        // nothing
                        continue;

                    case 1 :        // sml
                        switch( rand() % 4 ) {
                            case 0 : piece = ASTTYPE_PIECE3; break;
                            case 1 : piece = ASTTYPE_PIECE4; break;
                            case 2 : piece = ASTTYPE_PIECE5; break;
                            case 3 : piece = ASTTYPE_PIECE6; break;
                        }
                        i = med;
                        med++;
                        break;

                    case 2 :        // med
                        piece = ( 0 == rand() % 2 ? ASTTYPE_PIECE1 : ASTTYPE_PIECE2 );
                        i = sml;
                        sml++;
                        break;

                    case 3 :        // lge
                        piece = ASTTYPE_PIECE0;
                        i = lge;
                        lge++;
                        break;

                }

                m_astcols[ astcol ].astpieces[ piece ][ i ].state   = STATE_ALIVE;
                m_astcols[ astcol ].astpieces[ piece ][ i ].x       = (float)( rand() % SCREEN_WIDTH );
                m_astcols[ astcol ].astpieces[ piece ][ i ].y       = (float)( rand() % SCREEN_HEIGHT );

                m_astcols[ astcol ].astpieces[ piece ][ i ].spd         = ASTEROID_SPEED * ( ( ( m_level + 1 ) * 10 ) + ( m_round + 1 ) );
                m_astcols[ astcol ].astpieces[ piece ][ i ].direction   = rand() % 360;

                m_astcols[ astcol ].astpieces[ piece ][ i ].spdx    = m_astcols[ astcol ].astpieces[ piece ][ i ].spd *
                                                                      (float)cos( PI_OVER_180 * m_astcols[ astcol ].astpieces[ piece ][ i ].direction );

                m_astcols[ astcol ].astpieces[ piece ][ i ].spdy    = m_astcols[ astcol ].astpieces[ piece ][ i ].spd *
                                                                      (float)sin( PI_OVER_180 * m_astcols[ astcol ].astpieces[ piece ][ i ].direction );
                m_astcols[ astcol ].activepieces++;
            }

            if( !loopstate ) {
                break;
            }
            else {
                if( 1 == loopstate ) {
                    loopstate++;
                    sml   = med = lge = 0;
                    astcol  = ASTCOL_ROCK;
                }
                else {
                    break;
                }
            }
        }

        nme_mgrreset();
    }

    for( i = 0; i < MAX_PICKUPS; i++ ) {
        m_pickups[ i ].state = STATE_DEAD;
    }
    m_currentpickup = NULL;

    m_timemark = time( NULL );

    m_statsshotsfired   = 0;
    m_statsshotshit     = 0;
    m_statsshotsmissed  = 0;

    return 1;
}

static int game_pause( void )
{
    int i, astcol, piece;

    m_timeelapsed += ( time( NULL ) - m_timemark );

    m_nmepushstatethink = m_nmestatethink;
    m_nmestatethink     = STATE_PAUSED;

    m_hudsavedstate = m_hudstate;
    m_menuactive    = 1;

    m_stats.savedstate = m_stats.state;
    m_stats.state = STATE_PAUSED;

    m_leveltitles[ m_level ].savedstate = m_leveltitles[ m_level ].state;
    m_leveltitles[ m_level ].state      = STATE_PAUSED;
    m_roundtitles[ m_round ].savedstate = m_roundtitles[ m_round ].state;
    m_roundtitles[ m_round ].state      = STATE_PAUSED;

    m_finishscreen.savedstate = m_finishscreen.state;
    m_finishscreen.state      = STATE_PAUSED;
    sys_stopmp3( m_creditstrack );

    m_gameover.savedstate = m_gameover.state;
    m_gameover.state      = STATE_PAUSED;

    m_player.savedstate  = m_player.state;
    m_player.state       = STATE_PAUSED;

    for( piece = 0; piece < MAX_SMALLESTTYPES; piece++ ) {
        for( i = 0; i < MAX_SMALLESTDEBRIS; i++ ) {
            m_smallestplyr[ piece ][ i ].savedstate = m_gameover.state;
            m_smallestplyr[ piece ][ i ].state      = STATE_PAUSED;

            m_smallestnme[ piece ][ i ].savedstate = m_gameover.state;
            m_smallestnme[ piece ][ i ].state      = STATE_PAUSED;

            m_smallestast0[ piece ][ i ].savedstate = m_gameover.state;
            m_smallestast0[ piece ][ i ].state      = STATE_PAUSED;

            m_smallestast1[ piece ][ i ].savedstate = m_gameover.state;
            m_smallestast1[ piece ][ i ].state      = STATE_PAUSED;
        }
    }

    for( i = 0; i < MAX_PICKUPS; i++ ) {
        m_pickups[ i ].savedstate = m_pickups[ i ].state;
        m_pickups[ i ].state      = STATE_PAUSED;
    }

    for( i = 0; i < MAX_SHOTS * 3; i++ ) {                  // x3 coz of multishot
        m_shots[ i ].savedstate = m_shots[ i ].state;
        m_shots[ i ].state      = STATE_PAUSED;
    }

    for( i = 0; i < BOMBSCATTER_MAXSHOTS; i++ ) {
        m_scattershots[ i ].savedstate = m_scattershots[ i ].state;
        m_scattershots[ i ].state = STATE_PAUSED;
    }

    for( astcol = 0; astcol < MAX_CHARGEUPSIZES; astcol++ ) {
        for( i = 0; i < MAX_CHARGEUPSHOTS; i++ ) {
            m_chargeupshots[ astcol ][ i ].savedstate = m_chargeupshots[ astcol ][ i ].state;
            m_chargeupshots[ astcol ][ i ].state = STATE_PAUSED;
        }
    }

    for( i = 0; i < MAX_NMESHOTS; i++ ) {
        m_nmeshots[ i ].savedstate = m_nmeshots[ i ].state;
        m_nmeshots[ i ].state      = STATE_PAUSED;
    }

    for( astcol = 0; astcol < MAX_DEBRIS; astcol++ ) {
        for( i = 0; i < MAX_ACTIVEDEBRIS; i++ ) {
            m_debris[ astcol ][ i ].savedstate = m_debris[ astcol ][ i ].state;
            m_debris[ astcol ][ i ].state      = STATE_PAUSED;

            m_nmedebris[ astcol ][ i ].savedstate = m_nmedebris[ astcol ][ i ].state;
            m_nmedebris[ astcol ][ i ].state      = STATE_PAUSED;
        }
    }

    for( astcol = 0; astcol < MAX_ASTCOLS; astcol++ ) {
        for( piece = 0; piece < MAX_ASTPIECES; piece++ ) {
            for( i = 0; i < MAX_ASTS; i++ ) {
                m_astcols[ astcol ].astpieces[ piece ][ i ].savedstate  = m_astcols[ astcol ].astpieces[ piece ][ i ].state;
                m_astcols[ astcol ].astpieces[ piece ][ i ].state       = STATE_PAUSED;
            }
        }
    }

    for( i = 0; i < MAX_SHOTTRAILS; i++ ) {
        m_shottrails[ i ].savedstate = m_shottrails[ i ].state;
        m_shottrails[ i ].state      = STATE_PAUSED;
    }

    for( i = 0; i < MAX_NMESHOTTRAILS; i++ ) {
        m_nmeshottrails[ i ].savedstate = m_nmeshottrails[ i ].state;
        m_nmeshottrails[ i ].state      = STATE_PAUSED;

        m_chargeupshottrails[ i ].savedstate = m_chargeupshottrails[ i ].state;
        m_chargeupshottrails[ i ].state      = STATE_PAUSED;
    }

    for( i = 0; i < MAX_SHOTRICHOCHETS; i++ ) {
        m_shotrichochets[ i ].savedstate = m_shotrichochets[ i ].state;
        m_shotrichochets[ i ].state      = STATE_PAUSED;

        m_chargeupshotrichochets[ i ].savedstate = m_chargeupshotrichochets[ i ].state;
        m_chargeupshotrichochets[ i ].state      = STATE_PAUSED;
    }

    for( i = 0; i < MAX_NMESHOTRICHOCHETS; i++ ) {
        m_nmeshotrichochets[ i ].savedstate = m_nmeshotrichochets[ i ].state;
        m_nmeshotrichochets[ i ].state      = STATE_PAUSED;
    }

    m_chargeupspit.savedstate = m_chargeupspit.state;
    m_chargeupspit.state = STATE_PAUSED;

    m_plyrexp.savedstate = m_plyrexp.state;
    m_plyrexp.state = STATE_PAUSED;

    m_plyrtrail1.savedstate = m_plyrtrail1.state;
    m_plyrtrail1.state = STATE_PAUSED;
    m_plyrtrail2.savedstate = m_plyrtrail2.state;
    m_plyrtrail2.state = STATE_PAUSED;

    for( i = 0; i < MAX_SHOTRICHOCHETS; i++ ) {
        m_plyrcollide[ i ].savedstate = m_plyrcollide[ i ].state;
        m_plyrcollide[ i ].state      = STATE_PAUSED;
    }

    for( i = 0; i < MAX_NMESHIPS; i++ ) {
        m_nmeships[ i ].savedstate = m_nmeships[ i ].state;
        m_nmeships[ i ].state      = STATE_PAUSED;
    }

    sys_pausemp3( m_tracks[ m_leveltrackidx ] );

    return 1;
}

static int game_resume( void )
{
    int i, astcol, piece;

    m_nmestatethink = m_nmepushstatethink;

    m_menuactive = 0;
    m_hudstate   = m_hudsavedstate;

    m_leveltitles[ m_level ].state  = m_leveltitles[ m_level ].savedstate;
    m_roundtitles[ m_round ].state  = m_roundtitles[ m_round ].savedstate;
    m_stats.state                   = m_stats.savedstate;
    m_gameover.state                = m_gameover.savedstate;
    m_finishscreen.state            = m_finishscreen.savedstate;
    m_player.state                  = m_player.savedstate;

    for( i = 0; i < MAX_PICKUPS; i++ ) {
        m_pickups[ i ].state = m_pickups[ i ].savedstate;
    }

    for( i = 0; i < MAX_SHOTS * 3; i++ ) {              // x3 coz of multishot
        m_shots[ i ].state = m_shots[ i ].savedstate;
    }

    for( i = 0; i < BOMBSCATTER_MAXSHOTS; i++ ) {
        m_scattershots[ i ].state = m_scattershots[ i ].savedstate;
    }

    for( astcol = 0; astcol < MAX_CHARGEUPSIZES; astcol++ ) {
        for( i = 0; i < MAX_CHARGEUPSHOTS; i++ ) {
            m_chargeupshots[ astcol ][ i ].state = m_chargeupshots[ astcol ][ i ].savedstate;
        }
    }

    for( i = 0; i < MAX_NMESHOTS; i++ ) {
        m_nmeshots[ i ].state = m_nmeshots[ i ].savedstate;
    }

    for( astcol = 0; astcol < MAX_DEBRIS; astcol++ ) {
        for( i = 0; i < MAX_ACTIVEDEBRIS; i++ ) {
            m_debris[ astcol ][ i ].state = m_debris[ astcol ][ i ].savedstate;
            m_nmedebris[ astcol ][ i ].state = m_nmedebris[ astcol ][ i ].savedstate;
        }
    }

    for( piece = 0; piece < MAX_SMALLESTTYPES; piece++ ) {
        for( i = 0; i < MAX_SMALLESTDEBRIS; i++ ) {
            m_smallestplyr[ piece ][ i ].state = m_smallestplyr[ piece ][ i ].savedstate;
            m_smallestnme[ piece ][ i ].state = m_smallestnme[ piece ][ i ].savedstate;
            m_smallestast0[ piece ][ i ].state = m_smallestast0[ piece ][ i ].savedstate;
            m_smallestast1[ piece ][ i ].state = m_smallestast1[ piece ][ i ].savedstate;
        }
    }

    for( astcol = 0; astcol < MAX_ASTCOLS; astcol++ ) {
        for( piece = 0; piece < MAX_ASTPIECES; piece++ ) {
            for( i = 0; i < MAX_ASTS; i++ ) {
                m_astcols[ astcol ].astpieces[ piece ][ i ].state = m_astcols[ astcol ].astpieces[ piece ][ i ].savedstate;
            }
        }
    }

    for( i = 0; i < MAX_NMESHIPS; i++ ) {
        m_nmeships[ i ].state = m_nmeships[ i ].savedstate;
    }

    for( i = 0; i < MAX_SHOTTRAILS; i++ ) {
        m_shottrails[ i ].state = m_shottrails[ i ].savedstate;
    }

    for( i = 0; i < MAX_NMESHOTTRAILS; i++ ) {
        m_nmeshottrails[ i ].state = m_nmeshottrails[ i ].savedstate;
        m_chargeupshottrails[ i ].state = m_chargeupshottrails[ i ].savedstate;
    }

    for( i = 0; i < MAX_SHOTRICHOCHETS; i++ ) {
        m_shotrichochets[ i ].state = m_shotrichochets[ i ].savedstate;
        m_chargeupshotrichochets[ i ].state = m_chargeupshotrichochets[ i ].savedstate;
    }

    for( i = 0; i < MAX_NMESHOTRICHOCHETS; i++ ) {
        m_nmeshotrichochets[ i ].state = m_nmeshotrichochets[ i ].savedstate;
    }

    m_chargeupspit.state = m_chargeupspit.savedstate;
    m_plyrexp.state      = m_plyrexp.savedstate;
    m_plyrtrail1.state    = m_plyrtrail1.savedstate;
    m_plyrtrail2.state    = m_plyrtrail2.savedstate;

    for( i = 0; i < MAX_SHOTRICHOCHETS; i++ ) {
        m_plyrcollide[ i ].state = m_plyrcollide[ i ].savedstate;
    }

    sys_pausemp3( m_menutrack );
    sys_playmp3( m_tracks[ m_leveltrackidx ] );

    m_timemark = time( NULL );
    return 1;
}

static int game_stop( void )
{
    int i, astcol, piece;

    m_gameactive = 0;

    m_player.state = STATE_PREBIRTH;

    for( i = 0; i < MAX_SHOTS * 3; i++ ) {              // x3 coz of multishot
        m_shots[ i ].state = STATE_PREBIRTH;
    }

    for( i = 0; i < BOMBSCATTER_MAXSHOTS; i++ ) {
        m_scattershots[ i ].state = STATE_PREBIRTH;
    }

    for( astcol = 0; astcol < MAX_CHARGEUPSIZES; astcol++ ) {
        for( i = 0; i < MAX_CHARGEUPSHOTS; i++ ) {
            m_chargeupshots[ astcol ][ i ].state = STATE_PREBIRTH;
        }
    }

    for( astcol = 0; astcol < MAX_DEBRIS; astcol++ ) {
        for( i = 0; i < MAX_ACTIVEDEBRIS; i++ ) {
            m_debris[ astcol ][ i ].state = STATE_PREBIRTH;
            m_nmedebris[ astcol ][ i ].state = STATE_PREBIRTH;
        }
    }

    for( astcol = 0; astcol < MAX_ASTCOLS; astcol++ ) {
        for( piece = 0; piece < MAX_ASTPIECES; piece++ ) {
            for( i = 0; i < MAX_ASTS; i++ ) {
                m_astcols[ astcol ].astpieces[ piece ][ i ].state = STATE_PREBIRTH;
            }
        }
    }

    for( i = 0; i < MAX_SHOTTRAILS; i++ ) {
        m_shottrails[ i ].state = STATE_PREBIRTH;
    }

    for( i = 0; i < MAX_SHOTRICHOCHETS; i++ ) {
        m_shotrichochets[ i ].state = STATE_PREBIRTH;
    }

    entername_init();

    return 1;
}

static int help_view( void )
{
    int i;

    // disable all menu stuff
    m_title.state       = STATE_DEAD;
    m_menubar.state     = STATE_DEAD;
    m_menugame.state    = STATE_DEAD;
    m_menuscores.state  = STATE_DEAD;
    m_menuhelp.state    = STATE_DEAD;

    for( i = 0; m_buttons[ i ]; i++ ) {
        m_buttons[ i ]->state = STATE_DEAD;
    }

    m_butnext.state     = STATE_ALIVE;
    m_butprev.state     = STATE_PAUSED;
    m_butprev.src.left  = 128;
    m_butprev.src.right = 192;
    m_butback.state     = STATE_ALIVE;
    m_menuhelp2.state   = STATE_ALIVE;

    m_helppages[ 0 ].state = STATE_ALIVE;

    m_currentpage = PAGE_GAMEPLAY;

    return 1;
}

static int options_view( void )
{
    int i;

    // disable all menu stuff
    m_title.state       = STATE_DEAD;
    m_menubar.state     = STATE_DEAD;
    m_menugame.state    = STATE_DEAD;
    m_menuoptions.state = STATE_DEAD;
    m_menuscores.state  = STATE_DEAD;
    m_menuhelp.state    = STATE_DEAD;

    for( i = 0; m_buttons[ i ]; i++ ) {
        m_buttons[ i ]->state = STATE_DEAD;
    }

    m_menuabout.state       = STATE_ALIVE;
    m_butback.state         = STATE_ALIVE;
    m_menuoptions2.state    = STATE_ALIVE;

    m_butoptpages[ 0 ].state        = STATE_BIRTH;
    m_butoptpages[ 0 ].src.left     = 0;
    m_butoptpages[ 0 ].src.right    = m_butoptpages[ 0 ].width;

    m_butoptpages[ 1 ].state        = STATE_DYING;
    m_butoptpages[ 1 ].src.left     = 2 * m_butoptpages[ 1 ].width;
    m_butoptpages[ 1 ].src.right    = m_butoptpages[ 1 ].src.left + m_butoptpages[ 1 ].width;

    m_butoptpages[ 2 ].state        = STATE_DYING;
    m_butoptpages[ 2 ].src.left     = 2 * m_butoptpages[ 2 ].width;
    m_butoptpages[ 2 ].src.right    = m_butoptpages[ 2 ].src.left + m_butoptpages[ 2 ].width;

    m_butoptpages[ 3 ].state        = STATE_DYING;
    m_butoptpages[ 3 ].src.left     = 2 * m_butoptpages[ 3 ].width;
    m_butoptpages[ 3 ].src.right    = m_butoptpages[ 3 ].src.left + m_butoptpages[ 3 ].width;

    sys_playsound( m_sounds[ SND_ASTEXPL0 ], 0 );
    return 1;
}

static int options_viewpage( int page )
{
    int i;

    for( i = 0; i < MAX_OPTPAGES; i++ ) {
        m_butoptpages[ i ].state        = STATE_DYING;
        m_butoptpages[ i ].src.left     = 2 * m_butoptpages[ i ].width;
        m_butoptpages[ i ].src.right    = m_butoptpages[ i ].src.left + m_butoptpages[ 1 ].width;
    }

    m_butoptpages[ page ].state        = STATE_ALIVE;
    m_butoptpages[ page ].src.left     = m_butoptpages[ page ].width;
    m_butoptpages[ page ].src.right    = m_butoptpages[ page ].src.left + m_butoptpages[ page ].width;

    switch( page ) {
        case 0 :
            for( i = 0; m_proppagecontrols[ i ]; i++ ) {
                m_proppagecontrols[ i ]->state = STATE_PREBIRTH;
            }
            m_butsliding.state = STATE_PREBIRTH; m_butdirectional.state = STATE_PREBIRTH;
            m_drpshipphysicssel.state = STATE_PREBIRTH;
            m_sldrsound.state = m_sldrboxsound.state = STATE_ALIVE;
            m_sldrmusic.state = m_sldrboxmusic.state = STATE_ALIVE;

            m_lblDirectional.state  = STATE_DEAD;
            m_lblSliding.state      = STATE_DEAD;

            m_sldrboxsound.x = ( SCREEN_HALFWIDTH_f - 50.0f ) + ( m_options.soundlevel );
            m_sldrboxmusic.x = ( SCREEN_HALFWIDTH_f - 50.0f ) + ( m_options.musiclevel );
            break;

        case 1 :
            for( i = 0; m_proppagecontrols[ i ]; i++ ) {
                m_proppagecontrols[ i ]->state = STATE_PREBIRTH;
            }
            m_drpshipphysicssel.state   = STATE_ALIVE;
            m_drpshipphysics.state      = STATE_ALIVE;

            if( m_options.physicstype ) {
                m_drpshipphysicssel.src.left = 128;
                m_lblSliding.state      = STATE_ALIVE;
            }
            else {
                m_drpshipphysicssel.src.left = 0;
                m_lblDirectional.state  = STATE_ALIVE;
            }
            m_drpshipphysicssel.src.right = m_drpshipphysicssel.src.left + m_drpshipphysicssel.width;

            m_drpshipdropped        = 0;
            m_lstshipphysics.state  = STATE_DEAD;
            m_butsliding.state      = STATE_DEAD;
            m_butdirectional.state  = STATE_DEAD;
            break;

        case 2 :
            for( i = 0; m_proppagecontrols[ i ]; i++ ) {
                m_proppagecontrols[ i ]->state = STATE_PREBIRTH;
            }
            m_butsliding.state = STATE_DEAD; m_butdirectional.state = STATE_DEAD;
            m_drpshipphysicssel.state   = STATE_DEAD;

            m_lblDirectional.state  = STATE_DEAD;
            m_lblSliding.state      = STATE_DEAD;

            if( m_options.playershottrails ) {
                m_chkplyrshottrails.state = STATE_DYING;
                m_chkplyrshottrails.src.left = 2 * m_chkplyrshottrails.width;
                m_chkplyrshottrails.src.right = m_chkplyrshottrails.src.left + m_chkplyrshottrails.width;
            }
            else {
                m_chkplyrshottrails.state = STATE_BIRTH;
                m_chkplyrshottrails.src.left = 0;
                m_chkplyrshottrails.src.right = m_chkplyrshottrails.width;
            }

            if( m_options.playerthrusting ) {
                m_chkplyrthrusting.state = STATE_DYING;
                m_chkplyrthrusting.src.left = 2 * m_chkplyrthrusting.width;
                m_chkplyrthrusting.src.right = m_chkplyrthrusting.src.left + m_chkplyrthrusting.width;
            }
            else {
                m_chkplyrthrusting.state = STATE_BIRTH;
                m_chkplyrthrusting.src.left = 0;
                m_chkplyrthrusting.src.right = m_chkplyrthrusting.width;
            }

            if( m_options.playershotexpl ) {
                m_chkplyrshotexpl.state = STATE_DYING;
                m_chkplyrshotexpl.src.left = 2 * m_chkplyrshotexpl.width;
                m_chkplyrshotexpl.src.right = m_chkplyrshotexpl.src.left + m_chkplyrshotexpl.width;
            }
            else {
                m_chkplyrshotexpl.state = STATE_BIRTH;
                m_chkplyrshotexpl.src.left = 0;
                m_chkplyrshotexpl.src.right = m_chkplyrshotexpl.width;
            }

            if( m_options.playerexpl ) {
                m_chkplyrexpl.state = STATE_DYING;
                m_chkplyrexpl.src.left = 2 * m_chkplyrexpl.width;
                m_chkplyrexpl.src.right = m_chkplyrexpl.src.left + m_chkplyrexpl.width;
            }
            else {
                m_chkplyrexpl.state = STATE_BIRTH;
                m_chkplyrexpl.src.left = 0;
                m_chkplyrexpl.src.right = m_chkplyrexpl.width;
            }

            if( m_options.nmeshottrails ) {
                m_chknmeshottrails.state = STATE_DYING;
                m_chknmeshottrails.src.left = 2 * m_chknmeshottrails.width;
                m_chknmeshottrails.src.right = m_chknmeshottrails.src.left + m_chknmeshottrails.width;
            }
            else {
                m_chknmeshottrails.state = STATE_BIRTH;
                m_chknmeshottrails.src.left = 0;
                m_chknmeshottrails.src.right = m_chknmeshottrails.width;
            }

            if( m_options.nmeshotexpl ) {
                m_chknmeshotexpl.state = STATE_DYING;
                m_chknmeshotexpl.src.left = 2 * m_chknmeshotexpl.width;
                m_chknmeshotexpl.src.right = m_chknmeshotexpl.src.left + m_chknmeshotexpl.width;
            }
            else {
                m_chknmeshotexpl.state = STATE_BIRTH;
                m_chknmeshotexpl.src.left = 0;
                m_chknmeshotexpl.src.right = m_chknmeshotexpl.width;
            }

            if( m_options.nmeexpl ) {
                m_chknmeexpl.state = STATE_DYING;
                m_chknmeexpl.src.left = 2 * m_chknmeexpl.width;
                m_chknmeexpl.src.right = m_chknmeexpl.src.left + m_chknmeexpl.width;
            }
            else {
                m_chknmeexpl.state = STATE_BIRTH;
                m_chknmeexpl.src.left = 0;
                m_chknmeexpl.src.right = m_chknmeexpl.width;
            }

            break;

        case 3 :
            for( i = 0; m_proppagecontrols[ i ]; i++ ) {
                m_proppagecontrols[ i ]->state = STATE_PREBIRTH;
            }
            m_butsliding.state = STATE_DEAD; m_butdirectional.state = STATE_DEAD;
            m_drpshipphysicssel.state   = STATE_DEAD;

            m_lblDirectional.state  = STATE_DEAD;
            m_lblSliding.state      = STATE_DEAD;

            m_sldrmouse.state = m_sldrboxmouse.state = STATE_ALIVE;
            m_sldrboxmouse.x = ( SCREEN_HALFWIDTH_f - 50.0f ) + ( m_options.mouseoffset );

            break;
    }

    return 1;
}

static int about_view( void )
{
    m_menugame.state    = STATE_DEAD;
    m_butnew.state      = m_butresume.state = m_butstop.state = STATE_DEAD;

    m_menuscores.state  = STATE_DEAD;
    m_butviewhs.state   = m_butreset.state = STATE_DEAD;

    m_menuhelp.state    = STATE_DEAD;
    m_butabout.state    = STATE_DEAD;

    m_title.state       = STATE_DEAD;
    m_menubar.state     = STATE_DEAD;
    m_butgame.state     = m_butscores.state = m_butoptions.state = m_buthelp.state = STATE_DEAD;
    m_butviewhp.state   = m_butexit.state   = STATE_DEAD;

    m_menuabout.state   = STATE_ALIVE;
    m_butback.state     = STATE_ALIVE;
    m_marquee.state     = STATE_ALIVE;
    m_marquee.offset    = SCREEN_HEIGHT;

    sys_pausemp3( m_menutrack );
    sys_playmp3( m_creditstrack );

    return 1;
}

static int menu_view( void )
{
    m_mousecursor.state = STATE_ALIVE;
    m_title.state       = STATE_ALIVE;
    m_menubar.state     = STATE_ALIVE;

    m_butgame.state     = STATE_ALIVE;
    m_butscores.state   = STATE_ALIVE;
    m_buthelp.state     = STATE_ALIVE;
    m_butoptions.state  = STATE_ALIVE;
    m_butexit.state     = STATE_ALIVE;

    m_menuactive        = 1;
    lastfocus           = NULL;

    sys_playmp3( m_menutrack );

    return 1;
}

static void game_resolveshot( obj_t *src, obj_t *dst )
{
    int i, astcol, points;
    int count, piece;
    int direction;
    obj_t *dstobj;

    astcol = dst->stateasteroid.collection;

    sys_playsound( m_sounds[ 0 == rand() % 2 ? SND_ASTEXPL0 : SND_ASTEXPL1 ], 0 );

    // subtract one for the larger ast that was just shot
    m_astcols[ astcol ].activepieces--;

    // was the smallest piece shot?
    if( dst->stateasteroid.type > ASTTYPE_PIECE2 ) {
        if( m_astcols[ astcol ].activepieces <= 0 ) {
            if( !m_astcols[ ASTCOL_DIRT ].activepieces && !m_astcols[ ASTCOL_ROCK ].activepieces ) {
                // then end of level
                m_player.state = STATE_DEAD;

                // the player could have flown into the last asteroid
                if( &m_player == src ) {
                    // subtract a life - unless we're cheating ;)
                    if( !m_cheatgod && !ISBONUSROUND() ) {
                        if( --m_player.stateplayer.livesleft < 0 ) {
                            // game over - no lives left
                            game_stats();
                            return;
                        }
                    }
                }

                if( ISBONUSROUND() ) {
                    if( m_didanyleave ) {
                        points = ( dst->points * ( ( m_level + 1 ) * 10 ) );
                        m_player.stateplayer.score += points;
                        sprintf_s( m_risingscore.staterisingscore.string, 16, "%d", points );
                        risingscore_init( &m_risingscore, 0, dst->x, dst->y );
                    }
                    else {
                        points = ( dst->points * ( ( m_level + 1 ) * 50 ) );
                        sys_playsound( m_sounds[ SND_APPLAUSE ], 0 );
                        m_player.stateplayer.score += points;
                        sprintf_s( m_risingscore.staterisingscore.string, 16, "%d", points );
                        risingscore_init( &m_risingscore, 1, dst->x, dst->y );
                    }
                }

                if( ++m_round >= MAX_ROUNDS ) {
                    m_round = 0;
                    m_level++;
                }

                m_waitforplayer = ( MAX_ROUNDS - 1 == m_round );
                m_playerready   = 0;

                game_stats();
                return;
            }
        }

        if( ISBONUSROUND() ) {
            points = ( dst->points * ( ( m_level + 1 ) * 10 ) );
            m_player.stateplayer.score += points;
            sprintf_s( m_risingscore.staterisingscore.string, 16, "%d", points );
            risingscore_init( &m_risingscore, 0, dst->x, dst->y );
        }

        // the smallest piece was shot - nothing left to do
        return;
    }
    // else the shot piece must split into smaller pieces

    if( ISBONUSROUND() ) {
        points = ( dst->points * ( ( m_level + 1 ) * 10 ) );
        m_player.stateplayer.score += points;
        sprintf_s( m_risingscore.staterisingscore.string, 16, "%d", points );
        risingscore_init( &m_risingscore, 0, dst->x, dst->y );
    }
    else {
        m_player.stateplayer.score += dst->points;
    }

    // activate the two child pieces based on the ast that was just shot
    switch( dst->stateasteroid.type ) {
        case ASTTYPE_PIECE0 : piece = ASTTYPE_PIECE1; break;
        case ASTTYPE_PIECE1 : piece = ASTTYPE_PIECE3; break;
        case ASTTYPE_PIECE2 : piece = ASTTYPE_PIECE5; break;
        default :
            return;
    }

    if( src->spd * src->mass > dst->spd * dst->mass ) {
        direction = src->direction;
    }
    else {
        direction = dst->direction;
    }

    for( i = 0, count = 0; count < 2 && i < MAX_ASTS; i++ ) {
        dstobj = &m_astcols[ astcol ].astpieces[ piece ][ i ];
        if( STATE_PREBIRTH == dstobj->state ) {
            dstobj->state   = STATE_ALIVE;
            dstobj->x       = dst->x;
            dstobj->y       = dst->y;

            dstobj->direction = direction + game_rand( -20, 20 );

            //dstobj->spd = (float)( (int)rand() % (int)ASTEROID_SPEEDMAX ) + ASTEROID_SPEEDMIN;
            dstobj->spd = (float)( (float)ASTEROID_SPEED * ( ( ( (int)m_level + (int)1 ) * (int)10 ) + ( (int)m_round + (int)1 ) ) ) + ( (float)game_rand( 5, 20 ) / 100.0f );

            dstobj->spdx = dstobj->spd * (float)cos( PI_OVER_180 * dstobj->direction );
            dstobj->spdy = dstobj->spd * (float)sin( PI_OVER_180 * dstobj->direction );

            count++; piece++;
            m_astcols[ astcol ].activepieces++;

            // check to see if it's a reduce bomb that destroyed this asteroid
            if( src == dst && dst->stateasteroid.bombactive ) {
                // don't set the lowest type to blow up - it'll end the level ;)
                if( dstobj->stateasteroid.type <= ASTTYPE_PIECE2 ) {
                    // set this asteroid to blow up in the near future
                    dstobj->stateasteroid.bombactive    = 1;
                    dstobj->stateasteroid.bombcounter   = 0;
                    dstobj->stateasteroid.bombthreshold = game_rand( BOMBREDUCE_MINTHRESHOLD, BOMBREDUCE_MAXTHRESHOLD );

                    // change it's direction to be something random
                    dstobj->direction = rand() % 360;

                    dstobj->spdx = dstobj->spd * (float)cos( PI_OVER_180 * dstobj->direction );
                    dstobj->spdy = dstobj->spd * (float)sin( PI_OVER_180 * dstobj->direction );
                }
            }
        }
    }
}

static int game_pointinbox( int ptx, int pty, int boxx, int boxy, int boxw, int boxh )
{
    if( ptx <= boxx - boxw ) {
        return 0;
    }
    if( ptx >= boxx + boxw ) {
        return 0;
    }
    if( pty <= boxy - boxh ) {
        return 0;
    }
    if( pty >= boxy + boxh ) {
        return 0;
    }
    return 1;
}

static void clstr_begindraw( unsigned char **buffer, long *numBytesPerScanline )
{
    DDSURFACEDESC2 ddsd;

    (*buffer) = NULL;
    (*numBytesPerScanline)  = 0;

    ddsd.dwSize = sizeof( DDSURFACEDESC2 );
    if( SUCCEEDED( g_backbuffer->Lock( NULL, &ddsd, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT, NULL ) ) ) {
        (*buffer) = (unsigned char *)ddsd.lpSurface;
        (*numBytesPerScanline)  = ddsd.lPitch;
    }
}

static void clstr_enddraw( void )
{
    g_backbuffer->Unlock( NULL );
}

static void shottrail_init( clstr_t *clstr, int addrate
                          , int minr, int maxr
                          , int ming, int maxg
                          , int minb, int maxb
                          , int desthue, int inc )
{
    int i;

    memset( clstr, 0, sizeof( clstr_t ) );

    clstr->state   = STATE_DEAD;
    clstr->addrate = addrate;

    clstr->minr = minr;
    clstr->ming = ming;
    clstr->minb = minb;
    clstr->maxr = maxr;
    clstr->maxg = maxg;
    clstr->maxb = maxb;

    for( i = 0; i < MAX_SHOTTRAILPTLES; i++ ) {
        clstr->ptles[ i ].x         = 0.0f;
        clstr->ptles[ i ].y         = 0.0f;
        clstr->ptles[ i ].spdx      = 0.0f;
        clstr->ptles[ i ].spdy      = 0.0f;
        clstr->ptles[ i ].accelx    = 0.0f;
        clstr->ptles[ i ].accely    = 0.0f;

        clstr->ptles[ i ].r     = (unsigned char)( ( rand() % ( maxr - minr ) ) + minr );
        clstr->ptles[ i ].g     = (unsigned char)( ( rand() % ( maxg - ming ) ) + ming );
        clstr->ptles[ i ].b     = (unsigned char)( ( rand() % ( maxb - minb ) ) + minb );

        clstr->ptles[ i ].incr  = inc;
        clstr->ptles[ i ].incg  = inc;
        clstr->ptles[ i ].incb  = inc;

        clstr->ptles[ i ].dstr  = desthue;
        clstr->ptles[ i ].dstg  = desthue;
        clstr->ptles[ i ].dstb  = desthue;
    }
}

static void shottrail_think( clstr_t *clstr )
{
    int i, count;
    int r, g, b;

    if( STATE_ALIVE == clstr->state ) {

        if( clstr->obj && STATE_ALIVE == clstr->obj->state ) {
            shottrail_add( clstr );
        }
        else {
            clstr->obj = NULL;
        }

        count = 0;
        for( i = 0; i < clstr->ptlecount; i++ ) {
            if( STATE_ALIVE == clstr->ptles[ i ].state ) {
                r = g = b = 0;

                clstr->ptles[ i ].r += clstr->ptles[ i ].incr;
                if( clstr->ptles[ i ].r <= clstr->ptles[ i ].dstr ) {
                    clstr->ptles[ i ].r = clstr->ptles[ i ].dstr;
                    r = 1;
                }

                clstr->ptles[ i ].g += clstr->ptles[ i ].incg;
                if( clstr->ptles[ i ].g <= clstr->ptles[ i ].dstg ) {
                    clstr->ptles[ i ].g = clstr->ptles[ i ].dstg;
                    g = 1;
                }

                clstr->ptles[ i ].b += clstr->ptles[ i ].incb;
                if( clstr->ptles[ i ].b <= clstr->ptles[ i ].dstb ) {
                    clstr->ptles[ i ].b = clstr->ptles[ i ].dstb;
                    b = 1;
                }

                if( r && g && b ) {
                    clstr->ptles[ i ].state = STATE_DEAD;
                    count++;
                }
            }
            else {
                count++;
            }
        }

        if( count >= clstr->ptlecount ) {
            clstr->state = STATE_DEAD;
        }
    }
}

static void shottrail_move( clstr_t *clstr )
{
    int i;

    if( STATE_ALIVE == clstr->state ) {
        for( i = 0; i < clstr->ptlecount; i++ ) {
            if( STATE_ALIVE == clstr->ptles[ i ].state ) {
                clstr->ptles[ i ].x += (float)( (int)rand() % (int)3 - (int)1 );
                clstr->ptles[ i ].y += (float)( (int)rand() % (int)3 - (int)1 );
            }
        }
    }
}

static void clstr_move( clstr_t *clstr )
{
    int i;

    if( STATE_ALIVE == clstr->state ) {
        for( i = 0; i < clstr->ptlecount; i++ ) {
            if( STATE_ALIVE == clstr->ptles[ i ].state ) {
                clstr->ptles[ i ].x += clstr->ptles[ i ].spdx;
                clstr->ptles[ i ].y += clstr->ptles[ i ].spdy;
            }
        }
    }
}

static void clstr_collide( clstr_t *clstr )
{
}

static void shottrail_add( clstr_t *clstr )
{
    int startidx, finishidx;
    int i;

    if( !clstr->obj ) {
        return;
    }

    if( clstr->ptlecount >= MAX_SHOTTRAILPTLES ) {
        startidx             = clstr->recyclecount;
        clstr->recyclecount += clstr->addrate;

        if( clstr->recyclecount >= MAX_SHOTTRAILPTLES ) {
            startidx            = 0;
            clstr->recyclecount = 0;
        }
    }
    else {
        startidx = clstr->ptlecount;
        clstr->ptlecount += clstr->addrate;

        if( clstr->ptlecount >= MAX_SHOTTRAILPTLES ) {
            clstr->ptlecount    = MAX_SHOTTRAILPTLES;
            startidx            = 0;
            clstr->recyclecount = 0;
        }
    }

    finishidx = startidx + clstr->addrate;
    for( i = startidx; i <= finishidx; i++ ) {
        clstr->ptles[ i ].x = clstr->obj->x + (float)game_rand( -1, 1 );
        clstr->ptles[ i ].y = clstr->obj->y + (float)game_rand( -1, 1 );
        clstr->ptles[ i ].state = STATE_ALIVE;

        clstr->ptles[ i ].r = (unsigned char)( ( rand() % ( clstr->maxr - clstr->minr ) ) + clstr->minr );
        clstr->ptles[ i ].g = (unsigned char)( ( rand() % ( clstr->maxg - clstr->ming ) ) + clstr->ming );
        clstr->ptles[ i ].b = (unsigned char)( ( rand() % ( clstr->maxb - clstr->minb ) ) + clstr->minb );
    }
}

static void clstr_draw(clstr_t *clstr, unsigned char *buffer, long numBytesPerScanline)
{
    ptle_t *ptle;
    int i;
    unsigned int offset;

    if (STATE_ALIVE == clstr->state && buffer)
    {
        for (i = 0; i < clstr->ptlecount; i++)
        {
            ptle = &clstr->ptles[i];

            if ((ptle->r || ptle->g || ptle->b) &&
                ptle->x >= 0 && ptle->x < SCREEN_WIDTH_f &&
                ptle->y >= 0 && ptle->y < SCREEN_HEIGHT_f)
            {
                offset = (numBytesPerScanline * (long)ptle->y) + ((long)ptle->x * 4);
                buffer[offset + 0] = ptle->r;
                buffer[offset + 1] = ptle->g;
                buffer[offset + 2] = ptle->b;
                buffer[offset + 3] = 255;
            }
        }
    }
}

static void shottrail_setactive( clstr_t *clstr, obj_t *obj
                               , int minr, int maxr
                               , int ming, int maxg
                               , int minb, int maxb
                               , int desthue, int inc )
{
    int i;

    for( i = 0; i < MAX_SHOTTRAILS; i++ ) {
        if( STATE_ALIVE != clstr[ i ].state ) {
            break;
        }
    }

    if( i >= MAX_SHOTTRAILS ) {
        i = 0;
    }

    shottrail_init( &clstr[ i ], clstr->addrate
                  , minr, maxr
                  , ming, maxg
                  , minb, maxb
                  , desthue, inc );

    clstr[ i ].state = STATE_ALIVE;
    clstr[ i ].obj   = obj;
}

static void shotrichochet_init( clstr_t *clstr
                              , int angle, float x, float y
                              , int minr, int maxr
                              , int ming, int maxg
                              , int minb, int maxb
                              , int desthue, int inc )
{
    int i, ang;

    memset( clstr, 0, sizeof( clstr_t ) );
    clstr->state = STATE_DEAD;

    clstr->minr = minr;
    clstr->ming = ming;
    clstr->minb = minb;
    clstr->maxr = maxr;
    clstr->maxg = maxg;
    clstr->maxb = maxb;


    for( i = 0; i < MAX_SHOTRICHOCHETPTLES; i++ ) {
        if( angle < 0 ) {
            ang = rand() % 360;
        }
        else {
            ang = angle + ( ( rand() % ( MAX_SHOTRICHOCHETSPREADANG * 2 ) ) - MAX_SHOTRICHOCHETSPREADANG );
        }

        clstr->ptles[ i ].spdx      = (float)cos( PI_OVER_180 * (float)ang );
        clstr->ptles[ i ].spdy      = (float)sin( PI_OVER_180 * (float)ang );

        clstr->ptles[ i ].spdx      *= (float)( (int)rand() % (int)MAX_SHOTRICHOCHETSPEED ) / 10.0f;
        clstr->ptles[ i ].spdy      *= (float)( (int)rand() % (int)MAX_SHOTRICHOCHETSPEED ) / 10.0f;

        clstr->ptles[ i ].spdx      += MIN_SHOTRICHOCHETSPEED;
        clstr->ptles[ i ].spdy      += MIN_SHOTRICHOCHETSPEED;

        clstr->ptles[ i ].x         = x + (float)( (int)( rand() % (int)3 ) - (int)1 );
        clstr->ptles[ i ].y         = y + (float)( (int)( rand() % (int)3 ) - (int)1 );
        clstr->ptles[ i ].accelx    = 0.0f;
        clstr->ptles[ i ].accely    = 0.0f;

        clstr->ptles[ i ].r     = (unsigned char)( ( rand() % ( maxr - minr ) ) + minr );
        clstr->ptles[ i ].g     = (unsigned char)( ( rand() % ( maxg - ming ) ) + ming );
        clstr->ptles[ i ].b     = (unsigned char)( ( rand() % ( maxb - minb ) ) + minb );

        clstr->ptles[ i ].incr  = inc;
        clstr->ptles[ i ].incg  = inc;
        clstr->ptles[ i ].incb  = inc;

        clstr->ptles[ i ].dstr  = desthue;
        clstr->ptles[ i ].dstg  = desthue;
        clstr->ptles[ i ].dstb  = desthue;
    }
}

static void shotrichochet_think( clstr_t *clstr )
{
    int i, count;
    int r, g, b;

    if( STATE_ALIVE == clstr->state ) {
        shotrichochet_add( clstr );

        count = 0;
        for( i = 0; i < clstr->ptlecount; i++ ) {
            if( STATE_ALIVE == clstr->ptles[ i ].state ) {
                r = g = b = 0;

                clstr->ptles[ i ].r += clstr->ptles[ i ].incr;
                if( clstr->ptles[ i ].r <= clstr->ptles[ i ].dstr ) {
                    clstr->ptles[ i ].r = clstr->ptles[ i ].dstr;
                    r = 1;
                }

                clstr->ptles[ i ].g += clstr->ptles[ i ].incg;
                if( clstr->ptles[ i ].g <= clstr->ptles[ i ].dstg ) {
                    clstr->ptles[ i ].g = clstr->ptles[ i ].dstg;
                    g = 1;
                }

                clstr->ptles[ i ].b += clstr->ptles[ i ].incb;
                if( clstr->ptles[ i ].b <= clstr->ptles[ i ].dstb ) {
                    clstr->ptles[ i ].b = clstr->ptles[ i ].dstb;
                    b = 1;
                }

                if( r && g && b ) {
                    clstr->ptles[ i ].state = STATE_DEAD;
                    count++;
                }
            }
            else {
                count++;
            }
        }

        if( count >= clstr->ptlecount ) {
            clstr->state = STATE_DEAD;
        }
    }
}

static void shotrichochet_add( clstr_t *clstr )
{
    int startidx, finishidx;
    int i;

    if( clstr->ptlecount >= MAX_SHOTRICHOCHETPTLES ) {
        return;
    }

    startidx  = clstr->ptlecount;
    finishidx = startidx + SHOTRICHOCHET_ADD;

    clstr->ptlecount += SHOTRICHOCHET_ADD;

    if( clstr->ptlecount >= MAX_SHOTRICHOCHETPTLES ) {
        clstr->ptlecount = MAX_SHOTRICHOCHETPTLES;
        finishidx        = MAX_SHOTRICHOCHETPTLES - 1;
    }

    for( i = startidx; i <= finishidx; i++ ) {
        clstr->ptles[ i ].state = STATE_ALIVE;
    }

}

static void shotrichochet_setactive( clstr_t *clstr
                                   , int angle, float x, float y
                                   , int minr, int maxr
                                   , int ming, int maxg
                                   , int minb, int maxb
                                   , int desthue, int inc )
{
    int i;

    for( i = 0; i < MAX_SHOTRICHOCHETS; i++ ) {
        if( STATE_ALIVE != clstr[ i ].state ) {
            break;
        }
    }

    if( i >= MAX_SHOTRICHOCHETS ) {
        i = 0;
    }

    shotrichochet_init( &clstr[ i ]
                      , angle, x, y
                      , minr, maxr
                      , ming, maxg
                      , minb, maxb
                      , desthue, inc );

    clstr[ i ].state = STATE_ALIVE;
}

static void menu_draw( obj_t *obj )
{
    if( STATE_ALIVE == obj->state ) {
        game_maptoscreen( obj );
        g_backbuffer->Blt( &obj->dst
                         , obj->dds
                         , &obj->src
                         , DDBLT_WAIT
                         , NULL );
    }
}

static void menu_drawhelppage0( void )
{
    if( STATE_ALIVE == m_helppages[ 0 ].state ) {
        game_maptoscreen( &m_helppages[ 0 ] );
        g_backbuffer->Blt( &m_helppages[ 0 ].dst, m_helppages[ 0 ].dds, &m_helppages[ 0 ].src, DDBLT_WAIT, NULL );
    }
}

static void menu_drawhelppage1( void )
{
    if( STATE_ALIVE == m_helppages[ 1 ].state ) {
        game_maptoscreen( &m_helppages[ 1 ] );
        g_backbuffer->Blt( &m_helppages[ 1 ].dst, m_helppages[ 1 ].dds, &m_helppages[ 1 ].src, DDBLT_WAIT, NULL );
    }
}

static void menu_drawhelppage2( void )
{
    if( STATE_ALIVE == m_helppages[ 2 ].state ) {
        game_maptoscreen( &m_helppages[ 2 ] );
        g_backbuffer->Blt( &m_helppages[ 2 ].dst, m_helppages[ 2 ].dds, &m_helppages[ 2 ].src, DDBLT_WAIT, NULL );
    }
}

static void menu_drawhelppage3( void )
{
    if( STATE_ALIVE == m_helppages[ 3 ].state ) {
        game_maptoscreen( &m_helppages[ 3 ] );
        g_backbuffer->Blt( &m_helppages[ 3 ].dst, m_helppages[ 3 ].dds, &m_helppages[ 3 ].src, DDBLT_WAIT, NULL );
    }
}

static void menu_drawhelppage4( void )
{
    if( STATE_ALIVE == m_helppages[ 4 ].state ) {
        game_maptoscreen( &m_helppages[ 4 ] );
        g_backbuffer->Blt( &m_helppages[ 4 ].dst, m_helppages[ 4 ].dds, &m_helppages[ 4 ].src, DDBLT_WAIT, NULL );
    }
}

static void menu_drawhelppage5( void )
{
    if( STATE_ALIVE == m_helppages[ 5 ].state ) {
        game_maptoscreen( &m_helppages[ 5 ] );
        g_backbuffer->Blt( &m_helppages[ 5 ].dst, m_helppages[ 5 ].dds, &m_helppages[ 5 ].src, DDBLT_WAIT, NULL );
    }
}

static void menu_drawhelppage6( void )
{
    if( STATE_ALIVE == m_helppages[ 6 ].state ) {
        game_maptoscreen( &m_helppages[ 6 ] );
        g_backbuffer->Blt( &m_helppages[ 6 ].dst, m_helppages[ 6 ].dds, &m_helppages[ 6 ].src, DDBLT_WAIT, NULL );
    }
}

static void menu_drawhelppage7( void )
{
    if( STATE_ALIVE == m_helppages[ 7 ].state ) {
        game_maptoscreen( &m_helppages[ 7 ] );
        g_backbuffer->Blt( &m_helppages[ 7 ].dst, m_helppages[ 7 ].dds, &m_helppages[ 7 ].src, DDBLT_WAIT, NULL );
    }
}

static void button_execute( obj_t *obj, int popup )
{
    int i;

    if( popup ) {
        if( obj == &m_butgame ) {
            // activate the hovered over menu
            m_menugame.state    = STATE_ALIVE;
            m_butnew.state      = STATE_ALIVE;
            if( m_gameactive ) {
                m_butresume.state   = m_butstop.state = STATE_ALIVE;
            }
            else {
                m_butresume.state   = m_butstop.state = STATE_PAUSED;
                m_butresume.src.left  = 128;
                m_butresume.src.right = 192;
                m_butstop.src.left  = 128;
                m_butstop.src.right = 192;
            }

            // disable the other menus
            m_menuscores.state  = STATE_DEAD;
            m_butviewhs.state   = m_butreset.state = STATE_DEAD;
            m_menuhelp.state    = STATE_DEAD;
            m_butviewhp.state   = m_butabout.state = STATE_DEAD;
            m_menuoptions.state = STATE_DEAD;
            m_butviewopt.state  = STATE_DEAD;

            m_popupactive = 1;
        }
        else if( obj == &m_butscores ) {
            // activate the hovered over menu
            m_menuscores.state  = STATE_ALIVE;
            m_butviewhs.state   = m_butreset.state = STATE_ALIVE;

            // disable the other menus
            m_menugame.state    = STATE_DEAD;
            m_butnew.state      = m_butresume.state = m_butstop.state = STATE_DEAD;
            m_menuhelp.state    = STATE_DEAD;
            m_butviewhp.state   = m_butabout.state = STATE_DEAD;
            m_menuoptions.state = STATE_DEAD;
            m_butviewopt.state  = STATE_DEAD;

            m_popupactive = 1;
        }
        else if( obj == &m_buthelp ) {
            // activate the hovered over menu
            m_menuhelp.state    = STATE_ALIVE;
            m_butviewhp.state   = m_butabout.state = STATE_ALIVE;

            // disable the other menus
            m_menugame.state    = STATE_DEAD;
            m_butnew.state      = m_butresume.state = m_butstop.state = STATE_DEAD;
            m_menuscores.state  = STATE_DEAD;
            m_butviewhs.state   = m_butreset.state = STATE_DEAD;
            m_menuoptions.state = STATE_DEAD;
            m_butviewopt.state  = STATE_DEAD;

            m_popupactive = 1;
        }
        else if( obj == &m_butoptions ) {
            // activate the hovered over menu
            m_menuoptions.state = STATE_ALIVE;
            m_butviewopt.state  = STATE_ALIVE;

            // disable the other menus
            m_menugame.state    = STATE_DEAD;
            m_butnew.state      = m_butresume.state = m_butstop.state = STATE_DEAD;
            m_menuscores.state  = STATE_DEAD;
            m_butviewhs.state   = m_butreset.state = STATE_DEAD;
            m_menuhelp.state    = STATE_DEAD;
            m_butviewhp.state   = m_butabout.state = STATE_DEAD;

            m_popupactive = 1;
        }
    }
    else {
        if( obj == &m_butgame ) {
            // activate the clicked on menu
            m_menugame.state    = STATE_ALIVE;
            m_butnew.state      = STATE_ALIVE;
            if( m_gameactive ) {
                m_butresume.state   = m_butstop.state = STATE_ALIVE;
            }
            else {
                m_butresume.state   = m_butstop.state = STATE_PAUSED;
                m_butresume.src.left  = 128;
                m_butresume.src.right = 192;
                m_butstop.src.left  = 128;
                m_butstop.src.right = 192;
            }

            // disable the other menus
            m_menuscores.state  = STATE_DEAD;
            m_butviewhs.state   = m_butreset.state = STATE_DEAD;
            m_menuhelp.state    = STATE_DEAD;
            m_butviewhp.state   = m_butabout.state = STATE_DEAD;
            m_menuoptions.state = STATE_DEAD;
            m_butviewopt.state  = STATE_DEAD;

            m_popupactive = 1;
            sys_playsound( m_sounds[ SND_ASTEXPL0 ], 0 );
        }
        else if( obj == &m_butscores ) {
            // activate the clicked on menu
            m_menuscores.state = STATE_ALIVE;
            m_butviewhs.state  = m_butreset.state = STATE_ALIVE;

            // disable the other menus
            m_menugame.state    = STATE_DEAD;
            m_butnew.state      = m_butresume.state = m_butstop.state = STATE_DEAD;
            m_menuhelp.state    = STATE_DEAD;
            m_butviewhp.state   = m_butabout.state = STATE_DEAD;
            m_menuoptions.state = STATE_DEAD;
            m_butviewopt.state  = STATE_DEAD;

            m_popupactive = 1;
            sys_playsound( m_sounds[ SND_ASTEXPL0 ], 0 );
        }
        else if( obj == &m_buthelp ) {
            // activate the clicked on menu
            m_menuhelp.state    = STATE_ALIVE;
            m_butviewhp.state   = m_butabout.state = STATE_ALIVE;

            // disable the other menus
            m_menugame.state    = STATE_DEAD;
            m_butnew.state      = m_butresume.state = m_butstop.state = STATE_DEAD;
            m_menuscores.state  = STATE_DEAD;
            m_butviewhs.state   = m_butreset.state = STATE_DEAD;
            m_menuoptions.state = STATE_DEAD;
            m_butviewopt.state  = STATE_DEAD;

            m_popupactive = 1;
            sys_playsound( m_sounds[ SND_ASTEXPL0 ], 0 );
        }
        else if( obj == &m_butoptions ) {
            // activate the clicked on menu
            m_menuoptions.state = STATE_ALIVE;
            m_butviewopt.state  = STATE_ALIVE;

            // disable the other menus
            m_menugame.state    = STATE_DEAD;
            m_butnew.state      = m_butresume.state = m_butstop.state = STATE_DEAD;
            m_menuscores.state  = STATE_DEAD;
            m_butviewhs.state   = m_butreset.state = STATE_DEAD;
            m_menuhelp.state    = STATE_DEAD;
            m_butviewhp.state   = m_butabout.state = STATE_DEAD;

            m_popupactive = 1;
            sys_playsound( m_sounds[ SND_ASTEXPL0 ], 0 );
        }
        else if( obj == &m_butviewopt ) {
            m_popupactive = 0;
            options_view();
            options_viewpage( 0 );
        }
        else if( obj == &m_butexit ) {
            PostQuitMessage( 0 );
        }
        else if( obj == &m_butnew ) {
            m_newgamepending = 1;
            if( !entername_init() ) {
                m_newgamepending = 0;

                // disable all menu stuff
                m_title.state       = STATE_DEAD;
                m_menubar.state     = STATE_DEAD;
                m_menugame.state    = STATE_DEAD;
                m_menuscores.state  = STATE_DEAD;
                m_menuhelp.state    = STATE_DEAD;
                m_mousecursor.state = STATE_DEAD;

                for( i = 0; m_buttons[ i ]; i++ ) {
                    m_buttons[ i ]->state = STATE_DEAD;
                }

                m_menuactive  = 0;
                m_popupactive = 0;
                sys_playsound( m_sounds[ SND_ASTEXPL0 ], 0 );

                // start a new game
                m_level = 0;
                m_round = 0;
                game_new();

                m_player.stateplayer.livesleft  = PLAYER_LIVES - 1;
                m_player.stateplayer.score      = 0;
            }
        }
        else if( obj == &m_butresume ) {
            if( m_gameactive ) {
                m_menugame.state = STATE_DEAD;
                m_butnew.state   = m_butresume.state = m_butstop.state = STATE_DEAD;
                m_menuactive     = 0;
                m_popupactive    = 0;
                game_resume();
                sys_playsound( m_sounds[ SND_ASTEXPL0 ], 0 );
            }
        }
        else if( obj == &m_butstop ) {
            if( m_gameactive ) {
                m_menugame.state = STATE_DEAD;
                m_butnew.state   = m_butresume.state = m_butstop.state = STATE_DEAD;
                m_popupactive    = 0;
                game_stop();
                sys_playsound( m_sounds[ SND_ASTEXPL0 ], 0 );
            }
        }
        else if( obj == &m_butviewhs ) {
            m_menuscores.state = STATE_DEAD;
            m_butviewhs.state  = m_butreset.state = STATE_DEAD;
            m_popupactive      = 0;
            scores_view();
            sys_playsound( m_sounds[ SND_ASTEXPL0 ], 0 );
        }
        else if( obj == &m_butreset ) {
            m_menuscores.state = STATE_DEAD;
            m_butviewhs.state  = m_butreset.state = STATE_DEAD;
            m_popupactive      = 0;
            scores_reset();
            sys_playsound( m_sounds[ SND_ASTEXPL0 ], 0 );
        }
        else if( obj == &m_butviewhp ) {
            m_menuhelp.state    = STATE_DEAD;
            m_butviewhp.state   = m_butabout.state = STATE_DEAD;
            m_popupactive       = 0;
            help_view();
            sys_playsound( m_sounds[ SND_ASTEXPL0 ], 0 );
        }
        else if( obj == &m_butabout ) {
            m_menuhelp.state    = STATE_DEAD;
            m_butviewhp.state   = m_butabout.state = STATE_DEAD;
            m_popupactive       = 0;
            about_view();
            sys_playsound( m_sounds[ SND_ASTEXPL0 ], 0 );
        }
        else if( obj == &m_butback ) {
            m_menuscores2.state = STATE_DEAD;
            m_menuabout.state   = STATE_DEAD;
            m_menuhelp2.state   = STATE_DEAD;
            m_butnext.state     = STATE_DEAD;
            m_butprev.state     = STATE_DEAD;
            m_butback.state     = STATE_DEAD;
            m_marquee.state     = STATE_DEAD;
            m_entername.state   = STATE_DEAD;
            m_menuoptions.state = STATE_DEAD;
            m_butviewopt.state  = STATE_DEAD;

            m_drpshipphysicssel.state = STATE_DEAD;
            m_enternamecursor.state = STATE_DEAD;

            sys_stopmp3( m_creditstrack );

            portrait_deactivateall();

            m_menuoptions2.state    = STATE_DEAD;

            for( i = 0; i < MAX_OPTPAGES; i++ ) {
                m_butoptpages[ i ].state = STATE_PREBIRTH;
            }

            for( i = 0; i < MAX_HELPPAGES; i++ ) {
                m_helppages[ i ].state = STATE_DEAD;
            }

            for( i = 0; i < MAX_HIGHSCORES; i++ ) {
                m_scorerows[ i ].state = STATE_DEAD;
            }

            for( i = 0; m_proppagecontrols[ i ]; i++ ) {
                m_proppagecontrols[ i ]->state = STATE_PREBIRTH;
            }
            m_butsliding.state = STATE_DEAD; m_butdirectional.state = STATE_PREBIRTH;

            if( m_newgamepending ) {
                button_execute( &m_butnew, 0 );
            }
            else {
                menu_view();
            }
            sys_playsound( m_sounds[ SND_ASTEXPL0 ], 0 );
        }
        else if( obj == &m_butnext ) {
            m_helppages[ m_currentpage].state = STATE_DEAD;
            m_currentpage++;
            if( m_currentpage >= ( MAX_HELPPAGES - 1 ) ) {
                m_butnext.state = STATE_PAUSED;
                m_butnext.src.left  = 128;
                m_butnext.src.right = 192;
            }
            else {
                m_butnext.state = STATE_ALIVE;
            }
            m_butprev.state = STATE_ALIVE;
            m_helppages[ m_currentpage].state = STATE_ALIVE;
            portrait_activate();
            sys_playsound( m_sounds[ SND_ASTEXPL0 ], 0 );
        }
        else if( obj == &m_butprev ) {
            m_helppages[ m_currentpage].state = STATE_DEAD;
            m_currentpage--;
            if( m_currentpage <= 0 ) {
                m_butprev.state = STATE_PAUSED;
                m_butprev.src.left  = 128;
                m_butprev.src.right = 192;
            }
            else {
                m_butprev.state = STATE_ALIVE;
            }
            m_butnext.state = STATE_ALIVE;
            sys_playsound( m_sounds[ SND_ASTEXPL0 ], 0 );
            portrait_activate();
            m_helppages[ m_currentpage].state = STATE_ALIVE;
        }
        else if( obj == &m_butoptpages[ 0 ] ) {
            options_viewpage( 0 );
            sys_playsound( m_sounds[ SND_ASTEXPL0 ], 0 );
        }
        else if( obj == &m_butoptpages[ 1 ] ) {
            options_viewpage( 1 );
            sys_playsound( m_sounds[ SND_ASTEXPL0 ], 0 );
        }
        else if( obj == &m_butoptpages[ 2 ] ) {
            options_viewpage( 2 );
            sys_playsound( m_sounds[ SND_ASTEXPL0 ], 0 );
        }
        else if( obj == &m_butoptpages[ 3 ] ) {
            options_viewpage( 3 );
            sys_playsound( m_sounds[ SND_ASTEXPL0 ], 0 );
        }
        else if( obj == &m_sldrboxsound ) {
            m_mousecapture = obj;
            m_sldrboxsound.statesliderbox.offsetx = m_sldrboxsound.x - m_mousecursor.x;
            m_sldrboxsound.statesliderbox.offsety = m_sldrboxsound.y - m_mousecursor.y;
        }
        else if( obj == &m_sldrboxmusic ) {
            m_mousecapture = obj;
            m_sldrboxmusic.statesliderbox.offsetx = m_sldrboxmusic.x - m_mousecursor.x;
            m_sldrboxmusic.statesliderbox.offsety = m_sldrboxmusic.y - m_mousecursor.y;
        }
        else if( obj == &m_sldrboxmouse ) {
            m_mousecapture = obj;
            m_sldrboxmouse.statesliderbox.offsetx = m_sldrboxmouse.x - m_mousecursor.x;
            m_sldrboxmouse.statesliderbox.offsety = m_sldrboxmouse.y - m_mousecursor.y;
        }
        else if( obj == &m_drpshipphysics ) {
            if( m_drpshipdropped ) {
                m_drpshipdropped        = 0;
                m_lstshipphysics.state  = STATE_DEAD;
                m_butsliding.state      = STATE_DEAD;
                m_butdirectional.state  = STATE_DEAD;
            }
            else {
                m_drpshipdropped = 1;
                m_lstshipphysics.state  = STATE_ALIVE;
                m_butsliding.state      = STATE_ALIVE;
                m_butdirectional.state  = STATE_ALIVE;
            }
            sys_playsound( m_sounds[ SND_ASTEXPL0 ], 0 );
        }
        else if( obj == &m_butsliding ) {
            m_drpshipdropped        = 0;
            m_options.physicstype   = 1;

            m_drpshipphysicssel.src.left    = 128;
            m_drpshipphysicssel.src.right   = 256;

            m_lstshipphysics.state  = STATE_DEAD;
            m_butsliding.state      = STATE_DEAD;
            m_butdirectional.state  = STATE_DEAD;

            m_lblDirectional.state  = STATE_PREBIRTH;
            m_lblSliding.state      = STATE_ALIVE;

            sys_playsound( m_sounds[ SND_ASTEXPL0 ], 0 );
        }
        else if( obj == &m_butdirectional ) {
            m_drpshipdropped        = 0;
            m_options.physicstype   = 0;

            m_drpshipphysicssel.src.left    = 0;
            m_drpshipphysicssel.src.right   = 128;

            m_lstshipphysics.state  = STATE_DEAD;
            m_butsliding.state      = STATE_DEAD;
            m_butdirectional.state  = STATE_DEAD;

            m_lblDirectional.state  = STATE_ALIVE;
            m_lblSliding.state      = STATE_PREBIRTH;

            sys_playsound( m_sounds[ SND_ASTEXPL0 ], 0 );
        }
        else if( obj == &m_chkplyrthrusting ) {
            if( STATE_ALIVE == m_chkplyrthrusting.state ) {
                m_chkplyrthrusting.state       = STATE_DEAD;
                m_chkplyrthrusting.src.left    = 3 * m_chkplyrthrusting.width;
                m_chkplyrthrusting.src.right   = m_chkplyrthrusting.src.left + m_chkplyrthrusting.width;
                m_options.playerthrusting = 1;
            }
            else {
                m_chkplyrthrusting.state       = STATE_ALIVE;
                m_chkplyrthrusting.src.left    = m_chkplyrthrusting.width;
                m_chkplyrthrusting.src.right   = m_chkplyrthrusting.src.left + m_chkplyrthrusting.width;
                m_options.playerthrusting = 0;
            }
            sys_playsound( m_sounds[ SND_ASTEXPL0 ], 0 );
        }
        else if( obj == &m_chkplyrshottrails ) {
            if( STATE_ALIVE == m_chkplyrshottrails.state ) {
                m_chkplyrshottrails.state       = STATE_DEAD;
                m_chkplyrshottrails.src.left    = 3 * m_chkplyrshottrails.width;
                m_chkplyrshottrails.src.right   = m_chkplyrshottrails.src.left + m_chkplyrshottrails.width;
                m_options.playershottrails = 1;
            }
            else {
                m_chkplyrshottrails.state       = STATE_ALIVE;
                m_chkplyrshottrails.src.left    = m_chkplyrshottrails.width;
                m_chkplyrshottrails.src.right   = m_chkplyrshottrails.src.left + m_chkplyrshottrails.width;
                m_options.playershottrails = 0;
            }
            sys_playsound( m_sounds[ SND_ASTEXPL0 ], 0 );
        }
        else if( obj == &m_chkplyrshotexpl ) {
            if( STATE_ALIVE == m_chkplyrshotexpl.state ) {
                m_chkplyrshotexpl.state     = STATE_DEAD;
                m_chkplyrshotexpl.src.left  = 3 * m_chkplyrshotexpl.width;
                m_chkplyrshotexpl.src.right = m_chkplyrshotexpl.src.left + m_chkplyrshotexpl.width;
                m_options.playershotexpl    = 1;
            }
            else {
                m_chkplyrshotexpl.state     = STATE_ALIVE;
                m_chkplyrshotexpl.src.left  = m_chkplyrshotexpl.width;
                m_chkplyrshotexpl.src.right = m_chkplyrshotexpl.src.left + m_chkplyrshotexpl.width;
                m_options.playershotexpl    = 0;
            }
            sys_playsound( m_sounds[ SND_ASTEXPL0 ], 0 );
        }
        else if( obj == &m_chkplyrexpl ) {
            if( STATE_ALIVE == m_chkplyrexpl.state ) {
                m_chkplyrexpl.state     = STATE_DEAD;
                m_chkplyrexpl.src.left  = 3 * m_chkplyrexpl.width;
                m_chkplyrexpl.src.right = m_chkplyrexpl.src.left + m_chkplyrexpl.width;
                m_options.playerexpl    = 1;
            }
            else {
                m_chkplyrexpl.state     = STATE_ALIVE;
                m_chkplyrexpl.src.left  = m_chkplyrexpl.width;
                m_chkplyrexpl.src.right = m_chkplyrexpl.src.left + m_chkplyrexpl.width;
                m_options.playerexpl    = 0;
            }
            sys_playsound( m_sounds[ SND_ASTEXPL0 ], 0 );
        }
        else if( obj == &m_chknmeshottrails ) {
            if( STATE_ALIVE == m_chknmeshottrails.state ) {
                m_chknmeshottrails.state        = STATE_DEAD;
                m_chknmeshottrails.src.left     = 3 * m_chknmeshottrails.width;
                m_chknmeshottrails.src.right    = m_chknmeshottrails.src.left + m_chknmeshottrails.width;
                m_options.nmeshottrails         = 1;
            }
            else {
                m_chknmeshottrails.state        = STATE_ALIVE;
                m_chknmeshottrails.src.left     = m_chknmeshottrails.width;
                m_chknmeshottrails.src.right    = m_chknmeshottrails.src.left + m_chknmeshottrails.width;
                m_options.nmeshottrails         = 0;
            }
            sys_playsound( m_sounds[ SND_ASTEXPL0 ], 0 );
        }
        else if( obj == &m_chknmeshotexpl ) {
            if( STATE_ALIVE == m_chknmeshotexpl.state ) {
                m_chknmeshotexpl.state      = STATE_DEAD;
                m_chknmeshotexpl.src.left   = 3 * m_chknmeshotexpl.width;
                m_chknmeshotexpl.src.right  = m_chknmeshotexpl.src.left + m_chknmeshotexpl.width;
                m_options.nmeshotexpl       = 1;
            }
            else {
                m_chknmeshotexpl.state      = STATE_ALIVE;
                m_chknmeshotexpl.src.left   = m_chknmeshotexpl.width;
                m_chknmeshotexpl.src.right  = m_chknmeshotexpl.src.left + m_chknmeshotexpl.width;
                m_options.nmeshotexpl       = 0;
            }
            sys_playsound( m_sounds[ SND_ASTEXPL0 ], 0 );
        }
        else if( obj == &m_chknmeexpl ) {
            if( STATE_ALIVE == m_chknmeexpl.state ) {
                m_chknmeexpl.state      = STATE_DEAD;
                m_chknmeexpl.src.left   = 3 * m_chknmeexpl.width;
                m_chknmeexpl.src.right  = m_chknmeexpl.src.left + m_chknmeexpl.width;
                m_options.nmeexpl       = 1;
            }
            else {
                m_chknmeexpl.state      = STATE_ALIVE;
                m_chknmeexpl.src.left   = m_chknmeexpl.width;
                m_chknmeexpl.src.right  = m_chknmeexpl.src.left + m_chknmeexpl.width;
                m_options.nmeexpl       = 0;
            }
            sys_playsound( m_sounds[ SND_ASTEXPL0 ], 0 );
        }
        else {
            m_drpshipdropped        = 0;
            m_lstshipphysics.state  = STATE_DEAD;
            m_butsliding.state      = STATE_DEAD;
            m_butdirectional.state  = STATE_DEAD;

            m_popupactive       = 0;
            m_menugame.state    = STATE_DEAD;
            m_butnew.state      = m_butresume.state = m_butstop.state = STATE_DEAD;
            m_menuscores.state  = STATE_DEAD;
            m_butviewhs.state   = m_butreset.state = STATE_DEAD;
            m_menuoptions.state = STATE_DEAD;
            m_butviewopt.state  = STATE_DEAD;
            m_menuhelp.state    = STATE_DEAD;
            m_butviewhp.state   = m_butabout.state = STATE_DEAD;
        }
    }
}

static void button_draw( obj_t *obj )
{
    HRESULT hres;
    switch( obj->state ) {
        case STATE_PAUSED :
            //obj->src.left  = 128;
            //obj->src.right = 192;
            // fall thru

        case STATE_ALIVE :
            game_maptoscreen( obj );

            hres = g_backbuffer->Blt( &obj->dst
                             , obj->dds
                             , &obj->src
                             , DDBLT_WAIT | DDBLT_KEYSRC
                             , NULL );
            if( FAILED( hres ) ) {
                strcpy_s( g_gameerror, 1024, "pee pee" );
            }
            break;

        default :
            break;
    }
}

static void button_draw2( obj_t *obj )
{
    switch( obj->state ) {
        case STATE_PREBIRTH :
            break;

        case STATE_BIRTH :
        case STATE_ALIVE :
        case STATE_DYING :
        case STATE_DEAD :
            game_maptoscreen( obj );
            g_backbuffer->Blt( &obj->dst
                             , obj->dds
                             , &obj->src
                             , DDBLT_WAIT | DDBLT_KEYSRC
                             , NULL );
            break;

        default :
            break;
    }
}

static void cursor_think( void )
{
    if( STATE_ALIVE == m_mousecursor.state ) {
        if( ++m_mousecursor.statecursor.counter > MOUSE_ROTATESPEED ) {
            m_mousecursor.statecursor.counter = 0;

            if( ++m_mousecursor.statecursor.change > MOUSE_CHANGETHRESHOLD ) {
                m_mousecursor.statecursor.change = 0;
                m_mousecursor.statecursor.direction = !m_mousecursor.statecursor.direction;
            }

            m_mousecursor.statecursor.direction ? game_incframe( &m_mousecursor ) : game_decframe( &m_mousecursor );
        }
    }
}

static void cursor_move( void )
{
    m_mousecursor.x += ( (float)g_mousestate.lX * m_options.mousespeed );
    m_mousecursor.y += ( (float)g_mousestate.lY * m_options.mousespeed );
}

static void cursor_collide( void )
{
    int i, collision;

    if( m_mousecursor.x < 0.0f ) {
        m_mousecursor.x = 0;
    }
    if( m_mousecursor.x > SCREEN_WIDTH_f ) {
        m_mousecursor.x = SCREEN_WIDTH_f;
    }
    if( m_mousecursor.y < 0.0f ) {
        m_mousecursor.y = 0;
    }
    if( m_mousecursor.y > SCREEN_HEIGHT_f ) {
        m_mousecursor.y = SCREEN_HEIGHT_f;
    }

    collision = 0;

    for( i = 0; m_buttons[ i ]; i++ ) {
        if( STATE_ALIVE == m_buttons[ i ]->state ) {
            if( game_pointinbox( (int)m_mousecursor.x - (int)m_mousecursor.halfwidth
                               , (int)m_mousecursor.y - (int)m_mousecursor.halfheight
                               , (int)m_buttons[ i ]->x
                               , (int)m_buttons[ i ]->y
                               //, m_buttons[ i ]->bbwidth
                               //, m_buttons[ i ]->bbheight ) )
                               , m_buttons[ i ]->halfwidth
                               , m_buttons[ i ]->halfheight ) )
            {
                m_buttons[ i ]->src.left  = m_buttons[ i ]->width;
                m_buttons[ i ]->src.right = m_buttons[ i ]->src.left + m_buttons[ i ]->width;

                if( sys_mousebuttondown( 0 ) || sys_mousebuttondown( 1 ) ) {
                    button_execute( m_buttons[ i ], 0 );
                }
                else if( m_popupactive ){
                    button_execute( m_buttons[ i ], 1 );
                }

                collision = 1;
                if( lastfocus != m_buttons[ i ] ) {
                    lastfocus = m_buttons[ i ];
                    sys_playsound( m_sounds[ SND_CLICK ], 0 );
                }
            }
            else {
                m_buttons[ i ]->src.left  = 0;
                m_buttons[ i ]->src.right = m_buttons[ i ]->width;
            }
        }
    }
/*
    if( !collision ) {
        lastfocus = NULL;
        if( sys_mousebuttondown( 0 ) || sys_mousebuttondown( 1 ) ) {
            button_execute( m_buttons[ i ], 0 );
        }
    }
*/
    if( STATE_ALIVE == m_menuoptions2.state && !collision ) {

        collision = 0;
        for( i = 0; m_multistate[ i ]; i++ ) {
            if( STATE_PREBIRTH != m_multistate[ i ]->state ) {
                if( game_pointinbox( (int)m_mousecursor.x - (int)m_mousecursor.halfwidth
                                   , (int)m_mousecursor.y - (int)m_mousecursor.halfheight
                                   , (int)m_multistate[ i ]->x
                                   , (int)m_multistate[ i ]->y
                                   , m_multistate[ i ]->bbwidth
                                   , m_multistate[ i ]->bbheight ) )
                {
                    if( STATE_BIRTH == m_multistate[ i ]->state ) {
                        m_multistate[ i ]->src.left  = m_multistate[ i ]->width;
                        m_multistate[ i ]->src.right = m_multistate[ i ]->src.left + m_multistate[ i ]->width;
                        m_multistate[ i ]->state = STATE_ALIVE;
                    }
                    else if( STATE_DYING == m_multistate[ i ]->state ) {
                        m_multistate[ i ]->src.left  = 3 * m_multistate[ i ]->width;
                        m_multistate[ i ]->src.right = m_multistate[ i ]->src.left + m_multistate[ i ]->width;
                        m_multistate[ i ]->state = STATE_DEAD;
                    }

                    if( sys_mousebuttondown( 0 ) || sys_mousebuttondown( 1 ) ) {
                        button_execute( m_multistate[ i ], 0 );
                    }
                    else if( m_popupactive ){
                        button_execute( m_multistate[ i ], 1 );
                    }

                    collision = 1;
                    if( lastfocus != m_multistate[ i ] ) {
                        lastfocus = m_multistate[ i ];
                        sys_playsound( m_sounds[ SND_CLICK ], 0 );
                    }
                }
                else {
                    if( STATE_ALIVE == m_multistate[ i ]->state ) {
                        m_multistate[ i ]->src.left  = 0;
                        m_multistate[ i ]->src.right = m_multistate[ i ]->width;
                        m_multistate[ i ]->state = STATE_BIRTH;
                    }
                    else if( STATE_DEAD == m_multistate[ i ]->state ) {
                        m_multistate[ i ]->src.left  = 2 * m_multistate[ i ]->width;
                        m_multistate[ i ]->src.right = m_multistate[ i ]->src.left + m_multistate[ i ]->width;
                        m_multistate[ i ]->state = STATE_DYING;
                    }
                }
            }
        }

    }

    if( !collision ) {
        lastfocus = NULL;
        if( sys_mousebuttondown( 0 ) || sys_mousebuttondown( 1 ) ) {
            button_execute( NULL, 0 );
        }
    }
}

static void cursor_draw( void )
{
    if( STATE_ALIVE == m_mousecursor.state ) {
        game_maptoscreen( &m_mousecursor );

        g_backbuffer->Blt( &m_mousecursor.dst
                         , m_mousecursor.dds
                         , &m_mousecursor.src
                         , DDBLT_KEYSRC | DDBLT_WAIT
                         , NULL );
    }
}

static void starfield_init( int direction )
{
    int layer, i;
    int idx, ang, color;
    float spd;
    int layerminspds[ STARFIELD_LAYERS ] = {
        MIN_LAYER1SPEED, MIN_LAYER2SPEED, MIN_LAYER3SPEED, MIN_LAYER4SPEED
    };
    int layermaxspds[ STARFIELD_LAYERS ] = {
        MAX_LAYER1SPEED, MAX_LAYER2SPEED, MAX_LAYER3SPEED, MAX_LAYER4SPEED
    };
    int minreds[ 3 ] = {
        STARFIELD_COL1MINRED, STARFIELD_COL2MINRED, STARFIELD_COL3MINRED
    };
    int mingreens[ 3 ] = {
        STARFIELD_COL1MINGREEN, STARFIELD_COL2MINGREEN, STARFIELD_COL3MINGREEN
    };
    int minblues[ 3 ] = {
        STARFIELD_COL1MINBLUE, STARFIELD_COL2MINBLUE, STARFIELD_COL3MINBLUE
    };
    int maxreds[ 3 ] = {
        STARFIELD_COL1MAXRED, STARFIELD_COL2MAXRED, STARFIELD_COL3MAXRED
    };
    int maxgreens[ 3 ] = {
        STARFIELD_COL1MAXGREEN, STARFIELD_COL2MAXGREEN, STARFIELD_COL3MAXGREEN
    };
    int maxblues[ 3 ] = {
        STARFIELD_COL1MAXBLUE, STARFIELD_COL2MAXBLUE, STARFIELD_COL3MAXBLUE
    };

    memset( &m_starfield, 0, sizeof( clstr_t ) );
    m_starfield.state     = STATE_ALIVE;
    m_starfield.ptlecount = STARFIELD_LAYERS * STARFIELD_PTLESINLAYER;

    for( layer = 0, idx = 0; layer < STARFIELD_LAYERS; layer++ ) {
        for( i = 0; i < STARFIELD_PTLESINLAYER; i++, idx++ ) {\
            m_starfield.ptles[ idx ].state = STATE_ALIVE;

            ang = direction + ( ( rand() % ( STARFIELD_ANGDEVIATION * 2 ) ) - STARFIELD_ANGDEVIATION );

            spd = (float)( (int)rand() % ( layermaxspds[ layer ] - layerminspds[ layer ] ) + layerminspds[ layer ] ) / LAYERSPEEDDIVISOR;

            m_starfield.ptles[ idx ].x  = (float)( (int)rand() % (int)SCREEN_WIDTH  );
            m_starfield.ptles[ idx ].y  = (float)( (int)rand() % (int)SCREEN_HEIGHT );

            m_starfield.ptles[ idx ].spdx   = (float)cos( PI_OVER_180 * (float)ang );
            m_starfield.ptles[ idx ].spdy   = (float)sin( PI_OVER_180 * (float)ang );

            m_starfield.ptles[ idx ].spdx  *= spd;
            m_starfield.ptles[ idx ].spdy  *= spd;

            // we want mostly white stars
            switch( rand() % 10 ) {
                case 0 : case 1 : case 2 : case 3 :     // 80% chance of a white-ish star
                case 4 : case 5 : case 6 : case 7 :
                    color = 0;
                    break;
                case 8 :        // 10% chance of a blue-ish star
                    color = 1;
                    break;
                case 9 :        // 10% chance of a green-ish star
                    color = 2;
                    break;
            }

            m_starfield.ptles[ idx ].r  = (unsigned char)( ( rand() % ( maxreds[ color ]   - minreds[ color ]   ) ) + minreds[ color ]   );
            m_starfield.ptles[ idx ].g  = (unsigned char)( ( rand() % ( maxgreens[ color ] - mingreens[ color ] ) ) + mingreens[ color ] );
            m_starfield.ptles[ idx ].b  = (unsigned char)( ( rand() % ( maxblues[ color ]  - minblues[ color ]  ) ) + minblues[ color ]  );
        }
    }
}

static void starfield_think( void )
{
}

static void starfield_collide( void )
{
    int i;

    if( STATE_ALIVE == m_starfield.state ) {
        for( i = 0; i < m_starfield.ptlecount; i++ ) {
            if( STATE_ALIVE == m_starfield.ptles[ i ].state ) {
                if( m_starfield.ptles[ i ].x < 0.0f ) {
                    m_starfield.ptles[ i ].x = SCREEN_WIDTH_f;
                }
                if( m_starfield.ptles[ i ].x > SCREEN_WIDTH_f ) {
                    m_starfield.ptles[ i ].x = 0.0f;
                }
                if( m_starfield.ptles[ i ].y < 0.0f ) {
                    m_starfield.ptles[ i ].y = SCREEN_HEIGHT_f;
                }
                if( m_starfield.ptles[ i ].y > SCREEN_HEIGHT_f ) {
                    m_starfield.ptles[ i ].y = 0.0f;
                }
            }
        }
    }
}

static int marquee_init( void )
{
    memset( &m_marquee, 0, sizeof( marquee_t ) );

    if( !font_build( m_fonttitle, &m_marquee.lines[ 1 ].dds, &m_marquee.lines[ 1 ].length
                   , "A L D E B A R A N" ) ) {
        strcpy_s( g_gameerror, 1024, g_syserror );
        return 0;
    }

    if( !font_build( m_fontbody, &m_marquee.lines[ 2 ].dds, &m_marquee.lines[ 2 ].length
                   , "Developed by Damian 'Omen' Coventry." ) ) {
        strcpy_s( g_gameerror, 1024, g_syserror );
        return 0;
    }

    if( !font_build( m_fontbody, &m_marquee.lines[ 3 ].dds, &m_marquee.lines[ 3 ].length
                   , "Copyright (C) 2001, All rights reserved" ) ) {
        strcpy_s( g_gameerror, 1024, g_syserror );
        return 0;
    }

    if( !font_build( m_fonttitle, &m_marquee.lines[ 5 ].dds, &m_marquee.lines[ 5 ].length
                   , "Programming, Sound and Artwork:" ) ) {
        strcpy_s( g_gameerror, 1024, g_syserror );
        return 0;
    }

    if( !font_build( m_fontbody, &m_marquee.lines[ 6 ].dds, &m_marquee.lines[ 6 ].length
                   , "Damian Coventry" ) ) {
        strcpy_s( g_gameerror, 1024, g_syserror );
        return 0;
    }

    if( !font_build( m_fontbody, &m_marquee.lines[ 7 ].dds, &m_marquee.lines[ 7 ].length
                   , "web: www.omen2.50megs.com,    mail: omen2@pardise.net.nz" ) ) {
        strcpy_s( g_gameerror, 1024, g_syserror );
        return 0;
    }

    if( !font_build( m_fonttitle, &m_marquee.lines[ 9 ].dds, &m_marquee.lines[ 9 ].length
                   , "Music:" ) ) {
        strcpy_s( g_gameerror, 1024, g_syserror );
        return 0;
    }

    if( !font_build( m_fontbody, &m_marquee.lines[ 10 ].dds, &m_marquee.lines[ 10 ].length
                   , "Jason Pitt" ) ) {
        strcpy_s( g_gameerror, 1024, g_syserror );
        return 0;
    }

    if( !font_build( m_fontbody, &m_marquee.lines[ 11 ].dds, &m_marquee.lines[ 11 ].length
                   , "web: www.evilx.com,    mail: jason@evilx.com" ) ) {
        strcpy_s( g_gameerror, 1024, g_syserror );
        return 0;
    }

    if( !font_build( m_fontbody, &m_marquee.lines[ 13 ].dds, &m_marquee.lines[ 13 ].length
                   , "Robert DeVoe" ) ) {
        strcpy_s( g_gameerror, 1024, g_syserror );
        return 0;
    }

    if( !font_build( m_fontbody, &m_marquee.lines[ 14 ].dds, &m_marquee.lines[ 14 ].length
                   , "web: www.mp3.com/digitaloutcast,    mail: robbie_d51@hotmail.com" ) ) {
        strcpy_s( g_gameerror, 1024, g_syserror );
        return 0;
    }

    m_marquee.lines[ 15 ].length = -1;

    return 1;
}

/*
static int marquee_init( void )
{
    memset( &m_marquee, 0, sizeof( marquee_t ) );

    if( !font_build( m_fonttitle, &m_marquee.lines[ 0 ].dds, &m_marquee.lines[ 0 ].length
                   , "A L D E B A R A N" ) ) {
        strcpy_s( g_gameerror, 1024, g_syserror );
        return 0;
    }

    if( !font_build( m_fontbody, &m_marquee.lines[ 2 ].dds, &m_marquee.lines[ 2 ].length
                   , "Developed by Damian 'Omen' Coventry" ) ) {
        strcpy_s( g_gameerror, 1024, g_syserror );
        return 0;
    }

    if( !font_build( m_fontbody, &m_marquee.lines[ 3 ].dds, &m_marquee.lines[ 3 ].length
                   , "Copyright (C) 2001, All rights reserved" ) ) {
        strcpy_s( g_gameerror, 1024, g_syserror );
        return 0;
    }

    if( !font_build( m_fonttitle, &m_marquee.lines[ 5 ].dds, &m_marquee.lines[ 5 ].length
                   , "Programming:" ) ) {
        strcpy_s( g_gameerror, 1024, g_syserror );
        return 0;
    }

    if( !font_build( m_fontbody, &m_marquee.lines[ 6 ].dds, &m_marquee.lines[ 6 ].length
                   , "Damian Coventry" ) ) {
        strcpy_s( g_gameerror, 1024, g_syserror );
        return 0;
    }

    if( !font_build( m_fonttitle, &m_marquee.lines[ 8 ].dds, &m_marquee.lines[ 8 ].length
                   , "3D Artwork:" ) ) {
        strcpy_s( g_gameerror, 1024, g_syserror );
        return 0;
    }

    if( !font_build( m_fontbody, &m_marquee.lines[ 9 ].dds, &m_marquee.lines[ 9 ].length
                   , "Damian Coventry" ) ) {
        strcpy_s( g_gameerror, 1024, g_syserror );
        return 0;
    }

    if( !font_build( m_fonttitle, &m_marquee.lines[ 11 ].dds, &m_marquee.lines[ 11 ].length
                   , "2D Artwork:" ) ) {
        strcpy_s( g_gameerror, 1024, g_syserror );
        return 0;
    }

    if( !font_build( m_fontbody, &m_marquee.lines[ 12 ].dds, &m_marquee.lines[ 12 ].length
                   , "Damian Coventry" ) ) {
        strcpy_s( g_gameerror, 1024, g_syserror );
        return 0;
    }

    if( !font_build( m_fonttitle, &m_marquee.lines[ 14 ].dds, &m_marquee.lines[ 14 ].length
                   , "Sound Effects:" ) ) {
        strcpy_s( g_gameerror, 1024, g_syserror );
        return 0;
    }

    if( !font_build( m_fontbody, &m_marquee.lines[ 15 ].dds, &m_marquee.lines[ 15 ].length
                   , "Damian Coventry" ) ) {
        strcpy_s( g_gameerror, 1024, g_syserror );
        return 0;
    }

    if( !font_build( m_fonttitle, &m_marquee.lines[ 17 ].dds, &m_marquee.lines[ 17 ].length
                   , "Bugs:" ) ) {
        strcpy_s( g_gameerror, 1024, g_syserror );
        return 0;
    }

    if( !font_build( m_fontbody, &m_marquee.lines[ 18 ].dds, &m_marquee.lines[ 18 ].length
                   , "Damian Coventry" ) ) {
        strcpy_s( g_gameerror, 1024, g_syserror );
        return 0;
    }

    if( !font_build( m_fonttitle, &m_marquee.lines[ 20 ].dds, &m_marquee.lines[ 20 ].length
                   , "Stunts:" ) ) {
        strcpy_s( g_gameerror, 1024, g_syserror );
        return 0;
    }

    if( !font_build( m_fontbody, &m_marquee.lines[ 21 ].dds, &m_marquee.lines[ 21 ].length
                   , "Damian Coventry, Jason Norgate" ) ) {
        strcpy_s( g_gameerror, 1024, g_syserror );
        return 0;
    }

    if( !font_build( m_fonttitle, &m_marquee.lines[ 23 ].dds, &m_marquee.lines[ 23 ].length
                   , "Bad Jokes:" ) ) {
        strcpy_s( g_gameerror, 1024, g_syserror );
        return 0;
    }

    if( !font_build( m_fontbody, &m_marquee.lines[ 24 ].dds, &m_marquee.lines[ 24 ].length
                   , "Damian Coventry" ) ) {
        strcpy_s( g_gameerror, 1024, g_syserror );
        return 0;
    }

    if( !font_build( m_fonttitle, &m_marquee.lines[ 26 ].dds, &m_marquee.lines[ 26 ].length
                   , "Motion Capture:" ) ) {
        strcpy_s( g_gameerror, 1024, g_syserror );
        return 0;
    }

    if( !font_build( m_fontbody, &m_marquee.lines[ 27 ].dds, &m_marquee.lines[ 27 ].length
                   , "Jason Norgate, Richard Watson" ) ) {
        strcpy_s( g_gameerror, 1024, g_syserror );
        return 0;
    }

    if( !font_build( m_fonttitle, &m_marquee.lines[ 29 ].dds, &m_marquee.lines[ 29 ].length
                   , "Sheep Trainer:" ) ) {
        strcpy_s( g_gameerror, 1024, g_syserror );
        return 0;
    }

    if( !font_build( m_fontbody, &m_marquee.lines[ 30 ].dds, &m_marquee.lines[ 30 ].length
                   , "Richard Watson" ) ) {
        strcpy_s( g_gameerror, 1024, g_syserror );
        return 0;
    }

    if( !font_build( m_fonttitle, &m_marquee.lines[ 32 ].dds, &m_marquee.lines[ 32 ].length
                   , "Greets To:" ) ) {
        strcpy_s( g_gameerror, 1024, g_syserror );
        return 0;
    }

    if( !font_build( m_fontbody, &m_marquee.lines[ 33 ].dds, &m_marquee.lines[ 33 ].length
                   , "Xtreme Games LLC (www.xgames3d.com)" ) ) {
        strcpy_s( g_gameerror, 1024, g_syserror );
        return 0;
    }

    if( !font_build( m_fontbody, &m_marquee.lines[ 34 ].dds, &m_marquee.lines[ 34 ].length
                   , "The regulars at the Xtreme Games UBB" ) ) {
        strcpy_s( g_gameerror, 1024, g_syserror );
        return 0;
    }

    if( !font_build( m_fonttitle, &m_marquee.lines[ 36 ].dds, &m_marquee.lines[ 36 ].length
                   , "Special Thanks To:" ) ) {
        strcpy_s( g_gameerror, 1024, g_syserror );
        return 0;
    }

    if( !font_build( m_fontbody, &m_marquee.lines[ 37 ].dds, &m_marquee.lines[ 37 ].length
                   , "Andre LaMothe for his continued hard work." ) ) {
        strcpy_s( g_gameerror, 1024, g_syserror );
        return 0;
    }

    if( !font_build( m_fontbody, &m_marquee.lines[ 38 ].dds, &m_marquee.lines[ 38 ].length
                   , "The Auckland City Council for use of Aotea Square during" ) ) {
        strcpy_s( g_gameerror, 1024, g_syserror );
        return 0;
    }

    if( !font_build( m_fontbody, &m_marquee.lines[ 39 ].dds, &m_marquee.lines[ 39 ].length
                   , "the motion capture sessions." ) ) {
        strcpy_s( g_gameerror, 1024, g_syserror );
        return 0;
    }

    if( !font_build( m_fontbody, &m_marquee.lines[ 40 ].dds, &m_marquee.lines[ 40 ].length
                   , "Pizza Haven for all those 3:00am call outs." ) ) {
        strcpy_s( g_gameerror, 1024, g_syserror );
        return 0;
    }

    if( !font_build( m_fonttitle, &m_marquee.lines[ 42 ].dds, &m_marquee.lines[ 42 ].length
                   , "Special Abuse To:" ) ) {
        strcpy_s( g_gameerror, 1024, g_syserror );
        return 0;
    }

    if( !font_build( m_fontbody, &m_marquee.lines[ 43 ].dds, &m_marquee.lines[ 43 ].length
                   , "Hasbro Interactive for being greedy sons of bitches." ) ) {
        strcpy_s( g_gameerror, 1024, g_syserror );
        return 0;
    }

    if( !font_build( m_fontbody, &m_marquee.lines[ 55 ].dds, &m_marquee.lines[ 55 ].length
                   , "The story and characters depicted in this game are completely" ) ) {
        strcpy_s( g_gameerror, 1024, g_syserror );
        return 0;
    }

    if( !font_build( m_fontbody, &m_marquee.lines[ 56 ].dds, &m_marquee.lines[ 56 ].length
                   , "fictional.  Any similarities to actual persons, living or deceased," ) ) {
        strcpy_s( g_gameerror, 1024, g_syserror );
        return 0;
    }

    if( !font_build( m_fontbody, &m_marquee.lines[ 57 ].dds, &m_marquee.lines[ 57 ].length
                   , "is purely coincidental and unintentional." ) ) {
        strcpy_s( g_gameerror, 1024, g_syserror );
        return 0;
    }

    if( !font_build( m_fontbody, &m_marquee.lines[ 67 ].dds, &m_marquee.lines[ 67 ].length
                   , "No sheep were harmed during the making of this game." ) ) {
        strcpy_s( g_gameerror, 1024, g_syserror );
        return 0;
    }

    if( !font_build( m_fontbody, &m_marquee.lines[ 77 ].dds, &m_marquee.lines[ 77 ].length
                   , "Filmed in Technicolor." ) ) {
        strcpy_s( g_gameerror, 1024, g_syserror );
        return 0;
    }

    if( !font_build( m_fontbody, &m_marquee.lines[ 78 ].dds, &m_marquee.lines[ 78 ].length
                   , "MDMDXCCMCVII" ) ) {
        strcpy_s( g_gameerror, 1024, g_syserror );
        return 0;
    }

    m_marquee.state     = STATE_DEAD;
    m_marquee.offset    = SCREEN_HEIGHT;

    return 1;
}
*/
static void marquee_shutdown( void )
{
    int i;

    for( i = 0; i < MAX_MARQUEELINES; i++ ) {
        if( m_marquee.lines[ i ].dds ) {
            sys_deletesurface( m_marquee.lines[ i ].dds );
            m_marquee.lines[ i ].dds = NULL;
        }
    }
}
/*
static void marquee_move( void )
{
    if( STATE_ALIVE == m_marquee.state ) {
        if( ++m_marquee.counter >= MARQUEE_THRESHOLD ) {
            m_marquee.counter = 0;
            m_marquee.offset--;
            if( m_marquee.offset < -( MAX_MARQUEELINES * MARQUEE_LINESIZE ) ) {
                m_marquee.offset = SCREEN_HEIGHT;
            }
        }
    }
}
*/

static void marquee_draw( void )
{
    int line, yoffset;
    RECT src, dst;

    src.left = 0; src.top = 0; src.bottom = MAX_CHARDRAWSIZE;

    if( STATE_ALIVE == m_marquee.state ) {
        for( yoffset = 0, line = 0; m_marquee.lines[ line ].length != -1; line++, yoffset += 24 /*MARQUEE_LINESIZE  */) {
            if( m_marquee.lines[ line ].dds ) {
                src.right = m_marquee.lines[ line ].length;

                dst.left   = 32;//( SCREEN_WIDTH / 2 ) - ( m_marquee.lines[ line ].length / 2 );
                dst.right  = dst.left + m_marquee.lines[ line ].length;
                dst.top    = yoffset;
                dst.bottom = yoffset + MAX_CHARDRAWSIZE;

                g_backbuffer->Blt( &dst, m_marquee.lines[ line ].dds, &src
                                 , DDBLT_KEYSRC | DDBLT_WAIT, NULL );
            }
        }
    }
}

/*
static void marquee_draw( void )
{
    int line, yoffset;
    int firstline, linecount;
    RECT src, dst;

    if( STATE_ALIVE == m_marquee.state ) {

        if( m_marquee.offset >= SCREEN_HEIGHT ) {
            // don't draw anything - it's off the bottom
            // of the screen
            return;
        }
        else if( m_marquee.offset < 0 ) {
            // the topmost line has scrolled off the top
            // of the screen
            firstline = abs( m_marquee.offset / MARQUEE_LINESIZE );
            yoffset   = m_marquee.offset % MARQUEE_LINESIZE;
            linecount = ( SCREEN_HEIGHT / MARQUEE_LINESIZE ) + 1;
        }
        else {
            // the topmost line is somewhere in the screen
            firstline = 0;
            yoffset   = m_marquee.offset;
            linecount = ( ( SCREEN_HEIGHT - yoffset ) / MARQUEE_LINESIZE ) + 1;
        }

        src.left = 0; src.top = 0; src.bottom = MAX_CHARDRAWSIZE;

        for( line = firstline; line < firstline + linecount && line < MAX_MARQUEELINES; line++, yoffset += MARQUEE_LINESIZE ) {
            if( m_marquee.lines[ line ].dds ) {
                src.right = m_marquee.lines[ line ].length;

                dst.left   = ( SCREEN_WIDTH / 2 ) - ( m_marquee.lines[ line ].length / 2 );
                dst.right  = dst.left + m_marquee.lines[ line ].length;
                dst.top    = yoffset;
                dst.bottom = yoffset + MAX_CHARDRAWSIZE;

                g_backbuffer->Blt( &dst, m_marquee.lines[ line ].dds, &src
                                 , DDBLT_KEYSRC | DDBLT_WAIT, NULL );
            }
        }
    }
}
*/
static int scores_read( const char *filename )
{
    FILE *file;
    errno_t Error;
    int row;
    int i;

    if( NULL == filename ) {
        strcpy_s( g_gameerror, 1024, "scores_read: was passed a null pointer" );
        return 0;
    }

    Error = fopen_s( &file, filename, "rb" );
    if( Error != 0 ) {
        sprintf_s( g_gameerror, 1024, "scores_read: failed to open the file [%s]", filename );
        return 0;
    }

    if( sizeof( highscoretable_t ) != fread( &m_highscoretable, sizeof( char ), sizeof( highscoretable_t ), file ) ) {
        sprintf_s( g_gameerror, 1024, "scores_read: failed to read the required number of bytes from the file [%s]", filename );
        return 0;
    }

    fclose( file );

    // decrypt the file
    for( row = 0; row < MAX_HIGHSCORES; row++ ) {
        for( i = 0; i < MAX_SCORENAMELEN; i++ ) {
            m_highscoretable.rows[ row ].name[ i ] += 'A';
        }
    }

    scores_build();
    return 1;
}

static int scores_write( const char *filename )
{
    FILE *file;
    errno_t Error;
    int row;
    int i;

    if( NULL == filename ) {
        strcpy_s( g_gameerror, 1024, "scores_write: was passed a null pointer" );
        return 0;
    }

    // encrypt the file
    for( row = 0; row < MAX_HIGHSCORES; row++ ) {
        for( i = 0; i < MAX_SCORENAMELEN; i++ ) {
            m_highscoretable.rows[ row ].name[ i ] -= 'A';
        }
    }

    Error = fopen_s( &file, filename, "wb" );
    if( Error != 0 ) {
        sprintf_s( g_gameerror, 1024, "scores_write: failed to open the file [%s]", filename );
        return 0;
    }

    if( sizeof( highscoretable_t ) != fwrite( &m_highscoretable, sizeof( char ), sizeof( highscoretable_t ), file ) ) {
        sprintf_s( g_gameerror, 1024, "scores_write: failed to write the required number of bytes to the file [%s]", filename );
        return 0;
    }

    fclose( file );

    return 1;
}

static int scores_isnewscore( int score )
{
    int row;

    for( row = 0; row < MAX_HIGHSCORES && score < m_highscoretable.rows[ row ].score; row++ ) {
    }

    if( row >= MAX_HIGHSCORES ) {
        // then the score isn't good enough to
        // be entered into the table
        return 0;
    }

    sys_playsound( m_sounds[ SND_APPLAUSE ], 0 );
    return 1;
}

static int scores_insert( const char *name, int level, int score )
{
    int row;
    int i;

    if( NULL == name ) {
        strcpy_s( g_gameerror, 1024, "scores_insert: was passed a null pointer" );
        return 0;
    }

    for( row = 0; row < MAX_HIGHSCORES && score < m_highscoretable.rows[ row ].score; row++ ) {
    }

    if( row >= MAX_HIGHSCORES ) {
        // then the score isn't good enough to
        // be entered into the table
        return 0;
    }

    // move the remaining rows down the table
    for( i = MAX_HIGHSCORES - 1; i > row ; i-- ) {
        strcpy_s( m_highscoretable.rows[ i ].name, m_highscoretable.rows[ i - 1 ].name );
        m_highscoretable.rows[ i ].date  = m_highscoretable.rows[ i - 1 ].date;
        m_highscoretable.rows[ i ].level = m_highscoretable.rows[ i - 1 ].level;
        m_highscoretable.rows[ i ].score = m_highscoretable.rows[ i - 1 ].score;
    }

    // finally, insert the new row
    strncpy_s( m_highscoretable.rows[ row ].name, MAX_SCORENAMELEN, name, MAX_SCORENAMELEN );
    m_highscoretable.rows[ row ].name[ MAX_SCORENAMELEN - 1 ] = 0;
    m_highscoretable.rows[ row ].date  = time( NULL );
    m_highscoretable.rows[ row ].level = level;
    m_highscoretable.rows[ row ].score = score;

    return 1;
}

static int scores_view( void )
{
    int row;

    m_popupactive       = 0;

    m_menugame.state    = STATE_DEAD;
    m_butnew.state      = m_butresume.state = m_butstop.state = STATE_DEAD;

    m_menuscores.state  = STATE_DEAD;
    m_butviewhs.state   = m_butreset.state = STATE_DEAD;

    m_menuhelp.state    = STATE_DEAD;
    m_butabout.state    = STATE_DEAD;

    m_title.state       = STATE_DEAD;
    m_menubar.state     = STATE_DEAD;
    m_butgame.state     = m_butscores.state = m_buthelp.state = m_butoptions.state = m_butexit.state = STATE_DEAD;

    m_menuscores2.state = STATE_ALIVE;
    m_menuabout.state   = STATE_ALIVE;
    m_butback.state     = STATE_ALIVE;

    for( row = 0; row < MAX_HIGHSCORES; row++ ) {
        m_scorerows[ row ].state = STATE_ALIVE;
    }

    return 1;
}

static int scores_reset( void )
{
    int i;

    memset( &m_highscoretable, 0, sizeof( highscoretable_t ) );

    for( i = 0; i < MAX_HIGHSCORES; i++ ) {
        scores_insert( 0 == rand() % 2 ? "Damian" : "Omen"
                     , ( rand() % MAX_LEVELS ) + 1
                     , rand() % ( MAX_LEVELS * 1000 ) );
    }

    if( !m_gameactive ) {
        m_player.stateplayer.score = 0;
    }
    scores_build();
    return 1;
}

static int scores_build( void )
{
    int row;
    int offset;
    char buf[ 128 ];
    time_t temptime;
    struct tm tmstruct;
    errno_t Error;

    for( row = 0; row < MAX_HIGHSCORES; row++ ) {
        sys_clearsurface( m_scorerows[ row ].dds, 0 );

        font_draw( m_fontscores, m_scorerows[ row ].dds, m_highscoretable.rows[ row ].name, 10, 0 );

        Error = localtime_s( &tmstruct, &m_highscoretable.rows[ row ].date );
        if( Error != 0 ) {
            temptime = time( NULL );
            localtime_s( &tmstruct, &temptime );
        }

        strftime( buf, 128, "%I:%M %p %d %b %Y", &tmstruct );
        font_draw( m_fontscores, m_scorerows[ row ].dds, buf, 120, 0 );

        _itoa_s( m_highscoretable.rows[ row ].level, buf, 128, 10 );
        font_draw( m_fontscores, m_scorerows[ row ].dds, buf, 260, 0 );

        _itoa_s( m_highscoretable.rows[ row ].score, buf, 128, 10 );
        offset = 360 - font_length( m_fontscores, buf );
        font_draw( m_fontscores, m_scorerows[ row ].dds, buf, offset, 0 );

        m_scorerows[ row ].x          = SCREEN_HALFWIDTH_f;
        m_scorerows[ row ].y          = ((SCREEN_HALFHEIGHT_f - m_menuscores2.halfheight) + 80.0f) + ( 32.0f * (float)row );

        m_scorerows[ row ].width      = 384;
        m_scorerows[ row ].height     = 32;
        m_scorerows[ row ].halfwidth  = m_scorerows[ row ].width  / 2;
        m_scorerows[ row ].halfheight = m_scorerows[ row ].height / 2;

        m_scorerows[ row ].src.left   = 0;
        m_scorerows[ row ].src.right  = 384;
        m_scorerows[ row ].src.top    = 0;
        m_scorerows[ row ].src.bottom = 32;
    }

    return 1;
}

static int scores_iscorruptfile( void )
{
    int i;

    for( i = 0; i < MAX_HIGHSCORES; i++ ) {
        if( m_highscoretable.rows[ i ].score ) {
            return 0;
        }
    }

    return 1;
}

static void finishscreen_activate( void )
{
    game_pause();

    // enable the finishing screen
    m_finishscreen.state = STATE_ALIVE;

    // the game is over.
    m_gameactive = 0;
    m_menuactive = 0;
    m_hudstate = STATE_DEAD;

    sys_playsound( m_sounds[ SND_APPLAUSE ], 0 );
    sys_stopmp3( m_menutrack );
    sys_playmp3( m_creditstrack );
}

static void finishscreen_draw( void )
{
    if( STATE_ALIVE == m_finishscreen.state ) {
        g_backbuffer->Blt( &m_finishscreen.dst
                         , m_finishscreen.dds
                         , &m_finishscreen.src
                         , DDBLT_WAIT
                         , NULL );
    }
}

static void leveltitle_think( void )
{
    if( STATE_ALIVE == m_leveltitles[ m_level ].state ) {
        if( ++m_leveltitles[ m_level ].stateasteroid.framecounter > MAX_FRAMETHRESHOLD ) {
            m_leveltitles[ m_level ].stateasteroid.framecounter = 0;
            m_leveltitles[ m_level ].state = STATE_DEAD;
            m_roundtitles[ m_round ].stateasteroid.framecounter = 0;
            m_roundtitles[ m_round ].state = STATE_DEAD;
            game_start();
        }
    }
}

static void leveltitle_render( void )
{
    if( STATE_ALIVE == m_leveltitles[ m_level ].state ) {
        game_maptoscreen( &m_leveltitles[ m_level ] );
        g_backbuffer->Blt( &m_leveltitles[ m_level ].dst
                         , m_leveltitles[ m_level ].dds
                         , &m_leveltitles[ m_level ].src
                         , DDBLT_WAIT | DDBLT_KEYSRC
                         , NULL );
        game_maptoscreen( &m_roundtitles[ m_round ] );
        g_backbuffer->Blt( &m_roundtitles[ m_round ].dst
                         , m_roundtitles[ m_round ].dds
                         , &m_roundtitles[ m_round ].src
                         , DDBLT_WAIT | DDBLT_KEYSRC
                         , NULL );
    }
}

static void gameover_init( void )
{
    game_pause();

    m_currentpickup = NULL;

    m_menuactive = 0;
    m_gameactive = 0;
    m_hudstate   = STATE_DEAD;

    m_gameover.stateasteroid.framecounter = 0;
    m_gameover.state = STATE_ALIVE;
}

static void gameover_think( void )
{
    if( STATE_ALIVE == m_gameover.state ) {
        if( ++m_gameover.stateasteroid.framecounter > MAX_GAMEOVERTHRESHOLD ) {
            m_gameover.stateasteroid.framecounter = 0;
            m_gameover.state = STATE_DEAD;
            menu_view();
            entername_init();
        }
    }
}

static void gameover_draw( void )
{
    if( STATE_ALIVE == m_gameover.state ) {
        game_maptoscreen( &m_gameover );
        g_backbuffer->Blt( &m_gameover.dst
                         , m_gameover.dds
                         , &m_gameover.src
                         , DDBLT_WAIT | DDBLT_KEYSRC
                         , NULL );
    }
}

static void stats_think( void )
{
    switch( m_stats.state ) {
        case STATE_ALIVE :
            if( ++m_stats.stateasteroid.framecounter > STATS_THRESHOLD ) {
                m_stats.stateasteroid.framecounter = 0;
                m_stats.state = STATE_DEAD;

                if( m_player.stateplayer.livesleft < 0 ) {
                    gameover_init();
                }
                else {
#if DEMO_VERSION
                    if( m_level == 1 && m_round == 0 ) {
                        m_level = 6;
                        m_round = 0;
                        game_new();
                    }
                    else if( m_level == 7 && m_round == 0 ) {
                        game_pause();
                        // the game is over.
                        m_gameactive = 0;
                        m_hudstate = STATE_DEAD;
                        menu_view();
                    }
                    else {
                        game_new();
                    }
#else
                    if( m_level >= MAX_LEVELS ) {
                        // then the player has beaten the game
                        finishscreen_activate();
                        // the counter is on 10, put it back to 9
                        m_level--;
                    }
                    else {
                        game_new();
                    }
#endif
                }
            }
            break;

        default :
            break;
    }
}

static void stats_draw(void)
{
    char buf[128];

    if (STATE_ALIVE == m_stats.state) {
        game_maptoscreen(&m_stats);
        g_backbuffer->Blt(&m_stats.dst
            , m_stats.dds
            , &m_stats.src
            , DDBLT_WAIT
            , NULL);

        int x = (static_cast<int>(m_stats.x) - m_stats.halfwidth) + 20;
        int y = (static_cast<int>(m_stats.y) - m_stats.halfheight) + 50;

        font_draw(m_fontscores, g_backbuffer, "Shots Fired:", x, y); y += 20;
        font_draw(m_fontscores, g_backbuffer, "Shots Hit:", x, y); y += 20;
        font_draw(m_fontscores, g_backbuffer, "Shots Missed", x, y); y += 20;
        font_draw(m_fontscores, g_backbuffer, "Accuracy:", x, y); y += 20;
        font_draw(m_fontscores, g_backbuffer, "Time Bonus:", x, y); y += 20;
        font_draw(m_fontscores, g_backbuffer, "Accuracy Bonus:", x, y);

        x = (m_stats.dst.right - 20);
        y = (static_cast<int>(m_stats.y) - m_stats.halfheight) + 50;

        _itoa_s(m_statsshotsfired, buf, 128, 10);
        font_draw(m_fontscores, g_backbuffer, buf, x - font_length(m_fontscores, buf), y); y += 20;

        _itoa_s(m_statsshotshit, buf, 128, 10);
        font_draw(m_fontscores, g_backbuffer, buf, x - font_length(m_fontscores, buf), y); y += 20;

        _itoa_s(m_statsshotsmissed, buf, 128, 10);
        font_draw(m_fontscores, g_backbuffer, buf, x - font_length(m_fontscores, buf), y); y += 20;

        _itoa_s(m_accuracy, buf, 128, 10);
        font_draw(m_fontscores, g_backbuffer, buf, x - font_length(m_fontscores, buf), y);
        font_draw(m_fontscores, g_backbuffer, "%", x, y); y += 20;

        _itoa_s(m_timebonus, buf, 128, 10);
        font_draw(m_fontscores, g_backbuffer, buf, x - font_length(m_fontscores, buf), y); y += 20;

        _itoa_s(m_accuracybonus, buf, 128, 10);
        font_draw(m_fontscores, g_backbuffer, buf, x - font_length(m_fontscores, buf), y);
    }
}

static int entername_init( void )
{
    if( scores_iscorruptfile() ) {
        scores_reset();
    }

    if( scores_isnewscore( m_player.stateplayer.score ) ) {
        button_execute( &m_butviewhs, 0 );
        m_entername.state       = STATE_ALIVE;
        m_enternamecursor.state = STATE_ALIVE;
        m_butback.state         = STATE_PAUSED;
        m_butback.src.left  = 128;
        m_butback.src.right = 192;
        m_name[ 0 ] = 0;
        return 1;
    }

    m_entername.state       = STATE_DEAD;
    m_enternamecursor.state = STATE_DEAD;
    return 0;
}

static int entername_think( void )
{
    int len;

    if( STATE_ALIVE == m_entername.state ) {

        // make the cursor 'blink'
        if( ++m_enternamecursor.stateasteroid.framecounter > MAX_BLINKTHRESHOLD ) {
            m_enternamecursor.stateasteroid.framecounter = 0;
            m_enternamecursor.state = ( STATE_ALIVE == m_enternamecursor.state ? STATE_PAUSED : STATE_ALIVE );
        }

        if( sys_getalphanumerickey( m_name, MAX_SCORENAMELEN ) ) {
            if( sys_keydown( DIK_RETURN ) ) {
                sys_playsound( m_sounds[ SND_ASTEXPL1 ], 0 );
            }
            else {
                sys_playsound( m_sounds[ SND_CLICK ], 0 );
            }
        }

        // move the cursor along with the text
        m_enternamecursor.x = (float)( (int)260 + (int)font_length( m_fontscores, m_name ) ) + 4.0f;
        m_enternamecursor.y = 440.0f;

        len = static_cast<int>(strlen( m_name ));

        if( len ) {
            if( sys_keydown( DIK_RETURN ) ) {
                _strlwr_s( m_name, MAX_SCORENAMELEN );
                if( m_name[ 0 ] >= 'a' && m_name[ 0 ] <= 'z' ) {
                    m_name[ 0 ] -= ( 'a' - 'A' );
                }

                scores_insert( m_name, m_level + 1, m_player.stateplayer.score );
                scores_build();

                m_entername.state       = STATE_DEAD;
                m_enternamecursor.state = STATE_DEAD;
                m_butback.state         = STATE_ALIVE;
                m_player.stateplayer.score = 0;
            }
            else if( sys_keydown( DIK_BACK ) ) {
                m_name[ strlen( m_name ) - 1 ] = 0;
            }
        }
        else {
            m_butback.state = STATE_PAUSED;
            m_butback.src.left  = 128;
            m_butback.src.right = 192;
        }
    }
    return 1;
}

static int entername_draw( void )
{
    if( STATE_ALIVE == m_entername.state ) {
        // the dialog
        game_maptoscreen( &m_entername );
        g_backbuffer->Blt( &m_entername.dst
                         , m_entername.dds
                         , &m_entername.src
                         , DDBLT_WAIT
                         , NULL );
        font_draw( m_fontscores, g_backbuffer, m_name, 260, 432 );

        // draw the cursor only when status is alive
        if( STATE_ALIVE == m_enternamecursor.state ) {
            game_maptoscreen( &m_enternamecursor );
            g_backbuffer->Blt( &m_enternamecursor.dst
                             , m_enternamecursor.dds
                             , &m_enternamecursor.src
                             , DDBLT_WAIT
                             , NULL );
        }
    }

    return 1;
}

static void debris_spawn( obj_t *src, int type )
{
    int i, j;

    switch( type ) {
        case 0 :
            // try to find a free one
            for( i = 0; i < MAX_SMALLESTDEBRIS; i++ ) {
                if( STATE_ALIVE != m_smallestplyr[ 0 ][ i ].state ) {
                    break;
                }
            }
            if( i >= MAX_SMALLESTDEBRIS ) {
                i = 0;
            }
            for( j = 0; j < MAX_SMALLESTTYPES; j++ ) {
                m_smallestplyr[ j ][ i ].state   = STATE_ALIVE;
                m_smallestplyr[ j ][ i ].x       = src->x;
                m_smallestplyr[ j ][ i ].y       = src->y;

                m_smallestplyr[ j ][ i ].spd = (float)game_rand( 20, 200 ) / 100.0f;

                m_smallestplyr[ j ][ i ].direction = src->direction + game_rand( -30, 30 );
                m_smallestplyr[ j ][ i ].spdx = m_smallestplyr[ j ][ i ].spd * (float)cos( PI_OVER_180 * m_smallestplyr[ j ][ i ].direction );
                m_smallestplyr[ j ][ i ].spdy = m_smallestplyr[ j ][ i ].spd * (float)sin( PI_OVER_180 * m_smallestplyr[ j ][ i ].direction );

                m_smallestplyr[ j ][ i ].stateasteroid.framecounter   = 0;
                m_smallestplyr[ j ][ i ].stateasteroid.framethreshold = ( rand() % MAX_ASTFRAMEROT ) + MIN_ASTFRAMEROT;

                m_smallestplyr[ j ][ i ].stateasteroid.counter   = 0;
                m_smallestplyr[ j ][ i ].stateasteroid.threshold = 10 * m_smallestplyr[ j ][ i ].stateasteroid.framethreshold;
            }
            break;

        case 1 :
            // try to find a free one
            for( i = 0; i < MAX_SMALLESTDEBRIS; i++ ) {
                if( STATE_ALIVE != m_smallestast0[ 0 ][ i ].state ) {
                    break;
                }
            }
            if( i >= MAX_SMALLESTDEBRIS ) {
                i = 0;
            }
            for( j = 0; j < MAX_SMALLESTTYPES; j++ ) {
                m_smallestast0[ j ][ i ].state   = STATE_ALIVE;
                m_smallestast0[ j ][ i ].x       = src->x;
                m_smallestast0[ j ][ i ].y       = src->y;

                m_smallestast0[ j ][ i ].spd = (float)game_rand( 20, 200 ) / 100.0f;

                m_smallestast0[ j ][ i ].direction = src->direction + game_rand( -30, 30 );
                m_smallestast0[ j ][ i ].spdx = m_smallestast0[ j ][ i ].spd * (float)cos( PI_OVER_180 * m_smallestast0[ j ][ i ].direction );
                m_smallestast0[ j ][ i ].spdy = m_smallestast0[ j ][ i ].spd * (float)sin( PI_OVER_180 * m_smallestast0[ j ][ i ].direction );

                m_smallestast0[ j ][ i ].stateasteroid.framecounter   = 0;
                m_smallestast0[ j ][ i ].stateasteroid.framethreshold = ( rand() % MAX_ASTFRAMEROT ) + MIN_ASTFRAMEROT;

                m_smallestast0[ j ][ i ].stateasteroid.counter   = 0;
                m_smallestast0[ j ][ i ].stateasteroid.threshold = 10 * m_smallestast0[ j ][ i ].stateasteroid.framethreshold;
            }
            break;

        case 2 :
            // try to find a free one
            for( i = 0; i < MAX_SMALLESTDEBRIS; i++ ) {
                if( STATE_ALIVE != m_smallestast1[ 0 ][ i ].state ) {
                    break;
                }
            }
            if( i >= MAX_SMALLESTDEBRIS ) {
                i = 0;
            }
            for( j = 0; j < MAX_SMALLESTTYPES; j++ ) {
                m_smallestast1[ j ][ i ].state   = STATE_ALIVE;
                m_smallestast1[ j ][ i ].x       = src->x;
                m_smallestast1[ j ][ i ].y       = src->y;

                m_smallestast1[ j ][ i ].spd = (float)game_rand( 20, 200 ) / 100.0f;

                m_smallestast1[ j ][ i ].direction = src->direction + game_rand( -30, 30 );
                m_smallestast1[ j ][ i ].spdx = m_smallestast1[ j ][ i ].spd * (float)cos( PI_OVER_180 * m_smallestast1[ j ][ i ].direction );
                m_smallestast1[ j ][ i ].spdy = m_smallestast1[ j ][ i ].spd * (float)sin( PI_OVER_180 * m_smallestast1[ j ][ i ].direction );

                m_smallestast1[ j ][ i ].stateasteroid.framecounter   = 0;
                m_smallestast1[ j ][ i ].stateasteroid.framethreshold = ( rand() % MAX_ASTFRAMEROT ) + MIN_ASTFRAMEROT;

                m_smallestast1[ j ][ i ].stateasteroid.counter   = 0;
                m_smallestast1[ j ][ i ].stateasteroid.threshold = 10 * m_smallestast1[ j ][ i ].stateasteroid.framethreshold;
            }
            break;

        case 3 :
            // try to find a free one
            for( i = 0; i < MAX_SMALLESTDEBRIS; i++ ) {
                if( STATE_ALIVE != m_smallestnme[ 0 ][ i ].state ) {
                    break;
                }
            }
            if( i >= MAX_SMALLESTDEBRIS ) {
                i = 0;
            }
            for( j = 0; j < MAX_SMALLESTTYPES; j++ ) {
                m_smallestnme[ j ][ i ].state   = STATE_ALIVE;
                m_smallestnme[ j ][ i ].x       = src->x;
                m_smallestnme[ j ][ i ].y       = src->y;

                m_smallestnme[ j ][ i ].spd = (float)game_rand( 20, 200 ) / 100.0f;

                m_smallestnme[ j ][ i ].direction = src->direction + game_rand( -30, 30 );
                m_smallestnme[ j ][ i ].spdx = m_smallestnme[ j ][ i ].spd * (float)cos( PI_OVER_180 * m_smallestnme[ j ][ i ].direction );
                m_smallestnme[ j ][ i ].spdy = m_smallestnme[ j ][ i ].spd * (float)sin( PI_OVER_180 * m_smallestnme[ j ][ i ].direction );

                m_smallestnme[ j ][ i ].stateasteroid.framecounter   = 0;
                m_smallestnme[ j ][ i ].stateasteroid.framethreshold = ( rand() % MAX_ASTFRAMEROT ) + MIN_ASTFRAMEROT;

                m_smallestnme[ j ][ i ].stateasteroid.counter   = 0;
                m_smallestnme[ j ][ i ].stateasteroid.threshold = 10 * m_smallestnme[ j ][ i ].stateasteroid.framethreshold;
            }
            break;

    }
}

static void debris_think( obj_t *obj )
{
    if( obj->state == STATE_ALIVE ) {
        if( ++obj->stateasteroid.framecounter >= obj->stateasteroid.framethreshold ) {
            obj->stateasteroid.framecounter = 0;
            game_incframe( obj );
        }
        if( ++obj->stateasteroid.counter >= obj->stateasteroid.threshold ) {
            obj->state = STATE_DEAD;;
        }
    }
}

// added in 1.10
static void reflect_headingup( obj_t *obj )
{
    sys_playsound( m_sounds[ SND_BOUNCE ], 0 );
    if( obj->spd < 0 ) {
        obj->direction += 180;
        if( obj->direction >= 360 ) {
            obj->direction -= 360;
        }

        if( obj->direction >= 180 && obj->direction <= 270 ) {
            obj->direction = 180 - ( obj->direction - 180 );
        }
        else if( obj->direction > 270 && obj->direction <= 360 ) {
            obj->direction = 360 - obj->direction;
        }

        obj->direction += 180;
        if( obj->direction >= 360 ) {
            obj->direction -= 360;
        }
    }
    else {
        if( obj->direction >= 180 && obj->direction <= 270 ) {
            obj->direction = 180 - ( obj->direction - 180 );
        }
        else if( obj->direction > 270 && obj->direction <= 360 ) {
            obj->direction = 360 - obj->direction;
        }
    }
}

static void reflect_headingdown( obj_t *obj )
{
    sys_playsound( m_sounds[ SND_BOUNCE ], 0 );
    if( obj->spd < 0 ) {
        obj->direction += 180;
        if( obj->direction >= 360 ) {
            obj->direction -= 360;
        }

        if( obj->direction >= 0 && obj->direction <= 90 ) {
            obj->direction = 360 - obj->direction;
        }
        else if( obj->direction > 90 && obj->direction <= 180 ) {
            obj->direction = 270 - ( obj->direction - 90 );
        }

        obj->direction += 180;
        if( obj->direction >= 360 ) {
            obj->direction -= 360;
        }
    }
    else {
        if( obj->direction >= 0 && obj->direction <= 90 ) {
            obj->direction = 360 - obj->direction;
        }
        else if( obj->direction > 90 && obj->direction <= 180 ) {
            obj->direction = 270 - ( obj->direction - 90 );
        }
    }
}

static void reflect_headingleft( obj_t *obj )
{
    sys_playsound( m_sounds[ SND_BOUNCE ], 0 );
    if( obj->spd < 0 ) {
        obj->direction += 180;
        if( obj->direction >= 360 ) {
            obj->direction -= 360;
        }

        if( obj->direction >= 90 && obj->direction <= 180 ) {
            obj->direction = 90 - ( obj->direction - 90 );
        }
        else if( obj->direction > 180 && obj->direction <= 270 ) {
            obj->direction = 270 + ( 270 - obj->direction );
        }

        obj->direction += 180;
        if( obj->direction >= 360 ) {
            obj->direction -= 360;
        }
    }
    else {
        if( obj->direction >= 90 && obj->direction <= 180 ) {
            obj->direction = 90 - ( obj->direction - 90 );
        }
        else if( obj->direction > 180 && obj->direction <= 270 ) {
            obj->direction = 270 + ( 270 - obj->direction );
        }
    }
}

static void reflect_headingright( obj_t *obj )
{
    sys_playsound( m_sounds[ SND_BOUNCE ], 0 );
    if( obj->spd < 0 ) {
        obj->direction += 180;
        if( obj->direction >= 360 ) {
            obj->direction -= 360;
        }

        if( obj->direction >= 0 && obj->direction <= 90 ) {
            obj->direction = 180 - obj->direction;
        }
        else if( obj->direction > 270 && obj->direction <= 360 ) {
            obj->direction = 270 - ( obj->direction - 270 );
        }

        obj->direction += 180;
        if( obj->direction >= 360 ) {
            obj->direction -= 360;
        }
    }
    else {
        if( obj->direction >= 0 && obj->direction <= 90 ) {
            obj->direction = 180 - obj->direction;
        }
        else if( obj->direction > 270 && obj->direction <= 360 ) {
            obj->direction = 270 - ( obj->direction - 270 );
        }
    }
}

static int game_rand( int min, int max )
{
    return (int)( ( rand() % ( max - min ) ) + min );
}

static int game_getangle( float x1, float y1, float x2, float y2 )
{
    int angle;
    float ang;
    float hyp;
    float len1, len2;

    len1 = (float)fabs( x2 - x1 );
    len2 = (float)fabs( y2 - y1 );

    hyp = (float)sqrt( ( len1 * len1 ) + ( len2 * len2 ) );
    ang = (float)asin( (float)( len2 / hyp ) );

    angle = (int)( (float)ang * _180_OVER_PI );

    if( y1 > y2 ) {
        if( x1 > x2 ) {
            angle = 180 + angle;
        }
        else {
            angle = 360 - angle;
        }
    }
    else {
        if( x1 > x2 ) {
            angle = 180 - angle;
        }
        else {
            // don't touch
        }
    }

    return angle;
}

static int game_anglediff( int angle1, int angle2 )
{
    int sum;

    // calc the smallest diff between the two angles
    if( angle1 > angle2 ) {
        sum = ( 360 - angle1 ) + angle2;
        if( sum > 180 ) {
            sum = abs( angle1 - angle2 );

            // flag this negative to indicate that the caller
            // should turn counter clockwise to get to this
            // angle the fastest
            sum = -sum;
        }
    }
    else {
        sum = ( 360 - angle2 ) + angle1;
        if( sum > 180 ) {
            sum = abs( angle1 - angle2 );
        }
        else {
            // see previous note
            sum = -sum;
        }
    }

    return sum;
}

static void reflect_movingobj( obj_t *obj )
{
    float momentum1;
    float momentum2;
    float x, y;
    int damage;

    if( !m_player.stateplayer.cfcounteractive ) {
        m_player.stateplayer.cfcounteractive = 1;

        if( m_player.spd < 0 ) {
            x = m_player.x - ( m_player.halfwidth  * (float)cos( PI_OVER_180 * m_player.direction ) );
            y = m_player.y - ( m_player.halfheight * (float)sin( PI_OVER_180 * m_player.direction ) );
            shotrichochet_setactive( m_plyrcollide
                                   , m_player.direction - 180, x, y
                                   , PLYRCOLLIDE_MINRED,  PLYRCOLLIDE_MAXRED
                                   , PLYRCOLLIDE_MAXRED,  PLYRCOLLIDE_MAXGREEN
                                   , PLYRCOLLIDE_MINBLUE, PLYRCOLLIDE_MAXBLUE
                                   , PLYRCOLLIDE_DESTHUE, PLYRCOLLIDE_INC );
        }
        else {
            x = m_player.x + ( m_player.halfwidth  * (float)cos( PI_OVER_180 * m_player.direction ) );
            y = m_player.y + ( m_player.halfheight * (float)sin( PI_OVER_180 * m_player.direction ) );
            shotrichochet_setactive( m_plyrcollide
                                   , m_player.direction, x, y
                                   , PLYRCOLLIDE_MINRED,  PLYRCOLLIDE_MAXRED
                                   , PLYRCOLLIDE_MAXRED,  PLYRCOLLIDE_MAXGREEN
                                   , PLYRCOLLIDE_MINBLUE, PLYRCOLLIDE_MAXBLUE
                                   , PLYRCOLLIDE_DESTHUE, PLYRCOLLIDE_INC );
        }

        sys_playsound( m_sounds[ SND_BOUNCE ], 0 );

        momentum1 = (float)fabs( obj->spd ) * obj->mass;
        momentum2 = (float)fabs( m_player.spd ) * m_player.mass;

        damage = ( ( SHIELDDMG_MULTIPLIER * ( m_level + 1 ) ) + (int)momentum1 + (int)momentum2 );
        sprintf_s( m_risingscore.staterisingscore.string, 16, "%d%%", damage );
        risingscore_init( &m_risingscore, 0, x, y );

        m_player.stateplayer.shieldhealth -= damage;
        if( m_player.stateplayer.shieldhealth <= 0 ) {
            m_player.stateplayer.shieldhealth = 0;
            m_player.stateplayer.shield       = 0;
            sys_playsound( m_sounds[ SND_PLYRSHLDDN ], 0 );
        }

        // if the obj collided with has more momentum than the player...
        if( momentum1 > momentum2 ) {
            m_player.direction = ( obj->direction + game_rand( -COLLISION_ANGLENOISE, COLLISION_ANGLENOISE ) );
            if( m_player.direction < 0 ) {
                m_player.direction += 360;
            }
            else if( m_player.direction >= 360 ) {
                m_player.direction -= 360;
            }

            m_player.spd = obj->spd + ( (float)game_rand( COLLISION_SPEEDNOSEMIN, COLLISION_SPEEDNOSEMAX ) / 100.0f );
            if( m_player.spd < 0.5f ) {
                m_player.spd = 0.5f;
            }
            else if( m_player.spd > PLAYER_MAXSPEED ) {
                m_player.spd = PLAYER_MAXSPEED;
            }

            if( m_player.spd < 0 ) {
                m_player.direction += 180;
                if( m_player.direction >= 360 ) {
                    m_player.direction -= 360;
                }
            }

            obj->direction += ( 180 + game_rand( -COLLISION_ANGLENOISE, COLLISION_ANGLENOISE ) );
            if( obj->direction >= 360 ) {
                obj->direction -= 360;
            }
            obj->spd /= 2.0f;
            if( obj->spd < 0.5f ) {
                obj->spd = 0.5f;
            }
        }
        // else the player has more momentum than the obj collided with...
        else {
            obj->direction = ( m_player.direction + game_rand( -COLLISION_ANGLENOISE, COLLISION_ANGLENOISE ) );
            if( obj->direction >= 360 ) {
                obj->direction -= 360;
            }
            else if( obj->direction < 0 ) {
                obj->direction += 360;
            }

            obj->spd = obj->spd + ( (float)game_rand( COLLISION_SPEEDNOSEMIN, COLLISION_SPEEDNOSEMAX ) / 100.0f );
            if( obj->spd < 0.5f ) {
                obj->spd = 0.5f;
            }
            else if( obj->spd > PLAYER_MAXSPEED ) {
                obj->spd = PLAYER_MAXSPEED;
            }

            if( m_player.spd < 0 ) {
                obj->direction += 180;
                if( obj->direction >= 360 ) {
                    obj->direction -= 360;
                }

                m_player.direction += ( 180 + game_rand( -COLLISION_ANGLENOISE, COLLISION_ANGLENOISE ) );
                if( m_player.direction >= 360 ) {
                    m_player.direction -= 360;
                }
                else if( m_player.direction < 0 ) {
                    m_player.direction += 360;
                }

                m_player.spd /= 2.0f;
                if( m_player.spd > -0.5f ) {
                    m_player.spd = -0.5f;
                }
            }
            else {
                m_player.direction += ( 180 + game_rand( -COLLISION_ANGLENOISE, COLLISION_ANGLENOISE ) );
                if( m_player.direction >= 360 ) {
                    m_player.direction -= 360;
                }
                else if( m_player.direction < 0 ) {
                    m_player.direction += 360;
                }

                m_player.spd /= 2.0f;
                if( m_player.spd < 0.5f ) {
                    m_player.spd = 0.5f;
                }
            }
        }

        reflect_unjam( obj );

        obj->spdx = obj->spd * (float)cos( PI_OVER_180 * obj->direction );
        obj->spdy = obj->spd * (float)sin( PI_OVER_180 * obj->direction );
    }
}

static void reflect_unjam( obj_t *obj )
{
    state_t stateplayer;
    state_t stateobject;

    stateplayer = m_player.state;
    stateobject = obj->state;

    m_player.spdx = m_player.spd * (float)cos( PI_OVER_180 * m_player.direction );
    m_player.spdy = m_player.spd * (float)sin( PI_OVER_180 * m_player.direction );

    while( game_objcollide( &m_player, obj ) ) {
        m_player.x -= m_player.spdx;
        m_player.y -= m_player.spdy;
    }

    m_player.state = stateplayer;
    obj->state = stateobject;
}

static void risingscore_init( obj_t *obj, int largefont, float x, float y )
{
    obj->x      = x;
    obj->y      = y;
    obj->state  = STATE_ALIVE;

    obj->staterisingscore.counter   = 0;
    obj->staterisingscore.largefont = largefont;
}

static void risingscore_think( obj_t *obj )
{
    if( STATE_ALIVE == obj->state ) {
        if( ++obj->staterisingscore.counter > RISINGSCORE_THRESHOLD ) {
            obj->staterisingscore.counter = 0;
            obj->state = STATE_DEAD;
        }
    }
}

static void risingscore_move( obj_t *obj )
{
    if( STATE_ALIVE == obj->state ) {
        obj->y += RISINGSCORE_SPEED;
    }
}

static void nme_think( obj_t *obj )
{
    int anglediff;
    int angle;
    int piece, i;
    int shotfired;

    switch( obj->state ) {
        case STATE_DYING :
            obj->state = STATE_DEAD;
            break;

        case STATE_NMESTARTWAIT :
            if( ++obj->statenme.counter > obj->statenme.threshold ) {
                obj->statenme.counter = 0;
                obj->state = STATE_ALIVE;
            }
            break;

        case STATE_NMEWPWAIT :
            if( ++obj->statenme.cooldown >= m_nmecooldown ) {
                obj->statenme.cooldown = m_nmecooldown;
            }

            if( ++obj->statenme.waypoints[ obj->statenme.currentwaypoint ].counter > NME_WPWAITTHRESHOLD ) {
                obj->statenme.waypoints[ obj->statenme.currentwaypoint ].counter = 0;
                obj->state = STATE_ALIVE;

                /*if( ++obj->statenme.currentwaypoint >= obj->statenme.waypointcount ) {
                    obj->statenme.currentwaypoint = 0;
                    // if this is not a bonus round then...
                    if( ISBONUSROUND() ) {
                        obj->state = STATE_DEAD;
                    }
                    else {
                        obj->state = STATE_NMEWAITING;
                    }
                }
                obj->statenme.statewaypoint = STATE_NMETURNING;*/
                player_lookupframe( obj );
                break;
            }

            // turn towards the player
            angle = game_getangle( obj->x, obj->y, m_player.x, m_player.y );

            anglediff = game_anglediff( angle, obj->direction );

            if( anglediff >= -NME_TURNSPEED && anglediff <= NME_TURNSPEED ) {
                // the we're facing the player
                obj->direction = angle;

                // shoot at the player
                if( obj->statenme.canshoot &&
                    obj->x > 0 && obj->x < SCREEN_WIDTH_f &&
                    obj->y > 0 && obj->y < SCREEN_HEIGHT_f ) {

                    if( m_player.state == STATE_ALIVE ) {
                        nme_fireshot( obj );
                    }
                }
            }
            else if( anglediff < 0 ) {
                // turn counter clockwise
                obj->direction += NME_TURNSPEED;
                if( obj->direction >= 360 ) {
                    obj->direction -= 360;
                }
            }
            else {
                // turn clockwise
                obj->direction -= NME_TURNSPEED;
                if( obj->direction < 0 ) {
                    obj->direction += 360;
                }
            }

            obj->spd -= NME_ACCEL;
            if( obj->spd < 0.0f ) {
                obj->spd = 0.0f;
            }

            player_lookupframe( obj );
            break;

        case STATE_ALIVE :
            if( ++obj->statenme.cooldown >= m_nmecooldown ) {
                obj->statenme.cooldown = m_nmecooldown;
            }

            switch( obj->statenme.statewaypoint ) {
                case STATE_NMETURNING :
                    angle = game_getangle( obj->x, obj->y
                                         , obj->statenme.waypoints[ obj->statenme.currentwaypoint ].x
                                         , obj->statenme.waypoints[ obj->statenme.currentwaypoint ].y );

                    anglediff = game_anglediff( angle, obj->direction );

                    if( anglediff >= -NME_TURNSPEED && anglediff <= NME_TURNSPEED ) {
                        obj->statenme.statewaypoint = STATE_NMEMOVING;
                        obj->direction = angle;
                    }
                    else if( anglediff < 0 ) {
                        // turn counter clockwise
                        obj->direction += NME_TURNSPEED;
                        if( obj->direction >= 360 ) {
                            obj->direction -= 360;
                        }
                    }
                    else {
                        // turn clockwise
                        obj->direction -= NME_TURNSPEED;
                        if( obj->direction < 0 ) {
                            obj->direction += 360;
                        }
                    }

                    obj->spd -= NME_ACCEL;
                    if( obj->spd < 0.0f ) {
                        obj->spd = 0.0f;
                    }
                    break;

                case STATE_NMEMOVING :
                    obj->spd += NME_ACCEL;
                    if( obj->spd > m_nmemaxspeed ) {
                        obj->spd = m_nmemaxspeed;
                    }
                    break;
            }

            // check to see if the ship can shoot at the player or debris
            if( obj->statenme.canshoot &&
                obj->x > 0 && obj->x < SCREEN_WIDTH_f &&
                obj->y > 0 && obj->y < SCREEN_HEIGHT_f ) {

                shotfired = 0;

                if( m_player.state == STATE_ALIVE ) {
                    angle       = game_getangle( obj->x, obj->y, m_player.x, m_player.y );
                    anglediff   = game_anglediff( angle, obj->direction );

                    if( anglediff >= -m_nmefov && anglediff <= m_nmefov ) {
                        nme_fireshot( obj );
                        shotfired = 1;
                    }
                }

                if( !shotfired ) {
                    for( piece = 0; piece < MAX_DEBRIS && !shotfired; piece++ ) {
                        for( i = 0; i < MAX_ACTIVEDEBRIS && !shotfired; i++ ) {
                            if( m_debris[ piece ][ i ].state == STATE_ALIVE ) {
                                angle       = game_getangle( obj->x, obj->y, m_debris[ piece ][ i ].x, m_debris[ piece ][ i ].y );
                                anglediff   = game_anglediff( angle, obj->direction );

                                if( anglediff >= -m_nmefov && anglediff <= m_nmefov ) {
                                    nme_fireshot( obj );
                                    shotfired = 1;
                                }
                            }
                            else if( m_nmedebris[ piece ][ i ].state == STATE_ALIVE ) {
                                angle       = game_getangle( obj->x, obj->y, m_nmedebris[ piece ][ i ].x, m_nmedebris[ piece ][ i ].y );
                                anglediff   = game_anglediff( angle, obj->direction );

                                if( anglediff >= -m_nmefov && anglediff <= m_nmefov ) {
                                    nme_fireshot( obj );
                                    shotfired = 1;
                                }
                            }
                        }
                    }
                }
            }

            player_lookupframe( obj );
            break;

        default :
            break;
    }
}

static void nme_move( obj_t *obj )
{
    if( STATE_ALIVE == obj->state || STATE_NMEWPWAIT == obj->state ) {
        obj->x += ( obj->spd * (float)cos( PI_OVER_180 * obj->direction ) );
        obj->y += ( obj->spd * (float)sin( PI_OVER_180 * obj->direction ) );
    }
}

static void nme_collide( obj_t *obj )
{
    if( STATE_ALIVE == obj->state || STATE_NMEWPWAIT == obj->state ) {
        if( game_pointinbox( (int)obj->x, (int)obj->y
                           , (int)obj->statenme.waypoints[ obj->statenme.currentwaypoint ].x
                           , (int)obj->statenme.waypoints[ obj->statenme.currentwaypoint ].y
                           , WAYPOINT_BOXSIZE, WAYPOINT_BOXSIZE ) ) {
            if( obj->statenme.waypoints[ obj->statenme.currentwaypoint ].changestate ) {
                obj->state = STATE_NMEWPWAIT;
            }
            //else {
                if( ++obj->statenme.currentwaypoint >= obj->statenme.waypointcount ) {
                    obj->statenme.currentwaypoint = 0;
                    // if this is not a bonus round then...
                    if( ISBONUSROUND() ) {
                        obj->state = STATE_DEAD;
                    }
                    else {
                        obj->state = STATE_NMEWAITING;
                    }
                }
                obj->statenme.statewaypoint = STATE_NMETURNING;
            //}
        }
    }
}

static void nme_draw( obj_t *obj )
{
    if( STATE_ALIVE == obj->state || STATE_NMEWPWAIT == obj->state ) {
        game_maptoscreen( obj );
        g_backbuffer->Blt( &obj->dst, obj->dds, &obj->src, DDBLT_KEYSRC | DDBLT_WAIT, NULL );
    }
}

static void nme_explode( obj_t *nme, obj_t *obj )
{
    int i, j;
    int direction;

    for( i = 0; i < MAX_ACTIVEDEBRIS; i++ ) {
        if( STATE_PREBIRTH == m_nmedebris[ 0 ][ i ].state ) {
            break;
        }
    }

    if( i >= MAX_ACTIVEDEBRIS ) {
        // just use the first one
        i = 0;
    }

    if( nme->spd * nme->mass > fabs( obj->spd ) * obj->mass ) {
        direction = nme->direction;
    }
    else {
        if( obj->spd < 0 ) {
            direction = obj->direction - 180;
            if( direction < 0 ) {
                direction += 360;
            }
        }
        else {
            direction = obj->direction;
        }
    }

    for( j = 0; j < MAX_DEBRIS; j++ ) {
        m_nmedebris[ j ][ i ].state    = STATE_ALIVE;
        m_nmedebris[ j ][ i ].x        = obj->x;
        m_nmedebris[ j ][ i ].y        = obj->y;

        m_nmedebris[ j ][ i ].direction = direction + game_rand( -20, 20 );

        //m_nmedebris[ j ][ i ].spd = (float)( (int)rand() % (int)ASTEROID_SPEEDMAX ) + ASTEROID_SPEEDMIN;
        m_nmedebris[ j ][ i ].spd = (float)( (float)ASTEROID_SPEED * ( ( ( (int)m_level + (int)1 ) * (int)10 ) + ( (int)m_round + (int)1 ) ) ) + ( (float)game_rand( 5, 20 ) / 100.0f );

        m_nmedebris[ j ][ i ].spdx = m_nmedebris[ j ][ i ].spd * (float)cos( PI_OVER_180 * m_nmedebris[ j ][ i ].direction );
        m_nmedebris[ j ][ i ].spdy = m_nmedebris[ j ][ i ].spd * (float)sin( PI_OVER_180 * m_nmedebris[ j ][ i ].direction );

        game_setframe( &m_nmedebris[ j ][ i ], 0 );
    }

    if( m_options.nmeexpl ) {
        shotrichochet_setactive( m_nmeexp
                               , -1, nme->x, nme->y
                               , NMEEXP_MINRED,  NMEEXP_MAXRED
                               , NMEEXP_MAXRED,  NMEEXP_MAXGREEN
                               , NMEEXP_MINBLUE, NMEEXP_MAXBLUE
                               , NMEEXP_DESTHUE, NMEEXP_INC );
    }
    sys_playsound( m_sounds[ SND_PLYREXPL ], 0 );
}

static void nme_fireshot( obj_t *obj )
{
    int i;

    if( obj->statenme.cooldown < m_nmecooldown ) {
        return;
    }

    for( i = 0; i < MAX_NMESHOTS; i++ ) {
        if( STATE_ALIVE != m_nmeshots[ i ].state ) {
            break;
        }
    }

    if( i >= MAX_NMESHOTS ) {
        return;
    }

    m_nmeshots[ i ].state  = STATE_ALIVE;
    m_nmeshots[ i ].x      = obj->x;
    m_nmeshots[ i ].y      = obj->y;
    m_nmeshots[ i ].spd    = NMESHOT_SPEED;
    m_nmeshots[ i ].spdx   = m_nmeshots[ i ].spd * (float)cos( PI_OVER_180 * obj->direction );
    m_nmeshots[ i ].spdy   = m_nmeshots[ i ].spd * (float)sin( PI_OVER_180 * obj->direction );

    m_nmeshots[ i ].direction = obj->direction;

    obj->statenme.cooldown = 0;

    if( m_options.nmeshottrails ) {
        shottrail_setactive( m_nmeshottrails, &m_nmeshots[ i ]
                           , NMESHOTTRAIL_MINRED,   NMESHOTTRAIL_MAXRED
                           , NMESHOTTRAIL_MINGREEN, NMESHOTTRAIL_MAXGREEN
                           , NMESHOTTRAIL_MINBLUE,  NMESHOTTRAIL_MAXBLUE
                           , NMESHOTTRAIL_DESTHUE,  NMESHOTTRAIL_INC );
    }
    sys_playsound( m_sounds[ SND_PLYRSHT ], 0 );
}

static void nme_mgrinit( void )
{
    m_nmeactiveships    = ( ( m_level / 2 ) + 2 );
    m_nmefov            = ( MAX_LEVELS - m_level ) + NME_FOVBASE;
    m_nmecooldown       = ( ( MAX_LEVELS - m_level ) + 1 ) * NME_GUNCOOLDOWN;
    m_nmemaxspeed       = NME_BASESPEED + (float)( ( m_level / 3.0f ) * NME_INCSPEED );

    if( ISBONUSROUND() ) {
        m_nmethreshold = ( 3 * NME_WAITTHRESHOLD );
    }
    else {
        m_nmethreshold = ( 2 * NME_WAITTHRESHOLD ) + ( NME_WAITTHRESHOLD * ( m_level + 1 ) );
    }
}

static void nme_mgrreset( void )
{
    int i;

    // if this isn't a bonus round then...
    if( !ISBONUSROUND() ) {

        m_nmestatethink = STATE_NMEWAITING;
        m_nmeshipcount  = m_nmeactiveships;
        m_nmecounter    = 0;

        for( i = 0; i < m_nmeactiveships; i++ ) {
            m_nmeships[ i ].state = STATE_NMEWAITING;
        }
    }
}

static void nme_mgrthink( void )
{
    int i, j;
    int count;
    int type;

    switch( m_nmestatethink ) {
        case STATE_NMEWAITING :
            if( ++m_nmecounter > m_nmethreshold ) {
                m_nmecounter    = 0;
                m_nmestatethink = STATE_NMEACTIVE;

                for( i = 0; i < m_nmeactiveships; i++ ) {
                    if( STATE_NMEWAITING == m_nmeships[ i ].state ) {
                        m_nmeships[ i ].state = STATE_ALIVE;
                    }

                    m_nmeships[ i ].statenme.statewaypoint   = STATE_NMETURNING;
                    m_nmeships[ i ].statenme.currentwaypoint = 0;

                    // if this isn't a bonus round then...
                    if( !ISBONUSROUND() ) {
                        // nme ships are allowed to shoot on levels > 0
                        m_nmeships[ i ].statenme.canshoot = ( m_level > 0 );

                        if( 0 == i ) {
                            nme_mgrsetwaypoints( &m_nmeships[ 0 ], &type );
                        }
                        else {
                            m_nmeships[ i ].statenme.waypointcount = m_nmeships[ 0 ].statenme.waypointcount;

                            memcpy( m_nmeships[ i ].statenme.waypoints
                                  , m_nmeships[ 0 ].statenme.waypoints
                                  , sizeof( waypoint_t ) * MAX_WAYPOINTS );
                        }

                        switch( type ) {        // left side of screen
                            case 0 :
                                m_nmeships[ i ].x = -(float)NME_SPACING * (float)( (int)i + 2 );
                                m_nmeships[ i ].y = m_nmeships[ i ].statenme.waypoints[ 0 ].y;

                                m_nmeships[ i ].direction = 0;
                                break;

                            case 1 :            // right side of screen
                                m_nmeships[ i ].x = SCREEN_WIDTH_f + ( (float)( (int)i + 2 ) * (float)NME_SPACING );
                                m_nmeships[ i ].y = m_nmeships[ i ].statenme.waypoints[ 0 ].y;

                                m_nmeships[ i ].direction = 180;
                                break;

                            case 2 :            // top side of screen
                                m_nmeships[ i ].x = m_nmeships[ i ].statenme.waypoints[ 0 ].x;
                                m_nmeships[ i ].y = -(float)NME_SPACING * (float)( (int)i + 2 );

                                m_nmeships[ i ].direction = 90;
                                break;

                            case 3 :            // bottom side of screen
                                m_nmeships[ i ].x = m_nmeships[ i ].statenme.waypoints[ 0 ].x;
                                m_nmeships[ i ].y = SCREEN_WIDTH_f + ( (float)( (int)i + 2 ) * (float)NME_SPACING );

                                m_nmeships[ i ].direction = 270;
                                break;
                        }

                        for( j = 0; j < m_nmeships[ i ].statenme.waypointcount; j++ ) {
                            m_nmeships[ i ].statenme.waypoints[ j ].changestate = ( 0 == rand() % ( ( 2 * MAX_LEVELS ) - m_level ) );
                        }
                    }
                    // else this is a bonus round, the nme ships will already
                    // have their waypoints set
                }
            }
            break;

        case STATE_NMEACTIVE :
            count = 0;
            for( i = 0; i < m_nmeactiveships; i++ ) {               // how many nme ships are dead?
                if( m_nmeships[ i ].state == STATE_DEAD ) {
                    count++;
                }
            }
            if( count >= m_nmeactiveships ) {
                m_nmestatethink = STATE_NMEDEAD;                    // they're all dead, stop the mgr
            }
            else {                                                  // one of them is at least waiting
                count = 0;                                          // or alive
                for( i = 0; i < m_nmeactiveships; i++ ) {
                    if( m_nmeships[ i ].state != STATE_ALIVE && m_nmeships[ i ].state != STATE_NMESTARTWAIT ) {
                        count++;
                    }
                }
                if( count >= m_nmeactiveships ) {
                    m_nmestatethink = STATE_NMEWAITING;             // all the ships that are left are
                }                                                   // ready to wait again
            }
            break;

        case STATE_NMEDEAD :
            // if this is a bonus round then...
            if( ISBONUSROUND() && STATE_ALIVE != m_risingscore.state && m_playerready ) {
                // move to the next level
                m_round = 0;
                m_level++;
                game_stats();
            }
            break;
    }
}

static void nme_mgrsetwaypoints( obj_t *obj, int *type )
{
    int last, number;
    int i, count;


    // the first waypoint must be a point offscreen
    switch( (*type) = rand() % 4 ) {
        case 0 :                // left side of screen
            obj->statenme.waypoints[ 0 ].x = -NME_SPACING;
            obj->statenme.waypoints[ 0 ].y = SCREEN_HALFHEIGHT_f + (float)game_rand( -NME_SPACING, NME_SPACING );
            break;

        case 1 :                // right side of screen
            obj->statenme.waypoints[ 0 ].x = SCREEN_WIDTH_f + NME_SPACING;
            obj->statenme.waypoints[ 0 ].y = SCREEN_HALFHEIGHT_f + (float)game_rand( -NME_SPACING, NME_SPACING );
            break;

        case 2 :                // top side of screen
            obj->statenme.waypoints[ 0 ].x = SCREEN_HALFWIDTH_f + (float)game_rand( -NME_SPACING, NME_SPACING );
            obj->statenme.waypoints[ 0 ].y = -NME_SPACING;
            break;

        case 3 :                // bottom side of screen
            obj->statenme.waypoints[ 0 ].x = SCREEN_HALFWIDTH_f + (float)game_rand( -NME_SPACING, NME_SPACING );
            obj->statenme.waypoints[ 0 ].y = SCREEN_HEIGHT_f + NME_SPACING;
            break;
    }
    obj->statenme.waypoints[ 0 ].changestate = 0;

    obj->statenme.waypointcount = 1;

    count = ( MAX_LEVELS - m_level ) / 2;
    last  = -1;

    for( i = 0; i < ( 2 + count ); i++ ) {

        do {
            number = rand() % 4;
        }
        while( number == last );

        switch( number ) {
            case 0 :                        // top left quarter
                obj->statenme.waypoints[ obj->statenme.waypointcount ].x = ( SCREEN_WIDTH_f  / 4.0f ) + (float)game_rand( -NME_SPACING, NME_SPACING );
                obj->statenme.waypoints[ obj->statenme.waypointcount ].y = ( SCREEN_HEIGHT_f / 4.0f ) + (float)game_rand( -NME_SPACING, NME_SPACING );
                break;

            case 1 :                        // top right quarter
                obj->statenme.waypoints[ obj->statenme.waypointcount ].x = ( ( SCREEN_WIDTH_f  / 4.0f ) * 3.0f ) + (float)game_rand( -NME_SPACING, NME_SPACING );
                obj->statenme.waypoints[ obj->statenme.waypointcount ].y = ( SCREEN_HEIGHT_f / 4.0f ) + (float)game_rand( -NME_SPACING, NME_SPACING );
                break;

            case 2 :                        // bottom left quarter
                obj->statenme.waypoints[ obj->statenme.waypointcount ].x = ( SCREEN_WIDTH_f  / 4.0f ) + (float)game_rand( -NME_SPACING, NME_SPACING );
                obj->statenme.waypoints[ obj->statenme.waypointcount ].y = ( ( SCREEN_HEIGHT_f / 4.0f ) * 3.0f ) + (float)game_rand( -NME_SPACING, NME_SPACING );
                break;

            case 3 :                        // bottom right quarter
                obj->statenme.waypoints[ obj->statenme.waypointcount ].x = ( ( SCREEN_WIDTH_f  / 4.0f ) * 3.0f ) + (float)game_rand( -NME_SPACING, NME_SPACING );
                obj->statenme.waypoints[ obj->statenme.waypointcount ].y = ( ( SCREEN_HEIGHT_f / 4.0f ) * 3.0f ) + (float)game_rand( -NME_SPACING, NME_SPACING );
                break;
        }

        obj->statenme.waypoints[ obj->statenme.waypointcount ].changestate = 0;

        obj->statenme.waypointcount++;
        last = number;
    }

    // the last waypoint must be a point offscreen
    switch( rand() % 4 ) {
        case 0 :                // left side of screen
            obj->statenme.waypoints[ obj->statenme.waypointcount ].x = -NME_SPACING;
            obj->statenme.waypoints[ obj->statenme.waypointcount ].y = SCREEN_HALFHEIGHT_f + (float)game_rand( -NME_SPACING, NME_SPACING );
            break;

        case 1 :                // right side of screen
            obj->statenme.waypoints[ obj->statenme.waypointcount ].x = SCREEN_WIDTH_f + NME_SPACING;
            obj->statenme.waypoints[ obj->statenme.waypointcount ].y = SCREEN_HALFHEIGHT_f + (float)game_rand( -NME_SPACING, NME_SPACING );
            break;

        case 2 :                // top side of screen
            obj->statenme.waypoints[ obj->statenme.waypointcount ].x = SCREEN_HALFWIDTH_f + (float)game_rand( -NME_SPACING, NME_SPACING );
            obj->statenme.waypoints[ obj->statenme.waypointcount ].y = -NME_SPACING;
            break;

        case 3 :                // bottom side of screen
            obj->statenme.waypoints[ obj->statenme.waypointcount ].x = SCREEN_HALFWIDTH_f + (float)game_rand( -NME_SPACING, NME_SPACING );
            obj->statenme.waypoints[ obj->statenme.waypointcount ].y = SCREEN_HEIGHT_f + NME_SPACING;
            break;
    }
    obj->statenme.waypoints[ obj->statenme.waypointcount ].changestate = 0;

    obj->statenme.waypointcount++;
}

static void pickup_init( float x, float y, int type )
{
    if( type < 0 || type >= MAX_PICKUPS ) {
        type = pickup_choose();
    }

    m_currentpickup = &m_pickups[ type ];

    m_currentpickup->x = x;
    m_currentpickup->y = y;

    m_currentpickup->state  = rand() % 2 ? STATE_LINE : STATE_CIRCLE;
    m_currentpickup->spd    = PICKUP_SPEED;

    m_currentpickup->statepickup.framecounter  = 0;
    m_currentpickup->statepickup.statecounter  = 0;
    m_currentpickup->statepickup.lingercounter = 0;
    m_currentpickup->statepickup.exitpending   = 0;

    m_currentpickup->statepickup.lingerthreshold = MAX_LINGERBASE + ( MAX_LINGERBASE * ( MAX_LEVELS - m_level ) );
    m_currentpickup->statepickup.statethreshold  = game_rand( MIN_STATETHRESHOLD, MAX_STATETHRESHOLD );
}

static int pickup_choose( void )
{
    int done;
    int type;

    done = 0;

    while( !done ) {
        type = rand() % MAX_PICKUPS;

        switch( type ) {
            case PICKUP_BOMBREDUCE :
                // disallow the reduce bomb if nothing to reduce
                done = bombreduce_canreduce();
                break;

            case PICKUP_BOMBSCATTER :       // always allow the pickup
                done = 1;
                break;

            case PICKUP_GUNMULTISHOT :      // always allow the pickup
                done = 1;
                break;

            case PICKUP_GUNCHARGEUP :       // always allow the pickup
                done = 1;
                break;

            case PICKUP_BONUSEXTRALIFE :
                // disallow extra life if already at max lives
                done = ( m_player.stateplayer.livesleft < PLAYER_MAXLIVES );
                break;

            case PICKUP_BONUSFULLSHIELD :
                // disallow the full shield pickup if the player already has full shield
                done = ( m_player.stateplayer.shieldhealth < 100 );
                break;
        }

        // stop the same pickup being spawned that is already active
        if( &m_pickups[ type ] == m_currentpickup ) {
            done = 0;
        }
    }

    return type;
}

static void pickup_change( void )
{
    obj_t *newpickup;

    newpickup = &m_pickups[ pickup_choose() ];

    newpickup->state        = rand() % 2 ? STATE_LINE : STATE_CIRCLE;
    newpickup->direction    = m_currentpickup->direction;
    newpickup->x            = m_currentpickup->x;
    newpickup->y            = m_currentpickup->y;
    newpickup->spd          = m_currentpickup->spd;
    newpickup->spdx         = m_currentpickup->spdx;
    newpickup->spdy         = m_currentpickup->spdy;

    memcpy( &newpickup->statepickup, &m_currentpickup->statepickup, sizeof( statepickup_t ) );

    m_currentpickup = newpickup;
}

static void pickup_think( obj_t *obj )
{
    if( STATE_DEAD != obj->state ) {

        if( ++obj->statepickup.framecounter > MAX_STATEFRAMETHRESHOLD ) {
            obj->statepickup.framecounter = 0;
            game_incframe( obj );
        }

        switch( obj->state ) {
            case STATE_LINE :
                // nothing
                break;

            case STATE_CIRCLE :
                obj->direction += PICKUP_TURNSPEED;
                if( obj->direction >= 360 ) {
                    obj->direction -= 360;
                }
                break;
        }

        if( !obj->statepickup.exitpending ) {
            if( ++obj->statepickup.lingercounter > obj->statepickup.lingerthreshold ) {
                obj->statepickup.lingercounter = 0;
                obj->statepickup.exitpending   = 1;
                obj->state = STATE_LINE;
            }
            else if( ++obj->statepickup.statecounter > obj->statepickup.statethreshold ) {
                obj->statepickup.statecounter   = 0;
                obj->statepickup.statethreshold = game_rand( MIN_STATETHRESHOLD, MAX_STATETHRESHOLD );
                obj->state = rand() % 2 ? STATE_LINE : STATE_CIRCLE;
            }
        }
    }
}

static void pickup_move( obj_t *obj )
{
    if( STATE_DEAD != obj->state ) {
        obj->x += ( obj->spd * (float)cos( PI_OVER_180 * obj->direction ) );
        obj->y += ( obj->spd * (float)sin( PI_OVER_180 * obj->direction ) );
    }
}

static void pickup_collide( obj_t *obj )
{
    if( STATE_DEAD != obj->state ) {
        if( obj->x < 0.0f ) {
            if( obj->statepickup.exitpending ) {
                obj->state = STATE_DEAD;
                m_currentpickup = NULL;
                nme_mgrreset();
            }
            else {
                obj->x = 0.0f;
                reflect_headingleft( obj );
            }
        }
        else if( obj->x > SCREEN_WIDTH_f ) {
            if( obj->statepickup.exitpending ) {
                obj->state = STATE_DEAD;
                m_currentpickup = NULL;
                nme_mgrreset();
            }
            else {
                obj->x = SCREEN_WIDTH_f;
                reflect_headingright( obj );
            }
        }

        if( obj->y < 0.0f ) {
            if( obj->statepickup.exitpending ) {
                obj->state = STATE_DEAD;
                m_currentpickup = NULL;
                nme_mgrreset();
            }
            else {
                obj->y = 0.0f;
                reflect_headingup( obj );
            }
        }
        else if( obj->y > SCREEN_HEIGHT_f ) {
            if( obj->statepickup.exitpending ) {
                obj->state = STATE_DEAD;
                m_currentpickup = NULL;
                nme_mgrreset();
            }
            else {
                obj->y = SCREEN_HEIGHT_f;
                reflect_headingdown( obj );
            }
        }
    }
}

static void pickup_draw( obj_t *obj )
{
    if( STATE_DEAD != obj->state ) {
        game_maptoscreen( obj );
        g_backbuffer->Blt( &obj->dst, obj->dds, &obj->src, DDBLT_KEYSRC | DDBLT_WAIT, NULL );
    }
}

static void bombscatter_init( void )
{
    int i;
    int dir;
    int inc;

    dir = rand() % 360;
    inc = ( 360 / BOMBSCATTER_MAXSHOTS ) * ( rand() % 2 ? -1 : 1 );

    for( i = 0; i < BOMBSCATTER_MAXSHOTS; i++, dir += inc ) {
        m_scattershots[ i ].state = STATE_NMEWAITING;

        m_scattershots[ i ].stateasteroid.counter   = 0;
        m_scattershots[ i ].stateasteroid.threshold = i * BOMBSCATTER_THRESHOLD;
        m_scattershots[ i ].stateasteroid.direction = dir;
    }
}

static void bombscatter_think( obj_t *obj )
{
    switch( obj->state ) {
        case STATE_NMEWAITING :
            if( ++obj->stateasteroid.counter > obj->stateasteroid.threshold ) {
                obj->stateasteroid.counter = 0;

                obj->state       = STATE_ALIVE;
                obj->x           = m_player.x;
                obj->y           = m_player.y;
                obj->direction   = obj->stateasteroid.direction;
                obj->spd         = SHOT_SPEED;
                obj->spdx        = obj->spd * (float)cos( PI_OVER_180 * obj->direction );
                obj->spdy        = obj->spd * (float)sin( PI_OVER_180 * obj->direction );

                sys_playsound( m_sounds[ SND_PLYRSHT ], 0 );

                if( m_options.playershottrails ) {
                    shottrail_setactive( m_shottrails, obj
                                       , SHOTTRAIL_MINRED,   SHOTTRAIL_MAXRED
                                       , SHOTTRAIL_MINGREEN, SHOTTRAIL_MAXGREEN
                                       , SHOTTRAIL_MINBLUE,  SHOTTRAIL_MAXBLUE
                                       , SHOTTRAIL_DESTHUE,  SHOTTRAIL_INC );
                }
                m_statsshotsfired++;
            }
            break;

        case STATE_ALIVE :
            game_incframe( obj );
            break;

        case STATE_DYING :
            obj->state = STATE_PREBIRTH;
            break;
    }
}

static int bombreduce_canreduce( void )
{
    int astcol, piece, i;

    for( astcol = 0; astcol < MAX_ASTCOLS; astcol++ ) {
        for( piece = 0; piece < MAX_ASTPIECES; piece++ ) {
            for( i = 0; i < MAX_ASTS; i++ ) {
                if( m_astcols[ astcol ].astpieces[ piece ][ i ].stateasteroid.type <= ASTTYPE_PIECE2 &&
                    m_astcols[ astcol ].astpieces[ piece ][ i ].state              == STATE_ALIVE ) {
                    return 1;  // yep, there's something to reduce
                }
            }
        }
    }

    return 0;   // no, all active pieces are at their smallest size
}

static void spitparticles_think( clstr_t *clstr )
{
    int i, count;
    int r, g, b;

    if( STATE_ALIVE == clstr->state ) {
        if( clstr->obj ) {
            shottrail_add( clstr );
        }

        count = 0;
        for( i = 0; i < clstr->ptlecount; i++ ) {
            if( STATE_ALIVE == clstr->ptles[ i ].state ) {
                r = g = b = 0;

                clstr->ptles[ i ].r += clstr->ptles[ i ].incr;
                if( clstr->ptles[ i ].r <= clstr->ptles[ i ].dstr ) {
                    clstr->ptles[ i ].r = clstr->ptles[ i ].dstr;
                    r = 1;
                }

                clstr->ptles[ i ].g += clstr->ptles[ i ].incg;
                if( clstr->ptles[ i ].g <= clstr->ptles[ i ].dstg ) {
                    clstr->ptles[ i ].g = clstr->ptles[ i ].dstg;
                    g = 1;
                }

                clstr->ptles[ i ].b += clstr->ptles[ i ].incb;
                if( clstr->ptles[ i ].b <= clstr->ptles[ i ].dstb ) {
                    clstr->ptles[ i ].b = clstr->ptles[ i ].dstb;
                    b = 1;
                }

                if( r && g && b ) {
                    clstr->ptles[ i ].state = STATE_DEAD;
                    count++;
                }
            }
            else {
                count++;
            }
        }

        if( count >= clstr->ptlecount ) {
            clstr->state = STATE_DEAD;
        }
    }
}

static void portrait_thinkpickup( obj_t *obj )
{
//    if( STATE_ALIVE == obj->state ) {
        if( ++obj->statepickup.framecounter > PORTRAIT_THRESHOLD ) {
            obj->statepickup.framecounter = 0;
            game_incframe( obj );
        }
//    }
}

static void portrait_think( obj_t *obj )
{
//    if( STATE_ALIVE == obj->state ) {
        if( ++obj->stateasteroid.framecounter > PORTRAIT_THRESHOLD ) {
            obj->stateasteroid.framecounter = 0;
            game_incframe( obj );
        }
//    }
}

static void portrait_draw( obj_t *obj )
{
    if( STATE_ALIVE == obj->state ) {
        game_maptoscreen( obj );
        g_backbuffer->Blt( &obj->dst, obj->dds, &obj->src, DDBLT_KEYSRC | DDBLT_WAIT, NULL );
    }
}

static void portrait_deactivateall( void )
{
    int i;

    m_portraitplyr.state    = STATE_DEAD;
    m_portraitnme.state     = STATE_DEAD;

    m_astcols[ 0 ].astpieces[ ASTTYPE_PIECE0 ][ 0 ].state = STATE_DEAD;
    m_astcols[ 1 ].astpieces[ ASTTYPE_PIECE0 ][ 0 ].state = STATE_DEAD;

    for( i = 0; i < MAX_PICKUPS; i++ ) {
        m_pickups[ i ].state = STATE_DEAD;
    }
}

static void portrait_activate( void )
{
    int i;

    portrait_deactivateall();

    switch( m_currentpage ) {
        case PAGE_GAMEPLAY :
            break;

        case PAGE_PLAYER :
            m_portraitplyr.state = STATE_ALIVE;
            m_portraitplyr.x = ( m_helppages[ 0 ].x - ( m_helppages[ 0 ].width / 2.0f  ) ) + 50.0f;
            m_portraitplyr.y = ( m_helppages[ 0 ].y - ( m_helppages[ 0 ].height / 2.0f ) ) + 90.0f;
            break;

        case PAGE_ENEMY :
            m_portraitnme.state = STATE_ALIVE;
            m_portraitnme.x = ( m_helppages[ 0 ].x - ( m_helppages[ 0 ].width / 2.0f  ) ) + 50.0f;
            m_portraitnme.y = ( m_helppages[ 0 ].y - ( m_helppages[ 0 ].height / 2.0f ) ) + 90.0f;
            break;

        case PAGE_ASTEROIDS :
            m_astcols[ 0 ].astpieces[ ASTTYPE_PIECE0 ][ 0 ].state = STATE_ALIVE;
            m_astcols[ 0 ].astpieces[ ASTTYPE_PIECE0 ][ 0 ].x = ( m_helppages[ 0 ].x - ( m_helppages[ 0 ].width / 2.0f  ) ) + 50.0f;
            m_astcols[ 0 ].astpieces[ ASTTYPE_PIECE0 ][ 0 ].y = ( m_helppages[ 0 ].y - ( m_helppages[ 0 ].height / 2.0f ) ) + 90.0f;
            m_astcols[ 1 ].astpieces[ ASTTYPE_PIECE0 ][ 0 ].state = STATE_ALIVE;
            m_astcols[ 1 ].astpieces[ ASTTYPE_PIECE0 ][ 0 ].x = ( m_helppages[ 0 ].x - ( m_helppages[ 0 ].width / 2.0f  ) ) + 50.0f;
            m_astcols[ 1 ].astpieces[ ASTTYPE_PIECE0 ][ 0 ].y = ( m_helppages[ 0 ].y - ( m_helppages[ 0 ].height / 2.0f ) ) + 180.0f;
            break;

        case PAGE_PICKUPS :
            for( i = 0; i < MAX_PICKUPS; i++ ) {
                m_pickups[ i ].x = ( m_helppages[ 0 ].x - ( m_helppages[ 0 ].width / 2.0f  ) ) + 50.0f;
                m_pickups[ i ].y = ( i * 32 ) + ( m_helppages[ 0 ].y - ( m_helppages[ 0 ].height / 2.0f ) ) + 60.0f;
                m_pickups[ i ].state = STATE_ALIVE;
                m_pickups[ i ].statepickup.framecounter = rand() % PORTRAIT_FRAMESEED;
            }
            break;

        case PAGE_GUNS :
            m_pickups[ PICKUP_GUNMULTISHOT ].x = ( m_helppages[ 0 ].x - ( m_helppages[ 0 ].width / 2.0f  ) ) + 50.0f;
            m_pickups[ PICKUP_GUNMULTISHOT ].y = ( m_helppages[ 0 ].y - ( m_helppages[ 0 ].height / 2.0f ) ) + 60.0f;
            m_pickups[ PICKUP_GUNMULTISHOT ].stateasteroid.framecounter = rand() % PORTRAIT_FRAMESEED;

            m_pickups[ PICKUP_GUNCHARGEUP ].x = ( m_helppages[ 0 ].x - ( m_helppages[ 0 ].width / 2.0f  ) ) + 50.0f;
            m_pickups[ PICKUP_GUNCHARGEUP ].y = ( m_helppages[ 0 ].y - ( m_helppages[ 0 ].height / 2.0f ) ) + 124.0f;
            m_pickups[ PICKUP_GUNCHARGEUP ].stateasteroid.framecounter = rand() % PORTRAIT_FRAMESEED;

            m_pickups[ PICKUP_GUNCHARGEUP ].state = STATE_ALIVE;
            m_pickups[ PICKUP_GUNMULTISHOT ].state = STATE_ALIVE;
            break;

        case PAGE_BOMBS :
            m_pickups[ PICKUP_BOMBSCATTER ].x = ( m_helppages[ 0 ].x - ( m_helppages[ 0 ].width / 2.0f  ) ) + 50.0f;
            m_pickups[ PICKUP_BOMBSCATTER ].y = ( m_helppages[ 0 ].y - ( m_helppages[ 0 ].height / 2.0f ) ) + 60.0f;
            m_pickups[ PICKUP_BOMBSCATTER ].stateasteroid.framecounter = rand() % PORTRAIT_FRAMESEED;

            m_pickups[ PICKUP_BOMBREDUCE ].x = ( m_helppages[ 0 ].x - ( m_helppages[ 0 ].width / 2.0f  ) ) + 50.0f;
            m_pickups[ PICKUP_BOMBREDUCE ].y = ( m_helppages[ 0 ].y - ( m_helppages[ 0 ].height / 2.0f ) ) + 110.0f;
            m_pickups[ PICKUP_BOMBREDUCE ].stateasteroid.framecounter = rand() % PORTRAIT_FRAMESEED;

            m_pickups[ PICKUP_BOMBREDUCE ].state = STATE_ALIVE;
            m_pickups[ PICKUP_BOMBSCATTER ].state = STATE_ALIVE;
            break;

        case PAGE_BONUSES :
            m_pickups[ PICKUP_BONUSEXTRALIFE ].x = ( m_helppages[ 0 ].x - ( m_helppages[ 0 ].width / 2.0f  ) ) + 50.0f;
            m_pickups[ PICKUP_BONUSEXTRALIFE ].y = ( m_helppages[ 0 ].y - ( m_helppages[ 0 ].height / 2.0f ) ) + 62.0f;
            m_pickups[ PICKUP_BONUSEXTRALIFE ].stateasteroid.framecounter = rand() % PORTRAIT_FRAMESEED;

            m_pickups[ PICKUP_BONUSFULLSHIELD ].x = ( m_helppages[ 0 ].x - ( m_helppages[ 0 ].width / 2.0f  ) ) + 50.0f;
            m_pickups[ PICKUP_BONUSFULLSHIELD ].y = ( m_helppages[ 0 ].y - ( m_helppages[ 0 ].height / 2.0f ) ) + 122.0f;
            m_pickups[ PICKUP_BONUSFULLSHIELD ].stateasteroid.framecounter = rand() % PORTRAIT_FRAMESEED;

            m_pickups[ PICKUP_BONUSEXTRALIFE ].state = STATE_ALIVE;
            m_pickups[ PICKUP_BONUSFULLSHIELD ].state = STATE_ALIVE;
            break;
    }
}

static void sldr_move( obj_t *obj )
{
    if( obj && obj->state == STATE_ALIVE && m_mousecapture == obj ) {
        obj->x = m_mousecursor.x + obj->statesliderbox.offsetx;
        if( obj->x < obj->statesliderbox.minx ) {
            obj->x = obj->statesliderbox.minx;
        }
        else if( obj->x >= obj->statesliderbox.maxx ) {
            obj->x = obj->statesliderbox.maxx;
        }

        if( obj == &m_sldrboxsound ) {
            m_options.soundlevel = (int)( m_sldrboxsound.x - ( SCREEN_HALFWIDTH_f - 50.0f ) );
        }
        else if( obj == &m_sldrboxmusic ) {
            m_options.musiclevel = (int)( m_sldrboxmusic.x - ( SCREEN_HALFWIDTH_f - 50.0f ) );
        }
        else {
            m_options.mouseoffset = (int)( m_sldrboxmouse.x - ( SCREEN_HALFWIDTH_f - 50.0f ) );
        }
    }
}

static void options_read( const char *filename, options_t *options )
{
    FILE *file;
    errno_t Error;

    if( !filename || !options ) {
        return;
    }

    Error = fopen_s( &file, filename, "rb" );
    if( Error != 0 ) {
        // can't find file
        options_default( options );
        return;
    }

    if( sizeof( options_t ) != fread( (void *)options, sizeof( char ), sizeof( options_t ), file ) ) {
        // failed to read all the options
        fclose( file );
        options_default( options );
        return;
    }

    fclose( file );
}

static void options_write( const char *filename, options_t *options )
{
    FILE *file;
    errno_t Error;

    if( !filename || !options ) {
        return;
    }

    Error = fopen_s( &file, filename, "wb" );
    if( Error != 0 ) {
        return;
    }

    fwrite( (const void *)options, sizeof( char ), sizeof( options_t ), file );
    fclose( file );
}

static void options_default( options_t *options )
{
    if( options ) {
        options->soundlevel         = DEFAULT_SOUNDLEVEL;
        options->musiclevel         = DEFAULT_MUSICLEVEL;
        options->mousespeed         = DEFAULT_MOUSESPEED;
        options->physicstype        = DEFAULT_PHYSICSTYPE;
        options->playerthrusting    = DEFAULT_PLAYERTHRUSTING;
        options->playershottrails   = DEFAULT_PLAYERSHOTTRAILS;
        options->playershotexpl     = DEFAULT_PLAYERSHOTEXPL;
        options->playerexpl         = DEFAULT_PLAYEREXPL;
        options->nmeshottrails      = DEFAULT_NMESHOTTRAILS;
        options->nmeshotexpl        = DEFAULT_NMESHOTEXPL;
        options->nmeexpl            = DEFAULT_NMEEXPL;
    }
}

static void bonus_levelone( void )
{
    int i;

    nme_mgrinit();

    m_nmeshipcount  = m_nmeactiveships = 5;
    m_nmestatethink = STATE_NMEWAITING;
    m_nmecounter    = 0;

    for( i = 0; i < m_nmeactiveships; i++ ) {
        m_nmeships[ i ].state = STATE_NMEWAITING;
    }

    // for the first bonus level there will be 5 nme ships that fly down
    // the screen from top to bottom, each at a different x coord.  if the
    // player can shoot them all they get bonus points.

    for( i = 0; i < m_nmeactiveships; i++ ) {
        m_nmeships[ i ].statenme.waypointcount = 2;

        m_nmeships[ i ].x =  64.0f + (float)( (int)rand() % ( (int)SCREEN_WIDTH - (int)128 ) );
        m_nmeships[ i ].y = -64.0f - (float)( (int)i * (int)128 );

        // disallow the nme ships to shoot
        m_nmeships[ i ].statenme.canshoot = 0;

        // face downwards
        m_nmeships[ i ].direction = 90;

        m_nmeships[ i ].statenme.waypoints[ 0 ].x = m_nmeships[ i ].x;
        m_nmeships[ i ].statenme.waypoints[ 0 ].y = SCREEN_HEIGHT_f - 64.0f;

        m_nmeships[ i ].statenme.waypoints[ 1 ].x = m_nmeships[ i ].x;
        m_nmeships[ i ].statenme.waypoints[ 1 ].y = -64.0f;
    }

    // in this bonus level, the player can use only the normal gun
    m_player.stateplayer.gunpushed  = m_player.stateplayer.gun;
    m_player.stateplayer.gun        = GUN_NORMAL;
}

static void bonus_leveltwo( void )
{
    int i, piece;

    // for this bonus level there will be a shit load of asteroids moving
    // from the left side of the screen to the right

    m_astcols[ ASTCOL_DIRT ].activepieces = 0;
    m_astcols[ ASTCOL_ROCK ].activepieces = 0;

    for( piece = ASTTYPE_PIECE3; piece <= ASTTYPE_PIECE6; piece++ ) {
        for( i = 0; i < MAX_ASTS; i++ ) {
            m_astcols[ ASTCOL_DIRT ].astpieces[ piece ][ i ].stateasteroid.hasentered       = 0;
            m_astcols[ ASTCOL_DIRT ].astpieces[ piece ][ i ].stateasteroid.framecounter     = 0;
            m_astcols[ ASTCOL_DIRT ].astpieces[ piece ][ i ].stateasteroid.framethreshold   = ( rand() % MAX_ASTFRAMEROT ) + MIN_ASTFRAMEROT;
            m_astcols[ ASTCOL_DIRT ].astpieces[ piece ][ i ].stateasteroid.bombactive       = 0;
            m_astcols[ ASTCOL_DIRT ].astpieces[ piece ][ i ].stateasteroid.health           = 0;

            //m_astcols[ ASTCOL_DIRT ].astpieces[ piece ][ i ].state      = STATE_PREBIRTH;
            m_astcols[ ASTCOL_DIRT ].astpieces[ piece ][ i ].state      = STATE_ALIVE;
            m_astcols[ ASTCOL_DIRT ].astpieces[ piece ][ i ].points     = ( m_level + 1 ) * POINTS_ASTBASE;
            m_astcols[ ASTCOL_DIRT ].astpieces[ piece ][ i ].x          = SCREEN_WIDTH_f + (float)( rand() % 128 );
            m_astcols[ ASTCOL_DIRT ].astpieces[ piece ][ i ].y          = (float)( rand() % SCREEN_HEIGHT );
            m_astcols[ ASTCOL_DIRT ].astpieces[ piece ][ i ].spd        = ASTEROID_SPEED * ( ( m_level + 1 ) * 10 );
            m_astcols[ ASTCOL_DIRT ].astpieces[ piece ][ i ].direction  = 180;


            m_astcols[ ASTCOL_DIRT ].astpieces[ piece ][ i ].spdx    = m_astcols[ ASTCOL_DIRT ].astpieces[ piece ][ i ].spd *
                                                                  (float)cos( PI_OVER_180 * m_astcols[ ASTCOL_DIRT ].astpieces[ piece ][ i ].direction );

            m_astcols[ ASTCOL_DIRT ].astpieces[ piece ][ i ].spdy    = m_astcols[ ASTCOL_DIRT ].astpieces[ piece ][ i ].spd *
                                                                  (float)sin( PI_OVER_180 * m_astcols[ ASTCOL_DIRT ].astpieces[ piece ][ i ].direction );
            m_astcols[ ASTCOL_DIRT ].activepieces++;
        }
    }

    // in this bonus level, the player can use only the normal gun
    m_player.stateplayer.gunpushed  = m_player.stateplayer.gun;
    m_player.stateplayer.gun        = GUN_NORMAL;
}

static void bonus_levelthree( void )
{
    int i;
    int side;

    nme_mgrinit();

    m_nmemaxspeed   = NME_BASESPEED + (float)( ( m_level / 2.0f ) * NME_INCSPEED );
    m_nmeshipcount  = m_nmeactiveships = MAX_NMESHIPS;
    m_nmestatethink = STATE_NMEWAITING;
    m_nmecounter    = 0;

    // for this bonus level there will be 10 nme ships that fly across the level
    // in a straight line.

    for( i = 0; i < m_nmeactiveships; i++ ) {
        m_nmeships[ i ].state = STATE_NMESTARTWAIT;
        m_nmeships[ i ].statenme.waypointcount = 2;

        m_nmeships[ i ].statenme.counter    = 0;
        m_nmeships[ i ].statenme.threshold  = NME_STARTWAITBASE * ( i + 1 );

        // choose an offscreen location
        side = rand() % 4;
        switch( side ) {
            case 0 :    // top
                m_nmeships[ i ].x = (float)( (int)rand() % (int)SCREEN_WIDTH );
                m_nmeships[ i ].y = -64.0f;
                m_nmeships[ i ].direction = 90;
                break;
            case 1 :    // bottom
                m_nmeships[ i ].x = (float)( (int)rand() % (int)SCREEN_WIDTH );
                m_nmeships[ i ].y = 64.0f + SCREEN_HEIGHT_f;
                m_nmeships[ i ].direction = 270;
                break;
            case 2 :    // left
                m_nmeships[ i ].x = -64.0f;
                m_nmeships[ i ].y = (float)( (int)rand() % (int)SCREEN_HEIGHT );
                m_nmeships[ i ].direction = 180;
                break;
            case 3 :    // right
                m_nmeships[ i ].x = 64.0f + SCREEN_WIDTH_f;
                m_nmeships[ i ].y = (float)( (int)rand() % (int)SCREEN_HEIGHT );
                m_nmeships[ i ].direction = 0;
                break;
        }

        m_nmeships[ i ].statenme.waypoints[ 1 ].x = m_nmeships[ i ].x;
        m_nmeships[ i ].statenme.waypoints[ 1 ].y = m_nmeships[ i ].y;

        // allow the nme ships to shoot
        m_nmeships[ i ].statenme.canshoot = 1;

        // choose a location on the opposite side of the screen just in a bit from the
        // edge of the screen
        switch( side ) {
            case 0 :    // bottom
                m_nmeships[ i ].statenme.waypoints[ 0 ].x = (float)( (int)rand() % (int)SCREEN_WIDTH );
                m_nmeships[ i ].statenme.waypoints[ 0 ].y = SCREEN_HEIGHT_f;
                break;
            case 1 :    // top
                m_nmeships[ i ].statenme.waypoints[ 0 ].x = (float)( (int)rand() % (int)SCREEN_WIDTH );
                m_nmeships[ i ].statenme.waypoints[ 0 ].y = 0.0f;
                break;
            case 2 :    // right
                m_nmeships[ i ].statenme.waypoints[ 0 ].x = SCREEN_WIDTH_f;
                m_nmeships[ i ].statenme.waypoints[ 0 ].y = (float)( (int)rand() % (int)SCREEN_HEIGHT );
                break;
            case 3 :    // left
                m_nmeships[ i ].statenme.waypoints[ 0 ].x = 0.0f;
                m_nmeships[ i ].statenme.waypoints[ 0 ].y = (float)( (int)rand() % (int)SCREEN_HEIGHT );
                break;
        }

    }

    // in this bonus level, the player can use only the normal gun
    m_player.stateplayer.gunpushed  = m_player.stateplayer.gun;
    m_player.stateplayer.gun        = GUN_NORMAL;
}

static void bonus_levelfour( void )
{
    int i, piece;
    int side;

    // for this bonus level there will be a shit load of asteroids moving
    // from the left side of the screen to the right, and from the right side
    // of the screen to the left

    m_astcols[ ASTCOL_DIRT ].activepieces = 0;
    m_astcols[ ASTCOL_ROCK ].activepieces = 0;

    side = 0;

    for( piece = ASTTYPE_PIECE3; piece <= ASTTYPE_PIECE6; piece++ ) {
        for( i = 0; i < MAX_ASTS; i++ ) {
            m_astcols[ ASTCOL_DIRT ].astpieces[ piece ][ i ].stateasteroid.hasentered       = 0;
            m_astcols[ ASTCOL_DIRT ].astpieces[ piece ][ i ].stateasteroid.framecounter     = 0;
            m_astcols[ ASTCOL_DIRT ].astpieces[ piece ][ i ].stateasteroid.framethreshold   = ( rand() % MAX_ASTFRAMEROT ) + MIN_ASTFRAMEROT;
            m_astcols[ ASTCOL_DIRT ].astpieces[ piece ][ i ].stateasteroid.bombactive       = 0;
            m_astcols[ ASTCOL_DIRT ].astpieces[ piece ][ i ].stateasteroid.health           = 1;

            m_astcols[ ASTCOL_DIRT ].astpieces[ piece ][ i ].state      = STATE_ALIVE;
            m_astcols[ ASTCOL_DIRT ].astpieces[ piece ][ i ].points     = ( m_level + 1 ) * POINTS_ASTBASE;
            m_astcols[ ASTCOL_DIRT ].astpieces[ piece ][ i ].x          = side ? SCREEN_WIDTH_f + (float)( rand() % 128 ) : -(float)( rand() % 128 );
            m_astcols[ ASTCOL_DIRT ].astpieces[ piece ][ i ].y          = (float)( rand() % SCREEN_HEIGHT );
            m_astcols[ ASTCOL_DIRT ].astpieces[ piece ][ i ].spd        = ASTEROID_SPEED * ( ( m_level + 1 ) * 15 );
            m_astcols[ ASTCOL_DIRT ].astpieces[ piece ][ i ].direction  = side ? 180 : 0;


            m_astcols[ ASTCOL_DIRT ].astpieces[ piece ][ i ].spdx    = m_astcols[ ASTCOL_DIRT ].astpieces[ piece ][ i ].spd *
                                                                  (float)cos( PI_OVER_180 * m_astcols[ ASTCOL_DIRT ].astpieces[ piece ][ i ].direction );

            m_astcols[ ASTCOL_DIRT ].astpieces[ piece ][ i ].spdy    = m_astcols[ ASTCOL_DIRT ].astpieces[ piece ][ i ].spd *
                                                                  (float)sin( PI_OVER_180 * m_astcols[ ASTCOL_DIRT ].astpieces[ piece ][ i ].direction );
            m_astcols[ ASTCOL_DIRT ].activepieces++;

            side = !side;
        }
    }

    // in this bonus level, the player can use only the multishot gun
    m_player.stateplayer.gunpushed  = m_player.stateplayer.gun;
    m_player.stateplayer.ammopushed = m_player.stateplayer.ammo;

    m_player.stateplayer.gun    = GUN_MULTISHOT;
    m_player.stateplayer.ammo   = -1;
}

static void bonus_levelfive( void )
{
    int i, side;

    nme_mgrinit();

    m_nmeshipcount  = m_nmeactiveships = MAX_NMESHIPS;
    m_nmestatethink = STATE_NMEWAITING;
    m_nmecounter    = 0;

    // for this bonus level there will be 10 nme ships that fly into the left, then
    // just sit there and shoot at the player.

    for( i = 0; i < m_nmeactiveships; i++ ) {
        m_nmeships[ i ].state = STATE_NMESTARTWAIT;
        m_nmeships[ i ].statenme.waypointcount = 2;

        m_nmeships[ i ].statenme.counter    = 0;
        m_nmeships[ i ].statenme.threshold  = NME_STARTWAITBASE * ( i + 1 );

        // choose an offscreen location
        side = rand() % 4;
        switch( side ) {
            case 0 :    // top
                m_nmeships[ i ].x = (float)( (int)rand() % (int)SCREEN_WIDTH );
                m_nmeships[ i ].y = -64.0f;
                m_nmeships[ i ].direction = 90;
                break;
            case 1 :    // bottom
                m_nmeships[ i ].x = (float)( (int)rand() % (int)SCREEN_WIDTH );
                m_nmeships[ i ].y = 64.0f + SCREEN_HEIGHT_f;
                m_nmeships[ i ].direction = 270;
                break;
            case 2 :    // left
                m_nmeships[ i ].x = -64.0f;
                m_nmeships[ i ].y = (float)( (int)rand() % (int)SCREEN_HEIGHT );
                m_nmeships[ i ].direction = 180;
                break;
            case 3 :    // right
                m_nmeships[ i ].x = 64.0f + SCREEN_WIDTH_f;
                m_nmeships[ i ].y = (float)( (int)rand() % (int)SCREEN_HEIGHT );
                m_nmeships[ i ].direction = 0;
                break;
        }

        m_nmeships[ i ].statenme.waypoints[ 1 ].x = m_nmeships[ i ].x;
        m_nmeships[ i ].statenme.waypoints[ 1 ].y = m_nmeships[ i ].y;

        // allow the nme ships to shoot
        m_nmeships[ i ].statenme.canshoot = 1;

        // choose a location just inside the screen
        switch( side ) {
            case 0 :    // bottom
                m_nmeships[ i ].statenme.waypoints[ 0 ].x = (float)( (int)rand() % (int)SCREEN_WIDTH );
                m_nmeships[ i ].statenme.waypoints[ 0 ].y = 64.0f;
                break;
            case 1 :    // top
                m_nmeships[ i ].statenme.waypoints[ 0 ].x = (float)( (int)rand() % (int)SCREEN_WIDTH );
                m_nmeships[ i ].statenme.waypoints[ 0 ].y = SCREEN_HEIGHT_f - 64.0f;
                break;
            case 2 :    // right
                m_nmeships[ i ].statenme.waypoints[ 0 ].x = 64.0f;
                m_nmeships[ i ].statenme.waypoints[ 0 ].y = (float)( (int)rand() % (int)SCREEN_HEIGHT );
                break;
            case 3 :    // left
                m_nmeships[ i ].statenme.waypoints[ 0 ].x = SCREEN_WIDTH_f - 64.0f;
                m_nmeships[ i ].statenme.waypoints[ 0 ].y = (float)( (int)rand() % (int)SCREEN_HEIGHT );
                break;
        }
        m_nmeships[ i ].statenme.waypoints[ 0 ].changestate = 1;
    }

    // in this bonus level, the player can use only the normal gun
    m_player.stateplayer.gunpushed  = m_player.stateplayer.gun;
    m_player.stateplayer.gun        = GUN_NORMAL;
}

static void plyrtrail_add1( clstr_t *clstr, obj_t *obj, int flag
                          , int minr, int maxr
                          , int ming, int maxg
                          , int minb, int maxb )
{
    int startidx, finishidx;
    int i, ang;
    int frame;

    if( clstr->ptlecount >= MAX_SHOTTRAILPTLES ) {
        startidx             = clstr->recyclecount;
        clstr->recyclecount += clstr->addrate;

        if( clstr->recyclecount >= MAX_SHOTTRAILPTLES ) {
            startidx            = 0;
            clstr->recyclecount = 0;
        }
    }
    else {
        startidx = clstr->ptlecount;
        clstr->ptlecount += clstr->addrate;

        if( clstr->ptlecount >= MAX_SHOTTRAILPTLES ) {
            clstr->ptlecount    = MAX_SHOTTRAILPTLES;
            startidx            = 0;
            clstr->recyclecount = 0;
        }
    }

    finishidx    = startidx + clstr->addrate;
    ang          = obj->direction - 180;
    clstr->state = STATE_ALIVE;
    clstr->obj   = &m_player;

    frame = obj->direction / PLAYER_FRAMEINC;
    if( (obj->direction % PLAYER_FRAMEINC) >= (PLAYER_FRAMEINC / 2) ) {
        frame++;
    }

    if( frame > PLAYER_MAXFRAMEIDX ) {
        frame = 0;
    }

    if( flag ) {
        for( i = startidx; i <= finishidx; i++ ) {
            clstr->ptles[ i ].state = STATE_ALIVE;

            clstr->ptles[ i ].spdx      = (float)cos( PI_OVER_180 * (float)( (int)ang + (int)game_rand( -15, 15 ) ) );
            clstr->ptles[ i ].spdy      = (float)sin( PI_OVER_180 * (float)( (int)ang + (int)game_rand( -15, 15 ) ) );

            clstr->ptles[ i ].spdx      *= (float)( (int)game_rand( 5, 20 ) ) / 10.0f;
            clstr->ptles[ i ].spdy      *= (float)( (int)game_rand( 5, 20 ) ) / 10.0f;

            clstr->ptles[ i ].r         = (unsigned char)( ( rand() % ( maxr - minr ) ) + minr );
            clstr->ptles[ i ].g         = (unsigned char)( ( rand() % ( maxg - ming ) ) + ming );
            clstr->ptles[ i ].b         = (unsigned char)( ( rand() % ( maxb - minb ) ) + minb );

            clstr->ptles[ i ].x         = obj->x + m_fpoints[ frame ].x2 - obj->halfwidth;
            clstr->ptles[ i ].y         = obj->y + m_fpoints[ frame ].y2 - obj->halfheight;
        }
    }
    else {
        for( i = startidx; i <= finishidx; i++ ) {
            clstr->ptles[ i ].state = STATE_ALIVE;

            clstr->ptles[ i ].spdx      = (float)cos( PI_OVER_180 * (float)( (int)ang + (int)game_rand( -15, 15 ) ) );
            clstr->ptles[ i ].spdy      = (float)sin( PI_OVER_180 * (float)( (int)ang + (int)game_rand( -15, 15 ) ) );

            clstr->ptles[ i ].spdx      *= (float)( (int)game_rand( 5, 20 ) ) / 10.0f;
            clstr->ptles[ i ].spdy      *= (float)( (int)game_rand( 5, 20 ) ) / 10.0f;

            clstr->ptles[ i ].r         = (unsigned char)( ( rand() % ( maxr - minr ) ) + minr );
            clstr->ptles[ i ].g         = (unsigned char)( ( rand() % ( maxg - ming ) ) + ming );
            clstr->ptles[ i ].b         = (unsigned char)( ( rand() % ( maxb - minb ) ) + minb );

            clstr->ptles[ i ].x         = obj->x + m_fpoints[ frame ].x1 - obj->halfwidth;
            clstr->ptles[ i ].y         = obj->y + m_fpoints[ frame ].y1 - obj->halfheight;
        }
    }
}

static void plyrtrail_add2( clstr_t *clstr, obj_t *obj, int flag
                          , int minr, int maxr
                          , int ming, int maxg
                          , int minb, int maxb )
{
    int startidx, finishidx;
    int i, ang;
    int frame;

    if( clstr->ptlecount >= MAX_SHOTTRAILPTLES ) {
        startidx             = clstr->recyclecount;
        clstr->recyclecount += clstr->addrate;

        if( clstr->recyclecount >= MAX_SHOTTRAILPTLES ) {
            startidx            = 0;
            clstr->recyclecount = 0;
        }
    }
    else {
        startidx = clstr->ptlecount;
        clstr->ptlecount += clstr->addrate;

        if( clstr->ptlecount >= MAX_SHOTTRAILPTLES ) {
            clstr->ptlecount    = MAX_SHOTTRAILPTLES;
            startidx            = 0;
            clstr->recyclecount = 0;
        }
    }

    finishidx    = startidx + clstr->addrate;
    ang          = obj->direction;
    clstr->state = STATE_ALIVE;
    clstr->obj   = &m_player;

    frame = obj->direction / PLAYER_FRAMEINC;
    if( (obj->direction % PLAYER_FRAMEINC) >= (PLAYER_FRAMEINC / 2) ) {
        frame++;
    }

    if( frame > PLAYER_MAXFRAMEIDX ) {
        frame = 0;
    }

    if( flag ) {
        for( i = startidx; i <= finishidx; i++ ) {
            clstr->ptles[ i ].state = STATE_ALIVE;

            clstr->ptles[ i ].spdx      = (float)cos( PI_OVER_180 * (float)( (int)ang + (int)game_rand( -15, 15 ) ) );
            clstr->ptles[ i ].spdy      = (float)sin( PI_OVER_180 * (float)( (int)ang + (int)game_rand( -15, 15 ) ) );

            clstr->ptles[ i ].spdx      *= (float)( (int)game_rand( 5, 20 ) ) / 10.0f;
            clstr->ptles[ i ].spdy      *= (float)( (int)game_rand( 5, 20 ) ) / 10.0f;

            clstr->ptles[ i ].r         = (unsigned char)( ( rand() % ( maxr - minr ) ) + minr );
            clstr->ptles[ i ].g         = (unsigned char)( ( rand() % ( maxg - ming ) ) + ming );
            clstr->ptles[ i ].b         = (unsigned char)( ( rand() % ( maxb - minb ) ) + minb );

            clstr->ptles[ i ].x         = obj->x + m_bpoints[ frame ].x2 - obj->halfwidth;
            clstr->ptles[ i ].y         = obj->y + m_bpoints[ frame ].y2 - obj->halfheight;
        }
    }
    else {
        for( i = startidx; i <= finishidx; i++ ) {
            clstr->ptles[ i ].state = STATE_ALIVE;

            clstr->ptles[ i ].spdx      = (float)cos( PI_OVER_180 * (float)( (int)ang + (int)game_rand( -15, 15 ) ) );
            clstr->ptles[ i ].spdy      = (float)sin( PI_OVER_180 * (float)( (int)ang + (int)game_rand( -15, 15 ) ) );

            clstr->ptles[ i ].spdx      *= (float)( (int)game_rand( 5, 20 ) ) / 10.0f;
            clstr->ptles[ i ].spdy      *= (float)( (int)game_rand( 5, 20 ) ) / 10.0f;

            clstr->ptles[ i ].r         = (unsigned char)( ( rand() % ( maxr - minr ) ) + minr );
            clstr->ptles[ i ].g         = (unsigned char)( ( rand() % ( maxg - ming ) ) + ming );
            clstr->ptles[ i ].b         = (unsigned char)( ( rand() % ( maxb - minb ) ) + minb );

            clstr->ptles[ i ].x         = obj->x + m_bpoints[ frame ].x1 - obj->halfwidth;
            clstr->ptles[ i ].y         = obj->y + m_bpoints[ frame ].y1 - obj->halfheight;
        }
    }
}

static void plyrtrail_think( clstr_t *clstr )
{
    int i, count;
    int r, g, b;

    if( STATE_ALIVE == clstr->state ) {
        count = 0;
        for( i = 0; i < clstr->ptlecount; i++ ) {
            if( STATE_ALIVE == clstr->ptles[ i ].state ) {
                r = g = b = 0;

                clstr->ptles[ i ].r += clstr->ptles[ i ].incr;
                if( clstr->ptles[ i ].r <= clstr->ptles[ i ].dstr ) {
                    clstr->ptles[ i ].r = clstr->ptles[ i ].dstr;
                    r = 1;
                }

                clstr->ptles[ i ].g += clstr->ptles[ i ].incg;
                if( clstr->ptles[ i ].g <= clstr->ptles[ i ].dstg ) {
                    clstr->ptles[ i ].g = clstr->ptles[ i ].dstg;
                    g = 1;
                }

                clstr->ptles[ i ].b += clstr->ptles[ i ].incb;
                if( clstr->ptles[ i ].b <= clstr->ptles[ i ].dstb ) {
                    clstr->ptles[ i ].b = clstr->ptles[ i ].dstb;
                    b = 1;
                }

                if( r && g && b ) {
                    clstr->ptles[ i ].state = STATE_DEAD;
                    count++;
                }
            }
            else {
                count++;
            }
        }

        //if( count >= clstr->ptlecount ) {
        //    clstr->state = STATE_DEAD;
        //}
    }
}

//--------------------------------------------------------------------------------
// eof
//--------------------------------------------------------------------------------
