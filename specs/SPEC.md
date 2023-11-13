# BSON Specification
#### October
## Key names
- Only lowercase, capital letters, and underscores
- Numbers cannot be the first character
- Dashes are the only allows symbol, not allowed to be the first character
Correct
```
numberone       1
number-one      1
number_1        1
_number1        1
```
Incorrect
```
-numbertwo      2
2number         2
number!!        2
```
## Comments
C style comments. Every '//' found skips all text until the nearest newline character.
```
// This is ignored
mydata          3  // This data is read, but these words you read are not
```
## Data types
- Longs
- Doubles
- Strings
- Objects
- Arrays
Longs are (preferably) 64 bit integers, doubles are (preferably) 64 bit floating point numbers. The official 'libbson' will use 64 bits but it may differ between implementations.
Strings store text. Objects store data with a key names, arrays do not.
## Longs
Type the number with nothing fancy.
```
myinteger  4
```
## Doubles
Type the number with at least one digit and one decimal point.
```
mydecimal  3.14
```
Incorrect examples
```
mydecimal 5 // This is just an integer
mydecimal . // This is a syntax error
```
## Strings
Everything inbetween two quotes is read as a string. Strings can support escape sequences and multi-line strings as shown in the following.
```
// Simple string
mystring   "My string!"
// New line at the end
helloworld "Hello world!\n"
// Multi-line
much-text  "This is a bit of text, you may
            keep reading if you like."
```
## Objects
Objects are interpreted as everything inbetween two braces, '{' and '}'. Each element must provide key names and follow previous syntax patterns.
```
person {
    first "John"
    last  "Doe"
    age    33
}
```
## Arrays
Arrays are interpreted as everything inbetween two brackets, '[' and ']'. Each element will no longer have a key name. Otherwise, previous syntax patterns remain. They can also be mixed data.
```
array [
    5
    2.71
    "Data string"
    { x 1 y 2 }
    [ "2D" "Array???" ]
]
```








