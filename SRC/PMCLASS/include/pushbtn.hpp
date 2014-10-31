//-----------
// C_PUSHBUTTON \
//---------------------------------------------------------------------------
//	All information contained in this file is Copyright (c) 1994-1995 by
//	NeoLogic Inc.  All Rights Reserved
//
//	This source code shall not be implemented, duplicated or used as the basis
//	for any product without prior written consent by NeoLogic Inc.
//

//----------------------------
// C_PUSHBUTTON class definition \
//---------------------------------------------------------------------------
class C_PUSHBUTTON : public C_WINDOW_CHILD
{
	public:
		_Export	C_PUSHBUTTON( C_WINDOW *pxcParentObj, int iID,
												int iMode, char *szText );
		_Export	C_PUSHBUTTON( C_DIALOG *pxcParentObj, int iID );
};

