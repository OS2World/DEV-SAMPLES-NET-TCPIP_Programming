//----------------
// C_CONNECT_PING \
//---------------------------------------------------------------------------
//	All information contained in this file is Copyright (c) 1994-1995 by
//	NeoLogic Inc.  All Rights Reserved
//
//	This source code shall not be implemented, duplicated or used as the basis
//	for any product without prior written consent by NeoLogic Inc.
//

class C_CONNECT_PING : public C_CONNECT
{
	private:
		int			iTransmitCount;		// Packet number tx'ed
		USHORT		iIdent;				// Unique identifier for this ping process
		DATETIME	xtStartTime;		// Start time of the ping transmit

	public:
		_Export	C_CONNECT_PING( USHORT iIdentity, char *szConnectServer );
		int		_Export	Open( void );
		int		_Export	PingRx( BYTE *pbyPacket, char *szString );
		int		_Export	PingTx( BYTE *byPacket, int iLength );
		void	_Export	ResultString( char *szString,
								char *buf, int cc, struct sockaddr_in *from );
		USHORT	_Export	InChecksum( USHORT *pbyAddr, int iLen );
};

