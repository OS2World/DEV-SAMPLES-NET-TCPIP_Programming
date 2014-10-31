//-----------
//BUTTON.CPP \
//---------------------------------------------------------------------------
//
//      All information contained in this file is Copyright (c) 1994-1995 by
//      NeoLogic Inc.  All Rights Reserved
//
//      This source code shall not be implemented, duplicated or used as the basis
//      for any product without prior written consent by NeoLogic Inc.
//
// Description:
//
//      This file contains the methods to describe the C_BUTTON_TBAR class.  
//	C_BUTTON_TBAR provides the interface to create graphical buttons which
//	will be used mainly in conjunction with the C_TOOLBAR class.
//
//---------------------------------------------------------------------------

//-------------------
// OS/2 Conditionals \
//---------------------------------------------------------------------------
#define INCL_WIN
#define INCL_GPI

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
#include <mle.hpp>
#include <log.hpp>
#include <button.hpp>

//----------------------------------
// Message table for button objects \
//---------------------------------------------------------------------------
DECLARE_MSG_TABLE( xtButtonMsg )
		DECLARE_MSG( WM_MOUSEMOVE,		C_BUTTON_TBAR::MsgMouseMove )
		DECLARE_MSG( WM_PAINT,			C_BUTTON_TBAR::MsgPaint )
		DECLARE_MSG( WM_BUTTON1DOWN,	C_BUTTON_TBAR::MsgButton1Down )
		DECLARE_MSG( WM_BUTTON1UP,		C_BUTTON_TBAR::MsgButton1Up )
END_MSG_TABLE


//-------------
// Constructor \
//---------------------------------------------------------------------------
//
// Description:
//		This constructor creates an instance of the C_BUTTON class.
//
//	Parameters:
//		none
//
C_BUTTON_TBAR::C_BUTTON_TBAR( void ) : C_WINDOW_CHILD( xtButtonMsg )
{
}


//-------------
// Constructor \
//---------------------------------------------------------------------------
//
//	Description:
//		This constructor creates an instance of the C_BUTTON_TBAR class.
//
//	Parameters:
//		pxcParentObj	- Pointer to owner object
//
C_BUTTON_TBAR::C_BUTTON_TBAR( C_WINDOW *pxcParentObj )
						: C_WINDOW_CHILD( pxcParentObj, xtButtonMsg )
{
}


//--------------
// Initialize() \
//---------------------------------------------------------------------------
//
//	Description:
//		This method sets up the initial default values for the button instance.
//		Then it registers and creates a window for the button object, setting
//		its initial state to the UP position.
//
//	Parameters:
//		iButtonID		- Button ID for the button
//		iXPos,iYPos		- The initial X,Y position relative to the owner
//		iUp				- Resource ID of the Up position ICON
//		iDn				- Resource ID of the Down position ICON
//		iDis			- Resource ID of the Disable position ICON
//		szText			- Pointer to the fly over help text string
//
//	Returns:
//		void
//
void C_BUTTON_TBAR::Initialize( int iButtonID,
				int iXPos, int iYPos, int iUp, int iDn, int iDis, char *szText )
{
	// Save the important things
	iID = iButtonID;
	iX = iXPos;
	iY = iYPos;
	iIdUp = iUp;
	iIdDn = iDn;
	iIdDisable = iDis;
	iState = D_BUTTON_UP;
	szButtonText = NULL;
	if( strlen( szText ) )
		szButtonText = szText;

	// Make sure the toolbar class has been registered
	Register( "TBarButton" );

	Create( iID, WS_VISIBLE, "TBarButton", iX,  iY, 32, 32 );

	// Set the initial button state
	State( D_BUTTON_UP );
}


//--------------
// MsgMouseMove \
//---------------------------------------------------------------------------
// Event:	WM_MOUSEMOVE
// Cause:	Issued by the OS if the mouse is moved over the button
// Description:
//		This method gets called when the user moves the mouse over
//		the button regoin.  This forces the fly-over text to be set
//		for the button by issuing a BM_TEXT message to the owner.
//
void *C_BUTTON_TBAR::MsgMouseMove( void *mp1, void *mp2 )
{
	if( strlen( Text() ) > 2 )
	{
		// If the mouse is moving over the button, tell the parent to
		// display this buttons helper text in the status window
		ParentObject()->SendMsg( BM_TEXT, (void *)ID(), (void *)Text() );
	}

	return FALSE;
}


//----------
// MsgPaint \
//---------------------------------------------------------------------------
// Event:	WM_PAINT
// Cause:	Issued by the OS when the button needs to be redrawn
// Description:
//			This method gets called when the button needs to be redrawn
//			for any reason.
void *C_BUTTON_TBAR::MsgPaint( void *mp1, void *mp2 )
{
	HPS			hps;
	RECTL		rc;
	HPOINTER	hPointer;


	hps = WinBeginPaint( Window(), 0L, &rc );

	// Draw the correct button face
	switch( State() )
	{
		case D_BUTTON_UP:
			hPointer = WinLoadPointer( HWND_DESKTOP, 0, Up() );
			break;
		case D_BUTTON_DN:
			hPointer = WinLoadPointer( HWND_DESKTOP, 0, Down() );
			break;

		default:
			hPointer = WinLoadPointer( HWND_DESKTOP, 0, Disable());
			break;
	}

	WinDrawPointer( hps, 0, 0, hPointer, DP_NORMAL );
	WinDestroyPointer( hPointer );

	WinEndPaint( hps );

	return FALSE;
}


//------------------
// MsgBMButton1Down \
//---------------------------------------------------------------------------
// Event:		WM_BUTTON1DOWN
// Cause:		Issued by the OS when user presses the first mouse button
// Description:	This method gets called when the user presses mouse button 1
//				while over button.  It toggles the state of the button and
//				sends a BM_BUTTON1DOWN message to the owner.
void *C_BUTTON_TBAR::MsgButton1Down( void *mp1, void *mp2 )
{
	// Set the correct state for the button
	if( State() == D_BUTTON_UP && strlen( Text() ) > 2 )
	{
		Toggle();

		ParentObject()->PostMsg( BM_BUTTON1DOWN, 0, 0 );
	}

	return FALSE;
}


//----------------
// MsgBMButton1Up \
//---------------------------------------------------------------------------
// Event:		WM_BUTTON1DOWN
// Cause:		Issued by the OS when user presses the first mouse button
// Description:	This method gets called when the user presses mouse button 1
//				while over button.  It toggles the state of the button and
//				sends a BM_BUTTON1DOWN message to the owner.
void *C_BUTTON_TBAR::MsgButton1Up( void *mp1, void *mp2 )
{
	if( State() == D_BUTTON_DN  && strlen( Text() ) > 2 )
	{
		Toggle();

		// Tell the parent that this button has changed states
		ParentObject()->PostMsg( BM_BUTTON, (void *)ID(),0);

		// Force the button to be repainted
		MsgPaint( 0, 0 );
	}

	return FALSE;
}

//-------
// State \
//---------------------------------------------------------------------------
//	Description:
//		This method sets the state of the button to one of D_BUTTON_UP,
//		D_BUTTON_DN or D_BUTTON_DIS.
//
//	 Parameters:
//		iNewState	- The new state of the button
//
//	Returns:
//		none
//
void C_BUTTON_TBAR::State( int iNewState )
{
	// Change the state and force a redraw
	iState = iNewState;
	Invalidate();
}


//--------
// Toggle \
//---------------------------------------------------------------------------
//	Description:
//		This method toggles the state of the button from up to down or vice
//		versa.
//
//	Parameters:
//		none
//
//	Returns:
//		none
//
void C_BUTTON_TBAR::Toggle( void )
{
	// If the button is up
	if( State() == D_BUTTON_UP )
	{
		// Set the button in the down position
		State( D_BUTTON_DN );
	}
	else
	{
		// Otherwise, if the button is down
		if( State() == D_BUTTON_DN )
		{
			// Set the button in the up position
			State( D_BUTTON_UP );
		}
	}
}


