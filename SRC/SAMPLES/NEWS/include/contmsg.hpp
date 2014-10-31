class C_CONTAINER_MSG : public C_CONTAINER
{
	public:
		C_CONTAINER_MSG( C_WINDOW *pxcParentObj );
		~C_CONTAINER_MSG( void );
		void	RemoveAll( void );
		void	Remove( void *pvData );
		void 	*Fill( void *pRecord, char *szString );
		void	Columns( void );
};

// Record structure for subscription container records
typedef struct
{
	RECORDCORE	RecordCore;
	PSZ			szNumber;		// Message number
	PSZ			szLines;		// Line count
	PSZ			szSubject;		// Message subject
	PSZ			szAuthor;		// Message Author
} T_MSGRECORD;


#define		D_CONT_MSG		100

