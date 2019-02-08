#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv){
	if(argc != 2){
		printf("Foramt: %s [fileName]", argv[0]);
		exit(1);
	}
	FILE *fp = fopen(argv[1], "rb");
	double tmp; rewind(fp);
	for(int i=0;i<8;++i){
		for(int j=0;j<8;++j){
			fread(&tmp, sizeof(double), 1, fp);
			printf("%lf ", tmp);
		}
		printf("\n");
	}
	fread(&tmp, sizeof(double), 1, fp);
	printf("%lf\n", tmp); fclose(fp); exit(0);
}
