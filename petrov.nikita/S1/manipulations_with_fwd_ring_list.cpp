#include "manipulations_with_fwd_ring_list.hpp"
#include <limits>

std::istream & petrov::inputValuesIntoFwdRingList(std::istream & in, list_type & fwd_ring_list)
{
  std::string sequence_num = {};
  size_t number = 0;
  while (!in.eof())
  {
    in.clear();
    in >> sequence_num;
    petrov::ForwardRingList< size_t > sublist = {};
    while (!in.eof() && in)
    {
      in >> number;
      if (!in)
      {
        break;
      }
      sublist.push_front(number);
    }
    sublist.reverse();
    fwd_ring_list.push_front({ sequence_num, sublist });
  }
  fwd_ring_list.reverse();
  return in;
}

std::ostream & petrov::outputNamesOfSuquences(std::ostream & out, const list_type & fwd_ring_list)
{
  auto it = fwd_ring_list.cbegin();
  if (fwd_ring_list.size() == 1)
  {
    out << it->first;
  }
  else
  {
    out << (it++)->first;
    do
    {
      out << " " << it->first;
    }
    while (it++ != fwd_ring_list.cend());
  }
  out << "\n";
  return out;
}

petrov::result_list_type petrov::getListOfSequencesFromListOfSums(list_type fwd_ring_list)
{
  result_list_type sums = {};
  do
  {
    size_t sum = 0;
    auto prev_it_out = fwd_ring_list.end();
    auto it_out = fwd_ring_list.begin();
    do
    {
      if (it_out->second.empty() && fwd_ring_list.size() != 1)
      {
        auto val = *it_out;
        fwd_ring_list.remove(val);
        it_out = prev_it_out;
      }
      else if (it_out->second.empty() && fwd_ring_list.size() == 1)
      {
        fwd_ring_list.pop_front();
        break;
      }
      else
      {
        auto it = it_out->second.begin();
        if (it_out == fwd_ring_list.begin())
        {
          std::cout << *it;
          AddElementToSumAndPopFrontIt(sum, it_out, it);
        }
        else
        {
          std::cout << " " << *it;
          AddElementToSumAndPopFrontIt(sum, it_out, it);
        }
        ++prev_it_out;
      }
    }
    while (it_out++ != fwd_ring_list.end() && fwd_ring_list.begin() != fwd_ring_list.end());
    if (sum)
    {
      sums.push_front(sum);
      std::cout << "\n";
    }
  }
  while (!fwd_ring_list.empty());
  return sums;
}

void petrov::AddElementToSumAndPopFrontIt(size_t & sum, list_it_t it_out, sublist_it_t it)
{
  if (sum <= std::numeric_limits< size_t >::max() - *it)
  {
    sum += *it;
    it_out->second.pop_front();
  }
  else
  {
    throw std::out_of_range("ERROR: Overflow");
  }
}
