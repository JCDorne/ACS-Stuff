/*
================================================================================
Frenchies' Chaotic Tuesdays
Share Key library (ACS version, Zdaemon)

Version 1.0

You nee to use a patchinfo lump in order to make it work :
    [*]
    // Error Thing
    Thing_Delete (32000)
    // KEYS
    ThingH_Change (5, *,*,*,*,*,*,*,*,*,FIRST_TAG,  *,*,*,*,*,*)
    ThingH_Change (40,*,*,*,*,*,*,*,*,*,FIRST_TAG+1,*,*,*,*,*,*)
    ThingH_Change (13,*,*,*,*,*,*,*,*,*,FIRST_TAG+2,*,*,*,*,*,*)
    ThingH_Change (38,*,*,*,*,*,*,*,*,*,FIRST_TAG+3,*,*,*,*,*,*)
    ThingH_Change (6, *,*,*,*,*,*,*,*,*,FIRST_TAG+4,*,*,*,*,*,*)
    ThingH_Change (39,*,*,*,*,*,*,*,*,*,FIRST_TAG+5,*,*,*,*,*,*)

By JCD
================================================================================
*/

#library "sharelib"

#include "zcommon.acs"


//==============================================================================
//
//  VARIABLES
//
//==============================================================================

// Keys
#define BLUEKEYCARD         0
#define BLUESKULLKEY        1
#define REDKEYCARD          2
#define REDSKULLKEY         3
#define YELLOWKEYCARD       4
#define YELLOWSKULLKEY      5

// Flags
#define FLAG_BCARD          1
#define FLAG_BSKUL          2
#define FLAG_YCARD          4
#define FLAG_YSKUL          8
#define FLAG_RCARD          16
#define FLAG_RSKUL          32

// stuff
#define ID_SHARE            900
#define PLAYER_DEFAULT_TID  10000
#define snd                 "SND_KEYS"
#define FIRST_TAG           2000

// Scripts numbers
#define KEY_SCRIPT_OPEN     300
#define KEY_SCRIPT_ENTER    301
#define KEY_SCRIPT_RESPAWN  302
#define KEY_SCRIPT_JOIN     303
#define KEY_SCRIPT_CHANGE   304

str KeyList[6] =
{
    "Blue Keycard",
    "Blue Skullkey",
    "Red Keycard",
    "Red Skullkey",
    "Yellow Keycard",
    "Yellow Skullkey"
};

int keyValue = 0;



//==============================================================================
//
//  Keys Sharing
//
//==============================================================================
script KEY_SCRIPT_CHANGE (int key)
{
    str keyclass;
    int color;
    
    // Key unlock
    SetFont("BIGFONT");
    switch (key)
    {
        default : break;
        
        // Blue Card
        case BLUEKEYCARD : 
            keyValue |= FLAG_BCARD;
            keyclass = "BlueCard";
            Color = CR_BLUE;
            break;
        
        // Blue Skull
        case BLUESKULLKEY : 
            keyValue |= FLAG_BSKUL;
            keyclass = "BlueSkull";
            Color = CR_BLUE;
            break;
            
        // Red Card
        case REDKEYCARD : 
            keyValue |= FLAG_RCARD;
            keyclass = "RedCard";
            Color = CR_RED;
            break;
            
        // Red Skull
        case REDSKULLKEY : 
            keyValue |= FLAG_RSKUL;
            keyclass = "RedSkull";
            Color = CR_RED;
            break;
            
        // Yellow Card
        case YELLOWKEYCARD : 
            keyValue |= FLAG_YCARD;
            keyclass = "YellowCard";
            Color = CR_YELLOW;
            break;
            
        // Yellow Skull
        case YELLOWSKULLKEY : 
            keyValue |= FLAG_YSKUL;
            keyclass = "YellowSkull";
            Color = CR_YELLOW;
            break;
    }
    
    HudMessagebold(s:"\cd", n:0, s:"\cj is now sharing the \c-", 
        s:KeyList[key], s:"\cj !"; HUDMSG_PLAIN | HUDMSG_FADEINOUT, 
            ID_SHARE, Color, 0.5, 0.2, 3.0); 
    
    // give the key to everyone
    for(int i = PLAYER_DEFAULT_TID; i <= (PlayerTopIndex() + PLAYER_DEFAULT_TID); i++)
        if (PlayerInGame(i-PLAYER_DEFAULT_TID)) // give only if he is playing
            GiveActorInventory(i, keyclass, 1);
    
    // Sound
    if (GetCvar("sv_sharekeys_sound"))
        AmbientSound(snd, 127);
}


//==============================================================================
//
// OPEN
//
//==============================================================================
script KEY_SCRIPT_OPEN OPEN
{
    if (GetCvar("sv_sharekeys"))
    {
        Thing_SetSpecial (FIRST_TAG, 80, KEY_SCRIPT_CHANGE, 0, BLUEKEYCARD);
        Thing_SetSpecial (FIRST_TAG + 1, 80, KEY_SCRIPT_CHANGE, 0, BLUESKULLKEY);
        Thing_SetSpecial (FIRST_TAG + 2, 80, KEY_SCRIPT_CHANGE, 0, REDKEYCARD);
        Thing_SetSpecial (FIRST_TAG + 3, 80, KEY_SCRIPT_CHANGE, 0, REDSKULLKEY);
        Thing_SetSpecial (FIRST_TAG + 4, 80, KEY_SCRIPT_CHANGE, 0, YELLOWKEYCARD);
        Thing_SetSpecial (FIRST_TAG + 5, 80, KEY_SCRIPT_CHANGE, 0, YELLOWSKULLKEY);
        
        Log(s:"sharelib - \cdon");
        
        terminate;
    }
    
    Log(s:"sharelib - \cgoff");
}


//==============================================================================
//
// Player Enter / Respawn
//
//==============================================================================
script KEY_SCRIPT_ENTER ENTER
{
    if (!GetCvar("sv_sharekeys"))
        terminate;
        
    Thing_ChangeTID (0, PLAYER_DEFAULT_TID + PlayerNumber());
        
    ACS_ExecuteAlways (KEY_SCRIPT_JOIN, 0, 0, 0, 0);
}

script KEY_SCRIPT_RESPAWN RESPAWN
{
    if (!GetCvar("sv_sharekeys"))
        terminate;
        
    Thing_ChangeTID (0, PLAYER_DEFAULT_TID + PlayerNumber());
        
    ACS_ExecuteAlways (KEY_SCRIPT_JOIN, 0, 0, 0, 0);
}

script KEY_SCRIPT_JOIN (void)
{
    // Give all previously owned keys.
    if (keyValue & FLAG_BCARD) GiveInventory("BlueCard", 1);
    if (keyValue & FLAG_BSKUL) GiveInventory("BlueSkull", 1);
    if (keyValue & FLAG_YCARD) GiveInventory("YellowCard", 1);
    if (keyValue & FLAG_YSKUL) GiveInventory("YellowSkull", 1);
    if (keyValue & FLAG_RCARD) GiveInventory("RedCard", 1);
    if (keyValue & FLAG_RSKUL) GiveInventory("RedSkull", 1);
}