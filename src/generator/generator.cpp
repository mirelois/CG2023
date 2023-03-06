#include <cstdlib>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>

#include<tuple>
#include "matrix.h"

using namespace std;





/*
float* generate_box(float length,  int grid_slices)
{
    //a quantidade de pontos ´e definida pelo grid;
    //grid**2 * 2 * 3 * 6 (nr quadrados * nr triangulos * nr pontos no triangulo * nr faces)    

    tuple <float, float, float> point_array[grid_slices^2 *36];

    float delta = length/grid_slices;

    int index = 0;

    float referential_x = length/2;
    float 0 = -length/2;

    float referential_z = length/2;

    for(int i; i < grid_slices; i++){
        for (int j; j < grid_slices; j++)
        {
            point_array[index++] = make_tuple(referential_x, 0, referential_z);
            point_array[index++] = make_tuple(referential_x, 0-delta, referential_z);
            point_array[index++] = make_tuple(referential_x+delta, 0, referential_z);

            point_array[index++] = make_tuple(referential_x, 0-delta, referential_z);
            point_array[index++] = make_tuple(referential_x+delta, 0-delta, referential_z);
            point_array[index++] = make_tuple(referential_x+delta, 0, referential_z);
            //(x,y),(x,y+delta),(x+delta,y)
            //(x,y+delta),(x+delta,y+delta),(x+delta,y)
            point_array[index++] = make_tuple(referential_x, 0, -referential_z);
            point_array[index++] = make_tuple(referential_x, 0+delta, -referential_z);
            point_array[index++] = make_tuple(referential_x+delta, 0, -referential_z);

            point_array[index++] = make_tuple(referential_x, 0+delta, -referential_z);
            point_array[index++] = make_tuple(referential_x+delta, 0+delta, -referential_z);
            point_array[index++] = make_tuple(referential_x+delta, 0, -referential_z);

            referential_x += delta;
             
        }
        0 += delta;
    }

    referential_x = length/2;

    for(int i; i < grid_slices; i++){
        for (int j; j < grid_slices; j++)
        {
            point_array[index++] = make_tuple(referential_x, 0, referential_z);
            point_array[index++] = make_tuple(referential_x, 0+delta, referential_z);
            point_array[index++] = make_tuple(referential_x+delta, 0, referential_z);

            point_array[index++] = make_tuple(referential_x, 0+delta, referential_z);
            point_array[index++] = make_tuple(referential_x+delta, 0+delta, referential_z);
            point_array[index++] = make_tuple(referential_x+delta, 0, referential_z);
            //(x,y),(x,y+delta),(x+delta,y)
            //(x,y+delta),(x+delta,y+delta),(x+delta,y)
            point_array[index++] = make_tuple(referential_x, 0, -referential_z);
            point_array[index++] = make_tuple(referential_x, 0+delta, -referential_z);
            point_array[index++] = make_tuple(referential_x+delta, 0, -referential_z);

            point_array[index++] = make_tuple(referential_x, 0+delta, -referential_z);
            point_array[index++] = make_tuple(referential_x+delta, 0+delta, -referential_z);
            point_array[index++] = make_tuple(referential_x+delta, 0, -referential_z);

            referential_x += delta;
             
        }
        0 += delta;
    }

}
*/

void generate_cone(tuple<float,float,float>* point_array, float bottom_radius, float height, int slices, int stacks){
    int i, j, index = 0;
	double alfa = 2 * M_PI / slices;
	double division_height_step = height / stacks;
	double division_radius_step = bottom_radius / stacks;
	// Parte de baixo
	for (i = 0; i < slices; i++) {
		point_array[index++] = make_tuple(bottom_radius * sin(alfa * (i + 1)), 0.0f, bottom_radius * cos(alfa * (i + 1)));
		point_array[index++] = make_tuple(bottom_radius * sin(alfa * i), 0.0f, bottom_radius * cos(alfa * i));
		point_array[index++] = make_tuple(0.0f, 0.0f, 0.0f);
	}

	for (i = 0; i < stacks; i++) {
		for (j = 0; j < slices; j++) {
			double bot_height = division_height_step * i;
			double top_height = division_height_step * (i + 1);
			double bot_radius = bottom_radius - division_radius_step * i;
			double top_radius = bottom_radius - division_radius_step * (i + 1);

			// lados
			point_array[index++] = make_tuple(bot_radius * sin(alfa * j), bot_height, bot_radius * cos(alfa * j));
			point_array[index++] = make_tuple(bot_radius * sin(alfa * (j + 1)), bot_height, bot_radius * cos(alfa * (j + 1)));
			point_array[index++] = make_tuple(top_radius * sin(alfa * j), top_height, top_radius * cos(alfa * j));

			point_array[index++] = make_tuple(top_radius * sin(alfa * j), top_height, top_radius * cos(alfa * j));
			point_array[index++] = make_tuple(bot_radius * sin(alfa * (j + 1)), bot_height, bot_radius * cos(alfa * (j + 1)));
			point_array[index++] = make_tuple(top_radius * sin(alfa * (j + 1)), top_height, top_radius * cos(alfa * (j + 1)));
		}
	}
}


void generate_box(float length,  int grid_slices)
{
    //a quantidade de pontos ´e definida pelo grid;
    //grid**2 * 2 * 3 * 6 (nr quadrados * nr triangulos * nr pontos no triangulo * nr faces)    
    
    int points_total = grid_slices*grid_slices*36;

    tuple <float, float, float> point_array[points_total];

    float delta = length/grid_slices;

    int index = 0;

    float referential_x = -length/2;

    float referential_y = length/2;

    float referential_z = length/2;

    int j;

    for(int i = 0; i < grid_slices; i++){
        for (j = 0; j < grid_slices; j++)
        {
            point_array[index++] = make_tuple(referential_x, referential_y, referential_z);
            point_array[index++] = make_tuple(referential_x, referential_y-delta, referential_z);
            point_array[index++] = make_tuple(referential_x+delta, referential_y-delta, referential_z);

            point_array[index++] = make_tuple(referential_x, referential_y, referential_z);
            point_array[index++] = make_tuple(referential_x+delta, referential_y-delta, referential_z);
            point_array[index++] = make_tuple(referential_x+delta, referential_y, referential_z);

            referential_x += delta;
        }

        referential_y -= delta;
        referential_x = -length/2;
    }

    for (int i = 0; i < points_total/6; i++) {
        //back face
        point_array[index++] = make_tuple(-get<0>(point_array[i]), get<1>(point_array[i]), -get<2>(point_array[i]));
    }

    for(int i=0;  i < points_total/3; i++)
    {
        point_array[index++] = make_tuple(get<2>(point_array[i]), get<1>(point_array[i]), -get<0>(point_array[i]));
    }
    for(int i=0; i < points_total/3; i++)
    {
        point_array[index++] = make_tuple(get<0>(point_array[i]), -get<2>(point_array[i]), get<1>(point_array[i]));
    }


}


void generate_plane(float length, int grid_slices){

    int points_total = grid_slices*grid_slices*6;

    tuple <float, float, float> point_array[points_total];

    float delta = length/grid_slices;

    int index = 0;

    float referential_x = -length/2;

    float referential_z = -length/2;

    for(int i = 0; i < grid_slices; i++){
        for (int j = 0; j < grid_slices; j++)
        {
            point_array[index++] = make_tuple(referential_x, 0, referential_z);
            point_array[index++] = make_tuple(referential_x, 0, referential_z+delta);
            point_array[index++] = make_tuple(referential_x+delta, 0, referential_z+delta);

            point_array[index++] = make_tuple(referential_x, 0, referential_z);
            point_array[index++] = make_tuple(referential_x+delta, 0, referential_z+delta);
            point_array[index++] = make_tuple(referential_x+delta, 0, referential_z);

            referential_x += delta;
        }
        referential_z += delta;
        referential_x = -length/2;
    }
}


void generate_sphere(float radius, int slices, int stacks) {

    int points_total = slices*6*(stacks-1);
    tuple <float, float, float> point_array[points_total];
    int index = 0; 

	float alfa_x = 0;
	float delta_x = M_PI / stacks;

	float alfa_y = 0;
	float delta_y = 2 * M_PI / slices;

	float pivot_x = 0;
	float pivot_y = radius;
	float pivot_z = 0;
	
	tuple<float, float, float> master_line[stacks+1];


	//generate master line
	int master_line_index = 0;
	for (int i = 0; i < stacks + 1; i++) {

		master_line[master_line_index++] = make_tuple(
			pivot_x,
			pivot_y * cos(alfa_x) - pivot_z * sin(alfa_x),
			pivot_y * sin(alfa_x) + pivot_z * cos(alfa_x)
		);

		alfa_x += delta_x;
	}

	for (int j = 0; j < slices; j++) {
		for (int i = 0; i < stacks - 1; i++) {

			point_array[index++] = make_tuple(
				get<0>(master_line[i]) * cos(alfa_y) + get<2>(master_line[i]) * sin(alfa_y),
				get<1>(master_line[i]),
				-get<0>(master_line[i]) * sin(alfa_y) + get<2>(master_line[i]) * cos(alfa_y)
			);

			point_array[index++] = make_tuple(
				get<0>(master_line[i + 1]) * cos(alfa_y) + get<2>(master_line[i + 1]) * sin(alfa_y),
				get<1>(master_line[i + 1]),
				-get<0>(master_line[i + 1]) * sin(alfa_y) + get<2>(master_line[i + 1]) * cos(alfa_y)
			);

			point_array[index++] = make_tuple(
				get<0>(master_line[i + 1]) * cos(alfa_y + delta_y) + get<2>(master_line[i + 1]) * sin(alfa_y + delta_y),
				get<1>(master_line[i + 1]),
				-get<0>(master_line[i + 1]) * sin(alfa_y + delta_y) + get<2>(master_line[i + 1]) * cos(alfa_y + delta_y)
			);
		}
		alfa_y += delta_y;
	}

	alfa_y = 0;

	delta_y = -delta_y;

	for (int j = 0; j < slices; j++) {
		for (int i = stacks ; i > 0; i--) {

			point_array[index++] = make_tuple(
				get<0>(master_line[i]) * cos(alfa_y) + get<2>(master_line[i]) * sin(alfa_y),
				get<1>(master_line[i]),
				-get<0>(master_line[i]) * sin(alfa_y) + get<2>(master_line[i]) * cos(alfa_y)
			);

			point_array[index++] = make_tuple(
				get<0>(master_line[i - 1]) * cos(alfa_y) + get<2>(master_line[i - 1]) * sin(alfa_y),
				get<1>(master_line[i - 1]),
				-get<0>(master_line[i - 1]) * sin(alfa_y) + get<2>(master_line[i - 1]) * cos(alfa_y)
			);

			point_array[index++] = make_tuple(
				get<0>(master_line[i - 1]) * cos(alfa_y + delta_y) + get<2>(master_line[i - 1]) * sin(alfa_y + delta_y),
				get<1>(master_line[i - 1]),
				-get<0>(master_line[i - 1]) * sin(alfa_y + delta_y) + get<2>(master_line[i - 1]) * cos(alfa_y + delta_y)
			);
		}
		alfa_y += delta_y;
	}
} 





//i mean fine mas estas a fazer isto muito a C
void points_write (const char *filename, const unsigned int nVertices, const float points[])
{
  FILE *fp = fopen (filename, "w");
  if (!fp)
    {
      fprintf (stderr, "failed to open file: %s", filename);
      exit (1);
    }

  fwrite (&nVertices, sizeof (unsigned int), 1, fp);
  fwrite (points, 3 * sizeof (float), nVertices, fp);

  fclose (fp);
}
