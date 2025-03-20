//#include "postfix_infix_exprs.cpp"
#include <iostream>
#include <string>
#include <sstream>
#include <limits>
#include "stack.hpp"
#include "queue.hpp"

namespace abramov
{
  std::istream &calcInfix(std::istream &in);
}

namespace
{
  constexpr long long int max = std::numeric_limits< long long int >::max();
  constexpr long long int min = std::numeric_limits< long long int >::min();

  long long int *expArray(long long int *data, size_t &k)
  {
    long long int *array = new long long int[k * 2];
    for (size_t i = 0; i < k; ++i)
    {
      array[i] = data[i];
    }
    k *= 2;
    return array;
  }

  bool isNumber(const std::string &s)
  {
    std::stoll(s);
    for (auto it = s.begin(); it != s.end(); ++it)
    {
      if (!std::isdigit(*it))
      {
        throw std::logic_error("Not a number\n");
      }
    }
    return true;
  }

  abramov::Queue< std::string > *getPostfix(const std::string &s)
  {
    abramov::Queue< std::string > queue_infix;
    std::stringstream ss(s);
    std::string token;
    while (ss >> token)
    {
      queue_infix.push(token);
    }
    abramov::Stack< std::string > stack;
    abramov::Queue< std::string > *queue_postfix = new abramov::Queue< std::string >;
    bool bracket = false;
    while(!queue_infix.empty())
    {
      std::string temp = *queue_infix.front();
      queue_infix.pop();
      if (temp == "(")
      {
        stack.push("(");
        bracket = true;
      }
      else if (temp == ")")
      {
        if (!bracket)
        {
          throw std::logic_error("Unbalanced brackets\n");
        }
        while (*stack.top() != "(")
        {
          queue_postfix->push(*stack.top());
          stack.pop();
        }
        stack.pop();
        bracket = false;
      }
      else if (isNumber(temp))
      {
        queue_postfix->push(temp);
      }
      else if (temp == "+" || temp == "-")
      {
        if (!stack.empty())
        {
          if (*stack.top() != "(")
          {
            queue_postfix->push(*stack.top());
            stack.pop();
          }
        }
        stack.push(temp);
      }
      else if (temp == "*" || temp == "/" || temp == "%")
      {
        if (!stack.empty())
        {
          if (*stack.top() == "*" || *stack.top() == "/" || *stack.top() == "%")
          {
            queue_postfix->push(*stack.top());
            stack.pop();
          }
        }
        stack.push(temp);
      }
      else
      {
        throw std::logic_error("Not supported symbol\n");
      }
    }
    while (!stack.empty())
    {
      queue_postfix->push(*stack.top());
    }
    if (bracket)
    {
      throw std::logic_error("Unbalanced brackets\n");
    }
    return queue_postfix;
  }

  int sign(long long int a)
  {
    if (a > 0)
    {
      return 1;
    }
    else if (a < 0)
    {
      return -1;
    }
    else
    {
      return 0;
    }
  }

  bool same_sign(long long int a, long long int b)
  {
    return sign(a) * sign(b) > 0;
  }

  long long int sum(long long int a, long long int b)
  {
    if (same_sign(a, b) && a > 0 && max - a >= b)
    {
      return a + b;
    }
    else if (same_sign(a, b) && a < 0 && min - a <= b)
    {
      return a + b;
    }
    else if (a == 0 || b == 0)
    {
      return a + b;
    }
    else if (!same_sign(a, b))
    {
      return a + b;
    }
    throw std::logic_error("Overflow\n");
  }

  long long int diff(long long int a, long long int b)
  {
    if (same_sign(a, b) && a > 0 && min + b <= a)
    {
      return a - b;
    }
    else if (same_sign(a, b) && a < 0 && max - a >= b)
    {
      return a - b;
    }
    else if (a == 0 || b == 0)
    {
      return a - b;
    }
    else if (!same_sign(a, b))
    {
      return a - b;
    }
    throw std::logic_error("Overflow\n");
  }

  long long int prod(long long int a, long long int b)
  {
    if (same_sign(a, b) && a > 0 && max / a >= b)
    {
      return a * b;
    }
    else if (same_sign(a, b) && a < 0 && max / a >= std::abs(b))
    {
      return a * b;
    }
    else if (a == 0 || b == 0)
    {
      return 0ll;
    }
    else if (!same_sign(a, b) && std::abs(min / a) >= std::abs(b))
    {
      return a * b;
    }
    throw std::logic_error("Overflow\n");
  }

  long long int quot(long long int a, long long int b)
  {
    if (b == 0)
    {
      throw std::logic_error("Division by zero\n");
    }
    else if (a == 0)
    {
      return 0ll;
    }
    else if (same_sign(a, b) && a < 0 && a != min && b != -1ll)
    {
      return a / b;
    }
    else
    {
      return a / b;
    }
    throw std::logic_error("Overflow\n");
  }

  long long int doOperation(long long int a, long long int b, const std::string &op)
  {
    if (op == "+")
    {
      return sum(a, b);
    }
    else if (op == "-")
    {
      return diff(a, b);
    }
    else if (op == "*")
    {
      return prod(a, b);
    }
    else if (op == "/")
    {
      return quot(a, b);
    }
    else
    {
      return a % b;
    }
  }

  long long int calcPostfix(abramov::Queue< std::string > *queue)
  {
    abramov::Stack< long long int > stack;
    std::string temp;
    while(!queue->empty())
    {
      temp = *queue->front();
      queue->pop();
      if (isNumber(temp))
      {
        stack.push(std::stoll(temp));
      }
      else
      {
        long long int num_r = *stack.top();
        stack.pop();
        long long int num_l = *stack.top();
        stack.pop();
        stack.push(doOperation(num_l, num_r, temp));
      }
    }
    if (stack.size() != 1)
    {
      throw std::logic_error("Wrong expression\n");
    }
    return *stack.top();
  }

  long long int calcExpr(const std::string &s)
  {
    abramov::Queue< std::string > *postfix = getPostfix(s);
    return calcPostfix(postfix);
  }
}

std::istream &abramov::calcInfix(std::istream &in)
{
  size_t k = 10;
  size_t count = 0;
  long long int *results = new long long int[k];
  std::string s;
  while (in)
  {
    in >> s;
    if (s.empty())
    {
      continue;
    }
    if (count == k)
    {
      long long int *new_results = expArray(results, k);
      delete[] results;
      results = new_results;
    }
    try
    {
      results[count++] = calcExpr(s);
    }
    catch (...)
    {
      delete[] results;
      throw;
    }
  }
  if (count == 0)
  {
    return in;
  }
  for (size_t i = count - 1; i >= 1; --i)
  {
    std::cout << results[i] << " ";
  }
  std::cout << results[0] << "\n";
  delete[] results;
  return in;
}
