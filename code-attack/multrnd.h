#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int   multrnd(int n, float p[10], int inc);
float sump(int n, float p[]);
void  cumsum(int n, float p[10], float s[10]);
float generar_aleatorio(int inc);
float Maximo(int n, float S[10]);
int Encuentra_Maximo(int n, float S[10]);

int multrnd(int n, float p[10], int inc)
{
   float  P,s[10],r;
   int    i,o,seed;

   //P = sum(p)
   P = sump(n,p);

   if( (1.0 - P) <= 0.0005)
   {
      // s = cumsum(p/P)
      cumsum(n,p,s);
      // r = rand(1,n)
      //inc = rand();
      r = generar_aleatorio(inc);
      //r = RndReal(0);
      // o = o + (r > s(i));
      o    = 1;
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

void cumsum(int n, float p[10], float s[10])
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

float generar_aleatorio(int inc)
{
   /*time_t rawtime;
   struct tm * timeinfo;
   float r;
   int seed;

   time ( &rawtime );
   timeinfo = localtime ( &rawtime );
   seed = timeinfo->tm_sec + inc;
   srand(seed);
  */
   float r;
   //srand(time(NULL)*inc); //* inc;
   //r = (   (float)rand() / ((float)(RAND_MAX)+(float)(1)) );
   r = (float) (   (double)rand() / ((double)(RAND_MAX)+(double)(1)) );
   return r;
   //return (float) (   (double)rand() / ((double)(RAND_MAX)+(double)(1)) );
}

float Maximo(int n, float S[10])
{
   int   i,max;
   max = 0;
   for(i = 0; i < n; i++)
      if(S[i] > S[max])
         max = i;
   return S[max];
}

int Encuentra_Maximo(int n, float S[10])
{
   int   i,max;
   max = 0;
   for(i = 0; i < n; i++)
      if(S[i] > S[max])
         max = i;
   return max;
}
