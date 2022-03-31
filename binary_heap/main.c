#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ELEMENTS 20

void heapify (int * bHeap, int index){
    int parent=index/2;
    if ( index != 1 && bHeap[parent] < bHeap[index]){
        int help=0;

        help=bHeap[parent];
        bHeap[parent]=bHeap[index];
        bHeap[index]=help;

        heapify (bHeap, parent);
    }
}

void insert (int * bHeap, int new){
    bHeap[0] ++;
    int index=bHeap[0];

    bHeap[index]=new;
    heapify (bHeap,index);
}

void printHeap (int * bHeap){
    for (int i=1;i<=bHeap[0];i ++){
        printf("%d: %d\n",i,bHeap[i]);
    }
    printf("--------------------------\n");
}

int main() {
    srand(time(0));
    int * bHeap =(int *)calloc(256, sizeof(int));
    int new = 0;
    for (int i=0;i<ELEMENTS;i++){
        new=rand() % (1000-1)+1;
        insert(bHeap,new);
        printHeap(bHeap);
    }
    return 0;
}
