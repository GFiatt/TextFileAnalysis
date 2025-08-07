#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include "List.h"
#include "Node.h"
#include <stdexcept>
#include <iostream>

using std::runtime_error;
using std::cout;
using std::endl;

// Clase de memoria dinamica. Utiliza nodos sencillos.

template <typename E>
class LinkedList : public List<E> {
private:
    Node<E> *head;
    Node<E> *tail;
    Node<E> *current;
    int size;

public:
    LinkedList() {
        head = current = tail = new Node<E>();
        size = 0;
    }
    ~LinkedList() {
        clear();
        delete head;
    }
    void insert(E element) {
        current->next = new Node<E>(element, current->next);
        if (current == tail)
            tail = tail->next;
        size++;
    }
    void append(E element) {
        tail = tail->next = new Node<E>(element);
        size++;
    }
    E remove() {
        if (size == 0)
            throw runtime_error("List is empty.");
        if (current == tail)
            throw runtime_error("No current element.");
        E result = current->next->element;
        Node<E> *temp = current->next;
        current->next = temp->next;
        if (temp == tail)
            tail = current;
        delete temp;
        size--;
        return result;
    }
    void clear() {
        while (head->next != nullptr) {
            current = head->next;
            head->next = current->next;
            delete current;
        }
        current = tail = head;
        size = 0;
    }
    E getElement() {
        if (size == 0)
            throw runtime_error("List is empty.");
        if (current == tail)
            throw runtime_error("No current element.");
        return current->next->element;
    }
    void goToStart() {
        current = head;
    }
    void goToEnd() {
        current = tail;
    }
    void goToPos(int pos) {
        if (pos < 0 || pos > size)
            throw runtime_error("Index out of range.");
        current = head;
        for (int i = 0; i < pos; i++)
            current = current->next;
    }
    void next() {
        if (current != tail)
            current = current->next;
    }
    void previous() {
        if (current != head) {
            Node<E> *temp = head;
            while (temp->next != current)
                temp = temp->next;
            current = temp;
        }
    }
    bool atStart() {
        return current == head;
    }
    bool atEnd() {
        return current == tail;
    }
    int getPos() {
        Node<E> *temp = head;
        int pos = 0;
        while (temp != current) {
            temp = temp->next;
            pos++;
        }
        return pos;
    }
    int getSize() {
        return size;
    }
    void print() {
        cout << "[ ";
        for (goToStart(); !atEnd(); next())
            cout << getElement() <<endl;
        cout << "]" << endl;
    }
    int indexOf(E element) {
        int pos = 0;
        Node<E> *temp = head->next;
        while (temp != nullptr) {
            if (temp->element == element)
                return pos;
            temp = temp->next;
            pos++;
        }
        return -1;
    }
    bool contains(E element) {
        return indexOf(element) != -1;
    }
    void extend(List<E> *L) {
        int pos = L->getPos();
        L->goToStart();
        while (!L->atEnd()) {
            append(L->getElement());
            L->next();
        }
        L->goToPos(pos);
    }
    bool equals(List<E> *L) {
        if (size != L->getSize())
            return false;
        Node<E> *oldCurrent = current;
        int pos = L->getPos();
        goToStart();
        L->goToStart();
        while (!L->atEnd()) {
            if (getElement() != L->getElement()) {
                current = oldCurrent;
                L->goToPos(pos);
                return false;
            }
        }
        current = oldCurrent;
        L->goToPos(pos);
        return true;
    }
};

#endif // LINKEDLIST_H
