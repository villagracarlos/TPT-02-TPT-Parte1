Comparación de Propuestas de Modelado de Autómatas Finitos

Integrantes
•	Aramayo Juan Pablo
•	Vilca Saravia David Sebastián
•	Villagra Carlos Emanuel 

Para la implementación del TAD de Autómatas Finitos analizamos las distintas propuestas de representación. Todas las propuestas permiten modelar autómatas deterministas y no deterministas, aunque presentan diferencias en cuanto a eficiencia, facilidad de implementación.
A continuación, se resumen las propuestas analizadas y se justifica la elección realizada.

Propuesta 1: Delta como lista de transiciones
Esta propuesta almacena cada transición como una 3-upla.
Las transiciones se almacenan en una lista global perteneciente al autómata.
Ventajas
•	Implementación sencilla.
•	Fácil de debuggear.
•	Representación cercana a la definición matemática.
Desventajas
•	Para procesar una transición es necesario recorrer toda la lista.
•	El costo de búsqueda aumenta con la cantidad de transiciones.
•	Poco eficiente para simulaciones grandes.

Propuesta 2: Delta indexado por estado
En esta propuesta cada estado almacena directamente sus transiciones, modelando al autómata como un grafo.
Cada estado posee una lista de transiciones y cada transición contiene:
•	el símbolo de entrada;
•	el conjunto de estados destino.
Ventajas
•	Acceso rápido a las transiciones de un estado.
•	Representación natural mediante grafos.
•	Fácil implementación de AFD y AFND.
Desventajas
•	La estructura interna resulta más compleja que una lista global.
•	Requiere mayor cantidad de estructuras auxiliares.

Propuesta 3: Delta como matriz
Esta alternativa representa la función de transición mediante una matriz.
[\delta(q_i,a_j)]
Cada estado y símbolo se transforma en un índice entero.
Ventajas
•	Acceso de orden uno.
•	Muy eficiente.
Desventajas
•	Requiere numerar estados y símbolos.
•	Resulta poco flexible.
•	Desaprovecha memoria cuando existen pocas transiciones.
•	Complica la representación de AFND.

Propuesta 4: Delta funcional
Cada entrada representa directamente:
[\delta(q,a)={q_1,q_2}]
La estructura se aproxima mucho a la definición formal de la teoría.
Ventajas
•	Muy elegante matemáticamente.
•	Facilita el razonamiento formal.
•	Excelente para AFND.
•	Ideal para determinización
Desventajas
•	Las búsquedas requieren recorrer todas las entradas.
•	Menor eficiencia durante la simulación.

Propuesta 5: Estados compuestos
Esta propuesta se orienta principalmente al algoritmo de determinización.
Cada estado del AFD es un subconjunto de estados del AFND.
Ventajas
•	Ideal para determinizacion.
•	Muy alineado a la teoría formal.
•	Ideal para determinización
Desventajas
•	No resulta conveniente como representación principal.
•	Está pensada específicamente para la determinización.

Justificación de la propuesta elegida
Elegimos la Propuesta 2, la principal razón de esta elección es que representa al autómata como un grafo, permitiendo acceder directamente a las transiciones de cada estado. Esto reduce el costo de búsqueda durante el procesamiento de cadenas y facilita la simulación tanto de AFD como de AFND.
Además, la estructura elegida permite reutilizar los TAD desarrollados previamente, ya que los estados destino se almacenan mediante conjuntos implementados con el TAD_AST y así mismo pudiendo utilizar las implementaciones que trabajan con conjuntos.
Por estas razones, se consideramos que la Propuesta 2 ofrece el mejor equilibrio entre eficiencia y claridad de implementación.
Conclusión
El análisis de las distintas alternativas permitió comprender que una misma función matemática puede representarse mediante diversas estructuras de datos.
La solución elegida proporciona una representación eficiente, alineada con los objetivos del trabajo práctico.

