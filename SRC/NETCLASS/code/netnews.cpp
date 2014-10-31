//-------------
// NETNEWS.CPP \
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
//	This file contains the class methods for TCP/IP network news interaction.
//
//---------------------------------------------------------------------------

//---------------
// Include Files \
//---------------------------------------------------------------------------
#define	INCL_DOS
#define	INCL_WIN
#define	INCL_GDI

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <os2.h>

// NETCLASS DLL Headers
#include <net.hpp>
#include <netnews.hpp>

//----------------
// C_CONNECT_NEWS \
//---------------------------------------------------------------------------
// Description:
//		This constructor creates an instance of the C_CONNECT_NEWS class.
//
//	Parameters:
//		szConnectServer		- Pointer to domain name or IP string of server
//		iConnectPort		- TCP/IP port to be used for this connection
//
//	Returns:
//				none
//
C_CONNECT_NEWS::C_CONNECT_NEWS( char *szConnectServer, int iConnectPort )
						: C_CONNECT( szConnectServer, iConnectPort )
{
	// Default constructor
}


//--------
// Open() \
//---------------------------------------------------------------------------
// Description:
//		This method opens the News socket for communications.  It reuses the
//		current C_NET::Open() code, then reads the server response string
//		from the news server.
//
//	Parameters:
//				none
//
//	Returns:
//				none
//
int C_CONNECT_NEWS::Open( void )
{
	char	szBuffer[1024];
	int		iResult;
	int		iError;


	// Default to an error result
	iError = D_NET_CONNECT;

	// Call the parent to open the socket
	iResult = C_CONNECT::Open();
	if( iResult >= D_NET_OK )
	{
		// Read the response from the server
		Receive( szBuffer );

		// Get the error result code
		iError = atoi( szBuffer );

		// If there was an error, flag it
		if( iError != 200 && iError != 201 )
		{
			iSocket = 0;
			return D_NET_CONNECT;
		}
	}

	// Connected OK
	return iError;
}


//---------
// Close() \
//---------------------------------------------------------------------------
// Description:
//		This method closes the News socket after communication is completed.
//		It send a QUIT command to the server to inform it of the shutdown
//		then calls the parent Close().
//
//	Parameters:
//				none
//
//	Returns:
//				none
//
void C_CONNECT_NEWS::Close( void )
{
	Send( "QUIT\r\n" );

	// Call the parent close to kill the socket
	C_CONNECT::Close();
}


//------------
// OpenPost() \
//---------------------------------------------------------------------------
// Description:
//		This method initiates a message posting.  This is called any time
//		a new article is being posted to USENET.
//
//	Parameters:
//				none
//
//	Returns:
//				none
//
int C_CONNECT_NEWS::OpenPost( void )
{
	char	szBuffer[1024];


	// Send the group command
	sprintf( szBuffer, "post\r\n" );
	Send( szBuffer );
	Receive( szBuffer );

	// Process the response
	return atoi( szBuffer );
}


//-------------
// ClosePost() \
//---------------------------------------------------------------------------
// Description:
//		This method is sent at the end of a posting operation.  It sends the "."
//		terminator character and waits for a posting response from the
//		server.
//
//	Parameters:
//				none
//
//	Returns:
//				none
//
int C_CONNECT_NEWS::ClosePost( char *szResponse )
{
	char	szBuffer[1024];


	// Send the group command
	sprintf( szBuffer, "\r\n.\r\n" );
	Send( szBuffer );
	Receive( szBuffer );

	memset( szResponse, 0, 256 );
	strncpy( szResponse, szBuffer, 255 );

	// Process the response
	return atoi( szBuffer );
}


//--------
// List() \
//---------------------------------------------------------------------------
// Description:
//		This method retrieves a list of available news groups from the server
//		into the specified file.  The file will be created or overwritten if
//		it already exists.
//
//	Parameters:
//				szFilename	- File to which the list will be written
//
//	Returns:
//				none
//
int C_CONNECT_NEWS::List( char *szFilename )
{
	char	szBuffer[1024];
	int		iResult;


	// Request the article
	sprintf( szBuffer, "list\r\n" );
	Send( szBuffer );
	Receive( szBuffer );

	// Get the response back from the server
	iResult = atoi( szBuffer );

	// If there was no error, load the response file
	if( iResult < 400 && iResult > 0 )
		LoadFile( szFilename );

	return iResult;
}


//-----------------
// ListNewsGroups() \
//---------------------------------------------------------------------------
// Description:
//		This method retrieves a list of descriptions for news groups from the
//		server into the specified file.  The file will be created or 
//		overwritten if it already exists.
//
//	Parameters:
//				szFilename	- File to which the list will be written 
//
//	Returns:
//				none
//
int C_CONNECT_NEWS::ListNewsGroups( char *szFilename )
{
	char	szBuffer[1024];
	int		iResult;


	// Request the article
	sprintf( szBuffer, "list newsgroups\r\n" );
	Send( szBuffer );
	Receive( szBuffer );

	// Get the response back from the server
	iResult = atoi( szBuffer );

	// If there was no error, load the response file
	if( iResult < 400 && iResult > 0 )
		LoadFile( szFilename );

	return iResult;
}


//-----------------
// ListNewGroups() \
//---------------------------------------------------------------------------
// Description:
//		This method retrieves a list of new news groups since a specified
//		date and time from the  server.  Data is written into the
//		specified file.  The file will be created or overwritten if it 
//		already exists.
//
//	Parameters:
//				szDate		- Date/time string to use as a starting point
//				szFilename	- File to which the list will be written
//
//	Returns:
//				none
//
int C_CONNECT_NEWS::ListNewGroups( char *szDate, char *szFilename )
{
	char	szBuffer[1024];
	int		iResult;


	// Request the article
	sprintf( szBuffer, "newgroups %s\r\n", szDate );
	Send( szBuffer );
	Receive( szBuffer );

	// Get the response back from the server
	iResult = atoi( szBuffer );

	// If there was no error, load the response file
	if( iResult < 400 && iResult > 0 )
		LoadFile( szFilename );

	return iResult;
}


//------------
// Overview() \
//---------------------------------------------------------------------------
// Description:
//		This method retrieves an article overview from the server and writes
//		to the specified file name.  lStart and lEnd specify the range of
//		articles to overview.
//		Note: the overview command is not supported on all servers.
//
//	Parameters:
//				lStart		- starting message number for overview
//				lEnd		- starting message number for overview
//				szFilename	- File to which the overview will be written
//
//	Returns:
//				none
//
int C_CONNECT_NEWS::Overview( ULONG lStart, ULONG lEnd, char *szFilename )
{
	char	szBuffer[1024];
	int		iResult;


	// Request the article
	sprintf( szBuffer, "xover %ld-%ld\r\n", lStart, lEnd );
	Send( szBuffer );
	Receive( szBuffer );

	// Get the response back from the server
	iResult = atoi( szBuffer );

	// If there was no error, load the response file
	if( iResult < 400 && iResult > 0 )
		LoadFile( szFilename );

	return iResult;
}


//---------
// Group() \
//---------------------------------------------------------------------------
// Description:
//		This method sends a GROUP command to the news server to select
//		a new group.  The server returns the first and last article numbers
//		in the group.
//
//	Parameters:
//				szGroup			- News group to select
//				plFirstArticle	- pointer to First article number returned
//				plLastArticle	- Pointer to Last article number returned
//
//	Returns:
//				none
//
int C_CONNECT_NEWS::Group( char *szGroup,
		ULONG *plFirstArticle, ULONG *plLastArticle, ULONG *plTotal )
{
	char	szBuffer[1024];
	int		iResponse;


	// Send the group command
	sprintf( szBuffer, "group %s\r\n", szGroup );
	Send( szBuffer );
	Receive( szBuffer );

	// Process the response
	iResponse = atoi( szBuffer );
	if( iResponse > 0 && iResponse < 300 )
	{
		// Skip the unimportant parts
		atol( strtok( szBuffer, " " ) );

		// Get article information
		*plTotal = atol( strtok( NULL, " " ) );
		*plFirstArticle = atol( strtok( NULL, " " ) );
		*plLastArticle = atol( strtok( NULL, " " ) );
	}

	return iResponse;
}


//---------
// First() \
//---------------------------------------------------------------------------
// Description:
//		This method uses the internal indexing in the news server to select
//		the specified article in the current news group.
//
//	Parameters:
//				lArticle	- Article number to select
//
//	Returns:
//				none
//
int C_CONNECT_NEWS::First( ULONG lArticle )
{
	char	szBuffer[1024];
	int		iResult;


	// Request the article
	sprintf( szBuffer, "stat %ld\r\n", lArticle );
	Send( szBuffer );
	Receive( szBuffer );

	// Get the response back from the server
	iResult = atoi( szBuffer );

	return iResult;
}


//--------
// Next() \
//---------------------------------------------------------------------------
// Description:
//		This method uses the internal indexing in the news server to select
//		the next article in the current news group.  The article number is
//		returned to the caller.
//
//	Parameters:
//				plArticle	- Pointer to article number selected
//
//	Returns:
//				none
//
int C_CONNECT_NEWS::Next( ULONG *plArticle )
{
	char	szBuffer[1024];
	int		iResult;


	// Request the article
	sprintf( szBuffer, "next\r\n" );
	Send( szBuffer );
	Receive( szBuffer );

	// Get the response back from the server
	iResult = atoi( szBuffer );

	// If there was no error, load the response file
	if( iResult < 400 && atoi( szBuffer ) > 0 )
		*plArticle = atol( szBuffer + 4 );

	return iResult;
}


//-----------
// Article() \
//---------------------------------------------------------------------------
// Description:
//		This method retieves the article text for the specified article
//		number from the server and writes the data to the filename supplied
//		by the caller.
//
//	Parameters:
//				lArticle	- Article number to retieve
//				szFilename	- point to filename where output will be written.
//
//	Returns:
//				none
//
int C_CONNECT_NEWS::Article( ULONG lArticle, char *szFilename )
{
	char	szBuffer[1024];
	int		iResult;


	// Request the article
	if( lArticle > 0 )
		sprintf( szBuffer, "article %ld\r\n", lArticle );
	else
		strcpy( szBuffer, "article\r\n" );
	Send( szBuffer );
	Receive( szBuffer );

	// Get the response back from the server
	iResult = atoi( szBuffer );

	// If there was no error, load the response file
	if( iResult < 400 && atoi( szBuffer ) > 0 )
		LoadFile( szFilename );

	return iResult;
}


//--------
// Body() \
//---------------------------------------------------------------------------
// Description:
//		This method retieves the body text for the specified article
//		number from the server and writes the data to the filename supplied
//		by the caller.  The header for the article is not retrieved by this
//		method.
//
//	Parameters:
//				lArticle	- Article number whose body is to be retieved
//				szFilename	- point to filename where output will be written.
//
//	Returns:
//				none
//
int C_CONNECT_NEWS::Body( ULONG lArticle, char *szFilename )
{
	char	szBuffer[1024];
	int		iResult;


	// Request the article
	if( lArticle > 0 )
		sprintf( szBuffer, "body %ld\r\n", lArticle );
	else
		strcpy( szBuffer, "body\r\n" );
	Send( szBuffer );
	Receive( szBuffer );

	// Get the response back from the server
	iResult = atoi( szBuffer );

	// If there was no error, load the response file
	if( iResult < 400 && atoi( szBuffer ) > 0 )
		LoadFile( szFilename );

	return iResult;
}


//--------
// Head() \
//---------------------------------------------------------------------------
// Description:
//		This method retieves the header text for the specified article
//		number from the server and writes the data to the filename supplied
//		by the caller.  The body for the article is not retrieved by this
//		method.
//
//	Parameters:
//				lArticle	- Article number whose header is to be retieved
//				szFilename	- point to filename where output will be written.
//
//	Returns:
//				none
//
int C_CONNECT_NEWS::Head( ULONG lArticle, char *szFilename )
{
	char	szBuffer[10242];
	int		iResult;


	// Request the article
	if( lArticle > 0 )
		sprintf( szBuffer, "head %ld\r\n", lArticle );
	else
		strcpy( szBuffer, "head\r\n" );
	Send( szBuffer );
	Receive( szBuffer );

	// Get the response back from the server
	iResult = atoi( szBuffer );

	// If there was no error, load the response file
	if( iResult < 400 && atoi( szBuffer ) > 0 )
		LoadFile( szFilename );

	return iResult;
}

