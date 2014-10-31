//-------
// C_INI \
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
//	This file contains the methods to describe the C_INI class.  C_INI provides
//	an interface to the operating system profile data area.  In OS/2 this is in
//	the form of an .INI system file.  These files can be used to store any user
//	definable parameters which need to be maintained to preserve the program
//	configuration.
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
C_INI::C_INI( char *szFile, char *szName )
{
	// Save the supplied data within the class attributes
	strcpy( szIniFile, szFile );
	strcpy( szAppName, szName );
}


//------
// Open \
//---------------------------------------------------------------------------
// Description:
//		This method will open the INI File associated with this instance.
//
//	Parameters:
//		none
//
//	Returns:
//				none
//
void C_INI::Open( void )
{
	// OPen the INI file saving the handle to the file
	hIni = PrfOpenProfile( 0, (PSZ)szIniFile );
}


//-------
// Close \
//---------------------------------------------------------------------------
// Description:
//		This method will close the INI File associated with this instance.
//
//	Parameters:
//		none
//
void C_INI::Close( void )
{
	if( strlen( szIniFile ) )
	{
		// Close the INI file
		PrfCloseProfile( hIni );
	}
}


//------
// Read \
//---------------------------------------------------------------------------
// Description:
//		This method will read a string from the INI file as specified by
//		the supplied keyword.  If the value is not extracted from the INI
//		file, then the supplied default will be returned.
//
//	Parameters:
//		szField			- Keyword to be fetched
//		szData			- Pointer to buffer where data from INI will be written
//		szDefault		- Pointer to the default string
//		iLength			- Maximum size of the output buffer
//
//	Returns:
//				none
//
void C_INI::Read( char *szField, char *szData, char *szDefault, int iLength )
{
	// Query the INI file for a string
	PrfQueryProfileString(hIni, (PSZ)szAppName, (PSZ)szField,
									(PSZ)szDefault, (PSZ)szData, iLength );
}


//-------
// Write \
//---------------------------------------------------------------------------
// Description:
//		This method will write a string to the INI file as specified by
//		the supplied parameters.
//
//	Parameters:
//		szField			- Keyword to be fetched
//		szData			- Pointer to buffer where data from INI will be written
//
//	Returns:
//				none
//
void C_INI::Write(char *szField, char *szData )
{
	// Write the supplied string to the INI file
	PrfWriteProfileString( hIni, (PSZ)szAppName, (PSZ)szField, (PSZ)szData );
}

