#include "forward_list.h"
#include <initializer_list>
#include <stdexcept>

namespace mys {

template <typename T>
forward_list<T>::forward_list(std::initializer_list<T> init) : forward_list() {
    for (const auto &val : init) {
        push_back(val);
    }
}

template <typename T>
forward_list<T>::forward_list(const forward_list &other) {
    if (!other.length) {
        head = nullptr;
        tail = nullptr;
        length = 0;
        return;
    }
    head = new Node(other.head->val);
    tail = head;
    length = 1;
    for (auto it = ++other.begin(); it != other.end(); it++) {
        push_back(*it);
    }
}

template <typename T>
forward_list<T>::forward_list(forward_list &&other) noexcept {
    head = other.head;
    tail = other.tail;
    length = other.length;
    other.head = nullptr;
    other.tail = nullptr;
    other.length = 0;
}

template <typename T>
forward_list<T>::~forward_list() {
    clear();
}

template <typename T>
forward_list<T> &forward_list<T>::operator=(const forward_list &other) {
    if (this != &other) {
        forward_list temp(other);
        std::swap(head, temp.head);
        std::swap(tail, temp.tail);
        std::swap(length, temp.length);
    }
    return *this;
}

template <typename T>
forward_list<T> &forward_list<T>::operator=(forward_list &&other) noexcept {
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

template <typename T>
size_t forward_list<T>::size() const noexcept {
    return length;
}

template <typename T>
bool forward_list<T>::empty() const noexcept {
    return length == 0;
}

template <typename T>
T &forward_list<T>::front() {
    if (!length) {
        throw std::out_of_range("Head not exist");
    }
    return head->val;
}

template <typename T>
const T &forward_list<T>::front() const {
    if (!length) {
        throw std::out_of_range("Head not exist");
    }
    return head->val;
}

template <typename T>
T &forward_list<T>::back() {
    if (!length) {
        throw std::out_of_range("Tail not exist");
    }
    return tail->val;
}

template <typename T>
const T &forward_list<T>::back() const {
    if (!length) {
        throw std::out_of_range("Tail not exist");
    }
    return tail->val;
}

template <typename T>
T &forward_list<T>::at(size_t index) {
    if (index >= length) {
        throw std::out_of_range("At index out of range");
    }
    Node *current = head;
    for (size_t i = 0; i < index; i++)
        current = current->next;
    return current->val;
}

template <typename T>
const T &forward_list<T>::at(size_t index) const {
    if (index >= length) {
        throw std::out_of_range("At index out of range");
    }
    const Node *current = head;
    for (size_t i = 0; i < index; i++)
        current = current->next;
    return current->val;
}

template <typename T>
T &forward_list<T>::operator[](size_t index) {
    Node *cur = head;
    for (size_t i = 0; i < index; i++) {
        cur = cur->next;
    }
    return cur->val;
}

template <typename T>
const T &forward_list<T>::operator[](size_t index) const {
    const Node *cur = head;
    for (size_t i = 0; i < index; i++) {
        cur = cur->next;
    }
    return cur->val;
}

template <typename T>
void forward_list<T>::push_front(const T &value) {
    Node *p = new Node(value);
    if (!length) tail = p;
    p->next = head;
    head = p;
    length++;
}

template <typename T>
void forward_list<T>::push_back(const T &value) {
    if (length == 0) {
        head = new Node(value);
        tail = head;
        length++;
        return;
    }
    Node *p = new Node(value);
    tail->next = p;
    tail = p;
    length++;
}

template <typename T>
void forward_list<T>::pop_front() {
    if (length == 0) {
        throw std::out_of_range("Pop front from empty list");
    }
    if (length == 1) {
        delete head;
        head = nullptr;
        tail = nullptr;
    } else {
        Node *p = head;
        head = head->next;
        delete p;
    }
    length--;
}

template <typename T>
void forward_list<T>::pop_back() {
    if (length == 0) {
        throw std::out_of_range("Pop back from empty list");
    }
    if (length == 1) {
        delete head;
        head = nullptr;
        tail = nullptr;
    } else {
        Node *pre = head;
        for (size_t i = 0; i < length - 2; i++) {
            pre = pre->next;
        }
        delete tail;
        tail = pre;
        tail->next = nullptr;
    }
    length--;
}

template <typename T>
typename forward_list<T>::iterator forward_list<T>::insert(size_t index, const T &value) {
    if (index > size()) throw std::out_of_range("Insert index out of range");

    Node *newnode = new Node(value);

    if (index == 0) {
        newnode->next = head;
        head = newnode;
        if (tail == nullptr) tail = newnode;
    } else {
        Node *current = head;

        for (size_t i = 0; i < index - 1; i++) {
            current = current->next;
        }
        newnode->next = current->next;
        current->next = newnode;
        if (newnode->next == nullptr) tail = newnode;
    }

    length++;
    return iterator(newnode);
}

template <typename T>
typename forward_list<T>::iterator forward_list<T>::erase(size_t index) {
    if (index >= size()) throw std::out_of_range("Erase index out of range");

    if (index == 0) {
        Node *toDelete = head;
        head = head->next;
        delete toDelete;
        length--;
        if (head == nullptr) {
            tail = nullptr;
        }
        return iterator(head);
    }

    Node *pre = head;
    for (size_t i = 0; i < index - 1; i++) {
        pre = pre->next;
    }
    Node *toDelete = pre->next;
    pre->next = toDelete->next;
    if (toDelete == tail) {
        tail = pre;
    }
    delete toDelete;
    length--;

    return iterator(pre->next);
}

template <typename T>
void forward_list<T>::clear() noexcept {
    while (length > 0) {
        Node *p = head;
        head = head->next;
        delete p;
        length--;
    }
    tail = nullptr;
}

template <typename T>
typename forward_list<T>::iterator forward_list<T>::find(const T &value) {
    for (iterator it = begin(); it != end(); it++) {
        if (*it == value) return it;
    }
    return end();
}

template <typename T>
typename forward_list<T>::const_iterator forward_list<T>::find(const T &value) const {
    for (const_iterator it = begin(); it != end(); it++) {
        if (*it == value) return it;
    }
    return end();
}

template <typename T>
bool forward_list<T>::contains(const T &value) const {
    for (const_iterator it = begin(); it != end(); it++) {
        if (*it == value) return true;
    }
    return false;
}

template <typename T>
typename forward_list<T>::iterator forward_list<T>::begin() noexcept {
    return iterator(head);
}

template <typename T>
typename forward_list<T>::iterator forward_list<T>::end() noexcept {
    return iterator(nullptr);
}

template <typename T>
typename forward_list<T>::const_iterator forward_list<T>::begin() const noexcept {
    return const_iterator(head);
}

template <typename T>
typename forward_list<T>::const_iterator forward_list<T>::end() const noexcept {
    return const_iterator(nullptr);
}

template <typename T>
typename forward_list<T>::const_iterator forward_list<T>::cbegin() const noexcept {
    return const_iterator(head);
}

template <typename T>
typename forward_list<T>::const_iterator forward_list<T>::cend() const noexcept {
    return const_iterator(nullptr);
}

template <typename T>
void forward_list<T>::reverse() noexcept {
    if (length <= 1) return;
    Node *pre = nullptr, *cur = head;
    tail = head;
    while (cur) {
        Node *next = cur->next;
        cur->next = pre;
        pre = cur;
        cur = next;
    }
    head = pre;
}

template <typename T>
void forward_list<T>::remove_value(const T &value) {
    while (head != nullptr && head->val == value) {
        Node *toDelete = head;
        head = head->next;
        delete toDelete;
        length--;
    }
    if (!length) {
        tail = nullptr;
        return;
    }
    Node *cur = head;
    while (cur->next) {
        if (cur->next->val == value) {
            Node *toDelete = cur->next;
            cur->next = toDelete->next;
            if (toDelete == tail) tail = cur;
            delete toDelete;
            length--;
        } else {
            cur = cur->next;
        }
    }
}

template <typename T>
T &forward_list<T>::iterator::operator*() const {
    return current->val;
}

template <typename T>
T *forward_list<T>::iterator::operator->() {
    return &(current->val);
}

template <typename T>
typename forward_list<T>::iterator &forward_list<T>::iterator::operator++() {
    if (current) current = current->next;
    return *this;
}

template <typename T>
typename forward_list<T>::iterator forward_list<T>::iterator::operator++(int) {
    iterator temp = *this;
    if (current) current = current->next;
    return temp;
}

template <typename T>
bool forward_list<T>::iterator::operator==(const iterator &other) const {
    return current == other.current;
}

template <typename T>
bool forward_list<T>::iterator::operator!=(const iterator &other) const {
    return !(*this == other);
}

template <typename T>
const T &forward_list<T>::const_iterator::operator*() const {
    return current->val;
}

template <typename T>
const T *forward_list<T>::const_iterator::operator->() const {
    return &(current->val);
}

template <typename T>
typename forward_list<T>::const_iterator &forward_list<T>::const_iterator::operator++() {
    if (current) {
        current = current->next;
    }
    return *this;
}

template <typename T>
typename forward_list<T>::const_iterator forward_list<T>::const_iterator::operator++(int) {
    const_iterator temp = *this;
    if (current) current = current->next;
    return temp;
}

template <typename T>
bool forward_list<T>::const_iterator::operator==(const const_iterator &other) const {
    return current == other.current;
}

template <typename T>
bool forward_list<T>::const_iterator::operator!=(const const_iterator &other) const {
    return !(*this == other);
}

} // namespace mys