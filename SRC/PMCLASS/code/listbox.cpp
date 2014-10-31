//-------------
// LISTBOX.CPP \
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
//	This file contains the methods to describe the C_LISTBOX class.
//	C_LISTBOX provides a class used to create and manage standard PM listboxes.
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
#include <listbox.hpp>


//-------------
// Constructor \
//---------------------------------------------------------------------------
//
// Description:
//		This constructor creates an instance of the C_STATUS class.  It is
//		used for dynamic listbox creation, and accepts a pointer to a parent
//		window object, and a listbox window identifer.  The third iMode
//		parameter is used to supply any addtion operating system specific
//		parameters for the listbox control.
//
//	Parameters:
//		pxcParentObj		- Pointer to owner object window
//		iID					- Listbox window identifier
//		iMode				- Additional OS specific listbox parameters
//
C_LISTBOX::C_LISTBOX( C_WINDOW *pxcParentObj, int iID, int iMode )
								: C_WINDOW_CHILD( pxcParentObj, 0 )
{
	ClassName( WC_LISTBOX );
	Create( iID, iMode, " ", 0, 0, 90, 30 );
}


//-------------
// Constructor \
//---------------------------------------------------------------------------
//
// Description:
//		This constructor creates an instance of the C_STATUS class.  It is
//		used for dynamic listbox creation, and accepts a pointer to a parent
//		window object, and a listbox window identifer.  This construct defaults
//		the control to be visible and force it not to adjust its position
//		automatically
//
//	Parameters:
//		pxcParentObj		- Pointer to owner object window
//		iID					- Listbox window identifier
//
C_LISTBOX::C_LISTBOX( C_WINDOW *pxcParentObj, int iID )
								: C_WINDOW_CHILD( pxcParentObj, 0 )
{
	ClassName( WC_LISTBOX );
	Create( iID, WS_VISIBLE | LS_NOADJUSTPOS, " ", 0, 0, 90, 30 );
}


//-------------
// Constructor \
//---------------------------------------------------------------------------
//
// Description:
//		This constructor creates an instance of the C_STATUS class.  It is
//		used for creation of a listbox that is already part of a dialog
//		resource.  Any modes for the listbox are supplied by the dialog
//		resource when it is loaded.
//
//
//	Parameters:
//		pxcParentObj		- Pointer to owner object window
//		iID					- Listbox window identifier
//
C_LISTBOX::C_LISTBOX( C_DIALOG *pxcParentObj, int iID )
							: C_WINDOW_CHILD( (C_WINDOW *)pxcParentObj, 0 )
{
	ClassName( WC_LISTBOX );

	// The window was created by the dialog system, so all we need to
	// do is call our parent Create().
	C_WINDOW::Create( ParentObject()->Window(),
					WinWindowFromID( ParentObject()->Window(), iID ) );
}


//--------
// Insert \
//---------------------------------------------------------------------------
//
//	Description:
//		This method is a wrapper to hide the system specific code to insert an
//		item into the listbox.
//
//	Parameters:
//		szText				- Pointer to text string to insert
//		iHow				- System specific parameter to indicate how the
//							  item will be inserted
//	Returns:
//		void
//
void C_LISTBOX::Insert( char *szText, int iHow )
{
	SendMsg( LM_INSERTITEM, MPFROMSHORT( iHow ), MPFROMP( szText ) );
}


//--------
// Delete \
//---------------------------------------------------------------------------
//
//	Description:
//		This method is a wrapper to hide the system specific code to delete an
//		item from the listbox.
//
//	Parameters:
//		iItem				- Item to remove from the list (0-n)
//
//	Returns:
//		void
//
void C_LISTBOX::Delete( int iItem )
{
	SendMsg( LM_DELETEITEM, MPFROMSHORT( iItem ), 0 );
}


//-----------
// DeleteAll \
//---------------------------------------------------------------------------
//
//	Description:
//		This method is a wrapper to hide the system specific code to delete all
//		items from the listbox.
//
//	Parameters:
//		none
//
//	Returns:
//		void
//
void C_LISTBOX::DeleteAll( void )
{
	SendMsg( LM_DELETEALL, 0, 0 );
}


//--------
// Select \
//---------------------------------------------------------------------------
//
//	Description:
//		This method is a wrapper to hide the system specific code to select an
//		item from the listbox.
//
//	Parameters:
//		iItem				- Item to select from the list (0-n)
//		bBoolean			- TRUE to select, FALSE to deselect
//
//	Returns:
//		void
//
void C_LISTBOX::Select( int iItem, BOOL bBoolean )
{
	SendMsg( LM_SELECTITEM, MPFROMSHORT( iItem ), MPFROMSHORT( bBoolean ) );
}


//-----------
// Selection \
//---------------------------------------------------------------------------
//
//	Description:
//		This method is a wrapper to hide the system specific code to query
//		the current list selection.
//
//	Parameters:
//		iFrom				- Item number to start searching from (0-n)
//
//	Returns:
//		int			- Item number select.  <0 = no selection
//
int C_LISTBOX::Selection( int iFrom )
{
	return (int)SendMsg( LM_QUERYSELECTION, MPFROMSHORT( iFrom ), 0 );
}


//-----------
// ItemCount \
//---------------------------------------------------------------------------
//
//	Description:
//		This method is a wrapper to hide the system specific code to query
//		the number of items in the listbox.
//
//	Parameters:
//		none
//
//	Returns:
//		int		- number of items in the list box.
//
int C_LISTBOX::ItemCount( void )
{
	return (int)SendMsg( LM_QUERYITEMCOUNT, 0, 0 );
}


//----------
// ItemText \
//---------------------------------------------------------------------------
//
//	Description:
//		This method is a wrapper to hide the system specific code to query
//		the text string for a specified item.
//
//	Parameters:
//		iItem				- Item to query from the list (0-n)
//		szString			- Pointer to buffer where text will be written
//		iBufferSize			- Size of the output buffer in bytes.
//
//	Returns:
//		void
//
void C_LISTBOX::ItemText( int iItem, char *szString, int iBufferSize )
{
	SendMsg( LM_QUERYITEMTEXT, MPFROM2SHORT( iItem, iBufferSize ),
												MPFROMP( szString ) );
}



