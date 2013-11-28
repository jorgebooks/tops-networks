/* ========================================================================== */
/*              Printing                                                      */
/* ========================================================================== */
/* -------------------------------------------------------------------------- */
void Stampa(struct NETWORK *NTW, struct SPARAMETERS *SM_Parameters)
{
   int               ndr, TotalNDL, linkin, wave, AAdd, Olink,NDLperNODE[100];
   float             AveD, AveDpN[100],AveApN[100];
   float             TotalTabs,Tabs[100],TotalTinj,Tinj[100],TotalPloss,util[100];
   unsigned long int TotalnumHops, TotalnumAbs,TotalnumSlots,TNFULLIN[100],totalABS, TotalnumIN;
   float             Pt[100];
   int n1,n2,n12,n21,n1dl,n2dl;

   TotalTabs     = 0;
   TotalTinj     = 0;
   TotalPloss    = 0;
   TotalnumHops  = 0;
   TotalnumSlots = 0;
   TotalnumAbs   = 0;
   TotalnumIN    = 0;
   TotalNDL      = 0;

   int TotalPacketsinFiber = 0;
   int TotalPacketsinDLs   = 0;
   int i,j,k;
   // Traverse each link
   for (i = 0; i <= NTW->n_links; i++)
      for (j = 1; j <= SM_Parameters->num_waves; j++)
         for (k = 1; k <= NTW->link_size[i]; k++)
         {
            if(NTW->source[i][j][k] != VirtualNode)
            {
            	TotalPacketsinFiber++;
            }
            //NTW->source[i][j][k]       = NTW->source[i][j][k+1];
            //NTW->destination[i][j][k]  = NTW->destination[i][j][k+1];
         }

   // Traverse each buffer
   int buffer, Pos_buffer, dest;

   for (ndr = 0; ndr < NTW->nbr_of_nodes; ndr++)
      for (Olink = 1; Olink <= NTW->output_links[ndr] + 1; Olink++)
      /* Olink assigment   */
      {
         //Banbuffer = 0;
         //Banlink   = 0;
         for (wave = 1 ; wave <= SM_Parameters->num_waves; wave++ )
         /* channel asigment  */
            for (buffer = 1; buffer <= NTW->NDL[ndr][Olink]; buffer++)
            /* Buffer assigment  */
            {
               Pos_buffer = Olink+(ndr) * max_links_per_node;
               if (NTW->OutBuffer[Pos_buffer][wave][buffer+1] == 1)
               {
                  // dest = NTW->Bsource[Pos_buffer][wave][1];
                  dest = NTW->connected_to[ndr][Olink];
                  if( (NTW->Bsource[Pos_buffer][wave][buffer] != VirtualNode) && (dest != ndr) )
                  {
            	     TotalPacketsinDLs++;
                  }
               }
            }
      }
   // End traverse each buffer

   for (ndr = 0; ndr < NTW->nbr_of_nodes; ndr++)
   {
      Tabs[ndr]       = 0;
      Tinj[ndr]       = 0;
      AveDpN[ndr]     = 0;
      AveApN[ndr]     = 0;
      util[ndr]       = 0;
      TNFULLIN[ndr]   = 0;
      Pt[ndr]         = 0;
      NDLperNODE[ndr] = 0;
   }

   for (ndr = 0; ndr < NTW->nbr_of_nodes; ndr++)
      for (linkin = 1; linkin <= NTW->input_links[ndr]; linkin++)
         for (wave = 1; wave <= SM_Parameters->num_waves; wave++)
         {
            fprintf (fileout,"node= %d , link= %d , wave= %d , numAbs= %d \n", ndr, linkin,wave, (int) NTW->numAbs[ndr][linkin][wave]);
            fprintf (fileout,"node= %d , link= %d , wave= %d , numFullIN= %d \n", ndr, linkin,wave, (int) NTW->numFullIN[ndr][linkin][wave]);
            //fprintf (fileout,"node= %d , link= %d , wave= %d , numHops= %d \n", ndr, linkin,wave,(int) NTW->numHops[ndr][linkin][wave]);
         }
   fprintf (fileout,"tempo (%d) \n", (int) NTW->tempo ) ;

   /*!> Initialization of utilization "u" */
   for (ndr = 0; ndr < NTW->nbr_of_nodes; ndr++)
   {
      for (linkin = 1; linkin <= NTW->input_links[ndr]; linkin++)
      {
         NTW->u[ndr][linkin] = 0.0;
      }
   }

   for (ndr = 0; ndr < NTW->nbr_of_nodes; ndr++)
      for (linkin = 1; linkin <= NTW->input_links[ndr]; linkin++)
         for (wave = 1; wave <= SM_Parameters->num_waves; wave++)
         {
            NTW->u[ndr][linkin] += ((float)NTW->numFullIN[ndr][linkin][wave]) / (NTW->tempo);
            NTW->a[ndr][linkin] += ((float)NTW->numAbs[ndr][linkin][wave])    / (NTW->tempo);
            if (NTW->numAbs[ndr][linkin][wave] != 0)
               //NTW->D[ndr][linkin] += ((float)NTW->numHops[ndr][linkin][wave])/(NTW->numAbs[ndr][linkin][wave]);

            //TotalnumSlots += NTW->numSlots[ndr][linkin][wave];
            //TotalnumHops  += NTW->numHops[ndr][linkin][wave];
            TotalnumAbs   += NTW->numAbs[ndr][linkin][wave];
            TNFULLIN[ndr] += NTW->numFullIN[ndr][linkin][wave];

            TotalnumIN += NTW->numInj[ndr][linkin][wave];

            //AveDpN[ndr]   += (float)NTW->numHops[ndr][linkin][wave];
            AveApN[ndr]   += (float)NTW->numAbs[ndr][linkin][wave];
         }

   AveD = 0;

   for (ndr = 0; ndr < NTW->nbr_of_nodes; ndr++)
   {
      for (linkin = 1; linkin <= NTW->input_links[ndr]; linkin++)
      {
         fprintf (fileout,"node= %d , link= %d , utilization= %9.7f \n", ndr, linkin,NTW->u[ndr][linkin]/SM_Parameters->num_waves);
         fprintf (fileout,"node= %d , link= %d , absorption= %9.7f \n", ndr, linkin,NTW->a[ndr][linkin]/SM_Parameters->num_waves);
         fprintf (fileout,"node= %d , link= %d , Delay-hops= %9.7f \n", ndr, linkin,NTW->D[ndr][linkin]/SM_Parameters->num_waves);

         if (linkin > 1)
            util[ndr] += NTW->u[ndr][linkin]/SM_Parameters->num_waves;

      }
      util[ndr] = util[ndr] / (NTW->input_links[ndr]-1);
      AveD      = AveD + AveDpN[ndr] / AveApN[ndr];
   }

   AveD = AveD / NTW->nbr_of_nodes;

   fprintf (fileout,"NETWORK Delays: D-Hops1= %9.7f D-Hops2= %9.7f D-Slots= %9.7f Ave_mesage= %9.7f \n",AveD, (float)TotalnumHops/TotalnumAbs,(float)TotalnumSlots/TotalnumAbs,NTW->AveMess/NTW->numMess);
   fprintf (fileout,"NETWORK TotalnumAbs = %d TotalnumIN = %d TPF = %d TPDLs = %d \n",TotalnumAbs,TotalnumIN,TotalPacketsinFiber,TotalPacketsinDLs);


   for (ndr = 0; ndr < NTW->nbr_of_nodes; ndr++)
      for (linkin = 1; linkin <= NTW->input_links[ndr]; linkin++)
         for (wave = 1; wave <= SM_Parameters->num_waves; wave++)
            Tabs[ndr] += ((float)NTW->numAbs[ndr][linkin][wave]) / (NTW->tempo);

   for (ndr = 0; ndr < NTW->nbr_of_nodes; ndr++)
      for (wave = 1; wave <= SM_Parameters->num_waves; wave++)
         Tinj[ndr] += ((float)NTW->numInj[ndr][1][wave]) / (NTW->tempo);

   for (ndr = 0; ndr < NTW->nbr_of_nodes; ndr++)
      for (AAdd = 1; AAdd <= 7; AAdd++)
         Pt[ndr] += (float)NTW->Pc[ndr][AAdd] / TNFULLIN[ndr];

   for (ndr = 0; ndr < NTW->nbr_of_nodes; ndr++)
   {
      fprintf (fileout,"ndr= %3d u= %7.5f P1= %5.3f P2= %5.3f P3= %5.3f Pt= %5.3f #MPR= %5d #LMPR= %3d Ta= %7.5f Td= %5.3f Ti= %7.5f \n",ndr, util[ndr],(float)NTW->Pc[ndr][1]/TNFULLIN[ndr],(float)NTW->Pc[ndr][2]/TNFULLIN[ndr],(float)NTW->Pc[ndr][3]/TNFULLIN[ndr],Pt[ndr], (int) NTW->numMPR[ndr], (int) NTW->numLossmpr[ndr],Tabs[ndr],(float)NTW->numPPL[ndr]/NTW->tempo,Tinj[ndr]);
      TotalTabs  += Tabs[ndr];
      TotalTinj  += Tinj[ndr];
      TotalPloss += (float)NTW->numPPL[ndr]/NTW->tempo;
   }

   fprintf (fileout,"TotalTabs= %9.7f , TotalPloss= %9.7f,(%9.7f), TotalTinj=%9.7f \n",TotalTabs,TotalPloss,TotalTabs+TotalPloss,TotalTinj);

   for (ndr = 0; ndr < NTW->nbr_of_nodes; ndr++)
      fprintf (fileout,"ndr= %3d D-Hops= %9.7f numPPL= %4d TNFULLIN= %7d PPL= %9.7f  \n",ndr,AveDpN[ndr]/AveApN[ndr],(int)NTW->numPPL[ndr],(int)TNFULLIN[ndr],(float)NTW->numPPL[ndr]/TNFULLIN[ndr]);

   for (ndr = 0; ndr < NTW->nbr_of_nodes; ndr++)
   {
      fprintf (fileout,"node= %d , input_links= %d , output_links= %d\n",ndr,NTW->input_links[ndr],NTW->output_links[ndr]);
      fprintf (fileout,"Olink \t Co_to \t #Fibs \t O-U \t \t \t B-U \t DroP \t  WCU \t WCUBA \t NDL \n");
      for (Olink = 1; Olink <= NTW->output_links[ndr]; Olink++)
      {
         fprintf (fileout,"%2d \t %2d \t %2d \t %6d \t %6d \t %3d \t %5d \t %5d \t %3d\n",
                  Olink, NTW->connected_to[ndr][Olink], NTW->num_of_fibers[ndr][NTW->connected_to[ndr][Olink]],
                  (int) NTW->OutlinkUsage[ndr][Olink],  (int) NTW->BufferUsage[ndr][Olink], (int) NTW->Drop_P_Link[ndr][Olink],
                  (int) NTW->WCUsage[ndr][Olink],       (int) NTW->WCUsageBA[ndr][Olink],   NTW->NDL[ndr][Olink]);
         NDLperNODE[ndr]  = NDLperNODE[ndr] + NTW->NDL[ndr][Olink];
         TotalNDL        += NTW->NDL[ndr][Olink];
      }
   }

   fprintf (fileout," # total of delay lines=%d, # total of fibers=%d \n", TotalNDL,NTW->n_fibers);

   //fprintf (fileout," hop #, numpackloss \n");
   //for (AAdd = 0 ; AAdd <= 30; AAdd++)
      //fprintf (fileout," %d %d \n", AAdd, (int)NTW->hop_distribution[AAdd]);

   totalABS = 0;
   //for (AAdd = 0 ; AAdd <= 50; AAdd++)
      //totalABS = totalABS + NTW->hop_absorption[AAdd];

   fprintf (fileout," hop #, num-absorption \n");
   //for (AAdd = 0 ; AAdd <= 30; AAdd++)
      //fprintf (fileout," %d %9.7f \n", AAdd, (float)NTW->hop_absorption[AAdd]/totalABS);

   fprintf (fileout," Total number of absorbed packets= %d  \n", (int)totalABS);

   fprintf (fileout," node Ave-U-per-wave inlets Max#WCactiveperslot outlets NDL   \n");
   for (ndr = 0; ndr < NTW->nbr_of_nodes; ndr++)
      fprintf (fileout,"%3d %7.5f %3d %3d %3d %3d \n",ndr, util[ndr],NTW->input_links[ndr], NTW->MaxWCactive[ndr],NTW->output_links[ndr],NDLperNODE[ndr]);

   for (ndr = 0; ndr < NTW->nbr_of_nodes; ndr++)
   {
      fprintf (fileout,"node= %d , input_links= %d , output_links= %d\n",ndr,NTW->input_links[ndr],NTW->output_links[ndr]);
      for (linkin = 1; linkin <= NTW->input_links[ndr]; linkin++)
         fprintf (fileout,"connected_from= %2d Link #= %3d # Input= %2d \n",
         NTW->connected_from[ndr][linkin],
         NTW->num_input_link[ndr][linkin],
         linkin);

      for (Olink = 1; Olink <= NTW->output_links[ndr]; Olink++)
      {
         fprintf (fileout,"Olink= %2d connected_to %2d #Fibs= %2d Link #= %3d \n",
                  Olink,
                  NTW->connected_to[ndr][Olink],
                  NTW->num_of_fibers[ndr][NTW->connected_to[ndr][Olink]],
                  NTW->num_output_link[ndr][Olink]);
      }
      fprintf (fileout,"\n");
   }

   fprintf (fileout,"\n ======== Fibers per link ========== \n");
   fprintf (fileout,"N1 N2 IN OUT TF N1DL N2DL TDL \n");
   for (ndr = 0; ndr < NTW->nbr_of_edges; ndr++)
   {
      n1  = NTW->consecutive_link[ndr][0];
      n2  = NTW->consecutive_link[ndr][1];
      n12 = NTW->num_of_fibers[n1][n2];
      n21 = NTW->num_of_fibers[n2][n1];
      n1dl = 0;
      n2dl = 0;
      for (Olink = 2; Olink <= NTW->output_links[n1]; Olink++)
      {
         if(NTW->connected_to[n1][Olink] == n2)
         {
            n1dl = n1dl + NTW->NDL[n1][Olink];
         }
      }

      for (Olink = 2; Olink <= NTW->output_links[n2]; Olink++)
      {
         if(NTW->connected_to[n2][Olink] == n1)
         {
            n2dl = n2dl + NTW->NDL[n2][Olink];
         }
      }
      fprintf (fileout,"%2d %2d %2d %2d %2d %2d %2d %2d\n",n1,n2,n12,n21,n12 + n21,n1dl,n2dl,n1dl + n2dl);
   }


   // ======================= Packet Reordering ======================
   fprintf(fileout,"\n ======= Real Values RD ===== ");
   int CountGlobalRD = 0;
   fprintf(fileout,"\n CountGlobalRD = %d ",CountGlobalRD);
   // Negatives
   //NTW->CountGlobalRD = 0;
   for(ndr = MaxBLength - 1; ndr >= 0; ndr--)
   {
	   //
	   fprintf(fileout,"\n %d", NTW->GlobalRDN[ndr]);
	   CountGlobalRD += NTW->GlobalRDN[ndr];
   }
   // Positives
   for(ndr = 0; ndr < MaxBLength; ndr++)
   {
	   //
	   fprintf(fileout,"\n %d ", NTW->GlobalRDP[ndr]);
	   CountGlobalRD += NTW->GlobalRDP[ndr];
   }
   fprintf(fileout,"\n ======= DONE Computing RD ===== ");
   fprintf(fileout,"\n ======= Computing RD ===== ");
   fprintf(fileout,"\n NTW->CountGlobalRD = %d ",CountGlobalRD);
   // Negatives
   for(ndr = MaxBLength -1; ndr >= 0; ndr--)
   {
	   //
	   fprintf(fileout,"\n %f",(float)NTW->GlobalRDN[ndr]/(float)CountGlobalRD);
   }
   // Positives
   for(ndr = 0; ndr < MaxBLength; ndr++)
   {
	   //
	   fprintf(fileout,"\n %f",(float)NTW->GlobalRDP[ndr]/(float)CountGlobalRD);
   }
   fprintf(fileout,"\n ======= DONE Computing RD ===== ");
}
/* ===========================================================================*/
/*                            Export Results                                  */
/* ===========================================================================*/
void Export_Results(struct NETWORK *NTW, struct SPARAMETERS *SM_Parameters)
{
   int ndr, TLossmpr, TnumPPL, TFullIN, TnumPPL_Failures;
	TnumPPL_Failures = 0;
   TLossmpr         = 0;
   TnumPPL          = 0;

   for (ndr = 0; ndr < NTW->nbr_of_nodes; ndr++)
   {
      TLossmpr         = TLossmpr         + NTW->numLossmpr[ndr];
      TnumPPL          = TnumPPL          + NTW->numPPL[ndr];
		TnumPPL_Failures = TnumPPL_Failures + NTW->numPPL_Failures[ndr];
   }
   fprintf(fileout,"%4d %7d %7d %7d\n", (int)NTW->tempo, TLossmpr, TnumPPL, TnumPPL_Failures);

}

/* ===========================================================================*/
/*                            Print Utilization                               */
/* ===========================================================================*/
void Print_Utilization(struct NETWORK *NTW, struct SPARAMETERS *SM_Parameters)
{
   int ndr, linkin, wave;

   /*!> Initialization of utilization "u" */
   for (ndr = 0; ndr < NTW->nbr_of_nodes; ndr++)
      for (linkin = 1; linkin <= NTW->input_links[ndr]; linkin++)
         NTW->u[ndr][linkin] = 0.0;


   for (ndr = 0; ndr < NTW->nbr_of_nodes; ndr++)
      for (linkin = 1; linkin <= NTW->input_links[ndr]; linkin++)
      {
         for (wave = 1; wave <= SM_Parameters->num_waves; wave++)
         {
            NTW->u[ndr][linkin] += ((float)NTW->numFullIN[ndr][linkin][wave]) / (NTW->tempo);
         }
         //            time,node,link,utilization
         fprintf(fUtilization,"%d %d %d %9.7f \n", (int)NTW->tempo, ndr, linkin, NTW->u[ndr][linkin]/SM_Parameters->num_waves);
      }

}
