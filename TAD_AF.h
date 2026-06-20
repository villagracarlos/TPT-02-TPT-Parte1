#ifndef TAD_AF_H
#define TAD_AF_H

#include "TAD_AST.h"
#include <stdbool.h>

typedef Tdata State;
typedef Tdata Symbol;
                      
//forward declaration para ser cool
typedef struct transition Transition; 
typedef struct stateNode StateNode;

struct transition{
    Symbol symbol;

    Tdata to; // SET de estados destino

    Transition* next;
};

struct stateNode{
    State name;

    Transition* transitions;

    bool isFinal;

    StateNode* next;
};
//La estructura de stateNode representa q0: a -> {q1,q2}    Se representa como grafos
//                                          b -> {q3}

typedef struct{
    StateNode* states;

    //State q0; Usamos StateNode para poder acceder a las transiciones de q0
    //          sin la necesidad de tener que usar find_state(A, "q0");
    StateNode* q0;

    bool deterministic;
} Automata;

Automata* create_automata();

void add_state(Automata* A, char* name, bool isFinal);
//ejemplo add_state(A,"q0",false); add_state(A,"q1",true);

StateNode* find_state(Automata* A, char* name);

void add_transition(Automata* A, char* from, char* symbol, char* to); 
//ejemplo add_transition(A,"q0","a","q1"); add_transition(A,"q0","a","q2");

Transition* find_transition(StateNode* q, char* symbol);

void print_transition(Transition* transition);

void print_automata(Automata* A);

Tdata delta(StateNode* q, char* symbol); //Sea q0, a → {q1,q2} devuelve {q1,q2}

bool process(Automata* A, char* cadena); //ejemplo process(A, "abba");

//Automata* determinize(Automata* AFND); //Vuelve determinista un automata

#endif
