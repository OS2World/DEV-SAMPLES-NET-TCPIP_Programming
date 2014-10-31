//-------------
// TBARTOP.CPP \
//---------------------------------------------------------------------------
//
//	All information contained in this file is Copyright (c) 1994-1995 by
//	Steven Gutz.  All Rights Reserved
//
//	This source code shall not be implemented, duplicated or used as the basis
//	for any product without prior written consent by the author
//
//---------------------------------------------------------------------------

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
		DECLARE_BUTTON( DB_CONFIG, DB_CONFIG_UP, DB_CONFIG_DN, 0,
							"Configure the program", 8, 4 )
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
		DECLARE_BUTTON_CMD( DB_CONFIG, DM_CONFIGURE )
	END_BUTTON_CMD_TABLE


	// Call the parent controller to process the items
	C_TOOLBAR::Control( mp1, xtCommandLookup );
}

