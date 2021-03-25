#ifndef ITEM_H
#define ITEM_H
#include<iostream>

class Item
{
public:
    Item(int weight = 0,int profit = 0,int bound = 0, unsigned int level = 0);
    ~Item(){w = p = b = 0;}
    //Item (Item& other);
    Item& operator=(Item other);

    void set_weight(int val = 0){w = val;}
    void set_profit(int val = 0){p = val;}
    void set_lvl(unsigned int val){lvl = val;}
    void set_bound(int val){b = val;}

    int weight()const{return w;}
    int profit()const{return p;}
    int ratio()const{return w > 0 ? p/w :0;}
    int bound()const{return b;}
    unsigned int level()const{return lvl;}


    friend
    bool operator ==(const Item& x,const Item& y){return x.ratio() == y.ratio();}
    friend
    bool operator !=(const Item& x,const Item& y){return !(x == y);}
    friend
    bool operator<(const Item& x,const Item& y){return x.ratio() < y.ratio();}
    friend
    bool operator>(const Item& x,const Item& y){return x.ratio() > y.ratio();}
    friend
    bool operator>=(const Item& x,const Item& y){return x > y || x == y;}
    friend
    bool operator<=(const Item& x,const Item& y){return x < y || x == y;}
    friend
    void swap(Item& x, Item& y);
    friend
    void print(Item& x);
private:
    int w;
    int p;
    int b;
    unsigned int lvl;

    void copy(Item& other);
};

#endif // ITEM_H
