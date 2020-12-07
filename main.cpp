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

std::map<std::string, short> Operators = { {"+", 1}, {"-", 1}, {"*", 2}, {"/", 2}, {"^", 3}, {"==", 4}, {"=", 5} };
//===============Functions===============//
short CheckOperator(const std::string& op) {
	const auto& it = Operators.find(op);

	if (it != Operators.end())
		return it->second;

	return 0;
}

bool IsFunction(const std::string& op) {
	if (op == "pow")
		return true;

	return false;
}

bool IsNumber(const std::string& op) {
	return std::regex_match(op, std::regex(("((\\+|-)?[[:digit:]]+)(\\.(([[:digit:]]+)?))?")));
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
	else if (op == "=") {
		long double b = GetParameter();
		long double a = GetParameter();
		Results.push(b);
	}
	else if (op == "==") {
		long double b = GetParameter();
		long double a = GetParameter();
		Results.push(a == b);
	}
	else if (op == "pow") {
		long double b = GetParameter();
		long double a = GetParameter();
		Results.push(std::pow(a, b));
	}
	
}

long double Evaluate() {
	for (auto it = PostfixNotation.begin(); it != PostfixNotation.end(); ++it) {
		if (CheckOperator(*it) || IsFunction(*it))
			Calculate(*it);
		else if (IsNumber(*it)){
			Results.push(std::stold(*it));
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
	std::string szExpression, szToken;

	std::cout << "1. Use space between numbers and operators (it must be like this so the program can use both positive and negative numbers).\n2. Use only '(' and ')' for paranthesis.\nEnter the expression that needs to be calculated: \n\n\n\n\n";

	while (std::getline(std::cin, szToken)) {
		szExpression.append(szToken + " ");

		if (szToken.find('\\') == std::string::npos)
			break;
	}

	szExpression.erase(std::remove_if(szExpression.begin(), szExpression.end(), [](const char& c) { return c == '\\'; }), szExpression.end());

	std::cout << "Calculating:\n" << szExpression << '\n';

	try
	{
		GeneratePostfix(szExpression);

		std::cout << "Result: " << Evaluate() << '\n';
	}
	catch (const std::exception& e)
	{
		std::cout << "Error: " << e.what() << '\n';
	}
	return 0;
}
