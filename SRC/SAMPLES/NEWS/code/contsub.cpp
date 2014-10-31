//-------------
// CONTSUB.CPP \
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

#include <contsub.hpp>


//-------------
// Constructor \
//---------------------------------------------------------------------------
//
//	Description:
//		This constructor initializes the container object instnace
//
C_CONTAINER_SUB::C_CONTAINER_SUB( C_WINDOW *pxcParentObj )
		: C_CONTAINER( pxcParentObj, D_CONT_SUB, D_VIEW_DETAIL_TITLE, 0 )
{
	// Set up the coontainer columns
	Columns();
}


//------------
// Destructor \
//---------------------------------------------------------------------------
C_CONTAINER_SUB::~C_CONTAINER_SUB( void )
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
void C_CONTAINER_SUB::RemoveAll( void )
{
	T_SUBRECORD *pRecord;


	// Get the first record in the list
	pRecord = (T_SUBRECORD *)FirstRecord();
	while( pRecord )
	{
		// Free up the dynamic space
		free( pRecord->szGroup );
		free( pRecord->szTotal );

		// Go to the next record
		pRecord = (T_SUBRECORD *)MemoryNextRecord( pRecord );
	}

	// Remove the container records
	C_CONTAINER::Remove();
}


//--------
// Remove \
//---------------------------------------------------------------------------
//
//	Description:
//		This method removes a single record from the container and frees
//		up the dynamic memory space used by its data elements
//
//	Parameters:
//		pvData		- Pointer to record to be removed
//
//	Returns:
//		none
//
void C_CONTAINER_SUB::Remove( void *pvData )
{
	T_SUBRECORD *pRecord;


	// Get the first record in the list
	pRecord = (T_SUBRECORD *)pvData;
	if( pRecord )
	{
		// Free up the dynamic space
		free( pRecord->szGroup );
		free( pRecord->szTotal );

		// Remove the record from the container
		C_CONTAINER::Remove( pvData, 1 );
	}
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
void *C_CONTAINER_SUB::Fill( void *pvData, char *szString )
{
	T_SUBRECORD	*pRecord;


	pRecord = (T_SUBRECORD *)pvData;

	// Allocate space and copy the subscription name
	pRecord->szGroup = (PSZ)malloc( strlen( szString ) + 1 );
	strcpy( (char *)pRecord->szGroup, szString );

	// Total and unread strings are fixed
	pRecord->szTotal = (PSZ)malloc( 10 );
	strcpy( pRecord->szTotal, "0" );

	// Set the icon text in case we ever support icon view
	pRecord->RecordCore.pszIcon = pRecord->szGroup;
	pRecord->RecordCore.cb = sizeof( T_SUBRECORD );

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
void C_CONTAINER_SUB::Columns( void )
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
								  MPFROMLONG( 2 ), NULL );
	pfiFirst = pfi;

	// Set up information about each container column
	pfi->flData     = CFA_STRING | CFA_HORZSEPARATOR | CFA_LEFT | CFA_SEPARATOR;
	pfi->flTitle    = CFA_LEFT;
	pfi->pTitleData = "Subscription";
	pfi->offStruct  = (ULONG)&(((T_SUBRECORD *)0)->szGroup );

	pfi             = pfi->pNextFieldInfo;
	pfi->flData     = CFA_STRING | CFA_LEFT | CFA_HORZSEPARATOR;
	pfi->flTitle    = CFA_LEFT;
	pfi->pTitleData = "Total";
	pfi->offStruct  = (ULONG)&(((T_SUBRECORD *)0)->szTotal );

	// Fill in information about the column data we are about to give the
	// container.
	(void) memset( &fii, 0, sizeof( FIELDINFOINSERT ) );
	fii.cb = sizeof( FIELDINFOINSERT );
	fii.pFieldInfoOrder = (PFIELDINFO) CMA_FIRST;
	fii.cFieldInfoInsert = (SHORT)2;
	fii.fInvalidateFieldInfo = TRUE;

	// Give the container the column information
	WinSendMsg( Window(), CM_INSERTDETAILFIELDINFO, MPFROMP( pfiFirst ),
										MPFROMP( &fii ) );
}

