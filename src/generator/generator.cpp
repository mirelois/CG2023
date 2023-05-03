#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <ostream>
#include <stdio.h>
#include <tuple>
#define _USE_MATH_DEFINES
#include <math.h>
#include <string.h>
#include <fstream>
#include "bezier.h"

using namespace std;

tuple<float*, unsigned int*> generate_torus_index(float inner_radius, float outer_radius, int vertical_divisions, int horizontal_divisions, unsigned int* points_total, unsigned int* index_total){

    *points_total = horizontal_divisions*vertical_divisions*3;
    *index_total = 6*vertical_divisions*horizontal_divisions;

    float* point_array = (float*) malloc(sizeof(float) * *points_total);
    unsigned int* index_array = (unsigned int*) malloc(sizeof(unsigned int) * *index_total);
    float* master_circle = (float*) malloc(sizeof(float) * (vertical_divisions*3));
    int index = 0;
    
    int master_circle_index = 0;

    float delta_x = 2*M_PI/vertical_divisions;

    float delta_y = 2*M_PI/horizontal_divisions;

    float pivot_x = 0;

    float pivot_y = (outer_radius-inner_radius)/2;

    //float pivot_z = 0;


    for (int i = 0; i < vertical_divisions; i++) {
        master_circle[master_circle_index++] = pivot_x;
        master_circle[master_circle_index++] = pivot_y*cos(i*delta_x);
        master_circle[master_circle_index++] = pivot_y*sin(i*delta_x) + (outer_radius+inner_radius)/2;
    }

    for (int j = 0; j < vertical_divisions; j++) {
        for (int i = 0; i < horizontal_divisions; i++) {
            point_array[index++] = (master_circle[3*j + 0])*cos(i*delta_y) + (master_circle[3*j + 2])*sin(i*delta_y);
            point_array[index++] = (master_circle[3*j + 1]);
            point_array[index++] = (master_circle[3*j + 0])*sin(i*delta_y) + (master_circle[3*j + 2])*cos(i*delta_y);
        }
    }

    index = 0;

    for (int j = 0; j < vertical_divisions; j++) {
        for (int i = 0; i < horizontal_divisions; i++) {
            index_array[index++] = horizontal_divisions*j + i;
            index_array[index++] = horizontal_divisions*((j+1)%vertical_divisions) + i;
            index_array[index++] = horizontal_divisions*((j+1)%vertical_divisions) + ((i+1)%horizontal_divisions);

            index_array[index++] = horizontal_divisions*j + i;
            index_array[index++] = horizontal_divisions*((j+1)%vertical_divisions) + ((i+1)%horizontal_divisions);
            index_array[index++] = horizontal_divisions*j + ((i+1)%horizontal_divisions);
        }
    }

    for (int i=0; i < *index_total; i++) {
        printf("%u,",index_array[i]); 
        if((i+1)%3 == 0) putchar('\n');
        if((i+1)%6 == 0) putchar('\n');
    }

    return make_tuple(point_array, index_array);
}




float* generate_torus(float inner_radius, float outer_radius, int vertical_divisions, int horizontal_divisions, int* points_total){

    *points_total = 18*vertical_divisions*horizontal_divisions;

    float* point_array = (float*) malloc(sizeof(float) * *points_total);
    int index = 0;

    
    tuple<float, float, float>* master_circle = new tuple<float, float, float>[vertical_divisions];
    
    int master_circle_index = 0;


    float delta_x = 2*M_PI/vertical_divisions;

    float delta_y = 2*M_PI/horizontal_divisions;

    float pivot_x = 0;

    float pivot_y = (outer_radius-inner_radius)/2;

    //float pivot_z = 0;


    for (int i = 0; i < vertical_divisions; i++) {

        master_circle[master_circle_index++] = make_tuple(
                pivot_x,
                pivot_y*cos(i*delta_x),
                pivot_y*sin(i*delta_x) + (outer_radius+inner_radius)/2
                );

    }

    for (int i = 0; i < horizontal_divisions; i++) {

        for (int j = 0; j < vertical_divisions; j++) {

            point_array[index++] = get<0>(master_circle[j])*cos(i*delta_y) + get<2>(master_circle[j])*sin(i*delta_y);
            point_array[index++] = get<1>(master_circle[j]);
            point_array[index++] = -get<0>(master_circle[j])*sin(i*delta_y) + get<2>(master_circle[j])*cos(i*delta_y);

            point_array[index++] = get<0>(master_circle[(j+1)%vertical_divisions])*cos(i*delta_y) + get<2>(master_circle[(j+1)%vertical_divisions])*sin(i*delta_y);
            point_array[index++] = get<1>(master_circle[(j+1)%vertical_divisions]);
            point_array[index++] = -get<0>(master_circle[(j+1)%vertical_divisions])*sin(i*delta_y) + get<2>(master_circle[(j+1)%vertical_divisions])*cos(i*delta_y);

            point_array[index++] = get<0>(master_circle[(j+1)%vertical_divisions])*cos(((i+1)%horizontal_divisions)*delta_y) + get<2>(master_circle[(j+1)%vertical_divisions])*sin(((i+1)%horizontal_divisions)*delta_y);
            point_array[index++] = get<1>(master_circle[(j+1)%vertical_divisions]);
            point_array[index++] = -get<0>(master_circle[(j+1)%vertical_divisions])*sin(((i+1)%horizontal_divisions)*delta_y) + get<2>(master_circle[(j+1)%vertical_divisions])*cos(((i+1)%horizontal_divisions)*delta_y);


            point_array[index++] = get<0>(master_circle[j])*cos(i*delta_y) + get<2>(master_circle[j])*sin(i*delta_y);
            point_array[index++] = get<1>(master_circle[j]);
            point_array[index++] = -get<0>(master_circle[j])*sin(i*delta_y) + get<2>(master_circle[j])*cos(i*delta_y);

            point_array[index++] = get<0>(master_circle[(j+1)%vertical_divisions])*cos(((i+1)%horizontal_divisions)*delta_y) + get<2>(master_circle[(j+1)%vertical_divisions])*sin(((i+1)%horizontal_divisions)*delta_y);
            point_array[index++] = get<1>(master_circle[(j+1)%vertical_divisions]);
            point_array[index++] = -get<0>(master_circle[(j+1)%vertical_divisions])*sin(((i+1)%horizontal_divisions)*delta_y) + get<2>(master_circle[(j+1)%vertical_divisions])*cos(((i+1)%horizontal_divisions)*delta_y);

            point_array[index++] = get<0>(master_circle[j])*cos(((i+1)%horizontal_divisions)*delta_y) + get<2>(master_circle[j])*sin(((i+1)%horizontal_divisions)*delta_y);
            point_array[index++] = get<1>(master_circle[j]);
            point_array[index++] = -get<0>(master_circle[j])*sin(((i+1)%horizontal_divisions)*delta_y) + get<2>(master_circle[j])*cos(((i+1)%horizontal_divisions)*delta_y);
        }
    }
    return point_array;
}


vector<float>* generate_cone(float bottom_radius, float height, int slices, int stacks){
    
    vector<float>* point_array = new vector<float>();
    int i, j;
	double alfa = 2 * M_PI / slices;
	double division_height_step = height / stacks;
	double division_radius_step = bottom_radius / stacks;
	// Base
	for (i = 0; i < slices; i++) {
		point_array->push_back(bottom_radius * sin(alfa * (i + 1)));
        point_array->push_back(0.0f);
        point_array->push_back(bottom_radius * cos(alfa * (i + 1)));

        point_array->push_back(bottom_radius * sin(alfa * i));
        point_array->push_back(0.0f);
        point_array->push_back(bottom_radius * cos(alfa * i));

		point_array->push_back(0.0f);
        point_array->push_back(0.0f);
        point_array->push_back(0.0f);
	}
    
	for (i = 0; i < stacks; i++) {
		for (j = 0; j < slices; j++) {
			double bot_height = division_height_step * i;
			double top_height = bot_height + division_height_step;
			double bot_radius = bottom_radius - division_radius_step * i;
			double top_radius = bot_radius - division_radius_step;

			// lados
			point_array->push_back(bot_radius * sin(alfa * j));
            point_array->push_back(bot_height);
            point_array->push_back(bot_radius * cos(alfa * j));

			point_array->push_back(bot_radius * sin(alfa * (j + 1)));
            point_array->push_back(bot_height);
            point_array->push_back(bot_radius * cos(alfa * (j + 1)));

			point_array->push_back(top_radius * sin(alfa * j));
            point_array->push_back(top_height);
            point_array->push_back(top_radius * cos(alfa * j));

			point_array->push_back(top_radius * sin(alfa * j));
            point_array->push_back(top_height);
            point_array->push_back(top_radius * cos(alfa * j));

			point_array->push_back(bot_radius * sin(alfa * (j + 1)));
            point_array->push_back(bot_height);
            point_array->push_back(bot_radius * cos(alfa * (j + 1)));

			point_array->push_back(top_radius * sin(alfa * (j + 1)));
            point_array->push_back(top_height);
            point_array->push_back(top_radius * cos(alfa * (j + 1)));
		}
	}
    

    return point_array;
}

vector<float>* generate_cylinder(float radius, float height, int slices, int stacks){
    vector<float>* point_array = new vector<float>();

    float division_height_step = height/stacks;
    float alfa = 2*M_PI/slices;

    for (int i = 0; i < slices; i++) {
        //bottom part
        point_array->push_back(0.0f);
        point_array->push_back(-height/2);
        point_array->push_back(0.0f);

        point_array->push_back(radius*sin(alfa * (i+1)));
        point_array->push_back(-height/2);
        point_array->push_back(radius*cos(alfa * (i+1)));

        point_array->push_back(radius*sin(alfa * i));
        point_array->push_back(-height/2);
        point_array->push_back(radius*cos(alfa * i));
        
        //top part
        point_array->push_back(radius*sin(alfa * i));
        point_array->push_back(height/2);
        point_array->push_back(radius*cos(alfa * i));

        point_array->push_back(radius*sin(alfa * (i+1)));
        point_array->push_back(height/2);
        point_array->push_back(radius*cos(alfa * (i+1)));

        point_array->push_back(0.0f);
        point_array->push_back(height/2);
        point_array->push_back(0.0f);

        //middle part
        for(int j=0; j<stacks; j++){
            double bot_height = -height/2 + j*division_height_step;
            double top_height = bot_height + division_height_step;

            point_array->push_back(radius*sin(alfa * i));
            point_array->push_back(bot_height);
            point_array->push_back(radius*cos(alfa * i));

            point_array->push_back(radius*sin(alfa * (i+1)));
            point_array->push_back(bot_height);
            point_array->push_back(radius*cos(alfa * (i+1)));

            point_array->push_back(radius*sin(alfa * i));
            point_array->push_back(top_height);
            point_array->push_back(radius*cos(alfa * i));

            point_array->push_back(radius*sin(alfa * i));
            point_array->push_back(top_height);
            point_array->push_back(radius*cos(alfa * i));

            point_array->push_back(radius*sin(alfa * (i+1)));
            point_array->push_back(bot_height);
            point_array->push_back(radius*cos(alfa * (i+1)));

            point_array->push_back(radius*sin(alfa * (i+1)));
            point_array->push_back(top_height);
            point_array->push_back(radius*cos(alfa * (i+1)));
        }
	}

    return point_array;
}


float* generate_box(float length,  int grid_slices, int* points_total)
{
    //a quantidade de pontos ´e definida pelo grid;
    //grid**2 * 2 * 3 * 6 (nr quadrados * nr triangulos * nr pontos no triangulo * nr faces)    
    
    *points_total = grid_slices*grid_slices*36*3;
    float* point_array = (float*) malloc(sizeof(float)* *points_total);

    float delta = length/grid_slices;

    int index = 0;

    float referential_x = -length/2;

    float referential_y = length/2;

    float referential_z = length/2;

    for(int i = 0; i < grid_slices; i++){
        for (int j = 0; j < grid_slices; j++)
        {
            point_array[index++] = j*delta+referential_x;
            point_array[index++] = -i*delta+referential_y;
            point_array[index++] = referential_z;

            point_array[index++] = j*delta+referential_x;
            point_array[index++] = -(i+1)*delta+referential_y;
            point_array[index++] = referential_z;

            point_array[index++] = (j+1)*delta+referential_x;
            point_array[index++] = -(i+1)*delta+referential_y;
            point_array[index++] = referential_z;


            point_array[index++] = j*delta+referential_x;
            point_array[index++] = -i*delta+referential_y;
            point_array[index++] = referential_z;

            point_array[index++] = (j+1)*delta+referential_x;
            point_array[index++] = -(i+1)*delta+referential_y;
            point_array[index++] = referential_z;

            point_array[index++] = (j+1)*delta+referential_x;
            point_array[index++] = -i*delta+referential_y;
            point_array[index++] = referential_z;
        }
    }
    
    //x -> i*3 + 0
    //y -> i*3 + 1
    //z -> i*3 + 2
    for (int i = 0; i < *points_total/18; i++) 
    {//-x, y, -z
        //back face
        point_array[index++] = -point_array[i*3 + 0];
        point_array[index++] =  point_array[i*3 + 1];
        point_array[index++] = -point_array[i*3 + 2];
    }

        for(int i=0;  i < *points_total/9; i++)
        {//z, y, -x
            point_array[index++] =  point_array[i*3 + 2];
            point_array[index++] =  point_array[i*3 + 1];
            point_array[index++] = -point_array[i*3 + 0];
        }
        
        for(int i=0; i < *points_total/9; i++)
        {//x, -z, y
            point_array[index++] =  point_array[i*3 + 0];
            point_array[index++] = -point_array[i*3 + 2];
            point_array[index++] =  point_array[i*3 + 1];
        }
    
    return point_array;
}

tuple<float*, unsigned int*> generate_plane_index(float length, int grid_slices, unsigned int* points_total, unsigned int* index_total){

    *index_total = grid_slices*grid_slices*6;
    *points_total = (grid_slices+1)*(grid_slices+1)*3;

    float* point_array = (float*) malloc(sizeof(float) * *points_total);
    unsigned int* index_array = (unsigned int*) malloc(sizeof(unsigned int) * *index_total);

    float delta = length/grid_slices;

    int index = 0;

    float referential_x = -length/2;

    float referential_z = -length/2;
    
    for(int i = 0; i < grid_slices+1; i++){
        for (int j = 0; j < grid_slices+1; j++){
            point_array[index++] = j*delta+referential_x;
            point_array[index++] = 0.0f;
            point_array[index++] = i*delta+referential_z;
        }
    }

    index = 0;

    for(int i = 0; i < grid_slices; i++){
        for (int j = 0; j < grid_slices; j++){

            index_array[index++] = (grid_slices+1)*i+j;
            index_array[index++] = (grid_slices+1)*(i+1)+j;
            index_array[index++] = (grid_slices+1)*(i+1)+(j+1);

            index_array[index++] = (grid_slices+1)*i+j;
            index_array[index++] = (grid_slices+1)*(i+1)+(j+1);
            index_array[index++] = (grid_slices+1)*i+(j+1);
        }
    }
    return make_tuple(point_array, index_array);
}

float* generate_plane(float length, int grid_slices, int* points_total){

    *points_total = grid_slices*grid_slices*18;

    float* point_array = (float*) malloc(sizeof(float) * *points_total);

    float delta = length/grid_slices;

    int index = 0;

    float referential_x = -length/2;

    float referential_z = -length/2;
    
    for(int i = 0; i < grid_slices; i++){
        for (int j = 0; j < grid_slices; j++)
        {
            point_array[index++] = j*delta+referential_x;
            point_array[index++] = 0.0f;
            point_array[index++] = i*delta+referential_z;
            
            point_array[index++] = j*delta+referential_x;
            point_array[index++] = 0.0f;
            point_array[index++] = (i+1)*delta+referential_z;

            point_array[index++] = (j+1)*delta+referential_x;
            point_array[index++] = 0.0f;
            point_array[index++] = (i+1)*delta+referential_z;


            point_array[index++] = j*delta+referential_x;
            point_array[index++] = 0.0f;
            point_array[index++] = i*delta+referential_z;

            point_array[index++] = (j+1)*delta+referential_x;
            point_array[index++] = 0.0f;
            point_array[index++] = (i+1)*delta+referential_z;

            point_array[index++] = (j+1)*delta+referential_x;
            point_array[index++] = 0.0f;
            point_array[index++] = i*delta+referential_z;
        }
    }
    

    return point_array;
}


tuple<float*, unsigned int*> generate_sphere_index(float radius, int slices, int stacks, unsigned int *points_total, unsigned int* index_total){
    *index_total = slices*6*(stacks-1);
    *points_total = 3*(slices*(stacks-1)+2);
    int master_line_size = (stacks+1)*3;
    float alfa_x = M_PI/stacks;
    float alfa_y = 2*M_PI/slices;

    float pivot_x = 0;
    float pivot_y = radius;
    //float pivot_z = 0;

    float* master_line = (float*) malloc(sizeof(float) * master_line_size);
    float* points_array = (float*) malloc(sizeof(float) * *points_total);
    unsigned int* index_array = (unsigned int*) malloc(sizeof(unsigned int) * *index_total);

    int master_line_index = 0;
    for (int i = 0; i < stacks+1; i++) {
        master_line[master_line_index++] = pivot_x;
        master_line[master_line_index++] = pivot_y*cos(i*alfa_x);
        master_line[master_line_index++] = pivot_y*sin(i*alfa_x);
    }

    int index = 0;

    points_array[index++] = master_line[0];
    points_array[index++] = master_line[1];
    points_array[index++] = master_line[2];

    for (int j = 0; j < slices; j++) {
        for (int i = 1; i <= stacks-1; i++) {
            points_array[index++] = (master_line[i*3 + 0])*cos(j*alfa_y) + (master_line[i*3 + 2])*sin(j*alfa_y);
            points_array[index++] = (master_line[i*3 + 1]);
            points_array[index++] = (master_line[i*3 + 0])*sin(j*alfa_y) + (master_line[i*3 + 2])*cos(j*alfa_y);
        }
    }

    points_array[*points_total-3] = master_line[master_line_size-1];
    points_array[*points_total-2] = master_line[master_line_size-2];
    points_array[*points_total-1] = master_line[master_line_size-3];

    index = 0;
    
    for (int j = 0; j < slices; j++) {
        
        //add top triangle
        index_array[index++] = 0;
        index_array[index++] = (stacks-1)*j+1;
        index_array[index++] = (stacks-1)*((j+1)%slices)+1;
        
        for (int i = 0; i < stacks-2; i++) {
            //primeiro triangulo da stack
            index_array[index++] = (stacks-1)*j+i+1;
            index_array[index++] = (stacks-1)*j+(i+1)+1;
            index_array[index++] = (stacks-1)*((j+1)%slices)+(i+1)+1;
            
            //segundo triangulo da stack
            index_array[index++] = (stacks-1)*j+i+1;
            index_array[index++] = (stacks-1)*((j+1)%slices)+(i+1)+1;
            index_array[index++] = (stacks-1)*((j+1)%slices)+i+1;
        }
        //add bottom triangle
        index_array[index++] = (stacks-1)+(stacks-1)*(j);
        index_array[index++] = (stacks-1)*(slices)+1;//last
        index_array[index++] = (stacks-1)+(stacks-1)*((j+1)%slices);
    }

    return make_tuple(points_array, index_array);
}

float* generate_sphere(float radius, int slices, int stacks, int *points_total){
    *points_total = 3*slices*6*(stacks-1);
    float alfa_x = M_PI/stacks;
    float alfa_y = 2*M_PI/slices;

    float pivot_x = 0;
    float pivot_y = radius;
    //float pivot_z = 0;

    tuple<float, float, float>* master_line = new tuple<float,float,float>[stacks+1];
    float* points_array = (float*) malloc(sizeof(float) * *points_total);

    //generate master line
    int master_line_index = 0;
    int i;
    for (i = 0; i < stacks+1; i++) {

        master_line[master_line_index++] = make_tuple(
                pivot_x,
                pivot_y*cos(i*alfa_x),
                pivot_y*sin(i*alfa_x)
                );
    }
    int index = 0;
    for (int j = 0; j < slices; j++) {
        for (int i = 0; i < stacks-1; i++) {
            
            points_array[index++] = get<0>(master_line[i])*cos(j*alfa_y) + get<2>(master_line[i])*sin(j*alfa_y);
            points_array[index++] = get<1>(master_line[i]);
            points_array[index++] = -get<0>(master_line[i])*sin(j*alfa_y) + get<2>(master_line[i])*cos(j*alfa_y);


            points_array[index++] = get<0>(master_line[i+1])*cos(j*alfa_y) + get<2>(master_line[i+1])*sin(j*alfa_y);
            points_array[index++] = get<1>(master_line[i+1]);
            points_array[index++] = -get<0>(master_line[i+1])*sin(j*alfa_y) + get<2>(master_line[i+1])*cos(j*alfa_y);

            points_array[index++] = get<0>(master_line[i+1])*cos(((j+1)%slices)*alfa_y) + get<2>(master_line[i+1])*sin(((j+1)%slices)*alfa_y);
            points_array[index++] = get<1>(master_line[i+1]);
            points_array[index++] = -get<0>(master_line[i+1])*sin(((j+1)%slices)*alfa_y) + get<2>(master_line[i+1])*cos(((j+1)%slices)*alfa_y);
            
        }
        for (int i = stacks; i > 1; i--) {
            
            points_array[index++] = get<0>(master_line[i])*cos(-j*alfa_y) + get<2>(master_line[i])*sin(-j*alfa_y);
            points_array[index++] = get<1>(master_line[i]);
            points_array[index++] = -get<0>(master_line[i])*sin(-j*alfa_y) + get<2>(master_line[i])*cos(-j*alfa_y);

            points_array[index++] = get<0>(master_line[i-1])*cos(-j*alfa_y) + get<2>(master_line[i-1])*sin(-j*alfa_y);
            points_array[index++] = get<1>(master_line[i-1]);
            points_array[index++] = -get<0>(master_line[i-1])*sin(-j*alfa_y) + get<2>(master_line[i-1])*cos(-j*alfa_y);

            points_array[index++] = get<0>(master_line[i-1])*cos(-((j+1)%slices)*alfa_y) + get<2>(master_line[i-1])*sin(-((j+1)%slices)*alfa_y);
            points_array[index++] = get<1>(master_line[i-1]);
            points_array[index++] = -get<0>(master_line[i-1])*sin(-((j+1)%slices)*alfa_y) + get<2>(master_line[i-1])*cos(-((j+1)%slices)*alfa_y);
        }
    }

    return points_array;
}


void points_write(const char *filename, int nVertices, float* points) 
{
    fstream file;
    file.open(filename,ios::out|ios::binary|ios::in|ios::trunc);

    file.write((char*)&nVertices, sizeof(int));
    file.write((char*)points, sizeof(float)*nVertices);

    file.close();
}

void write3D(const char *filename, unsigned int nVertices, float* points, unsigned int nIndices, unsigned int* indices){
    ofstream file;
    file.open(filename, ios::out|ios::binary|ios::trunc);

    // Pontos
    file.write((char*)&nVertices, sizeof(unsigned int));
    file.write((char*)points, sizeof(float)*nVertices);

    // Indices
    file.write((char*)&nIndices, sizeof(unsigned int));
    file.write((char*)indices, sizeof(unsigned int)*nIndices);

    file.close();
}

int main(int argc, char* argv[]){
    if(!strcmp(argv[1], "sphere")){
        unsigned int points_total, index_total;
        //float* sphere = generate_sphere(atof(argv[2]), atoi(argv[3]), atoi(argv[4]), &points_total);
        tuple<float*, unsigned int*> sphere = generate_sphere_index(atof(argv[2]), atoi(argv[3]), atoi(argv[4]), &points_total, &index_total);
        float* ar = get<0>(sphere);
        //points_write(argv[5], points_total, sphere);
        write3D(argv[5], points_total, get<0>(sphere), index_total, get<1>(sphere));
    } else if(!strcmp(argv[1], "box")){
        int points_total;
        float* box = generate_box(atof(argv[2]), atoi(argv[3]), &points_total);
        points_write(argv[4], points_total, box);
        free(box);
    } else if(!strcmp(argv[1], "plane")){
        unsigned int points_total, index_total;
        tuple<float*, unsigned int*> plane = generate_plane_index(atof(argv[2]), atoi(argv[3]), &points_total, &index_total);
        write3D(argv[4], points_total, get<0>(plane), index_total, get<1>(plane));
    } else if(!strcmp(argv[1], "cone")){
        vector<float>* cone = generate_cone(atof(argv[2]), atof(argv[3]),atoi(argv[4]),atoi(argv[5]));
        points_write(argv[6], cone->size(), cone->data());
        free(cone);
    } else if(!strcmp(argv[1], "torus")){
        unsigned int points_total, index_total;
        tuple<float*, unsigned int*> torus = generate_torus_index(atof(argv[2]), atoi(argv[3]), atoi(argv[4]), atoi(argv[5]), &points_total, &index_total);
        write3D(argv[6], points_total, get<0>(torus), index_total, get<1>(torus));
    } else if(!strcmp(argv[1], "cylinder")){
        vector<float>* cylinder = generate_cylinder(atof(argv[2]), atof(argv[3]), atoi(argv[4]), atoi(argv[5]));
        points_write(argv[6], cylinder->size(), cylinder->data());
        free(cylinder);
    }else if(!strcmp(argv[1], "patch")){
        vector<float>* bezier = generate_bezier(argv[2], atoi(argv[3]));
        points_write(argv[4], bezier->size(), bezier->data());
        free(bezier);
    }
    else{
        printf("Invalid Model\n");
    }

    return 0;
}
