#include <pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include <sys/time.h>

//cantidad de hilos
int canthilos;
// tamanio de la matriz
int N;
// maximo numero generado en cada posicion
int nRandom = 50;
// matrices
double *A,*B,*AB,*C,*D;
// cada arreglo en su ultima posicion tendra una posicion exlusiva para calcular su contenido(avg,max,min)
double *maxA,*maxB,*maxC,*minA,*minB,*minC,*totalA,*totalB,*totalC;
float escalar;
typedef struct{ 
  	pthread_mutex_t mutex;
  	pthread_cond_t condicion;
  	int count;
} barrier_struct;
barrier_struct mean_barrier; //Barrera esperar al escalador.

//Inicializacion de la barrera.
void init_barrier(barrier_struct *barrier){ 
  	barrier-> count = 0;
  	pthread_mutex_init(&(barrier-> mutex), NULL);
  	pthread_cond_init(&(barrier-> condicion), NULL);
}

//Implementacion de la barrera.
void barrier(barrier_struct *barrier){
	pthread_mutex_lock(&(barrier -> mutex));
		barrier-> count ++;
    // printf("contando barrera! %d\n", barrier -> count );
		if (barrier -> count == canthilos){ 
			barrier -> count = 0;     
			pthread_cond_broadcast(&(barrier -> condicion));
		}
		else
			pthread_cond_wait(&(barrier -> condicion),&(barrier -> mutex));
	pthread_mutex_unlock(&(barrier -> mutex));
}


// HILO QUE CALCULA EL PROMEDIO Y MAXIMOS,MINIMOS
void *computarExpresionEscalar(void *id) {
    int i,j,k;
    int threadId = (int) (__intptr_t) id;
    int inicio = threadId * (N/canthilos);
    int fin = inicio + (N/canthilos);
    //RECORRO A   
    for( i = inicio ; i < fin ; i++){
      for( j = 0; j < N; j++){
        totalA[threadId] = totalA[threadId] + A[i*N+j];        
        if(A[i*N+j] > maxA[threadId]) maxA[threadId] = A[i*N+j] ;
        if(A[i*N+j] < minA[threadId]) minA[threadId] = A[i*N+j] ;
      }
    }
    //RECORRO B
    for( i = inicio ; i < fin ; i++){
      for( j = 0; j < N; j++){
        totalB[threadId] += B[i*N+j];
        if(B[i*N+j] > maxB[threadId]) maxB[threadId] = B[i*N+j] ;
        if(B[i*N+j] < minB[threadId]) minB[threadId] = B[i*N+j] ;
      }
    }
    //RECORRO C
    for( i = inicio ; i < fin ; i++){
      for( j = 0; j < N; j++){
        totalC[threadId] += C[i*N+j];
        if(C[i*N+j] > maxC[threadId]) maxC[threadId] = C[i*N+j] ;
        if(C[i*N+j] < minC[threadId]) minC[threadId] = C[i*N+j] ;
      }
    }
    // A.B
    for( i=inicio;i<fin;i++){
        for(j=0;j<N;j++){
            for(k=0;k<N;k++){
                AB[i*N+j]= A[i*N+k] * B[k+j*N];
            }
        }
    }   
    // D = AB.C
    for(i=inicio;i<fin;i++){
        for(j=0;j<N;j++){
            for(k=0;k<N;k++){
                D[i*N+j]= AB[i*N+k] * C[k+j*N];
            }
        }
    }

    // EL HILO 0, RECORRE LO GENERADO POR LO HILOS Y CALCULA LOS AVG,MIN,MAX
    if( threadId == 0){
      for(i = 0; i < canthilos; i++){
        totalA[canthilos] += totalA[i];
        totalB[canthilos] += totalB[i];
        totalC[canthilos] += totalC[i];
        if( minA[canthilos] > minA[i] ) minA[canthilos] = minA[i];
        if( minB[canthilos] > minB[i] ) minB[canthilos] = minB[i];
        if( minC[canthilos] > minC[i] ) minC[canthilos] = minC[i];
        if( maxA[canthilos] < maxA[i] ) maxA[canthilos] = maxA[i];
        if( maxB[canthilos] < maxB[i] ) maxB[canthilos] = maxB[i];
        if( maxC[canthilos] < maxC[i] ) maxC[canthilos] = maxC[i];
        }
        // printf("El elegido! %d\n", threadId );
        // printf("Promedio A: %d\n", totalA[canthilos]/(N*N) );
        // printf("Promedio B: %d\n", totalB[canthilos]/(N*N) );
        // printf("Promedio C: %d\n", totalC[canthilos]/(N*N) );
        // printf("Mi minimoo total A %d\n", minA[canthilos]);
        // printf("Mi maximo total A %d\n", maxA[canthilos]);
        // printf("Mi minimoo total B %d\n", minB[canthilos]);
        // printf("Mi maximo total b %d\n", maxB[canthilos]);
        // printf("Mi minimoo total C %d\n", minC[canthilos]);
        // printf("Mi maximo total C %d\n", maxC[canthilos]);
        escalar = (maxA[canthilos] * maxB[canthilos] * maxC[canthilos] - minA[canthilos] * minB[canthilos] * minC[canthilos]) / totalA[canthilos]/(N*N) * totalB[canthilos]/(N*N) *totalC[canthilos]/(N*N);
        // printf("El escalar:  %f\n", escalar );
    }
    //BARRERA PARA ESPERAR AL QUE SE SACRIFICA POR EL TEAM Y CALCULA EL ESCALAR PARA QUE LOS DEMAS HILOS LO USEN
    // printf("El dale! %d\n", threadId );
    barrier(&mean_barrier);

    // MULTIPLICO POR EL ESCALAR
    for(i=inicio;i<fin;i++){
        for(j=0;j<N;j++){
            D[i*N+j]=D[i*N+j] * escalar;

        }
    }

}

//Para calcular tiempo
double dwalltime(){
    double sec;
    struct timeval tv;

    gettimeofday(&tv,NULL);
    sec = tv.tv_sec + tv.tv_usec/1000000.0;
    return sec;
}

int main(int argc,char*argv[]){
 
    int i,j;
    N = atoi(argv[1]);
    canthilos = atoi(argv[2]);
    //Aloca memoria para las matrices
    A=(double*)malloc(sizeof(double)*N*N);
    B=(double*)malloc(sizeof(double)*N*N);
    C=(double*)malloc(sizeof(double)*N*N);
    D=(double*)malloc(sizeof(double)*N*N);
    AB=(double*)malloc(sizeof(double)*N*N);
    // aloco los arreglos
    totalA = (double*)malloc(sizeof(double)*canthilos +1 );
    totalB = (double*)malloc(sizeof(double)*canthilos +1 );
    totalC = (double*)malloc(sizeof(double)*canthilos +1 );
    maxA = (double*)malloc(sizeof(double)*canthilos +1 );
    maxB = (double*)malloc(sizeof(double)*canthilos +1 );
    maxC = (double*)malloc(sizeof(double)*canthilos +1 );
    minA = (double*)malloc(sizeof(double)*canthilos +1 );
    minB = (double*)malloc(sizeof(double)*canthilos +1 );
    minC = (double*)malloc(sizeof(double)*canthilos +1 );

    // SETEO DE LOS VALORES EN LOS DISTINTOS ARREGLOS, CADA HILO TIENE UN ELEMENTO UNICO Y EXCLUSIVO EN CADA UNO DE ELLOS
    for(int i = 0; i <= canthilos; i++){
   		totalA[i] = 0;       
      totalB[i] = 0;
      totalC[i] = 0;
      maxA[i] = 0;
      maxB[i] = 0; 
      maxC[i] = 0;
      maxC[i] = 0;
      minA[i] = nRandom;
      minB[i] = nRandom;
      minC[i] = nRandom;
      // printf("El seteo quedo:! %d\n", minC[i] );

    } 

    srand ( time(NULL) );
    //srand genera la semilla random y cargo las matrices
      for( i = 0 ; i < N ; i++){
        for ( j = 0; j < N; j++){
            A[i*N+j] = rand() % nRandom;
            B[i*N+j] = rand() % nRandom;
            C[i*N+j] = rand() % nRandom;
        }
      }

    double timetick;
    timetick = dwalltime();
    pthread_t threads[canthilos];
    pthread_attr_t attr;


    for (i = 0; i < canthilos; i++)
      pthread_create(&threads[i], NULL, computarExpresionEscalar, (void *) (__intptr_t) i);  
    
    for (i = 0; i < canthilos; i++)
      pthread_join(threads[i], NULL);

    //TERMINO
    printf("Tiempo en segundos %f\n", dwalltime() - timetick);
  

 return(0);

}