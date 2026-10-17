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
#include <stdlib.h>

#include "struct.h"

#define INPUT_SIZE 100
#define LINE_SIZE 1024
#define INITIAL_CAPACITY 10
#define GROWTH_AMOUNT 25

// Dynamically allocated vector storage.
static vect *vectors = NULL;
static int capacity = 0;
static int count = 0;

/** Display help information. */
void printHelp(void)
{
    printf("MiniMat Vector Calculator\n");
    printf("name = x y z      Create or replace a vector\n");
    printf("name              Display a vector\n");
    printf("a + b             Add vectors\n");
    printf("a - b             Subtract vectors\n");
    printf("a * 2             Scalar multiplication\n");
    printf("2 * a             Scalar multiplication\n");
    printf("a . b             Dot product\n");
    printf("a x b             Cross product\n");
    printf("c = a + b         Store result of a vector operation\n");
    printf("list              List all vectors\n");
    printf("load <filename>   Load vectors from CSV file\n");
    printf("save <filename>   Save vectors to CSV file\n");
    printf("clear             Clear all vectors and release memory\n");
    printf("quit              Quit\n");
}

/** Display a vector. */
void printVector(const char *name, vect vector)
{
    printf("%s = %g %g %g\n",
           name, vector.data[0], vector.data[1], vector.data[2]);
}

/** Find a vector by name. Return its index or -1 if not found. */
int findVector(const char *name)
{
    for (int i = 0; i < count; i++)
    {
        if (strcmp(vectors[i].name, name) == 0)
        {
            return i;
        }
    }

    return -1;
}

/**
 * Store a vector, replacing an existing vector with the same name.
 * Grows the array when needed. Returns index on success or -1 on failure.
 */
int storeVector(const char *name, vect value)
{
    int location = findVector(name);

    if (location == -1)
    {
        // Need a new slot; expand the allocated array if necessary.
        if (count >= capacity)
        {
            int newCapacity = (capacity == 0)
                                  ? INITIAL_CAPACITY
                                  : capacity + GROWTH_AMOUNT;

            vect *temp = realloc(vectors,
                                 (size_t)newCapacity * sizeof(*vectors));
            if (temp == NULL)
            {
                printf("Memory allocation failed.\n");
                return -1;
            }

            vectors = temp;
            capacity = newCapacity;
        }

        location = count;
        count++;
    }

    vectors[location] = value;
    strcpy(vectors[location].name, name);
    return location;
}

/** Release all vector memory and forget the stored vectors. */
void clearVectors(void)
{
    free(vectors);
    vectors = NULL;
    capacity = 0;
    count = 0;
}

/** Display all stored vectors. */
void listVectors(void)
{
    if (count == 0)
    {
        printf("No vectors stored.\n");
        return;
    }

    for (int i = 0; i < count; i++)
    {
        printVector(vectors[i].name, vectors[i]);
    }
}

/** Parse a complete string as a number. */
bool parseNumber(const char *text, double *value)
{
    char extra;
    return sscanf(text, "%lf %c", value, &extra) == 1;
}

/** Perform a vector operation. Dot products print their scalar result. */
bool calculate(const char *left, char op, const char *right, vect *result)
{
    int leftIndex = findVector(left);
    int rightIndex = findVector(right);
    double number;

    if (op == '+')
    {
        if (leftIndex == -1 || rightIndex == -1)
            return false;

        *result = add(vectors[leftIndex], vectors[rightIndex]);
        return true;
    }

    if (op == '-')
    {
        if (leftIndex == -1 || rightIndex == -1)
            return false;

        *result = subtract(vectors[leftIndex], vectors[rightIndex]);
        return true;
    }

    if (op == '*')
    {
        if (leftIndex != -1 && parseNumber(right, &number))
        {
            *result = scalar(vectors[leftIndex], number);
            return true;
        }

        if (rightIndex != -1 && parseNumber(left, &number))
        {
            *result = scalar(vectors[rightIndex], number);
            return true;
        }

        return false;
    }

    if (op == '.')
    {
        if (leftIndex == -1 || rightIndex == -1)
            return false;

        double dot = dotProduct(vectors[leftIndex], vectors[rightIndex]);
        printf("Dot product: %g\n", dot);
        return true;
    }

    if (op == 'x')
    {
        if (leftIndex == -1 || rightIndex == -1)
            return false;

        *result = crossProduct(vectors[leftIndex], vectors[rightIndex]);
        return true;
    }

    return false;
}

/** Load comma-separated name,x,y,z rows, merging with stored vectors. */
int load(const char *filename)
{
    FILE *fpt = fopen(filename, "r");
    if (fpt == NULL)
    {
        perror(filename);
        return -1;
    }

    char line[LINE_SIZE];
    int loaded = 0;
    int lineNumber = 0;

    while (fgets(line, sizeof(line), fpt) != NULL)
    {
        lineNumber++;

        // Ignore blank lines (including CRLF line endings).
        if (strspn(line, " \t\r\n") == strlen(line))
            continue;

        // Skip and report overlong lines instead of parsing partial rows.
        if (strchr(line, '\n') == NULL && !feof(fpt))
        {
            int ch;
            while ((ch = fgetc(fpt)) != '\n' && ch != EOF)
            {
                // Discard the remainder of this line.
            }
            printf("Skipping invalid CSV row %d (too long).\n", lineNumber);
            continue;
        }

        char name[MAX_NAME];
        vect value = {0};
        char extra;

        // Read exactly four comma-separated fields; reject extra text.
        int fields = sscanf(line, " %19[^, \t\r\n] , %lf , %lf , %lf %c",
                            name,
                            &value.data[0],
                            &value.data[1],
                            &value.data[2],
                            &extra);

        if (fields != 4)
        {
            printf("Skipping invalid CSV row %d.\n", lineNumber);
            continue;
        }

        if (storeVector(name, value) == -1)
        {
            fclose(fpt);
            return -1;
        }

        loaded++;
    }

    if (ferror(fpt))
    {
        perror("Error reading file");
        fclose(fpt);
        return -1;
    }

    fclose(fpt);
    printf("Loaded %d vectors.\n", loaded);
    return loaded;
}

/** Save stored vectors as CSV (overwriting any existing file). */
int save(const char *filename)
{
    FILE *fpt = fopen(filename, "w");
    if (fpt == NULL)
    {
        perror(filename);
        return -1;
    }

    for (int i = 0; i < count; i++)
    {
        if (fprintf(fpt, "%s,%.17g,%.17g,%.17g\n",
                    vectors[i].name,
                    vectors[i].data[0],
                    vectors[i].data[1],
                    vectors[i].data[2]) < 0)
        {
            perror("Error writing file");
            fclose(fpt);
            return -1;
        }
    }

    if (fclose(fpt) != 0)
    {
        perror("Error closing file");
        return -1;
    }

    printf("Saved %d vectors to %s\n", count, filename);
    return count;
}

/** Read and execute calculator commands. */
void userInterface(void)
{
    char input[INPUT_SIZE];

    while (true)
    {
        printf("minimat> ");

        if (fgets(input, sizeof(input), stdin) == NULL)
            break;

        input[strcspn(input, "\n")] = '\0';
        if (input[0] == '\0')
            continue;

        if (strcmp(input, "h") == 0 || strcmp(input, "-h") == 0)
        {
            printHelp();
            continue;
        }

        if (strcmp(input, "quit") == 0)
            break;

        if (strcmp(input, "clear") == 0)
        {
            clearVectors();
            printf("Vector memory cleared.\n");
            continue;
        }

        if (strcmp(input, "list") == 0)
        {
            listVectors();
            continue;
        }

        // Filenames must be given on the same line, including any extension.
        if (strncmp(input, "load", 4) == 0 &&
            (input[4] == '\0' || input[4] == ' ' || input[4] == '\t'))
        {
            char filename[INPUT_SIZE];
            char extra;
            if (sscanf(input + 4, " %99s %c", filename, &extra) == 1)
                load(filename);
            else
                printf("Usage: load <filename>\n");
            continue;
        }

        if (strncmp(input, "save", 4) == 0 &&
            (input[4] == '\0' || input[4] == ' ' || input[4] == '\t'))
        {
            char filename[INPUT_SIZE];
            char extra;
            if (sscanf(input + 4, " %99s %c", filename, &extra) == 1)
                save(filename);
            else
                printf("Usage: save <filename>\n");
            continue;
        }

        // The interactive syntax also allows commas between coordinates.
        char clean[INPUT_SIZE];
        strcpy(clean, input);
        for (int i = 0; clean[i] != '\0'; i++)
        {
            if (clean[i] == ',')
                clean[i] = ' ';
        }

        char name[MAX_NAME];
        char resultName[MAX_NAME];
        char left[MAX_NAME];
        char right[MAX_NAME];
        char op, extra;
        double x, y, z;

        // Create or replace: a = 1 2 3
        if (sscanf(clean, " %19s = %lf %lf %lf %c",
                   name, &x, &y, &z, &extra) == 4)
        {
            vect value = {0};
            value.data[0] = x;
            value.data[1] = y;
            value.data[2] = z;

            int location = storeVector(name, value);
            if (location == -1)
                printf("Unable to store vector.\n");
            else
                printVector(vectors[location].name, vectors[location]);
            continue;
        }

        // Assign a vector calculation: c = a + b
        if (sscanf(clean, " %19s = %19s %c %19s %c",
                   resultName, left, &op, right, &extra) == 4)
        {
            if (op == '.')
            {
                printf("Cannot assign a dot product to a vector.\n");
                continue;
            }

            vect result = {0};
            if (!calculate(left, op, right, &result))
            {
                printf("Invalid operation or vector does not exist.\n");
                continue;
            }

            int location = storeVector(resultName, result);
            if (location == -1)
                printf("Unable to store vector.\n");
            else
                printVector(vectors[location].name, vectors[location]);
            continue;
        }

        // Calculate without assignment: a + b, a . b, 2 * a
        if (sscanf(clean, " %19s %c %19s %c",
                   left, &op, right, &extra) == 3)
        {
            vect result = {0};
            if (calculate(left, op, right, &result))
            {
                if (op != '.')
                    printVector("ans", result);
            }
            else
            {
                printf("Invalid operation or vector does not exist.\n");
            }
            continue;
        }

        // Display a single vector by name.
        if (sscanf(clean, " %19s %c", name, &extra) == 1)
        {
            int location = findVector(name);
            if (location == -1)
                printf("Vector '%s' does not exist.\n", name);
            else
                printVector(vectors[location].name, vectors[location]);
            continue;
        }

        printf("Invalid command.\n");
    }
}

int main(int argc, char *argv[])
{
    // Parse command-line arguments before allocating any vector memory.
    if (argc > 1)
    {
        if (argc == 2 && strcmp(argv[1], "-h") == 0)
        {
            printHelp();
            return 0;
        }

        printf("Usage: %s [-h]\n", argv[0]);
        return 1;
    }

    // Allocate storage for the first ten vectors.
    vectors = malloc(INITIAL_CAPACITY * sizeof(*vectors));
    if (vectors == NULL)
    {
        printf("Memory allocation failed.\n");
        return 1;
    }
    capacity = INITIAL_CAPACITY;

    userInterface();

    // Also safe if 'clear' was the last command (free(NULL) is allowed).
    free(vectors);
    return 0;
}
