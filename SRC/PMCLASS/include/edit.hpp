//--------
// C_EDIT \
//---------------------------------------------------------------------------
//	All information contained in this file is Copyright (c) 1994-1995 by
//	NeoLogic Inc.  All Rights Reserved
//
//	This source code shall not be implemented, duplicated or used as the basis
//	for any product without prior written consent by NeoLogic Inc.
//

//-------------------------
// C_EDIT class definition \
//---------------------------------------------------------------------------
class C_EDIT : public C_WINDOW_CHILD
{
	public:
		_Export	C_EDIT( C_WINDOW *pxcParentObj, int iID, int iMode );
		_Export	C_EDIT( C_WINDOW *pxcParentObj, int iID );
		_Export	C_EDIT( C_DIALOG *pxcParentObj, int iID );

		void	_Export	TextLimit( int iLimit );
};

