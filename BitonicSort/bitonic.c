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
    if (
        (dir == ASC && (a[i]>a[j])) 
        || (dir == DESC && (a[i]<a[j]))
    ){
        swap(&a[i],&a[j]);
    }
}
 

// Ordena em ordem crescente se dir for ASC ou descendente de dir for DES
void bitonicMerge(int a[], int start, int n, int dir)
{
    if (n>1)
    {
        int k = n/2;
        for (int i=start; i<start+k; i++){
            compAndSwap(a, i, i+k, dir);
        }
        bitonicMerge(a, start, k, dir);
        bitonicMerge(a, start+k, k, dir);
    }
}
// Cria uma sequencia bitonica recursivamente
void bitonicSort(int a[],int start, int n, int dir)
{
    if (n>1)
    {
        int k = n/2;
        
        bitonicSort(a, start, k, ASC);

        bitonicSort(a, start+k, k, DESC);

        bitonicMerge(a,start, n, dir);
    }
}

int main(int argc, char** argv) {
    if (argc != 2){
        printf("Uso: bitonic_sort <numero_de_elementos>\n");
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

    // Começo do bitonic
    //int N = num_elements/sizeof(list[0]);
   // means sort in ascending order
    bitonicSort(list, 0, num_elements, ASC);
    // Fim do bitonic
    
    clock_t end = clock();

    double time_spent = (double) (end - start) / CLOCKS_PER_SEC; // Resultado em milissegundos, queremos segundos

    printf("Algoritmo sequencial demorou %.6f segundos para ordenar uma lista de %d elementos\n",  time_spent, num_elements);
    printf("> Lista ordenada: ");
    print_list(list, num_elements);

    free(list);
    return 0;
}