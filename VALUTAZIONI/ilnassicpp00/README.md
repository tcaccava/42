This project has been created as part of the 42 curriculum by ilnassi

Project Structure

cpp_module_00/
├── ex00/
│   ├── Makefile
│   └── megaphone.cpp
├── ex01/
│   ├── Makefile
│   ├── main.cpp
│   ├── Contact.hpp
│   ├── Contact.cpp
│   ├── PhoneBook.hpp
│   └── PhoneBook.cpp
└── ex02/
    ├── Makefile
    ├── Account.hpp
    ├── Account.cpp
    ├── 19920104_091532.log
    └── tests.cpp

Description
This module is the entry point into object-oriented programming with C++. Coming from C, the goal is to understand the fundamental differences between the two languages and to start thinking in terms of classes, objects, and encapsulation.

The module covers three exercises of increasing complexity, each introducing new concepts that form the foundation of every C++ project that follows.

Concepts Studied
Streams — goodbye printf
In C++ you no longer use printf or scanf. Instead:

std::cout << "Hello World" << std::endl;  // output
std::cin  >> variable;                     // input
std::getline(std::cin, line);              // read full line
Classes — the core of C++
A class is a blueprint for creating objects. It groups data and the functions that operate on that data.

class MyClass {
private:
    int _value;       // only the class can access this

public:
    MyClass(int v);   // constructor
    ~MyClass();       // destructor
    int getValue() const;  // getter
};

Private vs Public:

private — data hidden from the outside world, accessible only through methods
public — the interface exposed to the rest of the program
Constructors and Destructors
Constructor — called automatically when an object is created
Destructor — called automatically when an object is destroyed
Initialization list — the correct C++ way to initialize attributes:
MyClass::MyClass(int v) : _value(v) {}

Static Members
Static attributes and methods belong to the class itself, not to any individual object. All instances share the same static data.

static int _nbAccounts;       // shared by ALL Account objects
static int getTotalAmount();  // callable without an instance
Formatting with <iomanip>
Used to control how output is displayed, particularly for table-like formatting:

std::cout << std::setw(10) << std::right << text << "|";  // right-aligned, 10 chars wide
std::cout << std::setfill('0') << std::setw(2) << month;  // zero-padded: "09" not " 9"
Exercises

Exercise 00 — Megaphone
Directory: ex00/

A warm-up exercise. The program takes command-line arguments and prints them in uppercase. If no arguments are given, it prints a feedback noise message.

Concepts: argc/argv, std::toupper, std::cout

./megaphone "hello world"
# HELLO WORLD

./megaphone
# * LOUD AND UNBEARABLE FEEDBACK NOISE *


Exercise 01 — My Awesome PhoneBook
Directory: ex01/

A command-line phonebook that supports three commands: ADD, SEARCH, and EXIT. Built using two classes — Contact and PhoneBook.

Concepts: classes, private/public, getters/setters, <iomanip>, circular buffer

Key rules:

Maximum 8 contacts — the oldest is replaced when a 9th is added
No dynamic memory allocation
SEARCH displays contacts in a formatted table (10-char columns, right-aligned, truncated with . if too long)
     Index| First Name|  Last Name|   Nickname|
         0|      Mario|      Rossi|       Micio|
         1|     Luigi.|     Verdi.|    Pallina.|

Exercise 02 — The Job Of Your Dreams
Directory: ex02/

A reverse-engineering exercise. Account.cpp was accidentally deleted. Using the provided Account.hpp, tests.cpp, and a log file, the task is to reconstruct the missing implementation so that the program's output matches the log exactly (timestamps excluded).

Concepts: static members, constructors/destructors, <ctime>, log-driven development

[timestamp] index:0;amount:42;created
[timestamp] accounts:8;total:20049;deposits:0;withdrawals:0
[timestamp] index:0;p_amount:47;withdrawal:refused
[timestamp] index:0;amount:47;closed
How to Compile and Run
Each exercise has its own Makefile. From inside the exercise directory:

make          # compile
make clean    # remove object files
make fclean   # remove object files and executable
make re       # recompile from scratch


Compilation flags used:

-Wall -Wextra -Werror -std=c++98
What is Forbidden
printf, scanf, malloc, free — use C++ equivalents
using namespace and friend keywords
STL containers and algorithms (available only from Module 08)
C++11 or later features — strictly C++98
