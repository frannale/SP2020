#include <pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include <sys/time.h>
#define ORDENXFILAS 0
#define ORDENXCOLUMNAS 1

//Dimension por defecto de las matrices
int N=100;
int canthilos=4;

//Retorna el valor de la matriz en la posicion fila y columna segun el orden que este ordenada
double getValor(double *matriz,int fila,int columna,int orden){
 if(orden==ORDENXFILAS){
  return(matriz[fila*N+columna]);
 }else{
  return(matriz[fila+columna*N]);
 }
}

//Establece el valor de la matriz en la posicion fila y columna segun el orden que este ordenada
void setValor(double *matriz,int fila,int columna,int orden,double valor){
 if(orden==ORDENXFILAS){
  matriz[fila*N+columna]=valor;
 }else{
  matriz[fila+columna*N]=valor;
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

void *multiplica(void *id) {
unsigned long threadId = (unsigned long) id;
  for(i = (threadId*(N/canthilos)); i < ((threadId+1)*(N/canthilos)); i++){
   for(j = (threadId*(N/canthilos)); j < ((threadId+1)*(N/canthilos)); j++){
    setValor(C,i,j,ORDENXFILAS,0);
    for(k = (threadId*(N/canthilos)); k < ((threadId+1)*(N/canthilos)); k++){
	setValor(C,i,j,ORDENXFILAS, getValor(C,i,j,ORDENXFILAS) + getValor(A,i,k,ORDENXFILAS)*getValor(B,k,j,ORDENXCOLUMNAS));
    }
   }
  }
}


int main(int argc,char*argv[]){
 double *A,*B,*C,*D,*E;
 int i,j,k;
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
  D=(double*)malloc(sizeof(double)*N*N);
  E=(double*)malloc(sizeof(double)*N*N);

 //Inicializa las matrices A y B en 1, el resultado sera una matriz con todos sus valores en N
  for(i=0;i<N;i++){
   for(j=0;j<N;j++){
	setValor(A,i,j,ORDENXFILAS,1);
	setValor(B,i,j,ORDENXCOLUMNAS,1);
	setValor(D,i,j,ORDENXCOLUMNAS,1);
   }
  }   

timetick = dwalltime();

for (i = 0; i < canthilos; i++){
     flag = pthread_create(&threads[i], NULL, multiplica, (void *) (unsigned long) i);
 
   for (i = 0; i < canthilos; i++)
     pthread_join(threads[i], NULL);


 printf("Multiplicacion de matrices de %dx%d. Tiempo en segundos %f\n",N,N, dwalltime() - timetick);

 //Verifica el resultado
  for(i=0;i<N;i++){
   for(j=0;j<N;j++){
	check=check&&(getValor(E,i,j,ORDENXFILAS)==N*N);
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


