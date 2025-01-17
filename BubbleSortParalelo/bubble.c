#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char** argv) {
    if (argc != 2){
        printf("Uso: bubble <numero_de_elementos>");
        return -1;
    }

    int num_elements = atoi(argv[1]);

    int* list = (int*) malloc(num_elements * sizeof(int));


    srand(1);

    for (int i = 0; i < num_elements; i++){
        list[i] = rand() % 1000000;
    }


    clock_t start = clock();

    // Devemos alternar entre indices pares e impares para garantir que haverão trocas em cada iteração
    int num_trocas = 0;
    do{
        num_trocas = 0;
        for(int i = 0; i < num_elements - 1; i++){
            if(list[i] > list[i+1]){
                int temp = list[i];
                list[i] = list[i+1];
                list[i+1] = temp;
                num_trocas++;
            }
        }
    } while( num_trocas != 0 );
    
    clock_t end = clock();

    double time_spent = (double) (end - start) / CLOCKS_PER_SEC; // Resultado em milissegundos, queremos segundos

    printf("Algoritmo sequencial demorou %.6f segundos para ordenar uma lista de %d elementos\n", time_spent, num_elements);
    /*printf("> Lista ordenada: ");
    print_list(list, num_elements);*/

    free(list);
    return 0;
}