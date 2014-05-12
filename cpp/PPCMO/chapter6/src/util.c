/*
 * util.c
 *
 *  Created on: May 19, 2013
 *      Author: xiangqian
 */

#include "mpi.h"
#include "common.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * the last process reads data and distribute to others
 * since the last process(p-1) has the maximum row number, and use the largest storage
 */
void read_row_striped_matrix(char *datafile, void ***matrix, void **storage,
		MPI_Datatype mpi_type, int *row, int *col) {
	int num_of_proc;
	int my_rank;

	MPI_Comm_size(MPI_COMM_WORLD, &num_of_proc);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	// check if the data type is supported
	int item_size = 0;
	if (MPI_INT == mpi_type) {
		item_size = sizeof(int);
	} else {
		if (0 == my_rank) {
			printf("illegal data type\n");
			fflush(stdout);
		}

		MPI_Abort(MPI_COMM_WORLD, TYPE_ERROR);
	}

	// process p-1 reads data
	FILE *infile;		// infile only initialized for p-1
	if (num_of_proc - 1 == my_rank) {
		infile = fopen(datafile, "r");
		if (NULL == infile) {
			*row = 0;
		} else {
			fread(row, sizeof(int), 1, infile);
			fread(col, sizeof(int), 1, infile);
		}
	}

	// process p-1 broadcast the dimension to all processes
	MPI_Bcast(row, 1, MPI_INT, num_of_proc - 1, MPI_COMM_WORLD );
	if (0 == *row) {
		if (0 == my_rank) {
			printf("error open file\n");
			fflush(stdout);
		}

		MPI_Abort(MPI_COMM_WORLD, FILE_ERROR);
	}
	MPI_Bcast(col, 1, MPI_INT, num_of_proc - 1, MPI_COMM_WORLD );

	// ------ debug ------
#ifndef NDEBUG
	{
		printf("rank %d, row = %d, col = %d\n", my_rank, *row, *col);
		fflush(stdout);
	}
#endif

	// check if the matrix is square
	if (*row != *col) {
		if (0 == my_rank) {
			printf("matrix is not square\n");
			fflush(stdout);
		}

		MPI_Finalize();
		exit(-1);
	}

	// allocate memory
	int my_row_num = BLOCK_SIZE(my_rank, num_of_proc, *row);
	*storage = (void *) malloc(my_row_num * *col * item_size);
	if (NULL == *storage) {
		MPI_Abort(MPI_COMM_WORLD, MALLOC_ERROR);
	}
	*matrix = (void **) malloc(my_row_num * sizeof(void *));
	if (NULL == *matrix) {
		MPI_Abort(MPI_COMM_WORLD, MALLOC_ERROR);
	}

	// assign pointer
	// CAUTION: matrix and storage is not an array, so you can't use (*matrix)[i] or (*storage)[i]
	// CAUTION: use pointer arithmatic only
	int i;
	for (i = 0; i != my_row_num; ++i) {
		(*(*matrix + i)) = (void *) (*storage + i * *col * item_size);
	}

	// process p-1 read matrix value
	if (num_of_proc - 1 == my_rank) {
		// read and send value to other processes
		int i;
		for (i = 0; i != num_of_proc - 1; ++i) {
			int item_num = BLOCK_SIZE(i, num_of_proc, *row) * *col;
			fread(*storage, item_size, item_num, infile);
			MPI_Send(*storage, item_num, mpi_type, i, DATA_TAG,
					MPI_COMM_WORLD );
		}

		// values for p-1
		fread(*storage, item_size, my_row_num * *col, infile);

		fclose(infile);
	} else {
		MPI_Status status;
		MPI_Recv(*storage, my_row_num * *col, mpi_type, num_of_proc - 1,
				DATA_TAG, MPI_COMM_WORLD, &status);
	}

	return;
}

/**
 * rank 0 gather and print each submatrix in id order
 */
void print_row_striped_matrix(void **matrix, MPI_Datatype mpi_type, int row,
		int col) {
	int num_of_proc;
	int my_rank;

	MPI_Comm_size(MPI_COMM_WORLD, &num_of_proc);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	// check if the data type is supported
	if (MPI_INT != mpi_type) {
		if (0 == my_rank) {
			printf("illegal data type\n");
			fflush(stdout);
		}

		MPI_Abort(MPI_COMM_WORLD, TYPE_ERROR);
	}

	int my_row_num = BLOCK_SIZE(my_rank, num_of_proc, row);

	MPI_Status status;
	int request = 0;
	// process 0 is responsible to print
	if (0 == my_rank) {
		// print sub-matrix of process 0
		int r;
		int c;
		for (r = 0; r != my_row_num; ++r) {
			for (c = 0; c != col; ++c) {
				if (MPI_INT == mpi_type)
					printf("%6d ", ((int **) matrix)[r][c]);
			}

			putchar('\n');
		}
		fflush(stdout);

		// ***** check num_of_proc ****
		if (num_of_proc > 1) {
			int item_size;
			if (MPI_INT == mpi_type)
				item_size = sizeof(int);

			// the last process(p-1) has the maximum row number, so take its row number
			int max_row_num = BLOCK_SIZE(num_of_proc - 1, num_of_proc, row);
			void *recv_storage = (void *) malloc(max_row_num * col * item_size);
			void **recv_matrix = (void **) malloc(max_row_num * sizeof(void *));
			int i;
			for (i = 0; i != max_row_num; ++i)
				*(recv_matrix + i) = recv_storage + i * col * item_size;

			// get sub-matrix from other processes
			int p;
			for (p = 1; p != num_of_proc; ++p) {
				int recv_row_num = BLOCK_SIZE(p, num_of_proc, row);
				int recv_item_num = recv_row_num * col;
				MPI_Send(&request, 1, MPI_INT, p, REQUEST_TAG, MPI_COMM_WORLD );
				MPI_Recv(recv_storage, recv_item_num, mpi_type, p, RESPONSE_TAG,
						MPI_COMM_WORLD, &status);

				// print received sub-matrix
				int r;
				int c;
				for (r = 0; r != recv_row_num; ++r) {
					for (c = 0; c != col; ++c) {
						if (MPI_INT == mpi_type)
							printf("%6d ", ((int **) recv_matrix)[r][c]);
					}

					putchar('\n');
				}

				fflush(stdout);
			}

			free(recv_matrix);
			free(recv_storage);
		}
	} else {
		// each process sends its chunck only after recieve the request
		MPI_Recv(&request, 1, MPI_INT, 0, REQUEST_TAG, MPI_COMM_WORLD, &status);
		MPI_Send(*matrix, my_row_num * col, mpi_type, 0, RESPONSE_TAG,
				MPI_COMM_WORLD );
	}

	return;
}

