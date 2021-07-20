
# Regex-Engine

This project is a implementation of a basic Regex Engine in C++ from scratch.
It is implemented using backtracking algorithm and It supports the following Regular Expressions.
 


## Supported Regex

- List operator ([...] and [^...])
- Match Zero or More operator (*)
- Match One or More operator (+)
- Match Zero or One operator (?)
- Find a single character (.)
- Some MetaCharacters (\d, \w, \s)


## Documentation
This is a brief Documentation about the structure of engine.

- RegexOperator() is a base class and has many children classes each of which represent a operator and  have implementation of match() methods. match() is a pure virutal function and helps to implement matching logic for different types of operators.
- vector<RegexOperator *> parseRegex(const string &expr);
    - This functions parses the input regex into a vector of RegexOperator type pointers

- Range find(vector<RegexOperator *> regex, const string &s)
    - This parsed vector is used to find the pattern in input string s
    - returns Range(-1, -1) if the pattern is not present and otherwise returns the range of pattern (first Occurance).
- bool match(vector<RegexOperator *> regex, const string &s)
    - This function checks if string matches with regex and returns a bool variable accordingly.
- Range findAtIndex(); function implements the backtracking algorithm and is used as a utility function for find() function.

  
## Running Tests

- There is an in-built tester program which tests, the program for various types of regex, this is implemented by me.
- To run tests, run the following commands, I used a windows machine, for linux machines use make instead of mingw32-make

```bash
  mingw32-make 
  test_regex.exe
```

## Screenshots

![Alt text](./screenShots/testing.jpg?raw=true "Optional Title")