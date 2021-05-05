#include <iostream>
#include <string.h>
#include <time.h>
template<class T>
class Node {
public:
    T data;
    Node<T>* prev;
    Node<T>* next;
    T dataGiver();
    Node();
    Node(T data, Node<T>* prev, Node<T>* next);

    friend std::ostream& operator<<(std::ostream& os, const Node<T>& obj) {
        os << obj.data;
        return os;
    }
};

template<typename T>
T Node<T>::dataGiver(){
    return data;
}

template<class T>
Node<T>::Node() {
    this->prev = nullptr;
    this->next = nullptr;
}

template<class T>
Node<T>::Node(T data, Node<T>* prev, Node<T>* next) {
    this->data = data;
    this->prev = prev;
    this->next = next;
}


// General linked list implementation with some extras.
template<class T>
class LinkedList {
public:
    LinkedList();
    LinkedList(const T arr[], int arrSize);
    LinkedList(const LinkedList<T>& obj);

    ~LinkedList();

    Node<T>* getFirstNode() const;
    Node<T>* getLastNode() const;
    Node<T>* getNode(const T& data) const;

    int getNumberOfNodes() const;
    bool isEmpty() const;
    bool contains(Node<T>* node) const;

    void insertAtTheHead(const T& data);
    void insertAtTheTail(const T& data);
    void insertSorted(const T& data);

    void removeNode(Node<T>* node);
    void removeNode(const T& data);
    void removeAllNodes();

    void print() const;

    T* toArray() const;

    LinkedList<T>& operator=(const LinkedList<T>& rhs);
private:
    Node<T>* head;
    Node<T>* tail;
};

//Default constructor
template<class T>
LinkedList<T>::LinkedList() {
    this->head = NULL;
    this->tail = NULL;
}


template<class T>
LinkedList<T>::LinkedList(const T arr[], int arrSize) {
    head = NULL;
    tail = NULL;
    for (int i = 0; i < arrSize; i++) {
        if (head == NULL) {
            Node <T>* newNode = new Node<T>(arr[i], NULL, NULL);
            head = newNode;
            tail = newNode;
        }
        else {
            Node<T>* newNode = new Node<T>(arr[i], tail, NULL);
            tail->next = newNode;
            tail = newNode;
        }
    }
}

//Copy constructor
template<class T>
LinkedList<T>::LinkedList(const LinkedList<T>& obj) {
    head = NULL;
    tail = NULL;
    if (obj.head != NULL) {
        for (Node<T>* current = obj.head; current; current = current->next) {
            if (head == NULL) {
                Node <T>* newNode = new Node <T>(current->data, head, tail);
                head = newNode;
                tail = newNode;
            }
            else {
                Node<T>* newNode = new Node<T>(current->data, tail, NULL);
                tail->next = newNode;
                tail = newNode;
            }
        }
    }

}
//Destructor
template<class T>
LinkedList<T>::~LinkedList() {
    removeAllNodes();
}

template<class T>
Node<T>* LinkedList<T>::getFirstNode() const {
    if (head == NULL) {

        return NULL;

    }

    return head;
}

template<class T>
Node<T>* LinkedList<T>::getLastNode() const {
    if (head == NULL) {

        return NULL;

    }

    return tail;
}

//Gives a node corresponding data. If is not in the linked list, it returns NULL.
template<class T>
Node<T>* LinkedList<T>::getNode(const T& data) const {
    Node<T>* current = head;
    Node <T>* returning;
    int flag = 0;
    if (head != NULL) {

        while (current)
        {

            if (current->data == data) {
                flag = 1;
                returning = current;
                break;
            }
            current = current->next;
        }
        if (flag) {
            return returning;
        }
        else {
            return NULL;
        }
    }
    else {
        return NULL;
    }
}


template<class T>
int LinkedList<T>::getNumberOfNodes() const {

    Node<T>* current = head;
    int value = 0;
    while (current)
    {
        value++;
        current = current->next;
    }
    return value;
}

template<class T>
bool LinkedList<T>::isEmpty() const {

    if (head == NULL) {
        return true;
    }
    return false;

}


template<class T>
bool LinkedList<T>::contains(Node<T>* node) const {

    Node<T>* current = head;

    while (current) {
        if ((current->prev == node->prev)
        && (current->next == node->next)
        && (current->data == node->data)) {

            return true;

        }
        current = current->next;
    }
    return false;
}

template<class T>
void LinkedList<T>::insertAtTheHead(const T& data) {

    Node <T>* newNode = new Node<T>(data, NULL, NULL);
    if (head == NULL) {
        head = newNode;
        tail = newNode;
    }
    else {
        head->prev = newNode;
        newNode->next = head;
        head = newNode;
    }
}

template<class T>
void LinkedList<T>::insertAtTheTail(const T& data) {

    Node <T>* newNode = new Node<T>(data, NULL, NULL);
    if (head == NULL) {
        head = newNode;
        tail = newNode;
    }
    else {
        tail->next = newNode;
        newNode->prev = tail;
        tail = newNode;
    }
}

// Insert an element sorted in the linked list.
template<class T>
void LinkedList<T>::insertSorted(const T& data) {

    Node<T>* current = head;
    Node<T>* prev = NULL;

    if (head == NULL) {

        insertAtTheHead(data);

    }
    else if (head != NULL && ((current->data) < data)) {

        while (current) {

            if ((current->data) > data) {

                break;

            }

            prev = current;
            current = current->next;

        }
        if (current == NULL) {

            insertAtTheTail(data);

        }
        else if (current != NULL && ((current->data) > data)) {

            Node<T>* newNode = new Node<T>(data, NULL, NULL);
            newNode->prev = current->prev;
            newNode->prev->next = newNode;
            newNode->next = current;
            current->prev = newNode;

        }
    }
    else if (head != NULL && ((current->data) > data)) {

        insertAtTheHead(data);

    }

}

// Below two removeNode functions remove a node corresponding data or address of Node. This is overloaded
template<class T>
void LinkedList<T>::removeNode(Node<T>* node) {

    Node <T>* current = head;
    if (head != NULL) {
        if (getNumberOfNodes() == 1 && current == node) {
            delete node;
            head = tail = NULL;
        }
        else if (getNumberOfNodes() != 1) {
            while (current != node && current) {
                current = current->next;
            }
            if (current == tail) {
                tail->prev->next = NULL;
                tail = current->prev;
                delete node;
            }
            else if (current == head) {
                current->next->prev = NULL;
                head = current->next;
                delete node;
            }
            else if ((current->next == node->next) &&
            (current->prev == node->prev) &&
            (current->data == node->data)) {

                current->prev->next = current->next;
                current->next->prev = current->prev;
                delete node;

            }
        }
    }
}

template<class T>
void LinkedList<T>::removeNode(const T& data) {

    Node<T>* current = head;
    if (head != NULL) {
        if (getNumberOfNodes() == 1 && current->data == data) {
            delete current;
            head = tail = NULL;
            current = NULL;
        }
        else if (getNumberOfNodes() != 1) {
            while (current && current->data != data) {
                current = current->next;
            }
            if (current == tail) {
                tail->prev->next = NULL;
                tail = current->prev;
                delete current;
            }
            else if (current == head) {
                current->next->prev = NULL;
                head = current->next;
                delete current;
            }
            else if (current->data == data) {
                current->next->prev = current->prev;
                current->prev->next = current->next;
                delete current;
                current = NULL;
            }
            else {
                // DO NOTHING
            }

        }
    }
}


template<class T>
void LinkedList<T>::removeAllNodes() {

    Node<T>* current = head;
    if (head != NULL) {
        while (current) {
            Node<T>* next = current->next;
            delete current;
            current = NULL;
            current = next;
        }
        head = NULL;
        tail = NULL;
    }
}

//Prints the linked list
template<class T>
void LinkedList<T>::print() const {
    std::cout << "Printing the linked list ..." << std::endl;

    if (this->isEmpty()) {
        std::cout << "empty";
        return;
    }

    Node<T>* node = this->head;

    while (node) {
        std::cout << *node << " ";
        node = node->next;
    }
}

//Gives an array of elements of linked list

template<class T>
T* LinkedList<T>::toArray() const {

    T* myArr;
    Node<T>* current = head;
    int size = 0;
    if (head == NULL) {
        return NULL;
    }
    while (current) {
        size++;
        current = current->next;
    }
    myArr = new T[size];
    current = head;
    for (int i = 0; i < size; current = current->next, i++) {
        myArr[i] = (current->data);
    }
    return myArr;
}
template<class T>
LinkedList<T>& LinkedList<T>::operator=(const LinkedList<T>& rhs) {

    removeAllNodes();
    Node<T>* rhs_current = rhs.head;
    for (; rhs_current; rhs_current = rhs_current->next) {
        if (isEmpty()) {
            Node <T>* newNode = new Node<T>(rhs_current->data, NULL, NULL);
            head = newNode;
            tail = newNode;
        }
        else {
            insertAtTheTail(rhs_current->data);
        }
    }
    return *this;
}

// Time to test ..

int main(){

    LinkedList<int> lst;
    srand(time(NULL));    
    
    for(int i = rand() % 25; i > 0;i--){
        lst.insertAtTheHead(i*7);
    }
    
    for(int i = 0; i < rand() % 25;i++){
        lst.insertAtTheTail(i*7);
    }
    
    lst.print();
    std::cout << "\n";


    lst.removeNode(lst.getFirstNode());
    lst.print();
    std::cout << "\n";

    lst.removeNode(lst.getLastNode());
    lst.print();
    std::cout << "\n";

    
    lst.print();
    std::cout << "\n";
    std::cout << (lst.getFirstNode())->dataGiver();
    lst.removeNode((lst.getFirstNode())->dataGiver());
    lst.print();

    std::cout << "\n";
    lst.removeAllNodes();
    lst.print();
}
