#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "Dictionary.h"
#include "KVPair.h"
#include "LinkedList.h"
#include <string>
#include <cmath>

using namespace std;

// Clase de HashTable.

template <typename K, typename V>
class HashTable : public Dictionary<K, V> {
private:


    int h(K key) {
        return compress(hashCodeCyclicShift(key));
    }
    // Genera un código hash polinomial con constante de 33
    int hashCodePolynomial(K pKey) {
        int a = 33;
        int result = 0;
        char* bytes = reinterpret_cast<char*>(&pKey);
        for (unsigned int i = 0; i < sizeof(K); i++) {
            result += bytes[i] * pow(a, i);
        }
        return result;
    }
    // Genera un código hash con corrimiento cíclico con corrimientos
    // de 7 hacia la izquierda y 25 a la derecha. Usa XOR para unir
    // los bits.
    template <typename T>
    int hashCodeCyclicShift(T pKey) {
        int result = 0;
        char* bytes = reinterpret_cast<char*>(&pKey);
        for (unsigned int i = 0; i < sizeof(pKey); i++) {
            result = (result << (7)) ^ (result >> (25));
            result += (int) bytes[i];
        }
        return result;
    }
    int hashCodeCyclicShift(string pKey) {
        int result = 0;
        for (unsigned int i = 0; i < pKey.length(); i++) {
            result = (result << (7)) ^ (result >> (25));
            result += (int) pKey[i];
        }
        return result;
    }
    // Función de compresión por multiplicación, suma y división
    int compress(int pHashCode) {
        int a = 1097;
        int b = 1279;
        return abs(a * pHashCode + b) % nbuckets;
    }
    // Función que dibuja los bits de un entero, para propósitos
    // de prueba
    void printBits(int n) {
        int value = n;
        string result = "";
        for (int i = 0; i < 32; i++) {
            result = (value % 2? "o" : "_") + result;
            value = value / 2;
        }
        cout << result << endl;
    }
    // Método auxiliar que verifica que una llave no exista.
    // Si se encuentra la llave, lanza exepción.
    void checkNotExisting(K key) {
        int pos = h(key);
        if (buckets[pos].contains(key))
            throw runtime_error("Duplicated key.");
    }
    // Método auxiliar que verifica que una llave sí exista.
    // Si no se encuentra la llave, lanza excepción
    void checkExisting(K key) {
        int pos = h(key);
        if (!buckets[pos].contains(key)) {
            throw runtime_error("Key not found.");
        }
    }

public:
    LinkedList<KVPair<K, V>> *buckets;
    int nbuckets;
    HashTable(int nbuckets = 1021) {
        buckets = new LinkedList<KVPair<K, V> >[nbuckets];
        this->nbuckets = nbuckets;
    }
    ~HashTable() {
        delete [] buckets;
    }
    void insert(K key, V value) {
        checkNotExisting(key);
        KVPair<K, V> p(key, value);
        buckets[h(key)].append(p);
    }
    V remove(K key) {
        checkExisting(key);
        KVPair<K, V> p(key);
        int pos = h(key);
        int listPos = buckets[pos].indexOf(p);
        buckets[pos].goToPos(listPos);
        p = buckets[pos].remove();
        return p.value;
    }
    V getValue(K key) {
        checkExisting(key);
        int pos = h(key);
        KVPair<K, V> p(key);
        int listPos = buckets[pos].indexOf(p);
        buckets[pos].goToPos(listPos);
        p = buckets[pos].getElement();
        return p.value;
    }
    void setValue(K key, V value) {
        checkExisting(key);
        int pos = h(key);
        KVPair<K, V> p(key, value);
        int listPos = buckets[pos].indexOf(p);
        buckets[pos].goToPos(listPos);
        buckets[pos].remove();
        p.value = value;
        buckets[h(key)].insert(p);
    }
    void clear() {
        for (int i = 0; i < nbuckets; i++) {
            buckets[i].clear();
        }
    }
    bool contains(K key) {
        return buckets[h(key)].contains(key);
    }
    List<K>* getKeys() {
        List<K> *keys = new DLinkedList<K>();
        for (int i = 0; i < nbuckets; i++) {
            buckets[i].goToStart();
            while (!buckets[i].atEnd()) {
                KVPair<K, V> p;
                p = buckets[i].getElement();
                keys->append(p.key);
                buckets[i].next();
            }
        }
        return keys;
    }
    List<V>* getValues() {
        List<V> *values = new DLinkedList<V>();
        for (int i = 0; i < nbuckets; i++) {
            buckets[i].goToStart();
            while (!buckets[i].atEnd()) {
                KVPair<K, V> p;
                p = buckets[i].getElement();
                values->append(p.value);
                buckets[i].next();
            }
        }
        return values;
    }
    int getSize() {
        int size = 0;
        for (int i = 0; i < nbuckets; i++) {
            size += buckets[i].getSize();
        }
        return size;
    }
    void print() {
        for(int i=0;i<nbuckets;i++){
            buckets[i].print();
        }
    }

};

#endif // HASHTABLE_H
