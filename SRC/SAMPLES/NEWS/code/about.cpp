//-----------
// ABOUT.CPP \
//---------------------------------------------------------------------------
//
//	All information contained in this file is Copyright (c) 1994-1995 by
//	Steven Gutz.  All Rights Reserved
//
//	This source code shall not be implemented, duplicated or used as the basis
//	for any product without prior written consent by the author
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
#include <log.hpp>

//---------------------
// Application Headers \
//---------------------------------------------------------------------------
#include "rc.hpp"
#include <about.hpp>


//----------------------------
// About Window Message Table \
//---------------------------------------------------------------------------
DECLARE_MSG_TABLE( xtMsgAbout )
	DECLARE_MSG( PM_CREATE,	C_DIALOG_ABOUT::MsgInitDlg )
END_MSG_TABLE


//----------------------------
// About Window Command Table \
//---------------------------------------------------------------------------
DECLARE_COMMAND_TABLE( xtCommandAbout )
	DECLARE_COMMAND( DM_OK,	C_DIALOG_ABOUT::CmdOK )
END_COMMAND_TABLE


//-------------
// Constructor \
//---------------------------------------------------------------------------
C_DIALOG_ABOUT::C_DIALOG_ABOUT( C_WINDOW *pxcParentObj, int iID )
								: C_DIALOG( pxcParentObj, xtMsgAbout )
{
	// Enable the required handlers for this window
	CommandTable( xtCommandAbout );

	// Create the physical dialog
	Create( iID );

	// Begin processing the about box
	Process();
}


//------------
// MsgInitDlg \
//---------------------------------------------------------------------------
// Event:	PM_CREATE
// Cause:	Issued by OS when dialog is created
//
void *C_DIALOG_ABOUT::MsgInitDlg( void *mp1, void *mp2 )
{
	char	szString[256];


	// Create an edit control instance and associate it with the version
	// string in the dialog (ID 105)
	pxcEditVersion = (C_EDIT *) new C_EDIT( (C_DIALOG *)this, 105 );

	// Format the real version string and insert it into the dialog
	sprintf( szString, "Version: %s", D_VERSION );
	pxcEditVersion->SetText( szString );

	// We can dispose of the edit control object now
	delete pxcEditVersion;

	return FALSE;
}


//-------
// CmdOK \
//---------------------------------------------------------------------------
// Event:	DM_OK
// Cause:	User selects the OK button-
//
void *C_DIALOG_ABOUT::CmdOK( void *mp1, void *mp2 )
{
	// Close this dialog
	Close( FALSE );

	return FALSE;
}

