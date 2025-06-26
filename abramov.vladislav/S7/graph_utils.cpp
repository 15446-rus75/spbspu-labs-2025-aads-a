#include "graph_utils.hpp"

namespace
{
  using namespace abramov;

  using Arr = const SimpleArray< std::string >;
  using HashT = HashTable< std::string, SimpleArray< size_t > >;

  void printVertsWeights(Arr &vert_names, HashT &dest_verts, std::ostream &out)
  {
    for (size_t i = 0; i < vert_names.size(); ++i)
    {
      const std::string &vert = vert_names[i];
      out << vert;
      SimpleArray< size_t > &weights = dest_verts.find(vert)->second;
      if (weights.size() > 0)
      {
       std::sort(&weights[0], &weights[0] + weights.size());
      }
      for (size_t j = 0; j < weights.size(); ++j)
      {
        out << " " << weights[j];
      }
      out << "\n";
    }
  }
}

std::ifstream &abramov::readGraphs(std::ifstream &in, GraphCollection &collect)
{
  std::string name;
  size_t edges_count = 0;
  while(!in.eof())
  {
    if(!(in >> name >> edges_count))
    {
      break;
    }
    Graph graph(name);
    for (size_t i = 0; i < edges_count; ++i)
    {
      std::string vert_a;
      std::string vert_b;
      size_t w = 0;
      in >> vert_a >> vert_b >> w;
      graph.addEdge(vert_a, vert_b, w);
    }
    collect.addGraph(graph);
  }
  return in;
}

void abramov::printVertsSorted(const GraphCollection &collect, const std::string &name, std::ostream &out)
{
  const Graph &graph = collect.cgetGraph(name);
  SimpleArray< std::string > verts;
  HashTable< std::string, bool > uniq_verts;
  for (auto it = graph.cbegin(); it != graph.cend(); ++it)
  {
    const std::string &vert = it->first;
    if (uniq_verts.cfind(vert) == uniq_verts.cend())
    {
      uniq_verts.insert(vert, true);
      verts.pushBack(vert);
    }
    const SimpleArray< Edge > *edges = graph.getEdges(vert);
    if (edges)
    {
      for (size_t i = 0; i < edges->size(); ++i)
      {
        const std::string &vert = (*edges)[i].to;
        if (uniq_verts.cfind(vert) == uniq_verts.cend())
        {
          uniq_verts.insert(vert, true);
          verts.pushBack(vert);
        }
      }
    }
  }
  if (!verts.empty())
  {
    std::sort(&verts[0], &verts[0] + verts.size());
  }
  for (size_t i = 0; i < verts.size(); ++i)
  {
    out << verts[i] << "\n";
  }
}

void abramov::printOutVerts(const GraphCollection &collect, const std::string &n, const std::string &v, std::ostream &out)
{
  const Graph &graph = collect.cgetGraph(n);
  const SimpleArray< Edge > *edges = graph.getEdges(v);
  if (!edges)
  {
    throw std::logic_error("There is no such vertex\n");
  }
  if (edges->empty())
  {
    return;
  }
  HashTable< std::string, SimpleArray< size_t > > dest_verts;
  for (size_t i = 0; i < edges->size(); ++i)
  {
    const Edge &edge = (*edges)[i];
    auto it = dest_verts.find(edge.to);
    if (it != dest_verts.end())
    {
      it->second.pushBack(edge.weight);
    }
    else
    {
      SimpleArray< size_t > weights;
      weights.pushBack(edge.weight);
      dest_verts.insert(edge.to, weights);
    }
  }
  SimpleArray< std::string > vert_names;
  for (auto it = dest_verts.begin(); it != dest_verts.end(); ++it)
  {
    vert_names.pushBack(it->first);
  }
  if (!vert_names.empty())
  {
    std::sort(&vert_names[0], &vert_names[0] + vert_names.size());
  }
  printVertsWeights(vert_names, dest_verts, out);
}

void abramov::printInVerts(const GraphCollection &collect, const std::string &n, const std::string &v, std::ostream &out)
{
  const Graph &graph = collect.cgetGraph(n);
  HashTable< std::string, SimpleArray< size_t > > src_verts;
  for (auto it = graph.cbegin(); it != graph.cend(); ++it)
  {
    const SimpleArray< Edge > *edges = graph.getEdges(it->first);
    for (size_t i = 0; i < edges->size(); ++i)
    {
      const Edge &edge = (*edges)[i];
      if (edge.to == v)
      {
        auto srcIt = src_verts.find(edge.from);
        if (srcIt != src_verts.end())
        {
          srcIt->second.pushBack(edge.weight);
        }
      }
      else
      {
        SimpleArray< size_t > weights;
        weights.pushBack(edge.weight);
        src_verts.insert(edge.from, weights);
      }
    }
  }
  SimpleArray< std::string > vert_names;
  for (auto it = src_verts.begin(); it != src_verts.end(); ++it)
  {
    vert_names.pushBack(it->first);
  }
  if (!vert_names.empty())
  {
    std::sort(&vert_names[0], &vert_names[0] + vert_names.size());
  }
  printVertsWeights(vert_names, src_verts, out);
}

void abramov::addEdgeToGraph(GraphCollection &collect, const std::string &name, std::istream &in)
{
  Graph &graph = collect.getGraph(name);
  std::string from;
  std::string to;
  size_t w;
  in >> from >> to >> w;
  if (in)
  {
    graph.addEdge(from, to, w);
  }
  else
  {
    throw std::logic_error("Invalid arguments\n");
  }
}

void abramov::cutEdgeFromGraph(GraphCollection &collect, const std::string &name, std::istream &in)
{
  Graph &graph = collect.getGraph(name);
  std::string from;
  std::string to;
  size_t w;
  if (!(in >> from >> to >> w))
  {
    throw std::logic_error("Invalid arguments\n");
  }
  if (!graph.cutEdge(from, to, w))
  {
    throw std::logic_error("There is no such edge\n");
  }
}

void abramov::mergeGraphs(GraphCollection &collect, const std::string &name, std::istream &in)
{
  std::string n1;
  std::string n2;
  in >> n1 >> n2;
  const Graph &graph1 = collect.cgetGraph(n1);
  const Graph &graph2 = collect.cgetGraph(n2);
  Graph res(n);
  for (auto it = graph1.cbegin(); it != graph1.cend(); ++it)
  {
    res.addVertex(it->first);
  }
  for (auto it = graph2.cbegin(); it != graph2.cend(); ++it)
  {
    res.addVertex(it->first);
  }
  for (auto it = graph1.cbegin(); it != graph1.cend(); ++it)
  {
    const SimpleArray< Edge > *edges1 = graph1.getEdges(it->first);
    if (edges)
    {
      for (size_t i = 0; i < edges->size(); ++i)
      {
        const Edge &edge = (*edges)[i];
        res.addEdge(edge.from, edge.to, edge.weight);
      }
    }
  }
  for (auto it = graph2.cbegin(); it != graph2.cend(); ++it)
  {
    const SimpleArray< Edge > *edges2 = graph2.getEdges(it->first);
    if (edges)
    {
      for (size_t i = 0; i < edges->size(); ++i)
      {
        const Edge &edge = (*edges)[i];
        res.addEdge(edge.from, edge.to, edge.weight);
      }
    }
  }
  collection.addGraph(res);
}
