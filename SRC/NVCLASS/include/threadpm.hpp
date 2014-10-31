//-------------
// C_THREAD_PM \
//---------------------------------------------------------------------------
//	All information contained in this file is Copyright (c) 1994-1995 by
//	NeoLogic Inc.  All Rights Reserved
//
//	This source code shall not be implemented, duplicated or used as the basis
//	for any product without prior written consent by NeoLogic Inc.
//
class C_THREAD_PM : public C_THREAD
{
	private:
		HAB		hABThread;
		HMQ		hMQThread;

	public:
		_Export	C_THREAD_PM( void );
		_Export	C_THREAD_PM( void (*ThreadFunction)(void *),
									unsigned int iStackSize, void *pvData );
		void	_Export	InitializeThread( void );
		void	_Export	TerminateThread( void );
};
