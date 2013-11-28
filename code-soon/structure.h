
/* ========================================================================== */
/* -------------------------------------------------------------------------- */
/*          PARAMETERS OF THE ARCHITECTURE OF THE NETWORK                     */
/* -------------------------------------------------------------------------- */
/* ========================================================================== */
//#define     pareto             1       /* Self-similar traffic                */
//#define     dist_factor        50      /* European=50, Pan_Am=2, Japan=10     */

#define     Hurst_P            0.9     /* H=0.5-1,   alf=1-2                  */
#define     AveLength          6       /* Mess.length,if !=1 dists are active */
#define     MaxBLength         1000       /* Mess.length,if !=1 dists are active */

#define     MPR                1       /* 1 - Multiple Routing                */
#define     DIMEN              1       /* DIMEN==1, link dimenssioning        */
#define     SPR                1       /* IF we are usin SPR dimenssioning    */
#define     MaxnumAdd          5      /* Maximum number of address           */
#define     fibers_per_link    20      /* # of fibers per link                */
#define     max_links_per_node 30      /*                                     */
//#define     ADDNDL             1       /* # NDL added to the output buffer    */
#define     inputrec           1       /* Receivers at the inputs             */
#define     THENDL             0       /* Initial # of delay lines            */
#define     MaxDL              6      /* Number of delay lines  B/n_w        */
#define     max_wavelength     4       /* Number of wavelengths per fiber     */
#define     max_num_node       42     /* # of nodes                          */
#define     VirtualNode        99      /* Nodo Virtual                        */
#define     max_slots_per_link 50     /* European=100, Pan_Am=100, Japan=100 */
#define     max_num_links      120     /* European=500, Pan_Am=500, Japan=500 */
//#define     max_num_failure_links 50   /* ??????????????????????????????????? */
#define     num_fibers         1       /* # of fibers per link                */
#define     link_kind          2       /* 1=single,2=duplex,3=Msin,4=Mdup     */
//#define     load               1       /* Traffic load of the Net             */

#define     GenerationTH      1     // Time threshold to generate new links

/* ========================================================================== */
/* -------------------------------------------------------------------------- */
/*                      SIMULATION PARAMETERS                                 */
/* -------------------------------------------------------------------------- */
/* ========================================================================== */
//#define     TempoTransitorio  200    /* Transitorio           */
//#define     TempoMedio        10000 /* Mesurement time       */
//#define     TempoMax          5000   /* Mesurement time       */
//#define     hopMax            30     /* Time to live          */
//int         Optimiza;                /*                       */
float       StepG;                   /* Injected traffic load */

/* ========================================================================== */
/* -------------------------------------------------------------------------- */
/*                              NODE VARIABLES                                */
/* -------------------------------------------------------------------------- */
/* ========================================================================== */
#define         EmptyPacket     -1        /*  Empty packet                  */
#define         Alocated        -2        /* Alocated Packet = all set      */
#define         routing         0         /* 0=Random alocation             */
#define         Criterio        1         /* 0=Random Arrival, # Add. Prio. */
#define         Opzione         0         /*                                */

/* ========================================================================== */
/* -------------------------------------------------------------------------- */
/*                        WAVELENGTH VARIABLES                                */
/* -------------------------------------------------------------------------- */
/* ========================================================================== */
#define   FREE        0
#define   BLOCKED     1
#define   ALL_BLOCKED 2
#define   ONE_FREE    3

/* ========================================================================== */
/* -------------------------------------------------------------------------- */
/*              CONSTANT USED IN THE PROGRAM                                  */
/* -------------------------------------------------------------------------- */
/* ========================================================================== */
#define MAX_LINE_SIZE 256
#define MAX_CHARS 64
double var = 0.00000001 ;

float AlphaRL = 0.1;
float GammaRL = 0.9;
float EpsilonRL = 0.1;

/* ========================================================================== */
/* -------------------------------------------------------------------------- */
/*                    WAVELENGTH TRANSLATION VARIABLES                        */
/* -------------------------------------------------------------------------- */
/* ========================================================================== */


/* ========================================================================== */
/* -------------------------------------------------------------------------- */
/*                          S T R U C T U R E S                               */
/* -------------------------------------------------------------------------- */
/* ========================================================================== */
struct SPARAMETERS{
/* ========================================================================== */
/* -------------------------------------------------------------------------- */
/*          PARAMETERS OF THE ARCHITECTURE OF THE NETWORK                     */
/* -------------------------------------------------------------------------- */
/* ========================================================================== */
unsigned short int   pareto;
unsigned short int   AllowedDL;
unsigned short int   ADDNDL;

/* ========================================================================== */
/* -------------------------------------------------------------------------- */
/*                      SIMULATION PARAMETERS                                 */
/* -------------------------------------------------------------------------- */
/* ========================================================================== */
int   Initial_Time;
int   Optimization_Time;
int   Operation_Time;
float Umbral;
float load;
unsigned short int   num_waves;
unsigned short int   num_predefined_DL;
unsigned short int   SecondDL;
unsigned short int   hopMax;
};


struct NETWORK{
/* ========================================================================== */
/* -------------------------------------------------------------------------- */
/*                              NODE VARIABLES                                */
/* -------------------------------------------------------------------------- */
/* ========================================================================== */
float                  AveMess;
float                  numMess;
float                  fac[AveLength*3+1];
float                  poisson[AveLength*3+1];
unsigned short int     packnumber  [max_num_node+1][max_num_links+1][max_wavelength+1];
unsigned short int     olddest     [max_num_node+1][max_num_links+1][max_wavelength+1];
unsigned short int     BurstLength [max_num_node+1][max_num_links+1][max_wavelength+1];
unsigned long  int     FlowID      [max_num_node+1][max_num_links+1][max_wavelength+1];
unsigned long  int     Pc          [max_num_node+1][MaxnumAdd+1];  /* Percentage 1 Output */

unsigned short int       NDL [max_num_node+1][max_links_per_node+1];
unsigned short int       INL [max_links_per_node+1];
unsigned short int       StoreBuff;
unsigned long int  InputPack;
unsigned long int  OutputPack;
unsigned short int       ActiveNchan;
unsigned short int       WCactive    [max_num_node+1];
unsigned short int       MaxWCactive [max_num_node+1];
unsigned short int       OutBuffer   [max_num_node*(max_links_per_node+1)][max_wavelength+1][MaxDL+1];
//int       TransReq    [max_links_per_node+1][max_wavelength+1][MaxnumAdd+1];
unsigned short int       LCH         [max_links_per_node+1][max_wavelength+1];
unsigned short int       Outlink     [max_links_per_node+1];
unsigned short int       NA          [max_links_per_node+1][max_wavelength+1];
int       EmptySlots  [max_links_per_node+1];
float     AvePdef;
float     AvePPL;
float     AvePPL2;
float     AveDelay;
float     Pg;
short int SR [max_links_per_node+1];   /* serie del numeros aleatorios REQUEST    */
short int ST [max_wavelength+1];         /* serie del numeros aleatorios TRANSLATE  */

//unsigned long int    hop_absorption[100];
//unsigned long int    hop_distribution[100];
unsigned long int    OutlinkUsage [max_num_node+1][max_links_per_node+1];
unsigned long int    BufferUsage  [max_num_node+1][max_links_per_node+1];
unsigned long int    Drop_P_Link  [max_num_node+1][max_links_per_node+1];
unsigned long int    WCUsage      [max_num_node+1][max_links_per_node+1];
unsigned long int    WCUsageBA    [max_num_node+1][max_links_per_node+1];
unsigned long int    numdef       [max_num_node+1];  /* Number of deflections  */
unsigned long int    numPPL       [max_num_node+1];  /* Number of packet loss  */
unsigned long int    numPPL_Failures [max_num_node+1];  /* Num PL in Fibers when failures */
unsigned long int    numPPLFiber     [max_num_node+1][max_links_per_node+1];  /* Packet loss  */
unsigned long int    numMPR          [max_num_node+1];  /* Number of packet using MPR  */
unsigned long int    numLossmpr      [max_num_node+1];

unsigned short int Bfromlink     [max_num_node*(max_links_per_node+1)][max_wavelength+1][MaxDL+1];
unsigned short int Baddress      [max_num_node*(max_links_per_node+1)][max_wavelength+1][MaxDL+1];
unsigned short int Bsource       [max_num_node*(max_links_per_node+1)][max_wavelength+1][MaxDL+1];
unsigned short int Bdestination  [max_num_node*(max_links_per_node+1)][max_wavelength+1][MaxDL+1];
//unsigned long  int Bnum_slots[max_num_node*(max_links_per_node+1)][max_wavelength+1][MaxDL+1];
//unsigned long  int Bnum_hops[max_num_node*(max_links_per_node+1)][max_wavelength+1][MaxDL+1];
unsigned short int Bcell_num     [max_num_node*(max_links_per_node+1)][max_wavelength+1][MaxDL+1];
unsigned short int Bnum_of_cells [max_num_node*(max_links_per_node+1)][max_wavelength+1][MaxDL+1];
unsigned short int Bpriority     [max_num_node*(max_links_per_node+1)][max_wavelength+1][MaxDL+1];
//unsigned short int Boffset_time[max_num_node*(max_links_per_node+1)][max_wavelength+1][MaxDL+1];

int         link_size[max_num_links + 1];
int         n_links;
int         num_initial_links;
int         n_fibers;             /* Number of Fiber links in the network */

/* ========================================================================== */
/*   ----                      NETWORK VARIABLES                        ----  */
/* ========================================================================== */

unsigned short int connected_to   [max_num_node+1][max_links_per_node+1];
unsigned short int connected_from [max_num_node+1][max_links_per_node+1];
unsigned short int num_of_link    [max_num_node+1][max_num_node+1][fibers_per_link+1];
unsigned short int num_inj_links  [max_num_node+1];
unsigned short int output_links   [max_num_node+1];
unsigned short int input_links    [max_num_node+1];
unsigned short int num_input_link [max_num_node+1][max_links_per_node+1];
unsigned short int num_output_link[max_num_node+1][max_links_per_node+1];
unsigned short int num_of_fibers  [max_num_node+1][max_num_node+1];
unsigned short int link_dist      [max_num_node+1][max_num_node+1];
unsigned short int source         [max_num_links+1][max_wavelength+1][max_slots_per_link+1];
unsigned short int destination    [max_num_links+1][max_wavelength+1][max_slots_per_link+1];
//unsigned long  int num_slots[max_num_links+1][max_wavelength+1][max_slots_per_link+1];
//unsigned long  int num_hops[max_num_links+1][max_wavelength+1][max_slots_per_link+1];
unsigned short int cell_num       [max_num_links+1][max_wavelength+1][max_slots_per_link+1];
unsigned short int num_of_cells   [max_num_links+1][max_wavelength+1][max_slots_per_link+1];
unsigned short int priority       [max_num_links+1][max_wavelength+1][max_slots_per_link+1];
//unsigned short int offset_time[max_num_links+1][max_wavelength+1][max_slots_per_link+1];
int dist_factor; //unsigned short int dist_factor;

float             g[max_wavelength+1];
unsigned long int tempo;
unsigned long int tempoT;
//unsigned long int numHops[max_num_node+1][max_links_per_node+1][max_wavelength+1];
//unsigned long int numSlots[max_num_node+1][max_links_per_node+1][max_wavelength+1];
unsigned long int numFullIN [max_num_node+1][max_links_per_node+1][max_wavelength+1];
unsigned long int numAbs    [max_num_node+1][max_links_per_node+1][max_wavelength+1];
unsigned long int numInj    [max_num_node+1][max_links_per_node+1][max_wavelength+1];
float             u[max_num_node+1][max_num_links+1];
float             a[max_num_node+1][max_num_links+1];
float             D[max_num_node+1][max_num_links+1];

/* ========================================================================== */
/*   ----                      TOPOLOGY VARIABLES                       ----  */
/* ========================================================================== */

int xmin, xmax, ymin, ymax;         /* Network dimenssions                    */
int nbr_of_nodes;                   /* Number of nodes in the network         */
int nbr_of_edges;                   /* Number of links in the network         */
int num_outputs [max_num_node+1][max_num_node+1];     /* # of outputs          */
int forw_table  [max_num_node+1][max_num_node+1][fibers_per_link+1];  /* Forwarding table */
int forw_output [max_num_node+1][max_num_node+1][fibers_per_link+1]; /* Forwarding table */

int new_outputs     [max_num_node+1][max_num_node+1];                     /* # of outputs     */
int new_ftable      [max_num_node+1][max_num_node+1][fibers_per_link+1];  /* Forwarding table */
int new_foutput     [max_num_node+1][max_num_node+1][fibers_per_link+1];  /* Forwarding table */

int bak_forw_output [max_num_node+1][max_num_node+1][fibers_per_link+1]; /* F.  table    */
int bak_forw_table  [max_num_node+1][max_num_node+1][fibers_per_link+1];  /* F. table     */
//float cost_db[max_num_node+1][max_num_node+1] ;

float MyPolicy[max_num_node+1][max_num_node+1][fibers_per_link+1];
//float MyTrace [max_num_node+1][max_num_node+1][fibers_per_link+1];

int num_in_packets;
int num_out_packets;
//int num_abs_packets[max_num_node+1];

int consecutive_link[max_num_links/2][2];

/* ========================================================================== */
/*   ----                            FAILURES                           ----  */
/* ========================================================================== */
// Failure_Wave TimeIni TimneFin source target Fiber Wavelength
// int Failure_Waves [Operation_Time][6]; ????
int Failure_Waves [7000][8];
int Link_Fibers[max_num_links]; // Link [so-called fiber] [Initial fiber]
// Link_Fibers[x][y]
// x -fiber, y - initial fibers
// Link_Fibers[40]  = 40
// Link_Fibers[188] = 40
// Link_Fibers[215] = 40, it means 40, 188 and 215 all are connected to the same nodes.

int Phase;

// NEW THINGS ABOUT FLOWS
unsigned long int num_of_flow  [max_num_links+1][max_wavelength+1][max_slots_per_link+1];
unsigned long int Bnum_of_flow [max_num_node*(max_links_per_node+1)][max_wavelength+1][MaxDL+1];

unsigned long int Current_Flow  [max_num_node+1][max_num_node+1][3];
int Size_Flow     [max_num_node+1][max_num_node+1][3];
int TrackEmptyS   [max_num_node+1][max_num_node+1][3];
int PacketArrived [max_num_node+1][max_num_node+1][3][MaxBLength];
unsigned long int FromFlow      [max_num_node+1][max_num_node+1][3][MaxBLength];
int ArrivedSeq    [max_num_node+1][max_num_node+1][3][MaxBLength]; // Will contain negative values
unsigned long int GlobalRDP [MaxBLength];
unsigned long int GlobalRDN [MaxBLength];
unsigned long int MyFlowID;

// RL
int Num_at;
int Num_First;
int Num_Neg;
int Num_Pos;

int Myat;
int MyActions[8];

// Lightpath Requests
int dw;
int SendPK[7][4][6];
int AbsPK [7][4][6];
int LostPK[7][4][6];
int FreeW;
int ThisLoad;

int TimeGeneration[max_num_node+1][max_num_node+1];

int MaxBURST;
};

