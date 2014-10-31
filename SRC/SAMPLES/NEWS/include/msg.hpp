//------------------
// C_WINDOW_MESSAGE \
//---------------------------------------------------------------------------
// Derived from: C_WINDOW_STD
//
class C_WINDOW_MESSAGE : public C_WINDOW_STD
{
	private:
		C_STATUS		*pxcStatus;				// Pointer to status line obj
		C_TOOLBAR_MSG	*pxcTBar;				// Pointer to toolbar object
		C_CONTAINER_MSG	*pxcCont;				// Pointer to container object
		C_WINDOW_STD	*pxcParent;				// Pointer to owner window
		C_THREAD_PM		xcPopulateThread;		// Pointer to a PM aware thread
		C_THREAD_PM		xcLoadThread;			// Pointer to a PM aware thread
		char			szGroup[256];			// Name of current display group

	public:
		C_WINDOW_MESSAGE( void );
		~C_WINDOW_MESSAGE( void );

		C_CONTAINER_MSG *Container( void )	{ return pxcCont; };
		C_WINDOW_STD	*Parent( void )		{ return pxcParent; };
		C_STATUS		*Status( void )		{ return pxcStatus; };
		char			*Group( void )		{ return szGroup; };

		int		SlowParse( int iConnection, ULONG lFirst, ULONG lLast );
		int		FastParse( void );

		// Window Message processors
		void *	MsgCreate( void *mp1, void *mp2 );
		void *	MsgParent( void *mp1, void *mp2 );
		void *	MsgPopulate( void *mp1, void *mp2 );
		void *	MsgClose( void *mp1, void *mp2 );
		void *	MsgSize( void *mp1, void *mp2 );
		void *	MsgControl( void *mp1, void *mp2 );

		// Window Command Processors
		void *	CmdMsgRead( void *mp1, void *mp2 );
};


//------------------------------
// User defined window messages \
//---------------------------------------------------------------------------
#define	PM_PARENT			PM_USER
#define	PM_POPULATE			PM_USER+1


//--------------------------
// Child window definitions \
//----------------------------------------------------------------------------

