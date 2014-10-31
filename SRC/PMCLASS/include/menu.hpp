//--------
// C_MENU \
//---------------------------------------------------------------------------
//	All information contained in this file is Copyright (c) 1994-1995 by
//	NeoLogic Inc.  All Rights Reserved
//
//	This source code shall not be implemented, duplicated or used as the basis
//	for any product without prior written consent by NeoLogic Inc.
//

//-------------------------
// C_MENU class definition \
//---------------------------------------------------------------------------
class C_MENU : public C_WINDOW_CHILD
{
	public:
		_Export	C_MENU( C_WINDOW *pxcParentObj );
		void	_Export	EnableItem( int iIDItem );
		void	_Export	DisableItem( int iIDItem );
		void	_Export	SetItemText( int iIDItem, char *szText );
		void	_Export	GetItemText( int iIDItem, char *szText, int iSize );
};


