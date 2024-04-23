/* Universidad Nacional de Itapua.
   Proyecto 2 - Algoritmos y Estructuras de Datos I
   Listas y Polinomios
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>


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
        free(current->value);
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

    return count-1;                                   // Retorna el tamaño de la lista
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
    }
    else {
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

/*
 * Function to print all values stored in the LList
 */
void printLList(LList *list) {
    LList* current = list->sig;                          // Initialize current pointer to the beginning of the list
    while (current != NULL) {
        printf("%s\n", (char*)current->value);      // Print the value of the current node
        current = current->sig;                     // Move to the next node
    }
}

void printLListEquation(LList *list){
LList* current = list->sig;                          // Initialize current pointer to the beginning of the list
    while (current != NULL) {
        printf("%s ", (char*)current->value);      // Print the value of the current node
        current = current->sig;                     // Move to the next node
    }
    printf("\n");
}

/************************************************************
 *      C A L C U L A T O R
************************************************************/

BOOLEAN contains(const char *text, char value) {
    while (*text != '\0') {
        if (*text == value) {
            return TRUE;
        }
        text++;
    }
    return FALSE;
}

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

    return polynomial;                              // Return the polynomial string
}

void printPolynomial(const char* polynomial) {
    printf("Polynomial: %s\n", polynomial);
}

/**
 * Checks if the entered equation is valid and contains only the specified characters.
 * Allowed characters: Space, ^, x, number, +, and -
 * 
 * Parameters:
 *   equation: The equation string
 * 
 * Returns:
 *   BOOLEAN: OK if the equation follows the rules and contains only the allowed characters, ERROR otherwise.
 */
BOOLEAN isValidPolynomial(const char* equation) {
    int i = 0;

    // Iterate through each character in the equation
    while (equation[i] != '\0') {
        char currentChar = equation[i];

        /*********************************
        *   Skip whitespace characters   *
        *********************************/
        if (isspace(currentChar)) {
            i++;
            continue;
        }

        /************************************************
        *   Check if the current character is allowed   *
        ************************************************/
        // Number, 'x', '^', '+' or '-'
        if (!isdigit(currentChar) && currentChar != 'x' && currentChar != '^' && currentChar != '+' && currentChar != '-') {
            return ERROR; // Invalid character found
        }

        /*********************************
        *     Check additional rules     *
        *********************************/
        // After each '^', there must be a number
        if (currentChar == '^') {
            if (!isdigit(equation[i + 1])) {
                return ERROR;
            }
        }
        
        // After 'x', only one of '^', '-', '+', space, or nothing (if it's the last character) is allowed
        else if (currentChar == 'x') {
            char nextChar = equation[i + 1];
            if (nextChar != '^' && nextChar != '-' && nextChar != '+' && !isspace(nextChar) && nextChar != '\0') {
                return ERROR;
            }
        }
        
        // After each number, only 'x', '^', '-', '+', space, or nothing (if it's the last character) is allowed
        else if (isdigit(currentChar)) {
            char nextChar = equation[i + 1];
            if (nextChar != 'x' && nextChar != '^' && nextChar != '-' && nextChar != '+' && !isspace(nextChar) && nextChar != '\0') {
                return ERROR;
            }
        }

        i++;
    }

    return OK; // All characters are valid
}

/**
 * Prompts the user to enter the value of x for the equation.
 * 
 * Returns:
 *   int: The value of x entered by the user.
 */
int enterXValue() {
    int xValue;

    printf("Enter the value of x: ");
    scanf("%d", &xValue);

    return xValue;
}

/*
 * Function to remove spaces from a string
 */
char* removeSpaces(const char* equation) {
    // Allocate memory for the new string
    char* result = (char*)malloc(strlen(equation) + 1);
    if (result == NULL) {
        printf("Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    // Copy characters from the original string to the new string, excluding spaces
    int j = 0;
    for (int i = 0; equation[i] != '\0'; i++) {
        if (equation[i] != ' ') {
            result[j++] = equation[i];
        }
    }

    // Add null terminator to the end of the new string
    result[j] = '\0';

    return result;
}

LList* parseEquation(const char* equation) {
    LList* termsList = LList_create();
    CONFIRM_NOTNULL(termsList, ERROR);

    char* equationCopy = removeSpaces(equation);  // Assign the result of removeSpaces back to equationCopy
    
    int i = 0;
    
    while (1) { // Infinite loop
        // Traverse the string until finding a + or -
        char* before_sign = malloc(strlen(equationCopy) + 1); // Use malloc to allocate dynamic memory
        CONFIRM_NOTNULL(before_sign, ERROR);

        char* sign = malloc(2); // To store only one character, we need 2 bytes (one for the character and one for the null terminator)
        CONFIRM_NOTNULL(sign, ERROR);

        int j = 0;
        while (equationCopy[i] != '\0' && equationCopy[i] != '+' && equationCopy[i] != '-') {
            before_sign[j++] = equationCopy[i++];
        }

        before_sign[j] = '\0'; // Add the null terminator at the end

        // If a + or - is found, save it
        if (equationCopy[i] == '+' || equationCopy[i] == '-') {
            *sign = equationCopy[i];
            *(sign + 1) = '\0'; // Add the null terminator
            i++; // Move to the next character after the sign
        } 
        
        else {
            LList_add(termsList, strdup(before_sign));
            // If no more signs are found, print the remaining text and exit the loop
            //printf("Text before the sign: %s\n", before_sign);
            //printf("No sign found in the string.\n");
            break;
        }

        LList_add(termsList, strdup(before_sign));
        LList_add(termsList, strdup(sign));
        // Print the results
        //printf("Text before the sign: %s\n", before_sign);
        //printf("Sign found: %c\n", *sign);

        free(before_sign);
        free(sign);
    }

    return termsList;
}

int getPower(const char *string){
    int power = 0;
    
    const char *ptr = strchr(string, '^');

    if (ptr == NULL) {
        // Si no se encuentra el símbolo '^'
        power = 1;
    } else {
        // Si se encuentra el símbolo '^', obtener el número que le sigue al signo
        ptr++; // Avanza al siguiente caracter después del '^'
        power = atoi(ptr); // Convierte el número después del '^' a entero
    }
    return power;
}

// Función para reemplazar el coeficiente en un monomio
char *cambiar_coeficiente(char *monomio, int nuevo_coeficiente) {
    // Encontrar la posición de 'x' en el monomio
    char *posicion_x = strchr(monomio, 'x');
    if (posicion_x == NULL) {
        printf("Error: El monomio no contiene 'x'.\n");
        return monomio; // Devolver el monomio original si no contiene 'x'
    }

    // Obtener la longitud del coeficiente actual
    size_t longitud_coeficiente = posicion_x - monomio;

    // Crear una nueva cadena para el monomio con el nuevo coeficiente
    char *nuevo_monomio = malloc(longitud_coeficiente + 10); // Asumiendo que el nuevo coeficiente tenga un máximo de 10 dígitos
    if (nuevo_monomio == NULL) {
        printf("Error: No se pudo asignar memoria.\n");
        exit(1);
    }

    // Copiar el nuevo coeficiente al nuevo monomio
    sprintf(nuevo_monomio, "%d%s", nuevo_coeficiente, posicion_x);

    return nuevo_monomio;
}

int getCoefficient(char *cadena) {
    // Si la cadena está vacía o 'x' es el primer carácter, devolver 1
    if (*cadena == '\0' || *cadena == 'x') {
        return 1;
    }

    // Iterar sobre la cadena hasta encontrar la letra 'x' o el final de la cadena
    while (*cadena != '\0') {
        if (*cadena == 'x') {
            // Retroceder un carácter para obtener el carácter anterior
            char caracter_anterior = *(cadena - 1);
            // Si el carácter anterior es un número, devolver su valor numérico
            if (caracter_anterior >= '0' && caracter_anterior <= '9') {
                return caracter_anterior - '0';
            } else { // Si no es un número, devolver 1
                return 1;
            }
        }
        cadena++;
    }
    // Si no se encuentra la letra 'x', devolver 0
    return 0;
}

void reduceEquation(LList *termsList){
    LList *current = termsList->sig;
    LList *current2 = current->sig;
    LList *current2Prev = current;
    int currentPower = 0;

    while (current != NULL && current2 != NULL){
        if(contains(current->value, 'x') == TRUE){
            currentPower = getPower(current->value);

            while(current2 != NULL){
                
                if(contains(current2->value, 'x') == TRUE && (currentPower == getPower(current2->value))){
                    int coefficient = getCoefficient(current->value);
                    if(contains(current2Prev->value, '+') == TRUE){
                        coefficient += getCoefficient(current2->value);
                    } else{
                        coefficient -= getCoefficient(current2->value);
                    }

                    current->value = cambiar_coeficiente(current->value, coefficient);
                    LList_removeValue(termsList, current2Prev->value);
                    LList_removeValue(termsList, current2->value);

                    current2Prev = current;
                    current2 = current->sig;
                }

                current2Prev = current2;
                current2 = current2->sig;
            } 
        }
        
        current = current->sig;
        current2Prev = current;
        current2 = current->sig;
    }
}

int myPow(int base, int exponent){
    int i;
    int baseCopy = base;
    for(i = 1; i<exponent; i++){
        base *= baseCopy;
    }
    return base;
}

int calculate(LList *terms, int xValue){
    LList *current = terms->sig;
    int result = 0;
    int signBuffer = 0; // 0 for add and 1 for substract

    while (current != NULL){
        char *term = current->value;
        
        if (strchr(term, '+') == NULL && strchr(term, '-') == NULL && strchr(term, 'x') == NULL){
            // Verifica si el término no contiene "+" ni "-" ni "x": If it's a number
            if(signBuffer == 0){
                result += atoi(current->value);
            } else{
                result -= atoi(current->value);
            }

        } else if(strchr(term, '+') == NULL && strchr(term, '-') == NULL){
            // Verifica si el término no contiene "+" ni "-": If it's a monomial
            if(signBuffer == 0){
                result += (getCoefficient(current->value) * myPow(xValue, getPower(current->value)));
            } else {
                result -= (getCoefficient(current->value) * myPow(xValue, getPower(current->value)));
            }

        } else{
            // Is a sign
            if(strchr(term, '+') == NULL){
                signBuffer = 1;
            } else{
                signBuffer = 0;
            }
        }

        current = current->sig;    
    }

    return result;
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

BOOLEAN testCheckEquation(){
    // Prueba de ecuación válida
    const char* validEquation = "2x^2 + 3x - 1";
    BOOLEAN isValid = isValidPolynomial(validEquation);
    printf("Is the equation '%s' valid? %s\n", validEquation, isValid ? "Yes" : "No");

    // Prueba de ecuación no válida
    const char* invalidEquation = "2x^2 + 3y - 1";
    isValid = isValidPolynomial(invalidEquation);
    printf("Is the equation '%s' valid? %s\n", invalidEquation, isValid ? "Yes" : "No");

    
    // Prueba de ecuación no válida
    const char* validEquation1 = "2x^2 +3- 1";
    isValid = isValidPolynomial(validEquation1);
    printf("Is the equation '%s' valid? %s\n", validEquation1, isValid ? "Yes" : "No");

    
    // Prueba de ecuación no válida
    const char* invalidEquation2 = "2x^ + 3 - 1";
    isValid = isValidPolynomial(invalidEquation2);
    printf("Is the equation '%s' valid? %s\n", invalidEquation2, isValid ? "Yes" : "No");

    return FALSE;
}

void run(){
    
    /*************************
     *        PROGRAM        *
     ************************/

    int xValue;
    int result;
    char *polynomial;
    LList *terms = LList_create();

    // Ask for Equation
    polynomial = readPolynomial();

    // Verify Equation
    while(isValidPolynomial(polynomial) != OK){
        printf("ERROR, Invalid Polynomial, Try Again!\n");
        polynomial = readPolynomial();
    }

    // Ask for 'X' value
    xValue = enterXValue();

    // Split Equation
    terms = parseEquation(polynomial);

    // Transform Equation to math
    //reduceNumbers(terms);
    reduceEquation(terms);

    // Print Result
    printf("\n");
    printPolynomial(polynomial);
    printf("Reduced: ");
    printLListEquation(terms);
    result = calculate(terms, xValue);
    printf("For x = %d ; Result = %d\n\n", xValue, result);

    // Free memory allocated for linked list
    LList_destroy(terms);
    free(polynomial);
}

int main(int argc, char *argv[]){
    /* This are some tests, run them to verify that every part is running well */
    //testLList();
    //testCheckEquation();

    run();

    return 0;
}
