//----------
// C_DIALOG \
//---------------------------------------------------------------------------
//	All information contained in this file is Copyright (c) 1994-1995 by
//	NeoLogic Inc.  All Rights Reserved
//
//	This source code shall not be implemented, duplicated or used as the basis
//	for any product without prior written consent by NeoLogic Inc.
//

//---------------------------
// C_DIALOG class definition \
//---------------------------------------------------------------------------
class C_DIALOG : public C_WINDOW_CHILD
{
	public:
		_Export	C_DIALOG( C_WINDOW *pxcParentObj, T_MSG_TABLE *pxtMsg );
		void	_Export	Create( int iID );
		void	_Export	CreateWithParent( int iID );
		ULONG	_Export	Process( void );
		void	_Export	Close( int iFlag );
		void *	_Export	DialogProc( ULONG lMsg, void *mp1, void *mp2 );
};



