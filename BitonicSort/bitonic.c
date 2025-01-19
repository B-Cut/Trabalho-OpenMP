#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

int num_threads;

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

/*The parameter dir indicates the sorting direction, ASCENDING
   or DESCENDING; if (a[i] > a[j]) agrees with the direction,
   then a[i] and a[j] are interchanged.*/
void compAndSwap(int a[], int i, int j, int dir)
{
    if (dir==(a[i]>a[j])){
        swap(&a[i],&a[j]);
    }
}
 
/*It recursively sorts a bitonic sequence in ascending order,
  if dir = 1, and in descending order otherwise (means dir=0).
  The sequence to be sorted starts at index position low,
  the parameter cnt is the number of elements to be sorted.*/
void bitonicMerge(int a[], int low, int cnt, int dir)
{
    if (cnt>1)
    {
        int k = cnt/2;
        #pragma omp parallel for
        for (int i=low; i<low+k; i++){
            compAndSwap(a, i, i+k, dir);
        }
        bitonicMerge(a, low, k, dir);
        bitonicMerge(a, low+k, k, dir);
    }
}
/* This function first produces a bitonic sequence by recursively
    sorting its two halves in opposite sorting orders, and then
    calls bitonicMerge to make them in the same order */
void bitonicSort(int a[],int low, int cnt, int dir)
{
    if (cnt>1)
    {
        int k = cnt/2;
        // sort in ascending order since dir here is 1
        bitonicSort(a, low, k, 1);
        // sort in descending order since dir here is 0
        bitonicSort(a, low+k, k, 0);
        // Will merge whole sequence in ascending order
        // since dir=1.
        bitonicMerge(a,low, cnt, dir);
    }
}



int main(int argc, char** argv) {
    if (argc != 3){
        printf("Uso: bitonic_sort <numero de threads> <numero_de_elementos>");
        return -1;
    }

    num_threads = atoi(argv[1]);
    int num_elements = atoi(argv[2]);

    int* list = (int*) malloc(num_elements * sizeof(int));


    srand(1);

    // Vamos gerar a lista de elementos de forma paralela também
    // Como minha máquina tem 6 cores, vou usar 6 threads.
    omp_set_num_threads(6);

    #pragma omp parallel for
    for (int i = 0; i < num_elements; i++){
        list[i] = rand() % 100;
    }

    printf("> Lista não ordenada: ");
    print_list(list, num_elements);


    omp_set_num_threads(num_threads);

    clock_t start = clock();

    // Começo do bitonic
    //int N = num_elements/sizeof(list[0]);
 
    int up = 1;   // means sort in ascending order
    bitonicSort(list, 0, num_elements, up);
    // Fim do bitonic
    
    clock_t end = clock();

    double time_spent = (double) (end - start) / CLOCKS_PER_SEC; // Resultado em milissegundos, queremos segundos

    printf("%d threads demoraram %.6f segundos para ordenar uma lista de %d elementos\n", num_threads, time_spent, num_elements);
    printf("> Lista ordenada: ");
    print_list(list, num_elements);

    free(list);
    return 0;
}