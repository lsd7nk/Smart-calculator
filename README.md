# SmartCalc v1.0

## Introduction

In this project, an extended version of a conventional calculator is implemented in the C programming language using a structural approach, which can be found in standard applications of each operating system. In addition to basic arithmetic operations like plus-minus and multiply-divide, the calculator is supplemented with the ability to calculate arithmetic expressions based on priorities, as well as some mathematical functions (sine, cosine, logarithm, etc.). In addition to calculating expressions, the calculator supports the use of the variable _x_ and plotting the corresponding function.

## Implementation

- The program is developed in the C language of the C11 standard using the gcc compiler
- The program is designed in accordance with the principles of structured programming
- Implementation with a graphical user interface, based on a GUI library with API for C89/C99/C11 - GTK+
- Both integers and real numbers separated by a dot can be supplied to the program input. Optionally, you can process the input of numbers in exponential notation
- The calculation is performed after fully entering the calculated expression and clicking on the `=` symbol
- Calculation of arbitrary parenthesized arithmetic expressions in infix notation
- Calculation of arbitrary parenthesized arithmetic expressions in infix notation with substitution of the value of the variable _x_ as a number
- Plotting a function defined using an expression in infix notation with the variable _x_ (with coordinate axes, a mark of the scale used and a grid with an adaptive step)
- The user is not given the opportunity to change the scale
- The definition area and the value area of the functions are limited to at least numbers from -1000000 to 1000000
- To plot the function, it is necessary to additionally specify the displayed definition area and value area
- Verifiable accuracy of the fractional part - 7 decimal places
- The user can enter up to 255 characters
- Parenthesized arithmetic expressions in infix notation support the following arithmetic operations and mathematical functions:
    - **Arithmetic operators**:

      | Operator name | Infix notation <br /> (Classic) | Prefix notation <br /> (Polish notation) |  Postfix notation <br /> (Reverse Polish notation) |
      | --------- | ------ | ------ | ------ |
      | Brackets | (a + b) | (+ a b) | a b + |
      | Addition | a + b | + a b | a b + |
      | Subtraction | a - b | - a b | a b - |
      | Multiplication | a * b | * a b | a b * |
      | Division | a / b | / a b | a b \ |
      | Power | a ^ b | ^ a b | a b ^ |
      | Modulus | a mod b | mod a b | a b mod |
      | Unary plus | +a | +a | a+ |
      | Unary minus | -a | -a | a- |

      >Note that the multiplication operator contains the obligatory sign `*`
    - **Functions**:
  
      | Function description | Function |
      | ------ | ------ |
      | Computes cosine | cos(x) |
      | Computes sine | sin(x) |
      | Computes tangent | tan(x) |
      | Computes arc cosine | acos(x) |
      | Computes arc sine | asin(x) |
      | Computes arc tangent | atan(x) |
      | Computes square root | sqrt(x) |
      | Computes natural logarithm | ln(x) |
      | Computes common logarithm | log(x) |
