#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include "graph.hpp"
#include <istream>
#include <map>
#include <string>
#include <ostream>
namespace kiselev
{
  using Graphs = std::map< std::string, Graph >;
  void graphs(std::ostream&, const Graphs&);
  void vertexes(std::ostream&, std::istream&, const Graphs&);
  void outBound(std::ostream&, std::istream&, const Graphs&);
  void inBound(std::ostream&, std::istream&, const Graphs&);
  void bind(std::istream&, Graphs&);
  void cut(std::istream&, Graphs&);
  void create(std::istream&, Graphs&);
  void merge(std::istream&, Graphs&);
  void extract(std::istream&, Graphs&);
}
#endif
