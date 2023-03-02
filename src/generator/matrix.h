#include <tuple>

int matrix_mult(float* return_matrix[], float* matrix_1[], float* matrix_2[], int Row_1, int Col_1, int Row_2, int Col_2);
std::tuple<float, float, float> matrix_mult_tuple(float* return_matrix[], float* matrix_1[], std::tuple<float, float, float> point, int Col_1);


