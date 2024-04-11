#ifndef ARRAY_PRIORITY_QUEUE_HPP
#define ARRAY_PRIORITY_QUEUE_HPP

#include "PriorityQueue.hpp"
#include "Element.hpp"
#include <string>
#include <iostream>
#include <fstream>

template<typename T>
class ArrayPriorityQueue : public PriorityQueue<T> {

public:

    /*
        * Default constructor
        * Initializes the arrays' capacity and size with default capacity (100)
        * @param none
        * @return none
    */
    ArrayPriorityQueue() {
        elements = new Element<T>[100];
        numElements = 0;
        capacity = 100;
    }

    /*
        * Constructor
        * Initializes the arrays' capacity and size with given capacity
        * @param capacity capacity of the array
        * @return none
    */
    ArrayPriorityQueue(size_t capacity) {
        elements = new Element<T>[capacity];
        numElements = 0;
        this->capacity = capacity;
    }

    /*
        * Constructor
        * Initializes the arrays' capacity and size from a file
        * @param filename name of the file to read from
        * @return none
    */
    ArrayPriorityQueue(const char* filename) {
        std::ifstream file(filename);
        if (!file) {
            throw std::string("File not found");
        }
        capacity = 100; // domyślna pojemność
        elements = new Element<T>[capacity];
        numElements = 0;
        T value;
        int priority;
        while (file >> value >> priority) {
            if (numElements >= capacity) {
                // Zwiększ pojemność, jeśli potrzebne
                capacity *= 2;
                Element<T>* newElements = new Element<T>[capacity];
                for (int i = 0; i < numElements; ++i) {
                    newElements[i] = elements[i];
                }
                delete[] elements;
                elements = newElements;
            }
            insert(value, priority);
        }
        file.close();
    }


    /*
        * Constructor
        * Initializes the arrays' capacity and size from a file with given capacity
        * @param filename name of the file to read from
        * @param capacity capacity of the array
        * @return none
    */
    ArrayPriorityQueue(const char* filename, size_t capacity) {
        std::ifstream file(filename);
        if (!file) {
            throw std::string("File not found");
        }
        this->capacity = capacity;
        elements = new Element<T>[capacity];
        numElements = 0;
        T value;
        int priority;
        while (file >> value >> priority) {
            insert(value, priority);
        }
    }

    /*
        * Destructor
        * Deletes the array of elements
        * @param none
        * @return none
    */
    ~ArrayPriorityQueue() {
        delete[] elements;
    }

    /*
        * Inserts an element into the priority queue
        * @param e element to insert
        * @param p priority of the element
        * @return none
    */
    void insert(T e, int p) {
        if (numElements == capacity) {
            Element<T>* temp = new Element<T>[capacity * 2];
            for (int i = 0; i < numElements; i++) {
                temp[i] = elements[i];
            }
            delete[] elements;
            elements = temp;
            capacity *= 2;
        }
        Element<T> newElement(e, p);
        elements[numElements] = newElement;
        numElements++;
    }

    /*
        * Extracts the element with the highest priority from the priority queue and removes it
        * @param none
        * @return element with the highest priority
    */
    T extractMax() {
        if (numElements == 0) {
            throw std::string("Priority Queue is empty");
        }
        int maxIndex = 0;
        for (int i = 1; i < numElements; i++) {
            if (elements[i].priority > elements[maxIndex].priority) {
                maxIndex = i;
            }
        }
        T maxElement = elements[maxIndex].value;
        for (int i = maxIndex; i < numElements - 1; i++) {
            elements[i] = elements[i + 1];
        }
        numElements--;
        return maxElement;
    }

    /*
        * Finds the element with the highest priority in the priority queue without removing it
        * @param none
        * @return element with the highest priority
    */
    T findMax() const {
        if (numElements == 0) {
            throw std::string("Priority Queue is empty");
        }
        int maxIndex = 0;
        for (int i = 1; i < numElements; i++) {
            if (elements[i].priority > elements[maxIndex].priority) {
                maxIndex = i;
            }
        }
        return elements[maxIndex].value;
    }

    /*
        * Modifies the priority of an element in the priority queue
        * @param e element to modify
        * @param p new priority of the element
        * @return none
    */
    void modifyKey(T e, int p) {
        for (int i = 0; i < numElements; i++) {
            if (elements[i].value == e) {
                elements[i].priority = p;
                return;
            }
        }
        throw std::string("Element not found");
    }

    /*
        * Returns the number of elements in the priority queue
        * @param none
        * @return number of elements in the priority queue
    */
    size_t size() {
        return numElements;
    }

private:
    Element<T>* elements;
    size_t numElements;
    size_t capacity;
};
#endif