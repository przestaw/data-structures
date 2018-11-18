#ifndef AISDI_LINEAR_VECTOR_H
#define AISDI_LINEAR_VECTOR_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>

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
  const size_type def_size = 5;
  size_type size;
  size_type count;
  Type * vec_table;
public:
  Vector():
    size(0), count(0)
  {
    resize();
  }

  Vector(std::initializer_list<Type> l)
  {
    count = l.size();
    size = count + 1;
    vec_table = new Type[size];
    auto list = l.begin();
    for(size_type i = 0; i < count; i++)
    {
      vec_table[i] = *(list);
      list++;
    }
  }

  Vector(const Vector& other)
  {
    this->size = other.size;
    this->count = other.count;
    vec_table = new Type[size];
    for(size_type i = 0; i < size; i++)
    {
      this->vec_table[i] = other.vec_table[i];
    }
  }

  Vector(Vector&& other)
  {
    this->size = other.size;
    this->count = other.count;
    this->vec_table = other.vec_table;
    other.size = 0;
    other.count = 0;
    other.vec_table = new Type[def_size]; //maybe alocate anything to ensure destructor has anything to delete
  }

  ~Vector()
  {
    delete[] vec_table;
    vec_table = nullptr;
  }
private:
  void resize()
  {
    Type * temp;
    size += def_size;
    temp = new Type[size];
    for(size_type i = 0; i < size; i++)
    {
      temp[i] = vec_table[i];
    }
    delete[] vec_table;
    vec_table = temp;
  }

public:
  Vector& operator=(const Vector& other)
  {
    if(this == &other)
      {return *this;}

    delete[] vec_table;
    this->count = other.count;
    this->size = other.size;
    this->vec_table = new Type[size];

    for(size_type i = 0; i< this->count; i++)
    {
      this->vec_table[i] = other.vec_table[i];
    }
    return *this;
  }

  Vector& operator=(Vector&& other)
  {
    if(this == &other)
      {return *this;}

    delete[] vec_table;
    this->count = other.count;
    this->size = other.size;
    delete[] this->vec_table;
    this->vec_table = other.vec_table;
    other.size = 0;
    other.count = 0;
    other.vec_table = nullptr;
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
    ++count;
  }

  void prepend(const Type& item)
  {
    insert(begin(), item);
  }

  void insert(const const_iterator& insertPosition, const Type& item)
  {
    if(insertPosition == end())
    {
      append(item);
    }else
    {
      if(size == count)
      {
        resize();
      }
      ++count;
      for(ConstIterator it = end(); it != insertPosition; it--)
      {
        vec_table[it.getPosition()] = vec_table[it.getPosition() - 1];
      }
    }
  }

  Type popFirst()
  {
    if(this->isEmpty())
    {
      throw std::out_of_range("Vector: Can't pop from empty vector");
    }
    Type ret = vec_table[0];
    count--;
    for(size_type i = 0; i < count; i++)
    {
      vec_table[i] = vec_table[i+1];
    }
    return ret;
  }

  Type popLast()
  {
    if(this->isEmpty())
    {
      throw std::out_of_range("Vector: Can't pop from empty vector");
    }
    count--;
    return vec_table[count];
  }

  void erase(const const_iterator& possition)
  {
    if(this->isEmpty() || possition.getPosition() >= count)
    {
      throw std::out_of_range("Vector: Can't erase element because of no element");
    }

    size_type delete_position = possition.getPosition() - 1;

    for(ConstIterator it = end(); it.getPosition() != delete_position; it--)
    {
      vec_table[it.getPosition()-1] = vec_table[it.getPosition()];
    }
  }

  void erase(const const_iterator& firstIncluded, const const_iterator& lastExcluded)
  {
    if(firstIncluded==lastExcluded)
    {
      return; //nothing to delete
    }

    if(this->isEmpty())// || firstIncluded.getPosition() >= count || lastExcluded.getPosition() >= count)
    {
      throw std::out_of_range("Vector: Can't erase elements - out of range");
    }

    size_type begin_delete = firstIncluded.getPosition();
    size_type end_delete_inc = lastExcluded.getPosition();

    if(begin_delete>=end_delete_inc)
    {
      throw std::logic_error("Vector: Can't erase - begin of erase is further than End");
    }
    //No iterator - already have begin_delete and end_delete_inc

    size_type diff = end_delete_inc - begin_delete;

    while(end_delete_inc < count)
    {
      vec_table[begin_delete] = vec_table[begin_delete];
      begin_delete++;
      end_delete_inc++;
    }

    count -= diff;
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
    return  ConstIterator(this, vec_table, 0);
  }

  const_iterator cend() const
  {
    //point first empty place
    return  ConstIterator(this, (vec_table + count), count);
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
private:
  const aisdi::Vector<Type>* vector;
  Type* current;
  size_type position;
public:

  explicit ConstIterator():
    vector(nullptr), current(nullptr), position(0)
  {}

  ConstIterator(const Vector<Type>* vector_c = nullptr, Type* current_c = nullptr, size_type position_c = 0):
      vector(vector_c), current(current_c), position(position_c)
  {}

  ConstIterator(const ConstIterator &other)
  {
    this->current = other.current;
    this->vector = other.vector;
    this->position = other.position;
  }

  size_type getPosition() const
  {
    return position;
  }

  reference operator*() const
  {
    if(position >= vector->getSize() || position < 0)
    {
      throw std::out_of_range("ConstIterator: this in not object in Vector");
    }else
    {
      return *current;
    }
  }

  ConstIterator& operator++()
  {
    if(position >= vector->getSize())
    {
      throw std::out_of_range("ConstIterator: this in not place in Vector");
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
      throw std::out_of_range("ConstIterator: this in not place in Vector");
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
      throw std::out_of_range("ConstIterator: this in not place in Vector");
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
      throw std::out_of_range("ConstIterator: this in not place in Vector");
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
    size_type dif = static_cast<int_fast64_t >(d);
    if(position + d >= vector->getSize())
    {
      throw std::out_of_range("ConstIterator: this in not place in Vector");
    }else
    {
      return ConstIterator(vector, current + dif, position + dif);
    }
  }

  ConstIterator operator-(difference_type d) const
  {
    size_type dif = static_cast<int_fast64_t>(d);
    if(position - d < 0)
    {
      throw std::out_of_range("ConstIterator: this in not place in Vector");
    }else
    {
      //current -= dif;
      //position -= dif;
      return ConstIterator(vector, current - dif, position - dif);
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
