#include "TAD_String.h"

#define STR 1
#define SET 2
#define LIST 3

typedef struct dataType{
    int nodeType;
    union{
        String string;
        struct{
            struct dataType* data;
            struct dataType* next;
        };
    };
}*Tdata;

Tdata create_str_ast(String);
Tdata str(char*); //para crear un string en el main: Tdata a = str("Hola mundo");
Tdata create_list();
Tdata create_set();

//Operaciones sobre LIST
void append(Tdata*, Tdata);
int length(Tdata);
Tdata copy_list(Tdata);
Tdata concat(Tdata, Tdata);
bool search(Tdata, Tdata);
Tdata listo_to_str(Tdata);

//Operaciones sobre SET
void insert_set(Tdata*, Tdata);
bool belongs(Tdata, Tdata);
void remove_set(Tdata*, Tdata);
Tdata copy_set(Tdata);

//Opreaciones algebraicas
Tdata union_set(Tdata, Tdata);
Tdata intersection_set(Tdata, Tdata);
Tdata difference_set(Tdata, Tdata);
bool subset(Tdata, Tdata);
bool equals_set(Tdata, Tdata);

void print_Tdata(Tdata);

Tdata str_to_list(Tdata);
Tdata cartesian_product_set(Tdata , Tdata);
