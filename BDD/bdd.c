#include "bdd.h"

/*
 Meno: Richard Szarka
 Fakulta: FIIT
 ID: 111958
 Dátum: 3.5.2021
  */

char*split_begin(char * vector){ //funkcia na rozdelenie vektora 1. polovica
    char*left=(char*)calloc(((strlen(vector)/2)+1),sizeof(char));
    for(int i=0;i<strlen(vector)/2;i++)
    {
        left[i]=vector[i];
    }
    left[strlen(vector)/2]='\0';
    return left;
}

char*split_end(char * vector){ //funkcia na rozdelenie vektora 2. polovica
    char*right=(char*)calloc(((strlen(vector)/2)+1),sizeof(char));
    int index=0;
    for(int i=strlen(vector)/2;i<strlen(vector);i++)
    {
        right[index]=vector[i];
        index++;
    }
    right[strlen(vector)/2]='\0';
    return right;
}

VERTEX * create_vertex(char * vector,VERTEX * parent){ //vytvorenie noveho uzla
    VERTEX * newVertex = (VERTEX*)malloc(sizeof(VERTEX)); //alokovanie pamati
    newVertex->Vector = (char*)malloc((strlen(vector)+1)*sizeof(char)); //alokovanie a pridanie daneho vektora
    newVertex->Vector = vector;
    newVertex->left = NULL;
    newVertex->right = NULL;
    newVertex->previous = NULL;
    newVertex->next = NULL;
    newVertex->parent = parent; //nastavenie rodica
    return newVertex;
}

void attach_siblings(VERTEX * left, VERTEX * right){ //prepojenie surodencov
    left->next = right;
    right->previous = left;
}

void BDD_make(BDD * diagram, VERTEX * root){ //vytvaranie BDD
    char * left; //stringy na rozdelene vektory
    char * right;

    left=split_begin(root->Vector); //rozdelenie vektora
    right=split_end(root->Vector);
    root->left=create_vertex(left,root); //vytvorenie nových uzlov
    root->right=create_vertex(right,root);
    attach_siblings(root->left,root->right); //prepojenie uzlov

    diagram->numberOfVertices++;
    diagram->numberOfVertices++;
    if(strlen(left)>1 && strlen(right)>1){ //ak to neboli dlzky vektora 1 chod dalej
        BDD_make(diagram,root->left);
        BDD_make(diagram,root->right);
    }
    else
        return;

}

void BDD_attach_neighbours(BDD * bdd, VERTEX * root){ //prepojenie surodencov na celej hlbke
    if(strlen(root->Vector)==1) //na poslednej hlbke neprepajaj (listy)
    {
        return;
    }
    if(RIGHT_CHILD(root)->next==NULL && root->next!=NULL){ //prepojenie
        RIGHT_CHILD(root)->next=root->next->left;
        root->next->left->previous= RIGHT_CHILD(root);
    }
    BDD_attach_neighbours(bdd,root->left); //rekurzia
    BDD_attach_neighbours(bdd,root->right);

}

BDD *BDD_create(BF * bfunkcia){ //vytvor BDD

    for(int i=0;i<strlen(bfunkcia->vector);i++){ //zisti ci vstup su spravne hodnoty
        if(bfunkcia->vector[i]=='1' || bfunkcia->vector[i]=='0'){
            continue;
        }
        else{
            printf("Nespravne zadany vstup\n");
            return NULL;

        }
    }

    int variables = log2(strlen(bfunkcia->vector));
    if(variables - floor(variables)){ //zisti ci je spravny pocet premennych na dlzku vektora
        printf("Zle zadany vstup");
        return NULL;
    }

    BDD * diagram = (BDD*)malloc(sizeof(BDD)); //alokacia

    VERTEX * root = (VERTEX*)malloc(sizeof(VERTEX)); //alokacia

    diagram->numberOfVertices=1; //pridaj bf
    root->Vector = (char*)malloc((strlen(bfunkcia->vector))*sizeof(char)+1); // \0
    root->Vector=strdup(bfunkcia->vector);

    root->parent=NULL; //inicializuj
    root->next=NULL;
    root->previous=NULL;

    diagram->root = root; //inicializuj
    diagram->numberOfVariables=variables;

    BDD_make(diagram,root); //vytvor diagram
    BDD_attach_neighbours(diagram, root); //prepoj vzdialenejsich surodencov (susedov)
    return diagram;


}

BDD * rule1 (BDD *bdd,VERTEX * root,VERTEX * zero,VERTEX * one,int * reduced){ //pravidlo 1
    VERTEX * free_node=NULL;
    if(strlen(LEFT_CHILD_VECTOR(root))==1) //ak je to list
    {
        if(strcmp(LEFT_CHILD_VECTOR(root),"1")==0) //ak je to 0 tak ho nahrad uzlom Zero, inac ho nahrad uzlom One
        {
            free_node=root->left;
            root->left=one;
            free(free_node->Vector);
            free(free_node);
            (*reduced)++;
        }
        else
        {
            free_node=root->left;
            root->left=zero;
            free(free_node->Vector);
            free(free_node);
            (*reduced)++;

        }
        if(strcmp(RIGHT_CHILD_VECTOR(root),"1")==0) //ak je to 0 tak ho nahrad uzlom Zero, inac ho nahrad uzlom One
        {
            free_node=root->right;
            root->right=one;
            free(free_node);
            (*reduced)++;

            return bdd;
        }
        else
        {
            free_node=root->right;
            root->right=zero;
            free(free_node);
            (*reduced)++;

            return bdd;
        }
    }
    else { //ak nie si na liste vnor sa hlbšie
        bdd=rule1(bdd,root->left,zero,one,&(*reduced));
        bdd=rule1(bdd,root->right,zero,one,&(*reduced));
    }
}

void merge_vertices(VERTEX * root, VERTEX * compare){
    if(PARENT(compare)->left==compare) //zisti ktore dieta bol rodiča
    {
        if(compare->next) //podla toho nastav rodicu nove dieta
            compare->next->previous=compare->previous;

        if(compare->previous)
            compare->previous->next=compare->next;
        PARENT(compare)->left=root;
    }
    else//zisti ktore dieta bol rodiča
    {
        if(compare->next)//podla toho nastav rodicu nove dieta
            compare->next->previous=compare->previous;

        if(compare->previous)
            compare->previous->next=compare->next;
        PARENT(compare)->right=root;
    }
}

BDD * rule2(BDD * bdd,VERTEX * root, int * reduced) //pravidlo 2
{
    if(root==bdd->root) //ak sa na koreni vrať sa (zacina od spodku)
        return bdd;
    else if (root->next==NULL) //chod o uroven vyssie ak si uplne vpravo
    {
        while(root->previous) //vrat sa dolava
        {
            root=root->previous;
        }
        bdd=rule2(bdd,root->parent,&(*reduced));
    }
    else {
        VERTEX *compare = root;
        VERTEX * free_node = root;
        int freed=0;
        while (compare->next != NULL) { //porovnavaj
            compare = compare->next;
            if(freed) //uvolni
            {
                freed=0;
                free(free_node->Vector);
                free(free_node);
            }
            free_node = compare;
            if ((strcmp(root->Vector, compare->Vector) == 0) && (root->left == compare->left) && (root->right == compare->right)) {//ak najdes zhodu
                merge_vertices(root, compare); //zjednot uzly
                (*reduced)++;
                freed=1; //pri dalšom cykle ho uvolni
            }
        }
        if (root->next) //chod na suseda doprava
            bdd = rule2(bdd, root->next,&(*reduced));
        else
            bdd = rule2(bdd, root,&(*reduced));
    }

}

BDD * rule3(BDD * bdd,VERTEX * root, int * reduced){ //pravidlo 3

    if(LEFT_CHILD(root)==NULL) //ak si na konci vrat sa
    {
        return bdd;
    }
    if((LEFT_CHILD(root)->left == LEFT_CHILD(root)->right) && (LEFT_CHILD(root)->left!=NULL)) //ak su zhodne deti
    {

        if(LEFT_CHILD(root)->previous) { //horizontálne prepájanie (uprava pre pravidlo 2)
            LEFT_CHILD(root)->previous->next = LEFT_CHILD(root)->next;
        }
        if(LEFT_CHILD(root)->next)
            LEFT_CHILD(root)->next->previous = LEFT_CHILD(root)->previous;

        if(LEFT_CHILD(root)== RIGHT_CHILD(root)) //ak by na 1. urovni boli rovnake
            RIGHT_CHILD(root)= LEFT_CHILD(root)->left;

        LEFT_CHILD(root)= LEFT_CHILD(root)->left;
        LEFT_CHILD(root)->parent=root;
        (*reduced)++;


    }
    if((RIGHT_CHILD(root)->left == RIGHT_CHILD(root)->right) && (RIGHT_CHILD(root)->left!=NULL)) //ak su zhodne deti
    {

        if(RIGHT_CHILD(root)->previous) //horizontálne prepájanie (uprava pre pravidlo 2)
            RIGHT_CHILD(root)->previous->next = RIGHT_CHILD(root)->next;
        if(RIGHT_CHILD(root)->next)
            RIGHT_CHILD(root)->next->previous = RIGHT_CHILD(root)->previous;

        if(RIGHT_CHILD(root)== LEFT_CHILD(root)) //ak by na 1. urovni boli rovnake
            LEFT_CHILD(root)= RIGHT_CHILD(root)->left;

        RIGHT_CHILD(root)= RIGHT_CHILD(root)->left; //prepojenie
        RIGHT_CHILD(root)->parent=root; //nastavenie rodica
        (*reduced)++;


    }
    bdd=rule3(bdd,root->left,&(*reduced)); //vnorenie sa dalej
    bdd=rule3(bdd,root->right,&(*reduced));
    return bdd;

}

VERTEX * init_const_vertex(char * value){ //inicializovanie konštantného uzla One a Zero
    VERTEX * const_vertex = (VERTEX*)malloc(sizeof(VERTEX));
    const_vertex->left=NULL;
    const_vertex->right=NULL;
    const_vertex->parent=NULL;
    const_vertex->next=NULL;
    const_vertex->previous=NULL;
    const_vertex->Vector=(char*)malloc(sizeof(char));
    const_vertex->Vector = strdup(value);
    return const_vertex;
}

VERTEX * get_left_bottom(VERTEX * root) //nastavenie sa dolava dole v BDD
{
    while(root->left->left!=NULL)
    {
        root=root->left;
    }
    return root;
}

int BDD_reduce(BDD *bdd){
    if(bdd==NULL)
    {
        return -1;
    }
    VERTEX * zero = init_const_vertex("0\0"); //inicializovanie konštantných uzlov
    VERTEX * one = init_const_vertex("1\0");
    VERTEX * help=NULL;
    int reduced=-2;

    bdd=rule1(bdd,bdd->root,zero,one,&reduced); //pravidlo 1
    for(int i=0;i<bdd->numberOfVariables;i++)
    {
        help=get_left_bottom(bdd->root);//pre pravidlo 2 chod uplne dolava dole
        bdd=rule2(bdd, help,&reduced);//pravidlo 2
        bdd=rule3(bdd,bdd->root,&reduced); //pravidlo 3
    }
    bdd->numberOfVertices=bdd->numberOfVertices-reduced; //aktualizuj

    return reduced;

}


char BDD_use(BDD *bdd, char *vstupy){//pouzitie diagramu
    VERTEX * root=bdd->root;
    for(int i=0;i<strlen(vstupy);i++){
        if(strlen(vstupy)-i==log2(strlen(root->Vector))) //moze sa stat ze premenna ostane nepotrebna a preskoci uroven
        {
            if(vstupy[i]=='0')
            {
                root=root->left;
            }
            else
                root=root->right;
        }
    }

    return root->Vector[0];
}

