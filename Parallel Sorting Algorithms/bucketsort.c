#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define N 100000 // change the number of integers to generate here!!!
#define range 10000 

int main(int argc, char *argv[])
{
    int rank, size;
    
    int arr[N]; // unsorted array
    int sorted_arr[N]; // sorted array

    int counter; // numbers in each bucket
    int local_min; // minimun number in each bucket
    int local_max; // maximum number in each bucket

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int displs[size]; // displacement array
    int rcounts[size]; // number counter 

    /* time start */
    double start = MPI_Wtime();

    if(rank == 0)
    {
        /* generate random integer numbers (ranging from 0 to 9999) */
        for(int i=0; i<N; i++)
        {
            arr[i] = rand() % range;
        }
    }
    /* broadcast unsorted array to all processors */
    MPI_Bcast(arr, N, MPI_INT, 0, MPI_COMM_WORLD);

    counter = 0;
    local_min = rank * (range / size);
    local_max = (rank + 1) * (range / size);

    /* count numbers in each bucket */
    for(int i=0; i<N; i++)
    {
        if((arr[i] >= local_min) && (arr[i] < local_max))
        {
            counter++;
        }
    }
    
    int bucket[counter]; // initialize bucket

    /* assign values into bucket */
    counter = 0;
    for(int i=0; i<N; i++)
    {
        if((arr[i] >= local_min) && (arr[i] < local_max))
        {
            bucket[counter] = arr[i];
            counter++;
        }
    }

    /* sort numbers in bucket */
    int temp;
    for(int i=0; i<counter; i++)
    {
        for(int j=i+1; j<counter; j++)
        {
            if (bucket[i] > bucket[j]){
                temp = bucket[i];
                bucket[i] = bucket[j];
                bucket[j] = temp;
            }
        }
    }
    
    /* gather counter from each processors into rcounts array in process 0 */
    MPI_Gather(&counter, 1, MPI_INT, rcounts, 1, MPI_INT, 0, MPI_COMM_WORLD);

    /* specify the displacement relative to sorted array at which to place the incoming data from each process */
    if(rank == 0)
    {
        displs[0] = 0;

        for(int i=0; i<size-1; i++)
        {
            displs[i+1] = displs[i] + rcounts[i];
        }
    }

    /* merge all buckets into sorted array */
    MPI_Gatherv(bucket, counter, MPI_INT, sorted_arr, rcounts, displs, MPI_INT, 0, MPI_COMM_WORLD);

    /* time end */
    double end = MPI_Wtime();
    printf("Process %d took %f seconds to run.\n", rank, end - start);
    
    /* print the result */
    // if (rank == 0)
    // {
    //     printf("Before sort: \n");
    //     for(int i=0; i<N; i++)
    //     {
    //         printf("%d ", arr[i]);
    //     }

    //     printf("\nAfter sort: \n");
    //     for(int i=0; i<N; i++) 
    //     {
    //         printf("%d ", sorted_arr[i]);
    //     }
    // }
    
    MPI_Finalize();

    return 0;
}