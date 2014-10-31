//-------------
// NETNEWS.HPP \
//---------------------------------------------------------------------------
//
//
//	All information contained in this file is Copyright (c) 1994-1995 by
//	NeoLogic Inc.  All Rights Reserved
//
//	This source code shall not be implemented, duplicated or used as the basis
//	for any product without prior written consent by NeoLogic Inc.
//
// Description:
//
//	This file contains the class definition for TCP/IP network interaction
//	specific to USENET news. C_CONNECT_NEWS
//
// Author(s):
//				Steven Gutz
//
// Version:					Rev:
//				1.0					a
//
// Revision History:
//
//	1.0a	94-Oct-01		- Initial creation
//
//---------------------------------------------------------------------------

// Standard TCP News port
#define	D_NEWS_PORT			119


class C_CONNECT_NEWS : public C_CONNECT
{
	private:
		int		iPostFlag;			// Set if posting is permitted

	public:
		_Export	C_CONNECT_NEWS( char *szConnectServer, int iPort );
#ifdef __BORLANDC__
		_Export C_CONNECT_NEWS( void ) {};
#else
		C_CONNECT_NEWS( void ) {};
#endif

		int		_Export	Open( void );
		void	_Export	Close( void );
		int		_Export	OpenPost( void );
		int		_Export	ClosePost( char *szResponse );
		int		_Export	List( char *szFilename );
		int		_Export	ListNewsGroups( char *szFilename );
		int		_Export	ListNewGroups( char *szDate, char *szFilename );
		int		_Export	Overview( ULONG lStart, ULONG lEnd, char *szFilename );
		int		_Export	Group( char *szGroup, ULONG *plFirstArticle,
									ULONG *plLastArticle, ULONG *plTotal );
		int		_Export	First( ULONG lArticle );
		int		_Export	Next( ULONG *lArticle );
		int		_Export	Article( ULONG lArticle, char *szFilename );
		int		_Export	Body( ULONG lArticle, char *szFilename );
		int		_Export	Head( ULONG lArticle, char *szFilename );
};

