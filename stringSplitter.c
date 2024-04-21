#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void procesar_string(const char* string) {
    char antes_del_signo[100]; // Tamaño arbitrario para la cadena antes del signo
    char signo;
    int i = 0;
    
    while (1) { // Bucle infinito
        // Reiniciamos antes_del_signo
        memset(antes_del_signo, 0, sizeof(antes_del_signo));
        
        // Recorremos el string hasta encontrar un + o -
        int j = 0;
        while (string[i] != '\0' && string[i] != '+' && string[i] != '-') {
            antes_del_signo[j++] = string[i++];
        }
        antes_del_signo[j] = '\0'; // Añadimos el carácter nulo al final

        // Si hemos encontrado un + o -, lo guardamos
        if (string[i] == '+' || string[i] == '-') {
            signo = string[i];
            i++; // Pasamos al siguiente carácter después del signo
        } else {
            // Si no hay más signos, imprimimos el texto restante y salimos del bucle
            printf("Texto antes del signo: %s\n", antes_del_signo);
            printf("No se encontró un signo en el string.\n");
            break;
        }

        // Imprimimos los resultados
        printf("Texto antes del signo: %s\n", antes_del_signo);
        printf("Signo encontrado: %c\n", signo);
    }
}

int main() {
    const char* ejemplo = "Texto+restante-otro+si-otros2";
    procesar_string(ejemplo);
    return 0;
}
