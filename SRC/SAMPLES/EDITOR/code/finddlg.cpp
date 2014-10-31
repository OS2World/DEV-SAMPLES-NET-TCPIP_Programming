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
// NVCLASS Headers \
//---------------------------------------------------------------------------
#include <thread.hpp>
#include <threadpm.hpp>
#include <semev.hpp>

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
#include <tbartop.hpp>
#include <finddlg.hpp>
#include <editor.hpp>


//---------------------------
// Main Window Message Table \
//---------------------------------------------------------------------------
DECLARE_MSG_TABLE( xtEditorFind )
	DECLARE_MSG( PM_CREATE,		C_DIALOG_FIND::MsgCreate )
END_MSG_TABLE

//----------------------------
// About Window Command Table \
//---------------------------------------------------------------------------
DECLARE_COMMAND_TABLE( xtCommandFind )
	DECLARE_COMMAND( DM_FIND,			C_DIALOG_FIND::CmdFind )
	DECLARE_COMMAND( DM_CHANGE_FIND,	C_DIALOG_FIND::CmdChangeFind )
	DECLARE_COMMAND( DM_CHANGE_ALL,		C_DIALOG_FIND::CmdChangeAll )
	DECLARE_COMMAND( DM_CANCEL,			C_DIALOG_FIND::CmdCancel )
END_COMMAND_TABLE


//-------------
// Constructor \
//---------------------------------------------------------------------------
C_DIALOG_FIND::C_DIALOG_FIND( C_WINDOW *pxcParentObj, int iID )
						: C_DIALOG( pxcParentObj, xtEditorFind )
{
	// Enable the required handlers for this window
	CommandTable( xtCommandFind );

	// Create the physical dialog
	Create( iID );

	// Begin processing the dialog box
	Process();
}


//-----------
// MsgCreate \
//---------------------------------------------------------------------------
// Event:		PM_CREATE
// Cause:		Issued by OS when window is created
// Description:	This method gets called when the window is initially created.
//				It initializes all of the visual aspects of the class.
void *C_DIALOG_FIND::MsgCreate( void *mp1, void *mp2 )
{
	C_EDIT		*pxcEdit;


	// Get the find text
	pxcEdit = (C_EDIT *) new C_EDIT( (C_DIALOG *)this, DE_FIND_TEXT );

	WinSetFocus( HWND_DESKTOP, pxcEdit->Window() );

	// Dispose of the edit field object
	delete pxcEdit;

	return (void *)TRUE;
}


//---------
// CmdFind \
//---------------------------------------------------------------------------
// Event:	DM_FIND
// Cause:	User selects the Find button
//
void *C_DIALOG_FIND::CmdFind( void *mp1, void *mp2 )
{
	char		szString[256];
	C_EDIT		*pxcFind;
	int			iCase;


	// Determine the case sensitivity flag
	iCase = (int)WinSendDlgItemMsg( Window(), DC_CASE_SENSITIVE,
													BM_QUERYCHECK, 0, 0 );
	ParentObject()->SendMsg( PM_FIND_SET_CASE, (void *)iCase, 0 );

	// Get the find text
	pxcFind = (C_EDIT *) new C_EDIT( (C_DIALOG *)this, DE_FIND_TEXT );

	// Send a message to our parent telling it to find the next instance
	// of this text
	pxcFind->GetText( szString, 256 );
	ParentObject()->SendMsg( PM_FIND, (void *)szString, 0 );

	// Dispose of the edit field object
	delete pxcFind;

	return FALSE;
}


//---------------
// CmdChangeFind \
//---------------------------------------------------------------------------
// Event:	DM_CHANGE_FIND
// Cause:	User selects the "Change then Find" button
//
void *C_DIALOG_FIND::CmdChangeFind( void *mp1, void *mp2 )
{
	char		szChange[256];
	char		szString[256];
	C_EDIT		*pxcFind;
	C_EDIT		*pxcChange;
	int			iCase;


	// Determine the case sensitivity flag
	iCase = (int)WinSendDlgItemMsg( Window(), DC_CASE_SENSITIVE,
													BM_QUERYCHECK, 0, 0 );
	ParentObject()->SendMsg( PM_FIND_SET_CASE, (void *)iCase, 0 );


	// Get the find text
	pxcFind = (C_EDIT *) new C_EDIT( (C_DIALOG *)this, DE_FIND_TEXT );
	pxcChange = (C_EDIT *) new C_EDIT( (C_DIALOG *)this, DE_CHANGE_TEXT );


	// Send a message to our parent telling it to change then find
	pxcFind->GetText( szString, 256 );
	pxcChange->GetText( szChange, 256 );
	ParentObject()->SendMsg( PM_CHANGE_FIND,
							(void *)szString, (void *)szChange );

	// Dispose of the edit field objects
	delete pxcFind;
	delete pxcChange;

	return FALSE;
}


//-----------
// CmdChange \
//---------------------------------------------------------------------------
// Event:	DM_CHANGE_ALL
// Cause:	User selects the ChangeAll button
//
void *C_DIALOG_FIND::CmdChangeAll( void *mp1, void *mp2 )
{
	char		szChange[256];
	char		szString[256];
	C_EDIT		*pxcFind;
	C_EDIT		*pxcChange;
	int			iCase;


	// Determine the case sensitivity flag
	iCase = (int)WinSendDlgItemMsg( Window(), DC_CASE_SENSITIVE,
													BM_QUERYCHECK, 0, 0 );
	ParentObject()->SendMsg( PM_FIND_SET_CASE, (void *)iCase, 0 );

	// Get the find text
	pxcFind = (C_EDIT *) new C_EDIT( (C_DIALOG *)this, DE_FIND_TEXT );
	pxcChange = (C_EDIT *) new C_EDIT( (C_DIALOG *)this, DE_CHANGE_TEXT );

	// Send a message to our parent telling it to change all
	pxcFind->GetText( szString, 256 );
	pxcChange->GetText( szChange, 256 );
	ParentObject()->SendMsg( PM_CHANGE_ALL,
							(void *)szString, (void *)szChange );

	// Dispose of the edit field objects
	delete pxcFind;
	delete pxcChange;

	return FALSE;
}


//-----------
// CmdCancel \
//---------------------------------------------------------------------------
// Event:	DM_CANCEL
// Cause:	User selects the Cancel button
//
void *C_DIALOG_FIND::CmdCancel( void *mp1, void *mp2 )
{
	// Close this dialog
	Close( FALSE );

	return FALSE;
}

