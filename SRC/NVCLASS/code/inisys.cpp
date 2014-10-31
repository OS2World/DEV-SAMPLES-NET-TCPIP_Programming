//------------
// C_INI_SYSTEM \
//---------------------------------------------------------------------------
//
//	All information contained in this file is Copyright (c) 1994-1995 by
//	NeoLogic Inc.  All Rights Reserved
//
//	This source code shall not be implemented, duplicated or used as the basis
//	for any product without prior written consent by NeoLogic Inc.
//
// Description:
//
//	This file contains the methods to describe the C_INI_SYSTEM class.
//	C_INI_SYSTEM provides an interface to the operating system profile data
//	area specific to user programs (OS2.INI).
//
//---------------------------------------------------------------------------

//-------------------
// OS/2 Conditionals \
//---------------------------------------------------------------------------
#define INCL_DOS
#define INCL_WIN

//------------------
// Standard Headers \
//---------------------------------------------------------------------------
#include <os2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//-----------------
// NVCLASS Headers \
//---------------------------------------------------------------------------
#include <ini.hpp>
#include <inisys.hpp>

//-------------
// Constructor \
//---------------------------------------------------------------------------
//
//	Parameters:
//		szName				- Name of application
//
//	Returns:
//				none
//
C_INI_SYSTEM::C_INI_SYSTEM( char *szName ) : C_INI( "", szName )
{
}


//------
// Open \
//---------------------------------------------------------------------------
// Description:
//		This method will open the OS2.INI File associated with this
//		instance.
//
//	Parameters:
//		none
//
//	Returns:
//				none
//
void C_INI_SYSTEM::Open( void )
{
	// We don't need to open the OS2.INI file, we can just use it
	hIni = HINI_SYSTEMPROFILE;
}

