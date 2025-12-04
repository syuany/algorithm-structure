#pragma once

#include <cstddef>
#include <initializer_list>
#include <compare>
#include <concepts>
#include <iterator>
#include <memory>
#include <utility>

namespace mys {

template <typename T>
concept ForwardListable = std::movable<T> && std::destructible<T>;

template <ForwardListable T, typename Allocator = std::allocator<T>>
class forward_list {
private:
    // 基础节点（仅包含指针，用作哨兵）
    struct NodeBase {
        NodeBase *next = nullptr;
    };

    // 数据节点（继承自 NodeBase，包含数据）
    struct Node : public NodeBase {
        T val;
        template <typename... Args>
        Node(Args &&...args) : val(std::forward<Args>(args)...) {}
    };

    using NodeAlloc = typename std::allocator_traits<Allocator>::template rebind_alloc<Node>;
    [[no_unique_address]] NodeAlloc allocator_;

    // 哨兵节点
    NodeBase head_;
    std::size_t length_ = 0;

public:
    // ===========================================================
    // 1. Iterator Implementation
    // ===========================================================
    template <bool IsConst>
    class ForwardListIterator {
    private:
        // 迭代器内部持有的是 Base 指针，这样它可以指向 head_ (哨兵)
        using NodeBasePtr = std::conditional_t<IsConst, const NodeBase *, NodeBase *>;
        NodeBasePtr current_ = nullptr;

        friend class forward_list;
        friend class ForwardListIterator<!IsConst>;

    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using pointer = std::conditional_t<IsConst, const T *, T *>;
        using reference = std::conditional_t<IsConst, const T &, T &>;
        using difference_type = std::ptrdiff_t;

        ForwardListIterator() = default;
        explicit ForwardListIterator(NodeBasePtr node) : current_(node) {}

        ForwardListIterator(const ForwardListIterator &) = default;

        ForwardListIterator(const ForwardListIterator<false> &other)
            requires IsConst
            : current_(other.current_) {}

        reference operator*() const {
            // 安全性说明：永远不要对 before_begin() 进行解引用
            // static_cast 是安全的，因为除了 head_ 哨兵外，其他节点都是 Node
            return static_cast<std::conditional_t<IsConst, const Node *, Node *>>(current_)->val;
        }
        pointer operator->() const { return &(this->operator*()); }

        ForwardListIterator &operator++() {
            if (current_) current_ = current_->next;
            return *this;
        }

        ForwardListIterator operator++(int) {
            ForwardListIterator temp = *this;
            ++(*this);
            return temp;
        }

        friend bool operator==(const ForwardListIterator &lhs, const ForwardListIterator &rhs) { return lhs.current_ == rhs.current_; }
    };

    using iterator = ForwardListIterator<false>;
    using const_iterator = ForwardListIterator<true>;

    // ===========================================================
    // 2. Construction and Destruction
    // ===========================================================
    forward_list();
    forward_list(std::initializer_list<T> init);
    forward_list(const forward_list &other);
    forward_list(forward_list &&other) noexcept;
    forward_list &operator=(const forward_list &other);
    forward_list &operator=(forward_list &&other) noexcept;
    ~forward_list();

    // ===========================================================
    // 3. Element Access
    // ===========================================================
    template <typename Self>
    auto &&front(this Self &&self);

    // ===========================================================
    // 4. Capacity Query
    // ===========================================================
    [[nodiscard]] bool empty() const noexcept;
    [[nodiscard]] std::size_t size() const noexcept;

    // ===========================================================
    // 5. Modifiers
    // ===========================================================
    void clear() noexcept;
    void swap(forward_list &other) noexcept;

    void push_front(const T &value);
    void push_front(T &&value);

    template <typename... Args>
    void emplace_front(Args &&...args);

    void pop_front();

    iterator insert_after(const_iterator pos, const T &value);
    iterator insert_after(const_iterator pos, T &&value);

    template <typename... Args>
    iterator emplace_after(const_iterator pos, Args &&...args);

    iterator erase_after(const_iterator pos);
    iterator erase_after(const_iterator first, const_iterator last);

    // ===========================================================
    // 6. Iterator Interface
    // ===========================================================
    template <typename Self>
    auto before_begin(this Self &&self) noexcept;
    const_iterator cbefore_begin() const noexcept;

    template <typename Self>
    auto begin(this Self &&self) noexcept;
    const_iterator cbegin() const noexcept;

    template <typename Self>
    auto end(this Self &&self) noexcept;
    const_iterator cend() const noexcept;

    // ===========================================================
    // 7. Operations
    // ===========================================================
    void splice_after(const_iterator pos, forward_list &other);
    void splice_after(const_iterator pos, forward_list &&other);
    void splice_after(const_iterator pos, forward_list &other, const_iterator it);
    void splice_after(const_iterator pos, forward_list &other, const_iterator first, const_iterator last);

    void remove(const T &value);
    template <typename Predicate>
    void remove_if(Predicate pred);

    void unique();
    template <typename BinaryPredicate>
    void unique(BinaryPredicate pred);

    // void sort();
    // template <typename Compare>
    // void sort(Compare comp);

    void reverse() noexcept;

    // ===========================================================
    // 8. Comparison Operators (C++20)
    // ===========================================================
    std::strong_ordering operator<=>(const forward_list &other) const;
    bool operator==(const forward_list &other) const;

    // Helper functions
    template <typename... Args>
    Node *create_node(Args &&...args);
    void destroy_node(Node *ptr);

    // 获取 NodeBase* 的非 const 版本，用于 erase_after 等操作
    NodeBase *get_node_base(const_iterator it) {
        // const_cast 是安全的，因为我们只在非 const 成员函数中调用此函数修改链表结构
        return const_cast<NodeBase *>(it.current_);
    }
};

// External swap function
template <ForwardListable T>
void swap(forward_list<T> &lhs, forward_list<T> &rhs) noexcept {
    lhs.swap(rhs);
}

} // namespace mys

#include "forward_list.tpp"