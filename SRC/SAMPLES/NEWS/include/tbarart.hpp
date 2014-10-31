class C_TOOLBAR_ART : public C_TOOLBAR
{
	public:
		C_TOOLBAR_ART( C_WINDOW *pxcParentObj, C_STATUS *pxcStatus );
		void	Control( ULONG mp1 );
};

#define		D_ART_TBAR			52

// Toolbar button IDs
#define		DB_ART_SAVE		100
#define		DB_ART_COPY		101
#define		DB_ART_CUT		102

