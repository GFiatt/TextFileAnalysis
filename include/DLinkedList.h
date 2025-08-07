#ifndef DLINKEDLIST_H
#define DLINKEDLIST_H

#include "List.h"
#include "DNode.h"
#include <stdexcept>
#include <iostream>

using std::runtime_error;
using std::cout;
using std::endl;

// Clase que utiliza memoria dinamica para crear una lista.

template <typename E>
class DLinkedList : public List<E> {
private:
    DNode<E> *head;
    DNode<E> *tail;
    DNode<E> *current;
    int size;

public:
    DLinkedList() {
        current = head = new DNode<E>(nullptr, nullptr);
        head->next = tail = new DNode<E>(nullptr, head);
        size = 0;
    }
    ~DLinkedList() {
        clear();
        delete head;
        delete tail;
    }
    void insert(E element) {
        current->next = current->next->previous =
            new DNode<E>(element, current->next, current);
        size++;
    }
    void append(E element) {
        tail->previous = tail->previous->next =
            new DNode<E>(element, tail, tail->previous);
        size++;
    }
    E remove() {
        if (size == 0)
            throw runtime_error("List is empty.");
        if (current->next == tail)
            throw runtime_error("No current element.");
        E result = current->next->element;
        current->next = current->next->next;
        delete current->next->previous;
        current->next->previous = current;
        size--;
        return result;
    }
    void clear() {
        while (head->next != tail) {
            head->next = head->next->next;
            delete head->next->previous;
        }
        current = tail->previous = head;
        size = 0;
    }
    E getElement() {
        if (size == 0)
            throw runtime_error("List is empty.");
        if (current->next == tail)
            throw runtime_error("No current element.");
        return current->next->element;
    }
    void goToStart() {
        current = head;
    }
    void goToEnd() {
        current = tail->previous;
    }
    void goToPos(int pos) {
        if (pos < 0 || pos > size)
            throw runtime_error("Index out of range.");
        current = head;
        for (int i = 0; i < pos; i++)
            current = current->next;
    }
    void next() {
        if (current->next != tail)
            current = current->next;
    }
    void previous() {
        if (current != head)
            current = current->previous;
    }
    bool atStart() {
        return current == head;
    }
    bool atEnd() {
        return current == tail->previous;
    }
    int getPos() {
        DNode<E> *temp = head;
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
            cout << getElement() << " ";
        cout << "]" << endl;
    }
    int indexOf(E element) {
        int pos = 0;
        DNode<E> *temp = head->next;
        while (temp != tail) {
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
        DNode<E> *oldCurrent = current;
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

#endif // DLINKEDLIST_H
