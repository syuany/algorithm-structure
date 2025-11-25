#include "list.h"
#include <initializer_list>
#include <memory>

namespace mys {

template <Listable T, typename Allocator>
list<T, Allocator>::list(std::initializer_list<T> init) :
    list() {
    for (auto &x : init) {
        push_back(x);
    }
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
        head = p;
    }
    length++;
}

template <Listable T, typename Allocator>
void list<T, Allocator>::pop_back() {
    if (!tail) return;
    Node *p = tail;
    tail = tail->prev;
    destroy_node(p);
    length--;
}

template <Listable T, typename Allocator>
void list<T, Allocator>::pop_front() {
    if (!head) return;
    Node *p = head;
    head = head->next;
    destroy_node(p);
    length--;
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
