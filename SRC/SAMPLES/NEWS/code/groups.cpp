//------------
// GROUPS.CPP \
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
//	This file contains source code for the sample NNTP news client.  More
//  specifically it contains the all of the methods of the C_WINDOW_GROUP
//	class which is used to display a list of available groups from the server.
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
#include <ctype.h>
#include <string.h>
#include <process.h>
#include <io.h>

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
#include <conmgr.hpp>
#include "rc.hpp"
#include <tbartop.hpp>
#include <tbargrp.hpp>
#include <tbarsub.hpp>
#include <tbarmsg.hpp>
#include <tbarart.hpp>
#include <contgrp.hpp>
#include <contsub.hpp>
#include <contmsg.hpp>
#include <groups.hpp>
#include <subs.hpp>
#include <msg.hpp>
#include <article.hpp>
#include <news.hpp>


//-------------
// Global Data \
//---------------------------------------------------------------------------
extern	C_CONNECT_MGR	*pxcMgr;	// Connection Manager Instance
extern	C_LOG			*pxcLog;	// Debug log instnace

//----------------------------
// Group Window Message Table \
//---------------------------------------------------------------------------
DECLARE_MSG_TABLE( xtMsgGroup )
	DECLARE_MSG( PM_CREATE,		C_WINDOW_GROUP::MsgCreate )
	DECLARE_MSG( PM_PARENT,		C_WINDOW_GROUP::MsgParent )
	DECLARE_MSG( WM_CLOSE,		C_WINDOW_GROUP::MsgClose )
	DECLARE_MSG( WM_SIZE,		C_WINDOW_GROUP::MsgSize )
	DECLARE_MSG( WM_CONTROL,	C_WINDOW_GROUP::MsgControl )
	DECLARE_MSG( WM_PAINT,		C_WINDOW_STD::MsgPaint )
	DECLARE_MSG( PM_POPULATE,	C_WINDOW_GROUP::MsgPopulate )
END_MSG_TABLE


//----------------------------
// Group Window Command Table \
//---------------------------------------------------------------------------
DECLARE_COMMAND_TABLE( xtCommandGroup )
	DECLARE_COMMAND( DM_GROUP_SUBSCRIBE,	C_WINDOW_GROUP::CmdSubscribe )
	DECLARE_COMMAND( DM_GROUP_LOAD,			C_WINDOW_GROUP::CmdRefresh )
END_MSG_TABLE


//----------------
// PopulateThread \
//---------------------------------------------------------------------------
//
// Description:
//	This thread loads the groups from the groups.grp file into the container
//	displayed by the group window.  This is done on a separate thread because
// it can be a very time-consuming task.
//
void _Optlink PopulateGroupThread( void *pvData )
{
	C_WINDOW_GROUP	*pxcThis;
	C_THREAD_PM		*pxcThread;
	char			cChar;
	char			szString[1024];
	FILE			*hFile;
	int				iCount;
	int				iConnection;
	T_GRPRECORD		*pFirstRecord;
	T_GRPRECORD		*pRecord;


	// Get a point to the main window object
	pxcThread = (C_THREAD_PM *)pvData;
	pxcThis = (C_WINDOW_GROUP *)pxcThread->ThreadData();

	// Create a PM process for this thread
	pxcThread->InitializeThread();

	// Look to see if the group file is here
	if( access( "groups.grp", 0 ) != 0 )
	{
		pxcThis->Status()->Text( "Loading groups from server..." );

		// Debug
		pxcLog->Write( "Loading groups from the server" );

		// Get a network connection
		iConnection = pxcMgr->Connect();

		// Debug
		pxcLog->Write( "Got connection:%d", iConnection );

		if( iConnection >= 0 )
		{
			pxcLog->Write( "Getting list" );
			pxcMgr->Connection(iConnection)->List( "groups.grp" );
			pxcLog->Write( "Disconnecting" );
			pxcMgr->Disconnect( iConnection );
		}
		pxcLog->Write( "Done loading groups from the server" );
	}

	// The first thing we need to do is determine the number of
	// supported groups
	pxcThis->Status()->Text( "Populating news groups..." );
	iCount = 0;
	hFile = fopen( "groups.grp", "r" );
	if( hFile )
	{
		while( !feof( hFile ) && fgets( szString, 1024, hFile ) )
		{
			// Get rid of any CR/LF
			if( strstr( szString, "\r" ) )
				*strstr( szString, "\r" ) = 0;
			if( strstr( szString, "\n" ) )
				*strstr( szString, "\n" ) = 0;

			// Get the support indicator character
			cChar = (char)toupper( szString[strlen( szString ) - 1] );

			// If this group is supported count it
			if( cChar != 'X' || ( cChar == 'X' &&
								szString[strlen( szString ) - 2] != ' ' ) )
			{
				iCount++;
			}
		}
		fclose( hFile );
	}

	pxcLog->Write( "iCount =%d", iCount );

	// Insert groups if there are any
	if( iCount > 0 )
	{
		// Allocate some container space, but keep track of where it starts
		pFirstRecord = (T_GRPRECORD *)pxcThis->Container()->Allocate(
								sizeof( T_GRPRECORD ), (USHORT)(iCount+1) );
		pRecord = pFirstRecord;

		// Now insert each supported record into the container
		iCount = 0;
		hFile = fopen( "groups.grp", "r" );
		while( !feof( hFile ) && fgets( szString, 1024, hFile ) )
		{
			// Get rid of any CR/LF
			if( strstr( szString, "\r" ) )
				*strstr( szString, "\r" ) = 0;
			if( strstr( szString, "\n" ) )
				*strstr( szString, "\n" ) = 0;

			// Get the support indicator character
			cChar = (char)toupper( szString[strlen( szString ) - 1] );

			// If this group is supported count it
			if( cChar != 'X' || ( cChar == 'X' &&
								szString[strlen( szString ) - 2] != ' ' ) )
			{
				if( strstr( szString, " " ) )
					*strstr( szString, " " ) = 0;

				// Insert this record
				if( strlen( szString ) )
				{
					pRecord = (T_GRPRECORD *)pxcThis->Container()->Fill(
														pRecord, szString );
					iCount++;
				}
			}
		}
		fclose( hFile );

		// Perform the container insertion
		pxcThis->Container()->Insert( 0, pFirstRecord, iCount );

		// Sort the records
		pxcThis->Container()->Sort( SortGroupByAlpha );

		// Tell the user how many groups there are in the list
		pxcThis->Status()->Text( "%d Groups loaded", iCount );
	}

	// Terminate the thread
	pxcThread->TerminateThread();
}


//-------------
// Constructor \
//---------------------------------------------------------------------------
//
//	Description:
//		This constructor assigns the message and command tables for this class.
//
C_WINDOW_GROUP::C_WINDOW_GROUP( void ) : C_WINDOW_STD( xtMsgGroup )
{
	// Enable the required handlers for this window
	CommandTable( xtCommandGroup );
}


//-------------
// Destructor \
//---------------------------------------------------------------------------
//
// Description:
//		This destuctor disposes of the memory used by the child window classes.
//
C_WINDOW_GROUP::~C_WINDOW_GROUP( void )
{
	pxcLog->Write( "GROUP:Destructor:Start" );

	// Free up the child windows
	delete pxcTBar;
	delete pxcStatus;
	delete pxcCont;

	pxcLog->Write( "GROUP:Destructor:End" );
}


//-----------
// MsgCreate \
//---------------------------------------------------------------------------
// Event:	WM_CREATE
// Cause:	Issued by OS when window is created
//
void *C_WINDOW_GROUP::MsgCreate( void *mp1, void *mp2 )
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
	pxcTBar = (C_TOOLBAR_GRP *)new C_TOOLBAR_GRP( this, pxcStatus );

	// Create a new container to display group listing
	pxcCont = (C_CONTAINER_GRP *)new C_CONTAINER_GRP( this );

	// Load parameters out of the INI file
	C_INI_USER xcIni( "BookNews" );
	xcIni.Open();
	xcIni.Read( "GroupFont", szFont, "System Proportional", 80 );
	xcIni.Read( "GroupFontSize", szFontSize, "10", 10 );
	xcIni.Read( "GroupBColor", szBColor, "000,000,000", 80 );
	xcIni.Read( "GroupFColor", szFColor, "255,255,255", 80 );
	xcIni.Read( "GroupX", szX, "0", 10 );
	xcIni.Read( "GroupY", szY, "0", 10 );
	xcIni.Read( "GroupW", szW, "0", 10 );
	xcIni.Read( "GroupL", szL, "0", 10 );
	xcIni.Close();

	// Set the font in the window
	pxcCont->SetFont( szFont, atoi( szFontSize ) );

	// Set the window colors
	pxcCont->SetForegroundColor( atoi( &szFColor[0] ),
								atoi( &szFColor[4] ), atoi( &szFColor[8] ) );
	pxcCont->SetBackgroundColor( atoi( &szBColor[0] ),
								atoi( &szBColor[4] ), atoi( &szBColor[8] ) );

	if( atoi( szW ) != 0 && atoi( szL ) != 0 )
	{
		// Position and size the window
		SetSizePosition( atoi( szX ), atoi( szY ), atoi( szW ), atoi( szL ) );
	}

	// Make the window visible
	pxcCont->Focus();

	return (void *)TRUE;
}


//-----------
// MsgParent \
//---------------------------------------------------------------------------
// Event:	PM_PARENT
// Cause:	Issued by parent window to identify itself as the owner
//
void *C_WINDOW_GROUP::MsgParent( void *mp1, void *mp2 )
{
	// Assign a pointer to the parent window
	pxcParent = (C_WINDOW_STD *)mp1;

	return FALSE;
}


//----------
// MsgClose \
//---------------------------------------------------------------------------
// Event:	WM_CLOSE
// Cause:	Issued by OS when window is closed
//
void *C_WINDOW_GROUP::MsgClose( void *mp1, void *mp2 )
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
	C_INI_USER xcIni( "BookNews" );
	xcIni.Open();

	// Save the window dimensions
	sprintf( szString, "%d", iX );
	xcIni.Write( "GroupX", szString );
	sprintf( szString, "%d", iY );
	xcIni.Write( "GroupY", szString );
	sprintf( szString, "%d", iW );
	xcIni.Write( "GroupW", szString );
	sprintf( szString, "%d", iL );
	xcIni.Write( "GroupL", szString );

	// Save the font
	pxcCont->GetFont( szString );
	if( strstr( szString, "." ) )
	{
		xcIni.Write( "GroupFont", strstr( szString, "." ) + 1 );
		*strstr( szString, "." ) = 0;
		xcIni.Write( "GroupFontSize", szString );
	}

	// Save the window foreground color
	pxcCont->GetForegroundColor( &byR, &byG, &byB );
	sprintf( szString, "%03d,%03d,%03d", byR, byG, byB );
	xcIni.Write( "GroupFColor", szString );

	// Save the window background color
	pxcCont->GetBackgroundColor( &byR, &byG, &byB );
	sprintf( szString, "%03d,%03d,%03d", byR, byG, byB );
	xcIni.Write( "GroupBColor", szString );
	xcIni.Close();

	// Kill any threads we own that are still running
	xcPopulateThread.Kill();

	// Commit suicide
	Destroy();

	// Tell the parent that the user told us to shut down.  Our parent will
	// clean up our mess (ie. call our destructor)
	pxcParent->PostMsg( PM_GROUP_CLOSE, 0, 0 );

	return FALSE;
}


//---------
// MsgSize \
//---------------------------------------------------------------------------
// Event:	WM_SIZE
// Cause:	Issued by OS when window is resized
//
void *C_WINDOW_GROUP::MsgSize( void *mp1, void *mp2 )
{
	int		iCX;
	int		iCY;


	// Determine the size of the client area
	C_WINDOW::GetSize( &iCX, &iCY );

	// Strect the toolbar and status windows
	iCY -= 40;
	pxcTBar->SetSizePosition( 0, iCY, iCX, 40 );
	iCY -= 25;
	pxcStatus->SetSizePosition( 0, iCY, iCX, 25 );

	// Make the container window use whatever space is left
	pxcCont->SetSizePosition( 0, 0, iCX, iCY );

	return FALSE;
}


//------------
// MsgControl \
//---------------------------------------------------------------------------
// Event:	WM_CONTROL
// Cause:	Issued by OS for control functions (Toolbar interaction)
//
void *C_WINDOW_GROUP::MsgControl( void *mp1, void *mp2 )
{
	switch( SHORT1FROMMP( mp1 ) )
	{
		// Got a control message from the toolbar
		case D_GRP_TBAR:
			pxcTBar->Control( (ULONG)mp1 );
			break;
	}
	return FALSE;
}


//-------------
// MsgPopulate \
//---------------------------------------------------------------------------
// Event:	PM_POPULATE
// Cause:	Issued by OS during the initial creation of the groups window
//
//	This method populates the container object within the group window.  Since
//	this is a time-consuming task, it will be done on a separate PM thread.
//
void *C_WINDOW_GROUP::MsgPopulate( void *mp1, void *mp2 )
{
	// Begin a thread to populate the group list
	xcPopulateThread.Create( PopulateGroupThread, 40000, this );

	return FALSE;
}


//--------------
// CmdSubscribe \
//---------------------------------------------------------------------------
// Event:		DM_SUBSCRIBE
// Cause:		User selects the subscribe option from the toolbar or menu
// Description:	This method subscribes to all of the currently highlighted
//				container items.  It does this by sending a PM_SUBSCRIBE
//				message to its parent window (the News control panel).
//
void *C_WINDOW_GROUP::CmdSubscribe( void *mp1, void *mp2 )
{
	T_GRPRECORD *pRecord;


	// Get the first selected record in the list
	pRecord = (T_GRPRECORD *)pxcCont->FirstSelected();
	while( pRecord )
	{
		pxcLog->Write( "pRecord = %s", pRecord->szString );

		// Tell our parent that the user wants to subscribe to this group
		pxcParent->SendMsg( PM_GROUP_SUBSCRIBE, (void *)pRecord->szString, 0 );

		// Unselect as we go so the user knows something is happening
		pxcCont->SelectRecord( pRecord, FALSE );

		// Go to the next record
		pRecord = (T_GRPRECORD *)pxcCont->NextSelected( pRecord );
	}

	return FALSE;
}

//------------
// CmdRefresh \
//---------------------------------------------------------------------------
// Event:		DM_REFRESH
// Cause:		User selects the refresh option from the toolbar or menu
// Description:	This method removes the group file from the disk and forces
//				the news server to resend it.
//
void *C_WINDOW_GROUP::CmdRefresh( void *mp1, void *mp2 )
{
	// Force the groups file to go away
	DosForceDelete( "groups.grp" );

	// Say that we want to reload the groups from the server
	PostMsg( PM_POPULATE, 0, 0 );

	return FALSE;
}

