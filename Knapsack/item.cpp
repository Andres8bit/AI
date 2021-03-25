#include "item.h"

Item::Item(int weight, int profit, int bound, unsigned int level)
{
    w = weight;
    p = profit;
    lvl = level;
    b = bound;
}
//Item::Item(Item& other)
//{
//    copy(other);
//}

Item& Item::operator=(Item other)
{
    swap(*this,other);
    return *this;
}

void Item::copy(Item& other)
{
    this->w = other.weight();
    this->p = other.profit();
    this->b = other.bound();
    this->lvl = other.level();
}

void swap(Item& x, Item& y){
    std::swap(x.p,y.p);
    std::swap(x.w,y.w);
    std::swap(x.b,y.b);
    std::swap(x.lvl,y.lvl);
}
void print(Item& x)
{
    std::cout<<"weight:"<<x.weight()<<std::endl<<"profit:"<<x.profit()
            <<std::endl<<"ratio:"<<x.ratio()<<std::endl<<"bound:"<<x.bound()<<std::endl<<"level:"<<x.level()<<std::endl;
}
