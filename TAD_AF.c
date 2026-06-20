#include "TAD_AF.h"
#include <stdlib.h>
#include <stdio.h>

//Funciones privadas, el static hace que las funciones solo existan en TAD_AF.c
static bool process_AFD(Automata* A, char* cadena);
static bool process_AFND(Automata* A, char* cadena);
static StateNode* find_state_data(Automata* A, Tdata name);

Automata* create_automata(){
    Automata* A = (Automata*) malloc(sizeof(Automata));
    A->states = NULL;
    A->q0 = NULL;
    A->deterministic = true; // Por defecto asumimos que el autómata es determinista
    return A;
}

void add_transition(Automata* A, char* from, char* symbol, char* to) {
    StateNode* fromState = find_state(A, from);
    StateNode* toState = find_state(A, to);

    if(fromState == NULL){
        printf("Estado origen inexistente\n");
        return;
    }
    
    if(toState == NULL){
        printf("Estado destino inexistente\n");
        return;
    }

    Transition* transicion = find_transition(fromState, symbol);
    // Caso: se encontro una transicion con el mismo simbolo
    if(transicion != NULL){
        if(!belongs(transicion->to, toState->name)){
            insert_set(&(transicion->to), copy(toState->name));
            A->deterministic = false;
        }

        return;
    }

    // Caso: la transicion es nueva, no hay transicion con el simbolo
    Transition* newTransition = (Transition*) malloc(sizeof(Transition));

    newTransition->symbol = str(symbol);

    newTransition->to = create_set();
    insert_set(&(newTransition->to), copy(toState->name));

    newTransition->next = fromState->transitions;
    fromState->transitions = newTransition;
}

void print_transition(Transition *transition){
	Transition *aux = transition;
	while(aux != NULL){
        printf("  +--- ");
		print_Tdata(aux->symbol);
		printf(" ---> ");
		print_Tdata(aux->to);
		printf("\n");
		aux = aux->next;
	}
}

void print_automata(Automata* A){
    if(A == NULL){
        printf("Automata vacio.\n");
        return;
    }

	printf("\n");
	printf("  +--------------------------------------------------+\n");
	printf("  |                   AUTOMATA                       |\n");
	printf("  +--------------------------------------------------+\n");
	printf("\n");
	printf("  Estado inicial: ");
	print_Tdata(A->q0->name);
	printf("\n");
	
	StateNode *aux = A->states;
	while(aux != NULL){
		printf("\n  ");
		print_Tdata(aux->name);
		if(aux->isFinal) printf(" [ACEPTACION]");
		printf("\n");
		printf("  %s\n", aux->transitions ? "|" : "  (sin transiciones)");
		
		Transition *t = aux->transitions;
        print_transition(t);

		aux = aux->next;
	}
	
	printf("\n");
	printf("  Tipo: %s\n", A->deterministic ? "AFD (Determinista)" : "AFND (No Determinista)");
	printf("\n");
}

void add_state(Automata* A, char* name, bool isFinal){
    if(find_state(A, name) != NULL){
        printf("El estado ya existe.\n");
        return;
    }

	StateNode *newState = (StateNode*)malloc(sizeof(StateNode));
	
	newState->name = str(name);
	newState->transitions = NULL;
	newState->isFinal = isFinal;
	
	newState->next = A->states;
	A->states = newState;
	
	if(A->q0 == NULL){
		A->q0 = newState;
	}
}

StateNode* find_state(Automata* A, char* name){
	StateNode* actual = A->states;
	Tdata searchName = str(name);

	while(actual != NULL){
		if(equals(actual->name, searchName)){
            free_Tdata(searchName);
			return actual;
		}
		actual = actual->next;
	}

    free_Tdata(searchName);
	return NULL;
}

Transition* find_transition(StateNode* q, char* symbol){
    if(q == NULL) return NULL;

    Tdata simbolo = str(symbol);
    Transition* actual = q->transitions;

    while(actual != NULL){
        if(equals(actual->symbol, simbolo)){
            free_Tdata(simbolo);
            return actual;
        }
        actual = actual->next;
    }

    free_Tdata(simbolo);
    return NULL;
}

Tdata delta(StateNode* q, char* symbol){
    if(q == NULL) return create_set();
    
    Transition* t = find_transition(q, symbol);
    if(t == NULL) return create_set();

    return copy(t->to);
}

static StateNode* find_state_data(Automata* A, Tdata name){
    StateNode* actual = A->states;

    while(actual != NULL){
        if(equals(actual->name, name))
            return actual;

        actual = actual->next;
    }

    return NULL;
}

static bool process_AFD(Automata* A, char* cadena){
    if(A->q0 == NULL) return false;

    StateNode* actual = A->q0;

    int i = 0;
    while(cadena[i] != '\0'){

        char simbolo[2];
        simbolo[0] = cadena[i];
        simbolo[1] = '\0';

        Tdata destinos = delta(actual, simbolo);

        if(cardinal(destinos) == 0){//estado sin transiciones
            free_Tdata(destinos);
            return false;
        }

        //Como es AFD sabemos que hay un solo estado.
        Tdata nodo = destinos->data;
        State estado = nodo->data;

        actual = find_state_data(A, estado);

        free_Tdata(destinos);
        i++;
    }

    return actual->isFinal;
}

static bool process_AFND(Automata* A, char* cadena){
    if(A->q0 == NULL) return false;

    Tdata actuales = create_set();
    insert_set(&actuales, copy(A->q0->name));

    int i = 0;
    while(cadena[i] != '\0'){

        char simbolo[2];
        simbolo[0] = cadena[i];
        simbolo[1] = '\0';

        Tdata siguientes = create_set();
        Tdata nodo = actuales->data;

        while(nodo != NULL){
            State estado = nodo->data;
            StateNode* q = find_state_data(A, estado);
            Tdata destinos = delta(q, simbolo);
            Tdata d = destinos->data;

            while(d != NULL){
                insert_set(&siguientes,copy(d->data));
                d = d->next;
            }

            free_Tdata(destinos);
            nodo = nodo->next;
        }

        free_Tdata(actuales);
        actuales = siguientes;
        i++;
    }

    //verificacion de aceptacion
    Tdata nodo = actuales->data;

    while(nodo != NULL){
        StateNode* q = find_state_data(A, nodo->data);
        if(q->isFinal){
            free_Tdata(actuales);
            return true;
        }
        nodo = nodo->next;
    }

    free_Tdata(actuales);
    return false;
}

bool process(Automata* A, char* cadena){
    if(A == NULL) return false;

    if(A->deterministic) return process_AFD(A, cadena);

    return process_AFND(A, cadena);
}