//-----------
// C_TOOLBAR \
//---------------------------------------------------------------------------
//	All information contained in this file is Copyright (c) 1994-1995 by
//	NeoLogic Inc.  All Rights Reserved
//
//	This source code shall not be implemented, duplicated or used as the basis
//	for any product without prior written consent by NeoLogic Inc.
//

//--------------------------
// Toolbar macro definition \
//---------------------------------------------------------------------------
typedef struct
{
	int				iID;
	int				iIdUp;
	int				iIdDown;
	int				iIdDisable;
	char			*szText;
	int				iX;
	int				iY;
} T_BUTTON_TABLE;

#define	DECLARE_BUTTON_TABLE( button_table )\
	T_BUTTON_TABLE	button_table[] =\
	{\

#define	END_BUTTON_TABLE\
	{ 0, 0, 0, 0, 0, 0, 0 }\
};\

#define	DECLARE_BUTTON( iID, iIdUp, iIdDown, iIdDisable, szText, iX, iY )\
		{ (iID), (iIdUp), (iIdDown), (iIdDisable), (szText), (iX), (iY ) },

typedef struct
{
	int				iButtonId;
	int				iCommandId;
} T_BUTTON_CMD_TABLE;


#define	DECLARE_BUTTON_CMD_TABLE( button_cmd_table )\
	T_BUTTON_CMD_TABLE	button_cmd_table[] =\
	{\

#define	END_BUTTON_CMD_TABLE\
	{ 0, 0 }\
};\

#define	DECLARE_BUTTON_CMD( iButtonId, iCommandId )\
		{ (iButtonId), (iCommandId) },

#define	D_MAX_BUTTON	15		// Max number of buttons on any toolbar

//----------------------------
// C_TOOLBAR class definition \
//---------------------------------------------------------------------------
class C_TOOLBAR : public C_WINDOW_CHILD
{
	private:
		C_STATUS		*pxcStatus;		// Optional status bar for text display
		int				iID;			// Window ID of Toolbar
		int				iHeight;		// Height of the toolbar in pixels
		C_BUTTON_TBAR 	xcButton[D_MAX_BUTTON];	// toolbar button array
		int				iButtonCount;	// Number of buttons on toolbar
		int				iLastID;		// Last button that displayed help text
		char			szOldText[256];

	public:
		int			iText;
		int			iMouseButton;

		_Export C_TOOLBAR( C_WINDOW *pxcParentObj, int iTBarId, 
								int iTBarHeight );
		_Export	~C_TOOLBAR( void );

		void *			_Export		MsgCreate( void *mp1, void *mp2 );
		void *			_Export		MsgBMButton( void *mp1, void *mp2 );
		void *			_Export		MsgBMText( void *mp1, void *mp2 );
		void *			_Export		MsgBMButton1Down( void *mp1, 
										void *mp2 );
		void *			_Export		MsgMouseMove( void *mp1, void *mp2 );
		void *			_Export		MsgPaint( void *mp1, void *mp2 );

#ifdef __BORLANDC__
		char *			_Export	OldText( void ) { return szOldText; };
		C_BUTTON_TBAR *	_Export Buttons( void ) { return xcButton; };
		int				_Export	ButtonCount( void ) { return iButtonCount; };
		int				_Export	LastID( void ) { return iLastID; };
		void			_Export	LastID( int iValue ) { iLastID = iValue; };
		C_STATUS *		_Export	Status( void ) { return pxcStatus; };
#else
		char *			OldText( void ) { return szOldText; };
		C_BUTTON_TBAR *	Buttons( void ) { return xcButton; };
		int				ButtonCount( void ) { return iButtonCount; };
		int				LastID( void ) { return iLastID; };
		void			LastID( int iValue ) { iLastID = iValue; };
		C_STATUS *		Status( void ) { return pxcStatus; };
#endif

		void 			_Export	Status( C_STATUS *pxcStatusWindow );
		void			_Export	Control( ULONG mp1, T_BUTTON_CMD_TABLE *xtLookup );

		void 			_Export	CreateButtons( 
							T_BUTTON_TABLE *pxtButtonTable );
		C_BUTTON_TBAR * _Export	ButtonData( int );
		void 			_Export	ButtonToggle( int );
		void 			_Export	ButtonState( int iButtonId, int iState );
		void 			_Export	ButtonEnable( int iButtonId, int iValue );
};


