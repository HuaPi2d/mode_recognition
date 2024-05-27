#include "proprint.h"

void cout_2D_array(double* array, int rows, int cols)
{
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			cout << array[i * cols + j] << ' ';
		}
		cout << endl;
	}
}