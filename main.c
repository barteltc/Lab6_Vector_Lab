/**
 * File: main.c
 * Desc: MiniMat vector calculator
 * Author: Bartelt, Chase
 * Date: 09/29/2026
 *
 * Compile:
 * gcc -Wall -Wextra main.c struct.c -o minimat
 */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "struct.h"

#define MAX_VECTORS 10
#define INPUT_SIZE 100

// Vector storage
static vect vectors[MAX_VECTORS];
static bool used[MAX_VECTORS] = {false};

/**
 * Display help information.
 */
void printHelp(void)
{
    printf("MiniMat Vector Calculator\n");
    printf("name = x y z     Create or replace a vector\n");
    printf("name             Display a vector\n");
    printf("a + b            Add vectors\n");
    printf("a - b            Subtract vectors\n");
    printf("a * 2            Scalar multiplication\n");
    printf("2 * a            Scalar multiplication\n");
    printf("c = a + b        Store result of operation\n");
    printf("list             List all vectors\n");
    printf("clear            Clear all vectors\n");
    printf("quit             Quit\n");
}

/**
 * Display a vector.
 */
void printVector(const char *name, vect vector)
{
    printf("%s = %g %g %g\n",
           name,
           vector.data[0],
           vector.data[1],
           vector.data[2]);
}

/**
 * Find a vector by name.
 *
 * Returns the array index if found.
 * Returns -1 if not found.
 */
int findVector(const char *name)
{
    for (int i = 0; i < MAX_VECTORS; i++)
    {
        if (used[i] &&
            strcmp(vectors[i].name, name) == 0)
        {
            return i;
        }
    }

    return -1;
}

/**
 * Store a vector.
 *
 * If a vector with the same name exists,
 * replace it.
 *
 * Otherwise place it in the first empty location.
 *
 * Returns array index on success.
 * Returns -1 if memory is full.
 */
int storeVector(const char *name, vect value)
{
    int location = findVector(name);

    // Vector does not already exist.
    // Find first empty location.
    if (location == -1)
    {
        for (int i = 0; i < MAX_VECTORS; i++)
        {
            if (!used[i])
            {
                location = i;
                break;
            }
        }
    }

    // No available locations.
    if (location == -1)
    {
        return -1;
    }

    // Store vector.
    vectors[location] = value;

    strcpy(vectors[location].name, name);

    used[location] = true;

    return location;
}

/**
 * Clear all vectors from memory.
 */
void clearVectors(void)
{
    for (int i = 0; i < MAX_VECTORS; i++)
    {
        used[i] = false;

        vectors[i].name[0] = '\0';

        vectors[i].data[0] = 0.0;
        vectors[i].data[1] = 0.0;
        vectors[i].data[2] = 0.0;
    }
}

/**
 * Display all stored vectors.
 */
void listVectors(void)
{
    bool found = false;

    for (int i = 0; i < MAX_VECTORS; i++)
    {
        if (used[i])
        {
            printVector(vectors[i].name,
                        vectors[i]);

            found = true;
        }
    }

    if (!found)
    {
        printf("No vectors stored.\n");
    }
}

/**
 * Determine whether a string contains a number.
 *
 * If it does, place the number into value.
 */
bool parseNumber(const char *text, double *value)
{
    char extra;

    if (sscanf(text, "%lf%c",
               value, &extra) == 1)
    {
        return true;
    }

    return false;
}

//Perform a vector operation.
bool calculate(const char *left,
               char op,
               const char *right,
               vect *result)
{
    int leftIndex = findVector(left);
    int rightIndex = findVector(right);

    double number;

    // Addition
    if (op == '+')
    {
        if (leftIndex == -1 ||
            rightIndex == -1)
        {
            return false;
        }

        *result =
            add(vectors[leftIndex],
                vectors[rightIndex]);

        return true;
    }

    // Subtraction
    if (op == '-')
    {
        if (leftIndex == -1 ||
            rightIndex == -1)
        {
            return false;
        }

        *result =
            subtract(vectors[leftIndex],
                     vectors[rightIndex]);

        return true;
    }

    // Scalar multiplication
    if (op == '*')
    {
        // vector * number
        if (leftIndex != -1 &&
            parseNumber(right, &number))
        {
            *result =
                scalar(vectors[leftIndex],
                       number);

            return true;
        }

        // number * vector
        if (rightIndex != -1 &&
            parseNumber(left, &number))
        {
            *result =
                scalar(vectors[rightIndex],
                       number);

            return true;
        }
    }

    return false;
}

/**
 * Main user interface.
 */
void userInterface(void)
{
    char input[INPUT_SIZE];

    while (true)
    {
        printf("minimat> ");

        // Read input.
        if (fgets(input,
                  sizeof(input),
                  stdin) == NULL)
        {
            break;
        }

        // Remove newline.
        input[strcspn(input, "\n")] = '\0';

        // Ignore empty input.
        if (input[0] == '\0')
        {
            continue;
        }

        /* --------------------------------
           HELP
           -------------------------------- */
        if (strcmp(input, "h") == 0 ||
            strcmp(input, "-h") == 0)
        {
            printHelp();
            continue;
        }

        /* --------------------------------
           QUIT
           -------------------------------- */
        if (strcmp(input, "quit") == 0)
        {
            break;
        }

        /* --------------------------------
           CLEAR
           -------------------------------- */
        if (strcmp(input, "clear") == 0)
        {
            clearVectors();

            printf("Vector memory cleared.\n");

            continue;
        }

        /* --------------------------------
           LIST
           -------------------------------- */
        if (strcmp(input, "list") == 0)
        {
            listVectors();
            continue;
        }

        //Copy input
        char clean[INPUT_SIZE];

        strcpy(clean, input);

        for (int i = 0;
             clean[i] != '\0';
             i++)
        {
            if (clean[i] == ',')
            {
                clean[i] = ' ';
            }
        }

        // Variables used while parsing.
        char name[MAX_NAME];
        char resultName[MAX_NAME];

        char left[MAX_NAME];
        char right[MAX_NAME];

        char op;
        char extra;

        double x;
        double y;
        double z;

        //VECTOR CREATION
        if (sscanf(clean,
                   " %19s = %lf %lf %lf %c",
                   name,
                   &x,
                   &y,
                   &z,
                   &extra) == 4)
        {
            vect newVector = {0};

            newVector.data[0] = x;
            newVector.data[1] = y;
            newVector.data[2] = z;

            int location =
                storeVector(name,
                            newVector);

            if (location == -1)
            {
                printf("Vector memory full.\n");
            }
            else
            {
                printVector(
                    vectors[location].name,
                    vectors[location]);
            }

            continue;
        }

        //Operation and Assignment

        if (sscanf(clean,
                   " %19s = %19s %c %19s %c",
                   resultName,
                   left,
                   &op,
                   right,
                   &extra) == 4)
        {
            vect result = {0};

            bool valid =
                calculate(left,
                          op,
                          right,
                          &result);

            if (!valid)
            {
                printf("Invalid operation or vector does not exist.\n");
                continue;
            }

            int location =
                storeVector(resultName,
                            result);

            if (location == -1)
            {
                printf("Vector memory full.\n");
            }
            else
            {
                printVector(
                    vectors[location].name,
                    vectors[location]);
            }

            continue;
        }

        //Operation without assignment
        if (sscanf(clean,
                   " %19s %c %19s %c",
                   left,
                   &op,
                   right,
                   &extra) == 3)
        {
            vect result = {0};

            bool valid =
                calculate(left,
                          op,
                          right,
                          &result);

            if (valid)
            {
                printVector("ans",
                            result);
            }
            else
            {
                printf("Invalid operation or vector does not exist.\n");
            }

            continue;
        }

        //DISPLAY SINGLE VECTOR
        if (sscanf(clean,
                   " %19s %c",
                   name,
                   &extra) == 1)
        {
            int location =
                findVector(name);

            if (location == -1)
            {
                printf("Vector '%s' does not exist.\n",
                       name);
            }
            else
            {
                printVector(
                    vectors[location].name,
                    vectors[location]);
            }

            continue;
        }

        // Nothing matched.
        printf("Invalid command.\n");
    }
}

/**
 * Main
 */
int main(int argc, char *argv[])
{
    // Command line help.
    if (argc > 1)
    {
        if (argc == 2 &&
            strcmp(argv[1], "-h") == 0)
        {
            printHelp();
            return 0;
        }

        printf("Usage: %s [-h]\n",
               argv[0]);

        return 1;
    }

    // Start calculator.
    userInterface();

    return 0;
}