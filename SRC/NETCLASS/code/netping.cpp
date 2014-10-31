//-------------
// NETPING.CPP \
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
//	This file contains the class methods for TCP/IP network Ping interaction.
//
//---------------------------------------------------------------------------

// System includes
#define	INCL_DOS
#define	INCL_WIN

//---------------
// Include Files \
//---------------------------------------------------------------------------
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <os2.h>

//----------------
// NetClass Files \
//---------------------------------------------------------------------------
#include <net.hpp>
#include <netping.hpp>


//----------------
// C_CONNECT_PING \
//---------------------------------------------------------------------------
// Description:
//		This constructor creates an instance of the C_CONNECT_PING class.
//
//	Parameters:
//		iIdentity			- Identifying integer for this application
//		szConnectServer		- Pointer to domain name or IP string of server
//
//	Returns:
//				none
//
C_CONNECT_PING::C_CONNECT_PING( USHORT iIdentity, char *szConnectServer )
									: C_CONNECT( szConnectServer, 0 )
{
	iTransmitCount = 0;
	iIdent = iIdentity;
}


//------
// Open \
//---------------------------------------------------------------------------
// Description:
//		This method creates and opens a PING connection to the IP specified
//		when this instance was constructed.  If acquires the host information
//      and sets the protocol to ICMP, then creates a raw data connection.
//
//	Parameters:
//				void
//
//	Returns:
//				int		- result of the connection attempt
//
int C_CONNECT_PING::Open( void )
{
	int		iResult;


	// Fill in the host information
	iResult = FindHost();
	if( iResult != D_NET_OK )
		return iResult;

	// Set up the correct protocol
	iResult = Protocol( "icmp" );
	if( iResult != D_NET_OK )
		return iResult;

	// Connect to a raw data socket
	iResult = RawSocket();
	return iResult;
}


//--------
// PingRX \
//---------------------------------------------------------------------------
// Description:
//		This method receives a ping packet back from the host and formats
//		the approriate result string for any errors that may have occurred.
//
//	Parameters:
//		pbyPacket		- Pointer to a data area containing the data packet
//		szString		- Pointer area where the result string will be written
//
//	Returns:
//		int				- Number of bytes received by from the host.
//
int C_CONNECT_PING::PingRx( BYTE *pbyPacket, char *szString )
{
	int				iCtr;
	int				iResult;
	int				sock_arr[5];
	struct sockaddr_in	xsFrom;


   	iCtr = 0;
	strcpy( szString, "" );

	// Be willing to wait 5 seconds for a response
	sock_arr[0] = (short)Socket();
	iResult = select( (int *)sock_arr, 1, 0, 0, 5000L );
	if( iResult > 0 )
    {
		// Receive the packet from the host
		iCtr = ReceiveFrom( (char *)pbyPacket, D_NET_BUFFER, &xsFrom );

		// If there were no errors, format a PING string to return to the caller
		if( iCtr >= 0 )
			ResultString( szString, (char *)pbyPacket, iCtr, &xsFrom );
	}

	return iCtr;
}


//--------
// PingTX \
//---------------------------------------------------------------------------
// Description:
//		This method sends a ping packet to the host.  It also updates the
//		packet transmission count used to track received packets and records
//		the starting time for the round-trip calculation.
//
//	Parameters:
//		pbyPacket	- Pointer to a data area containing the data packet
//		iLength		- Size of the packet to be transmitted
//
//	Returns:
//		int			- D_NET_OK if transmission was OK
//
int C_CONNECT_PING::PingTx( BYTE *pbyPacket, int iLength )
{
	int		iResult;
	struct	icmp	*pxsICmp;


	// Populate the ICMP structure for transmission
	pxsICmp = (struct icmp *)pbyPacket;
	pxsICmp->icmp_type = ICMP_ECHO;
	pxsICmp->icmp_code = 0;
	pxsICmp->icmp_cksum = 0;
	pxsICmp->icmp_id = (UCHAR)iIdent;
	pxsICmp->icmp_seq = (UCHAR)iTransmitCount++;

	// Compute the packet checksum
	pxsICmp->icmp_cksum = InChecksum( (USHORT *)pxsICmp, iLength );

	// Get the start time for the ping TX
	DosGetDateTime( &xtStartTime );

	// Transmit the packet
	iResult = D_NET_RECV;

	if( SendTo( (char *)pbyPacket, iLength ) == iLength )
		iResult = D_NET_OK;
	return iResult;
}


//--------------
// ResultString \
//---------------------------------------------------------------------------
// Description:
//		This method calculates the round-trip time and formats a string which
//		indicates and errors that occurred during the Ping operation.
//
//	Parameters:
//      szString		- Pointer area where the result string will be written
//		pbyPacket		- Pointer to a data area containing the data packet
//		iLength			- size of the packet buffer
//		xsFrom			- Ping receive socket
//
//	Returns:
//		void
//
void C_CONNECT_PING::ResultString( char *szString,
					char *pbyPacket, int iLength, struct sockaddr_in *xsFrom )
{
	char			*szPacketType;
	int				iHeaderLength;
	long			lStart;
	long			lEnd;
	DATETIME		xtEndTime;
	struct ip		*ip;
	struct icmp		*icp;
	struct in_addr	in;
	static char 	*ttab[] = {
				"Echo Reply", "ICMP 1", "ICMP 2", "Dest Unreachable",
				"Source Quence", "Redirect", "ICMP 6", "ICMP 7", "Echo",
				"ICMP 9", "ICMP 10", "Time Exceeded", "Parameter Problem",
				"Timestamp", "Timestamp Reply", "Info Request", "Info Reply"
	};



	in.s_addr = xsFrom->sin_addr.s_addr;
	xsFrom->sin_addr.s_addr = ntohl( xsFrom->sin_addr.s_addr );

	ip = (struct ip *)pbyPacket;
	iHeaderLength = (ip->ip_hl << 2) + 4;
	if( iLength < iHeaderLength + ICMP_MINLEN)
	{
		// Format the output string
		sprintf( szString, "packet too short (%d bytes) from %s", iLength,
														inet_ntoa(in));
		return;
	}
	iLength -= iHeaderLength;

	icp = (struct icmp *)(pbyPacket + iHeaderLength);

	// Make sure this packet belongs to us
	if( icp->icmp_id == iIdent )
	{
		icp->icmp_type &= 0x0f;
		if( icp->icmp_type != ICMP_ECHOREPLY )
		{
			// Determine the packet type
			szPacketType = ttab[icp->icmp_type];

			// Format the output string
			sprintf( szString, "%d bytes from %s: icmp_type=%d (%s)",
					iLength, inet_ntoa(in), icp->icmp_type, szPacketType );
			return;
		}

		// Calculate the round-trip time
		DosGetDateTime( &xtEndTime );
		lStart = xtStartTime.hundredths + xtStartTime.seconds * 100 +
				xtStartTime.minutes * 6000 + xtStartTime.hours * 60 * 60000;
		lEnd = xtEndTime.hundredths + xtEndTime.seconds * 100 +
				xtEndTime.minutes * 6000 + xtEndTime.hours * 60 * 60000;
		lEnd -= lStart;

		// Format the output string
		sprintf( szString, "%d bytes from %s: icmp_seq=%d, time=%ldms",
							iLength, inet_ntoa(in), icp->icmp_seq, lEnd );
	}
	else
		strcpy( szString, "" );
}


//------------
// InChecksum \
//---------------------------------------------------------------------------
// Description:
//		This method returns a packet checksum for PING packets.
//
//	Parameters:
//		pbyAddre		- Pointer to a data area containing the data packet
//		iLen			- size of the packet buffer
//
//	Returns:
//		USHORT			- Checksum of the packet
//
USHORT C_CONNECT_PING::InChecksum( USHORT *pbyAddr, int iLen )
{
	int		iNLeft = iLen;
	USHORT	*piW = pbyAddr;
	USHORT	usAnswer;
	LONG	lSum = 0;


	// The algorithm is simple, using a 32-bit accumulator (iSum), we add
	// sequential 16-bit words to it, and at the end, fold back all the carry
	// bits from the top 16 bits into the lower 16 bits.
	while( iNLeft > 1 )
	{
		lSum += *piW++;
		iNLeft -= 2;
	}

	// Clean up odd byte if necessary
	if( iNLeft == 1 )
	{
		lSum += *(UCHAR *)piW;
	}

	// Add back carry-outs from top 16 bits to low 16 bits
	lSum = (lSum >> 16) + (lSum & 0xffff);	// Add hi 16 to low 16
	lSum += (lSum >> 16);					// Add carry
	usAnswer = (USHORT)~lSum;				// Truncate to 16 bits

	return usAnswer;
}

