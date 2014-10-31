//----------
// C_BUTTON \
//---------------------------------------------------------------------------
//	All information contained in this file is Copyright (c) 1994-1995 by
//	NeoLogic Inc.  All Rights Reserved
//
//	This source code shall not be implemented, duplicated or used as the basis
//	for any product without prior written consent by NeoLogic Inc.
//

//---------------------------
// C_BUTTON class definition \
//---------------------------------------------------------------------------
class C_BUTTON_TBAR : public C_WINDOW_CHILD
{
	private:
		int			iID;
		int			iX;
		int			iY;
		int			iState;

		int			iIdUp;
		int			iIdDn;
		int			iIdDisable;
		char		*szButtonText;

	public:
		_Export	C_BUTTON_TBAR( void );
		_Export	C_BUTTON_TBAR( C_WINDOW *pxcParentObj );

		void 	_Export	Initialize( int iButtonID, int iXPos, int iYPos,
								int iUp, int iDn, int iDis, char *szText );

		void *	_Export	MsgMouseMove( void *mp1, void *mp2 );
		void *	_Export	MsgPaint( void *mp1, void *mp2 );
		void *	_Export	MsgButton1Down( void *mp1, void *mp2 );
		void *	_Export	MsgButton1Up( void *mp1, void *mp2 );

		void 	_Export	State( int iNewState );
		void 	_Export	Toggle( void );

#ifdef __BORLANDC__
		int		_Export	State( void ) { return iState; };
		int		_Export	ID( void ) { return iID; };
		char *	_Export	Text( void ) { return szButtonText; };
		int		_Export	X( void ) { return iX; };
		int		_Export	Y( void ) { return iY; };
		int		_Export	Up( void ) { return iIdUp; };
		int		_Export	Down( void ) { return iIdDn; };
		int		_Export	Disable( void ) { return iIdDisable; };
#else
		int		State( void ) { return iState; };
		int		ID( void ) { return iID; };
		char *	Text( void ) { return szButtonText; };
		int		X( void ) { return iX; };
		int		Y( void ) { return iY; };
		int		Up( void ) { return iIdUp; };
		int		Down( void ) { return iIdDn; };
		int		Disable( void ) { return iIdDisable; };
#endif
};


//-----------------
// Button messages \
//---------------------------------------------------------------------------
#define		BM_CREATE		PM_USER
#define		BM_BUTTON		PM_USER+1
#define		BM_TEXT			PM_USER+2
#define		BM_BUTTON1DOWN	PM_USER+3


//---------------
// Button states \
//---------------------------------------------------------------------------
#define		D_BUTTON_UP		1
#define     D_BUTTON_DN		2
#define		D_BUTTON_DIS	-1

