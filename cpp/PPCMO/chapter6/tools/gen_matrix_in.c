#include <stdio.h>
#include <assert.h>

#define ROW_NUM 6
#define COL_NUM 6

int main(int argc, char **argv)
{
	int matrix_values[] = {ROW_NUM, COL_NUM, 0, 2, 5, 65535, 65535, 65535, 65535, 0, 7, 1, 65535, 8, 65535, 65535, 0, 4, 65535, 65535, 65535, 65535, 65535, 0, 3, 65535, 65535, 65535, 2, 65535, 0, 3, 65535, 5, 65535, 2, 4, 0};

	FILE *out_file = fopen("matrix_data.in", "w+");
	assert (NULL != out_file);

	int i;
	int matrix_size = ROW_NUM * COL_NUM; 
	int loop_end = matrix_size + 2;
	for (i = 0; i != loop_end; ++i)
		fwrite(matrix_values + i, sizeof(int), 1, out_file);
	printf("matrix file generated\n");
	
	printf("\ncheck generated file\n");

	// check the output
	rewind(out_file);
	int r, c, v;
	fread(&r, sizeof(int), 1, out_file);
	fread(&c, sizeof(int), 1, out_file);
	printf("matrix dimension: %d x %d \n", r, c);
	for (i = 0; i != matrix_size; ++i)
	{
		fread(&v, sizeof(int), 1, out_file);
		printf("%6d ", v);

		if ((i + 1) % 6 == 0)
			putchar('\n');
	}

	return 0;
}
