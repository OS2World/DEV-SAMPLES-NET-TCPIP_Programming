//----------
// NEWS.CPP \
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
#include <netnews.hpp>

//---------------------
// Application Headers \
//---------------------------------------------------------------------------
#include "rc.hpp"
#include <tbartop.hpp>
#include <tbargrp.hpp>
#include <tbarsub.hpp>
#include <tbarmsg.hpp>
#include <tbarart.hpp>
#include <contgrp.hpp>
#include <contsub.hpp>
#include <contmsg.hpp>
#include <about.hpp>
#include <groups.hpp>
#include <subs.hpp>
#include <msg.hpp>
#include <article.hpp>
#include <conmgr.hpp>
#include <news.hpp>

#define		D_NEWS_PORT			119		// Standard NNTP port number
#define		D_MAX_CONNECT		3		// Number of connections to make to the
										// server

//-------------
// Global Data \
//---------------------------------------------------------------------------
C_APPLICATION	xcApp;		// Application instance
C_WINDOW_MAIN	xcWindow;	// Main window instance
C_CONNECT_MGR	*pxcMgr;	// Connection Manager Instance
C_LOG	 		*pxcLog;	// Debug log instance


//---------------------------
// Main Window Message Table \
//---------------------------------------------------------------------------
DECLARE_MSG_TABLE( xtMsgMain )
	DECLARE_MSG( PM_CREATE,				C_WINDOW_MAIN::MsgCreate )
	DECLARE_MSG( PM_GROUP_CLOSE,		C_WINDOW_MAIN::MsgGroupClose )
	DECLARE_MSG( PM_GROUP_SUBSCRIBE,	C_WINDOW_MAIN::MsgGroupSubscribe )
	DECLARE_MSG( PM_SUB_CLOSE,			C_WINDOW_MAIN::MsgSubscriptionClose )
	DECLARE_MSG( PM_SUB_READ,			C_WINDOW_MAIN::MsgSubscriptionRead )
	DECLARE_MSG( PM_MSG_CLOSE,			C_WINDOW_MAIN::MsgMessageClose )
	DECLARE_MSG( PM_MSG_READ,			C_WINDOW_MAIN::MsgMessageRead )
	DECLARE_MSG( PM_ART_CLOSE,			C_WINDOW_MAIN::MsgArticleClose )
	DECLARE_MSG( PM_CONNECT,			C_WINDOW_MAIN::MsgConnect )
	DECLARE_MSG( WM_CLOSE,				C_WINDOW_MAIN::MsgClose )
	DECLARE_MSG( WM_SIZE,				C_WINDOW_MAIN::MsgSize )
	DECLARE_MSG( WM_CONTROL,			C_WINDOW_MAIN::MsgControl )
	DECLARE_MSG( WM_PAINT,				C_WINDOW_STD::MsgPaint )
END_MSG_TABLE


//---------------------------
// Main Window Command Table \
//---------------------------------------------------------------------------
DECLARE_COMMAND_TABLE( xtCommandMain )
	DECLARE_COMMAND( DM_GROUPS,			C_WINDOW_MAIN::CmdGroups )
	DECLARE_COMMAND( DM_SUBSCRIPTIONS,	C_WINDOW_MAIN::CmdSubscriptions )
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
//		class.
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
	pxcMgr->Initialize( D_MAX_CONNECT, pxcThis->ServerAddress(), D_NEWS_PORT );

	// If there was an error, tell the user about it
	if( pxcMgr->Connections() <= 0 )
	{
		// Tell the user that the connection failed
		WinMessageBox( HWND_DESKTOP, pxcThis->Window(),
				"News could not connect to the server",
										"News", 0, MB_OK | MB_ICONHAND );

		pxcThis->Status()->Text( "Not connected" );
	}
	else
	{
		// Tell the main window that there were no errors - we're on-line!
		pxcThis->PostMsg( PM_CONNECT, 0, 0 );
	}

	// Terminate the thread
	pxcThread->TerminateThread();
}


//-------------
// Constructor \
//---------------------------------------------------------------------------
//
// Description:
//		This constructor initializes the main window class for the editor.
//		It zeroes the class attributes and setups up the command handler and
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
	pxcMenu = 0;
	pxcGroups = 0;
	pxcSubs = 0;
	pxcMsg = 0;
	pxcArticle = 0;

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

	delete pxcTBar;
	delete pxcStatus;
	delete pxcMenu;

	delete pxcGroups;
	delete pxcSubs;
	delete pxcMsg;
	delete pxcArticle;

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


	// Create a status bar to display miscellaneous data
	pxcStatus = (C_STATUS *) new C_STATUS( this );

	// Create a toolbar control
	pxcTBar = (C_TOOLBAR_TOP *)new C_TOOLBAR_TOP( this, pxcStatus );

	// Keep track of the main menu so we can enable/disable items
	pxcMenu = (C_MENU *)new C_MENU( this );

	// Load parameters out of the INI file
	C_INI_USER xcIni( "BookNews" );
	xcIni.Open();
	xcIni.Read( "MainX", szX, "0", 10 );
	xcIni.Read( "MainY", szY, "0", 10 );
	xcIni.Close();

	// Make the window look like a control panel
	SetSizePosition( atoi( szX ), atoi( szY ), xcApp.DesktopWidth() / 10 * 4,
				xcApp.DialogBorderHeight() * 2 + xcApp.TitleBarHeight() +
													xcApp.MenuHeight() + 65 );

	// Make the window visible
	Show();

	// Disable menu options so the user can't select information
	// until we're connected
	pxcMenu->DisableItem( DM_WINDOWS );

	// Disable the toolbar buttons until we're connected
	pxcTBar->ButtonEnable( DB_WND_GRP, FALSE );
	pxcTBar->ButtonEnable( DB_WND_SUB, FALSE );

	// Begin a thread to open all of the news connections
	xcConnectThread.Create( ConnectThread, 40000, this );

	return FALSE;
}


//---------------
// MsgGroupClose \
//---------------------------------------------------------------------------
// Event:	PM_GROUP_CLOSE
// Cause:	Issued by the group window when the user closes it
//
void *C_WINDOW_MAIN::MsgGroupClose( void *mp1, void *mp2 )
{
	// Close and Delete the current group window
	delete pxcGroups;
	pxcGroups = 0;

	return FALSE;
}


//-------------------
// MsgGroupSubscribe \
//---------------------------------------------------------------------------
// Event:		PM_GROUP_SUBSCRIBE
// Cause:		Issued by the group window when the user subscribes to a group.
// Description:	This method is invoked any time a group is subscribed to.  The
//				mp1 parameter contains a pointer to the group name string being
//				subscribed.
//
void *C_WINDOW_MAIN::MsgGroupSubscribe( void *mp1, void *mp2 )
{
	// Tell the subscription window about the new group
	pxcSubs->SendMsg( PM_SUB_SUBSCRIBE, mp1, 0 );

	return FALSE;
}


//----------------------
// MsgSubscriptionClose \
//---------------------------------------------------------------------------
// Event:	PM_SUB_CLOSE
// Cause:	Issued by the subscription window when the user closes it
//
void *C_WINDOW_MAIN::MsgSubscriptionClose( void *mp1, void *mp2 )
{
	// Delete the current subscription window
	delete pxcSubs;
	pxcSubs = 0;

	return FALSE;
}


//----------------------
// MsgSubscriptionRead \
//---------------------------------------------------------------------------
// Event:		PM_SUB_READ
// Cause:		Issued by the subscription window when the user wants to read
//				a subscription.
// Description:	This method is called any time the user selects a subscription
//				to read.  This will create an instance of a message list window
//				and display the subscription contents.
//
void *C_WINDOW_MAIN::MsgSubscriptionRead( void *mp1, void *mp2 )
{
	char	*szGroup;
	char	szString[64];


	// Determine which group is being displayed
	szGroup = (char *)mp1;
pxcLog->Write( "MsgSubscriptionRead:Group=%s", szGroup );

	// Only permit the user to open a groups window if it isn't already open
	if( !pxcMsg )
	{
		// Create a new instance of the groups window
		pxcMsg = (C_WINDOW_MESSAGE *)new C_WINDOW_MESSAGE;

		// Set up message window
		pxcMsg->Register( "Message" );
		pxcMsg->WCF_Standard();
		pxcMsg->Create( ID_MESSAGES, "Message List" );

		// Tell the message window that this is its parent
		pxcMsg->SendMsg( PM_PARENT, (void *)this, 0 );

		// Tell the message window to populate for the given group in mp1
		pxcMsg->SendMsg( PM_POPULATE, mp1, 0 );
	}
	else
	{
		// Otherwise give the user focus to the existing window
		pxcMsg->Show();
	}

	return FALSE;
}


//-----------------
// MsgMessageClose \
//---------------------------------------------------------------------------
// Event:	PM_MSG_CLOSE
// Cause:	Issued by the message window when the user closes it
//
void *C_WINDOW_MAIN::MsgMessageClose( void *mp1, void *mp2 )
{
	// Delete the current message window
	delete pxcMsg;
	pxcMsg = 0;

	return FALSE;
}


//----------------
// MsgMessageRead \
//---------------------------------------------------------------------------
// Event:		PM_MSG_READ
// Cause:		Issued by the message window when the user wants to read
//				an article.
// Description:	This method is called any time the user selects an article
//				to read.  This will create an instance of an article window
//				and display the article supplied in mp1 to the viewer.
//
void *C_WINDOW_MAIN::MsgMessageRead( void *mp1, void *mp2 )
{
pxcLog->Write( "MsgMessageRead:File=%s", (char *)mp1 );

	// Only permit the user to open a window if it isn't already open
	if( !pxcArticle )
	{
		// Create a new instance of the groups window
		pxcArticle = (C_WINDOW_ARTICLE *)new C_WINDOW_ARTICLE;

		// Set up article window
		pxcArticle->Register( "Article" );
		pxcArticle->WCF_Standard();
		pxcArticle->Create( ID_ARTICLE, "News Article" );

		// Tell the message window that this is its parent
		pxcArticle->SendMsg( PM_ART_PARENT, (void *)this, 0 );

		// Tell the article window to populate for the given article in mp1
		pxcArticle->SendMsg( PM_ART_POPULATE, mp1, 0 );
	}
	else
	{
		// Otherwise give the user focus to the existing window
		pxcArticle->Show();
	}

	return FALSE;
}


//-----------------
// MsgMessageClose \
//---------------------------------------------------------------------------
// Event:	PM_MSG_CLOSE
// Cause:	Issued by the article window when the user closes it
//
void *C_WINDOW_MAIN::MsgArticleClose( void *mp1, void *mp2 )
{
	// Delete the current article window
	delete pxcArticle;
	pxcArticle = 0;

	return FALSE;
}


//------------
// MsgConnect \
//---------------------------------------------------------------------------
// Event:	PM_CONNECT
// Cause:	Issued by the connection thread when a connection has been
//			established
//
void *C_WINDOW_MAIN::MsgConnect( void *mp1, void *mp2 )
{
	// Tell the user he is connected
	pxcStatus->Text( "Connected to %s...", ServerAddress() );

	// Enable menu options so the user can select information
	pxcMenu->EnableItem( DM_WINDOWS );

	// Enable the toolbar buttons
	pxcTBar->ButtonEnable( DB_WND_GRP, TRUE );
	pxcTBar->ButtonEnable( DB_WND_SUB, TRUE );

	// Give the user some audible feedback to say that we've connected
	DosBeep( 100, 100 );

	// since we are now connected, open the subscription window and tell
	// it to update.
	SendMsg( WM_COMMAND, (void *)DM_SUBSCRIPTIONS, 0 );

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


	// Get all of the saveable parameters
	GetSizePosition( &iX, &iY, &iW, &iL );

	// Save parameters into the INI file
	C_INI_USER xcIni( "BookNews" );
	xcIni.Open();
	sprintf( szString, "%d", iX );
	xcIni.Write( "MainX", szString );
	sprintf( szString, "%d", iY );
	xcIni.Write( "MainY", szString );
	xcIni.Close();

	// Debug
	pxcLog->Write( "NEWS:WM_CLOSE:Start" );

	// If there is a groups window open, close it and destroy
	if( pxcGroups )
		pxcGroups->SendMsg( WM_CLOSE, 0, 0 );

	// If there is a subscriptions window open, close it and destroy
	if( pxcSubs )
		pxcSubs->SendMsg( WM_CLOSE, 0, 0 );

	// If there is a message window open, close it and destroy
	if( pxcMsg )
		pxcMsg->SendMsg( WM_CLOSE, 0, 0 );

	// If there is an article window open, close it and destroy
	if( pxcArticle )
		pxcArticle->SendMsg( WM_CLOSE, 0, 0 );

	// Applcation was told to close, so post a QUIT message to the OS
	PostMsg( WM_QUIT, 0, 0 );

	// Debug
	pxcLog->Write( "NEWS:WM_CLOSE:End" );
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


//-----------
// CmdGroups \
//---------------------------------------------------------------------------
// Event:	DM_GROUPS
// Cause:	User selects the Window/Groups option in order to display the
//			list of available groups supported by the server.
//
void *C_WINDOW_MAIN::CmdGroups( void *mp1, void *mp2 )
{
	// Only permit the user to open a groups window if it isn't already open
	if( !pxcGroups )
	{
		// Create a new instance of the groups window
		pxcGroups = (C_WINDOW_GROUP *)new C_WINDOW_GROUP;

		// Set up the available groups window
		pxcGroups->Register( "Groups" );
		pxcGroups->WCF_Standard();
		pxcGroups->Create( ID_GROUPS, "Available Newsgroups" );

		// Tell the group window that this is its parent
		pxcGroups->SendMsg( PM_PARENT, (void *)this, 0 );

		// Tell the group window to populate
		pxcGroups->SendMsg( PM_POPULATE, 0, 0 );
	}
	else
	{
		// Otherwise give the user focus to the existing window
		pxcGroups->Show();
	}
	return FALSE;
}


//------------------
// CmdSubscriptions \
//---------------------------------------------------------------------------
// Event:	DM_SUBSCRIPTION
// Cause:	User selects the Window/Subscriptions option in order to display
//			the list of available groups supported by the server.
//
void *C_WINDOW_MAIN::CmdSubscriptions( void *mp1, void *mp2 )
{
	// Only permit the user to open a sub window if it isn't already open
	if( !pxcSubs )
	{
		// Create a new instance of the groups window
		pxcSubs = (C_WINDOW_SUBSCRIPTION *)new C_WINDOW_SUBSCRIPTION;

		// Set up groups window
		pxcSubs->Register( "Subscriptions" );
		pxcSubs->WCF_Standard();
		pxcSubs->Create( ID_SUBSCRIPTIONS, "Current Subscriptions" );

		// Tell the group window that this is its parent
		pxcSubs->SendMsg( PM_PARENT, (void *)this, 0 );

		// Tell the group window to populate
		pxcSubs->SendMsg( PM_POPULATE, 0, 0 );
	}
	else
	{
		// Otherwise give the user focus to the existing window
		pxcSubs->Show();
	}
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
	C_DIALOG_ABOUT	xcAbout( this, D_DIALOG_ABOUT );
	return FALSE;
}


void main( int argc, char *argv[] )
{
	// Create a debugging log file
	pxcLog = (C_LOG *)new C_LOG( "news.log", 1 );
	pxcLog->Open();

	// Register the application window
	xcWindow.Register( "OS/2 News" );
	xcWindow.WCF_Icon();
	xcWindow.WCF_SysMenu();
	xcWindow.WCF_Menu();
	xcWindow.WCF_TitleBar();
	xcWindow.WCF_MinButton();
	xcWindow.WCF_TaskList();
	xcWindow.WCF_DialogBorder();
	xcWindow.Create( ID_WINDOW, "OS/2 News" );

	// Create the news connections
	pxcLog->Write( "Main:Creating Connection Manager" );
	pxcMgr = (C_CONNECT_MGR *)new C_CONNECT_MGR;
	pxcLog->Write( "Main:Created Connection Manager" );

	// Set the news server
	xcWindow.SetServer( argv[1] );

	// Start the message loop
	xcApp.Run();

	// Destroy the News connections
	delete pxcMgr;

	// Close and free the debug log
	pxcLog->Write( "NEWS:Closing Log" );
	pxcLog->Close();
	delete pxcLog;
}

