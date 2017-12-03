#include<iostream>
#include<fstream>
#include<set>
#include<list>
using namespace std;
int find_data(int);
typedef struct
{
    int data;
    int index;
}mydata;
std::list<mydata> LRU_stack;
std::set<int> record;
void frequency_change(mydata);
int data_in(char*);
int open_file(char*);
ifstream fin;
int page_size[4]={64,128,256,512};

int main()
{
    int size_count=0;
    char s[100];
    cout<<"Input file name: ";
    cin>>s;

    while(size_count<4)
    {
        int k=page_size[size_count];
        mydata d1;
        int LRU[k];
        int is_found;
        int miss=0,hit=0;
        int count=0;

        open_file(s);
        char c[100];

        while(!fin.eof())
        {
            fin.getline(c,100);
            if(c[2]!=' ')
                continue;

            d1.data=data_in(c);

            is_found=find_data(d1.data);
            if(!is_found)
            {
                miss++;
                record.insert(d1.data);
                if(record.size()>k)
                {
                    std::list<mydata>::iterator it;
                    it=LRU_stack.end();
                    it--;
                    d1.index=it->index;
                    record.erase(LRU[d1.index]);
                    LRU_stack.pop_back();
                    LRU[d1.index]=d1.data;
                }
                else
                {
                    d1.index=count;
                    LRU[d1.index]=d1.data;
                    count++;
                }
                LRU_stack.push_front(d1);
            }
            else
            {
                hit++;
                frequency_change(d1);
            }
            //for(int i=0;i<page_size;i++)cout<<LRU[i];
            //cout<<"*";
        }
        double page_fault_ratio=(double)miss/(double)hit;
        cout<<"size : "<<k;
        cout<<"  miss : "<<miss;
        cout<<"  hit : "<<hit;
        cout<<"  page fault ratio : "<<page_fault_ratio<<endl;

        fin.close();
        size_count++;
        record.clear();
        LRU_stack.clear();
    }
    return 0;
}
int find_data(int data)
{
    if(record.find(data)!=record.end())
        return 1;
    else
        return 0;
}
void frequency_change(mydata d1)
{
     std::list<mydata>::iterator it=LRU_stack.begin();
     if(it->data!=d1.data)
     {
         for(it=LRU_stack.begin();it!=LRU_stack.end();it++)
         {
             if(it->data==d1.data)
             {
                d1.index=it->index;
                LRU_stack.erase(it);
                LRU_stack.push_front(d1);
             }
         }
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

int data_in(char *c)
{
    int addr=0;

    for(int i=3;i<8;i++)
    {
        addr=addr*16;
        if(c[i]>='a'&&c[i]<='f')
            addr+=(c[i]-'a'+10);
        else
            addr+=(c[i]-'0');
    }
    return addr;
}
