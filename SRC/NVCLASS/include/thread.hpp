//----------
// C_THREAD \
//---------------------------------------------------------------------------
//	All information contained in this file is Copyright (c) 1994-1995 by
//	NeoLogic Inc.  All Rights Reserved
//
//	This source code shall not be implemented, duplicated or used as the basis
//	for any product without prior written consent by NeoLogic Inc.
//
class C_THREAD
{
	private:
		void	*pvThreadData;
		TID		hThreadID;

	public:
		_Export	C_THREAD( void );
		_Export	C_THREAD( void (*ThreadFunction)(void *),
									unsigned int iStackSize, void *pvData );

		// Inline methods
#ifdef __BORLANDC__
		void *	_Export	ThreadData( void )		{ return pvThreadData; };
#else
		void *	ThreadData( void )				{ return pvThreadData; };
#endif

		// Regular methods
		void	_Export	Create( void *ThreadFunction,
									unsigned int iStackSize, void *pvData );
		void	_Export	Kill( void );
		void	_Export	WaitIndefinite( void );
};

