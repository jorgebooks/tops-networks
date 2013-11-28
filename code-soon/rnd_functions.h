/* ========================================================================== */
/* -------------------------------------------------------------------------- */
/*               RANDOM NUMBER GENERATOR SEEDS                                */
/* -------------------------------------------------------------------------- */
/* ========================================================================== */
#define  m1   259200            /*      Costanti per il gen. di num. rnd      */
#define  m2   144456
#define  m3   243000
#define  ia1  7141
#define  ia2  8121
#define  ia3  4561
#define  ic1  54773
#define  ic2  28411
#define  ic3  51349
#define  rm1  1.0/m1
#define  rm2  1.0/m2

/* ========================================================================== */
/* ----                 RANDOM NUMBER GENERATOR VARIABLES                ---- */
/* ========================================================================== */
float   r[98];
int     iff;
int     ix1,ix2,ix3;

int   multrnd(int n, float p[30], int inc);
float sump(int n, float p[]);
void  cumsum(int n, float p[30], float s[30]);
float Maximo(int n, float S[30]);
int Encuentra_Maximo(int n, float S[30]);
int Find_action(int n, float p[30], float ep, int inc);

/* ========================================================================== */
/* -------------------------------------------------------------------------- */
/*                RndReal                                                     */
/* -------------------------------------------------------------------------- */
/* REAL RANDOM NUMBER GENERATOR FROM (0,1)                                    */

float   RndReal(int idum)
{
   return (float) (   (double)rand() / ((double)(RAND_MAX)+(double)(1)) );
}           /* end RndReal */

/* -------------------------------------------------------------------------- */
/*                               Rnd                                          */
/* -------------------------------------------------------------------------- */
/*  Questa funzione genera un numero casuale intero compreso tra il
    valore minimo [min] e il valore massimo [max] specificati.                */
int  Rnd(int min,int max)
{
   // [min, max) (including min, but strictly less than max)
   //srand(0);
   return (int) (max * (rand() / ((double)RAND_MAX + 1))) + min;
   //return (int)(min+(max-min+1)*RndReal(0)); //X * (rand() / ((double)RAND_MAX + 1))
}  /* end Rnd */

void KnuthShuffle(int* pArr,int N)
{
       int r,i,tmp;
       for(i=N-1;i>=0;i--)
       {
               r=rand()%N;
               tmp=pArr[i];
               pArr[i]=pArr[r];
               pArr[r]=tmp;

       }
}

// http://www.comp.dit.ie/rlawlor/Alg_DS/sorting/quickSort.c

void quickSort( float a[], int l, int r, int ind[])
{
   int j;

   if( l < r )
   {
      // divide and conquer
       j = partition( a, l, r, ind);
       quickSort( a, l, j-1, ind);
       quickSort( a, j+1, r, ind);
   }

}

int partition( float a[], int l, int r, int ind[])
{
   int i,j;
   float pivot, t;
   int   index, Z;

   pivot = a[l];
   index = ind[l];
   i = l; j = r+1;

   while( 1)
   {
      do ++i; while( a[i] <= pivot && i <= r );
      do --j; while( a[j] > pivot );
      if( i >= j ) break;
      t    = a[i];
      a[i] = a[j];
      a[j] = t;

      Z      = ind[i];
      ind[i] = ind[j];
      ind[j] = Z;

   }
   t    = a[l];
   a[l] = a[j];
   a[j] = t;

   Z      = ind[l];
   ind[l] = ind[j];
   ind[j] = Z;

   return j;
}



/* ==============================================================*/
/*                     RandomSeriesFW                            */
/*===============================================================*/
/* This subroutine generate uniform random series for F and W    */
void RandomSeriesFW (int n, int m, int *F, int *W, int wave, struct NETWORK *NTW, int ndr, int dest, int MyPriority)
{
   int ch, ca, CA;//, num_options;
   float Options[max_num_node + 1];
   float P[max_num_node + 1], MyMax, MyMin;
   int   Indexes[max_num_node + 1];
   //int at;

   if( ( (NTW->Phase <= 4) && (dest != ndr) ) ||
	   (MyPriority == 1) // Packet with Priority equal to 1 always use MPR strategy
	 )
   {
	   for (ch = 0; ch < max_links_per_node + 1; ch++)
	   {
		   F[ch] = 0;
	   }

      // Fibers
      for(ch = 0; ch < max_num_node + 1; ch++)
      {
         Options[ch] = 0;
      	 Indexes[ch] = 0;
      }
      Options[ch] = 999;

      for(ch = 1; ch <= n; ch++)
      {
         Options[ch] = NTW->u[ndr][NTW->forw_output[ndr][dest][ch]];
         Indexes[ch] = ch; //NTW->forw_output[ndr][dest][ch];
      }
      //quickSort(Options,0,num_options,Indexes);

      // Fibers
	  for( ch = 1 ; ch <= n; ch++ )
         F[ch] = Indexes[ch];

      // Wavelenghts
      for( ch = 1 ; ch <= m; ch++ )
         W[ch] = Rnd(1,m);

      for( ch = 2 ; ch <= m; ch++ )
      {
         ca = 1;
         while(ca != ch)
            if (W[ca]==W[ch])
            {
               while (W[ch] == W[ca])
                  W[ch] = Rnd(1,m);
               ca = 1;
            }
            else
               ca = ca + 1;
      }

      // Move wave to the first position
      if(W[1] != wave)
      {
         ca = W[1];
         for(ch =2; ch <= m; ch++)
         {
            if(W[ch] == wave)
            {
               W[ch] = ca;
               W[1]  = wave;
               break;
            }
         }
      }
   }
   else
   {
      if( (NTW->Phase <= 6) && (dest != ndr)  )
      {
         // Determine which fibers can be used to send packets from ndr to dest.
         //num_options = NTW->num_outputs[ndr][dest];
    	 NTW->Myat = 99;

  	     for (ch = 0; ch < max_links_per_node + 1; ch++)
  	     {
  		    F[ch] = 0;
  	     }

         for(ch = 0; ch < max_num_node + 1; ch++)
         {
        	 Options[ch] = 0;
        	 Indexes[ch] = 0;
         }
         Options[ch] = 999;
         MyMax = -100;
         for(ch = 1; ch <= n; ch++)
         {
            //Options[ch] = NTW->u[ndr][NTW->forw_output[ndr][dest][ch]] * NTW->MyPolicy [ndr][dest][ch];
        	if(NTW->MyPolicy [ndr][dest][ch] >= 0.01)
        	   Options[ch] = NTW->MyPolicy [ndr][dest][ch];
        	else
        	   Options[ch] = 0.01;

        	if(Options[ch] > MyMax)
            	MyMax = Options[ch];
            Indexes[ch] = ch; //NTW->forw_output[ndr][dest][ch];
         }
         quickSort(Options,0,n,Indexes);
         MyMin = Options[1];

         for(ch = 0; ch < n; ch++)
        	 P[ch] = (Options[ch + 1] - MyMin) / (MyMax - MyMin);
        	 //P[ch] = (MyMax - Options[ch + 1]) / (MyMax - MyMin);

         // Fibers
	     for( ch = 1 ; ch <= n; ch++ )
            F[ch] = Indexes[ch];

         //if(NTW->Phase < 6)
         //{
	        if(NTW->Phase == 6)
	           NTW->Myat = Find_action(n, P, EpsilonRL / 100, 1);
	        else
        	   NTW->Myat = Find_action(n, P, EpsilonRL, 1); //(int)(NTW->tempo*wave*dest*ndr));

	        if(NTW->Myat >= n)
            {
           	    printf("\n WE HAVE A HUGE ERROR ERROR ERROR ??????????????????????");
            }

	        if( NTW->Myat == (n-1) )
            {
	        	// NO NEED TO CHANGE ANYTHING,
           	    //printf("\n THE BEST OPTION WAS THE HIGHEST??? ");
           	    NTW->Num_First++;
            }
	        else
	        {
	        	// We need to change Indexes
	        	NTW->Myat++;
	        	ca = n;
	        	CA = NTW->Myat;
	        	while(ca > NTW->Myat)
	        	{
	        		ch    = F[ca];
	        		F[ca] = F[CA];
	        		F[CA] = ch;
	        		//CA--;
	        		ca--;
	        	}

	        	//ch           = F[n];
	            //F[n]         = F[NTW->Myat];
	            //F[NTW->Myat] = ch;

	            NTW->Num_at++;
	        }

	        // Invert F
	        for(ch = 1; ch <= n; ch++)
	        {
	        	Options[ch] = F[ch];
	        }

	        for(ch = 1; ch <= n; ch++)
	        {
	        	F[ch]       = Options[ (n+1) - ch];
	        	Indexes[ch] = F[ch];
	        }

         //}

         // Wavelenghts
         for( ch = 1 ; ch <= m; ch++ )
            W[ch] = Rnd(1,m);

         for( ch = 2 ; ch <= m; ch++ )
         {
            ca = 1;
            while(ca != ch)
               if (W[ca]==W[ch])
               {
                  while (W[ch] == W[ca])
                     W[ch] = Rnd(1,m);
                  ca = 1;
               }
               else
                  ca = ca + 1;
         }

         // Move wave to the first position
         if(W[1] != wave)
         {
            ca = W[1];
            for(ch =2; ch <= m; ch++)
            {
               if(W[ch] == wave)
               {
                  W[ch] = ca;
                  W[1]  = wave;
                  break;
               }
            }
         }

         //Check F
         /*printf("\n CHECKING_F : ");
         for( ch = 1 ; ch <= n; ch++ )
         {
        	 if(F[ch] == 0)
        	 {
        		 printf("\n WRONG_F_CHECK_IT");
        	 }
         }*/
      }
      else
      {
         // Here we use the policy
         // Testing
         // Fibers
         //num_options = NTW->num_outputs[ndr][dest];

         for(ch = 0; ch < max_num_node + 1; ch++)
         {
          Options[ch] = 0;
          Indexes[ch] = 0;
         }
         Options[ch] = 999;

         for(ch = 1; ch <= n; ch++)
         {
            Options[ch] = NTW->u[ndr][NTW->forw_output[ndr][dest][ch]];
            Indexes[ch] = NTW->forw_output[ndr][dest][ch];
         }
         //quickSort(Options,0,n,Indexes);

         // Fibers
        for( ch = 1 ; ch <= n; ch++ )
            F[ch] = Indexes[ch];

         // Wavelenghts
         for( ch = 1 ; ch <= m; ch++ )
            W[ch] = Rnd(1,m);

         for( ch = 2 ; ch <= m; ch++ )
         {
            ca = 1;
            while(ca != ch)
               if (W[ca]==W[ch])
               {
                  while (W[ch] == W[ca])
                     W[ch] = Rnd(1,m);
                  ca = 1;
               }
               else
                  ca = ca + 1;
         }

         // Move wave to the first position
         if(W[1] != wave)
         {
            ca = W[1];
            for(ch =2; ch <= m; ch++)
            {
               if(W[ch] == wave)
               {
                  W[ch] = ca;
                  W[1]  = wave;
                  break;
               }
            }
         }
         // End Testing
      }
   }
}                          /* END RANDOMSERIES */


// ========================================= ROUTINES RELATED TO MULTRND =========================================
int multrnd(int n, float p[30], int inc)
{
   float  P,s[30],r;
   int    i,o,seed;

   //P = sum(p)
   P = sump(n,p);

   if(abs(P-1.0) <= 0.01)
   {
      o    = 1;
      // s = cumsum(p/P)
      cumsum(n,p,s);
      // r = rand(1,n)
      //inc = rand();
      r = RndReal(inc);
      // o = o + (r > s(i));
      for(i = 0; i < n; i++)
      {
         if(r > s[i])
            o = o + 1;
      }
   }
   return(o-1);
}

float sump(int n, float p[])
{
   int   i;
   float P = 0.0;

   for(i = 0; i< n; i++)
      P    = P + p[i];
   return P;
}

void cumsum(int n, float p[30], float s[30])
{
   int   i;
   float acum;
   acum = 0;
   for(i = 0; i < n; i++)
   {
      s[i] = p[i] + acum;
      acum = s[i];
   }
}

float Maximo(int n, float S[30])
{
   int   i,max;
   max = 0;
   for(i = 0; i < n; i++)
      if(S[i] > S[max])
         max = i;
   return S[max];
}

int Encuentra_Maximo(int n, float S[30])
{
   int   i,max;
   max = 0;
   for(i = 0; i < n; i++)
      if(S[i] > S[max])
         max = i;
   return max;
}

int Find_action(int n, float p[30], float ep, int inc)
{
   int index, i, at;
   float PSum;
   float newp[30];

   index = Encuentra_Maximo(n,p);
   PSum = sump(n,p);

   for(i = 0; i < n; i++)
   {
      newp[i] = (ep/n);
   }
   newp[index] = 1-(n-1)*(ep/n);

   at = multrnd(n,newp,inc);
   return at;
}

