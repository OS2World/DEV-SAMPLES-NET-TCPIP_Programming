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
#include <threadpm.hpp>


//-------------
// Constructor \
//---------------------------------------------------------------------------
C_THREAD_PM::C_THREAD_PM( void ) : C_THREAD()
{
}


//-------------
// Constructor \
//---------------------------------------------------------------------------
// Description:
//		This method wraps the operating system dependant call to create a new
//  	thread of execution for a Presentation Manager.  To be able to send
//		PM messages, a thread in PM must create its own message queue.  This
//		constructor does this.
//
//	Parameters:
//		ThreadFunction		- Pointer to the desired thread function
//		StackSize			- Default stack size for the thread
//		pvData				- Pointer to any data passed to the thread function
//
//	Returns:
//				none
//
C_THREAD_PM::C_THREAD_PM( void (*ThreadFunction)(void *),
					unsigned int iStackSize, void *pvData )
					: C_THREAD( ThreadFunction, iStackSize, pvData )
{
}


//------------------
// InitializeThread \
//---------------------------------------------------------------------------
// Description:
//		This method wraps the operating system dependant calls to create a new
//  	message queue.  This should be called at the beginning of the thread
//		function.
//
//	Parameters:
//				none
//
//	Returns:
//				none
//
void C_THREAD_PM::InitializeThread( void )
{
	// Create a PM process for this thread
	hABThread = WinInitialize( 0 );
	hMQThread = WinCreateMsgQueue( hABThread, 0 );
}


//-----------------
// TerminateThread \
//---------------------------------------------------------------------------
// Description:
//		This method wraps the operating system dependant calls to destroy a
//  	thread's message queue.  This should be called at the end of the thread
//		function.
//
//	Parameters:
//				none
//
//	Returns:
//				none
//
void C_THREAD_PM::TerminateThread( void )
{
	// Terminate the thread
	WinDestroyMsgQueue( hMQThread );
	WinTerminate( hABThread );
}

