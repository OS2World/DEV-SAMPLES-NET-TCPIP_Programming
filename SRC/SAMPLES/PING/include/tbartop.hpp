class C_TOOLBAR_TOP : public C_TOOLBAR
{
	public:
		C_TOOLBAR_TOP( C_WINDOW *pxcParentObj, C_STATUS *pxcStatus );
		void	Control( ULONG mp1 );
};

#define	D_TOP_TBAR	50

// Toolbar button IDs
#define		DB_CONNECT			133
#define		DB_MAIN_HELP		135

