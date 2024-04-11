#ifndef HEAP_PRIORITY_QUEUE_HPP
#define HEAP_PRIORITY_QUEUE_HPP

#include "PriorityQueue.hpp"
#include "Element.hpp"
#include <iostream>
#include <fstream>

template<typename T>
class HeapPriorityQueue : public PriorityQueue<T> {
private:
    Element<T>* heapArray;
    size_t capacity;
    size_t heapSize;

    /*
        * Function to ensure that the heap has enough capacity
        * @param none
        * @return none
    */
    void ensureCapacity() {
        if (heapSize == capacity) {
            capacity *= 2;
            Element<T>* newHeap = new Element<T>[capacity];
            for (size_t i = 0; i < heapSize; ++i) {
                newHeap[i] = heapArray[i];
            }
            delete[] heapArray;
            heapArray = newHeap;
        }
    }

    /*
        * Function to heapify the heap upwards
        * @param index index of the element to heapify
        * @return none
    */
    void heapifyUp(size_t index) {
        while (index != 0 && heapArray[parent(index)].priority < heapArray[index].priority) {
            swap(heapArray[index], heapArray[parent(index)]);
            index = parent(index);
        }
    }

    /*
        * Function to heapify the heap downwards
        * @param index index of the element to heapify
        * @return none
    */
    void heapifyDown(size_t index) {
        size_t leftChild = left(index);
        size_t rightChild = right(index);
        size_t largest = index;

        if (leftChild < heapSize && heapArray[leftChild].priority > heapArray[index].priority) {
            largest = leftChild;
        }

        if (rightChild < heapSize && heapArray[rightChild].priority > heapArray[largest].priority) {
            largest = rightChild;
        }

        if (largest != index) {
            swap(heapArray[index], heapArray[largest]);
            heapifyDown(largest);
        }
    }

    /*
        * Function to return the index of the parent of a given index
        * @param index index of the element
        * @return index of the parent
    */
    size_t parent(size_t index) { return (index - 1) / 2; }

    /*
        * Function to return the index of the left child of a given index
        * @param index index of the element
        * @return index of the left child
    */
    size_t left(size_t index) { return 2 * index + 1; }

    /*
        * Function to return the index of the right child of a given index
        * @param index index of the element
        * @return index of the right child
    */
    size_t right(size_t index) { return 2 * index + 2; }


    /*
        * Function to swap two elements
        * @param x first element
        * @param y second element
        * @return none
    */
    void swap(Element<T>& x, Element<T>& y) {
        Element<T> temp = x;
        x = y;
        y = temp;
    }

public:

    /*
        * Constructor
        * Initializes the heap with a capacity of 1000
        * @param none
        * @return none
    */
    HeapPriorityQueue() : capacity(1000), heapSize(0) {
        heapArray = new Element<T>[capacity];
    }

    /*
        * Constructor
        * Initializes the heap with a given capacity
        * @param capacity capacity of the heap
        * @return none
    */
    HeapPriorityQueue(size_t capacity) : capacity(capacity), heapSize(0) {
        heapArray = new Element<T>[capacity];
    }

    /*
        * Constructor
        * Initializes the heap that reads elements from a file
        * @param filename name of the file to read from
        * @param capacity capacity of the heap
        * @return none
    */
    HeapPriorityQueue(const char* filename) : capacity(1000), heapSize(0) {
        heapArray = new Element<T>[capacity];
        std::fstream file(filename);
        if (!file) {
            throw std::string("File not found");
        }
        T value;
        int priority;
        while (file >> value >> priority) {
            insert(value, priority);
        }
        file.close();
    }

    /*
        * Constructor
        * Initializes the heap with a given capacity and reads elements from a file
        * @param filename name of the file to read from
        * @param capacity capacity of the heap
        * @return none
    */
    HeapPriorityQueue(const char* filename, size_t capacity) : capacity(capacity), heapSize(0) {
        heapArray = new Element<T>[capacity];
        std::fstream file(filename);
        if (!file) {
            throw std::string("File not found");
        }
        T value;
        int priority;
        while (file >> value >> priority) {
            insert(value, priority);
        }
        file.close();
    }
    
    /*
        * Destructor
        * Deletes the heapArray
        * @param none
        * @return none
    */
    ~HeapPriorityQueue() {
        delete[] heapArray;
    }

    /*
        * Function to insert an element into the heap
        * @param value value of the element
        * @param priority priority of the element
        * @return none
    */
    void insert(T value, int priority) override {
        ensureCapacity();
        heapSize++;
        size_t index = heapSize - 1;
        heapArray[index] = Element<T>(value, priority);

        heapifyUp(index);
    }


    /*
        * Function to extract the maximum element from the heap and remove it from the heap
        * @param none
        * @return value of the maximum element
    */
    T extractMax() override {
        if (heapSize == 0) {
            std::cerr << "Error: attempting to extract from an empty heap." << std::endl;
            return T(); // Zwraca wartość domyślną dla typu T
        }
        T maxValue = heapArray[0].value;
        heapArray[0] = heapArray[heapSize - 1];
        heapSize--;
        heapifyDown(0);
        return maxValue;
    }

    /*
        * Function to return the maximum element from the heap
        * @param none
        * @return value of the maximum element, nullptr if the heap is empty
    */
    T findMax() const override {
        if (heapSize == 0) {
            std::cerr << "Error: heap is empty." << std::endl;
            return nullptr;
        }
        return heapArray[0].value;
    }

    /*
        * Function to modify the priority of an element
        * @param value value of the element
        * @param newPriority new priority of the element
        * @return none
    */
    void modifyKey(T value, int newPriority) override {
        for (size_t i = 0; i < heapSize; i++) {
            if (heapArray[i].value == value) {
                int oldPriority = heapArray[i].priority;
                heapArray[i].priority = newPriority;
                if (newPriority > oldPriority) {
                    heapifyUp(i);
                } else {
                    heapifyDown(i);
                }
                return;
            }
        }
        std::cerr << "Error: element not found." << std::endl;
    }

    /*
        * Function to return the number of elements in the heap
        * @param none
        * @return number of elements in the heap
    */
    size_t size() const override {
        return heapSize;
    }
};

#endif // HEAP_PRIORITY_QUEUE_HPP