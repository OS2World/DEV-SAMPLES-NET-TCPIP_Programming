//----------
// TBAR.CPP \
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
//	This file contains the methods to describe the C_TOOLBAR class.
//	C_TOOLBAR provides a base class used to dervive new application toolbars
//	which include graphical buttons and optionally can include fly-over help
//	in an external status line.
//
//---------------------------------------------------------------------------

//-------------------
// OS/2 Conditionals \
//---------------------------------------------------------------------------
#define	INCL_WIN
#define	INCL_GPI

//------------------
// Standard Headers \
//---------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <os2.h>

//-----------------
// PMCLASS Headers \
//---------------------------------------------------------------------------
#include <window.hpp>
#include <winstd.hpp>
#include <winchild.hpp>
#include <dialog.hpp>
#include <status.hpp>
#include <button.hpp>
#include <tbar.hpp>
#include <mle.hpp>
#include <log.hpp>


//--------------------------------------
// Message table for the toolbar object \
//---------------------------------------------------------------------------
DECLARE_MSG_TABLE( xtToolBarMsg )
	DECLARE_MSG( PM_CREATE,			C_TOOLBAR::MsgCreate )
	DECLARE_MSG( BM_BUTTON,			C_TOOLBAR::MsgBMButton )
	DECLARE_MSG( BM_TEXT,			C_TOOLBAR::MsgBMText )
	DECLARE_MSG( BM_BUTTON1DOWN,	C_TOOLBAR::MsgBMButton1Down )
	DECLARE_MSG( WM_MOUSEMOVE,		C_TOOLBAR::MsgMouseMove )
	DECLARE_MSG( WM_PAINT,			C_TOOLBAR::MsgPaint )
END_MSG_TABLE


//-------------
// Constructor \
//---------------------------------------------------------------------------
//
// Description:
//		This constructor creates an instance of the C_TOOLBAR class.  This
//		class will typically be abstract so this constructor will generally not
//		be called directly.
//
//	Parameters:
//		pxcParentObj		- Pointer to owner object window
//		iTBarID				- Resource ID of the toolbar window
//		iTBarHeight			- Height of the toolbar in pixels
//
C_TOOLBAR::C_TOOLBAR( C_WINDOW *pxcParentObj, int iTBarID, int iTBarHeight )
							: C_WINDOW_CHILD( pxcParentObj, xtToolBarMsg )
{
	// Save the important things
	pxcStatus = NULLHANDLE;
	iHeight = iTBarHeight;
	iButtonCount = 0;
	iLastID = 0;
	iID = iTBarID;

	ParentObject( pxcParentObj );

	// Make sure the toolbar class has been registered
	Register( "ToolBar" );

	Create( iTBarID, 0, "ToolBar", 0, 0, 0, iHeight );
}


//------------
// Destructor \
//---------------------------------------------------------------------------
// Description:
//		This destructor, resets the button count to zero
//
//	Parameters:
//		none
//
C_TOOLBAR::~C_TOOLBAR( void )
{
	// Reset the button count because all of them are now invalid
	iButtonCount = 0;
}


//-----------
// MsgCreate \
//---------------------------------------------------------------------------
// Event:		PM_CREATE
// Cause:		Issued by OS when window is created
// Description:	This method gets called when the window is initially created.
//				It initializes all of the visual aspects of the class.
void *C_TOOLBAR::MsgCreate( void *mp1, void *mp2 )
{
	WinSetPointer( HWND_DESKTOP,
					WinQuerySysPointer( HWND_DESKTOP, SPTR_ARROW, FALSE ) );

	LastID( 0 );
	strcpy( OldText(), "" );
	iText = 0;
	iMouseButton = 0;

	return FALSE;
}


//-------------
// MsgBMButton \
//---------------------------------------------------------------------------
// Event:		BM_BUTTON
// Cause:		Issued by a button when it is pressed by the user
// Description:	This method gets called when the user presses a toolbar
//				button.  mp1 holds the ID of button pressed.
void *C_TOOLBAR::MsgBMButton( void *mp1, void *mp2 )
{
	int		iButtonID;


	// Get the ID of the button that was pressed
	iButtonID = (int)mp1;
	iMouseButton = 0;

	// Send message to parent to say a button was pressed
	ParentObject()->PostMsg( WM_CONTROL, MPFROM2SHORT( iID, iButtonID ), 0 );

	return FALSE;
}


//-----------
// MsgBMText \
//---------------------------------------------------------------------------
// Event:		BM_TEXT
// Cause:		Issued by a button when the fly-over text changes
// Description:	This method gets called when the user moves the mouse over
//				a toolbar button causing the fly-over text to change.
void *C_TOOLBAR::MsgBMText( void *mp1, void *mp2 )
{
	// Display the button help text in the status window if the
	// button has changed
	if( LastID() != (int)mp1 )
	{
		if( !iText )
		{
			Status()->GetText( OldText(), 256 );
			iText = 1;
		}
		LastID( (int)mp1 );
		Status()->Text( (char *)mp2 );
	}
	return FALSE;
}


//------------------
// MsgBMButton1Down \
//---------------------------------------------------------------------------
// Event:		BM_BUTTON1DOWN
// Cause:		Issued by a button when user presses a toolbar button
// Description:	This method gets called when the user presses mouse button 1
//				while over a toolbar button
void *C_TOOLBAR::MsgBMButton1Down( void *mp1, void *mp2 )
{
	iMouseButton = 1;
	return FALSE;
}


//--------------
// MsgMouseMove \
//---------------------------------------------------------------------------
// Event:		WM_MOUSEMOVE
// Cause:		Issued by the OS if the mouse is moved over the toolbar
// Description:	This method gets called when the user moves the mouse over
//				the toolbar window.  This forces the fly-over text for the last
//				button to be trashed, restoring to the status line what was
//				there previously.
void *C_TOOLBAR::MsgMouseMove( void *mp1, void *mp2 )
{
	WinSetPointer( HWND_DESKTOP,
					WinQuerySysPointer( HWND_DESKTOP, SPTR_ARROW, FALSE ) );
	if( LastID() )
	{
		// If the mouse button is depressed, make sure we toggle
		// the state of the last button pressed
		if( iMouseButton )
		{
			ButtonState( LastID(), D_BUTTON_UP );
		}

		iMouseButton = 0;
		LastID( 0 );
		Status()->Text( OldText() );
		iText = 0;
	}

	return FALSE;
}


//----------
// MsgPaint \
//---------------------------------------------------------------------------
// Event:		WM_PAINT
// Cause:		Issued by the OS when the window needs to be redrawn
// Description:	This method gets called when the toolbar needs to be redrawn
//				for any reason.  This causes the chiseled look to be drawn around
//				the toolbar window anf forces each button to be repainted.
void *C_TOOLBAR::MsgPaint( void *mp1, void *mp2 )
{
	HPS				hps;
	POINTL			pt;
	RECTL			rc;
	SWP				swp;
	int				iCtr;


	hps = WinBeginPaint( Window(), 0L, &rc );

	// Fill the basic client area with the menu color fitrst
	WinFillRect( hps, &rc, SYSCLR_MENU );
	GpiSetColor( hps, CLR_NEUTRAL );

	// Draw the chiseled window highlights (bottom and right first)
	WinQueryWindowPos( Window(), &swp );
	pt.x = 0;
	pt.y = 0;
	GpiMove( hps, &pt );
	GpiSetColor( hps, SYSCLR_BUTTONDARK );
	pt.x = swp.cx;
	GpiLine( hps, &pt );
	pt.y = swp.cy;
	GpiLine( hps, &pt );
	GpiSetColor( hps, SYSCLR_BUTTONLIGHT );
	pt.x = 0;
	GpiLine( hps, &pt );
	pt.y = 0;
	GpiLine( hps, &pt );

	// Now draw the left and top edges
	pt.x = 1;
	pt.y = 1;
	GpiMove( hps, &pt );
	GpiSetColor( hps, SYSCLR_BUTTONDARK );
	pt.x = swp.cx - 2;
	GpiLine( hps, &pt );
	pt.y = swp.cy - 2;
	GpiLine( hps, &pt );
	GpiSetColor( hps, SYSCLR_BUTTONLIGHT );
	pt.x = 1;
	GpiLine( hps, &pt );
	pt.y = 1;
	GpiLine( hps, &pt );

	WinEndPaint( hps );

	// Invalidate all of the buttons to force them to repaint
	for( iCtr = 0; iCtr < ButtonCount() - 1; iCtr++ )
	{
		if( (xcButton + iCtr) )
			(xcButton + iCtr)->Invalidate();
	}
	return FALSE;
}


//----------
// Status() \
//---------------------------------------------------------------------------
//
//	Description:
//		This method sets the reference status line associated with the tollbar.
//		This status line is used to display fly-over help.
//
//	Parameters:
//		pxcStatusWindow		- Pointer to status object to use for fly over help
//
//	Returns:
//		void
//
void C_TOOLBAR::Status( C_STATUS *pxcStatusWindow )
{
	pxcStatus = pxcStatusWindow;
}


//---------
// Control \
//---------------------------------------------------------------------------
//
//	Description:
//		This method manages control messages for the toolbar.  It determines
//		which button ivoked the control message, then passes this as a new
//		WM_COMMAND message back to the owner of the toolbar object.
//
//	Parameters:
//		mp1					- control parameter containing button ID
//      pxtCommandLookup	- Pointer to button command table
//
//	Returns:
//		void
//
void C_TOOLBAR::Control( ULONG mp1, T_BUTTON_CMD_TABLE *pxtCommandLookup )
{
	int		iCtr;


	// Test for a parent window
	if( ParentObject()->Window() )
	{
		// Look at each button in the button table
		iCtr = 0;
		while( (pxtCommandLookup+iCtr)->iButtonId != 0 )
		{
			// If we found a match, send the associated WM_COMMAND message
			if( SHORT2FROMMP( mp1 ) == (pxtCommandLookup+iCtr)->iButtonId )
			{
				// Send the associated command ID to the owner window
				ParentObject()->SendMsg( WM_COMMAND,
						(void *)(pxtCommandLookup+iCtr)->iCommandId, 0);
			}
			iCtr++;
		}
	}
}


//--------------
// CreateButton \
//-------------------------------------------------------------------------
//
//	Description:
//		This function accepts an an array of button descriptions and will
//		allocate an instance of a button for each item in the array.
//
// Parameters:
//		pxtButtonTable		- Pointer to the button table
//
//	Returns:
//		void
//
void C_TOOLBAR::CreateButtons( T_BUTTON_TABLE *pxtButtonTable )
{
	int				iCtr;
	C_BUTTON_TBAR	*pxcTemp;


	// Determine the number of buttons in the button table
	iButtonCount = 0;
	while( (pxtButtonTable+iButtonCount)->iID != 0 )
		iButtonCount++;

	// Create an instance of each button in the button table
	for( iCtr = 0; iCtr < iButtonCount; iCtr++ )
	{
		pxcTemp = (C_BUTTON_TBAR *)(xcButton + iCtr);

		// Populate the new button
		pxcTemp->ParentObject( this );
		pxcTemp->Initialize( (pxtButtonTable+iCtr)->iID,
				(pxtButtonTable+iCtr)->iX, (pxtButtonTable+iCtr)->iY,
				(pxtButtonTable+iCtr)->iIdUp, (pxtButtonTable+iCtr)->iIdDown,
						(pxtButtonTable+iCtr)->iIdDisable,
												(pxtButtonTable+iCtr)->szText);
	}
}


//------------
// ButtonData \
//-------------------------------------------------------------------------
//
//	Description:
//		This function returns a pointer to a button structure for the specified
//		button id in the specified toolbar window.
//
// Parameters:
//		iId			- Id of button to return
//
//	Returns:
//		void
//
C_BUTTON_TBAR *C_TOOLBAR::ButtonData( int iId )
{
	int			iCtr;


	// Search each button in the table for the specified ID
	for( iCtr = 0; iCtr < iButtonCount; iCtr++ )
	{
		// If we found our ID
		if( (xcButton + iCtr)->ID() == iId )
		{
			// Return a pointer to the button's object
			return (xcButton + iCtr);
		}
	}
	return NULL;
}


//--------------
// ButtonToggle \
//-------------------------------------------------------------------------
//
//	Description:
//		This routine will toggle the specified button.  This basically involves
//		swapping the up and down graphics and repainting.
//
// Parameters:
//		iId			- Id of button to toggle
//
//	Returns:
//		void
//
void C_TOOLBAR::ButtonToggle( int iButtonId )
{
	int			iCtr;


	// Search each button in the table for the specified ID
	for( iCtr = 0; iCtr < iButtonCount; iCtr++ )
	{
		// If we found the correct button
		if( (xcButton + iCtr)->ID() == iButtonId )
		{
			// Toggle it.
			(xcButton + iCtr)->Toggle();
			return;
		}
	}
}



//-------------
// ButtonState \
//-------------------------------------------------------------------------
//
//	Description:
//		This routine will set the state of the specified button.
//
// Parameters:
//		iButtonId		- Id of button whose state will be changed
//		iState			- New button state
//
//	Returns:
//		void
//
void C_TOOLBAR::ButtonState( int iButtonId, int iState )
{
	int			iCtr;


	// Search each button in the table for the specified ID
	for( iCtr = 0; iCtr < iButtonCount; iCtr++ )
	{
		// If we found the button ID
		if( (xcButton + iCtr)->ID() == iButtonId )
		{
			// Set the button state as specified
			(xcButton + iCtr)->State( iState );
			return;
		}
	}
}



//--------------
// ButtonEnable \
//-------------------------------------------------------------------------
//
//	Description:
//		This routine will enable/disable the specified button.
//
// Parameters:
//		iButtonId		- Id of button to enable/disable
//		iValue			- TRUE or FALSE to indicate new state
//
//	Returns:
//		void
//
void C_TOOLBAR::ButtonEnable( int iButtonId, int iValue )
{
	int			iCtr;


	// Search each button in the table for the specified ID
	for( iCtr = 0; iCtr < iButtonCount; iCtr++ )
	{
		// If we found the correct button ID
		if( (xcButton + iCtr)->ID() == iButtonId )
		{
			// Enable the button
			if( !iValue )
				(xcButton + iCtr)->State( D_BUTTON_DIS );
			else
				(xcButton + iCtr)->State( iValue );
			return;
		}
	}
}




