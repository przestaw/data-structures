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
    first->prev= nullptr;
    first->next=last;
    last->prev=first;
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
    auto iter = other.begin();
    while(iter != other.end())
    {
      this->append(*iter);
      iter++;
    }
  }

  LinkedList(LinkedList&& other):
          LinkedList()
  {
    first->next = other.first->next;
    last->prev = other.last->prev;
    other.first->next->prev = first;
    other.last->prev->next = last;
    other.first->next = other.last;
    other.last->prev = other.first;
  }

  ~LinkedList()
  {
    erase( begin(), end());
    delete last;
    delete first;
  }

  LinkedList& operator=(const LinkedList& other)
  {
      if(this == &other)
      {
          return *this;
      }

      this->erase(this->begin(), this->end());

      for(auto iter = other.begin(); iter != (other.end()); ++iter)
      {
          this->append(*iter);
      }
      return *this;
  }

  LinkedList& operator=(LinkedList&& other)
  {
      if(this == &other)
      {
          return *this;
      }

      this->erase(this->begin(), this->end());

      first->next=other.first->next;
      last->prev=other.last->prev;

      other.first->next->prev=first;
      other.last->prev->next=last;


      other.first->next= other.last;
      other.last->prev= other.first;

      return *this;
  }

  bool isEmpty() const
  {
    if(first->next == last)
    {
      return true;
    }else
    {
      return false;
    }
  }

  size_type getSize() const
  {
    if(isEmpty())
    {
        return 0;
    }else
    {
        Node<Type> * ptr;
        size_type count = 0;
        for(ptr = first; ptr->next != last; ptr = ptr->next)
        {
            count++;
        }
        return count;
    }
  }

  void append(const Type& item)
  {
      Node<Type>* newElement= new Node<Type>(item);
      Node<Type>* temp = last->prev;

      last->prev = newElement;
      newElement->next = last;
      newElement->prev = temp;

      temp->next = newElement;
  }

  void prepend(const Type& item)
  {
      Node<Type>* newElement= new Node<Type>(item);
      Node<Type>* temp = first->next;

      first->next = newElement;
      newElement->next = temp;
      newElement->prev = first;

      temp->prev = newElement;
  }

  void insert(const const_iterator& insertPosition, const Type& item)
  {
      Node<Type>* newElement = new Node<Type>(item);
      Node<Type>* behind = insertPosition.getActual()->prev;

      newElement->next = insertPosition.getActual();
      newElement->prev = behind;
      behind->next = newElement;
      insertPosition.getActual()->prev = newElement;
  }

  Type popFirst()
  {
    if(this->isEmpty())
    {
      throw std::logic_error("List: Can't pop from empty list");
    }

    auto temp = first->next;
    erase(begin());

    return temp->data;
  }

  Type popLast()
  {
    if(this->isEmpty())
    {
      throw std::logic_error("List: Can't pop from empty list");
    }

    auto temp = last->prev;
    erase(end()-1);

    return temp->data;
  }

  void erase(const const_iterator& possition)
  {
    Node<Type> * temp = possition.getActual();
    if(temp->next == nullptr || temp->prev == nullptr)
    {
      throw std::out_of_range("List: Can't delete sentinel");
    }

    temp->next->prev = temp->prev;
    temp->prev->next = temp->next;
    delete temp;
  }

  void erase(const const_iterator& firstIncluded, const const_iterator& lastExcluded)
  {
      auto current = firstIncluded;
      while(current != lastExcluded)
      {
          auto temp  = current;
          current++;
          erase(temp);
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
  Node<Type> *actual;
public:
  explicit ConstIterator():
    actual(nullptr)
  {}

  ConstIterator(Node<Type>* actual_c):
    actual(actual_c)
  {}

  Node<Type>* getActual() const
  {
      return (actual);
  }

  reference operator*() const
  {
      if(actual->next==nullptr || actual->prev==nullptr)
      {
          throw std::out_of_range("ConstIterator::List: Can't get data from sentinel - op*");
      }else
      {
          return actual->data;
      }
  }

  ConstIterator& operator++()
  {
     if(actual->next == nullptr)
     {
         throw std::out_of_range("ConstIterator::List: Can't go further - ++op");

     }else
     {
         actual=actual->next;
         return *this;
     }
  }

  ConstIterator operator++(int)
  {
      if(actual->next == nullptr)
      {
          throw std::out_of_range("ConstIterator::List: Can't go further - op++");
      }else
      {
          ConstIterator ret = *this;
          actual=actual->next;
          return ret;
      }

  }

  ConstIterator& operator--()
  {
    if(actual->prev->prev == nullptr)
    {
        throw std::out_of_range("ConstIterator::List: Can't go further - --op");
    }else
    {
        actual=actual->prev;
        return *this;
    }
  }

  ConstIterator operator--(int)
  {
     if(actual->prev->prev == nullptr)
     {
         throw std::out_of_range("ConstIterator::List: Can't go further - op--");
     }else
     {
         ConstIterator ret = *this;
         actual=actual->prev;
         return ret;
     }
  }

  ConstIterator operator+(difference_type d) const
  {
      ConstIterator temp = *this;
      if(d>=0)
          while(d>0)
          {
              --d;
              ++temp;
              if(temp.actual == nullptr) throw std::out_of_range("ConstIterator::List: Can't go forward - last  sentinel" );
          }
      else
          while(d<0)
          {
              ++d;
              --temp;
              if(temp.actual == nullptr) throw std::out_of_range("ConstIterator::List: Can't go backward - first  sentinel" );
          }
      return temp;
  }

  ConstIterator operator-(difference_type d) const
  {
      ConstIterator temp = *this;
      if(d<=0)
          while(d<0)
          {
              ++d;
              --temp;
              if(temp.actual == nullptr) throw std::out_of_range("ConstIterator::List: Can't go forward - last  sentinel");
          }
      else
          while(d>0)
          {
              --d;
              --temp;
              if(temp.actual == nullptr) throw std::out_of_range("ConstIterator::List: Can't go backward - first  sentinel" );
          }
      return temp;
  }

  bool operator==(const ConstIterator& other) const
  {
      if(this->actual == other.actual)
      {
          return true;
      }else
      {
          return false;
      }
  }

  bool operator!=(const ConstIterator& other) const
  {
      if(this->actual != other.actual)
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
