//------------
// NETFTP.CPP \
//---------------------------------------------------------------------------
//
//	All information contained in this file is Copyright (c) 1994 by
//	NeoLogic Software Systems.  All Rights Reserved
//
//	This source code shall not be implemented, duplicated or used as the basis
//	for any product without prior written consent by NeoLogic Software Systems.
//
// Description:
//
//	This file contains the class definition for TCP/IP network interaction
//	specific to FTP. C_CONNECT_FTP
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
#include <ctype.h>
#include <os2.h>

//-----------------
// PMCLASS Headers \
//---------------------------------------------------------------------------
#include <app.hpp>
#include <window.hpp>
#include <winstd.hpp>
#include <winchild.hpp>
#include <dialog.hpp>
#include <edit.hpp>
#include <menu.hpp>
#include <status.hpp>
#include <button.hpp>
#include <tbar.hpp>
#include <mle.hpp>
#include <contain.hpp>
#include <log.hpp>


//------------------
// NETCLASS Headers \
//---------------------------------------------------------------------------
#include <net.hpp>
#include <netftp.hpp>


//-------------
// Constructor \
//---------------------------------------------------------------------------
//
// Description:
//		This constructor initializes the instnace of C_CONNECT_FTP.
//
//	Parameters:
//		szConnectServer		- Address of the news server
//		iConnectPort		- Port number to use for the connection
//		pxcConsole			- Option MLE console window object
//
C_CONNECT_FTP::C_CONNECT_FTP( char *szConnectServer,
			int iConnectPort, C_MLE *pxcConsole )
								: C_CONNECT( szConnectServer, iConnectPort )
{
	pxcMLE = pxcConsole;
	iHash = 0;

	// If an MLE control was specified, trun on hash marks
	if( pxcMLE )
		iHash = 1;
}


//------
// Open \
//---------------------------------------------------------------------------
//
//	Description:
//		This method opens the FTp connection with the server.  It uses
//		the C_CONNECT::Open() method, then adds some FTP specific connection
//		code.
//
//	Parameters:
//			none
//
//	Returns:
//			int			- D_NET_OK if Open was successful
//
int C_CONNECT_FTP::Open( void )
{
	char	szBuffer[1024];
	int		iResult;
	int		iError;
	int		iFlag;


	// Default to an error result
	iError = D_NET_CONNECT;

	// Call the parent to open the socket
	iResult = C_CONNECT::Open();
	if( iResult >= D_NET_OK )
	{
		iFlag = 1;
		setsockopt( Socket(), (short)SOL_SOCKET, SO_OOBINLINE,
										(char *)&iFlag, sizeof( iFlag ) );
		do {
			memset( szBuffer, 0, 1024 );
			Receive( szBuffer );

			if( atoi( szBuffer ) > 400 || strlen( szBuffer ) < 4 )
			{
				return 421;
			}
		} while( szBuffer[3] == '-' );

		// Connected OK
		return D_NET_OK;
	}

	// Connected OK
	return iError;
}


//-------
// Close \
//---------------------------------------------------------------------------
//
//	Description:
//		This method closes the FTP connection
//
//	Parameters:
//			none
//
//	Return:
//			none
//
void C_CONNECT_FTP::Close( void )
{
	// Send a quit command to end the conversation
	Send( "QUIT\r\n" );

	// Call the parent close to kill the socket
	C_CONNECT::Close();
}


//------
// Send \
//---------------------------------------------------------------------------
//
//	Description:
//		This method sends a command to the FTP server.  It uses the
//		C_CONNECT::Send() method but adds hash marks to the MLE, if available.
//
//	Parameters:
//		szCommand		- Command to send to the server
//
//	Return:
//		int				- Result of the send operation
//
int C_CONNECT_FTP::Send( char *szCommand )
{
	int		iResult;


	// Write command to the console window if it is present
	if( pxcMLE )
		pxcMLE->Insert( szCommand );

	// Send the command to the server
	iResult = C_CONNECT::Send( szCommand );

	return iResult;
}


//---------
// Receive \
//---------------------------------------------------------------------------
//
//	Description:
//		This method receives a string from the FTP server.  It uses the
//		C_CONNECT::Receive() method, but adds the returned string to the
//		MLE, if available.
//
//	Parameters:
//
//	Return:
//
//
void C_CONNECT_FTP::Receive( char *szBuffer )
{
	memset( szBuffer, 0, 256 );
	do {
		// Read a response from the FTP server
		C_CONNECT::Receive( szBuffer );

		// Display the line on the console if it is present
		if( pxcMLE )
		{
			pxcMLE->Insert( szBuffer );
			pxcMLE->Insert( "\n" );
		}

		// Repeat until we don't get a line continuation ie '230-'
	} while( ( strlen( szBuffer ) != 0 && !isdigit( *szBuffer ) ) ||
												*(szBuffer+3) == '-' );
}


//--------
// Accept \
//---------------------------------------------------------------------------
//
//	Description:
//		This method accepts a block of binary data from the server and
//		writes it to the specified file.  Data are retrieved via a data
//		connection initiated by the server.
//
//	Parameters:
//		iSocket		- Control socket
//		szFile		- pointer to out file where data will be written
//
//	Return:
//		int			- returns zero if successful
//
int C_CONNECT_FTP::Accept( int iSocket, char *szFile )
{
	char	*szBuffer;
	FILE	*hFile;
	int		iDataSocket;
	int		iResult;
	int		iLength;
	struct sockaddr_in	xtTempSocket;


	// Remove any old file that may be present
	DosForceDelete( (PSZ)szFile );

	// Accept a data socket from the server
	iLength = sizeof( struct sockaddr_in );
	iDataSocket = accept( iSocket, (sockaddr *)&xtTempSocket, &iLength );

	// Close the listener socket
	soclose( iSocket );
	if( iDataSocket < 0 )
		return iDataSocket;

	szBuffer = (char *)malloc( 8192 );

	// Read data into a file
	hFile = fopen( szFile, "w+b" );
	if( hFile )
	{
		do {
			// Receive a packet of information
			iResult = recv( (short)iDataSocket, szBuffer, 8192, 0 );
			if( iResult > 0 )
			{
				// Write the data to the output file
				fwrite( szBuffer, 1, iResult, hFile );
			}

			// If hash marking is on, then display hashes
			if( iHash && pxcMLE )
				pxcMLE->Insert( "#" );
		} while( iResult > 0 );
		fclose( hFile );
	}

	// If hash marking is on, then display a new line to end the hash marks
	if( iHash && pxcMLE )
		pxcMLE->Insert( "\n" );

	// Done with the data socket so dispose of it.
	soclose( iDataSocket );

	free( szBuffer );
	return 0;
}


//--------
// Listen \
//---------------------------------------------------------------------------
//
//	Description:
//		This method listens for a data connection from the server and, if found,
//		binds this connection to a socket so communication can occur.
//
//	Parameters:
//			none
//
//	Return:
//			int		- Data socket >0 if successful
//
int C_CONNECT_FTP::Listen( void )
{
	char	*a;
	char	*p;
	char	szString[1024];
	int		iSocket;
	int		iLength;
	int		iFlag = 1;
	struct sockaddr_in	xtTempSocket;
	struct sockaddr_in	xtListenSocket;


	// Connect to the news reader port
	xtListenSocket.sin_family = AF_INET;
	xtListenSocket.sin_port = htons( 0 );
	xtListenSocket.sin_addr.s_addr = 0;

	// Open a socket for this connection
	iSocket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
	if( iSocket == -1 )
		return D_NET_SOCKET;

	// Reuse any previous port that may already exist
	if( setsockopt( iSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&iFlag,
												sizeof( iFlag ) ) < 0 )
	{
		return D_NET_CONNECT;
	}

	// Bind the listener socket to a data connection
	if( bind( iSocket, (struct sockaddr *)&xtListenSocket,
									sizeof( struct sockaddr_in ) ) < 0 )
	{
		return -1;
	}

	// Get the name of the listener socket and save it
	iLength = sizeof( struct sockaddr_in );
	if( getsockname( iSocket, (struct sockaddr *)&xtListenSocket,
													&iLength ) < 0 )
	{
		return D_NET_CONNECT;
	}

	// Listen for a data connection request from the server
	if( listen( iSocket, 1 ) != 0 )
	{
		soclose( iSocket );
	}

	// Inform remote host about the port we are using
	iLength = sizeof( struct sockaddr_in );
	getsockname( Socket(), (struct sockaddr *)&xtTempSocket, &iLength );
	a = (char *)&xtTempSocket.sin_addr;
	p = (char *)&xtListenSocket.sin_port;
	sprintf( szString, "PORT %d,%d,%d,%d,%d,%d\r\n",
							(UCHAR)a[0], (UCHAR)a[1], (UCHAR)a[2],
							(UCHAR)a[3], (UCHAR)p[0], (UCHAR)p[1] );
	Send( szString );
	Receive( szString );

	// Return the listen socket to the caller
	return iSocket;
}


//-------------
// SocketClose \
//---------------------------------------------------------------------------
//
//	Description:
//		This method closes a specified data socket
//
//	Parameters:
//			iSocket		- Data socket to close
//
//	Return:
//			int			- Always returns D_NET_OK
//
int C_CONNECT_FTP::SocketClose( int iSocket )
{
	soclose( iSocket );
	return D_NET_OK;
}


//---------
// SendOOB \
//---------------------------------------------------------------------------
//
//	Description:
//		This method sends an out-of-band command to the FTP server.
//
//	Parameters:
//		szCommand		- OOB command to issue
//
//	Return:
//		int				- Result of send command
//
int C_CONNECT_FTP::SendOOB( char *szCommand )
{
	return send( Socket(), szCommand, (short)strlen( szCommand ), MSG_OOB );
}


//-----
// Put \
//---------------------------------------------------------------------------
//
//	Description:
//		This method transfers a file to the server using the specified
//		data socket.
//
//	Parameters:
//		iSocket		- Data socket to use for transmission
//		szFile		- Name of file to transfer
//
//	Return:
//		int			- 0 if successful
//
int C_CONNECT_FTP::Put( int iSocket, char *szFile )
{
	char	*szBuffer;
	FILE	*hFile;
	int		iDataSocket;
	int		iResult;
	int	iLength;
	struct sockaddr_in	xtTempSocket;


	// Accept a data connection from the server
	iLength = sizeof( struct sockaddr_in );
	iDataSocket = accept( iSocket, (sockaddr *)&xtTempSocket, &iLength );

	// Close the listener socket
	soclose( iSocket );
	if( iDataSocket < 0 )
		return iDataSocket;

	szBuffer = (char *)malloc( 8192 );

	// Read data into a file
	hFile = fopen( szFile, "rb" );
	if( hFile )
	{
		iResult = 1;
		while( iResult > 0 )
		{
			iResult = fread( szBuffer, 1, 8192, hFile );
			send( (short)iDataSocket, szBuffer, (short)iResult, 0 );

			// If hash marking is on, then display hashes
			if( iHash && pxcMLE )
				pxcMLE->Insert( "#" );
		}
		fclose( hFile );
	}

	// If hash marking is on, then display a new line
	if( iHash && pxcMLE )
		pxcMLE->Insert( "\n" );

	free( szBuffer );

	soclose( iDataSocket );
	return 0;
}


//------------
// FTPCommand \
//---------------------------------------------------------------------------
//
//	Description:
//		This method sends an FTP command to the server and retrieves the
//		response string.
//
//	Parameters:
//		szCommand		- Command to send
//
//	Return:
//		none
//
void C_CONNECT_FTP::FTPCommand( char *szCommand )
{
	// Send an FTP command to the server
	Send( szCommand );

	// Get the command reply from the server
	Receive( szCommand );
}


//------
// SYST \
//---------------------------------------------------------------------------
//
//	Description:
//		This method sends a SYST command to determine the server type
//
//	Parameters:
//		none
//
//	Return:
//		int		- Result of the command
//
int C_CONNECT_FTP::SYST( void )
{
	char	szString[1024];


	// Send the SYST command and get the server response
	sprintf( szString, "SYST\r\n" );
	FTPCommand( szString );

	return atoi( szString );
}


//------
// SITE \
//---------------------------------------------------------------------------
//
//	Description:
//		This method sends a SITE command to issue server specific command
//
//	Parameters:
//		szText	- Specific server command to send
//
//	Return:
//		int		- Result of the command
//
int C_CONNECT_FTP::SITE( char *szText )
{
	char	szString[1024];

	sprintf( szString, "SITE %s\r\n", szText );
	FTPCommand( szString );

	return atoi( szString );
}


//------
// ACCT \
//---------------------------------------------------------------------------
//
//	Description:
//		This method sends an ACCT command to issue any required account data.
//
//	Parameters:
//		szText	- Account information
//
//	Return:
//		int		- Result of the command
//
int C_CONNECT_FTP::ACCT( char *szText )
{
	char	szString[1024];

	sprintf( szString, "ACCT %s\r\n", szText );
	FTPCommand( szString );

	return atoi( szString );
}


//------
// USER \
//---------------------------------------------------------------------------
//
//	Description:
//		This method sends a USER command to issue login username.
//
//	Parameters:
//		szText	- Username
//
//	Return:
//		int		- Result of the command
//
int C_CONNECT_FTP::USER( char *szText )
{
	char	szString[1024];

	sprintf( szString, "USER %s\r\n", szText );
	FTPCommand( szString );
	return atoi( szString );
}


//------
// PASS \
//---------------------------------------------------------------------------
//
//	Description:
//		This method sends a PASS command to issue login password.
//
//	Parameters:
//		szText	- Password
//
//	Return:
//		int		- Result of the command
//
int C_CONNECT_FTP::PASS( char *szText )
{
	char	szString[1024];

	sprintf( szString, "PASS %s\r\n", szText );
	FTPCommand( szString );
	return atoi( szString );
}


//------
// TYPE \
//---------------------------------------------------------------------------
//
//	Description:
//		This method sends a TYPE command to set the transfer mode.
//
//	Parameters:
//		szType	- Transfer type (I,A,E,L)
//
//	Return:
//		int		- Result of the command
//
int C_CONNECT_FTP::TYPE( char *szType )
{
	char	szString[1024];


	// Set the transfer type
	sprintf( szString, "TYPE %s\r\n", szType );
	FTPCommand( szString );
	return atoi( szString );
}


//-----
// PWD \
//---------------------------------------------------------------------------
//
//	Description:
//		This method sends a PWD command to return the current directory.
//
//	Parameters:
//		szDir	- Returned directory
//
//	Return:
//		int		- Result of the command
//
int C_CONNECT_FTP::PWD( char *szDir )
{
	char	szString[1024];


	// Send the PWD command and get the server response
	sprintf( szString, "PWD\r\n" );
	FTPCommand( szString );

	// Return the working directory to the caller
	strcpy( szDir, "" );
	if( atoi( szString ) >= 250 && atoi( szString ) < 300 )
	{
		if( strstr( szString, "\"" ) )
		{
			strcpy( szDir, strstr( szString, "\"" ) + 1 );
			if( strstr( szDir, "\"" ) )
				*strstr( szDir, "\"" ) = 0;
		}

		// If the directory wasn't located then it may be enclosed in [] chars
		if( strlen( szDir ) == 0 && strstr( szString, "[" ) )
		{
			strcpy( szDir, szString + 4 );
			if( strstr( szDir, "]" ) )
				*(strstr( szDir, "]" ) + 1) = 0;
		}
	}

	return atoi( szString );
}


//-----
// CWD \
//---------------------------------------------------------------------------
//
//	Description:
//		This method sends a CWD command to set the current work directory.
//
//	Parameters:
//		szDir	- New working directory
//
//	Return:
//		int		- Result of the command
//
int C_CONNECT_FTP::CWD( char *szDir )
{
	char	szString[1024];

	sprintf( szString, "CWD %s\r\n", szDir );
	FTPCommand( szString );
	return atoi( szString );
}


//------
// RMD \
//---------------------------------------------------------------------------
//
//	Description:
//		This method sends an RMD command to remove a directory.
//
//	Parameters:
//		szFile	- Directory to remove
//
//	Return:
//		int		- Result of the command
//
int C_CONNECT_FTP::RMD( char *szFile )
{
	char	szString[1024];

	sprintf( szString, "RMD %s\r\n", szFile );
	FTPCommand( szString );
	return atoi( szString );
}


//-----
// MKD \
//---------------------------------------------------------------------------
//
//	Description:
//		This method sends an MKD command to create a directory.
//
//	Parameters:
//		szFile	- Directory to create
//
//	Return:
//		int		- Result of the command
//
int C_CONNECT_FTP::MKD( char *szFile )
{
	char	szString[1024];

	sprintf( szString, "MKD %s\r\n", szFile );
	FTPCommand( szString );
	return atoi( szString );
}


//------
// DELE \
//---------------------------------------------------------------------------
//
//	Description:
//		This method sends a DELE command to remove a file.
//
//	Parameters:
//		szFile	- File to remove
//
//	Return:
//		int		- Result of the command
//
int C_CONNECT_FTP::DELE( char *szFile )
{
	char	szString[1024];

	sprintf( szString, "DELE %s\r\n", szFile );
	FTPCommand( szString );
	return atoi( szString );
}


//-----
// DIR \
//---------------------------------------------------------------------------
//
//	Description:
//		This method fetches a directory listing for the current directory
//		from the server.
//
//	Parameters:
//		szWildCards	- Any filename wildcards used for the listing
//		szFile		- Output file for the listing
//
//	Return:
//		int		- 0 if successful
//
int C_CONNECT_FTP::DIR( char *szWildcards, char *szFile )
{
	char	szString[1024];
	int		iListener;
	FILE	*hFile;


	// List for a reply connection
	iListener = Listen();

	// Send the command to the server
	if( !szWildcards || strlen( szWildcards ) == 0 )
		sprintf( szString, "LIST\r\n" );
	else
		sprintf( szString, "LIST %s\r\n", szWildcards );
	FTPCommand( szString );

	// Flag inaccessible directories
	if( atoi( szString ) > 500 || atoi( szString ) == 425 )
	{
		SocketClose( iListener );
		return atoi( szString );
	}

	if( Accept( iListener, szFile ) >= 0 )
	{
		// Display the directory if the MLE window is defined
		if( pxcMLE )
		{
			hFile = fopen( szFile, "r" );
			if( hFile )
			{
				// Display each line of the directory in the console window
				while( !feof( hFile ) && fgets( szString, 1024, hFile ) )
					pxcMLE->Insert( szString );

				fclose( hFile );

				DosForceDelete( szFile );
			}
		}

		Receive( szString );
		SocketClose( iListener );
		return atoi( szString );
	}
	SocketClose( iListener );
	return 0;
}


//------
// ABOR \
//---------------------------------------------------------------------------
//
//	Description:
// 		This method implements the transfer abort functionality speicifed
// 		in RFC 959.
//
//	Parameters:
//		none
//
//	Return:
//		int		- Result of the command
//
int C_CONNECT_FTP::ABOR( void )
{
	SHORT	sock_arr[5];
	char	szString[1024];
	int		iResult;


	// Send the abort sequence
	sprintf( szString,"%c%c\r\n",255,244);
	Send( szString );
	sprintf( szString,"%c%c",255,242);
	SendOOB( szString );
	Send( "ABOR\r\n" );

	// Wait for the server to respond
	sock_arr[0] = (short)Socket();
	do {
		iResult = select( (int *)sock_arr, 1, 0, 0, 18000L );
		C_CONNECT::Receive( szString );
	} while( iResult > 0 && strlen( szString ) == 0 );

	return 0;
}


//------
// RETR \
//---------------------------------------------------------------------------
//
//	Description:
//		This method sends a RETR command to retrieve a file.
//
//	Parameters:
//		szSrcFile	- Source file to retrieve
//		szDstFile	- Local file location where retrieval will be written
//
//	Return:
//		int		- Result of the command
//
int C_CONNECT_FTP::RETR( char *szSrcFile, char *szDstFile )
{
	char	szString[1024];
	int		iListener;


	iListener = Listen();
	sprintf( szString, "RETR %s\r\n", szSrcFile );
	FTPCommand( szString );

	if( atoi( szString ) > 500 )
		return 550;

	if( Accept( iListener, szDstFile ) >= 0 )
	{
		Receive( szString );

		SocketClose( iListener );
		return atoi( szString );
	}
	SocketClose( iListener );
	return 0;
}


//------
// STOR \
//---------------------------------------------------------------------------
//
//	Description:
//		This method sends a STOR command to send a file to the server.
//
//	Parameters:
//		szSrcFile	- Source file to send
//		szDstFile	- Remote file location where the file will be written
//
//	Return:
//		int		- Result of the command
//
int C_CONNECT_FTP::STOR( char *szSrcFile, char *szDstFile )
{
	char	szString[1024];
	int		iListener;


	iListener = Listen();
	sprintf( szString, "STOR %s\r\n", szDstFile );
	FTPCommand( szString );

	if( atoi( szString ) > 500 )
		return 550;

	if( Put( iListener, szSrcFile ) >= 0 )
	{
		Receive( szString );
		SocketClose( iListener );
		return atoi( szString );
	}
	SocketClose( iListener );
	return 0;
}


//------
// QUIT \
//---------------------------------------------------------------------------
//
//	Description:
//		This method sends a QUIT command to end the conversation.
//
//	Parameters:
//		none
//
//	Return:
//		int		- Result of the command
//
int C_CONNECT_FTP::QUIT( void )
{
	char	szString[1024];


	// Send the QUIT command and get the server response
	sprintf( szString, "QUIT\r\n" );
	FTPCommand( szString );

	return atoi( szString );
}


//------
// NOOP \
//---------------------------------------------------------------------------
//
//	Description:
//		This method sends a NOOP command to perform "no operation"
//
//	Parameters:
//		none
//
//	Return:
//		int		- Result of the command
//
int C_CONNECT_FTP::NOOP(  void )
{
	char	szString[1024];


	// Send the NOOP command and get the server response
	sprintf( szString, "NOOP\r\n" );
	FTPCommand( szString );

	return atoi( szString );
}


