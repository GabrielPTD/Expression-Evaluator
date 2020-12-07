# Expression-Evaluator
An simple expression evaluator that doesn't use OOP.

# How does it work?
* It converts the given expression from infix form to it's postfix form using the [Shunting-Yard Algorithm](https://en.wikipedia.org/wiki/Shunting-yard_algorithm)
* Using the postfix form of the expression, it follows this algorithm to get the result of the expression:
  * Create a stack to store operands (or values).
  * Scan the postfix expression and do following for every scanned element.
    * If the element is a number, push it into the stack
    * If the element is a operator, pop operands for the operator from stack. Evaluate the operator and push the result back to the stack
  * When the expression is ended, the number in the stack is the final answer
  
# What operations does it support ?
  - [x] Paranthesis: You can use `(` and `)` for the general purposes.
  - [x] Operators: `+, -, *, /, ==`
  - [ ] Functions: all functions from [here](https://docs.microsoft.com/en-us/dotnet/visual-basic/language-reference/functions/math-functions).
  - [x] You can use both real or integer constants. (E.g: `1.14 + 3.36 - 2`)
  
# How to use ?
* Compile the source with a compiler that supports at least c++11 (works on both **Windows** and **Linux**).
* Run the program and insert the expression, e.g: `(1 + (2 * 3) - 2) / 5` and it will return the answer `5`.
