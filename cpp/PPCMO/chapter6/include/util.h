/*
 * util.h
 *
 *  Created on: May 19, 2013
 *      Author: xiangqian
 */

#ifndef UTIL_H_
#define UTIL_H_

#include "mpi.h"

void read_row_striped_matrix(char *datafile, void ***matrix, void **storage,
		MPI_Datatype mpi_type, int *row, int *col);
void print_row_striped_matrix(void **matrix, MPI_Datatype mpi_type, int row,
		int col);


#endif /* UTIL_H_ */
