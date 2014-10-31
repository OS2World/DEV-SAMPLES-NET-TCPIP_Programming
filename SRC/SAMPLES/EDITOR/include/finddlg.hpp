//---------------
// C_DIALOG_FIND \
//---------------------------------------------------------------------------
// Derived from: C_DIALOG
//
class C_DIALOG_FIND : public C_DIALOG
{
	private:
		C_WINDOW_STD	*pxcParent;				// Pointer to owner window

	public:
		C_DIALOG_FIND::C_DIALOG_FIND( C_WINDOW *pxcParentObj, int iID );

		// Message processors
		void *	MsgCreate( void *mp1, void *mp2 );

		// Command Processors
		void *	CmdFind( void *mp1, void *mp2 );
		void *	CmdChangeFind( void *mp1, void *mp2 );
		void *	CmdChangeAll( void *mp1, void *mp2 );
		void *	CmdCancel( void *mp1, void *mp2 );
};

