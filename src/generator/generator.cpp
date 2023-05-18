#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <ostream>
#include <stdio.h>
#include <tuple>
#define _USE_MATH_DEFINES
#include "bezier.h"
#include <fstream>
#include <math.h>
#include <string.h>

using namespace std;

tuple<float *, unsigned int *>
generate_torus_index(float inner_radius, float outer_radius,
                     int vertical_divisions, int horizontal_divisions,
                     unsigned int *points_total, unsigned int *index_total) {

    *points_total = horizontal_divisions * vertical_divisions * 3;
    *index_total = 6 * vertical_divisions * horizontal_divisions;

    float *point_array = (float *)malloc(sizeof(float) * *points_total);
    unsigned int *index_array =
        (unsigned int *)malloc(sizeof(unsigned int) * *index_total);
    float *master_circle =
        (float *)malloc(sizeof(float) * (vertical_divisions * 3));
    int index = 0;

    int master_circle_index = 0;

    float delta_x = 2 * M_PI / vertical_divisions;

    float delta_y = 2 * M_PI / horizontal_divisions;

    float pivot_x = 0;

    float pivot_y = (outer_radius - inner_radius) / 2;

    // float pivot_z = 0;

    for (int i = 0; i < vertical_divisions; i++) {
        master_circle[master_circle_index++] = pivot_x;
        master_circle[master_circle_index++] = pivot_y * cos(i * delta_x);
        master_circle[master_circle_index++] =
            pivot_y * sin(i * delta_x) + (outer_radius + inner_radius) / 2;
    }

    for (int j = 0; j < vertical_divisions; j++) {
        for (int i = 0; i < horizontal_divisions; i++) {
            point_array[index++] =
                (master_circle[3 * j + 0]) * cos(i * delta_y) +
                (master_circle[3 * j + 2]) * sin(i * delta_y);
            point_array[index++] = (master_circle[3 * j + 1]);
            point_array[index++] =
                (master_circle[3 * j + 0]) * sin(i * delta_y) +
                (master_circle[3 * j + 2]) * cos(i * delta_y);
        }
    }

    index = 0;

    for (int j = 0; j < vertical_divisions; j++) {
        for (int i = 0; i < horizontal_divisions; i++) {
            index_array[index++] = horizontal_divisions * j + i;
            index_array[index++] =
                horizontal_divisions * ((j + 1) % vertical_divisions) + i;
            index_array[index++] =
                horizontal_divisions * ((j + 1) % vertical_divisions) +
                ((i + 1) % horizontal_divisions);

            index_array[index++] = horizontal_divisions * j + i;
            index_array[index++] =
                horizontal_divisions * ((j + 1) % vertical_divisions) +
                ((i + 1) % horizontal_divisions);
            index_array[index++] =
                horizontal_divisions * j + ((i + 1) % horizontal_divisions);
        }
    }

    return make_tuple(point_array, index_array);
}

tuple<float *, unsigned int *>
generate_cone_index(float bottom_radius, float height, int slices, int stacks,
                    unsigned int *point_total, unsigned int *index_total) {

    *point_total = 3 * ((slices + 1) * stacks + 2);
    *index_total = slices * stacks * 6;

    float *point_array = (float *)malloc(sizeof(float) * *point_total);
    unsigned int *index_array =
        (unsigned int *)malloc(sizeof(unsigned int) * *index_total);

    int i, j;
    double alfa = 2 * M_PI / slices;
    double division_height_step = height / stacks;
    double division_radius_step = bottom_radius / stacks;
    int index = 0;

    // top point
    point_array[index++] = 0;
    point_array[index++] = height;
    point_array[index++] = 0;

    for (i = 0; i < stacks; i++) {
        double sub_height = height - division_height_step * (i + 1);
        double sub_radius = (i + 1) * division_radius_step;
        for (j = 0; j < slices; j++) {

            // lados
            point_array[index++] = sub_radius * sin(alfa * j);
            point_array[index++] = sub_height;
            point_array[index++] = sub_radius * cos(alfa * j);
        }
    }

    for (j = 0; j < slices; j++) {

        // lados
        point_array[index++] = bottom_radius * sin(alfa * j);
        point_array[index++] = 0;
        point_array[index++] = bottom_radius * cos(alfa * j);
    }

    // bottom point
    point_array[index++] = 0;
    point_array[index++] = 0;
    point_array[index++] = 0;

    index = 0;

    for (int i = 0; i < slices; i++) {
        index_array[index++] = 0;
        index_array[index++] = i + 1;
        index_array[index++] = (i + 1) % (slices) + 1;
    }

    for (int j = 0; j < stacks - 1; j++) {
        for (int i = 0; i < slices; i++) {
            index_array[index++] = slices * j + i + 1;
            index_array[index++] = slices * (j + 1) + i + 1;
            index_array[index++] = slices * (j + 1) + ((i + 1) % slices) + 1;

            index_array[index++] = slices * j + i + 1;
            index_array[index++] = slices * (j + 1) + ((i + 1) % slices) + 1;
            index_array[index++] = slices * j + ((i + 1) % slices) + 1;
        }
    }

    for (int i = 0; i < slices; i++) {
        int offset = (slices + 1) * (stacks - 1) + 2;
        index_array[index++] = offset + i;
        index_array[index++] = slices * (stacks + 1) + 1;
        index_array[index++] = offset + (i + 1) % slices;
    }

    return make_tuple(point_array, index_array);
}

tuple<float *, unsigned int *>
generate_cylinder_index(float radius, float height, int slices, int stacks,
                        unsigned int *point_total, unsigned int *index_total) {

    *point_total = 3 * ((slices + 2) * (stacks + 1) + 2);
    *index_total = slices * ((stacks + 1) * 6);

    float *point_array = (float *)malloc(sizeof(float) * *point_total);
    unsigned int *index_array =
        (unsigned int *)malloc(sizeof(unsigned int) * *index_total);

    float division_height_step = height / stacks;
    float alfa = 2 * M_PI / slices;

    int index = 0;

    // top point
    point_array[index++] = 0.0f;
    point_array[index++] = height / 2;
    point_array[index++] = 0.0f;
    
    for (int i = 0; i < slices; i++) {
        // top ring
        point_array[index++] = radius * sin(alfa * i);
        point_array[index++] = height/2;
        point_array[index++] = radius * cos(alfa * i);
    }

    for (int j = 0; j < stacks + 1; j++) {
        double sub_height = height / 2 - j * division_height_step;
        for (int i = 0; i < slices; i++) {
            // middle part

            point_array[index++] = radius * sin(alfa * i);
            point_array[index++] = sub_height;
            point_array[index++] = radius * cos(alfa * i);
        }
    }
    
    for (int i = 0; i < slices; i++) {
        // bottom ring
        point_array[index++] = radius * sin(alfa * i);
        point_array[index++] = -height/2;
        point_array[index++] = radius * cos(alfa * i);
    }

    // bottom point
    point_array[index++] = 0.0f;
    point_array[index++] = -height / 2;
    point_array[index++] = 0.0f;

    index = 0;

    for (int i = 0; i < slices; i++) {
        index_array[index++] = 0;
        index_array[index++] = i + 1;
        index_array[index++] = (i + 1) % (slices) + 1;
    }

    int offset = slices + 1;

    for (int j = 0; j < stacks; j++) {
        for (int i = 0; i < slices; i++) {
            index_array[index++] = offset + slices * j + i;
            index_array[index++] = offset + slices * (j + 1) + i;
            index_array[index++] = offset + slices * (j + 1) + ((i + 1) % slices);

            index_array[index++] = offset + slices * j + i;
            index_array[index++] = offset + slices * (j + 1) + ((i + 1) % slices);
            index_array[index++] = offset + slices * j + ((i + 1) % slices);
        }
    }

    offset = slices * (stacks + 2) + 1;
    for (int i = 0; i < slices; i++) {
        index_array[index++] = offset + i;
        index_array[index++] = offset + slices;
        index_array[index++] = offset + (i + 1) % slices;
    }

    for(int i=0; i<*index_total; i++){
        printf("%u,", index_array[i]);
        if((i+1)%3 == 0) putchar('\n');
    }

    return make_tuple(point_array, index_array);
}

tuple<float *, unsigned int *> generate_box_index(float length, int grid_slices,
                                                  unsigned int *points_total,
                                                  unsigned int *index_total) {
    // a quantidade de pontos ´e definida pelo grid;
    // agora tem de ser pontos com normais diferentes por isso ´e por face
    *points_total = (grid_slices+1)*(grid_slices+1) * 3 * 6;
    *index_total = grid_slices * grid_slices * 36;

    float *point_array = (float *)malloc(sizeof(float) * *points_total);
    unsigned int *index_array =
        (unsigned int *)malloc(sizeof(unsigned int) * *index_total);

    float delta = length / grid_slices;

    int index = 0;

    float referential_x = -length / 2;

    float referential_y = length / 2;

    float referential_z = length / 2;

    //fromt face is in plane yx all others are relative to this one
    for (int i = 0; i < grid_slices + 1; i++) {
        for (int j = 0; j < grid_slices + 1; j++) {

            // variables are of the front
            point_array[index++] = j * delta + referential_x;
            point_array[index++] = -i * delta + referential_y;
            point_array[index++] = referential_z;
        }
    }
    for (int i = 0; i < grid_slices + 1; i++) {
        for (int j = 0; j < grid_slices + 1; j++) {

            // slide to the right
            point_array[index++] = referential_z;
            point_array[index++] = -i * delta + referential_y;
            point_array[index++] = -(j * delta + referential_x);
        }
    }
    for (int i = 0; i < grid_slices + 1; i++) {
        for (int j = 0; j < grid_slices + 1; j++) {

            // go back
            point_array[index++] = -(j * delta + referential_x);
            point_array[index++] = -i * delta + referential_y;
            point_array[index++] = -referential_z;
        }
    }
    for (int i = 0; i < grid_slices + 1; i++) {
        for (int j = 0; j < grid_slices + 1; j++) {

            // slide to the left
            point_array[index++] = -referential_z;
            point_array[index++] = -i * delta + referential_y;
            point_array[index++] = j * delta + referential_x;
        }
    }

    for (int i = 0; i < grid_slices + 1; i++) {
        for (int j = 0; j < grid_slices + 1; j++) {

            // step up now
            point_array[index++] = j * delta + referential_x;
            point_array[index++] = -referential_z;
            point_array[index++] = -i * delta + referential_y;
        }
    }
    
    for (int i = 0; i < grid_slices + 1; i++) {
        for (int j = 0; j < grid_slices + 1; j++) {
            // step down now
            point_array[index++] = j * delta + referential_x;
            point_array[index++] = referential_z;
            point_array[index++] = i * delta - referential_y;
        }
    }

    index = 0;

    for (int f = 0; f < 6; f++){
        for (int i = 0; i < grid_slices; i++) {
            for (int j = 0; j < grid_slices; j++) {

                index_array[index++] = (grid_slices + 1) * i + j             + f*((grid_slices + 1) * (grid_slices + 1));
                index_array[index++] = (grid_slices + 1) * (i + 1) + j       + f*((grid_slices + 1) * (grid_slices + 1));
                index_array[index++] = (grid_slices + 1) * (i + 1) + (j + 1) + f*((grid_slices + 1) * (grid_slices + 1));

                index_array[index++] = (grid_slices + 1) * i + j             + f*((grid_slices + 1) * (grid_slices + 1));
                index_array[index++] = (grid_slices + 1) * (i + 1) + (j + 1) + f*((grid_slices + 1) * (grid_slices + 1));
                index_array[index++] = (grid_slices + 1) * i + (j + 1)       + f*((grid_slices + 1) * (grid_slices + 1));
            }
        }
    }

    return make_tuple(point_array, index_array);
}

tuple<float *, unsigned int *> generate_plane_index(float length,
                                                    int grid_slices,
                                                    unsigned int *points_total,
                                                    unsigned int *index_total) {

    *index_total = grid_slices * grid_slices * 6;
    *points_total = (grid_slices + 1) * (grid_slices + 1) * 3;

    float *point_array = (float *)malloc(sizeof(float) * *points_total);
    unsigned int *index_array =
        (unsigned int *)malloc(sizeof(unsigned int) * *index_total);

    float delta = length / grid_slices;

    int index = 0;

    float referential_x = -length / 2;

    float referential_z = -length / 2;

    for (int i = 0; i < grid_slices + 1; i++) {
        for (int j = 0; j < grid_slices + 1; j++) {
            point_array[index++] = j * delta + referential_x;
            point_array[index++] = 0.0f;
            point_array[index++] = i * delta + referential_z;
        }
    }

    index = 0;

    for (int i = 0; i < grid_slices; i++) {
        for (int j = 0; j < grid_slices; j++) {

            index_array[index++] = (grid_slices + 1) * i + j;
            index_array[index++] = (grid_slices + 1) * (i + 1) + j;
            index_array[index++] = (grid_slices + 1) * (i + 1) + (j + 1);

            index_array[index++] = (grid_slices + 1) * i + j;
            index_array[index++] = (grid_slices + 1) * (i + 1) + (j + 1);
            index_array[index++] = (grid_slices + 1) * i + (j + 1);
        }
    }
    return make_tuple(point_array, index_array);
}

tuple<float *, unsigned int *>
generate_sphere_index(float radius, int slices, int stacks,
                      unsigned int *points_total, unsigned int *index_total) {
    *index_total = slices * 6 * (stacks - 1);
    *points_total = 3 * (slices * (stacks - 1) + 2);
    int master_line_size = (stacks + 1) * 3;
    float alfa_x = M_PI / stacks;
    float alfa_y = 2 * M_PI / slices;

    float pivot_x = 0;
    float pivot_y = radius;
    // float pivot_z = 0;

    float *master_line = (float *)malloc(sizeof(float) * master_line_size);
    float *points_array = (float *)malloc(sizeof(float) * *points_total);
    unsigned int *index_array =
        (unsigned int *)malloc(sizeof(unsigned int) * *index_total);

    int master_line_index = 0;
    for (int i = 0; i < stacks + 1; i++) {
        master_line[master_line_index++] = pivot_x;
        master_line[master_line_index++] = pivot_y * cos(i * alfa_x);
        master_line[master_line_index++] = pivot_y * sin(i * alfa_x);
    }

    int index = 0;

    points_array[index++] = master_line[0];
    points_array[index++] = master_line[1];
    points_array[index++] = master_line[2];

    for (int j = 0; j < slices; j++) {
        for (int i = 1; i <= stacks - 1; i++) {
            points_array[index++] = (master_line[i * 3 + 0]) * cos(j * alfa_y) +
                                    (master_line[i * 3 + 2]) * sin(j * alfa_y);
            points_array[index++] = (master_line[i * 3 + 1]);
            points_array[index++] = (master_line[i * 3 + 0]) * sin(j * alfa_y) +
                                    (master_line[i * 3 + 2]) * cos(j * alfa_y);
        }
    }

    points_array[*points_total - 3] = master_line[master_line_size - 1];
    points_array[*points_total - 2] = master_line[master_line_size - 2];
    points_array[*points_total - 1] = master_line[master_line_size - 3];

    index = 0;

    for (int j = 0; j < slices; j++) {

        // add top triangle
        index_array[index++] = 0;
        index_array[index++] = (stacks - 1) * j + 1;
        index_array[index++] = (stacks - 1) * ((j + 1) % slices) + 1;

        for (int i = 0; i < stacks - 2; i++) {
            // primeiro triangulo da stack
            index_array[index++] = (stacks - 1) * j + i + 1;
            index_array[index++] = (stacks - 1) * j + (i + 1) + 1;
            index_array[index++] =
                (stacks - 1) * ((j + 1) % slices) + (i + 1) + 1;

            // segundo triangulo da stack
            index_array[index++] = (stacks - 1) * j + i + 1;
            index_array[index++] =
                (stacks - 1) * ((j + 1) % slices) + (i + 1) + 1;
            index_array[index++] = (stacks - 1) * ((j + 1) % slices) + i + 1;
        }
        // add bottom triangle
        index_array[index++] = (stacks - 1) + (stacks - 1) * (j);
        index_array[index++] = (stacks - 1) * (slices) + 1; // last
        index_array[index++] = (stacks - 1) + (stacks - 1) * ((j + 1) % slices);
    }

    return make_tuple(points_array, index_array);
}

void write3D(const char *filename, unsigned int nVertices, float *points,
             unsigned int nIndices, unsigned int *indices) {
    ofstream file;
    file.open(filename, ios::out | ios::binary | ios::trunc);

    // Pontos
    file.write((char *)&nVertices, sizeof(unsigned int));
    file.write((char *)points, sizeof(float) * nVertices);

    free(points);

    // Indices
    file.write((char *)&nIndices, sizeof(unsigned int));
    file.write((char *)indices, sizeof(unsigned int) * nIndices);
    
    free(indices);

    file.close();
}

int main(int argc, char *argv[]) {
    if (!strcmp(argv[1], "sphere")) {
        unsigned int points_total, index_total;
        tuple<float *, unsigned int *> sphere =
            generate_sphere_index(atof(argv[2]), atoi(argv[3]), atoi(argv[4]),
                                  &points_total, &index_total);
        
        write3D(argv[5], points_total, get<0>(sphere), index_total,
                get<1>(sphere));
    } else if (!strcmp(argv[1], "box")) {
        unsigned int points_total, index_total;
        tuple<float *, unsigned int *> box = generate_box_index(
            atof(argv[2]), atoi(argv[3]), &points_total, &index_total);
        write3D(argv[4], points_total, get<0>(box), index_total, get<1>(box));
    } else if (!strcmp(argv[1], "plane")) {
        unsigned int points_total, index_total;
        tuple<float *, unsigned int *> plane = generate_plane_index(
            atof(argv[2]), atoi(argv[3]), &points_total, &index_total);
        write3D(argv[4], points_total, get<0>(plane), index_total,
                get<1>(plane));
    } else if (!strcmp(argv[1], "cone")) {
        unsigned int points_total, index_total;
        tuple<float *, unsigned int *> cone =
            generate_cone_index(atof(argv[2]), atoi(argv[3]), atoi(argv[4]),
                                atoi(argv[5]), &points_total, &index_total);
        write3D(argv[6], points_total, get<0>(cone), index_total, get<1>(cone));
    } else if (!strcmp(argv[1], "torus")) {
        unsigned int points_total, index_total;
        tuple<float *, unsigned int *> torus =
            generate_torus_index(atof(argv[2]), atoi(argv[3]), atoi(argv[4]),
                                 atoi(argv[5]), &points_total, &index_total);
        write3D(argv[6], points_total, get<0>(torus), index_total,
                get<1>(torus));
    } else if (!strcmp(argv[1], "cylinder")) {
        unsigned int points_total, index_total;
        tuple<float *, unsigned int *> cylinder =
            generate_cylinder_index(atof(argv[2]), atoi(argv[3]), atoi(argv[4]),
                                    atoi(argv[5]), &points_total, &index_total);
        write3D(argv[6], points_total, get<0>(cylinder), index_total,
                get<1>(cylinder));
    } else if (!strcmp(argv[1], "patch")) {
        tuple<vector<float> *, vector<unsigned int> *> bezier =
            generate_bezier(argv[2], atoi(argv[3]));
        write3D(argv[4], get<0>(bezier)->size(), get<0>(bezier)->data(),
                get<1>(bezier)->size(), get<1>(bezier)->data());
    } else {
        printf("Invalid Model\n");
    }

    return 0;
}
