#include <stdbool.h>

typedef struct Nodo{
    char dato;
    struct Nodo* sig;
}T_NodoSTR;

typedef T_NodoSTR *String;

String crearSTR();
String cargaSTR(char*);
String concatSTR(String, String);
bool esIgualSTR(String, String);
void printSTR(String);
String copySTR(String);