# Coding Guideline

This document contains some notes and remarks about the coding style applied in
this project.

## Commenting

Comments fulfill two main goals: 

* Explain how to use the code. Many parts / components of the project might be
  re-used in other projects, so they have to be documented in a way that
  explains their use, their interfaces and the contraints to observe.
  
* Explain the internal implementation of components. This helps other developers
  to be able to understand the internal functionality, e.g. when extending the
  components. 
   
To deal with these points, these are the rules:

* Modules, function prototypes and datatypes are documented in the header files
  with doxygen style comments. Explains how to use and what to expect.

* Internal implementation details are documented with normal C style comments
  in the *.c / *.cpp files. Explains how and why it works.
  
## Code standard

As the Arduino framework uses C++, all source files in this project have the cpp
extension. Still, the source code is written in plain C, so it can be re-used in
other projects, too.
