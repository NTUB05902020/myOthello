#include <stdio.h>
#include <stdlib.h>

int main(){
	FILE *fp = fopen("LI.eval", "wb");
	double zero = 0.0, point_five = 0.5;
	for(int i=0;i<64;++i) fwrite(&zero, sizeof(double), 1, fp);
	fwrite(&point_five, sizeof(double), 1, fp);
	fflush(fp); fclose(fp); exit(0);
}
