#ifndef TRIE_H
#define TRIE_H

#include "TrieNode.h"
#include "KVPair.h"
#include "ArrayList.h"
#include <string>
#include <iostream>
#include <stdexcept>

using namespace std;

// Clase de Trie. Utiliza el TrieNode y se usa para almacenar palabras.

class Trie {
private:
    TrieNode* root;

    // Encontrar un nodo dentro del árbol
    // Retorna su dirección si lo encuentra, si no null.
    TrieNode* findNode(string prefix) {
        TrieNode* current = root;
        for (unsigned int i = 0; i < prefix.size(); i++) {
            if (current->contains(prefix[i])) {
                current = current->getChild(prefix[i]);
            } else
                return nullptr;
        }
        return current;
    }
    void clearAux(TrieNode* current) {
        if (current->childrenCount() == 0) {
            delete current;
            return;
        }
        List<TrieNode*> *children = current->getChildrenPointers();
        children->goToStart();
        while (!children->atEnd()) {
            clearAux(children->getElement());
            children->next();
        }
        delete children;
        delete current;
    }
    void getMatchesAux(TrieNode *current, string prefix, List<string> *result) {
        if (current->isFinal)
            result->append(prefix);
        List<char> *children = current->getChildren();
        for (children->goToStart(); !children->atEnd(); children->next()) {
            char c = children->getElement();
            string newPrefix = prefix + c;
            getMatchesAux(current->getChild(c), newPrefix, result);
        }
        delete children;
    }
    void getMatchesAuxV2(TrieNode *current, string prefix, List<KVPair<int, string>> *result) {
        if (current->isFinal) {
//            KVPair<int, string> pair = KVPair<int, string>(current->lineasAparicion.getSize(), prefix);
//            pair.key = (x);
//            pair.value = (prefix);
            result->append(KVPair<int, string>(current->lineasAparicion->getSize(), prefix));
        }
        List<char> *children = current->getChildren();
        for (children->goToStart(); !children->atEnd(); children->next()) {
            char c = children->getElement();
            string newPrefix = prefix + c;
            getMatchesAuxV2(current->getChild(c), newPrefix, result);
        }
        delete children;
    }
    void getMatchesNAux(TrieNode *current ,int n, List<string> *result, string palabra) {       //                  a
        if (current->isFinal && n==0)                                                 // b c d e f g h i j k l m n o p q r s t u
            result->append(palabra);
        else{
            List<char> *children = current->getChildren();
            for (children->goToStart(); !children->atEnd(); children->next()) {
                char c = children->getElement();
                string newPrefix = palabra + c;
                getMatchesNAux(current->getChild(c), n-1, result, newPrefix);
            }
        }
    }

public:
    Trie() {
        root = new TrieNode();
    }
    ~Trie() {
        clear();
        delete root;
    }
    void insert(string word, int linea) {                          // [23, 765, 2342]
        if (!containsWord(word)){
            //throw runtime_error("Word already in trie.");
            TrieNode* current = root;
            for (unsigned int i = 0; i < word.size(); i++) {
                current->prefixCount++;
                if (!current->contains(word[i]))
                    current->addChild(word[i]);
                current = current->getChild(word[i]);
            }
            current->prefixCount++;
            current->isFinal = true;
            current->initList();
            current->lineasAparicion->append(linea);
        }
        else{
        TrieNode *current = findNode(word);
        //current->lineasAparicion->goToPos(current->lineasAparicion->getSize()-1);
        //current->lineasAparicion->goToEnd();
        //current->lineasAparicion->previous();

        if(current->lineasAparicion->getElement()!=linea)
            current->lineasAparicion->append(linea);
        }
    }
    bool containsWord(string word) {
        TrieNode *current = findNode(word);
        if (current == nullptr)
            return false;
        return current->isFinal;
    }
    bool containsPrefix(string prefix) {
        TrieNode *current = findNode(prefix);
        return current != nullptr;
    }
    int prefixCount(string prefix) {
        TrieNode *current = findNode(prefix);
        if (current == nullptr)
            return 0;
        return current->prefixCount;
    }
    void remove(string word) {
        if (!containsWord(word))
            throw runtime_error("Word not in trie.");
        TrieNode *current = root;
        for (unsigned int i = 0; i < word.size(); i++) {
            current->prefixCount--;
            TrieNode *child = current->getChild(word[i]);
            if (child->prefixCount == 1)
                current->removeChild(word[i]);
            if (current->prefixCount == 0)
                delete current;
            current = child;
        }
        current->prefixCount--;
        if (current->prefixCount == 0)
            delete current;
        else
            current->isFinal = false;
    }
    void clear() {
        clearAux(root);
        root = new TrieNode();
    }
    List<string>* getMatches(string prefix) {
        List<string> *result = new DLinkedList<string>();
        TrieNode *current = findNode(prefix);
        if (current != nullptr) {
            getMatchesAux(current, prefix, result);
        }
        return result;
    }
    List<KVPair<int, string>>* getMatchesV2(string prefix) {
        List<KVPair<int, string>>* result = new DLinkedList<KVPair<int, string>>();
        TrieNode *current = findNode(prefix);
        if (current != nullptr) {
            getMatchesAuxV2(current, prefix, result);
        }
        return result;
    }
    List<string>* getMatchesN(int n) {
        List<string> *result = new DLinkedList<string>();
        TrieNode *current = root;
        if (current != nullptr) {
            getMatchesNAux(current, n, result, "");
        }
        return result;
    }
    int getSize() {
        return root->prefixCount;
    }
    void print() {
        List<string> *words = getMatches("");
        words->print();
        delete words;
    }
    List<int>* getAparicionesPalabra(string word){//     a       b c d e f g
        TrieNode *current = findNode(word);//       b c d e f
        return current->lineasAparicion;
    }
    List<char>* getRoot(){
        return root->children->getKeys();
    }

};

#endif // TRIE_H
