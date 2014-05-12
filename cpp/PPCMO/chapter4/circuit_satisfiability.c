#include <stdio.h>
#include "mpi.h"

int main(int argc, char **argv) {
	int iMyRank = 0, iProcessNumber = 0;
	double dWallClockTime = 0.0f;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &iProcessNumber);
	MPI_Comm_rank(MPI_COMM_WORLD, &iMyRank);
	MPI_Barrier(MPI_COMM_WORLD);
	dWallClockTime -= MPI_Wtime();

	int i;
	int iMySolutionNumber = 0, iTotalSolutionNumber = 0;
	for (i = iMyRank; i < 65536; i += iProcessNumber)
		iMySolutionNumber += check_circuit(iMyRank, i);

	MPI_Reduce(&iMySolutionNumber, &iTotalSolutionNumber, 1, MPI_INT, MPI_SUM, 
		0, MPI_COMM_WORLD);
	dWallClockTime += MPI_Wtime();

	if (0 == iMyRank) {
		printf("%d solutions found in %f seconds.\n", iTotalSolutionNumber, dWallClockTime);
		fflush(stdout);
	}

	MPI_Finalize();
	return 0;
}
