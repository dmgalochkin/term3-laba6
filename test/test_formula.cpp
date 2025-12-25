#include <gtest/gtest.h>
#include "TFormula.hpp"
#include "TVector.hpp"

TEST(TFormulaTest, SimpleAddition)
{
  char formula[] = "2+3";
  TFormula<double> f(formula);
  
  TVector<int> brackets;
  int errors = f.FormulaChecker(brackets);
  EXPECT_EQ(errors, 0);
  
  f.FormulaConverter();
  double result = f.FormulaCalculator();
  EXPECT_DOUBLE_EQ(result, 5.0);
}

TEST(TFormulaTest, SimpleMultiplication)
{
  char formula[] = "3*4";
  TFormula<double> f(formula);
  
  TVector<int> brackets;
  int errors = f.FormulaChecker(brackets);
  EXPECT_EQ(errors, 0);
  
  f.FormulaConverter();
  double result = f.FormulaCalculator();
  EXPECT_DOUBLE_EQ(result, 12.0);
}

TEST(TFormulaTest, OperatorPrecedence)
{
  char formula[] = "2+3*4";
  TFormula<double> f(formula);
  
  TVector<int> brackets;
  int errors = f.FormulaChecker(brackets);
  EXPECT_EQ(errors, 0);
  
  f.FormulaConverter();
  double result = f.FormulaCalculator();
  EXPECT_DOUBLE_EQ(result, 14.0);
}

TEST(TFormulaTest, WithParentheses)
{
  char formula[] = "(2+3)*4";
  TFormula<double> f(formula);
  
  TVector<int> brackets;
  int errors = f.FormulaChecker(brackets);
  EXPECT_EQ(errors, 0);
  
  f.FormulaConverter();
  double result = f.FormulaCalculator();
  EXPECT_DOUBLE_EQ(result, 20.0);
}

TEST(TFormulaTest, ComplexExpression)
{
  char formula[] = "(2+3)*4-1";
  TFormula<double> f(formula);
  
  TVector<int> brackets;
  int errors = f.FormulaChecker(brackets);
  EXPECT_EQ(errors, 0);
  
  f.FormulaConverter();
  double result = f.FormulaCalculator();
  EXPECT_DOUBLE_EQ(result, 19.0);
}

TEST(TFormulaTest, Division)
{
  char formula[] = "10/2";
  TFormula<double> f(formula);
  
  TVector<int> brackets;
  int errors = f.FormulaChecker(brackets);
  EXPECT_EQ(errors, 0);
  
  f.FormulaConverter();
  double result = f.FormulaCalculator();
  EXPECT_DOUBLE_EQ(result, 5.0);
}

TEST(TFormulaTest, DecimalNumbers)
{
  char formula[] = "1.5*2.5";
  TFormula<double> f(formula);
  
  TVector<int> brackets;
  int errors = f.FormulaChecker(brackets);
  EXPECT_EQ(errors, 0);
  
  f.FormulaConverter();
  double result = f.FormulaCalculator();
  EXPECT_DOUBLE_EQ(result, 3.75);
}

TEST(TFormulaTest, UnbalancedParentheses)
{
  char formula[] = "((2+3)*4";
  TFormula<double> f(formula);
  
  TVector<int> brackets;
  int errors = f.FormulaChecker(brackets);
  EXPECT_GT(errors, 0);
}

TEST(TFormulaTest, ExtraClosingParenthesis)
{
  char formula[] = "2+3)*4";
  TFormula<double> f(formula);
  
  TVector<int> brackets;
  int errors = f.FormulaChecker(brackets);
  EXPECT_GT(errors, 0);
}

TEST(TFormulaTest, IntegerTemplate)
{
  char formula[] = "10+5";
  TFormula<int> f(formula);
  
  TVector<int> brackets;
  int errors = f.FormulaChecker(brackets);
  EXPECT_EQ(errors, 0);
  
  f.FormulaConverter();
  int result = f.FormulaCalculator();
  EXPECT_EQ(result, 15);
}

TEST(TFormulaTest, PowerOperation)
{
  char formula[] = "2^3";
  TFormula<double> f(formula);
  
  TVector<int> brackets;
  int errors = f.FormulaChecker(brackets);
  EXPECT_EQ(errors, 0);
  
  f.FormulaConverter();
  double result = f.FormulaCalculator();
  EXPECT_DOUBLE_EQ(result, 8.0);
}

TEST(TFormulaTest, GetFormulaMethod)
{
  char formula[] = "2+3*4";
  TFormula<double> f(formula);
  
  std::string result = f.GetFormula();
  EXPECT_EQ(result, "2+3*4");
}

TEST(TFormulaTest, GetPostfixMethod)
{
  char formula[] = "2+3*4";
  TFormula<double> f(formula);
  
  f.FormulaConverter();
  std::string postfix = f.GetPostfix();
  EXPECT_EQ(postfix, "2 3 4 * +");
}

TEST(TFormulaTest, SetFormulaMethod)
{
  TFormula<double> f;
  f.SetFormula("5*6");
  
  std::string result = f.GetFormula();
  EXPECT_EQ(result, "5*6");
  
  TVector<int> brackets;
  int errors = f.FormulaChecker(brackets);
  EXPECT_EQ(errors, 0);
  
  f.FormulaConverter();
  double calcResult = f.FormulaCalculator();
  EXPECT_DOUBLE_EQ(calcResult, 30.0);
}

TEST(TFormulaTest, CopyConstructor)
{
  char formula[] = "7+8";
  TFormula<double> f1(formula);
  TFormula<double> f2(f1);
  
  EXPECT_EQ(f1.GetFormula(), f2.GetFormula());
  
  TVector<int> brackets;
  int errors = f2.FormulaChecker(brackets);
  EXPECT_EQ(errors, 0);
  
  f2.FormulaConverter();
  double result = f2.FormulaCalculator();
  EXPECT_DOUBLE_EQ(result, 15.0);
}

TEST(TFormulaTest, AssignmentOperator)
{
  char formula1[] = "1+2";
  char formula2[] = "3*4";
  TFormula<double> f1(formula1);
  TFormula<double> f2(formula2);
  
  f1 = f2;
  EXPECT_EQ(f1.GetFormula(), "3*4");
  
  TVector<int> brackets;
  int errors = f1.FormulaChecker(brackets);
  EXPECT_EQ(errors, 0);
  
  f1.FormulaConverter();
  double result = f1.FormulaCalculator();
  EXPECT_DOUBLE_EQ(result, 12.0);
}

TEST(TFormulaTest, EmptyFormula)
{
  TFormula<double> f;
  std::string result = f.GetFormula();
  EXPECT_EQ(result, "");
}

TEST(TFormulaTest, ComplexPowerExpression)
{
  char formula[] = "2^3+4^2";
  TFormula<double> f(formula);
  
  TVector<int> brackets;
  int errors = f.FormulaChecker(brackets);
  EXPECT_EQ(errors, 0);
  
  f.FormulaConverter();
  double result = f.FormulaCalculator();
  EXPECT_DOUBLE_EQ(result, 24.0);
}

TEST(TFormulaTest, DefaultConstructor)
{
  TFormula<double> f;
  std::string result = f.GetFormula();
  EXPECT_EQ(result, "");
}

TEST(TFormulaTest, ConstructorWithNullptr)
{
  TFormula<double> f(nullptr);
  std::string result = f.GetFormula();
  EXPECT_EQ(result, "");
}

TEST(TFormulaTest, ConstructorWithEmptyString)
{
  char formula[] = "";
  TFormula<double> f(formula);
  std::string result = f.GetFormula();
  EXPECT_EQ(result, "");
}

TEST(TFormulaTest, MoveConstructor)
{
  char formula[] = "9+1";
  TFormula<double> f1(formula);
  TFormula<double> f2(std::move(f1));
  
  EXPECT_EQ(f2.GetFormula(), "9+1");
  
  TVector<int> brackets;
  int errors = f2.FormulaChecker(brackets);
  EXPECT_EQ(errors, 0);
  
  f2.FormulaConverter();
  double result = f2.FormulaCalculator();
  EXPECT_DOUBLE_EQ(result, 10.0);
}

TEST(TFormulaTest, MoveAssignmentOperator)
{
  char formula1[] = "1+1";
  char formula2[] = "5*5";
  TFormula<double> f1(formula1);
  TFormula<double> f2(formula2);
  
  f1 = std::move(f2);
  EXPECT_EQ(f1.GetFormula(), "5*5");
  
  TVector<int> brackets;
  int errors = f1.FormulaChecker(brackets);
  EXPECT_EQ(errors, 0);
  
  f1.FormulaConverter();
  double result = f1.FormulaCalculator();
  EXPECT_DOUBLE_EQ(result, 25.0);
}

TEST(TFormulaTest, SelfAssignment)
{
  char formula[] = "6+4";
  TFormula<double> f(formula);
  f = f;
  
  EXPECT_EQ(f.GetFormula(), "6+4");
  
  TVector<int> brackets;
  int errors = f.FormulaChecker(brackets);
  EXPECT_EQ(errors, 0);
  
  f.FormulaConverter();
  double result = f.FormulaCalculator();
  EXPECT_DOUBLE_EQ(result, 10.0);
}