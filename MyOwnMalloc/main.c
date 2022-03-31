#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define zarovanie 4 //zarovnanie pri alokácii pamäte pre lepšiu fragmentáciu
#define headsize 8 //kolko bytov potrebujem na header a footer
#define hodnota_ptr(x) (*((unsigned int*)x)) //hodnota int na mieste pamati
#define one_Byte_back(x) (((unsigned int*)(x)-1)) //o jeden bajt spat sa pozri
#define one_Byte_forward(x) (((unsigned int*)(x)+1)) //o jeden bajt pozri dopredu
#define mem_SIZE 100 // velkost pamate

/*
 *
 meno:Richard
 priezvisko: Szarka
 predmet: Datove struktury a algoritmy
 projekt: 1

 fakulta: FIIT STU
 datum: 8.3.2021

*/
// pre krajsi vypis treba odkomentovat riadky kodu kde sa nuluju hlavicky a paticky starych blokov a vynulovanie pamate na zaciatku

void * root;//pointer na začiatok mojej statickej pamäte

void vycisti_pamat(char * ptr,unsigned int size) //"vyčistí" mi pamat aby sa v debugeri lepšie orientovalo
{

    for(int i=0;i<size;i++)
    {
        ptr[i]=0;
    }
}

void pekny_vypis(char * ptr,unsigned int size) // pekný výpis na moju kontrolu pamate
{
    for(int i=0;i<size;i++)
    {
        if(i%16==0)
        {
            printf("\n%x:",(unsigned int)&ptr[i]);
        }
        if(i%4==0)
        {
            printf(" ");
        }
        printf("%d ",ptr[i]);
    }
    printf("\n\n");
}

unsigned int check_4(unsigned int size) //kotroluje delitelnost 4
{
    switch(size & 3)
    {
        case 0:
            return size;
        case 1:
            return size+3;
        case 2:
            return size+2;
        case 3:
            return size+1;
    }
    return size;
}

unsigned int check_mem4(unsigned int size) //zarovna pamat aby bola delitelna stvrokou
{
    switch(size & 3)
    {
        case 0:
            return size;
        case 1:
            return size-1;
        case 2:
            return size-2;
        case 3:
            return size-3;
    }
    return size;
}


void memory_init(void *ptr , unsigned int size)
{
    //vycisti_pamat((char*)ptr,size); //pre lepsie ladenie programu
    size=size-8; //posledne 4 bity na označenie konca pamate a v prvých 4 bytoch bude velkosť pamate
    size=check_mem4(size);

    //nastavenie globalneho pointra na zaciatok pamate
    root=ptr;

    *(int*)root=(int)size+4; //nastaví sa veľkosť pamate

    *(int*)one_Byte_forward(root)=size; //nastaví sa hlavicka prveho velekho bloku


    //pridanie na koniec 1. velkeho bloku jeho velkosť - paticka
    volatile unsigned int * footer=(unsigned int*)(root+size);
    *footer=size;
    *(footer+1)=0; //nastavenie 0 na konci pamate
}

void * first_fit(unsigned int size) //dorieš zarovnavanie o 4
{
    void * zaciatok=one_Byte_forward(root);
    void * vyhovujuci_blok=zaciatok; //nasmerovanie na blok
    unsigned int block_size=hodnota_ptr(zaciatok);

    while(((int)block_size-(int)headsize)<(int)size || ((block_size & 1)==1)) //zakomponovať nech nevyjde z pamate
    {
        if (block_size==0)
        {
            return NULL;
        }
        block_size=block_size>>1<<1; //zbavenie sa 1 na konci
        vyhovujuci_blok+=block_size; //
        block_size=hodnota_ptr(vyhovujuci_blok);
    }

    return vyhovujuci_blok;
}

void * memory_alloc(unsigned int size)
{
    size=check_4(size);

    void * block_pointer=NULL; // premenná ktorá bude ukazovať na začiatok nájdeného bloku -> blok ktorý sprístupním uživatelovi
    void * block_footer=NULL;

    block_pointer=first_fit(size);
    if(!block_pointer) //ak nenasiel miesto return 0
    {
        return NULL;
    }
    block_footer=block_pointer+size+zarovanie;

    unsigned int past_block_size=hodnota_ptr(block_pointer);         //zistí velkosť bloku ktorého mi našlo
    unsigned int new_free_size=past_block_size-size-2*zarovanie;    //zisti kolko ostalo to zvyšku volneho bloku

    *(unsigned int*)block_pointer=size+headsize+1;                //nastavenie headeru allokovaneho bloku (+1 aby sme videli že je alokovaný - spodný bit)
    *(unsigned int*)(block_footer)=size+headsize+1;              //nastavenie footeru alokovaneho bloku

    if(new_free_size!=0) //ak by bol zvyšný blok
    {
        *((unsigned int *) block_footer +1) = new_free_size; //odpočítam zo starej velkosti novu alokovanu a dostanem zvyšok ale mínus zarovnanie kvoli novemu headeri a footeri
        *((unsigned int *) block_footer + new_free_size / 4) = new_free_size; //nastavenie footera noveho volneho bloku
    }
    return block_pointer+zarovanie;
}

int memory_free(void *valid_ptr) //ošetri free na zaciatku a konci pamate
{
    unsigned int* block_pointer=one_Byte_back(valid_ptr);//posunutie pointra o 1 dozadu (*int) - hlavicka daneho bloku
    unsigned int * zaciatok=one_Byte_forward(root);

    if((*block_pointer) & 1) //zistenie ci je posledny bit nastaveny na 1
    {

        unsigned int * prev_block=one_Byte_back(block_pointer)-(*(one_Byte_back(block_pointer))/4-1);//posunutie na hlavicku bloku pred týmto blokom
        unsigned int * next_block=block_pointer+(hodnota_ptr(block_pointer)-headsize)/4+2; //posunutie na header dalšieho bloku
        unsigned int * block_footer=(block_pointer + (hodnota_ptr(block_pointer) - headsize) / 4 + 1);

        if((block_pointer==zaciatok && (*(next_block)&1)==1) || ((*(prev_block)&1)==1 && (*(next_block)&1)==1) || (*(one_Byte_forward(block_footer))==0 && (*(prev_block)&1)==1)) // blok pred aj po nom zabrane alebo uvolnuje 1 blok ci posledny blok
        {
            *block_pointer = hodnota_ptr(block_pointer) - 1; //nastavenie hlavicky na "volny"
            *(block_footer) = *block_pointer; //nastavenie nozičky na "volny" ( /4 kvoli skokom o 4 (u int))
            return 0;
        }
        else if((*(prev_block)&1)==0 && (*(next_block)&1)==1 ||(one_Byte_forward(block_footer)==0 && (*(prev_block)&1)==0)) //pred je volny ale za nie je
        {
            *block_pointer = hodnota_ptr(block_pointer) - 1; //nastavenie hlavicky na "volny"
            unsigned int new_size=*(prev_block)+*(block_pointer); //nova velkost pri spajani polí
            //*(block_pointer)=*(one_Byte_back(block_pointer))=0; //vynulovanie hlavicky daneho bloku a nozicky bloku pred
            *(prev_block)=new_size; //nastavenie hlavicky bloku pred na novu velkost
            *(block_footer)=new_size; //nastavenie nozicky daneho bloku na novu velkost

            return 0;
        }
        else if((block_pointer==zaciatok && (*(next_block)&1)==0) || ((*(prev_block)&1)==1 && (*(next_block)&1)==0)) //pred nie je ale za je alebo uvolnuje 1 blok
        {
            *block_pointer = hodnota_ptr(block_pointer) - 1; //nastavenie hlavicky na "volny"
            unsigned int new_size=*(next_block)+*(block_pointer); //nova velkost pri spajani polí
           // *block_footer=*next_block=0; //najprv 0 zapís a potom velkost aby sa neprepisal pri velkosti 1 byte
            *((block_pointer)+new_size/4-1)=new_size; //nastavenie footeru noveho bloku
            *block_pointer=new_size;
            return 0;
        }
        else //aj pred aj za je volny
        {
            *block_pointer = hodnota_ptr(block_pointer) - 1;
            unsigned int new_size=*(next_block)+*(block_pointer)+*(prev_block);
            unsigned int go_to_next_block_footer=hodnota_ptr(next_block); //vymazem hlavicku takze si ulozim jej hdontou (next blok)
            //*block_footer=0; //nozicka bloku=0
            //*(one_Byte_back(block_pointer))=0; //nozicka predtym bloku=0
            //*block_pointer=0; //hlavicka predtym bloku =0
            //*(one_Byte_forward(block_footer))=0; //hlavicka bloku dalej =0
            *prev_block=new_size; //nastavenie velkosti
            *(next_block+go_to_next_block_footer/4-1)=new_size;

            return 0;
        }

    }
    return 1;
}

int memory_check(void *ptr)
{
    if(ptr) //ak pointer je vobec platny
    {
        unsigned int *pointer = one_Byte_back((unsigned int *) ptr); //pozri sa na hlavicku
        unsigned int *koniec = (unsigned int*)root+(*(unsigned int*)root); // zistí kde je koniec pamate
        if(pointer>(unsigned int*)root && pointer<koniec && ((*pointer & 1) == 1)) //ak je pointer alokovany a je medzi zaciatkom a koncom pamate
        {
            return 1;
        }
        return 0; //ak nie
    }

    return 0;
}

void test_mem(unsigned int min_blok,unsigned int max_blok, unsigned int min_mem, unsigned int max_mem,char * memory) //testovac pamate
{
    unsigned int mem_size=((rand() % (max_mem-min_mem+1))+min_mem ); //vybere nahodne velku pamat podla argumentov funckie

    memory_init((void *)memory,mem_size); //inicializuje pamat

    char*pointer[max_mem]; //pole ukazovatelov (tam budem ukladat ukazovatelov na jednotlive bloky)


    int index=0;

    unsigned int allocation=0; //kolko krat chcel alokovat
    unsigned int allocated=0; //kolkokrat s amu podarilo alokovat
    unsigned int occupied=0; //kolko bajtov bolo "okupovaných" v pamati
    unsigned int request=0; //kolko bajtov chcel využívať povodne ("okupovat")
    printf("~~~~~TESTING MEMORY~~~~~\n\n");
    while(1)
    {
        allocation++;
        request=rand()%(max_blok-min_blok+1)+min_blok; //nahodne vybrana velkost bloku
        pointer[index]=(char*)memory_alloc(request); //alokovany blok
        if(pointer[index]) //ak sa alokoval
        {
            occupied+=request+8;
            allocated++;
        }
        if(allocation>allocated)
            break;
        index++;

    }
    printf("Tried to allocate in memory of %u bytes %d blocks, successful: %d (%.5lf%%)\n",mem_size,allocation,allocated,((float)allocated/(float)allocation)*100);
    printf("%d bytes occupied in memory of %d bytes (%.5lf%%)\n",occupied,mem_size-8,(float)occupied/(float)(mem_size-8)*100);
    for(int i=0;i<index;i++) //uvolnovanie blokov (predtym kontrola s memory_check)
    {
        if(memory_check(pointer[i]))
            memory_free(pointer[i]);
        else
            printf("Wrong memory chceck\n");
    }
    printf("\n~~~~~TEST FINISHED~~~~~\n");
}


//na jednotlive testy treba zmenit velkost pamate  na zaciatku v makre

int main(void) {
    char memory[mem_SIZE];

    srand(time(0)); // nahodny cas

   // memory_init((void *) memory, mem_SIZE); //TEST1

  // pekny_vypis(root, mem_SIZE);

    char*test;
    char*test2;
    char*test3;
    char*test4;
    char*test5;
    char*test6;
    char*test7;


//test2.1 - prvý test, velkost blokov 16, od 25 do 200 pamati
   /* test_mem(16,16,25,200,memory);
    test_mem(16,16,25,200,memory);
    test_mem(16,16,25,200,memory);
    test_mem(16,16,25,200,memory);
    test_mem(16,16,25,200,memory);
    test_mem(16,16,25,200,memory);
    test_mem(16,16,25,200,memory);
    test_mem(16,16,25,200,memory);
    test_mem(16,16,25,200,memory);
    test_mem(16,16,25,200,memory);*/

   //test2.2 - druhý test, velkost blokov od 8 do 24, od 25 do 200 pamati
    /*test_mem(8,24,25,200,memory);
    test_mem(8,24,25,200,memory);
    test_mem(8,24,25,200,memory);
    test_mem(8,24,25,200,memory);
    test_mem(8,24,25,200,memory);
    test_mem(8,24,25,200,memory);
    test_mem(8,24,25,200,memory);
    test_mem(8,24,25,200,memory);
    test_mem(8,24,25,200,memory);
    test_mem(8,24,25,200,memory);
    test_mem(8,24,25,200,memory);*/
   // printf("\n");

    //test2.3 - tretí test, velkost blokov od 500 do 5000, od 1000 do 100000 pamati
   /* test_mem(500,5000,1000,100000,memory);
    test_mem(500,5000,1000,100000,memory);
    test_mem(500,5000,1000,100000,memory);
    test_mem(500,5000,1000,100000,memory);
    test_mem(500,5000,1000,100000,memory);
    test_mem(500,5000,1000,100000,memory);
    test_mem(500,5000,1000,100000,memory);
    test_mem(500,5000,1000,100000,memory);
    test_mem(500,5000,1000,100000,memory);
    test_mem(500,5000,1000,100000,memory);*/

    //test2.4 - štvrt test, velkost blokov od 8 do 5000, od 1000 do 100000 pamati
    /*test_mem(8,5000,1000,100000,memory);
    test_mem(8,5000,1000,100000,memory);
    test_mem(8,5000,1000,100000,memory);
    test_mem(8,5000,1000,100000,memory);
    test_mem(8,5000,1000,100000,memory);
    test_mem(8,5000,1000,100000,memory);
    test_mem(8,5000,1000,100000,memory);
    test_mem(8,5000,1000,100000,memory);
    test_mem(8,5000,1000,100000,memory);
    test_mem(8,5000,1000,100000,memory);
    test_mem(8,5000,1000,100000,memory);*/

//test1
     /*test=(char*)memory_alloc(10);
     pekny_vypis(root, mem_SIZE);
     test2=(char*)memory_alloc(10);
     pekny_vypis(root, mem_SIZE);
     test3=(char*)memory_alloc(10);
     pekny_vypis(root, mem_SIZE);
     test4=(char*)memory_alloc(10);
     pekny_vypis(root, mem_SIZE);
     test5=(char*)memory_alloc(8);
     pekny_vypis(root, mem_SIZE);
     test6=(char*)memory_alloc(2);
     pekny_vypis(root, mem_SIZE);


     memory_free(test);
     pekny_vypis(root, mem_SIZE);
     memory_free(test4);
     pekny_vypis(root, mem_SIZE);
     memory_free(test2);
     pekny_vypis(root, mem_SIZE);
     memory_free(test3);
     pekny_vypis(root, mem_SIZE);
     test2=(char*)memory_alloc(10);
     pekny_vypis(root, mem_SIZE);
     memory_free(test2);
     pekny_vypis(root, mem_SIZE);
     test3=(char*)memory_alloc(20);
     pekny_vypis(root, mem_SIZE);
     memory_free(test3);
     pekny_vypis(root, mem_SIZE);
     test7=(char*)memory_alloc(30);
     pekny_vypis(root, mem_SIZE);
     memory_free(test7);
     pekny_vypis(root, mem_SIZE);
     memory_free(test6);
     pekny_vypis(root, mem_SIZE);*/



    return 0;

}

