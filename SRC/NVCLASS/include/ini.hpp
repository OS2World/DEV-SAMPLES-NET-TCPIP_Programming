//-------
// C_INI \
//---------------------------------------------------------------------------
//	All information contained in this file is Copyright (c) 1994-1995 by
//	NeoLogic Inc.  All Rights Reserved
//
//	This source code shall not be implemented, duplicated or used as the basis
//	for any product without prior written consent by NeoLogic Inc.
//
class C_INI
{
	protected:
		char	szIniFile[256];		// Filename of INI file
		char	szAppName[256];		// Application name of program
		HINI	hIni;				// INI handle

	public:
		_Export C_INI( char *szFile, char *szAppName );
		void _Export	Open( void );
		void _Export	Close( void );
		void _Export	Read(char *szField, char *szData, char *szDefault, int iLength);
		void _Export	Write( char *szField, char *szData );
};

