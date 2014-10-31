//--------------
// WINCHILD.CPP \
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
//	This file contains the methods to describe the C_WINDOW_CHILD abstract
//	window class.  C_WINDOW_CHILD provides an interface to all of the methods
//	used by control windows such as listboxes, containers, etc.  All PMCLASS
//	control windows are derived from this class.
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
#include <window.hpp>
#include <winstd.hpp>
#include <winchild.hpp>
#include <dialog.hpp>
#include <status.hpp>
#include <mle.hpp>
#include <log.hpp>


//-------------
// Constructor \
//---------------------------------------------------------------------------
//
// Description:
//		This is the void constructor for the C_WINDOW_CHILD class.  It does
//		nothing, but has been placed here for debugging purposes.
//
//	Parameters:
//		none
//
C_WINDOW_CHILD::C_WINDOW_CHILD( void ) : C_WINDOW()
{
}


//-------------
// Constructor \
//---------------------------------------------------------------------------
//
// Description:
//		This is the void constructor for the C_WINDOW_CHILD class.  It
//		accepts a pointer to a message table and passes this to the C_WINDOW
//		constructor which is is its parent.
//
//	Parameters:
//		pxtMsg		- Pointer to window message table
//
C_WINDOW_CHILD::C_WINDOW_CHILD( T_MSG_TABLE *pxtMsg ) : C_WINDOW( pxtMsg )
{
}


//-------------
// Constructor \
//---------------------------------------------------------------------------
//
// Description:
//		This is the void constructor for the C_WINDOW_CHILD class.  It
//		accepts a pointer to a message table and passes this to the C_WINDOW
//		constructor which is is its parent. The constructor also accepts a
//		pointer to a parent/owner object window.
//
//	Parameters:
//		pxcParentObj	- Pointer to parent window object
//		pxtMsg			- Pointer to window message table
//
C_WINDOW_CHILD::C_WINDOW_CHILD( C_WINDOW *pxcParentObj, T_MSG_TABLE *pxtMsg )
														 : C_WINDOW( pxtMsg )
{
	ParentObject( pxcParentObj );
}


//----------------
// ParentObject() \
//---------------------------------------------------------------------------
//
//	Description:
//		This method sets the parent object attribute within the class.  This
//		attribute is referenced by many of the other methods within the class.
//
//	Parameters:
//		pxcParentObj	- Pointer to parent window object
//
//	Returns:
//		void
//
void C_WINDOW_CHILD::ParentObject( C_WINDOW *pxcParentObj )
{
	pxcParent = pxcParentObj;
}


//----------------
// ParentObject() \
//---------------------------------------------------------------------------
//
//	Description:
//		This method queries the class attribute area and returns a pointer
//		to the parent object of this instance's window.
//
//	Parameters:
//		void
//
//	Returns:
//		C_WINDOW *		- returns pointer to parent window object
//
C_WINDOW *C_WINDOW_CHILD::ParentObject( void )
{
	return pxcParent;
}


//----------
// Create() \
//---------------------------------------------------------------------------
//
//	Description:
//		This method performs the actual window creation by calling the PM API
//		WinCreateWindow().  It creates a window using the parameters specified
//		by the caller.
//
//	Parameters:
//		iID			- window ID for the new window
//		iMode		- OS specific parameters for the window creation
//		szTitle		- Title string for the window
//		iX,iY		- X,Y coordinates for the new window relative to owner
//		iCX,iCY		- Width and height of the window in pels.
//
//	Returns:
//		void
//
void C_WINDOW_CHILD::Create( int iID, int iMode, char *szTitle,
								int iX, int iY, int iCX, int iCY )
{
	HWND	hWnd;


	// Create a new child window using the parameters specified
	hWnd = WinCreateWindow( ParentObject()->Window(), ClassName(), szTitle,
			iMode, iX, iY, iCX, iCY, ParentObject()->Window(),
														HWND_TOP, iID, 0, 0 );

	// Call the parent create to associate the new window with the parent
	C_WINDOW::Create( ParentObject()->Window(), hWnd );
}


//-------------------
// SetSizePosition() \
//---------------------------------------------------------------------------
//
//	Description:
//		This method accepts new X,Y and width, height and sets the size and
//		position of the window accordingly.
//
//	Parameters:
//		iX,iY		- X,Y coordinates for the window relative to owner
//		iCX,iCY		- Width and height of the window in pels.
//
//	Returns:
//		void
//
void C_WINDOW_CHILD::SetSizePosition( int iX, int iY, int iCX, int iCY )
{
	WinSetWindowPos( Window(), HWND_TOP,
			iX, iY, iCX, iCY, SWP_MOVE | SWP_SIZE | SWP_ZORDER | SWP_SHOW );
}


//---------------
// SetPosition() \
//---------------------------------------------------------------------------
//
//	Description:
//		This method accepts new X,Y and sets the position of the window
//		accordingly.
//
//	Parameters:
//		iX,iY		- X,Y coordinates for the window relative to owner
//
//	Returns:
//		void
//
void C_WINDOW_CHILD::SetPosition( int iX, int iY )
{
	WinSetWindowPos( Window(), HWND_TOP, iX, iY, 0, 0, SWP_MOVE );
}


//-----------
// SetSize() \
//---------------------------------------------------------------------------
//
//	Description:
//		This method accepts a new width, height and sets the size of the 
//		window accordingly.
//
//	Parameters:
//		iCX,iCY		- Width and height of the window in pels.
//
//	Returns:
//		void
//
void C_WINDOW_CHILD::SetSize( int iCX, int iCY )
{
	WinSetWindowPos( Window(), HWND_TOP, 0, 0, iCX, iCY, SWP_SIZE );
}

