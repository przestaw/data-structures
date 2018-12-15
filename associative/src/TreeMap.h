#ifndef AISDI_MAPS_TREEMAP_H
#define AISDI_MAPS_TREEMAP_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <utility>

#include <iostream>

namespace associative
{

template <typename KeyType, typename ValueType>
class TreeMap
{
public:
  using key_type = KeyType;
  using mapped_type = ValueType;
  using value_type = std::pair<const key_type, mapped_type>;
  using size_type = std::size_t;
  using reference = value_type&;
  using const_reference = const value_type&;

  class ConstIterator;
  class Iterator;
  using iterator = Iterator;
  using const_iterator = ConstIterator;

    class Node
    {
    public:
        Node *papa;
        Node *l_son;
        Node *r_son;
        int balance;

        value_type node_value;

        Node() :
            papa(nullptr), l_son(nullptr), r_son(nullptr), balance(0)
        {}

        Node(const key_type key, mapped_type value) :
                papa(nullptr), l_son(nullptr), r_son(nullptr), balance(0),
                node_value(std::make_pair(key, value))
        {}

        Node(value_type val_typ) :
            papa(nullptr), l_son(nullptr), r_son(nullptr), balance(0),
            node_value(val_typ)
        {}

        ~Node()
        {
          if(l_son != nullptr)
          {
            delete l_son;
          }
          if(r_son != nullptr)
          {
            delete r_son;
          }
        }

    };
private:
    Node * tree_root;
    size_type count;
public:

  TreeMap() :
    tree_root(nullptr), count(0)
  {}

  TreeMap(std::initializer_list<value_type> list) :
    TreeMap()
  {
    for (auto it = list.begin(); it != list.end(); ++it)
    {
      insert( new Node(*it) );
    }
  }

  TreeMap(const TreeMap& other) :
    TreeMap()
  {
    for (auto it = other.begin(); it != other.end(); ++it)
    {
      insert( new Node(*it) );
    }
  }

  TreeMap(TreeMap&& other) :
    TreeMap()
  {
    std::swap(this->tree_root, other.tree_root);
    std::swap(this->count, other.count);
  }

  ~TreeMap()
  {
    delete tree_root;
  }

  TreeMap& operator=(const TreeMap& other)
  {
    if(this == &other)
    {
      return *this;
    }
    else
    {
      delete tree_root;
      this->tree_root = nullptr;
      this->count = 0;

      for (auto it = other.begin(); it != other.end(); ++it)
      {
        insert( new Node(*it) );
      }
      return *this;
    }
  }

  TreeMap& operator=(TreeMap&& other)
  {
    if(this == &other)
    {
      return *this;
    }
    else
    {
      delete tree_root;
      this->tree_root = nullptr;
      this->count = 0;

      std::swap(this->tree_root, other.tree_root);
      std::swap(this->count, other.count);

      return *this;
    }
  }

  bool isEmpty() const
  {
    return count == 0;
  }

  mapped_type& operator[](const key_type& key)
  {
    Node* current = find_node(key);
    if( current == nullptr )
    {
      current = new Node(key,mapped_type());
      insert( current );
    }
    return current->node_value.second;
  }

  const mapped_type& valueOf(const key_type& key) const
  {
    const Node* current = find_node(key);
    if(current == nullptr)
    {
      throw std::out_of_range("valueOf");
    }
    else
    {
      return current->node_value.second;
    }
  }

  mapped_type& valueOf(const key_type& key)
  {
    Node* current = find_node(key);
    if( current == nullptr )
    {
      current = new Node(key,mapped_type());
      insert( current );
    }
    return current->node_value.second;
  }

  const_iterator find(const key_type& key) const
  {
    return const_iterator(find_node(key), this);
  }

  iterator find(const key_type& key)
  {
    return iterator(find_node(key), this);
  }

  void remove(const key_type& key)
  {
    Node * temp = find_node(key);
    if(temp != nullptr)
    {
      remove(temp);
    }
    else
    {
      throw std::out_of_range("TreeMap : can't remove - no mathing element");
    }
  }

  void remove(const const_iterator& it)
  {
    if(it.get_tree() != this)
    {
      throw std::out_of_range("TreeMap : can't remove - iterator from other tree");
    }
    else if(it == cend())
    {
      throw std::out_of_range("TreeMap : can't remove - no mathing element");
    }
    else
    {
      remove(it.get_node());
    }
  }

  size_type getSize() const
  {
    return count;
  }

  bool operator==(const TreeMap& other) const
  {
    if( count != other.count )
    {
      return false;
    }
    else
    {
      for(auto it = begin(), it2 = other.begin(); it != end(); ++it, ++it2)
      {
        if(*it != *it2)
        {
          return false;
        }
      }
    }
    return true;
  }

  bool operator!=(const TreeMap& other) const
  {
    return !(*this == other);
  }

  iterator begin()
  {
    return iterator(get_most_left(), this);
  }

  iterator end()
  {
    return iterator(nullptr, this);
  }

  const_iterator cbegin() const
  {
    return const_iterator(get_most_left(), this);
  }

  const_iterator cend() const
  {
    return const_iterator(nullptr, this);
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

    Node* get_most_left() const
    {
      Node *node = tree_root;
      if(node != nullptr)
      {
        while(node->l_son != nullptr)
        {
          node = node->l_son;
        }
      }
      return node;
    }

    Node* find_node(const key_type& key) const
    {
      Node* node = tree_root;
      while ( node != nullptr )
      {
        if (key > node->node_value.first)
        {
          node = node->r_son;
        }
        else if (key < node->node_value.first)
        {
          node = node->l_son;
        }
        else break;
      }
      return node;
    }

    void replace(Node* A, Node* B)
    {
      //przepinany papa od A
      if (A->papa == nullptr)
      {
        tree_root = B;
      }
      else if (A == A->papa->l_son)
      {
        A->papa->l_son = B;
      }
      else
      {
        A->papa->r_son = B;
      }

      if (B != nullptr)
      {
        //nowy papa dla B
        B->papa = A->papa;
        //nowy r_son dla B (jesli if jest spełniony to r_son od B powinien byc nullptr)
        if(A->r_son != nullptr && A->r_son != B)
        {
          B->r_son = A->r_son;
        }
        //nowy l_son dla B (jesli if jest spełniony to l_son od B powinien byc nullptr)
        if(A->l_son != nullptr && A->l_son != B)
        {
          B->l_son = A->l_son;
        }
      }
      //odseparowanie A
      A->papa = nullptr;
      A->l_son   = nullptr;
      A->r_son  = nullptr;
    }

    void remove(Node* node)
    {
      if (node->l_son == nullptr)
      {
        //w miejsce usuwanego wezla wstawiane jego prawe poddrzewo
        replace(node, node->r_son);
      }
      else if (node->r_son == nullptr)
      {
        //w miejsce usuwanego wezla wstawiane jego lewe poddrzewo
        replace(node, node->l_son);
      }
      else
      { //usuwany wezel ma 2 dzieci
        auto tmp = node->r_son;
        //wyszukiwanie kolejnego elementu wzgledem usuwanego
        while (tmp->l_son != nullptr)
        {
          tmp = tmp->l_son;
        }
        //w miejsce nastepnika wstawiany jego prawe poddrzewo
        replace(tmp, tmp->r_son);
        //w miejsce usuwanego wezla wstawiany jego nastepnik
        replace(node, tmp);
      }
      count--;
      delete node;
    }

    void insert(Node* node)
    {
      if(tree_root == nullptr)
      {
        count++;
        tree_root = node;
      }
      else
      {
        Node * temp = tree_root;

        while(temp != nullptr)
        {
          if(node->node_value.first < temp->node_value.first)
          {
            if(temp->l_son == nullptr)
            {
              temp->l_son = node;
              node->papa = temp;
              count++;
              break;
            }
            else
            {
              temp = temp->l_son;
            }
          }
          else if(node->node_value.first > temp->node_value.first)
          {

            if(temp->r_son == nullptr)
            {
              temp->r_son = node;
              node->papa = temp;
              count++;
              break;
            }
            else
            {
              temp = temp->r_son;
            }
          }
          else //already in tree
          {
            delete node;
            return;
          }
        }
        //balance ??
      }
    }
};

template <typename KeyType, typename ValueType>
class TreeMap<KeyType, ValueType>::ConstIterator
{
public:
  using reference = typename TreeMap::const_reference;
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = typename TreeMap::value_type;
  using pointer = const typename TreeMap::value_type*;
  using Node = typename TreeMap::Node;
private:
   Node *actual;
   const TreeMap *my_tree;
public:
  explicit ConstIterator() :
    actual(nullptr), my_tree(nullptr)
  {}

  ConstIterator(Node *actual_c, const TreeMap *my_tree_c) :
    actual(actual_c), my_tree(my_tree_c)
  {}

  ConstIterator(const ConstIterator& other)
  {
    this->actual = other.actual;
  }

  ConstIterator& operator++()
  {
    if(my_tree == nullptr || actual == nullptr)
    {
      throw std::out_of_range("operator++");
    }
    else if( actual->r_son != nullptr )
    {
      actual = actual->r_son;
      while(actual->l_son != nullptr)
      {
        actual = actual->l_son;
      }
    }
    else {
      while(true)
      {
        if(actual->papa == nullptr)
        {
          actual = nullptr;
          break;
        }
        if(actual->papa->l_son == actual)
        {
          actual = actual->papa;
          break;
        }
        actual = actual->papa;
      }
    }
    return *this;
  }

  ConstIterator operator++(int)
  {
    ConstIterator copy = *this;
    ++(*this);
    return copy;
  }

  ConstIterator& operator--()
  {
    if(my_tree == nullptr || my_tree->tree_root == nullptr)
    {
      throw std::out_of_range("operator--");
    }
    else if(actual == nullptr)
    {
      actual = my_tree->tree_root;
      while(actual->r_son != nullptr)
      {
        actual = actual->r_son;
      }
    }
    else if(actual->l_son != nullptr )
    {
      actual = actual->l_son;
      while(actual->r_son != nullptr)
      {
        actual = actual->r_son;
      }
    }
    else {
      while(true)
      {
        if(actual->papa == nullptr)
        {
          throw std::out_of_range("operator--");
        }
        if(actual->papa->r_son == actual)
        {
          actual = actual->papa;
          break;
        }
        actual = actual->papa;
      }
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
    if(my_tree == nullptr || actual == nullptr)
    {
      throw std::out_of_range("TreeMap::ConstIterator - getting from null op*");
    }
    else
    {
      return actual->node_value;
    }
  }

  pointer operator->() const
  {
    return &this->operator*();
  }

  bool operator==(const ConstIterator& other) const
  {
    if(actual == other.actual)
    {
      return true;
    }
    else
    {
      return false;
    }
  }

  bool operator!=(const ConstIterator& other) const
  {
    return !(*this == other);
  }

  const TreeMap* get_tree() const
  {
    return my_tree;
  }

  Node* get_node() const
  {
    return actual;
  }
};

template <typename KeyType, typename ValueType>
class TreeMap<KeyType, ValueType>::Iterator : public TreeMap<KeyType, ValueType>::ConstIterator
{
public:
  using reference = typename TreeMap::reference;
  using pointer = typename TreeMap::value_type*;

  explicit Iterator(Node *actual_c = nullptr, TreeMap *my_tree = nullptr) :
    ConstIterator(actual_c, my_tree)
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

#endif /* AISDI_MAPS_MAP_H */
