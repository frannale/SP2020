#include <pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include <sys/time.h>
#define ORDENXFILAS 0
#define ORDENXCOLUMNAS 1

//Dimension por defecto de las matrices
int N=100;
int canthilos=4;
double *A,*B,*C,*D,*E;
int i,j,k;

// //Retorna el valor de la matriz en la posicion fila y columna segun el orden que este ordenada
// double getValor(double *matriz,int fila,int columna,int orden){
//  if(orden==ORDENXFILAS){
//   return(matriz[fila*N+columna]);
//  }else{
//   return(matriz[fila+columna*N]);
//  }
// }

// //Establece el valor de la matriz en la posicion fila y columna segun el orden que este ordenada
// void setValor(double *matriz,int fila,int columna,int orden,double valor){
//  if(orden==ORDENXFILAS){
//   matriz[fila*N+columna]=valor;
//  }else{
//   matriz[fila+columna*N]=valor;
//  }
// }

//Para calcular tiempo
double dwalltime(){
        double sec;
        struct timeval tv;

        gettimeofday(&tv,NULL);
        sec = tv.tv_sec + tv.tv_usec/1000000.0;
        return sec;
}


//hilo que multiplica parte de la matriz
void *multiplica(void *id) {
  int i,j,k;
  int threadId = (int) id;
  int inicio = threadId * (N/canthilos);
  int fin = inicio + (N/canthilos);

  for(i = inicio ; i < fin ; i++){
   for(j = inicio; j < fin; j++){
      C[i*N+j] = 0;
    for(k = inicio; k < fin; k++){
      C[i*N+j]= C[i*N+j] + ( A[i*N+k] * B[k+j*N] );
    }
   }
  }
}

int main(int argc,char*argv[]){
 
 int check=1;
 double timetick;

 //Controla los argumentos al programa
 if ((argc != 2) || ((N = atoi(argv[1])) <= 0) )
  {
    printf("\nUsar: %s n\n  n: Dimension de la matriz (nxn X nxn)\n", argv[0]);
    exit(1);
  }

 //Aloca memoria para las matrices
  A=(double*)malloc(sizeof(double)*N*N);
  B=(double*)malloc(sizeof(double)*N*N);
  C=(double*)malloc(sizeof(double)*N*N);

 //Inicializa las matrices A y B en 1, el resultado sera una matriz con todos sus valores en N
  for(i=0;i<N;i++){
   for(j=0;j<N;j++){
    A[i*N+j]= 1;
    B[i+j*N]=1;
    C[i+j*N]=1;
   }
  }   

timetick = dwalltime();
pthread_t threads[canthilos];
pthread_attr_t attr;
int i, ids[canthilos];

for (i = 0; i < canthilos; i++)
  pthread_create(&threads[i], NULL, multiplica, (void *) (unsigned long) i);
 
for (i = 0; i < canthilos; i++)
  pthread_join(threads[i], NULL);


 printf("Multiplicacion de matrices de %dx%d. Tiempo en segundos %f\n",N,N, dwalltime() - timetick);

 //Verifica el resultado
  for(i=0;i<N;i++){
   for(j=0;j<N;j++){
	check=check&&(C[i*N+j]==N);
   }
  }   

  if(check){
   printf("Multiplicacion de matrices resultado correcto\n");
  }else{
   printf("Multiplicacion de matrices resultado erroneo\n");
  }

 free(A);
 free(B);
 free(C);
 return(0);

}


