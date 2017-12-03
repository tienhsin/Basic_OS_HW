#include<pthread.h>
#include<stdio.h>
#include<semaphore.h>
#include<assert.h>

void *func1();
void *func2();
int counter=1,counter1=0,counter2=1;
sem_t signal;

int main()
{
    sem_init(&signal,0,1);
    pthread_t thread1,thread2;
    int s1,s2;
    s1=pthread_create(&thread1,NULL,func1,NULL);
    s2=pthread_create(&thread2,NULL,func2,NULL);
    assert(s1==0&&s2==0);
    pthread_join(thread1,NULL);
    pthread_join(thread2,NULL);
    sem_destroy(&signal);
    return 0;
}

void *func1()
{
    int i=0;
    for(i=0;i<25;i++)
    {
        while(counter1+1!=counter){};
        sem_wait(&signal);
        printf("func1 : %d\n",counter);
        counter++;
        counter1=counter;
        sem_post(&signal);
    }
    pthread_exit(0);
}
void *func2()
{
    int i=0;
    for(i=0;i<25;i++)
    {
        while(counter2+1!=counter){};
        sem_wait(&signal);
        printf("func2 : %d\n",counter);
        counter++;
        counter2=counter;
        sem_post(&signal);
    }
    pthread_exit(0);
}
