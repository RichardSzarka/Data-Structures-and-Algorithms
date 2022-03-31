#include "tabulka.h"

TABLE * hash_table_init(int size) { // inicializacia zakladu tabulky
    TABLE * Hash_table = (TABLE*)calloc(1,sizeof(TABLE)); // 1 tabulka
    Hash_table->size = size; //velkost tabulky
    Hash_table->occupied = 0; //okopuvaných elementov
    Hash_table->items = (ITEM**)calloc(size,sizeof(ITEM*)); //prvky v tabulke

    return Hash_table;
}

int hashing(int key,int size){ //hashovacia funkcie
    int long long hashc=(((key+457)*797)-257);
    int hash=hashc%size;
    return hash;
}


TABLE * resize(TABLE * hash_table) {
    int new_size=(hash_table->size)*2+1; //zvacsenie tabulky
    TABLE * new_table = (TABLE*)calloc(1,sizeof(TABLE)); // 1 tabulka
    new_table->size = new_size; //velkost tabulky
    new_table->occupied = 0; //okopuvaných elementov
    new_table->items = (ITEM**)calloc(new_size,sizeof(ITEM*)); //spravenie nových elementov

    for(int i=0;i<hash_table->size;i++) //premiestnovanie starej tabulky do novej (podla hashu samozrejme)
    {
        if(hash_table->items[i]) //ak existuje prvok v tabulke premiestni ho
        {
            table_insert(new_table,hash_table->items[i]->value,hash_table->items[i]->key,0);
            free(hash_table->items[i]);
        }
    }
    free(hash_table);
    return new_table;
}

int count_collision() //ratanie kolizii
{
    static int count=0;
    count+=1;
    return count;
}


TABLE * table_insert(TABLE * hash_table,char * value,int key,int count_collisions) //funkcia na vkladanie prvkov
{
    int hash=hashing(key,hash_table->size); //hash z kluca
    int Collison1 = 1;
    while(hash_table->items[hash]){//ak existuje uz prvok na mieste kam ukazuje hash a aj kluc je zhodny - aktualizuj hodnotu (while cyklus kvoli kolizii)
        if(table_item_key(hash)==key)
        {
            table_item_value(hash)=(char*)calloc(strlen(value),sizeof(char)); //zmenenie velkosti stringu
            strcpy(table_item_value(hash),value); //prekopirovanie hodnoty string do tabulky
            return hash_table;

        }

        hash++; //ak nie je rovnaky kluc zvacsi hash (pozri dalsi element v tabulke)
        hash=hash%hash_table->size; //ak by vysiel von z tabulky nech ide od zaciatku
        if(count_collisions && Collison1){
            count_collision();
            Collison1=0;
        }

    }
    (hash_table->items)[hash] = (ITEM*)calloc(1,sizeof(ITEM)); //alokuj na mieste miesto na prvok

    table_item_value(hash) = calloc(strlen(value),sizeof(char)); //alokuj miesto pre hodnotu (string)
    strcpy(table_item_value(hash),value); //prekopiruj tam string

    table_item_key(hash) = key; //k danemu prvku v tabulke prirad aj kluc

    hash_table->occupied++; //inkrementuj kolko je miest okupovanych v tabulke

    if((hash_table->size*0.7) < hash_table->occupied) //skontroluj ci tabulka nie je moc zaplnena
    {
        hash_table=resize(hash_table); //ak ano zvacsi tabulku
    }

    return hash_table;
}

char* search_table(TABLE * hash_table,int key) { //prehladaj tabulku
    int hash = hashing(key, hash_table->size); //zisti hash podla klucu
    while(hash_table->items[hash] && table_item_key(hash)!=key) //kym existuje prvok na danom mieste a zaroven kluc nie je zhodny
    {
        hash=(hash+1)%hash_table->size;
    }
    if(hash_table->items[hash]) { //ak existuje prvok
        return hash_table->items[hash]->value;
    }//ak neexistuje prvok
    else{
        return "Not Found";
    }
}
