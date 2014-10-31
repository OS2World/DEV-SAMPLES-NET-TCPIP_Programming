//----------------
// C_WINDOW_GROUP \
//---------------------------------------------------------------------------
// Derived from: C_WINDOW_STD
//
class C_WINDOW_SUBSCRIPTION : public C_WINDOW_STD
{
	private:
		C_STATUS		*pxcStatus;				// Pointer to status line obj
		C_TOOLBAR_SUB	*pxcTBar;				// Pointer to toolbar object
		C_CONTAINER_SUB	*pxcCont;				// Pointer to container object
		C_WINDOW_STD	*pxcParent;				// Pointer to owner window
		C_THREAD_PM		xcPopulateThread;		// Pointer to a PM aware thread
		C_THREAD_PM		xcUnsubscribeThread;	// Pointer to a PM aware thread

	public:
		C_WINDOW_SUBSCRIPTION( void );
		~C_WINDOW_SUBSCRIPTION( void );

		C_CONTAINER_SUB *Container( void )	{ return pxcCont; };

		// Window Message processors
		void *	MsgCreate( void *mp1, void *mp2 );
		void *	MsgParent( void *mp1, void *mp2 );
		void *	MsgPopulate( void *mp1, void *mp2 );
		void *	MsgSubSubscribe( void *mp1, void *mp2 );
		void *	MsgClose( void *mp1, void *mp2 );
		void *	MsgSize( void *mp1, void *mp2 );
		void *	MsgControl( void *mp1, void *mp2 );

		// Window Command Processors
		void *	CmdSubUnsubscribe( void *mp1, void *mp2 );
		void *	CmdSubRead( void *mp1, void *mp2 );
};


//------------------------------
// User defined window messages \
//---------------------------------------------------------------------------
#define	PM_PARENT			PM_USER
#define	PM_POPULATE			PM_USER+1
#define	PM_SUB_SUBSCRIBE	PM_USER+2


//--------------------------
// Child window definitions \
//----------------------------------------------------------------------------
#define	D_ID_MLE		53

