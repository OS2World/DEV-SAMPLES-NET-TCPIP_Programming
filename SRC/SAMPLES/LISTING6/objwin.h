//----------------------
// Internal Definitions \
//------------------------------------------------------------------------
#define		D_APPNAME		"MinimalPMApp"
#define		D_ID_WINDOW		1

#define		WM_SLEEP		WM_USER + 1

#define		DM_DO_SLEEP		100


typedef struct
{
	HWND	hWndFrame;
	HWND	hWnd;
	HWND	hWndObj;
} T_SHARE;

//---------------------
// Function Prototypes \
//------------------------------------------------------------------------
MRESULT EXPENTRY ObjWndProc( HWND hWnd, ULONG msg, MPARAM mp1, MPARAM mp2 );
MRESULT EXPENTRY WndProc( HWND hWnd, ULONG msg, MPARAM mp1, MPARAM mp2 );
void _Optlink ThreadObject( void *pvData );

