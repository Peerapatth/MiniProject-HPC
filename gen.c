// gen number to txt file
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int n = argc > 1 ? atoi(argv[1]) : 1000;
    FILE *fptr;
    fptr = fopen("input.txt", "w");
    if (fptr == NULL)
    {
        printf("Error!");
        exit(1);
    }
    fprintf(fptr, "%d\n", n);
    // 1000 -> 9999
    for (int i = 0; i < n; i++)
    {
        fprintf(fptr, "%d ", rand() % 9000 + 1000);
    }
    fclose(fptr);
    return 0;
}
