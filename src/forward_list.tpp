#include "forward_list.h"
#include <stdexcept>
#include <algorithm>
#include <functional>

namespace mys {

// ===========================================================
// Helper Functions
// ===========================================================

template <ForwardListable T, typename Alloc>
template <typename... Args>
typename forward_list<T, Alloc>::Node *forward_list<T, Alloc>::create_node(Args &&...args) {
    Node *ptr = allocator_.allocate(1);
    try {
        std::allocator_traits<NodeAlloc>::construct(allocator_, ptr, std::forward<Args>(args)...);
    } catch (...) {
        allocator_.deallocate(ptr, 1);
        throw;
    }
    return ptr;
}

template <ForwardListable T, typename Alloc>
void forward_list<T, Alloc>::destroy_node(Node *ptr) {
    std::allocator_traits<NodeAlloc>::destroy(allocator_, ptr);
    allocator_.deallocate(ptr, 1);
}

// ===========================================================
// Construction and Destruction
// ===========================================================

template <ForwardListable T, typename Alloc>
forward_list<T, Alloc>::forward_list() {
    head_.next = nullptr;
    length_ = 0;
}

template <ForwardListable T, typename Alloc>
forward_list<T, Alloc>::forward_list(std::initializer_list<T> init) : forward_list() {
    // 逆序插入，保证顺序正确，或者维护一个 tail 指针进行尾插
    // 这里为了效率使用 insert_after 配合 before_begin 顺序插入
    auto it = before_begin();
    for (const auto &item : init) {
        it = insert_after(it, item);
    }
}

template <ForwardListable T, typename Alloc>
forward_list<T, Alloc>::forward_list(const forward_list &other) : forward_list() {
    // 深拷贝
    auto it = before_begin();
    for (const auto &item : other) {
        it = insert_after(it, item);
    }
}

template <ForwardListable T, typename Alloc>
forward_list<T, Alloc>::forward_list(forward_list &&other) noexcept : forward_list() {
    swap(other);
}

template <ForwardListable T, typename Alloc>
forward_list<T, Alloc> &forward_list<T, Alloc>::operator=(const forward_list &other) {
    if (this != &other) {
        forward_list temp(other);
        swap(temp);
    }
    return *this;
}

template <ForwardListable T, typename Alloc>
forward_list<T, Alloc> &forward_list<T, Alloc>::operator=(forward_list &&other) noexcept {
    if (this != &other) {
        clear();
        swap(other);
    }
    return *this;
}

template <ForwardListable T, typename Alloc>
forward_list<T, Alloc>::~forward_list() {
    clear();
}

// ===========================================================
// Element Access
// ===========================================================

template <ForwardListable T, typename Alloc>
template <typename Self>
auto &&forward_list<T, Alloc>::front(this Self &&self) {
    // 假设非空，调用者需保证
    // head_.next 是 NodeBase*，需要转为 Node* 才能访问 val
    return static_cast<std::conditional_t<std::is_const_v<std::remove_reference_t<Self>>, const Node *, Node *>>(self.head_.next)->val;
}

// ===========================================================
// Capacity Query
// ===========================================================

template <ForwardListable T, typename Alloc>
bool forward_list<T, Alloc>::empty() const noexcept {
    return head_.next == nullptr;
}

template <ForwardListable T, typename Alloc>
std::size_t forward_list<T, Alloc>::size() const noexcept {
    return length_;
}

// ===========================================================
// Modifiers
// ===========================================================

template <ForwardListable T, typename Alloc>
void forward_list<T, Alloc>::clear() noexcept {
    NodeBase *curr = head_.next;
    while (curr != nullptr) {
        NodeBase *next = curr->next;
        // 将 Base 转换为 Node 进行销毁
        destroy_node(static_cast<Node *>(curr));
        curr = next;
    }
    head_.next = nullptr;
    length_ = 0;
}

template <ForwardListable T, typename Alloc>
void forward_list<T, Alloc>::swap(forward_list &other) noexcept {
    using std::swap;
    swap(head_.next, other.head_.next); // 交换哨兵指向的第一个节点
    swap(length_, other.length_);
    // allocator 的 swap 取决于 allocator_traits，这里简化处理
    if constexpr (std::allocator_traits<Alloc>::propagate_on_container_swap::value) {
        swap(allocator_, other.allocator_);
    }
}

template <ForwardListable T, typename Alloc>
void forward_list<T, Alloc>::push_front(const T &value) {
    insert_after(before_begin(), value);
}

template <ForwardListable T, typename Alloc>
void forward_list<T, Alloc>::push_front(T &&value) {
    insert_after(before_begin(), std::move(value));
}

template <ForwardListable T, typename Alloc>
template <typename... Args>
void forward_list<T, Alloc>::emplace_front(Args &&...args) {
    emplace_after(before_begin(), std::forward<Args>(args)...);
}

template <ForwardListable T, typename Alloc>
void forward_list<T, Alloc>::pop_front() {
    erase_after(before_begin());
}

template <ForwardListable T, typename Alloc>
typename forward_list<T, Alloc>::iterator forward_list<T, Alloc>::insert_after(const_iterator pos, const T &value) {
    Node *new_node = create_node(value);
    NodeBase *prev = get_node_base(pos);

    new_node->next = prev->next;
    prev->next = new_node;

    length_++;
    return iterator(new_node);
}

template <ForwardListable T, typename Alloc>
typename forward_list<T, Alloc>::iterator forward_list<T, Alloc>::insert_after(const_iterator pos, T &&value) {
    Node *new_node = create_node(std::move(value));
    NodeBase *prev = get_node_base(pos);

    new_node->next = prev->next;
    prev->next = new_node;

    length_++;
    return iterator(new_node);
}

template <ForwardListable T, typename Alloc>
template <typename... Args>
typename forward_list<T, Alloc>::iterator forward_list<T, Alloc>::emplace_after(const_iterator pos, Args &&...args) {
    Node *new_node = create_node(std::forward<Args>(args)...);
    NodeBase *prev = get_node_base(pos);

    new_node->next = prev->next;
    prev->next = new_node;

    length_++;
    return iterator(new_node);
}

template <ForwardListable T, typename Alloc>
typename forward_list<T, Alloc>::iterator forward_list<T, Alloc>::erase_after(const_iterator pos) {
    NodeBase *prev = get_node_base(pos);
    NodeBase *curr = prev->next;

    if (curr) {
        prev->next = curr->next;
        destroy_node(static_cast<Node *>(curr));
        length_--;
        return iterator(prev->next);
    }
    return end();
}

template <ForwardListable T, typename Alloc>
typename forward_list<T, Alloc>::iterator forward_list<T, Alloc>::erase_after(const_iterator first, const_iterator last) {
    NodeBase *prev = get_node_base(first);
    while (prev->next != last.current_) {
        erase_after(iterator(prev));
    }
    return iterator(const_cast<NodeBase *>(last.current_));
}

// ===========================================================
// Iterator Interface Implementation
// ===========================================================

// 返回指向 head_ 哨兵节点的迭代器
template <ForwardListable T, typename Alloc>
template <typename Self>
auto forward_list<T, Alloc>::before_begin(this Self &&self) noexcept {
    // 根据 self 的 const 属性决定使用 iterator 还是 const_iterator
    using Iter = std::conditional_t<std::is_const_v<std::remove_reference_t<Self>>, const_iterator, iterator>;

    // 返回指向成员变量 head_ 的地址
    return Iter(&self.head_);
}

template <ForwardListable T, typename Alloc>
forward_list<T, Alloc>::const_iterator forward_list<T, Alloc>::cbefore_begin() const noexcept {
    return const_iterator(&head_);
}

template <ForwardListable T, typename Alloc>
template <typename Self>
auto forward_list<T, Alloc>::begin(this Self &&self) noexcept {
    using Iter = std::conditional_t<std::is_const_v<std::remove_reference_t<Self>>, const_iterator, iterator>;
    return Iter(self.head_.next);
}

template <ForwardListable T, typename Alloc>
forward_list<T, Alloc>::const_iterator forward_list<T, Alloc>::cbegin() const noexcept {
    return const_iterator(head_.next);
}

template <ForwardListable T, typename Alloc>
template <typename Self>
auto forward_list<T, Alloc>::end(this Self &&self) noexcept {
    using Iter = std::conditional_t<std::is_const_v<std::remove_reference_t<Self>>, const_iterator, iterator>;
    return Iter(nullptr);
}

template <ForwardListable T, typename Alloc>
forward_list<T, Alloc>::const_iterator forward_list<T, Alloc>::cend() const noexcept {
    return const_iterator(nullptr);
}

// ===========================================================
// Operations
// ===========================================================

// 1. Splice entire list: moves all elements from other to *this after pos
template <ForwardListable T, typename Alloc>
void forward_list<T, Alloc>::splice_after(const_iterator pos, forward_list &other) {
    if (other.empty()) return;
    if (this == &other) return; // 不能 splice 自身

    NodeBase *prev = get_node_base(pos);
    NodeBase *other_head = &other.head_;
    NodeBase *other_first = other_head->next;

    // 找到 other 的最后一个节点
    NodeBase *other_last = other_first;
    while (other_last->next != nullptr) {
        other_last = other_last->next;
    }

    // 链接
    other_last->next = prev->next;
    prev->next = other_first;

    // 清空 other
    other_head->next = nullptr;

    length_ += other.length_;
    other.length_ = 0;
}

template <ForwardListable T, typename Alloc>
void forward_list<T, Alloc>::splice_after(const_iterator pos, forward_list &&other) {
    splice_after(pos, other);
}

// 2. Splice single element: moves element *after* it from other to *this after pos
template <ForwardListable T, typename Alloc>
void forward_list<T, Alloc>::splice_after(const_iterator pos, forward_list &other, const_iterator it) {
    NodeBase *pos_ptr = get_node_base(pos);
    NodeBase *it_ptr = get_node_base(it); // 这里的 it 指向要移动的节点的前一个节点

    if (pos_ptr == it_ptr || pos_ptr == it_ptr->next) return;

    NodeBase *node_to_move = it_ptr->next;
    if (!node_to_move) return;

    // 从 other 中断开
    it_ptr->next = node_to_move->next;

    // 插入到 this
    node_to_move->next = pos_ptr->next;
    pos_ptr->next = node_to_move;

    other.length_--;
    length_++;
}

// 3. Splice range: moves (first, last) from other to *this after pos
template <ForwardListable T, typename Alloc>
void forward_list<T, Alloc>::splice_after(const_iterator pos, forward_list &other, const_iterator first, const_iterator last) {
    NodeBase *pos_ptr = get_node_base(pos);
    NodeBase *first_ptr = get_node_base(first);
    NodeBase *last_ptr = get_node_base(last); // last 是开区间，不移动

    if (first_ptr == last_ptr || first_ptr->next == last_ptr) return;

    // 找到要移动范围的尾部节点 (即 last 之前的一个节点)
    NodeBase *range_tail = first_ptr->next;
    std::size_t count = 0;
    while (range_tail->next != last_ptr) {
        range_tail = range_tail->next;
        count++;
    }
    count++; // 加上 range_tail 自己

    NodeBase *range_head = first_ptr->next;

    // 从 other 断开
    first_ptr->next = last_ptr;

    // 接入 this
    range_tail->next = pos_ptr->next;
    pos_ptr->next = range_head;

    other.length_ -= count;
    length_ += count;
}

template <ForwardListable T, typename Alloc>
void forward_list<T, Alloc>::remove(const T &value) {
    remove_if([&value](const T &v) { return v == value; });
}

template <ForwardListable T, typename Alloc>
template <typename Predicate>
void forward_list<T, Alloc>::remove_if(Predicate pred) {
    iterator prev = before_begin();
    iterator curr = begin();

    while (curr != end()) {
        if (pred(*curr)) {
            curr = erase_after(prev);
        } else {
            prev = curr;
            ++curr;
        }
    }
}

template <ForwardListable T, typename Alloc>
void forward_list<T, Alloc>::unique() {
    unique(std::equal_to<T>());
}

template <ForwardListable T, typename Alloc>
template <typename BinaryPredicate>
void forward_list<T, Alloc>::unique(BinaryPredicate pred) {
    iterator curr = begin();
    iterator last = end();

    if (curr == last) return;

    while (true) {
        iterator next_node = curr;
        ++next_node;
        if (next_node == last) break;

        if (pred(*curr, *next_node)) {
            erase_after(curr);
        } else {
            curr = next_node;
        }
    }
}

template <ForwardListable T, typename Alloc>
void forward_list<T, Alloc>::reverse() noexcept {
    if (empty()) return;

    NodeBase *prev = nullptr;
    NodeBase *curr = head_.next;
    NodeBase *next = nullptr;

    while (curr != nullptr) {
        next = curr->next; // 保存下一个
        curr->next = prev; // 反转指针
        prev = curr;       // 步进 prev
        curr = next;       // 步进 curr
    }

    head_.next = prev; // 更新头节点
}

// ===========================================================
// Comparison Operators
// ===========================================================

template <ForwardListable T, typename Alloc>
std::strong_ordering forward_list<T, Alloc>::operator<=>(const forward_list &other) const {
    auto it1 = begin();
    auto it2 = other.begin();
    auto end1 = end();
    auto end2 = other.end();

    while (it1 != end1 && it2 != end2) {
        auto cmp = std::compare_strong_order_fallback(*it1, *it2);
        if (cmp != 0) return cmp;
        ++it1;
        ++it2;
    }

    return (it1 == end1 && it2 == end2) ? std::strong_ordering::equal : (it1 == end1) ? std::strong_ordering::less : std::strong_ordering::greater;
}

template <ForwardListable T, typename Alloc>
bool forward_list<T, Alloc>::operator==(const forward_list &other) const {
    if (length_ != other.length_) return false; // 优化：如果长度不同直接返回 false

    auto it1 = begin();
    auto it2 = other.begin();
    while (it1 != end()) {
        if (*it1 != *it2) return false;
        ++it1;
        ++it2;
    }
    return true;
}

} // namespace mys