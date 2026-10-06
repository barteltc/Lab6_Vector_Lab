/**
 * File: struct.c
 * Desc: Vector math functions
 * Author: Bartelt, Chase
 * Date: 09/29/2026
 *
 * Compile:
 * gcc -Wall -Wextra struct.c -c
 */

#include "struct.h"

vect add(vect a, vect b)
{
    vect returnval = {0};

    for (int i = 0; i < 3; i++)
    {
        returnval.data[i] = a.data[i] + b.data[i];
    }

    return returnval;
}

vect subtract(vect a, vect b)
{
    vect returnval = {0};

    for (int i = 0; i < 3; i++)
    {
        returnval.data[i] = a.data[i] - b.data[i];
    }

    return returnval;
}

vect crossProduct(vect a, vect b)
{
    vect returnval = {0};

    returnval.data[0] =
        a.data[1] * b.data[2] -
        a.data[2] * b.data[1];

    returnval.data[1] =
        a.data[2] * b.data[0] -
        a.data[0] * b.data[2];

    returnval.data[2] =
        a.data[0] * b.data[1] -
        a.data[1] * b.data[0];

    return returnval;
}

double dotProduct(vect a, vect b)
{
    double returnval;

    returnval =
        a.data[0] * b.data[0] +
        a.data[1] * b.data[1] +
        a.data[2] * b.data[2];

    return returnval;
}

vect scalar(vect a, double b)
{
    vect returnvalue = {0};

    for (int i = 0; i < 3; i++)
    {
        returnvalue.data[i] = a.data[i] * b;
    }

    return returnvalue;
}