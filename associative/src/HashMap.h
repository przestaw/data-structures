#ifndef AISDI_MAPS_HASHMAP_H
#define AISDI_MAPS_HASHMAP_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <utility>
#include <list>
#include <algorithm>

#define DEFAULT_HASH 2601

namespace associative
{

template <typename KeyType, typename ValueType>
class HashMap
{
public:
  using key_type = KeyType;
  using mapped_type = ValueType;
  using value_type = std::pair<const key_type, mapped_type>;
  using size_type = std::size_t;
  using reference = value_type&;
  using const_reference = const value_type&;
  using list_iterator = typename std::list<value_type>::iterator;

  class ConstIterator;
  class Iterator;
  using iterator = Iterator;
  using const_iterator = ConstIterator;
private:
    std::list<value_type> **table;
    key_type hash;
    size_type count;
public:
  HashMap(key_type size = DEFAULT_HASH) :
      hash(size), count(0)
  {
    table = new std::list<value_type>*[hash];
  }

  HashMap(std::initializer_list<value_type> list) :
      HashMap(DEFAULT_HASH)
  {
    (void)list; // disables "unused argument" warning, can be removed when method is implemented.
    throw std::runtime_error("TODO");
  }

  HashMap(const HashMap& other)
  {
    (void)other;
    throw std::runtime_error("TODO");
  }

  HashMap(HashMap&& other)
  {
    hash = DEFAULT_HASH;
    table = nullptr;

    std::swap(this->hash, other.hash);
    std::swap(this->count, other.count);
    std::swap(this->table, other.table);
  }

  HashMap& operator=(const HashMap& other)
  {
    (void)other;
    throw std::runtime_error("TODO");
  }

  HashMap& operator=(HashMap&& other)
  {
    if(this == &other)
    {
      return *this;
    }
    else
    {
      delete this->table;
      this->table = nullptr;
      this->count = 0;

      std::swap(this->hash, other.hash);
      std::swap(this->count, other.count);
      std::swap(this->table, other.table);

      return *this;
    }
  }

  bool isEmpty() const
  {
    return count == 0;
  }

  mapped_type& operator[](const key_type& key)
  {
    (void)key;
    throw std::runtime_error("TODO");
  }

  const mapped_type& valueOf(const key_type& key) const
  {
    (void)key;
    throw std::runtime_error("TODO");
  }

  mapped_type& valueOf(const key_type& key)
  {
    (void)key;
    throw std::runtime_error("TODO");
  }

  const_iterator find(const key_type& key) const
  {
    key_type index = hash_value(key);
    if(table[index] == nullptr || table[index]->size() == 0)
    {
      return end();
    }
    else
    {
      list_iterator it1 = table[index]->begin();
      list_iterator it2 = table[index]->end();
      for(it1; it1 != it2; it1++)
      {
        if((*it1).first == key)
        {
          return const_iterator(index, it1, this);
        }
      }
      //else
      return cend();
    }
  }

  iterator find(const key_type& key)
  {
    key_type index = hash_value(key);
    if(table[index] == nullptr || table[index]->size() == 0)
    {
      return end();
    }
    else
    {
      list_iterator it1 = table[index]->begin();
      list_iterator it2 = table[index]->end();
      for(; it1 != it2; it1++)
      {
        if((*it1).first == key)
        {
          return iterator(index, it1, this);
        }
      }
      //else
      return end();
    }
  }

  void remove(const key_type& key)
  {
    (void)key;
    throw std::runtime_error("TODO");
  }

  void remove(const const_iterator& it)
  {
    (void)it;
    throw std::runtime_error("TODO");
  }

  size_type getSize() const
  {
    return count;
  }

  bool operator==(const HashMap& other) const
  {
    (void)other;
    throw std::runtime_error("TODO");
  }

  bool operator!=(const HashMap& other) const
  {
    return !(*this == other);
  }

  iterator begin()
  {
    return iterator(find_first());
  }

  iterator end()
  {
    return iterator(hash, list_iterator(), this);
  }

  const_iterator cbegin() const
  {
    return find_first();
  }

  const_iterator cend() const
  {
    return const_iterator(hash, list_iterator(),this);
  }

  const_iterator begin() const
  {
    return cbegin();
  }

  const_iterator end() const
  {
    return cend();
  }

private:

  key_type hash_value(key_type key)
  {
    //return std::hash<key_type>()(key) % this->hash;
    return key % this->hash;
  }

  void insert(value_type ins_val)
  {
    key_type key = hash_value(ins_val.first);
    if(table[key] == nullptr)
    {
      table[key] = new std::list<value_type >;
    }

    table[key]->push_back(ins_val);
    count++;
  }

  iterator find(value_type find_val)
  {
    key_type index = hash_value(find_val.first);
    if(table[index] == nullptr || table[index]->size() == 0)
    {
      return end();
    }
    else
    {
      list_iterator it;
      it = std::find(table[index]->begin(),table[index]->end(), find_val);

      if(it != table[index]->end())
      {
        return it;
      }
      else
      {
        return end();
      }
    }
  }

  const_iterator find_first()
  {

  }

};

template <typename KeyType, typename ValueType>
class HashMap<KeyType, ValueType>::ConstIterator
{
public:
  using reference = typename HashMap::const_reference;
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = typename HashMap::value_type;
  using pointer = const typename HashMap::value_type*;
  using key_type = typename HashMap::key_type;
  using list_iterator = typename std::list<value_type>::iterator;
private:
  HashMap *my_map;
  key_type current;
  list_iterator cur_list_iter;
public:
  explicit ConstIterator()
  {}

  ConstIterator(key_type current_c, list_iterator cur_list_iter_c,HashMap *mymap_c = nullptr) :
     my_map(mymap_c), current(current_c), list_iterator(cur_list_iter_c)
  {}

  ConstIterator(const ConstIterator& other) :
    my_map(other.my_map), current(other.current), list_iterator(cur_list_iter)
  {}

  ConstIterator& operator++()
  {
    if(current == my_map->hash)
    {
      throw std::out_of_range("HashMap::ConstIterator : can't increment end()");
    }
    else
    {
      if(cur_list_iter == my_map->table[current].end())
      {
        do{
          current++;
          if(my_map->table[current] != nullptr)
          {
            break;
          }
        }while(current <= my_map->hash);
        if(current == my_map->hash)
        {
          return my_map->end();
        }
        else
        {
          cur_list_iter = my_map->table[current].begin();
          return *this;
        }
      }
      else
      {
        cur_list_iter++;
        return *this;
      }
    }
  }

  ConstIterator operator++(int)
  {
    ConstIterator copy = *this;
    ++(*this);
    return copy;
  }

  ConstIterator& operator--()
  {
    if(*this == my_map->begin())
    {
      throw std::out_of_range("HashMap::ConstIterator : can't increment begin()");
    }
    else if(current == my_map->hash)
    {
      key_type temp = current;
      do{
        temp--;
        if(my_map->table[temp] != nullptr)
        {
          break;
        }
      }while(temp >= 0);
      if(temp == 0 && my_map->table[temp] == nullptr) //temp = 0 && no element in
      {
        return my_map->begin();
      }
      else
      {
        current = temp;
        cur_list_iter = my_map->table[current].end();
        cur_list_iter++;
        return *this;
      }
    }
    else
    {
      if(cur_list_iter == my_map->table[current].begin())
      {
        do{
          current--;
          if(my_map->table[current] != nullptr)
          {
            break;
          }
        }while(current >= 0);
        if(current == 0 && my_map->table[0] == nullptr)
        {
          return my_map->begin();
        }
        else
        {
          cur_list_iter = my_map->table[current].end();
          cur_list_iter--;
          return *this;
        }
      }
      else
      {
        cur_list_iter--;
        return *this;
      }
    }
  }

  ConstIterator operator--(int)
  {
    ConstIterator copy = *this;
    --(*this);
    return copy;
  }

  reference operator*() const
  {
    if(my_map == nullptr || current == my_map->hash)
    {
      throw std::out_of_range("HashMap::ConstIterator : can't dereference end()");
    }
    else
    {
      return *cur_list_iter;
    }
  }

  pointer operator->() const
  {
    return &this->operator*();
  }

  bool operator==(const ConstIterator& other) const
  {
    return (my_map == other.my_map && current == other.current && cur_list_iter == other.cur_list_iter);
  }

  bool operator!=(const ConstIterator& other) const
  {
    return !(*this == other);
  }
};

template <typename KeyType, typename ValueType>
class HashMap<KeyType, ValueType>::Iterator : public HashMap<KeyType, ValueType>::ConstIterator
{
public:
  using reference = typename HashMap::reference;
  using pointer = typename HashMap::value_type*;

  explicit Iterator()
  {}

  Iterator(key_type current_c, list_iterator cur_list_iter_c,HashMap *mymap_c = nullptr) :
     ConstIterator(current_c, cur_list_iter_c, mymap_c)
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

  pointer operator->() const
  {
    return &this->operator*();
  }

  reference operator*() const
  {
    // ugly cast, yet reduces code duplication.
    return const_cast<reference>(ConstIterator::operator*());
  }
};

}

#endif /* AISDI_MAPS_HASHMAP_H */
