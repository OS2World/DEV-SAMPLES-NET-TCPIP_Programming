//-------
// C_MLE \
//---------------------------------------------------------------------------
//	All information contained in this file is Copyright (c) 1994-1995 by
//	NeoLogic Inc.  All Rights Reserved
//
//	This source code shall not be implemented, duplicated or used as the basis
//	for any product without prior written consent by NeoLogic Inc.
//

//------------------------
// C_MLE class definition \
//---------------------------------------------------------------------------
class C_MLE : public C_WINDOW_CHILD
{
	private:
		MLE_SEARCHDATA	xtSearch;

	public:
		_Export	C_MLE( C_WINDOW *pxcParentObj, int iMLEID, int iMode );
		_Export	C_MLE( C_WINDOW *pxcParentObj, int iMLEID );
		_Export	C_MLE( C_DIALOG *pxcParentObj, int iMLEID );
		void _Export	ReadOnlyStatus( short iBool );
		void _Export	WordWrap( short iBool );
		void _Export	ResetDirtyBufferFlag( void );
		int	 _Export	IsBufferDirty( void );
		void _Export	Undo( void );
		void _Export	Copy( void );
		void _Export	Cut( void );
		void _Export	Paste( void );
		void _Export	Clear( void );
		LONG _Export	BufferLength( void );
		void _Export	DisableRefresh( void );
		void _Export	EnableRefresh( void );
		void _Export	Insert( char *szString );
		void _Export	Delete( LONG lStart, LONG lCount );
		void _Export	Select( LONG lStart, LONG lEnd );
		void _Export	QuerySelection( LONG *pAnchor, LONG *pCursor );
		void _Export	TransferBuffer( char *szString, LONG lSize );
		void _Export	ExportBuffer( LONG *ipStart, LONG *ipEnd );
		int  _Export	FindFirst( char *szString, int iCase );
		int  _Export	FindNext( int iCase );
		int  _Export	FindFromCursor( char *szString, int iCase );
		int  _Export	ChangeAll( char *szFind, char *szReplace, int iCase );
		LONG _Export	Line( LONG lCursor );
		LONG _Export	Column( LONG lLine );
		LONG _Export	NumberOfLines( void );
		void _Export	Load( C_STATUS *pxcStatus, char *szFilename );
		void _Export	Save( C_STATUS *pxcStatus, char *szFilename );
};

