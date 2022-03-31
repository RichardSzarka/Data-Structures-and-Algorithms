#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int polynomialHash(char * searched){
    int sum=0,c=1;

    for(int i=0;i<strlen(searched);i++){
        sum=sum+searched[i]*c;
        c=c*180;
    }
    return sum;
}

char * substring(char * sub,int lenght,int end,int offset){
    char * output = (char*)calloc(lenght+1,sizeof(char));
    int i=0;
    for(;offset<end;offset++){
        output[i]=sub[offset];
        i++;
    }
    return output;
}

void rabinKarp(char * string, char * searched){
    int lenght = strlen(searched);
    int hash = polynomialHash(searched);
    int newHash;

    char * sub;

    for(int i=0;i<strlen(string);i++){
        if(string[i]==' '){
            continue;
        }
        sub=substring(string,lenght,lenght+i,i);
        newHash=(polynomialHash(sub));
        if(newHash==hash){
            printf("%s %d",sub,i);
        }



    }


}

int main() {
    char * searched;
    char input[574]="Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop publishing software like Aldus PageMaker including versions of Lorem Ipsum.";
    int d;

    scanf("%d",&d);
    searched=(char*)calloc(d+1,sizeof(char));
    scanf("%s",searched);

    rabinKarp(input,searched);

    return 0;
}
