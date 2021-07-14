#include <cmath>
#include <limits>
#include <string>
#include <climits>
#include <iostream>

/*------------------------------------------------------------Exception----------------------------------------------------------------------------------------*/
class ItemNotFoundException : public std::exception{
public:
    const char * what() const throw() {
        return "Item Not Found in the Hash Table!";
    }
};
/*------------------------------------------------------------Exception ends------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------Hash Table helper functions----------------------------------------------------------------------*/


/*
 * Returns the hash value of the given key
 */
int Hash(const std::string& key);

/*
 * Returns the hash value of the given key
 */
int Hash(int key);

/*
 * Finds the next appropriate hash table size from a given number
 */
int NextCapacity(int prev);
const int Primes[] = {
        7, 13, 29, 43, 73, 107, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197, 199,
        211, 223, 227, 229, 233, 239, 241, 251, 257, 263, 269, 271, 277, 281, 283, 293,
        307, 311, 313, 317, 331, 337, 347, 349, 353, 431, 521, 631, 761, 919,
                       1103, 1327, 1597, 1931, 2333, 2801, 3371, 4049, 4861, 5839, 7013, 8419, 10103, 12143, 14591,
                       17519, 21023, 25229, 30293, 36353, 43627, 52361, 62851, 75431, 90523, 108631, 130363, 156437,
                       187751, 225307, 270371, 324449, 389357, 467237, 560689, 672827, 807403, 968897, 1162687, 1395263,
                       1674319, 2009191, 2411033, 2893249, 3471899, 4166287, 4999559, 5999471, 7199369, 9270769, 14000839 };

#define A 54059
#define B 76963
#define C 86969
unsigned UniversalHash(const std::string& data, int size) {
    unsigned h = 31;
    for (int i = 0; i < size; ++i) {
        h = (h * A) ^ (data[i] * B);
    }
    return h;
}

int Hash(const std::string& key) {
    return (UniversalHash(key, key.length()) & 0x7FFFFFFF);
}

int Hash(int key) {
    return (UniversalHash(std::to_string(key), std::to_string(key).length()) & 0x7FFFFFFF);
}

template<class T, int N>
static int length(const T(&arr)[N]) {
    return N;
}

static bool isPrime(int num) {
    if ((num & 1) != 0) {
        int limit = static_cast<int>(std::sqrt(num));
        for (int divisor = 3; divisor <= limit; divisor += 2) {
            if ((num % divisor) == 0)
                return false;
        }
        return true;
    }
    return (num == 2);
}

int NextCapacity(int from) {
    if (from > Primes[length(Primes) - 1]) {
        for (int i = (from | 1); i < std::numeric_limits<int>::max(); i += 2) {
            if (isPrime(i) && ((i - 1) % 101 != 0))
                return i;
        }

        return from;
    }

    for (int i = 0; i < length(Primes); ++i) {
        if (Primes[i] > from) {
            return Primes[i];
        }
    }

    return -1; 
}





/*------------------------------------------------------------Hash Table helper functions end----------------------------------------------------------------------*/




/*----------------------------------------------------------- Hash Table class implementation ----------------------------------------------------------------------*/
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




public:

    // CONSTRUCTORS, ASSIGNMENT OPERATOR, AND THE DESTRUCTOR
    HashTable();
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
    delete[] _table;

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

        if (_table[probingIndex].entries[0].Active == false || _table[probingIndex].entries[0].Deleted == true) {/*Active olmayan veya silinmiş 0. kısma ekleme ve break*/
            _table[probingIndex].entries[0].Key = key;
            _table[probingIndex].entries[0].Value = value;
            _table[probingIndex].entries[0].Active = true;
            _table[probingIndex].entries[0].Deleted = false;
            _size++;
            break;
        }

        else if ((_table[probingIndex].entries[0].Active == true && _table[probingIndex].entries[0].Deleted == false) && (_table[probingIndex].entries[0].Key == key)) {
            _table[probingIndex].entries[0].Value = value; /*if keys are equal, overwrite*/
            break;
        }

        else if ((_table[probingIndex].entries[0].Active == true && _table[probingIndex].entries[0].Deleted == false) && (_table[probingIndex].entries[0].Key != key)) {/*Active and undeleted*/
            if (_table[probingIndex].entries[1].Active == false || _table[probingIndex].entries[1].Deleted == true) {
                _table[probingIndex].entries[1].Key = key;
                _table[probingIndex].entries[1].Value = value;
                _table[probingIndex].entries[1].Active = true;
                _table[probingIndex].entries[1].Deleted = false;
                _size++;
                break;
            }

            else if ((_table[probingIndex].entries[1].Active == true && _table[probingIndex].entries[1].Deleted == false) && (_table[probingIndex].entries[1].Key == key)) {
                _table[probingIndex].entries[1].Value = value; /*overwriting for same key values.*/
                break;
            }

        }

    }

    if (double(_size) / (double(2 * _capacity)) >= double(0.6)) {
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
            return _table[probingIndex].entries[0].Value;  /* 0. indexte mi değil mi ?*/
        }

        else if (_table[probingIndex].entries[1].Active == true && _table[probingIndex].entries[1].Deleted == false && _table[probingIndex].entries[1].Key == key) {
            return _table[probingIndex].entries[1].Value;  /* 1. indexte mi değil mi ?*/
        }
        else if (_table[probingIndex].entries[0].Active == true && _table[probingIndex].entries[1].Active == false) {/*2 indexte boş ise */
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
    Entry* allData = new Entry[_size];
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
    delete[] allData;
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
/*----------------------------------------------------------TESTING*----------------------------------------------------------*/

int main(){

    HashTable<std::string, int> table;
    table.Insert("one",1);
    table.Insert("two",2);
    table.Insert("three",3);
    table.Insert("four",4);
    table.Insert("five",5);
    table.Insert("six",6);
    table.Insert("seven",7);
    table.Insert("eight",8);
    table.Insert("nine",9);
    table.Insert("ten",10);


    std::cout << "Getting one:  "<<table.Get("one") << std::endl;
    std::cout << "Getting three:  "<<table.Get("three") << std::endl;
    std::cout << "Deleting one, four, eight" << std::endl;
    table.Delete("one");
    table.Delete("four");
    table.Delete("eight");
    std::cout << "Table capacity and size: " << table.Capacity() << " / " << table.Size()<<std::endl;
    std::cout << "Deleting nine" << std::endl;
    table.Delete("nine");
    std::cout << "Table capacity and size: " << table.Capacity() << " / " << table.Size()<<std::endl;



}
