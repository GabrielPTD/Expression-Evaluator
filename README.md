# Expression-Evaluator
A simple expression evaluator that doesn't use OOP.

# How does it work?
* It converts the given expression from the infix form to it's postfix form using the [Shunting-Yard Algorithm](https://en.wikipedia.org/wiki/Shunting-yard_algorithm)
* Using the postfix form of the expression, it follows this algorithm to get the result:
  * Create a stack to store operands (or values).
  * Scan the postfix expression and do the following steps for each element.
    * If the element is a number, push it into the stack
    * If the element is an operator, pop operands for the operator from stack. Evaluate the operator and push the result back to the stack
  * When the scan is ended, the number in the stack is the final answer.
  
# What operations does it support ?
  - [x] Paranthesis: You can use `(` and `)` for the general purposes.
  - [x] Operators: `+, -, /, *, <>, =, <=, >=, or, and, xor, not, mod, \` - `=` is ONLY for comparsion.
  - [x] Functions: all functions from [here](https://docs.microsoft.com/en-us/dotnet/visual-basic/language-reference/functions/math-functions).
  - [x] You can use real, integer or logical constants. (E.g: `1.14 + 3.36 - 2 or false`)
  - [x] You can use variables and/or set values for certain variables(pi, e, etc).
  
# How to use ?
* Compile the source with a compiler that supports at least c++11 (works on both **Windows** and **Linux**).
* Run the program and insert the expression, e.g: `(1 + (2 * 3) - 2) / 5` and it will return the answer `5`.

# Changelog
* Changelog 1.1
  * Added support for new operators (`<>, =, <=, >=, or, and, xor, not, mod, \`).
  * Added support for variables and predefined variables
  * Added support for logical constants (`true, false`).
  * Added **debug** informations and **rules**.
* Changelog 1.2
  * Added support for all the functions found [here](https://docs.microsoft.com/en-us/dotnet/visual-basic/language-reference/functions/math-functions) (except DivRem - will be added in next update)
  * Fixed an error that made some functions that contain numbers in the name to not be recognized.
  * Deleted some useless comments.
* Changelog 1.3
  * Added support for function DivRem found [here](https://docs.microsoft.com/en-us/dotnet/visual-basic/language-reference/functions/math-functions).
  * Changed the Result stack from type `long double` to `std::string` so it can support changing the value of a variable after it was defined.
  * Fixed a bug with the `not` operator, had to change it's precedance.
