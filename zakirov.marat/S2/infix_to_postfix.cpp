#include "infix_to_postfix.hpp"
#include <cmath>
#include <cstddef>
#include <limits>
#include "stack.hpp"
#include "queue.hpp"
#include <iostream>

void zakirov::check_overflow(llint first, llint second, char oper)
{
  llint llint_max = std::numeric_limits< llint >::max();
  llint llint_min = std::numeric_limits< llint >::min();
  if (oper == '+')
  {
    if ((first < 0 && second < 0) && (llint_min - first > second))
    {
      throw std::overflow_error("Variable overflow");
    }
    else if ((first > 0 && second > 0) && (llint_max - first < second))
    {
      throw std::overflow_error("Variable overflow");
    }
  }
  else if (oper == '-')
  {
    if ((first < 0 && second > 0) && (llint_max + first < second))
    {
      throw std::overflow_error("Variable overflow");
    }
    else if ((first > 0 && second < 0) && (llint_min + first > second))
    {
      throw std::overflow_error("Variable overflow");
    }
  }
  else if (oper == '*' && (llint_max / std::abs(first) > std::abs(second)))
  {
    throw std::overflow_error("Variable overflow");
  }
}

bool zakirov::check_operand(const std::string & line)
{
  size_t dot_counter = 0;
  std::string::const_iterator fillable_it = line.cbegin();
  if ((*fillable_it == '-' || *fillable_it == '+') && line.size() != 1)
  {
    ++fillable_it;
  }

  for (; fillable_it != line.cend(); ++fillable_it)
  {
    if (*fillable_it == '.')
    {
      ++dot_counter;
    }
    else if (!isdigit(*fillable_it))
    {
      return 0;
    }
  }

  if (dot_counter > 1)
  {
    return 0;
  }

  return 1;
}

bool zakirov::check_operator(std::string symbol)
{
  constexpr char operators[5] = {'+', '-', '*', '/', '%'};
  for (size_t i = 0; i < 5; ++i)
  {
    if (symbol.front() == operators[i] && symbol.size() == 1)
    {
      return 1;
    }
  }

  return 0;
}

bool zakirov::check_priority(std::string symbol)
{
  if (symbol == "*" || symbol == "/")
  {
    return 1;
  }

  return 0;
}

zakirov::Queue< std::string > zakirov::transform_to_postfix(Queue< std::string > infix)
{
  Stack< std::string > op_buffer;
  Queue< std::string > result;

  while (!infix.empty())
  {
    std::string symbol = infix.front();
    infix.pop();
    if (check_operand(symbol))
    {
      result.push(symbol);
    }
    else if (check_operator(symbol))
    {
      while (!op_buffer.empty() && check_priority(symbol) <= check_priority(op_buffer.top()))
      {
        result.push(op_buffer.top());
        op_buffer.pop();
      }

      op_buffer.push(symbol);
    }
    else if (symbol == "(")
    {
      op_buffer.push(symbol);
    }
    else if (symbol == ")")
    {
      while (op_buffer.top() != "(" && !op_buffer.empty())
      {
        result.push(op_buffer.top());
        op_buffer.pop();
      }

      if (!op_buffer.empty())
      {
        op_buffer.pop();
      }
      else
      {
        throw std::invalid_argument("Incorrect placement of brackets");
      }
    }
    else
    {
      throw std::invalid_argument("Incorrect expression");
    }
  }

  while (!op_buffer.empty())
  {
    if (op_buffer.top() != "(")
    {
      result.push(op_buffer.top());
      op_buffer.pop();
    }
    else
    {
      throw std::invalid_argument("Incorrect placement of brackets");
    }
  }

  return result;
}

zakirov::llint zakirov::transform_to_llint(const std::string & line)
{
  Stack< char > num_buffer;
  bool flag = false;
  std::string::const_iterator fillable_it = line.cbegin();
  if (*fillable_it == '-')
  {
    flag = true;
    ++fillable_it;
  }

  llint result = 0;
  for (; fillable_it != line.end(); ++fillable_it)
  {
    num_buffer.push(*fillable_it);
  }

  size_t size_b = num_buffer.size();
  llint num_power = 1;
  if (flag)
  {
    num_power = -1;
  }

  for (size_t i = 0; i < size_b; ++i, num_power *= 10)
  {
    result += (num_buffer.top() - '0') * num_power;
    num_buffer.pop();
  }

  return result;
}

zakirov::llint zakirov::calculate_postfix(llint first, llint second, char oper)
{
  if (oper == '/')
  {
    return second / first;
  }
  else if (oper == '*')
  {
    check_overflow(second, first, oper);
    return second * first;
  }
  else if (oper == '-')
  {
    check_overflow(second, first, oper);
    return second - first;
  }
  else if (oper == '+')
  {
    check_overflow(second, first, oper);
    return second + first;
  }

  return std::abs(second) % std::abs(first);
}

zakirov::llint zakirov::calculate_postfix_expression(Queue< std::string > postfix)
{
  Stack< llint > result;
  llint first_v = 0;
  llint second_v = 0;
  llint result_v = 0;
  while (!postfix.empty())
  {
    if (check_operand(postfix.front()))
    {
      result.push(transform_to_llint(postfix.front()));
      postfix.pop();
    }
    else if (check_operator(postfix.front()))
    {
      first_v = result.top();
      result.pop();
      second_v = result.top();
      result.pop();
      result_v = calculate_postfix(first_v, second_v, postfix.front().front());
      result.push(result_v);
      postfix.pop();
    }
  }

  if (result.size() != 1)
  {
    throw std::invalid_argument("Incorrect expression");
  }

  return result.top();
}
