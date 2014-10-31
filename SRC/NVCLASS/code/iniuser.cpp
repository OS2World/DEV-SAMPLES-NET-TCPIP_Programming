//------------
// C_INI_USER \
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
//	This file contains the methods to describe the C_INI_USER class.
//	C_INI_USER provides an interface to the operating system profile data
//	area specific to user programs (OS2USER.INI).
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
#include <iniuser.hpp>

//-------------
// Constructor \
//---------------------------------------------------------------------------
//
//	Parameters:
//		szFile				- Full path name of INI file
//		szName				- Name of application
//
//	Returns:
//				none
//
C_INI_USER::C_INI_USER( char *szName ) : C_INI( "", szName )
{
}


//------
// Open \
//---------------------------------------------------------------------------
// Description:
//		This method will open the OS2USER.INI File associated with this
//		instance.
//
//	Parameters:
//		none
//
//	Returns:
//				none
//
void C_INI_USER::Open( void )
{
	// We don't need to open the OS2USER.INI file, we can just use it
	hIni = HINI_USERPROFILE;
}

