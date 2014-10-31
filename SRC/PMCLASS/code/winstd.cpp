//------------
// WINSTD.CPP \
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
//	This file contains the methods to describe the C_WINDOW_STD abstract
//	window class.  C_WINDOW_STD provides an interface to all of the methods
//	used by main application windows.  All PMCLASS applications are derived
//	from this class.
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



//-------------
// Constructor \
//---------------------------------------------------------------------------
//
// Description:
//		This is the void constructor for the C_WINDOW_STD class.  It defaults
//		the created window style word to FCF_SHELLPOSITION.
//
//	Parameters:
//		none
//
C_WINDOW_STD::C_WINDOW_STD( void ) : C_WINDOW()
{
	lFrameFlags = FCF_SHELLPOSITION;
}


//-------------
// Constructor \
//---------------------------------------------------------------------------
//
// Description:
//		This constructor for the C_WINDOW_STD class accepts a pointer to a
//		message table and sets up the message handler for the instance.  It
//		defaults the created window style word to 0.
//
//	Parameters:
//		pxtMsg		- Pointer to window message table
//
C_WINDOW_STD::C_WINDOW_STD( T_MSG_TABLE *pxtMsg ) : C_WINDOW( pxtMsg )
{
	lFrameFlags = 0;
}


//----------------
// WCF_Standard() \
//---------------------------------------------------------------------------
//
//	Description:
//		This method adds the standard window style to the style word.
//
//	Parameters:
//		void
//
//	Returns:
//		void
//
void C_WINDOW_STD::WCF_Standard( void )
{
	lFrameFlags |= FCF_STANDARD;
}


//---------------
// WCF_SysMenu() \
//---------------------------------------------------------------------------
//
//	Description:
//		This method adds the system menu window style to the style word.
//
//	Parameters:
//		void
//
//	Returns:
//		void
//
void C_WINDOW_STD::WCF_SysMenu( void )
{
	lFrameFlags |= FCF_SYSMENU;
}


//------------
// WCF_Icon() \
//---------------------------------------------------------------------------
//
//	Description:
//		This method adds the program icon style to the style word.  PM assumes
//		that there wil be an icon resource with the same id as the window
//		or window creation will fail.
//
//	Parameters:
//		void
//
//	Returns:
//		void
//
void C_WINDOW_STD::WCF_Icon( void )
{
	lFrameFlags |= FCF_ICON;
}


//------------
// WCF_Menu() \
//---------------------------------------------------------------------------
//
//	Description:
//		This method adds the program menu style to the style word.  The
//		Presentation Manager window handler assumes that there will be
//		menu and accelerator resources with the same id number as the window
//		or the window creation will fail.
//
//	Parameters:
//		void
//
//	Returns:
//		void
//
void C_WINDOW_STD::WCF_Menu( void )
{
	lFrameFlags |= (FCF_MENU | FCF_ACCELTABLE);
}


//-----------------
// WCF_MinButton() \
//---------------------------------------------------------------------------
//
//	Description:
//		This method adds a minimize button to the window.
//
//	Parameters:
//		void
//
//	Returns:
//		void
//
void C_WINDOW_STD::WCF_MinButton( void )
{
	lFrameFlags |= FCF_MINBUTTON;
}


//-----------------
// WCF_MaxButton() \
//---------------------------------------------------------------------------
//
//	Description:
//		This method adds a maximize button to the window.
//
//	Parameters:
//		void
//
//	Returns:
//		void
//
void C_WINDOW_STD::WCF_MaxButton( void )
{
	lFrameFlags |= FCF_MAXBUTTON;
}


//----------------
// WCF_TitleBar() \
//---------------------------------------------------------------------------
//
//	Description:
//		This method adds a title bar to the window.
//
//	Parameters:
//		void
//
//	Returns:
//		void
//
void C_WINDOW_STD::WCF_TitleBar( void )
{
	lFrameFlags |= FCF_TITLEBAR;
}


//--------------
// WCF_Border() \
//---------------------------------------------------------------------------
//
//	Description:
//		This method adds a thin-line border to the window.
//
//	Parameters:
//		void
//
//	Returns:
//		void
//
void C_WINDOW_STD::WCF_Border( void )
{
	lFrameFlags |= FCF_BORDER;
}


//--------------------
// WCF_DialogBorder() \
//---------------------------------------------------------------------------
//
//	Description:
//		This method adds a dialog border to the window.
//
//	Parameters:
//		void
//
//	Returns:
//		void
//
void C_WINDOW_STD::WCF_DialogBorder( void )
{
	lFrameFlags |= FCF_DLGBORDER;
}


//--------------------
// WCF_SizingBorder() \
//---------------------------------------------------------------------------
//
//	Description:
//		This method adds a sizable border to the window.
//
//	Parameters:
//		void
//
//	Returns:
//		void
//
void C_WINDOW_STD::WCF_SizingBorder( void )
{
	lFrameFlags |= FCF_SIZEBORDER;
}


//----------------
// WCF_TaskList() \
//---------------------------------------------------------------------------
//
//	Description:
//		This method adds the window to the WPS task list
//
//	Parameters:
//		void
//
//	Returns:
//		void
//
void C_WINDOW_STD::WCF_TaskList( void )
{
	lFrameFlags |= FCF_TASKLIST;
}


//---------------------
// WCF_ShellPosition() \
//---------------------------------------------------------------------------
//
//	Description:
//		This method tells PM to size the window automatically when it is
//		created
//
//	Parameters:
//		void
//
//	Returns:
//		void
//
void C_WINDOW_STD::WCF_ShellPosition( void )
{
	lFrameFlags |= FCF_SHELLPOSITION;
}


//-------------------
// GetSizePosition() \
//---------------------------------------------------------------------------
//
//	Description:
//		This method retreives the current window size and position reltative
//		to the WPS desktop.
//
//	Parameters:
//		*piX, *piY		-  Pointers to a data area to get the X,Y coordinates
//		*piW,*piL		-  Pointers to a data area to get the window dimensions
//
//	Returns:
//		void
//
void C_WINDOW_STD::GetSizePosition( int *piX, int *piY, int *piW, int *piL )
{
	SWP		swp;


	// Query PM to get the size and position of the window
	WinQueryWindowPos( WinQueryWindow( Window(), QW_PARENT ), &swp );

	// Format the size and position into a form we can use
	*piX = swp.x;
	*piY = swp.y;
	*piW = swp.cx;
	*piL = swp.cy;
}


//---------------
// GetPosition() \
//---------------------------------------------------------------------------
//
//	Description:
//		This method retreives the current window size
//
//	Parameters:
//		*piW,*piL		-  Pointers to a data area to get the window dimensions
//
//	Returns:
//		void
//
void C_WINDOW_STD::GetSize( int *piW, int *piL )
{
	SWP		swp;


	// Query PM to get the size and position of the window
	WinQueryWindowPos( WinQueryWindow( Window(), QW_PARENT ), &swp );

	// Format the size and position into a form we can use
	*piW = swp.cx;
	*piL = swp.cy;
}


//---------------
// GetPosition() \
//---------------------------------------------------------------------------
//
//	Description:
//		This method retreives the current window position reltative to the WPS
//		desktop.
//
//	Parameters:
//		*piX, *piY		-  Pointers to a data area to get the X,Y coordinates
//
//	Returns:
//		void
//
void C_WINDOW_STD::GetPosition( int *piX, int *piY )
{
	SWP		swp;


	// Query PM to get the size and position of the window
	WinQueryWindowPos( WinQueryWindow( Window(), QW_PARENT ), &swp );

	// Format the size and position into a form we can use
	*piX = swp.x;
	*piY = swp.y;
}


//-------------------
// SetSizePosition() \
//---------------------------------------------------------------------------
//
//	Description:
//		This method sets the current window size and position reltative
//		to the WPS desktop.
//
//	Parameters:
//		iX, iY		-  The new X,Y coordinates of the window
//		iCX,iCY		-  The new dimensions of the window
//
//	Returns:
//		void
//
void C_WINDOW_STD::SetSizePosition( int iX, int iY, int iCX, int iCY )
{
	// Set the window size and position
	WinSetWindowPos( WinQueryWindow( Window(), QW_PARENT ), HWND_TOP,
			iX, iY, iCX, iCY, SWP_MOVE | SWP_SIZE | SWP_ZORDER | SWP_SHOW );
}


//---------------
// SetPosition() \
//---------------------------------------------------------------------------
//
//	Description:
//		This method sets the current window position reltative to the WPS
//		desktop.
//
//	Parameters:
//		iX, iY		-  The new X,Y coordinates of the window
//
//	Returns:
//		void
//
void C_WINDOW_STD::SetPosition( int iX, int iY )
{
	// Set the window position
	WinSetWindowPos( WinQueryWindow( Window(), QW_PARENT ),
										HWND_TOP, iX, iY, 0, 0, SWP_MOVE );
}


//-----------
// SetSize() \
//---------------------------------------------------------------------------
//
//	Description:
//		This method sets the current size of the window in pixels.
//
//	Parameters:
//		iCX,iCY		-  The new dimensions of the window
//
//	Returns:
//		void
//
void C_WINDOW_STD::SetSize( int iCX, int iCY )
{
	// Set the window size
	WinSetWindowPos( WinQueryWindow( Window(), QW_PARENT ),
										HWND_TOP, 0, 0, iCX, iCY, SWP_SIZE );
}


//------------
// SetTitle() \
//---------------------------------------------------------------------------
//
//	Description:
//		This method sets the current window title shown in the title bar.
//
//	Parameters:
//		szTitle		- Pointer to new title string
//
//	Returns:
//		void
//
void C_WINDOW_STD::SetTitle( char *szTitle )
{
	// Show the new window title
	WinSetWindowText( ParentWindow(), szTitle );
}


//------------
// GetTitle() \
//---------------------------------------------------------------------------
//
//	Description:
//		This method retrieves the current window title shown in the title bar.
//
//	Parameters:
//		szTitle		- Pointer to a data area to get the current title string
//		iLength		- Size of the the out buffer in bytes
//
//	Returns:
//		void
//
void C_WINDOW_STD::GetTitle( char *szTitle, int iLength )
{
	// Get the title from the title bar
	WinQueryWindowText( ParentWindow(), iLength, szTitle );
}


//----------
// Create() \
//---------------------------------------------------------------------------
//
//	Description:
//		This method performs the actual window creation by calling the PM API
//		WinCreateStdWindow().  It creates a window using the parameters
//		specified by the caller.
//
//	Parameters:
//		iID			- window ID for the new window
//		szTitle		- Title string for the window
//
//	Returns:
//		void
//
void C_WINDOW_STD::Create( int iID, char *szTitle )
{
	HWND	hFrameWnd;
	HWND	hWnd;


	// Create a new standard window
	hFrameWnd = WinCreateStdWindow( HWND_DESKTOP, 0, &lFrameFlags,
				ClassName(), szTitle, 0, (HMODULE)NULL, iID, &hWnd );

	// Associate the client window with the frame
	C_WINDOW::Create( hFrameWnd, hWnd );
}


//----------
// MsgPaint \
//---------------------------------------------------------------------------
// Event:		WM_PAINT
// Cause:		Issued by the OS when the window needs to be redrawn
// Description:	This method is so commonly used that it has been provided
//				at this level in the code to avoid having to recode it for
//				each application window.
//
void *C_WINDOW_STD::MsgPaint( void *mp1, void *mp2 )
{
	HPS		hps;
	RECTL	rc;

	hps = WinBeginPaint( Window(), 0L, &rc );

	WinFillRect( hps, &rc, SYSCLR_APPWORKSPACE );
	GpiSetColor( hps, CLR_NEUTRAL );

	WinEndPaint( hps );

	return FALSE;
}

