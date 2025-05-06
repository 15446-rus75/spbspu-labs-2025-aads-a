#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <map>
#include "commands.hpp"

namespace
{
  void inputDicts(std::istream & in, std::map< std::string, std::map< int, std::string > > & dicts)
  {
    while (!in.eof())
    {
      std::map< int, std::string > temp;
      std::string name;
      in >> name;
      int key = 0;
      std::string value;
      while ((in >> key) && (in >> value))
      {
        temp.insert(std::make_pair(key, value));
      }
      dicts.insert(std::make_pair(name, temp));
    }
  }
}

int main(int argc, char * argv[])
{
  using namespace mozhegova;
  if (argc != 2)
  {
    std::cerr << "invalid arguments\n";
    return 1;
  }
  std::ifstream file(argv[1]);
  if (!file.is_open())
  {
    std::cerr << "file isn't open\n";
    return 1;
  }
  std::map< std::string, std::map< int, std::string > > dicts;
  inputDicts(file, dicts);
  std::string command;
  while ((!std::cin.eof()) && (std::cin >> command))
  {
    try
    {
      if (command == "print")
      {
        std::string name;
        std::cin >> name;
        print(std::cout, name, dicts);
      }
      else if (command == "comlement")
      {
        std::string newDict, dict1, dict2;
        std::cin >> newDict >> dict1 >> dict2;
        comlement(newDict, dict1, dict2, dicts);
      }
      else if (command == "intersect")
      {
        std::string newDict, dict1, dict2;
        std::cin >> newDict >> dict1 >> dict2;
        intersect(newDict, dict1, dict2, dicts);
      }
      else if (command == "union")
      {
        std::string newDict, dict1, dict2;
        std::cin >> newDict >> dict1 >> dict2;
        unionCmd(newDict, dict1, dict2, dicts);
      }
      else
      {
        std::cout << "<INVALID COMMAND>\n";
      }
    }
    catch (const std::exception & e)
    {
      std::cerr << e.what() << '\n';
      return 1;
    }
    std::cin.clear();
    std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
  }
}
