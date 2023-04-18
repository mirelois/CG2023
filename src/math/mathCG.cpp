#include "math.h"

void crossProduct(float *a, float *b, float *result){
    result[0] = a[1]*b[2] - a[2]*b[1];
    result[1] = a[2]*b[0] - a[0]*b[2];
    result[2] = a[0]*b[1] - a[1]*b[0];
}

void normalizeVector(float *v){
    float norm = sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);

    v[0] = v[0]/norm;
    v[1] = v[1]/norm;
    v[2] = v[2]/norm;
}

