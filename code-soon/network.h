/* ============================================================== */
/* -------------------------------------------------------------- */
/*                FUNCTION DEFINITION                             */
/* -------------------------------------------------------------- */
void Simula(struct NETWORK *NTW, struct SPARAMETERS *SM_Parameters);
void SimulaNet(struct NETWORK *NTW, struct SPARAMETERS *SM_Parameters);
void InicializaVariablesStadisticas (struct NETWORK *NTW, struct SPARAMETERS *SM_Parameters);

/* ---------------- S I M U L A   N E T  ------------------------ */
void Utilization (struct NETWORK *NTW, struct SPARAMETERS *SM_Parameters);
void Generation(struct NETWORK *NTW, struct SPARAMETERS *SM_Parameters);
void SimulaNode(struct NETWORK *NTW, struct SPARAMETERS *SM_Parameters);
void LinkSimulation(struct NETWORK *NTW, struct SPARAMETERS *SM_Parameters);
void ErrorPropagation(struct NETWORK *NTW, struct SPARAMETERS *SM_Parameters);
void StatusPropagation(struct NETWORK *NTW, struct SPARAMETERS *SM_Parameters);

/* -------------- S I M U L A    N O D E  ----------------------- */
void Initialization(int ndr, struct NETWORK *NTW, struct SPARAMETERS *SM_Parameters);
void EmptyBuffers (int ndr, struct NETWORK *NTW, struct SPARAMETERS *SM_Parameters);
void UpdateBuffers (int ndr, struct NETWORK *NTW, struct SPARAMETERS *SM_Parameters);
void SendError (int ndr, struct NETWORK *NTW, struct SPARAMETERS *SM_Parameters);
void Transmission(int ndr, struct NETWORK *NTW, struct SPARAMETERS *SM_Parameters);
void EmptySlotsinBuffers (int ndr, struct NETWORK *NTW, struct SPARAMETERS *SM_Parameters);
void Translation (int prioridad, int ndr, struct NETWORK *NTW, struct SPARAMETERS *SM_Parameters);

void BFassignment(int ndr, int Olink, int wave, int buffer, struct NETWORK *NTW, struct SPARAMETERS *SM_Parameters, int Olink2);
void Absorption (int ndr,int link,int Olink,int wave,int dest,int LoB, struct NETWORK *NTW, struct SPARAMETERS *SM_Parameters);
void FTallocation(int ndr, int link, int Olink, int Naddress, int wave, int wave_t, struct NETWORK *NTW, struct SPARAMETERS *SM_Parameters);
void BFallocation(int ndr, int link, int Olink, int wave, int wave_t,int buffer, int Naddress, struct NETWORK *NTW, struct SPARAMETERS *SM_Parameters);
void more_links (int ndr, int destNET, int Olink, struct NETWORK *NTW, struct SPARAMETERS *SM_Parameters);
void Discard (int ndr, int link,int wave, struct NETWORK *NTW, struct SPARAMETERS *SM_Parameters, int dest);
void Learning(int ndr, int link,int wave, struct NETWORK *NTW, struct SPARAMETERS *SM_Parameters, int dest, int Reward);
void Update_Transmission(int ndr, struct NETWORK *NTW, struct SPARAMETERS *SM_Parameters);

void read_failures(struct NETWORK *NTW, struct SPARAMETERS *SM_Parameters);

void Update_table(struct NETWORK *NTW, int ndr, struct SPARAMETERS *SM_Parameters);

int  *build_array(int size);
int **build_matrix(int m, int n);

void Stampa(struct NETWORK *NTW, struct SPARAMETERS *SM_Parameters);
void RandomSeries (int ndr, struct NETWORK *NTW, struct SPARAMETERS *SM_Parameters);

/* ------------------ F A I L U R E S  -------------------------- */
int search_link_failure(struct NETWORK * NTW, int link, struct SPARAMETERS *SM_Parameters, int wave, int dest, int ndr);
int search_wave_failure(struct NETWORK * NTW, int fiber, int wave, int Olink, struct SPARAMETERS *SM_Parameters, int dest, int ndr);
int get_wave_status(struct NETWORK * NTW, int fiber, int wave, struct SPARAMETERS *SM_Parameters, int dest, int ndr);

int find_initial_fiber(struct NETWORK * NTW, int fiber, struct SPARAMETERS *SM_Parameters);

/* ------------------ SIGNALING --------------------------------- */
void SendStatus (int ndr, struct NETWORK *NTW, struct SPARAMETERS *SM_Parameters);
float get_free_wavelengths(int ndr, int linkout, struct NETWORK *NTW, struct SPARAMETERS *SM_Parameters);
void update_priority(int ndr, int source, float free, struct NETWORK *NTW, struct SPARAMETERS *SM_Parameters);

int get_node_status(struct NETWORK * NTW, int node, struct SPARAMETERS *SM_Parameters);
void process_priorities (int ndr, struct NETWORK *NTW, struct SPARAMETERS *SM_Parameters);

void BFassignmentPriority(int ndr, int Olink, int wave, int buffer, struct NETWORK *NTW, struct SPARAMETERS *SM_Parameters, int Olink2);
void EmptyBuffersPriority (int ndr, struct NETWORK *NTW, struct SPARAMETERS *SM_Parameters);

void print_source_dest(struct NETWORK * NTW, struct SPARAMETERS *SM_Parameters);

float computePolicyChange(struct NETWORK * NTW, struct SPARAMETERS *SM_Parameters);

void more_delays(struct NETWORK * NTW, struct SPARAMETERS *SM_Parameters, int ndr, int dest, int MyFiber);

void computeINJ(struct NETWORK * NTW, struct SPARAMETERS *SM_Parameters);

// =================== Packet Reordering !!!!!!!
void computeRD (int ndr, int src, unsigned long int num_flow, int size_flow, int num_packet, struct NETWORK *NTW, struct SPARAMETERS *SM_Parameters, int LoB);
void add2GlobalRD(int ndr, int src, int this_flow, struct NETWORK *NTW, struct SPARAMETERS *SM_Parameters);

unsigned long int generateFlowID(int ndr, struct NETWORK *NTW, struct SPARAMETERS *SM_Parameters);

/* ============================================================== */
/*             Simula                                             */
/* ============================================================== */
/*  Start the Simulation                                          */
/* -------------------------------------------------------------- */
void Simula(struct NETWORK *NTW, struct SPARAMETERS *SM_Parameters)
{
   int  wave;
   NTW->num_initial_links = NTW->n_links;
   NTW->ThisLoad = 0;
   NTW->MaxBURST = 0;
   NTW->MyFlowID = 0;

   // ===================================================================
   //                      D I M E N S I O N I N G
   // ===================================================================
   printf("\n\n\t\t D I M E N S I O N I N G \n\n");
   fileout      = fopen("Statistics-D.txt","w");
   fileout2     = fopen("Error-D.txt","w");
   fUtilization = fopen("Utilization-D.txt","w");


   printf ("Tiempo transitorio \n") ;
   NTW->Phase = 1;
   NTW->tempoT = 1;

   printf("Inicializacion de variables Stadisticas()\n");
   InicializaVariablesStadisticas (NTW, SM_Parameters);
   printf("Termino Inicializacion de variables Stadisticas()\n");

   for(NTW->tempo = 1;NTW->tempo <= SM_Parameters->Initial_Time; NTW->tempo++)
   {
      printf ("\ntempo (%d) - Phase = %d", (int) NTW->tempo, NTW->Phase ) ;
      SimulaNet(NTW, SM_Parameters);
      Export_Results(NTW, SM_Parameters);
   }
   //if(NTW->tempo == SM_Parameters->Operation_Time)
   //Print_Utilization(NTW,SM_Parameters);

   NTW->Phase = 2;
   printf("Inicializacion de variables Stadisticas()\n");
   InicializaVariablesStadisticas (NTW, SM_Parameters);
   printf("Termino Inicializacion de variables Stadisticas()\n");

   /* ------------------------------------------------ */
   /*                Read Failures                     */
   /* ------------------------------------------------ */
   printf ("Reading Failures at file: Requests-CT-D \n") ;
   if ((f1 = fopen("Requests-CT-D","r")) == NULL)
   {
      printf ("Unable to read the parameter file Failures \n") ;
      exit (-1);
   }
   read_failures(NTW, SM_Parameters) ;
   fclose(f1);

   printf ("Tiempo de Optimizacion \n") ;
   for(wave = 1; wave <= SM_Parameters->num_waves; wave++)
      NTW->g[wave] = SM_Parameters->load;
   NTW->Phase = 2;
   for(NTW->tempo = 1; NTW->tempo <= SM_Parameters->Optimization_Time; NTW->tempo++)
   {

      SimulaNet(NTW, SM_Parameters);
      printf ("\ntempo (%d) - Phase = %d", (int) NTW->tempo, NTW->Phase ) ;
      Export_Results(NTW, SM_Parameters);
   }
   //if(NTW->tempo == SM_Parameters->Operation_Time)
   //Print_Utilization(NTW,SM_Parameters);

   printf("Inicializacion de variables Stadisticas()\n");
   InicializaVariablesStadisticas (NTW, SM_Parameters);
   printf("Termino Inicializacion de variables Stadisticas()\n");

   // Second round DIMENSIONING
   SM_Parameters->AllowedDL = SM_Parameters->SecondDL;
   printf ("Tiempo de Optimizacion \n") ;
   for(wave = 1; wave <= SM_Parameters->num_waves; wave++)
      NTW->g[wave] = SM_Parameters->load;
   NTW->Phase = 2;
   for(NTW->tempo = 1; NTW->tempo <= SM_Parameters->Optimization_Time; NTW->tempo++)
   {

      SimulaNet(NTW, SM_Parameters);
      printf ("\ntempo (%d) - Phase = %d", (int) NTW->tempo, NTW->Phase ) ;
      Export_Results(NTW, SM_Parameters);
   }
   //if(NTW->tempo == SM_Parameters->Operation_Time)
   //Print_Utilization(NTW,SM_Parameters);

   printf("Inicializacion de variables Stadisticas()\n");
   InicializaVariablesStadisticas (NTW, SM_Parameters);
   printf("Termino Inicializacion de variables Stadisticas()\n");



   // Variables to generate all the required Statistics, Resultados and Utilization files
   float SMload;
   //char fileD[20];
   //char filenamD[20];
   //char fileUtiD[20];
   char extD[]=".txt";
   int IncD;

   ///////////////////////// ============================  dw = 1
   // ------------------------------------------------
   //                Read Failures
   // ------------------------------------------------

   printf ("Reading Failures at file: R-TT-D1 \n") ;
   if ((f1 = fopen("Requests-TT-D1","r")) == NULL)
   {
      printf ("Unable to read the parameter file Requests-TT-D1 \n") ;
      exit (-1);
   }
   read_failures(NTW, SM_Parameters) ;
   fclose(f1);
   printf ("Tiempo de Simulacion \n") ;
   NTW->Phase = 3;
   //SM_Parameters->load  = SM_Parameters->Umbral;

    // MPR with different loads
    SMload = 1.0;
    char fileD1[20]   ="Statistic1-D";
    char filenamD1[20]="Resultado1-D";
    char fileUtiD1[20]="Utilizati1-D";
    IncD = 10;

    printf ("Final testing dw2 \n") ;
    printf ("Final testing dw2 \n") ;
    //fclose(fileout);
    //fclose(fUtilization);
    NTW->ThisLoad = 0;
    while(SMload > 0.2)
    {
        sprintf(fileD1+12,"%d",IncD);
        strcat(fileD1,extD);
        printf ("\n File %s \n",fileD1) ;
        printf ("\n Load %f \n",SMload) ;
        fileout = fopen(fileD1,"w");
        sprintf(fileUtiD1+12,"%d",IncD);
        strcat(fileUtiD1,extD);
        fUtilization = fopen(fileUtiD1,"w");

        for(wave = 1; wave <= SM_Parameters->num_waves; wave++)
            NTW->g[wave] = SMload;

        for(NTW->tempo = 1;NTW->tempo <= SM_Parameters->Operation_Time;NTW->tempo++)
        {
            SimulaNet(NTW, SM_Parameters);
            printf ("\ntempo (%d) - Phase = %d  - Load %f", (int) NTW->tempo, NTW->Phase,SMload ) ;
            //printf("\n BeforeRes ");
            //printf("\n BRes ");
            Export_Results(NTW, SM_Parameters);
        }
        printf("\n\n Finished Export_Res ");
        printf("\n\n FER ");
        //if(NTW->tempo == SM_Parameters->Operation_Time)
        Print_Utilization(NTW,SM_Parameters);
        fclose(fileout);
        printf("\n F1 "); printf("\n F1 ");
        //fclose(fileout2);
        printf("\n F2 "); printf("\n F2 ");
        fclose(fUtilization);
        printf("\n\n Closing ");
        printf("\n\n Closing ");

        sprintf(filenamD1+12,"%d",IncD);
        strcat(filenamD1,extD);

        //fileout = fopen("Resultados-L.txt","w");
        printf("\n\n Thius file : %s",filenamD1);
        fileout = fopen(filenamD1,"w");
        Stampa(NTW,SM_Parameters);  // Printing results
        fclose(fileout);

        IncD = IncD - 2;
        SMload = SMload - 0.2;
        printf("\n NewLoad %f ", SMload);
        NTW->ThisLoad++;

        printf("Inicializacion de variables Stadisticas()\n");
        InicializaVariablesStadisticas (NTW, SM_Parameters);
        printf("Termino Inicializacion de variables Stadisticas()\n");
    }// end while load dw = 1

   ///////////////////////// ============================  dw = 2
   // ------------------------------------------------
   //                Read Failures
   // ------------------------------------------------

   printf ("Reading Failures at file: R-TT-D \n") ;
   if ((f1 = fopen("Requests-TT-D2","r")) == NULL)
   {
      printf ("Unable to read the parameter file Requests-TT-D2 \n") ;
      exit (-1);
   }
   read_failures(NTW, SM_Parameters) ;
   fclose(f1);
   printf ("Tiempo de Simulacion \n") ;
   NTW->Phase = 3;
   //SM_Parameters->load  = SM_Parameters->Umbral;

    // MPR with different loads
    SMload = 1.0;
    char fileD2[20]   ="Statistic2-D";
    char filenamD2[20]="Resultado2-D";
    char fileUtiD2[20]="Utilizati2-D";
    IncD = 10;

    printf ("Final testing dw2 \n") ;
    printf ("Final testing dw2 \n") ;
    //fclose(fileout);
    //fclose(fUtilization);
    NTW->ThisLoad = 0;
    while(SMload > 0.2)
    {
        sprintf(fileD2+12,"%d",IncD);
        strcat(fileD2,extD);
        printf ("\n File %s \n",fileD2) ;
        printf ("\n Load %f \n",SMload) ;
        fileout = fopen(fileD2,"w");
        sprintf(fileUtiD2+12,"%d",IncD);
        strcat(fileUtiD2,extD);
        fUtilization = fopen(fileUtiD2,"w");

        for(wave = 1; wave <= SM_Parameters->num_waves; wave++)
            NTW->g[wave] = SMload;

        for(NTW->tempo = 1;NTW->tempo <= SM_Parameters->Operation_Time;NTW->tempo++)
        {
            SimulaNet(NTW, SM_Parameters);
            printf ("\ntempo (%d) - Phase = %d  - Load %f", (int) NTW->tempo, NTW->Phase,SMload ) ;
            //printf("\n BeforeRes ");
            //printf("\n BRes ");
            Export_Results(NTW, SM_Parameters);
        }
        printf("\n\n Finished Export_Res ");
        printf("\n\n FER ");
        //if(NTW->tempo == SM_Parameters->Operation_Time)
        Print_Utilization(NTW,SM_Parameters);
        fclose(fileout);
        printf("\n F1 "); printf("\n F1 ");
        //fclose(fileout2);
        printf("\n F2 "); printf("\n F2 ");
        fclose(fUtilization);
        printf("\n\n Closing ");
        printf("\n\n Closing ");

        sprintf(filenamD2+12,"%d",IncD);
        strcat(filenamD2,extD);

        //fileout = fopen("Resultados-L.txt","w");
        printf("\n\n Thius file : %s",filenamD2);
        fileout = fopen(filenamD2,"w");
        Stampa(NTW,SM_Parameters);  // Printing results
        fclose(fileout);

        IncD = IncD - 2;
        SMload = SMload - 0.2;
        printf("\n NewLoad %f ", SMload);
        NTW->ThisLoad++;

        printf("Inicializacion de variables Stadisticas()\n");
        InicializaVariablesStadisticas (NTW, SM_Parameters);
        printf("Termino Inicializacion de variables Stadisticas()\n");
    }// end while load dw = 2

    ///////////////////////// ============================  dw = 3
    // ------------------------------------------------
    //                Read Failures
    // ------------------------------------------------
    printf ("Reading Failures at file: R-TT-D \n") ;
    if ((f1 = fopen("Requests-TT-D3","r")) == NULL)
    {
       printf ("Unable to read the parameter file Requests-TT-D3 \n") ;
       exit (-1);
    }
    read_failures(NTW, SM_Parameters) ;
    fclose(f1);
    printf ("Tiempo de Simulacion \n") ;
    NTW->Phase = 3;
    //SM_Parameters->load  = SM_Parameters->Umbral;

     // MPR with different loads
     SMload = 1.0;
     char fileD3[20]   ="Statistic3-D";
     char filenamD3[20]="Resultado3-D";
     char fileUtiD3[20]="Utilizati3-D";
     IncD = 10;

     printf ("Final testing dw3 \n") ;
     printf ("Final testing dw3 \n") ;
     //fclose(fileout);
     //fclose(fUtilization);
     NTW->ThisLoad = 0;
     while(SMload > 0.2)
     {
         sprintf(fileD3+12,"%d",IncD);
         strcat(fileD3,extD);
         printf ("\n File %s \n",fileD3) ;
         printf ("\n Load %f \n",SMload) ;
         fileout = fopen(fileD3,"w");
         sprintf(fileUtiD3+12,"%d",IncD);
         strcat(fileUtiD3,extD);
         fUtilization = fopen(fileUtiD3,"w");

         for(wave = 1; wave <= SM_Parameters->num_waves; wave++)
             NTW->g[wave] = SMload;

         for(NTW->tempo = 1;NTW->tempo <= SM_Parameters->Operation_Time;NTW->tempo++)
         {
             SimulaNet(NTW, SM_Parameters);
             printf ("\ntempo (%d) - Phase = %d  - Load %f", (int) NTW->tempo, NTW->Phase,SMload ) ;
             //printf("\n BeforeRes ");
             //printf("\n BRes ");
             Export_Results(NTW, SM_Parameters);
         }
         printf("\n\n Finished Export_Res ");
         printf("\n\n FER ");
         //if(NTW->tempo == SM_Parameters->Operation_Time)
         Print_Utilization(NTW,SM_Parameters);
         fclose(fileout);
         printf("\n F1 "); printf("\n F1 ");
         //fclose(fileout2);
         printf("\n F2 "); printf("\n F2 ");
         fclose(fUtilization);
         printf("\n\n Closing ");
         printf("\n\n Closing ");

         sprintf(filenamD3+12,"%d",IncD);
         strcat(filenamD3,extD);

         //fileout = fopen("Resultados-L.txt","w");
         printf("\n\n Thius file : %s",filenamD3);
         fileout = fopen(filenamD3,"w");
         Stampa(NTW,SM_Parameters);  // Printing results
         fclose(fileout);

         IncD = IncD - 2;
         SMload = SMload - 0.2;
         printf("\n NewLoad %f ", SMload);
         NTW->ThisLoad++;

         printf("Inicializacion de variables Stadisticas()\n");
         InicializaVariablesStadisticas (NTW, SM_Parameters);
         printf("Termino Inicializacion de variables Stadisticas()\n");
     }// end while load dw = 3

     ///////////////////////// ============================  dw = 4
     //  ------------------------------------------------ /
     //                 Read Failures                     /
     //  ------------------------------------------------ /

     printf ("Reading Failures at file: R-TT-D \n") ;
     if ((f1 = fopen("Requests-TT-D4","r")) == NULL)
     {
        printf ("Unable to read the parameter file Requests-TT-D4 \n") ;
        exit (-1);
     }
     read_failures(NTW, SM_Parameters) ;
     fclose(f1);
     printf ("Tiempo de Simulacion \n") ;
     NTW->Phase = 3;
     //SM_Parameters->load  = SM_Parameters->Umbral;

      // MPR with different loads
      SMload = 1.0;
      char fileD4[20]   ="Statistic4-D";
      char filenamD4[20]="Resultado4-D";
      char fileUtiD4[20]="Utilizati4-D";
      IncD = 10;

      printf ("Final testing dw4\n") ;
      printf ("Final testing dw4\n") ;
      //fclose(fileout);
      //fclose(fUtilization);

      NTW->ThisLoad = 0;
      while(SMload > 0.2)
      {
          sprintf(fileD4+12,"%d",IncD);
          strcat(fileD4,extD);
          printf ("\n File %s \n",fileD4) ;
          printf ("\n Load %f \n",SMload) ;
          fileout = fopen(fileD4,"w");
          sprintf(fileUtiD4+12,"%d",IncD);
          strcat(fileUtiD4,extD);
          fUtilization = fopen(fileUtiD4,"w");

          for(wave = 1; wave <= SM_Parameters->num_waves; wave++)
              NTW->g[wave] = SMload;

          for(NTW->tempo = 1;NTW->tempo <= SM_Parameters->Operation_Time;NTW->tempo++)
          {
              SimulaNet(NTW, SM_Parameters);
              printf ("\ntempo (%d) - Phase = %d  - Load %f", (int) NTW->tempo, NTW->Phase,SMload ) ;
              //printf("\n BeforeRes ");
              //printf("\n BRes ");
              Export_Results(NTW, SM_Parameters);
          }
          printf("\n\n Finished Export_Res ");
          printf("\n\n FER ");
          //if(NTW->tempo == SM_Parameters->Operation_Time)
          Print_Utilization(NTW,SM_Parameters);
          fclose(fileout);
          printf("\n F1 "); printf("\n F1 ");
          //fclose(fileout2);
          printf("\n F2 "); printf("\n F2 ");
          fclose(fUtilization);
          printf("\n\n Closing ");
          printf("\n\n Closing ");

          sprintf(filenamD4+12,"%d",IncD);
          strcat(filenamD4,extD);

          //fileout = fopen("Resultados-L.txt","w");
          printf("\n\n Thius file : %s",filenamD4);
          fileout = fopen(filenamD4,"w");
          Stampa(NTW,SM_Parameters);  // Printing results
          fclose(fileout);

          IncD = IncD - 2;
          SMload = SMload - 0.2;
          printf("\n NewLoad %f ", SMload);
          NTW->ThisLoad++;

          printf("Inicializacion de variables Stadisticas()\n");
          InicializaVariablesStadisticas (NTW, SM_Parameters);
          printf("Termino Inicializacion de variables Stadisticas()\n");
      }// end while load dw = 4

   // ===================================================================
   //                         L E A R N I N G
   // ===================================================================
   printf("\n\n\t\t L E A R N I N G \n\n");
   fileout      = fopen("Statistics-L.txt","w");
   fileout2     = fopen("Error-L.txt","w");
   fUtilization = fopen("Utilization-L.txt","w");
   NTW->Num_at    = 0;
   NTW->Num_First = 0;
   NTW->Num_Neg   = 0;
   NTW->Num_Pos   = 0;

   NTW->MyActions[0] = 0;
   NTW->MyActions[1] = 0;
   NTW->MyActions[2] = 0;
   NTW->MyActions[3] = 0;
   NTW->MyActions[4] = 0;
   NTW->MyActions[5] = 0;
   NTW->MyActions[6] = 0;
   NTW->MyActions[7] = 0;

   printf ("Tiempo transitorio \n") ;

   printf("Inicializacion de variables Stadisticas()\n");
   InicializaVariablesStadisticas (NTW, SM_Parameters);
   printf("Termino Inicializacion de variables Stadisticas()\n");

   for(wave = 1; wave <= SM_Parameters->num_waves; wave++)
      NTW->g[wave] = SM_Parameters->load;
   NTW->Phase = 4;
   NTW->ThisLoad = 0;
   for(NTW->tempo = 1;NTW->tempo <= SM_Parameters->Initial_Time; NTW->tempo++)
   {
      printf ("\ntempo (%d) - Phase = %d", (int) NTW->tempo, NTW->Phase ) ;
      SimulaNet(NTW, SM_Parameters);
      Export_Results(NTW, SM_Parameters);
   }
   //if(NTW->tempo == SM_Parameters->Operation_Time)
   //Print_Utilization(NTW,SM_Parameters);

   printf("Inicializacion de variables Stadisticas()\n");
   InicializaVariablesStadisticas (NTW, SM_Parameters);
   printf("Termino Inicializacion de variables Stadisticas()\n");

   /* ------------------------------------------------ */
   /*                Read Failures                     */
   /* ------------------------------------------------ */
   printf ("Reading Failures at file: Requests-CT-L \n") ;
   if ((f1 = fopen("Requests-CT-L","r")) == NULL)
   {
      printf ("Unable to read the parameter file Failures \n") ;
      exit (-1);
   }
   read_failures(NTW, SM_Parameters) ;
   fclose(f1);

   for(wave = 1; wave <= SM_Parameters->num_waves; wave++)
      NTW->g[wave] = SM_Parameters->load;

   float NewPolicyChange = 0.5;
   float OldPolicyChange = 0.9;
   float MyOldAvg        = 0.01;
   float MyNewAvg        = 1.0;
   float MyMSE           = 1.0;
   float AvgChange;

   AvgChange = computePolicyChange(NTW,SM_Parameters);

   int i;
   i = 0;
   do
   {
      printf ("Tiempo de Optimizacion \n") ;

      NTW->Phase = 5;
      NTW->ThisLoad = 0;
      for(NTW->tempo = 1;NTW->tempo <= SM_Parameters->Optimization_Time;NTW->tempo++)
      {
         SimulaNet(NTW, SM_Parameters);
         printf ("\ntempo (%d) - Phase = %d - (%d)", (int) NTW->tempo, NTW->Phase, i ) ;
         Export_Results(NTW, SM_Parameters);
      }

      //OldPolicyChange = NewPolicyChange;
      NewPolicyChange = computePolicyChange(NTW,SM_Parameters);

      //MyNewAvg = (NewPolicyChange + OldPolicyChange) / 2.0;

      //MyMSE           = fabsf(MyNewAvg - MyOldAvg);

      //AvgChange       = MyMSE * MyMSE;

      //printf ("\n \t \t P_V %d Old %4.8f New %4.8f NAvg %4.8f MSE %4.8f MSE^2 %4.8f \n",i,OldPolicyChange,NewPolicyChange,MyNewAvg,MyMSE,AvgChange);
      printf ("\n \t \t P_V %d Packet Loss Rate %4.8f \n",i,NewPolicyChange);

      printf("Inicializacion de variables Stadisticas()\n");
      InicializaVariablesStadisticas (NTW, SM_Parameters);
      printf("Termino Inicializacion de variables Stadisticas()\n");
      i++;

   }while( (NewPolicyChange < 0.99) && (i <= 1));

   //if(NTW->tempo == SM_Parameters->Operation_Time)
   //Print_Utilization(NTW,SM_Parameters);

   //printf("Inicializacion de variables Stadisticas()\n");
   //InicializaVariablesStadisticas (NTW, SM_Parameters);
   //printf("Termino Inicializacion de variables Stadisticas()\n");

   // Let's simulate a bit more without requests after the learning has been done.
   NTW->ThisLoad = 1;
   for(NTW->tempo = 1;NTW->tempo <= SM_Parameters->Initial_Time * 2; NTW->tempo++)
   {
      printf ("\ntempo (%d) - Phase = %d", (int) NTW->tempo, NTW->Phase ) ;
      SimulaNet(NTW, SM_Parameters);
      Export_Results(NTW, SM_Parameters);
   }

   printf("Inicializacion de variables Stadisticas()\n");
   InicializaVariablesStadisticas (NTW, SM_Parameters);
   printf("Termino Inicializacion de variables Stadisticas()\n");

   SMload = SM_Parameters->load;
   //char fileN[20];
   //char filename[20];
   //char fileUti[20];
   char ext[]=".txt";
   int Inc = 10;
   // ======================================================= dw = 1
   // ------------------------------------------------
   //                Read Failures
   // ------------------------------------------------

   printf ("Reading Failures at file: Requests-TT-L \n") ;
   if ((f1 = fopen("Requests-TT-L1","r")) == NULL)
   {
      printf ("Unable to read the parameter file Requests-TT-L1 \n") ;
      exit (-1);
   }
   read_failures(NTW, SM_Parameters) ;
   fclose(f1);
   printf ("Tiempo de Simulacion \n") ;
   NTW->Phase = 6;
   //SM_Parameters->load  = SM_Parameters->Umbral;

   /// Changing load - starting with 0.9
       SMload = 1.0;
       char fileN1[20]   ="Statistic1-L";
       char filename1[20]="Resultado1-L";
       char fileUti1[20] ="Utilizati1-L";
       //char ext[]=".txt";
       Inc = 10;

       printf ("Final testing dtdw2\n") ;
       printf ("Final testing dtdw2\n") ;
       //fclose(fileout);
       //fclose(fUtilization);

       NTW->ThisLoad = 0;
       while(SMload > 0.2)
       {
           sprintf(fileN1+12,"%d",Inc);
           strcat(fileN1,ext);
           printf ("\n File %s \n",fileN1) ;
           printf ("\n Load %f \n",SMload) ;
           fileout = fopen(fileN1,"w");
           sprintf(fileUti1+12,"%d",Inc);
           strcat(fileUti1,ext);
           fUtilization = fopen(fileUti1,"w");

           for(wave = 1; wave <= SM_Parameters->num_waves; wave++)
               NTW->g[wave] = SMload;

           for(NTW->tempo = 1;NTW->tempo <= SM_Parameters->Operation_Time;NTW->tempo++)
           {
               SimulaNet(NTW, SM_Parameters);
               printf ("\ntempo (%d) - Phase = %d", (int) NTW->tempo, NTW->Phase ) ;
               //printf("\n BeforeRes ");
               //printf("\n BRes ");
               Export_Results(NTW, SM_Parameters);
           }
           printf("\n\n Finished Export_Res ");
           printf("\n\n FER ");
           //if(NTW->tempo == SM_Parameters->Operation_Time)
           Print_Utilization(NTW,SM_Parameters);
           fclose(fileout);
           printf("\n F1 "); printf("\n F1 ");
           //fclose(fileout2);
           printf("\n F2 "); printf("\n F2 ");
           fclose(fUtilization);
           printf("\n\n Closing ");
           printf("\n\n Closing ");

           sprintf(filename1+12,"%d",Inc);
           strcat(filename1,ext);

           //fileout = fopen("Resultados-L.txt","w");
           printf("\n\n Thius file : %s",filename1);
           fileout = fopen(filename1,"w");
           Stampa(NTW,SM_Parameters);  // Printing results
           fclose(fileout);

           Inc = Inc - 2;
           SMload = SMload - 0.2;
           printf("\n NewLoad %f ", SMload);
           NTW->ThisLoad++;



           printf("Inicializacion de variables Stadisticas()\n");
           InicializaVariablesStadisticas (NTW, SM_Parameters);
           printf("Termino Inicializacion de variables Stadisticas()\n");
       }// End while load -- dw = 1

   // ======================================================= dw = 2
   // ------------------------------------------------
   //                Read Failures
   // ------------------------------------------------

   printf ("Reading Failures at file: Requests-TT-L \n") ;
   if ((f1 = fopen("Requests-TT-L2","r")) == NULL)
   {
      printf ("Unable to read the parameter file Requests-TT-L2 \n") ;
      exit (-1);
   }
   read_failures(NTW, SM_Parameters) ;
   fclose(f1);
   printf ("Tiempo de Simulacion \n") ;
   NTW->Phase = 6;
   //SM_Parameters->load  = SM_Parameters->Umbral;

   /// Changing load - starting with 0.9
       SMload = 1.0;
       char fileN2[20]   ="Statistic2-L";
       char filename2[20]="Resultado2-L";
       char fileUti2[20] ="Utilizati2-L";
       //char ext[]=".txt";
       Inc = 10;

       printf ("Final testing dtdw2\n") ;
       printf ("Final testing dtdw2\n") ;
       //fclose(fileout);
       //fclose(fUtilization);

       NTW->ThisLoad = 0;
       while(SMload > 0.2)
       {
           sprintf(fileN2+12,"%d",Inc);
           strcat(fileN2,ext);
           printf ("\n File %s \n",fileN2) ;
           printf ("\n Load %f \n",SMload) ;
           fileout = fopen(fileN2,"w");
           sprintf(fileUti2+12,"%d",Inc);
           strcat(fileUti2,ext);
           fUtilization = fopen(fileUti2,"w");

           for(wave = 1; wave <= SM_Parameters->num_waves; wave++)
               NTW->g[wave] = SMload;

           for(NTW->tempo = 1;NTW->tempo <= SM_Parameters->Operation_Time;NTW->tempo++)
           {
               SimulaNet(NTW, SM_Parameters);
               printf ("\ntempo (%d) - Phase = %d", (int) NTW->tempo, NTW->Phase ) ;
               //printf("\n BeforeRes ");
               //printf("\n BRes ");
               Export_Results(NTW, SM_Parameters);
           }
           printf("\n\n Finished Export_Res ");
           printf("\n\n FER ");
           //if(NTW->tempo == SM_Parameters->Operation_Time)
           Print_Utilization(NTW,SM_Parameters);
           fclose(fileout);
           printf("\n F1 "); printf("\n F1 ");
           //fclose(fileout2);
           printf("\n F2 "); printf("\n F2 ");
           fclose(fUtilization);
           printf("\n\n Closing ");
           printf("\n\n Closing ");

           sprintf(filename2+12,"%d",Inc);
           strcat(filename2,ext);

           //fileout = fopen("Resultados-L.txt","w");
           printf("\n\n Thius file : %s",filename2);
           fileout = fopen(filename2,"w");
           Stampa(NTW,SM_Parameters);  // Printing results
           fclose(fileout);

           Inc = Inc - 2;
           SMload = SMload - 0.2;
           printf("\n NewLoad %f ", SMload);
           NTW->ThisLoad++;



           printf("Inicializacion de variables Stadisticas()\n");
           InicializaVariablesStadisticas (NTW, SM_Parameters);
           printf("Termino Inicializacion de variables Stadisticas()\n");
       }// End while load -- dw = 2

       // ======================================================= dw = 3
       // ------------------------------------------------
       //                Read Failures
       // ------------------------------------------------
       printf ("Reading Failures at file: Requests-TT-L \n") ;
       if ((f1 = fopen("Requests-TT-L3","r")) == NULL)
       {
          printf ("Unable to read the parameter file Requests-TT-L3 \n") ;
          exit (-1);
       }
       read_failures(NTW, SM_Parameters) ;
       fclose(f1);
       printf ("Tiempo de Simulacion \n") ;
       NTW->Phase = 6;
       //SM_Parameters->load  = SM_Parameters->Umbral;

       /// Changing load - starting with 0.9
           SMload = 1.0;
           char fileN3[20]   ="Statistic3-L";
           char filename3[20]="Resultado3-L";
           char fileUti3[20] ="Utilizati3-L";
           Inc = 10;

           printf ("Final testing dtdw3\n") ;
           printf ("Final testing dtdw3\n") ;
           //fclose(fileout);
           //fclose(fUtilization);

           NTW->ThisLoad = 0;
           while(SMload > 0.2)
           {
               sprintf(fileN3+12,"%d",Inc);
               strcat(fileN3,ext);
               printf ("\n File %s \n",fileN3) ;
               printf ("\n Load %f \n",SMload) ;
               fileout = fopen(fileN3,"w");
               sprintf(fileUti3+12,"%d",Inc);
               strcat(fileUti3,ext);
               fUtilization = fopen(fileUti3,"w");

               for(wave = 1; wave <= SM_Parameters->num_waves; wave++)
                   NTW->g[wave] = SMload;

               for(NTW->tempo = 1;NTW->tempo <= SM_Parameters->Operation_Time;NTW->tempo++)
               {
                   SimulaNet(NTW, SM_Parameters);
                   printf ("\ntempo (%d) - Phase = %d", (int) NTW->tempo, NTW->Phase ) ;
                   //printf("\n BeforeRes ");
                   //printf("\n BRes ");
                   Export_Results(NTW, SM_Parameters);
               }
               printf("\n\n Finished Export_Res ");
               printf("\n\n FER ");
               //if(NTW->tempo == SM_Parameters->Operation_Time)
               Print_Utilization(NTW,SM_Parameters);
               fclose(fileout);
               printf("\n F1 "); printf("\n F1 ");
               //fclose(fileout2);
               printf("\n F2 "); printf("\n F2 ");
               fclose(fUtilization);
               printf("\n\n Closing ");
               printf("\n\n Closing ");

               sprintf(filename3+12,"%d",Inc);
               strcat(filename3,ext);

               //fileout = fopen("Resultados-L.txt","w");
               printf("\n\n Thius file : %s",filename3);
               fileout = fopen(filename3,"w");
               Stampa(NTW,SM_Parameters);  // Printing results
               fclose(fileout);

               Inc = Inc - 2;
               SMload = SMload - 0.2;
               printf("\n NewLoad %f ", SMload);
               NTW->ThisLoad++;



               printf("Inicializacion de variables Stadisticas()\n");
               InicializaVariablesStadisticas (NTW, SM_Parameters);
               printf("Termino Inicializacion de variables Stadisticas()\n");
           }// End while load -- dw = 3

           // ======================================================= dw = 4
           // ------------------------------------------------
           //                Read Failures
           // ------------------------------------------------

           printf ("Reading Failures at file: Requests-TT-L \n") ;
           if ((f1 = fopen("Requests-TT-L4","r")) == NULL)
           {
              printf ("Unable to read the parameter file Requests-TT-L4 \n") ;
              exit (-1);
           }
           read_failures(NTW, SM_Parameters) ;
           fclose(f1);
           printf ("Tiempo de Simulacion \n") ;
           NTW->Phase = 6;
           //SM_Parameters->load  = SM_Parameters->Umbral;

           /// Changing load - starting with 0.9
               SMload = 1.0;
               char fileN4[20]   ="Statistic4-L";
               char filename4[20]="Resultado4-L";
               char fileUti4[20] ="Utilizati4-L";
               //ext[]=".txt";
               Inc = 10;

               printf ("Final testing dtdw4\n") ;
               printf ("Final testing dtdw4\n") ;
               //fclose(fileout);
               //fclose(fUtilization);

               NTW->ThisLoad = 0;
               while(SMload > 0.2)
               {

                   printf("Inicializacion de variables Stadisticas()\n");
                   InicializaVariablesStadisticas (NTW, SM_Parameters);
                   printf("Termino Inicializacion de variables Stadisticas()\n");

                   sprintf(fileN4+12,"%d",Inc);
                   strcat(fileN4,ext);
                   printf ("\n File %s \n",fileN4) ;
                   printf ("\n Load %f \n",SMload) ;
                   fileout = fopen(fileN4,"w");
                   sprintf(fileUti4+12,"%d",Inc);
                   strcat(fileUti4,ext);
                   fUtilization = fopen(fileUti4,"w");

                   for(wave = 1; wave <= SM_Parameters->num_waves; wave++)
                       NTW->g[wave] = SMload;

                   for(NTW->tempo = 1;NTW->tempo <= SM_Parameters->Operation_Time;NTW->tempo++)
                   {
                       SimulaNet(NTW, SM_Parameters);
                       printf ("\ntempo (%d) - Phase = %d", (int) NTW->tempo, NTW->Phase ) ;
                       //printf("\n BeforeRes ");
                       //printf("\n BRes ");
                       Export_Results(NTW, SM_Parameters);
                   }
                   printf("\n\n Finished Export_Res ");
                   printf("\n\n FER ");
                   //if(NTW->tempo == SM_Parameters->Operation_Time)
                   Print_Utilization(NTW,SM_Parameters);
                   fclose(fileout);
                   printf("\n F1 "); printf("\n F1 ");
                   //fclose(fileout2);
                   printf("\n F2 "); printf("\n F2 ");
                   fclose(fUtilization);
                   printf("\n\n Closing ");
                   printf("\n\n Closing ");

                   sprintf(filename4+12,"%d",Inc);
                   strcat(filename4,ext);

                   //fileout = fopen("Resultados-L.txt","w");
                   printf("\n\n Thius file : %s",filename4);
                   fileout = fopen(filename4,"w");
                   Stampa(NTW,SM_Parameters);  // Printing results
                   fclose(fileout);

                   Inc = Inc - 2;
                   SMload = SMload - 0.2;
                   printf("\n NewLoad %f ", SMload);
                   NTW->ThisLoad++;

               }// End while load -- dw = 4

}                       /* end SIMULA */

/* ============================================================== */
/*         Network Simulation                                     */
/* ============================================================== */
/* -------------------------------------------------------------- */
void SimulaNet(struct NETWORK *NTW, struct SPARAMETERS *SM_Parameters)
{

   int i;

   if ( (NTW->tempo == 1000) || (NTW->tempo == 5000) || (NTW->tempo == 10000) )
      fprintf (fscreen,"***** Utilization() *****\n") ;
   Utilization(NTW,SM_Parameters);                         /* Input link Utilization    */
   if ( (NTW->tempo == 1000) || (NTW->tempo == 5000) || (NTW->tempo == 10000))
      fprintf (fscreen,"***** Generation() *****\n") ;
   Generation(NTW,SM_Parameters);                          /* cell generation           */
   if ( (NTW->tempo == 1000) || (NTW->tempo == 5000) || (NTW->tempo == 10000))
      fprintf (fscreen,"***** SimulaNode() *****\n") ;
   SimulaNode(NTW, SM_Parameters);

   //print_source_dest(NTW, SM_Parameters);

   if ( (NTW->tempo == 1000) || (NTW->tempo == 5000) || (NTW->tempo == 10000))
      fprintf (fscreen,"***** LinkSimulation() *****\n") ;
   LinkSimulation(NTW, SM_Parameters);                      /* update the fiber links    */
   if ( (NTW->tempo == 1000) || (NTW->tempo == 5000) || (NTW->tempo == 10000))
      fprintf (fscreen,"***** StatusPropagation(NTW); *****\n") ;
   //StatusPropagation(NTW, SM_Parameters);      /* Transfer node's status for each link */

   NTW->tempoT++;

   // NumINJ
   // computeINJ(NTW,SM_Parameters);
}

/* ============================================================ */
/*             InicializaVariablesStadisticas                   */
/* ============================================================ */
void InicializaVariablesStadisticas (struct NETWORK *NTW, struct SPARAMETERS *SM_Parameters)
{
   int ndr, linkin, Olink,wave, AAdd;
   float Discount, MyValue;
   if(NTW->Phase == 1)
   {

      NTW->num_in_packets  = 0;
      NTW->num_out_packets = 0;
      //NTW->num_abs_packets = 0;

      for (ndr = 0; ndr < NTW->nbr_of_nodes; ndr++)
      {
         NTW->numdef[ndr]          = 0;         /* Number of deflections   */
         NTW->numPPL[ndr]          = 0;         /* Number of packets loss  */
	     NTW->numPPL_Failures[ndr] = 0;         /* Number of packets loss  */
         NTW->numMPR[ndr]          = 0;
         NTW->numLossmpr[ndr]      = 0;
         NTW->MaxWCactive[ndr]     = 0;
         //NTW->num_abs_packets[ndr] = 0;

         //NTW->NumFlowS[ndr] = 0;
         //NTW->NumFlowR[ndr] = 0;
         //NTW->MyFlowID[ndr] = 0;
      }

      for (ndr=0; ndr< NTW->nbr_of_nodes; ndr++)
         for (linkin=1; linkin<= max_links_per_node; linkin++)
            for (wave=1; wave<= SM_Parameters->num_waves; wave++)
            {
               NTW->numInj[ndr][linkin][wave]    = 0;
               NTW->numFullIN[ndr][linkin][wave] = 0;
               NTW->numAbs[ndr][linkin][wave]    = 0;
               //NTW->numHops[ndr][linkin][wave]   = 0;
               //NTW->numSlots[ndr][linkin][wave]  = 0;
            }

      for (ndr = 0; ndr < NTW->nbr_of_nodes; ndr++)
         for (Olink = 1; Olink <= max_links_per_node; Olink++)
         {
            NTW->OutlinkUsage[ndr][Olink] = 0;
            NTW->BufferUsage[ndr][Olink]  = 0;
            NTW->Drop_P_Link[ndr][Olink]  = 0;
            NTW->WCUsage[ndr][Olink]      = 0;
            NTW->WCUsageBA[ndr][Olink]    = 0;
            NTW->numPPLFiber[ndr][Olink]  = 0;
         }

      for (ndr = 0; ndr < NTW->nbr_of_nodes; ndr++)
         for (AAdd = 1 ; AAdd <= MaxnumAdd; AAdd++)
            NTW->Pc[ndr][AAdd] = 0;

      /*for (AAdd = 0 ; AAdd <= 99; AAdd++)
      {
         NTW->hop_distribution[AAdd] = 0;
         NTW->hop_absorption[AAdd]   = 0;
      }*/

      for (ndr = 0; ndr < NTW->nbr_of_nodes; ndr++)
    	  for( AAdd = 0; AAdd < NTW->nbr_of_nodes; AAdd++)
    	  {
    		  NTW->TimeGeneration[ndr][AAdd] = 0;
    		  Discount = 0.25;
    		  for( Olink = 1; Olink <= max_links_per_node; Olink++ )
    		  //for( Olink = 1; Olink <= NTW->num_outputs[ndr][AAdd]; Olink++ )
    		  {
                  // Randomly initialize policies and traces
                  //NTW->MyPolicy [ndr][AAdd][Olink] = RndReal(Olink);

    			  if(Olink == 1)
    				  MyValue = 0.8;
    			  else
                     MyValue = (float) ((float) 1 - (Discount * (float) Olink));

    			  if(MyValue <= 0.1)
    				  MyValue = 0.1;

    			  //MyValue = RndReal(Olink);
    			  NTW->MyPolicy [ndr][AAdd][Olink] = MyValue;
    		  }

    		  NTW->Current_Flow[ndr][AAdd][0] = 0;
    		  NTW->Current_Flow[ndr][AAdd][1] = 0;
    		  NTW->Current_Flow[ndr][AAdd][2] = 0;

    		  NTW->Size_Flow   [ndr][AAdd][0] = 0;
    		  NTW->Size_Flow   [ndr][AAdd][1] = 0;
    		  NTW->Size_Flow   [ndr][AAdd][2] = 0;

    		  NTW->TrackEmptyS [ndr][AAdd][0] = 0;
    		  NTW->TrackEmptyS [ndr][AAdd][1] = 0;
    		  NTW->TrackEmptyS [ndr][AAdd][2] = 0;

    		  for(Olink = 0; Olink < MaxBLength; Olink++)
    		  {
    		     NTW->ArrivedSeq[ndr][AAdd][0][Olink] = 0;
    		     NTW->ArrivedSeq[ndr][AAdd][1][Olink] = 0;
    		     NTW->ArrivedSeq[ndr][AAdd][2][Olink] = 0;

    		     NTW->PacketArrived [ndr][AAdd][0][Olink] = 0;
    		     NTW->PacketArrived [ndr][AAdd][1][Olink] = 0;
    		     NTW->PacketArrived [ndr][AAdd][2][Olink] = 0;

    		     NTW->FromFlow [ndr][AAdd][0][Olink] = 0;
    		     NTW->FromFlow [ndr][AAdd][1][Olink] = 0;
    		     NTW->FromFlow [ndr][AAdd][2][Olink] = 0;
    		  }

    		  //float GlobalRD [(AveLength * 2) + 1];

    	  }

      for(AAdd = 0; AAdd < 7; AAdd++)
      {
    	  for(wave = 0; wave < 6; wave++)
    	  {
   	         NTW->SendPK[AAdd][0][wave] = 0;
   	         NTW->AbsPK [AAdd][0][wave] = 0;
   	         NTW->LostPK[AAdd][0][wave] = 0;

   	         NTW->SendPK[AAdd][1][wave] = 0;
   	         NTW->AbsPK [AAdd][1][wave] = 0;
   	         NTW->LostPK[AAdd][1][wave] = 0;

   	         NTW->SendPK[AAdd][2][wave] = 0;
   	         NTW->AbsPK [AAdd][2][wave] = 0;
   	         NTW->LostPK[AAdd][2][wave] = 0;

   	         NTW->SendPK[AAdd][3][wave] = 0;
   	         NTW->AbsPK [AAdd][3][wave] = 0;
   	         NTW->LostPK[AAdd][3][wave] = 0;

    	  }
      }
   }// End Phase = 1
   else
   {
	      NTW->num_in_packets  = 0;
	      NTW->num_out_packets = 0;
	      //NTW->num_abs_packets = 0;

	      for (ndr = 0; ndr < NTW->nbr_of_nodes; ndr++)
	      {
	         NTW->numdef[ndr]          = 0;         /// Number of deflections
	         NTW->numPPL[ndr]          = 0;         /// Number of packets loss
		     NTW->numPPL_Failures[ndr] = 0;         /// Number of packets loss
	         NTW->numMPR[ndr]          = 0;
	         NTW->numLossmpr[ndr]      = 0;
	         NTW->MaxWCactive[ndr]     = 0;
	         //NTW->num_abs_packets[ndr] = 0;

	         //NTW->NumFlowS[ndr] = 0;
	         //NTW->NumFlowR[ndr] = 0;
	         //NTW->MyFlowID[ndr] = 0;
	      }

	      for (ndr=0; ndr< NTW->nbr_of_nodes; ndr++)
	         for (linkin=1; linkin<= max_links_per_node; linkin++)
	            for (wave=1; wave<= SM_Parameters->num_waves; wave++)
	            {
	               NTW->numInj[ndr][linkin][wave]    = 0;
	               NTW->numFullIN[ndr][linkin][wave] = 0;
	               NTW->numAbs[ndr][linkin][wave]    = 0;
	               //NTW->numHops[ndr][linkin][wave]   = 0;
	               //NTW->numSlots[ndr][linkin][wave]  = 0;
	            }

	      for (ndr = 0; ndr < NTW->nbr_of_nodes; ndr++)
	         for (Olink = 1; Olink <= max_links_per_node; Olink++)
	         {
	            NTW->OutlinkUsage[ndr][Olink] = 0;
	            NTW->BufferUsage[ndr][Olink]  = 0;
	            NTW->Drop_P_Link[ndr][Olink]  = 0;
	            NTW->WCUsage[ndr][Olink]      = 0;
	            NTW->WCUsageBA[ndr][Olink]    = 0;
	            NTW->numPPLFiber[ndr][Olink]  = 0;
	         }

	      for (ndr = 0; ndr < NTW->nbr_of_nodes; ndr++)
	         for (AAdd = 1 ; AAdd <= MaxnumAdd; AAdd++)
	            NTW->Pc[ndr][AAdd] = 0;

	      for (ndr = 0; ndr < NTW->nbr_of_nodes; ndr++)
	    	  for( AAdd = 0; AAdd < NTW->nbr_of_nodes; AAdd++)
	    	  {
	    		  NTW->Current_Flow[ndr][AAdd][0] = 0;
	    		  NTW->Current_Flow[ndr][AAdd][1] = 0;
	    		  NTW->Current_Flow[ndr][AAdd][2] = 0;

	    		  NTW->Size_Flow   [ndr][AAdd][0] = 0;
	    		  NTW->Size_Flow   [ndr][AAdd][1] = 0;
	    		  NTW->Size_Flow   [ndr][AAdd][2] = 0;

	    		  NTW->TrackEmptyS [ndr][AAdd][0] = 0;
	    		  NTW->TrackEmptyS [ndr][AAdd][1] = 0;
	    		  NTW->TrackEmptyS [ndr][AAdd][2] = 0;


	    		  for(Olink = 0; Olink < MaxBLength; Olink++)
	    		  {
	    		     NTW->ArrivedSeq[ndr][AAdd][0][Olink] = 0;
	    		     NTW->ArrivedSeq[ndr][AAdd][1][Olink] = 0;
	    		     NTW->ArrivedSeq[ndr][AAdd][2][Olink] = 0;

	    		     NTW->PacketArrived [ndr][AAdd][0][Olink] = 0;
	    		     NTW->PacketArrived [ndr][AAdd][1][Olink] = 0;
	    		     NTW->PacketArrived [ndr][AAdd][2][Olink] = 0;

	    		     NTW->FromFlow [ndr][AAdd][0][Olink] = 0;
	    		     NTW->FromFlow [ndr][AAdd][1][Olink] = 0;
	    		     NTW->FromFlow [ndr][AAdd][2][Olink] = 0;
	    		  }
	    	  }

   }// End else

   // Packet Reordering - RD
   NTW->MyFlowID = 0;
   //NTW->CountGlobalRD = 0;
   for(Olink =0; Olink < MaxBLength; Olink++)
   {
	   NTW->GlobalRDP[Olink] = 0;
	   NTW->GlobalRDN[Olink] = 0;
   }

   // Link_Fibers[max_num_links][fibers_per_link]
   /*for(Olink = 0; Olink < max_num_links; Olink++)
   {
	   for(AAdd = 1; AAdd < fibers_per_link; AAdd++)
	   {
		   NTW->Link_Fibers[Olink][AAdd] = 0;
	   }
   }*/
}              /* END  InicializaVariablesStadisticas    */


/* ============================================================ */
/*                     LINK UTILIZATION                         */
/* ============================================================ */
/* ------------------------------------------------------------ */

void Utilization (struct NETWORK *NTW, struct SPARAMETERS *SM_Parameters)
{
   /* node=# of node, num_link= # of links */
   int ndr, linkin, wave, FromNode[max_num_node+1], source, dest, link_source, AAdd;
   int blocked, Failure_Link;

   for (ndr = 0; ndr < NTW->nbr_of_nodes; ndr++)
      for (linkin = 1; linkin <= NTW->input_links[ndr]; linkin++)
      {
         /*!> Initialization of utilization "u" */
         //NTW->u[ndr][linkin] = 0.0;
         for (wave = 1; wave <= SM_Parameters->num_waves; wave++)
            if (NTW->destination[NTW->num_input_link[ndr][linkin]][wave][1] != VirtualNode)
            {
               //NTW->num_hops[NTW->num_input_link[ndr][linkin]][wave][1]++;
               //NTW->num_slots[NTW->num_input_link[ndr][linkin]][wave][1]++;
               //if( NTW->priority[NTW->num_input_link[ndr][linkin]][wave][1] == 0)
                  NTW->numFullIN[ndr][linkin][wave]++ ;
            }
      }
   /* In this phase the utilization of each fiber is computed and if necessary,
      a new fiber is added. */
   if( (NTW->tempo%50) == 0)
   {
      for (ndr = 0; ndr < NTW->nbr_of_nodes; ndr++)
	     for (linkin = 1; linkin <= NTW->input_links[ndr]; linkin++)
	     {
	        NTW->u[ndr][linkin] = 0.0;
	        for (wave = 1; wave <= SM_Parameters->num_waves; wave++)
	        {
	           //NTW->u[ndr][linkin] += ((float)NTW->numFullIN[ndr][linkin][wave]) / (NTW->tempo);
	        	NTW->u[ndr][linkin] += ((float)NTW->numFullIN[ndr][linkin][wave]) / (NTW->tempoT);
	        }
	     }
   }

   //if( (NTW->Phase == 2) && (NTW->tempo >= SM_Parameters->Optimization_Time/10) && ( (NTW->tempo%1000) == 0 ) )
   if( (NTW->Phase == 2) && ( (NTW->tempo%100) == 0 ) && ( NTW->tempo >= 100))
   {
      // time,       node, link,   utilization
      // NTW->tempo, ndr,  linkin, NTW->u[ndr][linkin]/SM_Parameters->num_waves);

      for (ndr = 0; ndr < NTW->nbr_of_nodes; ndr++) // we skip the first fiber
      {
         for (linkin = 0; linkin <= NTW->nbr_of_nodes; linkin++)
            FromNode[linkin] = 0;
         for (linkin = 2; linkin <= NTW->input_links[ndr]; linkin++)
         {

            source = NTW->connected_from[ndr][linkin];

            dest   = ndr;
            for(link_source = 2; link_source <= NTW->output_links[source] + 1; link_source++)
               if(NTW->connected_to[source][link_source] == dest)
                  break;

            Failure_Link = NTW->num_output_link[source][link_source];
            blocked      = BLOCKED;
            //blocked      = search_link_failure(NTW,Failure_Link, SM_Parameters, 1, dest);

            //if( (NTW->u[ndr][linkin]/SM_Parameters->num_waves >= SM_Parameters->Umbral) && (NTW->n_links < max_num_links) && (!blocked))
            if( (NTW->u[ndr][linkin]/SM_Parameters->num_waves >= SM_Parameters->Umbral) && (NTW->n_links < max_num_links))
            {
               // To add ONE fiber from the node who is connected to input_link linkin.
               //fibers[linkin] = 1;
               /*source = NTW->connected_from[ndr][linkin];
               dest   = ndr;
               for(link_source = 2; link_source <= NTW->output_links[source] + 1; link_source++)
                  if(NTW->connected_to[source][link_source] == dest)
                     break;*/

               if(FromNode[source] == 0)
               {
                  if( (NTW->Phase == 2) && (NTW->n_links < max_num_links) &&
                      (NTW->output_links[source]<max_links_per_node) && (NTW->NDL[source][link_source] == SM_Parameters->AllowedDL))
                  {
                     more_links(source,dest,link_source,NTW,SM_Parameters);
                     printf("\n\n WE_GOT_A NEW FIBER %d!!!",NTW->tempo);
                  }
                  FromNode[source] = 1;

                  /*if (NTW->num_of_fibers[source][NTW->forw_table[source][dest][1]] >= 2)
                  {
                     // To erase the number of DL in each new fiber.
                     // Number of fibers to the same node
                     // NTW->num_of_fibers[ndr][NTW->connected_to[ndr][Olink]]
                     // For each output fiber
                     for (AAdd = 1; AAdd <= NTW->output_links[source] + 1; AAdd++)
                     {
                        // If this fiber connects to link_source, i.e. extra fiber
                        if(NTW->connected_to[source][AAdd] == NTW->connected_to[source][link_source])
                        {
                           // To erase the number of DL.
                           NTW->NDL[ndr][AAdd] = 0;
                        }
                     }
                  }*/

               }
            }
         }// END FOR linkin = 2
      }
   }
}

/* =============================================================*/
/*                         GENERATION                           */
/*==============================================================*/
void Generation(struct NETWORK *NTW, struct SPARAMETERS *SM_Parameters)
/* ndr=# of node, num_link= # of links   */
{
   float a,alf;
   int   newdest, ndr,j;
   int   linkin,wave;
   float tost;
   int AreLPR = 0;
   unsigned long int flowID;

   for (ndr = 0; ndr < NTW->nbr_of_nodes; ndr++)
   {
      AreLPR = get_node_status(NTW,ndr,SM_Parameters);
      for (wave = 1; wave <= SM_Parameters->num_waves; wave++)
      {
         a = RndReal(0);               /* generation           */

         // Are there light path requests?
         if((AreLPR != 666) &&
            (NTW->dw >= wave) &&
            (a < NTW->g[wave])
           )
         {
             if( (NTW->Phase == 2) || (NTW->Phase == 3) || (NTW->Phase == 5) || (NTW->Phase == 6))
             {
          		// Yes, there is a LP request involving ndr
          		// Send trhough second input link
          		newdest = AreLPR;
          		if(newdest >= NTW->nbr_of_nodes )
          		{
          			printf("\n\n BIG_MISTAKE_NO_DEST %d",newdest);
             	}
                if(NTW->dw == 666)
                   NTW->SendPK[NTW->Phase][SM_Parameters->num_waves - 1][NTW->ThisLoad]++;
                else
          		   NTW->SendPK[NTW->Phase][NTW->dw - 1][NTW->ThisLoad]++;

                NTW->priority   [NTW->num_input_link[ndr][2]][wave][1] = 1;
                NTW->source     [NTW->num_input_link[ndr][2]][wave][1] = ndr;
                NTW->destination[NTW->num_input_link[ndr][2]][wave][1] = newdest; // CAREFULLLL
                // STOP counting High Priority traffic
                //NTW->numInj[ndr][2][wave]++;
                //NTW->numFullIN[ndr][2][wave]++;
             }
         }
         else
         {
      	    NTW->priority   [NTW->num_input_link[ndr][2]][wave][1] = 0;
      	    NTW->source     [NTW->num_input_link[ndr][2]][wave][1] = VirtualNode;
      	    NTW->destination[NTW->num_input_link[ndr][2]][wave][1] = VirtualNode;
         }
         //
         //if (a < NTW->g[wave])        /* if  a<g              */
         if( a < NTW->g[wave] )
         {
            newdest = Rnd(0,NTW->nbr_of_nodes);
            while(newdest == ndr) /* The new destination       */
            /* can not be the same as the source node   */
               newdest = Rnd(0,NTW->nbr_of_nodes);
            //
            /*  ***********   Correlated traffic ***********  */
            if (AveLength != 1)
            {
               tost = RndReal(0);
               //flowID  = generateFlowID(ndr,NTW,SM_Parameters);
               flowID = 0;
               j    = 1;

               if (SM_Parameters->pareto == 1)
               {
                  if (NTW->packnumber[ndr][1][wave] == 0)
                  {
                     alf = 3 - Hurst_P * 2;
                     NTW->BurstLength[ndr][1][wave] = floor(1/pow(tost,1/alf));

                     if(NTW->BurstLength[ndr][1][wave] > MaxBLength)
                    	 NTW->BurstLength[ndr][1][wave] = MaxBLength;

                     flowID  = generateFlowID(ndr,NTW,SM_Parameters);
                     NTW->FlowID[ndr][1][wave]      = flowID; //100 + flowID + wave + (NTW->tempoT / 2);
                     NTW->AveMess                   = NTW->AveMess+NTW->BurstLength[ndr][1][wave];
                     NTW->numMess++;

                     //NTW->NumFlowS[ndr]++;
                  }
               }
               else
                  if (NTW->packnumber[ndr][1][wave] == 0)
                  {
                     while (tost >= NTW->poisson[j])
                        j = j+1;
                     NTW->BurstLength[ndr][1][wave] = j;
                     flowID  = generateFlowID(ndr,NTW,SM_Parameters);
                     NTW->FlowID[ndr][1][wave]      = flowID; //100 + flowID + wave + j + (NTW->tempoT / 2);
                     NTW->AveMess                   = NTW->AveMess+NTW->BurstLength[ndr][1][wave];
                     NTW->numMess++;

                     //NTW->NumFlowS[ndr]++;
                  }

               if(NTW->BurstLength[ndr][1][wave] > NTW->MaxBURST)
               {
                  NTW->MaxBURST = NTW->BurstLength[ndr][1][wave];
               }

               if(NTW->FlowID[ndr][1][wave] == 490644 ) // 476580)
               {
                  printf("\n \t flowID, phase %d  and time %d ",NTW->Phase,NTW->tempo);
               }

               if ((NTW->packnumber[ndr][1][wave] >= 1) && // Bursty traffic
                  (NTW->packnumber[ndr][1][wave] <= NTW->BurstLength[ndr][1][wave]))
               {
                  newdest = NTW->olddest[ndr][1][wave];
                  NTW->packnumber[ndr][1][wave]++;
               }
               else
               {
                  NTW->olddest[ndr][1][wave]    = newdest;
                  NTW->packnumber[ndr][1][wave] = 1;
               }

               if (NTW->packnumber[ndr][1][wave] >=  NTW->BurstLength[ndr][1][wave])
               {
                  NTW->packnumber[ndr][1][wave] = 0;
               }
            }
            // End of if (AveLength != 1)

            if ((5000 <= NTW->tempo) && (NTW->tempo <= 5050 ) && (ndr == 4))
               printf("BurstLength==== %i %i %i %i\n",(int)NTW->tempo,newdest,NTW->packnumber[ndr][1][wave],
                       (int) NTW->BurstLength[ndr][1][wave]);

            /*  ***********   Correlated traffic ************** */

            // Send trhough First input link
               if( (NTW->numInj[ndr][1][wave] / NTW->tempo) > 1)
               {
            	   printf("\n WHAT_A_ERROR!!!");
               }

               NTW->numInj[ndr][1][wave]++;
               NTW->numFullIN[ndr][1][wave]++;

               NTW->source[NTW->num_input_link[ndr][1]][wave][1]       = ndr;
               NTW->destination[NTW->num_input_link[ndr][1]][wave][1]  = newdest;
               //NTW->num_slots[NTW->num_input_link[ndr][1]][wave][1]    = 0;
               //NTW->num_hops[NTW->num_input_link[ndr][1]][wave][1]     = 0;
               NTW->cell_num[NTW->num_input_link[ndr][1]][wave][1]     = NTW->packnumber[ndr][1][wave];
               NTW->num_of_cells[NTW->num_input_link[ndr][1]][wave][1] = NTW->BurstLength[ndr][1][wave];
               NTW->priority[NTW->num_input_link[ndr][1]][wave][1]     = 0;
               //NTW->offset_time[NTW->num_input_link[ndr][1]][wave][1]  = 0;

               if(NTW->FlowID[ndr][1][wave] == 0)
               {
            	   printf("\n INVALID ID EXIT \n");
            	   exit (-1);
               }
               NTW->num_of_flow[NTW->num_input_link[ndr][1]][wave][1]  = NTW->FlowID[ndr][1][wave];

               if(NTW->packnumber[ndr][1][wave] == 0)
               {
            	   // Erasing ID from FlowID
            	   NTW->FlowID[ndr][1][wave] = 0;
               }

         }  // END of if (a < g[wave])
         else
         {
            NTW->source[NTW->num_input_link[ndr][1]][wave][1]       = VirtualNode;
            NTW->destination[NTW->num_input_link[ndr][1]][wave][1]  = VirtualNode;
            //NTW->num_slots[NTW->num_input_link[ndr][1]][wave][1]    = 0;
            //NTW->num_hops[NTW->num_input_link[ndr][1]][wave][1]     = 0;
            NTW->cell_num[NTW->num_input_link[ndr][1]][wave][1]     = 0;
            NTW->num_of_cells[NTW->num_input_link[ndr][1]][wave][1] = 0;
            NTW->priority[NTW->num_input_link[ndr][1]][wave][1]     = 0;
            //NTW->offset_time[NTW->num_input_link[ndr][1]][wave][1]  = 0;

            NTW->num_of_flow[NTW->num_input_link[ndr][1]][wave][1]  = 0;
         }
      }// End of for (wave = 1; wave <= SM_Parameters->num_waves; wave++)
   }// End for ndr
}

/* =========================================================== */
/*        SimulaTuttiNodi();                                   */
/* =========================================================== */
/*  This subroutine simulate the node                          */
/* ----------------------------------------------------------- */
void SimulaNode(struct NETWORK *NTW, struct SPARAMETERS *SM_Parameters)
{

   int i,ndr;
   for (ndr = 0; ndr < NTW->nbr_of_nodes; ndr++)
   {
      if ( (NTW->tempo == 1000) || (NTW->tempo == 5000) || (NTW->tempo == 10000))
         fprintf (fscreen," Initialization() \n") ;
      Initialization(ndr,NTW, SM_Parameters);

      if ( (NTW->tempo == 1000) || (NTW->tempo == 5000) || (NTW->tempo == 10000))
         fprintf (fscreen," EmptyBuffersPriority() \n") ;
      EmptyBuffersPriority(ndr,NTW, SM_Parameters);          /* take cells from the buffer */

      if ( (NTW->tempo == 1000) || (NTW->tempo == 5000) || (NTW->tempo == 10000))
         fprintf (fscreen," EmptyBuffers() \n") ;
      EmptyBuffers(ndr,NTW, SM_Parameters);          /* take cells from the buffer */

      if ( (NTW->tempo == 1000) || (NTW->tempo == 5000) || (NTW->tempo == 10000))
         fprintf (fscreen," UpdateBuffers() \n") ;
      UpdateBuffers(ndr,NTW, SM_Parameters);         /* update the buffer          */


      if ( (NTW->tempo == 1000) || (NTW->tempo == 5000) || (NTW->tempo == 10000))
         fprintf (fscreen," Transmission() \n") ;
      Transmission(ndr,NTW, SM_Parameters);           /* cell generation            */

      if ( (NTW->tempo == 1000) || (NTW->tempo == 5000) || (NTW->tempo == 10000))
         fprintf (fscreen," UpdateBuffers() \n") ;
      EmptySlotsinBuffers(ndr,NTW, SM_Parameters);

      NTW->WCactive[ndr] = 0;


      //RIGHT HERE!!! Before Translation
      process_priorities(ndr,NTW, SM_Parameters);


      if ( (NTW->tempo == 1000) || (NTW->tempo == 5000) || (NTW->tempo == 10000))
         fprintf (fscreen," Translation() \n") ;
      if (routing == 0)
         Translation(0,ndr,NTW, SM_Parameters);
      else
         for (i = 1; i <= MaxnumAdd; i++) /* MaxnumAdd Wavelength conversion */
            Translation(i,ndr,NTW, SM_Parameters);

      if (NTW->WCactive[ndr] > NTW->MaxWCactive[ndr])
         NTW->MaxWCactive[ndr] = NTW->WCactive[ndr];

      //if ( (NTW->tempo == 1000) || (NTW->tempo == 5000) || (NTW->tempo == 10000))
      //   fprintf (fscreen," SendStatus() \n") ;
      //SendStatus(ndr,NTW,SM_Parameters);       /* Send node's status  */

   }

}

/* ============================================================ */
/*                LINK SIMULATION SUBROUTINE                    */
/*--------------------------------------------------------------*/
void  LinkSimulation(struct NETWORK *NTW, struct SPARAMETERS *SM_Parameters)
{
   int i,j,k,PacketLoss,ndr,node,blocked;

   // Before we simulate links, we need to drop (and count) packets in failure links
   // NOT HERE COZ IT'S NOT AN ATTACK

   for (i = 0; i <= NTW->n_links; i++)
      for (j = 1; j <= SM_Parameters->num_waves; j++)
         for (k = 1; k <= NTW->link_size[i]; k++)
         {
            NTW->source[i][j][k]       = NTW->source[i][j][k+1];
            NTW->destination[i][j][k]  = NTW->destination[i][j][k+1];
            //NTW->num_slots[i][j][k]    = NTW->num_slots[i][j][k+1];
            //NTW->num_hops[i][j][k]     = NTW->num_hops[i][j][k+1];
            NTW->cell_num[i][j][k]     = NTW->cell_num[i][j][k+1];
            NTW->num_of_cells[i][j][k] = NTW->num_of_cells[i][j][k+1];
            NTW->priority[i][j][k]     = NTW->priority[i][j][k+1];
            //NTW->offset_time[i][j][k]  = NTW->offset_time[i][j][k+1];
            NTW->num_of_flow[i][j][k]  = NTW->num_of_flow[i][j][k+1];
         }
}

/* ============================================================ */
/*                Initialization                                */
/* ============================================================ */
/* This sufroutine initialization the stadistical variables     */
/* ------------------------------------------------------------ */
void Initialization(int ndr, struct NETWORK *NTW, struct SPARAMETERS *SM_Parameters)
{
   int wave, Olink, link, Naddress, buffer;
   NTW->StoreBuff = 0;                       /* parameter for Buffer */

   for (Olink = 1; Olink <= NTW->output_links[ndr] + 1; Olink++)
   {
      NTW->Outlink[Olink]    = 0;               /* # of cells per link  */
      NTW->INL[Olink]        = 0;
      NTW->EmptySlots[Olink] = 0;
   }

   for (Olink = 1; Olink <= NTW->output_links[ndr] + 1; Olink++)
      for ( wave = 1 ; wave <= SM_Parameters->num_waves ; wave++ )
         NTW->LCH[Olink][wave] = 0;

   for (link = 1; link <= NTW->input_links[ndr]; link++) /*Link assigment*/
      for ( wave = 1 ; wave <= SM_Parameters->num_waves ; wave++ )  /*wave assigment*/
         NTW->NA[link][wave] = EmptyPacket;

}                                            /* end Inizialization  */

/* ==============================================================*/
/*                   EmptyBuffers                                */
/*===============================================================*/
/* This subroutine solve blocking using Buffering                             */
void   EmptyBuffersPriority (int ndr, struct NETWORK *NTW, struct SPARAMETERS *SM_Parameters)
{

   int wave,Olink;
   int n_fibers, next_node, next_node2, j, available_fibers[max_links_per_node], Olink2;
   float p[max_links_per_node],ep;

   for (Olink = 1; Olink <= NTW->output_links[ndr] + 1; Olink++)
      /*Olink assigment*/
      for (wave = 1 ; wave <= SM_Parameters->num_waves; wave++ ) /* channel asigment  */
         if (NTW->NDL[ndr][Olink] > 0)
            /* It is there a packet?  */
            if (NTW->OutBuffer[Olink+(ndr) * max_links_per_node][wave][1] == 1)
            {
               if (NTW->Outlink[Olink] < (SM_Parameters->num_waves+0)) /*waves filled? */
               {
                  Olink2 = Olink;
                  if (NTW->LCH[Olink2][wave] < 1) /* channel empty? */
                  {
                     if(NTW->Bpriority[Olink+(ndr)*max_links_per_node][wave][1] == 1)
                     {
                	    BFassignmentPriority(ndr, Olink, wave, 1, NTW, SM_Parameters,Olink2);
                        NTW->OutBuffer[Olink+(ndr)*max_links_per_node][wave][1] = 0; // Remove packet from first link
                        NTW->OutputPack        = NTW->OutputPack+1;  /*# packets*/
                        NTW->LCH[Olink2][wave] = 2;   /* Wavelength is busy */
                        NTW->Outlink[Olink2]++;  /* packets per link */

                        NTW->Bpriority[Olink+(ndr)*max_links_per_node][wave][1] = 0;
                     }
                  }
               }
            }
}                 /* END EmptyBuffers   */

/* ==============================================================*/
/*                   EmptyBuffers                                */
/*===============================================================*/
/* This subroutine solve blocking using Buffering                             */
void   EmptyBuffers (int ndr, struct NETWORK *NTW, struct SPARAMETERS *SM_Parameters)
{

   int wave,Olink;
   int n_fibers, next_node, next_node2, j, available_fibers[max_links_per_node], Olink2;
   float p[max_links_per_node],ep;

   for (Olink = 1; Olink <= NTW->output_links[ndr] + 1; Olink++)
      /*Olink assigment*/
      for (wave = 1 ; wave <= SM_Parameters->num_waves; wave++ ) /* channel asigment  */
         if (NTW->NDL[ndr][Olink] > 0)
            /* It is there a packet?  */
            if (NTW->OutBuffer[Olink+(ndr) * max_links_per_node][wave][1] == 1)
            {
               if (NTW->Outlink[Olink] < (SM_Parameters->num_waves+0)) /*waves filled? */
               {
                  Olink2 = Olink;
                  if (NTW->LCH[Olink2][wave] < 1) /* channel empty? */
                  {
                     BFassignment(ndr, Olink, wave, 1, NTW, SM_Parameters,Olink2);
                     NTW->OutBuffer[Olink+(ndr)*max_links_per_node][wave][1] = 0; // Remove packet from first link
                     NTW->OutputPack       = NTW->OutputPack+1;  /*# packets*/
                     NTW->LCH[Olink2][wave] = 2;   /* Wavelength is busy */
                     NTW->Outlink[Olink2]++;  /* packets per link */
                  }
               }
            }
}                 /* END EmptyBuffers   */


/* ==============================================================*/
/*                     UpdateBuffers                             */
/*===============================================================*/
/* This subroutine update the buffers                            */
void UpdateBuffers (int ndr, struct NETWORK *NTW, struct SPARAMETERS *SM_Parameters)
{
   int wave, Olink, buffer, Pos_buffer;
   int dest;
   int Banbuffer, Banlink;

   for (Olink = 1; Olink <= NTW->output_links[ndr] + 1; Olink++)
   /* Olink assigment   */
   {
      Banbuffer = 0;
      Banlink   = 0;
      for (wave = 1 ; wave <= SM_Parameters->num_waves; wave++ )
      /* channel asigment  */
         for (buffer = 1; buffer <= NTW->NDL[ndr][Olink]-1; buffer++)
         /* Buffer assigment  */
         {
            Pos_buffer = Olink+(ndr) * max_links_per_node;
            if (NTW->OutBuffer[Pos_buffer][wave][buffer+1] == 1)
            {
               // dest = NTW->Bsource[Pos_buffer][wave][1];
               dest = NTW->connected_to[ndr][Olink];
               if( (NTW->Bsource[Pos_buffer][wave][buffer] != VirtualNode) && (dest != ndr) )
               {

                  printf("\n The packet was lost in buffer - node (%d) - time (%d) ? ",ndr,(int)NTW->tempo);
                  if( (NTW->Bpriority[Pos_buffer][wave][buffer] == 1) &&
                      (NTW->Phase >=2)
                    )
                  {
                      if(NTW->dw == 666)
                         NTW->LostPK[NTW->Phase][SM_Parameters->num_waves - 1][NTW->ThisLoad]++;
                      else
                		 NTW->LostPK[NTW->Phase][NTW->dw - 1][NTW->ThisLoad]++;

                	 //NTW->LostPK[NTW->Phase][NTW->dw - 2][NTW->ThisLoad]++;
                  }
                  else
                  {
                     NTW->numPPL[ndr]++;
                     NTW->numPPLFiber[ndr][Olink]++;
                  }

                  // more DLs
                  more_delays(NTW, SM_Parameters, ndr, dest, Olink);

               }
               NTW->OutBuffer     [Pos_buffer][wave][buffer] = 1;
               NTW->Bfromlink     [Pos_buffer][wave][buffer] = NTW->Bfromlink     [Pos_buffer][wave][buffer+1];
               NTW->Baddress      [Pos_buffer][wave][buffer] = NTW->Baddress      [Pos_buffer][wave][buffer+1];
               NTW->Bsource       [Pos_buffer][wave][buffer] = NTW->Bsource       [Pos_buffer][wave][buffer+1];
               NTW->Bdestination  [Pos_buffer][wave][buffer] = NTW->Bdestination  [Pos_buffer][wave][buffer+1];
               //NTW->Bnum_slots    [Pos_buffer][wave][buffer] = NTW->Bnum_slots    [Pos_buffer][wave][buffer+1];
               //NTW->Bnum_hops     [Pos_buffer][wave][buffer] = NTW->Bnum_hops     [Pos_buffer][wave][buffer+1];
               NTW->Bcell_num     [Pos_buffer][wave][buffer] = NTW->Bcell_num     [Pos_buffer][wave][buffer+1];
               NTW->Bnum_of_cells [Pos_buffer][wave][buffer] = NTW->Bnum_of_cells [Pos_buffer][wave][buffer+1];
               NTW->Bpriority     [Pos_buffer][wave][buffer] = NTW->Bpriority     [Pos_buffer][wave][buffer+1];
               //NTW->Boffset_time  [Pos_buffer][wave][buffer] = NTW->Boffset_time  [Pos_buffer][wave][buffer+1];

               NTW->Bnum_of_flow  [Pos_buffer][wave][buffer] = NTW->Bnum_of_flow  [Pos_buffer][wave][buffer+1];

               NTW->OutBuffer     [Pos_buffer][wave][buffer+1] = 0;
               NTW->Bfromlink     [Pos_buffer][wave][buffer+1] = 0;
               NTW->Baddress      [Pos_buffer][wave][buffer+1] = 0;
               NTW->Bsource       [Pos_buffer][wave][buffer+1] = VirtualNode;
               NTW->Bdestination  [Pos_buffer][wave][buffer+1] = VirtualNode;
               //NTW->Bnum_slots    [Pos_buffer][wave][buffer+1] = 0;
               //NTW->Bnum_hops     [Pos_buffer][wave][buffer+1] = 0;
               NTW->Bcell_num     [Pos_buffer][wave][buffer+1] = 0;
               NTW->Bnum_of_cells [Pos_buffer][wave][buffer+1] = 0;
               NTW->Bpriority     [Pos_buffer][wave][buffer+1] = 0;
               //NTW->Boffset_time  [Pos_buffer][wave][buffer+1] = 0;

               NTW->Bnum_of_flow  [Pos_buffer][wave][buffer+1] = 0;
            }
            else
            {
               NTW->OutBuffer     [Pos_buffer][wave][buffer] = 0;
               NTW->Bfromlink     [Pos_buffer][wave][buffer] = 0;
               NTW->Baddress      [Pos_buffer][wave][buffer] = 0;
               NTW->Bsource       [Pos_buffer][wave][buffer] = VirtualNode;
               NTW->Bdestination  [Pos_buffer][wave][buffer] = VirtualNode;
               //NTW->Bnum_slots    [Pos_buffer][wave][buffer] = 0;
               //NTW->Bnum_hops     [Pos_buffer][wave][buffer] = 0;
               NTW->Bcell_num     [Pos_buffer][wave][buffer] = 0;
               NTW->Bnum_of_cells [Pos_buffer][wave][buffer] = 0;
               NTW->Bpriority     [Pos_buffer][wave][buffer] = 0;
               //NTW->Boffset_time  [Pos_buffer][wave][buffer] = 0;

               NTW->Bnum_of_flow  [Pos_buffer][wave][buffer] = 0;

               NTW->OutBuffer     [Pos_buffer][wave][buffer+1] = 0;
               NTW->Bfromlink     [Pos_buffer][wave][buffer+1] = 0;
               NTW->Baddress      [Pos_buffer][wave][buffer+1] = 0;
               NTW->Bsource       [Pos_buffer][wave][buffer+1] = VirtualNode;
               NTW->Bdestination  [Pos_buffer][wave][buffer+1] = VirtualNode;
               //NTW->Bnum_slots    [Pos_buffer][wave][buffer+1] = 0;
               //NTW->Bnum_hops     [Pos_buffer][wave][buffer+1] = 0;
               NTW->Bcell_num     [Pos_buffer][wave][buffer+1] = 0;
               NTW->Bnum_of_cells [Pos_buffer][wave][buffer+1] = 0;
               NTW->Bpriority     [Pos_buffer][wave][buffer+1] = 0;
               //NTW->Boffset_time  [Pos_buffer][wave][buffer+1] = 0;

               NTW->Bnum_of_flow  [Pos_buffer][wave][buffer+1] = 0;

            }
         }
   }
}        /* END UpdateBuffers */


/* ============================================================= */
/*        Transmission                                           */
/* ============================================================= */
void Transmission(int ndr, struct NETWORK *NTW, struct SPARAMETERS *SM_Parameters)
{

   int   dest, wave, Olink, PAdd, linkin, Naddress ;
   float a,per;

   for (linkin = 1; linkin <= NTW->input_links[ndr]; linkin++)
   /* Link assigment      */
      for (wave = 1 ; wave <= SM_Parameters->num_waves; wave++ )
      /* wavelength asigment */
      {
         dest = NTW->destination[NTW->num_input_link[ndr][linkin]][wave][1];
         /*packet destination*/

         if ( (NTW->tempo == 1000) || (NTW->tempo == 5000) || (NTW->tempo == 10000) )
            fprintf (fscreen,"ndr=%d,dest_node=%d,num_outputs=%d,linkin=%d,wave=%d,num_in_link=%d,",ndr,dest,NTW->num_outputs[ndr][dest],linkin,wave,NTW->num_input_link[ndr][linkin]) ;

         if (dest != VirtualNode)         /* Is there one packet? */
         {
            if(ndr == NTW->nbr_of_nodes - 1)
               NTW->num_in_packets++;

            NTW->NA[linkin][wave] = 1;    /* always assign one output */
            if (dest == ndr)       /* Is this router the destination */
            {
               NTW->NA[linkin][wave] = 1;
            }
            else
            {
               /* assign the number of possible outputs */
               NTW->NA[linkin][wave] = NTW->num_outputs[ndr][dest];
            }
         }
         else
         {
            NTW->NA[linkin][wave]  = 0;  /* NO dumi packets around   */
         }
      }
}       /* END TRANSMISSION */

/* =============================================================*/
/*            # of empty slots in the buffers                   */
/*==============================================================*/
/* This subroutine solve blocking using Buffering               */
void  EmptySlotsinBuffers (int ndr, struct NETWORK *NTW, struct SPARAMETERS *SM_Parameters)
{
   int       wave,Olink, buffer;

   for (Olink = 1; Olink <= NTW->output_links[ndr] + 1; Olink++)
   /* Olink assigment   */
      NTW->EmptySlots[Olink] = 0;

   for (Olink = 1; Olink <= NTW->output_links[ndr] + 1; Olink++)
   /* Olink assigment   */
      for (wave = 1 ; wave <=SM_Parameters->num_waves; wave++ )
      /* channel asigment  */
         for (buffer = 1; buffer <= NTW->NDL[ndr][Olink]; buffer++)
         /* Buffer assigment  */
            if  (NTW->OutBuffer[Olink+(ndr) * max_links_per_node][wave][buffer] == 0)
               NTW->EmptySlots[Olink]++;
}

/* ============================================================*/
/*                     process_priorities                       */
/* ============================================================*/
/*This subroutine takes care of packets with priority equal to 1*/
/* the packet must be assigned!!!                               */
void process_priorities (int ndr, struct NETWORK *NTW, struct SPARAMETERS *SM_Parameters)
{

   int preference, wt, ca, waveD,linkD,wave,link, Naddress, Olink, AAdd, buffer,dest, src;
   int Failure_Link, blocked, wave_blocked;

   //XX
   // New Definitions
   int F[max_links_per_node+1];
   int W[max_wavelength+1];
   int b, f, ft;
   int assigned;
   int MyFiber;
   int First_Option;
   int next_node;
   int Alternatives, second_node, Option; // = [max_links_per_node+1];

   int LPpriority;

   RandomSeries(ndr, NTW,SM_Parameters);
   for (linkD = 1; linkD <= NTW->input_links[ndr]; linkD++)
   /* Link assigment   */
      for (waveD = 1 ; waveD <= SM_Parameters->num_waves; waveD++ )
      /* channel asigment */
      {
         link     = NTW->SR[linkD]; // linkD; //NTW->SR[linkD];
         wave     = NTW->ST[waveD]; // waveD; //NTW->ST[waveD];
         Naddress = 1;
         /* ****************** PRIORITIES ********************** */

         dest       = NTW->destination[NTW->num_input_link[ndr][link]][wave][1];
         src        = NTW->source     [NTW->num_input_link[ndr][link]][wave][1];
         LPpriority = NTW->priority   [NTW->num_input_link[ndr][link]][wave][1];

         //if ( (preference == prioridad) && (NTW->NA[link][wave] >= 1) )
         if(LPpriority == 1)
         {
            assigned = 0;
            Alternatives = NTW->num_outputs[ndr][dest];

            // ONLY SP
            //Alternatives = 1;

            // CHANGE 19-05-2013
       	    if(NTW->Phase < 3) // Dimensioning only SP
            {
       		   // IT WAS CHANGED!!!
               // To determine the number of alternatives
               // If Phase <= 2 the nodes will know only about
               // the first path. However just in case of failure between
               // links A and B, the nodes A and B can use the second path.
               //      next_node = NTW->forw_table[ndr][dest][???];
               //      MyFiber = NTW->forw_output[ndr][dest][???];
               // First Option
               Alternatives = 0;

               next_node    = NTW->forw_table[ndr][dest][1];
               MyFiber      = NTW->forw_output[ndr][dest][1];

               Failure_Link = NTW->num_output_link[ndr][MyFiber];
               blocked      = FREE;
               blocked      = search_link_failure(NTW,Failure_Link, SM_Parameters,wave,dest,ndr);
               second_node  = VirtualNode;
               for(f = 1; f <= NTW->num_outputs[ndr][dest]; f++)
               {
                  Option = NTW->forw_output[ndr][dest][f];
                  if( (NTW->connected_to[ndr][Option] == next_node) )
                  {
                     Alternatives++;
                  }
                  else
                  {
                     if( (NTW->Phase == 2) && (blocked))
                     {
                        if(second_node == VirtualNode)
                           second_node = NTW->forw_table[ndr][dest][f];
                        if(NTW->connected_to[ndr][Option] == second_node)
                           Alternatives++;
                     }
                  }
               }
            }
            else
            {
               Alternatives = NTW->num_outputs[ndr][dest];
            }
            // END CHANGE 19-05-2013

        	RandomSeriesFW(Alternatives, SM_Parameters->num_waves, &F, &W, wave, NTW, ndr, dest,1);
        	//for(b = 0; b <= SM_Parameters->ADDNDL && assigned == 0; b++) // Exploring buffers
        	for(b = 0; b <= SM_Parameters->AllowedDL && assigned == 0; b++)
        	//for(b = 0; b <= 0 && assigned == 0; b++)
        	{
               wt = wave;
        	   for(f=1; f <= Alternatives && assigned == 0; f++) // Exploring fibers
               {
                  ft = F[f];
        		  next_node = NTW->forw_table[ndr][dest][ft];
                  if (dest == ndr)
        		     MyFiber = 1;
        		  else
        		     MyFiber = NTW->forw_output[ndr][dest][ft];

                  blocked      = FREE;
                  wave_blocked = FREE;
                  Failure_Link = NTW->num_output_link[ndr][MyFiber];
                  blocked      = search_link_failure(NTW,Failure_Link, SM_Parameters,wave,dest,ndr);
                  wave_blocked = search_wave_failure(NTW,Failure_Link,wt,MyFiber, SM_Parameters,dest, ndr);

                 if(b == 0)
                 {
                    if( (wave_blocked != ALL_BLOCKED) && (!blocked) )
                    {
                       if(wave_blocked == ONE_FREE)
                	   {
                          wt = NTW->FreeW; // it comes from search_wave_failure
                          if(wt >= 1)
                             wave_blocked = FREE;
                          else
                		     printf("\n HORRIBLE MISTAKE THERE MUST BE ONE FREE WAV");
                       }
                	   //
                	   if(wave_blocked == FREE)
                	   {
                          if(MyFiber > 1)
                          {
                             /* # of packet that exit  */
                             NTW->OutputPack = NTW->OutputPack+1;
                             /* The wavelength is busy */
                             NTW->LCH[MyFiber][wt] = 2;
                             /* # of packets in a link */
                             NTW->Outlink[MyFiber]++;
                           }

                           FTallocation(ndr,link,MyFiber,ft,wave,wt,NTW, SM_Parameters);

                           for (AAdd = 1 ; AAdd <= 7; AAdd++)
                              if (NTW->num_outputs[ndr][dest] == AAdd)
                                 NTW->Pc[ndr][NTW->num_outputs[ndr][dest]]++;

                           NTW->OutlinkUsage[ndr][MyFiber]++;
                              assigned = 1;
                              //break;
                	    }
                	    else
                	    {
                		   printf("\n SORRY_WAVELENGHTS_ARE_NOT FREE U MUST TRY BUFFERING");
                	    }
                    }// End WAVEBLOCKED
                 }// End b == 0
                 else
                 {
                 	 // B is greater than or equal to 1
                     if( (b <= NTW->NDL[ndr][MyFiber]) && (assigned == 0) && (!blocked))
                     {
                        // Buffers
                        if( (NTW->OutBuffer[MyFiber+(ndr) * max_links_per_node][wt][b] == 0) &&
                     	   (NTW->StoreBuff== 0) )
                        {
                     	   printf("\n THEN_BUFFERING %d - Phase %d \t Tempo %d ",b,NTW->Phase,NTW->tempo);
                     	   if(wt != wave) { NTW->WCactive[ndr]++;  NTW->WCUsageBA[ndr][MyFiber]++; }
                           BFallocation(ndr, link, MyFiber, wave, wt, b,ft,NTW,SM_Parameters);
                           NTW->BufferUsage[ndr][MyFiber]++;
                           assigned = 1;
                        }
                     }// END if(b <= NTW->NDL[ndr][MyFiber])

                	 /*
                	 if(MyFiber != 1)
                	 {
                        // Increasing DL
                        if ( (NTW->NDL[ndr][MyFiber] <= SM_Parameters->AllowedDL-1) &&
                	         (NTW->Phase == 2)
                	       )
                	    {
                	       // If there are more fibers pointing to the same node.
                	       if (NTW->num_of_fibers[ndr][NTW->forw_table[ndr][dest][1]] >= 200)
                	       {
                	          // To increase DL for each fiber pointing to the same node
                	          for (AAdd = 1; AAdd <= NTW->output_links[ndr] + 1; AAdd++)
                	          {
                	             // If this fiber connects to Olink, i.e. extra fiber
                	             if(NTW->connected_to[ndr][AAdd] == NTW->connected_to[ndr][MyFiber])
                	             {
                	                if(NTW->NDL[ndr][AAdd] <= SM_Parameters->AllowedDL-1)
                	                   NTW->NDL[ndr][AAdd] = NTW->NDL[ndr][AAdd] + SM_Parameters->ADDNDL;
                	             }
                	          }//end for
                	       }
                	       else
                	          NTW->NDL[ndr][MyFiber] = NTW->NDL[ndr][MyFiber] + SM_Parameters->ADDNDL;
                	    }
                        // End if
                        // More fibers ??
                        if ((DIMEN==1) && (NTW->Phase == 2) && (dest!=VirtualNode) &&
                    	    (NTW->NDL[ndr][MyFiber]==SM_Parameters->AllowedDL) &&
                    	    (NTW->output_links[ndr]<max_links_per_node) )
                        {
                           if( (NTW->n_links < max_num_links) && (SM_Parameters->Umbral < 1.0))
                              more_links(ndr,next_node,MyFiber,NTW,SM_Parameters);
                        }
                	 }// End MyFiber != 1
                     */
                	 // END Generating DLs and links
                     // END ELSE
                 }
        	   }// End for f = 1 - Exploring fibers
            }//End buffers
            //}// End w = 1
         }// End LPPriority
         else
        	 assigned = 1;

         if(assigned == 0)
         {
        	 printf("\n\t\t Couldn't_make_it %d - %d - %d -- %d", wave, dest, ndr, NTW->Phase);
        	 printf("\n\t\t Couldn't_make_it %d - %d - %d -- %d", wave, dest, ndr, NTW->Phase);
        	 // To which fiber?

        	 if(dest >= NTW->nbr_of_nodes)
        	 {
        		 dest = dest * 1;
        		 printf("\n\t\t BAD_DEST_HERE %d - %d - %d ", wave, dest, ndr);
        	 }

        	 if (dest == ndr)
        	 {
                MyFiber = 1;
        	 }
        	 else
        	 {
        	    MyFiber   = NTW->forw_output[ndr][dest][1]; // First option, i.e. Shortest Path!!!
        	    next_node = NTW->forw_table[ndr][dest][1];
        	 }

        	 if(MyFiber != 1)
        	 {
                // Increasing DL
                /*if ( (NTW->NDL[ndr][MyFiber] <= SM_Parameters->AllowedDL-1) &&
        	         (NTW->Phase == 2)
        	       )
        	    {
        	       // If there are more fibers pointing to the same node.
        	       if (NTW->num_of_fibers[ndr][NTW->forw_table[ndr][dest][1]] >= 200)
        	       {
        	          // To increase DL for each fiber pointing to the same node
        	          for (AAdd = 1; AAdd <= NTW->output_links[ndr] + 1; AAdd++)
        	          {
        	             // If this fiber connects to Olink, i.e. extra fiber
        	             if(NTW->connected_to[ndr][AAdd] == NTW->connected_to[ndr][MyFiber])
        	             {
        	                if(NTW->NDL[ndr][AAdd] <= SM_Parameters->AllowedDL-1)
        	                   NTW->NDL[ndr][AAdd] = NTW->NDL[ndr][AAdd] + SM_Parameters->ADDNDL;
        	             }
        	          }//end for
        	       }
        	       else
        	          NTW->NDL[ndr][MyFiber] = NTW->NDL[ndr][MyFiber] + SM_Parameters->ADDNDL;
        	    }*/
                // End if
                // More fibers ??
                if ((DIMEN==1) && (NTW->Phase == 2) && (dest!=VirtualNode) &&
            	    (NTW->NDL[ndr][MyFiber]==SM_Parameters->AllowedDL) &&
            	    (NTW->output_links[ndr]<max_links_per_node) )
                {
                   if( (NTW->n_links < max_num_links) && (SM_Parameters->Umbral < 1.0))
                      more_links(ndr,next_node,MyFiber,NTW,SM_Parameters);
                }
        	 }// End MyFiber != 1

         }// End assigned == 0

         /* ****************** PRIORITIES ********************** */
      }// End for waveD
}       /* END  process_priorities */

/* ============================================================*/
/*                     Translation                             */
/* ============================================================*/
/*This subroutine solves blocking using wavelength translation,*/
/* MPR and Buffering                                           */
void Translation (int prioridad, int ndr, struct NETWORK *NTW, struct SPARAMETERS *SM_Parameters)
{

   int preference, wt, ca, waveD,linkD,wave,link, Naddress, Olink, AAdd, buffer,dest,scr;
   int Failure_Link, blocked, wave_blocked;

   // New Definitions
   int F[max_links_per_node+1];
   int W[max_wavelength+1];
   int b, w, f, ft;
   int assigned;
   int MyFiber;
   int First_Option;
   int Next_NodeSP;
   int Next_NodeNow;
   int next_node;
   int Alternatives, second_node, Option; // = [max_links_per_node+1];

   int LPpriority;
   int AreLPR;
   AreLPR = get_node_status(NTW,ndr,SM_Parameters);

   RandomSeries(ndr, NTW,SM_Parameters);
   for (linkD = 1; linkD <= NTW->input_links[ndr]; linkD++)
   /* Link assigment   */
      for (waveD = 1 ; waveD <= SM_Parameters->num_waves; waveD++ )
      /* channel asigment */
      {
         link     = NTW->SR[linkD];
         wave     = NTW->ST[waveD];
         Naddress = 1;
         /* ****************** PRIORITIES ********************** */

         if (routing == 0)
            preference = prioridad;            /* Random Allocation    */
         else
            preference = NTW->num_outputs[ndr][dest]; //NTW->NA[link][wave]; /*  priorities */

         dest       = NTW->destination[NTW->num_input_link[ndr][link]][wave][1];
         scr        = NTW->source     [NTW->num_input_link[ndr][link]][wave][1];
         LPpriority = NTW->priority   [NTW->num_input_link[ndr][link]][wave][1];

         //if ( (preference == prioridad) && (NTW->NA[link][wave] >= 1) )
         if(LPpriority == 1)
         {
            // THERE MUST BE SOMETHING WRONG!!!!
            printf("\n\t\t\t BAD_EXTREAMLY_BAD!!!!!  scr %d ndr %d  dest %d - %d - %d",scr,ndr,dest,NTW->tempo,NTW->Phase);
            // Let's drop the packet
            //NTW->destination[NTW->num_input_link[ndr][link]][wave][1] = VirtualNode;
            //NTW->source     [NTW->num_input_link[ndr][link]][wave][1] = VirtualNode;
            //NTW->priority   [NTW->num_input_link[ndr][link]][wave][1] = 0;

            // Count PPL for this
            //NTW->LostPK[NTW->Phase][NTW->ThisLoad]++;
            //assigned = 1;

            //dest = VirtualNode;

         }// End LPPriority
         //else
         //{
        	//if(assigned == 0)
            if ( (preference == prioridad) && (dest != VirtualNode) )
            {
               // NA has the set of alternative paths

               //if( ( (Criterio == 1) && (NTW->NA[link][wave] > 1) ) || (NTW->Phase == 3) )
               //if ( (Criterio == 1) && (NTW->NA[link][wave] >= 1) )
               if((Criterio == 1) && (NTW->num_outputs[ndr][dest] >= 1))
               {

                  // NTW->NA[link][wave] = number of possible outputs
                  assigned = 0;
                  // Exploring buffers
                  if(NTW->Phase <= 9)
                  {
                     if(NTW->Phase < 3) // Dimensioning only SP
                     {
                        // IT WAS CHANGED!!!
                        // To determine the number of alternatives
                        // If Phase <= 2 the nodes will know only about
                        // the first path. However just in case of failure between
                        // links A and B, the nodes A and B can use the second path.
                        //      next_node = NTW->forw_table[ndr][dest][???];
                        //      MyFiber = NTW->forw_output[ndr][dest][???];
                        // First Option
                        Alternatives = 0;

                        next_node    = NTW->forw_table[ndr][dest][1];
                        MyFiber      = NTW->forw_output[ndr][dest][1];

                        Failure_Link = NTW->num_output_link[ndr][MyFiber];
                        blocked      = FREE;
                        blocked      = search_link_failure(NTW,Failure_Link, SM_Parameters,wave,dest,ndr);
                        second_node  = VirtualNode;
                        for(f = 1; f <= NTW->num_outputs[ndr][dest]; f++)
                        {
                           Option = NTW->forw_output[ndr][dest][f];
                           if( (NTW->connected_to[ndr][Option] == next_node) )
                           {
                              Alternatives++;
                           }
                           else
                           {
                              if( (NTW->Phase == 2) && (blocked))
                              {
                                 if(second_node == VirtualNode)
                                    second_node = NTW->forw_table[ndr][dest][f];
                                 if(NTW->connected_to[ndr][Option] == second_node)
                                    Alternatives++;
                              }
                           }
                        }
                     }
                     else
                     {
                        Alternatives = NTW->num_outputs[ndr][dest];
                     }

                     RandomSeriesFW(Alternatives, SM_Parameters->num_waves, &F, &W, wave, NTW, ndr, dest,0);
                     // To keep wavelength
                     // Recorrer Fibras con DL=0 y misma W
                     // Recorrer Buffers 1:MAX_DL y misma W
                     // Recorrer Fibras hasta en encontrar W libre para DL=0
                     // Recorrer Buffers 1:MAX_DL hasta encontrar W libre
                     // Exploring wavelengths
                     for(w=1; w <= SM_Parameters->num_waves && assigned == 0; w++)
                     {
                        wt = W[w];
                        // Exploring buffers
                        for(b = 0; b <= SM_Parameters->AllowedDL && assigned == 0; b++)
                        {
                           // Exploring fibers
                           //for(f=1; f <= NTW->NA[link][wave] && assigned == 0; f++)
                           for(f=1; f <= Alternatives && assigned == 0; f++)
                           {
                              ft = F[f];
                              // =======================================================
                              //dest      = NTW->destination[NTW->num_input_link[ndr][link]][wave][1];
                              next_node = NTW->forw_table[ndr][dest][ft];
                              if (dest == ndr)
                                 MyFiber = 1;
                              else
                                 MyFiber = NTW->forw_output[ndr][dest][ft];

                              // Fibers
                              /* REAL OUTPUT LINK  BETWEEN 0 - max_num_links */
                              blocked      = FREE;
                              wave_blocked = FREE;
                              Failure_Link = NTW->num_output_link[ndr][MyFiber];

							 			//if( (NTW->Phase == 2) || (NTW->Phase == 5) || (NTW->Phase == 8)) // Dimensioning with failures
                              //if(NTW->Phase == 2) // Dimensioning with failures
                              if(NTW->Phase >= 2) // Introducing Failures when the dimensioning was performed without them.
                              {
                                 blocked      = search_link_failure(NTW,Failure_Link, SM_Parameters,wave,dest,ndr);
                                 //wave_blocked = search_wave_failure(NTW,Failure_Link,wt,MyFiber, SM_Parameters);
                              }

                              // Always check wavelengths
                              wave_blocked = search_wave_failure(NTW,Failure_Link,wt,MyFiber, SM_Parameters,dest, ndr);
                              if(b == 0)
                              {

                                 if( (blocked) && (NTW->Phase >= 2) )
                                 //if( (blocked) && ((NTW->Phase == 2) || (NTW->Phase == 5) || (NTW->Phase == 8)) )
                                 {
                                    printf("\n Failure in fiber (%d) at time (%d) - node (%d) ", Failure_Link,(int)NTW->tempo,ndr);
                                 }

                                 if( (wave_blocked != ALL_BLOCKED) && (!blocked) )
                                 {
                                    if (wave_blocked == FREE)  /* Is the wavelength free */
                                    {
                                       //printf("\n HERE WE CAN FORWARD THE PACKET");
                                       if(wt != wave)
                                       {
                                          /*if ( (wt > NTW->dw) || // Wave bigger than dw or
                                         	     (AreLPR == 666)   // Node not involved in a LPrequest
                                         	 )  //
                                          {*/
                                             // Wavelength conversion
                                             NTW->WCUsage[ndr][MyFiber]++;
                                             NTW->WCactive[ndr]++;
                                          /*}
                                          else
                                          {
                                             assigned = 999;
                                        	 printf("\n U_CANNOT_USE_THIS_WAV_FIBER NDR %d DEST %d FIB %d WAV %d TMP %d Phase %d ",ndr,dest,Failure_Link,wt,NTW->tempo,NTW->Phase);
                                          }*/
                                       }

                                       if(assigned == 0)
                                       {

                                    	   FTallocation(ndr,link,MyFiber,ft,wave,wt,NTW, SM_Parameters);

                                          if(MyFiber > 1)
                                          {
                                             /* # of packet that exit  */
                                             NTW->OutputPack = NTW->OutputPack+1;
                                             /* The wavelength is busy */
                                             NTW->LCH[MyFiber][wt] = 2;
                                             /* # of packets in a link */
                                             NTW->Outlink[MyFiber]++;
                                          }

                                          for (AAdd = 1 ; AAdd <= 7; AAdd++)
                                            if (NTW->num_outputs[ndr][dest] == AAdd)
                                              NTW->Pc[ndr][NTW->num_outputs[ndr][dest]]++;

                                          NTW->OutlinkUsage[ndr][MyFiber]++;

                                          assigned = 1;
                                       }
                                    }// END of if ( (wave_blocked == FREE) && ( (AreLPR == 666) || (wt > NTW->dw)) )
                                    //else
                                 }
                              }// END if(b == 0)
                              else
                              {
                                 //if(b <= f->DL)
                                 if( (b <= NTW->NDL[ndr][MyFiber]) && (!blocked) )
                                 {
                                    // Buffers
                                    if ((NTW->OutBuffer[MyFiber+(ndr) * max_links_per_node][wt][b] == 0) && (NTW->StoreBuff== 0) )
                                    {
                                       if(wt != wave)
                                       {
                                          /*if ( (wt > NTW->dw) || // Wave bigger than dw or
                                               (AreLPR == 666)   // Node not involved in a LPrequest
                                             )  //
                                          {*/
                                             NTW->WCactive[ndr]++;
                                             NTW->WCUsageBA[ndr][MyFiber]++;
                                          /*}
                                          else
                                          {
                                        	  assigned = 999;
                                        	  printf("\n U_CANNOT_USE_THIS_WAV_BUFF NDR %d DEST %d FIB %d WAV %d TMP %d Phase %d ",ndr,dest,Failure_Link,wt,NTW->tempo,NTW->Phase);
                                          }*/
                                       }

                                       if(assigned == 0)
                                       {
                                       BFallocation(ndr, link, MyFiber, wave, wt, b,ft,NTW,SM_Parameters);
                                       NTW->BufferUsage[ndr][MyFiber]++;
                                       assigned = 1;
                                       }
                                       //allocate(f,w);
                                    }
                                 }// END ( (b <= NTW->NDL[ndr][MyFiber]) && (!blocked) && ...
                                 //else

                              }
                              // =======================================================
                           } // END for(f=1; f <= NTW->NA[link][wave] && assigned == 0; f++)
                        } // END for(b = 0; b <= SM_Parameters->AllowedDL && assigned == 0; b++)

                        // If we apply wavelenght conversion we should increase the DL of the last
                        // explored fiber.
                        // Increasing DL.
                        if ( (NTW->NDL[ndr][MyFiber] <= SM_Parameters->AllowedDL-1) && (assigned == 0) && (NTW->Phase == 2) && (!blocked)) //
                        {
                           // If there are more fibers pointing to the same node.
                           if (NTW->num_of_fibers[ndr][NTW->forw_table[ndr][dest][1]] >= 2)
                           {
                              // To increase DL for each fiber pointing to the same node
                              for (AAdd = 1; AAdd <= NTW->output_links[ndr] + 1; AAdd++)
                              {
                                 // If this fiber connects to Olink, i.e. extra fiber
                                 if(NTW->connected_to[ndr][AAdd] == NTW->connected_to[ndr][MyFiber])
                                 {
                                    if(NTW->NDL[ndr][AAdd] <= SM_Parameters->AllowedDL-1)
                                       NTW->NDL[ndr][AAdd] = NTW->NDL[ndr][AAdd] + SM_Parameters->ADDNDL;
                                 }
                              }
                           }
                           else
                              NTW->NDL[ndr][MyFiber] = NTW->NDL[ndr][MyFiber] + SM_Parameters->ADDNDL;
                        }
                     }// END for(w=1; w <= SM_Parameters->num_waves && assigned == 0; w++)

                     if ( assigned == 0)
                     {
                        if( (f > 1) && (LPpriority != 1))
                           NTW->numLossmpr[ndr]++;

                        // ==============================================================================
                        // To dimension the "first" fiber
                        blocked = BLOCKED;
                        //for(f=1; f <= NTW->num_outputs[ndr][dest] && blocked == BLOCKED; f++)
                        for(f=1; f <= Alternatives && blocked == BLOCKED; f++)
                        {
                           MyFiber      = NTW->forw_output[ndr][dest][f];
                           next_node    = NTW->forw_table[ndr][dest][f];
                           Failure_Link = NTW->num_output_link[ndr][MyFiber];
                           if(NTW->Phase == 2)
                              blocked = search_link_failure(NTW,Failure_Link, SM_Parameters,wave,dest,ndr);
                           else
                              blocked = FREE;
                        }


                        Discard (ndr,link,wave,NTW,SM_Parameters, dest);
                        NTW->Drop_P_Link[ndr][MyFiber]++;

                        // Increasing DL
                        if ( (NTW->NDL[ndr][MyFiber] <= SM_Parameters->AllowedDL-1) && (NTW->Phase == 2))
                        {
                           // If there are more fibers pointing to the same node.
                           if (NTW->num_of_fibers[ndr][NTW->forw_table[ndr][dest][1]] >= 200)
                           {
                              // To increase DL for each fiber pointing to the same node
                              for (AAdd = 1; AAdd <= NTW->output_links[ndr] + 1; AAdd++)
                              {
                                 // If this fiber connects to Olink, i.e. extra fiber
                                 if(NTW->connected_to[ndr][AAdd] == NTW->connected_to[ndr][MyFiber])
                                 {
                                    if(NTW->NDL[ndr][AAdd] <= SM_Parameters->AllowedDL-1)
                                       NTW->NDL[ndr][AAdd] = NTW->NDL[ndr][AAdd] + SM_Parameters->ADDNDL;
                                 }
                              }
                           }
                           else
                              NTW->NDL[ndr][MyFiber] = NTW->NDL[ndr][MyFiber] + SM_Parameters->ADDNDL;
                        }
                        //printf("\n\n\n THERE_MUST_BE A NEW FIBER");
                        // More fibers ??
                        if ((DIMEN==1) &&
                        	(NTW->Phase == 2) &&
                        	(dest!=VirtualNode) &&
                        	(NTW->NDL[ndr][MyFiber]==SM_Parameters->AllowedDL) &&
                        	(NTW->output_links[ndr]<max_links_per_node) )
                        {
                           if( (NTW->n_links < max_num_links) && (SM_Parameters->Umbral < 1.0))
                              more_links(ndr,next_node,MyFiber,NTW,SM_Parameters);
                        }
                        //
                        // ??? EmptySlotsinBuffers(ndr,NTW, SM_Parameters);
                        //
                        NTW->numdef[ndr]++; /* the packet is deflected*/
                        if( (LPpriority == 1) &&
                            (NTW->Phase >=2)
                          )
                        {
                            if(NTW->dw == 666)
                               NTW->LostPK[NTW->Phase][SM_Parameters->num_waves - 1][NTW->ThisLoad]++;
                            else
                      		   NTW->LostPK[NTW->Phase][NTW->dw - 1][NTW->ThisLoad]++;

                        	//NTW->LostPK[NTW->Phase][NTW->dw - 2][NTW->ThisLoad]++;
                        }
                        else
                        {
                           NTW->numPPL[ndr]++; /* or loss                */
                           NTW->numPPLFiber[ndr][MyFiber]++;
                        }
                        assigned = 0;
                     }
                  }// END if NTW->Phase <= 3
               }
            }/* end del if ((preference==prioridad) ) */
         //}// End else

         /* ****************** PRIORITIES ********************** */
      }
}       /* END  TRANSLATION */


/* ==============================================================*/
/*           BUFFER OUTPUT ASSIGNMENT                            */
/*===============================================================*/
/* This subroutine allocate Packets in the output links          */
void BFassignmentPriority(int ndr, int Olink, int wave, int buffer, struct NETWORK *NTW, struct SPARAMETERS *SM_Parameters, int Olink2)
{

   int dest,to_link,from_link, next_node, Aux_Pos, blocked;

   Aux_Pos   = Olink + (ndr) * max_links_per_node;
   dest      = NTW->Bdestination[Olink+(ndr)*max_links_per_node][wave][buffer];
   next_node = NTW->forw_table[ndr][dest][NTW->Baddress[Aux_Pos ][wave][buffer]];

   if (dest == ndr)
      if (Olink != 1)
      {
         printf ("problemas problemas \n");
         printf ("ndr=%d, Baddress=%d, Bdest=%d, Olink=%d \n",ndr,NTW->Baddress[Aux_Pos][wave][buffer],dest,Olink );
      }

   if ( (next_node != NTW->connected_to[ndr][Olink2]) && (NTW->Phase >= 3) )
   {
      printf ("BFassignment Priority \n") ;
      printf ("next_node != connected_to[ndr][Olink2] -- %d \n",dest) ;
      printf ("num_of_link=%d,num_output_link=%d,ndr=%d,dest=%d,Baddress=%d,next_node=%d,connected_to=%d,Olink=%d\n", NTW->num_of_link[ndr][next_node][1],NTW->num_output_link[ndr][Olink2],ndr,dest,NTW->Baddress[Aux_Pos][wave][buffer],next_node,NTW->connected_to[ndr][Olink2],Olink2);
      exit (-1);
   }

   /* to_link=num_of_link[ndr][next_node][1]; */
   to_link   = NTW->num_output_link[ndr][Olink2];     // Allocate packet in second option
   from_link = NTW->Bfromlink[Aux_Pos][wave][buffer]; // Take packet from first option

   if (Olink == 1)
      Absorption(ndr,from_link,Olink,wave,dest,2,NTW, SM_Parameters);
   else
   {
        	 NTW->source      [to_link][wave][NTW->link_dist[ndr][next_node]] = NTW->Bsource      [Aux_Pos][wave][buffer];
        	 NTW->destination [to_link][wave][NTW->link_dist[ndr][next_node]] = NTW->Bdestination [Aux_Pos][wave][buffer];
             //NTW->num_slots   [to_link][wave][NTW->link_dist[ndr][next_node]] = NTW->Bnum_slots   [Aux_Pos][wave][buffer];
        	 //NTW->num_hops    [to_link][wave][NTW->link_dist[ndr][next_node]] = NTW->Bnum_hops    [Aux_Pos][wave][buffer];
             NTW->cell_num    [to_link][wave][NTW->link_dist[ndr][next_node]] = NTW->Bcell_num    [Aux_Pos][wave][buffer];
        	 NTW->num_of_cells[to_link][wave][NTW->link_dist[ndr][next_node]] = NTW->Bnum_of_cells[Aux_Pos][wave][buffer];
        	 NTW->priority    [to_link][wave][NTW->link_dist[ndr][next_node]] = NTW->Bpriority    [Aux_Pos][wave][buffer];

        	 NTW->num_of_flow [to_link][wave][NTW->link_dist[ndr][next_node]] = NTW->Bnum_of_flow[Aux_Pos][wave][buffer];
   }


   NTW->Bfromlink    [Aux_Pos][wave][buffer] = 0;
   NTW->Baddress     [Aux_Pos][wave][buffer] = 0;
   NTW->Bsource      [Aux_Pos][wave][buffer] = VirtualNode;
   NTW->Bdestination [Aux_Pos][wave][buffer] = VirtualNode;
   //NTW->Bnum_slots   [Aux_Pos][wave][buffer] = 0;
   //NTW->Bnum_hops    [Aux_Pos][wave][buffer] = 0;
   NTW->Bcell_num    [Aux_Pos][wave][buffer] = 0;
   NTW->Bnum_of_cells[Aux_Pos][wave][buffer] = 0;
   NTW->Bpriority    [Aux_Pos][wave][buffer] = 0;
   //NTW->Boffset_time [Aux_Pos][wave][buffer] = 0;

   NTW->Bnum_of_flow [Aux_Pos][wave][buffer] = 0;

}

/* ==============================================================*/
/*           BUFFER OUTPUT ASSIGNMENT                            */
/*===============================================================*/
/* This subroutine allocate Packets in the output links          */
void BFassignment(int ndr, int Olink, int wave, int buffer, struct NETWORK *NTW, struct SPARAMETERS *SM_Parameters, int Olink2)
{

   int dest,to_link,from_link, next_node, Aux_Pos, blocked;

   Aux_Pos   = Olink + (ndr) * max_links_per_node;
   dest      = NTW->Bdestination[Olink+(ndr)*max_links_per_node][wave][buffer];
   next_node = NTW->forw_table[ndr][dest][NTW->Baddress[Aux_Pos ][wave][buffer]];

   if (dest == ndr)
      if (Olink != 1)
      {
         printf ("problemas problemas \n");
         printf ("ndr=%d, Baddress=%d, Bdest=%d, Olink=%d \n",ndr,NTW->Baddress[Aux_Pos][wave][buffer],dest,Olink );
      }

   if ( (next_node != NTW->connected_to[ndr][Olink2]) && (NTW->Phase >= 3) )
   {
      printf ("BFassignment \n") ;
      printf ("next_node != connected_to[ndr][Olink2] -- %d \n",dest) ;
      printf ("num_of_link=%d,num_output_link=%d,ndr=%d,dest=%d,Baddress=%d,next_node=%d,connected_to=%d,Olink=%d\n", NTW->num_of_link[ndr][next_node][1],NTW->num_output_link[ndr][Olink2],ndr,dest,NTW->Baddress[Aux_Pos][wave][buffer],next_node,NTW->connected_to[ndr][Olink2],Olink2);
      exit (-1);
   }

   /* to_link=num_of_link[ndr][next_node][1]; */
   to_link   = NTW->num_output_link[ndr][Olink2];     // Allocate packet in second option
   from_link = NTW->Bfromlink[Aux_Pos][wave][buffer]; // Take packet from first option

   if (Olink == 1)
      Absorption(ndr,from_link,Olink,wave,dest,2,NTW, SM_Parameters);
   else
   {
      // In this process the packets from Buffers are inserted into the links.
      // If links are blocked the process will be restricted.
      //if(NTW->Phase == 2)
      //if((NTW->Phase == 2) || (NTW->Phase == 5) || (NTW->Phase == 8))
      if(NTW->Phase >= 2)
      {
        	 NTW->source      [to_link][wave][NTW->link_dist[ndr][next_node]] = NTW->Bsource      [Aux_Pos][wave][buffer];
        	 NTW->destination [to_link][wave][NTW->link_dist[ndr][next_node]] = NTW->Bdestination [Aux_Pos][wave][buffer];
             //NTW->num_slots   [to_link][wave][NTW->link_dist[ndr][next_node]] = NTW->Bnum_slots   [Aux_Pos][wave][buffer];
        	 //NTW->num_hops    [to_link][wave][NTW->link_dist[ndr][next_node]] = NTW->Bnum_hops    [Aux_Pos][wave][buffer];
             NTW->cell_num    [to_link][wave][NTW->link_dist[ndr][next_node]] = NTW->Bcell_num    [Aux_Pos][wave][buffer];
        	 NTW->num_of_cells[to_link][wave][NTW->link_dist[ndr][next_node]] = NTW->Bnum_of_cells[Aux_Pos][wave][buffer];
        	 NTW->priority    [to_link][wave][NTW->link_dist[ndr][next_node]] = NTW->Bpriority    [Aux_Pos][wave][buffer];

        	 NTW->num_of_flow [to_link][wave][NTW->link_dist[ndr][next_node]] = NTW->Bnum_of_flow [Aux_Pos][wave][buffer];
      }
      else
      {
         NTW->source      [to_link][wave][NTW->link_dist[ndr][next_node]] = NTW->Bsource      [Aux_Pos][wave][buffer];

         NTW->destination [to_link][wave][NTW->link_dist[ndr][next_node]] = NTW->Bdestination [Aux_Pos][wave][buffer];
         //NTW->num_slots   [to_link][wave][NTW->link_dist[ndr][next_node]] = NTW->Bnum_slots   [Aux_Pos][wave][buffer];
         //NTW->num_hops    [to_link][wave][NTW->link_dist[ndr][next_node]] = NTW->Bnum_hops    [Aux_Pos][wave][buffer];
         NTW->cell_num    [to_link][wave][NTW->link_dist[ndr][next_node]] = NTW->Bcell_num    [Aux_Pos][wave][buffer];
         NTW->num_of_cells[to_link][wave][NTW->link_dist[ndr][next_node]] = NTW->Bnum_of_cells[Aux_Pos][wave][buffer];
         NTW->priority    [to_link][wave][NTW->link_dist[ndr][next_node]] = NTW->Bpriority    [Aux_Pos][wave][buffer];
         //NTW->offset_time [to_link][wave][NTW->link_dist[ndr][next_node]] = NTW->Boffset_time [Aux_Pos][wave][buffer];

         NTW->num_of_flow [to_link][wave][NTW->link_dist[ndr][next_node]] = NTW->Bnum_of_flow [Aux_Pos][wave][buffer];
      }
   }

   NTW->Bfromlink    [Aux_Pos][wave][buffer] = 0;
   NTW->Baddress     [Aux_Pos][wave][buffer] = 0;
   NTW->Bsource      [Aux_Pos][wave][buffer] = VirtualNode;
   NTW->Bdestination [Aux_Pos][wave][buffer] = VirtualNode;
   //NTW->Bnum_slots   [Aux_Pos][wave][buffer] = 0;
   //NTW->Bnum_hops    [Aux_Pos][wave][buffer] = 0;
   NTW->Bcell_num    [Aux_Pos][wave][buffer] = 0;
   NTW->Bnum_of_cells[Aux_Pos][wave][buffer] = 0;
   NTW->Bpriority    [Aux_Pos][wave][buffer] = 0;
   //NTW->Boffset_time [Aux_Pos][wave][buffer] = 0;

   NTW->Bnum_of_flow [Aux_Pos][wave][buffer] = 0;
}


/* ============================================================= */
/*                   ABSORPTION                                  */
/* ============================================================= */
/* ------------------------------------------------------------- */
void Absorption (int ndr,int link,int Olink,int wave,int dest,int LoB, struct NETWORK *NTW, struct SPARAMETERS *SM_Parameters)
{

   int Source, SizeFlow;

   unsigned short int NumPacket;
   unsigned long int NumFlow = 0;

   if (LoB == 1)
   {
	  // Absorption is from link
	  if( (NTW->priority[NTW->num_input_link[ndr][link]][wave][1] == 1) &&
		  (NTW->Phase >=2)
	    )
	  {
          if(NTW->dw == 666)
             NTW->AbsPK[NTW->Phase][SM_Parameters->num_waves - 1][NTW->ThisLoad]++;
          else
    		 NTW->AbsPK[NTW->Phase][NTW->dw - 1][NTW->ThisLoad]++;
	  }
	  else
	  {
		  //NTW->numAbs[ndr][link][wave]++;
	  }
	  //NTW->hop_absorption[NTW->num_hops[NTW->num_input_link[ndr][link]][wave][1]]++;
      NTW->numAbs[ndr][link][wave]++;
      //NTW->numHops[ndr][link][wave]  += NTW->num_hops[NTW->num_input_link[ndr][link]][wave][1];
      //NTW->numSlots[ndr][link][wave] += NTW->num_slots[NTW->num_input_link[ndr][link]][wave][1];

      if(NTW->num_of_flow[NTW->num_input_link[ndr][link]][wave][1] >= 1)
      {
    	  Source    = NTW->source       [NTW->num_input_link[ndr][link]][wave][1];
    	  NumFlow   = NTW->num_of_flow  [NTW->num_input_link[ndr][link]][wave][1];
          SizeFlow  = NTW->num_of_cells [NTW->num_input_link[ndr][link]][wave][1];
          NumPacket = NTW->cell_num     [NTW->num_input_link[ndr][link]][wave][1];
    	  NTW->num_of_flow  [NTW->num_input_link[ndr][link]][wave][1] = 0;
          NTW->num_of_cells [NTW->num_input_link[ndr][link]][wave][1] = 0;
          NTW->cell_num     [NTW->num_input_link[ndr][link]][wave][1] = 0;

    	  //if(NTW->cell_num[NTW->num_input_link[ndr][link]][wave][1] == 1)
    	  //   NTW->NumFlowR[ndr]++;
      }

	  NTW->source      [NTW->num_input_link[ndr][link]][wave][1] = VirtualNode;
	  NTW->destination [NTW->num_input_link[ndr][link]][wave][1] = VirtualNode;
      //NTW->num_hops    [NTW->num_input_link[ndr][link]][wave][1] = 0;
      //NTW->num_slots   [NTW->num_input_link[ndr][link]][wave][1] = 0;
      NTW->priority    [NTW->num_input_link[ndr][link]][wave][1] = 0;
   }
   else
   {
      // Absorption is from buffer
      if( (NTW->Bpriority[Olink+(ndr)*max_links_per_node][wave] == 1) &&
          (NTW->Phase >=2)
        )
      {
          if(NTW->dw == 666)
             NTW->AbsPK[NTW->Phase][SM_Parameters->num_waves - 1][NTW->ThisLoad]++;
          else
    		 NTW->AbsPK[NTW->Phase][NTW->dw - 1][NTW->ThisLoad]++;
      }
      else
      {
    	  //NTW->numAbs[ndr][link][wave]++;
      }
	  //NTW->hop_absorption[NTW->Bnum_hops[Olink+(ndr)*max_links_per_node][wave][1]]++;
      NTW->numAbs[ndr][link][wave]++;
      //NTW->numHops[ndr][link][wave]  += NTW->Bnum_hops[Olink+(ndr)*max_links_per_node][wave][1];
      //NTW->numSlots[ndr][link][wave] += NTW->Bnum_slots[Olink+(ndr)*max_links_per_node][wave][1];

      if(NTW->Bnum_of_flow[Olink+(ndr)*max_links_per_node][wave][1] >= 1)
      {

    	  Source    = NTW->Bsource       [Olink+(ndr)*max_links_per_node][wave][1];
    	  NumFlow   = NTW->Bnum_of_flow  [Olink+(ndr)*max_links_per_node][wave][1];
          SizeFlow  = NTW->Bnum_of_cells [Olink+(ndr)*max_links_per_node][wave][1];
          NumPacket = NTW->Bcell_num     [Olink+(ndr)*max_links_per_node][wave][1];

    	  NTW->Bnum_of_flow  [Olink+(ndr)*max_links_per_node][wave][1] = 0;
          NTW->Bnum_of_cells [Olink+(ndr)*max_links_per_node][wave][1] = 0;
          NTW->Bcell_num     [Olink+(ndr)*max_links_per_node][wave][1] = 0;

    	  //if(NTW->Bcell_num[Olink+(ndr)*max_links_per_node][wave][1] == 1)
    	     //NTW->NumFlowR[ndr]++;
      }

	  NTW->Bsource      [Olink+(ndr)*max_links_per_node][wave][1] = VirtualNode;
      NTW->Bdestination [Olink+(ndr)*max_links_per_node][wave][1] = VirtualNode;
      //NTW->Bnum_hops    [Olink+(ndr)*max_links_per_node][wave][1] = 0;
      //NTW->Bnum_slots   [Olink+(ndr)*max_links_per_node][wave][1] = 0;
      NTW->Bpriority    [Olink+(ndr)*max_links_per_node][wave][1] = 0;

   }

   // Let's compute Reorder Density using Source, NumFlow, SizeFlow and NumPacket
   if( (NumFlow != 0) &&
       ( NumFlow < NTW->MyFlowID)
     )
   {
	   // GO for it!!
	   computeRD(ndr,Source,NumFlow,SizeFlow,NumPacket,NTW,SM_Parameters,LoB);
   }

}


/* ==============================================================*/
/*             Flow Through PACKET ALLOCATION                    */
/*===============================================================*/
/* This subroutine allocate Packets in the output links          */
void   FTallocation(int ndr, int link, int Olink, int Naddress, int wave, int wave_t, struct NETWORK *NTW, struct SPARAMETERS *SM_Parameters)
{
   int dest,to_link,from_link,next_node,blocked,ThisReward, Alternatives;

   dest = NTW->destination[NTW->num_input_link[ndr][link]][wave][1];

   if ((NTW->tempo == 1000) || (NTW->tempo == 5000) || (NTW->tempo == 10000))
      fprintf (fscreen,"FTall,ndr=%d, dest=%d, Naddress= %d\n ", ndr, dest, Naddress) ;

   from_link = NTW->num_input_link[ndr][link];

   if ( (ndr == dest) || (Olink == 1) )
   {
      Absorption(ndr,link,Olink,wave,dest,1, NTW, SM_Parameters);
   }
   else
   {
      next_node = NTW->forw_table[ndr][dest][Naddress];

      if (next_node != NTW->connected_to[ndr][Olink])
      {
         printf ("FTallocation \n") ;
         printf ("num_of_link[ndr][next_node][1] != num_output_link[ndr][Olink] -- dest %d\n",dest) ;
         printf ("num_of_link=%d,num_output_link=%d,ndr=%d,dest=%d,Naddress=%d,next_node1=%d,connected_to=%d,Olink=%d,Forw_Output=%d\n", NTW->num_of_link[ndr][next_node][1],NTW->num_output_link[ndr][Olink],ndr,dest,Naddress,next_node,NTW->connected_to[ndr][Olink],Olink,NTW->forw_output[ndr][dest][Naddress]);
         printf ("\n ndr %d - next_node %d - Olink %d",ndr, next_node,Olink);
         printf ("\n ndr %d - dest %d - Naddress %d",ndr, dest,Naddress);
         printf ("\n EXIT");

         exit (-1);
      }

      /* to_link=num_of_link[ndr][next_node][1]; */

      to_link = NTW->num_output_link[ndr][Olink];

      if ( (NTW->tempo == 1000) || (NTW->tempo == 5000) || (NTW->tempo == 10000) )
         fprintf (fscreen,"from_link=%d,to_link=%d,link_dist=%d \n", from_link, to_link, NTW->link_dist[ndr][next_node]) ;

      if(ndr == NTW->nbr_of_nodes - 1)
         NTW->num_out_packets++;

      // If links are blocked the process will be restricted.
      if(NTW->Phase >= 2)
      {
         // Is this fiber empty ??
    	 if( (NTW->source     [to_link][wave_t][NTW->link_dist[ndr][next_node]] != VirtualNode) &&
    	     (NTW->destination[to_link][wave_t][NTW->link_dist[ndr][next_node]] != VirtualNode)
    	   )
    	 {
    	    // Loosing packets
    	    if( (NTW->priority[to_link][wave_t][NTW->link_dist[ndr][next_node]] == 1) &&
                (NTW->Phase >=2)
              )
    	    {
                if(NTW->dw == 666)
                   NTW->LostPK[NTW->Phase][SM_Parameters->num_waves - 1][NTW->ThisLoad]++;
                else
          		   NTW->LostPK[NTW->Phase][NTW->dw - 1][NTW->ThisLoad]++;
    	        //NTW->LostPK[NTW->Phase][NTW->dw - 2][NTW->ThisLoad]++;
    	    }
    	    else
    	    {
    	       NTW->numPPL[ndr]++; /* or loss                */
    	       NTW->numPPLFiber[ndr][Olink]++;
    	    }
    	    //if( (NTW->Phase == 5) && (NTW->priority    [from_link][wave][1] == 0))
    	    if(NTW->Phase == 5)
    	       Learning(ndr,Naddress,wave,NTW,SM_Parameters,dest,-100);
    	    more_delays(NTW,SM_Parameters,ndr,dest,Olink);
    	 }

         NTW->source       [to_link][wave_t][NTW->link_dist[ndr][next_node]] = NTW->source      [from_link][wave][1];
         NTW->destination  [to_link][wave_t][NTW->link_dist[ndr][next_node]] = NTW->destination [from_link][wave][1];
         NTW->priority     [to_link][wave_t][NTW->link_dist[ndr][next_node]] = NTW->priority    [from_link][wave][1];

         //NTW->num_slots    [to_link][wave_t][NTW->link_dist[ndr][next_node]] = NTW->num_slots   [from_link][wave][1];
         //NTW->num_hops     [to_link][wave_t][NTW->link_dist[ndr][next_node]] = NTW->num_hops    [from_link][wave][1];
         NTW->cell_num     [to_link][wave_t][NTW->link_dist[ndr][next_node]] = NTW->cell_num    [from_link][wave][1];
         NTW->num_of_cells [to_link][wave_t][NTW->link_dist[ndr][next_node]] = NTW->num_of_cells[from_link][wave][1];
         //NTW->offset_time  [to_link][wave_t][NTW->link_dist[ndr][next_node]] = NTW->offset_time [from_link][wave][1];

         NTW->num_of_flow  [to_link][wave_t][NTW->link_dist[ndr][next_node]] = NTW->num_of_flow [from_link][wave][1];

         // Let's find out how many alternatives are.
         // Myat is equal to the chosen action at RandomseriesFW
         // Myat within range [0,n-1] where n is the number of alternatives to send the packet

         //if( (NTW->Phase == 5) && (NTW->priority[from_link][wave][1] == 0))
         if(NTW->Phase == 5)
         {
            Alternatives = NTW->num_outputs[ndr][dest];
            // Default value
            ThisReward = -1;
            /*if( (Naddress == 1) || (Naddress == -2) || (Naddress == -3))
            {
               ThisReward = 1;
            }
            else
            {
               if( (Naddress == 2) || (Naddress == 3) || (Naddress == 4) )
               {
                  ThisReward = 0;
               }
               else
               {
                  if(Naddress <= (Alternatives / 3))
                     ThisReward = 0;
               }
            }*/

            switch(Naddress)
            {
               case 1:
            	   ThisReward = 1;
            	   break;
               case 2:
            	   ThisReward = 0;
            	   break;
               case 3:
            	   ThisReward = 0;
            	   break;
               case 4:
            	   ThisReward = 0;
            	   break;
               case 5:
            	   ThisReward = -1;
            	   break;
               default:
            	   ThisReward = -1;
            	   break;
            }

            Learning(ndr,Naddress,wave,NTW,SM_Parameters,dest, ThisReward);
         }
       	 if(NTW->tempo >= 100 && NTW->tempo <= 299 && NTW->priority[to_link][wave_t][NTW->link_dist[ndr][next_node]] == 1)
       	 {
       		  printf("\n \t \t \t IT_WENT_THROUGH src %d dest %d ndr %d link %d b %d - Phase %d - %d",NTW->source[to_link][wave_t][NTW->link_dist[ndr][next_node]],dest,ndr,link,Olink,NTW->Phase,NTW->tempo);
       	      printf("\n \t source : %d",NTW->source     [to_link][wave_t][NTW->link_dist[ndr][next_node]]);
       	      printf("\n \t destin : %d",NTW->destination[to_link][wave_t][NTW->link_dist[ndr][next_node]]);
       	      printf("\n \t priori : %d",NTW->priority   [to_link][wave_t][NTW->link_dist[ndr][next_node]]);
       	      printf("\n \t to_link : %d - wave_t : %d - pos : %d ",to_link,wave_t,NTW->link_dist[ndr][next_node]);
       	 }
      }// End if Phase >= 2
      else
      {
         NTW->source       [to_link][wave_t][NTW->link_dist[ndr][next_node]] = NTW->source      [from_link][wave][1];
         NTW->destination  [to_link][wave_t][NTW->link_dist[ndr][next_node]] = NTW->destination [from_link][wave][1];
         //NTW->num_slots    [to_link][wave_t][NTW->link_dist[ndr][next_node]] = NTW->num_slots   [from_link][wave][1];
         //NTW->num_hops     [to_link][wave_t][NTW->link_dist[ndr][next_node]] = NTW->num_hops    [from_link][wave][1];
         NTW->cell_num     [to_link][wave_t][NTW->link_dist[ndr][next_node]] = NTW->cell_num    [from_link][wave][1];
         NTW->num_of_cells [to_link][wave_t][NTW->link_dist[ndr][next_node]] = NTW->num_of_cells[from_link][wave][1];
         NTW->priority     [to_link][wave_t][NTW->link_dist[ndr][next_node]] = NTW->priority    [from_link][wave][1];
         //NTW->offset_time  [to_link][wave_t][NTW->link_dist[ndr][next_node]] = NTW->offset_time [from_link][wave][1];

         NTW->num_of_flow  [to_link][wave_t][NTW->link_dist[ndr][next_node]] = NTW->num_of_flow [from_link][wave][1];

         // Reward
         //Learning(ndr,link,wave,NTW,SM_Parameters,dest,0);
      }
   }

   NTW->source[from_link][wave][1]       = VirtualNode;
   NTW->destination[from_link][wave][1]  = VirtualNode;
   //NTW->num_slots[from_link][wave][1]    = 0;
   //NTW->num_hops[from_link][wave][1]     = 0;
   NTW->cell_num[from_link][wave][1]     = 0;
   NTW->num_of_cells[from_link][wave][1] = 0;
   NTW->priority[from_link][wave][1]     = 0;
   //NTW->offset_time[from_link][wave][1]  = 0;

   NTW->num_of_flow [from_link][wave][1] = 0;
}

/* ==============================================================*/
/*               BUFFER PACKET ALLOCATION                        */
/*===============================================================*/
/* This subroutine allocate Packets in the output links          */

void BFallocation(int ndr, int link, int Olink, int wave, int wave_t,int buffer, int Naddress, struct NETWORK *NTW, struct SPARAMETERS *SM_Parameters)
{
   int  from_link, dest, aux_buffer;
   int Next_node;

   from_link = NTW->num_input_link[ndr][link];
   dest      = NTW->destination[NTW->num_input_link[ndr][link]][wave][1];
   Next_node = NTW->forw_table[ndr][dest][Naddress];

   if(NTW->connected_to[ndr][Olink] != Next_node)
   {
	   //
	   printf("\n WE_HAVE_A_PROBLEM!!!");
   }

   if(ndr == NTW->nbr_of_nodes - 1)
      NTW->num_out_packets++;

   aux_buffer = Olink + (ndr) *max_links_per_node;

   // Is this buffer empty ??
   // if (NTW->OutBuffer[Olink+(ndr) * max_links_per_node][wave][1] == 1)
   if( (NTW->Bsource     [aux_buffer][wave_t][buffer] != VirtualNode) &&
       (NTW->Bdestination[aux_buffer][wave_t][buffer] != VirtualNode)
     )
   {
  	 // Loosing packets
  	 if( (NTW->Bpriority[aux_buffer][wave_t][buffer] == 1) &&
         (NTW->Phase >=2)
       )
  	 {
         if(NTW->dw == 666)
            NTW->LostPK[NTW->Phase][SM_Parameters->num_waves - 1][NTW->ThisLoad]++;
         else
   		    NTW->LostPK[NTW->Phase][NTW->dw - 1][NTW->ThisLoad]++;
  		 //NTW->LostPK[NTW->Phase][NTW->dw - 2][NTW->ThisLoad]++;
  	 }
  	 else
  	 {
	    NTW->numPPL[ndr]++; /* or loss                */
  	    NTW->numPPLFiber[ndr][link]++;
  	 }

  	 //if( (NTW->Phase == 5) && (NTW->priority     [from_link][wave][1] == 0))
  	if(NTW->Phase == 5)
  	    Learning(ndr,Naddress,wave,NTW,SM_Parameters,dest, -100);
  	 more_delays(NTW,SM_Parameters,ndr,dest,Olink);
   }

   //if( (NTW->Phase == 5) && (NTW->priority     [from_link][wave][1] == 0))
   if(NTW->Phase == 5)
      // Rewards for buffering = -5
      Learning(ndr,Naddress,wave,NTW,SM_Parameters,dest, 0);


   NTW->Bfromlink     [aux_buffer][wave_t][buffer] = link;
   NTW->OutBuffer     [aux_buffer][wave_t][buffer] = 1;
   NTW->Baddress      [aux_buffer][wave_t][buffer] = Naddress;
   NTW->Bsource       [aux_buffer][wave_t][buffer] = NTW->source       [from_link][wave][1];
   NTW->Bdestination  [aux_buffer][wave_t][buffer] = NTW->destination  [from_link][wave][1];
   //NTW->Bnum_slots    [aux_buffer][wave_t][buffer] = NTW->num_slots    [from_link][wave][1];
   //NTW->Bnum_hops     [aux_buffer][wave_t][buffer] = NTW->num_hops     [from_link][wave][1];
   NTW->Bcell_num     [aux_buffer][wave_t][buffer] = NTW->cell_num     [from_link][wave][1];
   NTW->Bnum_of_cells [aux_buffer][wave_t][buffer] = NTW->num_of_cells [from_link][wave][1];
   NTW->Bpriority     [aux_buffer][wave_t][buffer] = NTW->priority     [from_link][wave][1];
   //NTW->Boffset_time  [aux_buffer][wave_t][buffer] = NTW->offset_time  [from_link][wave][1];

   NTW->Bnum_of_flow  [aux_buffer][wave_t][buffer] = NTW->num_of_flow  [from_link][wave][1];

   NTW->source       [from_link][wave][1] = VirtualNode;
   NTW->destination  [from_link][wave][1] = VirtualNode;
   //NTW->num_slots    [from_link][wave][1] = 0;
   //NTW->num_hops     [from_link][wave][1] = 0;
   NTW->cell_num     [from_link][wave][1] = 0;
   NTW->num_of_cells [from_link][wave][1] = 0;
   NTW->priority     [from_link][wave][1] = 0;
   //NTW->offset_time  [from_link][wave][1] = 0;

   NTW->num_of_flow  [from_link][wave][1] = 0;

}


/*===============================================================*/
/*                    MORE-LINKS  FUNCTION                       */
/*===============================================================*/
void more_links (int ndr, int destNET, int Olink, struct NETWORK *NTW, struct SPARAMETERS *SM_Parameters)
{
   // read line until keyword EDGES
   // read all nodes until blank line or end of file

   int n2,dest,Naddress;
   int AAdd, Fndr;
   int Max_Fibers;
   Fndr = ndr;

   int Next_Node = NTW->connected_to[ndr][Olink];

   n2 = NTW->forw_table[ndr][destNET][1];

   //if(NTW->num_of_fibers[ndr][NTW->connected_to[ndr][Olink]] <= 2)

   if(NTW->tempoT > (SM_Parameters->Initial_Time + SM_Parameters->Operation_Time))
   {
	   Max_Fibers = 15;
   }
   else
   {
	   Max_Fibers = 5;
   }

   if( (NTW->num_of_fibers[ndr][n2] < Max_Fibers) &&
       (Olink > 1) &&
       ( (NTW->tempoT - NTW->TimeGeneration[ndr][n2]) > GenerationTH)
     )
   {

	   NTW->TimeGeneration[ndr][n2] = NTW->tempoT;

   /* ******************* LINKS-LINKS  ********************** */
   n2 = NTW->forw_table[ndr][destNET][1];
                                  /* Link #1 is for injection */
   NTW->output_links[ndr]++;      /* output-links from a node */

   NTW->input_links[n2]++;        /* input-links to a node    */
   /* [node][1-x links]        */
   NTW->num_input_link[n2][NTW->input_links[n2]] = NTW->n_links;
   NTW->connected_from[n2][NTW->input_links[n2]] = ndr;

   NTW->num_of_fibers[ndr][n2]++;
   NTW->num_of_link[ndr][n2][NTW->num_of_fibers[ndr][n2]] = NTW->n_links;

   NTW->num_output_link[ndr][NTW->output_links[ndr]]      = NTW->n_links;
   NTW->connected_to[ndr][NTW->output_links[ndr]]         = n2;

   NTW->link_size[NTW->n_links] = NTW->link_dist[ndr][n2];
   NTW->n_links++;
   NTW->n_fibers++;

   printf ("\nmore_links (ndr=%d,destNET=%d,Olink=%d) \n",ndr,destNET,Olink);
   printf ("tempo=%d, ndr= %d, n2=%d destNET=%d, forw_table=%d \n",(int)NTW->tempo,ndr,n2,destNET,NTW->forw_table[ndr][destNET][1]);


   for (dest = 0; dest < NTW->nbr_of_nodes; dest++)
      if ( (ndr != dest) && (NTW->forw_output[ndr][dest][1] == Olink) )
         for (Naddress = 1; Naddress <= NTW->num_outputs[ndr][dest]; Naddress++)
         {
            NTW->bak_forw_output[ndr][dest][Naddress] = NTW->forw_output[ndr][dest][Naddress];
            NTW->bak_forw_table[ndr][dest][Naddress]  = NTW->forw_table[ndr][dest][Naddress];
         }

   for (dest = 0; dest < NTW->nbr_of_nodes; dest++)
      if ((ndr != dest) && (NTW->forw_output[ndr][dest][1] == Olink))
      {
         NTW->forw_output[ndr][dest][2] = NTW->output_links[ndr];
         NTW->forw_table[ndr][dest][2]  = NTW->forw_table[ndr][dest][1];
      }

   for (dest = 0; dest < NTW->nbr_of_nodes; dest++)
      if ((ndr != dest) && (NTW->forw_output[ndr][dest][1] == Olink))
         for (Naddress = 2; Naddress <= NTW->num_outputs[ndr][dest]; Naddress++)
         {
            NTW->forw_output[ndr][dest][Naddress+1] = NTW->bak_forw_output[ndr][dest][Naddress];
            NTW->forw_table[ndr][dest][Naddress+1]  = NTW->bak_forw_table[ndr][dest][Naddress];
         }

   for (dest = 0; dest < NTW->nbr_of_nodes; dest++)
      if ( (ndr != dest) && (NTW->forw_output[ndr][dest][1] == Olink) )
      {
         NTW->num_outputs[ndr][dest]++;
         // Policy to 0.1
         //NTW->MyPolicy[ndr][dest][NTW->num_outputs[ndr][dest]] = 0.1;
      }

   printf ("tempo=%d,n_links=%d, output_links[%d]= %d, num_of_fibers[%d][%d]=%d,Update_Transmission(ndr); \n",(int)NTW->tempo,NTW->n_links,ndr,NTW->output_links[ndr],ndr,n2,NTW->num_of_fibers[ndr][n2]);

   if(NTW->n_links == 500)
      printf("\n We reached the limit");

   if (ndr != n2)
      Update_Transmission(ndr,NTW, SM_Parameters);
   printf ("tempo=%d, Regrese de Update_Transmission(ndr); \n",(int)NTW->tempo);

   /* More_forwarding(); */

   /*************  Printing forwarding table  ********************/

   fprintf (fscreen,"tempo=%d, ndr, dest, num_outputs, forw_output \n",(int)NTW->tempo);
   for (ndr=0; ndr< NTW->nbr_of_nodes; ndr++)
      for (dest=0; dest< NTW->nbr_of_nodes; dest++)
         if  (dest != ndr)
         {
            fprintf (fscreen,"%d %d %d |", ndr, dest, NTW->num_outputs[ndr][dest]);
            for (Naddress=1; Naddress<= NTW->num_outputs[ndr][dest]; Naddress++)
               fprintf (fscreen,"%d  ", NTW->forw_table[ndr][dest][Naddress]);
            fprintf (fscreen,"  |");
            for (Naddress=1; Naddress<= NTW->num_outputs[ndr][dest]; Naddress++)
               fprintf (fscreen,"%d  ", NTW->forw_output[ndr][dest][Naddress]);
            fprintf (fscreen," \n ");
         }

   printf ("tempo=%d, Termine de escribir las nuevas forw_outputs en Screen file \n",(int)NTW->tempo);
   /*************** Printing forwarding table  *******************/

                     //if (NTW->num_of_fibers[ndr][NTW->forw_table[ndr][destNET][1]] >= 2)
                     //{
                        // To split the number of DL in each new fiber. SM_Parameters->AllowedDL
                        // Number of fibers to the same node NTW->num_of_fibers[ndr][NTW->connected_to[ndr][Olink]]
                        // For each output fiber
                        for (AAdd = 1; AAdd <= NTW->output_links[Fndr]; AAdd++)
                        {
                           // If this fiber connects to Olink, i.e. extra fiber
                           if(NTW->connected_to[Fndr][AAdd] == NTW->connected_to[Fndr][Olink])
                           {
                              // Restart the number of DL
                              NTW->NDL[Fndr][AAdd] = SM_Parameters->num_predefined_DL;
                           }
                        }
                     //}

   }
}  /* ****************** LINKS-LINKS  ********************** */

/* =============================================================*/
/*               PACKET DROPING                                 */
/*==============================================================*/
/* This subroutine allocate Packets in the output links         */
void   Discard (int ndr, int link,int wave, struct NETWORK *NTW, struct SPARAMETERS *SM_Parameters, int dest)
{
   int from_link;
   from_link = NTW->num_input_link[ndr][link];
   //NTW->hop_distribution[NTW->num_hops[from_link][wave][1]]++;

   NTW->source[from_link][wave][1]       = VirtualNode;
   NTW->destination[from_link][wave][1]  = VirtualNode;
   //NTW->num_slots[from_link][wave][1]    = 0;
   //NTW->num_hops[from_link][wave][1]     = 0;
   NTW->cell_num[from_link][wave][1]     = 0;
   NTW->num_of_cells[from_link][wave][1] = 0;
   NTW->priority[from_link][wave][1]     = 0;
   //NTW->offset_time[from_link][wave][1]  = 0;
   NTW->num_of_flow [from_link][wave][1] = 0;

   //if( (NTW->Phase >= 5) && (NTW->priority[from_link][wave][1] == 0))
   if(NTW->Phase >= 5)
	   Learning(ndr,link,wave,NTW,SM_Parameters,dest,0);
}

void Learning(int ndr, int link,int wave, struct NETWORK *NTW, struct SPARAMETERS *SM_Parameters, int dest, int Reward)
{
	   // Punishment
	   // State  = ndr,dest
	   // Action = fiber
	   // Alpha  = 0.1
		// Reward = -1
		// Gama   = 0.9
		// Determine who send the packet,
		// Determine through which fiber

		// Max = sum(MyPolicy[ndr][dest][...]
	   // Current = NTW->MyPolicy[ndr][dest][f]

   int   Indexes[max_num_node + 1], MyFiber, Previous_Node, Num_Fiber, i, Alternatives;
   float Current, MaxR, DiffeRL, DeltaRL;

   Previous_Node = NTW->connected_from[ndr][link];

   // Reward
   if( NTW->Phase == 5 )
   {
	   if(Reward == -200)
	   {
		   // When dropping we punish the previous node
	      if (Previous_Node != ndr)
	      {
             Num_Fiber     = NTW->num_input_link[ndr][link];
             MyFiber  = 0;
	         for(i = 2; i <= NTW->output_links[Previous_Node]; i++)
             {
	            if(Num_Fiber == NTW->num_output_link[Previous_Node][i])
	            {
	               MyFiber = i;
	               break;
	            }
             }

	         if(MyFiber == 0)
             {
	            printf(" \n ERROR ERROR ERROR ERROR ERROR ERROR ");
	         }
             Alternatives = NTW->num_outputs[Previous_Node][dest];

	         Current = NTW->MyPolicy[Previous_Node][dest][MyFiber];
             MaxR    = 0.0;

             for(i = 1; i <= Alternatives; i++)
             {
	            Indexes[i] = NTW->forw_output[Previous_Node][dest][i];
	         }
             MaxR    = NTW->MyPolicy[Previous_Node][dest][Indexes[1]];

	         for (i = 1; i <= Alternatives; i++)
	            if(NTW->MyPolicy[Previous_Node][dest][Indexes[i]] > MaxR)
	               MaxR = NTW->MyPolicy[Previous_Node][dest][Indexes[i]];

	         DiffeRL = GammaRL * (MaxR - Current);
	         DeltaRL = AlphaRL * ((float)Reward +  DiffeRL);
             NTW->MyPolicy[Previous_Node][dest][MyFiber] = Current + DeltaRL;
	      }// End if Previous != ndr
	   }// End if reward == -10

      // ========================= This node
      Alternatives = NTW->num_outputs[ndr][dest];
      Current      = NTW->MyPolicy[ndr][dest][link];
      MaxR         = 0.0;

      for(i = 1; i <= Alternatives; i++)
      {
	     Indexes[i] = NTW->forw_output[ndr][dest][i];
	  }
      MaxR    = NTW->MyPolicy[ndr][dest][Indexes[1]];

	  for (i = 1; i <= Alternatives; i++)
	     if(NTW->MyPolicy[ndr][dest][Indexes[i]] > MaxR)
	        MaxR = NTW->MyPolicy[ndr][dest][Indexes[i]];

	  DiffeRL = GammaRL * (MaxR - Current);
	  DeltaRL = AlphaRL * ((float)Reward +  DiffeRL);
      NTW->MyPolicy[ndr][dest][link] = Current + DeltaRL;


      if(Reward >= 0)
      {
    	  NTW->Num_Pos++;
      }
      else
      {
    	  NTW->Num_Neg++;
      }
   }
}

/* ============================================================= */
/*         Update_Transmission                                   */
/* ============================================================= */
void Update_Transmission(int ndr, struct NETWORK *NTW, struct SPARAMETERS *SM_Parameters)
{

   int   dest, wave, Olink, PAdd, linkin, Naddress ;
   float a,per;

   /*       Link assigment             */
   for(linkin = 1; linkin <= NTW->input_links[ndr]; linkin++)
      /*     wavelength asigment    */
      for(wave = 1 ; wave <= SM_Parameters->num_waves; wave++ )
      {
         /* packet destination */
         dest = NTW->destination[NTW->num_input_link[ndr][linkin]][wave][1];

         if(dest != VirtualNode)        /* Is there one packet?  */
         {
            NTW->NA[linkin][wave] = 1;  /* always assign one output */
            if(dest == ndr)         /* Is this router the destination */
               NTW->NA[linkin][wave] = 1;
            else
            {
               /* assign the number of possible outputs */
               NTW->NA[linkin][wave] = NTW->num_outputs[ndr][dest];
            }
         }
         else
         {
            NTW->NA[linkin][wave]            = 0; /*NO dumi packets around*/
         }
      }
}     /* END Update_TRANSMISSION */

/* ============================================================= */
/*               Dynamic-memory Arrays                           */
/* ============================================================= */
/* -----------------    Array   -------------------------------- */
int *build_array(int size)
{
   int *new_array;

   if ((new_array=(int *)calloc(size, sizeof(int))) != NULL)
      return(new_array);
   else
   {
      printf ("**** Problems: It was not possible to create dynamic array \n") ;
      exit (-1) ;
   }
}

/* ------------------    Matrix   ------------------------------ */
int **build_matrix(int m, int n)
{
   int **new_matrix;
   int i;

   if ((new_matrix=(int **)calloc(m, sizeof(int *))) != NULL)
   {
      for (i=0; i < m; i++)
         new_matrix[i] = build_array(n);
      return(new_matrix);
   }
   else
   {
      printf("**** Problems: It was not possible to create dynamic matrix \n") ;
      exit(-1);
   }
}


/* ==============================================================*/
/*                     RandomSeries                              */
/*===============================================================*/
/* This subroutine generate uniform random series                */
void RandomSeries (int ndr, struct NETWORK *NTW, struct SPARAMETERS *SM_Parameters)
{
   int ch, ca;

   for( ch = 1 ; ch <= NTW->input_links[ndr]; ch++ )
      NTW->SR[ch] = Rnd(1,NTW->input_links[ndr]);

   for( ch = 2 ; ch <= NTW->input_links[ndr]; ch++ )
   {
      ca = 1;
      while(ca != ch)
      if(NTW->SR[ca] == NTW->SR[ch])
      {
         while (NTW->SR[ch] == NTW->SR[ca])
            NTW->SR[ch] = Rnd(1,NTW->input_links[ndr]);
         ca = 1;
      }
      else
         ca = ca+1;
   }

   for( ch = 1 ; ch <= SM_Parameters->num_waves; ch++ )
      NTW->ST[ch] = Rnd(1,SM_Parameters->num_waves);

   for( ch = 2 ; ch <= SM_Parameters->num_waves; ch++ )
   {
      ca = 1;
      while(ca != ch)
         if (NTW->ST[ca]==NTW->ST[ch])
         {
            while (NTW->ST[ch] == NTW->ST[ca])
               NTW->ST[ch] = Rnd(1,SM_Parameters->num_waves);
            ca = 1;
         }
         else
            ca = ca + 1;
   }
}                          /* END RANDOMSERIES */

void Update_table(struct NETWORK *NTW, int ndr, struct SPARAMETERS *SM_Parameters)
{
   int aux_node, outputs, dest, Naddress, next_node;;

   for (aux_node = 0; aux_node < NTW->nbr_of_nodes; aux_node++)
      if(ndr != aux_node)
      {
         for (outputs = 1; outputs <= NTW->num_outputs[ndr][aux_node]; outputs++)
         {
            NTW->forw_output[ndr][aux_node][outputs] = 0;
            NTW->forw_table [ndr][aux_node][outputs] = 0;
         }
         NTW->num_outputs[ndr][aux_node] = NTW->new_outputs[ndr][aux_node];
         for (outputs = 1; outputs <= NTW->new_outputs[ndr][aux_node]; outputs++)
         {
            NTW->forw_output[ndr][aux_node][outputs] = NTW->new_foutput[ndr][aux_node][outputs];
            NTW->forw_table [ndr][aux_node][outputs] = NTW->new_ftable [ndr][aux_node][outputs];
         }
      }

   for (dest=0; dest< NTW->nbr_of_nodes; dest++)
      if  (dest != ndr)
      {
         for (Naddress=1; Naddress<= NTW->num_outputs[ndr][dest]; Naddress++)
         {
            next_node = NTW->forw_table[ndr][dest][Naddress];
            NTW->num_output_link [ndr][NTW->forw_output[ndr][dest][Naddress]] =
            NTW->num_of_link     [ndr][next_node][NTW->num_of_fibers[ndr][next_node]];
            NTW->connected_to    [ndr][NTW->forw_output[ndr][dest][Naddress]] = next_node;
         }
      }

}

int search_link_failure(struct NETWORK * NTW, int fiber, struct SPARAMETERS *SM_Parameters, int wave, int dest, int ndr)
{
   int blocked, i, jj, INITIAL_FIBER, N2;
   blocked = 0;
   // Failure_Waves    INI   FIN     Fiber   W  Src  Dest
   //                  100 	 200 	 666 	 3 	 0 	 1
   //                  0     1       2       3   4   5
   //                  100 	 200 	 666 	 3 	 1 	 0
   /*if( (NTW->Phase == 2) || (NTW->Phase == 3) || (NTW->Phase == 5) || (NTW->Phase == 6))
   {
	  //INITIAL_FIBER = find_initial_fiber(NTW, fiber, SM_Parameters);
      // Which nodes are connected to this fiber?
	  N2 = find_second_node(ndr,fiber,NTW,SM_Parameters);
	  for(jj = 0; jj < SM_Parameters->Operation_Time; jj++)
      {
         if(NTW->Failure_Waves[jj][0] == 0)
            break;
         if( (NTW->tempo >= NTW->Failure_Waves[jj][0]) && // lower limit
             (NTW->tempo <= NTW->Failure_Waves[jj][1]) )  // upper limit
         {
            // We are at the "right" time - so there is an attack
        	//
            if( (NTW->Failure_Waves[jj][4] == ndr) &&
                (NTW->Failure_Waves[jj][5] == N2 ) &&
                (NTW->Failure_Waves[jj][3] >= wave)
              )
            {
               blocked = 1;
               break;
            }
         }
      }
   }
   */
   return blocked;
}

int search_wave_failure(struct NETWORK * NTW, int fiber, int wave, int Olink, struct SPARAMETERS *SM_Parameters, int dest, int ndr)
{
   // Check NTW->Outlink
   int Wave_Status, blocked, i, jj, resw, INITIAL_FIBER, N2;
   Wave_Status = 0;
   blocked = FREE;

   if( (NTW->Phase >= 2) &&
	   ( (fiber >= NTW->nbr_of_nodes) && (fiber < NTW->n_links))
	 )

   {
	  //INITIAL_FIBER = find_initial_fiber(NTW, fiber, SM_Parameters);
	  N2 = find_second_node(ndr,fiber,NTW,SM_Parameters);

      if(NTW->LCH[Olink][wave] >= 1)
         blocked = BLOCKED;

      if(NTW->Outlink[Olink] >= SM_Parameters->num_waves)
         blocked = ALL_BLOCKED;

      /*if(blocked == FREE)
      {
         Wave_Status = search_link_failure(NTW,fiber,SM_Parameters,wave,dest,ndr);
      }// End if(blocked == FREE)
      */
      // IF the wavelength is blocked
      NTW->FreeW = -1;
      if( (Wave_Status) || (blocked == BLOCKED) )
      {
         // Check NTW->LCH[Olink][wave]
         blocked = ALL_BLOCKED;
         // We assume that all wavelengths are blocked
         for ( i = 1 ; i <= SM_Parameters->num_waves ; i++ )
         {
            resw = FREE;
            resw = get_wave_status(NTW,fiber,i, SM_Parameters,dest,ndr);
            if( (NTW->LCH[Olink][i] == 0) && (wave != i) && (resw == FREE) )
            {
               blocked = ONE_FREE;
               NTW->FreeW = i; // This is the free wavelength
               break;
            }
         }
      }
   }
   return blocked;
}

int get_node_status(struct NETWORK * NTW, int node, struct SPARAMETERS *SM_Parameters)
{
	int jj;
	int node_status = 666;
	//NTW->dw = 0;

	if( (NTW->Phase == 2) || (NTW->Phase == 3) || (NTW->Phase == 5) || (NTW->Phase == 6))
	{
       for(jj = 0; jj < SM_Parameters->Operation_Time; jj++)
       {
           if(NTW->Failure_Waves[jj][0] == 0)
              break;
           if( (NTW->tempo >= NTW->Failure_Waves[jj][0]) && // lower limit
                (NTW->tempo <= NTW->Failure_Waves[jj][1]) )  // upper limit
           {
        	   // We are at the right time - so there is an active LP requests
        	   if( NTW->Failure_Waves[jj][4] == node )
        	   {
        		   // node is the source
        		   node_status = NTW->Failure_Waves[jj][5]; // Target
        		   //NTW->dw = NTW->Failure_Waves[jj][5];
        		   break;
        	   }
           }
       }
	}
	return node_status;
}

int get_wave_status(struct NETWORK * NTW, int fiber, int wave, struct SPARAMETERS *SM_Parameters, int dest, int ndr)
{
   int jj, wave_status,INITIAL_FIBER;
   wave_status = FREE;

   /*wave_status = search_link_failure(NTW,fiber,SM_Parameters,wave,dest,ndr);

   if( ( (NTW->Phase == 2) || (NTW->Phase == 3) || (NTW->Phase == 5) || (NTW->Phase == 6)) &&
		 ( (fiber >= NTW->nbr_of_nodes) && (fiber < NTW->n_links)))
   {
	   INITIAL_FIBER = find_initial_fiber(NTW, fiber, SM_Parameters);

      for(jj = 0; jj < SM_Parameters->Operation_Time; jj++)
      {
         if(NTW->Failure_Waves[jj][0] == 0)
            break;

         if( (NTW->tempo >= NTW->Failure_Waves[jj][0]) && // lower limit
              (NTW->tempo <= NTW->Failure_Waves[jj][1]) )  // upper limit
         {
            if(  (NTW->Failure_Waves[jj][7] != dest)          &&
                 (NTW->Failure_Waves[jj][4] == INITIAL_FIBER) &&
                 (NTW->Failure_Waves[jj][5] >= wave)
              )
            {
               wave_status = BLOCKED;

               break;
            }

         }
      }
   }*/

   return wave_status;
}

int find_initial_fiber(struct NETWORK * NTW, int fiber, struct SPARAMETERS *SM_Parameters)
{
	int ndr, Olink, Auxlink, F1, F2, N1, N2;
	int ThisFiber = 0;
	// j <= NTW->output_links[n1]
	// real_fiber = NTW->num_output_link[n1][j];
	// NTW->num_initial_links is lower than

	if(NTW->Link_Fibers[fiber] == 0)
	{
		for(ndr = 0; ndr < NTW->nbr_of_nodes && ThisFiber == 0; ndr++)
		{
			for(Olink = 2; Olink <= NTW->output_links[ndr] && ThisFiber == 0; Olink++)
			{
				F1 = NTW->num_output_link[ndr][Olink];
				if(F1 == fiber)
				{
					// We found it
					if(F1 < NTW->num_initial_links)
					{
						ThisFiber = F1;
						NTW->Link_Fibers[fiber] = ThisFiber;
						break;
					}
					else
					{
						// This time explore until Olink.
						// We must find a fiber F2 connecting to the same node as F1
						// F2 must be lower or equal than NTW->num_initial_links
						N1  = NTW->connected_to[ndr][Olink];
						for(Auxlink = 2; Auxlink < Olink; Auxlink++)
						{
							F2 = NTW->num_output_link[ndr][Auxlink];
							N2 = NTW->connected_to[ndr][Auxlink];

							if( (N1 == N2) && (F1 != F2) && (F2 < NTW->num_initial_links))
							{
								// got it
								ThisFiber = F2;
								NTW->Link_Fibers[fiber] = ThisFiber;
								break;
							}
						}// End FOR
					}// End ELSE
				}//End IF
			}// End FOR
		}// End FOR
	}// End IF
	else
	{
		// Get it from NTW->Link_Fibers[fiber][0]
		ThisFiber = NTW->Link_Fibers[fiber];

	}


	if(ThisFiber == 0)
	{
		printf("\n\n\t\t HUGE ERROR, WE COULDN'T FIND FIBER : %d", fiber);
	}

	return ThisFiber;
}

void print_source_dest(struct NETWORK * NTW, struct SPARAMETERS *SM_Parameters)
{
	int i, j;
	if(NTW->tempo >= 100 && NTW->tempo <= 299)
	{
		printf("\n ");
		printf("\n ");
		for(i = 1; i <=2; i++)
		{
			printf("\n SR ");
			for(j = 0; j <= 6; j++)
			{
		       printf(" \t %d ",NTW->source     [38][i][j]);
			}
			printf("\n DT ");
		    for(j = 0; j <= 6; j++)
			{
			   printf(" \t %d ",NTW->destination     [38][i][j]);
			}
			printf("\n PR ");
		    for(j = 0; j <= 6; j++)
			{
			   printf(" \t %d ",NTW->priority     [38][i][j]);
			}
		}
	}
    //a = NTW->source     [to_link][wave_t][NTW->link_dist[ndr][next_node]]
}

float computePolicyChange(struct NETWORK * NTW, struct SPARAMETERS *SM_Parameters)
{
	//float Change = 0.0;
	float AvgN1, AvgN2, AvgCH;
	float SumN1, SumN2, SumN3;
	int i,j,k;

	SumN1 = 0.0;
	SumN2 = 0.0;
	SumN3 = 0.0;

    //for (i = 0; i < NTW->nbr_of_nodes; i++)
    //{
  	  //for( j = 1; j < NTW->nbr_of_nodes; j++)
  	  //{
	      i = 0;
	      j = 10;
  		  if(j == 10)
  		  {
  		     printf("\n POLICY ");
  		     for( k = 1; k <= NTW->num_outputs[i][j]; k++ )
  		     {
  			    //
  			    SumN3 += NTW->MyPolicy [i][j][k];
  			    //if(k == 2)
  			       printf("\t %4.8f", NTW->MyPolicy[i][j][k]);
  		     }
  		  }
  		  //AvgCH = SumN3 / max_links_per_node;
  		  //SumN2 += AvgCH;
  	  //}
  	  //AvgN2 = SumN2 / (NTW->nbr_of_nodes - 1);
  	  //SumN1 += AvgN2;
    //}
    AvgN1 = SumN1 / NTW->nbr_of_nodes;
    //Change = AvgN1;

	float Change;
	float MyPPL = 0.0;
	int TotalnumAbs,TotalnumIN;
    int ndr, linkin, wave;

    TotalnumAbs = 0;
    TotalnumIN  = 0;

	for (ndr = 0; ndr < NTW->nbr_of_nodes; ndr++)
	      for (linkin = 1; linkin <= NTW->input_links[ndr]; linkin++)
	         for (wave = 1; wave <= SM_Parameters->num_waves; wave++)
	         {
	            TotalnumAbs += NTW->numAbs[ndr][linkin][wave];
	            TotalnumIN  += NTW->numInj[ndr][linkin][wave];
                // A
	            if( (NTW->numInj[ndr][linkin][wave] / NTW->tempo) > 1)
	            {
	            	printf("\n \t SOMETHING_WRONG!!!");
	            }
	         }
	Change = (float) TotalnumAbs / (float) TotalnumIN;
	printf("\n TotalnumAbs %d  TotalnumIN %d  Change = %4.8f",TotalnumAbs,TotalnumIN,Change);


    //NTW->MyPolicy [ndr][dest][ch]
	return Change;
}


void more_delays(struct NETWORK * NTW, struct SPARAMETERS *SM_Parameters, int ndr, int dest, int MyFiber)
{
   int AAdd;

   if ( (NTW->NDL[ndr][MyFiber] <= SM_Parameters->AllowedDL-1) &&
	    (NTW->Phase == 2)
      )
   {
      // If there are more fibers pointing to the same node.
	  if (NTW->num_of_fibers[ndr][NTW->forw_table[ndr][dest][1]] >= 2)
	  {
	     // To increase DL for each fiber pointing to the same node
	     for (AAdd = 1; AAdd <= NTW->output_links[ndr] + 1; AAdd++)
	     {
	        // If this fiber connects to Olink, i.e. extra fiber
	        if(NTW->connected_to[ndr][AAdd] == NTW->connected_to[ndr][MyFiber])
	        {
	           if(NTW->NDL[ndr][AAdd] <= SM_Parameters->AllowedDL-1)
	              NTW->NDL[ndr][AAdd] = NTW->NDL[ndr][AAdd] + SM_Parameters->ADDNDL;
	        }
	     }//end for
	  }
	  else
         NTW->NDL[ndr][MyFiber] = NTW->NDL[ndr][MyFiber] + SM_Parameters->ADDNDL;
   }

   // We are dimensioning THE SHORTEST PATH
   int next_node = NTW->forw_table[ndr][dest][1];

   // More fibers ??
   if((DIMEN==1) && (NTW->Phase == 200) && (dest!=VirtualNode) &&
      (NTW->NDL[ndr][MyFiber]==SM_Parameters->AllowedDL) && (NTW->output_links[ndr]<max_links_per_node)
     )
   {
      if( (NTW->n_links < max_num_links) && (SM_Parameters->Umbral < 1.0))
         more_links(ndr,next_node,MyFiber,NTW,SM_Parameters);
   }
}

void computeINJ(struct NETWORK * NTW, struct SPARAMETERS *SM_Parameters)
{
	int ndr, linkin, wave;
	int NINJ, DiffIN;

    for (ndr = 0; ndr < NTW->nbr_of_nodes; ndr++)
	      for (linkin = 1; linkin <= NTW->input_links[ndr]; linkin++)
	         for (wave = 1; wave <= SM_Parameters->num_waves; wave++)
	         {
	        	 NINJ   = NTW->numInj[ndr][linkin][wave];
	        	 DiffIN = NINJ - NTW->tempo;
                // A
	            if( DiffIN > 1)
	            {
	            	printf("\n \t INJ_IS_WRONG!!!");
	            }
	         }
}

void computeRD (int ndr, int src, unsigned long int num_flow, int size_flow, int num_packet, struct NETWORK *NTW, struct SPARAMETERS *SM_Parameters, int LoB)
{
   // Reorder Density (RD) captures both the amount and extent of reordering of packets
   // in an arrival sequence using a discrete density that represents the fractions of
   // the sequence size with respect to packet displacements.
   // Metrics for Packet Reordering Ð A Comparative Analysis. Nischal M. Piratla1 Anura P. Jayasumana2

   // Source    = src
   // NumFlow   = num_flow
   // SizeFlow  = size_flow
   // NumPacket = num_packet

   // Affecting: Current_Flow [ndr][src][0-2] = num_flow
   //            Size_Flow    [ndr][src][0-2] = size_flow
   //            Track_EmptyS [ndr][src][0-2] = EmptySlot
   //            ArrivedSeq   [ndr][src][0-2][0-12] = -12, ... -3,-2,-1,0,1,2,3, ... 12

   int LowerFlow;
   int Index, EmptyS;
   Index = 0;

   // Is num_flow in Current_Flow?
   //    YES
   //       Get Index
   //    NO
   //       IF Current_Flow[ndr][src][0] == Current_Flow[ndr][src][1] == Current_Flow[ndr][src][2] == 0
   //          - Index = 0;
   //       ELSE
   //          - Find lower flow number: LowerFlow <-
   //          - Add info regarding LowerFlow to Global RD
   //          - Remove LowerFlow
   //          - Assign Index <- LowerFlow
   //          - Set Track_EmptyS[ndr][src][Index] = 1, i.e. first packet

   if( (NTW->Current_Flow[ndr][src][0] == num_flow) ||
       (NTW->Current_Flow[ndr][src][1] == num_flow) ||
       (NTW->Current_Flow[ndr][src][2] == num_flow)
	 )
   {
      // Get Index
      if(NTW->Current_Flow[ndr][src][0] == num_flow)
         Index = 0;
      else
         if(NTW->Current_Flow[ndr][src][1] == num_flow)
            Index = 1;
         else
            Index = 2;
   }
   else
   {
      if( (NTW->Current_Flow[ndr][src][0] == 0) &&
          (NTW->Current_Flow[ndr][src][1] == 0) &&
          (NTW->Current_Flow[ndr][src][2] == 0)
        )
      {
         // First time dealing with a Flow for this ndr-src combination!
         Index = 0;
         NTW->TrackEmptyS[ndr][src][Index] = 1;
      }
      else
      {
         // Find LowerFlow, add info regarding LowerFlow to GlobalRD
         if(NTW->Current_Flow[ndr][src][0] < NTW->Current_Flow[ndr][src][1])
         {
            if(NTW->Current_Flow[ndr][src][0] < NTW->Current_Flow[ndr][src][2])
               LowerFlow = 0;
            else
               LowerFlow = 2;
         }
         else
         {
            if(NTW->Current_Flow[ndr][src][1] < NTW->Current_Flow[ndr][src][2])
               LowerFlow = 1;
            else
               LowerFlow = 2;
         }

         // Find the LoweFlow based on NTW->Size_Flow - NTW->TrackEmptyS, so the flow with the lowest difference
         // between NTW->Size_Flow and NTW->TrackEmptyS as that is the flow closer to be completed.
         /*if( (NTW->Size_Flow[ndr][src][0] - NTW->TrackEmptyS[ndr][src][0]) <
             (NTW->Size_Flow[ndr][src][1] - NTW->TrackEmptyS[ndr][src][1])
           )
         {
             if( (NTW->Size_Flow[ndr][src][0] - NTW->TrackEmptyS[ndr][src][0]) <
                 (NTW->Size_Flow[ndr][src][2] - NTW->TrackEmptyS[ndr][src][2])
               )
             {
            	 LowerFlow = 0;
             }
             else
             {
            	 LowerFlow = 2;
             }
         }
         else
         {
             if( (NTW->Size_Flow[ndr][src][1] - NTW->TrackEmptyS[ndr][src][1]) <
                 (NTW->Size_Flow[ndr][src][2] - NTW->TrackEmptyS[ndr][src][2])
               )
             {
            	 LowerFlow = 1;
             }
             else
             {
            	 LowerFlow = 2;
             }
         }*/

         add2GlobalRD(ndr,src,LowerFlow,NTW,SM_Parameters);
         // Remove LowerFlow, Index <- LowerFlow
         NTW->Current_Flow[ndr][src][LowerFlow] = 0;
         Index = LowerFlow;
         // Set Track_EmptyS[ndr][src][Index] = 1
         NTW->TrackEmptyS[ndr][src][Index] = 1;
      }
   }

   // Ok, we got an Index
   // Size_Flow    [ndr][src][Index] = size_flow
   // Current_Flow [ndr][src][Index] = num_flow
   // EmptyS <- Track_EmptyS [ndr][src][Index]
   // IF num_packet == 0
   //    IF EmptyS == size_flow
   //       ArrivedSeq[ndr][src][Index][EmptyS] = 0
   //       Add info about this Flow (i.e. Index) to Global RD
   //    ELSE
   //       ArrivedSeq[ndr][src][Index][EmptyS] = size_flow - EmptyS
   //       TrackEmptyS[ndr][src][Index]++
   // ELSE
   //    ArrivedSeq[ndr][src][Index][EmptyS] = num_packet - EmptyS
   //    TrackEmptyS[ndr][src][Index]++
   NTW->Size_Flow   [ndr][src][Index] = size_flow;
   NTW->Current_Flow[ndr][src][Index] = num_flow;

   if(num_flow == 490644)
   {
	   printf("\n TRACKING_THIS_FLOW %d , %d ",NTW->Phase, NTW->tempo);
   }

   EmptyS = NTW->TrackEmptyS[ndr][src][Index];

   if(EmptyS > size_flow)
   {
	   printf("\n CANNOT_BE_EMPTY_SLOT_WRONG - EmptyS > size_flow ");
	   exit(-1);
   }

   if(num_packet == 0)
   {
      if(EmptyS == size_flow)
      {
         NTW->ArrivedSeq    [ndr][src][Index][EmptyS] = 0;
         NTW->PacketArrived [ndr][src][Index][EmptyS] = num_packet;
         NTW->FromFlow      [ndr][src][Index][EmptyS] = num_flow;
         add2GlobalRD(ndr,src,Index,NTW,SM_Parameters);
      }
      else
      {
         NTW->ArrivedSeq    [ndr][src][Index][EmptyS] = size_flow - EmptyS;
         NTW->PacketArrived [ndr][src][Index][EmptyS] = num_packet;
         NTW->FromFlow      [ndr][src][Index][EmptyS] = num_flow;
         LowerFlow = NTW->TrackEmptyS   [ndr][src][Index];
         NTW->TrackEmptyS   [ndr][src][Index]++;
         if(NTW->TrackEmptyS[ndr][src][Index] > ( MaxBLength + 2))
         {
        	 printf("\n MISTAKE_EMPTY1");
         }
      }
   }
   else
   {
      NTW->ArrivedSeq    [ndr][src][Index][EmptyS] = num_packet - EmptyS;
      NTW->PacketArrived [ndr][src][Index][EmptyS] = num_packet;
      NTW->FromFlow      [ndr][src][Index][EmptyS] = num_flow;
      LowerFlow = NTW->TrackEmptyS   [ndr][src][Index];
      NTW->TrackEmptyS   [ndr][src][Index]++;
      if(NTW->TrackEmptyS[ndr][src][Index] > (MaxBLength + 2) )
      {
     	 printf("\n MISTAKE_EMPTY2");
      }
   }
}

void add2GlobalRD(int ndr, int src, int this_flow, struct NETWORK *NTW, struct SPARAMETERS *SM_Parameters)
{
   // A
   // After processing, SET TO -999 ArrivedSeq [ndr][src][this_flow]
   int i,PacketDelay;
   for(i = 0; i < MaxBLength; i++)
   {
	   // NTW->GlobalRDP Positives
	   // NTW->GlobalRDN Negatives
	   PacketDelay = NTW->ArrivedSeq[ndr][src][this_flow][i];
	   if(PacketDelay < 0)
	   {
		   PacketDelay = -1* PacketDelay;
		   NTW->GlobalRDN[PacketDelay]++;
	   }
	   else
	   {
		   NTW->GlobalRDP[PacketDelay]++;
	   }
	   NTW->ArrivedSeq    [ndr][src][this_flow][i] = 0;
	   NTW->PacketArrived [ndr][src][this_flow][i] = 0;
	   NTW->FromFlow      [ndr][src][this_flow][i] = 0;
	   //NTW->CountGlobalRD++;
   }
   NTW->TrackEmptyS [ndr][src][this_flow] = 1;
   NTW->Size_Flow   [ndr][src][this_flow] = 0;
   NTW->Current_Flow[ndr][src][this_flow] = 0;

}

unsigned long int generateFlowID(int ndr, struct NETWORK *NTW, struct SPARAMETERS *SM_Parameters)
{
	int MyID = 666;
	//MyID = Rnd(1,1000) + NTW->NumFlowS[ndr] + NTW->tempo;
	NTW->MyFlowID++;
	MyID = NTW->MyFlowID;
	return MyID;
}

int find_second_node(int ndr, int fiber, struct NETWORK * NTW, struct SPARAMETERS *SM_Parameters)
{
	int Olink, Auxlink, F1, N2;
	// j <= NTW->output_links[n1]
	// real_fiber = NTW->num_output_link[n1][j];
	// NTW->num_initial_links is lower than
	for(Olink = 2; Olink <= NTW->output_links[ndr]; Olink++)
	{
		F1 = NTW->num_output_link[ndr][Olink];
		if(F1 == fiber)
		{
			// We found it
			N2 = NTW->connected_to[ndr][Olink];
			break;
		}
	}
	return N2;
}
