#include "mpi.h"
#include "common.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>

void compute_shortest_path(data_type ***matrix, int row, int col) {
	int r, c, k;
	int num_of_proc, my_rank;

	MPI_Comm_size(MPI_COMM_WORLD, &num_of_proc);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	// temp storage for the kth row
	data_type *k_row_storage = (data_type *) malloc(col * sizeof(data_type));
	int my_row_num = BLOCK_SIZE(my_rank, num_of_proc, row);

	for (k = 0; k < row; ++k) {
		// copy kth row to temp storage
		int k_proc = BLOCK_OWNER(k, num_of_proc, row);// the process contains row k
		if (my_rank == k_proc) {
#ifndef NDEBUG
			printf("the kth row in iteration %d:\t", k);
			fflush(stdout);
#endif
			// index of row k in the sum-matrix
			int local_index = k - BLOCK_LOW(my_rank, num_of_proc, row);
			for (c = 0; c != col; ++c) {
				*(k_row_storage + c) = *(*(*matrix + local_index) + c);

#ifndef NDEBUG
				printf("%6d ", *(k_row_storage + c));
				fflush(stdout);
#endif
			}
#ifndef NDEBUG
			putchar('\n');
			fflush(stdout);
#endif
		}

		// broadcast kth row from k_proc
		MPI_Bcast(k_row_storage, col, MPI_TYPE, k_proc, MPI_COMM_WORLD );

		// calculate the shorter path
		for (r = 0; r != my_row_num; ++r)
			for (c = 0; c != col; ++c)
				(*matrix)[r][c] =
						MIN((*matrix)[r][c], (*matrix)[r][k] + k_row_storage[c]);

#ifndef NDEBUG
		if (0 == my_rank) {
			printf("iteration %d:\n", k);
			fflush(stdout);
		}
		print_row_striped_matrix((void **) *matrix, MPI_TYPE, row, col);
#endif
	}

	free(k_row_storage);
}

int main(int argc, char **argv) {
	data_type **sub_matrix;
	data_type *sub_matrix_storage;
	int num_of_proc;
	int my_rank;
	int matrix_row;
	int matrix_col;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &num_of_proc);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	//assert(2 == argc);

	// read matrix value
	// why cast to void* for 2nd argument, should it be void** ???
	read_row_striped_matrix(argv[1], (void *) &sub_matrix,
			(void *) &sub_matrix_storage, MPI_TYPE, &matrix_row, &matrix_col);

	if (0 == my_rank) {
		printf("initial path vector:\n");
		fflush(stdout);
	}
	print_row_striped_matrix((void **) sub_matrix, MPI_TYPE, matrix_row,
			matrix_col);

	compute_shortest_path((data_type ***) &sub_matrix, matrix_row, matrix_col);

	if (0 == my_rank) {
		printf("shortest path vector:\n");
		fflush(stdout);
	}
	print_row_striped_matrix((void **) sub_matrix, MPI_TYPE, matrix_row,
			matrix_col);

	// CATTION is sub_matrix and sub_matrix_storage freed?

	MPI_Finalize();

	return 0;
}
