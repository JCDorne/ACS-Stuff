/*
================================================================================
Frenchies' Chaotic Tuesdays
Teleport Library

Version 1.0

You nee to use a custom dehacked thing that look like a teleporter smoke in
order to make it work properly.
    
By JCD
================================================================================
*/

#library "telelib"

#include "zcommon.acs"


//==============================================================================
//
// VARIABLES
//
//==============================================================================

#define TELEDEST                0
#define TELEX                   1
#define TELEY                   2
#define TELEZ                   3
#define TELESIZE                4

// Stuff
#define ID_TELE                 910
#define THING_SPAWNED           "HealthBonus"

// scripts
#define TELE_SCRIPT_OPEN        320
#define TELE_SCRIPT_TP          321
#define TELE_SCRIPT_DISPLAY     322
#define TELE_SCRIPT_DEST        323
#define TELE_SCRIPT_SOURCE      324


global int 1:Teleporter[];
int TeleporterIndex = 0;



//==============================================================================
//
// OPEN
//
//==============================================================================
script TELE_SCRIPT_OPEN OPEN
{
    Log(s:"telelib - \cdon");
}



//==============================================================================
//
// TP Player (Use)
//
//==============================================================================
script TELE_SCRIPT_TP (int tele)
{
    SetActorPosition(ActivatorTID(),
        Teleporter[(tele*TELESIZE) + TELEX],
        Teleporter[(tele*TELESIZE) + TELEY], 
        Teleporter[(tele*TELESIZE) + TELEZ], 1);
}



//==============================================================================
//
// Display message (Enter sector)
//
//==============================================================================
script TELE_SCRIPT_DISPLAY (void)
{
    HudMessage(s:"Press use to Teleport !"; HUDMSG_PLAIN, ID_TELE, 
        CR_YELLOW, 0.5, 0.2, 2.0);
}



//==============================================================================
//
// Create destination
//
//==============================================================================
script TELE_SCRIPT_DEST (int x, int y, int z)
{
    Teleporter[(TeleporterIndex*TELESIZE) + TELEDEST] = 1;
    Teleporter[(TeleporterIndex*TELESIZE) + TELEX] = x << 16;
    Teleporter[(TeleporterIndex*TELESIZE) + TELEY] = y << 16;
    Teleporter[(TeleporterIndex*TELESIZE) + TELEZ] = z << 16;
}



//==============================================================================
//
// Create Source
//
//==============================================================================
script TELE_SCRIPT_SOURCE (int x, int y, int z)
{
    if (!Teleporter[TeleporterIndex*TELESIZE + TELEDEST])
    {
        Log(s:"[telelib] : Set destination before !");
        terminate;
    }

    SpawnForced(THING_SPAWNED, x << 16, y << 16, z << 16, UniqueTID (1000, 2000) + (TeleporterIndex*TELESIZE), 0);
    
    int tid1 = UniqueTID (1000, 2000);
    SpawnForced("SecActUse", x << 16, y << 16, z << 16, tid1 + (TeleporterIndex*TELESIZE), 0);
    Thing_SetSpecial(tid1 + (TeleporterIndex*TELESIZE), 226, TELE_SCRIPT_TP, 0, TeleporterIndex);
    
    int tid2 = UniqueTID (1000, 2000);
    SpawnForced("SecActEnter", x << 16, y << 16, z << 16, tid2 + (TeleporterIndex*TELESIZE), 0);
    Thing_SetSpecial(tid2 + (TeleporterIndex*TELESIZE), 226, TELE_SCRIPT_DISPLAY, 0, 0);
    
    Teleporter[(TeleporterIndex*TELESIZE) + TELEDEST] = 2;
    TeleporterIndex++;
}