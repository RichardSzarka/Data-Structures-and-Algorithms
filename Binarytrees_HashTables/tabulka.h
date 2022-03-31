#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define table_item_value(x) (hash_table->items[x]->value) //vrat mi hodnotu itemu v tabulke
#define table_item_key(x) (hash_table->items[x]->key) //vrat mi kluc itemu v tabulke


typedef struct item{ //struktura predmetu - prvku
    char * value;
    int key;
}ITEM;

typedef struct Hash_table{ //struktura tabulky
    int size;
    int occupied;
    ITEM ** items;
}TABLE;

TABLE * table_insert(TABLE * hash_table,char * value,int key,int count_collisions);
TABLE * hash_table_init(int size);
int hashing(int key,int size);
TABLE * resize(TABLE * hash_table);
char* search_table(TABLE * hash_table,int key);
int count_collision();