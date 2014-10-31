//------------
// DIALOG.CPP \
//---------------------------------------------------------------------------
//
//	All information contained in this file is Copyright (c) 1994-1995 by
//	NeoLogic Inc.  All Rights Reserved
//
//	This source code shall not be implemented, duplicated or used as the basis
//	for any product without prior written consent by NeoLogic Inc.
//
// Description:
//
//	This file contains the methods to describe the C_DIALOG class.
//	C_DIALOG provides a class used to create instances of dialog boxes
//	which are designed and stored in the program resources area.  These
//	dialogs are typically developed using a dialog editor such as Borland's
//	Resource Workshop.
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
#include <string.h>

//-----------------
// PMCLASS Headers \
//---------------------------------------------------------------------------
#include <window.hpp>
#include <winchild.hpp>
#include <dialog.hpp>


MRESULT EXPENTRY StdDlgProc( HWND hWnd, ULONG lMsg, MPARAM mp1, MPARAM mp2 );


//-------------
// Constructor \
//---------------------------------------------------------------------------
//
// Description:
//		This constructor creates an instance of the C_DIALOG class.  It accepts
//		a pointer to a parent window object and a pointer to a message table
//		and passes both of these to its parent constructor.
//
//	Parameters:
//		pxcParentObj		- Pointer to owner object window
//		pxtMsg				- Pointer to message table for this class
//
C_DIALOG::C_DIALOG( C_WINDOW *pxcParentObj, T_MSG_TABLE *pxtMsg ) :
								C_WINDOW_CHILD( pxcParentObj, pxtMsg )
{
}


//--------
// Create \
//---------------------------------------------------------------------------
//
//	Description:
//		This method is used to create the window associated with this instance.
//		It first loads the dialog resource from the resource section of the EXE,
//		then calls the C_WINDOW::Create() method to associate the object
//		with its parent window.  The owner of the dialog window is set to the
//		WPS desktop.
//
//	Parameters:
//		iID					- Resource identifier of the dialog
//
//	Returns:
//		void
//
void C_DIALOG::Create( int iID )
{
	HWND			hWnd;


	// Load the dialog box into memory
	hWnd = WinLoadDlg( HWND_DESKTOP, ParentObject()->Window(),
										StdDlgProc, 0, iID, this );

	// Tell the C_WINDOW portion of the code about the dialog
	C_WINDOW::Create( ParentObject()->Window(), hWnd );
}


//------------------
// CreateWithParent \
//---------------------------------------------------------------------------
//
//	Description:
//		This method is used to create the window associated with this instance.
//		It first loads the dialog resource from the resource section of the EXE,
//		then calls the C_WINDOW::Create() method to associate the object
//		with its owner window.  Both the parent and owner of this dialog window
//		are set to the parent window.
//
//		This creation technique is used when the dialog requires an owner window
//		which is not the WPS desktop. For example a notebook dialog page must be
//		owned by the notebook control.
//
//	Parameters:
//		iID					- Resource identifier of the dialog
//
//	Returns:
//		void
//
void C_DIALOG::CreateWithParent( int iID )
{
	HWND			hWnd;


	// Load the dialog box into memory
	hWnd = WinLoadDlg( ParentObject()->Window(), ParentObject()->Window(),
										StdDlgProc, 0, iID, this );

	// Tell the C_WINDOW portion of the code about the dialog
	C_WINDOW::Create( ParentObject()->Window(), hWnd );
}


//---------
// Process \
//---------------------------------------------------------------------------
//
//	Description:
//		This method is a simple wrapper that calls the standard API function
//		to process the dialog box.
//
//	Parameters:
//		none
//
//	Returns:
//		void
//
ULONG C_DIALOG::Process( void )
{
	// Process the dialog box
	return WinProcessDlg( Window() );
}


//-------
// Close \
//---------------------------------------------------------------------------
//
//	Description:
//		This method is called to close the window and dispose of any system
//		resources it is using.
//
//	Parameters:
//		iFlag		- Set TRUE or FALSE to pass to the Dismiss API function.
//
//	Returns:
//		void
//
void C_DIALOG::Close( int iFlag )
{
	// Prevent class from processing further messages
	WinSetWindowULong( Window(), 0, (ULONG)0 );

	// Get rid of the dialog
	WinDismissDlg( Window(), iFlag );
}


//------------
// DialogProc \
//---------------------------------------------------------------------------
//
//	Description:
//		This method is the main window message manager for the window.  It
//		accepts a message and two optional parameters and compares the
//		supplied message to those stored in the dialog's message table.
//		If a message match is found the appropriate message handler is invoked.
//
//	Parameters:
//		lMsg		- Message passed to the dialog window
//		mp1			- Pointer to first optional parameter
//		mp2			- Pointer to second optional parameter
//
//	Returns:
//		Result of command or default handler result.
//
void *C_DIALOG::DialogProc( ULONG lMsg, void *mp1, void *mp2 )
{
	int		iCtr;
	int		iCmdCtr;
	void *(C_WINDOW::*Function)( void *, void * );


	iCtr = 0;
	while( (pxtMsgTable+iCtr)->lMsg )
	{
		// If this is a WM_COMMAND message, see if the dialog has defined
		// a command table
		if( lMsg == WM_COMMAND && pxtCommandTable )
		{
			// Scan the dialog's command table for the command
			iCmdCtr = 0;
			while( (pxtCommandTable+iCmdCtr)->lMsg )
			{
				// Did we find a command match
				if( (pxtCommandTable+iCmdCtr)->lMsg == COMMANDMSG(&lMsg)->cmd )
				{
					// Process the dialog command by calling the correct
					// processor
					Function = (pxtCommandTable+iCmdCtr)->Function;
					return (this->*Function)( mp1, mp2 );
				}

				iCmdCtr++;
			}
		}

		if( (pxtMsgTable+iCtr)->lMsg == lMsg )
		{
			Function = (pxtMsgTable+iCtr)->Function;
			return (this->*Function)( mp1, mp2 );
		}

		iCtr++;
	}

	return WinDefDlgProc( Window(), lMsg , mp1, mp2 );
}


//-------------
// StdDlgProc \
//---------------------------------------------------------------------------
//
//	Description:
//		Function acts as the interface between the PM C API interface and
//		the C++ class interface used by the PMCLASS library.  Once the instance
//		has been created, all message processing passes to the handler method
//		within the object.  If the window instance has not yet been created
//		the the PM default interface manages the message.
//
//	Parameters:
//		lMsg		- Message passed to the dialog window
//		mp1			- Pointer to first optional parameter
//		mp2			- Pointer to second optional parameter
//
//	Returns:
//		Result of command or default handler result.
//
MRESULT EXPENTRY StdDlgProc( HWND hWnd, ULONG lMsg, MPARAM mp1, MPARAM mp2 )
{
	C_DIALOG		*pxcThis;


	// If we are initializing the window
	if( lMsg == WM_INITDLG )
	{
		// Get a pointer to the instance
		pxcThis = (C_DIALOG *)mp2;

		// Save the instance pointer so the clas methods can access it
		WinSetWindowULong( hWnd, 0, (ULONG)pxcThis );
	}

	// Get a pointer to this instance
	pxcThis = (C_DIALOG *)WinQueryWindowULong( hWnd, 0 );

	// if this instance has be constructed, then let its handler method
	// look after message processing
	if( pxcThis && pxcThis->Window() )
		return pxcThis->DialogProc( lMsg, mp1, mp2 );

	// The object hasn't been created yet so we have to let PM handle
	// all of the messages
	return WinDefDlgProc( hWnd, lMsg , mp1, mp2 );
}

