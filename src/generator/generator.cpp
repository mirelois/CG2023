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

void drawCone(float bottom_radius, float height, int slices, int stacks) {
	int i, j;
	double alfa = 2 * M_PI / slices;
	double division_height_step = height / stacks;
	double division_radius_step = bottom_radius / stacks;
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_TRIANGLES);
	// Parte de baixo
	for (i = 0; i < slices; i++) {
		glVertex3f(bottom_radius * sin(alfa * (i + 1)), 0.0f, bottom_radius * cos(alfa * (i + 1)));
		glVertex3f(bottom_radius * sin(alfa * i), 0.0f, bottom_radius * cos(alfa * i));
		glVertex3f(0.0f, 0.0f, 0.0f);
	}

	for (i = 0; i < stacks; i++) {
		for (j = 0; j < slices; j++) {
			double bot_height = division_height_step * i;
			double top_height = division_height_step * (i + 1);
			double bot_radius = bottom_radius - division_radius_step * i;
			double top_radius = bottom_radius - division_radius_step * (i + 1);


			// lados
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

void generate_cone(flaot* point_array, float bottom_radius, float height, int slices, int stacks){
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


float* generate_box(float length,  int grid_slices)
{
    //a quantidade de pontos ´e definida pelo grid;
    //grid**2 * 2 * 3 * 6 (nr quadrados * nr triangulos * nr pontos no triangulo * nr faces)    
    
    int points_total = grid_slices*grid_slices*36;

    tuple <float, float, float> point_array[points_total];

    float delta = length/grid_slices;

    int index = 0;

    float referential_x = length/2;

    float referential_y = -length/2;

    float referential_z = length/2;

    for(int i; i < grid_slices; i++){
        for (int j; j < grid_slices; j++)
        {
            point_array[index++] = make_tuple(referential_x, referential_y, referential_z);
            point_array[index++] = make_tuple(referential_x, referential_y-delta, referential_z);
            point_array[index++] = make_tuple(referential_x+delta, referential_y, referential_z);

            point_array[index++] = make_tuple(referential_x, referential_y-delta, referential_z);
            point_array[index++] = make_tuple(referential_x+delta, referential_y-delta, referential_z);
            point_array[index++] = make_tuple(referential_x+delta, referential_y, referential_z);
            referential_x += delta;
        }

        referential_y += delta;
    }

    float rotation_matrix_back[3][3] = {{-1,0,0},
                                        {0,1,0},
                                        {0,0,-1}};
    for (int i = 0; i < points_total/6; i++) {
        //back face
        point_array[index++] = matrix_mult_tuple(rotation_matrix_back, point_array[i]);
    }

    float rotation_matrix_y[3][3] = {{0,0,1},
                                   {0,1,0},
                                   {-1,0,0}};

    float rotation_matrix_x[3][3] = {{1,0,0},
                                    {0,0,-1},
                                    {0,1,0}};

    for(int i=0; points_total/3; i++)
    {
        point_array[index++] = matrix_mult_tuple(rotation_matrix_y, point_array[i]);
        point_array[index++] = matrix_mult_tuple(rotation_matrix_x, point_array[i]);
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
            point_array[index++] = make_tuple(referential_x, 0, referential_z);
            point_array[index++] = make_tuple(referential_x, 0, referential_z+delta);
            point_array[index++] = make_tuple(referential_x+delta, 0, referential_z);

            point_array[index++] = make_tuple(referential_x, 0, referential_z+delta);
            point_array[index++] = make_tuple(referential_x+delta, 0, referential_z);
            point_array[index++] = make_tuple(referential_x+delta, 0, referential_z+delta);

            referential_x += delta;
        }
        referential_z += delta;
    }
}

float* generate_sphere(float radius, int slices, int stacks){

    int points_total = 6*(stacks-1)*slices;
    
    tuple <float, float, float> point_array[points_total];

    int index = 0;

    float pivot_x = 0;

    float pivot_y = radius;

    float pivot_z = 0;

    float alfa_x = -2*M_PI/stacks;

    float alfa_y = -2*M_PI/slices;

    //inicialmente gera-se uma slice da esfera
    //
    //este ciclo gera os triangulos do lado esquerdo por rotaçoes nos eixos x e y
    //
    //o pivot ´e o ponto à volta do qual se faz a rotaçao

    for (int i = 0; i < stacks-1; i++) {

        point_array[index++] = make_tuple(pivot_x, pivot_y, pivot_z);

        //pivot_x = pivot_x;
        pivot_y = pivot_y*cos(alfa_x)-pivot_z*sin(alfa_x);
        pivot_z = pivot_y*sin(alfa_x)-pivot_z*cos(alfa_x);

        point_array[index++] = make_tuple(pivot_x, pivot_y, pivot_y);

        point_array[index++] = make_tuple(
                pivot_x*cos(alfa_y) + pivot_z*sin(alfa_y),
                pivot_y,
                -pivot_x*sin(alfa_y) + pivot_z*cos(alfa_y)
                );
    }

    //aqui é preciso desenhar o triangulo de baixo

    point_array[index++] = make_tuple(pivot_x, pivot_y, pivot_z);

    point_array[index++] = make_tuple(
            pivot_x,
            pivot_y*cos(alfa_x)-pivot_z*sin(alfa_x),
            pivot_y*sin(alfa_x)-pivot_z*cos(alfa_x)
            );

    pivot_x = pivot_x*cos(alfa_y) + pivot_z*sin(alfa_y);
    //pivot_y = pivot_y;
    pivot_z = -pivot_x*sin(alfa_y) + pivot_z*cos(alfa_y);

    point_array[index++] = make_tuple(pivot_x, pivot_y, pivot_z);

    //aqui desenha se os triangulos do lado direito
    alfa_y = -alfa_y;
    alfa_x = -alfa_x;

    for (int i = 0; i < stacks-2; i++){

        point_array[index++] = make_tuple(pivot_x, pivot_y, pivot_z);

        //pivot_x = pivot_x;
        pivot_y = pivot_y*cos(alfa_x)-pivot_z*sin(alfa_x);
        pivot_z = pivot_y*sin(alfa_x)-pivot_z*cos(alfa_x);

        point_array[index++] = make_tuple(pivot_x, pivot_y, pivot_y);

        point_array[index++] = make_tuple(
                pivot_x*cos(alfa_y) + pivot_z*sin(alfa_y),
                pivot_y,
                -pivot_x*sin(alfa_y) + pivot_z*cos(alfa_y)
                );
    }

    //a esfera é constuida rodando a slice criada slices vezes

    alfa_y = -alfa_y;

    for (int i = 0; i < slices; i++) {

        for (int j = 0; j < 6*(stacks-1); j++) {

            point_array[index++] = make_tuple(
                    get<0>(point_array[j])*cos(alfa_y) + get<2>(point_array[j])*sin(alfa_y),
                    get<1>(point_array[j]),
                    -get<0>(point_array[j])*sin(alfa_y) + get<2>(point_array[j])*cos(alfa_y)
                    );
            
        }
        
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
