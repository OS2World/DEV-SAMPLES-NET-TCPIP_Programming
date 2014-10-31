//----------------
// C_WINDOW_CLASS \
//---------------------------------------------------------------------------
// Derived from: C_WINDOW_STD
//
class C_WINDOW_MAIN : public C_WINDOW_STD
{
	private:
		C_STATUS		*xcStatus;				// Pointer to status line obj
		C_TOOLBAR_TOP	*xcTBar;				// Pointer to toolbar object
		C_MLE			*xcMLE;					// Pointer to MLE object
		C_THREAD_PM		xcPingThread;			// Pointer to a PM aware thread
		char			szServerAddress[256];	// Address of computer to PING
		int				iMustExit;				// Set when window must close

	public:
		C_WINDOW_MAIN( void );

		// Inline methods
		int		ExitFlag( void )		{ return iMustExit; };
		void	ExitFlag( int iValue )	{ iMustExit = iValue; };
		char	*ServerAddress( void  )	{ return szServerAddress; };
		C_MLE	*MLE( void  )			{ return xcMLE; };

		// Window Message processors
		void *	MsgCreate( void *mp1, void *mp2 );
		void *	MsgDone( void *mp1, void *mp2 );
		void *	MsgClose( void *mp1, void *mp2 );
		void *	MsgDestroy( void *mp1, void *mp2 );
		void *	MsgSize( void *mp1, void *mp2 );
		void *	MsgControl( void *mp1, void *mp2 );

		// Window Command Processors
		void *	CmdExit( void *mp1, void *mp2 );
		void *	CmdConnect( void *mp1, void *mp2 );
		void *	CmdHelpInfo( void *mp1, void *mp2 );
};

//------------------------------
// User defined window messages \
//---------------------------------------------------------------------------
#define	PM_DONE			PM_USER


//--------------------------
// Child window definitions \
//----------------------------------------------------------------------------
#define	D_ID_MLE		53

