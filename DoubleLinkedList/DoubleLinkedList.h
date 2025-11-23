#pragma once

#ifndef DOUBLE_LINKED_LIST_H
#define DOUBLE_LINKED_LIST_H

#include <cstddef>          // for size_t
#include <initializer_list> // for std::initializer_list
#include <compare>          // C++20: for operator <=>
#include <concepts>         // C++20: for requires
#include <iterator>         // for std::bidirectional_iterator_tag
#include <memory>           // for std::allocator (可选，进阶挑战)
#include <utility>          // for std::move, std::forward
// 需兼容 std::ranges

namespace mys {

// C++20: 定义一个 Concept，约束存入链表的类型必须是可移动且可析构的
// 可以根据需要增加更多约束，比如 std::equality_comparable
template <typename T>
concept Listable = std::movable<T> && std::destructible<T>;

template <Listable T>
class DoubleLinkedList {
private:
    // 内部节点结构
    struct Node {
        T data;
        Node *prev = nullptr;
        Node *next = nullptr;

        // Perfect Forwarding
        template <typename... Args>
        Node(Args &&...args) :
            data(std::forward<Args>(args)...) {
        }
    };

    Node *head = nullptr;
    Node *tail = nullptr;
    std::size_t size_ = 0;

public:
    // ===========================================================
    // 1. 迭代器实现 (挑战：符合 C++20 Iterator Concepts)
    // ===========================================================
    template <bool IsConst>
    class ListIterator {
    private:
        Node *current_ = nullptr;
        friend class DoubleLinkedList;

    public:
        // 必要的类型定义，使STL算法能识别该迭代器
        using iterator_category = std::bidirectional_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = std::conditional_t<IsConst, const T *, T *>;
        using reference = std::conditional_t<IsConst, const T *, T *>;

        ListIterator() = default;
        explicit ListIterator(Node *node);

        reference operator*() const;
        pointer operator->() const;

        ListIterator &operator++();
        ListIterator &operator++(int);
        ListIterator &operator--();
        ListIterator &operator--(int);
        bool operator==(const ListIterator &other) const = default;
    };

    using iterator = ListIterator<false>;
    using const_iterator = ListIterator<true>;

    // ===========================================================
    // 2. 构造与析构 (生命周期管理)
    // ===========================================================

    DoubleLinkedList(const DoubleLinkedList &other);
    DoubleLinkedList(DoubleLinkedList &&other) noexcept;
    DoubleLinkedList &operator=(const DoubleLinkedList &other);
    DoubleLinkedList &operator=(DoubleLinkedList &&other) noexcept;
    ~DoubleLinkedList();

    // ===========================================================
    // 3. 元素访问
    // ===========================================================

    // C++23: Deducing this
    template <typename Self>
    auto &&front(this Self &&self);
    template <typename Sefl>
    auto &&back(this Self &&self);

    // [[nodiscard]] T &front();
    // [[nodiscard]] const T &front() const;
    // [[nodiscard]] T &back();
    // [[nodiscard]] const T &back() const;

    // ===========================================================
    // 4. 容量查询
    // ===========================================================

    [[nodiscard]] bool empty() const noexcept;
    [[nodiscard]] std : size_t size() const noexcept;

    // ===========================================================
    // 5. 修改器
    // ===========================================================

    void clear() noexcept;

    void push_back(const T &value);
    void push_back(T &&value);
    void push_front(const T &value);
    void push_front(T &&value);

    // C++11: 原地构造 (Variadic Templates)
    // 这里的 Args&&... 需要完美转发给 Node 的构造函数
    template <typename... Args>
    void emplace_back(Args &&...args);
    template <typename... Args>
    void emplace_front(Args &&...args);

    void pop_back();
    void pop_front();

    iterator insert(const_iterator pos, const T &value);
    iterator insert(const_iterator pos, T &&value);

    template <typename... Args>
    iterator emplace(const_iterator pos, Args &&...args);

    iterator erase(const_iterator pos);
    iterator erase(const_iterator first, const_iterator last);

    // ===========================================================
    // 6. 迭代器接口
    // ===========================================================

    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    const_iterator cbegin() const noexcept;

    iterator end() noexcept;
    const_iterator end() const noexcept;
    const_iterator cend() const noexcept;

    // 反向迭代器 (std::reverse_iterator 适配器)
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    reverse_iterator rbegin() noexcept;
    const_reverse_iterator crbegin() const noexcept;
    reverse_iterator rend() noexcept;
    const_reverse_iterator crend() const noexcept;

    // ===========================================================
    // 7. C++20 比较操作 (Spaceship Operator)
    // ===========================================================

    // 自动生成 == 和 !=
    bool operator==(const DoubleLinkedList &other) const;

    // C++20: 实现三路比较
    // 需要 T 也支持 <=>，否则需要退化为普通比较
    std::strong_ordering operator<=>(const DoubleLinkedList &other) const;

    // ===========================================================
    // 8. 其他操作
    // ===========================================================

    void swap(DoubleLinkedList &other) noexcept;
};

// 外部 swap 函数，用于 ADL (Argument Dependent Lookup)
template <Listable T>
void swap(DoubleLinkedList<T> &lhs, DoubleLinkedList<T> &rhs) noexcept {
    lhs.swap(rhs);
}

} // namespace mys

#include "DoubleLinkedList.tpp"
#endif