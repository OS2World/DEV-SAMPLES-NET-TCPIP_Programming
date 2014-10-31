//----------
// SUBS.CPP \
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
//  specifically it contains the all of the methods of the
//	C_WINDOW_SUBSCRIPTION class which is used to display and control the list
//	of news groups subscriptions.
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
extern	C_LOG			*pxcLog;	// Debug log instance

//-----------------------------------
// Subscription Window Message Table \
//---------------------------------------------------------------------------
DECLARE_MSG_TABLE( xtMsgSubscription )
	DECLARE_MSG( PM_CREATE,			C_WINDOW_SUBSCRIPTION::MsgCreate )
	DECLARE_MSG( PM_PARENT,			C_WINDOW_SUBSCRIPTION::MsgParent )
	DECLARE_MSG( PM_POPULATE,		C_WINDOW_SUBSCRIPTION::MsgPopulate )
	DECLARE_MSG( PM_SUB_SUBSCRIBE,	C_WINDOW_SUBSCRIPTION::MsgSubSubscribe )
	DECLARE_MSG( WM_CLOSE,			C_WINDOW_SUBSCRIPTION::MsgClose )
	DECLARE_MSG( WM_SIZE,			C_WINDOW_SUBSCRIPTION::MsgSize )
	DECLARE_MSG( WM_CONTROL,		C_WINDOW_SUBSCRIPTION::MsgControl )
	DECLARE_MSG( WM_PAINT,			C_WINDOW_STD::MsgPaint )
END_MSG_TABLE


//-----------------------------------
// Subscription Window Command Table \
//---------------------------------------------------------------------------
DECLARE_COMMAND_TABLE( xtCommandSub )
	DECLARE_COMMAND( DM_SUB_UNSUBSCRIBE,
								C_WINDOW_SUBSCRIPTION::CmdSubUnsubscribe )
	DECLARE_COMMAND( DM_SUB_READ,	C_WINDOW_SUBSCRIPTION::CmdSubRead )
END_MSG_TABLE


//----------------
// PopulateThread \
//---------------------------------------------------------------------------
//
// Description:
//		This thread function is used to populate the subscription window
//		container.  It loads data from the subscription file, parses it and
//		writes it to the container window.
//
void _Optlink PopulateSubscriptionThread( void *pvData )
{
	C_WINDOW_SUBSCRIPTION	*pxcThis;
	C_THREAD_PM				*pxcThread;
	char					szString[1024];
	FILE					*hFile;
	int						iConnection;
	int						iCount;
	T_SUBRECORD				*pFirstRecord;
	T_SUBRECORD				*pRecord;
	ULONG					lFirst;
	ULONG					lLast;
	ULONG					lTotal;


	// Get a point to the main window object
	pxcThread = (C_THREAD_PM *)pvData;
	pxcThis = (C_WINDOW_SUBSCRIPTION *)pxcThread->ThreadData();

	// Create a PM process for this thread
	pxcThread->InitializeThread();

	// Remove any previous data because this could be an update
	pxcThis->Container()->RemoveAll();

	// The first thing we need to do is determine the number of
	// supported groups
	iCount = 0;
	hFile = fopen( "groups.sub", "r" );
	if( hFile )
	{
		while( !feof( hFile ) && fgets( szString, 1024, hFile ) )
		{
			// Found a subscription
			iCount++;
		}
		fclose( hFile );
	}

	pxcLog->Write( "SUBS:iCount =%d", iCount );

	// Insert groups if there are any
	if( iCount > 0 )
	{
		// Allocate some container space, but keep track of where it starts
		pFirstRecord = (T_SUBRECORD *)pxcThis->Container()->Allocate(
								sizeof( T_SUBRECORD ), (USHORT)iCount );
		pRecord = pFirstRecord;

		// Now insert each supported record into the container
		hFile = fopen( "groups.sub", "r" );
		while( !feof( hFile ) && fgets( szString, 1024, hFile ) )
		{
			// Get rid of any CR/LF
			if( strstr( szString, "\r" ) )
				*strstr( szString, "\r" ) = 0;
			if( strstr( szString, "\n" ) )
				*strstr( szString, "\n" ) = 0;

			// Insert this record
			pRecord = (T_SUBRECORD *)pxcThis->Container()->Fill(
													pRecord, szString );
		}
		fclose( hFile );

		// Perform the container insertion
		pxcThis->Container()->Insert( 0, pFirstRecord, iCount );

		// Redraw the container to show the subscriptions
		pxcThis->Container()->Redraw( 0 );

		// Get a network connection
		iConnection = pxcMgr->Connect();
		if( iConnection >= 0 )
		{
			// Update the group information
			pRecord = (T_SUBRECORD *)pxcThis->Container()->FirstRecord();
			while( pRecord )
			{
				// Get the group information and article status
				pxcMgr->Connection(iConnection)->Group(
								pRecord->szGroup, &lFirst, &lLast, &lTotal );

				// Update the record and redraw it
				sprintf( pRecord->szTotal, "%ld", lTotal );
				pxcThis->Container()->Redraw( pRecord );

				// Step to the next record
				pRecord = (T_SUBRECORD *)pxcThis->
										Container()->NextRecord( pRecord );
			}
		}

		// Free up the network connection
		pxcMgr->Disconnect( iConnection );
	}

	// Terminate the thread
	pxcThread->TerminateThread();
}


//-------------------
// UnsubscribeThread \
//---------------------------------------------------------------------------
//
// Description:
//		This thread is responsible for unsubscribing to news groups.  It
//		removes the group(s) from the subscription container and rewrites
//		the group.sub subscription file.
//
void _Optlink UnsubscribeThread( void *pvData )
{
	C_WINDOW_SUBSCRIPTION	*pxcThis;
	C_THREAD_PM				*pxcThread;
	FILE					*hFile;
	T_SUBRECORD				*pRecord;
	T_SUBRECORD				*pNext;


	// Get a point to the main window object
	pxcThread = (C_THREAD_PM *)pvData;
	pxcThis = (C_WINDOW_SUBSCRIPTION *)pxcThread->ThreadData();

	// Create a PM process for this thread
	pxcThread->InitializeThread();

	// Update the group information
	pRecord = (T_SUBRECORD *)pxcThis->Container()->FirstSelected();
	while( pRecord )
	{
		// Get the next record
		pNext = (T_SUBRECORD *)pxcThis->Container()->NextSelected( pRecord );

		// Remove the record
		pxcThis->Container()->Remove( pRecord );

		// Set the current record equal to the next
		pRecord = pNext;
	}

	// Refresh the container view
	pxcThis->Container()->Redraw( 0 );

	// Look for the first container item
	pRecord = (T_SUBRECORD *)pxcThis->Container()->FirstRecord();
	if( pRecord )
	{
		// Update the subscription file
		hFile = fopen( "groups.sub", "w" );
		if( hFile )
		{
			// Loop to every subscription in the container
			while( pRecord )
			{
				// Write the item to the subscription file
				fprintf( hFile, "%s\n", pRecord->szGroup );

				// Get the next record
				pRecord = (T_SUBRECORD *)pxcThis->Container()->
													NextRecord( pRecord );
			}
			fclose( hFile );
		}
	}
	else
	{
		// No records left, so clean up the file
		DosForceDelete( "groups.sub" );
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
C_WINDOW_SUBSCRIPTION::C_WINDOW_SUBSCRIPTION( void )
								: C_WINDOW_STD( xtMsgSubscription )
{
	// Enable the required handlers for this window
	CommandTable( xtCommandSub );
}


//------------
// Destructor \
//---------------------------------------------------------------------------
C_WINDOW_SUBSCRIPTION::~C_WINDOW_SUBSCRIPTION( void )
{
	pxcLog->Write( "SUBSCRIPTION:Destructor:Start" );

	// Free up the child windows
	delete pxcTBar;
	delete pxcStatus;
	delete pxcCont;

	pxcLog->Write( "SUBSCRIPTION:Destructor:End" );
}


//-----------
// MsgCreate \
//---------------------------------------------------------------------------
// Event:	WM_CREATE
// Cause:	Issued by OS when window is created
//
void *C_WINDOW_SUBSCRIPTION::MsgCreate( void *mp1, void *mp2 )
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
	pxcTBar = (C_TOOLBAR_SUB *)new C_TOOLBAR_SUB( this, pxcStatus );

	// Create a new container to display subscription listing
	pxcCont = (C_CONTAINER_SUB *)new C_CONTAINER_SUB( this );

	// Load parameters out of the INI file
	C_INI_USER xcIni( "BookNews" );
	xcIni.Open();
	xcIni.Read( "SubFont", szFont, "System Proportional", 80 );
	xcIni.Read( "SubFontSize", szFontSize, "10", 10 );
	xcIni.Read( "SubBColor", szBColor, "000,000,000", 80 );
	xcIni.Read( "SubFColor", szFColor, "255,255,255", 80 );
	xcIni.Read( "SubX", szX, "0", 10 );
	xcIni.Read( "SubY", szY, "0", 10 );
	xcIni.Read( "SubW", szW, "0", 10 );
	xcIni.Read( "SubL", szL, "0", 10 );
	xcIni.Close();

	// Set the font in the window
	pxcCont->SetFont( szFont, atoi( szFontSize ) );

	// Set the colors
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
	Show();

	return FALSE;
}



//-----------
// MsgParent \
//---------------------------------------------------------------------------
// Event:	PM_PARENT
// Cause:	Issued by parent window to idetify itself as the owner
//
void *C_WINDOW_SUBSCRIPTION::MsgParent( void *mp1, void *mp2 )
{
	// Populate the parent object attribute within the class
	pxcParent = (C_WINDOW_STD *)mp1;

	return FALSE;
}


//-------------
// MsgPopulate \
//---------------------------------------------------------------------------
// Event:	PM_POPULATE
// Cause:	Issued by OS during the initial creation of the groups window
//
//	This method populates the container object within the subscription window.
//	Since this is a time consuming task, it will be done on a separate PM
//	thread.
//
void *C_WINDOW_SUBSCRIPTION::MsgPopulate( void *mp1, void *mp2 )
{
	// Begin a thread to populate the subscription window
	xcPopulateThread.Create( PopulateSubscriptionThread, 40000, this );

	return FALSE;
}


//-----------------
// MsgSubSubscribe \
//---------------------------------------------------------------------------
// Event:	PM_SUB_SUBSCRIBE
// Cause:	Issued by parent window when a new subscription is sent
//			mp1 contains a pointer to the group string being subscribed
//
void *C_WINDOW_SUBSCRIPTION::MsgSubSubscribe( void *mp1, void *mp2 )
{
	char	*szGroup;
	FILE	*hFile;


	// Wait for any current subscription update to complete
	xcPopulateThread.WaitIndefinite();

	// Get the group that is being subscribed
	szGroup = (char *)mp1;
	pxcLog->Write( "PM_SUB_SUBSCRIBE:%s", szGroup );

	// Write the new group to the subscription file
	hFile = fopen( "groups.sub", "a" );
	if( hFile )
	{
		pxcLog->Write( "PM_SUB_SUBSCRIBE:Writing:%d",
		fprintf( hFile, "%s\n", szGroup ) );
		fclose( hFile );
	}

	// Repopulate the container
	PostMsg( PM_POPULATE, 0, 0 );

	return FALSE;
}


//----------
// MsgClose \
//---------------------------------------------------------------------------
// Event:	WM_CLOSE
// Cause:	Issued by OS when window is closed
//
void *C_WINDOW_SUBSCRIPTION::MsgClose( void *mp1, void *mp2 )
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
	xcIni.Write( "SubX", szString );
	sprintf( szString, "%d", iY );
	xcIni.Write( "SubY", szString );
	sprintf( szString, "%d", iW );
	xcIni.Write( "SubW", szString );
	sprintf( szString, "%d", iL );
	xcIni.Write( "SubL", szString );

	// Save the font
	pxcCont->GetFont( szString );
	if( strstr( szString, "." ) )
	{
		xcIni.Write( "SubFont", strstr( szString, "." ) + 1 );
		*strstr( szString, "." ) = 0;
		xcIni.Write( "SubFontSize", szString );
	}

	// Save the window foreground color
	pxcCont->GetForegroundColor( &byR, &byG, &byB );
	sprintf( szString, "%03d,%03d,%03d", byR, byG, byB );
	xcIni.Write( "SubFColor", szString );

	// Save the window background color
	pxcCont->GetBackgroundColor( &byR, &byG, &byB );
	sprintf( szString, "%03d,%03d,%03d", byR, byG, byB );
	xcIni.Write( "SubBColor", szString );
	xcIni.Close();

	// Wait for the unsubscribe thread to finish (if running)
	xcUnsubscribeThread.WaitIndefinite();

	// Kill any threads we own that are still running
	xcPopulateThread.Kill();

	// Commit suicide
	Destroy();

	// Tell the parent that the use told us to shut down.  Our parent will
	// clean up our mess (ie. call our destructor)
	pxcParent->PostMsg( PM_SUB_CLOSE, 0, 0 );

	return FALSE;
}


//---------
// MsgSize \
//---------------------------------------------------------------------------
// Event:	WM-SIZE
// Cause:	Issued by OS when window is resized
//
void *C_WINDOW_SUBSCRIPTION::MsgSize( void *mp1, void *mp2 )
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
void *C_WINDOW_SUBSCRIPTION::MsgControl( void *mp1, void *mp2 )
{
	switch( SHORT1FROMMP( mp1 ) )
	{
		// Got a message from the tool bar
		case D_SUB_TBAR:
			pxcTBar->Control( (ULONG)mp1 );
			break;

		// Got a control message from the container
		case D_CONT_SUB:
			// User double-clicked an item in the subscrioption container
			if(	SHORT2FROMMP( mp1 ) == CN_ENTER )
				SendMsg( WM_COMMAND, (void *)DM_SUB_READ, 0 );
			break;
	}
	return FALSE;
}


//-------------------
// CmdSubUnsubscribe \
//---------------------------------------------------------------------------
// Event:		DM_SUB_UNSUBSCRIBE
// Cause:		User selects the Unsubscribe button or menu option
// Description: This method is called any time the user wants to unsubscribe
//				to news group(s).  The method finds each selected item and
//				removes it from the list.
void *C_WINDOW_SUBSCRIPTION::CmdSubUnsubscribe( void *mp1, void *mp2 )
{
	// Begin a thread to unsubscribe all selected items
	xcUnsubscribeThread.Create( UnsubscribeThread, 40000, this );

	return FALSE;
}


//------------
// CmdSubRead \
//---------------------------------------------------------------------------
// Event:		DM_SUB_READ
// Cause:		User selects the Read button or menu option
// Description: This method is called any time the user wants to read the
//				message headers for a selected subscription.  If more than
//				one subscription is selected when this command is invoked,
//				the first selected subscription is used and the remainder
//				are ignored.
void *C_WINDOW_SUBSCRIPTION::CmdSubRead( void *mp1, void *mp2 )
{
	T_SUBRECORD	*pRecord;


	// Get the first selected group
	pRecord = (T_SUBRECORD *)pxcCont->FirstSelected();
	if( pRecord )
	{
		// Tell the parent to display the message list
		pxcParent->SendMsg( PM_SUB_READ, pRecord->szGroup, 0 );
	}

	return FALSE;
}

