/*
    Singly-Linked List
*/

#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <initializer_list>
#include <string>
#include <iostream>

template <typename T>
class SingleLinkedList {
private:
    struct Node {
        T val;
        Node *next;
        Node(const T &value, Node *n = nullptr) :
            val(value), next(n) {
        }
    };

    // 不带头节点（哨兵）
    Node *head;
    Node *tail;
    size_t length;

public:
    // 迭代器相关类型定义
    using value_type = T;
    using reference = T &;
    using const_reference = const T &;
    using pointer = T *;
    using const_pointer = const T *;
    using size_type = size_t;
    using difference_type = std::ptrdiff_t;

    // 正向迭代器
    class iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using defference_type = std::ptrdiff_t;
        using pointer = T *;
        using reference = T &;

        iterator(Node *p = nullptr) :
            current(p) {
        }
        reference operator*() const;
        pointer operator->();
        iterator &operator++();
        iterator operator++(int);
        bool operator==(const iterator &other) const;
        bool operator!=(const iterator &other) const;

    private:
        Node *current;
    };

    class const_iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = const T;
        using difference_type = std::ptrdiff_t;
        using pointer = const T *;
        using reference = const T &;

        const_iterator(const Node *p = nullptr) :
            current(p) {
        }
        reference operator*() const;
        pointer operator->() const;
        const_iterator &operator++();
        const_iterator operator++(int);
        bool operator==(const const_iterator &other) const;
        bool operator!=(const const_iterator &other) const;

    private:
        const Node *current;
    };

    // 构造函数
    SingleLinkedList();
    SingleLinkedList(std::initializer_list<T> init);     // 允许 ={1，2，3}
    SingleLinkedList(const SingleLinkedList &other);     // 拷贝构造
    SingleLinkedList(SingleLinkedList &&other) noexcept; // 移动构造

    // 析构函数
    ~SingleLinkedList();

    // 赋值运算符
    SingleLinkedList &operator=(const SingleLinkedList &other);     // 拷贝赋值
    SingleLinkedList &operator=(SingleLinkedList &&other) noexcept; // 移动赋值

    // 容量
    size_type size() const noexcept;
    bool empty() const noexcept;

    // 访问
    reference front();
    reference back();
    reference at(size_t index);
    reference operator[](size_t index);
    const_reference front() const;
    const_reference back() const;
    const_reference at(size_t index) const;
    const_reference operator[](size_t index) const;

    // 修改
    void push_front(const T &value);
    void push_back(const T &value);
    void pop_front();
    void pop_back();
    iterator insert(size_t index, const T &value);
    iterator erase(size_t index);
    void clear() noexcept;

    // 查找
    iterator find(const T &value);
    const_iterator find(const T &value) const;
    bool contains(const T &value) const;

    // 迭代器
    iterator begin() noexcept;
    iterator end() noexcept;
    const_iterator begin() const noexcept;
    const_iterator end() const noexcept;
    const_iterator cbegin() const noexcept;
    const_iterator cend() const noexcept;

    // 特殊操作
    void reverse() noexcept;
    void remove_value(const T &value);
};

#include "SingleLinkedList.tpp"
#endif