//----------
// C_WINDOW \
//---------------------------------------------------------------------------
//	All information contained in this file is Copyright (c) 1994-1995 by
//	NeoLogic Inc.  All Rights Reserved
//
//	This source code shall not be implemented, duplicated or used as the basis
//	for any product without prior written consent by NeoLogic Inc.
//

class C_WINDOW;				// Predefine the class so we can use it in the
							// following definitions

//----------------
// T_MSG_FUNCTION \
//--------------------------------------------------------------------------
// Definition, used by class to reference message methods
typedef void *(C_WINDOW::*T_MSG_FUNCTION)( void *, void * );

//-------------
// T_MSG_TABLE \
//--------------------------------------------------------------------------
// Definition used to define message table elements
typedef struct
{
	ULONG			lMsg;
	T_MSG_FUNCTION	Function;
} T_MSG_TABLE;

//-----------------------
// DECLARE_MESSAGE_TABLE \
//--------------------------------------------------------------------------
// macro used to define the start of a message table
#define	DECLARE_MSG_TABLE( msg_table )\
	T_MSG_TABLE	msg_table[] =\
	{\

//-------------------
// END_MESSAGE_TABLE \
//--------------------------------------------------------------------------
// macro used to define the end of a message table
#define	END_MSG_TABLE\
	{ 0, 0 }\
};\


//-------------
// DECLARE_MSG \
//--------------------------------------------------------------------------
// macro used to define a message table element
#define	DECLARE_MSG( msg, function )\
					{ (msg), ((T_MSG_FUNCTION)(function)) },

//----------------
// Command Macros \
//--------------------------------------------------------------------------
// The following macros are used to define command table constuctors
// Since these are the format as the MESSAGE macros we can simply redefine
// the message macros.
#define	DECLARE_COMMAND_TABLE	DECLARE_MSG_TABLE
#define	DECLARE_COMMAND			DECLARE_MSG
#define	END_COMMAND_TABLE		END_MSG_TABLE


//----------------------
// PMCLASS Message ID's \
//--------------------------------------------------------------------------
// PM_CREATE is the first message that a window receives after it has been
// created.
// PM_USER is the first message that can be user defined
//
#define	PM_CREATE	WM_USER			// Sent when window is created
#define	PM_USER		WM_USER + 1		// First available user defined message

//---------------------------
// C_WINDOW class definition \
//---------------------------------------------------------------------------
class	C_WINDOW
{
	private:
		HWND	hFrameWnd;				// Frame window handle
		HWND	hWnd;					// Handle to this window
		char	*szClassName;			// Class name string
		ULONG	lFrameFlags;			// Frame creation flags

	public:
		T_MSG_TABLE	*pxtMsgTable;		// Table of Window messages
		T_MSG_TABLE	*pxtCommandTable;	// Table of Window command handlers

		_Export	C_WINDOW( void );
		_Export	C_WINDOW( T_MSG_TABLE *pxtMsg );
		_Export	~C_WINDOW( void );

		HWND	_Export	ParentWindow( void );
		HWND	_Export	Window( void );
		void	_Export	Window( HWND hNewWindow );
		void	_Export	ParentWindow( HWND hNewWindow );
		void *	_Export	SendMsg( ULONG lMsg, void *mp1, void *mp2 );
		void	_Export	PostMsg( ULONG lMsg, void *mp1, void *mp2 );
		void	_Export	SetText( char *szString );
		void	_Export	GetText( char *szString, int iBufferLength );
		void	_Export	Enable( BOOL bState );
		void	_Export	Show( void );
		void	_Export	Hide( void );
		void	_Export	Update( void );
		char *	_Export ClassName( void );
		void	_Export	ClassName( char *szClass );
		void	_Export	GetSizePosition( int *piX, int *piY,
													int *piCX, int *piCY );
		void	_Export	GetSize( int *piCX, int *piCY );
		void	_Export	GetPosition( int *piX, int *piY );
		void	_Export	SetForegroundColor( BYTE byRed,
												BYTE byGreen, BYTE byBlue );
		void	_Export	SetBackgroundColor( BYTE byRed,
												BYTE byGreen, BYTE byBlue );
		void	_Export	SetFont( char *szFont, int iSize );
		void	_Export	GetForegroundColor( BYTE *pbyRed,
										BYTE *pbyGreen, BYTE *pbyBlue );
		void	_Export	GetBackgroundColor( BYTE *pbyRed,
										BYTE *pbyGreen, BYTE *pbyBlue );
		void	_Export	GetFont( char *szFont );
		void	_Export Focus( void );
		void	_Export	Invalidate( void );
		BOOL	_Export	Register( char *szClassName );
		void	_Export Create( HWND hFrameWnd, HWND hWnd );
		void	_Export Destroy( void );
		void	_Export MessageTable( T_MSG_TABLE *pxtMsg );
		void	_Export	CommandTable( T_MSG_TABLE *pxtCommands );

		virtual void * _Export	WindowProc( ULONG lMsg, void *mp1, 
									void *mp2 );
};

// OS/2 PM window procedure
MRESULT EXPENTRY StdWndProc( HWND hWnd, ULONG lMsg, MPARAM mp1, MPARAM mp2 );

