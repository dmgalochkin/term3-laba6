#pragma once
#include "TStack.hpp"
#include "TVector.hpp"
#include <cstring>
#include <string>
#include <cmath>
#include <stdexcept>

template<typename T>
class TFormula
{
private:
  TVector<char> formula;
  TVector<char> postfixForm;
  TStack<int> bracketStack;
  TStack<char> operatorStack;
  TStack<T> calcStack;

  int GetPriority(char op);
  bool IsOperator(char c);
  bool IsDigit(char c);
  bool IsBracket(char c);

public:
  TFormula(const char* form = nullptr);
  TFormula(const TFormula& other);
  TFormula(TFormula&& other);
  ~TFormula();

  void SetFormula(const char* form);
  std::string GetFormula() const;
  std::string GetPostfix() const;

  int FormulaChecker(TVector<int>& bracketPairs);

  int FormulaConverter();

  T FormulaCalculator();

  TFormula& operator=(const TFormula& other);
  TFormula& operator=(TFormula&& other);
};

template<typename T>
TFormula<T>::TFormula(const char* form) 
  : bracketStack(100), operatorStack(100), calcStack(100)
{
  if (form != nullptr) 
    SetFormula(form);
}

template<typename T>
TFormula<T>::TFormula(const TFormula& other)
  : formula(other.formula),
    postfixForm(other.postfixForm),
    bracketStack(other.bracketStack),
    operatorStack(other.operatorStack),
    calcStack(other.calcStack)
{
}

template<typename T>
TFormula<T>::TFormula(TFormula&& other)
  : formula(std::move(other.formula)),
    postfixForm(std::move(other.postfixForm)),
    bracketStack(std::move(other.bracketStack)),
    operatorStack(std::move(other.operatorStack)),
    calcStack(std::move(other.calcStack))
{
}

template<typename T>
TFormula<T>::~TFormula()
{
}

template<typename T>
void TFormula<T>::SetFormula(const char* form) 
{
  if (form == nullptr)
  {
    formula = TVector<char>(0);
    return;
  }

  int len = strlen(form);
  formula = TVector<char>(len);
  for (int i = 0; i < len; i++)
    formula[i] = form[i];

  postfixForm = TVector<char>(0);
}

template<typename T>
std::string TFormula<T>::GetFormula() const 
{
  std::string result;
  for (size_t i = 0; i < formula.GetSize(); i++)
    result.push_back(formula[i]);

  return result;
}

template<typename T>
std::string TFormula<T>::GetPostfix() const 
{
  std::string result;
  for (size_t i = 0; i < postfixForm.GetSize(); i++)
    result.push_back(postfixForm[i]);
  
  return result;
}

template<typename T>
int TFormula<T>::GetPriority(char op) 
{
  switch (op) 
  {
  case '(': return 0;
  case ')': return 1;
  case '+': return 2;
  case '-': return 2;
  case '*': return 3;
  case '/': return 3;
  case '^': return 4;
  default: return -1;
  }
}

template<typename T>
bool TFormula<T>::IsOperator(char c) 
{
  return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
}

template<typename T>
bool TFormula<T>::IsDigit(char c) 
{
  return (c >= '0' && c <= '9') || c == '.';
}

template<typename T>
bool TFormula<T>::IsBracket(char c)
{
  return c == '(' || c == ')';
}

template<typename T>
int TFormula<T>::FormulaChecker(TVector<int>& bracketPairs)
{
  int errorCount = 0;
  int formulaLen = formula.GetSize();

  bracketPairs = TVector<int>(formulaLen * 2);
  
  for (size_t i = 0; i < bracketPairs.GetSize(); i++)
    bracketPairs[i] = 0;

  int pairIndex = 0;

  while (!bracketStack.IsEmpty())
    bracketStack.Pop();

  for (int i = 0; i < formulaLen; i++)
  {
    char c = formula[i];

    if (c == '(')
      bracketStack.Push(i + 1);

    else if (c == ')')
    {
      if (!bracketStack.IsEmpty())
      {
        int openBracketPos = bracketStack.Top();
        bracketStack.Pop();
        if (pairIndex < (int)bracketPairs.GetSize() - 1)
        {
          bracketPairs[pairIndex++] = openBracketPos;
          bracketPairs[pairIndex++] = i + 1;
        }
      }
      else
      {
        if (pairIndex < (int)bracketPairs.GetSize() - 1)
        {
          bracketPairs[pairIndex++] = 0;
          bracketPairs[pairIndex++] = i + 1;
        }
        errorCount++;
      }
    }
  }

  while (!bracketStack.IsEmpty())
  {
    int openBracketPos = bracketStack.Top();
    bracketStack.Pop();
    if (pairIndex < (int)bracketPairs.GetSize() - 1)
    {
      bracketPairs[pairIndex++] = openBracketPos;
      bracketPairs[pairIndex++] = 0;
    }
    errorCount++;
  }

  return errorCount;
}

template<typename T>
int TFormula<T>::FormulaConverter() 
{
  TVector<int> bracketPairs;
  int errorCount = FormulaChecker(bracketPairs);

  if (errorCount > 0)
    return -1;

  postfixForm = TVector<char>(0);

  while (!operatorStack.IsEmpty())
    operatorStack.Pop();

  int formulaLen = formula.GetSize();
  bool lastWasDigit = false;

  for (int i = 0; i < formulaLen; i++)
  {
    char c = formula[i];

    if (IsDigit(c))
    {
      if (lastWasDigit)
        postfixForm.PushBack(std::move(c));

      else
      {
        if (postfixForm.GetSize() > 0 && postfixForm[postfixForm.GetSize() - 1] != ' ')
          postfixForm.PushBack(std::move(' '));

        postfixForm.PushBack(std::move(c));
      }
      lastWasDigit = true;
    }
    else if (c == '(')
    {
      operatorStack.Push(c);
      lastWasDigit = false;
    }
    else if (c == ')')
    {
      while (!operatorStack.IsEmpty() && operatorStack.Top() != '(')
      {
        char op = operatorStack.Top();
        operatorStack.Pop();
        postfixForm.PushBack(std::move(' '));
        postfixForm.PushBack(std::move(op));
      }

      if (!operatorStack.IsEmpty())
        operatorStack.Pop();

      lastWasDigit = false;
    }
    else if (IsOperator(c))
    {
      if (postfixForm.GetSize() > 0 && postfixForm[postfixForm.GetSize() - 1] != ' ')
        postfixForm.PushBack(std::move(' '));

      lastWasDigit = false;

      int currentPriority = GetPriority(c);

      while (!operatorStack.IsEmpty())
      {
        char topOp = operatorStack.Top();
        int topPriority = GetPriority(topOp);

        if (topPriority >= currentPriority && topOp != '(')
        {
          char op = operatorStack.Top();
          operatorStack.Pop();
          postfixForm.PushBack(std::move(' '));
          postfixForm.PushBack(std::move(op));
        }
        else
          break;
      }

      operatorStack.Push(c);
    }
    else if (c == ' ')
      continue;

    else
      return -1;
  }

  while (!operatorStack.IsEmpty())
  {
    char op = operatorStack.Top();
    operatorStack.Pop();
    postfixForm.PushBack(std::move(' '));
    postfixForm.PushBack(std::move(op));
  }

  return 0;
}

template<typename T>
T TFormula<T>::FormulaCalculator() 
{
  if (FormulaConverter() != 0)
    throw std::runtime_error("Ошибка в формуле");

  while (!calcStack.IsEmpty())
    calcStack.Pop();

  int postfixLen = postfixForm.GetSize();
  char* buffer = new char[postfixLen + 1];

  for (int i = 0; i < postfixLen; i++)
    buffer[i] = postfixForm[i];

  buffer[postfixLen] = '\0';

  char* token = strtok(buffer, " ");

  while (token != nullptr)
  {
    if (strlen(token) == 1 && IsOperator(token[0]))
    {
      if (calcStack.GetSize() < 2)
      {
        delete[] buffer;
        throw std::runtime_error("Недостаточно операндов для операции");
      }

      T operand2 = calcStack.Top();
      calcStack.Pop();
      T operand1 = calcStack.Top();
      calcStack.Pop();
      T result;

      switch (token[0])
      {
      case '+':
        result = operand1 + operand2;
        break;
      case '-':
        result = operand1 - operand2;
        break;
      case '*':
        result = operand1 * operand2;
        break;
      case '/':
        if (operand2 == static_cast<T>(0))
        {
          delete[] buffer;
          throw std::runtime_error("Деление на ноль");
        }
        result = operand1 / operand2;
        break;
      case '^':
        result = static_cast<T>(std::pow(static_cast<double>(operand1), static_cast<double>(operand2)));
        break;
      default:
        delete[] buffer;
        throw std::runtime_error("Неизвестный оператор");
      }

      calcStack.Push(result);
    }
    else
    {
      char* endptr;
      double value = strtod(token, &endptr);

      if (*endptr != '\0')
      {
        delete[] buffer;
        throw std::runtime_error("Некорректное число");
      }

      calcStack.Push(static_cast<T>(value));
    }

    token = strtok(nullptr, " ");
  }

  delete[] buffer;

  if (calcStack.GetSize() != 1)
    throw std::runtime_error("Ошибка в вычислениях");

  T result = calcStack.Top();
  calcStack.Pop();
  return result;
}

template<typename T>
TFormula<T>& TFormula<T>::operator=(const TFormula& other) 
{
  if (this != &other) 
  {
    formula = other.formula;
    postfixForm = other.postfixForm;
    bracketStack = other.bracketStack;
    operatorStack = other.operatorStack;
    calcStack = other.calcStack;
  }
  return *this;
}

template<typename T>
TFormula<T>& TFormula<T>::operator=(TFormula&& other) 
{
  if (this != &other) 
  {
    formula = std::move(other.formula);
    postfixForm = std::move(other.postfixForm);
    bracketStack = std::move(other.bracketStack);
    operatorStack = std::move(other.operatorStack);
    calcStack = std::move(other.calcStack);
  }
  return *this;
}