#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

void merge(int *, int *, int, int, int);
void mergeSort(int *, int *, int, int);

int main(int argc, char** argv) {
    
    /*
     *  You can simply compile and run this script using:
     *  - mpicc main.c -o out && mpirun -np 4 out 24
     *
     *  The prerequsite is to have installed some
     *  MPI implementation (e.g. OpenMPI, MPICH). 
     * */

    int n = atoi(argv[1]);
    int *org_arry = malloc(n * sizeof(int));

    srand(time(NULL));
    for(int c=0;c<n;c++) 
        org_arry[c] = rand() % (2*n);

	int rank, size;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (!rank)
    {
        printf("Input array: \n------------ \n");
        for(int c=0;c<n;c++) 
            printf("%d ", org_arry[c]);
        printf("\n");
    }

	size = n / size;
	int *sub_arry = malloc(size * sizeof(int));
	
	MPI_Scatter(org_arry, size, MPI_INT,
			    sub_arry, size, MPI_INT,
                0 /*root*/, MPI_COMM_WORLD);
	
	int *tmp_arry = malloc(size * sizeof(int));
	mergeSort(sub_arry, tmp_arry, 0, (size-1));
    
	int *sorted = NULL;
	if (!rank) sorted = malloc(n * sizeof(int));
	
	MPI_Gather(sub_arry, size, MPI_INT,
			   sorted, size, MPI_INT,
  			   0 /*root*/, MPI_COMM_WORLD);
	
	if (!rank)
	{
		int *fin_arry = malloc(n * sizeof(int));
		mergeSort(sorted, fin_arry, 0, (n-1));
        
        printf("Output array: \n------------- \n");
    	for(int c=0;c<n;c++)
            printf("%d ", fin_arry[c]);
        printf("\n");
        
        free(sorted);
        free(fin_arry);
	}

	free(org_arry);
    free(sub_arry);
	free(tmp_arry);
	
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Finalize();
}

void mergeSort(int *arry_a, int *arry_b, int l, int r)
{
    int m;
    if (l < r /*index left < index right*/)
    {
        m = (l + r) / 2; // middle
        mergeSort(arry_a, arry_b, l, m);
        mergeSort(arry_a, arry_b, (m+1), r);
        merge(arry_a, arry_b, l, m, r);
    }
}

void merge(int *arry_a, int *arry_b, int lft, int mid, int rgt)
{
    int i      /*main iterator*/            = lft;
    int ptr_l  /*pointer starting left*/    = lft;   
    int ptr_m  /*pointer starting middle*/  = mid + 1;

    while ((ptr_l <= mid) && (ptr_m <= rgt))
    {
        if (arry_a[ptr_l] > arry_a[ptr_m]) {
            arry_b[i] = arry_a[ptr_l];
            ptr_l++;
        } 
        else 
        {
            arry_b[i] = arry_a[ptr_m];
            ptr_m++;
        }

        i++;
    }
    
    if (mid < ptr_l)
    {
        for (int k=ptr_m; k<=rgt; k++) {
            arry_b[i] = arry_a[k];
            i++;
        }
    } 
    else 
    {
        for (int k=ptr_l; k<=mid; k++) {
            arry_b[i] = arry_a[k];
            i++;        
        }
    }

    for (int k=lft; k<=rgt; k++) 
        arry_a[k] = arry_b[k];

}

