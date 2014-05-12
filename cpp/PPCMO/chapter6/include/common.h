#ifndef __COMMON_H
#define __COMMON_H

typedef int data_type;
#define MPI_TYPE MPI_INT

#define BLOCK_LOW(id, p, n) ((id) * (n) / (p))
#define BLOCK_HIGH(id, p, n) (BLOCK_LOW((id) + 1, p, n) - 1)
#define BLOCK_SIZE(id, p, n) (BLOCK_HIGH(id, p, n) - BLOCK_LOW(id, p, n) + 1)
#define BLOCK_OWNER(i, p, n) (((p) * ((i) + 1) - 1) / (n))

#define TYPE_ERROR -1
#define FILE_ERROR -2
#define MALLOC_ERROR -3

#define DATA_TAG 0
#define REQUEST_TAG 1
#define RESPONSE_TAG 2

#define MIN(a, b) (a) < (b) ? (a) : (b)

#endif
