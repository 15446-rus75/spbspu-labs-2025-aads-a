#include <iostream>
#include <string>
#include <limits>
#include <list>

namespace
{
  using lop = std::list< std::pair< std::string, std::list< unsigned long long > > >;
  size_t getMax(const lop & listOfPairs)
  {
    size_t maxNums = 0;
    for (auto it = listOfPairs.begin(); it != listOfPairs.end(); ++it)
    {
      maxNums = std::max(maxNums, it->second.size());
    }
    return maxNums;
  }
  void printNums(std::ostream & out, const lop & listOfPairs)
  {
    for (size_t i = 0; i < getMax(listOfPairs); i++)
    {
      auto it = listOfPairs.begin();
      auto currIt = it->second.begin();
      if (it->second.size() > i)
      {
        std::advance(currIt, i);
        out << *currIt;
      }
      else
      {
        while (it->second.size() <= i)
        {
          ++it;
        }
        currIt = it->second.begin();
        std::advance(currIt, i);
        out << *currIt;
      }
      ++it;
      for (; it != listOfPairs.end(); ++it)
      {
        currIt = it->second.begin();
        if (it->second.size() > i)
        {
          std::advance(currIt, i);
          out << " " << *currIt;
        }
      }
      out << "\n";
    }
  }
  std::list< unsigned long long > calculateSum(const lop & listOfPairs)
  {
    std::list< unsigned long long > sumList;
    const unsigned long long max = std::numeric_limits< unsigned long long >::max();
    for (size_t i = 0; i < getMax(listOfPairs); i++)
    {
      unsigned long long sum = 0;
      auto it = listOfPairs.begin();
      for (; it != listOfPairs.end(); ++it)
      {
        auto currIt = it->second.begin();
        if (it->second.size() > i)
        {
          std::advance(currIt, i);
          if (max - *currIt < sum)
          {
            throw std::overflow_error("Overflow");
          }
          sum += *currIt;
        }
      }
      sumList.push_back(sum);
    }
    return sumList;
  }
  void printSums(std::ostream & out, const std::list< unsigned long long > sumList)
  {
    out << *sumList.begin();
    for (auto it = ++sumList.begin(); it != sumList.end(); ++it)
    {
      out << " " << *it;
    }
    out << "\n";
  }
}

int main()
{
  std::list< std::pair< std::string, std::list< unsigned long long > > > listOfPairs;
  std::string nameList;
  while (std::cin >> nameList)
  {
    std::list< unsigned long long > numList;
    unsigned long long num = 0;
    while (std::cin >> num)
    {
      numList.push_back(num);
    }
    listOfPairs.push_back(std::make_pair(nameList, numList));
    std::cin.clear();
  }

  if (listOfPairs.size() == 0)
  {
    std::cout << "0\n";
    return 0;
  }

  std::cout << listOfPairs.begin()->first;
  for (auto it = ++listOfPairs.begin(); it != listOfPairs.end(); ++it)
  {
    std::cout << " " << it->first;
  }
  std::cout << "\n";

  printNums(std::cout, listOfPairs);
  std::list< unsigned long long > sumList;
  try
  {
    sumList = calculateSum(listOfPairs);
  }
  catch (const std::overflow_error & e)
  {
    std::cerr << e.what() << "\n";
    return 1;
  }
  printSums(std::cout, sumList);
}
