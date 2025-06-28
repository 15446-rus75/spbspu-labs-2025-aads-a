#include <iostream>
#include <fstream>
#include <avl_tree.hpp>
#include "traverse_info_handler.hpp"

namespace
{
  int checkArguments(const int argc);

  int checkArguments(const int argc)
  {
    if (argc == 3)
    {
      return 1;
    }
    else
    {
      return 0;
    }
  }
}

int main(const int argc, const char * const * argv)
{
  using namespace petrov;
  if (!checkArguments(argc))
  {
    std::cerr << "ERROR: Invalid number of command line arguments";
    std::cerr << '\n';
    return 1;
  }
  AVLTree< int, std::string > tree;
  std::ifstream in(argv[2]);
  while (!in.eof())
  {
    int key = 0;
    std::string val;
    if (in >> key >> val)
    {
      tree.insert({ key, val });
    }
  }
  SumUpKeysDisplayValues sum_and_display;
  std::string traverse_order(argv[1]);
  if (tree.empty())
  {
    std::cout << "<EMPTY>" << '\n';
  }
  if (traverse_order == "ascending")
  {
    SumUpKeysDisplayValues new_sum_and_display = tree.traverse_lnr(sum_and_display);
    std::cout << new_sum_and_display.result << ' ';
    new_sum_and_display.displayValuesInTraverseOrder(std::cout);
  }
  else if (traverse_order == "descending")
  {
    SumUpKeysDisplayValues new_sum_and_display = tree.traverse_rnl(sum_and_display);
    std::cout << new_sum_and_display.result << ' ';
    new_sum_and_display.displayValuesInTraverseOrder(std::cout);
  }
  else if (traverse_order == "breadth")
  {
    SumUpKeysDisplayValues new_sum_and_display = tree.traverse_breadth(sum_and_display);
    std::cout << new_sum_and_display.result << ' ';
    new_sum_and_display.displayValuesInTraverseOrder(std::cout);
  }
  std::cout << '\n';
}
