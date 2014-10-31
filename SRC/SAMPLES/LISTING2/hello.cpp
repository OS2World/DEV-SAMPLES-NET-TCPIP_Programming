#define	INCL_DOS
#define	INCL_WIN

#include <os2.h>
#include <app.hpp>
#include <window.hpp>
#include <winstd.hpp>

DECLARE_MSG_TABLE( xcMsg )
	DECLARE_MSG( WM_PAINT, C_WINDOW_STD::MsgPaint )
END_MSG_TABLE


void main( void )
{
	C_APPLICATION	xcApp;
	C_WINDOW_STD	xcWindow( xcMsg );


	// Register and create a new program window
	xcWindow.Register( "TestClass" );

	// Set the window characteristics
	xcWindow.WCF_SizingBorder();
	xcWindow.WCF_SysMenu();
	xcWindow.WCF_TaskList();
	xcWindow.WCF_ShellPosition();
	xcWindow.WCF_MinButton();
	xcWindow.WCF_MaxButton();
	xcWindow.WCF_TitleBar();

	xcWindow.Create( 1, "Minimal PMCLASS Program" );
	xcWindow.Show();

	// Start the message loop
	xcApp.Run();
}
