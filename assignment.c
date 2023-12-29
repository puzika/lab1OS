#include <stdlib.h> 
#include <stdio.h> 
#include <stdbool.h> 
#include <unistd.h> 
#include <pthread.h>

pthread_mutex_t mutex; 
pthread_cond_t cond1; 
int ready = 1;

void* produce() {
   while (true) {
      pthread_mutex_lock(&mutex);
      
      if (ready == 1) {
         pthread_mutex_unlock (&mutex);
         continue;
      }

      ready = 1;
      printf("producer sent event\n");
      pthread_mutex_unlock (&mutex);
      pthread_cond_signal(&cond1);
      sleep(1);
   }
}

void consume() {
   while (true) {
      pthread_mutex_lock (&mutex);

      if (ready == 0) pthread_cond_wait(&cond1, &mutex);

      ready = 0;
      printf("consumer received event\n"); pthread_mutex_unlock (&mutex);
   }
}

int main() {
   pthread_mutex_init(&mutex, NULL);
   pthread_cond_init(&cond1, NULL);
   pthread_t threadProducer, threadConsumer;
   pthread_create(&threadProducer, NULL, &produce, NULL);
   pthread_create(&threadConsumer, NULL, &consume, NULL);
   pthread_join(threadProducer, NULL);
   pthread_join(threadConsumer, NULL);
   pthread_mutex_destroy(&mutex);
   pthread_cond_destroy(&cond1);

   return 0;
}