//-------------
// ARTICLE.CPP \
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
//	C_WINDOW_ARTICLE class which is used to display article text for the user.
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

//------------------------------
// Article Window Message Table \
//---------------------------------------------------------------------------
DECLARE_MSG_TABLE( xtMsgArticle )
	DECLARE_MSG( PM_CREATE,			C_WINDOW_ARTICLE::MsgCreate )
	DECLARE_MSG( PM_ART_PARENT,		C_WINDOW_ARTICLE::MsgParent )
	DECLARE_MSG( PM_ART_POPULATE,	C_WINDOW_ARTICLE::MsgPopulate )
	DECLARE_MSG( WM_CLOSE,			C_WINDOW_ARTICLE::MsgClose )
	DECLARE_MSG( WM_SIZE,			C_WINDOW_ARTICLE::MsgSize )
	DECLARE_MSG( WM_CONTROL,		C_WINDOW_ARTICLE::MsgControl )
	DECLARE_MSG( WM_PAINT,			C_WINDOW_STD::MsgPaint )
END_MSG_TABLE


//-----------------------------
// Artcle Window Command Table \
//---------------------------------------------------------------------------
//DECLARE_COMMAND_TABLE( xtCommandArt )
//END_MSG_TABLE


//----------------
// PopulateThread \
//---------------------------------------------------------------------------
//
//	Description:
//		This thread function loads the article window's MLE control with the
//		text from the message file.
//
void _Optlink PopulateArticleThread( void *pvData )
{
	C_WINDOW_ARTICLE	*pxcThis;
	C_THREAD_PM			*pxcThread;


	// Get a point to the main window object
	pxcThread = (C_THREAD_PM *)pvData;
	pxcThis = (C_WINDOW_ARTICLE *)pxcThread->ThreadData();

	// Create a PM process for this thread
	pxcThread->InitializeThread();

	// Load the multiline editor with the article file
	pxcThis->MLE()->Load( pxcThis->Status(), pxcThis->Article() );

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
C_WINDOW_ARTICLE::C_WINDOW_ARTICLE( void )
								: C_WINDOW_STD( xtMsgArticle )
{
	// Enable the required handlers for this window
//	CommandTable( xtCommandArt );
}


//------------
// Destructor \
//---------------------------------------------------------------------------
C_WINDOW_ARTICLE::~C_WINDOW_ARTICLE( void )
{
	pxcLog->Write( "ARTICLE:Destructor:Start" );

	// Free up the child windows
	delete pxcTBar;
	delete pxcStatus;
	delete pxcMLE;

	pxcLog->Write( "ARTICLE:Destructor:End" );
}


//-----------
// MsgCreate \
//---------------------------------------------------------------------------
// Event:	WM_CREATE
// Cause:	Issued by OS when window is created
//
void *C_WINDOW_ARTICLE::MsgCreate( void *mp1, void *mp2 )
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
	pxcTBar = (C_TOOLBAR_ART *)new C_TOOLBAR_ART( this, pxcStatus );

	// Create a new container to display the article
	pxcMLE = (C_MLE *)new C_MLE( this, D_ART_MLE,
							MLS_VSCROLL | MLS_HSCROLL | MLS_BORDER );

	// Load parameters out of the INI file
	C_INI_USER xcIni( "BookNews" );
	xcIni.Open();
	xcIni.Read( "ArtFont", szFont, "System Proportional", 80 );
	xcIni.Read( "ArtFontSize", szFontSize, "10", 80 );
	xcIni.Read( "ArtBColor", szBColor, "000,000,000", 80 );
	xcIni.Read( "ArtFColor", szFColor, "255,255,255", 80 );
	xcIni.Read( "ArtX", szX, "0", 10 );
	xcIni.Read( "ArtY", szY, "0", 10 );
	xcIni.Read( "ArtW", szW, "0", 10 );
	xcIni.Read( "ArtL", szL, "0", 10 );
	xcIni.Close();

	// Set the font in the MLE
	pxcMLE->SetFont( szFont, atoi( szFontSize ) );

	// Set the MLE colors
	pxcMLE->SetForegroundColor( atoi( &szFColor[0] ),
								atoi( &szFColor[4] ), atoi( &szFColor[8] ) );
	pxcMLE->SetBackgroundColor( atoi( &szBColor[0] ),
								atoi( &szBColor[4] ), atoi( &szBColor[8] ) );

	if( atoi( szW ) != 0 && atoi( szL ) != 0 )
	{
		// Position and size the window
		SetSizePosition( atoi( szX ), atoi( szY ), atoi( szW ), atoi( szL ) );
	}

	pxcMLE->Focus();
	return FALSE;
}



//-----------
// MsgParent \
//---------------------------------------------------------------------------
// Event:	PM_PARENT
// Cause:	Issued by parent window to idetify itself as the owner
//
void *C_WINDOW_ARTICLE::MsgParent( void *mp1, void *mp2 )
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
//	This method populates the container object within the group window.  Since
//	this is a time-consuming task, it will be done on a separate PM thread.
//
void *C_WINDOW_ARTICLE::MsgPopulate( void *mp1, void *mp2 )
{
	// Get the filename we're supposed to load
	strcpy( szArticle, (char *)mp1 );

	// Begin a thread to load the article text
	xcPopulateThread.Create( PopulateArticleThread, 40000, this );

	return FALSE;
}


//----------
// MsgClose \
//---------------------------------------------------------------------------
// Event:	WM_CLOSE
// Cause:	Issued by OS when window is closed
//
void *C_WINDOW_ARTICLE::MsgClose( void *mp1, void *mp2 )
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
	xcIni.Write( "ArtX", szString );
	sprintf( szString, "%d", iY );
	xcIni.Write( "ArtY", szString );
	sprintf( szString, "%d", iW );
	xcIni.Write( "ArtW", szString );
	sprintf( szString, "%d", iL );
	xcIni.Write( "ArtL", szString );

	// Save the font
	pxcMLE->GetFont( szString );
	if( strstr( szString, "." ) )
	{
		xcIni.Write( "ArtFont", strstr( szString, "." ) + 1 );
		*strstr( szString, "." ) = 0;
		xcIni.Write( "ArtFontSize", szString );
	}

	// Save the window foreground color
	pxcMLE->GetForegroundColor( &byR, &byG, &byB );
	sprintf( szString, "%03d,%03d,%03d", byR, byG, byB );
	xcIni.Write( "ArtFColor", szString );

	// Save the window background color
	pxcMLE->GetBackgroundColor( &byR, &byG, &byB );
	sprintf( szString, "%03d,%03d,%03d", byR, byG, byB );
	xcIni.Write( "ArtBColor", szString );
	xcIni.Close();

	// Kill any threads we own that are still running
	xcPopulateThread.Kill();

	// Commit suicide
	Destroy();

	// Tell the parent that the use told us to shut down.  Our parent will
	// clean up our mess (ie. call our destructor)
	pxcParent->PostMsg( PM_ART_CLOSE, 0, 0 );

	return FALSE;
}


//---------
// MsgSize \
//---------------------------------------------------------------------------
// Event:	WM_SIZE
// Cause:	Issued by OS when window is resized
//
void *C_WINDOW_ARTICLE::MsgSize( void *mp1, void *mp2 )
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
	pxcMLE->SetSizePosition( 0, 0, iCX, iCY );

	return FALSE;
}


//------------
// MsgControl \
//---------------------------------------------------------------------------
// Event:	WM_CONTROL
// Cause:	Issued by OS for control functions (Toolbar interaction)
//
void *C_WINDOW_ARTICLE::MsgControl( void *mp1, void *mp2 )
{
	switch( SHORT1FROMMP( mp1 ) )
	{
		// Got a control message from the toolbar
		case D_ART_TBAR:
			pxcTBar->Control( (ULONG)mp1 );
			break;
	}
	return FALSE;
}

