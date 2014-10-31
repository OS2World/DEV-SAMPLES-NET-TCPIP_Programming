//---------
//EDIT.CPP \
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
//	This file contains the methods to describe the C_EDIT class.  C_EDIT
//	provides the interface to create single line edit controls and is simply
//	a C++ wrapper around the standard PM WC_ENTRYFIELD control.
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

//-----------------
// PMCLASS Headers \
//---------------------------------------------------------------------------
#include <window.hpp>
#include <winchild.hpp>
#include <dialog.hpp>
#include <edit.hpp>


//-------------
// Constructor \
//---------------------------------------------------------------------------
//
// Description:
//		This constructor creates an instance of the C_EDIT class with optional
//		programmer specified edit parameters.
//
//	Parameters:
//		pxcParentObj	- Owner object
//		iID			- Resource ID of the edit control
//		iMode			- Operating system specifc configuration parameters
//
C_EDIT::C_EDIT( C_WINDOW *pxcParentObj, int iID, int iMode )
								: C_WINDOW_CHILD( pxcParentObj, 0 )
{
	ClassName( WC_ENTRYFIELD );
	Create( iID, iMode, "", 0, 0, 90, 30 );
}


//-------------
// Constructor \
//---------------------------------------------------------------------------
//
// Description:
//		This constructor creates an instance of the C_EDIT class with
//		predefined control parameters.
//
//	Parameters:
//		pxcParentObj	- Owner object
//		iID			- Resource ID of the edit control
//
C_EDIT::C_EDIT( C_WINDOW *pxcParentObj, int iID )
								: C_WINDOW_CHILD( pxcParentObj, 0 )
{
	ClassName( WC_ENTRYFIELD );
	Create( iID, WS_VISIBLE | ES_MARGIN | ES_AUTOSCROLL, " ", 0, 0, 90, 30 );
}


//-------------
// Constructor \
//---------------------------------------------------------------------------
//
// Description:
//		This constructor creates an instance of the C_EDIT class for an
//		edit control that is part of a dialog box window.
//
//	Parameters:
//		pxcParentObj	- Owner object
//		iID			- Resource ID of the edit control
//
C_EDIT::C_EDIT( C_DIALOG *pxcParentObj, int iID )
							: C_WINDOW_CHILD( (C_WINDOW *)pxcParentObj, 0 )
{
	ClassName( WC_ENTRYFIELD );

	// The window was created by the dialog system, so all we need to
	C_WINDOW::Create( ParentObject()->Window(),
					WinWindowFromID( ParentObject()->Window(), iID ) );
}


//-------------
// TextLimit() \
//---------------------------------------------------------------------------
//
//	Description:
//		This method sets the text limit of the edit control to a specified
//		number of characters.  It simply wraps the PM API that performs the
//		same operation
//
//	Parameters:
//		iLimit				-  Character limit to set for the control
//
//	Returns:
//		void
//
void C_EDIT::TextLimit( int iLimit )
{
	// Call the API to set the character limit
	SendMsg( EM_SETTEXTLIMIT, MPFROMSHORT( iLimit ), 0 );
}




