#ifndef AVLTREE_H
#define AVLTREE_H

#include <stdexcept>
#include "BSTNode.h"
#include "DLinkedList.h"

using std::runtime_error;

// AVL TREE: Clase que funciona como arbol. Es una estructura de datos.

template <typename E>
class AVLTree {
private:
    BSTNode<E> *root;
    int size;

    E findAux(BSTNode<E> *current, E element) {
        if (current == nullptr)
            throw runtime_error("Element not found.");
        if (element == current->element)
            return current->element;
        if (element < current->element)
            return findAux(current->left, element);
        else
            return findAux(current->right, element);
    }
    BSTNode<E>* insertAux(BSTNode<E> *current, E element) {
        if (current == nullptr)
            return new BSTNode<E>(element);
        // Si el arbol no permite repetidos
        if (element == current->element)
            throw runtime_error("Duplicated element.");
        if (element < current->element) {
            current->left = insertAux(current->left, element);
            return rebalanceLeft(current);
        } else {
            current->right = insertAux(current->right, element);
            return rebalanceRight(current);
        }
    }
    BSTNode<E>* removeAux(BSTNode<E> *current, E element, E *result) {
        if (current == nullptr)
            throw runtime_error("Element not found.");
        if (element < current->element) {
            current->left = removeAux(current->left, element, result);
            return rebalanceRight(current);
        }
        if (element > current->element) {
            current->right = removeAux(current->right, element, result);
            return rebalanceLeft(current);
        } else { // element == current->element
            (*result) = current->element;
            if (current->childrenCount() == 0) {
                delete current;
                return nullptr;
            }
            if (current->childrenCount() == 1) {
                BSTNode<E> *child = current->getUniqueChild();
                delete current;
                return child;
            } else { // current->childrenCount() == 2
                BSTNode<E> *successor = getSuccessor(current);
                swap(current, successor);
                current->right = removeAux(current->right, element, result);
                return rebalanceLeft(current);
            }
        }
    }
    BSTNode<E>* getSuccessor(BSTNode<E> *current) {
        BSTNode<E> *successor = current->right;
        while (successor->left != nullptr)
            successor = successor->left;
        return successor;
    }
    void swap(BSTNode<E> *node1, BSTNode<E> *node2) {
        E temp = node1->element;
        node1->element = node2->element;
        node2->element = temp;
    }
    bool containsAux(BSTNode<E> *current, E element) {
        if (current == nullptr)
            return false;
        if (element == current->element)
            return true;
        if (element < current->element)
            return containsAux(current->left, element);
        else
            return containsAux(current->right, element);
    }
    void clearAux(BSTNode<E> *current) {
        if (current == nullptr)
            return;
        clearAux(current->left);
        clearAux(current->right);
        delete current;
    }
    void getElementsAux(BSTNode<E> *current, List<E> *elements) {
        if (current == nullptr)
            return;
        getElementsAux(current->left, elements);
        elements->append(current->element);
        getElementsAux(current->right, elements);
    }
    int height(BSTNode<E> *current) {
        if (current == nullptr) return 0;
        int leftHeight = height(current->left);
        int rightHeight = height(current->right);
        return 1 + (leftHeight > rightHeight? leftHeight : rightHeight);
    }
    BSTNode<E>* rotateRight(BSTNode<E> *current) {
        if (current->left == nullptr)
            throw runtime_error("Can't rotate right with null left child.");
        BSTNode<E> *temp = current->left;
        current->left = temp->right;
        temp->right = current;
        return temp;
    }
    BSTNode<E>* rotateLeft(BSTNode<E> *current) {
        if (current->right == nullptr)
            throw runtime_error("Can't rotate left with null right child.");
        BSTNode<E> *temp = current->right;
        current->right = temp->left;
        temp->left = current;
        return temp;
    }
    BSTNode<E>* rebalanceLeft(BSTNode<E> *current) {
        int leftH = height(current->left);
        int rightH = height(current->right);
        if (leftH - rightH > 1) {
            int leftLeftH = height(current->left->left);
            int leftRightH = height(current->left->right);
            if (leftLeftH >= leftRightH) { // caso izq izq
                return rotateRight(current);
            } else { // caso izq der
                current->left = rotateLeft(current->left);
                return rotateRight(current);
            }
        }
        return current;
    }

    BSTNode<E>* rebalanceRight(BSTNode<E> *current) {
        int rightH = height(current->right);
        int leftH = height(current->left);
        if (rightH - leftH > 1) {
            int rightRightH = height(current->right->right);
            int rightLeftH = height(current->right->left);
            if (rightRightH >= rightLeftH) { // caso der der
                return rotateLeft(current);
            } else { // caso der izq
                current->right = rotateRight(current->right);
                return rotateLeft(current);
            }
        }
        return current;
    }

public:
    AVLTree() {
        root = nullptr;
        size = 0;
    }
    ~AVLTree() {
        clear();
    }
    E find(E element) {
        return findAux(root, element);
    }
    void insert(E element) {
        root = insertAux(root, element);
        size++;
    }
    E remove(E element) {
        E result;
        root = removeAux(root, element, &result);
        size--;
        return result;
    }
    bool contains(E element) {
        return containsAux(root, element);
    }
    void clear() {
        clearAux(root);
        root = nullptr;
        size = 0;
    }
    List<E>* getElements() {
        List<E> *elements = new DLinkedList<E>();
        getElementsAux(root, elements);
        return elements;
    }
    int getSize() {
        return size;
    }
    void print() {
        List<E> *elements = getElements();
        elements->print();
        delete elements;
    }
};

#endif // AVLTREE_H
