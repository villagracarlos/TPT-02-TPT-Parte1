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
Automata* determinize(Automata* AFND) {
	if (AFND == NULL) return NULL;
	if (AFND->deterministic) return AFND;
	
	Automata* AFD = create_automata();
	AFD->deterministic = true;
	
	Tdata sigma = create_set();//conjnto sigma de simbolos
	StateNode* q = AFND->states;
	while (q != NULL) {
		Transition* t = q->transitions;
		while (t != NULL) {
			insert_set(&sigma, t->symbol);
			t = t->next;
		}
		q = q->next;
	}
	
	Tdata subsets = create_list();
	Tdata names   = create_list();
	int counter   = 0;//contador para nombrar los estados cundo los agrege
	int processed = 0;//que se va a procesar despues
	
	//el estado inicial p0 sera {q0}
	Tdata q0set = create_set();
	insert_set(&q0set, copy(AFND->q0->name));
	
	char pname[3];
	pname[0] = 'p';
	pname[1] = '0' + counter;
	pname[2] = '\0';
	counter++;

	append(&subsets, q0set);
	append(&names, str(pname));
	
	bool isFinal = false;
	Tdata nodo = q0set->data;
	while (nodo != NULL) {
		if (find_state_data(AFND, nodo->data)->isFinal) { 
			isFinal = true; 
			break; 
		}
		nodo = nodo->next;
	}
	add_state(AFD, pname, isFinal);
	AFD->q0 = AFD->states;
	
	while (processed < length(subsets)) {
		Tdata curSet  = NULL;
		Tdata curName = NULL;
		Tdata it = subsets->data;
		int   idx = 0;
		while (it != NULL) {
			if (idx == processed) {
				curSet = it->data; break; 
			}
			it = it->next; idx++;
		}
		it = names->data; idx = 0;
		while (it != NULL) {
			if (idx == processed) {
				curName = it->data; break; 
			}
			it = it->next; idx++;
		}
		
		char fromBuf[20];
		String s = curName->string;
		int i = 0;
		while (s != NULL) { fromBuf[i++] = s->dato; s = s->sig; }
		fromBuf[i] = '\0';
		
		Tdata sigNodo = sigma->data;
		while (sigNodo != NULL) {
			char symBuf[20];
			String sym = sigNodo->data->string;
			i = 0;
			while (sym != NULL) { symBuf[i++] = sym->dato; sym = sym->sig; }
			symBuf[i] = '\0';
			
			Tdata dest = create_set();
			Tdata qi = curSet->data;
			while (qi != NULL) {
				StateNode* node = find_state_data(AFND, qi->data);
				if (node != NULL) {
					Tdata d = delta(node, symBuf);
					Tdata dj = d->data;
					while (dj != NULL) {
						insert_set(&dest, dj->data);
						dj = dj->next;
					}
					free_Tdata(d);
				}
				qi = qi->next;
			}
			
			if (cardinal(dest) == 0) {
				free_Tdata(dest);
				sigNodo = sigNodo->next;
				continue;
			}
			
			char toBuf[4];
			bool found = false;
			Tdata si = subsets->data;
			Tdata ni = names->data;
			while (si != NULL) {
				if (equals_set(si->data, dest)) {
					String ns = ni->data->string;
					int j = 0;
					while (ns != NULL) {
						toBuf[j++] = ns->dato; ns = ns->sig; 
					}
					toBuf[j] = '\0';
					found = true;
					break;
				}
				si = si->next;
				ni = ni->next;
			}
			
			if (!found) {
				toBuf[0] = 'p';
				toBuf[1] = '0' + counter;
				toBuf[2] = '\0';
				counter++;
				
				append(&subsets, dest);
				append(&names, str(toBuf));
				
				isFinal = false;
				Tdata d2 = dest->data;
				while (d2 != NULL) {
					if (find_state_data(AFND, d2->data)->isFinal) {
						isFinal = true; break; 
					}
					d2 = d2->next;
				}
				add_state(AFD, toBuf, isFinal);
			} else {
				free_Tdata(dest);
			}
			
			add_transition(AFD, fromBuf, symBuf, toBuf);
			sigNodo = sigNodo->next;
		}
		processed++;
	}
	
	free_Tdata(sigma);
	free_Tdata(subsets);
	free_Tdata(names);
	
	return AFD;
}
