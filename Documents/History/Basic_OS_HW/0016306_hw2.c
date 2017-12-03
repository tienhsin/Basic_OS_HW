#include <unistd.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/ipc.h>
#include <stdio.h>
#include<stdlib.h>

struct timeval start1, end1;
struct timeval start2, end2;
struct timeval start3, end3;
int main()
{
while(1)
{
    unsigned int a[800][800],b[800][800],c[800][800];
    int i,j,k,in;
    unsigned long long int sum=0;

    pid_t pid1,pid2,pid3,pid4;
    int shmid,shmid2;
    unsigned int *shm;
    unsigned int *ptr2,*ptr1;
    key_t key,key1,key2,key3;
    key1=0000;
    int counter=0;

    printf("input the matrix dimension: ");
    scanf("%d",&in);
    for(i=0;i<in;i++)
    {
        for(j=0;j<in;j++)
        {
            a[i][j]=i*in+j;
            b[i][j]=i*in+j;
        }
    }
    while(counter<3)
    {
        sum=0;
        counter++;
        if(counter==1)
        {
            gettimeofday(&start1, 0);
            for(i=0;i<in;i++)
            {
                for(j=0;j<in;j++)
                {
                    for(k=0;k<in;k++)
                    {
                        c[i][j]+=a[i][k]*b[k][j];
                    }
                    sum+=c[i][j];
                }
            }
            gettimeofday(&end1, 0);
            int sec1 = end1.tv_sec - start1.tv_sec;
            int usec1 = end1.tv_usec - start1.tv_usec;
            printf("Multiplying matrices by 1 process: %f ms \n", sec1*1000+(usec1/1000.0));
            printf("element sum: %lld\n",sum);

        }
        else if(counter==2)
        {
            key=1111;
            pid1=fork();
            if(pid1<0)
            {
                fprintf(stderr,"Fork Faild\n");
                exit(-1);
            }
            else if(pid1==0)
            {
                if ((shmid = shmget(key, in*in*sizeof(unsigned int), 0666)) < 0)
                {
                    fprintf(stderr,"shmget faild\n");
                    perror("shmget");
                    exit(1);
                }
                if ((shm = shmat(shmid, NULL, 0)) == (char *) -1)
                {
                    printf(stderr,"shmat faild\n");
                    perror("shmat");
                    exit(1);
                }
                ptr1=shm;
                for(i=0;i<in;i++)
                {
                    for(j=0;j<in/2;j++)
                    {
                        ptr1=shm+i*in+j;
                        for(k=0;k<in;k++)
                        {
                            *ptr1+=a[i][k]*b[k][j];
                        }
                    }
                }
                ptr1=NULL;
                exit(0);
            }
            else
            {
                if ((shmid = shmget(key, in*in*sizeof(unsigned int), IPC_CREAT | 0666)) < 0)
                {
                    fprintf(stderr,"shmget faild\n");
                    perror("shmget");
                    exit(1);
                }
                if ((shm = shmat(shmid, NULL, 0)) == (char *) -1)
                {
                    fprintf(stderr,"shmat faild\n");
                    perror("shmat");
                    exit(1);
                }
                gettimeofday(&start2, 0);
                for(i=0;i<in;i++)
                {
                    for(j=in/2;j<in;j++)
                    {
                        ptr2=shm+i*in+j;
                        for(k=0;k<in;k++)
                        {
                            *ptr2+=a[i][k]*b[k][j];
                        }
                    }
                }

                wait(NULL);
                gettimeofday(&end2, 0);
                int sec2 = end2.tv_sec - start2.tv_sec;
                int usec2 = end2.tv_usec - start2.tv_usec;
                printf("Multiplying matrices by 1 process: %f ms \n", sec2*1000+(usec2/1000.0));
                //printf("Memorattached at %X\n", shm);
            }

            ptr2=shm;
            for(i=0;i<in*in;i++)
            {
                sum+=*(ptr2+i);
            }
            printf("element sum: %lld\n",sum);
            ptr2=NULL;
        }

        if(counter==3)
        {
            pid_t pid0;
            gettimeofday(&start3, 0);

            if ((shmid2 = shmget(key1, in*in*sizeof(unsigned int), IPC_CREAT | 0666)) < 0)
            {
                fprintf(stderr,"shmget faild3\n");
                perror("shmget");
                exit(1);
            }
            if ((shm = shmat(shmid2, NULL, 0)) == (char *) -1)
            {
                fprintf(stderr,"shmat faild4\n");
                perror("shmat");
                exit(1);
            }
            printf("child one Memorattached at %X\n", shm);
            pid3=fork();
            if(pid3<0)
            {
                fprintf(stderr,"Fork Faild2\n");
                exit(-1);
            }
            else if(pid3==0)
            {
                pid2=fork();
                if(pid2==0)
                {
                    ptr1=shm;
                    for(i=0;i<in/2;i++)
                    {
                        for(j=in/2;j<in;j++)
                        {
                            ptr1=shm+i*in+j;
                            for(k=0;k<in;k++)
                            {
                                *ptr1+=a[i][k]*b[k][j];
                            }
                        }
                    }

                    ptr1=NULL;
                    exit(0);
                }
                else if(pid2>0)
                {

                    ptr1=shm;
                    for(i=in/2;i<in;i++)
                    {
                        for(j=in/2;j<in;j++)
                        {
                            ptr1=shm+i*in+j;
                            for(k=0;k<in;k++)
                            {
                                *ptr1+=a[i][k]*b[k][j];
                            }
                           // printf("%u\n", *ptr1);
                        }
                    }
                    ptr1=NULL;
                    wait(NULL);
                }
                exit(0);
            }
            else if(pid3>0)
            {
                pid2=fork();
                if(pid2==0)
                {
                    ptr1=shm;
                    for(i=0;i<in/2;i++)
                    {
                        for(j=0;j<in/2;j++)
                        {
                            ptr1=shm+i*in+j;
                            for(k=0;k<in;k++)
                            {
                                *ptr1+=a[i][k]*b[k][j];
                            }
                           // printf("%u\n", *ptr1);
                        }
                    }
                    ptr1=NULL;
                    //wait(NULL);
                    exit(0);
                }
                else if(pid2>0)
                {

                    ptr1=shm;
                    for(i=in/2;i<in;i++)
                    {
                        for(j=0;j<in/2;j++)
                        {
                            ptr1=shm+i*in+j;
                            for(k=0;k<in;k++)
                            {
                                *ptr1+=a[i][k]*b[k][j];
                            }
                          //  printf("%u\n", *ptr1);
                        }
                    }
                    ptr1=NULL;
                    wait(NULL);
                    wait(NULL);


                    ptr1=shm;
  //                wait(NULL);
                    gettimeofday(&end3, 0);
                    int sec3 = end3.tv_sec - start3.tv_sec;
                    int usec3 = end3.tv_usec - start3.tv_usec;
                    printf("Multiplying matrices by 1 process: %f ms \n", sec3*1000+(usec3/1000.0));
                    //printf("Memorattached at %X\n", shm);
                    for(i=0;i<in*in;i++)
                    {
                        sum+=*(ptr1+i);
                       // printf("%lld    %lld\n",sum,*(ptr1+i));
                    }
                    printf("element sum: %llu\n",sum);
                    ptr1=NULL;



                }
            }
        }

        shmdt((void *) shm);
        shmctl(shmid, IPC_RMID, NULL);
        shmctl(shmid2, IPC_RMID, NULL);
        if(counter==4)break;
    }
}
    return 0;
}
