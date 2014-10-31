//---------
// APP.CPP \
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
//	This file contains the methods to describe the C_APPLICATION class.
//	C_APPLICATION provides an interface to all of the methods used by any
//	PMCLASS windowed application.  This includes some system constants as
//	well.
//
//---------------------------------------------------------------------------

//-------------------
// OS/2 Conditionals \
//---------------------------------------------------------------------------
#define	INCL_DOS
#define	INCL_WIN

//------------------
// Standard Headers \
//---------------------------------------------------------------------------
#include <os2.h>

//-----------------
// PMCLASS Headers \
//---------------------------------------------------------------------------
#include <app.hpp>


//-------------
// Constructor \
//---------------------------------------------------------------------------
//
// Description:
//		This constructor initializes the window manager and create a messsage
//		queue for the application.
//
//	Parameters:
//		none
//
C_APPLICATION::C_APPLICATION( void )
{
	// Initialize the window manager
	hAB = WinInitialize(0);

	// Create a message queu for this application
	hMQ = WinCreateMsgQueue( hAB, 0 );
}


//------------
// Destructor \
//---------------------------------------------------------------------------
//
// Description:
//		This destructor tears down the application.  It destroys the
//		application's message queue and deinitializes the window manager.
//
//	Parameters:
//		none
//
C_APPLICATION::~C_APPLICATION( void )
{
	// Get rid of the application's queue
	WinDestroyMsgQueue( hMQ );

	// Terminate the window manager function
	WinTerminate( hAB );
}


//-------------
// AnchorBlock \
//---------------------------------------------------------------------------
//
// Description:
//		This method returns a value for the application's anchor block
//
//	Parameters:
//		none
//
//	Returns:
//		HAB		- Handle of the app's anchor block
//
HAB C_APPLICATION::AnchorBlock( void )
{
	return hAB;
}


//-----
// Run \
//---------------------------------------------------------------------------
//
// Description:
//		This method is called to start the message loop for the application.
//		It retrives messages from the system queue and dispatches them to the
//		application's message queue.
//
//	Parameters:
//		void
//
//	Returns:
//		void
//
void C_APPLICATION::Run( void )
{
	QMSG	qmsg;

	// Start the message loop
	while( WinGetMsg( hAB, &qmsg, 0L, 0, 0 ) )
	{
		// Dispatch the message to the application queue
		WinDispatchMsg( hAB, &qmsg );
	}
}


//---------------
// DesktopHeight \
//---------------------------------------------------------------------------
//
// Description:
//		This method returns a value for the height of the desktop window in
//		pixels.
//
//	Parameters:
//		none
//
//	Returns:
//		int		- Height of the desktop in pixels
//
int C_APPLICATION::DesktopHeight( void )
{
	RECTL		rc;

	WinQueryWindowRect( HWND_DESKTOP, &rc );
	return rc.yTop;
}


//--------------
// DesktopWidth \
//---------------------------------------------------------------------------
//
// Description:
//		This method returns a value for the width of the desktop window in
//		pixels.
//
//	Parameters:
//		none
//
//	Returns:
//		int		- Width of the desktop in pixels
//
int C_APPLICATION::DesktopWidth( void )
{
	RECTL		rc;

	WinQueryWindowRect( HWND_DESKTOP, &rc );
	return rc.xRight;
}


//------------
// MenuHeight \
//---------------------------------------------------------------------------
//
// Description:
//		This method returns a value for the height of a menu in pixels.
//
//	Parameters:
//		none
//
//	Returns:
//		int		- Height of a menu bar in pixels
//
int C_APPLICATION::MenuHeight( void )
{
	return WinQuerySysValue( HWND_DESKTOP, SV_CYMENU );
}


//----------------
// TitleBarHeight \
//---------------------------------------------------------------------------
//
// Description:
//		This method returns a value for the height of a titlebar in pixels.
//
//	Parameters:
//		none
//
//	Returns:
//		int		- Height of a title bar in pixels
//
int C_APPLICATION::TitleBarHeight( void )
{
	return WinQuerySysValue( HWND_DESKTOP, SV_CYTITLEBAR );
}


//--------------------
// DialogBorderHeight \
//---------------------------------------------------------------------------
//
// Description:
//		This method returns a value for the height of a dialog border in pixels.
//
//	Parameters:
//		none
//
//	Returns:
//		int		- Height (thickness) of a dialog border in pixels
//
int C_APPLICATION::DialogBorderHeight( void )
{
	return WinQuerySysValue( HWND_DESKTOP, SV_CYDLGFRAME );
}

