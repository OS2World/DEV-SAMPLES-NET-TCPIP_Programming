//--------------
// C_WINDOW_STD \
//---------------------------------------------------------------------------
//	All information contained in this file is Copyright (c) 1994-1995 by
//	NeoLogic Inc.  All Rights Reserved
//
//	This source code shall not be implemented, duplicated or used as the basis
//	for any product without prior written consent by NeoLogic Inc.
//
class C_WINDOW_STD : public C_WINDOW
{
	private:
		char	*szClassName;
		ULONG	lFrameFlags;

	public:
		_Export	C_WINDOW_STD( void );
		_Export	C_WINDOW_STD( T_MSG_TABLE *pxtMsg );
		void	_Export	Create( int iID, char *szTitle );
		void 	_Export	GetSizePosition( int *piX, int *piY,
													int *piW, int *piL );
		void 	_Export	GetSize( int *piX, int *piY );
		void 	_Export	GetPosition( int *piW, int *piL );
		void	_Export	SetSizePosition( int iX, int iY, int iCX, int iCY );
		void	_Export	SetPosition( int iX, int iY );
		void	_Export	SetSize( int iCX, int iCY );
		void	_Export	SetTitle( char *szTitle );
		void	_Export	GetTitle( char *szTitle, int iLength );

		void	_Export WCF_Standard( void );
		void	_Export WCF_Icon( void );
		void	_Export WCF_SysMenu( void );
		void	_Export WCF_Menu( void );
		void	_Export WCF_MinButton( void );
		void	_Export WCF_MaxButton( void );
		void	_Export WCF_TitleBar( void );
		void	_Export WCF_Border( void );
		void	_Export WCF_DialogBorder( void );
		void	_Export WCF_SizingBorder( void );
		void	_Export WCF_TaskList( void );
		void	_Export WCF_ShellPosition( void );

		// Window Message Handlers
		virtual void *	_Export	MsgPaint( void *mp1, void *mp2 );
};



