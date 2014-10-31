//-------------
// CONTAIN.CPP \
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
//	This file contains the methods to describe the C_CONTAINER class.
//	C_CONTAINER provides an interface to the PM Container control.  The
//	container class code includes support for all of the views specified in
//	PM including detail, tree and name view.
//
//---------------------------------------------------------------------------

//-------------------
// OS/2 Conditionals \
//---------------------------------------------------------------------------
#define INCL_DOS
#define INCL_WIN
#define INCL_GPI

//------------------
// Standard Headers \
//---------------------------------------------------------------------------
#include <os2.h>
#include <process.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <io.h>

//-----------------
// PMCLASS Headers \
//---------------------------------------------------------------------------
#include <window.hpp>
#include <winchild.hpp>
#include <dialog.hpp>
#include <contain.hpp>


//-------------
// C_CONTAINER \
//---------------------------------------------------------------------------
//
// Description:
//		This constructor creates an instance of the C_CONTAINER class.  This
//		class will typically be abstract, so this constructor will generally not
//		be called directly.
//
//	Parameters:
//		pxcParentObj		- Pointer to owner object window
//		iID					- Resource ID of the container window
//		iView				- View of the container (see header file)
//		iFlags				- Any additional setup flags
//		iMode				- Any additional container window parameters
//
C_CONTAINER::C_CONTAINER( C_WINDOW *pxcParentObj, int iID, int iView,
					int iFlags, int iMode ) : C_WINDOW_CHILD( pxcParentObj, 0 )
{
	ClassName( WC_CONTAINER );

	// Create the group list box
	if( iView == D_VIEW_NONE )
		Create( iID, iMode, "", 0, 0, 90, 30 );
	else
		Create( iID, WS_VISIBLE | iMode, "", 0, 0, 90, 30 );

	// Setup the group container
	Setup( iView, iFlags );
}


//-------------
// C_CONTAINER \
//---------------------------------------------------------------------------
//
// Description:
//		This constructor creates an instance of the C_CONTAINER class.  This
//		class will typically be abstract, so this constructor will generally not
//		be called directly.  With this constructor, the container defaults to
//		extended selection, read only, and auto-position.
//
//	Parameters:
//		pxcParentObj		- Pointer to owner object window
//		iID					- Resource ID of the container window
//		iView				- View of the container (see header file)
//		iFlags				- Any additional setup flags
//
C_CONTAINER::C_CONTAINER( C_WINDOW *pxcParentObj, int iID, int iView,
							int iFlags ) : C_WINDOW_CHILD( pxcParentObj, 0 )
{
	ClassName( WC_CONTAINER );

	// Create the group list box
	if( iView == D_VIEW_NONE )
		Create( iID, CCS_READONLY|CCS_EXTENDSEL|
					CCS_VERIFYPOINTERS|CCS_AUTOPOSITION, "", 0, 0, 90, 30 );
	else
		Create( iID, WS_VISIBLE|CCS_READONLY|CCS_EXTENDSEL|
					CCS_VERIFYPOINTERS|CCS_AUTOPOSITION, "", 0, 0, 90, 30 );

	// Setup the group container
	Setup( iView, iFlags );
}


//-------------
// C_CONTAINER \
//---------------------------------------------------------------------------
//
// Description:
//		This constructor creates an instance of the C_CONTAINER class which
//		originates in a dialog box supplied by the resource file.  This
//		class will typically be abstract, so this constructor will generally not
//		be called directly.
//
//	Parameters:
//		pxcParentObj		- Pointer to owner object dialog
//		iID					- Resource ID of the container window
//		iView				- View of the container (see header file)
//		iFlags				- Any additional setup flags
//		iMode				- Any additional container window parameters
//
C_CONTAINER::C_CONTAINER( C_DIALOG *pxcParentObj, int iID, int iView,
							int iFlags ) : C_WINDOW_CHILD( pxcParentObj, 0 )
{
	ClassName( WC_CONTAINER );

	// The window was created by the dialog system, so all we need to
	// do is attach the instance to it
	C_WINDOW::Create( ParentObject()->Window(),
					WinWindowFromID( ParentObject()->Window(), iID ) );

	// Setup the group container
	Setup( iView, iFlags );
}


//--------------
// ~C_CONTAINER \
//---------------------------------------------------------------------------
// Description:
//		This destructor, removes all items from the container before it is
//		destroyed.
//
//	Parameters:
//		none
//
C_CONTAINER::~C_CONTAINER( void )
{
	// Remove all records in the container
	Remove();
}


//-------
// Setup \
//---------------------------------------------------------------------------
// Description:
//		This method sets up the container according to specified view.
//
//	Parameters:
//		iView				- D_VIEW_DETAIL_TITLE, D_VIEW_DETAIL, D_VIEW_TREE
//		iFlags				- Any additional PM container flags
//
//	Returns:
//				none
//
void C_CONTAINER::Setup( int iView, int iFlags )
{
	CNRINFO		CI;


	// Setup the container format
	CI.cb = sizeof( CNRINFO );

	// Configure the correct container view
	switch( iView )
	{
		case D_VIEW_DETAIL_TITLE:
			CI.flWindowAttr = CV_DETAIL | CA_TITLESEPARATOR | CA_DETAILSVIEWTITLES | iFlags;
			break;
		case D_VIEW_DETAIL:
			CI.flWindowAttr = CV_DETAIL | iFlags;
			break;
		case D_VIEW_TREE:
			CI.flWindowAttr = CV_TREE | CV_ICON | CA_TREELINE | iFlags;
			break;
	}

	// Set the icon size and spacing for any icons that get shown
	CI.cyLineSpacing = 0;
	CI.slBitmapOrIcon.cx = WinQuerySysValue( HWND_DESKTOP, SV_CYMENU );
	CI.slBitmapOrIcon.cy = CI.slBitmapOrIcon.cx;

	// Perform the container setup
	SendMsg( CM_SETCNRINFO, MPFROMP( &CI ), MPFROMLONG(
					CMA_FLWINDOWATTR | CMA_LINESPACING|CMA_SLBITMAPORICON ) );
}


//----------
// Allocate \
//---------------------------------------------------------------------------
// Description:
//		This method allocates dynamic space for the specified number of
//		new container records.
//
//	Parameters:
//		iLength				- Sizeof the record structure for the container
//		iCtr				- Number of records to allocate
//
//	Returns:
//				none
//
void *C_CONTAINER::Allocate( ULONG iLength, USHORT iCtr )
{
	ULONG			cbData;


	// Determine the additional data space required
	cbData = iLength - sizeof(RECORDCORE);

	// Allocate a record
	return (void *)SendMsg( CM_ALLOCRECORD,
									MPFROMLONG( cbData ), MPFROMSHORT(iCtr) );
}


//--------
// Insert \
//---------------------------------------------------------------------------
// Description:
//		This method inserts a specified number of records into the container.
//		If the pParent attribute points to an existing record, then the inserts
//		will become children of the parent record.  This is used primarily for
//		Tree View.
//		The update determines if the container should be repainted after the
//		records have been inserted.
//
//	Parameters:
//		pParent				- Optional parent record
//		pRecord				- Pointer to array of records to be inserted
//      iCount				- Number of records to be inserted
//		iUpdate				- Set if repaint is required after insert
//
//	Returns:
//				none
//
void C_CONTAINER::Insert( void *pParent, void *pRecord, int iCount, int iUpdate )
{
	RECORDINSERT	RI;

	// Create Insert information
	RI.cb = sizeof(RECORDINSERT);
	RI.pRecordOrder = (RECORDCORE *)CMA_END;
	RI.pRecordParent = (RECORDCORE *)pParent;
	RI.zOrder = (ULONG)CMA_TOP;
	RI.cRecordsInsert = iCount;
	RI.fInvalidateRecord = iUpdate;

	// Insert the record
	SendMsg( CM_INSERTRECORD, MPFROMP( pRecord ), MPFROMP( &RI ) );
}


//--------
// Insert \
//---------------------------------------------------------------------------
// Description:
//		This method inserts a specified number of records into the container.
//		If the pParent attribute points to an existing record, then the inserts
//		will become children of the parent record.  This is used primarily for
//		Tree View.
//
//	Parameters:
//		pParent				- Optional parent record
//		pRecord				- Pointer to array of records to be inserted
//      iCount				- Number of records to be inserted
//
//	Returns:
//				none
//
void C_CONTAINER::Insert( void *pParent, void *pRecord, int iCount )
{
	Insert( pParent, pRecord, iCount, FALSE );
}


//--------------
// InsertUpdate \
//---------------------------------------------------------------------------
// Description:
//		This method inserts a specified number of records into the container.
//		If the pParent attribute points to an existing record, then the inserts
//		will become children of the parent record.  This is used primarily for
//		Tree View.
//
//	Parameters:
//		pParent				- Optional parent record
//		pRecord				- Pointer to array of records to be inserted
//      iCount				- Number of records to be inserted
//
//	Returns:
//				none
//
void C_CONTAINER::InsertUpdate( void *pParent, void *pRecord, int iCount )
{
	Insert( pParent, pRecord, iCount, TRUE );
}


//--------
// Remove \
//---------------------------------------------------------------------------
// Description:
//		This method removes all records from the container and frees the
//		memory associated with the items.
//		If records within the container use dynamic memory, this memory is
//		NOT freed by this function.
//
//	Parameters:
//		none
//
//	Returns:
//				none
//
void C_CONTAINER::Remove( void )
{
	// Remove all records from the container
	SendMsg( CM_REMOVERECORD, 0, MPFROM2SHORT( 0, CMA_FREE ) );
}


//--------
// Remove \
//---------------------------------------------------------------------------
// Description:
//		This method removes a specified number of records from the container
//		and frees the memory associated with the items.
//		If records within the container use dynamic memory, this memory is
//		NOT freed by this function.
//
//	Parameters:
//		pvData		- Pointer to first container record to be removed
//		iCount		- Number of record to remove
//
//	Returns:
//				none
//
void C_CONTAINER::Remove( void *pvData, short iCount )
{
	PRECORDCORE		*pRecordArray;

	pRecordArray = (PRECORDCORE *)new PRECORDCORE[iCount];

	pRecordArray[0] = (PRECORDCORE)pvData;
	SendMsg( CM_REMOVERECORD, pRecordArray, MPFROM2SHORT( iCount, CMA_FREE ) );

	delete pRecordArray;
}


//--------
// Redraw \
//---------------------------------------------------------------------------
// Description:
//		This method forces all records in the container to be refreshed,
//		forcing the entire container to redraw.
//
//	Parameters:
//		pvData		- Pointer to first container record to be redrawn
//
//	Returns:
//				none
//
void C_CONTAINER::Redraw( void *pRecord )
{
	PRECORDCORE		*pRecordArray;


	// Redraw records in the container
	if( pRecord )
	{
		pRecordArray = (PRECORDCORE *)new PRECORDCORE[1];
		pRecordArray[0] = (PRECORDCORE)pRecord;

		SendMsg( CM_INVALIDATERECORD, pRecordArray,
									MPFROM2SHORT( 1, CMA_TEXTCHANGED ) );
		delete pRecordArray;
	}
	else
		SendMsg( CM_INVALIDATERECORD, 0, MPFROM2SHORT( 0, CMA_ERASE ) );
}


//------
// Sort \
//---------------------------------------------------------------------------
// Description:
//		This method forces all records in the container to be sorted according
//		to the sort "C" function specified.  The sort function must be defined
//		with "C" linkage to prevent compiler errors.
//
//	Parameters:
//		SortFunc	- Pointer to the sorting function
//
//	Returns:
//				none
//
void C_CONTAINER::Sort( void *SortFunc )
{
	// Remove all records from the container
	SendMsg( CM_SORTRECORD, SortFunc, 0 );
}


//--------
// Search \
//---------------------------------------------------------------------------
// Description:
//		This method accepts a starting record and string and searches for the
//		specified string in the container view determined by the iType
//		parameter.
//
//		iType is one of CV_DETAIL, CV_ICON, or CV_TREE views.
//
//	Parameters:
//		pStart		- Search starting record (0 for first record)
//		szString	- Pointer to search string
//		iType		- Container view to search
//
//	Returns:
//				none
//
void *C_CONTAINER::Search( void *pStart, char *szString, unsigned int iType )
{
	SEARCHSTRING	xtSearch;


	// Set up general search parameters
	xtSearch.cb = sizeof( SEARCHSTRING );
	xtSearch.pszSearch = (PSZ)szString;
	xtSearch.usView = iType;
	xtSearch.fsPrefix = (ULONG)FALSE;
	xtSearch.fsCaseSensitive = (ULONG)FALSE;

	// Convert NULL start points to a real container start (CMA_FIRST)
	if( pStart == NULL )
		pStart = (void *)CMA_FIRST;

	// Search the container for the selected record
	return SendMsg( CM_SEARCHSTRING, MPFROMP( &xtSearch ), MPFROMP( pStart ) );
}


//--------------
// ParentRecord \
//---------------------------------------------------------------------------
// Description:
//		This method will return a pointer to the parent record of the
//		container record specified.  If no parent exists, a NULL value is
//		returned.  The function would only be applicable to tree views of
//		containers.
//
//	Parameters:
//		hCurrent	- Pointer to child record
//
//	Returns:
//				none
//
void *C_CONTAINER::ParentRecord( void *hCurrent )
{
	// Return the first record
	return SendMsg( CM_QUERYRECORD, hCurrent,
								MPFROM2SHORT( CMA_PARENT, CMA_ITEMORDER ) );
}


//-------------
// FirstRecord \
//---------------------------------------------------------------------------
// Description:
//		This method will return a pointer to the first record in the container.
//		If the container is empty, this method will return a NULL pointer.
//
//	Parameters:
//				none
//
//	Returns:
//				none
//
void *C_CONTAINER::FirstRecord( void )
{
	// Return the first record
	return SendMsg( CM_QUERYRECORD, 0,
							MPFROM2SHORT( CMA_FIRST, CMA_ITEMORDER ) );
}


//-------------------
// MemoryFirstRecord \
//---------------------------------------------------------------------------
// Description:
//		This method will return a pointer to the first record in the container.
//		If the container is empty, this method will return a NULL pointer.
//
//	Parameters:
//				none
//
//	Returns:
//				none
//
void *C_CONTAINER::MemoryFirstRecord( void )
{
	// Return the first record
	return SendMsg( CM_QUERYRECORD, 0, MPFROM2SHORT( CMA_FIRST, CMA_ZORDER ) );
}


//------------------
// MemoryNextRecord \
//---------------------------------------------------------------------------
// Description:
//		This method will return a pointer to the next record from the
//		specified current record by using the memory pointer rather than
//		performaing an actual query to the container.
//		NOTE:  This method is only usefull for performing fast deletes because
//		the order is not guranteed to be correct.
//		If the current record is the last record, a NULL value is returned.
//
//	Parameters:
//		hCurrent	- Pointer to current record
//
//	Returns:
//				none
//
void *C_CONTAINER::MemoryNextRecord( void *hCurrent )
{
	RECORDCORE *pRecord;


	pRecord = (RECORDCORE *)hCurrent;

	// Return the next record
	return (void *)pRecord->preccNextRecord;
}


//------------
// NextRecord \
//---------------------------------------------------------------------------
// Description:
//		This method will return a pointer to the next record from the
//		specified current record.
//		If the current record is the last record, a NULL value is returned.
//
//	Parameters:
//		hCurrent	- Pointer to current record
//
//	Returns:
//				none
//
void *C_CONTAINER::NextRecord( void *hCurrent )
{
	// Return the next record
	return SendMsg( CM_QUERYRECORD, hCurrent,
							MPFROM2SHORT( CMA_NEXT, CMA_ITEMORDER ) );
}


//----------------
// PreviousRecord \
//---------------------------------------------------------------------------
// Description:
//		This method will return a pointer to the previous record from the
//		specified current record.
//		If the current record is the first record, a NULL value is returned.
//
//	Parameters:
//		hCurrent	- Pointer to current record
//
//	Returns:
//				none
//
void *C_CONTAINER::PreviousRecord( void *hCurrent )
{
	// Return the next record
	return SendMsg( CM_QUERYRECORD, hCurrent,
							MPFROM2SHORT( CMA_PREV, CMA_ITEMORDER ) );
}


//------------
// FirstChild \
//---------------------------------------------------------------------------
// Description:
//		This method will return a pointer to the first child record of the
//		specified parent record.  If no child exists, this method returns a NULL
//		pointer.
//
//	Parameters:
//		hCurrent	- Pointer to current record
//
//	Returns:
//				none
//
void *C_CONTAINER::FirstChild( void *hParent )
{
	void	*pRecord;


	// Return the first record
	pRecord = SendMsg( CM_QUERYRECORD, hParent,
							MPFROM2SHORT( CMA_FIRSTCHILD, CMA_ZORDER ) );
	if( (LONG)pRecord == -1 )
		return 0;
	return pRecord;
}


//-----------
// LastChild \
//---------------------------------------------------------------------------
// Description:
//		This method will return a pointer to the last child record of the
//		specified parent record.  If no child exists, this method returns a NULL
//		pointer.
//
//	Parameters:
//		hCurrent	- Pointer to current record
//
//	Returns:
//				none
//
void *C_CONTAINER::LastChild( void *hParent )
{
	// Return the next record
	return SendMsg( CM_QUERYRECORD, hParent,
							MPFROM2SHORT( CMA_LASTCHILD, CMA_ZORDER ) );
}


//---------------
// FirstSelected \
//---------------------------------------------------------------------------
// Description:
//		This method will return a pointer to the first record that is selected
//		within the container.  If no record is selected, this method will
//		return a NULL pointer.
//
//	Parameters:
//				none
//
//	Returns:
//				none
//
void *C_CONTAINER::FirstSelected( void )
{
	// Get the currently selected record
	return SendMsg( CM_QUERYRECORDEMPHASIS, (void *)CMA_FIRST,
										(void *)MPFROMSHORT( CRA_SELECTED ) );
}


//--------------
// NextSelected \
//---------------------------------------------------------------------------
// Description:
//		This method will return a pointer to the next record that is selected
//		within the container.  If no record is selected, this method will
//		return a NULL pointer.  This is useful only if the container is a
//		detailed view with multiselection enabled.
//
//	Parameters:
//				none
//
//	Returns:
//				none
//
void *C_CONTAINER::NextSelected( void *hCurrent )
{
	LONG	lValue;

	// Get the currently selected record
	lValue = (LONG)SendMsg( CM_QUERYRECORDEMPHASIS,
								hCurrent, MPFROMSHORT( CRA_SELECTED ) );
	if( lValue == -1 )
		lValue = 0;

	return (void *)lValue;
}

//------------
// ExpandTree \
//---------------------------------------------------------------------------
// Description:
//		This method will expand the children of the specified parent record
//		in tree fashion.  This is useful only in tree view containers.
//
//	Parameters:
//		pRecord		- Pointer to current record
//
//	Returns:
//				none
//
void C_CONTAINER::ExpandTree( void *pRecord )
{
	// Get the currently selected record
	SendMsg( CM_EXPANDTREE, pRecord, 0 );
}


//--------------
// CompressTree \
//---------------------------------------------------------------------------
// Description:
//		This method will compress the children of the specified parent record
//		in tree fashion.  This is useful only in tree view containers.
//
//	Parameters:
//		pRecord		- Pointer to current record
//
//	Returns:
//				none
//
void C_CONTAINER::CompressTree( void *pRecord )
{
	// Get the currently selected record
	SendMsg( CM_COLLAPSETREE, pRecord, 0 );
}


//--------------
// SelectRecord \
//---------------------------------------------------------------------------
// Description:
//		This method will select or deselect the specified record as determined
//		by the boolean state flag passed from the caller.
//
//	Parameters:
//		pRecord		- Pointer to current record
//		sBool		- Select or Deselect the record (TRUE or FALSE)
//
//	Returns:
//				none
//
void C_CONTAINER::SelectRecord( void *pRecord, short sBool )
{
	// Get the currently selected record
	SendMsg( CM_SETRECORDEMPHASIS, pRecord,
					MPFROM2SHORT( sBool, CRA_SELECTED ) );
}

