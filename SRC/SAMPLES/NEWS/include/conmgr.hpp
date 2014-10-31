class C_CONNECT_MGR
{
	private:
		int				iConnections;		// Max # of connections
		int				iConnectionCount;	// # of idle connections
		char			szNewsServer[256];
		int				iNewsPort;
		C_SEM_EVENT		hSemConMgr;

	public:
		C_CONNECT_NEWS	*pxcConnect;

		C_CONNECT_MGR( int iConnections, char *szServer, int iPort );
		C_CONNECT_MGR( void );
		~C_CONNECT_MGR( void );
		void	Close( int iConnection );
		void	Initialize( int iConCount, char *szServer, int iPort );
		int		Connect( void );
		void 	Disconnect( int iConnect );
		C_CONNECT_NEWS *Connection( int iConnect )
												{ return pxcConnect+iConnect; };
		int		FreeConnections( void );
		char	*Server( void )					{ return szNewsServer; };
		int		Port( void )					{ return iNewsPort; };
		int		Connections( void )				{ return iConnectionCount; };
		int		Connection( void )				{ return iConnectionCount; };
		int		MaxConnections( void )			{ return iConnections; };
		void	IncrementConnectionCount( void ){ iConnectionCount++; };
};

extern C_CONNECT_MGR	*pxcMgr;

