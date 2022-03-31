#include "tabulka.h"
#include "tree.h"
#include <time.h>

#define ELEMENTS 1000000 //pocet prvkov
#define MAX_KEY 1000000 //kluce
#define MIN_KEY 1
#define TABLE_SIZE 1 //inicializacna velkost tabulky (mojej)

void test_tree(VERTEX* my_tree,int * random)
{
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n---------------My AVL TREE--------------\n"); //vygeneruj nahodne prvky
    clock_t t;

    t = clock();
    for(int i=0;i<ELEMENTS;i++) //vloz nahodne prvky
    {
        my_tree=tree_insert(random[i],"test",my_tree);
    }
    t=clock()-t;
    double time = ((double)t)/CLOCKS_PER_SEC;
    printf("Insert of %d values, time: %f\n",ELEMENTS,time);

    t = clock();
    for(int i=0;i<ELEMENTS;i++) //vyhladaj prvky
    {
        search_value(random[i],my_tree);
    }
    t=clock()-t;
    time = ((double)t)/CLOCKS_PER_SEC;
    printf("Searching every value of %d values, time: %f\n",ELEMENTS,time);

    int random_element;
    t = clock();
    for(int i=0;i<ELEMENTS/10;i++) //vyhladaj prvky
    {
        random_element = rand() % (ELEMENTS);
        search_value(random[random_element],my_tree);
    }
    t=clock()-t;
    time = ((double)t)/CLOCKS_PER_SEC;
    printf("Searching %d values of %d values, time: %f\n",ELEMENTS/10,ELEMENTS,time);

    printf("Number of rotation: %d\n\n",count_rotation()-1);

}

void test_table(TABLE * my_table,int * random)
{
    printf("---------My LINEAR PROBING TABLE--------\n"); //vygeneruj nahodne prvky
    clock_t t;

    t = clock();
    for(int i=0;i<ELEMENTS;i++) //vloz nahodne prvky
    {
        my_table=table_insert(my_table,"test",random[i],1);
    }
    t=clock()-t;
    double time = ((double)t)/CLOCKS_PER_SEC;
    printf("Insert time: %f\n",time);

    t = clock();
    for(int i=0;i<ELEMENTS;i++) //vyhladaj prvky
    {
        search_table(my_table,random[i]);
    }
    t=clock()-t;
    time = ((double)t)/CLOCKS_PER_SEC;
    printf("Searching every value time: %f\n",time);

    int random_element;
    t = clock();
    for(int i=0;i<ELEMENTS/10;i++) //vyhladaj prvky
    {
        random_element = rand() % (MAX_KEY)+MIN_KEY;
        search_table(my_table,random_element);
    }
    t=clock()-t;
    time = ((double)t)/CLOCKS_PER_SEC;
    printf("Searching %d values of %d values, time: %f\n",ELEMENTS/10,ELEMENTS,time);

    printf("Number of collisions: %d\n\n",count_collision()-1);


}

int main() {

    int * random;
    srand(time(0));
    random=(int*)calloc(ELEMENTS,sizeof(int));
    for(int i=0;i<ELEMENTS;i++) //vygeneruj nahodne prvky
    {
        int temp = rand() % (MAX_KEY)+MIN_KEY;
        temp=temp<<5;
        random[i]=temp+rand() % (MAX_KEY+1)-1+MIN_KEY;
    }

    VERTEX * my_tree=NULL;
    my_tree=tree_init(my_tree);
    test_tree(my_tree,random);

    TABLE * my_table=NULL;
    my_table=hash_table_init(TABLE_SIZE);

    test_table(my_table,random);

}
