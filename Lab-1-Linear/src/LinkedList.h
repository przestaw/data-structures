#ifndef AISDI_LINEAR_LINKEDLIST_H
#define AISDI_LINEAR_LINKEDLIST_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>

namespace aisdi
{

template <typename Type>
struct Node
{
  const Type data;
  Node *prev;
  Node *next;

  Node()
  {
    next = nullptr;
    prev = nullptr;
  }

  Node(const Type &A):
    data(A)
  {
    next = nullptr;
    prev = nullptr;
  }
};

template <typename Type>
class LinkedList
{
public:
  using difference_type = std::ptrdiff_t;
  using size_type = std::size_t;
  using value_type = Type;
  using pointer = Type*;
  using reference = Type&;
  using const_pointer = const Type*;
  using const_reference = const Type&;

  class ConstIterator;
  class Iterator;
  using iterator = Iterator;
  using const_iterator = ConstIterator;
private:
  Node<Type> *first;
  Node<Type> *last;
public:
  LinkedList()
  {
    first=new Node<Type>(Type{});
    last=new Node<Type>(Type{});
    last->next= nullptr;
    first->previous= nullptr;
    first->next=last;
    last->previous=first;
  }

  LinkedList(std::initializer_list<Type> l):
    LinkedList()
  {
    auto iter = l.begin();
    while(iter != l.end())
    {
      this->append(*iter);
      iter++;
    }
  }

  LinkedList(const LinkedList& other):
          LinkedList()
  {
    auto iter = l.begin();
    while(iter != l.end())
    {
      this->append(*iter);
      iter++;
    }
  }

  LinkedList(LinkedList&& other):
          LinkedList()
  {
    first->next = other.first->next;
    last->previous = other.last->previous;
    other.first->next->previous = first;
    other.last->previous->next = last;
    other.first->next = other.last;
    other.last->previous = other.first;
  }

  ~LinkedList()
  {
    erase( begin(), end());
    delete last;
    delete first;
  }

  LinkedList& operator=(const LinkedList& other)
  {
    (void)other;
    throw std::runtime_error("TODO");
  }

  LinkedList& operator=(LinkedList&& other)
  {
    (void)other;
    throw std::runtime_error("TODO");
  }

  bool isEmpty() const
  {
    if(first->next==last)
    {
      return true;
    }else
    {
      return false;
    }
  }

  size_type getSize() const
  {
    throw std::runtime_error("TODO");
  }

  void append(const Type& item)
  {
    (void)item;
    throw std::runtime_error("TODO");
  }

  void prepend(const Type& item)
  {
    (void)item;
    throw std::runtime_error("TODO");
  }

  void insert(const const_iterator& insertPosition, const Type& item)
  {
    (void)insertPosition;
    (void)item;
    throw std::runtime_error("TODO");
  }

  Type popFirst()
  {
    if(this->isEmpty())
    {
      throw std::out_of_range("Vector: Can't pop from empty list");
    }
    throw std::runtime_error("TODO");
  }

  Type popLast()
  {
    if(this->isEmpty())
    {
      throw std::out_of_range("Vector: Can't pop from empty list");
    }
    throw std::runtime_error("TODO");
  }

  void erase(const const_iterator& possition)
  {
    if(possition.pter->next==nullptr || possition.pter->previous==nullptr)
    {
      throw std::out_of_range("Vector: Can't delete sentinel");
    }

    possition.pter->next->previous = possition.pter->previous;
    possition.pter->previous->next = possition.pter->next;
    delete possition.pter;
  }

  void erase(const const_iterator& firstIncluded, const const_iterator& lastExcluded)
  {
      auto iter_point;
      for(auto iter = firstIncluded; iter != lastExcluded; )
      {
          iter_point = iter + 1;
          this->erase(iter);
          iter=iter_point;
      }
  }

  iterator begin()
  {
    if(this->isEmpty())
    {
      return Iterator(last);
    }
    else
    {
      return Iterator(first->next);
    }
  }

  iterator end()
  {
    return Iterator(last);
  }

  const_iterator cbegin() const
  {
    if(this->isEmpty())
    {
      return ConstIterator(last);
    }
    else
    {
      return ConstIterator(first->next);
    }
  }

  const_iterator cend() const
  {
    return ConstIterator(last);
  }

  const_iterator begin() const
  {
    return cbegin();
  }

  const_iterator end() const
  {
    return cend();
  }
};

template <typename Type>
class LinkedList<Type>::ConstIterator
{
public:
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = typename LinkedList::value_type;
  using difference_type = typename LinkedList::difference_type;
  using pointer = typename LinkedList::const_pointer;
  using reference = typename LinkedList::const_reference;
private:
  Node<Type>* actual;
public:
  explicit ConstIterator()
  {}

  ConstIterator(Node<Type>* actual)
  {
     this->actual = actual;
  }

  reference operator*() const
  {
      if(actual->next==nullptr || actual->previous==nullptr)
      {
          throw std::out_of_range("List: Can't get data from sentinel");
      }else
      {
          return actual->data;
      }
  }

  ConstIterator& operator++()
  {
     if(actual->next!=nullptr)
     {
       actual=actual->next;
       return *this;
     }else
     {
       throw std::out_of_range("List: Can't go further");
     }
  }

  ConstIterator operator++(int)
  {
      if(actual->next!=nullptr)
      {
          ConstIterator ret = *this;
          actual=actual->next;
          return ret;
      }else
      {
         throw std::out_of_range("List: Can't go further");
      }

  }

  ConstIterator& operator--()
  {
    if(actual->prev!=nullptr)
    {
       actual=actual->prev;
       return *this;
    }else
    {
      throw std::out_of_range("List: Can't go further");
    }
  }

  ConstIterator operator--(int)
  {
     if(actual->prev!=nullptr)
     {
       ConstIterator ret = *this;
       actual=actual->prev;
       return ret;
     }else
     {
       throw std::out_of_range("List: Can't go further");
     }
  }

  ConstIterator operator+(difference_type d) const
  {
      (void)d;
      throw std::runtime_error("TODO");
  }

  ConstIterator operator-(difference_type d) const
  {
    (void)d;
    throw std::runtime_error("TODO");
  }

  bool operator==(const ConstIterator& other) const
  {
      if(this->actual == other)
      {
          return true;
      }else
      {
          return false;
      }
  }

  bool operator!=(const ConstIterator& other) const
  {
      if(this->actual != other)
      {
          return true;
      }else
      {
          return false;
      }
  }
};

template <typename Type>
class LinkedList<Type>::Iterator : public LinkedList<Type>::ConstIterator
{
public:
  using pointer = typename LinkedList::pointer;
  using reference = typename LinkedList::reference;

  explicit Iterator()
  {}

  Iterator(const ConstIterator& other)
    : ConstIterator(other)
  {}

  Iterator& operator++()
  {
    ConstIterator::operator++();
    return *this;
  }

  Iterator operator++(int)
  {
    auto result = *this;
    ConstIterator::operator++();
    return result;
  }

  Iterator& operator--()
  {
    ConstIterator::operator--();
    return *this;
  }

  Iterator operator--(int)
  {
    auto result = *this;
    ConstIterator::operator--();
    return result;
  }

  Iterator operator+(difference_type d) const
  {
    return ConstIterator::operator+(d);
  }

  Iterator operator-(difference_type d) const
  {
    return ConstIterator::operator-(d);
  }

  reference operator*() const
  {
    // ugly cast, yet reduces code duplication.
    return const_cast<reference>(ConstIterator::operator*());
  }
};

}

#endif // AISDI_LINEAR_LINKEDLIST_H
