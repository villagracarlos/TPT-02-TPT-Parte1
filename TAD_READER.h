#ifndef TAD_READER_H
#define TAD_READER_H

#include "TAD_AF.h"

Automata* load_automata(char* filename);

//devuelve la primer linea del archivo de texto.
char* get_first_line(char* filename);
#endif