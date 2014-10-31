//----------
// MENU.CPP \
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
//	This file contains the methods to describe the C_MENU class.
//	C_MENU provides an interface to all of the methods used by any
//	PMCLASS application menus.
//
//---------------------------------------------------------------------------

//-----------------
// OS/2 Defnitions \
//---------------------------------------------------------------------------
#define	INCL_DOS
#define	INCL_WIN

//-------------------
// OS/2 Conditionals \
//---------------------------------------------------------------------------
#include <os2.h>
#include <stdio.h>
#include <string.h>

//-----------------
// PMCLASS Headers \
//---------------------------------------------------------------------------
#include <window.hpp>
#include <winchild.hpp>
#include <menu.hpp>


//-------------
// Constructor \
//---------------------------------------------------------------------------
//
// Description:
//		This constructor retrieves a window handle for the owner window's menu.
//
//	Parameters:
//		pxcParentObj	- Pointer to owner of the menu.
//
C_MENU::C_MENU( C_WINDOW *pxcParentObj ) : C_WINDOW_CHILD( pxcParentObj, 0 )
{
	HWND	hWnd;


	// This window is of the menu class
	ClassName( WC_MENU );

	// The window was created by the parent window, so all we need to
	// do is find out where it is
	hWnd = WinWindowFromID( ParentObject()->ParentWindow(), FID_MENU );
	C_WINDOW::Create( ParentObject()->ParentWindow(), hWnd );
}


//------------
// EnableItem \
//---------------------------------------------------------------------------
//
// Description:
//		This method enables the menu item with the specified identifier
//
//	Parameters:
//		iIDItem		- ID of the menu item to enable
//
//	Returns:
//		void
//
void C_MENU::EnableItem( int iIDItem )
{
	// Call the API to enable the menu item
	WinEnableMenuItem( Window(), iIDItem, TRUE );
}


//-------------
// DisableItem \
//---------------------------------------------------------------------------
//
// Description:
//		This method disables the menu item with the specified identifier
//
//	Parameters:
//		iIDItem		- ID of the menu item to disable
//
//	Returns:
//		void
//
void C_MENU::DisableItem( int iIDItem )
{
	// Call the API to disable the menu item
	WinEnableMenuItem( Window(), iIDItem, FALSE );
}


//-------------
// SetItemText \
//---------------------------------------------------------------------------
//
// Description:
//		This method changes the menu item text for the specified menu identifer
//
//	Parameters:
//		iIDItem		- ID of the menu item to set
//		szText		- Pointer to new menu item text
//
//	Returns:
//		void
//
void C_MENU::SetItemText( int iIDItem, char *szText )
{
	// Call the API to set the menu item text
	WinSetMenuItemText( Window(), iIDItem, szText );
}


//-------------
// GetItemText \
//---------------------------------------------------------------------------
//
// Description:
//		This method retrieves the menu item text for the specified menu
//		identifer.
//
//	Parameters:
//		iIDItem		- ID of the menu item to get
//		szText		- Pointer to menu item text
//		iSize		- Size of the szText output buffer
//
//	Returns:
//		void
//
void C_MENU::GetItemText( int iIDItem, char *szText, int iSize )
{
	// Call the API to query the menu item text
	SendMsg( MM_QUERYITEMTEXT, MPFROM2SHORT( iIDItem, iSize ), szText  );
}

