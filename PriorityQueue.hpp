#ifndef PRIORITY_QUEUE_HPP
#define PRIORITY_QUEUE_HPP

#include <vector>
#include <string>

template<typename T>
class PriorityQueue {
public:
    virtual ~PriorityQueue() = default;

    virtual void insert(T e, int p) = 0;
    virtual T extractMax() = 0;
    virtual T findMax() const = 0;
    virtual void modifyKey(T e, int p) = 0;
    virtual size_t size() = 0;
};

#endif // PRIORITY_QUEUE_HPP
