//---------------
// C_WINDOW_MAIN \
//---------------------------------------------------------------------------
// Derived from: C_WINDOW_STD
//
class C_WINDOW_MAIN : public C_WINDOW_STD
{
	private:
		C_STATUS			*pxcStatus;			// Pointer to status line obj
		C_TOOLBAR_TOP		*pxcTBar;			// Pointer to toolbar object
		C_MLE				*pxcConsole;		// Console display
		C_EDIT				*pxcCommand;		// Command entry window
		C_PUSHBUTTON		*pxcEnterButton;	// Button used to send commands

		char				szServerAddress[256];	// Address of NNTP server
		C_THREAD_PM			xcConnectThread;		// Thread to make a server
													// connection
		C_THREAD_PM			xcParseThread;

	public:
		C_CONNECT_FTP		*pxcConnection;		// FTP server connection

		C_WINDOW_MAIN( void );
		~C_WINDOW_MAIN( void );

		void 	SetServer( char *szServer );

		// Inline methods
		char			*ServerAddress( void )		{ return szServerAddress; };
		C_STATUS		*Status( void )				{ return pxcStatus; };
		C_MLE			*Console( void )			{ return pxcConsole; };
		C_EDIT			*Command( void )			{ return pxcCommand; };
		C_PUSHBUTTON	*EnterButton( void )		{ return pxcEnterButton; };

		// Window Message processors
		void *	MsgCreate( void *mp1, void *mp2 );
		void *	MsgClose( void *mp1, void *mp2 );
		void *	MsgSize( void *mp1, void *mp2 );
		void *	MsgControl( void *mp1, void *mp2 );
		void *	MsgConnect( void *mp1, void *mp2 );

		// Window Command Processors
		void *	CmdCommand( void *mp1, void *mp2 );
		void *	CmdExit( void *mp1, void *mp2 );
		void *	CmdHelpInfo( void *mp1, void *mp2 );
};

//------------------------------
// User defined window messages \
//---------------------------------------------------------------------------
#define	PM_CONNECT		PM_USER


