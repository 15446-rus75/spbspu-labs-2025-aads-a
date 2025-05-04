#ifndef PROCESSINGDICTIONARY_HPP
#define PROCESSINGDICTIONARY_HPP
#include <iostream>
#include <map>
#include <string>
#include <list>

namespace zakirov
{
  template < class K, class T >
  void execute_command(std::istream & in, std::ostream & out, std::map< std::string, std::map< K, T > > & dictionary);
  template < class K, class T >
  void print_d(std::ostream & out, std::string name, const std::map< K, T > & dictionary);
  template < class K, class T >
  void complement_d(std::map< K, T > & result_d, const std::map< K, T > & first_d, const std::map< K, T > & second_d);
  template < class K, class T >
  void intersect_d(std::map< K, T > & result_d, const std::map< K, T > & first_d, const std::map< K, T > & second_d);
  template < class K, class T >
  void union_d(std::map< K, T > & result_d, const std::map< K, T > & first_d, const std::map< K, T > & second_d);

  template < class K, class T >
  void execute_command(std::istream & in, std::ostream & out, std::map< std::string, std::map< K, T > > & dictionary)
  {
    std::string command;
    in >> command;
    if (command == "print")
    {
      std::string dataset;
      in >> dataset;
      zakirov::print_d(out, dataset, dictionary[dataset]);
    }
    else if (command == "complement")
    {
      std::pair< std::string, std::map< K, T > > dataset_new;
      std::string dataset_1;
      std::string dataset_2;
      in >> dataset_new.first >> dataset_1 >> dataset_2;
      complement_d(dataset_new.second, dictionary[dataset_1], dictionary[dataset_2]);
      auto iterator_new = dictionary.find(dataset_new.first);
      if (iterator_new != dictionary.end())
      {
        dictionary.erase(iterator_new);
      }

      dictionary.insert(dataset_new);
    }
    else if (command == "intersect")
    {
      std::pair< std::string, std::map< K, T > > dataset_new;
      std::string dataset_1;
      std::string dataset_2;
      in >> dataset_new.first >> dataset_1 >> dataset_2;
      intersect_d(dataset_new.second, dictionary[dataset_1], dictionary[dataset_2]);
      auto iterator_new = dictionary.find(dataset_new.first);
      if (iterator_new != dictionary.end())
      {
        dictionary.erase(iterator_new);
      }

      dictionary.insert(dataset_new);
    }
    else if (command == "union")
    {
      std::pair< std::string, std::map< K, T > > dataset_new;
      std::string dataset_1;
      std::string dataset_2;
      in >> dataset_new.first >> dataset_1 >> dataset_2;
      union_d(dataset_new.second, dictionary[dataset_1], dictionary[dataset_2]);
      auto iterator_new = dictionary.find(dataset_new.first);
      if (iterator_new != dictionary.end())
      {
        dictionary.erase(iterator_new);
      }

      dictionary.insert(dataset_new);
    }
    else if (in.eof())
    {
      return;
    }
    else
    {
      throw std::logic_error("<INVALID ARGUMENT>");
    }
  }

  template < class K, class T >
  void print_d(std::ostream & out, std::string name, const std::map< K, T > & dictionary)
  {
    if (dictionary.empty())
    {
      throw std::invalid_argument("<EMPTY>");
    }

    out << name;
    auto iter = dictionary.cbegin();
    for (; iter != dictionary.cend(); ++iter)
    {
      out << ' ' << iter->first << ' ' << iter->second;
    }

    out << '\n';
  }

  template < class K, class T >
  void complement_d(std::map< K, T > & result_d, const std::map< K, T > & first_d, const std::map< K, T > & second_d)
  {
    auto first_iter = first_d.cbegin();
    auto second_iter = second_d.cbegin();

    while (first_iter != first_d.cend() && second_iter != second_d.cend())
    {
      if (first_iter->first < second_iter->first)
      {
        result_d.insert({first_iter->first, first_iter->second});
        ++first_iter;
      }
      else if (first_iter->first > second_iter->first)
      {
        result_d.insert({second_iter->first, second_iter->second});
        ++second_iter;
      }
      else
      {
        ++second_iter;
        ++first_iter;
      }
    }

    while (first_iter != first_d.cend())
    {
      result_d.insert({first_iter->first, first_iter->second});
      ++first_iter;
    }

    while (second_iter != second_d.cend())
    {
      result_d.insert({second_iter->first, second_iter->second});
      ++second_iter;
    }
  }

  template < class K, class T >
  void intersect_d(std::map< K, T > & result_d, const std::map< K, T > & first_d, const std::map< K, T > & second_d)
  {
    auto first_iter = first_d.cbegin();
    auto second_iter = second_d.cbegin();

    while (first_iter != first_d.cend() && second_iter != second_d.cend())
    {
      if (first_iter->first < second_iter->first)
      {
        ++first_iter;
      }
      else if (first_iter->first > second_iter->first)
      {
        ++second_iter;
      }
      else
      {
        result_d.insert({first_iter->first, first_iter->second});
        ++second_iter;
        ++first_iter;
      }
    }
  }

  template < class K, class T >
  void union_d(std::map< K, T > & result_d, const std::map< K, T > & first_d, const std::map< K, T > & second_d)
  {
    auto first_iter = first_d.cbegin();
    auto second_iter = second_d.cbegin();

    while (first_iter != first_d.cend() && second_iter != second_d.cend())
    {
      if (first_iter->first < second_iter->first)
      {
        result_d.insert({first_iter->first, first_iter->second});
        ++first_iter;
      }
      else if (first_iter->first > second_iter->first)
      {
        if (second_iter->first != first_iter->first)
        {
          result_d.insert({second_iter->first, second_iter->second});
        }

        ++second_iter;
      }
      else if (first_iter->first == second_iter->first)
      {
        result_d.insert({first_iter->first, first_iter->second});
        ++second_iter;
        ++first_iter;
      }
    }

    while (first_iter != first_d.cend())
    {
      result_d.insert({first_iter->first, first_iter->second});
      ++first_iter;
    }

    while (second_iter != second_d.cend())
    {
      result_d.insert({second_iter->first, second_iter->second});
      ++second_iter;
    }
  }
}

#endif
