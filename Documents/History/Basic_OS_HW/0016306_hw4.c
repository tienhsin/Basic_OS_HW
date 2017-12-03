#include<stdio.h>
#include<stdlib.h>
#include<semaphore.h>
#include<assert.h>
#include<unistd.h>
#include<pthread.h>
#include <sys/time.h>

struct timeval start1, end1;
struct timeval start2, end2;
int element[1000000],n,p1,p2,p3;
int in();
int out(float);
void *qsort_t1();
void *qsort_t2();
void *qsort_t3();
void *qsort_t4();
void *qsort_t5();
void *qsort_t6();
void *qsort_t7();
void quick_sort(int*,int,int);
void Swap(int*,int*);
void print();
int part(int,int);
sem_t signal1,signal2,signal3,signal4;

int main()
{
    int i,s1;
    pthread_t thread1;
    in();
    
    gettimeofday(&start1, 0);
    sem_init(&signal1,0,0);
    s1=pthread_create(&thread1,NULL,qsort_t1,NULL);
    assert(s1==0);
    sem_wait(&signal1);
    sem_destroy(&signal1);
    gettimeofday(&end1, 0);
    int sec1 = end1.tv_sec - start1.tv_sec;
    int usec1 = end1.tv_usec - start1.tv_usec;
	float time=sec1*1000+(usec1/1000.0);
	out(time);    
	return 0;
}

void quick_sort(int *ele,int left, int right)
{
    int j;
    j=part(left,right);
    if(j==-1){return;}
    quick_sort(ele,left,j-1);
    quick_sort(ele,j+1,right);
}

int part(int left,int right)
{
    int pivot=element[left];
    int i=left+1,j=right;

    if(left>=right){return -1;}

    while(i<=j)
    {
        if(element[i]<pivot){i++;}
        else
        {
            while(i<=j)
            {
                if(element[j]>pivot){j--;}
                else
                {
                    Swap(&element[i],&element[j]);
                    j--;
                    break;
                }
            }
        }
    }
    Swap(&element[left],&element[j]);
    return j;
}

void *qsort_t1()
{
    int s2,s3;
    if(0<n-1)
        p1=part(0,n-1);
    else
        p1=0;
    pthread_t thread2,thread3;
    sem_init(&signal2,0,0);
    s2=pthread_create(&thread2,NULL,qsort_t2,NULL);
    s3=pthread_create(&thread3,NULL,qsort_t3,NULL);
    assert(s2==0&&s3==0);
    sem_wait(&signal2);
    sem_wait(&signal2);
    sem_destroy(&signal2);
    sem_post(&signal1);
    pthread_exit(0);
}

void *qsort_t2()
{
    int s4,s5;
    if(0<p1-1)
        p2=part(0,p1-1);
    else
        p2=0;
    pthread_t thread4,thread5;
    sem_init(&signal3,0,0);
    s4=pthread_create(&thread4,NULL,qsort_t4,NULL);
    s5=pthread_create(&thread5,NULL,qsort_t5,NULL);
    assert(s4==0&&s5==0);
    sem_wait(&signal3);
    sem_wait(&signal3);
    sem_destroy(&signal3);
    sem_post(&signal2);
    pthread_exit(0);
}

void *qsort_t3()
{
    int s6,s7;
    if(p1+1<n-1)
        p3=part(p1+1,n-1);
    else
        p3=p1+1;
    pthread_t thread6,thread7;
    sem_init(&signal4,0,0);
    s6=pthread_create(&thread6,NULL,qsort_t6,NULL);
    s7=pthread_create(&thread7,NULL,qsort_t7,NULL);
    assert(s6==0&&s7==0);
    sem_wait(&signal4);
    sem_wait(&signal4);
    sem_destroy(&signal4);
    sem_post(&signal2);
    pthread_exit(0);
}
void *qsort_t4()
{
    quick_sort(element,0,p2-1);
    sem_post(&signal3);
    pthread_exit(0);
}
void *qsort_t5()
{
    quick_sort(element,p2+1,p1-1);
    sem_post(&signal3);
    pthread_exit(0);
}
void *qsort_t6()
{
    quick_sort(element,p1+1,p3-1);
    sem_post(&signal4);
    pthread_exit(0);
}
void *qsort_t7()
{
    quick_sort(element,p3+1,n-1);
    sem_post(&signal4);
    pthread_exit(0);
}
int out(float time)
{
	FILE *file=fopen("output","w");
    if(!file)
    {
        puts("Fail");
        return 1;
    }
	fprintf(file,"After sort\n");
	int i;
    for(i=0;i<n;i++)
        fprintf(file, "%d  ",element[i]);
	fprintf(file,"\n");
	fprintf(file,"elapsed %f ms \n",time);
	fclose(file);
	return ;
}
int in()
{
    char s[20];
    scanf("%s",s);
    FILE *file=fopen(s,"r");
    if(!file)
    {
        puts("Fail");
        return 1;
    }
    int i;
    fscanf(file,"%d",&n);
    for(i=0;i<n;i++)
        fscanf(file, "%d",&element[i]);

    fclose(file);
    return ;
}

void Swap(int *a,int *b)
{
    int temp;
    temp=*b;
    *b=*a;
    *a=temp;
}

void print()
{
    int i;
    for(i=0;i<n;i++)
    {
        printf("%d  ",element[i]);
    }
    printf("\n");
}
