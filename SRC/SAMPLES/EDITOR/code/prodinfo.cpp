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
#include <prodinfo.hpp>


//---------------------------
// Main Window Message Table \
//---------------------------------------------------------------------------
DECLARE_MSG_TABLE( xtEditorProdInfo )
	DECLARE_MSG( PM_CREATE,		C_DIALOG_ABOUT::MsgCreate )
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
						: C_DIALOG( pxcParentObj, xtEditorProdInfo )
{
	// Enable the required handlers for this window
	CommandTable( xtCommandAbout );

	// Create the physical dialog
	Create( iID );

	// Begin processing the about box
	Process();
}


//-----------
// MsgCreate \
//---------------------------------------------------------------------------
// Event:		PM_CREATE
// Cause:		Issued by OS when window is created
// Description:	This method gets called when the window is initially created.
//				It initializes all of the visual aspects of the class.
void *C_DIALOG_ABOUT::MsgCreate( void *mp1, void *mp2 )
{
	DosBeep( 100, 100 );

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

