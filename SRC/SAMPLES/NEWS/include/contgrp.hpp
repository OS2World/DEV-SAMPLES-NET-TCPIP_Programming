class C_CONTAINER_GRP : public C_CONTAINER
{
	public:
		C_CONTAINER_GRP( C_WINDOW *pxcParentObj );
		~C_CONTAINER_GRP( void );
		void	RemoveAll( void );
		void 	*Fill( void *pRecord, char *szString );
		void	Columns( void );
};

// Record structure for subscription container records
typedef struct
{
	RECORDCORE	RecordCore;
	PSZ			szString;		// Full name of the group
} T_GRPRECORD;


#define		D_CONT_GRP		100

extern "C"
{
	SHORT APIENTRY SortGroupByAlpha( T_GRPRECORD *r1, T_GRPRECORD *r2, PVOID pvStorage );
}

