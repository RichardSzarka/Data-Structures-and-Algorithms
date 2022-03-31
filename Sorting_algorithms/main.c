#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ELEMENTS 10
#define SORTS 7
#define RANGE 100

int ** generate_values(){
    int ** field=(int**)calloc(SORTS,sizeof(int*));
    for(int i=0;i<SORTS;i++){
        field[i]=(int*)calloc(ELEMENTS,sizeof(int));
        for(int j=0;j<ELEMENTS;j++) {
            field[i][j] = rand() % (RANGE);
        }
    }
    return field;
}


void isSorted(int * field){
    int before = 0;
    for(int i=0;i<ELEMENTS;i++){
        if(before>field[i]){
            printf("Unsorted\n");
            return;
        }
        before=field[i];
    }
    printf(" - Sorted\n");
}


void insertSort(int * field){
    int j;
    float time;
    clock_t t=clock();

    int key;
        for(int i=1;i<ELEMENTS;i++){
            key=field[i];
            j=i-1;

            while(j>=0 && field[j]>key){
                field[j+1]=field[j];
                j--;
            }
            field[j+1]=key;
        }

    t=clock()-t;
    time = ((double)t)/CLOCKS_PER_SEC;
    printf("Insert sort: %f",time);
    isSorted(field);
}

void selectSort(int * field){
    float time;
    clock_t t=clock();
    int min,help,j;

    for(int i=0;i<ELEMENTS-1;i++){
        min=i;
        for( j=i+1;j<ELEMENTS;j++){
            if(field[j]<field[min]){
                min=j;
            }
        }
        help=field[min];
        field[min]=field[i];
        field[i]=help;
    }

    t=clock()-t;
    time = ((double)t)/CLOCKS_PER_SEC;
    printf("Select sort: %f",time);
    isSorted(field);
};

void merge(int * field, int start, int mid, int end){
    int i,j,k;

    int n1 = mid - start + 1;
    int n2 = end - mid;

    int Left[n1],Right[n2];

    for(i=0; i<n1;i++){
        Left[i]=field[start+i];
    }
    for(j=0;j<n2;j++){
        Right[j]=field[mid+1+j];
    }

    i=0;
    j=0;
    k=start;

    while(i<n1 && j<n2){ //kym oba maju
        if (Left[i]<=Right[j]){ //porovna
            field[k]=Left[i];
            i++;
        }
        else{
            field[k]=Right[j]; //porovna
            j++;
        }
        k++;
    }

    while(i < n1){
        field[k]=Left[i];
        i++;
        k++;
    }
    while(j < n2){
        field[k]=Right[j];
        j++;
        k++;
    }
}

void mergeSort(int * field,int start,int end){
    if(start<end){
        int mid=start + (end-start)/2;
        mergeSort(field,start,mid);
        mergeSort(field,mid+1,end);

        merge(field,start,mid,end);
    }
}

int part(int * field, int start, int end){
    int pivot=field[end];
    int i = start -1;
    int help;

    for(int j=start;j<=end;j++){
        if(pivot>field[j]){
            i++;
            help=field[j];
            field[j]=field[i];
            field[i]=help;
        }
    }

    help=field[i+1];
    field[i+1]=field[end];
    field[end]=help;
    return i+1;
}

void quickSort(int * field,int start,int end){
    if(start<end) {

        int index=part(field, start, end);

        quickSort(field,start,index-1);
        quickSort(field,index+1,end);
    }

}

void bubbleSort(int * field){
    int end=0,help;
    float time;
    clock_t t=clock();

    while(!end){
        end=1;
        for(int i=0;i< ELEMENTS-1;i++){
            if(field[i]>field[i+1]){
                help=field[i];
                field[i]=field[i+1];
                field[i+1]=help;
                end=0;
            }
        }
    }

    t=clock()-t;
    time = ((double)t)/CLOCKS_PER_SEC;
    printf("Bubble sort: %f",time);
    isSorted(field);

}

void print(int * field){
    for(int i=0;i<ELEMENTS;i++){
        printf("%d \t",field[i]);
    }
    //jeb
}

int main() {
    srand(time(0));
    float time;
    clock_t t;
    int ** field=generate_values();
    isSorted(field[0]);
    insertSort(field[1]);
    selectSort(field[2]);
    print(field[2]);
    bubbleSort(field[2]);

    isSorted(field[3]);
    t=clock();
    mergeSort(field[4],0,ELEMENTS-1);
    t=clock()-t;
    time = ((double)t)/CLOCKS_PER_SEC;
    printf("Merge sort: %f",time);
    isSorted(field[4]);

    t=clock();
    quickSort(field[5],0,ELEMENTS-1);
    t=clock()-t;
    time = ((double)t)/CLOCKS_PER_SEC;
    printf("Quick sort: %f",time);
    isSorted(field[5]);


    return 0;
}
