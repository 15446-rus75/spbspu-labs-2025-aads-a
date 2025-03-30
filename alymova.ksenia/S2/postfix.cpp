#include "postfix.hpp"
#include <string>
#include "postfixProcess.hpp"
#include "stack.hpp"
#include "queue.hpp"

alymova::Postfix::Postfix(const std::string& s):
  postfix_()
{
  Stack< std::string > stack;
  std::string s_local = s;
  while (!s_local.empty())
  {
    size_t space = s_local.find(" ");
    std::string now;
    if (space == std::string::npos)
    {
      now = s_local;
      space = s_local.size() - 1;
    }
    else
    {
      now = s_local.substr(0, space);
    }
    s_local = s_local.substr(space + 1);

    if (now == "(")
    {
      stack.push(now);
    }
    else if (now == ")")
    {
      while (stack.top() != "(")
      {
        postfix_.push(stack.top());
        stack.pop();
        if (stack.empty())
        {
          throw std::logic_error("Incorrect expression");
        }
      }
      stack.pop();
    }
    else if (my_isdigit(now))
    {
      postfix_.push(now);
    }
    else if (is_first_priority(now) || is_second_priority(now))
    {
      if (!stack.empty())
      {
        while (is_need_priority(now, stack.top()))
        {
          postfix_.push(stack.top());
          stack.pop();
          if (stack.empty())
          {
            break;
          }
        }
      }
      stack.push(now);
    }
    else
    {
      throw std::logic_error("Incorrect char");
    }
  }
  while (!stack.empty())
  {
    if (stack.top() == "(")
    {
      throw std::logic_error("Incorrect expression");
    }
    postfix_.push(stack.top());
    stack.pop();
  }
}
long long int alymova::Postfix::operator()()
{
  Stack< long long int > stack;
  while (!postfix_.empty())
  {
    if (my_isdigit(postfix_.front()))
    {
      stack.push(std::stoll(postfix_.front()));
    }
    else
    {
      if (stack.size() < 2)
      {
        throw std::logic_error("Incorrect expression");
      }
      long long int item2 = stack.top();
      stack.pop();
      long long int item1 = stack.top();
      stack.pop();
      switch ((postfix_.front())[0])
      {
      case '+':
        if (is_overflow_addition(item1, item2))
        {
          throw std::overflow_error("Addition overflow");
        }
        stack.push(item1 + item2);
        break;
      case '-':
        if (is_overflow_substraction(item1, item2))
        {
          throw std::overflow_error("Substraction overflow");
        }
        stack.push(item1 - item2);
        break;
      case '*':
        if (is_overflow_multi(item1, item2))
        {
          throw std::overflow_error("Multiplication overflow");
        }
        stack.push(item1 * item2);
        break;
      case '/':
        if (item2 == 0)
        {
          throw std::logic_error("Division by 0");
        }
        stack.push(item1 / item2);
        break;
      case '%':
        if (item2 == 0)
        {
          throw std::logic_error("Division by 0");
        }
        stack.push(my_mod(item1, item2));
        break;
      }
    }
    postfix_.pop();
  }
  if (stack.size() > 1)
  {
    throw std::logic_error("Incorrect expression");
  }
  return stack.top();
}
long long int alymova::Postfix::operator+(const Postfix& other)
{
  Postfix copy(*this);
  copy.push_operator(other, "+");
  return copy();
}
long long int alymova::Postfix::operator-(const Postfix& other)
{
  Postfix copy(*this);
  copy.push_operator(other, "-");
  return copy();
}
long long int alymova::Postfix::operator*(const Postfix& other)
{
  Postfix copy(*this);
  copy.push_operator(other, "*");
  return copy();
}
long long int alymova::Postfix::operator/(const Postfix& other)
{
  Postfix copy(*this);
  copy.push_operator(other, "/");
  return copy();
}
long long int alymova::Postfix::operator%(const Postfix& other)
{
  Postfix copy(*this);
  copy.push_operator(other, "%");
  return copy();
}
void alymova::Postfix::push_operator(Postfix other, std::string operation)
{
  while (!other.postfix_.empty())
  {
    postfix_.push(other.postfix_.front());
    other.postfix_.pop();
  }
  postfix_.push(operation);
}
