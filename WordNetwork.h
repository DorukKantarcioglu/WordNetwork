/**
* Title: WordNetwork
* Author: DorukKantarcioglu
* Description: Interface file for the classes HashTable, Queue, LinkedList and WordNetwork.
*/

#ifndef WORDNETWORK_H_INCLUDED
#define WORDNETWORK_H_INCLUDED
#include <string>

const int WORD_COUNT = 5757;    /// 5757 words in the file

class HashTable
{
    public:
        HashTable();    /// Default constructor
        ~HashTable();   /// Destructor
        int searchByKey(const std::string) const;   /// Returns the index of the word
        std::string searchByIndex(const int) const; /// Returns the key of the word
        bool insert(const std::string, const int);
        bool remove(const std::string);

    protected:
        int hash(const std::string) const;  /// Computes the hash value for words

    private:
        static const int size = WORD_COUNT + 22;    /// First prime number after 5757
        struct Word {
            std::string key;
            int index;
            Word* next;
            Word() {    /// Struct default constructor
                key = "";
                index = -1;
                next = NULL;
            }
            Word(std::string key, int index): key(key), index(index) {  /// Struct parametrized constructor
                next = NULL;
            }
        };
        Word* table[size];  /// Pointer array for separate chaining
};

class Queue
{
    public:
        Queue();    /// Default constructor
        ~Queue();   /// Destructor
        bool isEmpty() const;
        bool enqueue(const int);
        bool dequeue();
        bool dequeue(int&);

    private:
        struct Node {
            int index;
            Node* next;
            Node(int index) : index(index) {    /// Struct parametrized constructor
                next = NULL;
            }
        };
        Node* front;
        Node* back;
};

class LinkedList
{
    public:
        LinkedList();   /// Default constructor
        ~LinkedList();  /// Destructor
        bool isEmpty() const;
        bool insert(const int);
        bool remove();
        bool remove(int&);

    private:
        struct Node {
            int index;
            Node* next;
            Node(int index) : index(index) {    /// Struct parametrized constructor
                next = NULL;
            }
        };
        Node* head;
};

class WordNetwork
{
    public:
        WordNetwork(const std::string, const std::string);  /// Parametrized constructor
        ~WordNetwork(); /// Destructor
        void listNeighbors(const std::string);
        void listNeighbors(const std::string, const int);
        void listConnectedComponents();
        void findShortestPath(const std::string, const std::string);

    protected:
        void insert(const int, const int);
        void remove(const int, const int);
        void listNeighbors(const int, const int, bool*&);
        void listConnectedComponents(const int, bool*&);
        bool findShortestPath(const int, const int, bool*&, int*&);

    private:
        bool** matrix;   /// Adjacency matrix
        HashTable wordTable;    /// Lookup table for words
};

#endif // WORDNETWORK_H_INCLUDED
