//-------------------
// OS/2 Conditionals \
//---------------------------------------------------------------------------
#define INCL_DOS
#define INCL_WIN
#define INCL_GPI

//------------------
// Standard Headers \
//---------------------------------------------------------------------------
#include <os2.h>
#include <stdio.h>
#include <stdlib.h>

//-----------------
// PMCLASS Headers \
//---------------------------------------------------------------------------
#include <app.hpp>
#include <window.hpp>
#include <winstd.hpp>
#include <winchild.hpp>
#include <status.hpp>
#include <button.hpp>
#include <tbar.hpp>

//---------------------
// Application Headers \
//---------------------------------------------------------------------------
#include <tbartop.hpp>
#include "rc.hpp"


//-------------
// Constructor \
//---------------------------------------------------------------------------
C_TOOLBAR_TOP::C_TOOLBAR_TOP( C_WINDOW *pxcParentObj, C_STATUS *pxcStatus )
								: C_TOOLBAR( pxcParentObj, D_TOP_TBAR, 40 )
{
	DECLARE_BUTTON_TABLE( xtButtons )
		DECLARE_BUTTON( DB_OPEN, DB_OPEN_UP, DB_OPEN_DN, 0,
							"Open a file", 8, 4 )
		DECLARE_BUTTON( DB_SAVE, DB_SAVE_UP, DB_SAVE_DN, 0,
							"Save a file", 40, 4 )
		DECLARE_BUTTON( DB_CUT, DB_CUT_UP, DB_CUT_DN, 0,
							"Cut selected text", 80, 4 )
		DECLARE_BUTTON( DB_COPY, DB_COPY_UP, DB_COPY_DN, 0,
							"Copy selected text", 112, 4 )
		DECLARE_BUTTON( DB_PASTE, DB_PASTE_UP, DB_PASTE_DN, 0,
							"Paste text from the clipboard", 144, 4 )
		DECLARE_BUTTON( DB_FIND, DB_FIND_UP, DB_FIND_DN, 0,
						"Display the text search/replace dialog", 184, 4 )
	END_BUTTON_TABLE


	// Set the status bar object used by the toolbar
	Status( pxcStatus );

	// Add some toolbar buttons
	CreateButtons( xtButtons );
}


//---------
// Control \
//---------------------------------------------------------------------------
void C_TOOLBAR_TOP::Control( ULONG mp1 )
{
	// Button-Command cross reference
	DECLARE_BUTTON_CMD_TABLE( xtCommandLookup )
		DECLARE_BUTTON_CMD( DB_OPEN, DM_FILE_OPEN )
		DECLARE_BUTTON_CMD( DB_SAVE, DM_FILE_SAVE )
		DECLARE_BUTTON_CMD( DB_CUT, DM_EDIT_CUT )
		DECLARE_BUTTON_CMD( DB_COPY, DM_EDIT_COPY )
		DECLARE_BUTTON_CMD( DB_PASTE, DM_EDIT_PASTE )
		DECLARE_BUTTON_CMD( DB_FIND, DM_EDIT_FIND )
	END_BUTTON_CMD_TABLE


	// Call the parent controller to process the items
	C_TOOLBAR::Control( mp1, xtCommandLookup );
}

