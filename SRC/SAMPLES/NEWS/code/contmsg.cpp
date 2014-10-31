//-------------
// CONTMSG.CPP \
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

#include <contmsg.hpp>


//-------------
// Constructor \
//---------------------------------------------------------------------------
//
//	Description:
//		This constructor initializes the container object instnace
//
C_CONTAINER_MSG::C_CONTAINER_MSG( C_WINDOW *pxcParentObj )
		: C_CONTAINER( pxcParentObj, D_CONT_MSG, D_VIEW_DETAIL_TITLE, 0 )
{
	// Set up the coontainer columns
	Columns();
}


//------------
// Destructor \
//---------------------------------------------------------------------------
C_CONTAINER_MSG::~C_CONTAINER_MSG( void )
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
void C_CONTAINER_MSG::RemoveAll( void )
{
	T_MSGRECORD *pRecord;


	// Get the first record in the list
	pRecord = (T_MSGRECORD *)FirstRecord();
	while( pRecord )
	{
		// Free up the dynamic space
		free( pRecord->szNumber );
		free( pRecord->szLines );
		free( pRecord->szSubject );
		free( pRecord->szAuthor );

		// Go to the next record
		pRecord = (T_MSGRECORD *)MemoryNextRecord( pRecord );
	}

	// Remove the records from the container
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
void C_CONTAINER_MSG::Remove( void *pvData )
{
	T_MSGRECORD *pRecord;


	// Get the first record in the list
	pRecord = (T_MSGRECORD *)pvData;
	if( pRecord )
	{
		// Free up the dynamic space
		free( pRecord->szNumber );
		free( pRecord->szLines );
		free( pRecord->szSubject );
		free( pRecord->szAuthor );

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
void *C_CONTAINER_MSG::Fill( void *pvData, char *szString )
{
	char		*szPtr;
	T_MSGRECORD	*pRecord;


	pRecord = (T_MSGRECORD *)pvData;
	strcat( szString, "\t" );

	// Allocate space and copy the message number
	szPtr = strtok( szString, "\t" );
	pRecord->szNumber = (PSZ)malloc( strlen( szPtr ) + 1 );
	strcpy( (char *)pRecord->szNumber, szPtr );

	// Allocate space and copy the number of lines
	szPtr = strtok( NULL, "\t" );
	pRecord->szLines = (PSZ)malloc( strlen( szPtr ) + 1 );
	strcpy( (char *)pRecord->szLines, szPtr );

	// Allocate space and copy the subject
	szPtr = strtok( NULL, "\t" );
	pRecord->szAuthor = (PSZ)malloc( strlen( szPtr ) + 1 );
	strcpy( (char *)pRecord->szAuthor, szPtr );

	// Allocate space and copy the subject
	szPtr = strtok( NULL, "\t" );
	pRecord->szSubject = (PSZ)malloc( strlen( szPtr ) + 1 );
	strcpy( (char *)pRecord->szSubject, szPtr );

	// Set the icon text in case we ever support icon view
	pRecord->RecordCore.pszIcon = pRecord->szSubject;
	pRecord->RecordCore.cb = sizeof( T_MSGRECORD );

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
void C_CONTAINER_MSG::Columns( void )
{
	PFIELDINFO      pfi;
	PFIELDINFO		pfiFirst;
	FIELDINFOINSERT fii;

	// Remove any previous container column data
	pfi = (PFIELDINFO)SendMsg( CM_QUERYDETAILFIELDINFO,
											  0, MPFROMSHORT( CMA_FIRST ) );
	if( pfi )
		SendMsg( CM_REMOVEDETAILFIELDINFO,
							  MPFROMP( pfi ), MPFROM2SHORT( 0, CMA_FREE ) );

	// Allocate memory for the container column data
	pfi = (PFIELDINFO)SendMsg( CM_ALLOCDETAILFIELDINFO,
								  MPFROMLONG( 4 ), NULL );
	pfiFirst = pfi;

	// Set up information about each container column
	pfi->flData     = CFA_STRING | CFA_HORZSEPARATOR | CFA_LEFT | CFA_SEPARATOR;
	pfi->flTitle    = CFA_LEFT;
	pfi->pTitleData = "Number";
	pfi->offStruct  = (ULONG)&(((T_MSGRECORD *)0)->szNumber );

	pfi             = pfi->pNextFieldInfo;
	pfi->flData     = CFA_STRING | CFA_HORZSEPARATOR | CFA_LEFT | CFA_SEPARATOR;
	pfi->flTitle    = CFA_LEFT;
	pfi->pTitleData = "Lines";
	pfi->offStruct  = (ULONG)&(((T_MSGRECORD *)0)->szLines );

	pfi             = pfi->pNextFieldInfo;
	pfi->flData     = CFA_STRING | CFA_HORZSEPARATOR | CFA_LEFT | CFA_SEPARATOR;
	pfi->flTitle    = CFA_LEFT;
	pfi->pTitleData = "Subject";
	pfi->offStruct  = (ULONG)&(((T_MSGRECORD *)0)->szSubject );

	pfi             = pfi->pNextFieldInfo;
	pfi->flData     = CFA_STRING | CFA_LEFT | CFA_HORZSEPARATOR;
	pfi->flTitle    = CFA_LEFT;
	pfi->pTitleData = "Author";
	pfi->offStruct  = (ULONG)&(((T_MSGRECORD *)0)->szAuthor );

	// Fill in information about the column data we are about to give the
	// container.
	(void) memset( &fii, 0, sizeof( FIELDINFOINSERT ) );
	fii.cb = sizeof( FIELDINFOINSERT );
	fii.pFieldInfoOrder = (PFIELDINFO)CMA_FIRST;
	fii.cFieldInfoInsert = (SHORT)4;
	fii.fInvalidateFieldInfo = TRUE;

	// Give the container the column information
	SendMsg( CM_INSERTDETAILFIELDINFO, MPFROMP( pfiFirst ), &fii );
}

