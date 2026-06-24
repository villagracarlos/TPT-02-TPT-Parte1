#include "TAD_READER.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

Automata* load_automata(char* filename){
    FILE* f = fopen(filename, "r");

    if(f == NULL){
        printf("No se pudo abrir el archivo.\n");
        return NULL;
    }

    // CREA AUTOMATA
    Automata* A = create_automata();

    // Parseo 
    //Enumero secciones del archivo de texto
    enum{NINGUNO, ESTADOS, INICIAL, FINALES, TRANSICIONES};
    //Ninguno = 0, Estados = 1, Inicial = 2, etc.
    int seccion = NINGUNO;

    char linea[100]; //99 caracteres, más '\0'
    while(fgets(linea,100,f) != NULL){  //lee una linea, la guarda en un char de maximo 99 caracteres, lee desde f

        if(linea[0] == '#' || linea[0] == '\n') continue; //ignora todas las lines vacias o que cominzen con #
        //"continue;" termina la iteracion actual del while para pasar a la siguiente itearcion.

        if(strcmp(linea,"ESTADOS\n") == 0){
            seccion = ESTADOS;
            continue;
        }

        if(strcmp(linea,"INICIAL\n") == 0){
            seccion = INICIAL;
            continue;
        }

        if(strcmp(linea,"FINALES\n")== 0){
            seccion = FINALES;
            continue;
        }

        if(strcmp(linea,"TRANSICIONES\n") == 0){
            seccion = TRANSICIONES;
            continue;
        }

        // CARGA ESTADOS
        if(seccion == ESTADOS){
            char* token = strtok(linea, " \n"); //strtok() separa una línea en tokens.
            while(token != NULL){
                add_state(A, token, false);
                token = strtok(NULL, " \n");
            }
        }
        
        // CARGA ESTADO INICIAL
        if(seccion == INICIAL){
            char nombre[20];
            sscanf(linea,"%s", nombre); //sscanf() extrae datos siguiendo un formato, sabemos la cantidad de datos por eso no se usa strtok()
            A->q0 = find_state(A, nombre);
        }

        // CARGA ESTADOS FINALES
        if(seccion == FINALES){
            char* token = strtok(linea," \n");
            while(token != NULL){
                StateNode* q = find_state(A, token);
                if(q) q->isFinal = true;
                token = strtok(NULL," \n");
            }
        }
        
        // TRANSICIONES
        if(seccion == TRANSICIONES){
            char origen[20];
            char simbolo[20];
            char destino[20];
            sscanf(linea,"%s %s %s", origen, simbolo, destino);
            add_transition(A, origen, simbolo, destino);
        }
    }

    fclose(f);

    return A;
}

char* get_first_line(char* filename){
    FILE* f = fopen(filename, "r");
    if(f == NULL){
        printf("No se pudo abrir el archivo.\n");
        return NULL;
    }

    char linea[100];
    while(fgets(linea, 100, f) != NULL){
            fclose(f);
            return strdup(linea); //devuelve una copia de la linea
    }

    fclose(f);
    return NULL; //si no se encuentra ninguna línea válida
}