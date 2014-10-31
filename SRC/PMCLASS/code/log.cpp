//-------------
// CONTAIN.CPP \
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
//	This file contains the methods to describe the C_LOG class.  C_LOG manages
//	a controlled strem of debugging output to a file or Multi-line edit window
//	to provide feedback about real-time program problems which are generally
//	impossible to correct with a normal debugger.
//
//---------------------------------------------------------------------------

//-------------------
// OS/2 Conditionals \
//---------------------------------------------------------------------------
#define INCL_DOS
#define INCL_WIN

//------------------
// Standard Headers \
//---------------------------------------------------------------------------
#include <os2.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>

//-----------------
// PMCLASS Headers \
//---------------------------------------------------------------------------
#include <window.hpp>
#include <winstd.hpp>
#include <winchild.hpp>
#include <dialog.hpp>
#include <status.hpp>
#include <mle.hpp>
#include <log.hpp>


DECLARE_MSG_TABLE( xtMsgDebug )
	DECLARE_MSG( PM_CREATE,		C_WINDOW_DEBUG::MsgCreate )
	DECLARE_MSG( DM_DEBUG_ADD,	C_WINDOW_DEBUG::MsgAdd )
	DECLARE_MSG( WM_SIZE,		C_WINDOW_DEBUG::MsgSize )
	DECLARE_MSG( WM_PAINT,		C_WINDOW_STD::MsgPaint )
END_MSG_TABLE


//-------------
// Constructor \
//---------------------------------------------------------------------------
//
// Description:
//		This constructor creates an instance of the C_LOG class.  If the
//		iLogMode is set to 2, then the MLE display window is also created.
//
//	Parameters:
//		szLogFile				- Pointer to debug log filename
//		iLogMode				- Log mode (0=no log, 1=file, 2=file and MLE)
//
C_LOG::C_LOG( char *szLogFile, int iLogMode )
{
	strcpy( szFileName, szLogFile );
	iLogging = iLogMode;
	pxcDebug = NULL;

	if( iLogging == 2 )
	{
		// Create an instance of a debugging window
		pxcDebug = (C_WINDOW_DEBUG *)new C_WINDOW_DEBUG;
		pxcDebug->Register( "DebugConsole" );

		// Set the window characteristics
		pxcDebug->WCF_SizingBorder();
		pxcDebug->WCF_SysMenu();
		pxcDebug->WCF_TaskList();
		pxcDebug->WCF_ShellPosition();
		pxcDebug->WCF_MinButton();
		pxcDebug->WCF_MaxButton();
		pxcDebug->WCF_TitleBar();
	}
}


//-------------
// Destructor \
//---------------------------------------------------------------------------
//
// Description:
//  	This destructor destroys the MLE display window if it was created
//		during the construction process.
//
C_LOG::~C_LOG( void )
{
	if( pxcDebug )
		delete pxcDebug;
}

//--------
// Open() \
//---------------------------------------------------------------------------
//
//	Description:
//		This method opens the debug log.  If first creates a new log file then,
//		if required, displays the MLE window.
//
//	Parameters:
//		none
//
//	Returns:
//		void
//
void C_LOG::Open( void )
{
	// Remove any log file left from a previous run
	DosForceDelete( (PSZ)szFileName );

	if( iLogging )
	{
		hLogFile = fopen( szFileName, "w" );
		if( iLogging == 2 )
		{
			pxcDebug->Create( 99999, "PMCLASS Debugging Window" );
			pxcDebug->Show();
		}
		Write( "Log Open" );
	}
}


//---------
// Close() \
//---------------------------------------------------------------------------
//
//	Description:
//		This method closes the debug log.  If the MLE window is currently
//		visible it will be hidden.
//
//	Parameters:
//		none
//
//	Returns:
//		void
//
void C_LOG::Close( void )
{
	if( iLogging )
	{
		Write( "Log Closed" );
		fclose( hLogFile );
		if( iLogging == 2 )
			pxcDebug->Hide();
	}
}


//---------
// Write() \
//---------------------------------------------------------------------------
//
//	Description:
//		This method accepts a printf-style string and will format and write
//		the string to the log file and optionally to the MLE window.  A timestamp
//		will be refixed to all strings written.
//
//	Parameters:
//		szFormat		- a printf-style variable arguement list
//
//	Returns:
//		void
//
void C_LOG::Write( char *szFormat, ... )
{
	DATETIME	dt;
	va_list		xtArgs;
	char		szString[4096];
	char		szTemp[80];


	if( iLogging )
	{
		va_start( xtArgs, szFormat );
		vsprintf( szString, szFormat, xtArgs );
		va_end( xtArgs );

		DosGetDateTime( &dt );

		fprintf( hLogFile, "%02d:%02d:%02d->", dt.minutes, dt.seconds,
															dt.hundredths );
		fprintf( hLogFile, "%s\n", szString );
		fflush( hLogFile );

		if( iLogging == 2 )
		{
			sprintf( szTemp, "%02d:%02d:%02d->", dt.minutes,
												dt.seconds, dt.hundredths );
			pxcDebug->SendMsg( DM_DEBUG_ADD, szTemp, 0 );
			strcat( szString, "\n" );
			pxcDebug->SendMsg( DM_DEBUG_ADD, szString, 0 );
		}
	}
}


//-------------
// Constructor \
//---------------------------------------------------------------------------
//
//	Description:
//		This constructor creates an instance of the MLE window.
//
//	Parameters:
//		none
//
//	Returns:
//		void
//
C_WINDOW_DEBUG::C_WINDOW_DEBUG( void ) : C_WINDOW_STD( xtMsgDebug )
{
}


//-------------
// Constructor \
//---------------------------------------------------------------------------
//
//	Description:
//		This destructor destroys the MLE window.  It frees up the MLE object
//		used by the window.
//
//	Parameters:
//		none
//
//	Returns:
//		void
//
C_WINDOW_DEBUG::~C_WINDOW_DEBUG( void )
{
	delete pxcDebugMLE;
}


//-----------
// MsgCreate \
//---------------------------------------------------------------------------
// Event:		PM_CREATE
// Cause:		Issued by OS when window is created
// Description:	This method gets called when the window is initially created.
//				It initializes all of the visual aspects of the class.
void *C_WINDOW_DEBUG::MsgCreate( void *mp1, void *mp2 )
{
	// Create the MLE
	pxcDebugMLE = (C_MLE *)new C_MLE( this, 99998 );
	pxcDebugMLE->Delete( 0, 30 );

	// Set the desired color and font
	SetFont( "Helv", 8 );
	SetBackgroundColor( 0, 0, 0 );
	SetForegroundColor( 255, 255, 255 );

	return FALSE;
}


//--------
// MsgAdd \
//---------------------------------------------------------------------------
// Event:		DM_DEBUG_ADD
// Cause:		Issued by Write() method when new text is to be written to MLE
// Description:	This method is called when new text needs to be written to the
//				MLE control.  The mp1 parameter points to the text to insert.
void *C_WINDOW_DEBUG::MsgAdd( void *mp1, void *mp2 )
{
	pxcDebugMLE->Insert( (char *)mp1 );

	return FALSE;
}


//---------
// MsgSize \
//---------------------------------------------------------------------------
// Event:		WM_SIZE
// Cause:		Issued by OS when window is resized
// Description:	This method is called any time PM decides the window needs
//				to be resized.  It determines the new window dimensions and
//				resizes the visual components accordingly.
void *C_WINDOW_DEBUG::MsgSize( void *mp1, void *mp2 )
{
	int		iCX;
	int		iCY;


	// Determine the size of the client area
	GetSize( &iCX, &iCY );

	// Draw the MLE Window
	pxcDebugMLE->SetSizePosition( 0, 0, iCX, iCY );

	return FALSE;
}


