/* Universidad Nacional de Itapua.
   Proyecto 2 - Algoritmos y Estructuras de Datos I
   Listas y Polinomios
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



/* TRUE/OK  o FALSE/ERROR */
typedef char BOOLEAN;

#define TRUE 1
#define FALSE 0

#define ERROR FALSE
#define OK TRUE

#define CONFIRM_TRUE(v, retval) \
    if (!(v)) { \
        printf("Error en linea %u de archivo %s\n", __LINE__, __FILE__); \
        system("pause"); \
        return (retval); \
    }

#define CONFIRM_NOTNULL(v, retval) \
    if (NULL == v) { \
        printf("Error en linea %u de archivo %s\n", __LINE__, __FILE__); \
        system("pause"); \
        return (retval); \
    }



/**

    Esta seccion contiene los esqueletos para la implementacion de una lista con listas ligadas
	simples.

*/

/************************************************************
        L I S T A   C O N   L I S T A S   L I G A D A S
/***********************************************************/

/* La estructura de la lista */
typedef struct _LList {
    void* value;
    struct _LList* sig;
} LList;


/* Crea la lista
    Retorna NULL si hubo algun error (ej: no hay suficiente memoria)
    Inicializar la lista aqui
*/
LList* LList_create() {
    LList* lista = (LList*)malloc(sizeof(LList));   // Asigna memoria para la lista
    CONFIRM_NOTNULL(lista, NULL);                   // Verifica si se pudo asignar memoria
    lista->value = NULL;                            // Inicializa el valor del nodo como NULL
    lista->sig = NULL;                              // Inicializa el siguiente nodo como NULL
    return lista;                                   // Retorna la lista creada
}

/* Destruye la lista. No se debe utilizar mas la lista despues de destruirla.
    Retorna OK si no hay errores, sino ERROR
*/
BOOLEAN LList_destroy(LList* lista) {
    CONFIRM_NOTNULL(lista, ERROR);                  // Verifica si la lista es NULL

    LList* current = lista;                         // Inicializa el puntero actual al inicio de la lista
    LList* next;                                    // Declara un puntero para almacenar el siguiente nodo

    while (current != NULL) {                       // Itera sobre la lista
        next = current->sig;                        // Almacena el siguiente nodo antes de liberar el actual
        free(current);                              // Libera la memoria del nodo actual
        current = next;                             // Mueve al siguiente nodo
    }

    return OK;                                      // Retorna OK después de destruir la lista
}

/* Retorna el tamano (la cantidad de elementos dentro de la lista)
    Retorna -1 si hubo un error
*/
int LList_size(LList* lista) {
    CONFIRM_NOTNULL(lista, -1);                     // Verifica si la lista es NULL

    int count = 0;                                  // Inicializa el contador de elementos

    LList* current = lista;                         // Inicializa el puntero actual al inicio de la lista
    while (current != NULL) {                       // Itera sobre la lista
        count++;                                    // Incrementa el contador
        current = current->sig;                     // Mueve al siguiente nodo
    }

    return count;                                   // Retorna el tamaño de la lista
}



/* Agrega un valor al final de la lista
    Retorna ERROR si hay algun error (ej: argumento invalido)
*/
BOOLEAN LList_add(LList* lista, void* valor) {
    CONFIRM_NOTNULL(lista, ERROR);                  // Verifica si la lista es NULL

    LList* nuevoNodo=(LList*)malloc(sizeof(LList)); // Crear un nuevo nodo
    CONFIRM_NOTNULL(nuevoNodo, ERROR);              // Verifica si se pudo asignar memoria para el nuevo nodo

    nuevoNodo->value = valor;                       // Establece el valor del nuevo nodo
    nuevoNodo->sig = NULL;                          // El nuevo nodo será el último, por lo que su siguiente es NULL

    if (lista->sig == NULL) {                       // Si la lista está vacía
        lista->sig = nuevoNodo;                     // El nuevo nodo se convierte en el primer nodo de la lista
    } else {
        LList* current = lista->sig;                // Inicializa el puntero actual al primer nodo de la lista
        while (current->sig != NULL) {              // Encuentra el último nodo de la lista
            current = current->sig;                 // Mueve al siguiente nodo
        }
        current->sig = nuevoNodo;                   // Establece el siguiente del último nodo como el nuevo nodo
    }

    return OK; // Retornar OK para indicar que el valor se agregó correctamente
}

/* Inserta un valor antes de la posicion dada
    Retorna OK si no hay errores, sino ERROR.
    Ej index = 0 primera posicion
    index = 1 segunda posicion..
    index = n (n+1) posicion.
*/
BOOLEAN LList_addPos(LList* lista, int index, void* valor) {
    CONFIRM_NOTNULL(lista, ERROR);                  // Verifica si la lista es NULL
    CONFIRM_TRUE(index >= 0, ERROR);                // Verifica si el índice es válido

    LList* nuevoNodo=(LList*)malloc(sizeof(LList)); // Crea un nuevo nodo
    CONFIRM_NOTNULL(nuevoNodo, ERROR);              // Verifica si se pudo asignar memoria para el nuevo nodo

    nuevoNodo->value = valor;                       // Establece el valor del nuevo nodo

    if (index == 0) {                               // Si se inserta en la primera posición
        nuevoNodo->sig = lista->sig;                // El siguiente del nuevo nodo es el primer nodo de la lista
        lista->sig = nuevoNodo;                     // El nuevo nodo se convierte en el primer nodo de la lista
    } else {
        LList* current = lista->sig;                // Inicializa el puntero actual al primer nodo de la lista
        int i = 0;
        while (current != NULL && i < index - 1) {  // Encuentra el nodo en la posición anterior a la posición dada
            current = current->sig;                 // Mueve al siguiente nodo
            i++;
        }
        if (current == NULL) {                      // Si el índice es mayor que el tamaño de la lista
            free(nuevoNodo);                        // Libera la memoria del nuevo nodo
            return ERROR;
        }
        nuevoNodo->sig = current->sig;              // El siguiente del nuevo nodo es el siguiente del nodo actual
        current->sig = nuevoNodo;                   // El siguiente del nodo actual es el nuevo nodo
    }

    return OK;
}


/* Carga el parametro pResultado con el valor
   en la posicion dada de la lista
    Retorna OK si no hay errores, sino ERROR
    Retorna ERROR si no hay mas
    Si hay error pResultado no cambia
*/
BOOLEAN LList_get(LList* lista, int index, void** pResultado) {
    CONFIRM_NOTNULL(lista, ERROR);                  // Verifica si la lista es NULL
    CONFIRM_TRUE(index >= 0, ERROR);                // Verifica si el índice es válido

    LList* current = lista->sig;                    // Inicializa el puntero actual al primer nodo de la lista
    int i = 0;
    while (current != NULL && i < index) {          // Encuenta el nodo en la posición dada
        current = current->sig;                     // Mueve al siguiente nodo
        i++;
    }

    if (current == NULL) {                          // Si el índice es mayor que el tamaño de la lista
        return ERROR;                               // Retorna ERROR
    }

    *pResultado = current->value;                   // Carga el valor del nodo en el parámetro pResultado
    return OK;
}



/* Borra el dato de la lista especificado por la posicion dada
    Retorna OK si no hay errores, sino ERROR
*/
BOOLEAN LList_remove(LList* lista, int index) {
    CONFIRM_NOTNULL(lista, ERROR);                  // Verifica si la lista es NULL
    CONFIRM_TRUE(index >= 0, ERROR);                // Verifica si el índice es válido

    LList* prev = NULL;                             // Puntero al nodo anterior al nodo que se va a eliminar
    LList* current = lista->sig;                    // Inicializa el puntero actual al primer nodo de la lista
    int i = 0;
    while (current != NULL && i < index) {          // Busca el nodo en la posición dada
        prev = current;                             // Almacena el nodo anterior al nodo actual
        current = current->sig;                     // Mueve al siguiente nodo
        i++;
    }

    if (current == NULL) {                          // Si el índice es mayor que el tamaño de la lista
        return ERROR;                               // Retornar ERROR
    }

    if (prev == NULL) {                             // Si el nodo a eliminar es el primer nodo de la lista
        lista->sig = current->sig;                  // El siguiente del primer nodo se convierte en el nuevo primer nodo
    } else {
        prev->sig = current->sig;                   // El siguiente del nodo anterior al nodo a eliminar es el siguiente del nodo a eliminar
    }

    free(current);                                  // Liberar la memoria del nodo
    return OK;}

/* Borra el valor de la lista.
    Retorna OK si no hay errores, sino ERROR
*/
BOOLEAN LList_removeValue(LList* lista, void* valor) {
    CONFIRM_NOTNULL(lista, ERROR);                  // Verifica si la lista es NULL

    LList* prev = NULL;                             // Puntero al nodo anterior al nodo que se va a eliminar
    LList* current = lista->sig;                    // Inicializa el puntero actual al primer nodo de la lista

                                                    // Busca el nodo que contiene el valor a eliminar
    while (current != NULL && current->value != valor) {
        prev = current;                             // Almacena el nodo anterior al nodo actual
        current = current->sig;                     // Mueve al siguiente nodo
    }

    if (current == NULL) {                          // Si no se encontró el valor en la lista
        return ERROR;                               // Retorna ERROR
    }

    if (prev == NULL) {                             // Si el nodo a eliminar es el primer nodo de la lista
        lista->sig = current->sig;                  // El siguiente del primer nodo se convierte en el nuevo primer nodo
    } else {
        prev->sig = current->sig;                   // El siguiente del nodo anterior al nodo a eliminar es el siguiente del nodo a eliminar
    }

    free(current);                                  // Libera la memoria del nodo
    return OK;
}

/************************************************************
 *      C A L C U L A T O R
************************************************************/

/**
 * Reads a polynomial from the standard input (keyboard).
 * 
 * Returns:
 *   char*: The polynomial entered by the user.
 */
char* readPolynomial() {
    char* polynomial = (char*)malloc(100 * sizeof(char)); // Allocate memory for the polynomial string
    if (polynomial == NULL) {
        printf("Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    printf("Enter the polynomial: ");               // Prompt the user to enter the polynomial
    scanf("%99[^\n]", polynomial);                  // Read the polynomial from standard input (up to 99 characters excluding newline)
    getchar();                                      // Consume the newline character left in the input buffer

    return polynomial; // Return the polynomial string
}

/**
 * Checks if the entered equation is valid and follows an acceptable format for a polynomial.
 * 
 * Parameters:
 *   equation: The equation entered by the user.
 * 
 * Returns:
 *   bool: true if the equation is valid, false otherwise.
 */
BOOLEAN isValidPolynomial(const char* equation) {
    int i = 0;
    char prevChar = '\0';
    BOOLEAN validTerm = ERROR;

    // Iterate through each character in the equation
    while (equation[i] != '\0') {
        char currentChar = equation[i];

        // Skip whitespace characters
        if (isspace(currentChar)) {
            i++;
            continue;
        }

        // Check if the current character is valid
        if (!isdigit(currentChar) && currentChar != 'x' && currentChar != '^' && currentChar != '+' && currentChar != '-' && currentChar != '*' && currentChar != '/' && currentChar != '.' && currentChar != '^') {
            return ERROR; // Invalid character found
        }

        // Check if consecutive operators are present
        if ((currentChar == '+' || currentChar == '-' || currentChar == '*' || currentChar == '/') && (prevChar == '+' || prevChar == '-' || prevChar == '*' || prevChar == '/')) {
            return ERROR; // Consecutive operators found
        }

        // Check if 'x' is followed by '^'
        if (currentChar == 'x' && equation[i + 1] != '^') {
            return ERROR; // Invalid 'x' term found
        }

        // Check if '^' is followed by a digit
        if (currentChar == '^' && !isdigit(equation[i + 1])) {
            return ERROR; // Invalid exponent found
        }

        // Check if there are valid terms in the equation
        if ((isdigit(currentChar) || currentChar == 'x') && (prevChar == '+' || prevChar == '-' || prevChar == '\0')) {
            validTerm = OK; // A valid term is found
        } else if (prevChar == '^' && !isdigit(currentChar)) {
            return ERROR; // Invalid coefficient found after '^'
        }

        prevChar = currentChar;
        i++;
    }

    return validTerm; // Return OK if at least one valid term is found
}


/************************************************************
        M A I N
/***********************************************************/

BOOLEAN testLList() {
    LList* lista = LList_create();                  // Crea una lista ligada
    CONFIRM_NOTNULL(lista, ERROR);                  // Verifica si la lista es NULL

    // Agrega algunos elementos a la lista
    CONFIRM_TRUE(LList_add(lista, "apple"), ERROR);
    CONFIRM_TRUE(LList_add(lista, "banana"), ERROR);
    CONFIRM_TRUE(LList_add(lista, "orange"), ERROR);

    // Obtiene el primer elemento de la lista
    char* value = NULL;
    CONFIRM_TRUE(LList_get(lista, 0, (void**)&value), ERROR);
    CONFIRM_TRUE(strcmp("apple", value) == 0, ERROR);

    // Obtiene un elemento fuera del rango de la lista
    CONFIRM_TRUE(LList_get(lista, 3, (void**)&value) == ERROR, OK);

    // Inserta un elemento en una posición específica
    CONFIRM_TRUE(LList_addPos(lista, 1, "grape"), ERROR);
    CONFIRM_TRUE(LList_get(lista, 1, (void**)&value), ERROR);
    CONFIRM_TRUE(strcmp("grape", value) == 0, ERROR);

    // Elimina un elemento de la lista
    CONFIRM_TRUE(LList_remove(lista, 2), ERROR);

    // Elimina un valor de la lista
    CONFIRM_TRUE(LList_removeValue(lista, "orange"), ERROR);

    // Destruye la lista
    CONFIRM_TRUE(LList_destroy(lista), ERROR);

    printf("Everything OK buddy, your list works as a well lubed and silent car\n");
    return FALSE; // La función siempre retorna FALSE porque no se espera ningún error
}

int main(int argc, char *argv[]){
    //testLList();

     // Prueba de ecuación válida
    const char* validEquation = "2x^2 + 3x - 1";
    BOOLEAN isValid = isValidPolynomial(validEquation);
    printf("Is the equation '%s' valid? %s\n", validEquation, isValid ? "Yes" : "No");

    // Prueba de ecuación no válida
    const char* invalidEquation = "2x^2 + 3y - 1";
    isValid = isValidPolynomial(invalidEquation);
    printf("Is the equation '%s' valid? %s\n", invalidEquation, isValid ? "Yes" : "No");


    return 0;
}
