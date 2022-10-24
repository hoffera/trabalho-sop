
#include <pthread.h>
#include <stdio.h>
#include <intrin.h>

#define NUM_THREADS 30

#pragma intrinsic(__rdtsc)

void *runner(void *param);

int main(int argc, char *argv[]) {
   int i, scope;   pthread_t tid[NUM_THREADS];
   unsigned __int64 start, end;

   pthread_attr_t attr;
   struct sched_param param;

   /* get the default attributes */
   pthread_attr_init(&attr);
   pthread_attr_getschedparam (&attr, &param);
   param.sched_priority = 20;
   pthread_attr_setschedparam (&attr, &param);

   /* first inquire on the current scope */
   if (pthread_attr_getscope(&attr, &scope) != 0)
      fprintf(stderr, "Unable to get scheduling scope\n");
   else {
      if (scope == PTHREAD_SCOPE_PROCESS)
         printf("PTHREAD_SCOPE_PROCESS\n");
      else if (scope == PTHREAD_SCOPE_SYSTEM)
         printf("PTHREAD_SCOPE_SYSTEM\n");
      else
        fprintf(stderr, "Illegal scope value.\n");
   }
   /* set the scheduling algorithm to PCS or SCS */
   pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);

   start = __rdtsc();
   /* create the threads */
   for (i = 0; i < NUM_THREADS; i++){
      pthread_create(&tid[i],&attr,runner,NULL);
   }

   /* now join on each thread */
   for (i = 0; i < NUM_THREADS; i++) {
      pthread_join(tid[i], NULL);
   }

   end = __rdtsc();
   printf_s("\n%I64d ticks\n", (end-start));

}


/* Each thread will begin control in this function */
void *runner(void *param){

    int i, j, soma = 0, mat[100][200];
    srand(time(NULL));
    // preenche a matriz com números entre 0 e 99
    for(i = 0; i < 100; i++){
     for(j = 0; j < 200; j++){
     mat[i][j] = rand() % 100;
     }
    }
    // acumula a soma dos elementos e imprime a matriz
    for(i = 0; i < 100; i++){
     for(j = 0; j < 200; j++){
     soma += mat[i][j]; // acumula a soma
     printf("%2d ", mat[i][j]); // imprime a matriz
     }
     printf("\n");
    }
    // imprime a soma dos elementos da matriz
    printf("\n\nSoma total: %d\n\n", soma);

   pthread_exit(0);
}