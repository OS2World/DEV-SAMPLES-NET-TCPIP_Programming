//------------
// STATUS.CPP \
//---------------------------------------------------------------------------
//
//	All information contained in this file is Copyright (c) 1994-1995 by
//	NeoLogic Inc.  All Rights Reserved
//
//	This source code shall not be implemented, duplicated or used as the basis
//	for any product without prior written consent by NeoLogic Inc.
//
// Description:
//
//	This file contains the methods to describe the C_STATUS class.
//	C_STATUS provides a class used to create a status text line within a
//	window.  It can accept printf-style string for output.
//
//---------------------------------------------------------------------------

//-------------------
// OS/2 Conditionals \
//---------------------------------------------------------------------------
#define INCL_DOS
#define INCL_WIN
#define INCL_GPI

//------------------
// Standard Headers \
//---------------------------------------------------------------------------
#include <os2.h>
#include <process.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

//-----------------
// PMCLASS Headers \
//---------------------------------------------------------------------------
#include <window.hpp>
#include <winchild.hpp>
#include <status.hpp>


//-------------
// Constructor \
//---------------------------------------------------------------------------
//
// Description:
//		This constructor creates an instance of the C_STATUS class.  It sets
//		the initial font and color for the window and configures the test
//		limit at 256 characters.
//
//	Parameters:
//		pxcParentObj		- Pointer to owner object window
//
C_STATUS::C_STATUS( C_WINDOW *pxcParentObj ) : C_WINDOW_CHILD( pxcParentObj, 0 )
{
	// Create the status window
	ClassName( WC_ENTRYFIELD );
	Create( D_ID_STATUS, WS_VISIBLE|ES_READONLY, "", 0, 20, 90, 30 );

	// Limit the line to 265 characters
	PostMsg( EM_SETTEXTLIMIT, MPFROMSHORT( 256 ), 0 );

	// Set the default font and colors for the status line
	SetFont( "Helv", 10 );
	SetBackgroundColor( 204, 204, 204 );
	SetForegroundColor( 0, 0, 0 );
}


//------
// Text \
//---------------------------------------------------------------------------
//
//	Description:
//		This method accepts a variable argument printf-style string and after
//		formatting, it will display the string within the status line window.
//
//	Parameters:
//		szFormat			- Pointer to printf-style string
//
//	Returns:
//		void
//
void C_STATUS::Text( char *szFormat, ... )
{
	char		szString[1024];
	va_list		xtArgs;


	// Format the text
	va_start( xtArgs, szFormat );
	vsprintf( szString, szFormat, xtArgs );
	va_end( xtArgs );

	// Set the text in the status window
	SetText( szString );
}

