#include<iostream>
#include<fstream>
#include<list>
#include<cstdlib>
using namespace std;
ifstream fin;
std::list<int> FIFO_Queue;
void Open_File();
void full_queue(int);
int Dist_accumulate();
int open_file(char*);
unsigned int dist;
int queue_size[4]={32,64,128,256};
int main()
{
    int size_count=0;
    Open_File();

    while(size_count<4)
    {
        while(!fin.eof())
        {
            full_queue(size_count);
            Dist_accumulate();
            FIFO_Queue.pop_front();
        }
        while(FIFO_Queue.size()>1)
        {
            Dist_accumulate();
            FIFO_Queue.pop_front();
        }
        cout<<queue_size[size_count]<<"  "<<dist<<endl;
        fin.close();
        FIFO_Queue.clear();
        size_count++;
    }
    return 0;
}
void Open_File()
{
    char s[100];
    cout<<"Input file name: ";
    cin>>s;
    open_file(s);
}
void full_queue(int k)
{
    int data=0;
    while(FIFO_Queue.size()<queue_size[k])
    {
        fin>>data;
        if(data==0)
        {
            if(fin.eof())
                return;
        }
        FIFO_Queue.push_back(data);
        if(FIFO_Queue.size()==1)
            dist+=data;
    }
}
int open_file(char *s)
{
    fin.open(s);
    if(!fin.is_open())
    {
        cout<<"fail"<<endl;
        return -1;
    }
}
int Dist_accumulate()
{
    int d1,d2,d0;
    std::list<int>::iterator it;
    it=FIFO_Queue.begin();
    d1=*it;
    it++;
    d2=*it;
    d0=d1-d2;
    if(d0>0)
        dist+=d0;
    else
    {
        d0=d0*(-1);
        dist+=d0;
    }
}
