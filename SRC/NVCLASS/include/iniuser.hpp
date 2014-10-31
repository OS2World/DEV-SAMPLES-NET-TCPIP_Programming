//------------
// C_INI_USER \
//---------------------------------------------------------------------------
//	All information contained in this file is Copyright (c) 1994-1995 by
//	NeoLogic Inc.  All Rights Reserved
//
//	This source code shall not be implemented, duplicated or used as the basis
//	for any product without prior written consent by NeoLogic Inc.
//
class C_INI_USER : public C_INI
{
	public:
		_Export C_INI_USER( char *szAppName );
		void _Export	Open( void );
#ifdef __BORLANDC__
		virtual void _Export	Close( void ) {};
#else
		virtual void 	Close( void ) {};
#endif
};

