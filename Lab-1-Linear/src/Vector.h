#ifndef AISDI_LINEAR_VECTOR_H
#define AISDI_LINEAR_VECTOR_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>

#define ALLOC_SIZE 150

namespace aisdi
{

template <typename Type>
class Vector
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

  size_type size;
  size_type count;
  Type * vec_table;

  void resize()
  {
    Type * temp;
    size = size + ALLOC_SIZE;
    temp = new Type[size];
    for(int i = 0; i < (int)size; i++)
    {
      temp[i] = vec_table[i];
    }
    delete[] vec_table;
    vec_table = temp;
  }

public:
  Vector():
    size(0), count(0)
  {
    vec_table = new value_type[ALLOC_SIZE];
  }

  Vector(std::initializer_list<Type> l)
  {
    count = 0;
    size =  l.size();

    vec_table = new Type[size];

    auto list = l.begin();
    while(list != l.end())
    {
        this->append(*list);
        ++list;
    }
  }

  Vector(const Vector& other)
  {
    count = 0;
    size =  other.getSize();

    vec_table = new Type[size];

    for (int i = 0; i < (int) other.size; ++i)
    {
        this->append(other.vec_table[i]);
    }
    /*
    auto vec = other.begin();
    while(vec != other.end())
    {
        this->append(*vec);
        ++vec;
    }
     */
  }

  Vector(Vector&& other)//:
    //Vector()     //delegated constructor deleted to fill with test - GivenNonEmptyCollection_WhenMovingToOther_ThenAllItemsAreMoved
  {
    this->vec_table = other.vec_table;
    other.vec_table = nullptr; //we steal from other
    this->size = other.size;
    other.size = 0;
    this->count = other.count;
    other.count = 0;
  }

  ~Vector()
  {
    if(vec_table != nullptr)
    {
      delete[] vec_table;
    }
  }

  Vector& operator=(const Vector& other)
  {
    if(this == &other)
    {
      return *this;
    }

    delete[] vec_table;
    this->count = 0;
    this->size = other.size;
    this->vec_table = new Type[size];

    for (int i = 0; i < (int) other.size; ++i)
    {
        this->append(other.vec_table[i]);
    }

/*
    auto vec = other.begin();
    while(vec != other.end())
    {
        this->append(*vec);
        ++vec;
    }
*/
    return *this;
  }

  Vector& operator=(Vector&& other)
  {
    if(this == &other)
    {
      return *this;
    }

    delete[] vec_table;
    this->size = 0;
    this->count = 0;
    //this->vec_table = new Type[ALLOC_SIZE];
    this->vec_table = nullptr; //we steal form other

    std::swap(this->vec_table, other.vec_table);
    std::swap(this->size, other.size);
    std::swap(this->count, other.count);
    return *this;
  }

  bool isEmpty() const
  {
    if(count == 0)
    {
      return true;
    }else
    {
      return false;
    }
  }

  size_type getSize() const
  {
    return count;
  }

  void append(const Type& item)
  {
    if(size == count)
    {
      resize();
    }
    vec_table[count] = item;
    count++;
  }

  void prepend(const Type& item)
  {
    insert(begin(), item);
  }

  void insert(const const_iterator& insertPosition, const Type& item)
  {
    if(this->size < insertPosition.position)
    {
      throw std::logic_error("Vector: Can't insert outside vector");
    }

    if(insertPosition == cend())
    {
      append(item); //resize already in place
    }else
    {
      if(size == count)
      {
        resize();
      }

      int where = insertPosition.position;
      int i = count;
      while(i > where)
      {
          vec_table[i] = vec_table[i-1];
          i--;
      }
      vec_table[where] = item;
      count++;
      /*
      if(size == count)
      {
        resize();
      }
      ++count;
      for(ConstIterator it = end(); it != insertPosition; it--)
      {
        vec_table[it.position] = vec_table[it.position - 1];
      }
      */
    }
  }

  Type popFirst()
  {
    if(this->isEmpty())
    {
      throw std::out_of_range("Vector: Can't pop from empty vector");
    }
    Type ret = vec_table[0];

    for(int i = 0; i < (int)count-1; i++)
    {
      vec_table[i] = vec_table[i+1];
    }

    count--;
    return ret;
  }

  Type popLast()
  {
    if(this->isEmpty())
    {
      throw std::out_of_range("Vector: Can't pop from empty vector");
    }
    Type ret = vec_table[count-1];

    count--;
    return ret;
  }

  void erase(const const_iterator& possition)
  {
    if(this->isEmpty() || possition.position >= count)
    {
      throw std::out_of_range("Vector: Can't erase element because of no element");
    }

    int delete_position = possition.position;

    for(int i = delete_position; i < (int)count; i++)
    {
      vec_table[i] = vec_table[i+1];
    }

    count--;
  }

  void erase(const const_iterator& firstIncluded, const const_iterator& lastExcluded)
  {
    if(firstIncluded==lastExcluded)
    {
      return; //nothing to delete
    }

    if(this->isEmpty())// || firstIncluded.position >= count || lastExcluded.position >= count)
    {
      throw std::out_of_range("Vector: Can't erase elements - out of range");
    }

    size_type begin_delete = firstIncluded.position;
    size_type end_delete_inc = lastExcluded.position;

    if(begin_delete>=end_delete_inc)
    {
      throw std::logic_error("Vector: Can't erase - begin of erase is further than End");
    }
    //No iterator - already have begin_delete and end_delete_inc

    size_type diff = end_delete_inc - begin_delete;

    while(end_delete_inc < count)
    {
      vec_table[begin_delete] = vec_table[end_delete_inc];
      begin_delete++;
      end_delete_inc++;
    }
    /*
    //TODO : Delete loop below
    while (end_delete_inc > begin_delete) {
        this->popLast();
        end_delete_inc--;
    }
    */
    count = count - diff;
  }

  iterator begin()
  {
    return iterator(cbegin());
  }

  iterator end()
  {
    return iterator(cend());
  }

  const_iterator cbegin() const
  {
    return  ConstIterator(this, &(vec_table[0]), 0);
  }

  const_iterator cend() const
  {
    //point first empty place
    return  ConstIterator(this, &(vec_table[count]), count);
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
class Vector<Type>::ConstIterator
{
public:
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = typename Vector::value_type;
  using difference_type = typename Vector::difference_type;
  using pointer = typename Vector::const_pointer;
  using reference = typename Vector::const_reference;
//protected:
  const Vector<Type>* vector;
  Type* current;
  size_type position;
public:

  explicit ConstIterator():
    vector(nullptr), current(nullptr), position(0)
  {}

  ConstIterator(const Vector<Type>* vector_c, Type* current_c, size_type position_c):
      vector(vector_c), current(current_c), position(position_c)
  {}
/*
  ConstIterator(const ConstIterator &other)
  {
    this->current = other.current;
    this->vector = other.vector;
    this->position = other.position;
  }
*/

/*
  size_type getPosition const
  {
    return position;
  }
*/
  reference operator*() const
  {
    if(position >= vector->getSize() || position < 0)
    {
      throw std::out_of_range("Vector::ConstIterator: this in not object in Vector - op*");
    }else
    {
      return *current;
    }
  }

  ConstIterator& operator++()
  {
    if(position >= vector->getSize())
    {
      throw std::out_of_range("Vector::ConstIterator: this in not place in Vector - ++op");
    }else
    {
      current++;
      position++;
      return *this;
    }
  }

  ConstIterator operator++(int)
  {
    if(position >= vector->getSize())
    {
      throw std::out_of_range("Vector::ConstIterator: this in not place in Vector - op++");
    }else
    {
      ConstIterator ret = *this;
      current++;
      position++;
      return ret;
    }
  }

  ConstIterator& operator--()
  {
    if(position <= 0)
    {
      throw std::out_of_range("Vector::ConstIterator: this in not place in Vector - --op");
    }else
    {
      current--;
      position--;
      return *this;
    }
  }

  ConstIterator operator--(int)
  {
    if(position <= 0)
    {
      throw std::out_of_range("Vector::ConstIterator: this in not place in Vector - op--");
    }else
    {
      ConstIterator ret = *this;
      current--;
      position--;
      return ret;
    }
  }

  ConstIterator operator+(difference_type d) const
  {
    size_type dif = static_cast<size_type>(d);
    if(position + d > vector->getSize())
    {
      throw std::out_of_range("ConstIterator: this in not place in Vector - op+");
    }else
    {
      size_type new_size = (position + dif);
      return ConstIterator(vector, &(vector->vec_table[position + dif]), new_size);
    }
  }

  ConstIterator operator-(difference_type d) const
  {
    size_type dif = static_cast<size_type>(d);
    if(position - d < 0)
    {
      throw std::out_of_range("ConstIterator: this in not place in Vector - op-");
    }else
    {
      return ConstIterator(vector, &(vector->vec_table[position - dif]), (position - dif));
    }
  }

  bool operator==(const ConstIterator& other) const
  {
    if(this->current == other.current)
    {
      return true;
    }else
    {
      return false;
    }
  }

  bool operator!=(const ConstIterator& other) const
  {
    if(this->current != other.current)
    {
      return true;
    }else
    {
      return false;
    }
  }
};

template <typename Type>
class Vector<Type>::Iterator : public Vector<Type>::ConstIterator
{
public:
  using pointer = typename Vector::pointer;
  using reference = typename Vector::reference;

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

#endif // AISDI_LINEAR_VECTOR_H
