#ifndef BSTNODE_H
#define BSTNODE_H

// Clase de nodo que se usa en los BSTDictionary.

template <typename E>
class BSTNode {
public:
    E element;
    BSTNode<E> *left;
    BSTNode<E> *right;

    BSTNode(E element) {
        this->element = element;
        left = nullptr;
        right = nullptr;
    }
    BSTNode() {}
    bool isLeaf() {
        return left == nullptr && right == nullptr;
    }
    int childrenCount() {
        return (left == nullptr? 0 : 1)
             + (right == nullptr? 0 : 1);
    }
    BSTNode<E> *getUniqueChild() {
        return left != nullptr? left : right;
    }
};

#endif // BSTNODE_H
