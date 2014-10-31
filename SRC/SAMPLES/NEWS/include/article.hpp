//----------------
// C_WINDOW_GROUP \
//---------------------------------------------------------------------------
// Derived from: C_WINDOW_STD
//
class C_WINDOW_ARTICLE : public C_WINDOW_STD
{
	private:
		C_STATUS		*pxcStatus;				// Pointer to status line obj
		C_TOOLBAR_ART	*pxcTBar;				// Pointer to toolbar object
		C_MLE			*pxcMLE;				// Pointer to editor control
		C_WINDOW_STD	*pxcParent;				// Pointer to owner window
		C_THREAD_PM		xcPopulateThread;		// Pointer to a PM aware thread
		char			szArticle[256];			// Pointer to load file

	public:
		C_WINDOW_ARTICLE( void );
		~C_WINDOW_ARTICLE( void );

		C_STATUS	*Status( void )		{ return pxcStatus; };
		C_MLE		*MLE( void )		{ return pxcMLE; };
		char		*Article( void )	{ return szArticle; };

		// Window Message processors
		void *	MsgCreate( void *mp1, void *mp2 );
		void *	MsgParent( void *mp1, void *mp2 );
		void *	MsgPopulate( void *mp1, void *mp2 );
		void *	MsgClose( void *mp1, void *mp2 );
		void *	MsgSize( void *mp1, void *mp2 );
		void *	MsgControl( void *mp1, void *mp2 );

		// Window Command Processors
//		void *	CmdSubRead( void *mp1, void *mp2 );
};


//------------------------------
// User defined window messages \
//---------------------------------------------------------------------------
#define	PM_ART_PARENT			PM_USER
#define	PM_ART_POPULATE			PM_USER+1


//--------------------------
// Child window definitions \
//----------------------------------------------------------------------------
#define	D_ART_MLE		53

