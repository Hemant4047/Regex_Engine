
# Regex-Engine

This project is a implementation of a basic Regex Engine in C++ from scratch.
It is implemented using backtracking algorithm and It supports the following Regular Expressions.
 


## Supported Regex

- List operator ([...] and [^...])
- Ranged Expressions ([1-9A-V])
- Sequence containing exact X characters ({X})
- Sequence containing X to Y characters ({X,Y})
- Match Zero or More operator (*)
- Match One or More operator (+)
- Match Zero or One operator (?)
- Escape character to use . (\.)
- Find a single character (.)
- Finding any digit (\d)
- Finding any Alpha-Numeric character (\w)
- Finding space character (\s)


## Screenshots

- Sample run
![App Screenshot](https://user-images.githubusercontent.com/62866353/126300891-6fe27e43-fa88-4680-a77c-910e4673cda7.png)

- Tester's results
![App Screenshot](https://user-images.githubusercontent.com/62866353/126296932-0446553c-c4b3-40d6-8d82-e58c18f5b035.png)


  
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

- There is an in-built tester program which tests, the program for various types of regex, this is not implemented by me.
- To run tests, run the following commands, I used a windows machine, for linux machines use make instead of mingw32-make

```bash
  mingw32-make 
  test_regex.exe
```

  