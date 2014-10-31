//------------
// SLIDER.CPP \
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
//	This file contains the methods to describe the C_SLIDER class.
//	C_SLIDER provides an interface to all of the methods used by the CUA'91
//	slider control window.  Sliders are useful for implementing analog-like
//	controls, precent complete indicators, etc.
//
//---------------------------------------------------------------------------

//-----------------
// OS/2 Defnitions \
//---------------------------------------------------------------------------
#define INCL_DOS
#define INCL_WIN
#define INCL_GPI

//-------------------
// OS/2 Conditionals \
//---------------------------------------------------------------------------
#include <os2.h>
#include <process.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

//-----------------
// PMCLASS Headers \
//---------------------------------------------------------------------------
#include <window.hpp>
#include <winchild.hpp>
#include <dialog.hpp>
#include <slider.hpp>


//-------------
// Constructor \
//---------------------------------------------------------------------------
//
// Description:
//		This constructor creates a slider control which has a C_WINDOW
//		parent.
//
//	Parameters:
//		pxcParentObj	- Pointer to owner of the slider
//		iID				- Window ID for the slider
//		iMode			- Any additional OS/2 specific scale modifiers
//		iIncrements		- The numer of increments on the slider
//		iSpacing		- The spacing between increments
//
C_SLIDER::C_SLIDER( C_WINDOW *pxcParentObj, int iID, int iMode,
			int iIncrements, int iScale ) : C_WINDOW_CHILD( pxcParentObj, 0 )
{
	HWND	hWnd;


	// Set some default scale increments
	Scale( iIncrements, iScale );

	ClassName( WC_SLIDER );
	hWnd = WinCreateWindow( ParentObject()->Window(), WC_SLIDER, NULL,
			SLS_PRIMARYSCALE1 | WS_VISIBLE | iMode,
			0,0,22,16, ParentObject()->Window(), HWND_TOP, iID, &slData, 0 );

	// Tell the main Window interface about the window
	C_WINDOW::Create( ParentObject()->Window(), hWnd );
}


//-------------
// Constructor \
//---------------------------------------------------------------------------
//
// Description:
//		This constructor connects to a slider control which originates inside
//		a dialog resource.
//
//	Parameters:
//		pxcParentObj	- Pointer to dliag owner of the slider
//		iID				- Window ID for the slider
//
C_SLIDER::C_SLIDER( C_DIALOG *pxcParentObj, int iID )
							: C_WINDOW_CHILD( (C_WINDOW *)pxcParentObj, 0 )
{
	ClassName( WC_SLIDER );

	// The window was created by the dialog system, so all we need to
	C_WINDOW::Create( ParentObject()->Window(),
					WinWindowFromID( ParentObject()->Window(), iID ) );
}


//-------
// Scale \
//---------------------------------------------------------------------------
//
// Description:
//		This method defines the number of increments and the spacing of
//		the slider scale.
//
//	Parameters:
//		iIncrements		- The numer of increments on the slider
//		iSpacing		- The spacing between increments
//
//	Returns:
//		void
//
void C_SLIDER::Scale( int iIncrements, int iSpacing )
{
	slData.cbSize = sizeof( SLDCDATA );
	slData.usScale1Increments = iIncrements;
	slData.usScale1Spacing = iSpacing;
}


//-------
// Value \
//---------------------------------------------------------------------------
//
// Description:
//		This method sets the value contained in the slider to that specified.
//		The iValue parameter should be less than the number of increments
//		on the slider.
//
//	Parameters:
//		iValue			- New valu for the slider position
//
//	Returns:
//		void
//
void C_SLIDER::Value( int iValue )
{
	WinSendMsg( Window(), SLM_SETSLIDERINFO, MPFROM2SHORT(
			SMA_SLIDERARMPOSITION, SMA_INCREMENTVALUE ),
											MPFROMSHORT( (SHORT)iValue ) );
}

