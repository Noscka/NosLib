# Library Rules
a simple rule book so I remember how I should name functions, variables and etc.
1.	Namespaces, classes and functions must start with capital letter
2.	namespaces (if it will help), classes, functions and variables must need comments or summaries to help understand what they do
4.	Summaries require "- " at the start of every \<Param\> Section
5.	If function has default. then include (default = {DefaultValue}) before the "- "
6.	Function Parameters must start with lowercase letter
7.	Do not use #param region inside functions as it lowers readablity
8.	Class and Enum Members/attributes must start with capital letter
9.	Experimental features must be in the header file to which they relate
10.	Try to make functions as widely usable as possible (instead of making a function for each string type to center it, make a templated function that can take in any char type)
11.	Adding `TODO: {comment describing what needs to be done}` and `WARNING: {comment describing why this might becoming a problem}` in comments to either tag a line of code or an area of code for either something to be done or for something that might become a problem
