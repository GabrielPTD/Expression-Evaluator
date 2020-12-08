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
std::stack<std::string> Results;

std::map<std::string, short> Operators = { {"^", 1}, {"+", 2}, {"-", 2}, {"*", 3}, {"/", 3}, {"\\", 4}, {"mod", 5}, {"=", 6}, {"<>", 6}, {"<", 6}, {"<=", 6},
											{">", 6}, {">=", 6}, {"and", 7}, {"or", 8}, {"xor", 9}, {"not", 10} };

std::map<std::string, long double> Predefined = { {"true", 1}, {"false", 0}, {"pi", 3.14159}, {"e", 2.71828} };
std::map<std::string, long double> Variables;
//===============Functions===============//
short CheckOperator(const std::string& op) {
	const auto& it = Operators.find(op);

	if (it != Operators.end())
		return it->second;

	return 0;
}

bool IsFunction(const std::string& op) {
	if (op == "abs" || op == "acos" || op == "asin" || op == "atan" || op == "asin2" || op == "bigmul" || op == "ceiling"
		|| op == "cos" || op == "cosh" || op == "divrem" || op == "exp" || op == "exp" || op == "floor" || op == "ieeeremainder" || op == "log" || op == "log10"
		|| op == "max" || op == "min" || op == "pow" || op == "round" || op == "sign" || op == "sin" || op == "sinh" || op == "sqrt" || op == "tan" || op == "tanh"
		|| op == "truncate" || op == "acosh" || op == "asinh" || op == "atanh" || op == "bitdecrement" || op == "bitincrement" || op == "cbrt" || op == "clamp"
		|| op == "copysign" || op == "fusedmultiplyadd" || op == "ilogb" || op == "log2" || op == "maxmagnitude" || op == "minmagnitude" || op == "scaleb")
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

bool GetVariable(const std::string& var, long double& X) {
	const auto& it = Variables.find(var);

	if (it != Variables.end()) {
		X = it->second;
		return true;
	}

	return false;
}

void AddVariable(const std::string& var, long double X) {
	Variables.insert({ var, X });
}

void ReplaceVariable(const std::string& var, long double X) {
	auto it = Variables.find(var);

	if (it != Variables.end())
		it->second = X;
	else
		AddVariable(var, X);
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
	if (expr.empty())
		throw std::exception("Expression is empty.");
	
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

				if (!isalnum(c))
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

	long double ret = 0;
	if (!IsVariable(Results.top()))
		ret = std::stold(Results.top());
	else {
		if (GetVariable(Results.top(), ret)) {
			Results.pop();
			return ret;
		}
		else {
			std::string token;
			std::cout << "What is the value of " << Results.top() << " ?\n";
			std::getline(std::cin, token);
			if (IsNumber(token)) {
				ret = std::stold(token);
				AddVariable(Results.top(), ret);
			}
			else if (IsPredefined(token))
				ret = GetPredefinedValue(token);
		}
	}
	Results.pop();
	return ret;
}

const std::string GetParameterVariable() {
	if (Results.empty())
		throw std::exception("Not enough parameters for this expression.");

	if (!IsVariable(Results.top()))
		throw std::exception("Parameter is not a variable");

	const std::string ret = Results.top();

	Results.pop();
	return ret;
}

void AddResult(const std::string& res) {
	Results.push(res);
}

void AddResult(long double res) {
	Results.push(std::to_string(res));
}

void Calculate(const std::string& op) {
	if (op == "+") {
		long double b = GetParameter();
		long double a = GetParameter();
		
		AddResult(a + b);
	}
	else if (op == "-") {
		long double b = GetParameter();
		long double a = GetParameter();
		
		AddResult(a - b);
	}
	else if (op == "*") {
		long double b = GetParameter();
		long double a = GetParameter();
		AddResult(a * b);
	}
	else if (op == "/") {
		long double b = GetParameter();
		long double a = GetParameter();
		AddResult(a / b);
	}
	else if (op == "^") {
		long double b = GetParameter();
		long double a = GetParameter();
		AddResult(std::pow(a, b));
	}
	else if (op == "\\") {
		long B = (long)GetParameter();
		long A = (long)GetParameter();

		AddResult((long)(A - (B * (long)(A / B))));
	}
	else if (op == "mod") {
		long double B = GetParameter();
		long double A = GetParameter();

		AddResult(A - (B * (long)((long)A / (long)B)));
	}
	else if (op == "=") {
		long double B = GetParameter();
		long double A = GetParameter();

		AddResult(A == B);
	}
	else if (op == "<>") {
		long double B = GetParameter();
		long double A = GetParameter();

		AddResult(A != B);
	}
	else if (op == "<") {
		long double B = GetParameter();
		long double A = GetParameter();

		AddResult(A < B);
	}
	else if (op == "<=") {
		long double B = GetParameter();
		long double A = GetParameter();

		AddResult(A <= B);
	}
	else if (op == ">") {
		long double B = GetParameter();
		long double A = GetParameter();

		AddResult(A > B);
	}
	else if (op == ">=") {
		long double B = GetParameter();
		long double A = GetParameter();

		AddResult(A >= B);
	}
	else if (op == "not") {
		long double A = GetParameter();

		AddResult(!A);
	}
	else if (op == "and") {
		long double B = GetParameter();
		long double A = GetParameter();

		AddResult(A && B);
	}
	else if (op == "or") {
		long double B = GetParameter();
		long double A = GetParameter();

		AddResult(A || B);
	}
	else if (op == "xor") {
		long double B = GetParameter();
		long double A = GetParameter();
		if (A && B)
			AddResult(0);
		else if ((A && !B) || (!A && B))
			AddResult(1);
		else
			AddResult(0);
	}
	else if (op == "abs") {
		long double A = GetParameter();
		AddResult(std::abs(A));
	}
	else if (op == "acos") {
		long double A = GetParameter();
		AddResult(std::acos(A));
	}
	else if (op == "asin") {
		long double A = GetParameter();
		AddResult(std::asin(A));
	}
	else if (op == "atan") {
		long double A = GetParameter();
		AddResult(std::atan(A));
	}
	else if (op == "atan2") {
		long double B = GetParameter();
		long double A = GetParameter();
		AddResult(std::atan2(A, B));
	}else if (op == "bigmul") {
		long double B = GetParameter();
		long double A = GetParameter();
		AddResult((long)(A *B));
	}else if (op == "ceiling") {
		long double A = GetParameter();
		AddResult(std::ceill(A));
	}else if (op == "cos") {
		long double A = GetParameter();
		AddResult(std::cosl(A));
	}else if (op == "cosh") {
		long double A = GetParameter();
		AddResult(std::coshl(A));
	}else if (op == "exp") {
		long double A = GetParameter();
		AddResult(std::expl(A));
	}else if (op == "floor") {
		long double A = GetParameter();
		AddResult(std::floorl(A));
	}else if (op == "ieeeremainder") {
		long double B = GetParameter();
		long double A = GetParameter();
		AddResult((long)((long) A % (long) B));
	}else if (op == "log") {
		long double A = GetParameter();
		AddResult(std::logl(A));
	}else if (op == "log10") {
		long double A = GetParameter();
		AddResult(std::log10l(A));
	}else if (op == "max") {
		long double B = GetParameter();
		long double A = GetParameter();
		AddResult(std::fmaxl(A, B));
	}else if (op == "min") {
		long double B = GetParameter();
		long double A = GetParameter();
		AddResult(std::fminl(A, B));
	}else if (op == "pow") {
		long double B = GetParameter();
		long double A = GetParameter();
		AddResult(std::powl(A, B));
	}else if (op == "round") {
		long double A = GetParameter();
		AddResult(std::roundl(A));
	}else if (op == "sign") {
		long double A = GetParameter();
		if (A < 0)
			AddResult(-1);
		else if (A == 0)
			AddResult(0);
		else
			AddResult(1);
	}else if (op == "sin") {
		long double A = GetParameter();
		AddResult(std::sinl(A));
	}else if (op == "sinh") {
		long double A = GetParameter();
		AddResult(std::sinhl(A));
	}else if (op == "tan") {
		long double A = GetParameter();
		AddResult(std::tanl(A));
	}else if (op == "tanh") {
		long double A = GetParameter();
		AddResult(std::tanhl(A));
	}else if (op == "truncate") {
		long double A = GetParameter();
		AddResult(std::truncl(A));
	}else if (op == "acosh") {
		long double A = GetParameter();
		AddResult(std::acoshl(A));
	}else if (op == "asinh") {
		long double A = GetParameter();
		AddResult(std::asinhl(A));
	}else if (op == "atanh") {
		long double A = GetParameter();
		AddResult(std::tanhl(A));
	}
	else if (op == "cbrt") {
		long double A = GetParameter();
		AddResult(std::cbrtl(A));
	}else if (op == "copysign") {
		long double B = GetParameter();
		long double A = GetParameter();
		AddResult(std::copysignl(A, B));
	}else if (op == "fusedmultiplyadd") {
		long double C = GetParameter();
		long double B = GetParameter();
		long double A = GetParameter();
		AddResult(std::roundl((A * B) + C));
	}else if (op == "ilogb") {
		long double A = GetParameter();
		AddResult(std::ilogbl(A));
	}else if (op == "log2") {
		long double A = GetParameter();
		AddResult(std::log2l(A));
	}else if (op == "maxmagnitude") {
		long double B = GetParameter();
		long double A = GetParameter();
		if (A > B)
			AddResult(A);
		else if (A < B)
			AddResult(B);
	}else if (op == "minmagnitude") {
		long double B = GetParameter();
		long double A = GetParameter();
		if (A > B)
			AddResult(B);
		else if (A < B)
			AddResult(A);
	}else if (op == "scaleb") {
		long double B = GetParameter();
		long double A = GetParameter();
		AddResult(A * (std::pow(2, B)));
	}else if (op == "sqrt") {
		long double A = GetParameter();
		AddResult(std::sqrtl(A));
	}
	else if (op == "divrem") {
		const std::string var = GetParameterVariable();
		long double B = GetParameter();
		long double A = GetParameter();
		AddResult((long)((long)A / (long)B));
		ReplaceVariable(var, (long)((long)A % (long)B));
	}

}

long double Evaluate() {
	for (auto it = PostfixNotation.begin(); it != PostfixNotation.end(); ++it) {
		if (CheckOperator(*it) || IsFunction(*it))
			Calculate(*it);
		else if (IsNumber(*it)) {
			AddResult(std::stold(*it));
		}
		else if (IsVariable(*it)) {
			AddResult(*it);
		}
		else {
			std::string szError("Invalid value " + *it);
			throw std::exception(szError.c_str());
		}
	}
	return std::stold(Results.top());
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
		szExpression.clear();

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