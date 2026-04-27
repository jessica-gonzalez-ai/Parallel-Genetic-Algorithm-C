#include <sys/time.h>
#include <unistd.h>
#include "mt.h"

///////////GENERACION DE ALEATORIOS///
#define UNI mt_get_pos(&prng)              //(0,1)
#define VNI (1-2*UNI)                   //(-1,1)
#define NNI(n) (n*VNI)                  //(-n,n)

//ENTEROS
#define ZNM(n) (mt_get(&prng)%n)           //[0,N-1]
#define OTN(n) (ZNM(n)+1)               //[1,N]
//Inicialización de la semilla aleatoria

mt_state_t prng;

double mt_get_pos(mt_state_t *prng)
{
    double x;
    do {
        x=mt_get_double(prng);
    } while(x==0.0d);
    return x;
}

void inirndm_msec(void)
{
    struct timeval tv;
    struct timezone tz;
    gettimeofday(&tv,&tz);
    mt_set(&prng,tv.tv_usec);
}

void pinirndm(void)
{
    mt_set(&prng,getpid());
}

void inirndm(void)
{
   unsigned long seed;
    FILE *f=fopen("/dev/random","rb");
    fread(&seed,sizeof(seed),1,f);
    mt_set(&prng,seed);
}

void inirndm_seed(unsigned long seed)
{
    mt_set(&prng,seed);
}


