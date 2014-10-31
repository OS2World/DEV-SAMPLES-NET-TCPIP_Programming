//-----------
// C_LISTBOX \
//---------------------------------------------------------------------------
//	All information contained in this file is Copyright (c) 1994-1995 by
//	NeoLogic Inc.  All Rights Reserved
//
//	This source code shall not be implemented, duplicated or used as the basis
//	for any product without prior written consent by NeoLogic Inc.
//

//----------------------------
// C_LISTBOX class definition \
//---------------------------------------------------------------------------
class C_LISTBOX : public C_WINDOW_CHILD
{
	public:
		_Export	C_LISTBOX( C_WINDOW *pxcParentObj, int iID, int iMode );
		_Export	C_LISTBOX( C_WINDOW *pxcParentObj, int iID );
		_Export	C_LISTBOX( C_DIALOG *pxcParentObj, int iID );
		void	_Export	Insert( char *szText, int iHow );
		void	_Export	Delete( int iItem );
		void	_Export	DeleteAll( void );
		void	_Export	Select( int iItem, BOOL bBoolean );
		int		_Export	Selection( int iFrom );
		int		_Export	ItemCount( void );
		void	_Export	ItemText( int iIndex, char *szString, int iBufferSize );
};


//----------------------------------
// ListBox Insert() iHow parameters \
//---------------------------------------------------------------------------
#define	LB_INSERT_END		LIT_END				// Insert at end of list
#define	LB_INSERT_SORTA 	LIT_SORTASCENDING	// Insert in ascending order
#define	LB_INSERT_SORTD 	LIT_SORTDESCENDING	// Insert in descending order


//--------------------------------------
// ListBox Selection() iFrom parameters \
//---------------------------------------------------------------------------
#define	LB_SELECT_FIRST		LIT_FIRST			// Start at first item

