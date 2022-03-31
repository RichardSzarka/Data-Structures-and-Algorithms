#include <stdio.h>
#include <stdlib.h>

#define BORDER 50
#define START 0
#define END 100
#define JUMP 1
#define STARTD 0
#define ENDD 5
#define ENTER printf("----------------------\n")

typedef struct element{
     int value;
     struct element * next;
}ELEMENT;

typedef struct queue{
    int size;
    int border;
    struct element * front;
    struct element * rear;
}QUEUE;

ELEMENT * create_element(int value){
    ELEMENT * new =(ELEMENT*)malloc(sizeof(ELEMENT));
    new->value=value;
    new->next=NULL;
}

void enqueue(QUEUE * queue, int value){
    if(queue->size<queue->border){
        if(queue->front==NULL && queue->rear==NULL) {
            queue->front = queue->rear = create_element(value);
            queue->size++;
        }
        else{
            queue->rear->next= create_element(value);
            queue->rear=queue->rear->next;
            queue->size++;
        }
    }
}

int dequeue(QUEUE * queue){
    if(queue && queue->front){
        ELEMENT * free_element=queue->front;
        int value=free_element->value;
        queue->front=queue->front->next;
        free(free_element);
        queue->size--;
        return value;
    }
    return -1;
}

int peek(QUEUE * queue){
    if(queue && queue->front){
        return queue->front->value;
    }
    return -1;
}

int size(QUEUE * queue){
    return queue->size;
}

int isEmpty(QUEUE * queue){
    if(queue->size==0 && queue->front==NULL){
        return 0;
    }
    return 1;
}

int  isFull(QUEUE * queue){
    if(queue->size==queue->border){
        return 0;
    }
    return 1;
}


void print(ELEMENT * element){
    if(element){
        printf("%d - printed\n",element->value);
        print(element->next);
    }
}

void test_enqueue(QUEUE * queue){
    for(int i=START;i<END;i=i+JUMP){
        enqueue(queue,i);
    }
    printf("%d - front | %d - rear\n",queue->front->value,queue->rear->value);
}

void test_dequeue(QUEUE * queue){
    for(int i=STARTD;i<ENDD;i++){
        printf("%d - dequeue\n", dequeue(queue));
    }
    printf("%d - front | %d - rear\n",queue->front->value,queue->rear->value);
}

void test_peek(QUEUE * queue){
    for(int i=STARTD;i<ENDD;i++){
        printf("%d - peeked\n", peek(queue));
    }
    printf("%d - front | %d - rear\n",queue->front->value,queue->rear->value);
}

int main() {
    QUEUE * queue=(QUEUE*)malloc(sizeof(QUEUE));
    queue->size=0;
    queue->front=NULL;
    queue->rear=NULL;
    queue->border=BORDER;

    printf("%d - empty?\n",isEmpty(queue));
    printf("%d - full?\n",isFull(queue));
    ENTER;
    test_enqueue(queue);
    ENTER;
    printf("%d - empty?\n",isEmpty(queue));
    printf("%d - full?\n",isFull(queue));
    ENTER;
    print(queue->front);
    ENTER;
    test_dequeue(queue);
    ENTER;
    print(queue->front);
    ENTER;
    test_peek(queue);
    ENTER;
    print(queue->front);
    ENTER;
    printf("%d - empty?\n",isEmpty(queue));
    printf("%d - full?\n",isFull(queue));
    return 0;
}
