#include "TAD_String.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

String crearSTR(){
    return NULL;
}

void agregarCaracter(String *cadena, char caracter){
    String newNodo = (String)malloc(sizeof(T_NodoSTR));
    newNodo->dato = caracter;
    newNodo->sig = NULL;

    if(*cadena == NULL){
        *cadena = newNodo;
        return;
    } 

    String temp = *cadena;
    while(temp->sig != NULL){
        temp = temp->sig;
    }
    temp->sig = newNodo;
}

String cargaSTR(char* texto){
    String cadena = crearSTR();

    int i=0;
    while(texto[i] != '\0'){
        agregarCaracter(&cadena, texto[i]);
        i++;
    }

    return cadena;
}

String concatSTR(String str1, String str2){
    String newStr = crearSTR();

    if(str1 == NULL && str2 == NULL) return newStr;

    if(str1 == NULL){
        while (str2 != NULL){
            agregarCaracter(&newStr, str2->dato);
            str2 = str2->sig;
        }
        return newStr;
    }

    if(str2 == NULL){
        while (str1 != NULL){
            agregarCaracter(&newStr, str1->dato);
            str1 = str1->sig;
        }
        return newStr;
    }

    while (str1 != NULL){
        agregarCaracter(&newStr, str1->dato);
        str1 = str1->sig;
    }
    while (str2 != NULL){
        agregarCaracter(&newStr, str2->dato);
        str2 = str2->sig;
    }
    return newStr;
}

bool esIgualSTR(String str1, String str2){
    while(str1 != NULL && str2 != NULL){
        if(str1->dato != str2->dato) return false;
        str1 = str1->sig;
        str2 = str2->sig;
    }

    return (str1 == NULL && str2 == NULL);
}

void printSTR(String str){
    while(str != NULL){
        printf("%c", str->dato);
        str = str->sig;
    }
}

String copySTR(String original){
    String copia = crearSTR();

    while(original != NULL){
        agregarCaracter(&copia, original->dato);
        original = original->sig;
    }
    return copia;
}