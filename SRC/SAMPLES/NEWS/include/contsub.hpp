class C_CONTAINER_SUB : public C_CONTAINER
{
	public:
		C_CONTAINER_SUB( C_WINDOW *pxcParentObj );
		~C_CONTAINER_SUB( void );
		void	RemoveAll( void );
		void	Remove( void *pvData );
		void 	*Fill( void *pRecord, char *szString );
		void	Columns( void );
};

// Record structure for subscription container records
typedef struct
{
	RECORDCORE	RecordCore;
	PSZ			szGroup;		// Full name of the group
	PSZ			szTotal;		// Total number of articles as a string
} T_SUBRECORD;


#define		D_CONT_SUB		100

