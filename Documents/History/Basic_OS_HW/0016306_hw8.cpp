#include<iostream>
#include<fstream>
#include<cstdlib>
using namespace std;

void initial();
void print_queue();
void set_flag();
void add_to_queue(int );
int find_dist(int);
int find_current();
int rfind_current();
int find_dist();
int Open_File(char*);
bool check_end();

ifstream fin;
int size_count=0;
int *head;
int present_head;
int dist=0;
int queue[256];
bool flag=true;
bool finish=false;
int queue_size[4]={32,64,128,256};

int main()
{
    char s[100];
    cout<<"Input file name: ";
    cin>>s;

    while(size_count<4)
    {
        int current;
        Open_File(s);
        initial();
        while(1)
        {
            if(flag==true)
                current=find_current();
            else
                current=rfind_current();
            if(finish==true)
                break;
            //cout<<endl;
            //cout<<"current : "<<current<<endl;
            //cout<<"current head :  "<<*(head+current)<<endl;
            dist+=find_dist(current);
            add_to_queue(current);
            //print_queue();
            //cout<<"current dist*  "<<dist<<endl;
            //system("pause");
        }
    cout<<queue_size[size_count]<<"  "<<dist<<endl;
    size_count++;
    fin.close();
    }
    return 0;
}
void add_to_queue(int current)
{
    present_head=*(head+current);
    if(!fin.eof())
        fin>>*(head+current);
    else
        *(head+current)=200000;
}
int find_dist(int current)
{
    int dist_s;
    dist_s=*(head+current)-present_head;
    if(dist_s<0)
        dist_s*=-1;
    return dist_s;
}
int find_current()
{
    int current=0;
    int shortest=100000;
    for(int i=0;i<queue_size[size_count];i++)
    {
        if(*(head+i)-present_head<shortest && *(head+i)-present_head>=0)
        {
            shortest=*(head+i)-present_head;
            current=i;
        }
    }
    if(shortest==100000)//turn back
    {
        finish=check_end();
        if(finish==true)
            return 0;
        dist+=((59999-present_head)*2);
        current=rfind_current();
        set_flag();
    }
    return current;
}
bool check_end()
{
    for(int i=0;i<queue_size[size_count];i++)
    {
        if(queue[i]==200000)
            continue;
        else
            return false;
    }
    return true;
}
int rfind_current()
{
    int current=0;
    int shortest=100000;
    for(int i=0;i<queue_size[size_count];i++)
    {
        if(present_head-*(head+i)<shortest && present_head-*(head+i)>=0)
        {
            shortest=present_head-*(head+i);
            current=i;
        }
    }
    if(shortest==100000)//turn back
    {
        finish=check_end();
        if(finish==true)
            return 0;
        dist+=((present_head-0)*2);
        current=find_current();

        set_flag();
    }
    return current;
}
void set_flag()
{
    if(flag==true)
        flag=false;
    else
        flag=true;
}
void initial()
{
    for(int i=0;i<queue_size[size_count];i++)
        if(fin.eof())
            queue[i]=200000;
        else
            fin>>queue[i];
    head=queue;
    present_head=0;
    dist=0;
    flag=true;
    finish=false;
}
void print_queue()
{
    for(int i=0;i<queue_size[size_count];i++)
    {
        cout<<queue[i]<<"  ";
        if(i==10||i==20)
            cout<<endl;
    }
    cout<<endl;
}
int Open_File(char* s)
{
    fin.open(s);
    if(!fin.is_open())
    {
        cout<<"fail"<<endl;
        return -1;
    }
}
