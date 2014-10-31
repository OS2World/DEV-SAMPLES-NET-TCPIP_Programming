class C_TOOLBAR_GRP : public C_TOOLBAR
{
	public:
		C_TOOLBAR_GRP( C_WINDOW *pxcParentObj, C_STATUS *pxcStatus );
		void	Control( ULONG mp1 );
};

#define	D_GRP_TBAR			52

// Toolbar button IDs
#define	DB_SUBSCRIBE		100
#define	DB_REFRESH			101

