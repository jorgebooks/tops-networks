/*
 * Attack.c
 *
 *  Created on: Jun 12, 2013
 *      Author: ivan
 */

/* ============================================================== */
/* -------------------------------------------------------------- */
/*   Program Attack.c
     This program emulate  WDM optical networks.
     This program requires topology information of the network.
     The distance between nodes should be specified in kilometers.

     The logic is:

     1. Topology Information.
     2. Generation of Links.
     3. Generation of packet (Poisson, Self-similar).
     4. k-shortes paths generation.

     Node logic.

     1. Absorption.
     2. Injection.
     3. Internal Routing (Wavelength Conversion).
     4. Buffer Scheduling
     5. Buffer Management
                                                                  */
/* -------------------------------------------------------------- */
/* ============================================================== */

/* ============================================================== */
/* -------------------------------------------------------------- */
/*                 I/O LIBRARIES                                  */
/* -------------------------------------------------------------- */
/* ============================================================== */
#include        <stdio.h>       /* Libreria para la funcion I/O           */
#include        <stdlib.h>
#include        <math.h>        /* mathematic functions                   */
#include        <limits.h>
#include        <time.h>        /* Time functions                         */
#include        <string.h>

/* ============================================================== */
/*                   ---- OUTPUT FILES   ----                     */
/* ============================================================== */
FILE        *f1;         /* External Files, Topology, Table             */
FILE        *f2;         /* General Output                              */
FILE        *fscreen;    /* Throughput Output, Delays                   */
FILE        *fileout;    /* Puntatore al file d'uscita                  */

FILE        *fileout2;   /* Puntatore al file d'uscita                  */
FILE        *fUtilization;


/* ============================================================== */
/*               ---- NETWORK LIBRARIES ----                      */
/* ============================================================== */
#include        "structure.h"
#include        "rnd_functions.h"
#include        "printing.h"
#include        "network.h"
#include        "read_net.h"
#include        "forwarding.h"

/* ============================================================== */
/* -------------------------------------------------------------- */
/*                    MAIN PROGRAM                                */
/* -------------------------------------------------------------- */
/* ============================================================== */
/* -------------------------------------------------------------- */
void main()
{

   int wave;
   int i, j;
   struct SPARAMETERS *SM_Parameters;
   struct NETWORK *NTW;

   /* ------------------------------------------------------------ */
   /*          Simulation Parameters                               */
   /* ------------------------------------------------------------ */
   SM_Parameters = (struct SPARAMETERS *)malloc(sizeof(struct SPARAMETERS));
   printf ("Reading Simulation Parameters at file: Simula_Parameters \n") ;
   if ((f1 = fopen("Simula_Parameters","r")) == NULL)
   {
      printf ("Unable to read the parameter file Simula_Parameters \n") ;
      exit (-1) ;
   }
   read_simulation_parameters(SM_Parameters);
   fclose(f1);

   NTW = (struct NETWORK *)malloc(sizeof(struct NETWORK));
   /* ------------------------------------------------------------ */
   /*                  Read Topology                               */
   /* ------------------------------------------------------------ */

   printf ("Reading Injection Table at file: t_mx \n") ;
   if ((f1 = fopen("t_mx","r")) == NULL)
   {
      printf ("Unable to read the parameter file table \n") ;
      exit (-1) ;
   }
   read_injections (NTW) ;
   fclose(f1);

   printf ("Reading Topology at file: ts11 \n") ;
   if ((f1 = fopen("ts11","r")) == NULL)
   {
      printf ("Unable to read the parameter file ts11 \n") ;
      exit (-1) ;
   }
   printf ("Reading NBR at file ts11 \n");
   //read_nbr   (&NTW->nbr_of_nodes, &NTW->nbr_of_edges) ;
   read_nbr   (NTW) ;
   //printf ("Reading Simulation Parameters file ts11 \n");
   //read_simulation_parameters(NTW);
   printf ("Reading Nodes at file ts11 \n");
   read_nodes (NTW) ;
   printf ("LinkInitialization \n");
   LinkInitialization (NTW,SM_Parameters);
   printf ("Reading EDGES at file ts11 \n");
   read_edges (NTW) ;
   fclose(f1);
   /* ------------------------------------------------------------ */
   /*              Read Forwarding Table                           */
   /* ------------------------------------------------------------ */

   printf ("Reading Forwarding Table at file: table \n") ;
   if ((f1 = fopen("table","r")) == NULL)
   {
      printf ("Unable to read the parameter file table \n") ;
      exit (-1);
   }
   read_tables (NTW) ;
   fclose(f1);


   /* ------------------------------------------------------------ */
   /*                      Read New Routes                         */
   /* ------------------------------------------------------------ */
   /*
   printf ("Reading New routes at file: table_cut \n") ;
   if ((f1 = fopen("table_cut","r")) == NULL)
   {
      printf ("Unable to read the parameter file table_cut \n") ;
      exit (-1);
   }
   read_new_routes(NTW);
   fclose(f1);*/

   fscreen=fopen("Screen","w");
   Forwarding(NTW);                   /* create node's forwarding tables */
   More_forwarding(NTW);
   for (wave = 1; wave <= SM_Parameters->num_waves; wave++)
            NTW->g[wave] = SM_Parameters->load;
      //NTW->g[wave] = SM_Parameters->load/SM_Parameters->num_waves;

   New_Forwarding(NTW);
   memorias(NTW,SM_Parameters);

   for(i = 0; i <= NTW->nbr_of_nodes; i++)
   {
      printf("\n %d | ",i);
      for(j = 0; j <= NTW->nbr_of_nodes; j++)
      {
         if(NTW->link_dist[i][j] > 0)
            if(NTW->link_dist[i][j] < 10)
               printf("  %d",NTW->link_dist[i][j]);
            else
               printf(" %d",NTW->link_dist[i][j]);
         else
            printf(" --");
      }
      printf(" | ");
   }


   //srand((unsigned)time(NULL)); //TO MAKE EACH SIMULATION DIFFERENT
   srand(1); // TO MAKE EACH SIMULATION SIMILAR
   Simula(NTW,SM_Parameters);
   fclose(fscreen);

   //fileout = fopen("Resultados","w");
   //Stampa(NTW,SM_Parameters);  /* Printing results */
   //fclose(fileout);

   for(i = 0; i <= NTW->nbr_of_nodes; i++)
   {
      printf("\n %d | ",i);
      for(j = 0; j <= NTW->nbr_of_nodes; j++)
      {
         if(NTW->link_dist[i][j] > 0)
            if(NTW->link_dist[i][j] < 10)
               printf("  %d",NTW->link_dist[i][j]);
            else
               printf(" %d",NTW->link_dist[i][j]);
         else
            printf(" --");
      }
      printf(" | ");
   }

   printf("\n\n ===== POLICY ");
   int k;
   for (i = 0; i < NTW->nbr_of_nodes; i++)
   {
	  printf("\n NODE %d", i);
      {
         for( j = 1; j < NTW->nbr_of_nodes; j++)
         {
    	    if(i != j)
    	    {
    	       printf("\n Node %d", j);
 	           for( k = 1; k <= NTW->num_outputs[i][j]; k++ )
 	           {
 		          printf("\t %10.10f", NTW->MyPolicy[i][j][k]);
 	           }
    	    }
         }
      }
      printf("\n END NODE %d \n\n", i);
   }

   printf("\n =========== ACTIONS ====== ");
   for (i = 0; i < 8; i++)
   {
	   printf("\n A[%d] = %d",i,NTW->MyActions[i]);
   }

   printf("\n ====== NODES ===== ");
   for(i = 0; i <= NTW->nbr_of_nodes; i++)
   {
      printf("\n Node[%d] = %d ",i,(int)NTW->numPPL[i]);
      for (j = 1; j <= NTW->output_links[i] + 1; j++)
         printf("\t = %d ",(int)NTW->numPPLFiber[i][j]);
   }
   printf("\n ====== MaxWCactive ===== ");
   for(i = 0; i <= NTW->nbr_of_nodes; i++)
   {
      printf("\n Node[%d] = %d ",i,NTW->MaxWCactive[i]);
   }

   printf("\n");
   printf("\n Initial Links = %d - Final Links = %d",NTW->num_initial_links,NTW->n_links);
   printf("\n Pareto = %d",SM_Parameters->pareto);
   printf("\n TIME: %d - %d - %d ",SM_Parameters->Initial_Time,SM_Parameters->Optimization_Time,SM_Parameters->Operation_Time);
   printf("\n Load %f - Threshold %f",SM_Parameters->load, SM_Parameters->Umbral);
   printf("\n N_ats = %d, N_F = %d, Pos %d, Neg %d",NTW->Num_at,NTW->Num_First,NTW->Num_Pos,NTW->Num_Neg);
   printf("\n");

   /*printf("\n");
   for(i = 2; i < 7; i++)
   {
	   for(k = 0; k <= 2; k++)
	   {
	      for(j = 0; j < 6; j++)
	      {
             printf("\n SendP[%d][%d][%d] = %d ",i,k,j,NTW->SendPK[i][k][j]);
             printf("\t AbsP [%d][%d][%d] = %d ",i,k,j,NTW->AbsPK [i][k][j]);
             printf("\t LostP[%d][%d][%d] = %d ",i,k,j,NTW->LostPK[i][k][j]);
	      }
	      printf("\n ");
	   }
	   printf("\n ");
   }
   printf("\n");
   */
   /*printf("\n FlowS , FlowR \n");
   int ndr;
   for (ndr = 0; ndr < NTW->nbr_of_nodes; ndr++)
   {
	  printf("\t %d = %d , %d  \n",ndr,NTW->NumFlowS[ndr],NTW->NumFlowR[ndr]);
   }*/

   printf("\n MaxBURST : %d \n", NTW->MaxBURST);

}                  /*     end  main */
/* ============================================================= */

