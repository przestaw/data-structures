#include <cstddef>
#include <cstdlib>
#include <string>

#include <chrono>

#include "TreeMap.h"
#include "HashMap.h"

using associative::HashMap;
using associative::TreeMap;

void find_test(HashMap<int, std::string> &h, TreeMap<int, std::string> &t, int count)
{
  auto start = std::chrono::system_clock::now();
  for(int i = count; i > 0; --i)
  {
    h.find(i);
  }
  auto end = std::chrono::system_clock::now();

  std::chrono::duration<double> elapsed_seconds_H = end - start;
  std::time_t end_time_H = std::chrono::system_clock::to_time_t(end);

  std::cout << "Finished find for hash_map - " << count
            << " elements, time : " << elapsed_seconds_H.count() << " s"
            << "  - at time : " << std::ctime(&end_time_H)
            << "Avg time = " << elapsed_seconds_H.count()/count << " s\n\n";

  start = std::chrono::system_clock::now();
  for(int i = count; i > 0; --i)
  {
    t.find(i);
  }
  end = std::chrono::system_clock::now();

  std::chrono::duration<double> elapsed_seconds_T = end - start;
  std::time_t end_time_T = std::chrono::system_clock::to_time_t(end);

  std::cout << "Finished find for tree_map - " << count
            << " elements, time : " << elapsed_seconds_T.count() << " s"
            << "  - at time : " << std::ctime(&end_time_T)
            << "Avg time = " << elapsed_seconds_T.count()/count << " s\n\n";

}

void operator_test(HashMap<int, std::string> &h, TreeMap<int, std::string> &t, int count)
{
  auto start = std::chrono::system_clock::now();
  for(int i = count; i > 0; --i)
  {
    h[i] = "Steffen";
  }
  auto end = std::chrono::system_clock::now();

  std::chrono::duration<double> elapsed_seconds_H = end - start;
  std::time_t end_time_H = std::chrono::system_clock::to_time_t(end);

  std::cout << "Finished operator[] for hash_map - " << count
            << " elements, time : " << elapsed_seconds_H.count() << " s"
            << "  - at time : " << std::ctime(&end_time_H)
            << "Avg time = " << elapsed_seconds_H.count()/count << " s\n\n";

  start = std::chrono::system_clock::now();
  for(int i = count; i > 0; --i)
  {
    t[i] = "Steffen";
  }
  end = std::chrono::system_clock::now();

  std::chrono::duration<double> elapsed_seconds_T = end - start;
  std::time_t end_time_T = std::chrono::system_clock::to_time_t(end);

  std::cout << "Finished operator[] for tree_map - " << count
            << " elements, time : " << elapsed_seconds_T.count() << " s"
            << "  - at time : " << std::ctime(&end_time_T)
            << "Avg time = " << elapsed_seconds_T.count()/count << " s\n\n";

}

void copy_test(HashMap<int, std::string> &h, TreeMap<int, std::string> &t, int count)
{
  auto start = std::chrono::system_clock::now();
  HashMap<int, std::string> copy_h(h);
  auto end = std::chrono::system_clock::now();

  std::chrono::duration<double> elapsed_seconds_H = end - start;
  std::time_t end_time_H = std::chrono::system_clock::to_time_t(end);

  std::cout << "Finished copy for hash_map - " << count
          << " elements, at time : " << std::ctime(&end_time_H)
          << "Time : " << elapsed_seconds_H.count() << " s\n\n";


  start = std::chrono::system_clock::now();
  TreeMap<int, std::string> copy_t(t);
  end = std::chrono::system_clock::now();

  std::chrono::duration<double> elapsed_seconds_T = end - start;
  std::time_t end_time_T = std::chrono::system_clock::to_time_t(end);

  std::cout << "Finished copy for tree_map - " << count
            << " elements, at time : " << std::ctime(&end_time_T)
            << "Time : " << elapsed_seconds_T.count() << " s\n\n";

}

void remove_test(HashMap<int, std::string> &h, TreeMap<int, std::string> &t, int count)
{
  auto start = std::chrono::system_clock::now();
  for(int i = 0; i > 0; --i)
  {
    h.remove(i);
  }
  auto end = std::chrono::system_clock::now();

  std::chrono::duration<double> elapsed_seconds_H = end - start;
  std::time_t end_time_H = std::chrono::system_clock::to_time_t(end);

  std::cout << "Finished remove for hash_map - " << count
            << " elements, time : " << elapsed_seconds_H.count() << " s"
            << "  - at time : " << std::ctime(&end_time_H)
            << "Avg time = " << elapsed_seconds_H.count()/count << " s\n\n";

  start = std::chrono::system_clock::now();
  for(int i = 1; i <= count; ++i)
  {
    t.remove(i);
  }
  end = std::chrono::system_clock::now();

  std::chrono::duration<double> elapsed_seconds_T = end - start;
  std::time_t end_time_T = std::chrono::system_clock::to_time_t(end);

  std::cout << "Finished remove for tree_map - " << count
            << " elements, time : " << elapsed_seconds_T.count() << " s"
            << "  - at time : " << std::ctime(&end_time_T)
            << "Avg time = " << elapsed_seconds_T.count()/count << " s\n\n";

}
/*
HashMap<int, std::string> hash(75000);
TreeMap<int, std::string> tree;
*/
int main()
{
  for(int i : {100, 1000, 10000, 100000, 1000000, 10000000})
  {
    HashMap<int, std::string> hash(3*i);//def hash is 3 times i - HashMap should be really fast for ~30% of usage
    TreeMap<int, std::string> tree;
    std::cout << i << " - + - + - + - + - + - + - + - + - + " << i << " + - + - + - + - + - + - + - + - + - " << i << "\n";
    operator_test(hash, tree, i);
    find_test(hash, tree, i);
    copy_test(hash, tree, i);
    remove_test(hash, tree, i);
  }

  for(int i : {100, 1000, 10000, 100000, 1000000, 10000000})
  {
    HashMap<int, std::string> hash(i/5);//
    TreeMap<int, std::string> tree;
    std::cout << i << " | | | | | | | | | | | | | | | | | |  " << i << " | | | | | | | | | | | | | | | | | |  " << i << "\n";
    operator_test(hash, tree, i);
    find_test(hash, tree, i);
    copy_test(hash, tree, i);
    remove_test(hash, tree, i);
  }
  return 0;
}