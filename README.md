# Algorithm and Data Structures Library in C

## About this project
This project is a portfolio piece to demonstrate proficiency in C programming, data structures, and algorithms. Built from scratch without external libraries, it showcases clean code, robust error handling, and testing, tailored for entry-level software engineering roles.

## Project Overview
A modular library implementing fundamental data structures (e.g., singly linked list) and algorithms, designed for Linux (WSL Debian) using C17. The project emphasizes:
- Clean, maintainable code with consistent style (Clang-Format).
- Robust error handling (null checks, memory safety, integer overflow).
- Comprehensive testing to ensure functionality.
- Git for version control with clear commit history.
- Universal Makefile for automated builds.

The library is a companion to a testing framework project, highlighting skills in systems programming, memory management, and algorithm design.

## Key Features
- **Data Structures**:
  - Singly linked list: (insert, delete, print, free);
  - Doubly linked list: (Insert, delete, print, free).
- **Algorithms**:
  - Bubble sort: Implementation for integer arrays with null and size checks (no tests or documentation yet).
- **Safety**: Null pointer checks, memory leak prevention, integer overflow handling.
- **Build System**: Universal Makefile compiles all `*.c` files.
- **Code Style**: Enforced via `.clang-format` (LLVM, 4-space indent, 80-column limit).
- **Version Control**: Git with branches:
  - `main`: Stable code.
  - `feature`: New containers/algorithms.
  - `refactor`: Safety and performance improvements.
  - `docs`: Documentation updates.
  - `clang`: Clang-Format changes.
  - `tests`: Test additions/changes.

## Skills Demonstrated
- **C Programming**: Pointers, memory management, modular design.
- **Data Structures**: Implementation of linked lists with plans for trees and graphs.
- **Error Handling**: Robust checks for edge cases and invalid inputs.
- **Testing**: Comprehensive test suites for each data structure.
- **Tools**: Git, Makefile, Clang-Format, Linux environment.
- **Documentation**: Doxygen-style comments in code; clear README for project overview.

## Build and Run
```bash
make
./launcher
