#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

/*
This program provides a possible solution for the produce-conseumer
problem using mutex and semaphore
I have used 5 producers and 5 conseumers to demonstrate the solution.
You can always play wiht these values.
*/
//Maximum items a producer can produce or a consumer can consume.

#define MaxItems 5
#define BufferSize 5 // Size of the BufferSize

sem_t empty;
sem_t full;
int in = 0;
int out = 0;
int counter = 0;
int sleepTime = 5;  // let producer and consumer run slowly
int buffer[BufferSize];
pthread_mutex_t mutex;

void *producer(void *pno)
{
  int item;
  for(int i=0; i< MaxItems; i++){
    item = rand();  // Produce a random item
    sem_wait(&empty);
    pthread_mutex_lock(&mutex);
    buffer[in] = item;
    printf("producer %d: Insert Item %d at %d\n", *((int *)pno),buffer[in],in);
    sleep(sleepTime);
    in = (in+1)%BufferSize;
    pthread_mutex_unlock(&mutex);
    sem_post(&full);
  }
}
void *consumer(void *cno)
{
  for(int i = 0; i < MaxItems; i++) {
    sem_wait(&full);
    pthread_mutex_lock(&mutex);
    int item = buffer[out];
    counter = counter + item;
    printf("Consumer %d: Remove Item %d from %d\n", * ((int *)cno),item,out);
    printf("The total count for items is %d\n", counter);
    sleep(sleepTime);
    out = (out+1)%BufferSize;
    pthread_mutex_unlock(&mutex);
    sem_post(&empty);
  }
}

int main()
{
  
  pthread_t pro[5],con[5];
  pthread_mutex_init(&mutex, NULL);
  sem_init(&empty,0,BufferSize);
  sem_init(&full,0,0);

  //Just used for numbering the producer and Consumer
  int a[5] = {1,2,3,4,5};

  for(int i = 0; i < 5; i++){
    pthread_create(&pro[i], NULL, (void *)producer, (void *)&a[i]);
  }
  for(int i = 0; i < 5; i++){
    pthread_create(&con[i], NULL, (void *)consumer, (void *)&a[i]);
  }

  for(int i = 0; i < 5; i++){
    pthread_join(pro[i], NULL);
  }
  for(int i=0; i < 5; i++ ){
    pthread_join(con[i], NULL);
  }

  pthread_mutex_destroy(&mutex);
  sem_destroy(&empty);
  sem_destroy(&full);

  return 0;
}
