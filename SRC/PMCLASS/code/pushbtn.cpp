//-------------
// PUSHBTN.CPP \
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
//	This file contains the methods to describe the C_PUSHBUTTON class.
//	C_PUSHBUTTON provides a class used to create and manage standard PM
//	pushbutton controls.
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
#include <pushbtn.hpp>


//-------------
// Constructor \
//---------------------------------------------------------------------------
//
// Description:
//
//	Parameters:
//		pxcParentObj		- Pointer to owner object window
//		iID					- Button window identifier
//		iMode				- Additional OS specific parameters
//		szText				- Text used for the button face
//
C_PUSHBUTTON::C_PUSHBUTTON( C_WINDOW *pxcParentObj, int iID, int iMode,
							char *szText ) : C_WINDOW_CHILD( pxcParentObj, 0 )
{
	ClassName( WC_BUTTON );
	Create( iID, WS_VISIBLE | BS_PUSHBUTTON | iMode, szText, 0, 0, 90, 30 );
}


//-------------
// Constructor \
//---------------------------------------------------------------------------
//
// Description:
//
//	Parameters:
//		pxcParentObj		- Pointer to owner object window
//		iID					- Listbox window identifier
//
C_PUSHBUTTON::C_PUSHBUTTON( C_DIALOG *pxcParentObj, int iID )
							: C_WINDOW_CHILD( (C_WINDOW *)pxcParentObj, 0 )
{
	ClassName( WC_BUTTON );

	// The window was created by the dialog system, so all we need to
	// do is call our parent Create()
	C_WINDOW::Create( ParentObject()->Window(),
					WinWindowFromID( ParentObject()->Window(), iID ) );
}

