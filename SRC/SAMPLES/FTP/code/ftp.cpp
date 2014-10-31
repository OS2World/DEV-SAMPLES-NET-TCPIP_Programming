//---------
// FTP.CPP \
//---------------------------------------------------------------------------
//
//	All information contained in this file is Copyright (c) 1994-1995 by
//	Steven Gutz.  All Rights Reserved
//
//	This source code shall not be implemented, duplicated or used as the basis
//	for any product without prior written consent by the author
//
// Description:
//
//	This file contains the source code for the sample NNTP news client.  More
//  specifically it contains the main() and all of the methods of the
//	C_WINDOW_MAIN class.
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
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
#include <pushbtn.hpp>
#include <menu.hpp>
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
#include <semev.hpp>
#include <ini.hpp>
#include <iniuser.hpp>

//------------------
// NETCLASS Headers \
//---------------------------------------------------------------------------
#include <net.hpp>
#include <netftp.hpp>

//---------------------
// Application Headers \
//---------------------------------------------------------------------------
#include "rc.hpp"
#include <tbartop.hpp>
#include <ftp.hpp>

#define		D_FTP_PORT			21		// Standard FTP port number

//-------------
// Global Data \
//---------------------------------------------------------------------------
C_APPLICATION	xcApp;		// Application instance
C_WINDOW_MAIN	xcWindow;	// Main window instance
C_LOG	 		*pxcLog;	// Debug log instance


//---------------------------
// Main Window Message Table \
//---------------------------------------------------------------------------
DECLARE_MSG_TABLE( xtMsgMain )
	DECLARE_MSG( PM_CREATE,				C_WINDOW_MAIN::MsgCreate )
	DECLARE_MSG( WM_CLOSE,				C_WINDOW_MAIN::MsgClose )
	DECLARE_MSG( WM_SIZE,				C_WINDOW_MAIN::MsgSize )
	DECLARE_MSG( WM_CONTROL,			C_WINDOW_MAIN::MsgControl )
	DECLARE_MSG( PM_CONNECT,			C_WINDOW_MAIN::MsgConnect )
	DECLARE_MSG( WM_PAINT,				C_WINDOW_STD::MsgPaint )
END_MSG_TABLE


//---------------------------
// Main Window Command Table \
//---------------------------------------------------------------------------
DECLARE_COMMAND_TABLE( xtCommandMain )
	DECLARE_COMMAND( ID_ENTERBUTTON,	C_WINDOW_MAIN::CmdCommand )
	DECLARE_COMMAND( DM_EXIT,			C_WINDOW_MAIN::CmdExit )
	DECLARE_COMMAND( DM_INFO,			C_WINDOW_MAIN::CmdHelpInfo )
END_MSG_TABLE


//---------------
// ConnectThread \
//---------------------------------------------------------------------------
//
//	Description:
//		This thread function creates a connection to the news server.  If
//		successful, it sends a PM_CONNECT message back to the main window
//		class
//
void _Optlink ConnectThread( void *pvData )
{
	C_WINDOW_MAIN	*pxcThis;
	C_THREAD_PM		*pxcThread;


	// Get a point to the main window object
	pxcThread = (C_THREAD_PM *)pvData;
	pxcThis = (C_WINDOW_MAIN *)pxcThread->ThreadData();

	// Create a PM process for this thread
	pxcThread->InitializeThread();

	pxcThis->Status()->Text( "Connecting to %s...", pxcThis->ServerAddress() );

	// Create an instance of an FTP connection
	pxcThis->pxcConnection = (C_CONNECT_FTP *)new C_CONNECT_FTP(
					pxcThis->ServerAddress(), D_FTP_PORT, pxcThis->Console() );

	// Open a connection to the server
	pxcThis->pxcConnection->Open();

	// Send a connect message to the main window
	pxcThis->PostMsg( PM_CONNECT, 0, 0 );

	// Terminate the thread
	pxcThread->TerminateThread();
}


void _Optlink InterpretFTPCommand( void *pvData )
{
	char			szData[256];
	char			szCommand[256];
	char			szFTPCommand[256];
	char			*szOption;
	C_WINDOW_MAIN	*pxcThis;
	C_THREAD_PM		*pxcThread;


	// Get a point to the main window object
	pxcThread = (C_THREAD_PM *)pvData;
	pxcThis = (C_WINDOW_MAIN *)pxcThread->ThreadData();

	// Create a PM process for this thread
	pxcThread->InitializeThread();

	// Get the contents of the edit line
	pxcThis->Command()->GetText( szFTPCommand, 256 );

	// Extract the FTP command
	strcpy( szCommand, szFTPCommand );
	if( strstr( szCommand, " " ) )
		*strstr( szCommand, " " ) = 0;

	// Get the remainder of the command line (options)
	szOption = szFTPCommand + strlen( szCommand );
	while( isspace( *szOption ) )
		szOption++;

	// Process the FTP commands using RFC 959 style
	if( strcmpi( szCommand, "SYST" ) == 0 )
		pxcThis->pxcConnection->SYST();
	if( strcmpi( szCommand, "SITE" ) == 0 )
		pxcThis->pxcConnection->SITE( szOption );
	if( strcmpi( szCommand, "ACCT" ) == 0 )
		pxcThis->pxcConnection->ACCT( szOption );
	if( strcmpi( szCommand, "USER" ) == 0 )
		pxcThis->pxcConnection->USER( szOption );
	if( strcmpi( szCommand, "PASS" ) == 0 )
		pxcThis->pxcConnection->PASS( szOption );
	if( strcmpi( szCommand, "TYPE" ) == 0 )
		pxcThis->pxcConnection->TYPE( szOption );
	if( strcmpi( szCommand, "PWD" ) == 0 )
		pxcThis->pxcConnection->PWD( szData );
	if( strcmpi( szCommand, "CWD" ) == 0 )
		pxcThis->pxcConnection->CWD( szOption );
	if( strcmpi( szCommand, "RMD" ) == 0 )
		pxcThis->pxcConnection->RMD( szOption );
	if( strcmpi( szCommand, "MKD" ) == 0 )
		pxcThis->pxcConnection->MKD( szOption );
	if( strcmpi( szCommand, "DELE" ) == 0 )
		pxcThis->pxcConnection->DELE( szOption );
	if( strcmpi( szCommand, "DIR" ) == 0 )
		pxcThis->pxcConnection->DIR( szOption, "dir.txt" );
	if( strcmpi( szCommand, "RETR" ) == 0 )
		pxcThis->pxcConnection->RETR( szOption, szOption );
	if( strcmpi( szCommand, "STOR" ) == 0 )
		pxcThis->pxcConnection->STOR( szOption, szOption );
	if( strcmpi( szCommand, "QUIT" ) == 0 )
	{
		// Send the QUIT command
		pxcThis->pxcConnection->QUIT();

		// Close the data connection
		pxcThis->pxcConnection->Close();

		// Since the connection is now dead, close the application
		pxcThis->PostMsg( WM_CLOSE, 0, 0 );
	}

	// Re-enable the ENTER button so the user can give us another command
	pxcThis->EnterButton()->Enable( TRUE );

	// Reset the command buffer to a NULL
	pxcThis->Command()->SetText( "" );

	// Terminate the thread
	pxcThread->TerminateThread();
}


//-------------
// Constructor \
//---------------------------------------------------------------------------
//
// Description:
//		This constructor initializes the main window class for the editor.
//		It zeroes the class attributes and sets up up the command handler and
//		server address.
//
//	Parameters:
//		szServer		- Address of the news server
//
C_WINDOW_MAIN::C_WINDOW_MAIN( void ) : C_WINDOW_STD( xtMsgMain )
{
	// Initialize all child objects
	pxcTBar = 0;
	pxcStatus = 0;
	pxcConsole = 0;
	pxcCommand = 0;

	// Enable the required command handler for this window
	CommandTable( xtCommandMain );

	// Set the server address
	strcpy( szServerAddress, "" );
}


//-------------
// Destructor \
//---------------------------------------------------------------------------
//
//	Description:
//		The destructor frees up all of the dynamically allocated objects
//		and attributes used by this instance.
//
C_WINDOW_MAIN::~C_WINDOW_MAIN( void )
{
	pxcLog->Write( "~C_WINDOW_MAIN:Start" );

	// Free up dynamic window space.
	delete pxcTBar;
	delete pxcStatus;
	delete pxcConsole;
	delete pxcCommand;
	delete pxcEnterButton;

	pxcLog->Write( "~C_WINDOW_MAIN:End" );
}


//-----------
// SetServer \
//---------------------------------------------------------------------------
//
//	Description:
//		This method sets the news server specified by the user on the command
//		line when the application was started
//
//	Parameters:
//		szServer		-  Address of news server
//
void C_WINDOW_MAIN::SetServer( char *szServer )
{
	// Set the server address
	strcpy( szServerAddress, szServer );

	// Set the window title
	SetTitle( szServer );
}


//-----------
// MsgCreate \
//---------------------------------------------------------------------------
// Event:		WM_CREATE
// Cause:		Issued by OS when window is created
// Description:	This method gets called when the window is initially created.
//				It initializes all of the visual aspects of the class.
//
void *C_WINDOW_MAIN::MsgCreate( void *mp1, void *mp2 )
{
	char	szX[10];
	char	szY[10];
	char	szW[10];
	char	szL[10];
	char	szFont[80];
	char	szFontSize[10];
	char	szBColor[80];
	char	szFColor[80];


	// Create a status bar to display miscellaneous data
	pxcStatus = (C_STATUS *) new C_STATUS( this );

	// Create a toolbar control
	pxcTBar = (C_TOOLBAR_TOP *)new C_TOOLBAR_TOP( this, pxcStatus );

	// Create a multiline edit control for the console window
	pxcConsole = (C_MLE *)new C_MLE( this, ID_CONSOLE );

	// Create a command entry window
	pxcCommand = (C_EDIT *)new C_EDIT( this, ID_COMMAND );
	pxcCommand->SetText( "" );

	pxcEnterButton = (C_PUSHBUTTON *)new C_PUSHBUTTON( this,
												ID_ENTERBUTTON, 0, "Enter" );

	// Load parameters out of the INI file
	C_INI_USER xcIni( "BookFTP" );
	xcIni.Open();
	xcIni.Read( "MainX", szX, "0", 10 );
	xcIni.Read( "MainY", szY, "0", 10 );
	xcIni.Read( "MainW", szW, "100", 10 );
	xcIni.Read( "MainL", szL, "100", 10 );
	xcIni.Read( "ConsoleFont", szFont, "System Monospaced", 80 );
	xcIni.Read( "ConsoleFontSize", szFontSize, "10", 80 );
	xcIni.Read( "ConsoleBColor", szBColor, "000,000,000", 80 );
	xcIni.Read( "ConsoleFColor", szFColor, "255,255,255", 80 );
	xcIni.Close();

	// Set the font in the MLE
	pxcConsole->SetFont( szFont, atoi( szFontSize ) );

	// Set the MLE colors
	pxcConsole->SetForegroundColor( atoi( &szFColor[0] ),
								atoi( &szFColor[4] ), atoi( &szFColor[8] ) );
	pxcConsole->SetBackgroundColor( atoi( &szBColor[0] ),
								atoi( &szBColor[4] ), atoi( &szBColor[8] ) );

	// Make the window look like a control panel
	SetSizePosition( atoi( szX ), atoi( szY ), atoi( szW ), atoi( szL ) );

	// Make the window visible
	Show();

	// Prevent the user from entering commands until we're ready
	pxcEnterButton->Enable( FALSE );

	// Start a thread to connect to the server
	xcConnectThread.Create( ConnectThread, 40000, this );

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
	char	szString[80];
	int		iX;
	int		iY;
	int		iW;
	int		iL;
	BYTE	byR;
	BYTE	byG;
	BYTE	byB;


	// Get all of the saveable parameters
	GetSizePosition( &iX, &iY, &iW, &iL );

	// Save parameters into the INI file
	C_INI_USER xcIni( "BookFTP" );
	xcIni.Open();
	sprintf( szString, "%d", iX );
	xcIni.Write( "MainX", szString );
	sprintf( szString, "%d", iY );
	xcIni.Write( "MainY", szString );
	sprintf( szString, "%d", iW );
	xcIni.Write( "MainW", szString );
	sprintf( szString, "%d", iL );
	xcIni.Write( "MainL", szString );

	// Save the font
	pxcConsole->GetFont( szString );
	if( strstr( szString, "." ) )
	{
		xcIni.Write( "ConsoleFont", strstr( szString, "." ) + 1 );
		*strstr( szString, "." ) = 0;
		xcIni.Write( "ConsoleFontSize", szString );
	}

	// Save the window foreground color
	pxcConsole->GetForegroundColor( &byR, &byG, &byB );
	sprintf( szString, "%03d,%03d,%03d", byR, byG, byB );
	xcIni.Write( "ConsoleFColor", szString );

	// Save the window background color
	pxcConsole->GetBackgroundColor( &byR, &byG, &byB );
	sprintf( szString, "%03d,%03d,%03d", byR, byG, byB );
	xcIni.Write( "ConsoleBColor", szString );

	xcIni.Close();

	// Debug
	pxcLog->Write( "FTP:WM_CLOSE:Start" );

	// Applcation was told to close so post a QUIT message to the OS
	PostMsg( WM_QUIT, 0, 0 );

	// Debug
	pxcLog->Write( "FTP:WM_CLOSE:End" );
	return FALSE;
}


//---------
// MsgSize \
//---------------------------------------------------------------------------
// Event:	WM_SIZE
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
	pxcTBar->SetSizePosition( 0, iCY, iCX, 40 );
	iCY -= 25;
	pxcStatus->SetSizePosition( 0, iCY, iCX, 25 );

	pxcConsole->SetSizePosition( 3, 25, iCX - 3, iCY - 25 );
	pxcEnterButton->SetSizePosition( 3, 0, 80, 25 );
	pxcCommand->SetSizePosition( 90, 0, iCX - 90, 20 );

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
		// Control message from our toolbar
		case D_TOP_TBAR:
			pxcTBar->Control( (ULONG)mp1 );
			break;
	}

	return FALSE;
}


//------------
// MsgConnect \
//---------------------------------------------------------------------------
// Event:	PM_CONNECT
// Cause:	Issued by the connection thread when a connect is completed
//
void *C_WINDOW_MAIN::MsgConnect( void *mp1, void *mp2 )
{
	// We are ready to accept commands, so enable the "ENTER" button
	pxcEnterButton->Enable( TRUE );

	return FALSE;
}


//---------
// CmdCommand \
//---------------------------------------------------------------------------
// Event:	ID_ENTERCOMMAND
// Cause:	User presses the ENTER button on the main window
//
void *C_WINDOW_MAIN::CmdCommand( void *mp1, void *mp2 )
{
	// Diable the button to prevent commands from being entered
	// during processing
	pxcEnterButton->Enable( FALSE );

	// Start a thread to connect to the server
	xcParseThread.Create( InterpretFTPCommand, 40000, this );

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


//-------------
// CmdHelpInfo \
//---------------------------------------------------------------------------
// Event:	DM_INFO
// Cause:	User selects the Product Information menu option
//
void *C_WINDOW_MAIN::CmdHelpInfo( void *mp1, void *mp2 )
{
	// Create an instance of the about dialog
//	C_DIALOG_ABOUT	xcAbout( this, D_DIALOG_ABOUT );
	return FALSE;
}


void main( int argc, char *argv[] )
{
	// Create a debugging log file
	pxcLog = (C_LOG *)new C_LOG( "ftp.log", 1 );
	pxcLog->Open();

	// Register the application window
	xcWindow.Register( "OS/2 FTP" );
	xcWindow.WCF_SizingBorder();
	xcWindow.WCF_SysMenu();
	xcWindow.WCF_TaskList();
	xcWindow.WCF_ShellPosition();
	xcWindow.WCF_MinButton();
	xcWindow.WCF_MaxButton();
	xcWindow.WCF_Icon();
	xcWindow.WCF_Menu();
	xcWindow.WCF_TitleBar();
	xcWindow.Create( ID_WINDOW, "OS/2 FTP" );

	// Set the news server
	xcWindow.SetServer( argv[1] );

	// Start the message loop
	xcApp.Run();

	// Close and free the debug log
	pxcLog->Write( "FTP:Closing Log" );
	pxcLog->Close();
	delete pxcLog;
}

