# A L D E B A R A N

## Developed by Damian Coventry

### Copyright (C) 2001, All rights reserved

#### updated:

+ Sunday, 12 August 2001        Added final music
+ Tuesday, 10 July 2001         Updated to 1.20
+ Sunday, 17 June 2001          Updated to 1.10
+ Saturday, 19 May 2001         Created Version 1.00

#### feature history

##### v1.20
added in game help.  eight pages of help explain how the game works.  the
about menu has been replaced with a 'help' menu.

added music support.  a directory named mp3 off the game root stores mp3 files
used by the game.

added level and round system.  there are 10 levels each with 5 rounds within
them.  the last round in a level is a bonus round where the user is awarded
lots of points for destroying each asteroid and an even larger bonus for
destroying absolutely everything.

added options property sheet.  the user can control the sound/music volume,
toggle the player ship physics to/from directional and sliding and can 
enable/disable almost all particle clusters in the game.

added player ship thrust particles.  the player's ship emits particles
from the front and back of its thrusters when the relevant key is held on
the keyboard.

removed marquee about info.  static about info is now being displayed in the
about page.

added splash screen support.  game will play waves and display the bitmaps
placed in the Splash directory before loading game related files.

removed many minor bugs.

##### v1.10
added elastic collisions.  the player's ship bounces of almost every other
object in the game.  each bounce reduces the player's shield.

added enemy ships.  periodically enemy ships fly out and shoot ship debris,
and the player if either or both are in their field of view.

added pickups.  if the player destroys all enemy ships a pickup pops outta
the last shot ship and the player can collect it by flying into it.  the
pickups are:

###### guns (colored green):
\[M\] multi shot. shoots 3 shots at once in a 'V' like pattern, instead
of the usual one shot.

\[C\] charge up.  hold the left mouse button and 'charge' your shot,
the longer you hold it the more charge you generate.  releasing the
button fires the shot.  it will fly thru and destroy many objects
based on how much charge you built up.

##### bombs (colored red):
\[S\] scatter bomb.  fires 36 normal shots in a circular pattern.

\[R\] reduce bomb.  reduces all asteroids to their smallest debris
size.

##### bonuses (colored blue):
\[F\] full shield.  takes your shield back up to 100% power.

\[E\] extra ship.  adds another ship to your total, max 5.

note that certain pickups will not appear if there is no point to them at
that time, the rules are:

+ if all active asteroids are already at their smallest debris size
  the \[R\] pickup will not appear.

+ if the player already has 100% shield the \[F\] will not appear.

+ if the player already has 5 lives the \[E\] will not appear.

##### v1.00
    created initial version. destroy all asteroid pieces to beat a level

####  system requirements

#####  software
windows 95/98/NT/ME/2000
directx 7.0+

##### hardware
video card capable of 640x480x16
33Mb free hdd space
mouse (optional)
sound card (optional)

#### alde_demo notes
the demo version of this game warps the player to level 7 after beating
level 1.   once the player has beaten level 7 the player is returned to
the menu.  no other levels are included in the demo - the idea of the demo
is to show the user both asteroid types and two different bonus levels.

#### controls

##### the player
*turn left* left arrow/move mouse left
*turn right* right arrow/move mouse right
*move forward* up arrow
*move back* down arrow
*stop* space bar/enter
*shoot* control/left mouse button
*shield* shift/right mouse button

##### the game
*pause/go to menu* escape
*quick exit* alt+f4
