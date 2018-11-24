#include <cstddef>
#include <cstdlib>
#include <string>
#include <chrono>
#include <iostream>

#include "Vector.h"
#include "LinkedList.h"

template <typename T>
using ListCollection = aisdi::LinkedList<T>;

template <typename T>
using VectorCollection = aisdi::Vector<T>;

void prependTest(ListCollection<int> list, VectorCollection<int> vec, int count)
{
  //list

    auto start = std::chrono::system_clock::now();
    for(int i = count; i > 0; i++)
    {
      list.prepend(i);
    }
    auto end = std::chrono::system_clock::now();

    std::chrono::duration<double> elapsedSecondsL = end - start;

    std::cout << "Finished prepend for list - " << count << " elements, time : " << elapsedSecondsL.count() << std::endl;


  //vector

    start = std::chrono::system_clock::now();
    for(int i = count; i > 0; i++)
    {
      vec.prepend(i);
    }
    end = std::chrono::system_clock::now();

    std::chrono::duration<double> elapsedSecondsV = end - start;

    std::cout << "Finished prepend for vector - " << count << " elements, time : " << elapsedSecondsV.count() << std::endl;
}

void appendTest(ListCollection<int> list, VectorCollection<int> vec, int count)
{
  //list

  auto start = std::chrono::system_clock::now();
  for(int i = count; i > 0; i++)
  {
    list.append(i);
  }
  auto end = std::chrono::system_clock::now();

  std::chrono::duration<double> elapsedSecondsL = end - start;

  std::cout << "Finished prepend for list - " << count << " elements, time : " << elapsedSecondsL.count() << std::endl;


  //vector

  start = std::chrono::system_clock::now();
  for(int i = count; i > 0; i++)
  {
    vec.append(i);
  }
  end = std::chrono::system_clock::now();

  std::chrono::duration<double> elapsedSecondsV = end - start;

  std::cout << "Finished prepend for vector - " << count << " elements, time : " << elapsedSecondsV.count() << std::endl;
}


void popFirstTest(ListCollection<int> list, VectorCollection<int> vec, int count)
{
  //list

  auto start = std::chrono::system_clock::now();
  for(int i = count; i > 0; i++)
  {
    list.popFirst();
  }
  auto end = std::chrono::system_clock::now();

  std::chrono::duration<double> elapsedSecondsL = end - start;

  std::cout << "Finished popFirst for list - " << count << " elements, time : " << elapsedSecondsL.count() << std::endl;


  //vector

  start = std::chrono::system_clock::now();
  for(int i = count; i > 0; i++)
  {
    vec.popFirst();
  }
  end = std::chrono::system_clock::now();

  std::chrono::duration<double> elapsedSecondsV = end - start;

  std::cout << "Finished popFirst for vector - " << count << " elements, time : " << elapsedSecondsV.count() << std::endl;
}

void popLastTest(ListCollection<int> list, VectorCollection<int> vec, int count)
{
  //list

  auto start = std::chrono::system_clock::now();
  for(int i = count; i > 0; i++)
  {
    list.popLast();
  }
  auto end = std::chrono::system_clock::now();

  std::chrono::duration<double> elapsedSecondsL = end - start;

  std::cout << "Finished popLast for list - " << count << " elements, time : " << elapsedSecondsL.count() << std::endl;


  //vector

  start = std::chrono::system_clock::now();
  for(int i = count; i > 0; i++)
  {
    vec.popLast();
  }
  end = std::chrono::system_clock::now();

  std::chrono::duration<double> elapsedSecondsV = end - start;

  std::cout << "Finished popLast for vector - " << count << " elements, time : " << elapsedSecondsV.count() << std::endl;
}


int main()//int argc, char** argv)
{
  ListCollection <int> list;
  VectorCollection <int> vec;
  for(int i : {1 , 2, 3})
  {
    prependTest(list, vec, i);
    popLastTest(list, vec, i);
    appendTest(list, vec, i);
    popFirstTest(list, vec, i);
  }

  return 0;
}
