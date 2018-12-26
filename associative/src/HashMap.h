#ifndef ASSOCIATIVE_HASHMAP
#define ASSOCIATIVE_HASHMAP

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <utility>
#include <algorithm>

#include <list>
#define DEFAULT_HASH 25000
#define COLLECTION std::list<value_type>

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

  using collection = COLLECTION;
  using collection_iterator = typename collection::iterator;

  class ConstIterator;
  class Iterator;
  using iterator = Iterator;
  using const_iterator = ConstIterator;
private:
    collection *table;
    size_type hash;
    size_type count;
public:
  HashMap(key_type size = DEFAULT_HASH) :
      hash(size), count(0)
  {
    table = new collection[hash];
  }

  HashMap(std::initializer_list<value_type> list) :
      HashMap(DEFAULT_HASH)
  {
    for(auto it = list.begin(); it != list.end(); ++it)
    {
      insert(*it);
    }
  }

  HashMap(const HashMap& other)
  {
    this->hash = other.hash;
    this->table = new collection[this->hash];
    this->count = 0;

    for(auto it = other.begin(); it != other.end(); ++it)
    {
      insert(*it);
    }
  }

  HashMap(HashMap&& other)
  {
    hash = DEFAULT_HASH;
    table = nullptr;
    count = 0;

    std::swap(this->hash, other.hash);
    std::swap(this->count, other.count);
    std::swap(this->table, other.table);
  }

  ~HashMap()
  {
    if(table != nullptr)
    {
      delete[] table;
    }
  }

  HashMap& operator=(const HashMap& other)
  {
    if(this == &other)
    {
      return *this;
    }
    else
    {
      hash = other.hash;
      count = 0;

      if(table != nullptr)
      {
        delete[] table;
      }
      table = new collection[hash];

      for (auto it = other.begin(); it != other.end(); ++it)
      {
        insert(*it);
      }

      return *this;
    }
  }

  HashMap& operator=(HashMap&& other)
  {
    if(this == &other)
    {
      return *this;
    }
    else
    {
      if(table != nullptr)
      {
        delete[] table;
      }
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
    auto it = find(key); //end()?
    if(it == end())
    {
      //typically insert
      key_type index = hash_value(key);
      table[index].push_back(value_type(key, {}));
      count++;
      return table[index].back().second;
    }
    else
    {
      return it->second;
    }
  }

  const mapped_type& valueOf(const key_type& key) const
  {
    auto it = find(key); //cend()?
    return it->second;
  }

  mapped_type& valueOf(const key_type& key)
  {
    auto it = find(key); //end()?
    return it->second;
  }

  const_iterator find(const key_type& key) const
  {
    if(count == 0)
    {
      return cend();
    }
    else
    {
      key_type index = hash_value(key);
      if(table[index].size() == 0)
      {
        return cend();
      }
      else
      {
        for(auto it = table[index].begin(); it != table[index].end(); it++)
        {
          if(it->first == key)
          {
            return const_iterator(index, it, this);
          }
        }
        return cend();
      }
    }
  }

  iterator find(const key_type& key)
  {
    if(count == 0)
    {
      return end();
    }
    else
    {
      key_type index = hash_value(key);
      if(table[index].size() == 0)
      {
        return end();
      }
      else
      {
        for(auto it = table[index].begin(); it != table[index].end(); it++)
        {
          if(it->first == key)
          {
            return iterator(index, it, this);
          }
        }
        return end();
      }
    }
  }

  void remove(const key_type& key)
  {
    remove(find(key));
  }

  void remove(const const_iterator& it)
  {
    if(it != cend() && count !=0)
    {
      table[hash_value(it->first)].erase(it.get_sub_iterator());
      count--;
    }
    else
    {
      throw std::out_of_range("HashMap : can't remove because of no element / end remove");
    }
  }

  size_type getSize() const
  {
    return count;
  }

  bool operator==(const HashMap& other) const
  {
    if(this->count != other.count)
    {
      return false;
    }
    else
    {
      for(auto it = begin(); it != end(); it++)
      {
        auto temp = other.find(it->first);
        if(temp == other.end() || temp->second != it->second)
        {
          return false;
        }
      }

      return true;
    }
  }

  bool operator!=(const HashMap& other) const
  {
    return !(*this == other);
  }

  iterator begin()
  {
    return find_first();
  }

  iterator end()
  {
    return iterator(hash-1, table[hash-1].end(), this);
  }

  const_iterator cbegin() const
  {
    return c_find_first();
  }

  const_iterator cend() const
  {
    return const_iterator(hash-1, table[hash-1].end(),this);
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


  size_type hash_value(key_type key) const
  {
    return std::hash<key_type>()(key) % this->hash;
  }

  void insert(value_type ins_val)
  {
    key_type index = hash_value(ins_val.first);
    table[index].push_back(ins_val);
    count++;
  }

  iterator find(value_type find_val)
  {
    key_type index = hash_value(find_val.first);
    if(table[index].empty())
    {
      return end();
    }
    else
    {

      collection_iterator it;
      it = std::find(table[index].begin(),table[index].end(), find_val);

      if(it != table[index].end())
      {
        return it;
      }
      else
      {
        return end();
      }
    }
  }

  const_iterator c_find_first() const
  {
    for(uint i = 0; i < hash; i++)//why uint?
    {
      if(table[i].size() > 0)
      {
        return const_iterator(i, table[i].begin(), this);
      }
    }

    return cend();
  }

  iterator find_first()
  {
    for(uint i = 0; i < hash; i++)//why uint?
    {
      if(!table[i].empty())
      {
        return iterator(i, table[i].begin(), this);
      }
    }
    return end();
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
  using collection_iterator = typename HashMap::collection_iterator;
  using collection = typename HashMap::collection;
  using size_type = typename HashMap::size_type;
private:
  const HashMap *my_map;
  size_type current;
  collection_iterator cur_sub_iterator;
public:
  explicit ConstIterator()
  {}

  ConstIterator(const key_type current_c,const collection_iterator cur_sub_iterator_c, const HashMap *mymap_c = nullptr) :
     my_map(mymap_c), current(current_c), cur_sub_iterator(cur_sub_iterator_c)
  {}

  ConstIterator(const ConstIterator& other) :
     my_map(other.my_map), current(other.current), cur_sub_iterator(other.cur_sub_iterator)
  {}

  ConstIterator& operator++()
  {
    if( *this == my_map->end())
    {
      throw std::out_of_range("HashMap::ConstIterator : can't increment end()");
    }
    else if (cur_sub_iterator != --(my_map->table[current].end()))
    {
      cur_sub_iterator++;
      return *this;
    }
    else
    {
      /*
      do{
        current++;
        if (!my_map->table[current].empty())
        {
          break;
        }
      }while (current < my_map->hash);

      cur_sub_iterator = my_map->table[current].begin();
       */
      for(u_int i = current + 1; i < my_map->hash; ++i)
      {
        if(!my_map->table[i].empty())
        {
          current = i;
          cur_sub_iterator = my_map->table[current].begin();
          return *this;
        }
      }
      current = my_map->hash -1;
      cur_sub_iterator = my_map->table[current].end();
      return *this;
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
    else if(cur_sub_iterator != my_map->table[current].begin())
    {
      cur_sub_iterator--;
    }
    else
    {
      /*
      do{
        current--;
        if (!my_map->table[current].empty())
        {
          break;
        }
      }while (current >= 0);

      cur_sub_iterator = --(my_map->table[current].end());
      */
      for(u_int i = current - 1; i > 0; --i)
      {
        if(!(my_map->table[i].empty()))
        {
          current = i;
          cur_sub_iterator = (my_map->table[current].end());
          --cur_sub_iterator;
          return *this;
        }
      }
      current = 0;
      cur_sub_iterator = my_map->table[current].begin();//invalid?
      return *this;
    }
    return *this;
  }

  ConstIterator operator--(int)
  {
    ConstIterator copy = *this;
    --(*this);
    return copy;
  }

  reference operator*() const
  {
    if(*this == my_map->end())
    {
      throw std::out_of_range("HashMap::ConstIterator : can't dereference end()");
    }
    else if(cur_sub_iterator == my_map->table[current].end())
    {
      throw std::out_of_range("HashMap::ConstIterator : can't dereference because of wrong");
    } else
    {
      return *cur_sub_iterator;
    }
  }

  pointer operator->() const
  {
    return &this->operator*();
  }

  bool operator==(const ConstIterator& other) const
  {
    return (my_map == other.my_map && current == other.current && cur_sub_iterator == other.cur_sub_iterator);
  }

  bool operator!=(const ConstIterator& other) const
  {
    return !(*this == other);
  }

  collection_iterator get_sub_iterator() const
  {
    return cur_sub_iterator;
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

  Iterator(key_type current_c, collection_iterator cur_sub_iterator_c, const HashMap *mymap_c = nullptr) :
     ConstIterator(current_c, cur_sub_iterator_c, mymap_c)
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

#endif /* ASSOCIATIVE_HASHMAP */
