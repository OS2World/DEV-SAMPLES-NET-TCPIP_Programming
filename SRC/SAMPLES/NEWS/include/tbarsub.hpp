class C_TOOLBAR_SUB : public C_TOOLBAR
{
	public:
		C_TOOLBAR_SUB( C_WINDOW *pxcParentObj, C_STATUS *pxcStatus );
		void	Control( ULONG mp1 );
};

#define		D_SUB_TBAR			52

// Toolbar button IDs
#define		DB_UNSUBSCRIBE		100

