/*
    Singly-Linked List 
*/ 

#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include<initializer_list>
#include<string>
#include<iostream>

template <typename T>
class LinkedList{
private:
    struct Node
    {
        T val;
        Node *next;
        Node(const T &value, Node *n = nullptr) : val(value), next(n) {}
    };

    Node *head;
    Node *tail;
    size_t length;

public:
    using value_type = T;
    using reference = T &;
    using const_reference = const T &;
    using pointer = T &;
    using const_pointer = constT *;
    using size_type = size_t;
    using defference_type = std::ptrdiff_t;

    class iterator{
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using defference_type = std::ptrdiff_t;
        using pointer = T *;
        using reference = T &;

        iterator(Node *p = nullptr) : current(p) {}
        reference operator*() const;
        pointer operator->();
        iterator &operator++();
        iterator operator++(int);
        bool operator==(const iterator &other) const;
        bool operator!=(const iterator &other) const;

    private:
        Node *current;
    }

    class const_iterator{
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = const T;
        using difference_type = std::ptrdiff_t;
        using pointer = const T *;
        using reference = const T &;

        const_iterator(const Node *p = nullptr) : current(p) {}
        reference operator*() const;
        pointer operator->() const;
        const_iterator &operator++();
        const_iterator operator++(int);
        bool operator==(const const_iterator &other) const;
        bool operator!=(const const_iterator &other) const;

    private:
        const Node *current;
    }

    LinkedList();
    // LinkedList(const T &value);
    // LinkedList(const T &value, const LinkedList &other);
    LinkedList(std::initializer_list<T> init);  // 允许 ={1，2，3}
    LinkedList(const LinkedList &other);        // 拷贝构造
    LinkedList(LinkedList &&other) noexcept;    // 移动构造

    ~LinkedList();
    
    LinkedList &operator=(const LinkedList &other);     //拷贝赋值
    LinkedList &operator=(LinkedList &&other) noexcept; //移动赋值

    size_type size() const noexcept;
    bool empty() const noexcept;

    reference front();
    reference back();
    reference at(size_t index);
    reference operator[](size_t index);
    const_reference front() const;
    const_reference back() const;    
    const_reference at(size_t index) const;   
    const_reference operator[](size_t index) const;

    void push_front(const T &value);
    void push_back(const T &value);
    void pop_front();
    void pop_back();
    iterator insert(size_t index, const T &value);
    iterator erase(size_t index);
    void clear() noexcept;


    // 暂不考虑过长链表
    iterator find(const T &value) ;
    const_iterator find(const T &value) const;
    bool contains(const T &value) const;
    
    // TODO
    iterator begin() noexcept;
    iterator end() noexcept;
    const_iterator begin() const noexcept;
    const_iterator end() const noexcept;
    const_iterator cbegin() const noexcept;
    const_iterator cend() const noexcept;

    void reverse() noexcept;
    void remove_value(const T &value);
};

#endif