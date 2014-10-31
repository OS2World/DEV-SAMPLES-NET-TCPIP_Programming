//--------------
// NETFTP.HPP \
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
//	specific to USENET news. C_CONNECT_FTP
//
//---------------------------------------------------------------------------

class C_CONNECT_FTP : public C_CONNECT
{
	private:
		C_MLE		*pxcMLE;			// Pointer to console window, if any
		int			iHash;

	public:
		_Export	C_CONNECT_FTP( char *szConnectServer, int iPort,
													C_MLE *pxcConsole );

		int		_Export	Open( void );
		void	_Export	Close( void );
		int		_Export	Send( char *szCommand );
		void	_Export	Receive( char *szBuffer );
		int		_Export	Accept( int iSocket, char *szFile );
		int		_Export	Put( int iSocket, char *szFile );
		int		_Export	Listen( void );
		int		_Export	SocketClose( int iSocket );
		int		_Export	SendOOB( char *szCommand );

		// Command handlers
		void	_Export	FTPCommand( char *szCommand );
		int		_Export	SYST( void );
		int		_Export	SITE( char *szText );
		int		_Export	ACCT( char *szText );
		int		_Export	USER( char *szText );
		int		_Export	PASS( char *szText );
		int		_Export	TYPE( char *szText );
		int		_Export	PWD( char *szText );
		int		_Export	CWD( char *szText );
		int		_Export	RMD( char *szText );
		int		_Export	MKD( char *szText );
		int		_Export	DELE( char *szText );
		int		_Export	DIR( char *szWildcards, char *szFile );
		int		_Export	ABOR( void );
		int		_Export	RETR( char *szSrcFile, char *szDstFile );
		int		_Export	STOR( char *szSrcFile, char *szDstFile );
		int		_Export	QUIT( void );
		int		_Export	NOOP( void );
};

