/**
 * File: struct.h
 * Desc: Header file for MiniMat vector calculator
 * Author: Bartelt, Chase
 * Date: 09/29/2026
 */

#ifndef STRUCT_H
#define STRUCT_H

#define MAX_NAME 20

typedef struct
{
    char name[MAX_NAME];
    double data[3];
} vect;

vect add(vect a, vect b);
vect subtract(vect a, vect b);
vect crossProduct(vect a, vect b);
double dotProduct(vect a, vect b);
vect scalar(vect a, double b);

#endif