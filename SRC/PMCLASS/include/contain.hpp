//-------------
// C_CONTAINER \
//---------------------------------------------------------------------------
//	All information contained in this file is Copyright (c) 1994-1995 by
//	NeoLogic Inc.  All Rights Reserved
//
//	This source code shall not be implemented, duplicated or used as the basis
//	for any product without prior written consent by NeoLogic Inc.
//


//----------------------------------
// View definitions for a container \
//---------------------------------------------------------------------------
#define		D_VIEW_NONE				-1		// Invisible (no view)
#define		D_VIEW_DETAIL_TITLE		0		// Detail view with titles
#define		D_VIEW_DETAIL			1		// Detail view - no titles
#define		D_VIEW_TREE				2		// Tree View
#define		D_VIEW_ICON				3		// Icon View (Currently unsupported)


//------------------------------
// C_CONTAINER class definition \
//---------------------------------------------------------------------------
class C_CONTAINER : public C_WINDOW_CHILD
{
	private:
		int		iView;		// Display view

	public:
		_Export	C_CONTAINER( C_WINDOW *pxcParentObj, int iID, int iView,
													int iFlags, int iMode );
		_Export	C_CONTAINER( C_WINDOW *pxcParentObj, int iID, int iView,
																int iFlags );
		_Export	C_CONTAINER( C_DIALOG *pxcParentObj, int iID, int iView,
																int iFlags );
		_Export	~C_CONTAINER( void );
		void _Export 	Setup( int iView, int iFlags );
		void * _Export	Allocate( ULONG iLength, USHORT iCtr );
		void _Export	Insert( void *pParent, void *pRecord, int iCount, int iUpdate );
		void _Export	Insert( void *pParent, void *pRecord, int iCount );
		void _Export	InsertUpdate( void *pParent, void *pRecord, int iCount );
		void _Export	Remove( void );
		void _Export	Remove( void*pvData, short iCount );
		void _Export	Redraw( void *pRecord );
		void _Export	Sort( void * );
		void * _Export	Search( void *pStart, char *szString, unsigned int iType );
		void * _Export	ParentRecord( void *hCurrent );
		void * _Export	FirstRecord( void );
		void * _Export	MemoryFirstRecord( void );
		void * _Export	MemoryNextRecord( void *hCurrent );
		void * _Export	NextRecord( void *hCurrent );
		void * _Export	PreviousRecord( void *hCurrent );
		void * _Export	FirstChild( void *hParent );
		void * _Export	LastChild( void *hParent );
		void * _Export	FirstSelected( void );
		void * _Export	NextSelected( void *hCurrent );
		void _Export	ExpandTree( void *pRecord );
		void _Export	CompressTree( void *pRecord );
		void _Export	SelectRecord( void *pRecord, short sBool );
};


// Structure used by icon and tree views to hold icon resources
typedef struct
{
	char		szType[2];
	int			iResource;
	HPOINTER	hIcon;
} T_LOOKUP;

