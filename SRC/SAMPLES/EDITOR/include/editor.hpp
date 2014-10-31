//---------------
// C_WINDOW_MAIN \
//---------------------------------------------------------------------------
// Derived from: C_WINDOW_STD
//
class C_WINDOW_MAIN : public C_WINDOW_STD
{
	private:
		C_STATUS		*pxcTopStatus;		// Pointer to top status bar
		C_STATUS		*pxcBottomStatus;	// Pointer to the bottom status bar
		C_TOOLBAR_TOP	*pxcTBar;			// Pointer to toolbar object
		C_MLE			*pxcMLE;			// Pointer to the editor object
		C_THREAD_PM		xcLoadThread;		// Pointer to a PM aware thread
		C_THREAD_PM		xcSaveThread;		// Pointer to a PM aware thread
		C_THREAD_PM		xcCloseThread;		// Pointer to a PM aware thread
		char			szFile[512];		// String containing current file
		int				iCaseSensitive;		// Set if searches are to be cased

	public:
		C_WINDOW_MAIN( void );
		~C_WINDOW_MAIN( void );

		// Inline methods
		C_STATUS	*BottomStatus( void )	{ return pxcBottomStatus; };
		C_MLE		*MLE( void )			{ return pxcMLE; };
		C_THREAD_PM	*LoadThread( void )		{ return &xcLoadThread; };
		C_THREAD_PM	*SaveThread( void )		{ return &xcSaveThread; };
		char		*File( void )			{ return szFile; };

		// Message Processor Methods
		void	*MsgCreate( void *mp1, void *mp2 );
		void	*MsgLoadFile( void *mp1, void *mp2 );
		void	*MsgFindSetCase( void *mp1, void *mp2 );
		void	*MsgFind( void *mp1, void *mp2 );
		void	*MsgChangeFind( void *mp1, void *mp2 );
		void	*MsgChangeAll( void *mp1, void *mp2 );
		void	*MsgSize( void *mp1, void *mp2 );
		void	*MsgControl( void *mp1, void *mp2 );
		void	*MsgClose( void *mp1, void *mp2 );
		void	*MsgChar( void *mp1, void *mp2 );

		// Command Processor Methods
		void	*CmdFileNew( void *mp1, void *mp2 );
		void	*CmdFileOpen( void *mp1, void *mp2 );
		void	*CmdFileSave( void *mp1, void *mp2 );
		void	*CmdFileSaveAs( void *mp1, void *mp2 );
		void	*CmdEditUndo( void *mp1, void *mp2 );
		void	*CmdEditCut( void *mp1, void *mp2 );
		void	*CmdEditCopy( void *mp1, void *mp2 );
		void	*CmdEditClear( void *mp1, void *mp2 );
		void	*CmdEditPaste( void *mp1, void *mp2 );
		void	*CmdEditFind( void *mp1, void *mp2 );
		void	*CmdEditSelect( void *mp1, void *mp2 );
		void	*CmdOptionsWrapOn( void *mp1, void *mp2 );
		void	*CmdOptionsWrapOff( void *mp1, void *mp2 );
		void	*CmdHelpIndex( void *mp1, void *mp2 );
		void	*CmdHelpGeneral( void *mp1, void *mp2 );
		void	*CmdHelpUsing( void *mp1, void *mp2 );
		void	*CmdHelpKeys( void *mp1, void *mp2 );
		void	*CmdHelpInfo( void *mp1, void *mp2 );
};

//------------------------------
// User defined window messages \
//---------------------------------------------------------------------------
#define		PM_FIND				PM_USER
#define		PM_FIND_SET_CASE    PM_USER+1
#define		PM_CHANGE_FIND		PM_USER+2
#define		PM_CHANGE_ALL		PM_USER+3
#define		PM_LOAD_FILE		PM_USER+4

