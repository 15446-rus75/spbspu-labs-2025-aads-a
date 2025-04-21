#include "convertExpressions.hpp"
#include <stdexcept>
#include <iostream>
#include <sstream>

namespace
{
  bool isExpression(std::string a)
  {
    return ((a[0] == '+' || a[0] == '-' || a[0] == '*' || a[0] == '/' || a[0] == '%' || a[0] == '^') && a.size() == 1);
  }

  bool isMorePriority(std::string a, std::string b)
  {
    if (b[0] == '^')
    {
      return false;
    }
    else if ((a[0] == '*' || a[0] == '/' || a[0] == '%') && (b[0] == '+' || b[0] == '-'))
    {
      return true;
    }
    else if (a[0] == '^' && b[0] != '^')
    {
      return true;
    }
    return false;
  }
}

void finaev::countFinalResults(std::istream& in)
{
  std::string str = "";
  finaev::Stack< long long > res;
  while (in)
  {
    std::getline(in, str);
    if (str.size() == 0)
    {
      continue;
    }
    finaev::Queue< std::string > inf;
    std::istringstream iss(str);
    std::string a;
    while (iss >> a)
    {
      inf.push(a);
    }
    finaev::Queue< std::string > post = fromInfToPost(inf);
    res.push(calculatePost(post));
  }
  if (res.isEmpty())
  {
    std::cout << "\n";
    return;
  }
  std::cout << res.top();
  res.pop();
  while (!res.isEmpty())
  {
    std::cout << " " << res.top();
    res.pop();
  }
  std::cout << "\n";
}

finaev::Queue< std::string > finaev::fromInfToPost(Queue< std::string >& inf)
{
  Stack< std::string > temp;
  Queue< std::string > res;
  bool isBracket = 0;
  while (!inf.isEmpty())
  {
    if (inf.top()[0] == '(' && inf.top().size() == 1)
    {
      isBracket = 1;
      temp.push(inf.top());
      inf.pop();
    }
    else if (isExpression(inf.top()))
    {
      while (!temp.isEmpty() && isExpression(temp.top()) && (isMorePriority(temp.top(), inf.top()) || (!isMorePriority(inf.top(), temp.top()))))
      {
        res.push(temp.top());
        temp.pop();
      }
      temp.push(inf.top());
      inf.pop();
    }
    else if (inf.top()[0] == ')' && inf.top().size() == 1)
    {
      isBracket = 0;
      while (temp.top()[0] != '(')
      {
        res.push(temp.top());
        temp.pop();
      }
      temp.pop();
      inf.pop();
    }
    else
    {
      res.push(inf.top());
      inf.pop();
    }
  }
  while (!temp.isEmpty())
  {
    res.push(temp.top());
    temp.pop();
  }
  if (isBracket)
  {
    throw std::logic_error("Incorrect Expression!\n");
  }
  return res;
}

long long finaev::calculatePost(finaev::Queue< std::string >& post)
{
  Stack< long long > res;
  while (!post.isEmpty())
  {
    if (isExpression(post.top()))
    {
      long long s = res.top();
      res.pop();
      long long f = res.top();
      res.pop();
      res.push(finaev::count(f, s, post.top()[0]));
      post.pop();
    }
    else
    {
      try
      {
        long long num = std::stoll(post.top(), nullptr, 10);
        res.push(num);
        post.pop();
      }
      catch (const std::out_of_range&)
      {
        throw std::overflow_error("Overflow\n");
      }
      catch(const std::invalid_argument&)
      {
        throw std::invalid_argument("invalid token\n");
      }
    }
  }
  return res.top();
}
