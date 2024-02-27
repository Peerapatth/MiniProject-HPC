//Parallel QuciSort using OpenMP and MPI by read file input and write file output

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include <omp.h>

#define MAX 1000000
#define MAX_THREADS 8

int compare(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

void merge(int *arr, int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    int *L = (int *)malloc(n1 * sizeof(int));
    int *R = (int *)malloc(n2 * sizeof(int));

    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    i = 0;
    j = 0;
    k = l;

    while (i < n1 && j < n2) {
        if (L[i] <= R[j])
            arr[k++] = L[i++];
        else
            arr[k++] = R[j++];
    }

    while (i < n1)
        arr[k++] = L[i++];
    while (j < n2)
        arr[k++] = R[j++];

    free(L);
    free(R);
}

void parallelQuickSort(int *arr, int l, int r, int threads) {
    if (l < r) {
        if (r - l < 1000) {
            qsort(arr + l, r - l + 1, sizeof(int), compare);
            return;
        }

        int m = l + (r - l) / 2;

        if (threads <= 1) {
            parallelQuickSort(arr, l, m, threads);
            parallelQuickSort(arr, m + 1, r, threads);
        } else {
            #pragma omp parallel sections
            {
                #pragma omp section
                parallelQuickSort(arr, l, m, threads / 2);
                #pragma omp section
                parallelQuickSort(arr, m + 1, r, threads / 2);
            }
        }

        merge(arr, l, m, r);
    }
}

int main(int argc, char *argv[]) {
    int rank, size;
    int *arr = (int *)malloc(MAX * sizeof(int));
    int *tmp = (int *)malloc(MAX * sizeof(int));
    int *recv = (int *)malloc(MAX * sizeof(int));
    int *displs = (int *)malloc(MAX * sizeof(int));
    int *scounts = (int *)malloc(MAX * sizeof(int));
    int n, i, j, threads;
    double start, end;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        FILE *file = fopen("input.txt", "r");
        fscanf(file, "%d", &n);
        for (i = 0; i < n; i++)
            fscanf(file, "%d", &arr[i]);
        fclose(file);
    }

    start = MPI_Wtime();

    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(arr, n / size, MPI_INT, recv, n / size, MPI_INT, 0, MPI_COMM_WORLD);

    threads = MAX_THREADS / size;
    parallelQuickSort(recv, 0, n / size - 1, threads);

    MPI_Gather(recv, n / size, MPI_INT, arr, n / size, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        for (i = 1; i < size; i++) {
            displs[i] = i * (n / size);
            scounts[i] = n / size;
        }
        scounts[0] = n / size;
        displs[0] = 0;

        for (i = 1; i < size; i++) {
            for (j = 0; j < n / size; j++) {
                tmp[i * (n / size) + j] = arr[i * (n / size) + j];
            }
        }

        for (i = 1; i < size; i++) {
            merge(tmp, 0, i * (n / size) - 1, (i + 1) * (n / size) - 1);
        }

        for (i = 1; i < size; i++) {
            for (j = 0; j < n / size; j++) {
                arr[i * (n / size) + j] = tmp[i * (n / size) + j];
            }
        }

        merge(arr, 0, n / size - 1, n - 1);

        end = MPI_Wtime();

        printf("Time: %f\n", end - start);

        FILE *file = fopen("output.txt", "w");

        for (i = 0; i < n; i++)
            fprintf(file, "%d ", arr[i]);

        fclose(file);
    }

    MPI_Finalize();
    return 0;
}