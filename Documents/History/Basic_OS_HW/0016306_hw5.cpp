#include<iostream>
#include<fstream>
#include<set>
using namespace std;

int add_to_record(int);
int remove_from_record(int);
int search(int);
int data_in(char*);
int open_file(char*);
void data_no_used();
std::set<int> record;
int size_array[4]={64,128,256,512};
ifstream fin;

int main()
{
    int size_count=0;
    char s[100];
    cout<<"Input file name : ";
    cin>>s;

    while(size_count<4)
    {
        int k=size_array[size_count];
        int FIFO[k],current=0,addr=0,is_found,miss=0,hit=0;
        char c[100];
        double page_fault_ratio=0;
        for(int i=0;i<k;i++)FIFO[i]=0;
        open_file(s);

        while(!fin.eof())
        {
            fin.getline(c,100);
            if(c[2]!=' ')
                continue;

            addr=data_in(c);
            is_found=search(addr);
            if(!is_found)  //miss
            {
                miss++;
                if(FIFO[current]!=0)
                {
                    remove_from_record(FIFO[current]);
                }
                add_to_record(addr);
                FIFO[current]=addr;
                current=(current+1)%k;
            }
            else
            {
                hit++;
            }

        }
        page_fault_ratio=(double)miss/(double)hit;

        cout<<"size : "<<k;
        cout<<"  miss : "<<miss;
        cout<<"  hit : "<<hit;
        cout<<"  page fault ratio : "<<page_fault_ratio<<endl;

        fin.close();
        size_count++;
        record.erase (record.begin(), record.end());
    }

}

int add_to_record(int addr)
{
    record.insert(addr);
}

int remove_from_record(int addr)
{
    record.erase(addr);
}

int search(int addr)
{
    if(record.find(addr)!=record.end())
        return 1;
    else
        return 0;
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
