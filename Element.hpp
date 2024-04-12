#ifndef ELEMENT_HPP
#define ELEMENT_HPP

template<typename T>
struct Element {
    T value;
    int priority;
    //Constructor
    Element(T value, int priority) : value(value), priority(priority) {};
    Element() : value(T()), priority(0) {};
};

#endif