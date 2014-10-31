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
#include <process.h>

//-----------------
// NVCLASS Headers \
//---------------------------------------------------------------------------
#include <thread.hpp>


//-------------
// Constructor \
//---------------------------------------------------------------------------
C_THREAD::C_THREAD( void )
{
	hThreadID = 0;
}


//-------------
// Constructor \
//---------------------------------------------------------------------------
//
//	Parameters:
//		ThreadFunction		- Pointer to the desired thread function
//		StackSize			- Default stack size for the thread
//		pvData				- Pointer to any data passed to the thread function
//
//#ifdef __BORLANDC__
C_THREAD::C_THREAD( void (*ThreadFunction)(void *),
									unsigned int iStackSize, void *pvData )
//#else
//C_THREAD::C_THREAD( void (*ThreadFunction)(void *), void *pvStack,
//									unsigned int iStackSize, void *pvData )
//#endif
{
	// Call the create method to create a thread upon instantiation
	Create( ThreadFunction, iStackSize, pvData );
}


//--------
// Create \
//---------------------------------------------------------------------------
// Description:
//		This method wraps the operating system dependant call to create a new
//  	thread of execution.
//
//	Parameters:
//		ThreadFunction		- Pointer to the desired thread function
//		StackSize			- Default stack size for the thread
//		pvData				- Pointer to any data passed to the thread function
//
//	Returns:
//				none
//
void C_THREAD::Create( void *ThreadFunction,
									unsigned int iStackSize, void *pvData )
{
	// Save a pointer to the supplied data
	pvThreadData = pvData;

	// Create a new thread of execution
#ifdef __BORLANDC__
	hThreadID = _beginthread( (void (*)(void *))ThreadFunction, iStackSize, this );
#else
	hThreadID = _beginthread( (void (* _LNK_CONV)( void * ))ThreadFunction, 
								0, iStackSize, this );
#endif
}


//------
// Kill \
//---------------------------------------------------------------------------
// Description:
//		This method wraps the operating system dependent call to terminate
//		a thread.
//
//	Parameters:
//				none
//
//	Returns:
//				none
//
void C_THREAD::Kill( void )
{
	// If the thread has previously been started
	if( hThreadID )
	{
		// Kill the thread
		DosKillThread( hThreadID );
	}
}


//----------------
// WaitIndefinite \
//---------------------------------------------------------------------------
// Description:
//		This method wraps the operating system dependant call to wait for the
//		thread to complete it's execution.
//
//	Parameters:
//				none
//
//	Returns:
//				none
//
void C_THREAD::WaitIndefinite( void )
{
	// If the thread has previously been started
	if( hThreadID )
	{
		// Wait for the thread to complete
		DosWaitThread( &hThreadID, DCWW_WAIT );
	}
}

