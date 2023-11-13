# BS-less Object Notation Specification 23 (BSON23)
#### October 29th, 2023
## BSON Overview
BSON is a general purpose replacement of JSON. It is not developed with web and JavaScript 100% in mind.
This specification covers the syntax of BSON.
Bson can store:
- + Longs/integers (64 bit)
- + Floating points (64 bit)
- + Strings
- + Objects
- + Arrays
## Specification 23.0 Overview
To summarize, BSON is the same as JSON with these notable syntax changes as of this specification.
- \- No colons
- \- Quotes only surround string data
- \- No commas between data
- + C style comments '//'
# Usage
## Comments
To write comments, use '//'. All the data preceding it is ignored until the next line.
```
// This is a comment
```
## Key names
In order to write data a name must be associated with it, which will be referred to as a key. The rules for the key name for BSON23 is currently:
- Only letters a-z/A-Z and underscores
- Numbers can appear only after the first letter
- No symbols... except underscores.
These are valid examples of key names.
```
keyname      ...
key_name     ...
_key_name    ...
key123name   ...
keyname123   ...
_key_name123 ...
```
These are invalid examples of key names.
```
1keyname     ...
#key-name    ...
key-name     ...
key*name     ...
```
## Longs
Integers are stored as 64 bit integers. These are referred to as longs. To create a long you specify the name, some whitespace, then the number.
```
myinteger 3
```
## Floating points
Floating point numbers are stored as 64 bit doubles. To create a double you specify the name, some whitespace, then the number with at least one digit and one decimal point. The following are examples of valid doubles.
```
dec1 3.14
dec2 2.0
dec3 1.
dec4 0.
dec5 .0
```
These are not valid doubles.
```
mydecimal        5 // This is an integer
anotherdecimal   . // This is a syntax error
```
## Strings
Strings as of this version are anything inbetween two matching quotes.
```
mystring "Hello World!"
```
Escape sequences are planned in the future.
## Objects
Objects are defined with braces, '{' and '}'. Inside the braces you can put any data inside so long it follows the previously associated syntax.
```
person {
    first "John"
    last "Doe"
    age 33
}
```
## Arrays
Arrays behave similarly to objects except that the name does no longer gets specified. Types in arrays do not matter. Arrays are defined with brackets, '[' and ']'.
```
ints [ 1 2 3 4 ]
decs [ 2.2 3.3 4.4 ]
```
They can also store objects
```
vecs [
    { x 1 y 2 }
    { x 3 y 4 }
    { x 5 y 6 }
]
people [
    {
        first "John"
        last "Doe"
        age 33
    }
    {
        first "Jane"
        lsat "Doe"
        age 32
    }
]
```
As mentioned, they all do not have to be the same type
```
array [
    4
    2.71
    "String!"
    arr2 [ 1 "Wow" { x 1 y 2 } ]
    obj {
        number 6
        friend "alfred"
    }
    coherently "nonsense"
]
```



