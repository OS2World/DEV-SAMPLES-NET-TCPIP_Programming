//------------
// WINDOW.CPP \
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
//	This file contains the methods to describe the C_WINDOW abstract
//	window class.  C_WINDOW provides an interface to all of the methods
//	used by any PMCLASS window.  C_WINDOW is the base class for all windows
//	managed by this class library.
//
//---------------------------------------------------------------------------

//-------------------
// OS/2 Conditionals \
//---------------------------------------------------------------------------
#define	INCL_DOS
#define	INCL_GPI
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
#include <winstd.hpp>
#include <winchild.hpp>
#include <dialog.hpp>
#include <status.hpp>
#include <mle.hpp>


//-------------
// Constructor \
//---------------------------------------------------------------------------
//
// Description:
//		This is the void constructor for the C_WINDOW class.  It simply
//		defaults the created window parameters.
//
//	Parameters:
//		none
//
C_WINDOW::C_WINDOW( void )
{
	// Initialize the window handles
	hFrameWnd = NULLHANDLE;
	hWnd = NULLHANDLE;
	pxtMsgTable = 0;
	pxtCommandTable = 0;
	szClassName = 0;
}


//-------------
// Constructor \
//---------------------------------------------------------------------------
//
// Description:
//		This is the void constructor for the C_WINDOW class.  It defaults the
//		created window parameters and initialize the message table for the
//		window.
//
//	Parameters:
//		none
//
C_WINDOW::C_WINDOW( T_MSG_TABLE *pxtMsg )
{
	// Initialize the window handles
	hFrameWnd = NULLHANDLE;
	hWnd = NULLHANDLE;
	pxtMsgTable = 0;
	pxtCommandTable = 0;
	szClassName = 0;

	// Set up the specified message table
	MessageTable( pxtMsg );
}


//------------
// Destructor \
//---------------------------------------------------------------------------
//
//	Parameters:
//		none
//
C_WINDOW::~C_WINDOW( void )
{
	// Get rid of the window handles
	hFrameWnd = NULLHANDLE;
	hWnd = NULLHANDLE;
}


//--------------
// ParentWindow \
//---------------------------------------------------------------------------
//
// Description:
//		This method returns a value for the parent window parameter within
//		the class
//
//	Parameters:
//		none
//
//	Returns:
//		HWND	- Window handle of the parent/owner window
//
HWND C_WINDOW::ParentWindow( void )
{
	return	hFrameWnd;
}


//--------
// Window \
//---------------------------------------------------------------------------
//
// Description:
//		This method returns a handle for the PM window parameter within
//		the class
//
//	Parameters:
//		none
//
//	Returns:
//		HWND	- Window handle of the window
//
HWND C_WINDOW::Window( void )
{
	return hWnd;
}


//--------
// Window \
//---------------------------------------------------------------------------
//
// Description:
//		This method assignes a handle to the PM window parameter within
//		the class
//
//	Parameters:
//		hNewWnd		- Window handle of the window
//
//	Returns:
//		void
//
void C_WINDOW::Window( HWND hNewWnd )
{
	hWnd = hNewWnd;
}


//--------------
// ParentWindow \
//---------------------------------------------------------------------------
//
// Description:
//		This method assignes a handle to the parent window parameter within
//		the class
//
//	Parameters:
//		hNewWnd		- Window handle of the parent/owner window
//
//	Returns:
//		void
//
void C_WINDOW::ParentWindow( HWND hNewWnd )
{
	hFrameWnd = hNewWnd;
}


//---------
// SendMsg \
//---------------------------------------------------------------------------
//
// Description:
//		This method wraps the API call to send a message and its parameters
//		to the window message handler.
//
//	Parameters:
//      lMsg		- Window message to send
//		mp1,mp2		- message parameters send with the message
//
//	Returns:
//		void *		- result of the send operation
//
void *C_WINDOW::SendMsg( ULONG lMsg, void *mp1, void *mp2 )
{
	// Send a message to this window
	return (void *)WinSendMsg( hWnd, lMsg, mp1, mp2 );
}


//---------
// PostMsg \
//---------------------------------------------------------------------------
//
// Description:
//		This method wraps the API call to post a message and its parameters
//		to the window message handler.
//
//	Parameters:
//      lMsg		- Window message to post
//		mp1,mp2		- message parameters send with the message
//
//	Returns:
//		void
//
void C_WINDOW::PostMsg( ULONG lMsg, void *mp1, void *mp2 )
{
	// Post a message to this window
	WinPostMsg( hWnd, lMsg, mp1, mp2 );
}


//---------
// SetText \
//---------------------------------------------------------------------------
//
// Description:
//		This method wraps the API call to set the text contents of a window
//		to a specific string.
//
//	Parameters:
//		szString	- pointer to new text string
//
//	Returns:
//		void
//
void C_WINDOW::SetText( char *szString )
{
	// Set the window text (title)
	WinSetWindowText( hWnd, szString );
}


//---------
// GetText \
//---------------------------------------------------------------------------
//
// Description:
//		This method wraps the API call to retrieve the text contents of a
//		window and write this string to an output buffer.
//
//	Parameters:
//		szString	- pointer to a location where text string will be written
//		iBufferSize	- Size of the output buffer
//
//	Returns:
//		void
//
void C_WINDOW::GetText( char *szString, int iBufferSize )
{
	// Set the window text (title)
	WinQueryWindowText( hWnd, iBufferSize, szString );
}


//--------
// Enable \
//---------------------------------------------------------------------------
//
// Description:
//		This method wraps the API call to enable or disable the window, which
//		allows or prevents the user from invoking any action.  This is
//		particularly useful for enabling of disabling child controls.
//
//	Parameters:
//		bState		- TRUE or FALSE to enable or disable the window
//
//	Returns:
//		void
//
void C_WINDOW::Enable( BOOL bState )
{
	// Enable of disable the window
	WinEnableWindow( hWnd, bState );
}


//------
// Show \
//---------------------------------------------------------------------------
//
// Description:
//		This method wraps the API call to display a window at the top of the
//		window stack.  If the windo is currently hidden or minimized it is
//		restored.
//
//	Parameters:
//		void
//
//	Returns:
//		void
//
void C_WINDOW::Show( void )
{
	WinSetWindowPos( hFrameWnd, HWND_TOP, 0, 0, 0, 0,
					SWP_SHOW | SWP_ZORDER | SWP_ACTIVATE | SWP_RESTORE );
}


//------
// Hide \
//---------------------------------------------------------------------------
//
// Description:
//		This method wraps the API call to hide a window.  The window is
//		removed from the screen but is not destroyed.
//
//	Parameters:
//		void
//
//	Returns:
//		void
//
void C_WINDOW::Hide( void )
{
	WinShowWindow( hFrameWnd, FALSE );
}


//------
// Show \
//---------------------------------------------------------------------------
//
// Description:
//		This method wraps the API call to update a window display.
//
//	Parameters:
//		void
//
//	Returns:
//		void
//
void C_WINDOW::Update( void )
{
	// Update the window to show any changes
	WinUpdateWindow( hFrameWnd );
}


//-----------
// ClassName \
//---------------------------------------------------------------------------
//
// Description:
//		This method sets the internal class name attribute within C_WINDOW.
//		This attribute is used to register the class with the operating system.
//
//	Parameters:
//		szClass		- pointer to name for this window class
//
//	Returns:
//		void
//
void C_WINDOW::ClassName( char *szClass )
{
	szClassName = szClass;
}


//-----------
// ClassName \
//---------------------------------------------------------------------------
//
// Description:
//		This method returns the internal class name attribute within C_WINDOW.
//		This attribute is used to register the class with the operating system.
//
//	Parameters:
//		void
//
//	Returns:
//		char *		- pointer to the name of this window class
//
char *C_WINDOW::ClassName( void )
{
	return szClassName;
}


//-------------------
// GetSizePosition() \
//---------------------------------------------------------------------------
//
//	Description:
//		This method retreives the current window size and position reltative
//		to its owner.
//
//	Parameters:
//		*piX, *piY		-  Pointers to a data area to get the X,Y coordinates
//		*piCX,*piCY		-  Pointers to a data area to get the window dimensions
//
//	Returns:
//		void
//
void C_WINDOW::GetSizePosition( int *piX, int *piY, int *piCX, int *piCY )
{
	SWP		swp;

	WinQueryWindowPos( hWnd, &swp );

	*piX = swp.x;
	*piY = swp.y;
	*piCX = swp.cx;
	*piCY = swp.cy;
}


//-----------
// GetSize() \
//---------------------------------------------------------------------------
//
//	Description:
//		This method retreives the current window size.
//
//	Parameters:
//		*piW,*piL		-  Pointers to a data area to get the window dimensions
//
//	Returns:
//		void
//
void C_WINDOW::GetSize( int *piCX, int *piCY )
{
	SWP		swp;

	WinQueryWindowPos( hWnd, &swp );

	*piCX = swp.cx;
	*piCY = swp.cy;
}


//---------------
// GetPosition() \
//---------------------------------------------------------------------------
//
//	Description:
//		This method retreives the current window position reltative
//		to its owner.
//
//	Parameters:
//		*piX, *piY		-  Pointers to a data area to get the X,Y coordinates
//		*piW,*piL		-  Pointers to a data area to get the window dimensions
//
//	Returns:
//		void
//
void C_WINDOW::GetPosition( int *piX, int *piY )
{
	SWP		swp;

	WinQueryWindowPos( hWnd, &swp );

	*piX = swp.x;
	*piY = swp.y;
}


//----------------------
// SetForegroundColor() \
//---------------------------------------------------------------------------
//
//	Description:
//		This method sets the foreground (text) color of the window using
//		the specified RGB color parameters.
//
//	Parameters:
//		byRed		- Red level for the window
//		byGreen		- Green level for the window
//		byBlue		- Blue level for the window
//
//	Returns:
//		void
//
void C_WINDOW::SetForegroundColor( BYTE byRed, BYTE byGreen, BYTE byBlue )
{
	RGB2	rgb;


	// Create an RGB value
	rgb.bRed = byRed;
	rgb.bGreen = byGreen;
	rgb.bBlue = byBlue;
	rgb.fcOptions = 0;

	// Set the foreground color
	WinSetPresParam( Window(), PP_FOREGROUNDCOLOR, sizeof(RGB2), &rgb );

	// Force the window to update
	Invalidate();
	Update();
}


//----------------------
// SetBackgroundColor() \
//---------------------------------------------------------------------------
//
//	Description:
//		This method sets the background color of the window using
//		the specified RGB color parameters.
//
//	Parameters:
//		byRed		- Red level for the window
//		byGreen		- Green level for the window
//		byBlue		- Blue level for the window
//
//	Returns:
//		void
//
void C_WINDOW::SetBackgroundColor( BYTE byRed, BYTE byGreen, BYTE byBlue )
{
	RGB2	rgb;


	// Create an RGB value
	rgb.bRed = byRed;
	rgb.bGreen = byGreen;
	rgb.bBlue = byBlue;
	rgb.fcOptions = 0;

	// Change the background color
	WinSetPresParam( Window(), PP_BACKGROUNDCOLOR, sizeof(RGB2), &rgb );

	// Force the window to update
	Invalidate();
	Update();
}


//-----------
// SetFont() \
//---------------------------------------------------------------------------
//
//	Description:
//		This method  set the font of the window to the value specified.
//
//	Parameters:
//		szFont		- New font
//		iSize		- Font size
//
//	Returns:
//		void
//
void C_WINDOW::SetFont( char *szFont, int iSize )
{
	char	szString[256];

	// Create an OS/2 compatable font string
	sprintf( szString, "%d.%s", iSize, szFont );

	// Set the new font string
	WinSetPresParam( Window(), PP_FONTNAMESIZE, 256, szString );
}


//----------------------
// GetForegroundColor() \
//---------------------------------------------------------------------------
//
//	Description:
//		This method gets the foreground (text) color of the window returning
//		it as separated RGB values.
//
//	Parameters:
//		pbyRed		- pointer to output location for Red level for the window
//		pbyGreen	- pointer to output location for Green level for the window
//		pbyBlue		- pointer to output location for Blue level for the window
//
//	Returns:
//		void
//
void C_WINDOW::GetForegroundColor( BYTE *pbyRed, BYTE *pbyGreen, BYTE *pbyBlue )
{
	RGB2	rgb;

	// Get the foreground color
	WinQueryPresParam( Window(), PP_FOREGROUNDCOLOR, 0, NULL, sizeof(RGB2),
													&rgb, QPF_NOINHERIT );

	// Separate the RGB values
	*pbyRed = rgb.bRed;
	*pbyGreen = rgb.bGreen;
	*pbyBlue = rgb.bBlue;
}


//----------------------
// GetForegroundColor() \
//---------------------------------------------------------------------------
//
//	Description:
//		This method gets the background color of the window returning
//		it as separated RGB values.
//
//	Parameters:
//		pbyRed		- pointer to output location for Red level for the window
//		pbyGreen	- pointer to output location for Green level for the window
//		pbyBlue		- pointer to output location for Blue level for the window
//
//	Returns:
//		void
//
void C_WINDOW::GetBackgroundColor( BYTE *pbyRed, BYTE *pbyGreen, BYTE *pbyBlue )
{
	RGB2	rgb;


	// Get the background color
	WinQueryPresParam( Window(), PP_BACKGROUNDCOLOR, 0, NULL, sizeof(RGB2),
													&rgb, QPF_NOINHERIT );

	// Separate the RGB value
	*pbyRed = rgb.bRed;
	*pbyGreen = rgb.bGreen;
	*pbyBlue = rgb.bBlue;
}


//-----------
// GetFont() \
//---------------------------------------------------------------------------
//
//	Description:
//		This method gets the current font of the window, returning as a string
//		in the format size.font.
//
//	Parameters:
//		szFont		- pointer to current window font
//
//	Returns:
//		void
//
void C_WINDOW::GetFont( char *szFont )
{
	char	szString[256];

	WinQueryPresParam( Window(), PP_FONTNAMESIZE, 0, NULL, 256, szString,
															QPF_NOINHERIT );
	strcpy( szFont, szString );
}


//--------------
// Invalidate() \
//---------------------------------------------------------------------------
//
//	Description:
//		This method wraps the WinInvalidateRect() API function used to
//		force a redraw of the window.
//
//	Parameters:
//		void
//
//	Returns:
//		void
//
void C_WINDOW::Invalidate( void )
{
	// Force the window to be redrawn
	WinInvalidateRect( hWnd, NULL, TRUE );
}


//---------
// Focus() \
//---------------------------------------------------------------------------
//
//	Description:
//		This method wraps the WinSetFocus() API function and is used to
//		give the control of the mouse and keyboard to the window.
//
//	Parameters:
//		void
//
//	Returns:
//		void
//
void C_WINDOW::Focus( void )
{
	WinSetFocus( HWND_DESKTOP, hWnd );
}


//------------
// Register() \
//---------------------------------------------------------------------------
//
//	Description:
//		This method registers a window class of the specified name.  This
//		must be performed before the window can be created.
//
//	Parameters:
//		szClass		- Name of the class being registered
//
//	Returns:
//		BOOL		- Error result of the registration
//
BOOL C_WINDOW::Register( char *szClass )
{
	ClassName( szClass );

	return WinRegisterClass( 0, szClassName, (PFNWP)StdWndProc,
										CS_SIZEREDRAW, sizeof(C_WINDOW *) );
}


//----------
// Create() \
//---------------------------------------------------------------------------
//
//	Description:
//		This method accepts a parent/owner window and a window handle
//		and creates an association between the two.  If a message table
//		was specified when the instance was created, a PM_CREATE message is
//		is generated.
//
//	Parameters:
//		hFrameWnd		- Owner window handle
//		hWnd			- Window handle of this instance
//
//	Returns:
//		void
//
void C_WINDOW::Create( HWND hFrameWnd, HWND hWnd )
{
	// Make sure our parent knows about the new window handle
	ParentWindow( hFrameWnd );
	Window( hWnd );

	// Tell the Window procedure about the this pointer
	if( pxtMsgTable )
		SendMsg( PM_CREATE, MPFROMP( this ), 0 );
}


//-----------
// Destroy() \
//---------------------------------------------------------------------------
//
//	Description:
//		This method can be called to destroy a window.  It wraps the
//		WinDestroyWindow() API to accomplish this.
//
//	Parameters:
//		void
//
//	Returns:
//		void
//
void C_WINDOW::Destroy( void )
{
	// Destroy the frame window for this instance
	WinDestroyWindow( WinQueryWindow( hWnd, QW_PARENT ) );
}


//----------------
// MessageTable() \
//---------------------------------------------------------------------------
//
//	Description:
//		This method assigns a vlue to the message table class attribute
//
//	Parameters:
//		void
//
//	Returns:
//		void
//
void C_WINDOW::MessageTable( T_MSG_TABLE *pxtMsg )
{
	pxtMsgTable = pxtMsg;
}


//----------------
// CommandTable() \
//---------------------------------------------------------------------------
//
//	Description:
//		This method assigns a vlue to the command table class attribute
//
//	Parameters:
//		void
//
//	Returns:
//		void
//
void C_WINDOW::CommandTable( T_MSG_TABLE *pxtCommands )
{
	pxtCommandTable = pxtCommands;
}


//------------
// WindowProc \
//---------------------------------------------------------------------------
//
//	Description:
//		This method is the message manager for the window.  It accepts a
//		message and two optional parameters and compares the supplied
//		message to those stored in the window's message table.
//		If a message match is found the appropriate message handler is invoked.
//
//	Parameters:
//		lMsg		- Message passed to the disalog window
//		mp1			- Pointer to first optional parameter
//		mp2			- Pointer to second optional parameter
//
//	Returns:
//		Result of command or default handler result.
//
void *C_WINDOW::WindowProc( ULONG lMsg, void *mp1, void *mp2 )
{
	int		iCtr;
	int		iCmdCtr;
	void *(C_WINDOW::*Function)( void *, void * );


	iCtr = 0;
	while( (pxtMsgTable+iCtr)->lMsg )
	{
		// If this is a WM_COMMAND message, see if the window has defined
		// a command table
		if( lMsg == WM_COMMAND && pxtCommandTable )
		{
			// Scan the window's command table for the command
			iCmdCtr = 0;
			while( (pxtCommandTable+iCmdCtr)->lMsg )
			{
				// Did we find a command match
				if( (pxtCommandTable+iCmdCtr)->lMsg == COMMANDMSG(&lMsg)->cmd )
				{
					// Process the window command by calling the correct
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

	return WinDefWindowProc( Window(), lMsg, mp1, mp2 );
}


//-------------
// StdWndProc \
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
//		lMsg		- Message passed to the window
//		mp1			- Pointer to first optional parameter
//		mp2			- Pointer to second optional parameter
//
//	Returns:
//		Result of command or default handler result.
//
MRESULT EXPENTRY StdWndProc( HWND hWnd, ULONG lMsg, MPARAM mp1, MPARAM mp2 )
{
	C_WINDOW	*pxcThis;


	// Look for PM_CREATE messages
	if( lMsg == PM_CREATE )
	{
		// Set the user window word to an instnace of this
		WinSetWindowULong( hWnd, 0, (ULONG)mp1 );
	}

	// Look for WM_DESTROY
	if( lMsg == WM_DESTROY )
	{
		// Remove the pointer to this from the window word
		WinSetWindowULong( hWnd, 0, (ULONG)0 );
	}

	// Get a pointer to the current window instance
	pxcThis = (C_WINDOW *)WinQueryWindowULong( hWnd, 0 );

	// If the instance is defined and it has a window, pass the message
	// to the objects window message handler
	if( pxcThis && pxcThis->Window() )
	{
		return pxcThis->WindowProc( lMsg, mp1, mp2 );
	}

	// No instance so default the message
	return WinDefWindowProc( hWnd, lMsg, mp1, mp2 );
}



