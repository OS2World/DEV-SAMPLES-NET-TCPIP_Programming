//---------------
// C_WINDOW_MAIN \
//---------------------------------------------------------------------------
// Derived from: C_WINDOW_STD
//
class C_WINDOW_MAIN : public C_WINDOW_STD
{
	private:
		C_STATUS				*pxcStatus;		// Pointer to status line obj
		C_TOOLBAR_TOP			*pxcTBar;		// Pointer to toolbar object
		C_MENU					*pxcMenu;		// Pointer to window menu

		C_WINDOW_GROUP			*pxcGroups;		// Pointer to the groups window
		C_WINDOW_SUBSCRIPTION	*pxcSubs;		// Pointer to the subscription
												// window
		C_WINDOW_MESSAGE		*pxcMsg;		// Pointer to the message window
		C_WINDOW_ARTICLE		*pxcArticle;	// Pointer to the article window

		char			szServerAddress[256];	// Address of NNTP server
		C_THREAD_PM		xcConnectThread;		// Thread to make a server
												// connection

	public:
		C_WINDOW_MAIN( void );
		~C_WINDOW_MAIN( void );

		void 	SetServer( char *szServer );

		// Inline methods
		char	*ServerAddress( void  )	{ return szServerAddress; };
		C_STATUS	*Status( void )		{ return pxcStatus; };

		// Window Message processors
		void *	MsgCreate( void *mp1, void *mp2 );
		void *	MsgGroupClose( void *mp1, void *mp2 );
		void *	MsgGroupSubscribe( void *mp1, void *mp2 );
		void *	MsgSubscriptionClose( void *mp1, void *mp2 );
		void *	MsgSubscriptionRead( void *mp1, void *mp2 );
		void *	MsgMessageClose( void *mp1, void *mp2 );
		void *	MsgMessageRead( void *mp1, void *mp2 );
		void *	MsgArticleClose( void *mp1, void *mp2 );
		void *	MsgClose( void *mp1, void *mp2 );
		void *	MsgSize( void *mp1, void *mp2 );
		void *	MsgControl( void *mp1, void *mp2 );
		void *	MsgConnect( void *mp1, void *mp2 );

		// Window Command Processors
		void *	CmdGroups( void *mp1, void *mp2 );
		void *	CmdSubscriptions( void *mp1, void *mp2 );
		void *	CmdExit( void *mp1, void *mp2 );
		void *	CmdConnect( void *mp1, void *mp2 );
		void *	CmdHelpInfo( void *mp1, void *mp2 );
};

//------------------------------
// User defined window messages \
//---------------------------------------------------------------------------
#define	PM_GROUP_CLOSE			PM_USER
#define	PM_GROUP_SUBSCRIBE		PM_USER + 1
#define	PM_SUB_CLOSE			PM_USER + 2
#define	PM_SUB_READ				PM_USER + 3
#define	PM_MSG_CLOSE			PM_USER + 4
#define	PM_MSG_READ				PM_USER + 5
#define	PM_ART_CLOSE			PM_USER + 6
#define	PM_CONNECT				PM_USER + 7


