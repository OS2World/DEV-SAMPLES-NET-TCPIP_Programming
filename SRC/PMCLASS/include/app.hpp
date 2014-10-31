//---------------
// C_APPLICATION \
//---------------------------------------------------------------------------
//	All information contained in this file is Copyright (c) 1994-1995 by
//	NeoLogic Inc.  All Rights Reserved
//
//	This source code shall not be implemented, duplicated or used as the basis
//	for any product without prior written consent by NeoLogic Inc.
//

//--------------------------------
// C_APPLICATION class definition \
//---------------------------------------------------------------------------
class C_APPLICATION
{
	private:
		HAB		hAB;		// Handle to application's anchor block
		HMQ		hMQ;		// Handle to application's message queue

	public:
		_Export	C_APPLICATION( void );
		_Export	~C_APPLICATION( void );
		void	_Export	Run( void );
		HAB		_Export	AnchorBlock( void );

		// System Metrics
		int		_Export	DesktopHeight( void );
		int		_Export	DesktopWidth( void );
		int		_Export	MenuHeight( void );
		int		_Export	TitleBarHeight( void );
		int		_Export	DialogBorderHeight( void );
};
