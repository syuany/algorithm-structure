#include "list.h"
#include <initializer_list>
#include <memory>

namespace mys {

// ===========================================================
// 2. Construction and Destruction (Lifecycle Management)
// ===========================================================

template <Listable T, typename Allocator>
list<T, Allocator>::list(std::initializer_list<T> init) : list() {
    for (auto &x : init) {
        push_back(x);
    }
}

template <Listable T, typename Allocator>
list<T, Allocator>::list(const list &other) : list() {
    for (const auto &item : other) {
        push_back(item);
    }
}

template <Listable T, typename Allocator>
list<T, Allocator>::list(list &&other) noexcept :
    head(other.head), tail(other.tail), length(other.length), allocator_(std::move(other.allocator_)) {
    other.head = nullptr;
    other.tail = nullptr;
    other.length = 0;
}

template <Listable T, typename Allocator>
list<T, Allocator> &list<T, Allocator>::operator=(const list &other) {
    if (this != &other) {
        clear();
        for (const auto &item : other) {
            push_back(item);
        }
    }
    return *this;
}

template <Listable T, typename Allocator>
list<T, Allocator> &list<T, Allocator>::operator=(list &&other) noexcept {
    if (this != &other) {
        clear();
        head = other.head;
        tail = other.tail;
        length = other.length;
        other.head = nullptr;
        other.tail = nullptr;
        other.length = 0;
    }
    return *this;
}

template <Listable T, typename Allocator>
list<T, Allocator>::~list() {
    clear();
}

// ===========================================================
// 3. Element Access
// ===========================================================

template <Listable T, typename Allocator>
template <typename Self>
auto &&list<T, Allocator>::front(this Self &&self) {
    if (self.empty()) {
        throw std::out_of_range("empty");
    }
    using ReturnType = std::conditional_t<std::is_const_v<std::remove_reference_t<Self>>, const T &, T &>;
    return static_cast<ReturnType>(self.head->val);
    // #include<utility>
    // return std::forward_like<Self>(self.head->val);
}

template <Listable T, typename Allocator>
template <typename Self>
auto &&list<T, Allocator>::back(this Self &&self) {
    if (self.empty()) {
        throw std::out_of_range("empty");
    }
    using ReturnType = std::conditional_t<std::is_const_v<std::remove_reference_t<Self>>, const T &, T &>;
    return static_cast<ReturnType>(self.tail->val);
}

// ===========================================================
// 4. Capacity Query
// ===========================================================

template <Listable T, typename Allocator>
bool list<T, Allocator>::empty() const noexcept {
    return length == 0;
}

template <Listable T, typename Allocator>
std::size_t list<T, Allocator>::size() const noexcept {
    return length;
}

// ===========================================================
// 5. Modifiers
// ===========================================================

template <Listable T, typename Allocator>
void list<T, Allocator>::clear() noexcept {
    while (head) {
        Node *cur = head;
        head = head->next;
        destroy_node(cur);
    }
    tail = nullptr;
    length = 0;
}

template <Listable T, typename Allocator>
void list<T, Allocator>::swap(list &other) noexcept {
    std::swap(head, other.head);
    std::swap(tail, other.tail);
    std::swap(length, other.length);
    std::swap(allocator_, other.allocator_);
}

template <Listable T, typename Allocator>
void list<T, Allocator>::push_back(const T &value) {
    // Node *p = new Node(value);
    Node *p = create_node(value);
    if (head == nullptr) {
        head = p;
        tail = p;
    } else {
        tail->next = p;
        p->prev = tail;
        tail = p;
    }
    length++;
}

template <Listable T, typename Allocator>
void list<T, Allocator>::push_back(T &&value) {
    // Node *p = new Node(value);
    Node *p = create_node(std::move(value));
    if (head == nullptr) {
        head = p;
        tail = p;
    } else {
        tail->next = p;
        p->prev = tail;
        tail = p;
    }
    length++;
}

template <Listable T, typename Allocator>
void list<T, Allocator>::push_front(const T &value) {
    // Node *p = new Node(value);
    Node *p = create_node(value);
    if (head == nullptr) {
        head = p;
        tail = p;
    } else {
        p->next = head;
        head->prev = p;
        head = p;
    }
    length++;
}

template <Listable T, typename Allocator>
void list<T, Allocator>::push_front(T &&value) {
    // Node *p = new Node(value);
    Node *p = create_node(std::move(value));
    if (head == nullptr) {
        head = p;
        tail = p;
    } else {
        p->next = head;
        head->prev = p;
        head = p;
    }
    length++;
}

template <Listable T, typename Allocator>
template <typename... Args>
void list<T, Allocator>::emplace_back(Args &&...args) {
    Node *p = create_node(std::forward<Args>(args)...);
    if (head == nullptr) {
        head = p;
        tail = p;
    } else {
        tail->next = p;
        p->prev = tail;
        tail = p;
    }
    length++;
}

template <Listable T, typename Allocator>
template <typename... Args>
void list<T, Allocator>::emplace_front(Args &&...args) {
    Node *p = create_node(std::forward<Args>(args)...);
    if (head == nullptr) {
        head = p;
        tail = p;
    } else {
        p->next = head;
        head->prev = p;
        head = p;
    }
    length++;
}

template <Listable T, typename Allocator>
void list<T, Allocator>::pop_back() {
    if (!tail) return;
    Node *p = tail;
    tail = tail->prev;
    if (tail)
        tail->next = nullptr;
    else
        head = nullptr;
    destroy_node(p);
    length--;
}

template <Listable T, typename Allocator>
void list<T, Allocator>::pop_front() {
    if (!head) return;
    Node *p = head;
    head = head->next;
    if (head)
        head->prev = nullptr;
    else
        tail = nullptr;
    destroy_node(p);
    length--;
}

template <Listable T, typename Allocator>
list<T, Allocator>::iterator list<T, Allocator>::insert(const_iterator pos, const T &value) {
    if (pos == begin()) {
        emplace_front(value);
        return begin();
    } else if (pos == end()) {
        emplace_back(value);
        return iterator(tail, this);
    } else {
        Node *newnode = create_node(value);
        Node *cur = const_cast<Node *>(pos.current_);
        Node *pre = pos.current_->prev;
        cur->prev = newnode;
        newnode->next = cur;
        pre->next = newnode;
        newnode->prev = pre;
        length++;
        return iterator(newnode, this);
    }
}

template <Listable T, typename Allocator>
list<T, Allocator>::iterator list<T, Allocator>::insert(const_iterator pos, T &&value) {
    if (pos == begin()) {
        emplace_front(std::move(value));
        return begin();
    } else if (pos == end()) {
        emplace_back(std::move(value));
        return iterator(tail, this);
    } else {
        Node *newnode = create_node(std::move(value));
        Node *cur = const_cast<Node *>(pos.current_);
        Node *pre = pos.current_->prev;
        cur->prev = newnode;
        newnode->next = cur;
        pre->next = newnode;
        newnode->prev = pre;
        length++;
        return iterator(newnode, this);
    }
}

// emplace_front
template <Listable T, typename Allocator>
template <typename... Args>
list<T, Allocator>::iterator list<T, Allocator>::emplace(const_iterator pos, Args &&...args) {
    if (pos == begin()) {
        emplace_front(std::forward<Args>(args)...);
        return begin();
    } else if (pos == end()) {
        emplace_back(std::forward<Args>(args)...);
        return iterator(tail, this);
    } else {
        Node *newnode = create_node(std::forward<Args>(args)...);
        Node *cur = const_cast<Node *>(pos.current_);
        Node *pre = cur->prev;
        cur->prev = newnode;
        newnode->next = cur;
        pre->next = newnode;
        newnode->prev = pre;
        length++;
        return iterator(newnode, this);
    }
}

template <Listable T, typename Allocator>
list<T, Allocator>::iterator list<T, Allocator>::erase(const_iterator pos) {
    if (pos.current_ == nullptr) throw std::out_of_range("Erase out of range");

    Node *to_delete = const_cast<Node *>(pos.current_);
    Node *prev_node = to_delete->prev;
    Node *next_node = to_delete->next;

    // 处理前驱节点链接
    if (prev_node) {
        prev_node->next = next_node;
    } else {
        // 删除的是头节点
        head = next_node;
    }

    // 处理后继节点链接
    if (next_node) {
        next_node->prev = prev_node;
    } else {
        // 删除的是尾节点
        tail = prev_node;
    }

    // 保存下一个节点的指针用于返回
    iterator result(next_node, this);

    // 销毁被删除的节点
    destroy_node(to_delete);
    length--;

    return result;
}

template <Listable T, typename Allocator>
list<T, Allocator>::iterator list<T, Allocator>::erase(const_iterator first, const_iterator last) {
    Node *current = const_cast<Node *>(first.current_);
    if (current == nullptr) throw std::out_of_range("Erase out of range");
    if (first == last) return iterator(current, this);

    auto it = first;
    while (it != last) {
        it = erase(it);
    }

    return iterator(const_cast<Node *>(last.current_), this);
}

// ===========================================================
// 6. Iterator Interface
// ===========================================================

template <Listable T, typename Allocator>
template <typename Self>
auto list<T, Allocator>::begin(this Self &&self) noexcept {
    // using BaseSelf = std::remove_reference_t<Self>;
    // using IterType = std::conditional_t<std::is_const_v<BaseSelf>, const_iterator, iterator>;
    // return IterType(self.head);
    constexpr bool is_const = std::is_const_v<std::remove_reference_t<Self>>;
    return ListIterator<is_const>(self.head, &self);
}

template <Listable T, typename Allocator>
list<T, Allocator>::const_iterator list<T, Allocator>::cbegin() const noexcept {
    return const_iterator(head, this);
}

template <Listable T, typename Allocator>
template <typename Self>
auto list<T, Allocator>::end(this Self &&self) noexcept {
    constexpr bool is_const = std::is_const_v<std::remove_reference_t<Self>>;
    return ListIterator<is_const>(nullptr, &self);
}

template <Listable T, typename Allocator>
list<T, Allocator>::const_iterator list<T, Allocator>::cend() const noexcept {
    return const_iterator(nullptr, this);
}

template <Listable T, typename Allocator>
template <typename Self>
auto list<T, Allocator>::rbegin(this Self &&self) noexcept {
    return std::reverse_iterator(self.end()); // 利用 CTAD
}

template <Listable T, typename Allocator>
list<T, Allocator>::const_reverse_iterator list<T, Allocator>::crbegin() const noexcept {
    return const_reverse_iterator(end());
}

template <Listable T, typename Allocator>
template <typename Self>
auto list<T, Allocator>::rend(this Self &&self) noexcept {
    return std::reverse_iterator(self.begin());
}

template <Listable T, typename Allocator>
list<T, Allocator>::const_reverse_iterator list<T, Allocator>::crend() const noexcept {
    return const_reverse_iterator(begin());
}

// ===========================================================
// 7. C++20 Comparison Operations (Spaceship Operator)
// ===========================================================

// 定义 concept 检查是否支持 <=>
template <typename T>
concept ThreeWayComparable = requires(const T &a, const T &b) {
    { a <=> b } -> std::convertible_to<std::strong_ordering>;
};

template <Listable T, typename Allocator>
std::strong_ordering list<T, Allocator>::operator<=>(const list &other) const {
    auto it1 = begin();
    auto it2 = other.begin();

    while (it1 != end() && it2 != other.end()) {
        if constexpr (ThreeWayComparable<T>) {
            if (auto cmp = *it1 <=> *it2; cmp != 0) return cmp;
        } else {
            // 回退到传统比较
            if (*it1 < *it2) return std::strong_ordering::less;
            if (*it1 > *it2) return std::strong_ordering::greater;
        }
        ++it1;
        ++it2;
    }
    return size() <=> other.size();
}

template <Listable T, typename Allocator>
bool list<T, Allocator>::operator==(const list &other) const {
    return (*this <=> other) == std::strong_ordering::equal;
}

// ===========================================================
// Helper Functions
// ===========================================================

template <Listable T, typename Allocator>
template <typename... Args>
list<T, Allocator>::Node *list<T, Allocator>::create_node(Args &&...args) {
    Node *ptr = std::allocator_traits<NodeAlloc>::allocate(allocator_, 1);
    try {
        std::construct_at(ptr, std::forward<Args>(args)...);
    } catch (...) {
        std::allocator_traits<NodeAlloc>::deallocate(allocator_, ptr, 1);
        throw;
    }
    return ptr;
}

template <Listable T, typename Allocator>
void list<T, Allocator>::destroy_node(Node *ptr) {
    if (!ptr) return;
    // ptr->~Node();
    std::allocator_traits<NodeAlloc>::destroy(allocator_, ptr);
    // free(ptr)
    std::allocator_traits<NodeAlloc>::deallocate(allocator_, ptr, 1);
}

} // namespace mys
