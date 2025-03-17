#include <exception>
#include <fstream>
#include <iostream>
#include <istream>
#include <pthread.h>
#include "actionsExpr.hpp"

int main(int argc, char* argv[])
{
  try
  {
    kiselev::queue infixExprs;
    //if (argc < 1)
    //{
    //  kiselev::inputExprs(std::cin, infixExprs);
    //}
    if (argc > 1)
    {
      std::ifstream file(argv[1]);
      kiselev::inputExprs(file, infixExprs);
    }
    else
    {
      kiselev::inputExprs(std::cin, infixExprs);
    }
    kiselev::queue postfixExprs = kiselev::convertExpr(infixExprs);
    kiselev::stackNumber results = kiselev::calculationExpr(postfixExprs);
    kiselev::outputResults(std::cout, results) << "\n";
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << "\n";
    return 1;
  }
}
