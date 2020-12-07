#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <regex>
#include <stack>
#include <string>
#include <vector>

//===============Vars===============//
std::vector<std::string> PostfixNotation;
std::stack<long double> Results;

//std::map<std::string, short> Operators = { {"+", 1}, {"-", 1}, {"*", 2}, {"/", 2}, {"^", 3}, {"==", 4}, {"=", 5} };
std::map<std::string, short> Operators = { {"^", 1}, {"+", 2}, {"-", 2}, {"*", 3}, {"/", 3}, {"\\", 4}, {"mod", 5}, {"=", 6}, {"<>", 6}, {"<", 6}, {"<=", 6},
											{">", 6}, {">=", 6}, {"not", 7}, {"and", 8}, {"or", 9}, {"xor", 10}};

std::map<std::string, long double> Predefined = { {"true", 1}, {"false", 0}, {"pi", 3.14159}, {"e", 2.71828} };
std::map<std::string, long double> Variables;
//===============Functions===============//
short CheckOperator(const std::string& op) {
	const auto& it = Operators.find(op);

	if (it != Operators.end())
		return it->second;

	return 0;
}

bool GetVariable(const std::string& var, long double& X) {
	const auto& it = Variables.find(var);

	if (it != Variables.end()) {
		X = it->second;
		return true;
	}

	return false;
}

void AddVariable(const std::string& var, long double X) {
	Variables.insert({var, X});
}
bool IsFunction(const std::string& op) {
	if (op == "pow" || op == "sin")
		return true;

	return false;
}

bool IsNumber(const std::string& op) {
	return std::regex_match(op, std::regex(("((\\+|-)?[[:digit:]]+)(\\.(([[:digit:]]+)?))?")));
}

bool IsVariable(const std::string& op) {
	if (!isalpha(op[0]))
		return false;

	for (size_t i = 1; i < op.length(); ++i)
		if (!isalnum(op[i]))
			return false;

	return true;
}

bool IsPredefined(const std::string& op) {
	return Predefined.find(op) != Predefined.end();
}

long double GetPredefinedValue(const std::string& op) {
	const auto& it = Predefined.find(op);
	if (it != Predefined.end())
		return it->second;

	return 0;
}

void GeneratePostfix(const std::string& expr) {
	std::stack<std::string> ops;
	std::string token;

	size_t i = 0, len = expr.length();

	short priority = 0;

	while (i < len) {
		char c = expr[i];
		token.clear();

		token += c;

		if (isdigit(c)) {
			++i;
			while (i < len) {
				c = expr[i];

				if (!isdigit(c) && c != '.')
					break;
				
				token += c;
				++i;
			}
			PostfixNotation.push_back(token);
			continue;
		}
		else if (isalpha(c)) {
			++i;
			while (i < len) {
				c = expr[i];

				if (!isalpha(c))
					break;

				token += c;
				++i;
			}
			if (IsFunction(token))
				ops.push(token);
			else if ((priority = CheckOperator(token))) {
				while (!ops.empty() && ops.top() != "(" && (CheckOperator(ops.top()) >= priority)) {
					PostfixNotation.push_back(ops.top());
					ops.pop();
				}
				ops.push(token);
			}
			else if (IsPredefined(token)) 
				PostfixNotation.push_back(std::to_string(GetPredefinedValue(token)));
			
			else if (IsVariable(token))
				PostfixNotation.push_back(token);
			continue;
			
		}
		else if ((priority = CheckOperator(token))) {
			bool isNum = false;
			++i;
			while (i < len) {
				c = expr[i];

				if (c == ' ')
					break;

				if (isdigit(c))
					isNum = true;

				token += c;
				++i;
			}

			if (isNum)
				PostfixNotation.push_back(token);
			else {
				while (!ops.empty() && ops.top() != "(" && (CheckOperator(ops.top()) >= priority)) {
					PostfixNotation.push_back(ops.top());
					ops.pop();
				}
				ops.push(token);
			}
			continue;

		}
		else if (c == '(') {
			ops.push("(");
		}
		else if (c == ')') {
			while (!ops.empty() && ops.top() != "(") {

				PostfixNotation.push_back(ops.top());
				ops.pop();
			}

			if (!ops.empty() && ops.top() == "(")
				ops.pop();
			else
				throw std::exception("Invalid paranthesis, missing '('.");

			if (!ops.empty() && IsFunction(ops.top())) {
				PostfixNotation.push_back(ops.top());
				ops.pop();
			}
		}
		++i;
	}

	while (!ops.empty())
	{
		PostfixNotation.push_back(ops.top());
		ops.pop();
	}
}

long double GetParameter() {
	if (Results.empty())
		throw std::exception("Not enough parameters for this expression.");

	long double ret = Results.top();
	Results.pop();
	return ret;
}

void Calculate(const std::string& op) {
	if (op == "+") {
		long double b = GetParameter();
		long double a = GetParameter();
		
		Results.push(a + b);
	}
	else if (op == "-") {
		long double b = GetParameter();
		long double a = GetParameter();
		
		Results.push(a - b);
	}
	else if (op == "*") {
		long double b = GetParameter();
		long double a = GetParameter();
		Results.push(a * b);
	}
	else if (op == "/") {
		long double b = GetParameter();
		long double a = GetParameter();
		Results.push(a / b);
	}
	else if (op == "^") {
		long double b = GetParameter();
		long double a = GetParameter();
		Results.push(std::pow(a, b));
	}
	else if (op == "\\") {
		long B = (long)GetParameter();
		long A = (long)GetParameter();

		Results.push((long)(A - (B * (long)(A / B))));
	}
	else if (op == "mod") {
		long double B = GetParameter();
		long double A = GetParameter();

		Results.push(A - (B * (long)((long)A / (long)B)));
	}
	else if (op == "=") {
		long double B = GetParameter();
		long double A = GetParameter();

		Results.push(A == B);
	}
	else if (op == "<>") {
		long double B = GetParameter();
		long double A = GetParameter();

		Results.push(A != B);
	}
	else if (op == "<") {
		long double B = GetParameter();
		long double A = GetParameter();

		Results.push(A < B);
	}
	else if (op == "<=") {
		long double B = GetParameter();
		long double A = GetParameter();

		Results.push(A <= B);
	}
	else if (op == ">") {
		long double B = GetParameter();
		long double A = GetParameter();

		Results.push(A > B);
	}
	else if (op == ">=") {
		long double B = GetParameter();
		long double A = GetParameter();

		Results.push(A >= B);
	}
	else if (op == "not") {
		long double A = GetParameter();

		Results.push(!A);
	}
	else if (op == "and") {
		long double B = GetParameter();
		long double A = GetParameter();

		Results.push(A && B);
	}
	else if (op == "or") {
		long double B = GetParameter();
		long double A = GetParameter();

		Results.push(A || B);
	}
	else if (op == "xor") {
		long double B = GetParameter();
		long double A = GetParameter();
		if (A && B)
			Results.push(0);
		else if ((A && !B) || (!A && B))
			Results.push(1);
		else
			Results.push(0);
	}
	else if (op == "pow") {
		long double b = GetParameter();
		long double a = GetParameter();
		Results.push(std::pow(a, b));
	}
	else if (op == "sin") {
		long double a = GetParameter();
		Results.push(std::sin(a));
	}
}

long double Evaluate() {
	for (auto it = PostfixNotation.begin(); it != PostfixNotation.end(); ++it) {
		if (CheckOperator(*it) || IsFunction(*it))
			Calculate(*it);
		else if (IsNumber(*it)) {
			Results.push(std::stold(*it));
		}
		else if (IsVariable(*it)) {
			long double A;
			if (GetVariable(*it, A))
				Results.push(A);
			else {
				std::string token;
				std::cout << "What is the value of " << *it << " ?\n";
				std::getline(std::cin, token);
				if (IsNumber(token)) {
					A = std::stold(token);
					Results.push(A);
					AddVariable(*it, A);
				}
				else if (IsPredefined(token)) {
					A = GetPredefinedValue(token);
					Results.push(A);
				}
			}
		}
		else {
			std::string szError("Invalid value " + *it);
			throw std::exception(szError.c_str());
		}
	}
	return Results.top();
}

//===============Main===============//

int main() {
	std::string szExpression;
	std::cout << "1. Use space between numbers and operators (it must be like this so the program can use both positive and negative numbers).\n2. Use only '(' and ')' for paranthesis.\n3. For a better result, please use paranthesis where is the case.\nEnter the expression that needs to be calculated:\n\n\n";
	std::getline(std::cin, szExpression);
	std::cout << "Calculating:\n" << szExpression << '\n';

	try
	{
		GeneratePostfix(szExpression);

		std::cout << "The postfix form of the expression is: ";

		for (const auto& p : PostfixNotation)
			std::cout << p << ' ';
		
		std::cout << '\n';

		std::cout << "Result: " << Evaluate() << '\n';
	}
	catch (const std::exception& e)
	{
		std::cout << "Error: " << e.what() << '\n';
	}
	return 0;
}