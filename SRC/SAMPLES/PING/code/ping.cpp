//-------------------
// OS/2 Conditionals \
//---------------------------------------------------------------------------
#define	INCL_DOS
#define	INCL_WIN

//------------------
// Standard Headers \
//---------------------------------------------------------------------------
#include <os2.h>
#include <stdio.h>
#include <string.h>
#include <process.h>

//-----------------
// PMCLASS Headers \
//---------------------------------------------------------------------------
#include <app.hpp>
#include <window.hpp>
#include <winstd.hpp>
#include <winchild.hpp>
#include <dialog.hpp>
#include <edit.hpp>
#include <status.hpp>
#include <button.hpp>
#include <tbar.hpp>
#include <mle.hpp>
#include <contain.hpp>
#include <log.hpp>

//-----------------
// NVCLASS Headers \
//---------------------------------------------------------------------------
#include <thread.hpp>
#include <threadpm.hpp>

//------------------
// NETCLASS Headers \
//---------------------------------------------------------------------------
#include <net.hpp>
#include <netping.hpp>

//---------------------
// Application Headers \
//---------------------------------------------------------------------------
#include "rc.hpp"
#include <tbartop.hpp>
#include <address.hpp>
#include <about.hpp>
#include <ping.hpp>


//-------------
// Global Data \
//---------------------------------------------------------------------------
C_APPLICATION		xcApp;
C_WINDOW_MAIN		xcWindow;


//---------------------------
// Main Window Message Table \
//---------------------------------------------------------------------------
DECLARE_MSG_TABLE( xtMsgMain )
	DECLARE_MSG( PM_CREATE,		C_WINDOW_MAIN::MsgCreate )
	DECLARE_MSG( PM_DONE,		C_WINDOW_MAIN::MsgDone )
	DECLARE_MSG( WM_CLOSE,		C_WINDOW_MAIN::MsgClose )
	DECLARE_MSG( WM_DESTROY,	C_WINDOW_MAIN::MsgDestroy )
	DECLARE_MSG( WM_SIZE,		C_WINDOW_MAIN::MsgSize )
	DECLARE_MSG( WM_CONTROL,	C_WINDOW_MAIN::MsgControl )
	DECLARE_MSG( WM_PAINT,		C_WINDOW_STD::MsgPaint )
END_MSG_TABLE


//---------------------------
// Main Window Command Table \
//---------------------------------------------------------------------------
DECLARE_COMMAND_TABLE( xtCommandMain )
	DECLARE_COMMAND( DM_EXIT,			C_WINDOW_MAIN::CmdExit )
	DECLARE_COMMAND( DM_CONNECT,		C_WINDOW_MAIN::CmdConnect )
	DECLARE_COMMAND( DM_INFO,			C_WINDOW_MAIN::CmdHelpInfo )
END_MSG_TABLE


//------------
// PingThread \
//---------------------------------------------------------------------------
void _Optlink PingThread( void *pvData )
{
	char			szString[256];
	BYTE			byPacket[100];
	int				iResult;
	C_WINDOW_MAIN	*pxcThis;
	C_THREAD_PM		*pxcThread;


	// Get a point to the main window object
	pxcThread = (C_THREAD_PM *)pvData;
	pxcThis = (C_WINDOW_MAIN *)pxcThread->ThreadData();

	// Create a PM process for this thread
	pxcThread->InitializeThread();

	// Create an instance of a PING networking object
	C_CONNECT_PING xcPingPort( 1, pxcThis->ServerAddress() );

	// Open the Ping channel
	iResult = xcPingPort.C_CONNECT_PING::Open();
	if( iResult == D_NET_OK )
	{
		// Loop until the user stops this thread
		while( !pxcThis->ExitFlag() )
		{
			// Transmit a PING to the other end of the connection
			memset( byPacket, 0xff, 100 );
			iResult = xcPingPort.PingTx( byPacket, 64 );
			if( iResult == D_NET_OK )
			{
				// Receive the ping results
				memset( byPacket, 0xff, 100 );
				strcpy( szString, "" );
				xcPingPort.PingRx( byPacket, szString );

				// Print the ping results to the MLE
				if( strlen( szString ) )
				{
					strcat( szString, "\r\n" );
					pxcThis->MLE()->Insert( szString );
				}

				// Sleep for a second before we ping again
				DosSleep( 1000 );
			}
			else
			{
				// Tell the user that something is amiss
				WinMessageBox( HWND_DESKTOP, pxcThis->Window(),
						"PING transmission error - aborting", D_APPNAME,
													0, MB_OK | MB_ICONHAND );

				// Force the window to close
				pxcThis->ExitFlag( 1 );
			}
		}

		// Close the PING port
		xcPingPort.Close();
	}
	else
	{
		// Tell the user that we couldn't find the selected host
		WinMessageBox( HWND_DESKTOP, pxcThis->Window(),
			"Couldn't Open PING socket", D_APPNAME, 0,
												MB_OK | MB_ICONHAND );
	}

	// Terminate the thread
	pxcThread->TerminateThread();
}


//-------------
// Constructor \
//---------------------------------------------------------------------------
C_WINDOW_MAIN::C_WINDOW_MAIN( void ) : C_WINDOW_STD( xtMsgMain )
{
	// Enable the required handlers for this window
	CommandTable( xtCommandMain );
}


//-----------
// MsgCreate \
//---------------------------------------------------------------------------
// Event:	WM_CREATE
// Cause:	Issued by OS when window is created
//
void *C_WINDOW_MAIN::MsgCreate( void *mp1, void *mp2 )
{

	// Create a status bar to display miscellaneous data
	xcStatus = (C_STATUS *) new C_STATUS( this );

	// Create a toolbar control
	xcTBar = (C_TOOLBAR_TOP *)new C_TOOLBAR_TOP( this, xcStatus );

	// Create a container control
	xcMLE = (C_MLE *)new C_MLE( this, D_ID_MLE );

	// Set the color and font for the container

	xcMLE->SetFont( "Helv", 10 );
	xcMLE->SetForegroundColor( 255, 255, 255 );
	xcMLE->SetBackgroundColor( 0, 0, 128 );


	// Initialize the instance attributes
	iMustExit = 0;
	strcpy( szServerAddress, "" );

	// Make the window visible
	Show();

	return FALSE;
}


//----------
// MsgClose \
//---------------------------------------------------------------------------
// Event:	WM_CLOSE
// Cause:	Issued by OS when window is closed
//
void *C_WINDOW_MAIN::MsgClose( void *mp1, void *mp2 )
{
	// Applcation was told to close, so post a QUIT message to the OS
	PostMsg( WM_QUIT, 0, 0 );
	return FALSE;
}


//------------
// MsgDestroy \
//---------------------------------------------------------------------------
// Event:	WM_DESTROY
// Cause:	Issued by OS when window is destroyed
//
void *C_WINDOW_MAIN::MsgDestroy( void *mp1, void *mp2 )
{
	// Get rid of the synamic memory we allocated
	delete	xcStatus;
	delete	xcTBar;

	return FALSE;
}


//---------
// MsgSize \
//---------------------------------------------------------------------------
// Event:	WM-SIZE
// Cause:	Issued by OS when window is resized
//
void *C_WINDOW_MAIN::MsgSize( void *mp1, void *mp2 )
{
	int		iCX;
	int		iCY;


	// Determine the size of the client area
	C_WINDOW::GetSize( &iCX, &iCY );

	// Strect the toolbar and status windows so they use the entire window
	iCY -= 40;
	xcTBar->SetSizePosition( 0, iCY, iCX, 40 );
	iCY -= 25;
	xcStatus->SetSizePosition( 0, iCY, iCX, 25 );

	// Make the MLE take up the remainder of the client area space
	xcMLE->SetSizePosition( 0, 0, iCX, iCY );

	return FALSE;
}


//------------
// MsgControl \
//---------------------------------------------------------------------------
// Event:	WM_CONTROL
// Cause:	Issued by OS for control functions (Toolbar interaction)
//
void *C_WINDOW_MAIN::MsgControl( void *mp1, void *mp2 )
{
	switch( SHORT1FROMMP( mp1 ) )
	{
		case D_TOP_TBAR:
			xcTBar->Control( (ULONG)mp1 );
			break;
	}
	return FALSE;
}


//---------
// MsgDone \
//---------------------------------------------------------------------------
// Event:	PM_DONE
// Cause:	Issued by the connection dialog when a new connection has been
//			selected.  Also issued by main() if user specifies a command line
//			address
void *C_WINDOW_MAIN::MsgDone( void *mp1, void *mp2 )
{
	char	szString[256];


	// Format the window title correctly
	xcStatus->Text( "Pinging %s", (char *)mp1 );

	// Get the selected server from the caller
	strcpy( szServerAddress, (char *)mp1 );

	// Terminate the existing thread if it is running
	iMustExit = 1;
	xcPingThread.WaitIndefinite();
	iMustExit = 0;

	sprintf( szString, "\r\nPinging %s\r\n", szServerAddress );
	xcMLE->Insert( szString );

	// Begin a thread to ping servers
	xcPingThread.Create( PingThread, 40000, this );

	return FALSE;
}


//---------
// CmdExit \
//---------------------------------------------------------------------------
// Event:	DM_EXIT
// Cause:	User selects the exit menu option
//
void *C_WINDOW_MAIN::CmdExit( void *mp1, void *mp2 )
{
	// If the user picks "Exit" from the main menu, close the program
	SendMsg( WM_CLOSE, 0, 0 );
	return FALSE;
}


//------------
// CmdConnect \
//---------------------------------------------------------------------------
// Event:	DM_CONNECT
// Cause:	User selects the connect option or toolbar button
//
void *C_WINDOW_MAIN::CmdConnect( void *mp1, void *mp2 )
{
	// Create an instance of the address dialog
	C_DIALOG_ADDRESS xcAddress( this, D_DIALOG_ADDRESS );

	// Tell the window who its parent is so it can talk back
	xcAddress.SendMsg( PM_PARENT, this, 0 );

	// Process the dialog
	xcAddress.Process();

	return FALSE;
}


//-------------
// CmdHelpInfo \
//---------------------------------------------------------------------------
// Event:	DM_INFO
// Cause:	User selects the Product Information menu option
//
void *C_WINDOW_MAIN::CmdHelpInfo( void *mp1, void *mp2 )
{
	// Create an instance of the about dialog
	C_DIALOG_ABOUT	xcAbout( this, D_DIALOG_ABOUT );
	return FALSE;
}


void main( int iArgCount, char *szArgV[] )
{

	// Register the application window
	xcWindow.Register( "Ping - Control Panel" );

	// Create a new program window
	xcWindow.WCF_Standard();
	xcWindow.Create( ID_WINDOW, "Ping" );

	// Process any command line arguments
	if( iArgCount > 1 )
	{
		// Pass in the command-line arguement
		xcWindow.PostMsg( PM_DONE, szArgV[1], 0 );
	}

	// Start the message loop
	xcApp.Run();
}
