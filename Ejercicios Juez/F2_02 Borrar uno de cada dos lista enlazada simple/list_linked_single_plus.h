/*
 * Implementación del TAD Lista mediante listas enlazadas simples.
 * Clase original de Manuel Montenegro. Adaptada por Ignacio Fábregas
 *
 * Versión con clase genérica, nodo fantasma, cabeza y cola.
 */

#ifndef __LIST_LINKED_SINGLE_PLUS_H
#define __LIST_LINKED_SINGLE_PLUS_H

#include <cassert>
#include <iostream>

using namespace std;

template<typename T>
class ListLinkedSingle {
protected:
    struct Node {
        T value;
        Node* next;
    };

public:
    ListLinkedSingle() {
        head = new Node;
        head->next = nullptr;
        tail = head;
    }

    ~ListLinkedSingle() {
        delete_list(head);
    }

    ListLinkedSingle(const ListLinkedSingle& other)
        : head(copy_nodes(other.head)) {
        tail = nth_node(other.size() - 1);
    }

    void push_front(const T& elem) {
        Node* new_node = new Node{ elem, head->next };
        head->next = new_node;
        if (tail == head) //si la lista era unitaria
            tail = new_node;
    }

    //Coste constante
    void push_back(const T& elem);

    void pop_front() {
        assert(head->next != nullptr);
        Node* old_head = head->next;
        head->next = head->next->next;
        delete old_head;
        if (head->next == nullptr)
            tail = head;
    }

    void pop_back();

    int size() const;

    bool empty() const {
        return head->next == nullptr;
    };

    const T& front() const {
        assert(head->next != nullptr);
        return head->next->value;
    }

    T& front() {
        assert(head->next != nullptr);
        return head->next->value;
    }

    const T& back() const {
        assert(tail != nullptr);
        return tail->value;
    }

    T& back() {
        assert(tail != nullptr);
        return tail->value;
    }

    const T& at(int index) const {
        Node* result_node = nth_node(index);
        assert(result_node != nullptr);
        return result_node->value;
    }

    T& at(int index) {
        Node* result_node = nth_node(index);
        assert(result_node != nullptr);
        return result_node->value;
    }

    void display(std::ostream& out) const;

    void eliminarPares() const;


    void display() const {
        display(std::cout);
    }

private:
    Node* head;
    Node* tail;

    void delete_list(Node* start_node);
    Node* nth_node(int n) const;
    Node* copy_nodes(Node* start_node) const;

};


template <typename T>
typename ListLinkedSingle<T>::Node* ListLinkedSingle<T>::copy_nodes(Node* start_node) const {
    if (start_node != nullptr) {
        Node* result = new Node{ start_node->value, copy_nodes(start_node->next) };
        return result;
    }
    else {
        return nullptr;
    }
}

template <typename T>
void ListLinkedSingle<T>::delete_list(Node* start_node) {
    if (start_node != nullptr) {
        delete_list(start_node->next);
        delete start_node;
    }
}

template <typename T>
void ListLinkedSingle<T>::push_back(const T& elem) {
    Node* new_node = new Node{ elem, nullptr };
    tail->next = new_node;
    tail = tail->next;
}

template <typename T>
void ListLinkedSingle<T>::pop_back() {
    assert(head->next != nullptr);
    Node* previous = head;
    Node* current = head->next;

    while (current->next != nullptr) {
        previous = current;
        current = current->next;
    }

    delete current;
    previous->next = nullptr;
    tail = previous;
}

template <typename T>
int ListLinkedSingle<T>::size() const {
    int num_nodes = 0;

    Node* current = head->next;
    while (current != nullptr) {
        num_nodes++;
        current = current->next;
    }

    return num_nodes;
}

template <typename T>
typename ListLinkedSingle<T>::Node* ListLinkedSingle<T>::nth_node(int n) const {
    assert(0 <= n);
    int current_index = 0;
    Node* current = head->next;

    while (current_index < n && current != nullptr) {
        current_index++;
        current = current->next;
    }

    return current;
}

template <typename T>
void ListLinkedSingle<T>::display(std::ostream& out) const {
    if (head->next != nullptr) {
        out << head->next->value;
        Node* current = head->next->next;
        while (current != nullptr) {
            out << " " << current->value;
            current = current->next;
        }
    }
}

template <typename T>
void ListLinkedSingle<T>::eliminarPares() const {
    Node* previous = this->head;
    Node* current = this->head->next;
    bool par = false;
    
    while (current != nullptr) {
        if (par) {
            previous->next = current->next;
            par = false;
        }
        else {
            par = true;
        }
        previous = current;
        current = current->next;
    }
}

#endif