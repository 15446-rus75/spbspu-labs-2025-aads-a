#include <string>
#include <iostream>
#include <exception>
#include "utils.hpp"

void alymova::PrintCommand::operator()(const CompositeDataset_t& dicts)
{
  std::string name;
  in >> name;

  Dataset_t dataset = dicts.at(name);
  if (dataset.empty())
  {
    out << "<EMPTY>";
    return;
  }
  out << name;
  for (auto it = dataset.begin(); it != dataset.end(); it++)
  {
    out << ' ' << it->first << ' ' << it->second;
  }
}

void alymova::ComplementCommand::operator()(CompositeDataset_t& dicts)
{
  std::string newname, name1, name2;
  in >> newname >> name1 >> name2;

  Dataset_t dataset1 = dicts.at(name1);
  Dataset_t dataset2 = dicts.at(name2);

  for (auto it = dataset1.begin(); it != dataset1.end(); it++)
  {
    if (dataset2.find(it->first) == dataset2.end())
    {
      dataset2[it->first] = it->second;
    }
    else
    {
      dataset2.erase(it->first);
    }
  }
  dicts[newname] = dataset2;
}

void alymova::IntersectCommand::operator()(CompositeDataset_t& dicts)
{
  std::string newname, name1, name2;
  in >> newname >> name1 >> name2;

  Dataset_t dataset1 = dicts.at(name1);
  Dataset_t dataset2 = dicts.at(name2);
  for (auto it = dataset1.begin(); it != dataset1.end();)
  {
    if (dataset2.find(it->first) == dataset2.end())
    {
      it = dataset1.erase(it);
    }
    else
    {
      it++;
    }
  }
  dicts[newname] = dataset1;
}

void alymova::UnionCommand::operator()(CompositeDataset_t& dicts)
{
  std::string newname, name1, name2;
  in >> newname >> name1 >> name2;

  Dataset_t dataset1 = dicts.at(name1);
  Dataset_t dataset2 = dicts.at(name2);
  for (auto it = dataset2.begin(); it != dataset2.end(); it++)
  {
    if (dataset1.find(it->first) == dataset1.end())
    {
      dataset1[it->first] = it->second;
    }
  }
  dicts[newname] = dataset1;
}

alymova::CompositeDataset_t alymova::readDictionaryFile(std::istream& in)
{
  CompositeDataset_t dataset_comp;
  std::string name;
  while (in >> name)
  {
    Dataset_t dataset;
    size_t key;
    std::string value;
    while (in.peek() != '\n' && in)
    {
      in >> key >> value;
      dataset[key] = value;
    }
    dataset_comp[name] = dataset;
  }
  if ((in).fail() && !(in).eof())
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  return dataset_comp;
  /*std::string s_local = s;
  Dataset_t tree;
  if (s.empty())
  {
    return tree;
  }
  s_local = s_local.substr(1);

  while (!s_local.empty())
  {
    size_t tree_key;
    std::string tree_value;
    size_t space = s_local.find(" ");
    if (space != std::string::npos)
    {
      tree_key = std::stoll(s_local.substr(0, space));
    }
    s_local = s_local.substr(space + 1);

    space = s_local.find(" ");
    tree_value = s_local.substr(0, space);
    if (space == std::string::npos)
    {
      space = s_local.size() - 1;
    }
    s_local = s_local.substr(space + 1);
    tree[tree_key] = tree_value;
  }
  return tree;*/
}

alymova::CommandDataset_t alymova::complectCommands()
{
  return
  {
    {"print", PrintCommand{std::cin, std::cout}},
    {"complement", ComplementCommand{std::cin}},
    {"intersect", IntersectCommand{std::cin}},
    {"union", UnionCommand{std::cin}}
  };
}
