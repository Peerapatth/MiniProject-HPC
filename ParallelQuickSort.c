// Step 1: Include the MPI and openMP header files
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <omp.h>
#include <time.h>

// Function prototypes
void quicksort(int arr[], int low, int high);
int partition(int arr[], int low, int high);
void parallelQuicksort(int arr[], int low, int high);

// Step 2: Add a parallel quicksort function using OpenM
// Sequential Quicksort implementation
void quicksort(int arr[], int low, int high)
{
    if (low < high)
    {
        // Partition the array and get the pivot position
        int pivot = partition(arr, low, high);

        // Recursively sort the subarrays on separate threads
#pragma omp task
        quicksort(arr, low, pivot - 1);

#pragma omp task
        quicksort(arr, pivot + 1, high);
    }
}

// Helper function to partition the array
int partition(int arr[], int low, int high)
{
    int pivot = arr[high];
    int i = low - 1;

    for (int j = low; j <= high - 1; j++)
    {
        if (arr[j] <= pivot)
        {
            i++;
            // Swap arr[i] and arr[j]
            int temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }

    // Swap arr[i + 1] and arr[high] to place pivot in the correct position
    int temp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = temp;

    return i + 1;
}

// Step 3: Add a parallelQuicksort function to wrap the parallel quicksort
// Parallel Quicksort wrapper
void parallelQuicksort(int arr[], int low, int high)
{
    // OpenMP parallel region with a single task to initiate parallel quicksort
#pragma omp parallel
    {
#pragma omp single
        quicksort(arr, low, high);
    }
}

// Step 4: Main Function - MPI Initialization and Command-line Argument Check
int main(int argc, char *argv[])
{
    // MPI Initialization
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Input and output file names
    const char *inputFileName = "input.txt";
    const char *outputFileName = "result.txt";

    int *data = NULL;
    int dataSize = 0;

    // Step 5: Read input data
    // Root process reads input data from the file
    if (rank == 0)
    {
        FILE *inputFile = fopen(inputFileName, "r");
        if (inputFile == NULL)
        {
            fprintf(stderr, "Error opening input file.\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        // Determine the size of the array
        fscanf(inputFile, "%d\n", &dataSize);

        data = (int *)malloc(dataSize * sizeof(int));

        for (int i = 0; i < dataSize; i++)
        {
            fscanf(inputFile, "%d ", &data[i]);
        }

        fclose(inputFile);
    }

    time_t start, end;
    double cpu_time_used;
    start = clock();

    // Step 6: MPI Broadcast - Broadcast Size and Data to All Processes
    // Broadcast the size of the data to all processes
    MPI_Bcast(&dataSize, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Broadcast the data to all processes
    if (rank != 0)
    {
        data = (int *)malloc(dataSize * sizeof(int));
    }

    MPI_Bcast(data, dataSize, MPI_INT, 0, MPI_COMM_WORLD);

    omp_set_num_threads(size);
    // Step 7: Perform parallel Quicksort using OpenMP
    // Perform parallel Quicksort using OpenMP
#pragma omp parallel
    {
#pragma omp single
        parallelQuicksort(data, 0, dataSize - 1);
    }

    // Gather sorted data to the root process
    int *sortedData = NULL;
    if (rank == 0)
    {
        sortedData = (int *)malloc(dataSize * size * sizeof(int));
    }

    // Step 8: MPI Gather - Gather Sorted Data to the Root Process
    MPI_Gather(data, dataSize, MPI_INT, sortedData, dataSize, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        end = clock();
        cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
        printf("Time taken: %f\n", cpu_time_used);
    }

    // Step 9: Write Sorted Data to the Output File on the Root Process
    // Write sorted data to the output file on the root process
    if (rank == 0)
    {
        FILE *outputFile = fopen(outputFileName, "w");
        if (outputFile == NULL)
        {
            fprintf(stderr, "Error opening output file.\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        fprintf(outputFile, "%d\n", dataSize);
        for (int i = 0; i < dataSize; i++)
        {
            fprintf(outputFile, "%d ", sortedData[i]);
        }

        fclose(outputFile);
    }

    // Step 10: Free Allocated Memory and MPI Finalization
    // Free allocated memory
    free(data);
    free(sortedData);

    // MPI Finalization
    MPI_Finalize();

    return 0;
}