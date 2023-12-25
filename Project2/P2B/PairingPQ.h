// Project identifier: 9504853406CBAC39EE89AA3AD238AA12CA198043
//0E04A31E0D60C01986ACB20081C9D8722A1899B6

#ifndef PAIRINGPQ_H
#define PAIRINGPQ_H

#include "Eecs281PQ.h"
#include <deque>
#include <utility>

// A specialized version of the priority queue ADT implemented as a pairing
// heap.
template<typename TYPE, typename COMP_FUNCTOR = std::less<TYPE>>
class PairingPQ : public Eecs281PQ<TYPE, COMP_FUNCTOR> {
    // This is a way to refer to the base class object.
    using BaseClass = Eecs281PQ<TYPE, COMP_FUNCTOR>;

public:
    // Each node within the pairing heap
    class Node {
        public:
            // TODO: After you add add one extra pointer (see below), be sure
            //       to initialize it here.
            explicit Node(const TYPE &val)
                : elt{ val }, child{ nullptr }, sibling{ nullptr }, prev{ nullptr }
            {}

            // Description: Allows access to the element at that Node's
            //              position. There are two versions, getElt() and a
            //              dereference operator, use whichever one seems
            //              more natural to you.
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
            // TODO: Add one extra pointer (parent or previous) as desired.
            Node* prev;
    }; // Node


    // Description: Construct an empty pairing heap with an optional
    //              comparison functor.
    // Runtime: O(1)
    explicit PairingPQ(COMP_FUNCTOR comp = COMP_FUNCTOR()) :
        BaseClass{ comp }, root(nullptr), count(0) {
        // TODO: Implement this function.
    } // PairingPQ()


    // Description: Construct a pairing heap out of an iterator range with an
    //              optional comparison functor.
    // Runtime: O(n) where n is number of elements in range.
    template<typename InputIterator>
    PairingPQ(InputIterator start, InputIterator end, COMP_FUNCTOR comp = COMP_FUNCTOR()) :
        BaseClass{ comp }, root(nullptr), count(0) {
        // TODO: Implement this function.
        while (start != end)
            push(*start++);
    } // PairingPQ()


    // Description: Copy constructor.
    // Runtime: O(n)
    PairingPQ(const PairingPQ &other) :
        BaseClass{ other.compare }, root(nullptr), count(0) {
        // TODO: Implement this function.
        // NOTE: The structure does not have to be identical to the original,
        //       but it must still be a valid pairing heap.
        if (!other.root)
            return;

        std::deque<Node*> nodes;
        nodes.emplace_back(other.root);
        while (nodes.size())
        {
            Node* cur_node = nodes.front();
            nodes.pop_front();
            if (cur_node->sibling)
                nodes.emplace_front(cur_node->sibling);
            if (cur_node->child)
                nodes.emplace_back(cur_node->child);
            push(cur_node->getElt());
        }
    } // PairingPQ()


    // Description: Copy assignment operator.
    // Runtime: O(n)
    PairingPQ &operator=(const PairingPQ &rhs) {
        // TODO: Implement this function.
        // HINT: Use the copy-swap method from the "Arrays and Containers"
        //       lecture.
        PairingPQ cp(rhs);
        Node* tmp = cp.root;
        cp.root = root;
        root = tmp;
        std::swap(count, cp.count);

        return *this;
    } // operator=()


    // Description: Destructor
    // Runtime: O(n)
    ~PairingPQ() {
        // TODO: Implement this function.
        clean();
    } // ~PairingPQ()


    // Description: Assumes that all elements inside the pairing heap are out
    //              of order and 'rebuilds' the pairing heap by fixing the
    //              pairing heap invariant. You CANNOT delete 'old' nodes
    //              and create new ones!
    // Runtime: O(n)
    virtual void updatePriorities() {
        // TODO: Implement this function.
        if (!root)
            return;

        std::deque<Node*> nodes;
        nodes.emplace_back(root);
        Node* tmp = nullptr;
        while (nodes.size())
        {
            Node* cur_node = nodes.front();
            nodes.pop_front();
            if (cur_node->sibling)
                nodes.emplace_front(cur_node->sibling);
            if (cur_node->child)
                nodes.emplace_back(cur_node->child);
                
            cur_node->child = nullptr;
            cur_node->sibling = nullptr;
            cur_node->prev = nullptr;
            tmp = meld(tmp, cur_node);
        }
        root = tmp;
    } // updatePriorities()


    // Description: Add a new element to the pairing heap. This is already
    //              done. You should implement push functionality entirely in
    //              the addNode() function, and this function calls
    //              addNode().
    // Runtime: O(1)
    virtual void push(const TYPE &val) {
        addNode(val);
    } // push()


    // Description: Remove the most extreme (defined by 'compare') element
    //              from the pairing heap.
    // Note: We will not run tests on your code that would require it to pop
    //       an element when the pairing heap is empty. Though you are
    //       welcome to if you are familiar with them, you do not need to use
    //       exceptions in this project.
    // Runtime: Amortized O(log(n))
    virtual void pop() {
        // TODO: Implement this function.
        count--;

        if (!(root->child))
        {
            delete root;
            root = nullptr;
            return;
        }

        std::deque<Node*> trees;
        Node* node = root->child;
        while (node)
        {
            if (node->sibling)
            {
                Node* tmp1 = node->sibling;
                node->prev = nullptr;
                node->sibling = nullptr;

                Node* tmp2 = tmp1->sibling;
                tmp1->prev = nullptr;
                tmp1->sibling = nullptr;
                trees.emplace_back(meld(node, tmp1));
                node = tmp2;
            }
            else
            {
                node->prev = nullptr;
                trees.emplace_back(node);
                break;
            }
        }

        while (trees.size() > 1)
        {
            Node* tmp1 = trees.back();
            trees.pop_back();
            Node* tmp2 = trees.back();
            trees.pop_back();
            trees.emplace_back(meld(tmp1, tmp2));
        }
        delete root;
        root = trees[0];
    } // pop()


    // Description: Return the most extreme (defined by 'compare') element of
    //              the pairing heap. This should be a reference for speed.
    //              It MUST be const because we cannot allow it to be
    //              modified, as that might make it no longer be the most
    //              extreme element.
    // Runtime: O(1)
    virtual const TYPE &top() const {
        // TODO: Implement this function
        return root->getElt();
    } // top()


    // Description: Get the number of elements in the pairing heap.
    // Runtime: O(1)
    virtual std::size_t size() const {
        // TODO: Implement this function
        return count; // TODO: Delete or change this line
    } // size()

    // Description: Return true if the pairing heap is empty.
    // Runtime: O(1)
    virtual bool empty() const {
        // TODO: Implement this function
        return !root; // TODO: Delete or change this line
    } // empty()


    // Description: Updates the priority of an element already in the pairing
    //              heap by replacing the element refered to by the Node with
    //              new_value. Must maintain pairing heap invariants.
    //
    // PRECONDITION: The new priority, given by 'new_value' must be more
    //               extreme (as defined by comp) than the old priority.
    //
    // Runtime: As discussed in reading material.
    void updateElt(Node* node, const TYPE &new_value) {
        // TODO: Implement this function
        if (node == root)
        {
            if (!this->compare(new_value, node->getElt()))
            {
                node->elt = new_value;
                return;
            }
        }

        node->elt = new_value;
        Node* tmp = node->prev;
        if (tmp->child == node)
            tmp->child = node->sibling;
        else
            tmp->sibling = node->sibling;

        node->prev = node->sibling = nullptr;
        root = meld(node, root);
    } // updateElt()


    // Description: Add a new element to the pairing heap. Returns a Node*
    //              corresponding to the newly added element.
    // NOTE: Whenever you create a node, and thus return a Node *, you must
    //       be sure to never move or copy/delete that node in the future,
    //       until it is eliminated by the user calling pop(). Remember this
    //       when you implement updateElt() and updatePriorities().
    // Runtime: O(1)
    Node* addNode(const TYPE &val) {
        // TODO: Implement this function
        count++;
        Node* new_node = new Node(val);
        root = meld(root, new_node);

        return new_node; // TODO: Delete or change this line
    } // addNode()


private:
    // TODO: Add any additional member variables or member functions you
    //       require here.
    Node* root;
    size_t count;
    // TODO: We recommend creating a 'meld' function (see the Pairing Heap
    //       papers).
    Node* meld(Node* node1, Node* node2)
    {
        if (node1 == nullptr)
            return node2;
        if (node2 == nullptr)
            return node1;
        if (this->compare(node1->getElt(), node2->getElt()))
        {
            node1->sibling = node2->child;
            if (node2->child)
                node2->child->prev = node1;
            node2->child = node1;
            node1->prev = node2;

            return node2;
        }
        else
        {
            node2->sibling = node1->child;
            if (node1->child)
                node1->child->prev = node2;
            node1->child = node2;
            node2->prev = node1;

            return node1;
        }
    }

    void clean()
    {
        if (!root)
            return;

        std::deque<Node*> nodes;
        nodes.emplace_back(root);
        while (nodes.size())
        {
            Node* cur_node = nodes.front();
            nodes.pop_front();
            if (cur_node->sibling)
                nodes.emplace_front(cur_node->sibling);
            if (cur_node->child)
                nodes.emplace_back(cur_node->child);
            delete cur_node;
        }
    }
    // NOTE: For member variables, you are only allowed to add a "root
    //       pointer" and a "count" of the number of nodes. Anything else
    //       (such as a deque) should be declared inside of member functions
    //       as needed.
};


#endif // PAIRINGPQ_H
