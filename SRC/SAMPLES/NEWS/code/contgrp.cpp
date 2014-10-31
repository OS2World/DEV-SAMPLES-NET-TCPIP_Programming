//-------------
// CONTGRP.CPP \
//---------------------------------------------------------------------------
//
//	All information contained in this file is Copyright (c) 1994-1995 by
//	Steven Gutz.  All Rights Reserved
//
//	This source code shall not be implemented, duplicated or used as the basis
//	for any product without prior written consent by the author
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
// PMCLASS Headers \
//---------------------------------------------------------------------------
#include <app.hpp>
#include <window.hpp>
#include <winstd.hpp>
#include <winchild.hpp>
#include <dialog.hpp>
#include <edit.hpp>
#include <status.hpp>
#include <button.hpp>
#include <tbar.hpp>
#include <mle.hpp>
#include <contain.hpp>
#include <log.hpp>

#include <contgrp.hpp>


//-------------
// Constructor \
//---------------------------------------------------------------------------
//
//	Description:
//		This constructor initializes the container object instnace
//
C_CONTAINER_GRP::C_CONTAINER_GRP( C_WINDOW *pxcParentObj )
		: C_CONTAINER( pxcParentObj, D_CONT_GRP, D_VIEW_DETAIL_TITLE, 0 )
{
	// Set up the coontainer columns
	Columns();
}


//------------
// Destructor \
//---------------------------------------------------------------------------
C_CONTAINER_GRP::~C_CONTAINER_GRP( void )
{
	// Remove all container records
	RemoveAll();
}


//-----------
// RemoveAll \
//---------------------------------------------------------------------------
//
//	Description:
//		This method removes all of the records from the container and frees
//		up the dynamic memory space used by the data element of its records.
//
//	Parameters:
//		none
//
//	Returns:
//		none
//
void C_CONTAINER_GRP::RemoveAll( void )
{
	T_GRPRECORD *pRecord;


	// Get the first record in the list
	pRecord = (T_GRPRECORD *)FirstRecord();
	while( pRecord )
	{
		// Free up the dynamic space
		free( pRecord->szString );

		// Go to the next record
		pRecord = (T_GRPRECORD *)MemoryNextRecord( pRecord );
	}

	// Remove the container records
	C_CONTAINER::Remove();
}


//------
// Fill \
//---------------------------------------------------------------------------
//
//	Description:
//		This method populates the specfied record space with the data
//		supplied in szString.  The szString contents are parsed and stored in
//		the appropriate record fields.
//
//	Parameters:
//		pvData		- Pointer to the record to be populated
//		szString	- data string containing field information
//
//	Returns:
//		void *		- pointer to the next available container record
//
void *C_CONTAINER_GRP::Fill( void *pvData, char *szString )
{
	T_GRPRECORD	*pRecord;


	// Get a pointer to the current container record
	pRecord = (T_GRPRECORD *)pvData;

	// Copy the record contents
	pRecord->szString = (char *)malloc( strlen( szString ) + 2 );
	strcpy( pRecord->szString, szString );

	// Set the icon text in case we ever support icon view
	pRecord->RecordCore.pszIcon = pRecord->szString;
	pRecord->RecordCore.cb = sizeof( T_GRPRECORD );

	// Return the next available record
	return (void *)pRecord->RecordCore.preccNextRecord;
}


//---------
// Columns \
//---------------------------------------------------------------------------
//
//	Description:
//		This method creates the display columns for the container.
//
//	Parameters:
//		none
//
//	Returns:
//		none
//
void C_CONTAINER_GRP::Columns( void )
{
	PFIELDINFO      pfi;
	PFIELDINFO		pfiFirst;
	FIELDINFOINSERT fii;

	// Remove any previous container column data
	pfi = (PFIELDINFO)WinSendMsg( Window(), CM_QUERYDETAILFIELDINFO,
											  0, MPFROMSHORT( CMA_FIRST ) );
	if( pfi )
		WinSendMsg( Window(), CM_REMOVEDETAILFIELDINFO,
							  MPFROMP( pfi ), MPFROM2SHORT( 0, CMA_FREE ) );

	// Allocate memory for the container column data
	pfi = (PFIELDINFO)WinSendMsg( Window(), CM_ALLOCDETAILFIELDINFO,
								  MPFROMLONG( 1 ), NULL );
	pfiFirst = pfi;

	// Set up information about each container column
	pfi->flData     = CFA_STRING | CFA_HORZSEPARATOR | CFA_LEFT;
	pfi->flTitle    = CFA_LEFT;
	pfi->pTitleData = "Name";
	pfi->offStruct  = (ULONG)&(((T_GRPRECORD *)0)->szString );

	// Fill in information about the column data we are about to give the
	// container.
	(void) memset( &fii, 0, sizeof( FIELDINFOINSERT ) );
	fii.cb                   = sizeof( FIELDINFOINSERT );
	fii.pFieldInfoOrder      = (PFIELDINFO) CMA_FIRST;
	fii.cFieldInfoInsert     = (SHORT)1;
	fii.fInvalidateFieldInfo = TRUE;

	// Give the container the column information
	WinSendMsg( Window(), CM_INSERTDETAILFIELDINFO, MPFROMP( pfiFirst ),
										MPFROMP( &fii ) );
}


//------------------
// SortGroupByAlpha \
//---------------------------------------------------------------------------
//
//	Description:
//		This C Function is used by the PM container code to sort records
//		alphabetically.
//
//	Parameters:
//		r1, r2		- pointers to the 2 records being compared
//
//	Returns:
//		SHORT		-1, 0, or 1 depending on the comparison
//		none
//
SHORT APIENTRY SortGroupByAlpha( T_GRPRECORD *r1, T_GRPRECORD *r2, PVOID pvStorage )
{
	return (SHORT)strcmp( r1->szString, r2->szString );
}

