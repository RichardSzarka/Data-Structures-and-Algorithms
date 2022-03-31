#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

/*
 Meno: Richard Szarka
 Fakulta: FIIT
 ID: 111958
 Dátum: 3.5.2021
  */

#define LEFT_CHILD_VECTOR(x) ((x)->left->Vector) //vektor laveho dietata
#define RIGHT_CHILD_VECTOR(x) ((x)->right->Vector) // vektor praveho dietata
#define LEFT_CHILD(x) ((x)->left) //lave dieta
#define RIGHT_CHILD(x) ((x)->right) //prave dieta
#define PARENT(x) ((x)->parent) //rodic

typedef struct Vertex{ //štruktúra na jeden uzol
    struct Vertex * left;
    struct Vertex * right;
    struct Vertex * parent;
    struct Vertex * previous;
    struct Vertex * next;
    char * Vector;

}VERTEX;

typedef struct BDD{ //štruktúra diagramu
    int numberOfVertices;
    int numberOfVariables;
    VERTEX * root;
}BDD;

typedef struct bool_func{ //boolovska funkcia
    char* vector;
}BF;

BDD *BDD_create(BF * bfunkcia); //vytvor BDD
int BDD_reduce(BDD *bdd);
char BDD_use(BDD *bdd, char *vstupy);//pouzitie diagramu


