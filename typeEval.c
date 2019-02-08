#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv){
	if(argc != 2){ printf("format: %s [FileName]\n", argv[0]); exit(1);}
	double tmp; FILE *fp = fopen(argv[1], "wb");
	for(int i=0;i<38;++i){
		scanf("%lf", &tmp);
		fwrite(&tmp, sizeof(double), 1, fp);
	}
	fflush(fp); fclose(fp); exit(0);
}
