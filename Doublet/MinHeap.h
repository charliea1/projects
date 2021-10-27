#include <exception>
#include <iostream>
#include <map>
#include <stdexcept>
#include <vector>
using namespace std;

// struct to hold a T and a priority
// qitems will be stored in heap
template <class T>
struct qitem
{
    T m_data;
    int m_priority;
    // id and index for keeping track of positions
    int m_id;
    int m_index;
};

template <class T>
class MinHeap
{
public:
    MinHeap(int d);

    ~MinHeap();

    void update(int loc, int priority);

    void add(T item, int priority);
    // adds the item to the heap, with the given priority.

    const T &peek() const;
    //returns the element with smallest priority

    void remove();
    // removes the element with smallest priority.

    bool isEmpty();
    // returns true iff there are no elements on the heap

private:
    // max number of child nodes per node
    int m_dim;

    int idNo = 0;

    // vector to store qitems holding T's
    vector<qitem<T> *> heap;

    // index for update function
    vector<qitem<T> *> locations;

    // returns parent of node location pos
    int parent(int pos);

    // returns child of node pos, leaf specifies which child
    int child(int pos, int leaf);

    // bubbleUp/trickleDown as needed in heap
    void bubbleUp(int pos);
    void trickleDown(int pos);

    // determine if a node is a leaf
    bool isLeaf(int pos) const;

    // swap two elements in vector
    void swap(vector<qitem<T> *> in, int a, int b);
};

template <class T>
MinHeap<T>::MinHeap(int d) : m_dim(d) {}

template <class T>
void MinHeap<T>::swap(vector<qitem<T> *> in, int a, int b)
{
    // return if same index
    if (a == b)
    {
        return;
    }
    // deep copy/swap of qitem data and priorities
    qitem<T> *first = in.at(a);
    qitem<T> *second = in.at(b);
    int temp_pri = first->m_priority;
    T temp_dat = first->m_data;
    first->m_priority = second->m_priority;
    first->m_data = second->m_data;
    second->m_priority = temp_pri;
    second->m_data = temp_dat;

    // swap locations stored in index vector
    qitem<T> *temp = locations.at(first->m_id);
    locations.at(first->m_id)->m_index = locations.at(second->m_id)->m_index;
    locations.at(second->m_id)->m_index = temp->m_index;
}

template <class T>
void MinHeap<T>::add(T item, int priority)
{
    // create new qitem
    qitem<T> *inserted = new qitem<T>;
    inserted->m_data = item;
    inserted->m_priority = priority;
    inserted->m_index = heap.size();
    inserted->m_id = locations.size();

    // add item as new leaf and bubbleUp to maintain priority
    heap.push_back(inserted);
    locations.push_back(inserted);
    bubbleUp(heap.size() - 1);
}

template <class T>
void MinHeap<T>::remove()
{
    // only remove if heap not empty
    if (!isEmpty())
    {
        // swap top (minimum) element with last leaf (heap.size())
        swap(heap, 0, heap.size() - 1);

        // remove minimum element (now last)
        delete heap.at(heap.size() - 1);
        heap.at(heap.size() - 1) = nullptr;
        heap.pop_back();

        // trickleDown at top (0) to maintain priority
        trickleDown(0);
        return;
    }
}

template <class T>
int MinHeap<T>::parent(int pos)
{
    // parent of pos for d-ary heap is (pos - 1)/d
    pos = (pos - 1) / m_dim;
    return pos;
}

template <class T>
int MinHeap<T>::child(int pos, int leaf)
{
    // child of pos at jth leaf in d-ary heap is (d * pos) + j
    pos = (m_dim * pos) + leaf;
    return pos;
}

// first node n without child in d-ary heap is n = (size + d - 1) / d
// if pos >= n then pos is a leaf node
template <class T>
bool MinHeap<T>::isLeaf(int pos) const
{
    int firstLeaf = (heap.size() + m_dim - 1) / m_dim;
    if (pos >= firstLeaf)
    {
        return true;
    }
    else
    {
        return false;
    }
}

template <class T>
void MinHeap<T>::bubbleUp(int pos)
{
    // return if empty or size 1
    if (heap.size() < 2)
    {
        return;
    }
    // child is at pos, find parent node
    qitem<T> *l_child = heap.at(pos);
    qitem<T> *l_parent = heap.at(parent(pos));

    // if pos not at top of heap and its priority is less than its parent,
    // swap pos with its parent and recurse
    if (pos > 0 && (l_child->m_priority < l_parent->m_priority))
    {
        swap(heap, pos, parent(pos));

        bubbleUp(parent(pos));
    }
}

template <class T>
const T &MinHeap<T>::peek() const
{
    // if empty heap, throw exception
    // otherwies return top element (first in vector)
    if (heap.size() == 0)
    {
        throw logic_error("Tried to peek empty heap!");
    }
    else
    {
        return heap.at(0)->m_data;
    }
}

template <class T>
bool MinHeap<T>::isEmpty()
{
    if (heap.size() == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

template <class T>
void MinHeap<T>::trickleDown(int pos)
{
    int start = child(pos, 1);
    int l_child = start;
    // compare each child (m_dim number of children) to determine which child should be swapped
    if (start < heap.size())
    {
        for (int i = 1; i < m_dim; i++)
        {
            // if the next child node is smaller than the current, it is now "l_child"
            if (((start + i) < heap.size()) && (heap.at(start + i)->m_priority < heap.at(l_child)->m_priority))
            {
                l_child = start + i;
            }
        }
        // having found smallest child, compare it with pos
        // if it is smaller than pos, swap them and recurse
        if (heap.at(l_child)->m_priority < heap.at(pos)->m_priority)
        {
            swap(heap, l_child, pos);
            trickleDown(l_child);
        }
    }
}

template <class T>
MinHeap<T>::~MinHeap()
{
    // remove top item until empty
    while (!isEmpty())
    {
        remove();
    }
}

// access index vector, check pos, update priority and bubble/trickle as needed
template <class T>
void MinHeap<T>::update(int loc, int priority)
{
    // if loc out of locations range, return
    if (loc > (locations.size() - 1))
    {
        return;
    }
    int pos = locations.at(loc)->m_index;
    // if pos out of heap range, return
    if (pos > (heap.size() - 1))
    {
        return;
    }
    if (heap.at(pos)->m_priority < priority)
    {
        heap.at(pos)->m_priority = priority;
        trickleDown(pos);
    }
    else
    {
        heap.at(pos)->m_priority = priority;
        bubbleUp(pos);
    }
}