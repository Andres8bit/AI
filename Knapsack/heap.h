#ifndef HEAP_H
#define HEAP_H
#include<vector>
#include<iostream>

template<typename T>
class heap{
public:
    heap(){tree = std::vector<T*>();}
    ~heap();
    heap(std::vector<T>& vector);
    heap(heap &other){copy(other,*this);}
    heap& operator=(heap<T> other);
    //setters:
    void insert(T add);
    T remove_max();
    void clear(){tree.clear();}
    //getters:
    unsigned int size()const{return tree.size();}
    //friends:
    template<typename K>
    friend void swap(heap<K>& x,heap<K>& y);
    template<typename K>
    friend void copy(heap<K>& source ,heap<K>& dest);
    template<typename K>
    friend void print(heap<K>& myHeap);
    template<typename K>
    friend heap<K>& operator+(heap<K>& x,heap<K> y);
    template<typename K>
    friend void merge(heap<K> source,heap<K>& dest);
private:
    std::vector<T*> tree;
    void heapify_up(int index);
    void heapify_down(int index);
};
template<typename T>
heap<T>::heap(std::vector<T>& vector){
    unsigned int N = vector.size() -1;
    for(unsigned int i = 0; i < N;i++)
        this->insert(vector.at(i));
}
template<typename T>
heap<T>::~heap<T>(){
    unsigned int N = tree.size();
    for(unsigned int i = 0; i < N;i++){
        delete tree[i];
    }
   tree.clear();
}
template<typename T>
heap<T>& heap<T>::operator=(heap<T> other){
    swap(*this,other);
    return *this;
}
template<typename T>
void heap<T>::insert(T add){
    T* temp = new T(add);
    tree.push_back(temp);
    heapify_up(tree.size()-1);
}
template<typename T>
T heap<T>::remove_max(){
    std::swap(tree.at(0),tree.at(tree.size() - 1));//swap root and last node
    T* max = tree.at(tree.size() - 1);
    tree.pop_back();
    heapify_down(0);
    return *max;
}
//private:
template<typename T>
void heap<T>::heapify_up(int index){
    int parent = index%2 == 0 ?(index - 2)/2:(index - 1)/2;
    T* parent_node;
    while(parent >= 0 && *(tree.at(index)) > *(tree.at(parent))){
        parent_node = tree.at(parent);
        tree.at(parent) = tree.at(index);
        tree.at(index) = parent_node;
        index = parent;
        parent = index%2 == 0 ?(index-2)/2:(index -1)/2;
    }
}
template<typename T>
void heap<T>::heapify_down(int index){
    int parent = index;
    int largest = parent;
    int l = 2*parent + 1;
    int r = 2*parent + 2;
    int N = tree.size();
    bool swapped = true;
    while(parent   < N - 1 && swapped){
        if (l < N && *(tree[l]) > *(tree[largest]))
            largest = l;
        if (r < N && *(tree[r]) > *(tree[largest]))
            largest = r;
        if (largest != parent){
            std::swap(*(tree.at(largest)), *(tree.at(parent)));
            swapped = true;
        }
        else
            swapped = false;
        parent = largest;
        l = 2*parent + 1;
        r = 2*parent + 2;
    }
}
//friends:
template<typename K>
void swap(heap<K>& x,heap<K>& y){
    std::swap(x.tree,y.tree);
}
template<typename K>
void copy(heap<K>& source, heap<K> &dest){
    dest.tree = source.tree;
}
template<typename K>
void print(heap<K>& my_heap){
    unsigned int n = my_heap.tree.size();
    for(unsigned int i = 0;i< n;i++){
        if(i%n == 0)
            std::cout<<std::endl<<*(my_heap.tree.at(i))<<" , ";
        else
            std::cout<<*(my_heap.tree.at(i))<<" , ";
    }
}
template<typename K>
heap<K>& operator+(heap<K>& x,heap<K> y){
    heap<K> temp;
    unsigned int N = x.size() - 1;
    for(unsigned int i = 0; i < N;i++)temp.insert(x.remove_min());
    N = y.size() - 1;
    for(unsigned int i = 0; i < N;i++)temp.insert(y.remove_min());
    return temp;
}
template<typename K>
void merge(heap<K> source,heap<K>& dest){
    unsigned int N = source.size();
    for(unsigned int i = 0;i< N;i++)
        dest.insert(source.remove_min());
}
#endif // HEAP_H
