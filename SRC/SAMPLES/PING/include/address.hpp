//------------------
// C_DIALOG_ADDRESS \
//---------------------------------------------------------------------------
// Derived from: C_DIALOG
//
class C_DIALOG_ADDRESS : public C_DIALOG
{
	private:
		C_WINDOW		*pxcParent;			// Point to parent object
		C_EDIT			*pxcEditAddress;	// Address field in dialog

	public:
		C_DIALOG_ADDRESS::C_DIALOG_ADDRESS( C_WINDOW *pxcParentObj, int iID );

		// Message processors
		void *	MsgInitDlg( void *mp1, void *mp2 );
		void *	MsgDestroy( void *mp1, void *mp2 );
		void *	MsgParent( void *mp1, void *mp2 );

		// Command Processors
		void *	CmdOK( void *mp1, void *mp2 );
		void *	CmdCancel( void *mp1, void *mp2 );
};


//------------------------------
// User defined window messages \
//---------------------------------------------------------------------------
#define	PM_PARENT	PM_USER

//--------------------------
// Child window definitions \
//----------------------------------------------------------------------------
#define	DE_ADDRESS		102			// ID for address field

