//-----------
// C_CONNECT \
//---------------------------------------------------------------------------
//	All information contained in this file is Copyright (c) 1994-1995 by
//	NeoLogic Inc.  All Rights Reserved
//
//	This source code shall not be implemented, duplicated or used as the basis
//	for any product without prior written consent by NeoLogic Inc.
//


//----------------------------------
// IBM TCP/IP header files required \
//---------------------------------------------------------------------------
#ifndef NET_INCL
#define NET_INCL
extern "C"
{
	#include <types.h>
	#include <sys\socket.h>
	#include <netinet\in.h>
	#include <netinet\in_systm.h>
	#include <netinet\ip.h>
	#include <netinet\ip_icmp.h>
	#include <netdb.h>
}
#endif

//----------------------------
// Network error return codes \
//---------------------------------------------------------------------------
#define	D_NET_OK			0			// Net operation successful
#define	D_NET_HOST			-1			// Error resolving host
#define	D_NET_SOCKET		-2			// Error while creating socket
#define	D_NET_CONNECT		-3			// Error while connecting
#define	D_NET_RECV			-4			// Error while receiving
#define	D_NET_BUSY			-5			// Error: Connection Busy
#define	D_NET_PROTOCOL		-6			// Error in protocol

#define		D_NET_BUFFER	4096		// Size of network read buffer


//---------------------------------
// C_CONNECT base class definition \
//---------------------------------------------------------------------------
class C_CONNECT
{
	protected:
		char	szServer[256];					// Domain Name or IP
		int		iPort;							// Port number used
		int		iSocket;						// Socket for connection
		int		iBusy;							// Set if instance is busy
		char	szNetBuffer[D_NET_BUFFER];		// Buffer for network data
		char	*szNetBufferPtr;				// Current pointer to buffer

		struct	sockaddr_in	xtSocket;			// Socket structure used
		struct	protoent	*pxtProtocol;		// Protocol used

	public:
		_Export C_CONNECT( char *szConnectServer, int iPort );
#ifdef __BORLANDC__
		_Export C_CONNECT( void ) {};
#else
		C_CONNECT( void ) {};
#endif
		void	_Export	Initialize( char *szConnectServer, int iConnectPort );
		int		_Export	FindHost( void );
		int		_Export	Protocol( char *szProtocol );
		int		_Export	StreamSocket( void );
		int		_Export	RawSocket( void );
		int 	_Export	Open( void );
		void 	_Export	Close( void );
		int		_Export	Send( char *szCommand );
		int		_Export	SendTo( char *pbyBuff, short sLength );
		int		_Export	ReceiveFrom( char *pbyBuffer, short sLength,
											struct sockaddr_in *pxsFrom );
		int		_Export	ReceiveBuffer( char *szBuffer, int iSize );
		void 	_Export	Receive( char *szBuffer );
		void 	_Export	LoadFile( char *szFilename );

		// Inline methods
#ifdef __BORLANDC__
		char *	_Export	Server( void )		{ return szServer; };
		int		_Export	Port( void )		{ return iPort; };
		int		_Export	Socket( void )		{ return iSocket; };
		void	_Export	Busy( int iValue )	{ iBusy = iValue; };
		int		_Export	Busy( void )		{ return iBusy; };
#else
		char *	Server( void )		{ return szServer; };
		int		Port( void )		{ return iPort; };
		int		Socket( void )		{ return iSocket; };
		void	Busy( int iValue )	{ iBusy = iValue; };
		int		Busy( void )		{ return iBusy; };
#endif
};

