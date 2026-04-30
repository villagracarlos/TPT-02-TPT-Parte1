#include "TAD_AST.h"
#include <stdio.h>
#include <stdbool.h>

int main(){
    printf("\n================Union de conjuntos================");
    Tdata A = create_set();
    insert_set(&A, str("a"));
    insert_set(&A, str("b"));
    insert_set(&A, str("c"));

    Tdata B = create_set();
    insert_set(&B, str("c"));
    insert_set(&B, str("d"));
    insert_set(&B, str("e"));
    
    Tdata U = union_set(A, B);
    printf("\nConjunto A = "); print_Tdata(A);
    printf("\nConjunto B = "); print_Tdata(B);
    printf("\nUnion(A, B)= "); print_Tdata(U);

    printf("\n================Interseccion de conjuntos================");
    Tdata I = intersection_set(A, B);

    printf("\nConjunto A = "); print_Tdata(A);
    printf("\nConjunto B = "); print_Tdata(B);
    printf("\nInterseccion(A, B)= "); print_Tdata(I);

    printf("\n================Producto cartesiano de conjuntos================");
    printf("\nConjunto A = "); print_Tdata(A);
	printf("\nConjunto B = "); print_Tdata(B);
	Tdata PC = cartesian_product_set(A ,B);
	printf("\nProducto cartesiano ="); print_Tdata(PC);

    printf("\n================Conversion STR → LIST → STR================");
    Tdata cadena = str("HOLA");
    printf("\nCadena = "); print_Tdata(cadena);
    
    Tdata lista = str_to_list(cadena);
    printf("\nCadena a lista = "); print_Tdata(lista);

    Tdata cadena2 = listo_to_str(lista);
    printf("\nLista a cadena = "); print_Tdata(cadena2);

    printf("\n================Conjuntos vacios================");
    Tdata vacio = create_set();
    printf("\nConjunto vacio = "); print_Tdata(vacio);

    printf("\n================Elementos duplicados================");
    Tdata C = create_set();
    insert_set(&C, str("a"));
    insert_set(&C, str("a"));
    insert_set(&C, str("c"));

    printf("\nSe crea el conjunto C y se le carga: a, a, c");
    printf("\nConjunto C = "); print_Tdata(C);

    printf("\n================Listas anidadas================");
    Tdata listAni = create_list();
    append(&listAni, A);
    append(&listAni, cadena);
    append(&listAni, B);

    printf("\nConjunto A = "); print_Tdata(A);
    printf("\nCadena = "); print_Tdata(cadena);
    printf("\nConjunto B = "); print_Tdata(B);
    printf("\nLista anidada = "); print_Tdata(listAni);

    printf("\n================Copias vs referencias================");
    Tdata D = create_set();
    insert_set(&D, str("a"));

    Tdata E = copy_set(D);
    insert_set(&D, str("b"));

    printf("\nSe crea el conjunto D y se le carga 'a', se copia D en un nuevo conjunto E y se le agrega 'b' al conjunto D");
    printf("\nConjunto D = "); print_Tdata(D);
    printf("\nConjunto E = "); print_Tdata(E);

    return 0;
}