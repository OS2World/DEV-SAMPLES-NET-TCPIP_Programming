//---------
// MLE.CPP \
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
//	This file contains the methods to describe the C_MLE class.
//	C_MLE provides a class used to create multi-line editor controls.  It
//	supports font and color changes from the system palettes and unlimited
//	buffer size.  Files can be imported or exported from the MLE buffer and
//	text searching is also supported.
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
#include <string.h>

//-----------------
// PMCLASS Headers \
//---------------------------------------------------------------------------
#include <window.hpp>
#include <winchild.hpp>
#include <dialog.hpp>
#include <status.hpp>
#include <mle.hpp>


//-------------
// Constructor \
//---------------------------------------------------------------------------
//
//	Parameters:
//		pxcParentObj		- Pointer to parent window object which owns this
//		iMLEID				- ID of the control
//		iMode				- OS specific parameters to configure the MLE
//
C_MLE::C_MLE( C_WINDOW *pxcParentObj, int iMLEID, int iMode )
									: C_WINDOW_CHILD( pxcParentObj, 0 )
{
	// Set the class to the default OS/2 control name
	ClassName( WC_MLE );

	// Create the MLE control
	Create( iMLEID, iMode, " ", 0, 0, 90, 30 );
}


//-------------
// Constructor \
//---------------------------------------------------------------------------
//
//	Parameters:
//		pxcParentObj		- Pointer to parent window object which owns this
//		iMLEID				- ID of the control
C_MLE::C_MLE( C_WINDOW *pxcParentObj, int iMLEID )
									: C_WINDOW_CHILD( pxcParentObj, 0 )
{
	// Set the class to the default OS/2 control name
	ClassName( WC_MLE );

	// Create the MLE control with some default characteristics
	Create( iMLEID, WS_VISIBLE | MLS_VSCROLL | MLS_HSCROLL | MLS_BORDER,
														"", 0, 0, 90, 30 );
}


//-------------
// Constructor \
//---------------------------------------------------------------------------
//
//	Parameters:
//		pxcParentObj		- Pointer to parent dialog object which owns this
//		iMLEID				- ID of the control
//		iMode				- OS specific parameters to configure the MLE
C_MLE::C_MLE( C_DIALOG *pxcParentObj, int iMLEID )
							: C_WINDOW_CHILD( (C_WINDOW *)pxcParentObj, 0 )
{
	// Set the class to the default OS/2 control name
	ClassName( WC_MLE );

	// The window was created by the dialog system, so all we need to
	// do is assoicate the instance with a resource from the RES file
	C_WINDOW::Create( ParentObject()->Window(),
					WinWindowFromID( ParentObject()->Window(), iMLEID ) );
}


//----------------
// ReadOnlyStatus \
//---------------------------------------------------------------------------
//
//	Parameters:
//		iBool			- Set the read-only status of the MLE on or off
//
//	Returns:
//				none
//
void C_MLE::ReadOnlyStatus( short iBool )
{
	// Toggle the editor's read-only status
	SendMsg( MLM_SETREADONLY, MPFROMSHORT( iBool ), 0 );
}


//----------
// WordWrap \
//---------------------------------------------------------------------------
//
//	Parameters:
//		iBool			- Set the word-wrap state of the MLE on or off
//
//	Returns:
//				none
//
void C_MLE::WordWrap( short iBool )
{
	// Set the word-wrap state
	SendMsg( MLM_SETWRAP, MPFROMSHORT( iBool ), 0 );
}


//----------------------
// ResetDirtyBufferFlag \
//---------------------------------------------------------------------------
//
//	Parameters:
//		none
//
//	Returns:
//		none
//
//	Description:
//				This method resets the detection mechanism used to determine
//				if the MLE buffer has changed.
//
void C_MLE::ResetDirtyBufferFlag( void )
{
	// Clear the dirty buffer indicator
	SendMsg( MLM_SETCHANGED, 0, 0 );
}


//---------------
// IsBufferDirty \
//---------------------------------------------------------------------------
//
//	Parameters:
//		none
//
//	Returns:
//		TRUE or FALSE indicating the dirtiness of the MLE
//
//	Description:
//				This method returns the states of the "dirty buffer" flag
//				which a program can use to determine if the user has made
//				any changes to the MLE contents.
//
int C_MLE::IsBufferDirty( void )
{
	// Toggle the editor's read-only status
	return (int)SendMsg( MLM_QUERYCHANGED, 0, 0 );
}


//------
// UnDo \
//---------------------------------------------------------------------------
//
//	Returns:
//				none
//
void C_MLE::Undo( void )
{
	// Undo the last edit operation
	SendMsg( MLM_UNDO, 0, 0 );
}


//------
// Copy \
//---------------------------------------------------------------------------
//
//	Returns:
//				none
//
void C_MLE::Copy( void )
{
	// Copy selected text to the clipboard
	SendMsg( MLM_COPY, 0, 0 );
}


//-----
// Cut \
//---------------------------------------------------------------------------
//
//	Returns:
//				none
//
void C_MLE::Cut( void )
{
	// Cut selected text to the clipboard
	SendMsg( MLM_CUT, 0, 0 );
}


//-------
// Paste \
//---------------------------------------------------------------------------
//
//	Returns:
//				none
//
void C_MLE::Paste( void )
{
	// Paste clipboard contents into editor
	SendMsg( MLM_PASTE, 0, 0 );
}


//-------
// Clear \
//---------------------------------------------------------------------------
//
//	Returns:
//				none
//
void C_MLE::Clear( void )
{
	// Clear the current text selection
	SendMsg( MLM_CLEAR, 0, 0 );
}


//--------------
// BufferLength \
//---------------------------------------------------------------------------
//
//	Returns:
//				Number of bytes currently stored in the MLE
//
LONG C_MLE::BufferLength( void )
{
	// Return the number of bytse in the editor control
	return (LONG)SendMsg( MLM_QUERYTEXTLENGTH, 0, 0 );
}


//----------------
// DisableRefresh \
//---------------------------------------------------------------------------
//
//	Returns:
//				none
void C_MLE::DisableRefresh( void )
{
	// Disable MLE refreshing
	SendMsg( MLM_DISABLEREFRESH, 0, 0 );
}


//---------------
// EnableRefresh \
//---------------------------------------------------------------------------
//
//	Returns:
//				none
void C_MLE::EnableRefresh( void )
{
	// Enable editor refreshing
	SendMsg( MLM_ENABLEREFRESH, 0, 0 );
}


//--------
// Insert \
//---------------------------------------------------------------------------
//	Parameters:
//		szString		- Pointer t string data to be inserted into the MLE
//
//	Returns:
//				none
void C_MLE::Insert( char *szString )
{
	// Insert the supplied text into the MLE at the current insertion point
	SendMsg( MLM_INSERT, (void *)szString, 0 );
}


//--------
// Delete \
//---------------------------------------------------------------------------
//	Parameters:
//		lStart			- starting byte offset in MLE buffer for the delete
//		lCount			- number of bytes to delete from the MLE
//
//	Returns:
//				none
void C_MLE::Delete( LONG lStart, LONG lCount )
{
	// Delete a range of text from the editor starting at byte lStart for
	// lCount bytes.
	SendMsg( MLM_DELETE, (void *)lStart, (void *)lCount );
}


//--------
// Select \
//---------------------------------------------------------------------------
//	Parameters:
//		lStart			- starting byte offset in MLE buffer for the selection
//		lEnd			- Ending byte offset in MLE buffer for the selection
//
//	Returns:
//				none
void C_MLE::Select( LONG lStart, LONG lEnd )
{
	// Select the specified range of text
	SendMsg( MLM_SETSEL, (void *)lStart, (void *)lEnd );
}


//----------------
// QuerySelection \
//---------------------------------------------------------------------------
//	Parameters:
//		pAnchor			- Pointer to location where selection anchor point
//						  byte offset will be stored
//		pCursor			- Pointer to location where selection cursor byte
//						  offset will be stored
//
//	Returns:
//				none
void C_MLE::QuerySelection( LONG *pAnchor, LONG *pCursor )
{
	// Query the current anchor point
	*pAnchor = (LONG)SendMsg( MLM_QUERYSEL,
										MPFROMSHORT( MLFQS_ANCHORSEL ),0 );

	// Query the current cursor location
	*pCursor = (LONG)SendMsg( MLM_QUERYSEL,
										MPFROMSHORT( MLFQS_CURSORSEL ),0 );
}


//----------------
// TransferBuffer \
//---------------------------------------------------------------------------
// Description:
//		This method sets the buffer where MLE transfers will take place.
//		This is primarily used by OS/2 to control the import and export of text
//		from the MLE window.
//
//	Parameters:
//			szString		- Pointer to import/export buffer
//			lSize			- Size of the buffer
//
//	Returns:
//				none
//
void C_MLE::TransferBuffer( char *szString, LONG lSize )
{
	// Set the import/export transfer buffer
	SendMsg( MLM_SETIMPORTEXPORT, (void *)szString, (void *)lSize );
}


//--------------
// ExportBuffer \
//---------------------------------------------------------------------------
// Description:
//		This method exports the text in the specified range to the transfer
//		buffer.
//
//	Parameters:
//			ipStart			- Byte offset to start of export
//			ipEnd			- Byte offset for end of export
//
//	Returns:
//				none
//
void C_MLE::ExportBuffer( LONG *ipStart, LONG *ipEnd )
{
	SendMsg( MLM_EXPORT, (void *)ipStart, (void *)ipEnd );
}


//-----------
// FindFirst \
//---------------------------------------------------------------------------
// Description:
//		This method searches for the first instance of the specified string
//		within the editor window.
//
//	Parameters:
//			szString		- String to search for
//
//	Returns:
//				none
//
int C_MLE::FindFirst( char *szString, int iCaseSensitive )
{
	int		iResult;


	iResult = TRUE;

	memset( &xtSearch, 0, sizeof( MLE_SEARCHDATA ) );
	xtSearch.cb = sizeof( MLE_SEARCHDATA );
	xtSearch.pchFind = szString;
	xtSearch.cchFind = (short)strlen( szString );
	xtSearch.iptStart = 0;
	xtSearch.iptStop = -1;

	if( iCaseSensitive )
		iResult = (int)SendMsg( MLM_SEARCH,
					(void *)(MLFSEARCH_CASESENSITIVE),
													(void *)&xtSearch );
	else
		iResult = (int)SendMsg( MLM_SEARCH,
					(void *)(MLFSEARCH_SELECTMATCH),
													(void *)&xtSearch );

	if( !iResult )
		DosBeep( 100, 100 );

	return iResult;
}


//----------
// FindNext \
//---------------------------------------------------------------------------
// Description:
//		This method searches for the next instance of the previously defined
//		search string.
//
//	Parameters:
//				none
//
//	Returns:
//				none
//
int C_MLE::FindNext( int iCaseSensitive )
{
	int		iResult;


	iResult = TRUE;
	if( xtSearch.cchFind > 0 )
	{
		xtSearch.iptStart = -1;

		if( iCaseSensitive )
			iResult = (int)SendMsg( MLM_SEARCH,
					(void *)(MLFSEARCH_CASESENSITIVE),
													(void *)&xtSearch );
		else
			iResult = (int)SendMsg( MLM_SEARCH,
					(void *)(MLFSEARCH_SELECTMATCH),
													(void *)&xtSearch );

		if( !iResult )
			DosBeep( 100, 100 );
	}
	else
	{
		DosBeep( 100, 100 );
		iResult = FALSE;
	}

	return iResult;
}


//----------
// FindFrom \
//---------------------------------------------------------------------------
// Description:
//		This method searches for the first instance of the specified string
//		within the editor window occurring after the current cursor location.
//
//	Parameters:
//			szString		- String to search for
//
//	Returns:
//				none
//
int C_MLE::FindFromCursor( char *szString, int iCaseSensitive )
{
	LONG	lAnchor;
	LONG	lCursor;
	int		iResult;


	iResult = TRUE;

	// Determine where the cursor is
	QuerySelection( &lAnchor, &lCursor );

	memset( &xtSearch, 0, sizeof( MLE_SEARCHDATA ) );
	xtSearch.cb = sizeof( MLE_SEARCHDATA );
	xtSearch.pchFind = szString;
	xtSearch.cchFind = (short)strlen( szString );
	xtSearch.iptStart = lCursor;
	xtSearch.iptStop = -1;

	if( iCaseSensitive )
		iResult = (int)SendMsg( MLM_SEARCH,
					(void *)(MLFSEARCH_CASESENSITIVE),
													(void *)&xtSearch );
	else
		iResult = (int)SendMsg( MLM_SEARCH,
					(void *)(MLFSEARCH_SELECTMATCH),
													(void *)&xtSearch );

	if( !iResult )
		DosBeep( 100, 100 );

	return iResult;
}


//-----------
// ChangeAll \
//---------------------------------------------------------------------------
// Description:
//
//	Parameters:
//
//	Returns:
//				none
//
int C_MLE::ChangeAll( char *szFind, char *szReplace, int iCaseSensitive )
{
	int		iResult;


	memset( &xtSearch, 0, sizeof( MLE_SEARCHDATA ) );
	xtSearch.cb = sizeof( MLE_SEARCHDATA );
	xtSearch.pchFind = szFind;
	xtSearch.cchFind = (short)strlen( szFind );
	xtSearch.pchReplace = szReplace;
	xtSearch.cchReplace = (short)strlen( szReplace );
	xtSearch.iptStart = 0;
	xtSearch.iptStop = -1;

	if( iCaseSensitive )
		iResult = (int)SendMsg( MLM_SEARCH,
					(void *)(MLFSEARCH_CHANGEALL|MLFSEARCH_CASESENSITIVE),
													(void *)&xtSearch );
	else
		iResult = (int)SendMsg( MLM_SEARCH,
					(void *)(MLFSEARCH_CHANGEALL|MLFSEARCH_SELECTMATCH),
													(void *)&xtSearch );
	if( !iResult )
	{
		DosBeep( 100, 100 );
	}

	return iResult;
}


//------
// Line \
//---------------------------------------------------------------------------
// Description:
//		This method returns the line number within the editor where the
//		specified buffer offset occurrs.
//
//	Parameters:
//			lPointer	- Location within the editor buffer
//
//	Returns:
//			LONG	- Line number containing the specified point
//
LONG C_MLE::Line( LONG	lPointer )
{
	// Return the line number where the speficed pointer is
	return (LONG)SendMsg( MLM_LINEFROMCHAR, (void *)lPointer, 0 );
}


//--------
// Column \
//---------------------------------------------------------------------------
// Description:
//		This method returns the column number within specified line where the
//		cursor is currently located.
//
//	Parameters:
//			lLine		- Line number to search for cursor column
//
//	Returns:
//			LONG	- Column number containing the cursor
//
LONG C_MLE::Column( LONG lLine )
{
	// Return the column on the specified line where the cursor is
	return (LONG)SendMsg( MLM_CHARFROMLINE, (void *)lLine,0 );
}


//---------------
// NumberOfLines \
//---------------------------------------------------------------------------
// Description:
//		This method returns the number of text lines within the editor.
//
//	Parameters:
//				none
//
//	Returns:
//			LONG	- Number of lines in the editor
//
LONG C_MLE::NumberOfLines( void )
{
	// Return the number of lines in the MLE
	return (LONG)SendMsg( MLM_QUERYLINECOUNT, 0, 0 );
}


//------
// Load \
//---------------------------------------------------------------------------
// Description:
//		This method loads the specified file contents into the editor window.
//
//	Parameters:
//			pxcStatus       - Pointer to optional status line where load status
//							  will be displayed
//			szFileName		- Filename to load
//
//	Returns:
//			none
//
void C_MLE::Load( C_STATUS *pxcStatus, char *szFileName )
{
	char	szBuffer[3000];
	char	szString[512];
	FILE	*hFile;


	// Disable refresh and insure that the MLE is erased
	Delete( 0, BufferLength() );
	SendMsg( MLM_FORMAT, MPFROMSHORT( MLFIE_CFTEXT ), 0 );
	DisableRefresh();

	if( pxcStatus )
		pxcStatus->Text( "Loading..." );

	hFile = fopen( szFileName, "r" );
	if( hFile )
	{
		strcpy( szBuffer, "" );
		while( !feof( hFile ) && fgets( szString, 512, hFile ) )
		{
			// Replace EOL with a NULL
			if( strstr( szString, "\r" ) )
				*strstr( szString, "\r" ) = 0;
			if( strstr( szString, "\n" ) )
				*strstr( szString, "\n" ) = 0;

			strcat( szBuffer, szString );
			strcat( szBuffer, "\r\n" );

			if( strlen( szBuffer ) > 2048 )
			{
				Insert( szBuffer );
				strcpy( szBuffer, "" );
			}
		}

		Insert( szBuffer );

		fclose( hFile );
	}

	// Clear the selection and refresh the window
	Select( 0, 0 );
	EnableRefresh();

	if( pxcStatus )
		pxcStatus->Text( "Loading Complete..." );

	// Reset the "dirty" buffer flag so we can detect any changes
	ResetDirtyBufferFlag();
}


//------
// Save \
//---------------------------------------------------------------------------
// Description:
//		This method saves the edit window contents to the specified file.
//
//	Parameters:
//			pxcStatus       - Pointer to optional status line where save status
//							  will be displayed
//			szFileName	- Filename to save
//
//	Returns:
//			LONG	- Line number containing the specified point
//
void C_MLE::Save( C_STATUS *pxcStatus, char *szFileName )
{
	char		szString[4096];
	FILE		*hFile;
	IPT			iPoint;
	IPT			iEndPoint;
	LONG		lTotal;
	LONG		lCount;


	hFile = fopen( szFileName, "w" );

	lTotal = NumberOfLines();

	SendMsg( MLM_FORMAT, MPFROMSHORT( MLFIE_NOTRANS ), 0 );
	TransferBuffer( szString, 4096 );
	iPoint = 0;

	for( lCount = 0; lCount < lTotal; lCount++ )
	{
		// Get the text parameter for the current line
		iPoint = Column( lCount );
		iEndPoint = (IPT)SendMsg( MLM_QUERYLINELENGTH, (void *)iPoint, 0 );

		// Get the text for the current line
		memset( szString, 0, 4096 );
		ExportBuffer( &iPoint, &iEndPoint );

		if( strlen( szString ) )
		{
			// Replace EOL with a NULL
			if( strstr( szString, "\r" ) )
				*strstr( szString, "\r" ) = 0;
			if( strstr( szString, "\n" ) )
				*strstr( szString, "\n" ) = 0;

			fprintf( hFile, "%s\r\n", szString );
		}
	}

	fclose( hFile );

	if( pxcStatus )
		pxcStatus->Text( "Saving Complete..." );

	// Reset the "dirty" buffer flag so we can detect any changes
	ResetDirtyBufferFlag();
}

