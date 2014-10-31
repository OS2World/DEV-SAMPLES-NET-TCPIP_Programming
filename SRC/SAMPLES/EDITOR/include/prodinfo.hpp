//----------------
// C_DIALOG_ABOUT \
//---------------------------------------------------------------------------
// Derived from: C_DIALOG
//
class C_DIALOG_ABOUT : public C_DIALOG
{
	public:
		C_DIALOG_ABOUT::C_DIALOG_ABOUT( C_WINDOW *pxcParentObj, int iID );

		// Message processors
		void *	MsgCreate( void *mp1, void *mp2 );

		// Command Processors
		void *	CmdOK( void *mp1, void *mp2 );
};

