class C_TOOLBAR_TOP : public C_TOOLBAR
{
	public:
		C_TOOLBAR_TOP( C_WINDOW *pxcParentObj, C_STATUS *pxcStatus );
		void	Control( ULONG mp1 );
};

// Main Toolbar Identifier
#define	D_TOP_TBAR	50

// Main Toolbar button IDs
#define		DB_OPEN				100
#define		DB_SAVE				101
#define		DB_CUT				102
#define		DB_COPY				103
#define		DB_PASTE			104
#define		DB_FIND				105

