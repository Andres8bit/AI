#include <iostream>
#include<stdlib.h>
#include<time.h>
using namespace std;

/*solves n queens*/
void solve(int queens[]);
/*generates a random initial state space*/
void generate_random(int queens[]);
/*prints board*/
void print(int queens[]);

const int N = 12;

int main()
{
    srand(time(NULL));
    int queens[N];/*1D-array, 1->N,index = col,value = row*/
    generate_random(queens);
    cout<<"original board:"<<endl;
    print(queens);
    solve(queens);
    return 0;
}
/*generates a random initial state space*/
void generate_random(int queens[])
{
    for(int i = 1; i < N;i++)
        queens[i] = rand() % N + 1;
}
/*prints board*/
void print(int queens[]){
    for(int i = 1; i < N ;i++){
        cout<<"|";
        for(int j = 1;j < N;j++){
            if(queens[i] == j)
                std::cout<<"[Q]";
            else
                cout<<"[ ]";
        }
            cout<<"|"<<endl;
    }
cout<<endl;
}
/*h1:counts conflicts of queen at ith col*/
int conflicts(int queens[],int i){
    int count = 0;
    for(int j = 1; j < N ;j++ )
        if(i != j)
            if(queens[j] == queens[i] || abs(queens[j] - queens[i]) == abs(i - j))
                count++;
    return count;
}
/*h2: returns the col with max # of conflicts*/
int max_conflicts(int queens[]){
    int max_value = 0;
    int max_index = 1;
    int conflict_count = 0;

    for(int i = 1; i < N;i++){
        conflict_count = conflicts(queens,i);
        if(conflict_count >= max_value){
            max_index = i;
            max_value = conflict_count;
        }
    }
    return max_index;
}
/*returns true when no conflicts are found*/
bool solved(int queens[]){
    for(int i = 1;i < N;i++)
        if(conflicts(queens,i) != 0)
            return false;
    return true;
}
/*climber: moves state to local optimum*/
void move_queen(int queens[],int i){
    int pos = 1;/*change in pos = change in state*/
    int min_pos = 1;/*row position of queen[i] with min conflicts */
    int conflict_count = 0;
    int min_count = queens[i];

    for(;pos < N;pos++){
        queens[i] =  pos;
        conflict_count = conflicts(queens,i);
        /*if state has less conflicts it is a possible local optimum*/
        if(conflict_count <= min_count){
            min_count = conflict_count;
            min_pos = pos;
        }
    }
    queens[i] = min_pos;/*sets state to local optimum*/
}
/*solves n queens*/
void solve(int queens[]){
    for(int i = 0; i < 300;i++){
        if(solved(queens)){
            cout<<"*******solved*******"<<endl;
            print(queens);
            break;
        }
        if(i%5 == 0)//random restart
            generate_random(queens);
       move_queen(queens,max_conflicts(queens));//climbing
    }
}
