//----------------
// C_WINDOW_GROUP \
//---------------------------------------------------------------------------
// Derived from: C_WINDOW_STD
//
class C_WINDOW_GROUP : public C_WINDOW_STD
{
	private:
		C_STATUS		*pxcStatus;			// Pointer to status line obj
		C_TOOLBAR_GRP	*pxcTBar;			// Pointer to toolbar object
		C_CONTAINER_GRP	*pxcCont;			// Pointer to container object
		C_WINDOW_STD	*pxcParent;			// Pointer to owner window
		C_THREAD_PM		xcPopulateThread;	// Pointer to a PM aware thread

	public:
		C_WINDOW_GROUP( void );
		~C_WINDOW_GROUP( void );

		C_CONTAINER_GRP *Container( void )	{ return pxcCont; };
		C_STATUS		*Status( void )		{ return pxcStatus; };

		// Window Message processors
		void *	MsgCreate( void *mp1, void *mp2 );
		void *	MsgParent( void *mp1, void *mp2 );
		void *	MsgClose( void *mp1, void *mp2 );
		void *	MsgSize( void *mp1, void *mp2 );
		void *	MsgControl( void *mp1, void *mp2 );
		void *	MsgPopulate( void *mp1, void *mp2 );

		// Window Command Processors
		void *	CmdSubscribe( void *mp1, void *mp2 );
		void *	CmdRefresh( void *mp1, void *mp2 );
};

//------------------------------
// User defined window messages \
//---------------------------------------------------------------------------
#define	PM_PARENT		PM_USER
#define	PM_POPULATE		PM_USER+1

//--------------------------
// Child window definitions \
//----------------------------------------------------------------------------
#define	D_ID_MLE		53

