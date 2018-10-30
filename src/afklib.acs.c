/*
================================================================================
Frenchies' Chaotic Tuesdays
AFK library

Version 1.0

By JCD
================================================================================
*/

#library "afklib"

#include "zcommon.acs"


//==============================================================================
//
// VARIABLES
//
//==============================================================================

#define AFKX                0
#define AFKY                1
#define AFKZ                2
#define AFKWARN             3

// Stuff
#define PLAYER_DEFAULT_TID  10000
#define ID_AFK              901
#define MAX_PLAYERS         100
#define snd                 "DMG_SND"

// Scripts
#define AFK_SCRIPT_OPEN     310
#define AFK_SCRIPT_ENTER    311
#define AFK_SCRIPT_CLIENT   312

int afk[MAX_PLAYERS][4];


//==============================================================================
//
// OPEN
//
//==============================================================================
Script AFK_SCRIPT_OPEN OPEN
{
    if (!GetCvar("sv_afk"))
    {
        Log(s:"afklib - \cgoff");
        terminate;
    }
    
    Log(s:"afklib - \cdon");
    
    // Vars
    int x, y, z;
    
    int dmg = GetCvar("sv_afk_dmg");
    if (!dmg) dmg = 25;
    
    int afk_delay = GetCvar("sv_afk_delay");
    if (!afk_delay) afk_delay = 60;
        
    int dist = GetCvar("sv_afk_distance");
    if (!dist) dist = 8;
    dist <<= 16;
    
    // Looping (woohoo !)
    while (True)
    {
        For(int i = 0; i < PlayerTopIndex() + 1; i++)
        {
            // if player is not playing
            if (!PlayerInGame(i))
                continue;
        
            // Get Actual Coords
            x = GetActorX(i + PLAYER_DEFAULT_TID);
            y = GetActorY(i + PLAYER_DEFAULT_TID);
            z = GetActorZ(i + PLAYER_DEFAULT_TID);
        
            // not move ?
            if (afk[i][AFKX] < (x + dist) && afk[i][AFKX] > (x - dist) && 
                afk[i][AFKY] < (y + dist) && afk[i][AFKY] > (y - dist) &&
                afk[i][AFKZ] < (z + dist) && afk[i][AFKZ] > (z - dist))
            {
                if (!afk[i][AFKWARN])
                {
                    // Warning
                    Client_Execute(i, AFK_SCRIPT_CLIENT, 0, 0, 0, 0);
                    afk[i][AFKWARN] = 1;
                }
                // Do damage
                else
                {
                    Client_Execute(i, AFK_SCRIPT_CLIENT, 0, 0, 0, 0);
                    Thing_Damage (i + PLAYER_DEFAULT_TID, dmg, 0);
                }
            }
            // He moved
            else
            {
                afk[i][AFKX] = x;
                afk[i][AFKY] = y;
                afk[i][AFKZ] = z;
                afk[i][AFKWARN] = 0;
            }
        }
    
        delay(afk_delay*35);
    }
}



//==============================================================================
//
// ENTER
//
//==============================================================================
script AFK_SCRIPT_ENTER ENTER
{
    if (!GetCvar("sv_afk"))
        terminate;
        
    Thing_ChangeTID (0, PLAYER_DEFAULT_TID + PlayerNumber());
}



//==============================================================================
//
// CLIENT
//
//==============================================================================
script AFK_SCRIPT_CLIENT (void) CLIENTSIDE
{
    HudMessage(s:"!!! WARNING !!! DON'T BE AFK !!! WARNING !!!"; 
        HUDMSG_PLAIN|HUDMSG_FADEINOUT, ID_AFK, CR_RED, 0.5, 0.5, 3.0);
        
    // Sound
    if (GetCvar("sv_afk_sound"))
        LocalAmbientSound(snd, 127);
}