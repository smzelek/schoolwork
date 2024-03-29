/*
  Filename   : Array.hpp
  Author     : Steven Zelek
  Course     : CSCI 362-01
  Assignment : Lab 5
  Description: Array class, our implementation of a list ADT 
               with random access and dynamic resizing.
*/   

/************************************************************/
// Macro guard to prevent multiple inclusions

#ifndef ARRAY_H
#define ARRAY_H

/************************************************************/
// System includes

#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <iterator>

/************************************************************/
// Local includes

/************************************************************/
// Using declarations

using std::fill;
using std::copy;
using std::copy_backward;
using std::ostream;
using std::ptrdiff_t;
using std::distance;

/************************************************************/

template <typename T>
class Array 
{
public:

  // Some necessary typedef's
  typedef T          value_type;
  // Iterators are just pointers to objects of type T.
  typedef T*         iterator;
  typedef const T*   const_iterator;

  typedef T&         reference;
  typedef const T&   const_reference;
  typedef size_t     size_type;
  typedef ptrdiff_t  difference_type;
  
  // Initialize an empty Array.
  Array ()
    : m_size (0),
      m_capacity (0),
      m_array (nullptr)
  { }
  
  // Initialize an Array of size "pSize", with each element
  //   set to "value".
  explicit Array (size_t pSize, const T& value = T ())
    : m_size (pSize),
      m_capacity (m_size),
      m_array (new T[m_capacity])
  {
    fill (m_array, m_array + m_size, value);
  }
  
  // Initialize an Array from the range [first, last).
  Array (const_iterator first, const_iterator last)
    : m_size (distance (first, last)),
      m_capacity (m_size),
      m_array (new T[m_capacity])
  {
    copy (first, last, m_array);
  }
  
  // Initialize this object from "a".
  Array (const Array &a)
    : m_size (a.m_size),
      m_capacity (a.m_capacity),
      m_array (new T[m_capacity])
  {
    copy (a.m_array, a.m_array + m_size, m_array);
  }
  
  // Release allocated memory.
  ~Array ()
  {
    delete[] m_array;
  }
  
  // Assign "a" to this object.
  Array&
  operator= (const Array& a)
  {
    if (this != &a)
    {
      m_size = a.m_size;
      m_capacity = a.m_capacity;
      m_array = new T[m_capacity];
      copy (a.begin (), a.end (), m_array);
    }
    return *this;
  }

  size_t
  size () const
  {
    return m_size;
  }

  size_t
  capacity () const
  {
    return m_capacity;
  }

  // Allows subscripting of array object.
  T&
  operator[] (size_t index)
  {
    return m_array[index];
  }
  const T&
  operator[] (size_t index) const
  {
    return m_array[index];
  }

  // Adds a given item to the end of the array.
  void
  push_back (const T& item)
  {
    insert (end (), item);
  }

  // Removes the last valid item at the end of the array.
  void
  pop_back ()
  {
    erase (end () - 1);
  }

  // Reserve capacity for "space" elements
  //   "space" must be  > capacity
  //   "size" is unchanged
  // Calling methods ask for double the capacity when extra space is needed
  void
  reserve (size_t space)
  {
    if (space > capacity ())
    {
      T* temp = new T[space];
      copy (begin (), end (), temp);
      delete[] m_array;
      m_capacity = space;
      m_array = temp;
    }
  }


  // Reuses reserve when the newSize is greater than current.
  // Fills unused space with the new default value.

  // Otherwise, truncates the array down to newSize,
  // copying as few elements as needed.
  void
  resize (size_t newSize, const T& defValue = T ())
  {
    if (newSize > capacity ())
    {
      reserve (newSize);
      fill (end (), begin () + capacity (), defValue);
      m_size = newSize;
    }
	
    else if (newSize < capacity ())
      {
	T* temp = new T[newSize];

	size_t copyAmt = newSize;
	if (size () < copyAmt)
	  copyAmt = size ();

	copy (begin (), begin () + copyAmt, temp);
	delete[] m_array;
	m_size = copyAmt;
	m_capacity = newSize;
	m_array = temp;
      }
  }

  // Insert "item" before "pos", and return iterator referencing "item".
  iterator insert (iterator pos, const T& item)
  {
    // If size = capacity, call a reserve and double capacity.
    // The + 1 allows for the increase in size of an empty array.
    if (size () == capacity ())
    {
      // Updates pos to point to the correct index of the new memory.
      int newPos = pos - begin ();
      reserve (2 * capacity () + 1);
      pos = m_array + newPos;
    }

    // Shifts all elements after pos one to the right.
    // Updates element at pos, updates size.
    copy_backward (pos, end (), end () + 1);
    *pos = item;
    ++m_size;
    
    return pos;
  }
  
  // Remove element at "pos", and return iterator referencing next element.
  iterator erase  (iterator pos)
  {
    // Shifts all elements after pos one to the left, overwrites
    // the element at pos. Updates size.
    copy (pos + 1, end (), pos);
    --m_size;
    
    return pos;
  }

  // Return pointer to the first element of the array.
  iterator begin ()
  {
    return m_array;
  }
  const_iterator begin () const
  {
    return m_array;
  }

  // Return pointer to one past the last element.
  iterator end ()
  {
    return m_array + size ();
  }
  const_iterator end () const
  {
    return m_array + size ();
  }

private:

  size_t m_size;
  size_t m_capacity;
  T*     m_array;

};

/************************************************************************/
// Free functions associated with class

template <typename T>
ostream&
operator<< (ostream& output, const Array<T>& a)
{
  output << "[ ";
  for (const auto& elem : a)
  {
    output << elem << " ";
  }
  output << "]";

  return output;
}

#endif

/************************************************************************/
