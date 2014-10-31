//------------
// EDITOR.CPP \
//---------------------------------------------------------------------------
//
//	All information contained in this file is Copyright (c) 1994-1995 by
//	Steven Gutz.  All Rights Reserved
//
//	This source code shall not be implemented, duplicated or used as the basis
//	for any product without prior written consent by the author
//
// Description:
//
//	This file contains the source code the the enhanced system editor.  More
//  specifically it contains the main() and all of the methods of the
//	C_MAIN_WINDOW class.
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
#include <stdlib.h>
#include <string.h>
#include <process.h>

//-----------------
// NVCLASS Headers \
//---------------------------------------------------------------------------
#include <thread.hpp>
#include <threadpm.hpp>
#include <semev.hpp>
#include <ini.hpp>
#include <iniuser.hpp>

//-----------------
// PMCLASS Headers \
//---------------------------------------------------------------------------
#include <app.hpp>
#include <window.hpp>
#include <winstd.hpp>
#include <winchild.hpp>
#include <dialog.hpp>
#include <status.hpp>
#include <button.hpp>
#include <tbar.hpp>
#include <mle.hpp>

//---------------------
// Application Headers \
//---------------------------------------------------------------------------
#include "rc.hpp"
#include <tbartop.hpp>
#include <prodinfo.hpp>
#include <finddlg.hpp>
#include <editor.hpp>


//-------------
// Global Data \
//---------------------------------------------------------------------------
C_APPLICATION	xcApp;
C_WINDOW_MAIN	xcWindow;


//---------------------------
// Main Window Message Table \
//---------------------------------------------------------------------------
DECLARE_MSG_TABLE( xtEditorMain )
	DECLARE_MSG( PM_CREATE,			C_WINDOW_MAIN::MsgCreate )
	DECLARE_MSG( PM_LOAD_FILE,		C_WINDOW_MAIN::MsgLoadFile )
	DECLARE_MSG( PM_FIND,			C_WINDOW_MAIN::MsgFind )
	DECLARE_MSG( PM_FIND_SET_CASE,	C_WINDOW_MAIN::MsgFindSetCase )
	DECLARE_MSG( PM_CHANGE_FIND,	C_WINDOW_MAIN::MsgChangeFind )
	DECLARE_MSG( PM_CHANGE_ALL,		C_WINDOW_MAIN::MsgChangeAll )
	DECLARE_MSG( WM_SIZE,			C_WINDOW_MAIN::MsgSize )
	DECLARE_MSG( WM_CONTROL,		C_WINDOW_MAIN::MsgControl )
	DECLARE_MSG( WM_CHAR,			C_WINDOW_MAIN::MsgChar )
	DECLARE_MSG( WM_CLOSE,			C_WINDOW_MAIN::MsgClose )
	DECLARE_MSG( WM_PAINT,			C_WINDOW_STD::MsgPaint )
END_MSG_TABLE


//---------------------------
// Main Window Command Table \
//---------------------------------------------------------------------------
DECLARE_COMMAND_TABLE( xtCommandMain )
	DECLARE_COMMAND( DM_FILE_NEW,			C_WINDOW_MAIN::CmdFileNew )
	DECLARE_COMMAND( DM_FILE_OPEN,			C_WINDOW_MAIN::CmdFileOpen )
	DECLARE_COMMAND( DM_FILE_SAVE,			C_WINDOW_MAIN::CmdFileSave )
	DECLARE_COMMAND( DM_FILE_SAVEAS,		C_WINDOW_MAIN::CmdFileSaveAs )
	DECLARE_COMMAND( DM_EDIT_UNDO,			C_WINDOW_MAIN::CmdEditUndo )
	DECLARE_COMMAND( DM_EDIT_CUT,			C_WINDOW_MAIN::CmdEditCut )
	DECLARE_COMMAND( DM_EDIT_COPY,			C_WINDOW_MAIN::CmdEditCopy )
	DECLARE_COMMAND( DM_EDIT_PASTE,			C_WINDOW_MAIN::CmdEditPaste )
	DECLARE_COMMAND( DM_EDIT_CLEAR,			C_WINDOW_MAIN::CmdEditClear )
	DECLARE_COMMAND( DM_EDIT_FIND,			C_WINDOW_MAIN::CmdEditFind )
	DECLARE_COMMAND( DM_EDIT_SELECT,		C_WINDOW_MAIN::CmdEditSelect )
	DECLARE_COMMAND( DM_OPTIONS_WRAP_ON,	C_WINDOW_MAIN::CmdOptionsWrapOn )
	DECLARE_COMMAND( DM_OPTIONS_WRAP_OFF,	C_WINDOW_MAIN::CmdOptionsWrapOff )
	DECLARE_COMMAND( DM_HELP_INDEX,			C_WINDOW_MAIN::CmdHelpIndex )
	DECLARE_COMMAND( DM_HELP_GENERAL,		C_WINDOW_MAIN::CmdHelpGeneral )
	DECLARE_COMMAND( DM_HELP_USING,			C_WINDOW_MAIN::CmdHelpUsing )
	DECLARE_COMMAND( DM_HELP_KEYS,			C_WINDOW_MAIN::CmdHelpKeys )
	DECLARE_COMMAND( DM_HELP_INFO,			C_WINDOW_MAIN::CmdHelpInfo )
END_MSG_TABLE


//----------------
// FileLoadThread \
//---------------------------------------------------------------------------
void _Optlink FileLoadThread( void *pvData )
{
	char			szFile[256];
	char			szString[512];
	C_WINDOW_MAIN	*pxcThis;
	C_THREAD_PM	 	*pxcThread;


	// Get a point to the main window object
	pxcThread = (C_THREAD_PM *)pvData;
	pxcThis = (C_WINDOW_MAIN *)pxcThread->ThreadData();

	// Create a PM process for this thread
	pxcThread->InitializeThread();

	// Load the multiline editor with the article file
	pxcThis->MLE()->Load( pxcThis->BottomStatus(), pxcThis->File() );

	// Isolate the file portion of the pathname
	strcpy( szFile, pxcThis->File() );
	if( strstr( szFile, "\\" ) )
	{
		strrev( szFile );
		*strstr( szFile, "\\" ) = 0;
		strrev( szFile );
	}

	// Put the name of the file in the titlebar
	sprintf( szString, "Enhanced System Editor - %s", szFile  );
	pxcThis->SetTitle( szString );

	// Terminate the thread
	pxcThread->TerminateThread();
}


//----------------
// FileSaveThread \
//---------------------------------------------------------------------------
void _Optlink FileSaveThread( void *pvData )
{
	C_WINDOW_MAIN	*pxcThis;
	C_THREAD_PM	 	*pxcThread;


	// Get a point to the main window object
	pxcThread = (C_THREAD_PM *)pvData;
	pxcThis = (C_WINDOW_MAIN *)pxcThread->ThreadData();

	// Create a PM process for this thread
	pxcThread->InitializeThread();

	// Load the multiline editor with the article file
	pxcThis->MLE()->Save( pxcThis->BottomStatus(), pxcThis->File() );

	// Terminate the thread
	pxcThread->TerminateThread();
}


//-------------
// CloseThread \
//---------------------------------------------------------------------------
void _Optlink CloseThread( void *pvData )
{
	C_WINDOW_MAIN	*pxcThis;
	C_THREAD_PM	 	*pxcThread;
	int				iFlag;


	// Get a point to the main window object
	pxcThread = (C_THREAD_PM *)pvData;
	pxcThis = (C_WINDOW_MAIN *)pxcThread->ThreadData();

	// Create a PM process for this thread
	pxcThread->InitializeThread();

	// Kill any load thread that is is progress
	pxcThis->LoadThread()->Kill();

	// Test to see if the buffer is dirty.
	if( pxcThis->MLE()->IsBufferDirty() )
	{
		// Ask the user if he wants to save
		iFlag = (int)WinMessageBox( HWND_DESKTOP, pxcThis->Window(),
				"Buffer has changed, would you like to save?",
				"Enhanced System Editor", 0, MB_YESNO | MB_ICONEXCLAMATION );
		if( iFlag == MBID_YES )
		{
			// Determine if we can do a save or if we have to do a SaveAs
			if( strlen( pxcThis->File() ) )
			{
				// Normal save
				pxcThis->SendMsg( WM_COMMAND, (void *)DM_FILE_SAVE, 0 );
			}
			else
			{
				// Save as
				pxcThis->SendMsg( WM_COMMAND, (void *)DM_FILE_SAVEAS, 0 );
			}
		}

		// Wait for any executing threads to finish
		pxcThis->SaveThread()->WaitIndefinite();
	}

	// Applcation was told to close so post a QUIT message to the OS
	pxcThis->PostMsg( WM_QUIT, 0, 0 );

	// Terminate the thread
	pxcThread->TerminateThread();
}


//-------------
// Constructor \
//---------------------------------------------------------------------------
C_WINDOW_MAIN::C_WINDOW_MAIN( void ) : C_WINDOW_STD( xtEditorMain )
{
	// Enable the required command handlers for this window
	CommandTable( xtCommandMain );
}


//------------
// Destructor \
//---------------------------------------------------------------------------
C_WINDOW_MAIN::~C_WINDOW_MAIN( void )
{
	// Free up the dynamic memory used for status bars
	delete	pxcTopStatus;
	delete	pxcBottomStatus;

	// Free up the multi-line editor object
	delete	pxcMLE;

	// Free up the toolbar object
	delete	pxcTBar;
}

//-----------
// MsgCreate \
//---------------------------------------------------------------------------
// Event:		PM_CREATE
// Cause:		Issued by OS when window is created
// Description:	This method gets called when the window is initially created.
//				It initializes all of the visual aspects of the class.
void *C_WINDOW_MAIN::MsgCreate( void *mp1, void *mp2 )
{
	char	szX[10];
	char	szY[10];
	char	szW[10];
	char	szL[10];
	char	szFont[80];
	char	szFontSize[10];
	char	szBColor[80];
	char	szFColor[80];


	// Create the status bars to display miscellaneous data
	pxcTopStatus = (C_STATUS *) new C_STATUS( this );
	pxcBottomStatus = (C_STATUS *) new C_STATUS( this );

	// Create a toolbar control
	pxcTBar = (C_TOOLBAR_TOP *)new C_TOOLBAR_TOP( this, pxcTopStatus );

	// Create a new multiline editor control
	pxcMLE = (C_MLE *)new C_MLE( this, ID_MAIN_MLE );

	strcpy( szFile, "" );

	// Load parameters out of the INI file
	C_INI_USER xcIni( "BookESE" );
	xcIni.Open();
	xcIni.Read( "WindowFont", szFont, "System Proportional", 80 );
	xcIni.Read( "WindowFontSize", szFontSize, "10", 80 );
	xcIni.Read( "WindowBColor", szBColor, "000,000,000", 80 );
	xcIni.Read( "WindowFColor", szFColor, "255,255,255", 80 );
	xcIni.Read( "WindowX", szX, "0", 10 );
	xcIni.Read( "WindowY", szY, "0", 10 );
	xcIni.Read( "WindowW", szW, "0", 10 );
	xcIni.Read( "WindowL", szL, "0", 10 );
	xcIni.Close();

	// Set the font in the MLE
	pxcMLE->SetFont( szFont, atoi( szFontSize ) );

	// Set the MLE colors
	pxcMLE->SetForegroundColor( atoi( &szFColor[0] ),
								atoi( &szFColor[4] ), atoi( &szFColor[8] ) );
	pxcMLE->SetBackgroundColor( atoi( &szBColor[0] ),
								atoi( &szBColor[4] ), atoi( &szBColor[8] ) );

	if( atoi( szW ) != 0 && atoi( szL ) != 0 )
	{
		// Position and size the window
		SetSizePosition( atoi( szX ), atoi( szY ), atoi( szW ), atoi( szL ) );
	}

	pxcMLE->Focus();
	return (void *)TRUE;
}


//---------
// MsgFind \
//---------------------------------------------------------------------------
// Event:		PM_LOAD_FILE
// Cause:		Issued by the main() routine if a file was specified on the
//				command line
// Description:	This method loads the file specified in mp1.  This will only
//				done if the user specified a filename on the command-line.
void *C_WINDOW_MAIN::MsgLoadFile( void *mp1, void *mp2 )
{
	// Save the filename
	strcpy( szFile, (char *)mp1 );

	// Begin a thread to load the file
	xcLoadThread.Create( FileLoadThread, 40000, this );

	return FALSE;
}


//---------
// MsgFind \
//---------------------------------------------------------------------------
// Event:		PM_FIND
// Cause:		Issued by the Find dialog when a find is required
// Description:	This method searches the MLE from the current cursor location
//				for the string supplied in mp1
void *C_WINDOW_MAIN::MsgFind( void *mp1, void *mp2 )
{
	char	*szFind;


	// Get the find string passed to us
	szFind = (char *)mp1;

	pxcMLE->FindFromCursor( szFind, iCaseSensitive );

	return FALSE;
}


//----------------
// MsgFindSetCase \
//---------------------------------------------------------------------------
// Event:		PM_FIND_SET_CASE
// Cause:		Issued by the Find dialog to set case sensitivity flag
// Description:	This method accepts a value in mp1 which is used to determine
//				if case sensitivity is used for searches and replacements.
void *C_WINDOW_MAIN::MsgFindSetCase( void *mp1, void *mp2 )
{
	// Determine the case sensitivity flag
	if( mp1 )
		iCaseSensitive = 1;
	else
		iCaseSensitive = 0;

	return FALSE;
}


//---------------
// MsgChangeFind \
//---------------------------------------------------------------------------
// Event:		PM_CHANGE_FIND
// Cause:		Issued by the Find dialog when a "change then find" is required
// Description:	This method changes the current selection to the string pointed
//				to by mp2, then searches the MLE from the current cursor
//				location for the string supplied in mp1.
void *C_WINDOW_MAIN::MsgChangeFind( void *mp1, void *mp2 )
{
	char	*szFind;
	char	*szChange;


	// Get the find and change strings string passed to us
	szFind = (char *)mp1;
	szChange = (char *)mp2;

	pxcMLE->Insert( szChange );
	pxcMLE->FindFromCursor( szFind, iCaseSensitive );

	return FALSE;
}


//--------------
// MsgChangeAll \
//---------------------------------------------------------------------------
// Event:		PM_CHANGE_ALL
// Cause:		Issued by the Find dialog when a "change all" is required
// Description:	This method changes all instances of MLE text matching the
//				string supplied in mp1 with the the string supplied in mp2.
void *C_WINDOW_MAIN::MsgChangeAll( void *mp1, void *mp2 )
{
	char	*szFind;
	char	*szChange;


	// Get the find and change strings string passed to us
	szFind = (char *)mp1;
	szChange = (char *)mp2;

	pxcMLE->ChangeAll( szFind, szChange, iCaseSensitive );

	return FALSE;
}


//---------
// MsgSize \
//---------------------------------------------------------------------------
// Event:		WM_SIZE
// Cause:		Issued by OS when window is resized
// Description:	This method is called any time PM decides the window needs
//				to be resized.  It determines the new window dimensions and
//				resizes the visual components accordingly.
//
void *C_WINDOW_MAIN::MsgSize( void *mp1, void *mp2 )
{
	int		iCX;
	int		iCY;


	// Determine the new size of the client area
	C_WINDOW::GetSize( &iCX, &iCY );

	// Stretch the toolbar across the entire window
	iCY -= 40;
	pxcTBar->SetSizePosition( 0, iCY, iCX, 40 );

	// Stretch the top status across the entire window
	iCY -= 20;
	pxcTopStatus->SetSizePosition( 0, iCY, iCX, 20 );

	// Make the container window use whatever space is left minus the height
	// of the second status bar
	pxcMLE->SetSizePosition( 0, 20, iCX, iCY - 20 );

	// Draw the final status bar in the lower region of the window
	pxcBottomStatus->SetSizePosition( 0, 0, iCX, 20 );

	return FALSE;
}


//------------
// MsgControl \
//---------------------------------------------------------------------------
// Event:	WM_CONTROL
// Cause:	Issued by OS for control functions (Toolbar interaction)
//
void *C_WINDOW_MAIN::MsgControl( void *mp1, void *mp2 )
{
	switch( SHORT1FROMMP( mp1 ) )
	{
		case D_TOP_TBAR:
			pxcTBar->Control( (ULONG)mp1 );
			break;
	}
	return FALSE;
}

//---------
// MsgChar \
//---------------------------------------------------------------------------
// Event:	WM_CHAR
// Cause:	Issued by OS when a key is pressed
//
void *C_WINDOW_MAIN::MsgChar( void *mp1, void *mp2 )
{
	LONG	lCursor;
	LONG	lAnchor;
	LONG	lLine;
	LONG	lColumn;


	// Determin the line and column we are at.
	pxcMLE->QuerySelection( &lAnchor, &lCursor );
	lLine = pxcMLE->Line( lCursor );
	lColumn = lCursor - pxcMLE->Column( lLine ) + 1;
	lLine++;

	// Display the line and column in the bottom status bar
	pxcBottomStatus->Text( "Line:%ld:%ld    Column:%ld",
							lLine, pxcMLE->NumberOfLines(), lColumn );

	return FALSE;
}


//----------
// MsgClose \
//---------------------------------------------------------------------------
// Event:	WM_CLOSE
// Cause:	Issued by OS when the user terminates the program
//
void *C_WINDOW_MAIN::MsgClose( void *mp1, void *mp2 )
{
	char	szString[80];
	int		iX;
	int		iY;
	int		iW;
	int		iL;
	BYTE	byR;
	BYTE	byG;
	BYTE	byB;


	// Get all of the saveable parameters
	GetSizePosition( &iX, &iY, &iW, &iL );

	// Save parameters into the INI file
	C_INI_USER xcIni( "BookESE" );
	xcIni.Open();

	// Save the window dimensions
	sprintf( szString, "%d", iX );
	xcIni.Write( "WindowX", szString );
	sprintf( szString, "%d", iY );
	xcIni.Write( "WindowY", szString );
	sprintf( szString, "%d", iW );
	xcIni.Write( "WindowW", szString );
	sprintf( szString, "%d", iL );
	xcIni.Write( "WindowL", szString );

	// Save the font
	pxcMLE->GetFont( szString );
	if( strstr( szString, "." ) )
	{
		xcIni.Write( "WindowFont", strstr( szString, "." ) + 1 );
		*strstr( szString, "." ) = 0;
		xcIni.Write( "WindowFontSize", szString );
	}

	// Save the window foreground color
	pxcMLE->GetForegroundColor( &byR, &byG, &byB );
	sprintf( szString, "%03d,%03d,%03d", byR, byG, byB );
	xcIni.Write( "WindowFColor", szString );

	// Save the window background color
	pxcMLE->GetBackgroundColor( &byR, &byG, &byB );
	sprintf( szString, "%03d,%03d,%03d", byR, byG, byB );
	xcIni.Write( "WindowBColor", szString );

	xcIni.Close();

	// Begin a thread to close the program
	xcCloseThread.Create( CloseThread, 40000, this );

	return FALSE;
}


//------------
// CmdFileNew \
//---------------------------------------------------------------------------
// Event:	DM_FILE_NEW
// Cause:	User selects the File/New menu option
//
void *C_WINDOW_MAIN::CmdFileNew( void *mp1, void *mp2 )
{
	// Select all data in the MLE
	pxcMLE->Delete( 0 , pxcMLE->BufferLength() );

	// Reset the dirty buffer indicator
	pxcMLE->ResetDirtyBufferFlag();

	return FALSE;
}


//-------------
// CmdFileOpen \
//---------------------------------------------------------------------------
// Event:	DM_FILE_OPEN
// Cause:	User selects the File/Open menu option
//
void *C_WINDOW_MAIN::CmdFileOpen( void *mp1, void *mp2 )
{
	FILEDLG		fild;
	HWND		hDlgWnd;


	// Use the OS/2 API to query the user for a file
	memset( &fild, 0, sizeof(FILEDLG) );
	fild.cbSize = sizeof(FILEDLG);
	fild.fl = FDS_CENTER | FDS_OPEN_DIALOG;
	fild.pszTitle = "Open file";
	strcpy( fild.szFullFile, "*.*" );
	hDlgWnd = WinFileDlg( HWND_DESKTOP, Window(), &fild );
	if( hDlgWnd && ( fild.lReturn == DID_OK ) )
	{
		// Get the filename we're supposed to load
		strcpy( szFile, fild.szFullFile );

		// Begin a thread to load the file
		xcLoadThread.Create( FileLoadThread, 40000, this );
	}
	return FALSE;
}


//-------------
// CmdFileSave \
//---------------------------------------------------------------------------
// Event:	DM_FILE_SAVE
// Cause:	User selects the File/Save menu option
//
void *C_WINDOW_MAIN::CmdFileSave( void *mp1, void *mp2 )
{
	// Begin a thread to Save the file
	xcSaveThread.Create( FileSaveThread, 40000, this );

	return FALSE;
}


//---------------
// CmdFileSaveAs \
//---------------------------------------------------------------------------
// Event:	DM_FILE_SAVEAS
// Cause:	User selects the File/SaveAs menu option
//
void *C_WINDOW_MAIN::CmdFileSaveAs( void *mp1, void *mp2 )
{
	FILEDLG		fild;
	HWND		hDlgWnd;


	// Use the OS/2 API to query the user for a file
	memset( &fild, 0, sizeof(FILEDLG) );
	fild.cbSize = sizeof(FILEDLG);
	fild.fl = FDS_CENTER | FDS_SAVEAS_DIALOG;
	fild.pszTitle = "Save file as";
	strcpy( fild.szFullFile, "*.*" );
	hDlgWnd = WinFileDlg( HWND_DESKTOP, Window(), &fild );
	if( hDlgWnd && ( fild.lReturn == DID_OK ) )
	{
		// Get the filename we're supposed to load
		strcpy( szFile, fild.szFullFile );

		// Begin a thread to load the file
		xcSaveThread.Create( FileSaveThread, 40000, this );
	}
	return FALSE;
}


//-------------
// CmdEditUndo \
//---------------------------------------------------------------------------
// Event:	DM_EDIT_UNDO
// Cause:	User selects the Edit/Undo menu option
//
void *C_WINDOW_MAIN::CmdEditUndo( void *mp1, void *mp2 )
{
	// Undo the last editor operation
	pxcMLE->Undo();

	return FALSE;
}


//------------
// CmdEditCut \
//---------------------------------------------------------------------------
// Event:	DM_EDIT_CUT
// Cause:	User selects the Edit/Cut menu option
//
void *C_WINDOW_MAIN::CmdEditCut( void *mp1, void *mp2 )
{
	// Remove the selected text and place it on the clipboard
	pxcMLE->Cut();

	return FALSE;
}


//-------------
// CmdEditCopy \
//---------------------------------------------------------------------------
// Event:	DM_EDIT_COPY
// Cause:	User selects the Edit/Copy menu option
//
void *C_WINDOW_MAIN::CmdEditCopy( void *mp1, void *mp2 )
{
	// Copy the selected text and place it on the clipboard
	pxcMLE->Copy();

	return FALSE;
}


//-------------
// CmdEditPaste \
//---------------------------------------------------------------------------
// Event:	DM_EDIT_PASTE
// Cause:	User selects the Edit/Paste menu option
//
void *C_WINDOW_MAIN::CmdEditPaste( void *mp1, void *mp2 )
{
	// Paste the contents of the clipboard into the file
	pxcMLE->Paste();

	return FALSE;
}


//--------------
// CmdEditClear \
//---------------------------------------------------------------------------
// Event:	DM_EDIT_CLEAR
// Cause:	User selects the Edit/Clear menu option
//
void *C_WINDOW_MAIN::CmdEditClear( void *mp1, void *mp2 )
{
	// Clear the current selection
	pxcMLE->Clear();

	return FALSE;
}


//-------------
// CmdEditFind \
//---------------------------------------------------------------------------
// Event:	DM_EDIT_FIND
// Cause:	User selects the Edit/Find menu option
//
void *C_WINDOW_MAIN::CmdEditFind( void *mp1, void *mp2 )
{
	// Create an instance of the about dialog
	C_DIALOG_FIND	xcFind( this, ID_FIND_DIALOG );

	return FALSE;
}


//---------------
// CmdEditSelect \
//---------------------------------------------------------------------------
// Event:	DM_EDIT_SELECT
// Cause:	User selects the Edit/Select menu option
//
void *C_WINDOW_MAIN::CmdEditSelect( void *mp1, void *mp2 )
{
	// Select all data in the MLE
	pxcMLE->Select( 0 , pxcMLE->BufferLength() );

	return FALSE;
}


//------------------
// CmdOptionsWrapOn \
//---------------------------------------------------------------------------
// Event:	DM_EDIT_WRAP_ON
// Cause:	User selects the Options/Wrap/On menu option
//
void *C_WINDOW_MAIN::CmdOptionsWrapOn( void *mp1, void *mp2 )
{
	// Enable word wrapping
	pxcMLE->WordWrap( TRUE );

	return FALSE;
}

//-------------------
// CmdOptionsWrapOff \
//---------------------------------------------------------------------------
// Event:	DM_EDIT_WRAP_OFF
// Cause:	User selects the Options/Wrap/Off menu option
//
void *C_WINDOW_MAIN::CmdOptionsWrapOff( void *mp1, void *mp2 )
{
	// Disable word wrapping
	pxcMLE->WordWrap( FALSE );

	return FALSE;
}


//--------------
// CmdHelpIndex \
//---------------------------------------------------------------------------
// Event:	DM_HELP_INDEX
// Cause:	User selects the Help/Index menu option
//
void *C_WINDOW_MAIN::CmdHelpIndex( void *mp1, void *mp2 )
{
	return FALSE;
}


//----------------
// CmdHelpGeneral \
//---------------------------------------------------------------------------
// Event:	DM_HELP_General
// Cause:	User selects the Help/General menu option
//
void *C_WINDOW_MAIN::CmdHelpGeneral( void *mp1, void *mp2 )
{
	return FALSE;
}


//--------------
// CmdHelpUsing \
//---------------------------------------------------------------------------
// Event:	DM_HELP_USING
// Cause:	User selects the Help/Using menu option
//
void *C_WINDOW_MAIN::CmdHelpUsing( void *mp1, void *mp2 )
{
	return FALSE;
}


//-------------
// CmdHelpKeys \
//---------------------------------------------------------------------------
// Event:	DM_HELP_KEYS
// Cause:	User selects the Help/Keys menu option
//
void *C_WINDOW_MAIN::CmdHelpKeys( void *mp1, void *mp2 )
{
	return FALSE;
}


//-------------
// CmdHelpInfo \
//---------------------------------------------------------------------------
// Event:	DM_HELP_INFO
// Cause:	User selects the Help/Index menu option
//
void *C_WINDOW_MAIN::CmdHelpInfo( void *mp1, void *mp2 )
{
	// Create an instance of the about dialog
	C_DIALOG_ABOUT	xcAbout( this, ID_PROD_INFO );

	return FALSE;
}


void main( int argc, char *argv[] )
{
	// Register and create a new program window
	xcWindow.Register( "EnhancedSystemEditor" );

	// Set the window characteristics
	xcWindow.WCF_SizingBorder();
	xcWindow.WCF_SysMenu();
	xcWindow.WCF_TaskList();
	xcWindow.WCF_ShellPosition();
	xcWindow.WCF_MinButton();
	xcWindow.WCF_MaxButton();
	xcWindow.WCF_TitleBar();
	xcWindow.WCF_Menu();

	// Create the window
	xcWindow.Create( ID_MAIN, "Enhanced Sytem Editor" );
	xcWindow.Show();

	// If there was a command line argment specified load the file associated
	// with the argument.
	if( argc > 1 )
		xcWindow.SendMsg( PM_LOAD_FILE, (void *)argv[1], 0 );

	// Start the message loop
	xcApp.Run();
}

