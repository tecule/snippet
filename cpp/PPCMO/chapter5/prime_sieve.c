#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mpi.h"

#define BLOCK_LOW(id, p, n) ((id) * (n) / (p))
#define BLOCK_HIGH(id, p, n) (BLOCK_LOW((id + 1), p, n) - 1)

int main(int argc, char **argv)
{
	int iProcessNumber, iMyRank;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &iProcessNumber);
	MPI_Comm_rank(MPI_COMM_WORLD, &iMyRank);

	// check if we can sieve by the method
	int iNumberRange = 0;
	//iNumberRange = atoi(argv[0]);
	iNumberRange = 100;
	// ensure the sieve only resides in the first block
	if ((int)(sqrt(iNumberRange)) > (iNumberRange - 1) / iProcessNumber + 2) 
	{
		if (0 == iMyRank)
			printf("Too Many Processes, Number Range: %d, Process Number: %d.\n", iNumberRange, iProcessNumber);
		MPI_Finalize();
		return(-1);
	}

	// ignore 0 and 1, start mark from number 2, so the total numbers till ranger N is N - 1

	// alocate this block in memory and initialize this block
	int iNumberFrom = BLOCK_LOW(iMyRank, iProcessNumber, iNumberRange - 1) + 2;
	int iNumberTo = BLOCK_HIGH(iMyRank, iProcessNumber, iNumberRange - 1) + 2;
	int iBlockSize = iNumberTo - iNumberFrom + 1;
	int *iBlock = (int *)malloc(sizeof(int) * iBlockSize);
	int i;
	for (i = 0; i < iBlockSize; i++)
		iBlock[i] = 0;

	// mark this block
	int iMarkStep = 2;
	do 
	{
		// use global and local index. global index for sieve, and local index for mark
		int iBlockIndex = 0;
		if (iMarkStep * iMarkStep < iNumberFrom)
		{
			if (iNumberFrom % iMarkStep != 0)
				iBlockIndex = iMarkStep - iNumberFrom % iMarkStep;
		}
		else
		{
			iBlockIndex = iMarkStep * iMarkStep - iNumberFrom;
		}

		for (; iBlockIndex <= iBlockSize; iBlockIndex += iMarkStep)
		{
			iBlock[iBlockIndex] = 1;
		}

		// process 0 is responsible for finding the next mark step
		if (0 == iMyRank) {
			int iNextStep = iMarkStep + 1;
			while ((iNextStep <= iNumberTo) && (1 == iBlock[iNextStep - 2]))
				iNextStep++;

			if (iNextStep > iNumberTo)
			{
				printf("Finished Sieving.\n");
				break;
			}

			iMarkStep = iNextStep;
		}

		// broadcast the next step
		MPI_Bcast(&iMarkStep, 1, MPI_INT, 0, MPI_COMM_WORLD);
	} while (iMarkStep * iMarkStep <= iNumberRange);

	// find prime count in the block
	int iPrimeInBlock = 0;
	for (i = 0; i < iBlockSize; i++) {
		if (0 == iBlock[i])
			iPrimeInBlock++;
	}
	printf("Prime Number Found in Process %d is %d.\n", iMyRank, iPrimeInBlock);
	printf("Prime Number Found in Process %d: \n", iMyRank);
	for (i = 0; i < iBlockSize; i++) {
		if (0 == iBlock[i])
			printf("%d\t", iNumberFrom + i);	
	}
	printf("\n");

	MPI_Finalize();
	return(0);
}
