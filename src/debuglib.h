/*
================================================================================
Frenchies' Chaotic Tuesdays
Debug Library (ACS version, Zdaemon)

Version 1.0

By JCD
================================================================================
*/


#define INFO	0
#define WARNING 1
#define ERROR	2

str lvl_error[3] =
{
	"\cf[INFO]\c- ",		// Gold
	"\ci[WARNING]\c- ",		// Orange
	"\cg[ERROR]\c- "		// Red
};



//==============================================================================
//
// Simple Debug Message
//
//==============================================================================
function void debug (int lvlerror, str msg)
{
	if (!GetCvar("sv_debug"))
		return;
	
	Log(s:lvl_error[lvlerror], s:" - ", s:msg);
}


//==============================================================================
//
// Debug int
//
//==============================================================================
function void debug_int (int lvlerror, str msg, int valeur)
{
	if (!GetCvar("sv_debug"))
		return;
	
	Log(s:lvl_error[lvlerror], s:" - ", s:msg, s:" : ", d:valeur);
}


//==============================================================================
//
// Debug Fixed
//
//==============================================================================
function void debug_fixed (int lvlerror, str msg, int valeur)
{
	if (!GetCvar("sv_debug"))
		return;
	
	Log(s:lvl_error[lvlerror], s:" - ", s:msg, s:" : ", f:valeur);
}


//==============================================================================
//
// Debug Char
//
//==============================================================================
function void debug_char (int lvlerror, str msg, int valeur)
{
	if (!GetCvar("sv_debug"))
		return;
	
	Log(s:lvl_error[lvlerror], s:" - ", s:msg, s:" : ", c:valeur);
}


//==============================================================================
//
// Debug Bool
//
//==============================================================================
function void debug_bool (int lvlerror, str msg, int valeur)
{
	if (!GetCvar("sv_debug"))
		return;

	if (valeur > 0)
		Log(s:lvl_error[lvlerror], s:" - ", s:msg, s:" : True");
	else
		Log(s:lvl_error[lvlerror], s:" - ", s:msg, s:" : False");
}


//==============================================================================
//
// Debug String
//
//==============================================================================
function void debug_str (int lvlerror, str msg, str valeur)
{
	if (!GetCvar("sv_debug"))
		return;
	
	Log(s:lvl_error[lvlerror], s:" - ", s:msg, s:" : ", s:valeur);
}