#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ASC 1
#define DESC 0

void print_list(int* list, int size){
    for (int i = 0; i < size; i++){
        printf("%d ", list[i]);
    }

    printf("\n");
}


void swap(int *a, int *b){
    int temp = *a;
    *a = *b;
    *b = temp;
}


// dir indica se a troca é ascendente ou descendente
void compAndSwap(int a[], int i, int j, int dir)
{
    if (dir == a[i]>a[j]){
        swap(&a[i],&a[j]);
    }
}

// Ordena em ordem crescente se dir for ASC ou descendente de dir for DES
void sort(int list[], int start, int n, int dir){
    int passo = n /2;
    while(passo > 0){
        for(int i = start; i < start + n; i += 2*passo){
            for(int j = i, k = 0; k < passo; j++, k++){
                int temp = j + passo;
                compAndSwap(list, j, temp, dir);
            }
        }
        passo /= 2;
    }
}

int main(int argc, char **argv) {
if (argc != 2){
        printf("Uso: parallel_bitonic <numero_de_elementos>\n");
        return -1;
    }
    int num_elements = atoi(argv[1]);

    int* list = (int*) malloc(num_elements * sizeof(int));


    srand(1);

    for (int i = 0; i < num_elements; i++){
        list[i] = rand() % 1000000;
    }

    printf("> Lista não ordenada: ");
    print_list(list, num_elements);

    clock_t start = clock();

    // Sort bitonico
    for (int window = 2; window <= num_elements; window*=2) {
        for (int i = 0; i < num_elements; i += (2*window)) {
            int middle = i+window;
            
            sort(list, i, window, ASC);
            sort(list, middle, window, DESC); 
        }
    }

    clock_t end = clock();

    double time_spent = (double) (end - start) / CLOCKS_PER_SEC;

    printf("Algoritmo sequencial demorou %.6f segundos para ordenar uma lista de %d elementos\n",  time_spent, num_elements);
    printf("> Lista ordenada: ");
    print_list(list, num_elements);

    free(list);
    return 0;
}
