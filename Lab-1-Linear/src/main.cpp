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

void prependTest(ListCollection<int> &list, VectorCollection<int> &vec, int count)
{
  //list

    auto start = std::chrono::system_clock::now();
    for(int i = count; i > 0; i--)
    {
      list.prepend(i);
    }
    auto end = std::chrono::system_clock::now();

    std::chrono::duration<double> elapsedSecondsL = end - start;
	std::time_t end_timeL = std::chrono::system_clock::to_time_t(end);
    std::cout << "Finished prepend for list - " << count << " elements, time : " << elapsedSecondsL.count() << ",\n at time : " << std::ctime(&end_timeL) << std::endl;


  //vector

    start = std::chrono::system_clock::now();
    for(int i = count; i > 0; i--)
    {
      vec.prepend(i);
    }
    end = std::chrono::system_clock::now();
	std::time_t end_timeV = std::chrono::system_clock::to_time_t(end);
    std::chrono::duration<double> elapsedSecondsV = end - start;

    std::cout << "Finished prepend for vector - " << count << " elements, time : " << elapsedSecondsV.count() << ",\n at time : " << std::ctime(&end_timeV) << std::endl;
}

void appendTest(ListCollection<int> &list, VectorCollection<int> &vec, int count)
{
  //list

  auto start = std::chrono::system_clock::now();
  for(int i = count; i > 0; i--)
  {
    list.append(i);
  }
  auto end = std::chrono::system_clock::now();
	std::time_t end_timeL = std::chrono::system_clock::to_time_t(end);
  std::chrono::duration<double> elapsedSecondsL = end - start;

  std::cout << "Finished append for list - " << count << " elements, time : " << elapsedSecondsL.count() << ",\n at time : " << std::ctime(&end_timeL) << std::endl;


  //vector

  start = std::chrono::system_clock::now();
  for(int i = count; i > 0; i--)
  {
    vec.append(i);
  }
  end = std::chrono::system_clock::now();
	std::time_t end_timeV = std::chrono::system_clock::to_time_t(end);
  std::chrono::duration<double> elapsedSecondsV = end - start;

  std::cout << "Finished append for vector - " << count << " elements, time : " << elapsedSecondsV.count()  << ",\n at time : " << std::ctime(&end_timeV) << std::endl;
}


void popFirstTest(ListCollection<int> list, VectorCollection<int> vec, int count)
{
  //list

  auto start = std::chrono::system_clock::now();
  for(int i = count; i > 0; i--)
  {
    list.popFirst();
  }
  auto end = std::chrono::system_clock::now();
	std::time_t end_timeL = std::chrono::system_clock::to_time_t(end);
  std::chrono::duration<double> elapsedSecondsL = end - start;

  std::cout << "Finished popFirst for list - " << count << " elements, time : " << elapsedSecondsL.count() << ",\n at time : " << std::ctime(&end_timeL) << std::endl;


  //vector

  start = std::chrono::system_clock::now();
  for(int i = count; i > 0; i--)
  {
    vec.popFirst();
  }
  end = std::chrono::system_clock::now();
	std::time_t end_timeV = std::chrono::system_clock::to_time_t(end);
  std::chrono::duration<double> elapsedSecondsV = end - start;

  std::cout << "Finished popFirst for vector - " << count << " elements, time : " << elapsedSecondsV.count()  << ",\n at time : " << std::ctime(&end_timeV) << std::endl;
}

void popLastTest(ListCollection<int> list, VectorCollection<int> vec, int count)
{
  //list

  auto start = std::chrono::system_clock::now();
  for(int i = count; i > 0; i--)
  {
    list.popLast();
  }
  auto end = std::chrono::system_clock::now();
	std::time_t end_timeL = std::chrono::system_clock::to_time_t(end);
  std::chrono::duration<double> elapsedSecondsL = end - start;

  std::cout << "Finished popLast for list - " << count << " elements, time : " << elapsedSecondsL.count() << ",\n at time : " << std::ctime(&end_timeL) << std::endl;


  //vector

  start = std::chrono::system_clock::now();
  for(int i = count; i > 0; i--)
  {
    vec.popLast();
  }
  end = std::chrono::system_clock::now();
	std::time_t end_timeV = std::chrono::system_clock::to_time_t(end);
  std::chrono::duration<double> elapsedSecondsV = end - start;

  std::cout << "Finished popLast for vector - " << count << " elements, time : " << elapsedSecondsV.count()  << ",\n at time : " << std::ctime(&end_timeV) << std::endl;
}


void eraseTest(ListCollection <int> &l, VectorCollection <int> &v, int count)
{
  auto start = std::chrono::system_clock::now();
  l.erase(l.begin(), l.end());
  auto end = std::chrono::system_clock::now();

  std::chrono::duration<double> elapsedSecondsL = end - start;
  std::time_t end_timeL = std::chrono::system_clock::to_time_t(end);

  std::cout << "Finished ERASE for list - " << count << " elements, time : " << elapsedSecondsL.count() << ",\n at time : " << std::ctime(&end_timeL) << std::endl;

  start = std::chrono::system_clock::now();
  v.erase(v.begin(), v.end());
  end = std::chrono::system_clock::now();

  std::chrono::duration<double> elapsedSecondsV = end - start;
  std::time_t end_timeV = std::chrono::system_clock::to_time_t(end);

    std::cout << "Finished ERASE for vector - " << count << " elements, time : " << elapsedSecondsV.count() << ",\n at time : " << std::ctime(&end_timeV) << std::endl;

}

  ListCollection <int> list;
  VectorCollection <int> vec;

int main()
{

  for(int i : {100 , 1000, 10000})
  {
    std::cout << "P- + - + - + - + - + - + - + - + - + " << i << " + - + - + - + - + - + - + - + - + -P\n";
    prependTest(list, vec, i);
    popLastTest(list, vec, i);
	  eraseTest(list, vec, i);
    appendTest(list, vec, i);
    popFirstTest(list, vec, i);
	  eraseTest(list, vec, i);
  }

  return 0;
}
