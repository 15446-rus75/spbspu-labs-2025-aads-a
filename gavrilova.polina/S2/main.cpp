#include <iostream>
#include <fstream>
#include <string>

#include "parse_expr.hpp"
#include "Queue.hpp"
#include "Stack.hpp"


int main(int argc, char* argv[])
{
  using namespace gavrilova;

  std::ifstream inputFile;
  if (argc > 1) {
    inputFile.open(argv[1]);
    if (!inputFile.is_open()) {
      std::cerr << "Error! Invalid openong file!" << "\n";
      return 1;
    }
  }
  std::istream& in = (argc > 1) ? inputFile : std::cin;
  std::string line;
  Stack< long long > results;
  while (getline(in, line)) {
    if (line.empty()) {
      continue;
    }
    try {
      Queue< std::string > infix_q = split(line, ' ');
      Queue< std:: string > postfix_q = infix_to_postfix(infix_q);
      results.push(calculate(postfix_q));
    } catch(const std::bad_alloc& e) {
      std::cerr << e.what() << "\n";
      return 1;
    } catch(const std::invalid_argument& e) {
      std::cerr << e.what() << "\n";
      return 1;
    }
  }

  while (!results.empty()) {
    std::cout << results.top();
    results.pop();
  }
}
