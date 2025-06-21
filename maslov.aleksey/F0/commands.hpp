#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iosfwd>
#include <hashTable/hashTable.hpp>

namespace maslov
{
  using Dicts = HashTable< std::string, HashTable< std::string, int > >;
  void createDictionary(std::istream & in, Dicts & dicts);
  void showDictionary(std::ostream & out, const Dicts & dicts);
  void load(std::istream & in, Dicts & dicts);
  void loadFromFile(std::ifstream & file, const std::string & dictName, Dicts & dicts);
  void unionDictionary(std::istream & in, Dicts & dicts);
  void intersectDictionary(std::istream & in, Dicts & dicts);
  void copyDictionary(std::istream & in, Dicts & dicts);
  void addWord(std::istream & in, Dicts & dicts);
  void printSize(std::istream & in, std::ostream & out, const Dicts & dicts);
  void cleanWord(std::istream & in, Dicts & dicts);
  void cleanDictionary(std::istream & in, Dicts & dicts);
  void printTop(std::istream & in, std::ostream & out, const Dicts & dicts);
  void printRare(std::istream & in, std::ostream & out, const Dicts & dicts);
  void printFrequency(std::istream & in, std::ostream & out, const Dicts & dicts);
  void createWordRange(std::istream & in, Dicts & dicts);
  void printHelp(std::ostream & out);
}

#endif
