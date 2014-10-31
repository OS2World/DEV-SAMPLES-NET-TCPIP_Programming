//-------------------
// OS/2 Conditionals \
//---------------------------------------------------------------------------
#define	INCL_DOS

//------------------
// Standard Headers \
//---------------------------------------------------------------------------
#include <os2.h>
#include <stdio.h>
#include <string.h>
#include <process.h>

#include <semev.hpp>


//-------------
// Constructor \
//---------------------------------------------------------------------------
C_SEM_EVENT::C_SEM_EVENT( char *szSemaphoreName )
{
	// Named semaphore
	strcpy( szName, szSemaphoreName );
}


//-------------
// Constructor \
//---------------------------------------------------------------------------
C_SEM_EVENT::C_SEM_EVENT( void )
{
	// Unnamed semaphore
	strcpy( szName, "" );
}


//--------
// Create \
//---------------------------------------------------------------------------
// Description:
//		This method wraps the operating system dependant call to create a new
//  	event semaphore.
//
//	Parameters:
//				none
//
//	Returns:
//				Operating system status of create operation
//
int C_SEM_EVENT::Create( void )
{
	// Create a new event semaphore
	return (int)DosCreateEventSem( szName, &hSemaphore, 0, FALSE );
}


//------
// Open \
//---------------------------------------------------------------------------
// Description:
//		This method wraps the operating system dependant call to open an
//  	event semaphore.
//
//	Parameters:
//				none
//
//	Returns:
//				Operating system status of open operation
//
int C_SEM_EVENT::Open( void )
{
	// Open an event semaphore
	return (int)DosOpenEventSem( szName, &hSemaphore );
}


//-------
// Close \
//---------------------------------------------------------------------------
// Description:
//		This method wraps the operating system dependant call to close an
//  	event semaphore.
//
//	Parameters:
//				none
//
//	Returns:
//				Operating system status of close operation
//
int C_SEM_EVENT::Close( void )
{
	// Close the event semaphore
	return (int)DosCloseEventSem( hSemaphore );
}


//-------
// Reset \
//---------------------------------------------------------------------------
// Description:
//		This method wraps the operating system dependant call to reset an
//  	event semaphore.
//
//	Parameters:
//				plPostCount		pointer to the number of times the semaphore has
//								been posted
//
//	Returns:
//				Operating system status of reset operation
//
int C_SEM_EVENT::Reset( ULONG *plPostCount )
{
	// Reset the event semaphore
	return (int)DosResetEventSem( hSemaphore, plPostCount );
}


//------
// Post \
//---------------------------------------------------------------------------
// Description:
//		This method wraps the operating system dependant call to Post an
//  	event semaphore.
//
//	Parameters:
//				none
//
//	Returns:
//				Operating system status of post operation
//
int C_SEM_EVENT::Post( void )
{
	// Post the event semaphore
	return (int)DosPostEventSem( hSemaphore );
}


//----------------
// WaitIndefinite \
//---------------------------------------------------------------------------
// Description:
//		This method wraps the operating system dependant call to wait
//		indefinitely for the semaphore to be posted.
//
//	Parameters:
//				none
//
//	Returns:
//				Operating system status of wait operation
//
int C_SEM_EVENT::WaitIndefinite( void )
{
	// Post the event semaphore
	return (int)DosWaitEventSem( hSemaphore, SEM_INDEFINITE_WAIT );
}




