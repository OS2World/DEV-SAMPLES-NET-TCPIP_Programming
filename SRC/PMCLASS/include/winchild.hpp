//----------------
// C_WINDOW_CHILD \
//---------------------------------------------------------------------------
//	All information contained in this file is Copyright (c) 1994-1995 by
//	NeoLogic Inc.  All Rights Reserved
//
//	This source code shall not be implemented, duplicated or used as the basis
//	for any product without prior written consent by NeoLogic Inc.
//
class C_WINDOW_CHILD : public C_WINDOW
{
	private:
		C_WINDOW	*pxcParent;		// Pointer to parent window

	public:
		_Export	C_WINDOW_CHILD( void );
		_Export	C_WINDOW_CHILD( T_MSG_TABLE *pxtMsg );
		_Export	C_WINDOW_CHILD( C_WINDOW *pxcParentObj, T_MSG_TABLE *pxtMsg );
		void	_Export	ParentObject( C_WINDOW *pxcParentObj );
		void	_Export	Create( int iID, int iMode, char *szTitle,
										int iX, int iY, int iCX, int iCY );
		void	_Export SetSizePosition( int iX, int iY, int iCX, int iCY );
		void	_Export	SetPosition( int iX, int iY );
		void	_Export	SetSize( int iCX, int iCY );

		C_WINDOW * _Export	ParentObject( void );
};



