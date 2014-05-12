#include <stdio.h>

#define ITH_BIT(n, i) ((n & (1 << i)) ? 1 : 0)

int check_circuit(int iProcNum, int iValue){
	int v[16];
	int i;

	for (i = 0; i < 16; i++)
		v[i] = ITH_BIT(iValue, i);

	if ((v[0] || v[1]) && (!v[1] || !v[3]) && (v[2] || v[3]) && (!v[3] || !v[4])
		&& (v[4] || !v[5]) && (v[5] || !v[6]) && (v[5] || v[6]) && (v[6] || !v[15])
		&& (v[7] || !v[8]) && (!v[7] || !v[13]) && (v[8] || v[9]) && (v[8] || !v[9])
		&& (!v[9] || !v[10]) && (v[9] || v[11]) && (v[10] || v[11]) && (v[12] || v[13])
		&& (v[13] || !v[14]) && (v[14] || v[15])) {
			printf("%d, %d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d\n", iProcNum, v[0], v[1], 
				v[2], v[3], v[4], v[5], v[6], v[7], v[8], v[9], v[10], v[11], v[12], 
				v[13], v[14], v[15]);
			fflush(stdout);
			return 1;
		}

	return 0;
}
