/*
 *Cop3503 Programming Assignment 3 [Simple Calculator]
 --------------------------------------------------------------------------------------------
 * C++ program that emulates a simple calculator. The operations that the calculator can handle
 * include: addition, subtraction, multiplication, division, sine, cosine, logarithm, power, square
 * root. The program takes in an input from the user, which cotains the expression that the user
 * wants to compute. Then the program parses the user's input, detects the operators and their
 * argument(s), and returns the result. The program also checks that the user has correctly balanced
 * the parenthesis, and reports errors if user trys to divide by 0 or take the logarithm/squareroot
 * of a negative number. The program uses stacks to handle the operators/arguments. 
-----------------------------------------------------------------------------------------------------
 * @uthor Scott Thelemann
 *
 * Version 1.0 11/14/2014
 *
*/

#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include "pa3.h"
#include <sstream>
#include <string>
#include <vector>

using namespace std;

//Calling each of the methods below that I implemented.
string convert(string infix);
bool TakesPrecedence(char OperatorA, char OperatorB);
double evaluate(string postfix);
double operate(double num1, double num2, char opt);
string tokenize(string user_input);
bool isOperator(char check);

int main() {
	cout << "Please enter the expression:" << endl << endl;

	string user_input, postfix, tokenized;

	//Takes in user's input, and then removes all whitespace from the input.
	getline(cin, user_input);
	user_input.erase(std::remove_if(user_input.begin(), user_input.end(), [](char c){ return (c == ' ' );}), user_input.end());

	//For loop below counts how many left and right parenthesis there are. Then if statement checks
	//if parenthesis are correctly balanced.
	int left_par_count = 0;
	int right_par_count = 0;
	for(int i = 0; i < user_input.size(); ++i) {
		if(user_input.at(i) == '(') {
			++right_par_count;
		}
		else if(user_input.at(i) == ')') {
			++left_par_count;
		}
	}
	if(right_par_count != left_par_count) {
		cout << endl << "Error: Unbalanced parenthesis!!!";
		return 0;
	}

	tokenized = tokenize(user_input);
	postfix = convert(tokenized);

	double result = evaluate(postfix);

	cout << endl << "The result is: " << result;
	return 0;
}

//Method takes in the users input, and checks for sin(),cos(),log(), sqrt(), and negative
//numbers. The method then replaces these inputs with a single letter/symbol to indicate
//which operation to call. Tokenized is then returned to the main function.
string tokenize(string user_input) {
	string tokenized;
	char symbol;
	char sine = 's';
	char cosine = 'c';
	char logarithm = 'l';
	char squareroot = 'q';
	char negative = '>';

	for(int i = 0; i < user_input.size(); ++i) {
		symbol = user_input.at(i);

		if(user_input.at(i) == 's' && user_input.at(i + 1) == 'i') {
			symbol = sine;
			tokenized = tokenized + symbol;
			++i;
			++i;
		}
		else if(user_input.at(i) == 'c' && user_input.at(i + 1) == 'o') {
			symbol = cosine;
			tokenized = tokenized + symbol;
			++i;
			++i;
		}
		else if(user_input.at(i) == 'l' && user_input.at(i + 1) == 'o') {
			symbol = logarithm;
			tokenized = tokenized + symbol;
			++i;
			++i;
		}
		else if(user_input.at(i) == 's' && user_input.at(i+1) == 'q') {
			symbol = squareroot;
			tokenized = tokenized + symbol;
			++i;
			++i;
			++i;
		}
		else if(user_input.at(i) == '-' && (isOperator(user_input.at(i - 1)))) {
			symbol = negative;
			tokenized = tokenized + symbol;
		}
		else {
			tokenized = tokenized + symbol;
		}
	}
	return tokenized;
}

//Method below is used to convert the user's infix expression into postfix, so that it is
//easier to evaluate using stacks. The method runs through the entire string passed into it
//and adds operators to a stack, and the numbers to a new string called output. Once the method
//completes it then returns the new string which includes the operators and numbers in postfix form.
string convert(string infix) {
	string output, double_to_string;
	ostringstream s;
	Stack_Char OperatorStack(infix.size());
	char symbol, topSymbol;


	for(int i = 0; i < infix.size(); ++i) {
		symbol = infix.at(i);
		if(isdigit(symbol) && isdigit(infix.at(i + 1))) {
			output = output + symbol + ',' + infix.at(i + 1);
			++i; 
		}
		else if(isdigit(symbol)) {
			output = output + symbol;
		}
		else if(symbol == '>') {
			output = output + symbol;
		}
		else if(symbol == 'l' || symbol == 'q') {
			for(int j = i + 2; j < infix.size(); ++j) {
				if(infix.at(j) == ')') {
					i = j;
					break;
				}
				else if(infix.at(j) == '>') {
					if(symbol == 'l') {
						cout << endl << "Error: Taking logarithm of a negative number!!!";
						exit(0);
					}
					else {
						cout << endl << "Error: Taking square root of a negative number!!!";
						exit(0);
					}
				}
				else {
					if(isdigit(infix.at(j + 1))) {
						double two_digits1 = infix.at(j) - '0';
						double two_digits2 = infix.at(j + 1) - '0';
						double num = (two_digits1 * 10) + two_digits2;
						double result = operate(0, num, symbol);
						s << result;
						double_to_string = s.str();
						output = output + double_to_string;
						s.str("");
						++j;
					}
					else {
						char num_char = infix.at(j);
						double num = num_char - '0';
						double result = operate(0, num, symbol);
						s << result;
						double_to_string = s.str();
						output = output + double_to_string;
						s.str("");
					}
				}
			}
		}
		else if(symbol == 's' || symbol == 'c') {
			const double PI = atan(1) * 4;

			for(int j = i + 2; j < infix.size(); ++j) {
				if(infix.at(j) == ')') {
					i = j;
					break;
				}
				else {
					if(isdigit(infix.at(j + 1))) {
						double two_digits1 = infix.at(j) - '0';
						double two_digits2 = infix.at(j + 1) - '0';
						double num = (two_digits1 * 10) + two_digits2;
						num = (num * PI / 180.0);
						double result = operate(0, num, symbol);
						if(result < 0.01) {
							result = 0;
						}
						s << result;
						double_to_string = s.str();
						output = output + double_to_string;
						s.str("");
						++j;
					}
					else if(isdigit(infix.at(j)) && (!isdigit(infix.at(j - 1)))) {
					char num_char = infix.at(j);
					double num = num_char - '0';
					num = (num * PI / 180.0);
					double result = operate(0, num, symbol);
					if(result < 0.01) {
						result = 0;
					}
					s << result;
					double_to_string = s.str();
					output = output + double_to_string;
					s.str("");
					}
				}
			}
		}
		else {
			while((! OperatorStack.isEmpty()) && (TakesPrecedence(OperatorStack.peek(), symbol))) {
				topSymbol = OperatorStack.peek();
				OperatorStack.pop();
				output = output + topSymbol;
			}
			if((! OperatorStack.isEmpty()) && (symbol == ')'))
				OperatorStack.pop();
			else
				OperatorStack.push(symbol);
		}
	}
	while(! OperatorStack.isEmpty()) {
		topSymbol = OperatorStack.peek();
		OperatorStack.pop();
		output = output + topSymbol;
	}
	return output;
}

//Method below is used to check if OperatorA takes precedence over OperatorB or not. The
//method then returns either true or false depending on the operators.
bool TakesPrecedence(char OperatorA, char OperatorB)
{
   if (OperatorA == '(')
      return false;
   else if (OperatorB == '(')
      return false;
   else if (OperatorB == ')')
      return true;
   else if ((OperatorA == '^') && (OperatorB == '^'))
      return false;
   else if (OperatorA == '^')
      return true;
   else if (OperatorB == '^')
      return false;
   else if ((OperatorA == '*') || (OperatorA == '/'))
      return true;
   else if ((OperatorB == '*') || (OperatorB == '/'))
      return false;
   else if ((OperatorA == 's') || (OperatorA == 'c') || (OperatorA == 'l') || (OperatorA == 'q'))
  	return true;
  else if ((OperatorB == 's') || (OperatorB == 'c') || (OperatorB == 'l') || (OperatorB == 'q'))
  	return false;
   else
      return true;
       
}

//Method below is used to check if a character is an operator or not, and then returns
//either true or false.
bool isOperator(char check) {
	if(check == '+' || check == '-' || check == '*' || check == '/' || check == 's' ||
		check == 'c' || check == 'l' || check == 'q' || check == '^' || check == '(') {
			return true;
		}
	else {
		return false;
	} 
}

//Method below is used to evaluate the postfix expression that it is given through the main
//function. It then checks if the number has multiple digits(flagged by a ','), is a decimal,
//or a negative number(flagged by '>'). It then adds the numbers to a stack that contains doubles
//and when the for loop hits an operator it will take the top number from the stack then pop the
//stack followed by another peek at the top number and again another pop followed by pushing the
//result of the evaluation into the top of stack, and eventually returning the result of the 
//expression back to the main function.
double evaluate(string expression) {
	Stack_Double operands(expression.size());
	char symbol, topSymbol;
	double result;

	if(expression.size() == 1) {
		result = expression.at(0) - '0';
		return result; 
	} 	
	for(int i = 0; i < expression.size(); ++i) {
		symbol = expression.at(i);
		if(isdigit(symbol) && expression.at(i + 1) == ',') {
			double two_digits1 = symbol - '0';
			two_digits1 = two_digits1 * 10;
			double two_digits2 = expression.at(i + 2) - '0';
			operands.push(two_digits1 + two_digits2);
			++i;
			++i;
		} 
		else if(isdigit(symbol) && expression.at(i + 1) == '.') {
			double decimal_digit1 = symbol - '0';
			double decimal_digit2 = expression.at(i + 2) - '0';
			decimal_digit2 = decimal_digit2 / 10;
			double decimal = decimal_digit1 + decimal_digit2;
			operands.push(decimal);
			++i;
			++i;
		}
		else if(symbol == '>') {
			double num = expression.at(i + 1) - '0';
			num = -(num);
			operands.push(num);
			++i;
		}
		else if(isdigit(symbol)) {
			double num = symbol - '0';
			operands.push(num);
		}
		else {
			double oper1, oper2, result;
			oper2 = operands.peek();
			operands.pop();
			oper1 = operands.peek();
			operands.pop();
			result = operate(oper1, oper2, symbol);
			operands.push(result);
		}
	}
	result = operands.peek();
	return result;	
}

//Method below is used to do the actual calculations of the different operations contained in the
//original expression, and returns this result back to wherever it was called. Error's will
//get called if user trys to divide by 0, take the log/sqrt of a negative number, or evaluate
//an invalid operation.
double operate(double num1, double num2, char opt) {
	double result;
	if(opt == '+') {
		result = num1 + num2;
		return result;
	}
	else if(opt == '-') {
		result	= num1 - num2;
		return result;
	}
	else if(opt == '*') {
		result = num1 * num2;
		return result;
	}
	else if(opt == '/') {
		if(num2 == 0) {
			cout << endl << "Error: Division by zero!!!";
			exit(0);
		}
		else {
			result = num1/num2;
			return result;
		}
	}
	else if(opt == 's') {
		result = sin(num2);
		return result;
	}
	else if(opt == 'c') {
		result = cos(num2);
		return result;
	}
	else if(opt == 'l') {
		result = log2(num2);
		return result;
	}
	else if(opt == '^') {
		result = pow(num1, num2);
		return result;
	}
	else if(opt == 'q') {
		result = sqrt(num2);
		return result; 
	}
	else {
		cout << "Can not perform: " << opt << endl;
		exit(0);
	}
} 