//---------
// MSG.CPP \
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
//	C_WINDOW_MESSAGE class which is used to display and control the list
//	of news subjects for a given subscription.
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

//------------------------------
// Message Window Message Table \
//---------------------------------------------------------------------------
DECLARE_MSG_TABLE( xtMsgMessage )
	DECLARE_MSG( PM_CREATE,			C_WINDOW_MESSAGE::MsgCreate )
	DECLARE_MSG( PM_PARENT,			C_WINDOW_MESSAGE::MsgParent )
	DECLARE_MSG( PM_POPULATE,		C_WINDOW_MESSAGE::MsgPopulate )
	DECLARE_MSG( WM_CLOSE,			C_WINDOW_MESSAGE::MsgClose )
	DECLARE_MSG( WM_SIZE,			C_WINDOW_MESSAGE::MsgSize )
	DECLARE_MSG( WM_CONTROL,		C_WINDOW_MESSAGE::MsgControl )
	DECLARE_MSG( WM_PAINT,			C_WINDOW_STD::MsgPaint )
END_MSG_TABLE


//------------------------------
// Message Window Command Table \
//---------------------------------------------------------------------------
DECLARE_COMMAND_TABLE( xtCommandMsg )
	DECLARE_COMMAND( DM_MSG_READ,	C_WINDOW_MESSAGE::CmdMsgRead )
END_MSG_TABLE


//-----------
// SlowParse \
//---------------------------------------------------------------------------
//
//	Description:
//		This method is used to load message information from servers which
//		do not support the XOVER command.  It loads the header for each message
//		using the HEAD command and parses the information into a format which
//		is acceptible to News.
//
//	Parameters:
//		iConnection			- News connection to use for acquire
//		lFirst				- First message number in the range
//      lLast				- Last message number in the range
//
//	Returns:
//		int		- The number of messages found in the specified range.
//
int C_WINDOW_MESSAGE::SlowParse( int iConnection, ULONG lFirst, ULONG lLast )
{
	char	szHeader[4096];
	char	szAuthor[256];
	char	szSubject[256];
	char	szLines[256];
	FILE	*hFile;
	int		iCount;
	ULONG	lCtr;


	iCount = 0;
	hFile = fopen( "news.msg", "w" );
	if( hFile )
	{
		// go to the first article
		lCtr = lFirst;
		while( lCtr <= lLast )
		{
			// Tell the user what's going on with the loading process
			pxcStatus->Text( "Loading %ld of %ld", lCtr - lFirst + 1,
														lLast - lFirst + 1 );

			// Get the header for the new article and return the result
			sprintf( szHeader, "head %ld\r\n", lCtr );
			pxcMgr->Connection(iConnection)->Send( szHeader );
			pxcMgr->Connection(iConnection)->Receive( szHeader );
			if( atoi( szHeader ) >= 200 && atoi( szHeader ) < 300 )
			{
				// Skip the path string
				pxcMgr->Connection(iConnection)->Receive( szHeader );

				// Initial data areas
				memset( szAuthor, 0, 256 );
				memset( szSubject, 0, 256 );
				memset( szLines, 0, 256 );

				while( strcmp( szHeader, "." ) != 0 )
				{
					// Parse Lines
					if( strncmp( szHeader, "Lines:", 6 ) == 0 )
					{
						strncpy( szLines, szHeader + 7, 256 );
						if( strstr( szLines, "\r" ) )
							*strstr( szLines, "\r" ) = 0;
					}
					if( strncmp( szHeader, "From:", 5 ) == 0 )
					{
						strncpy( szAuthor, szHeader + 6, 256 );
						if( strstr( szAuthor, "\r" ) )
							*strstr( szAuthor, "\r" ) = 0;
					}
					if( strncmp( szHeader, "Subject:", 8 ) == 0 )
					{
						strncpy( szSubject, szHeader + 9, 256 );
						if( strstr( szSubject, "\r" ) )
							*strstr( szSubject, "\r" ) = 0;
					}

					pxcMgr->Connection(iConnection)->Receive( szHeader );
				}

				// Write this to the file and increment the message count
				sprintf( szHeader, "%ld\t%s\t%s\t%s",
									lCtr, szLines, szAuthor, szSubject );
				fprintf( hFile, "%s\n", szHeader );
				iCount++;
			}

			// Go to the next message
			lCtr++;
		}
		fclose( hFile );
	}

	return iCount;
}


//-----------
// FastParse \
//---------------------------------------------------------------------------
//
//	Description:
//		This method is used to load message information from servers that
//		do support the XOVER command.  It loads the headers in overview format
//		and parses the information into a format acceptible to News.
//
//	Parameters:
//		none
//
//	Returns:
//		int		- The number of messages found in the overview file.
//
int C_WINDOW_MESSAGE::FastParse( void )
{
	char	szString[2048];
	char	szHeader[4096];
	char	szAuthor[256];
	char	szSubject[256];
	char	szLines[256];
	char	*szPtr;
	FILE	*hFile;
	FILE	*hInFile;
	int		iCount;
	ULONG	lCtr;


	iCount = 0;
	hInFile = fopen( "news.ovr", "r" );
	if( hInFile )
	{
		hFile = fopen( "news.msg", "w" );
		if( hFile )
		{
			while( !feof( hInFile ) && fgets( szString, 2048, hInFile ) )
			{
				// Get rid of any CR/LF
				if( strstr( szString, "\r" ) )
					*strstr( szString, "\r" ) = 0;
				if( strstr( szString, "\n" ) )
					*strstr( szString, "\n" ) = 0;

				// Make sure every field has a value, even if it is blank
				while( strstr( szString, "\t\t" ) )
				{
					// Pad empty fields in the overview line with a space char
					szPtr = strstr( szString, "\t\t" ) + 1;
					memmove( szPtr + 1, szPtr, strlen( szPtr ) + 1 );
					*szPtr = ' ';
				}

				// Initial data areas
				memset( szAuthor, 0, 256 );
				memset( szSubject, 0, 256 );
				memset( szLines, 0, 256 );

				// Get next message number from overview file
				szPtr = strtok( szString, "\t" );
				if( szPtr )
					lCtr = atol( szPtr );

				// Get the subject text
				szPtr = strtok( NULL, "\t" );
				if( szPtr )
				{
					strncpy( szSubject, szPtr, 256 );
					if( strstr( szSubject, "\t" ) )
						*strstr( szSubject, "\t" ) = 0;
				}

				// Get the author text
				szPtr = strtok( NULL, "\t" );
				if( szPtr )
				{
					strncpy( szAuthor, szPtr, 256 );
					if( strstr( szAuthor, "\t" ) )
						*strstr( szAuthor, "\t" ) = 0;
				}

				// Skip the date,Message-ID and Reference
				szPtr = strtok( NULL, "\t" );
				szPtr = strtok( NULL, "\t" );
				szPtr = strtok( NULL, "\t" );
				szPtr = strtok( NULL, "\t" );

				// Get the Line text
				szPtr = strtok( NULL, "\t" );
				if( szPtr )
				{
					strncpy( szLines, szPtr, 256 );
					if( strstr( szLines, "\t" ) )
						*strstr( szLines, "\t" ) = 0;
				}

				// Write this to the file and increment the message count
				sprintf( szHeader, "%ld\t%s\t%s\t%s",
							lCtr, szLines, szAuthor, szSubject );
				fprintf( hFile, "%s\n", szHeader );

				// Count the number of items written to the output file
				iCount++;
			}

			// Close the output file
			fclose( hFile );
		}

		// CLose the overview file
		fclose( hInFile );
	}

	// Return the number of messages in the overview
	return iCount;
}


//-----------------------
// PopulateMessageThread \
//---------------------------------------------------------------------------
//
// Description:
//		This thread function is used to populate the message window
//		container.  It loads data from the message file, parses it, and writes
//		it to the container window.
//
void _Optlink PopulateMessageThread( void *pvData )
{
	char				szString[1024];
	C_WINDOW_MESSAGE	*pxcThis;
	C_THREAD_PM			*pxcThread;
	FILE				*hFile;
	int					iCount;
	int					iConnection;
	int					iResult;
	T_MSGRECORD			*pRecord;
	T_MSGRECORD			*pFirstRecord;
	ULONG				lStart;
	ULONG				lLast;
	ULONG				lTotal;


	// Get a point to the main window object
	pxcThread = (C_THREAD_PM *)pvData;
	pxcThis = (C_WINDOW_MESSAGE *)pxcThread->ThreadData();

	// Create a PM process for this thread
	pxcThread->InitializeThread();

	// Get a network connection
	iConnection = pxcMgr->Connect();
	if( iConnection >= 0 )
	{
		// Get the group information and article status
		iResult = pxcMgr->Connection(iConnection)->Group(
								pxcThis->Group(), &lStart, &lLast, &lTotal );
		if( iResult >= 200 && iResult <= 300 )
		{
			pxcThis->Status()->Text( "Loading Overview..." );

			// Try to do an overview of the messages
			iResult = pxcMgr->Connection(iConnection)->Overview(
											lStart, lLast, "news.ovr" );
			if( iResult >= 200 && iResult < 300 )
			{
				// Overview supported, so do a quick parse
				iCount = pxcThis->FastParse();

				// Get rid of the overview file
				DosForceDelete( "news.ovr" );
			}
			else
			{
				// Overview not supported. so do the listing the slow way
				iCount = pxcThis->SlowParse( iConnection, lStart, lLast );
			}
		}

		// Free up the network connection
		pxcMgr->Disconnect( iConnection );

		// Remove any previous data because this could be an update
		pxcThis->Container()->RemoveAll();

		// Insert groups if there are any
		if( iCount > 0 )
		{
			// Allocate some container space, but keep track of where it starts
			pFirstRecord = (T_MSGRECORD *)pxcThis->Container()->Allocate(
								sizeof( T_MSGRECORD ), (USHORT)iCount );
			pRecord = pFirstRecord;

			// Now insert each supported record into the container
			hFile = fopen( "news.msg", "r" );
			while( !feof( hFile ) && fgets( szString, 1024, hFile ) )
			{
				// Get rid of any CR/LF
				if( strstr( szString, "\r" ) )
					*strstr( szString, "\r" ) = 0;
				if( strstr( szString, "\n" ) )
					*strstr( szString, "\n" ) = 0;

				// Insert this record
				pRecord = (T_MSGRECORD *)pxcThis->Container()->Fill(
														pRecord, szString );
			}

			// Perform the container insertion
			pxcThis->Container()->Insert( 0, pFirstRecord, iCount );

			// Redraw the container to show the subscriptions
			pxcThis->Container()->Redraw( 0 );

			fclose( hFile );

			// Get rid of the message file
			DosForceDelete( "news.msg" );
		}
	}

	// Terminate the thread
	pxcThread->TerminateThread();
}


//-------------------
// LoadMessageThread \
//---------------------------------------------------------------------------
//
// Description:
//		This thread function is used to load the text for a message into a
//		disk file on the local system.  Once loaded, the code sends a message
//		back to message to display the article in an article window.
//
void _Optlink LoadMessageThread( void *pvData )
{
	C_WINDOW_MESSAGE	*pxcThis;
	C_THREAD_PM			*pxcThread;
	int					iConnection;
	int					iResult;
	T_MSGRECORD			*pRecord;
	ULONG				lStart;
	ULONG				lLast;
	ULONG				lTotal;


	// Get a point to the main window object
	pxcThread = (C_THREAD_PM *)pvData;
	pxcThis = (C_WINDOW_MESSAGE *)pxcThread->ThreadData();

	// Create a PM process for this thread
	pxcThread->InitializeThread();

	// Find out which record is selected
	pRecord = (T_MSGRECORD *)pxcThis->Container()->FirstSelected();

	// Get a network connection
	iConnection = pxcMgr->Connect();
	if( iConnection >= 0 )
	{
		// Get the group information and article status
		iResult = pxcMgr->Connection(iConnection)->Group(
								pxcThis->Group(), &lStart, &lLast, &lTotal );
		if( iResult >= 200 && iResult <= 300 )
		{
			pxcThis->Status()->Text( "Loading Article..." );

			// Try to load the article
			iResult = pxcMgr->Connection(iConnection)->Article(
								atol( pRecord->szNumber ), "article.txt" );
			if( iResult >= 200 && iResult < 300 )
			{
				// Tell the parent to create an article window for this.
				pxcThis->Parent()->SendMsg( PM_MSG_READ,
												(void *)"article.txt", 0 );
			}
		}

		// Free up the network connection
		pxcMgr->Disconnect( iConnection );
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
C_WINDOW_MESSAGE::C_WINDOW_MESSAGE( void ) : C_WINDOW_STD( xtMsgMessage )
{
	// Enable the required handlers for this window
	CommandTable( xtCommandMsg );
}


//------------
// Destructor \
//---------------------------------------------------------------------------
C_WINDOW_MESSAGE::~C_WINDOW_MESSAGE( void )
{
	pxcLog->Write( "MESSAGE:Destructor:Start" );

	// Free up the child windows
	delete pxcTBar;
	delete pxcStatus;
	delete pxcCont;

	pxcLog->Write( "MESSAGE:Destructor:End" );
}


//-----------
// MsgCreate \
//---------------------------------------------------------------------------
// Event:	WM_CREATE
// Cause:	Issued by OS when window is created
//
void *C_WINDOW_MESSAGE::MsgCreate( void *mp1, void *mp2 )
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
	pxcTBar = (C_TOOLBAR_MSG *)new C_TOOLBAR_MSG( this, pxcStatus );

	// Create a new container to display message listing
	pxcCont = (C_CONTAINER_MSG *)new C_CONTAINER_MSG( this );

	// Load parameters out of the INI file
	C_INI_USER xcIni( "BookNews" );
	xcIni.Open();
	xcIni.Read( "MsgFont", szFont, "System Proportional", 80 );
	xcIni.Read( "MsgFontSize", szFontSize, "10", 10 );
	xcIni.Read( "MsgBColor", szBColor, "000,000,000", 80 );
	xcIni.Read( "MsgFColor", szFColor, "255,255,255", 80 );
	xcIni.Read( "MsgX", szX, "0", 10 );
	xcIni.Read( "MsgY", szY, "0", 10 );
	xcIni.Read( "MsgW", szW, "0", 10 );
	xcIni.Read( "MsgL", szL, "0", 10 );
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
void *C_WINDOW_MESSAGE::MsgParent( void *mp1, void *mp2 )
{
	// Populate the parent object attribute within the class
	pxcParent = (C_WINDOW_STD *)mp1;

	return FALSE;
}


//-------------
// MsgPopulate \
//---------------------------------------------------------------------------
// Event:		PM_POPULATE
// Cause:		Issued by the main window whenever it wants us to display a new
//				message.
// Description:	This method is invoked whenever the main window wants to change
//				the group for this window is listing messages.  The mp1
//				parameter contains a string holding the name of the new group
//
void *C_WINDOW_MESSAGE::MsgPopulate( void *mp1, void *mp2 )
{
	char	szString[64];


	// Get the group name to populate
	strcpy( szGroup, (char *)mp1 );
	pxcLog->Write( "MESSAGE:MsgPopulate:%s", szGroup );

	// Set the window title to the first 56 characters of the group name
	// This is a limitation of OS/2
	memset( szString, 0, 64 );
	strncpy( szString, szGroup, 56 );
	SetTitle( szString );

	// Begin a thread to load the messages
	xcPopulateThread.Create( PopulateMessageThread, 40000, this );

	return FALSE;
}


//----------
// MsgClose \
//---------------------------------------------------------------------------
// Event:	WM_CLOSE
// Cause:	Issued by OS when window is closed
//
void *C_WINDOW_MESSAGE::MsgClose( void *mp1, void *mp2 )
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
	xcIni.Write( "MsgX", szString );
	sprintf( szString, "%d", iY );
	xcIni.Write( "MsgY", szString );
	sprintf( szString, "%d", iW );
	xcIni.Write( "MsgW", szString );
	sprintf( szString, "%d", iL );
	xcIni.Write( "MsgL", szString );

	// Save the font
	pxcCont->GetFont( szString );
	if( strstr( szString, "." ) )
	{
		xcIni.Write( "MsgFont", strstr( szString, "." ) + 1 );
		*strstr( szString, "." ) = 0;
		xcIni.Write( "MsgFontSize", szString );
	}

	// Save the window foreground color
	pxcCont->GetForegroundColor( &byR, &byG, &byB );
	sprintf( szString, "%03d,%03d,%03d", byR, byG, byB );
	xcIni.Write( "MsgFColor", szString );

	// Save the window background color
	pxcCont->GetBackgroundColor( &byR, &byG, &byB );
	sprintf( szString, "%03d,%03d,%03d", byR, byG, byB );
	xcIni.Write( "MsgBColor", szString );
	xcIni.Close();

	// Kill any threads we own that are still running
	xcLoadThread.Kill();
	xcPopulateThread.Kill();

	// Commit suicide
	Destroy();

	// Tell the parent that the use told us to shut down.  Our parent will
	// clean up our mess (ie. call our destructor)
	pxcParent->PostMsg( PM_MSG_CLOSE, 0, 0 );

	return FALSE;
}


//---------
// MsgSize \
//---------------------------------------------------------------------------
// Event:	WM-SIZE
// Cause:	Issued by OS when window is resized
//
void *C_WINDOW_MESSAGE::MsgSize( void *mp1, void *mp2 )
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
void *C_WINDOW_MESSAGE::MsgControl( void *mp1, void *mp2 )
{
	switch( SHORT1FROMMP( mp1 ) )
	{
		// Got a control message from the toolbar
		case D_MSG_TBAR:
			pxcTBar->Control( (ULONG)mp1 );
			break;

		// Got a control message from the message list container
		case D_CONT_MSG:
			// User double-clicked on an item in the list
			if(	SHORT2FROMMP( mp1 ) == CN_ENTER )
				SendMsg( WM_COMMAND, (void *)DM_MSG_READ, 0 );
			break;
	}
	return FALSE;
}


//------------
// CmdMsgRead \
//---------------------------------------------------------------------------
// Event:		DM_MSG_READ
// Cause:		User selects the Read button or menu option
// Description: This method is called any time the user wants to read the
//				article text for a specified message.
//				If more than one message is selected when this command is
//				invoked, the first selected subscription is used and the
//				remainder are ignored.
void *C_WINDOW_MESSAGE::CmdMsgRead( void *mp1, void *mp2 )
{
	T_MSGRECORD	*pRecord;


	// Get the first selected group
	pxcLog->Write( "MESSAGE:CmdMsgRead:Start" );
	pRecord = (T_MSGRECORD *)pxcCont->FirstSelected();
	if( pRecord )
	{
		// Start a thread to load the article
		xcLoadThread.Create( LoadMessageThread, 40000, this );
	}
	pxcLog->Write( "MESSAGE:CmdMsgRead:End" );

	return FALSE;
}

