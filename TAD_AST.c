#include "TAD_AST.h"
#include <stdio.h>
#include <stdlib.h>

//Prototipos
Tdata copy(Tdata elem);
Tdata copy_list(Tdata list);
Tdata copy_set(Tdata set);

bool equals(Tdata a, Tdata b);
bool search(Tdata list, Tdata elem);
bool belongs(Tdata set, Tdata elem);

void free_Tdata(Tdata t);

//Definicion de funciones
Tdata create_str_ast(String str){
    Tdata nuevo = (Tdata)malloc(sizeof(struct dataType));
    nuevo->nodeType = STR;
    nuevo->string = copySTR(str);
    return nuevo;
}

Tdata str(char* texto){
    return create_str_ast(cargaSTR(texto));
}

Tdata create_list(){
    Tdata nuevo = (Tdata) malloc(sizeof(struct dataType));
    nuevo->nodeType = LIST;
    nuevo->data = NULL;
    nuevo->next = NULL;
    return nuevo;
}

Tdata create_set(){
    Tdata nuevo = (Tdata) malloc(sizeof(struct dataType));
    nuevo->nodeType = SET;
    nuevo->data = NULL;
    nuevo->next = NULL;
    return nuevo;
}

//OPERACIONES SOBRE LIST
void append(Tdata* list, Tdata elem){
    if(*list == NULL || (*list)->nodeType != LIST){
        printf("\nERROR: No es una lista");
        return;
    }

    Tdata nuevoNodo = (Tdata) malloc(sizeof(struct dataType));
    nuevoNodo->data = elem;
    nuevoNodo->next = NULL;

    if((*list)->data == NULL){
        (*list)->data = nuevoNodo;
        return;
    }

    Tdata temp = (*list)->data;
    while(temp->next != NULL){
        temp = temp->next;
    }
    temp->next = nuevoNodo;
}

int length(Tdata list){
    if(list == NULL || list->nodeType != LIST){
        printf("\nERROR: No es una lista");
        return -1;
    } 

    Tdata actual = list->data;
    int c=0;
    while(actual != NULL){
        c++;
        actual = actual->next;
    }
    return c;
}

Tdata copy(Tdata elem){
    if(elem == NULL) return NULL;

    if(elem->nodeType == STR){
        String nueva = copySTR(elem->string);
        return create_str_ast(nueva);
    }
    
    if(elem->nodeType == LIST){
        return copy_list(elem);
    }
    
    if(elem->nodeType == SET){
        return copy_set(elem);
    }
    
    return NULL;
}

Tdata copy_list(Tdata list){
    if(list->nodeType != LIST){
        printf("\nERROR: No es una lista");
        return create_list();
    } 

    Tdata copia = create_list();
    Tdata actual = list->data;

    while(actual != NULL){
        Tdata elemCopiado = copy(actual->data);
        append(&copia, elemCopiado);
        actual = actual->next;
    }
    return copia;
}

Tdata concat(Tdata list1, Tdata list2){
    if(list1 == NULL || list2 == NULL || list1->nodeType != LIST || list2->nodeType != LIST){
        printf("\nERROR: No se ingresaron dos listas");
        return create_list();
    }

    Tdata newLista = create_list();
    Tdata actual1 = list1->data;
    while(actual1 != NULL){
        Tdata elemCopiado = copy(actual1->data);
        append(&newLista, elemCopiado);
        actual1 = actual1->next;
    }
    Tdata actual2 = list2->data;
    while(actual2 != NULL){
        Tdata elemCopiado = copy(actual2->data);
        append(&newLista, elemCopiado);
        actual2 = actual2->next;
    }
    return newLista;
}

bool equals(Tdata a, Tdata b){
    if(a == NULL && b == NULL) return true;
    if(a == NULL || b == NULL) return false;
    if(a->nodeType != b->nodeType) return false;

    //Caso STR
    if(a->nodeType == STR) return esIgualSTR(a->string, b->string);

    //Caso LIST
    if(a->nodeType == LIST){
    Tdata l1 = a->data;
    Tdata l2 = b->data;

    while(l1 != NULL && l2 != NULL){
        if(!equals(l1->data, l2->data)) return false;
        l1 = l1->next;
        l2 = l2->next;
    }

    return (l1 == NULL && l2 == NULL);
    }

    //Caso SET
    if(a->nodeType == SET){
        Tdata e1 = a->data;

        while(e1 != NULL){
            if(!belongs(b, e1->data)) return false;
            e1 = e1->next;
        }

        // verificar al revés también
        Tdata e2 = b->data;
        while(e2 != NULL){
            if(!belongs(a, e2->data)) return false;
            e2 = e2->next;
        }
        return true;
    }
    return false;
}

bool search(Tdata list, Tdata elem){ //tambien lo uso para set
    if(list == NULL) return false;

    Tdata actual = list->data;
    while(actual != NULL){
        if(equals(actual->data, elem)) return true;
        actual = actual->next;
    }
    return false;
}

//OPERACIONES SOBRE SET
void insert_set(Tdata* set, Tdata elem){ //sin duplicados
    if((*set)->nodeType != SET){
        printf("\nERROR: No es un conjunto");
        return;
    }

    if(belongs(*set, elem)) return;

    Tdata nuevoNodo = (Tdata) malloc(sizeof(struct dataType));
    nuevoNodo->data = copy(elem);
    nuevoNodo->next = NULL;

    if((*set)->data == NULL){
        (*set)->data = nuevoNodo;
        return;
    }

    Tdata temp = (*set)->data;
    while(temp->next != NULL){
        temp = temp->next;
    }
    temp->next = nuevoNodo;
}

bool belongs(Tdata set, Tdata elem){
    if(set == NULL || set->nodeType != SET){
        printf("\nERROR: No es un conjunto o conjunto vacio");
        return false;
    }

    return search(set, elem);
}

void remove_set(Tdata *set, Tdata elem){
    if((*set)->nodeType != SET){
        printf("\nERROR: No es un conjunto");
        return;
    }

    Tdata actual = (*set)->data;
    Tdata ant = NULL;
    while(actual != NULL && !equals(actual->data, elem)){
        ant = actual;
        actual = actual->next;
    }
    
    if(actual == NULL) return; // no encontrado
    if(ant == NULL){ //es el primero
        (*set)->data = actual->next;
    } 
    else{// está en el medio o final
        ant->next = actual->next;
    }
    
    free_Tdata(actual->data);
    free(actual);
}

void free_Tdata(Tdata t){
    if(t == NULL) return;

    // Caso STR
    if(t->nodeType == STR){
        String s = t->string;
        while(s != NULL){
            String aux = s;
            s = s->sig;
            free(aux);
        }
    }

    // Caso LIST o SET
    else if(t->nodeType == LIST || t->nodeType == SET){
        Tdata actual = t->data;

        while(actual != NULL){
            Tdata aux = actual;
            free_Tdata(actual->data);
            actual = actual->next;
            free(aux);
        }
    }
    free(t);
}

Tdata copy_set(Tdata set){
    if(set->nodeType != SET){
        printf("\nERROR: No es un conjunto");
        return create_set();
    }

    Tdata copia = create_set();
    Tdata actual = set->data;
    
    while(actual != NULL){
        insert_set(&copia, actual->data);
        actual = actual->next;
    }
    return copia;
}

//Opreaciones algebraicas
Tdata union_set(Tdata set1, Tdata set2){
    if(set1 == NULL || set2 == NULL || set1->nodeType != SET || set2->nodeType != SET){
        printf("\nERROR: No se ingresaron dos conjuntos");
        return create_set();
    }

    Tdata newSet = copy(set1);
    Tdata actual = set2->data;

    while(actual != NULL){
        insert_set(&newSet, actual->data);
        actual = actual->next;
    }
    return newSet;
}

Tdata intersection_set(Tdata set1, Tdata set2){
    if(set1 == NULL || set2 == NULL || set1->nodeType != SET || set2->nodeType != SET){
        printf("\nERROR: No se ingresaron dos conjuntos");
        return create_set();
    }

    Tdata interseccion = create_set();
    Tdata actual = set1->data;

    while(actual != NULL){
        if(belongs(set2, actual->data)){
            insert_set(&interseccion, actual->data);
        }
        actual = actual->next;
    }
    return interseccion;
}

Tdata difference_set(Tdata set1, Tdata set2){ //set1 - set2
    if(set1 == NULL || set2 == NULL || set1->nodeType != SET || set2->nodeType != SET){
        printf("\nERROR: No se ingresaron dos conjuntos");
        return create_set();
    }

    Tdata diferencia = create_set();
    Tdata actual = set1->data;

    while(actual != NULL){
        if(!belongs(set2, actual->data)){
            insert_set(&diferencia, actual->data);
        }
        actual = actual->next;
    }
    return diferencia;
}

bool subset(Tdata set1, Tdata set2){ // set2 contiene a set1?
    if(set1 == NULL || set2 == NULL || set1->nodeType != SET || set2->nodeType != SET){
        printf("\nERROR: No se ingresaron dos conjuntos");
        return false;
    }

    Tdata actual = set1->data;

    while(actual != NULL){
        if(!belongs(set2, actual->data)){
            return false;
        }
        actual = actual->next;
    }
    return true;
}

bool equals_set(Tdata set1, Tdata set2){
    if(set1 == NULL || set2 == NULL || set1->nodeType != SET || set2->nodeType != SET){
        printf("\nERROR: No se ingresaron dos conjuntos");
        return false;
    }

    return (subset(set1,set2) == true && subset(set2,set1) == true);
}

void print_Tdata(Tdata t){
    if(t == NULL) return;

    //Caso STR
    if(t->nodeType == STR){
        printSTR(t->string);
        return;
    }

    //Caso LIST
    if(t->nodeType == LIST){
        printf("[");
        Tdata actual = t->data;

        while(actual != NULL){
            print_Tdata(actual->data);
            if(actual->next != NULL) printf(", ");
            actual = actual->next;
        }
        printf("]");
        return;
    }

    //Caso SET
    if(t->nodeType == SET){
        printf("{");
        Tdata actual = t->data;

        while(actual != NULL){
            print_Tdata(actual->data);
            if(actual->next != NULL) printf(", ");
            actual = actual->next;
        }
        printf("}");
        return;
    }
}

Tdata str_to_list(Tdata cadena){
    if(cadena == NULL || cadena->nodeType != STR){
        printf("\nERROR: No es ingreso un string");
        return create_list();
    }

    Tdata lista = create_list();
    String actual = copySTR(cadena->string);
    while(actual != NULL){
        Tdata caracter = str(&actual->dato);
        append(&lista, caracter);
        actual = actual->sig;
    }
    return lista;
}

Tdata listo_to_str(Tdata lista){
    if(lista == NULL || lista->nodeType != LIST){
        printf("\nERROR: No es ingreso una lista");
        return str("");
    }

    String nueva = crearSTR();
    Tdata actual = lista->data;
    while(actual != NULL){
        String cadena = copySTR(actual->data->string);
        nueva = concatSTR(nueva, cadena);
        actual = actual->next; 
    }
    return create_str_ast(nueva);
}

Tdata cartesian_product_set(Tdata set1, Tdata set2){
	if(set1 == NULL || set2 == NULL || set1->nodeType != SET || set2->nodeType != SET){
		printf("\nERROR: No se ingresaron dos conjuntos");
		return create_set();
	}
	Tdata retorno = create_set();
	Tdata actual = set1->data;
	
	while(actual != NULL){
		Tdata aux = set2->data;
		while(aux != NULL){
			Tdata uni = create_list();
			append(&uni, actual->data);
			append(&uni, aux->data);
			insert_set(&retorno,uni);
			//:v
			aux = aux->next;
		}
		actual = actual->next;
	}
	return retorno;
}