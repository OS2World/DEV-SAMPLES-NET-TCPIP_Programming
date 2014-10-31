#define	INCL_DOS
#define	INCL_PM

#include <os2.h>
#include <stdio.h>


//-----------------
// PMCLASS Headers \
//---------------------------------------------------------------------------
#include <app.hpp>
#include <window.hpp>
#include <winstd.hpp>
#include <winchild.hpp>
#include <dialog.hpp>
#include <edit.hpp>
#include <pushbtn.hpp>
#include <menu.hpp>
#include <status.hpp>
#include <button.hpp>
#include <tbar.hpp>
#include <mle.hpp>
#include <contain.hpp>
#include <log.hpp>

#include <net.hpp>
#include <netftp.hpp>

int main( void )
{
	int		iPort = 21;						// FTP Port
	int		iResult;
	char	*szServer = "ftp.cdrom.com";	// Example server


	// Create an FTP network object
	C_CONNECT_FTP	xcFTPClass( "ftp.cdrom.com", iPort, NULL );

	// Open a connection to the FTP server
	if( xcFTPClass.Open() )
	{
		//
		// At this point we are connected and ready to go.
		//
		printf( "Connection successful!!\n" );

		iResult = xcFTPClass.SYST();
		printf( "SYST Response = %d", iResult );

		iResult = xcFTPClass.QUIT();
		printf( "Quit Response = %d", iResult );

		// Close the connection
		xcFTPClass.Close();
	}

	return 0;
}
