//-------------
// Definitions \
//------------------------------------------------------------------------
#define INCL_WIN

//---------------
// Include Files \
//------------------------------------------------------------------------
#include <os2.h>
#include "hello.h"

//---------
// WndProc \
//------------------------------------------------------------------------
// Window Procedure
// All messages sent to the main window are processed by this function.
// This function is called by the operating system.
MRESULT EXPENTRY WndProc( HWND hWnd, ULONG msg, MPARAM mp1, MPARAM mp2 )
{
	HPS			hps;
	RECTL		rc;


	switch( msg )
	{
		case WM_PAINT:	// Process paint messages
			// Get a handle to our presentation space
			hps = WinBeginPaint( hWnd, 0L, &rc );

			// Fill our client rectangle with some neutral color
			WinFillRect( hps, &rc, SYSCLR_APPWORKSPACE );
			GpiSetColor( hps, CLR_NEUTRAL );

			// Tell PM that we are finished painting
			WinEndPaint( hps );
			break;

		default:	// Let PM process anything that we don't do here
			return WinDefWindowProc( hWnd, msg, mp1, mp2 );
	}

	// Tell PM that we took care of the message we care about
	return (MRESULT)FALSE;
}

int main( void )
{
	HAB			hAB;
	HMQ			hmq;
	HWND		hWnd;
	HWND		hWndFrame;
	QMSG		qmsg;
	ULONG		flCreate;


	// Initialize the PM interface fr our application
	hAB = WinInitialize(0);

	// Create a message queue for this app.
	hmq = WinCreateMsgQueue( hAB, 0 );

	// Register our window class with the operating system
	WinRegisterClass( hAB, (PSZ)D_APPNAME, (PFNWP)WndProc, CS_SIZEREDRAW, 0 );

	// Create in instance of our application window on the desktop
	flCreate = FCF_TITLEBAR | FCF_SYSMENU | FCF_SIZEBORDER | FCF_BORDER | FCF_MINBUTTON | FCF_MAXBUTTON
										| FCF_TASKLIST | FCF_SHELLPOSITION;
	hWndFrame = WinCreateStdWindow( HWND_DESKTOP, WS_VISIBLE,
		&flCreate, (PSZ)D_APPNAME, (PSZ)"Basic PM Program",
										0, NULLHANDLE, D_ID_WINDOW, &hWnd );

	// Start the message loop to monitor the system queue for any messages
	// That belong to this app.
	while( WinGetMsg( hAB, &qmsg, 0L, 0, 0 ) )
	{
		// Found a message belonging to us, so dispatch it to our window
		// procedure
		WinDispatchMsg( hAB, &qmsg );
	}

	// The user closed our app window, so destroy the window
	WinDestroyWindow( hWndFrame );

	// Disconnect from the system message queue
	WinDestroyMsgQueue( hmq );

	// Deinitialize the PM interface
	WinTerminate( hAB );

	// Return to the operating system
	return 0;
}

