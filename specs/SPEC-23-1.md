# BS-less Object Notation Specification 23 (BSON23)
#### October 29th, 2023
## BSON Overview
BSON is a general purpose replacement of JSON. It is not developed with web and JavaScript 100% in mind.
This specification covers the syntax of BSON.
## Specification 23.1 Overview
- + More leniant key names.
- + Escape sequences in strings.
- + Multi-line strings.
# Usage
## Key names
All names follow these rules
- Will never contain the following symbols:
 - !@#$%^&*()=+[]\{}|;:'",./<>?`~
- Must start with:
 - Letter a-z/A-Z or underscore
- The rest may contain numbers
The following are valid examples of valid names
```
helloworld          "Hello World!"
hello-world         "Hello World!"
hello_world         "Hello World!"
_hello-world        "Hello World!"
_hello-world-123    "Hello World!"
```
The following are invalid examples of valid names
```
-byeworld           "Bye World!"        
-bye-world          "Bye World!"
123bye-world        "Bye World!"
```
## Escape sequences
All strings now inherently take into account escape sequences. These escape sequences are the same as any language inspired by C escape sequences.
```
mystring "Hello world!\n"
i_can {
    quote "I can \"quote\""
}
```
## Multi-line string syntax
The syntax for multi-line strings are inherent. Strings are read until it finds the matching quote, and handles the odd formatting and the previously mentioned escape sequences.
```
paragraph "\tThis is a long string of text that I am writing to demonstrate mutli-line
            strings. This allows for bigger paragraphs of text without making the file look too bad. With
            proper formating you could make this look a lot better. The formatting in this text, like every
            time I press \"Enter\", will not be taken into account, but it will take into account...\n\t... Escape Sequences..."
```









