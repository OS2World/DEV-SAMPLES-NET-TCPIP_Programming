//--------------
// C_WINDOW_LOG \
//---------------------------------------------------------------------------
//	All information contained in this file is Copyright (c) 1994-1995 by
//	NeoLogic Inc.  All Rights Reserved
//
//	This source code shall not be implemented, duplicated or used as the basis
//	for any product without prior written consent by NeoLogic Inc.
//

//---------------------------------
// C_WINDOW_DEBUG class definition \
//---------------------------------------------------------------------------
class C_WINDOW_DEBUG : public C_WINDOW_STD
{
	private:
		C_MLE		*pxcDebugMLE;

	public:
		_Export C_WINDOW_DEBUG( void );
		_Export ~C_WINDOW_DEBUG( void );
		void * _Export MsgCreate( void *mp1, void *mp2 );
		void * _Export MsgAdd( void *mp1, void *mp2 );
		void * _Export MsgSize( void *mp1, void *mp2 );
};


//------------------------
// C_LOG class definition \
//---------------------------------------------------------------------------
class	C_LOG
{
	private:
		char			szFileName[256];	// Name of log file
		FILE			*hLogFile;			// Log file handle
		int				iLogging;			// Logging mode (TRUE or FALSE)
		C_WINDOW_DEBUG	*pxcDebug;			// Debug window class

	public:
		_Export	C_LOG( char *szLogFile, int iLogMode );
		_Export ~C_LOG( void );
		void _Export	Open( void );
		void _Export	Close( void );
		void _Export	Write( char *, ... );
};


//-----------------------------------
// C_WINDOW_DEBUG message definition \
//---------------------------------------------------------------------------
#define	DM_DEBUG_ADD	PM_USER

