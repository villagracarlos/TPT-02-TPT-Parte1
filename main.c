#include "TAD_AF.h"
#include "TAD_READER.h"
#include <stdio.h>
#include <stdbool.h>

void harcodeado();
void archivotxt();

int main(){
    int opcion=0;

    printf("Ingrese tipo de carga:\n");
    printf("Harcodeado = 1\n");
    printf("Archivo de texto = 2\n");
    printf("Salir = 0\n");

    fflush(stdout);
    printf("Opcion:");
    scanf("%d", &opcion);
    
    switch(opcion){
        case 1:
            harcodeado();
            break;
        case 2:
            archivotxt();
            break;
        default:
            printf("Saliendo del programa.\n");
    }

    return 0;
}

void harcodeado(){
    // ============================================================
    //  Ejemplo 1: AFD que reconoce cadenas que terminan en 'b'
    // ============================================================
    printf("\n\n");
    printf("  ==================================================\n");
    printf("  AFD 1 - Reconoce cadenas que terminan en 'b'\n");
    printf("  ==================================================\n");

    Automata *AFD1 = create_automata();
    add_state(AFD1, "q0", false);
    add_state(AFD1, "q1", true);
    add_transition(AFD1, "q0", "a", "q0");
    add_transition(AFD1, "q0", "b", "q1");
    add_transition(AFD1, "q1", "a", "q0");
    add_transition(AFD1, "q1", "b", "q1");

    print_automata(AFD1);

    printf("Pruebas:\n");
    printf("epsilon-> %s\n",process(AFD1, "") ? "ACEPTADA" : "RECHAZADA");
    printf("a      -> %s\n",process(AFD1, "a") ? "ACEPTADA" : "RECHAZADA");
    printf("b      -> %s\n",process(AFD1, "b") ? "ACEPTADA" : "RECHAZADA");
    printf("ab     -> %s\n",process(AFD1, "ab") ? "ACEPTADA" : "RECHAZADA");
    printf("aba    -> %s\n",process(AFD1, "aba") ? "ACEPTADA" : "RECHAZADA");
    
    // ============================================================
    //  Ejemplo 2: AFD que reconoce el lenguaje "ab"
    // ============================================================
    printf("  ==================================================\n");
    printf("  AFD 2 - Reconoce la cadena \"ab\"\n");
    printf("  ==================================================\n");

    Automata *AFD2 = create_automata();
    add_state(AFD2, "q0", false);
    add_state(AFD2, "q1", false);
    add_state(AFD2, "q2", true);
    add_transition(AFD2, "q0", "a", "q1");
    add_transition(AFD2, "q0", "b", "q0");
    add_transition(AFD2, "q1", "a", "q1");
    add_transition(AFD2, "q1", "b", "q2");
    add_transition(AFD2, "q2", "a", "q2");
    add_transition(AFD2, "q2", "b", "q2");

    print_automata(AFD2);

    // ============================================================
    //  Ejemplo 3: AFND con transiciones multiples
    // ============================================================
    printf("  ==================================================\n");
    printf("  AFND 1 - Transiciones multiples desde q0\n");
    printf("  ==================================================\n");

    Automata *AFND1 = create_automata();
    add_state(AFND1, "q0", false);
    add_state(AFND1, "q1", true);
    add_transition(AFND1, "q0", "a", "q0");
    add_transition(AFND1, "q0", "a", "q1");
    add_transition(AFND1, "q0", "b", "q1");
    add_transition(AFND1, "q1", "a", "q0");

    print_automata(AFND1);

    // ============================================================
    //  Ejemplo 4: AFND que contiene "ab"
    // ============================================================
    printf("  ==================================================\n");
    printf("  AFND 2 - Contiene \"ab\" como subcadena\n");
    printf("  ==================================================\n");

    Automata *AFND2 = create_automata();
    add_state(AFND2, "q0", false);
    add_state(AFND2, "q1", false);
    add_state(AFND2, "q2", true);
    add_transition(AFND2, "q0", "a", "q0");
    add_transition(AFND2, "q0", "b", "q0");
    add_transition(AFND2, "q0", "a", "q1");
    add_transition(AFND2, "q1", "b", "q2");
    add_transition(AFND2, "q2", "a", "q2");
    add_transition(AFND2, "q2", "b", "q2");

    print_automata(AFND2);
    
    printf("Pruebas:\n");
    printf("epsilon-> %s\n",process(AFND2, "") ? "ACEPTADA" : "RECHAZADA");
    printf("a      -> %s\n",process(AFND2, "a") ? "ACEPTADA" : "RECHAZADA");
    printf("b      -> %s\n",process(AFND2, "b") ? "ACEPTADA" : "RECHAZADA");
    printf("ab     -> %s\n",process(AFND2, "ab") ? "ACEPTADA" : "RECHAZADA");
    printf("aba    -> %s\n",process(AFND2, "aba") ? "ACEPTADA" : "RECHAZADA");
    printf("abab   -> %s\n",process(AFND2, "abab") ? "ACEPTADA" : "RECHAZADA");
    printf("aaaa   -> %s\n",process(AFND2, "aaaa") ? "ACEPTADA" : "RECHAZADA");
    printf("ba     -> %s\n",process(AFND2, "ba") ? "ACEPTADA" : "RECHAZADA");
    printf("bab    -> %s\n",process(AFND2, "bab") ? "ACEPTADA" : "RECHAZADA");
    printf("bbbabbb-> %s\n",process(AFND2, "bbbabbb") ? "ACEPTADA" : "RECHAZADA");
}

void archivotxt(){
    char nombreArchivo[100];
    char cadena[100];
    int opcion=0;

    printf("Ingrese el nombre del archivo mas su extension (.txt): ");
    scanf("%99s", nombreArchivo);   //%99s para limitar la longitud del texto ingresado

    Automata* A = load_automata(nombreArchivo);
    if(A == NULL){
        printf("No se pudo cargar el automata.\n");
        return;
    }

    printf("  ==================================================\n");
    printf("  %s", get_first_line(nombreArchivo));
    printf("  ==================================================\n");
    print_automata(A);

    do{
        printf("Ingregse una cadena a evaluar: ");
        scanf("%99s", cadena);
        if(process(A, cadena)) printf("Cadena aceptada.\n");
        else printf("Cadena rechazada.\n");

        printf("Ingresar otra cadena? Si=1 No=0\t");
        scanf("%d", &opcion);
    }while(opcion == 1);
    
}