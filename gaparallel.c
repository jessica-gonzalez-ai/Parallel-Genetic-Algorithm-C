#include <stdio.h>
#include <string.h>
#include "alea.h"
#include <pthread.h>

#define Objetivo "Muchos anios despues, frente al peloton de fusilamiento el Coronel Aureliano Buendia habria de recordar aquella remota tarde en que su padre lo llevo a conocer el hielo."
#define TC 169
#define TP 512 //ya que 512/8 es 64
#define GMAX 1000
#define PM 0.2

//Hilos!!
#define THREADS 8

typedef struct {
    char cromosoma[TC+1];
    int  aptitud;
} individuo;

individuo pob[TP];
individuo pobsig[TP];
int aptitud(char *s);

void * evaluar_aptitud(void *arg)
{
    long int id = (long int) arg;
    int pedazo = TP / THREADS;
    int inicio = id * pedazo;
    int fin = inicio + pedazo;

    //El hilo solo evalúa su pedazo
    for(int i = inicio; i < fin; i++)
    {
        pobsig[i].aptitud = aptitud(pobsig[i].cromosoma);
    }
    return NULL;
}

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

individuo ini_pob(individuo *p)
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
        psig[j] = (p[i].aptitud<p[i+1].aptitud) ? p[i+1]:p[i]; //está linea es pro
    }
}
individuo cruza(individuo *p) //quite individuo m
{
    //individuo mejor=m;
    individuo h1; //quite h2
    int i,pc,p1,p2;
    for(i=TP/2; i<TP; i++)
    {
        p1 = ZNM((TP/2));
        do {
            p2 = ZNM((TP/2));
        } while(p1==p2);
        pc = ZNM(TC);
        strncpy(h1.cromosoma,p[p1].cromosoma,pc);
        //strncpy(h2.cromosoma,p[p2].cromosoma,pc);
        strcpy(h1.cromosoma+pc,p[p2].cromosoma+pc);
        //strcpy(h2.cromosoma+pc,p[p1].cromosoma+pc);
        //h1.aptitud = aptitud(h1.cromosoma);
        //h2.aptitud = aptitud(h2.cromosoma);
        //p[i] = (h1.aptitud<h2.aptitud) ? h2:h1;
        //if(p[i].aptitud > mejor.aptitud)
        //{
        //mejor = p[i];
        //}

        p[i]= h1;
    }
}
individuo mutacion(individuo *p) //quite individuo m
{
    int i;
    int gen;
    //individuo mejor =m;
    for(i=0; i<TP; i++)
    {
        if(UNI < PM)
        {
            gen = ZNM(TC);
            p[i].cromosoma[gen]=32 + ZNM(95);
            //p[i].aptitud = aptitud(p[i].cromosoma);
            /*if(p[i].aptitud > mejor.aptitud)
            {
            	mejor=p[i];
            } */
        }
    }
    //return mejor;
}
int main()
{
    individuo *p=pob, *psig=pobsig, *aux; //de aquí quite (individuo pob[TP], pobsig[TP],) para que los hilos lo lean
    individuo mejor;

    //hilos!
    pthread_t t[THREADS];
    mejor = ini_pob(pob);
    despliega(mejor);
    for(int gen=0; gen<GMAX && mejor.aptitud<TC; gen++)
    {
        seleccion(p,psig);
        cruza(psig);
        mutacion(psig);

        for(long int i=0; i<THREADS; i++)
        {
            pthread_create(&t[i],NULL,evaluar_aptitud,(void*)i);
        }

        for(int i=0; i<THREADS; i++)
        {
            pthread_join(t[i],NULL);
        }

        mejor= psig[0];
        for(int i=1; i<TP; i++)
        {
            if(psig[i].aptitud > mejor.aptitud)
            {
                mejor=psig[i];
            }
        }

        aux=p;
        p=psig;
        psig=aux;
        printf("Generación: %d ",gen+1);
        despliega(mejor);
    }
    despliega(mejor);
    return 0;
}
