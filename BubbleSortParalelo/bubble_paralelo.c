#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

void print_list(int* list, int size){
    for (int i = 0; i < size; i++){
        printf("%d ", list[i]);
    }

    printf("\n");
}

int main(int argc, char** argv) {
    if (argc != 3){
        printf("Uso: bubble_paralelo <numero de threads> <numero_de_elementos>");
        return -1;
    }

    int num_threads = atoi(argv[1]);
    int num_elements = atoi(argv[2]);

    int* list = (int*) malloc(num_elements * sizeof(int));


    srand(1);

    // Vamos gerar a lista de elementos de forma paralela também
    // Como minha máquina tem 6 cores, vou usar 6 threads.
    omp_set_num_threads(6);

    #pragma omp parallel for
    for (int i = 0; i < num_elements; i++){
        list[i] = rand() % 1000000;
    }

    /*printf("> Lista não ordenada: ");
    print_list(list, num_elements);*/


    // Agora fazemos de fato o bubble sort

    // Esse algoritmo é uma implementação extramamente simples. Cada thread é responsável por um par de trocas

    omp_set_num_threads(num_threads);

    clock_t start = clock();

    // Devemos alternar entre indices pares e impares para garantir que haverão trocas em cada iteração
    int impar = 0;
    int num_trocas = 0;
    do{
        num_trocas = 0;

        #pragma omp parallel for reduction(+:num_trocas)
        for(int i = impar; i < num_elements - impar; i += 2){
            if(list[i] > list[i+1]){
                int temp = list[i];
                list[i] = list[i+1];
                list[i+1] = temp;
                num_trocas++;
            }
        }
        impar = !impar;
    } while( num_trocas != 0 );
    
    clock_t end = clock();

    double time_spent = (double) (end - start) / CLOCKS_PER_SEC; // Resultado em milissegundos, queremos segundos

    printf("%d threads demoraram %.6f segundos para ordenar uma lista de %d elementos\n", num_threads, time_spent, num_elements);
    /*printf("> Lista ordenada: ");
    print_list(list, num_elements);*/

    free(list);
    return 0;
}