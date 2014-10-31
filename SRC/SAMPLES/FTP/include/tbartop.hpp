class C_TOOLBAR_TOP : public C_TOOLBAR
{
	public:
		C_TOOLBAR_TOP( C_WINDOW *pxcParentObj, C_STATUS *pxcStatus );
		void	Control( ULONG mp1 );
};

#define	D_TOP_TBAR	50

// Toolbar button IDs
#define		DB_CONFIG			133
#define		DB_WND_GRP			134
#define		DB_WND_SUB			135
#define		DB_MAIN_HELP		136

