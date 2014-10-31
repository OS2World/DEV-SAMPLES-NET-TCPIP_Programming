//-------------
// C_SEM_EVENT \
//---------------------------------------------------------------------------
//	All information contained in this file is Copyright (c) 1994-1995 by
//	NeoLogic Inc.  All Rights Reserved
//
//	This source code shall not be implemented, duplicated or used as the basis
//	for any product without prior written consent by NeoLogic Inc.
//
class C_SEM_EVENT
{
	private:
		char	szName[256];		// Semaphore name string
		HEV		hSemaphore;			// Handle for an OS/2 semaphore

	public:
		_Export C_SEM_EVENT( char *szSemaphoreName );
		_Export C_SEM_EVENT( void );
		int		_Export Create( void );
		int		_Export Open( void );
		int		_Export Close( void );
		int		_Export	Reset( ULONG *plPostCount );
		int		_Export	Post( void );
		int		_Export	WaitIndefinite( void );
};
