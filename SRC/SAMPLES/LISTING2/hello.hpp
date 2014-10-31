class C_WINDOW_APP : public C_WINDOW_STD
{
	public:
		C_WINDOW_APP( void );
		void *	MsgCreate( void *mp1, void *mp2 );
		void *	MsgNextStep( void *mp1, void *mp2 );
		void *	Paint( void *mp1, void *mp2 );
};

