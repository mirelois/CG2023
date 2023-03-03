#include <cstdlib>
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
            point_array[++index] = make_tuple(referential_x, 0, referential_z);
            point_array[++index] = make_tuple(referential_x, 0-delta, referential_z);
            point_array[++index] = make_tuple(referential_x+delta, 0, referential_z);

            point_array[++index] = make_tuple(referential_x, 0-delta, referential_z);
            point_array[++index] = make_tuple(referential_x+delta, 0-delta, referential_z);
            point_array[++index] = make_tuple(referential_x+delta, 0, referential_z);
            //(x,y),(x,y+delta),(x+delta,y)
            //(x,y+delta),(x+delta,y+delta),(x+delta,y)
            point_array[++index] = make_tuple(referential_x, 0, -referential_z);
            point_array[++index] = make_tuple(referential_x, 0+delta, -referential_z);
            point_array[++index] = make_tuple(referential_x+delta, 0, -referential_z);

            point_array[++index] = make_tuple(referential_x, 0+delta, -referential_z);
            point_array[++index] = make_tuple(referential_x+delta, 0+delta, -referential_z);
            point_array[++index] = make_tuple(referential_x+delta, 0, -referential_z);

            referential_x += delta;
             
        }
        0 += delta;
    }

    referential_x = length/2;

    for(int i; i < grid_slices; i++){
        for (int j; j < grid_slices; j++)
        {
            point_array[++index] = make_tuple(referential_x, 0, referential_z);
            point_array[++index] = make_tuple(referential_x, 0+delta, referential_z);
            point_array[++index] = make_tuple(referential_x+delta, 0, referential_z);

            point_array[++index] = make_tuple(referential_x, 0+delta, referential_z);
            point_array[++index] = make_tuple(referential_x+delta, 0+delta, referential_z);
            point_array[++index] = make_tuple(referential_x+delta, 0, referential_z);
            //(x,y),(x,y+delta),(x+delta,y)
            //(x,y+delta),(x+delta,y+delta),(x+delta,y)
            point_array[++index] = make_tuple(referential_x, 0, -referential_z);
            point_array[++index] = make_tuple(referential_x, 0+delta, -referential_z);
            point_array[++index] = make_tuple(referential_x+delta, 0, -referential_z);

            point_array[++index] = make_tuple(referential_x, 0+delta, -referential_z);
            point_array[++index] = make_tuple(referential_x+delta, 0+delta, -referential_z);
            point_array[++index] = make_tuple(referential_x+delta, 0, -referential_z);

            referential_x += delta;
             
        }
        0 += delta;
    }

}
*/

void drawCone(float bottom_radius, float height, int slices, int stacks) {
	int i, j;
	double alfa = 2 * M_PI / slices;
	double division_height_step = height / stacks;
	double division_radius_step = bottom_radius / stacks;
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_TRIANGLES);
	for (i = 0; i < stacks; i++) {
		for (j = 0; j < slices; j++) {
			double bot_height = division_height_step * i;
			double top_height = division_height_step * (i + 1);
			double bot_radius = bottom_radius - division_radius_step * i;
			double top_radius = bottom_radius - division_radius_step * (i + 1);


			// Parte de baixo
			glColor3f(1.0f, 0.0f, 0.0f);
			glVertex3f(bot_radius * sin(alfa * j), bot_height, bot_radius * cos(alfa * j));
			glVertex3f(bot_radius * sin(alfa * (j + 1)), bot_height, bot_radius * cos(alfa * (j + 1)));
			glVertex3f(0.0f, bot_height, 0.0f);

			// Parte de cima
			glVertex3f(top_radius * sin(alfa * j), top_height, top_radius * cos(alfa * j));
			glVertex3f(top_radius * sin(alfa * (j + 1)), top_height, top_radius * cos(alfa * (j + 1)));
			glVertex3f(0.0f, top_height, 0.0f);


			// lados
			glColor3f(0.0f, 1.0f, 0.0f);
			glVertex3f(bot_radius * sin(alfa * j), bot_height, bot_radius * cos(alfa * j));
			glVertex3f(bot_radius * sin(alfa * (j + 1)), bot_height, bot_radius * cos(alfa * (j + 1)));
			glVertex3f(top_radius * sin(alfa * j), top_height, top_radius * cos(alfa * j));

			glVertex3f(top_radius * sin(alfa * j), top_height, top_radius * cos(alfa * j));
			glVertex3f(bot_radius * sin(alfa * (j + 1)), bot_height, bot_radius * cos(alfa * (j + 1)));
			glVertex3f(top_radius * sin(alfa * (j + 1)), top_height, top_radius * cos(alfa * (j + 1)));
		}
	}
	glEnd();
}


float* generate_box(float length,  int grid_slices)
{
    //a quantidade de pontos ´e definida pelo grid;
    //grid**2 * 2 * 3 * 6 (nr quadrados * nr triangulos * nr pontos no triangulo * nr faces)    
    
    int points_total = grid_slices*grid_slices*36;

    tuple <float, float, float> point_array[points_total];

    float delta = length/grid_slices;

    int index = 0;

    float referential_x = length/2;
    float 0 = -length/2;

    float referential_z = length/2;

    for(int i; i < grid_slices; i++){
        for (int j; j < grid_slices; j++)
        {
            point_array[++index] = make_tuple(referential_x, 0, referential_z);
            point_array[++index] = make_tuple(referential_x, 0-delta, referential_z);
            point_array[++index] = make_tuple(referential_x+delta, 0, referential_z);

            point_array[++index] = make_tuple(referential_x, 0-delta, referential_z);
            point_array[++index] = make_tuple(referential_x+delta, 0-delta, referential_z);
            point_array[++index] = make_tuple(referential_x+delta, 0, referential_z);
            referential_x += delta;
        }
        0 += delta;
    }

    float rotation_matrix_back[3][3] = {{-1,0,0},
                                        {0,1,0},
                                        {0,0,-1}};
    for (int i = 0; i < points_total/6; i++) {
        //back face
        point_array[++index] = matrix_mult_tuple(rotation_matrix_back, point_array[i]);
    }

    float rotation_matrix_y[3][3] = {{0,0,1},
                                   {0,1,0},
                                   {-1,0,0}};

    float rotation_matrix_x[3][3] = {{1,0,0},
                                    {0,0,-1},
                                    {0,1,0}};

    for(int i=0; points_total/3; i++)
    {
        point_array[++index] = matrix_mult_tuple(rotation_matrix_y, point_array[i]);
        point_array[++index] = matrix_mult_tuple(rotation_matrix_x, point_array[i]);
    }
}


float* generate_plane(float length, int grid_slices){

    int points_total = grid_slices*grid_slices*6;

    tuple <float, float, float> point_array[points_total];

    float delta = length/grid_slices;

    int index = 0;

    float referential_x = -length/2;

    float referential_z = -length/2;

    for(int i; i < grid_slices; i++){
        for (int j; j < grid_slices; j++)
        {
            point_array[++index] = make_tuple(referential_x, 0, referential_z);
            point_array[++index] = make_tuple(referential_x, 0, referential_z+delta);
            point_array[++index] = make_tuple(referential_x+delta, 0, referential_z);

            point_array[++index] = make_tuple(referential_x, 0, referential_z+delta);
            point_array[++index] = make_tuple(referential_x+delta, 0, referential_z);
            point_array[++index] = make_tuple(referential_x+delta, 0, referential_z+delta);

            referential_x += delta;
        }
        referential_z += delta;
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
