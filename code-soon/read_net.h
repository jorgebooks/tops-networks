/*===========================================================================*/
/*                             READ TOPOLOGY                                 */
/*===========================================================================*/
/*  I START TO READ THE TOPOLOGY FILES AND THE FORWARDING TABLE FILE         */
/*---------------------------------------------------------------------------*/

/*===========================================================================*/
/*                            read_line FUNCTION                             */
/*===========================================================================*/
int read_line (char *line)
{
   char c ;
   int  rc ;
   int  index = 0 ;
   do
   {
      rc             = fscanf (f1, "%c", &c) ;
      line [index++] = c ;
   } while ( ! ((c == '\n') || (c == '\r') || (rc == EOF))) ;
   line [index++] = '\0' ;
   if (rc == EOF)
      return (rc) ;
   else
      return (index) ;
}

/*===========================================================================*/
/*                            read_injections  FUNCTION                      */
/*===========================================================================*/
void read_injections (struct NETWORK *NTW)
{

   // read injection table information  until blank line

   char line [MAX_LINE_SIZE] ;
   int  rc,i ;
   int  ndr, n_inj;
   int  count = 0 ;
   char node_name[MAX_CHARS];

   rc = read_line (line) ;
   while ( ! ((strlen(line) < 2) || (rc == EOF)))
   {
      sscanf(line, "%d %d", &ndr, &n_inj);
      printf("%d %d ", ndr, n_inj);
      NTW->num_inj_links[ndr] = n_inj; /* # of output in the forwarding table */
      printf("%d %d  ", ndr, NTW->num_inj_links[ndr]);
      printf(" \n ");

      count++ ;
      rc = read_line (line) ;
   }
   printf ("Scanned %d nodes (%d)\n", NTW->nbr_of_nodes, count) ;

}

/*===========================================================================*/
/*                            read_nbr  FUNCTION                             */
/*===========================================================================*/
void read_nbr (struct NETWORK *NTW)
{
   char line [MAX_LINE_SIZE];
   int rc;
   do
   {
      rc = read_line (line);
   }while ( ! ((strncmp (line, "STATISTICS", sizeof("STATISTICS")-1) == 0) || (rc == EOF)));
   rc = read_line (line); sscanf (line, "%d", &NTW->nbr_of_nodes) ;
   rc = read_line (line); sscanf (line, "%d", &NTW->nbr_of_edges) ;
   rc = read_line (line); sscanf (line, "%d", &NTW->xmin) ;
   rc = read_line (line); sscanf (line, "%d", &NTW->xmax) ;
   rc = read_line (line); sscanf (line, "%d", &NTW->ymin) ;
   rc = read_line (line); sscanf (line, "%d", &NTW->ymax) ;
   rc = read_line (line); sscanf (line, "%d", &NTW->dist_factor);
}

/*===========================================================================*/
/*               read_simulation_parameters  FUNCTION                        */
/*===========================================================================*/
void read_simulation_parameters (struct SPARAMETERS *SM)
{
   char line [MAX_LINE_SIZE];
   int rc;

   do
   {
      rc = read_line (line) ;
   }while ( ! ((strncmp (line, "SIMULATION", sizeof("SIMULATION")-1) == 0) || (rc == EOF))) ;
   rc = read_line (line); sscanf (line, "%d", &SM->pareto);
   rc = read_line (line); sscanf (line, "%d", &SM->AllowedDL);
   rc = read_line (line); sscanf (line, "%d", &SM->ADDNDL);
   rc = read_line (line); sscanf (line, "%d", &SM->Initial_Time);
   rc = read_line (line); sscanf (line, "%d", &SM->Optimization_Time);
   rc = read_line (line); sscanf (line, "%d", &SM->Operation_Time);
   rc = read_line (line); sscanf (line, "%f", &SM->Umbral);
   rc = read_line (line); sscanf (line, "%f", &SM->load);
   rc = read_line (line); sscanf (line, "%d", &SM->num_waves);
   rc = read_line (line); sscanf (line, "%d", &SM->num_predefined_DL);
   rc = read_line (line); sscanf (line, "%d", &SM->SecondDL);
}


/*===========================================================================*/
/*                            read_nodes  FUNCTION                           */
/*===========================================================================*/
void read_nodes (struct NETWORK *NTW)
{
   // read line until keyword VERTICES
   // read all nodes until blank line
   char line [MAX_LINE_SIZE] ;
   int  rc ;
   int  nbr, level, x, y;
   int  count = 0 ;
   char node_name[MAX_CHARS];

   do
   {
      rc = read_line (line) ;
   }while ( ! ((strncmp (line, "VERTICES", sizeof("VERTICES")-1) == 0) || (rc == EOF))) ;

   rc = read_line (line) ;
   while ( ! ((strlen(line) < 2) || (strncmp (line, "EDGES", sizeof("EDGES")-1) == 0) || (rc == EOF)))
   {
      sscanf (line, "%d %d %d %d", &nbr, &level, &x, &y) ;
      sprintf(node_name, "node_%d", nbr);
      if (nbr >= NTW->nbr_of_nodes)
      {
         printf ("**** Problems: the sequence nbr of the node is bigger than the nbr_of_nodes\n") ;
         exit (-1) ;
      }

      // ?? store ....
      // not needed

      count++ ;
      rc = read_line (line) ;
   }
   printf ("Scanned %d nodes (%d)\n", NTW->nbr_of_nodes, count) ;
   printf ("Dimensions (X: %d %d - Y: %d %d)\n", NTW->xmin, NTW->xmax, NTW->ymin, NTW->ymax) ;
   if (count != NTW->nbr_of_nodes)
   {
      printf ("**** Problems: different nbr_of_nodes as expected\n") ;
      exit (-1) ;
   }
}


/* ========================================================================== */
/*                             LINK INITIALIZATION                            */
/* ========================================================================== */
void  LinkInitialization(struct NETWORK *NTW,struct SPARAMETERS *SM_Parameters)
{
   int i,j,k, AAdd;
   int ndr, ndr1, linkin, wave;

   NTW->AveMess = 1;
   NTW->numMess = 1;

   /*  ************  Packet length distribution   *************  */
   if(AveLength != 1)                    /*   Correlated traffic        */
   {
      printf("Inizia poisson()\n");
      NTW->fac[1] = 1;
      for (k = 2; k <= AveLength * 2; k++)
      {
         NTW->fac[k] = NTW->fac[k-1] * k;
         printf("factorial->%f \n",NTW->fac[k] );
      }
      NTW->poisson[1] = pow(AveLength,0) * exp(-AveLength)/1;
      for (k = 2; k <= AveLength * 2; k++)
      {
         NTW->poisson[k] = NTW->poisson[k-1] + pow(AveLength,k-1) * exp(-AveLength)/NTW->fac[k-1];
         printf("APDF %i %f %f\n",k,NTW->poisson[k], pow(AveLength,k-1));
      }
      NTW->poisson[AveLength*2+1] = 1;
      printf("APDF %i %f \n",AveLength*2+1,NTW->poisson[AveLength*2+1]);
      printf("Termina Poisson()\n");
   }

   /*  ************  Packet length distribution   *************  */
   NTW->n_links = 0;
   printf("nbr_of_nodes= %d, nbr_of_edges= %d \n", NTW->nbr_of_nodes, NTW->nbr_of_edges) ;

   for (i = 0; i < NTW->nbr_of_nodes; i++)
      for (j = 0; j < NTW->nbr_of_nodes; j++)
      {
         NTW->num_of_fibers[i][j] = 0;         /* what link is ???         */
         NTW->link_dist[i][j]     = 0;         /* Longitud of the link     */
      }

   for (i = 0; i < NTW->nbr_of_nodes; i++)
   {
      NTW->num_outputs[i][i]   = 1;
      NTW->output_links[i]     = 0;
      NTW->input_links[i]      = 0;
      NTW->num_of_fibers[i][i] = 0;
      NTW->link_dist[i][i]     = 0;
   }

   for (i = 0; i < NTW->nbr_of_nodes; i++)         /* inicializa injection links   */
      for (j = 1; j <= NTW->num_inj_links[i]; j++) /* depending on injection table */
      {
         NTW->output_links[i]++;
         NTW->input_links[i]++;
         NTW->num_input_link[i][NTW->input_links[i]] = NTW->n_links;
         NTW->connected_from[i][NTW->input_links[i]] = i;
         NTW->num_of_link[i][i][1]                   = NTW->n_links;
         NTW->num_of_fibers[i][i]                    = 1;
         NTW->link_dist[i][i]                        = 1;
         NTW->link_size[NTW->n_links]                = NTW->link_dist[i][i];
         NTW->n_links++;
         printf ("num_input_link= %d \n", NTW->num_input_link[i][NTW->input_links[i]]) ;
      }

   for (i = 0; i <= max_num_links; i++)    /* inicializa packets  */
      for (j = 1; j <= SM_Parameters->num_waves; j++)
         for (k = 1; k <= max_slots_per_link; k++)
         {
            NTW->source[i][j][k]       = VirtualNode;
            NTW->destination[i][j][k]  = VirtualNode;
            //NTW->num_slots[i][j][k]    = 0;
            //NTW->num_hops[i][j][k]     = 0;
            //NTW->cell_num[i][j][k]     = 0;
            //NTW->num_of_cells[i][j][k] = 0;
            //NTW->priority[i][j][k]     = 0;
            //NTW->offset_time[i][j][k]  = 0;

         }

   for (i = 1; i <= max_num_node * (max_links_per_node); i++)
      for (j = 1; j <= SM_Parameters->num_waves; j++)
         for (k = 1; k <= SM_Parameters->AllowedDL; k++)
         {                                 /* inicializa Buffers  */
            NTW->OutBuffer[i][j][k]     = 0;
            NTW->Bfromlink[i][j][k]     = 0;
            NTW->Baddress[i][j][k]      = 0;
            NTW->Bsource[i][j][k]       = VirtualNode;
            NTW->Bdestination[i][j][k]  = VirtualNode;
            //NTW->Bnum_slots[i][j][k]    = 0;
            //NTW->Bnum_hops[i][j][k]     = 0;
            //NTW->Bcell_num[i][j][k]     = 0;
            //NTW->Bnum_of_cells[i][j][k] = 0;
            //NTW->Bpriority[i][j][k]     = 0;
            //NTW->Boffset_time[i][j][k]  = 0;
         }

   for (ndr = 0; ndr < NTW->nbr_of_nodes; ndr++)
      for (AAdd = 1 ; AAdd <= MaxnumAdd; AAdd++)
         NTW->Pc[ndr][AAdd] = 0;

   for (ndr = 0; ndr< NTW->nbr_of_nodes; ndr++)
      for (linkin = 1; linkin <= max_links_per_node; linkin++)
         for (wave = 1; wave <= SM_Parameters->num_waves; wave++)
         {                              /* inicializa Stadistical Variables */
            NTW->numInj[ndr][linkin][wave]    = 0;
            NTW->numFullIN[ndr][linkin][wave] = 0;
            NTW->numAbs[ndr][linkin][wave]    = 0;
            //NTW->numHops[ndr][linkin][wave]   = 0;
            //NTW->numSlots[ndr][linkin][wave]  = 0;
            NTW->numdef[ndr]                  = 0;     /* Number of deflections   */
            NTW->numPPL[ndr]                  = 0;     /* Number of packets loss  */
            NTW->numMPR[ndr]                  = 0;
            NTW->numLossmpr[ndr]              = 0;

         }


}        /* End LINK INITIALIZATION  */

/*===========================================================================*/
/*                            read_edges  FUNCTION                           */
/*===========================================================================*/
void read_edges (struct NETWORK *NTW)
{
   // read line until keyword EDGES
   // read all nodes until blank line or end of file
   char line [MAX_LINE_SIZE] ;
   int  rc ;
   int  n1, n2, i ;
   int  count = 0 ;
   char link_name[MAX_CHARS];

   int    *link_obj ;
   char   name1[MAX_CHARS], name2[MAX_CHARS];
   char   xmt1_name[MAX_CHARS], xmt2_name[MAX_CHARS];
   char   rcv1_name[MAX_CHARS], rcv2_name[MAX_CHARS];
   double x1, y1 ;
   double x2, y2 ;
   double distance ;
   int    *next_span ;

   NTW->n_fibers = 0;

   // Link_Fibers[max_num_links][fibers_per_link]
   int Olink;
   for(Olink = 0; Olink < max_num_links; Olink++)
   {
	   NTW->Link_Fibers[Olink] = 0;
   }

   do
   {
      rc = read_line (line);
   }while ( ! ((strncmp (line, "EDGES", sizeof("EDGES")-1) == 0) || (rc == EOF))) ;

   rc = read_line (line) ;
   while ( ! ((strlen(line) < 2) || (rc == EOF)))
   {
      sscanf (line, "%d %d %lf", &n1, &n2, &distance) ;
      if (n1 >= NTW->nbr_of_nodes)
      {
         printf ("**** Problems: nbr is biger than the nbr_of_nodes  (link origin)\n") ;
         exit (-1) ;
      }
      if (n2 >= NTW->nbr_of_nodes)
      {
         printf ("**** Problems: nbr is biger than the nbr_of_nodes  (link origin)\n") ;
         exit (-1) ;
      }

      sprintf(link_name, "link_%d", count);

      // ?? store ....

      /* *******************    LINKS-LINKS     ********************** */
                                                /* Link #1 is for injection */
      NTW->output_links[n1]++;                  /* output-links from a node */
      NTW->num_of_link[n1][n2][1] = NTW->n_links;
      NTW->input_links[n2]++;                   /* input-links to a node    */
      NTW->num_input_link[n2][NTW->input_links[n2]] = NTW->n_links; /* [node][1-x links]   */
      NTW->connected_from[n2][NTW->input_links[n2]] = n1;
      NTW->num_of_fibers[n1][n2]                    = 1;              /* what link is ???  */
      NTW->link_dist[n1][n2]       = (int)(distance/NTW->dist_factor); /* Longitud of the link */
      NTW->link_size[NTW->n_links] = NTW->link_dist[n1][n2];
      NTW->n_links++;
      NTW->n_fibers++;

      if(link_kind == 2)
      {
         NTW->output_links[n2]++;
         NTW->num_of_link[n2][n1][1]                   = NTW->n_links;
         NTW->input_links[n1]++;
         NTW->num_input_link[n1][NTW->input_links[n1]] = NTW->n_links;
         NTW->connected_from[n1][NTW->input_links[n1]] = n2;
         NTW->num_of_fibers[n2][n1]                    = 1;
         NTW->link_dist[n2][n1]                        = (int)(distance/NTW->dist_factor);
         NTW->link_size[NTW->n_links]                  = NTW->link_dist[n2][n1];
         NTW->n_links++;
         NTW->n_fibers++;
      }

      //NTW->cost_db [n1] [n2]  = distance  + var ;
      var                    += 0.00000001 ;
      //NTW->cost_db [n2] [n1]  = distance  + var ;
      var                    += 0.00000001 ;
      /* ******************* LINKS-LINKS  ********************** */

      printf ("From node %d, to node %d, cost: %f (%d) %d \n", n1, n2, (double)distance, NTW->link_dist[n1][n2],NTW->num_of_fibers[n1][n2]) ;

      NTW->consecutive_link[count][0] = n1;
      NTW->consecutive_link[count][1] = n2;

      count++ ;
      rc = read_line (line) ;
   }
   printf ("Scanned %d edges (%d)\n", NTW->nbr_of_edges, count) ;
   if (count != NTW->nbr_of_edges)
   {
      printf ("**** Problems: different nbr_of_edges as expected\n") ;
      exit (-1) ;
   }
}


/*===========================================================================*/
/*                            read_tables  FUNCTION                          */
/*===========================================================================*/
void read_tables (struct NETWORK *NTW)
{
   // read forwarding table information  until blank line

   char line [MAX_LINE_SIZE] ;
   int  rc,i ;
   int  ndr, dest,n_out,o[10];
   int  count = 0 ;
   char node_name[MAX_CHARS];

   rc = read_line (line) ;
   while ( ! ((strlen(line) < 2) || (strncmp (line, "EDGES", sizeof("EDGES")-1) == 0) || (rc == EOF)))
   {
      sscanf (line, "%d %d %d %d %d %d %d %d %d %d", &ndr, &dest, &n_out, &o[1], &o[2],&o[3],&o[4],&o[5],&o[6],&o[7]) ;

      printf("%d %d %d ", ndr, dest,n_out);
      NTW->num_outputs[ndr][dest]=n_out;         /* # of output in the forwarding table */

      for(i = 1; i <= n_out; i++)
         NTW->forw_table[ndr][dest][i]=o[i];     /* # of the next hop node              */

      for(i = 1; i <= NTW->num_outputs[ndr][dest]; i++)
         printf("%d  ", NTW->forw_table[ndr][dest][i]);
      printf(" \n ");

      if(ndr >= NTW->nbr_of_nodes)
      {
         printf ("**** Problems: the sequence ndr of the node is bigger than the nbr_of_nodes\n") ;
         exit (-1) ;
      }
      count++ ;
      rc = read_line (line) ;
   }
   printf ("Scanned %d nodes (%d)\n", (NTW->nbr_of_nodes)*(NTW->nbr_of_nodes-1), count) ;

   /*if (count != NTW->nbr_of_nodes*(NTW->nbr_of_nodes-1))
   {
      printf ("**** Problems: different nbr_of_nodes as expected\n") ;
      exit (-1) ;
   }*/
   // Se debe realizar esta validaci—n !!!!!!!!
   // Pero la estoy omitiendo para esta prueba
}

/*===========================================================================*/
/*                          read_failures  FUNCTION                          */
/*===========================================================================*/
void read_failures(struct NETWORK *NTW, struct SPARAMETERS *SM_Parameters)
{
   char line [MAX_LINE_SIZE] ;
   int  rc, i,j, timeINI, timeFIN, link, k, l;
   int  fiber, wavelength, cFW, real_fiber;
   int MySource, MyDest;

   i = 5001; j = 501;

   printf("\n --- READING FAILURES --- ");

   // Clear NTW->Failure_Waves
   for (j=0; j < 7000; j++)
   {
      NTW->Failure_Waves[j][0] = 0;
      NTW->Failure_Waves[j][1] = 0;
      NTW->Failure_Waves[j][2] = 0;
      NTW->Failure_Waves[j][3] = 0;
      NTW->Failure_Waves[j][4] = 0;
      NTW->Failure_Waves[j][5] = 0;
      //NTW->Failure_Waves[j][6] = 0;
      //NTW->Failure_Waves[j][7] = 0;
   }

   rc  = read_line (line) ;
   rc  = read_line (line) ;
   cFW = 0;
   while( (strlen(line) > 2) && (rc != EOF) )
   {
      sscanf (line, "%d %d %d %d %d %d", &timeINI, &timeFIN, &fiber, &wavelength, &MySource, &MyDest);

      if(timeINI <= timeFIN)
      {
         //if(fiber != 666)
         //{
            NTW->Failure_Waves[cFW][0] = timeINI;
            NTW->Failure_Waves[cFW][1] = timeFIN;
            NTW->Failure_Waves[cFW][2] = fiber;
            NTW->Failure_Waves[cFW][3] = wavelength;
            // Determining real_fiber
            // n1 is the source node
            /*k          = 0;
            real_fiber = 0;
            for(j = 2; j <= NTW->output_links[n1] && real_fiber == 0; j++)
            {
               if(NTW->connected_to[n1][j] == n2)
               {
                  //k++;
                  //if(k == fiber)
                  //if(k == 0)
                  real_fiber = NTW->num_output_link[n1][j];
               }
            }
            NTW->Failure_Waves[cFW][4] = real_fiber;
            NTW->Failure_Waves[cFW][5] = wavelength;  // If 666 all the wavelengths
            */
            NTW->Failure_Waves[cFW][4] = MySource;
            NTW->Failure_Waves[cFW][5] = MyDest;

            cFW++;
         //}
         /*else
         {
            printf("\n LINKS ENTRE %d y %d",n1,n2);
            for(i = 2; i <= NTW->output_links[n1]; i++)
            {
               if(NTW->connected_to[n1][i] == n2)
               {
                  printf(" - %d ",NTW->num_output_link[n1][i]);
                  real_fiber = NTW->num_output_link[n1][i];

                  NTW->Failure_Waves[cFW][0] = timeINI;
                  NTW->Failure_Waves[cFW][1] = timeFIN;
                  NTW->Failure_Waves[cFW][2] = n1;
                  NTW->Failure_Waves[cFW][3] = n2;
                  NTW->Failure_Waves[cFW][4] = real_fiber;  // If 666 all the fibers
                  NTW->Failure_Waves[cFW][5] = wavelength;  // If 666 all the wavelengths
                  cFW++;
               }
            }

            for(i = 2; i <= NTW->output_links[n2]; i++)
            {
               if(NTW->connected_to[n2][i] == n1)
               {
                  printf(" + %d ",NTW->num_output_link[n2][i]);
                  real_fiber = NTW->num_output_link[n2][i];

                  NTW->Failure_Waves[cFW][0] = timeINI;
                  NTW->Failure_Waves[cFW][1] = timeFIN;
                  NTW->Failure_Waves[cFW][2] = n1;
                  NTW->Failure_Waves[cFW][3] = n2;
                  NTW->Failure_Waves[cFW][4] = real_fiber;  // If 666 all the fibers
                  NTW->Failure_Waves[cFW][5] = wavelength;  // If 666 all the wavelengths
                  cFW++;

               }
            }
         }*/
      }
      printf("\n Time : %d - %d -> Link : %d",timeINI,timeFIN,real_fiber);
      rc = read_line (line) ;
   }

   printf("\n i : %d -  j : %d",i,j);
   for(k=0; k < SM_Parameters->Operation_Time; k++)
   {
      if(NTW->Failure_Waves[k][0] == 0)
         break;
      //
      //printf("\n FA-LINK Time : %d - %d | %d - %d | Fiber: %d W: %d | MSD %d %d ",NTW->Failure_Waves[k][0],NTW->Failure_Waves[k][1],NTW->Failure_Waves[k][2],NTW->Failure_Waves[k][3],NTW->Failure_Waves[k][4],NTW->Failure_Waves[k][5],NTW->Failure_Waves[k][6],NTW->Failure_Waves[k][7]);
      printf("\n FA-LINK Time : %d - %d | Fiber: %d W: %d | MSD %d %d ",NTW->Failure_Waves[k][0],NTW->Failure_Waves[k][1],NTW->Failure_Waves[k][2],NTW->Failure_Waves[k][3],NTW->Failure_Waves[k][4],NTW->Failure_Waves[k][5]);
   }

   NTW->dw = NTW->Failure_Waves[0][3];

   for(k=0; k <= NTW->nbr_of_nodes; k++)
   {
      printf("\n");
      for(l=2; l <= NTW->output_links[k]; l++)
      {
         printf(" %d(%d)",NTW->num_output_link[k][l],NTW->connected_to[k][l]);
      }
   }

}

/*===========================================================================*/
/*                            read_tables  FUNCTION                          */
/*===========================================================================*/
void read_new_routes (struct NETWORK *NTW)
{
   // read forwarding table information  until blank line

   // comm table_complete table_cut.txt

   char line [MAX_LINE_SIZE] ;
   int  rc,i ;
   int  ndr, dest,n_out,o[10];
   int  count = 0 ;
   char node_name[MAX_CHARS];

   rc = read_line (line) ;
   while ( ! ((strlen(line) < 2) || (rc == EOF)))
   {
      sscanf (line, "%d %d %d %d %d %d %d %d %d %d", &ndr, &dest, &n_out, &o[1], &o[2],&o[3],&o[4],&o[5],&o[6],&o[7]) ;

      printf("%d %d %d ", ndr, dest,n_out);
      NTW->new_outputs[ndr][dest]=n_out;         /* # of output in the forwarding table */

      for(i = 1; i <= n_out; i++)
         NTW->new_ftable[ndr][dest][i]=o[i];     /* # of the next hop node              */

      for(i = 1; i <= NTW->new_outputs[ndr][dest]; i++)
         printf("%d  ", NTW->new_ftable[ndr][dest][i]);
      printf(" \n ");

      if(ndr >= NTW->nbr_of_nodes)
      {
         printf ("**** Problems: the sequence ndr of the node is bigger than the nbr_of_nodes - Reading new routes\n") ;
         exit (-1) ;
      }
      count++ ;
      rc = read_line (line) ;
   }
   printf ("Scanned %d nodes (%d) for new routes \n", (NTW->nbr_of_nodes)*(NTW->nbr_of_nodes-1), count) ;
   if (count != NTW->nbr_of_nodes*(NTW->nbr_of_nodes-1))
   {
      printf ("**** Problems: different nbr_of_nodes as expected in new routes\n") ;
      exit (-1) ;
   }
}


