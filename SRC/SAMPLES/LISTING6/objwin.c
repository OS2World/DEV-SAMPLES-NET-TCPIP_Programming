//-------------
// Definitions \
//------------------------------------------------------------------------
#define INCL_DOS
#define INCL_WIN

//---------------
// Include Files \
//------------------------------------------------------------------------
#include <os2.h>

#include <stdio.h>
#include <stdlib.h>
#include <process.h>

#include "objwin.h"



void _Optlink ThreadObject( void *pvData )
{
	HAB			hABThread;
	HMQ			hMQThread;
	QMSG        qmsg;
	T_SHARE 	*pxtWnd;


	// Create a PM process for this thread
	hABThread = WinInitialize( 0 );
	hMQThread = WinCreateMsgQueue( hABThread, 0 );

	pxtWnd = (T_SHARE *)pvData;

	// Register our object window class with the operating system
	WinRegisterClass( hABThread, "TestObjWindow", (PFNWP)ObjWndProc,
												0, sizeof( T_SHARE *) );

	// Create an instance of the object window
	pxtWnd->hWndObj = WinCreateWindow( HWND_OBJECT, "TestObjWindow", "",
		0, 0, 0, 0, 0, HWND_OBJECT, HWND_BOTTOM, 0, pxtWnd, NULL );

	// Start the message loop to monitor the system queue for any messages
	// That belong to this app.
	while( WinGetMsg( hABThread, &qmsg, 0L, 0, 0 ) )
	{
		// Found a message belonging to us, so dispatch it to our window
		// procedure
		WinDispatchMsg( hABThread, &qmsg );
	}

	// Tell the client window to quit
	WinPostMsg( pxtWnd->hWnd, WM_QUIT, 0, 0 );

	WinDestroyWindow( pxtWnd->hWndObj );

	WinDestroyMsgQueue( hMQThread );
}


MRESULT EXPENTRY ObjWndProc( HWND hWnd, ULONG msg, MPARAM mp1, MPARAM mp2 )
{
	HPS			hps;
	RECTL		rc;
	T_SHARE		*pxtWnd;


	switch( msg )
	{
		case WM_CREATE:
			// Save a point to the global data in the object window data
			// area
			WinSetWindowULong( hWnd, QWL_USER, (ULONG)mp1 );
			break;

		case WM_SLEEP:
			DosSleep( 1000 );
			DosBeep( 100, 100 );
			DosSleep( 1000 );
			DosBeep( 300, 100 );
			DosBeep( 50, 100 );
			break;

		case WM_CLOSE:
			WinPostMsg( hWnd, WM_QUIT, 0, 0 );
			break;

		default:
			return WinDefWindowProc( hWnd, msg, mp1, mp2 );
	}
	return (MRESULT)FALSE;
}


//---------
// WndProc \
//------------------------------------------------------------------------
MRESULT EXPENTRY WndProc( HWND hWnd, ULONG msg, MPARAM mp1, MPARAM mp2 )
{
	HPS			hps;
	RECTL		rc;
	T_SHARE		*pxtWnd;


	switch( msg )
	{
		case WM_CREATE:
			pxtWnd = (T_SHARE *)malloc( sizeof( T_SHARE ) );
			WinSetWindowULong( hWnd, QWL_USER, (ULONG)pxtWnd );
			pxtWnd->hWndFrame = WinQueryWindow( QW_PARENT, hWnd );
			pxtWnd->hWnd = hWnd;
			pxtWnd->hWndObj = NULLHANDLE;

#ifdef __BORLANDC__
			_beginthread( ThreadObject, 40000, pxtWnd );
#else
			_beginthread( ThreadObject, 0, 40000, pxtWnd );
#endif
			break;

		case WM_COMMAND:
			pxtWnd = (T_SHARE *)WinQueryWindowULong( hWnd, QWL_USER );
			switch( COMMANDMSG( &msg )->cmd )
			{
				case DM_DO_SLEEP:
					WinPostMsg( pxtWnd->hWndObj, WM_SLEEP, 0, 0 );
					break;
			}
			break;

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
	WinRegisterClass( hAB, (PSZ)D_APPNAME, (PFNWP)WndProc,
										CS_SIZEREDRAW, sizeof( T_SHARE * ) );

	// Create an instance of our application window on the desktop
	flCreate = FCF_STANDARD;
	hWndFrame = WinCreateStdWindow( HWND_DESKTOP, WS_VISIBLE,
		&flCreate, (PSZ)D_APPNAME, (PSZ)"ObjectWindow PM Program",
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

