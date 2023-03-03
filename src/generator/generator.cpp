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
    float referential_y = -length/2;

    float referential_z = length/2;

    for(int i; i < grid_slices; i++){
        for (int j; j < grid_slices; j++)
        {
            point_array[++index] = make_tuple(referential_x, referential_y, referential_z);
            point_array[++index] = make_tuple(referential_x, referential_y-delta, referential_z);
            point_array[++index] = make_tuple(referential_x+delta, referential_y, referential_z);

            point_array[++index] = make_tuple(referential_x, referential_y-delta, referential_z);
            point_array[++index] = make_tuple(referential_x+delta, referential_y-delta, referential_z);
            point_array[++index] = make_tuple(referential_x+delta, referential_y, referential_z);
            //(x,y),(x,y+delta),(x+delta,y)
            //(x,y+delta),(x+delta,y+delta),(x+delta,y)
            point_array[++index] = make_tuple(referential_x, referential_y, -referential_z);
            point_array[++index] = make_tuple(referential_x, referential_y+delta, -referential_z);
            point_array[++index] = make_tuple(referential_x+delta, referential_y, -referential_z);

            point_array[++index] = make_tuple(referential_x, referential_y+delta, -referential_z);
            point_array[++index] = make_tuple(referential_x+delta, referential_y+delta, -referential_z);
            point_array[++index] = make_tuple(referential_x+delta, referential_y, -referential_z);

            referential_x += delta;
             
        }
        referential_y += delta;
    }

    referential_x = length/2;

    for(int i; i < grid_slices; i++){
        for (int j; j < grid_slices; j++)
        {
            point_array[++index] = make_tuple(referential_x, referential_y, referential_z);
            point_array[++index] = make_tuple(referential_x, referential_y+delta, referential_z);
            point_array[++index] = make_tuple(referential_x+delta, referential_y, referential_z);

            point_array[++index] = make_tuple(referential_x, referential_y+delta, referential_z);
            point_array[++index] = make_tuple(referential_x+delta, referential_y+delta, referential_z);
            point_array[++index] = make_tuple(referential_x+delta, referential_y, referential_z);
            //(x,y),(x,y+delta),(x+delta,y)
            //(x,y+delta),(x+delta,y+delta),(x+delta,y)
            point_array[++index] = make_tuple(referential_x, referential_y, -referential_z);
            point_array[++index] = make_tuple(referential_x, referential_y+delta, -referential_z);
            point_array[++index] = make_tuple(referential_x+delta, referential_y, -referential_z);

            point_array[++index] = make_tuple(referential_x, referential_y+delta, -referential_z);
            point_array[++index] = make_tuple(referential_x+delta, referential_y+delta, -referential_z);
            point_array[++index] = make_tuple(referential_x+delta, referential_y, -referential_z);

            referential_x += delta;
             
        }
        referential_y += delta;
    }

}
*/



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
            point_array[++index] = make_tuple(referential_x, referential_y, referential_z);
            point_array[++index] = make_tuple(referential_x, referential_y-delta, referential_z);
            point_array[++index] = make_tuple(referential_x+delta, referential_y, referential_z);

            point_array[++index] = make_tuple(referential_x, referential_y-delta, referential_z);
            point_array[++index] = make_tuple(referential_x+delta, referential_y-delta, referential_z);
            point_array[++index] = make_tuple(referential_x+delta, referential_y, referential_z);
            referential_x += delta;
        }
        referential_y += delta;
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
