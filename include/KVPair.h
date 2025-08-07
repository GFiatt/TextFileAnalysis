#ifndef KVPAIR_H
#define KVPAIR_H

#include <iostream>

using std::ostream;

template <typename K, typename V>
class KVPair {
public:
    K key;
    V value;

    KVPair(K key, V value) {
        this->key = key;
        this->value = value;
    }
    KVPair(K key) {
        this->key = key;
    }
    KVPair() {}
    bool operator ==(const KVPair<K, V> &other) {
        return key == other.key;
    }
    bool operator <(const KVPair<K, V> &other) {
        return key < other.key;
    }
    bool operator <=(const KVPair<K, V> &other) {
        return key <= other.key;
    }
    bool operator >(const KVPair<K, V> &other) {
        return key > other.key;
    }
    bool operator >=(const KVPair<K, V> &other) {
        return key >= other.key;
    }
    bool operator !=(const KVPair<K, V> &other) {
        return key != other.key;
    }
    void operator =(const KVPair<K, V> &other) {
        this->key = other.key;
        this->value = other.value;
    }
};

template <typename K, typename V>
ostream& operator<<(ostream& os, const KVPair<K, V>& p) {
    os << "(" << p.key << ", " << p.value << ")";
    return os;
}
#endif // KVPAIR_H
