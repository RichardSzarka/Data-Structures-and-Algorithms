#include <stdio.h>
#include <stdlib.h>

#define BOTTOM 0
#define TOP 200
#define JUMP 2
#define POP 3
#define PEEK 3
#define BORDER 50

typedef struct element{
    int value;
    struct element * next;
}ELEMENT;

typedef struct stack{
    int size;
    int border;
    struct element * first;
}STACK;


ELEMENT * create_element(int value){
    ELEMENT * new = (ELEMENT*)malloc(sizeof(ELEMENT));
    new->value = value;
    new->next=NULL;
}

void  push(int value,STACK * stack){
    if(stack->border>stack->size){
            ELEMENT * new=create_element(value);
            new->next=stack->first;
            stack->first=new;
            (stack->size)++;
    }
}

int pop(STACK * stack){
    if(stack->first) {
        int value = stack->first->value;
        ELEMENT *free_element = stack->first;
        stack->first = stack->first->next;
        free(free_element);
        stack->size--;
        return value;
    }
    else
        return -1;

}

int peek(STACK * stack){
    if(stack->first){
        return stack->first->value;
    }
    else{
        return -1;
    }
}

int size(STACK * stack){
    return stack->size;
}

int isEmpty(STACK * stack){
    if(stack->first)
    {
        return 1;
    }
        return 0;
}

int isFull(STACK * stack){
    if(stack->size==stack->border){
        return 0;
    }
    return 1;
}

void print(ELEMENT * element){
    if(element) {
        printf("%d - printed\n", element->value);
        print(element->next);
    }
    return;
}

void pop_test(STACK * stack){
    for(int i=0;i<POP;i++){
        printf("%d - popped\n",pop(stack));
    }
}

void push_test(STACK * stack){
    for(int i=BOTTOM;i<TOP;i=i+JUMP)
    {
        push(i,stack);
    }
}

void peek_test(STACK * stack){
    for(int i=0;i<PEEK;i++){
        printf("%d - peek\n", peek(stack));
    }
}

int main() {
    STACK * stack=(STACK*)malloc(sizeof(STACK));
    stack->size=0;
    stack->first=0;
    stack->border=BORDER;

    printf("%d - is empty?\n", isEmpty(stack));

    printf("%d - is full?\n", isFull(stack));

    push_test(stack);

    printf("%d - is empty?\n", isEmpty(stack));

    printf("%d - is full?\n", isFull(stack));

    printf("%d - size\n", size(stack));

    pop_test(stack);

    printf("%d - is full?\n", isFull(stack));

    peek_test(stack);

    print(stack->first);




    return 0;
}
