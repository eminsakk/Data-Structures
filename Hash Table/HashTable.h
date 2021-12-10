#ifndef __HASHTABLE__
#define __HASHTABLE__

#include "HashUtils.h"
#include "ItemNotFoundException.h"

#define BUCKET_SIZE 2

template <class K, class T>
class HashTable {
    struct Entry {
        K Key;             // the key of the entry
        T Value;   // the value of the entry
        bool Deleted;        // flag indicating whether this entry is deleted
        bool Active;         // flag indicating whether this item is currently used

        Entry() : Key(), Value(), Deleted(false), Active(false) {}
    };

    struct Bucket {
        Entry entries[BUCKET_SIZE];
    };

    int _capacity; // INDICATES THE TOTAL CAPACITY OF THE TABLE
    int _size; // INDICATES THE NUMBER OF ITEMS IN THE TABLE

    Bucket* _table; // THE HASH TABLE

    // == DEFINE HELPER METHODS & VARIABLES BELOW ==


public:
    
    // CONSTRUCTORS, ASSIGNMENT OPERATOR, AND THE DESTRUCTOR
    HashTable();
    // COPY THE WHOLE CONTENT OF RHS INCLUDING THE KEYS THAT WERE SET AS DELETED
    HashTable(const HashTable<K, T>& rhs);
    HashTable<K, T>& operator=(const HashTable<K, T>& rhs);
    ~HashTable();

    
    // INSERT THE ENTRY IN THE HASH TABLE WITH THE GIVEN KEY & VALUE
    // IF THE GIVEN KEY ALREADY EXISTS, THE NEW VALUE OVERWRITES
    // THE ALREADY EXISTING ONE. IF THE LOAD FACTOR OF THE TABLE IS GREATER THAN 0.6,
    // RESIZE THE TABLE WITH THE NEXT PRIME NUMBER.
    void Insert(const K& key, const T& value);

    
    // DELETE THE ENTRY WITH THE GIVEN KEY FROM THE TABLE
    // IF THE GIVEN KEY DOES NOT EXIST IN THE TABLE, THROW ItemNotFoundException()
    void Delete(const K& key);

    
    // IT SHOULD RETURN THE VALUE THAT CORRESPONDS TO THE GIVEN KEY.
    // IF THE KEY DOES NOT EXIST, THROW ItemNotFoundException()
    T& Get(const K& key) const;

    
    // AFTER THIS FUNCTION IS EXECUTED THE TABLE CAPACITY MUST BE
    // EQUAL TO newCapacity AND ALL THE EXISTING ITEMS MUST BE REHASHED
    // ACCORDING TO THIS NEW CAPACITY.
    // WHEN CHANGING THE SIZE, YOU MUST REHASH ALL OF THE ENTRIES
    void Resize(int newCapacity);


    int Capacity() const; // RETURN THE TOTAL CAPACITY OF THE TABLE
    int Size() const; // RETURN THE NUMBER OF ACTIVE ITEMS
    void getKeys(K* keys); // PUT THE ACTIVE KEYS TO THE GIVEN INPUT PARAMETER
};


template <class K, class T>
HashTable<K, T>::HashTable() {
    
    _size = 0;
    _capacity = 11;
    _capacity = NextCapacity(_capacity);
    _table = new Bucket[_capacity];
}

template <class K, class T>
HashTable<K, T>::HashTable(const HashTable<K, T>& rhs) {
    
    _capacity = rhs.Capacity();
    _size = rhs.Size();
    _table = new Bucket[_capacity];
    for (int i = 0; i < rhs.Capacity(); i++) {
        _table[i].entries[0] = (rhs._table)[i].entries[0];
        _table[i].entries[1] = (rhs._table)[i].entries[1];
    }
    
}

template <class K, class T>
HashTable<K, T>& HashTable<K, T>::operator=(const HashTable<K, T>& rhs) {
    
    _size = _capacity = 0;
    delete [] _table;
    
    _capacity = rhs.Capacity();
    _size = rhs.Size();
    _table = new Bucket[_capacity];

    for (int i = 0; i < _capacity; i++) {
        _table[i].entries[0] = (rhs._table)[i].entries[0];
        _table[i].entries[1] = (rhs._table)[i].entries[1];
    }
    return *this;
}

template <class K, class T>
HashTable<K, T>::~HashTable() {
    
    _size = _capacity = 0;
    delete[] _table;
    _table = NULL;
}

template <class K, class T>
void HashTable<K, T>::Insert(const K& key, const T& value) {
    
    
    
    int hashingIndex = Hash(key);
    int probingIndex;

    for (int quadTracker = 0; quadTracker < _capacity; quadTracker++) {
        probingIndex = hashingIndex + pow(quadTracker, 2);
        if (probingIndex >= _capacity) {
            probingIndex %= _capacity;
        }
        
        if (_table[probingIndex].entries[0].Active == false || _table[probingIndex].entries[0].Deleted == true) {
            _table[probingIndex].entries[0].Key = key; 
            _table[probingIndex].entries[0].Value = value;
            _table[probingIndex].entries[0].Active = true;
            _table[probingIndex].entries[0].Deleted = false;
            _size++;
            break;
        }
        
        else if ((_table[probingIndex].entries[0].Active == true && _table[probingIndex].entries[0].Deleted == false) && (_table[probingIndex].entries[0].Key == key)) {
            _table[probingIndex].entries[0].Value = value; 
            break;
        }
        
        else if ((_table[probingIndex].entries[0].Active == true && _table[probingIndex].entries[0].Deleted == false) && (_table[probingIndex].entries[0].Key != key)) {
            if (_table[probingIndex].entries[1].Active == false || _table[probingIndex].entries[1].Deleted == true) {
                _table[probingIndex].entries[1].Key = key;
                _table[probingIndex].entries[1].Value = value;
                _table[probingIndex].entries[1].Active = true;
                _table[probingIndex].entries[1].Deleted = false;
                _size++;
                break;
            }
            
            else if ((_table[probingIndex].entries[1].Active == true && _table[probingIndex].entries[1].Deleted == false) && (_table[probingIndex].entries[1].Key == key)) {
                _table[probingIndex].entries[1].Value = value; 
                break;
            }

        }
        
    }
    
    if (double(_size) / (double(2*_capacity)) >= double(0.6)) {
        Resize(NextCapacity(_capacity));   
    }
}

template <class K, class T>
void HashTable<K, T>::Delete(const K& key) {
    
    
    int hashingIndex = Hash(key);
    int probingIndex;
    int flag = 1;
    for (int quadTracker = 0; quadTracker < _capacity; quadTracker++) {
        probingIndex = hashingIndex + pow(quadTracker, 2);
        
        if (probingIndex >= _capacity) {
            probingIndex %= _capacity;
        }

        if (_table[probingIndex].entries[0].Active == true && _table[probingIndex].entries[0].Deleted == false && _table[probingIndex].entries[0].Key == key) {
            _table[probingIndex].entries[0].Deleted = true;
            _size--;
            break;
        }

        else if (_table[probingIndex].entries[1].Active == true && _table[probingIndex].entries[1].Deleted == false && _table[probingIndex].entries[1].Key == key) {
            _table[probingIndex].entries[1].Deleted = true;
            _size--;
            break;
        }

        else if (_table[probingIndex].entries[0].Active == true && _table[probingIndex].entries[1].Active == false) {
            flag = 0;
            break;
        }

        else if (_table[probingIndex].entries[0].Active == false && _table[probingIndex].entries[1].Active == false) {
            flag = 0;
            break;
        }

    }

    if (!flag) {
        throw ItemNotFoundException();
    }
    
}

template <class K, class T>
T& HashTable<K, T>::Get(const K& key) const {
     
    int hashingIndex = Hash(key);
    int probingIndex;
    int flag = 1;
    for (int quadTracker = 0; quadTracker < _capacity; quadTracker++) {
        probingIndex = hashingIndex + pow(quadTracker, 2);
        
        if (probingIndex >= _capacity) {
            probingIndex %= _capacity;
        }

        if (_table[probingIndex].entries[0].Active == true && _table[probingIndex].entries[0].Deleted == false && _table[probingIndex].entries[0].Key == key) {
            return _table[probingIndex].entries[0].Value;  
        }

        else if (_table[probingIndex].entries[1].Active == true && _table[probingIndex].entries[1].Deleted == false && _table[probingIndex].entries[1].Key == key) {
            return _table[probingIndex].entries[1].Value;  
        }
        else if (_table[probingIndex].entries[0].Active == true && _table[probingIndex].entries[1].Active == false) {
            flag = 0;
            break;
        }

        else if (_table[probingIndex].entries[0].Active == false && _table[probingIndex].entries[1].Active == false) {
            flag = 0;
            break;
        }

    }

    if (!flag) {
        throw ItemNotFoundException();
    }

}


template <class K, class T>
void HashTable<K, T>::Resize(int newCapacity) {
    Entry *allData = new Entry[_size];
    int numEntries = 0;

    for (int i = 0; i < _capacity; i++) {
        
        if (_table[i].entries[0].Active) {
            allData[numEntries] = _table[i].entries[0];
            numEntries++;
        }

        if (_table[i].entries[1].Active) {
            allData[numEntries] = _table[i].entries[1];
            numEntries++;
        }

    }

    delete[] _table;

    _table = new Bucket[newCapacity];
    _capacity = newCapacity;
    _size = 0;

    for (int i = 0; i < numEntries; i++) {
        Insert(allData[i].Key, allData[i].Value);
    }
    delete [] allData;
}


template <class K, class T>
int HashTable<K, T>::Capacity() const {
    return _capacity;
}

template <class K, class T>
int HashTable<K, T>::Size() const {
    return _size;
}


template <class K, class T>
void HashTable<K, T>::getKeys(K* keys) {
    int index = 0;

    for (int i = 0; i < _capacity; i++) {
        Bucket& bucket = _table[i];
        for (int j = 0; j < BUCKET_SIZE; j++) {
            if (bucket.entries[j].Active && !bucket.entries[j].Deleted) {
                keys[index++] = bucket.entries[j].Key;
            }
        }
    }
}

#endif
