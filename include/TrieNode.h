#ifndef TRIENODE_H
#define TRIENODE_H

#include "BSTDictionary.h"
#include "List.h"
#include "ArrayList.h"


// Clase de TrieNode. Es utilizada para los arboles Trie.

class TrieNode {
public:
    int prefixCount;
    bool isFinal;
    List<int> *lineasAparicion;
    Dictionary<char, TrieNode*> *children;

    TrieNode() {
        prefixCount = 0;
        isFinal = false;
        children = new BSTDictionary<char, TrieNode*>();
        lineasAparicion = new ArrayList<int>(1);
    }
    ~TrieNode() {
        //clear();
        //children->clear();
        delete children;
        //lineasAparicion->clear();
        delete lineasAparicion;
    }
    bool contains(char c) {
        return children->contains(c);
    }
    TrieNode* getChild(char c) {
        return children->getValue(c);
    }
    void addChild(char c) {
        children->insert(c, new TrieNode());
    }
    void removeChild(char c) {
        children->remove(c);
    }
    int childrenCount() {
        return children->getSize();
    }
    List<char>* getChildren() {
        return children->getKeys();
    }
    List<TrieNode*>* getChildrenPointers() {
        return children->getValues();
    }
//    void clear(){
//        //lineasAparicion->clear();
//        //delete lineasAparicion;
//    }
    void initList(){
        lineasAparicion = new ArrayList<int>(100000);
    }
};

#endif // TRIENODE_H
