/**
* Title: WordNetwork
* Author: DorukKantarcioglu
* Description: Implementation/driver file for the classes HashTable, Queue, LinkedList and WordNetwork. Includes the main function for testing purposes.
*/

#include "WordNetwork.h"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

HashTable::HashTable()
{
    for (int i = 0; i < size; i++) {
        table[i] = new Word();
    }
}

HashTable::~HashTable()
{
    for (int i = 0; i < size; i++) {
        Word* currPtr = table[i];
        while (currPtr) {   /// Traversing each bucket
            Word* delPtr = currPtr;
            currPtr = currPtr->next;
            delete delPtr;
        }
    }
}

int HashTable::searchByKey(const string key) const
{
    int hashValue = hash(key);
    Word* currPtr = table[hashValue];
    while (currPtr) {
        if (currPtr->key == key) {
            return currPtr->index;
        }
        currPtr = currPtr->next;
    }
    return -1;
}

string HashTable::searchByIndex(const int index) const
{
    for (int i = 0; i < size; i++) {
        Word* currPtr = table[i];
        while (currPtr) {
            if (currPtr->index == index) {
                return currPtr->key;
            }
            currPtr = currPtr->next;
        }
    }
    return "";
}

bool HashTable::insert(const string key, const int index)
{
    int hashValue = hash(key);
    if (table[hashValue]->key.empty()) {
        table[hashValue]->key = key;
        table[hashValue]->index = index;
    }
    else {
        Word* currPtr = table[hashValue];
        Word* newPtr = new Word(key, index);
        while (currPtr->next) {
            currPtr = currPtr->next;
        }
        currPtr->next = newPtr;
    }
    return true;
}

bool HashTable::remove(const string key)
{
    int hashValue = hash(key);
    if (table[hashValue]->key.empty() && table[hashValue]->index == -1) {
        return false;
    }
    else if (table[hashValue]->key == key && !table[hashValue]->next) {
        table[hashValue]->key = "";
        table[hashValue]->index = -1;
        return true;
    }
    else if (table[hashValue]->key == key) {
        Word* delPtr = table[hashValue];
        table[hashValue] = table[hashValue]->next;
        delete delPtr;
        return true;
    }
    else {
        Word* currPtr = table[hashValue]->next;
        Word* prevPtr = table[hashValue];
        while (currPtr && currPtr->key != key) {
            prevPtr = currPtr;
            currPtr = currPtr->next;
        }
        if (!currPtr) {
            return false;
        }
        else {
            Word* delPtr = currPtr;
            currPtr = currPtr->next;
            prevPtr->next = currPtr;
            delete delPtr;
            return true;
        }
    }
}

int HashTable::hash(const string key) const
{
    int hashValue = 0;
    for (unsigned int i = 0; i < key.length(); i++) {
        hashValue = 37 * hashValue + key[i];
    }
    hashValue %= size;
    if (hashValue < 0) {
        hashValue += size;
    }
    return hashValue;
}

Queue::Queue()
{
    front = NULL;
    back = NULL;
}

Queue::~Queue()
{
    while (!isEmpty()) {
        dequeue();
    }
}

bool Queue::isEmpty() const
{
    return !back;
}

bool Queue::enqueue(const int newIndex)
{
    Node *newPtr = new Node(newIndex);
    if (isEmpty()) {
        front = newPtr;
    }
    else {
        back->next = newPtr;
    }
    back = newPtr;
    return true;
}

bool Queue::dequeue()
{
    if (isEmpty()) {
        return false;
    }
    else {
        Node *tempPtr = front;
        if (front == back) {
            front = NULL;
            back = NULL;
        }
        else {
            front = front->next;
        }
        tempPtr->next = NULL;
        delete tempPtr;
        return true;
    }
}

bool Queue::dequeue(int& frontIndex)
{
    if (isEmpty()) {
        return false;
    }
    else {
        frontIndex = front->index;
        dequeue();
        return true;
    }
}

LinkedList::LinkedList()
{
    head = NULL;
}

LinkedList::~LinkedList()
{
    Node* currPtr;
    while (head) {
        currPtr = head;
        head = head->next;
        delete currPtr;
    }
}

bool LinkedList::isEmpty() const
{
    return !head;
}

bool LinkedList::insert(const int newIndex)
{
    Node* newPtr = new Node(newIndex);
    if (isEmpty()) {
        head = newPtr;
    }
    else {
        newPtr->next = head;
        head = newPtr;
    }
    return true;
}

bool LinkedList::remove()
{
    if (isEmpty()) {
        return false;
    }
    else {
        Node* currPtr = head;
        head = head->next;
        delete currPtr;
        return true;
    }
}

bool LinkedList::remove(int& headIndex)
{
    if (isEmpty()) {
        return false;
    }
    else {
        headIndex = head->index;
        Node* currPtr = head;
        head = head->next;
        delete currPtr;
        return true;
    }
}

WordNetwork::WordNetwork(const string vertexFile, const string edgeFile)
{
    /// Initialize the adjacency matrix
    matrix = new bool*[WORD_COUNT];
    for (int i = 0; i < WORD_COUNT; i++) {
        matrix[i] = new bool[WORD_COUNT];
        for (int j = 0; j < WORD_COUNT; j++) {
            matrix[i][j] = false;
        }
    }
    /// Read the vertices and insert them to the hash table
    ifstream vertices(vertexFile);
    string line;
    int index = 0;
    while(vertices >> line) {
        wordTable.insert(line, index++);
    }
    /// Read the edges of the graph and mark them on the graph
    ifstream edges(edgeFile);
    string firstWord, secondWord;
    while (edges >> line) {
        istringstream iss(line);
        getline(iss, firstWord, ',');
        getline(iss, secondWord, '\n');
        insert(wordTable.searchByKey(firstWord), wordTable.searchByKey(secondWord));
    }
}

WordNetwork::~WordNetwork()
{
    for (int i = 0; i < WORD_COUNT; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
}

void WordNetwork::listNeighbors(const string word)
{
    if (wordTable.searchByKey(word) == -1) {
        cout << "The word " << word << " does not exist in the graph." << endl;
    }
    else {
        cout << "Neighbors of " << word << ":" << endl;
        for (int i = 0; i < WORD_COUNT; i++) {
            if (matrix[wordTable.searchByKey(word)][i]) {
                cout << wordTable.searchByIndex(i) << endl;
            }
        }
    }
}

void WordNetwork::listNeighbors(const string word, const int distance)  /// Utilizes depth-first search
{
    if (wordTable.searchByKey(word) == -1) {
        cout << "The word " << word << " does not exist in the graph." << endl;
    }
    else {
        bool* visited = new bool[WORD_COUNT];
        for (int i = 0; i < WORD_COUNT; i++) {
            visited[i] = false;
        }
        cout << "Neighbors of " << word << " within distance " << distance << ":" << endl;
        listNeighbors(wordTable.searchByKey(word), distance, visited);
        delete[] visited;
    }
}

void WordNetwork::listConnectedComponents() /// Utilizes depth-first search
{
    bool* visited = new bool[WORD_COUNT];
    for (int i = 0; i < WORD_COUNT; i++) {
        visited[i] = false;
    }
    int count = 1;
    for (int i = 0; i < WORD_COUNT; i++) {
        if (!visited[i]) {
            cout << "Connected component " << count++ << ":" << endl;
            listConnectedComponents(i, visited);
        }
    }
    delete[] visited;
}

void WordNetwork::findShortestPath(const string word1, const string word2)  /// Utilizes breadth-first search, as edges have no weight
{
    bool* visited = new bool[WORD_COUNT];
    int* previous = new int[WORD_COUNT];
    for (int i = 0; i < WORD_COUNT; i++) {
        visited[i] = false;
        previous[i] = -1;
    }
    if (!findShortestPath(wordTable.searchByKey(word1), wordTable.searchByKey(word2), visited, previous)) {
        cout << "Unable to find shortest path from " << word1 << " to " << word2 << "." << endl;
    }
    else {
        int wordIndex = wordTable.searchByKey(word2);
        LinkedList path;
        path.insert(wordIndex);
        while (previous[wordIndex] != -1) {
            path.insert(previous[wordIndex]);
            wordIndex = previous[wordIndex];
        }
        cout << "Shortest path from " << word1 << " to " << word2 << ":"  << endl;
        int newIndex;
        while (path.remove(newIndex)) {
            cout << wordTable.searchByIndex(newIndex) << endl;
        }
    }
    delete[] visited;
    delete[] previous;
}

void WordNetwork::insert(const int row, const int column)
{
    /// Undirected graph insertion
    matrix[row][column] = true;
    matrix[column][row] = true;
}

void WordNetwork::remove(const int row, const int column)
{
    /// Undirected graph removal
    matrix[row][column] = false;
    matrix[column][row] = false;
}

void WordNetwork::listNeighbors(const int index, const int distance, bool*& visited)
{
    if (distance < 0) {
        return;
    }
    visited[index] = true;
    cout << wordTable.searchByIndex(index) << endl;
    for (int i = 0; i < WORD_COUNT; i++) {
        if (matrix[index][i] && !visited[i]) {
            listNeighbors(i, distance - 1, visited);
        }
    }
}

void WordNetwork::listConnectedComponents(const int index, bool*& visited)
{
    visited[index] = true;
    cout << wordTable.searchByIndex(index) << endl;
    for (int i = 0; i < WORD_COUNT; i++) {
        if (matrix[index][i] && !visited[i]) {
            listConnectedComponents(i, visited);
        }
    }
}

bool WordNetwork::findShortestPath(const int origin, const int destination, bool*& visited, int*& previous)
{
    if (origin == -1 || destination == -1) {
        return false;
    }
    Queue q;
    q.enqueue(origin);
    visited[origin] = true;
    int next;
    while (q.dequeue(next)) {
        for (int i = 0; i < WORD_COUNT; i++) {
            if (matrix[next][i] && !visited[i]) {
                q.enqueue(i);
                visited[i] = true;
                previous[i] = next;
                if (i == destination) {
                    return true;
                }
            }
        }
    }
    return false;
}

int main(int argc, char** argv)
{
    WordNetwork graph(argv[1], argv[2]);    /// Running the program with the following arguments: "words_vertices.txt words_edges.txt"
    graph.listNeighbors("doruk");
    cout << endl;
    graph.listNeighbors("trace");
    cout << endl;
    graph.listNeighbors("barry", 2);
    cout << endl;
    graph.listNeighbors("roger", 3);
    cout << endl;
    graph.findShortestPath("graph", "lords");
    cout << endl;
    graph.findShortestPath("price", "rover");
    cout << endl;
    graph.listConnectedComponents();    /// 853 connected components in the graph
}
