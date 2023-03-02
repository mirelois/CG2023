#include <cstdlib>
#include <iostream>
#include <tuple>
#include "matrix.h"
using namespace std;


int matrix_mult(float* return_matrix[], float* matrix_1[], float* matrix_2[], int Row_1, int Col_1, int Row_2, int Col_2)
{

    if(Col_1 != Row_2){
        return -1;
    }

    int i,j,k;

    for(i = 0; i < Row_1; ++i)
        for(j = 0; j < Col_2; ++j)
        {
            return_matrix[i][j]=0;
        }

    // Multiplying matrix a and b and storing in array return_matrix.
    for(i = 0; i < Row_1; ++i)
        for(j = 0; j < Col_2; ++j)
            for(k = 0; k < Col_1; ++k)
            {
                return_matrix[i][j] += matrix_1[i][k] * matrix_2[k][j];
            }


    return 0;
}


tuple<float, float, float> matrix_mult_tuple( float* matrix_1[], tuple<float, float, float> point)
{

    return make_tuple( matrix_1[0][0]+get<0>(point) + matrix_1[0][1]+get<1>(point) + matrix_1[0][2]+get<2>(point),
                matrix_1[1][0]+get<0>(point) + matrix_1[1][1]+get<1>(point) + matrix_1[1][2]+get<2>(point),
                matrix_1[2][0]+get<0>(point) + matrix_1[2][1]+get<1>(point) + matrix_1[2][2]+get<2>(point));
}



int main()
{
    float a1[] = {1,2};
    float a2[] = {3,4};
    float* a[] ={a1,a2};

    float b1[] = {5,6};
    float b2[] = {7,8};
    float* b[] ={b1,b2};

    float** return_matrix;

    return_matrix = (float**)malloc(2);
    for (int i=0; i<2; i++){
        return_matrix[i] = (float*)malloc(2);
    }

    matrix_mult(return_matrix, a, b, 2, 2, 2, 2);

    // Displaying the return_matrixiplication of two matrix.
    cout << endl << "Output Matrix: " << endl;
    for(i = 0; i < Row_1; ++i)
    for(j = 0; j < Col_2; ++j)
    {
        cout << " " << return_matrix[i][j];
        if(j == Col_2-1)
            cout << endl;
    }
}

