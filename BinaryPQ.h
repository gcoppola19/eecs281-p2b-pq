// Project identifier: 43DE0E0C4C76BFAA6D8C2F5AEAE0518A9C42CF4E

#ifndef BINARYPQ_H
#define BINARYPQ_H

#include <algorithm>

#include "Eecs281PQ.h"

// A specialized version of the priority queue ADT implemented as a binary heap.
template <typename TYPE, typename COMP_FUNCTOR = std::less<TYPE>>
class BinaryPQ : public Eecs281PQ<TYPE, COMP_FUNCTOR>
{
    // This is a way to refer to the base class object.
    using BaseClass = Eecs281PQ<TYPE, COMP_FUNCTOR>;

public:
    // Description: Construct an empty PQ with an optional comparison functor.
    // Runtime: O(1)
    explicit BinaryPQ(COMP_FUNCTOR comp = COMP_FUNCTOR()) : BaseClass{comp}
    {
        
        data.push_back(temp);
    } // BinaryPQ

    // Description: Construct a PQ out of an iterator range with an optional
    //              comparison functor.
    // Runtime: O(n) where n is number of elements in range.
    template <typename InputIterator>
    BinaryPQ(InputIterator start, InputIterator end, COMP_FUNCTOR comp = COMP_FUNCTOR()) : BaseClass{comp}, data{start, end}
    {
        static TYPE temp;
        data.push_back(temp);
        std::swap(data[0], data[data.size() - 1]);
        updatePriorities();
    } // BinaryPQ

    // Description: Destructor doesn't need any code, the data vector will
    //              be destroyed automatically.
    virtual ~BinaryPQ()
    {
    } // ~BinaryPQ()

    // Description: Assumes that all elements inside the heap are out of order and
    //              'rebuilds' the heap by fixing the heap invariant.
    // Runtime: O(n)
    virtual void updatePriorities()
    {
        for (size_t i = data.size() / 2; i > 0; --i)
        {
            fixDown(i);
        }
    } // updatePriorities()

    // Description: Add a new element to the PQ.
    // Runtime: O(log(n))
    virtual void push(const TYPE &val)
    {
        data.push_back(val);
        fixUp(data.size() - 1);
    } // push()

    // Description: Remove the most extreme (defined by 'compare') element
    //              from the PQ.
    // Note: We will not run tests on your code that would require it to pop
    // an element when the PQ is empty. Though you are welcome to if you are
    // familiar with them, you do not need to use exceptions in this project.
    // Runtime: O(log(n))
    virtual void pop()
    {
        data[1] = data.back();
        data.pop_back();
        fixDown(1);
    } // pop()

    // Description: Return the most extreme (defined by 'compare') element of
    //              the PQ. This should be a reference for speed. It MUST
    //              be const because we cannot allow it to be modified, as
    //              that might make it no longer be the most extreme element.
    // Runtime: O(1)
    // good
    virtual const TYPE &top() const
    {
        return data.at(1);
    } // top()

    // Description: Get the number of elements in the PQ.
    // Runtime: O(1)
    // good
    virtual std::size_t size() const
    {
        return data.size() - 1;
    } // size()

    // Description: Return true if the PQ is empty.
    // Runtime: O(1)
    // good
    virtual bool empty() const
    {
        return data.size() == 1;
    } // empty()

private:
    // Note: This vector *must* be used for your PQ implementation.
    std::vector<TYPE> data;
    // NOTE: You are not allowed to add any member variables. You don't need
    //       a "heapSize", since you can call your own size() member
    //       function, or check data.size().

    void fixUp(size_t index)
    {
        while (index > 1 && this->compare(data[index / 2], data[index]))
        {
            std::swap(data[index], data[index / 2]);
            index /= 2;
        }
    }

    void fixDown(size_t index)
    {
        size_t heapSize = data.size() - 1;
        while (2 * index <= heapSize)
        {
            size_t j = 2 * index;
            if (j < heapSize && this->compare(data[j], data[j + 1]))
            {
                ++j;
            }
            if (this->compare(data[j], data[index]))
            {
                break;
            }
            std::swap(data[index], data[j]);
            index = j;
        }
    }

#endif // BINARYPQ_H
};