//-------------
// TBARSUB.CPP \
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
#include <dialog.hpp>
#include <status.hpp>
#include <button.hpp>
#include <tbar.hpp>
#include <mle.hpp>
#include <log.hpp>

//---------------------
// Application Headers \
//---------------------------------------------------------------------------
#include <tbarsub.hpp>
#include "rc.hpp"


//-------------
// Constructor \
//---------------------------------------------------------------------------
C_TOOLBAR_SUB::C_TOOLBAR_SUB( C_WINDOW *pxcParentObj, C_STATUS *pxcStatus )
								: C_TOOLBAR( pxcParentObj, D_SUB_TBAR, 40 )
{
	DECLARE_BUTTON_TABLE( xtButtons )
		DECLARE_BUTTON( DB_UNSUBSCRIBE, DB_UNSUB_UP, DB_UNSUB_DN, 0,
							"Unsubscribe currently selected groups", 8, 4 )
	END_BUTTON_TABLE


	// Set the status bar object used by the toolbar
	Status( pxcStatus );

	// Add some toolbar buttons
	CreateButtons( xtButtons );
}


//---------
// Control \
//---------------------------------------------------------------------------
void C_TOOLBAR_SUB::Control( ULONG mp1 )
{
	// Button-Command cross reference
	DECLARE_BUTTON_CMD_TABLE( xtCommandLookup )
		DECLARE_BUTTON_CMD( DB_UNSUBSCRIBE, DM_SUB_UNSUBSCRIBE )
	END_BUTTON_CMD_TABLE


	// Call the parent controller to process the items
	C_TOOLBAR::Control( mp1, xtCommandLookup );
}

