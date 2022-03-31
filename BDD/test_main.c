#include "bdd.h"
#include <time.h>

/*
 Meno: Richard Szarka
 Fakulta: FIIT
 ID: 111958
 Dátum: 3.5.2021
*/

#define VARIABLES 17 //pocet premennych
#define DIAGRAM_NUM 1 //pocet diagramov

char * binary_var(int number, int threshold) //vypisovanie binarnych cisel po nejaky index
{
    char* string=(char*)calloc(threshold+1,sizeof(char));
    int index=0;
    for (int i = threshold-1; i >= 0; i--) {
        string[index]=((number >> i) & 1)+48;
        index++;
    }
    string[threshold]='\0';
    return string;
}

char ** generate_var(int variables) //generovanie hodnot premennych
{
    int number=pow(2,variables);
    char ** field;

    field=(char**)calloc(number,sizeof(char*));
    for(int i=0;i<number;i++){
        field[i]=(char*)calloc(variables,sizeof(char));
        field[i]=binary_var(i,variables);
    }

    return field;
}

char * generate_random_vector(int variables){ //generovanie nahodných vektorov
    int func_length=pow(2,variables);
    char*random=(char*)calloc((func_length+1),sizeof(char));
    for(int i=0;i<pow(2,variables);i++)
    {
        int temp = rand() % (10000000000);
        random[i]=temp%2+48;
    }
    random[func_length]='\0';
    return random;
}

int count_vertices(BDD * bdd) //funkcia na spocitanie uzlov po zredukovani
{
    int count=1;
    int which_side=0;
    VERTEX * root=bdd->root;
    while(strlen(root->Vector)!=1)
    {
        if(which_side){
            while(root->next){
                root=root->next;
                count++;
            }
        }
        else{
            while(root->previous){
                root=root->previous;
                count++;
            }
        }
        count++;
        if(!which_side){
            root=root->left;
            which_side=1;
        }
        else{
            root=root->right;
            which_side=0;
        }

    }
    return count;
}


void test(BDD * bdd,BF * function) //tester
{
    int error=0;
    int newVertices=0;
    int oldvertices=0;
    clock_t t;
    clock_t create;
    clock_t reduce;
    clock_t check;
    double time;
    char**field_variables = generate_var(VARIABLES);
    double * reduce_percentage = (double*)calloc(DIAGRAM_NUM,sizeof (double));
    double * create_times=(double*)calloc(DIAGRAM_NUM,sizeof (double));
    double * reduce_times=(double*)calloc(DIAGRAM_NUM,sizeof (double));
    double * check_times=(double*)calloc(DIAGRAM_NUM,sizeof (double));
    double * whole_times=(double*)calloc(DIAGRAM_NUM,sizeof (double));
    for(int j=0;j<DIAGRAM_NUM;j++) {
        function->vector = strdup(generate_random_vector(VARIABLES));
        t = clock();

        create = clock();
        bdd = BDD_create(function);
        create = clock()-create;
        oldvertices=bdd->numberOfVertices;

        reduce=clock();
        BDD_reduce(bdd);
        reduce=clock()-reduce;

        check=clock();
        for (int i = 0; i < pow(2, VARIABLES); i++) { //pouzitie
            if (BDD_use(bdd, field_variables[i]) != function->vector[i]) {
                printf("ERROR");
                error = 1;
            }
        }


        check=clock()-check; //vypocty casov uz len dalej
        t = clock() - t;
        time = ((double) t) / CLOCKS_PER_SEC;
        if (!error) {
            printf("[%d]BDD num %d done in %f\n",VARIABLES, j ,time);
        }
        whole_times[j]=time;
        time = ((double) create) / CLOCKS_PER_SEC;
        create_times[j]=time;
        time = ((double) reduce) / CLOCKS_PER_SEC;
        reduce_times[j]=time;
        time = ((double) check) / CLOCKS_PER_SEC;
        check_times[j]=time;
        newVertices = count_vertices(bdd);

        reduce_percentage[j]=100-((double)newVertices/((double)oldvertices))*100;

        free(bdd);
        free(function->vector);
    }
    double sum_create=0,sum_reduce=0,sum_check=0,sum_whole=0,sum_percentage=0;

    for(int i=0;i<DIAGRAM_NUM;i++){
        sum_create+=create_times[i];
        sum_reduce+=reduce_times[i];
        sum_whole+=whole_times[i];
        sum_check+=check_times[i];
        sum_percentage+=reduce_percentage[i];
    }

    printf("~~~~~~TEST DONE~~~~~~\nAverage create time: %lf\nAverage reduce time: %lf\nAverage check time: %lf\nAverage test time: %lf\nAverage reduction rate: %lf of %d vertices",sum_create/DIAGRAM_NUM,sum_reduce/DIAGRAM_NUM,sum_check/DIAGRAM_NUM,sum_whole/DIAGRAM_NUM,sum_percentage/DIAGRAM_NUM,oldvertices);
}

int main() {
    srand(time(0));
    BDD * diagram=(BDD*)malloc(sizeof (BDD));
    BF * function=(BF*)malloc(sizeof(BF));

    test(diagram,function);


    return 0;
}
