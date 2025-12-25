#include "TFormula.hpp"
#include <iostream>
#include <string>

int main()
{
  std::string input;
  
  std::cout << "Введите формулу: ";
  std::getline(std::cin, input);
  
  try
  {
    TFormula<double> formula(input.c_str());
    
    TVector<int> brackets;
    int errors = formula.FormulaChecker(brackets);
    
    if (errors > 0)
    {
      std::cout << "Ошибка: неправильно расставлены скобки (" << errors << " ошибок)" << std::endl;
      return 1;
    }
    
    if (formula.FormulaConverter() != 0)
    {
      std::cout << "Ошибка: некорректная формула" << std::endl;
      return 1;
    }
    
    double result = formula.FormulaCalculator();
    std::cout << result << std::endl;
    
  }
  catch (const std::exception& e)
  {
    std::cout << "Ошибка: " << e.what() << std::endl;
    return 1;
  }
  
  return 0;
}