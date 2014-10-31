//-------
// C_NET \
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
//	This file contains the methods to describe the C_NET class.  C_NET 
//	provides an interface to the low level TCP/IP socket interface in OS/2.
//  This class is the basis for all other network classes.
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

//------------------
// NETCLASS Headers \
//---------------------------------------------------------------------------
#include <net.hpp>


//-------------
// Constructor \
//---------------------------------------------------------------------------
//
//	Parameters:
//		szConnectServer		- address of server to cinnect to
//		iConnectPort		- Port number used for this connection
//
//	Returns:
//				none
//
C_CONNECT::C_CONNECT( char *szConnectServer, int iConnectPort )
{
	// Transfer parameter data into class attributes
	Initialize( szConnectServer, iConnectPort );
}


//------------
// Initialize \
//---------------------------------------------------------------------------
// Description:
//		This method is called to initialize the required class attributes.
//
//	Parameters:
//		szConnectServer		- Pointer to domain name or IP string of server
//		iConnectPort		- TCP/IP port to be used for this connection
//
//	Returns:
//				none
//
void C_CONNECT::Initialize( char *szConnectServer, int iConnectPort )
{
	// Transfer parameter data into class attributes
	strcpy( szServer, szConnectServer );
	iPort = iConnectPort;

	// Initialize the network read buffers
	strcpy( szNetBuffer, "" );
	memset( szNetBuffer, 0, D_NET_BUFFER + 1 );
	szNetBufferPtr = szNetBuffer;

	// Initialize the socket for communications
	pxtProtocol = NULL;

	// Initialize the TCP/IP socket interface
	sock_init();

	// Default the socket number
	iSocket = 0;
}


//----------
// FindHost \
//---------------------------------------------------------------------------
// Description:
//		This method attempts to build the host portion of the socket 
//		structure.  The server address attribute must previously be
//		set.
//
//	Parameters:
//		none
//
//	Returns:
//      		D_NET_HOST		- Host definition error
//              D_NET_SOCKET	- Could not get a socket
//				D_NET_CONNECT	- Error during connection
//				D_NET_OK		- Connection established successfully
//
int C_CONNECT::FindHost( void )
{
	struct hostent		*pxtHost;


	// Initialize the socket structure
	bzero( &xtSocket, sizeof( struct sockaddr_in ) );
	xtSocket.sin_addr.s_addr = 0;
	xtSocket.sin_family = AF_INET;

	// Try to resolve the INET address
	xtSocket.sin_addr.s_addr = inet_addr( szServer );
	if( (LONG)xtSocket.sin_addr.s_addr == -1 )
	{
		// Try to resolve the server by name
		pxtHost = gethostbyname( szServer );
		if( pxtHost == NULL )
		{
			// Couldn't resolve the server
			return D_NET_HOST;
		}

		// Copy the host data into the socket structure
		bcopy( pxtHost->h_addr, &xtSocket.sin_addr, pxtHost->h_length );
	}

	// Host was initialized correctly
	return D_NET_OK;
}


//----------
// Protocol \
//---------------------------------------------------------------------------
// Description:
//		This method permits the caller to specify a protocol for the
//		connection. (i.e. "icmp" )
//
//	Parameters:
//		szProtocol		- protocol definition string
//
//	Returns:
//      		D_NET_PROTOCOL	- Error setting protocol
//				D_NET_OK		- Connection established successfully
//
int C_CONNECT::Protocol( char *szProtocol )
{
	// Try to set the selected protocol
	pxtProtocol = getprotobyname( szProtocol );
	if( !pxtProtocol )
	{
		// Could find the specified protcol
		return D_NET_PROTOCOL;
	}

	// Protocol was set correctly
	return D_NET_OK;
}


//--------------
// StreamSocket \
//---------------------------------------------------------------------------
// Description:
//		This method set the socket interface to stream.
//
//	Parameters:
//				none
//
//	Returns:
//      		D_NET_SOCKET	- Error selecting stream mode
//				D_NET_OK	- Connection established successfully
//
int C_CONNECT::StreamSocket( void )
{
	// Open a socket for this streamed connection
	iSocket = socket( AF_INET, SOCK_STREAM, 0 );
	if( iSocket == -1 )
	{
		iSocket = 0;
		return D_NET_SOCKET;
	}

	return D_NET_OK;
}


//-----------
// RawSocket \
//---------------------------------------------------------------------------
// Description:
//		This method sets the socket interface to raw, which is typically
//		used for low level data transfers such as Ping.
//
//	Parameters:
//				none
//
//	Returns:
//      		D_NET_SOCKET	- Error selecting raw interface
//				D_NET_OK	- Connection established successfully.
//
int C_CONNECT::RawSocket( void )
{
	// Make sure a protocol has been defined
	if( !pxtProtocol )
		D_NET_PROTOCOL;

	// Create a socket for this raw connection
	iSocket = socket( AF_INET, SOCK_RAW, pxtProtocol->p_proto );
	if( iSocket == -1 )
	{
		// Failed to set raw socket
		iSocket = 0;
		return D_NET_SOCKET;
	}

	return D_NET_OK;
}


//------
// Open \
//---------------------------------------------------------------------------
// Description:
//		This method performs basic open functionality common to many
//		of the TCP/IP protocols.  It determines the host data and creates
//		streaming data socket.
//
//	Parameters:
//				none
//
//	Returns:
//      		D_NET_SOCKET	- Error selecting stream mode
//				D_NET_OK		- Connection established successfully
//
int C_CONNECT::Open( void )
{
	int		iResult;


	// Get the host data for this connection
	iResult = FindHost();
	if( iResult != D_NET_OK )
	{
		// Return any host errors
		return iResult;
	}

	// Select streaming socket
	iResult = StreamSocket();
	if( iResult != D_NET_OK )
	{
		// Return any socket setting errors
		return iResult;
	}

	// Connect to the correct port
	xtSocket.sin_port = htons( iPort );

	// Connect the socket
	if( connect( iSocket, (sockaddr *)&xtSocket, sizeof( xtSocket ) ) < 0 )
	{
		// Return any connection errors
		iSocket = 0;
		return D_NET_CONNECT;
	}

	// Connected OK
	return D_NET_OK;
}


//-------
// Close \
//---------------------------------------------------------------------------
// Description:
//		This method closes the socket associated with the current
//		connection.  This method should be called to terminate a
//		socket connection properly.
//
//	Parameters:
//				none
//
//	Returns:
//      		none
//
void C_CONNECT::Close( void )
{
	// Shut down the connection
	shutdown( iSocket, 2 );

	// Close the socket
	soclose( iSocket );
}


//------
// Send \
//---------------------------------------------------------------------------
// Description:
//		This method sends the supplied string to the TCP/IP connection.
//
//	Parameters:
//		szCommand		- The command string to be issued
//
//	Returns:
//				The number of characters sent.
//
int C_CONNECT::Send( char *szCommand )
{
	return send( iSocket, szCommand, (short)strlen( szCommand ), 0 );
}


//------
// SendTo \
//---------------------------------------------------------------------------
// Description:
//		This method wraps the standard TCP/IP sendto() API.
//
//	Parameters:
//		pbyBuffer		- Pointer to buffer to be sent
//		sLength		- Length of the buffer
//
//	Returns:
//				The number of characters sent
//
int C_CONNECT::SendTo( char *pbyBuffer, short sLength )
{
	return sendto( iSocket, pbyBuffer, sLength, 0,
				(struct sockaddr *)&xtSocket, sizeof( struct sockaddr_in ) );
}


//-------------
// ReceiveFrom \
//---------------------------------------------------------------------------
// Description:
//		This method wraps the standard TCP/IP receivefrom() API.
//
//	Parameters:
//		pbyBuffer		- Pointer to buffer where received data will be written
//		sLength		- Size of the buffer
//		pxsFrom		- Sockt from which data will be received
//
//	Returns:
//				The number of characters received or -1 for an error
//
int C_CONNECT::ReceiveFrom( char *pbyBuffer, short sLength,
								struct sockaddr_in *pxsFrom )
{
	int		iSize;

	iSize = sizeof( struct sockaddr_in );
	return recvfrom( iSocket, pbyBuffer, sLength, 0,
								(struct sockaddr *)pxsFrom, &iSize );
}


//---------------
// ReceiveBuffer \
//---------------------------------------------------------------------------
// Description:
//		This method receives a specified maximum number of characters from
//		TCP/IP connection and stores them in the buffer pointed to by szBuffer.
//
//	Parameters:
//		szBuffer		- Target buffer for the received characters
//		iSize			- Maximum number of characters that the buffer can hold
//
//	Returns:
//				The number of characters received.
//
int C_CONNECT::ReceiveBuffer( char *szBuffer, int iSize )
{
	int		iResult;


	iResult = recv( iSocket, szBuffer, iSize, 0 );

	return iResult;
}


//---------
// Receive \
//---------------------------------------------------------------------------
// Description:
//		This method will receive one carriage return delimted line from the
//		TCP/IP network buffer.  Upon detection, the carriage control is removed
//		and the buffer is NULL terminated.
//
//	Parameters:
//		szBuffer		- Pointer to target area for the received line.
//
//	Returns:
//				none
//
void C_CONNECT::Receive( char *szBuffer )
{
	char	szString[1025];
	int		iResult;


	// If there are no more CR-LFs in the buffer, we must have more data
	// to load
	iResult = 0;
	memset( szBuffer, 0, 1024 );

	if( szNetBufferPtr == 0 )
	{
		memset( szNetBuffer, 0, D_NET_BUFFER + 1 );
		szNetBufferPtr = szNetBuffer;
	}

	// If there are no more complete lines, read on in from the netwrok
	if( !strstr( szNetBuffer, "\r" ) )
	{
		// Loop until we have at least one complete line
		do {
			// Get the next chunk of data from the server
			iResult = ReceiveBuffer( szNetBuffer + strlen( szNetBuffer ),
									D_NET_BUFFER - strlen( szNetBuffer ) );
		} while( iResult > 0 && ( !strstr( szNetBuffer, "\r" )
        								|| !strstr( szNetBuffer, "\n" ) ) );
	}

	if( strstr( szNetBuffer, "\r" )  || strstr( szNetBuffer, "\n" ) )
    {
		// Copy the line out of the network buffer
		strncpy( szString, szNetBuffer, 1024 );

		// Eliminate the line termination
		if( strstr( szString, "\r" ) )
			*strstr( szString, "\r" ) = 0;
		if( strstr( szString, "\n" ) )
			*strstr( szString, "\n" ) = 0;

		// Chop the line out of the buffer and shove everything over
		memmove( szNetBuffer, szNetBuffer + strlen( szString ),
			        				D_NET_BUFFER - strlen( szString ) + 3 );

		// Get rid of the line termination
		if( *szNetBuffer == '\r' )
			memmove( szNetBuffer, szNetBuffer + 1, D_NET_BUFFER );
		if( *szNetBuffer == '\n' )
			memmove( szNetBuffer, szNetBuffer + 1, D_NET_BUFFER );

		memset( szNetBuffer + strlen( szNetBuffer ), 0,
									D_NET_BUFFER - strlen( szNetBuffer ) );

		strcpy( szBuffer, szString );
	}
}


//----------
// LoadFile \
//---------------------------------------------------------------------------
// Description:
//		This method will read until a \r\n.\r\n character sequence is detected in
//		the data in the specified text file.  Many decendant classed will need
//		to perform this operation for command processing, so the functionality
//		has been added to the base class.
//		This method will receive one carriage return delimted line from the
//		TCP/IP network buffer.  Upon detection, the carriage control is removed
//		and the buffer is NULL terminated.
//		If the specified file exists, it will be overwritten.
//
// Last Revision Date:
//						94-Oct-01
//
//	Parameters:
//		szFilename		- Pointer to filename where data will be written
//
void C_CONNECT::LoadFile( char *szFilename )
{
	char	*szBuffer;
	FILE	*hFile;
	int		iFlag;


	// Create the specified file
	hFile = fopen( szFilename, "w" );
	iFlag = 0;
	szBuffer = (char *)malloc( 2048 );

	// Loop until an end of data sequence has been detected
	while( !iFlag )
	{
		// Get a line from the network
		Receive( szBuffer );

		// If the line is an end of buffer marker, flag it for termination
		if( strcmp( szBuffer, "." ) == 0 )
		{
			iFlag = 1;
		}
		else
		{
			// Write the line to the output file
			fprintf( hFile, "%s\n", szBuffer );
		}
	}
	fclose( hFile );

	free( szBuffer );
}

