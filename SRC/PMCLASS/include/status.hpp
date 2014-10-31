//----------
// C_STATUS \
//---------------------------------------------------------------------------
//	All information contained in this file is Copyright (c) 1994-1995 by
//	NeoLogic Inc.  All Rights Reserved
//
//	This source code shall not be implemented, duplicated or used as the basis
//	for any product without prior written consent by NeoLogic Inc.
//

//---------------------------
// C_STATUS class definition \
//---------------------------------------------------------------------------
class C_STATUS : public C_WINDOW_CHILD
{
	public:
		_Export	C_STATUS( C_WINDOW *pxcParentObj );
		void _Export	Text( char *szFormat, ... );
};


#define		D_ID_STATUS		110

