#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define left_child_height(x) (((x)->left==NULL) ? -1 : ((x)->left->height)) // vráť hodnotu lavého dieťata
#define right_child_height(x) (((x)->right==NULL) ? -1 : ((x)->right->height)) // vráť hodnotu praveho dieťaťa
#define left_child(x) ((x)->left) //lave dieťa
#define right_child(x) ((x)->right) //prave dieťa
#define VERTEX_balance(x) ((left_child_height(x))-(right_child_height(x))) //zisti rovnovahu na danom vrchole
#define HIGHER_child_height(x) ((left_child_height(x))>(right_child_height(x)) ? (left_child_height(x)+1) : (right_child_height(x)+1) ) //vrať väčšiu vyšku detí

#define INORDER // druh vypisu
#define LEFT_HEAVY 1 //hodnota pri ktorej je viac váhy nalavo
#define RIGHT_HEAVY (-1) //hodnota pri ktorej je viac váhy napravo

//struktura vrchola
typedef struct vertex{

    int vertex_key; //kluc
    int height; //vyska
    char * value; //hodnota
    struct vertex * left; //lave dieta
    struct vertex * right; //prave dieta

}VERTEX;

VERTEX * tree_init(VERTEX * root);

VERTEX * create_vertex(int key,char * value);

void update_height(VERTEX * act_root);

VERTEX * right_rotation(VERTEX * act_root);

VERTEX * left_rotation(VERTEX * act_root);

VERTEX * balancing(VERTEX * act_root);

VERTEX * tree_insert(int key,char * value,VERTEX * act_root);

char* search_value(int key,VERTEX * act_root);

void output(VERTEX * act_root);

int count_rotation();
