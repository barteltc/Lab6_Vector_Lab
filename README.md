# MiniMat - Vector Calculator
**CPE-2600 Systems Programming - Lab 7**

**Author:** Chase Bartelt

## Description

Vector calculator written in C that Allows users to create, store, and do math operations on vectors.

This update added dynamic memory, more vector storage and CSV file I/O

## Features

- Create and store named 3D vectors.
- Do math operations on the stored vectors
- Save vectors dynamically with options for clearing and listing
- Dynamically expand vector storage.
- Load and save vectors using CSV files.

## Building the Program

The program can be compiled using GCC or the provided Makefile.

**Using Make:**

```bash
make clean
make
```

**Compiling manually:**

```bash
gcc -Wall -Wextra main.c struct.c -o minimat
```

## Running the Program

After compiling, run the program using:

```bash
./minimat
```

To display the available commands without entering the calculator:

```bash
./minimat -h
```

The `-h` flag is the only supported command-line option. All other operations are entered interactively after launching the program.

## Supported Commands

### Vector Operations

- `a = 1 2 3` - Create or replace vector `a`.
- `a` - Display the contents of vector `a`.
- `a + b` - Add two vectors.
- `a - b` - Subtract two vectors.
- `a * 2` - Multiply a vector by a scalar.
- `2 * a` - Multiply a scalar by a vector.
- `a . b` - Calculate the dot product.
- `a x b` - Calculate the cross product.
- `c = a + b` - Store the result of an operation as vector `c`.

### Memory and File Commands

- `list` - Display all stored vectors.
- `clear` - Remove all vectors and release allocated memory.
- `load filename.csv` - Load vectors from a CSV file.
- `save filename.csv` - Save all stored vectors to a CSV file.
- `h` or `-h` - Display the help menu.
- `quit` - Exit the program.

## Dynamic Memory

The original vector calculator used a fixed array that could store a maximum of 10 vectors.

Lab 7 replaces this with a dynamically allocated array.

- `malloc()` initially allocates enough memory for 10 vectors.
- `realloc()` expands the array by 25 additional vector slots whenever the current capacity is reached.
- The program tracks the number of stored vectors and the current allocated capacity.
- `free()` releases allocated memory when the `clear` command is used.
- Remaining allocated memory is released when the program exits.

This allows the program to store additional vectors as needed, limited by available system memory.

## Source Files

- `main.c` - Main program, command processing, dynamic storage, and file I/O.
- `struct.c` - Vector mathematical operations.
- `struct.h` - Vector structures and function declarations.
- `Makefile` - Build instructions for compiling the program.

## Lab 7 Updates

1. Replacing fixed vector storage with dynamic memory allocation.
4. Adding CSV file loading and saving.
5. Handling missing files and invalid CSV data.
