//-----------
// C_SLIDER \
//---------------------------------------------------------------------------
//	All information contained in this file is Copyright (c) 1994-1995 by
//	NeoLogic Inc.  All Rights Reserved
//
//	This source code shall not be implemented, duplicated or used as the basis
//	for any product without prior written consent by NeoLogic Inc.
//

//---------------------------
// C_SLIDER class definition \
//---------------------------------------------------------------------------
class C_SLIDER : public C_WINDOW_CHILD
{
	private:
		SLDCDATA	slData;		// Slider data area

	public:
		_Export	C_SLIDER( C_WINDOW *pxcParentObj, int iID, int iMode,
										int iIncrements, int iScale );
		_Export	C_SLIDER( C_DIALOG *pxcParentObj, int iID );
		void _Export	Scale( int iIncrements, int iSpacing );
		void _Export	Value( int iValue );
};

