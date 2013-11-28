
/* ========================================================================== */
/*           FORWARDING                                                       */
/* ========================================================================== */
void Forwarding(struct NETWORK *NTW)
{
   int ndr, count, dest,dest2,salida, Naddress, Naddress2;

   count = 1;
   for (Naddress = 1; Naddress <= NTW->num_outputs[0][1]; Naddress++)
      NTW->forw_output[0][1][Naddress] = Naddress + 1;     /* the fisrt dest is for */
                                                           /* output 2              */
   for (ndr = 1; ndr < NTW->nbr_of_nodes; ndr++)
      for (Naddress = 1; Naddress <= NTW->num_outputs[ndr][0]; Naddress++)
         NTW->forw_output[ndr][0][Naddress] = Naddress + 1;

   salida = 2;
   for(ndr = 0; ndr < NTW->nbr_of_nodes; ndr++)
   {
      if (ndr == 0)
         salida = NTW->num_outputs[0][1]+1;
      else
         salida = NTW->num_outputs[ndr][0]+1;

      for (dest = 1; dest < NTW->nbr_of_nodes; dest++)
         for (Naddress = 1; Naddress <= NTW->num_outputs[ndr][dest]; Naddress++)
         {
            for (dest2 = 0; dest2 <= dest; dest2++ )
               for (Naddress2 = 1; Naddress2 <= NTW->num_outputs[ndr][dest2]; Naddress2++)
                  if  ( (dest != dest2) && (dest2 != ndr) )
                     if( (NTW->forw_table[ndr][dest][Naddress] == NTW->forw_table[ndr][dest2][Naddress2]) && (count == 1))
                     {
                        count = 10;
                        NTW->forw_output[ndr][dest][Naddress] = NTW->forw_output[ndr][dest2][Naddress2];
                        count = 10;
                     }

            if ( (ndr == 0) && (dest == 1) )
               count = 10;

            if ( (count == 1) && (dest != ndr) )
            {
               salida++;
               NTW->forw_output[ndr][dest][Naddress] = salida;
            }
            count = 1;
         }
   }

   /*************************  Printing forwarding table  *********************/

   fprintf (fscreen,"ndr, dest, num_outputs, forw_table, forw_output \n");
   for (ndr = 0; ndr < NTW->nbr_of_nodes; ndr++)
      for (dest = 0; dest < NTW->nbr_of_nodes; dest++)
         if  (dest != ndr)
         {
            fprintf (fscreen,"%d %d %d  |", ndr, dest, NTW->num_outputs[ndr][dest]);
            for (Naddress = 1; Naddress <= NTW->num_outputs[ndr][dest]; Naddress++)
               fprintf (fscreen,"%d  ", NTW->forw_table[ndr][dest][Naddress]);
            fprintf (fscreen,"    | ");
            for (Naddress = 1; Naddress <= NTW->num_outputs[ndr][dest]; Naddress++)
               fprintf (fscreen,"%d  ", NTW->forw_output[ndr][dest][Naddress]);
            fprintf (fscreen," \n ");
         }

   /*************************  Printing forwarding table  *********************/
}                      /* end of FORWARDING   */

/* ===========================================================================*/
/*                      MORE forwarding                                       */
/*============================================================================*/
/* This subroutine allocate Packets in the output links                       */
void   More_forwarding(struct NETWORK *NTW)
{
   int ndr,dest,Naddress,next_node;
   for (ndr=0; ndr< NTW->nbr_of_nodes; ndr++)
      for (dest=0; dest< NTW->nbr_of_nodes; dest++)
         if  (dest != ndr)
         {
            for (Naddress=1; Naddress<= NTW->num_outputs[ndr][dest]; Naddress++)
            {
               next_node = NTW->forw_table[ndr][dest][Naddress];
               NTW->num_output_link[ndr][NTW->forw_output[ndr][dest][Naddress]] = NTW->num_of_link[ndr][next_node][NTW->num_of_fibers[ndr][next_node]];
               NTW->connected_to[ndr][NTW->forw_output[ndr][dest][Naddress]] = next_node;
            }
         }
}

/* -------------------------------------------------------------------------- */
/*                                MEMORIAS                                    */
/* -------------------------------------------------------------------------- */

memorias(struct NETWORK *NTW, struct SPARAMETERS *SM_Parameters)
{

   int ndr, Olink, wave, AAdd;

   for (ndr = 0; ndr < NTW->nbr_of_nodes; ndr++)
      for (Olink = 1; Olink <= max_links_per_node; Olink++)
	   //for (Olink = 1; Olink <= NTW->output_links[ndr]; Olink++)
         NTW->NDL[ndr][Olink] = SM_Parameters->num_predefined_DL;
}

/* ========================================================================== */
/*           NEW FORWARDING                                                       */
/* ========================================================================== */
void New_Forwarding(struct NETWORK *NTW)
{
   int ndr, count, dest,dest2,salida, Naddress, Naddress2;

   count = 1;
   for (Naddress = 1; Naddress <= NTW->num_outputs[0][1]; Naddress++)
      NTW->new_foutput[0][1][Naddress] = Naddress + 1;     /* the fisrt dest is for */
                                                           /* output 2              */
   for (ndr = 1; ndr < NTW->nbr_of_nodes; ndr++)
      for (Naddress = 1; Naddress <= NTW->new_outputs[ndr][0]; Naddress++)
         NTW->new_foutput[ndr][0][Naddress] = Naddress + 1;

   salida = 2;
   for(ndr = 0; ndr < NTW->nbr_of_nodes; ndr++)
   {
      if (ndr == 0)
         salida = NTW->new_outputs[0][1]+1;
      else
         salida = NTW->new_outputs[ndr][0]+1;

      for (dest = 1; dest < NTW->nbr_of_nodes; dest++)
         for (Naddress = 1; Naddress <= NTW->new_outputs[ndr][dest]; Naddress++)
         {
            for (dest2 = 0; dest2 <= dest; dest2++ )
               for (Naddress2 = 1; Naddress2 <= NTW->new_outputs[ndr][dest2]; Naddress2++)
                  if  ( (dest != dest2) && (dest2 != ndr) )
                     if( (NTW->new_ftable[ndr][dest][Naddress] == NTW->new_ftable[ndr][dest2][Naddress2]) && (count == 1))
                     {
                        count = 10;
                        NTW->new_foutput[ndr][dest][Naddress] = NTW->new_foutput[ndr][dest2][Naddress2];
                        count = 10;
                     }

            if ( (ndr == 0) && (dest == 1) )
               count = 10;

            if ( (count == 1) && (dest != ndr) )
            {
               salida++;
               NTW->new_foutput[ndr][dest][Naddress] = salida;
            }
            count = 1;
         }
   }

   /*************************  Printing new forwarding table  *********************/
   fprintf (fscreen," NEW ROUTES \n");
   fprintf (fscreen,"ndr, dest, num_outputs, forw_table, forw_output \n");
   for (ndr = 0; ndr < NTW->nbr_of_nodes; ndr++)
      for (dest = 0; dest < NTW->nbr_of_nodes; dest++)
         if  (dest != ndr)
         {
            fprintf (fscreen,"%d %d %d  |", ndr, dest, NTW->new_outputs[ndr][dest]);
            for (Naddress = 1; Naddress <= NTW->new_outputs[ndr][dest]; Naddress++)
               fprintf (fscreen,"%d  ", NTW->new_ftable[ndr][dest][Naddress]);
            fprintf (fscreen,"    | ");
            for (Naddress = 1; Naddress <= NTW->new_outputs[ndr][dest]; Naddress++)
               fprintf (fscreen,"%d  ", NTW->new_foutput[ndr][dest][Naddress]);
            fprintf (fscreen," \n ");
         }

   /*************************  Printing forwarding table  *********************/
}                      /* end of FORWARDING   */


