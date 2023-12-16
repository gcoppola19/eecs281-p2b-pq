// Project identifier: 43DE0E0C4C76BFAA6D8C2F5AEAE0518A9C42CF4E

#ifndef PAIRINGPQ_H
#define PAIRINGPQ_H

#include "Eecs281PQ.h"
#include <deque>
#include <utility>

// A specialized version of the priority queue ADT implemented as a pairing
// heap.
template <typename TYPE, typename COMP_FUNCTOR = std::less<TYPE>>
class PairingPQ : public Eecs281PQ<TYPE, COMP_FUNCTOR>
{
    // This is a way to refer to the base class object.
    using BaseClass = Eecs281PQ<TYPE, COMP_FUNCTOR>;

public:
    // Each node within the pairing heap
    class Node
    {
    public:
        explicit Node(const TYPE &val)
            : elt{val}, child{nullptr}, sibling{nullptr}, prev{nullptr}
        {
        }

        // Description: Allows access to the element at that Node's
        // position.  There are two versions, getElt() and a dereference
        // operator, use whichever one seems more natural to you.
        // Runtime: O(1) - this has been provided for you.
        const TYPE &getElt() const { return elt; }
        const TYPE &operator*() const { return elt; }

        // The following line allows you to access any private data
        // members of this Node class from within the PairingPQ class.
        // (ie: myNode.elt is a legal statement in PairingPQ's add_node()
        // function).
        friend PairingPQ;

    private:
        TYPE elt;
        Node *child;
        Node *sibling;
        Node *prev;
    }; // Node

    // Description: Construct an empty pairing heap with an optional
    //              comparison functor.
    // Runtime: O(1)
    explicit PairingPQ(COMP_FUNCTOR comp = COMP_FUNCTOR()) : BaseClass{comp}
    {
        root = nullptr;
        count = 0;
    } // PairingPQ()

    // Description: Construct a pairing heap out of an iterator range with an
    //              optional comparison functor.
    // Runtime: O(n) where n is number of elements in range.
    template <typename InputIterator>
    PairingPQ(InputIterator start, InputIterator end, COMP_FUNCTOR comp = COMP_FUNCTOR()) : BaseClass{comp}
    {
        // assuming this doesnt work bc push doesnt
        root = nullptr;
        count = 0;
        while (start != end)
        {
            push(*start);
            ++start;
        }
    } // PairingPQ()

    // Description: Copy constructor.
    // Runtime: O(n)
    PairingPQ(const PairingPQ &other) : BaseClass{other.compare}
    {
        // double check but thinking the same philosphoy
        count = 0;
        root = nullptr;
        if (other.root == nullptr)
        {
            root = nullptr;
        }
        else
        {
            std::deque<Node *> list;
            list.push_back(other.root);
            while (!list.empty())
            {
                Node *curr = list.front();
                list.pop_front();

                if (curr->child != nullptr)
                {
                    list.push_back(curr->child);
                }
                if (curr->sibling != nullptr)
                {
                    list.push_back(curr->sibling);
                }
                push(curr->elt);
            }
        }

    } // PairingPQ()

    // Description: Copy assignment operator.
    // Runtime: O(n)
    PairingPQ &operator=(const PairingPQ &rhs)
    {
        // is there more to this? or is it bc copy const
        PairingPQ temp{rhs};
        std::swap(count, temp.count);
        std::swap(root, temp.root);

        return *this;

    } // operator=()

    // Description: Destructor
    // Runtime: O(n)
    ~PairingPQ()
    {
        std::deque<Node *> list;
        if (root)
        {
            list.push_back(root);
        }
        while (!list.empty())
        {
            Node *curr = list.front();

            if (curr->child)
            {
                list.push_back(curr->child);
            }
            if (curr->sibling)
            {
                list.push_back(curr->sibling);
            }
            list.pop_front();
            delete curr;
        }
        count = 0;
    } // ~PairingPQ()

    // Description: Assumes that all elements inside the pairing heap are out
    //              of order and 'rebuilds' the pairing heap by fixing the
    //              pairing heap invariant.  You CANNOT delete 'old' nodes
    //              and create new ones!
    // Runtime: O(n)
    virtual void updatePriorities()
    {
        std::deque<Node *> list;
        if (root)
        {
            list.push_back(root);
        }
        root = nullptr;
        while (!list.empty())
        {
            Node *temp = list.front();
            list.pop_front();
            if (temp->child)
            {
                list.push_back(temp->child);
                temp->child = nullptr;
            }
            if (temp->sibling)
            {
                list.push_back(temp->sibling);
                temp->sibling = nullptr;
            }
            // do i do this somewhere else??
            temp->prev = nullptr;
            if (root == nullptr)
            {
                root = temp;
            }
            else
            {
                root = meld(root, temp);
            }
        }
    } // updatePriorities()

    // Description: Add a new element to the pairing heap. This is already
    //              done. You should implement push functionality entirely
    //              in the addNode() function, and this function calls
    //              addNode().
    // Runtime: O(1)
    virtual void push(const TYPE &val)
    {
        addNode(val);
    } // push()

    /*virtual void pop()
      {
          if (!root)
          {
              return;
          }
          Node *oldRoot = root;
          std::deque<Node *> sibs;
          Node *curr = root->child;
          while (curr)
          {
              sibs.push_front(curr);
              curr = curr->sibling;
          }
          delete oldRoot;
          //oldRoot = nullptr;
          count--;
          while (sibs.size() > 1)
          {
              Node *melder1 = sibs.front();
              sibs.pop_front();
              Node *melder2 = sibs.front();
              sibs.pop_front();
              sibs.push_back(meld(melder1, melder2));
          }
          if (sibs.empty())
          {
              root = nullptr;
          }
          else
          {
              root = sibs.front();
          }
      }*/

    // Description: Remove the most extreme (defined by 'compare') element
    //              from the pairing heap.
    // Note: We will not run tests on your code that would require it to pop
    // an element when the pairing heap is empty. Though you are welcome to
    // if you are familiar with them, you do not need to use exceptions in
    // this project.
    // Runtime: Amortized O(log(n))
    virtual void pop()
    {
        if (root->child == nullptr)
        {
            delete root;
            root = nullptr;
            count--;
            return;
        }
        Node *oldRoot = root;
        std::deque<Node *> sibs;
        Node *curr = root->child;
        while (curr)
        {
            sibs.push_front(curr);
            curr->prev = nullptr;
            Node *temp = curr->sibling;
            if (curr->sibling)
            {
                curr->sibling = nullptr;
            }
            curr = temp;
        }
        delete oldRoot;
        // oldRoot = nullptr;
        count--;
        while (sibs.size() > 1)
        {
            Node *melder1 = sibs.front();
            sibs.pop_front();
            Node *melder2 = sibs.front();
            sibs.pop_front();
            sibs.push_back(meld(melder1, melder2));
        }
        root = sibs.front();
    }

    // Description: Return the most extreme (defined by 'compare') element of
    //              the pairing heap. This should be a reference for speed.
    //              It MUST be const because we cannot allow it to be
    //              modified, as that might make it no longer be the most
    //              extreme element.
    // Runtime: O(1)
    virtual const TYPE &top() const
    {
        return root->elt;
    } // top()

    // Description: Get the number of elements in the pairing heap.
    // Runtime: O(1)
    virtual std::size_t size() const
    {
        return count;
    } // size()

    // Description: Return true if the pairing heap is empty.
    // Runtime: O(1)
    // good
    virtual bool empty() const
    {
        return root == nullptr;
    } // empty()

    // Description: Updates the priority of an element already in the pairing
    //              heap by replacing the element refered to by the Node with
    //              new_value.  Must maintain pairing heap invariants.
    //
    // PRECONDITION: The new priority, given by 'new_value' must be more
    //              extreme (as defined by comp) than the old priority.
    //
    // Runtime: As discussed in reading material.
    void updateElt(Node *node, const TYPE &new_value)
    {

        node->elt = new_value;
        // Remove the node from its current location in the heap
        if (node->prev->child == node)
        {
            node->prev->child = node->sibling;
        }
        else
        {
            node->prev->sibling = node->sibling;
        }
        if (node->sibling)
        {
            node->sibling->prev = node->prev;
        }

        // Reset node's pointers to make it a root
        node->prev = nullptr;
        node->sibling = nullptr;

        // Meld the updated node with the root
        root = meld(root, node);
    } // updateElt()

    // Description: Add a new element to the pairing heap. Returns a Node*
    //              corresponding to the newly added element.
    // Runtime: O(1)
    // NOTE: Whenever you create a node, and thus return a Node *, you must
    //       be sure to never move or copy/delete that node in the future,
    //       until it is eliminated by the user calling pop(). Remember this
    //       when you implement updateElt() and updatePriorities().
    Node *addNode(const TYPE &val)
    {
        Node *newNode = new Node(val);
        count++;
        if (root == nullptr)
        {
            root = newNode;
            return root;
        }

        root = meld(root, newNode);
        return newNode;

    } // addNode()

private:
    Node *root;
    size_t count;

    Node *meld(Node *a, Node *b)
    {
        // A IS LESS THAN B

        if (this->compare(a->elt, b->elt)) // a is less than b  and no kids
        {
            a->sibling = b->child;
            if (b->child)
            {
                (b->child)->prev = a;
            }
            b->child = a;
            a->prev = b;
            return b;
        }
        // B IS LESS THAN A
        else /*if(this->compare(b->elt, a->elt))*/
        // b is less than a and no kids
        {
            b->sibling = a->child;
            if (a->child)
            {
                (a->child)->prev = b;
            }
            a->child = b;
            b->prev = a;
            return a;
        }
    }
};

#endif // PAIRINGPQ_H
