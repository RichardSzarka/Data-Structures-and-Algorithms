#include "tree.h"
#define _CRT_DEBUG_NO_WARNINGS



VERTEX * tree_init(VERTEX * root) //inicializuj prazdny strom
{
    root=(struct vertex*)malloc(sizeof(struct vertex));
    root->vertex_key=0;
    root->left=NULL;
    root->right=NULL;
    root->height=-1;
    return root;
}

VERTEX * create_vertex(int key,char * value) //vytvor vrchol s danou hodnotou
{
    VERTEX * node;
    node=(struct vertex*)malloc(sizeof(struct vertex));
    node->vertex_key=key;
    node->left=NULL;
    node->right=NULL;
    node->height=0;

    node->value=(char*)calloc(strlen(value),sizeof(char));
    strcpy(node->value,value);

    return node;
}

void update_height(VERTEX * act_root) //vrati vačšiu vysku deti
{
    act_root->height=HIGHER_child_height(act_root);
}

int count_rotation() //pocitanie rotacii
{
    int static rotation=0;
    rotation++;
    return rotation;
}

VERTEX * right_rotation(VERTEX * act_root) //rotacia doprava
{
    if(left_child(act_root)){ //zisti ci netreba najprv spravit rotaciu dolava
        if(VERTEX_balance(left_child(act_root))==RIGHT_HEAVY)
        {
            act_root->left=left_rotation(left_child(act_root));
        }
    }

    VERTEX * Y = act_root->left; //zrotuj
    VERTEX * Z = Y->right;
    Y->right=act_root;
    act_root->left=Z;


    update_height(act_root); //updateni vysky
    update_height(Y);
    count_rotation();
    return Y;

}

VERTEX * left_rotation(VERTEX * act_root) //rotacia dolava
{
    if(right_child(act_root)){ //zisti ci netreba spravit najprv rotaciu doprava
        if(VERTEX_balance(right_child(act_root))==LEFT_HEAVY)
        {
            act_root->right=right_rotation(right_child(act_root));
        }
    }

    VERTEX * Y = act_root->right; //zrotuj
    VERTEX * Z = Y->left;
    Y->left=act_root;
    act_root->right=Z;
    update_height(act_root); //aktualizuj vysky
    update_height(Y);
    count_rotation();
    return Y;
}

VERTEX * balancing(VERTEX * act_root) //zisti ci treba spravit vyvazovanie
{
    if (abs(VERTEX_balance(act_root))<=1) //netreba
        return act_root;
    else if(VERTEX_balance(act_root)>LEFT_HEAVY) //treba doprava
    {
        act_root=right_rotation(act_root);
    }
    else if(VERTEX_balance(act_root)<RIGHT_HEAVY) //treba dolava
    {
        act_root=left_rotation(act_root);
    }
    return act_root;
}

VERTEX * tree_insert(int key,char * value,VERTEX * act_root) //vkladanie vrchola
{
    if (act_root->height==-1)
    {
        act_root->vertex_key = key;
        act_root->value = value;
        act_root->height = 0;
        return act_root;
    } //ak je strom hned po incializacii

    if(key<=act_root->vertex_key) //ak je vkladana vertex_value mensia alebo rovna ako vertex_value na aktualnom vrchole -> chod do laveho dietata
    {
        if(act_root->left!=NULL){ //ak lave dieta existuje chod dalej
            act_root->left=tree_insert(key,value, act_root->left);
        }
        else{
            act_root->left=create_vertex(key, value); //ak lave dieta neexistuje vytvor tam vrchol s hodnotou
        }
    }
    else //ak je vkladana vertex_value vacsia ako vertex_value na aktualnom vrchole -> chod do praveho dietata
    {
        if(act_root->right!=NULL){ //ak prave dieta existuje chod dalej
            act_root->right=tree_insert(key,value, act_root->right);
        }
        else{ //ak prave dieta neexistuje vytvor novy vrchol s hodnotou
            act_root->right=create_vertex(key,value);
        }
    }
    update_height(act_root); // aktualizuj vysky vrcholov
    act_root=balancing(act_root);
    return act_root;

}


char* search_value(int key,VERTEX * act_root) //najdenie hodnoty v grafe
{
    if (act_root==NULL) //ak aktualny vrchol neexistuje vrat hodnotu 0
        return "Not found\n";
    else if(act_root->vertex_key == key) //ak// aktualny vrchol ma hodnotu hladanej hodnoty vrat 1
        return act_root->value;
    else if(key<act_root->vertex_key) //ak hladana vertex_value je mensia ako vertex_value vrcholu, chod do laveho dietata
        search_value(key, act_root->left);
    else //inac chod do praveho dietata
        search_value(key, act_root->right);
}

//vypis

void output(VERTEX * act_root)// vypis hodnot
{
#ifdef PREORDER

    if (act_root != NULL)
        {
            printf("%d (%d) (%s),", act_root->vertex_key, act_root->height,act_root->value);
            output(act_root->left);
            output(act_root->right);
        }

#elif defined(INORDER)

    if (act_root != NULL)
    {
        output(act_root->left);
        printf("%d (%d) (%s),", act_root->vertex_key, act_root->height,act_root->value);
        output(act_root->right);
    }

#elif defined(POSTORDER)

    if (act_root != NULL)
        {
            output(act_root->left);
            output(act_root->right);
            printf("%d (%d) (%s),", act_root->vertex_key, act_root->height,act_root->value);
        }
    }
#endif
}



