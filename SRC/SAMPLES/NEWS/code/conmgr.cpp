//------------
// CONMGR.CPP \
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
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <os2.h>

//-----------------
// NVCLASS Headers \
//---------------------------------------------------------------------------
#include <semev.hpp>

//-----------------
// Network Headers \
//---------------------------------------------------------------------------
#include "net.hpp"
#include "netnews.hpp"

//---------------------
// Application Headers \
//---------------------------------------------------------------------------
#include <conmgr.hpp>


//-------------
// Constructor \
//---------------------------------------------------------------------------
//
//	Description:
//		This constructor initializes a new instnace of the connection manager.
//
//	Parameters:
//		iConCount		- Number of server connections to create
//		szServer		- Address of the news server
//		iPort			- TCP/IP port number to use for connections
//
C_CONNECT_MGR::C_CONNECT_MGR( int iConCount, char *szServer, int iPort )
{
	// Create an event semaphore to track the state of the connection manager
	hSemConMgr.Create();
	hSemConMgr.Open();
	hSemConMgr.Post();

	// Initializa the connection manager
	Initialize( iConCount, szServer, iPort );
}

//-------------
// Constructor \
//---------------------------------------------------------------------------
C_CONNECT_MGR::C_CONNECT_MGR( void )
{
	// Create an event semaphore to track the state of the connection manager
	hSemConMgr.Create();
	hSemConMgr.Open();
	hSemConMgr.Post();
}

//------------
// Destructor \
//---------------------------------------------------------------------------
C_CONNECT_MGR::~C_CONNECT_MGR( void )
{
	int		iCtr;


	// Open each connection
	iCtr = iConnectionCount - 1;
	while(  iCtr >= 0 )
	{
		// Close the server connection
		(pxcConnect + iCtr )->Close();
		iCtr--;
	}

	// Get rid of the network object
	delete pxcConnect;
}


//------------
// Initialize \
//---------------------------------------------------------------------------
//
//	Description:
//		This  method establishes a connection instamce for each of the required
//		server connections and initialies each of them.
//
//	Parameters:
//		iConCount		- Number of server connections to create
//		szServer		- Address of the news server
//		iPort			- TCP/IP port number to use for connections
//
//	Returns:
//		void
//
void  C_CONNECT_MGR::Initialize( int iConCount, char *szServer, int iPort )
{
	int		iResult;
	int		iCtr;


	// Save the attributes
	iConnections = iConCount;
	iConnectionCount = 0;
	strcpy( szNewsServer, szServer );
	iNewsPort = iPort;

	// Create the specified number of connection instances
	pxcConnect = (C_CONNECT_NEWS *)new C_CONNECT_NEWS[iConnections];

	// Initialize each connection
	for( iCtr = 0; iCtr < MaxConnections(); iCtr++ )
	{
		// Initialize the news server connection
		(pxcConnect+iCtr)->Initialize( Server(), Port() );
	}


	// Open each connection
	for( iCtr = 0; iCtr < MaxConnections(); iCtr++ )
	{
		// Open the connection
		iResult = (pxcConnect+iCtr)->Open();
		if( iResult >= D_NET_OK )
		{
			// Say that we have a connection open
			IncrementConnectionCount();
		}

		// Indicate that the connection is initially idle
		(pxcConnect+iCtr)->Busy( 0 );
	}
}


//-------
// Close \
//---------------------------------------------------------------------------
//
//	Description:
//		This  method closes the specified news server connection.
//
//	Parameters:
//		iConnection		-  Connection to close
//
//	Returns:
//		void
//
void  C_CONNECT_MGR::Close( int iConnection )
{
	// Close the connection
	(pxcConnect+iConnection)->Close();

	// Indicate that it is not longer busy
	(pxcConnect+iConnection)->Busy( 0 );

	// Reduce the number of available connections
	if( iConnectionCount > 0 )
		iConnectionCount--;
}


//---------
// Connect \
//---------------------------------------------------------------------------
//
//	Description:
//		This method returns connection from the idle connection pool to the
//		caller.  The connection is marked as busy and the calling function is
//		responsible for disconnecting when its task is complete.
//
//	Parameters:
//		none
//
//	Returns:
//		int			- >=0 connection for use. <0 no connection available
//
int	C_CONNECT_MGR::Connect( void )
{
	int		iCtr;
	ULONG	lCount;


	// Wait for a free connection
	hSemConMgr.WaitIndefinite();
	hSemConMgr.Reset( &lCount );

	do {
		// Look at each connection
		for( iCtr = 0; iCtr < iConnections; iCtr++ )
		{
			// Is the connection busy?
			if( (pxcConnect+iCtr)->Socket() && !(pxcConnect+iCtr)->Busy() )
			{
				// Not busy, so we'll use it.  Mark as busy.
				(pxcConnect+iCtr)->Busy( 1 );

				// Return the connection number that was connected
				hSemConMgr.Post();
				return iCtr;
			}
		}

		// Wait around a while until a connection becomes free
		DosSleep( 500 );
	} while( 1 == 1 );


	// The program should never get here, but return something to keep the
	// compiler happy
	return -1;
}


//-----------------
// FreeConnections \
//---------------------------------------------------------------------------
//
//	Description:
//		This method returns the number of currently idle connections
//
//	Parameters:
//		none
//
//	Returns:
//		int			- the number of free connections.
//
int	C_CONNECT_MGR::FreeConnections( void )
{
	int		iCtr;
	int		iFreeCtr;


	iFreeCtr = 0;

	// Check each connection
	for( iCtr = 0; iCtr < iConnections; iCtr++ )
	{
		// If the connection is idle, count it.
		if( (pxcConnect+iCtr)->Socket() && !(pxcConnect+iCtr)->Busy() )
			iFreeCtr++;
	}

	// Return the number of free connections
	return iFreeCtr;
}


//------------
// Disconnect \
//---------------------------------------------------------------------------
//
//	Description:
//		This method returns the specified connection to the idle connection
//		pool for use by other processes.
//
//	Parameters:
//		iConnect		- Connection to return to the pool
//
//	Returns:
//		none
//
void C_CONNECT_MGR::Disconnect( int iConnect )
{
	// Mark the connection as idle
	(pxcConnect+iConnect)->Busy( 0 );
}


