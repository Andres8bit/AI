#include "heap.h"

heap::heap()
{
   // tree.clear();
}

heap::~heap(){
   // tree.clear();
}

heap::heap(heap& other){
    copy(other,*this);
}

heap& heap::operator =(heap other){
    swap(*this,other);
    return *this;
}

void heap::insert(route* node){
//    tree.push_back(node);
//    heapifyUp(tree.size()-1);
}

route* heap::removeMin(){//swap root and last node then re-heapify
//    std::swap(tree.at(0),tree.at(tree.size() - 1));
//    route* min = tree.at(tree.size()-1);
//    tree.pop_back();
   // print(*this);
    heapifyDown(0);

    return min;
}
//friends:
void copy(heap& source, heap &dest){
    dest.tree = source.tree;
}
void print(heap& myHeap){
    //unsigned int n = myHeap.tree.size();
    for(unsigned int i = 0;i< n;i++){
       // if(i%n == 0)
           // std::cout<<std::endl<<*(myHeap.tree.at(i))<<" , ";
       // else
            //std::cout<<*(myHeap.tree.at(i))<<" , ";
    }
}

//private:
void heap::heapifyUp(int index){
    int parent = index%2 == 0 ?(index - 2)/2:(index - 1)/2;
    route* parentNode;

//    while( parent >= 0 && *tree.at(index) < *tree.at(parent)){
//        parentNode = tree.at(parent);
//        tree.at(parent) = tree.at(index);
//        tree.at(index) = parentNode;
//        index = parent;
//        parent = index%2 == 0 ?(index-2)/2:(index -1)/2;
//    }
}

void heap::heapifyDown(int index){
    //std::cout<<"call to heapify down"<<std::endl;
    int parent = index;
    int smallest = parent;
    int l = 2*parent + 1;
    int r = 2*parent + 2;
    //int N = tree.size();
    bool swapped = true;


    //& (tree.at(parent) > tree.at(l)
    //|| tree.at(parent) > tree.at(r))
    while(parent   < N - 1 && (swapped)){

//        if (l < N && *tree.at(l) < *tree.at(smallest))
//            smallest = l;
//        if (r < N && *tree[r] < *tree[smallest])
//            smallest = r;
//        if (smallest != parent)
//            std::swap(*tree.at(smallest), *tree.at(parent));
//        else
//            swapped = false;


        parent = smallest;
        l = 2*parent + 1;
        r = 2*parent + 2;
        //print(*this);
    }
}

void swap(heap& x,heap& y){
    std::swap(x.tree,y.tree);
}
