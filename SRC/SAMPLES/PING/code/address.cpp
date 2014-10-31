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

//-----------------
// NVCLASS Headers \
//---------------------------------------------------------------------------
#include <thread.hpp>
#include <threadpm.hpp>

//---------------------
// Application Headers \
//---------------------------------------------------------------------------
#include "rc.hpp"
#include <tbartop.hpp>
#include <address.hpp>
#include <ping.hpp>


//------------------------------
// Address Window Message Table \
//---------------------------------------------------------------------------
DECLARE_MSG_TABLE( xtMsgAddress )
	DECLARE_MSG( PM_CREATE,	C_DIALOG_ADDRESS::MsgInitDlg )
	DECLARE_MSG( WM_DESTROY,C_DIALOG_ADDRESS::MsgDestroy )
	DECLARE_MSG( PM_PARENT,	C_DIALOG_ADDRESS::MsgParent )
END_MSG_TABLE


//------------------------------
// Address Window Command Table \
//---------------------------------------------------------------------------
DECLARE_COMMAND_TABLE( xtCommandAddress )
	DECLARE_COMMAND( DM_CANCEL,	C_DIALOG_ADDRESS::CmdCancel )
	DECLARE_COMMAND( DM_OK,	C_DIALOG_ADDRESS::CmdOK )
END_COMMAND_TABLE


//-------------
// Constructor \
//---------------------------------------------------------------------------
C_DIALOG_ADDRESS::C_DIALOG_ADDRESS( C_WINDOW *pxcParentObj, int iID )
								: C_DIALOG( pxcParentObj, xtMsgAddress )
{
	// Enable the required handlers for this window
	CommandTable( xtCommandAddress );

	Create( iID );
}


//------------
// MsgInitDlg \
//---------------------------------------------------------------------------
// Event:	PM_CREATE
// Cause:	Issued by OS when dialog is created
//
void *C_DIALOG_ADDRESS::MsgInitDlg( void *mp1, void *mp2 )
{
	pxcEditAddress = (C_EDIT *) new C_EDIT( (C_DIALOG *)this, DE_ADDRESS );

	// Set the text limit of the address field
	pxcEditAddress->SendMsg( EM_SETTEXTLIMIT, MPFROMSHORT( 256 ), 0);

	// Give the edit window the focus
	pxcEditAddress->Focus();

	// Prevent the default window procedure from executing
	return (MRESULT)TRUE;
}


//------------
// MsgDestroy \
//---------------------------------------------------------------------------
// Event:	WM_DESTROY
// Cause:	Issued by OS when window is destroyed
//
void *C_DIALOG_ADDRESS::MsgDestroy( void *mp1, void *mp2 )
{
	// Dispose of the dydnamic memory used by this object
	delete pxcEditAddress;

	return FALSE;
}


//-----------
// MsgParent \
//---------------------------------------------------------------------------
// Event:	PM_PARENT
// Cause:	Issued by parent to identify itself to this child
//
void *C_DIALOG_ADDRESS::MsgParent( void *mp1, void *mp2 )
{
	// Save the pointer to the parent window object
	pxcParent = (C_WINDOW *)mp1;

	return FALSE;
}


//-----------
// CmdCancel \
//---------------------------------------------------------------------------
// Event:	DM_CANCEL
// Cause:	User selects the Cancel button-
//
void *C_DIALOG_ADDRESS::CmdCancel( void *mp1, void *mp2 )
{
	// Close this dialog
	Close( FALSE );

	return FALSE;
}


//-------
// CmdOK \
//---------------------------------------------------------------------------
// Event:	DM_OK
// Cause:	User selects the OK button-
//
void *C_DIALOG_ADDRESS::CmdOK( void *mp1, void *mp2 )
{
	char	szString[256];


	// Tell the parent what server was picked
	pxcEditAddress->GetText( szString, 256 );

	// Tell the window who its parent is so it can talk back
	if( strlen( szString ) > 0 )
		pxcParent->SendMsg( PM_DONE, szString, 0 );

	// Close this dialog
	Close( FALSE );

	return FALSE;
}


