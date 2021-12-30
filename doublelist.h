// doublelist.h

#ifndef DOUBLELIST_H
#define DOUBLELIST_H

#include <iostream>     // deklaracje strumieni cout, cin, cerr
#include <cassert>    // assert()

template <typename T>
struct DoubleNode {
// the default access mode and default inheritance mode are public
    T value;
    DoubleNode *next, *prev;
    // konstruktor domyslny (niepotrzebny)
    DoubleNode() : value(T()), next(nullptr), prev(nullptr) {}
    DoubleNode(const T& item, DoubleNode *nptr=nullptr, DoubleNode *pptr=nullptr)
        : value(item), next(nptr), prev(pptr) {} // konstruktor
    ~DoubleNode() {} // destruktor
};


template <typename T>
class DoubleList {
    DoubleNode<T> *head, *tail;
public:
    DoubleList() : head(nullptr), tail(nullptr) {}
    ~DoubleList()
    {
        for (DoubleNode<T> *node; !empty();){
        node = head->next;
        delete head;
        head = node;
    }
    } // tu trzeba wyczyscic wezly
    DoubleList(const DoubleList& other)
    {
        DoubleNode<T> *othernode = other.head;
        while(othernode != nullptr){
            if(!empty()){
                tail->next = new DoubleNode<T>(othernode->value,nullptr,tail);
                tail = tail->next;
            }
            else{
                head = tail = new DoubleNode<T>(othernode->value);
            }
            othernode = othernode->next;
        }

    } // copy constructor
    // usage:   DoubleList<int> list2(list1);
    DoubleList(DoubleList&& other); // move constructor NIEOBOWIAZKOWE
    // usage:   DoubleList<int> list2(std::move(list1));
    DoubleList& operator=(const DoubleList& other)
    {
        DoubleNode<T> *othernode = other.head;
        while(othernode != nullptr){
            if(!empty()){
                tail->next = new DoubleNode<T>(othernode->value,nullptr,tail);
                tail = tail->next;
            }
            else{
                head = tail = new DoubleNode<T>(othernode->value);
            }
            othernode = othernode->next;
        }
        return *this;
    } // copy assignment operator, return *this
    // usage:   list2 = list1;
    DoubleList& operator=(DoubleList&& other); // move assignment operator, return *this
    // usage:   list2 = std::move(list1); NIEOBOWIAZKOWE
    bool empty() const { return head == nullptr; }
    int size() const
    {
        if (empty()){
            return 0;
        }
        int i = 0;
        DoubleNode<T> *node = head;
        while (node != nullptr){
            i++;
            node = node->next;
        }
        return i;
    } // O(n) bo trzeba policzyc
    void push_front(const T& item); // O(1)
    void push_front(T&& item); // O(1) NIEOBOWIAZKOWE
    void push_back(const T& item)
    {
        if(!empty()){
            tail->next = new DoubleNode<T>(item,nullptr,tail);
            tail = tail->next;

        }
        else{
            head = tail = new DoubleNode<T>(item);
        }
    } // O(1)
    //void push_back(T&& item); // O(1) NIEOBOWIAZKOWE
    T& front() const { return head->value; } // zwraca poczatek, nie usuwa
    T& back() const { return tail->value; } // zwraca koniec, nie usuwa
    void pop_front()
    {
        assert(!empty());
        DoubleNode<T> *node = head;
        if (head == tail) { // jeden wezel na liscie
            head = tail = nullptr;
        }
        else{
            head = head->next;
            head->prev = nullptr;
        }
        delete node;
    } // usuwa poczatek O(1)
    void pop_back()
    {
        assert(!empty());
        DoubleNode<T> *node = tail; // zapamietujemy
        if (head == tail) { // jeden wezel na liscie
            head = tail = nullptr;
        }
        else{
            tail = tail->prev;
            tail->next = nullptr;
            
        }
        delete node;
    } // usuwa koniec O(n)
    void clear()
    {
        DoubleNode<T> *node = head;
        while(!empty()){
            node = head->next;
            delete head;
            head = node;
        }
    } // czyszczenie listy z elementow O(n)
    void display()
    {
        DoubleNode<T> *node = head;
        while(node != nullptr){
            std::cout << " " << node->value << " ";
            node = node->next;
        }
        std::cout <<"\n";
    } // O(n)
    void display_reversed(); // O(n)
    // Operacje z indeksami. NIEOBOWIAZKOWE
    T& operator[](int pos)
    {
        if(pos > size()){
            DoubleNode<T> *delnode = new DoubleNode<T>(-1); 
            std::cout<<"Za duzy indeks\n";
            return delnode->value;
        }
        DoubleNode<T> *node = head;
        int i = 0;
        while(i != pos){
            i++;
            node = node->next;
        }
        return node->value;
    } // podstawienie L[pos]=item
    const T& operator[](int pos) const
    {
        if(pos > size()){
            DoubleNode<T> *delnode = new DoubleNode<T>(-1); 
            std::cout<<"Za duzy indeks\n";
            return delnode->value;
        }
        DoubleNode<T> *node = head;
        int i = 0;
        while(i != pos){
            i++;
            node = node->next;
        }
        return node->value;
    } // odczyt L[pos]
    void erase(int pos)
    {
        DoubleNode<T> *node = head;
        int i = 0;
        while(i != pos){
            i++;
            node = node->next;
        }
        node->prev->next = node->next;
        delete node;
    }
    int index(const T& item)
    {
        DoubleNode<T> *node = head;
        bool sem = 0;
        int i = 0;
        while(node != nullptr){
            if(node->value == item){
                sem = 1;
                break;
            }
            i++;
            node = node->next;
        }
        if(sem){
            return i;
        }
        else{
            return -1;
        }
    } // jaki index na liscie (-1 gdy nie ma) O(n)
    void insert(int pos, const T& item)
    {
        if(pos > size()-1){
            std::cout<<"Nieprawidlowy indesk\n";
            return;
        }
        DoubleNode<T> *node = head;
        DoubleNode<T> *newnode = new DoubleNode<T>(item);
        int i = 0;
        while(i != pos){
            i++;
            node = node->next;
        }
        newnode->next = node;
        newnode->prev = node->prev;
        node->prev->next = newnode; // 0 1 2 L 3 4
        node->prev = newnode;
    } // inserts item before pos,
    void insert(int pos, T&& item)
    {
        if(pos == 0){
            push_front(std::move(item));
            return;
        }
        if(pos == size()){
            push_back(std::move(item));
            return;
        }
        if(pos > size()-1){
            std::cout<<"Nieprawidlowy indesk\n";
            return;
        }
        std::cout<<"Jest item  "<< item<<"\n";
        DoubleNode<T> *node = head;
        DoubleNode<T> *newnode = new DoubleNode<T>(std::move(item));
        int i = 0;
        while(i != pos){
            i++;
            node = node->next;
        }
        newnode->next = node;
        newnode->prev = node->prev;
        node->prev->next = newnode; // 0 1 2 L 3 4
        node->prev = newnode;

    } // inserts item before pos,
    // Jezeli pos=0, to wstawiamy na poczatek.
    // Jezeli pos=size(), to wstawiamy na koniec.
};

template <typename T>
void DoubleList<T>::push_front(const T& item) {
    if (!empty()) {
        head = new DoubleNode<T>(item, head);
        head->next->prev = head;
    } else {
        head = tail = new DoubleNode<T>(item);
    }
}

template <typename T>
void DoubleList<T>::push_front(T&& item) {
    if (!empty()) {
        head = new DoubleNode<T>(std::move(item), head);
        head->next->prev = head;
    } else {
        head = tail = new DoubleNode<T>(std::move(item));
    }
}

#endif

// EOF