#include "list.h"
#include <initializer_list>
#include <memory>

namespace mys {

template <Listable T, typename Allocator>
template <bool IsConst>
list<T, Allocator>::template ListIterator<IsConst>::ListIterator(NodePtr *node) : current_(node) {}

template <Listable T, typename Allocator>
template <bool IsConst>
list<T, Allocator>::ListIterator<IsConst>::reference list<T, Allocator>::ListIterator<IsConst>::operator*() const {
    return current_->val;
}

template <Listable T, typename Allocator>
template <bool IsConst>
list<T, Allocator>::ListIterator<IsConst>::pointer list<T, Allocator>::ListIterator<IsConst>::operator->() const {
    return current_->val;
}

template <Listable T, typename Allocator>
template <bool IsConst>
list<T, Allocator>::ListIterator<IsConst> &list<T, Allocator>::ListIterator<IsConst>::operator++() {
    if (current_) current_ = current_->next;
    return *this;
}

template <Listable T, typename Allocator>
template <bool IsConst>
list<T, Allocator>::ListIterator<IsConst> &list<T, Allocator>::ListIterator<IsConst>::operator++(int) {
    iterator temp = *this;
    if (current_) current_ = current_->next;
    return temp;
}

template <Listable T, typename Allocator>
template <bool IsConst>
list<T, Allocator>::ListIterator<IsConst> &list<T, Allocator>::ListIterator<IsConst>::operator--() {
    if (current_) current_ = current_->prev;
    return *this;
}

template <Listable T, typename Allocator>
template <bool IsConst>
list<T, Allocator>::ListIterator<IsConst> &list<T, Allocator>::ListIterator<IsConst>::operator--(int) {
    iterator temp = *this;
    if (current_) current_ = current_->prev;
    return temp;
}

template <Listable T, typename Allocator>
list<T, Allocator>::list(std::initializer_list<T> init) : list() {
    for (auto &x : init) {
        push_back(x);
    }
}

template <Listable T, typename Allocator>
template <typename Self>
auto &&list<T, Allocator>::front(this Self &&self) {
    using ReturnType = std::conditional_t<std::is_const_v<std::remove_reference_t<Self>>, const T &, T &>;
    if (self.empty()) {
        throw std::out_of_range("list is empty");
    }
    return static_cast<ReturnType>(self.head->val);
}

template <Listable T, typename Allocator>
template <typename Self>
auto &&list<T, Allocator>::back(this Self &&self) {
    using ReturnType = std::conditional_t<std::is_const_v<std::remove_reference_t<Self>>, const T &, T &>;
    if (self.empty()) {
        throw std::out_of_range("list is empty");
    }
    return static_cast<ReturnType>(self.tail->val);
}

template <Listable T, typename Allocator>
bool list<T, Allocator>::empty() const noexcept {
    return length == 0;
}

template <Listable T, typename Allocator>
std::size_t list<T, Allocator>::size() const noexcept {
    return length;
}

template <Listable T, typename Allocator>
void list<T, Allocator>::clear() noexcept {
    while (length) {
        Node *p = head;
        head = head->next;
        destroy_node(p);
        length--;
    }
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
    tail->next = nullptr;
    destroy_node(p);
    length--;
}

template <Listable T, typename Allocator>
void list<T, Allocator>::pop_front() {
    if (!head) return;
    Node *p = head;
    head = head->next;
    head->prev = nullptr;
    destroy_node(p);
    length--;
}

template <Listable T, typename Allocator>
list<T, Allocator>::iterator list<T, Allocator>::insert(const_iterator pos, const T &value) {
    Node *newnode = create_node(value);
    if (pos == begin()) {
        emplace_front(value)
    } else if (pos == end()) {
        emplace_back(value);
    } else {
        Node *cur = pos.current_;
        Node *pre = pos.current_->prev;
        cur->prev = newnode;
        newnode->next = cur;
        pre->next = newnode;
        newnode->prev = pre;
    }
    length++;
    return iterator(newnode);
}

template <Listable T, typename Allocator>
list<T, Allocator>::iterator list<T, Allocator>::insert(const_iterator pos, T &&value) {
    Node *newnode = create_node(value);
    if (pos == begin()) {
        emplace_front(value)
    } else if (pos == end()) {
        emplace_back(value);
    } else {
        Node *cur = pos.current_;
        Node *pre = pos.current_->prev;
        cur->prev = newnode;
        newnode->next = cur;
        pre->next = newnode;
        newnode->prev = pre;
    }
    length++;
    return iterator(newnode);
}

template <Listable T, typename Allocator>
template <typename... Args>
list<T, Allocator>::iterator list<T, Allocator>::emplace(const_iterator pos, Args &&...args) {
    Node *newnode = create_node(args);
    if (pos == begin()) {
        emplace_front(value)
    } else if (pos == end()) {
        emplace_back(value);
    } else {
        Node *cur = pos.current_;
        Node *pre = pos.current_->prev;
        cur->prev = newnode;
        newnode->next = cur;
        pre->next = newnode;
        newnode->prev = pre;
    }
    length++;
    return iterator(newnode);
}

template <Listable T, typename Allocator>
list<T, Allocator>::iterator list<T, Allocator>::erase(const_iterator pos) {
    if (pos.current_ == nullptr) throw std::out_of_range("Erase out of range");
    Node *to_delete = pos.current_;
    Node *prev_node = to_delete->prev;
    Node *next_node = to_delete->next;
    if (prev_node) {
        prev_node->next = next_node;
    } else {
        head = next_node;
    }
    if (!next_node) {
        tail = prev_node;
    }
    iterator it();
    destroy_node(to_delete);
    length--;
    return it;
}

template <Listable T, typename Allocator>
list<T, Allocator>::iterator list<T, Allocator>::erase(const_iterator first, const_iterator last) {
    if (first.current_ == nulIlptr) throw std::out_of_range("Erase out of range");
    for (; first != last; first++) {
        Node *cur = first.current_;
        Node *pre = cur->prev;
        if (pre) pre->next = nullptr;
        destroy_node(cur);
        length--;
    }
}

template <Listable T, typename Allocator>
template <typename Self>
auto list<T, Allocator>::begin(this Self &&self) noexcept {
    // using BaseSelf = std::remove_reference_t<Self>;
    // using IterType = std::conditional_t<std::is_const_v<BaseSelf>, const_iterator, iterator>;
    // return IterType(self.head);
    return ListIterator(self.head);
}

template <Listable T, typename Allocator>
list<T, Allocator>::const_iterator list<T, Allocator>::cbegin() const noexcept {
    return const_iterator(head);
}

template <Listable T, typename Allocator>
template <typename Self>
auto list<T, Allocator>::end(this Self &&self) noexcept {
    // using BaseSelf = std::remove_reference_t<Self>;
    // using IterType = std::conditional_t<std::is_const_v<BaseSelf>, const_iterator, iterator>;
    // return IterType(nullptr);
    return ListIterator(nullptr);
}

template <Listable T, typename Allocator>
list<T, Allocator>::const_iterator list<T, Allocator>::cend() const noexcept {
    return const_iterator(nullptr);
}

template <Listable T, typename Allocator>
template <typename Self>
auto list<T, Allocator>::rbegin(this Self &&self) noexcept {
    // using BaseSelf = std::remove_reference_t<Self>;
    // using IterType = std::conditional_t<std::is_const_v<BaseSelf>, const_reverse_iterator, reverse_iterator>;
    // return IterType(tail);
    return ListIterator(self.tail);
}

template <Listable T, typename Allocator>
template <typename Self>
auto list<T, Allocator>::rend(this Self &&self) noexcept {
    // using BaseSelf = std::remove_reference_t<Self>;
    // using IterType = std::conditional_t<std::is_const_v<BaseSelf>, const_reverse_iterator, reverse_iterator>;
    // return IterType(nullptr);
    return ListIterator(nullptr);
}

template <Listable T, typename Allocator, typename... Args>
list<T, Allocator>::Node *create_node(Args &&...args) {
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
