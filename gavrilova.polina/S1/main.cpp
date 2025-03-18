#include <string>
#include <iostream>
#include "inputList.hpp"
#include "outputList.hpp"
#include "FwdList.hpp"

int main() {
  using namespace gavrilova;
  using ULL = unsigned long long;

  FwdList< std::pair< std::string, FwdList< ULL > > > listOfPairs{};
  size_t numOfPairs = 0;
  std::string name;
  size_t maxLen = 0;
  while (std::cin >> name && !std::cin.eof()) {
    FwdList< ULL > numbers = inputNumbers(std::cin);
    maxLen = (maxLen < numbers.size()) ? numbers.size() : maxLen;
    listOfPairs.push_front({name, numbers});
    ++numOfPairs;
  }
  listOfPairs.reverse();

  if (numOfPairs) {
    outNames(std::cout, listOfPairs);
    std::cout << "\n";
  }

  FwdList< ULL > sums {};
  try {
    sums = outNumbers(std::cout, listOfPairs, maxLen, numOfPairs);
  } catch(const std::overflow_error& e) {
    std::cout << "\n";
    std::cerr << e.what();
    return 1;
  }

  if (maxLen == 0) {
    std::cout << "0\n";
  } else {
    outFwdListULL(std::cout, sums);
  }
}
