#define INCL_DOS

#include <os2.h>
#include <stdio.h>
#include <process.h>

void _Optlink Thread1( void *pvData )
{
	int		iCtr;


	// Loop 5 times printing the count
	for( iCtr = 0; iCtr < 10; iCtr++)
	{
		printf( "Thread1:%d\n", iCtr );

		// Sleep for 10 msec
		DosSleep( 10 );
	}
}


void _Optlink Thread2( void *pvData )
{
	int		iCtr;


	// Loop 5 times printing the count
	for( iCtr = 0; iCtr < 10; iCtr++)
	{
		printf( "Thread2:%d\n", iCtr );

		// Sleep for 5 msec
		DosSleep( 5 );
	}
}

void main( void )
{
	PID		Thread1Pid;
	PID		Thread2Pid;


	printf( "Thread demo started\n" );

	// Start a couple of threads
#ifdef __BORLANDC__
	Thread1Pid = _beginthread( Thread1, 10000, 0 );
	Thread2Pid = _beginthread( Thread2, 10000, 0 );
#else
	Thread1Pid = _beginthread( Thread1, 0, 10000, 0 );
	Thread2Pid = _beginthread( Thread2, 0, 10000, 0 );
#endif

	// Don't exit until the threads have completed
	printf( "Waiting for threads to complete\n" );
	DosWaitThread( &Thread1Pid, DCWW_WAIT );
	DosWaitThread( &Thread2Pid, DCWW_WAIT );

	// See ya!
	printf( "Thread demo completed\n" );
}



