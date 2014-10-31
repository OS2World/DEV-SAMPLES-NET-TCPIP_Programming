#define	INCL_DOS

#include <os2.h>
#include <stdio.h>

#include <types.h>
#include <sys\socket.h>
#include <netinet\in.h>
#include <netdb.h>

int main( void )
{
	int		iResult;
	int		iSocket;
	int		iPort = 21;						// FTP Port
	char	*szServer = "ftp.cdrom.com";	// Example server
	char	szBuffer[512];
	struct hostent		*pxtHost;
	struct sockaddr_in	xtSocket;


	// Initialize the socket for communications
	sock_init();
	iSocket = 0;

	// Resolve the host
	xtSocket.sin_addr.s_addr = inet_addr( szServer );
	if( (LONG)xtSocket.sin_addr.s_addr == -1 )
	{
		pxtHost = gethostbyname( szServer );
		if( pxtHost == NULL )
		{
			// Error getting host so terminate
			printf( "Error:Could not get host information\n" );
			return -1;
		}

		bcopy( pxtHost->h_addr, &xtSocket.sin_addr, pxtHost->h_length );
	}

	// Open a socket for this connection
	iSocket = socket( AF_INET, SOCK_STREAM, 0 );
	if( iSocket == -1 )
	{
		// Error getting socket so terminate
		iSocket = 0;
		printf( "Error:Could not create socket\n" );
		return -1;
	}

	// Connect to the correct port
	xtSocket.sin_family = AF_INET;
	xtSocket.sin_port = htons( iPort );

	if( connect( iSocket, &xtSocket, sizeof( xtSocket ) ) < 0 )
	{
		// Error connecting so terminate
		iSocket = 0;
		printf( "Error:Could not connect\n" );
		return -1;
	}

	//
	// At this point we are connected and ready to go.
	//
	printf( "Connection successful!!\n" );

	// Send a command and get a response
	send( iSocket, "SYST\r\n", (short)6, 0 );
	iResult = recv( iSocket, szBuffer, (short)512, 0 );
	if( iResult > 0 )
	{
		printf( "SYST Response String = %s", szBuffer );
	}

	// See ya!
	send( iSocket, "QUIT\r\n", (short)6, 0 );
	iResult = recv( iSocket, szBuffer, (short)512, 0 );
	if( iResult > 0 )
	{
		printf( "QUIT Response String = %s", szBuffer );
	}

	// Close the connection
	shutdown( iSocket, 2 );
	soclose( iSocket );

	return 0;
}
