#include <iostream>
#include<queue>
#include "item.h"
using namespace std;

int MAX_WEIGHT = 13;
int MAX_PROFIT = 0;
long node_count = 0;
unsigned int N = 0;

struct compare_bound{
    bool operator()(Item x,Item y)
    {
        return x.bound() > y.bound();
    }
};

int knapsack_brute_force(Item items[],int weight,unsigned int n,int& count);
void knapsack_back_tracking(int i, Item items[], bool include[], bool best_set[], float profit, float weight);
bool promising(int i, Item items[], float profit, float weight);
void knapsack_branch_and_bound(Item items[]);
int bound(Item node, Item items[]);
void insertion_sort(Item A[],int low,int high);
void quick_sort(Item A[],int low,int high);
void partition(Item A[],int low,int high,int& pivot);
void swap(Item A[], int i, int j);
void print(Item A[], int low, int high);
void copy(bool source[],bool dest[],int start,int end,int dest_index);
void init(Item A[]);
void set_zero(bool A[],unsigned int n);

int main()
{
    cout<<"number of items:";
    cin>>N;
    Item test[N];
    init(test);
    bool include[N];
    bool best_set[N];
    int count = 0;
    set_zero(include,N);
    set_zero(best_set,N);

    quick_sort(test,0,N);//sort by greatest ratio

    //brute force solution:
    cout<<"brute force solution:=============================="<<endl<<
          "max profit:"<<knapsack_brute_force(test,MAX_WEIGHT,N,count)<<endl;
    cout<<"============================================="<<endl;
    node_count = 0;
    //knap sack back tracking solution:
    knapsack_back_tracking(-1,test,include,best_set,0,0);
    cout<<"back tracking solution:"<<MAX_PROFIT<<endl<<"nodes visited:"<<node_count<<endl;
    cout<<"solution set back_tracking:============================"<<endl;
    for(unsigned int i = 0; i < N;i++)
        if(best_set[i]){
            cout<<endl;
            print(test[i]);
        }
    node_count = 0;

    //branch and bound solution:
    cout<<"============================================="<<endl;
    knapsack_branch_and_bound(test);
    cout<<"MAX_PROFIT:"<<MAX_PROFIT<<endl<<"node count:"<<node_count<<endl;

    return 0;
}
//brute force recursive solution
int knapsack_brute_force(Item items[], int weight, unsigned int n, int& count)
{
    count = count+1;
    cout<<"node_count:"<<count<<endl;
    if(weight == 0 || n == 0)
        return 0;
    if(items[n-1].weight() > weight)
        return knapsack_brute_force(items,weight,n-1,count);
    else
    {
        float temp_val = items[n-1].profit() + knapsack_brute_force(items,weight - items[n-1].weight(),n-1,count);
        return temp_val > knapsack_brute_force(items,weight,n-1,count) ? temp_val : knapsack_brute_force(items,weight,n-1,count);
    }
}
//back tracking solution
void knapsack_back_tracking(int i, Item items[],bool include[],bool best_set[], float profit, float weight)
{
    node_count++;
    if(weight <= MAX_WEIGHT && profit > MAX_PROFIT)
    {
        MAX_PROFIT = profit;
        copy(include,best_set,0,N,0);
    }
    if(promising(i,items,profit,weight))
    {
        include[i+1] = true;
        knapsack_back_tracking(i+1,items,include,best_set,profit + items[i+1].profit(),weight + items[i+1].weight());
        include[i+1] = false;
        knapsack_back_tracking(i+1,items,include,best_set,profit,weight);
    }
}
//helper function for back tracking solution
//returns true is a branch is deemmed promising, false otherwise
bool promising(int i, Item items[], float profit, float weight)
{
    unsigned int j = 0;
    unsigned int k = 0;
    int total_weight;
    float b = profit;

    if(weight >= MAX_WEIGHT)
        return false;
    else
    {
        j = i + 1;
        b = profit;
        total_weight = weight;
        while(j < N && total_weight + items[j].weight() <= MAX_WEIGHT)
        {

            total_weight = total_weight + items[j].weight();
            b = b + items[j].profit();
            j++;
        }
        k = j;
        if( k < N)
            b = b + (MAX_WEIGHT - total_weight) * items[k].ratio();
    }
    return b > MAX_PROFIT;
}
void knapsack_branch_and_bound(Item items[])
{
    priority_queue<Item> pq;

    Item v = Item();
    Item u = Item();
    MAX_PROFIT = 0;


    v.set_bound(bound(v,items));
    pq.push(v);

    while(!pq.empty())
    {
        node_count++;
        v = pq.top();
        pq.pop();
        cout<<"v:"<<endl;
        print(v);
        cout<<endl;

        if(v.bound() > MAX_PROFIT)
        {
            node_count++;
            u.set_lvl(v.level() + 1);
            u.set_weight(v.weight()  + items[u.level()].weight());
            u.set_profit(v.profit() + items[u.level()].profit());
            u.set_bound(bound(u,items));
            cout<<"u1:"<<endl;
            print(u);
            cout<<endl;
            if(u.weight() <= MAX_WEIGHT && u.profit() > MAX_PROFIT)
                MAX_PROFIT = u.profit();
            if(u.bound() > MAX_PROFIT)
                pq.push(u);
            node_count++;
            u.set_weight(v.weight());
            u.set_profit(v.profit());
            u.set_bound(bound(u,items));
            cout<<"u2:"<<endl;
            print(u);
            cout<<endl;
            if(u.bound() > MAX_PROFIT)
                pq.push(u);
        }
    }
}
int bound(Item node,Item items[])
{
    unsigned int j = 0;
    unsigned int k = 0;
    int total_weight;
    int result;
    int temp;

    if(node.weight() >= MAX_WEIGHT)
        return 0;
    else
    {
        result = node.profit();
        j = node.level() + 1;
        total_weight = node.weight();
        temp = total_weight = total_weight + items[j].weight();
        while( j < N && temp <= MAX_WEIGHT)
        {
            total_weight  = temp;
            result += items[j].profit();
            j++;
        }
    }
    k = j;
    if( k < N)
        result += (MAX_WEIGHT - total_weight) * items[k].ratio();
    return result;
}
//used in quick_sort
void insertion_sort(Item A[],int low,int high){
    for(int i = low; i < high;i++)
        for(int j = i ;j > 0 && A[j-1] > A[j];j--)
            swap(A,j,j-1);
}
//used to sort items based on ratio
void quick_sort(Item A[],int low,int high){
    int pivot_point;
    if(high > low){
        if(high - low < 16)
            insertion_sort(A,low,high);
        else
            if(high - low > 16)
                swap(A,low,rand()% (high - low) + 1);
        partition(A,low,high,pivot_point);
        quick_sort(A,low,pivot_point);
        quick_sort(A,pivot_point + 1,high);
    }
}
void partition(Item A[],int low,int high,int& pivot)
{
    int j = low;
    Item pivot_item = A[low];
    for(int i = low + 1;i < high;i++)
        if(A[i] > pivot_item){
            j++;
            swap(A,i,j);
        }
    pivot = j;
    swap(A,low,pivot);
}
//swaps two items within an array
void swap(Item A[],int i,int j)
{
    Item temp = A[i];
    A[i] = A[j];
    A[j] = temp;
}
//copies source into dest
void copy(bool source[], bool dest[], int start, int end, int dest_index)
{
    while(start < end)
        dest[dest_index++] = source[start++];
}
void init(Item A[])
{
    cout<<"max weight:";
    cin>>MAX_WEIGHT;

    int wi = 0;
    int pi =0;

    for(unsigned int i = 0 ; i < N;i++)
    {
        cout<<"weight of item "<<i + 1<<":";
        cin>>wi;
        cout<<"profit of item "<<i + 1<<":";
        cin>>pi;
        A[i].set_weight(wi);
        A[i].set_profit(pi);
    }
}
void set_zero(bool A[],unsigned int n)
{
    for(unsigned int i = 0; i < n;i++)
        A[i] = 0;
}
