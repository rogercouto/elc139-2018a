#include <omp.h>
#include <complex>
#include <iostream>

using namespace std;

int main(int argc, char** argv){
	int max_row, max_column, max_n;
	if (argc < 4){
		cout << "usage ./mandelbrot <threads> <rows> <columns> <n>\n";
		return -1;
	}
	omp_set_num_threads(atoi(argv[1]));
	max_row = atoi(argv[2]);
	max_column = atoi(argv[3]);
	max_n = atoi(argv[4]);

	char **mat = (char**)malloc(sizeof(char*)*max_row);
	
	for (int i=0; i<max_row;i++)
		mat[i]=(char*)malloc(sizeof(char)*max_column);

	for(int r = 0; r < max_row; ++r){
		#pragma omp parallel for schedule(dynamic)
		for(int c = 0; c < max_column; ++c){
			complex<float> z;
			int n = 0;
			while(abs(z) < 2 && ++n < max_n)
				z = pow(z, 2) + decltype(z)(
					(float)c * 2 / max_column - 1.5,
					(float)r * 2 / max_row - 1
				);
			mat[r][c]=(n == max_n ? '#' : '.');
		}
	}

	for(int r = 0; r < max_row; ++r){
		for(int c = 0; c < max_column; ++c)
			std::cout << mat[r][c];
		cout << '\n';
	}	
}


