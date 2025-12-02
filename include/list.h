#pragma once

#include <cstddef>          // for size_t
#include <initializer_list> // for std::initializer_list
#include <compare>          // C++20: for operator <=>
#include <concepts>         // C++20: for requires
#include <iterator>         // for std::bidirectional_iterator_tag
#include <memory>           // for std::allocator (optional, advanced challenge)
#include <utility>          // for std::move, std::forward

namespace mys {

// C++20: Define a Concept to constrain that types stored in the list must be movable and destructible
// More constraints can be added as needed, such as std::equality_comparable
template <typename T>
concept Listable = std::movable<T> && std::destructible<T>;

template <Listable T, typename Allocator = std::allocator<T>>
class list {
private:
    // Internal node structure
    struct Node {
        T val;
        Node *prev = nullptr;
        Node *next = nullptr;

        // Perfect Forwarding
        template <typename... Args>
        Node(Args &&...args) : val(std::forward<Args>(args)...) {}
    };

    using NodeAlloc = typename std::allocator_traits<Allocator>::template rebind_alloc<Node>;
    [[no_unique_address]] NodeAlloc allocator_;

    Node *head = nullptr;
    Node *tail = nullptr;
    std::size_t length = 0;

public:
    // ===========================================================
    // 1. Iterator Implementation (Challenge: Compliant with C++20 Iterator Concepts)
    // ===========================================================
    template <bool IsConst>
    class ListIterator {
    private:
        using NodePtr = std::conditional_t<IsConst, const Node *, Node *>;
        NodePtr current_ = nullptr;
        const list *list_ = nullptr;

        friend class list;
        friend class ListIterator<!IsConst>;

    public:
        // Necessary type definitions so STL algorithms can recognize this iterator
        using iterator_category = std::bidirectional_iterator_tag;
        using iterator_concept = std::bidirectional_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = std::conditional_t<IsConst, const T *, T *>;
        using reference = std::conditional_t<IsConst, const T &, T &>;

        ListIterator() = default;
        explicit ListIterator(NodePtr node, const list *l = nullptr) : current_(node), list_(l){};
        ListIterator(const ListIterator &) = default;

        // 允许从 iterator (IsConst=false) 隐式转换为 const_iterator (IsConst=true)
        // 使用 requires IsConst 确保这个构造函数只在当前是 const_iterator 时存在，
        // 避免干扰 iterator 自身的默认拷贝构造函数。
        ListIterator(const ListIterator<false> &other)
            requires IsConst
            : current_(other.current_), list_(other.list_) {}

        reference operator*() const { return current_->val; }

        pointer operator->() const { return &(current_->val); }

        ListIterator &operator++() {
            if (current_) current_ = current_->next;
            return *this;
        }

        ListIterator operator++(int) {
            ListIterator temp = *this;
            if (current_) current_ = current_->next;
            return temp;
        }

        ListIterator &operator--() {
            if (current_) {
                current_ = current_->prev;
            } else if (list_) {
                current_ = list_->tail;
            }
            return *this;
        }

        ListIterator operator--(int) {
            ListIterator temp = *this;
            --(*this);
            return temp;
        }

        // bool operator==(const ListIterator &other) const = default;

        // 使用 hidden friend 替换原本的 default member operator==
        // 这能完美解决 C++20 的 ambiguity 问题
        friend bool operator==(const ListIterator &lhs, const ListIterator &rhs) {
            return lhs.current_ == rhs.current_;
        }
    };

    using iterator = ListIterator<false>;
    using const_iterator = ListIterator<true>;

    // ===========================================================
    // 2. Construction and Destruction (Lifecycle Management)
    // ===========================================================

    list() = default;
    list(std::initializer_list<T> init);
    list(const list &other);
    list(list &&other) noexcept;
    list &operator=(const list &other);
    list &operator=(list &&other) noexcept;
    ~list();

    // ===========================================================
    // 3. Element Access
    // ===========================================================

    // C++23: Deducing this
    template <typename Self>
    auto &&front(this Self &&self);
    template <typename Self>
    auto &&back(this Self &&self);

    // [[nodiscard]] T &front();
    // [[nodiscard]] const T &front() const;
    // [[nodiscard]] T &back();
    // [[nodiscard]] const T &back() const;

    // ===========================================================
    // 4. Capacity Query
    // ===========================================================

    [[nodiscard]] bool empty() const noexcept;
    [[nodiscard]] std::size_t size() const noexcept;

    // ===========================================================
    // 5. Modifiers
    // ===========================================================

    void clear() noexcept;
    void swap(list &other) noexcept;
    // void resize(size_t count);
    // void resize(size_t count, const T &value);

    void push_back(const T &value);
    void push_back(T &&value);
    void push_front(const T &value);
    void push_front(T &&value);

    // C++11: In-place construction (Variadic Templates)
    // The Args&&... here need to be perfectly forwarded to Node's constructor
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
    // 6. Iterator Interface
    // ===========================================================

    template <typename Self>
    auto begin(this Self &&self) noexcept;
    const_iterator cbegin() const noexcept;

    template <typename Self>
    auto end(this Self &&self) noexcept;
    const_iterator cend() const noexcept;

    // Reverse iterators (std::reverse_iterator adapter)
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    template <typename Self>
    auto rbegin(this Self &&self) noexcept;
    const_reverse_iterator crbegin() const noexcept;
    template <typename Self>
    auto rend(this Self &&self) noexcept;
    const_reverse_iterator crend() const noexcept;

    // ===========================================================
    // 7. C++20 Comparison Operations (Spaceship Operator)
    // ===========================================================

    // C++20: Implement three-way comparison
    // Requires T to also support <=>, otherwise needs to fall back to regular comparison
    std::strong_ordering operator<=>(const list &other) const;
    bool operator==(const list &other) const;

    // ===========================================================
    // 8. Other Operations
    // ===========================================================

    template <typename... Args>
    Node *create_node(Args &&...args);
    void destroy_node(Node *ptr);
};

// External swap function, for ADL (Argument Dependent Lookup)
template <Listable T>
void swap(list<T> &lhs, list<T> &rhs) noexcept {
    lhs.swap(rhs);
}

} // namespace mys

#include "list.tpp"