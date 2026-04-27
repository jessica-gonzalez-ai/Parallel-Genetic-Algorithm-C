#include <stdio.h>
#include <string.h>
#include <mpi.h>
#include "alea.h"


#define Objetivo "Muchos anios despues, frente al peloton de fusilamiento el Coronel Aureliano Buendia habria de recordar aquella remota tarde en que su padre lo llevo a conocer el hielo."
#define TC 169
#define TP 500
#define GMAX 1000
#define PM 0.2

typedef struct {
   char cromosoma[TC+1];
   int  aptitud;
} individuo;

int aptitud(char *s)
{
   int i=0;
   int ap=0;
   while(s[i]!='\0')
   {
      if(s[i]==Objetivo[i])
      {
         ap++;
      }
      i++;
   }
   return ap;
}

individuo  ini_pob(individuo *p)
{
   int i,j;
   individuo mejor;
   inirndm();
   mejor.aptitud = -1;
   for(i=0; i<TP; i++)
   {
      for(j=0; j<TC; j++)
      {
         p[i].cromosoma[j] = 32 + ZNM(95);
      }
      p[i].cromosoma[TC] = '\0';
      p[i].aptitud = aptitud(p[i].cromosoma);
      if(mejor.aptitud < p[i].aptitud)
      {
         mejor = p[i];
      }
   }
   return mejor;
}

void despliega(individuo indi)
{
   printf ("Cadena: %s Aptitud: %d\n",indi.cromosoma, indi.aptitud);
}

void seleccion(individuo *p,individuo *psig)
{
   int i,j;
   for(i=0,j=0; i<TP; i+=2,j++)
   {
      psig[j] = (p[i].aptitud<p[i+1].aptitud) ? p[i+1]:p[i];
   }
}

individuo cruza(individuo *p, individuo m)
{
   individuo mejor=m;
   individuo h1,h2;
   int i,pc,p1,p2;

   for(i=TP/2; i<TP; i++)
   {
      p1 = ZNM((TP/2));
      do {
         p2 = ZNM((TP/2));
      } while(p1==p2);
      pc = ZNM(TC);
      strncpy(h1.cromosoma,p[p1].cromosoma,pc);
      strncpy(h2.cromosoma,p[p2].cromosoma,pc);
      strcpy(h1.cromosoma+pc,p[p2].cromosoma+pc);
      strcpy(h2.cromosoma+pc,p[p1].cromosoma+pc);
      h1.aptitud = aptitud(h1.cromosoma);
      h2.aptitud = aptitud(h2.cromosoma);
      p[i] = (h1.aptitud<h2.aptitud) ? h2:h1;
      if(p[i].aptitud > mejor.aptitud)
      {
         mejor = p[i];
      }
   }

   return mejor;
}

individuo mutacion(individuo *p, individuo m)
{
	int i;
	int gen; 
	individuo mejor =m;
	for(i=0;i<TP;i++)
	{
		if(UNI < PM)
		{
			gen = ZNM(TC);
			p[i].cromosoma[gen]=32 + ZNM(95);
			p[i].aptitud = aptitud(p[i].cromosoma);
			if(p[i].aptitud > mejor.aptitud)
			{
				mejor=p[i];
			}
		}
	}
	return mejor;
}

int main(int argc, char **argv)
{
   individuo pob[TP], pobsig[TP], *p=pob, *psig=pobsig, *aux;
   individuo mejor;
   int gen; 
   int rank; 
   
   MPI_Init(&argc,&argv); 
   MPI_Comm_rank(MPI_COMM_WORLD,&rank); 
   MPI_Comm_size(MPI_COMM_WORLD,&size); 

   mejor = ini_pob(pob);
   //despliega(mejor);
   for(int gen=0; gen<GMAX && mejor.aptitud<TC; gen++)
   {
      seleccion(p,psig);
      mejor=cruza(psig, mejor);
      mejor=mutacion(psig,mejor);
      aux=p;
      p=psig;
      psig=aux;
      //printf("Generación: %d ",gen+1);
      //despliega(mejor);
   }
    printf("Generaciòn: %d", gen);  
   	despliega(mejor);
   MPI_Finalize();
   return 0;
}




