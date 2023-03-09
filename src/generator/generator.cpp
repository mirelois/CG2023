#include <cstdlib>
#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <string.h>
#include <tuple>
#include <fstream>
#include <vector>
#include "matrix.h"

using namespace std;

tuple<float,float,float>* generate_cone(float bottom_radius, float height, int slices, int stacks, int* total_points){
    
    vector<tuple<float,float, float>> point_array;
    int i, j;
	double alfa = 2 * M_PI / slices;
	double division_height_step = height / stacks;
	double division_radius_step = bottom_radius / stacks;
	// Parte de baixo
	for (i = 0; i < slices; i++) {
		point_array.push_back(make_tuple(bottom_radius * sin(alfa * (i + 1)), 0.0f, bottom_radius * cos(alfa * (i + 1))));
        point_array.push_back(make_tuple(bottom_radius * sin(alfa * i), 0.0f, bottom_radius * cos(alfa * i)));
		point_array.push_back(make_tuple(0.0f, 0.0f, 0.0f));
        
	}

	for (i = 0; i < stacks; i++) {
		for (j = 0; j < slices; j++) {
			double bot_height = division_height_step * i;
			double top_height = division_height_step * (i + 1);
			double bot_radius = bottom_radius - division_radius_step * i;
			double top_radius = bottom_radius - division_radius_step * (i + 1);

			// lados
			point_array.push_back(make_tuple(bot_radius * sin(alfa * j), bot_height, bot_radius * cos(alfa * j)));
			point_array.push_back(make_tuple(bot_radius * sin(alfa * (j + 1)), bot_height, bot_radius * cos(alfa * (j + 1))));
			point_array.push_back(make_tuple(top_radius * sin(alfa * j), top_height, top_radius * cos(alfa * j)));

			point_array.push_back(make_tuple(top_radius * sin(alfa * j), top_height, top_radius * cos(alfa * j)));
			point_array.push_back(make_tuple(bot_radius * sin(alfa * (j + 1)), bot_height, bot_radius * cos(alfa * (j + 1))));
			point_array.push_back(make_tuple(top_radius * sin(alfa * (j + 1)), top_height, top_radius * cos(alfa * (j + 1))));
		}
	}

    *total_points = point_array.size();

    tuple<float,float,float>* temp = (tuple<float,float,float>*) malloc(sizeof(tuple<float,float,float>) * point_array.size());
    for(int i=0; i< point_array.size(); i++){
        temp[i] = point_array[i];
    }
    return temp;
}


tuple<float,float,float>* generate_box(float length,  int grid_slices, int* points_total)
{
    //a quantidade de pontos ´e definida pelo grid;
    //grid**2 * 2 * 3 * 6 (nr quadrados * nr triangulos * nr pontos no triangulo * nr faces)    
    
    *points_total = grid_slices*grid_slices*36;
    tuple <float, float, float>* point_array = new tuple <float, float, float>[*points_total];

    float delta = length/grid_slices;

    int index = 0;

    float referential_x = -length/2;

    float referential_y = length/2;

    float referential_z = length/2;

    for(int i = 0; i < grid_slices; i++){
        for (int j = 0; j < grid_slices; j++)
        {
            point_array[index++] = make_tuple(j*delta+referential_x, -i*delta+referential_y, referential_z);
            point_array[index++] = make_tuple(j*delta+referential_x, -(i+1)*delta+referential_y, referential_z);
            point_array[index++] = make_tuple((j+1)*delta+referential_x, -(i+1)*delta+referential_y, referential_z);

            point_array[index++] = make_tuple(j*delta+referential_x, -i*delta+referential_y, referential_z);
            point_array[index++] = make_tuple((j+1)*delta+referential_x, -(i+1)*delta+referential_y, referential_z);
            point_array[index++] = make_tuple((j+1)*delta+referential_x, -i*delta+referential_y, referential_z);
        }
    }

    for (int i = 0; i < *points_total/6; i++) {
        //back face
        point_array[index++] = make_tuple(-get<0>(point_array[i]), get<1>(point_array[i]), -get<2>(point_array[i]));
    }

    for(int i=0;  i < *points_total/3; i++)
    {
        point_array[index++] = make_tuple(get<2>(point_array[i]), get<1>(point_array[i]), -get<0>(point_array[i]));
    }
    for(int i=0; i < *points_total/3; i++)
    {
        point_array[index++] = make_tuple(get<0>(point_array[i]), -get<2>(point_array[i]), get<1>(point_array[i]));
    }
    
    return point_array;
}


tuple <float, float, float>* generate_plane(float length, int grid_slices, int* points_total){

    *points_total = grid_slices*grid_slices*6;

    tuple <float, float, float>* point_array = new tuple <float, float, float>[*points_total];

    float delta = length/grid_slices;

    int index = 0;

    float referential_x = -length/2;

    float referential_z = -length/2;

    for(int i = 0; i < grid_slices; i++){
        for (int j = 0; j < grid_slices; j++)
        {
            point_array[index++] = make_tuple(j*delta+referential_x, 0, i*delta+referential_z);
            point_array[index++] = make_tuple(j*delta+referential_x, 0, (i+1)*delta+referential_z);
            point_array[index++] = make_tuple((j+1)*delta+referential_x, 0, (i+1)*delta+referential_z);

            point_array[index++] = make_tuple(j*delta+referential_x, 0, i*delta+referential_z);
            point_array[index++] = make_tuple((j+1)*delta+referential_x, 0, (i+1)*delta+referential_z);
            point_array[index++] = make_tuple((j+1)*delta+referential_x, 0, i*delta+referential_z);
        }
    }

    return point_array;
}


tuple <float, float, float>* generate_sphere(float radius, int slices, int stacks, int* points_total) {

    *points_total = slices*6*(stacks-1);
    std::tuple <float, float, float>* point_array = new tuple<float,float,float>[*points_total];
    int index = 0; 

	float alfa_x = 0;
	float delta_x = M_PI / stacks;

	float alfa_y = 0;
	float delta_y = 2 * M_PI / slices;

	float pivot_x = 0;
	float pivot_y = radius;
	float pivot_z = 0;
	
	std::tuple<float, float, float>* master_line = new std::tuple<float,float,float>[stacks+1];


	//generate master line
	int master_line_index = 0;
	for (int i = 0; i < stacks + 1; i++) {

		master_line[master_line_index++] = std::make_tuple(
			pivot_x,
			pivot_y * cos(i*alfa_x) - pivot_z * sin(i*alfa_x),
			pivot_y * sin(i*alfa_x) + pivot_z * cos(i*alfa_x)
		);
	}

	for (int j = 0; j < slices; j++) {
		for (int i = 0; i < stacks - 1; i++) {

			point_array[index++] = std::make_tuple(
				std::get<0>(master_line[i]) * cos(j*alfa_y) + std::get<2>(master_line[i]) * sin(j*alfa_y),
				get<1>(master_line[i]),
				-get<0>(master_line[i]) * sin(j*alfa_y) + get<2>(master_line[i]) * cos(j*alfa_y)
			);

			point_array[index++] = make_tuple(
				get<0>(master_line[i + 1]) * cos(j*alfa_y) + get<2>(master_line[i + 1]) * sin(j*alfa_y),
				get<1>(master_line[i + 1]),
				-get<0>(master_line[i + 1]) * sin(j*alfa_y) + get<2>(master_line[i + 1]) * cos(j*alfa_y)
			);

			point_array[index++] = make_tuple(
				get<0>(master_line[i + 1]) * cos((j+1)*alfa_y) + get<2>(master_line[i + 1]) * sin((j+1)*alfa_y),
				get<1>(master_line[i + 1]),
				-get<0>(master_line[i + 1]) * sin((j+1)*alfa_y) + get<2>(master_line[i + 1]) * cos((j+1)*alfa_y)
			);
		}
	}

	for (int j = 0; j < slices; j++) {
		for (int i = stacks ; i > 1; i--) {

			point_array[index++] = make_tuple(
				std::get<0>(master_line[i]) * cos(-j*alfa_y) + get<2>(master_line[i]) * sin(-j*alfa_y),
				get<1>(master_line[i]),
				-get<0>(master_line[i]) * sin(-j*alfa_y) + get<2>(master_line[i]) * cos(-j*alfa_y)
			);

			point_array[index++] = make_tuple(
				get<0>(master_line[i - 1]) * cos(-j*alfa_y) + get<2>(master_line[i - 1]) * sin(-j*alfa_y),
				get<1>(master_line[i - 1]),
				-get<0>(master_line[i - 1]) * sin(-j*alfa_y) + get<2>(master_line[i - 1]) * cos(-j*alfa_y)
			);

			point_array[index++] = make_tuple(
				get<0>(master_line[i - 1]) * cos(-(j+1)*alfa_y) + get<2>(master_line[i - 1]) * sin(-(j+1)*alfa_y),
				get<1>(master_line[i - 1]),
				-get<0>(master_line[i - 1]) * sin(-(j+1)*alfa_y) + get<2>(master_line[i - 1]) * cos(-(j+1)*alfa_y)
			);
		}
		alfa_y += delta_y;
	}

    for(int i=0; i < *points_total; i++){
            printf("dentro %d: %f %f %f\n", i, get<0>(point_array[i]), get<1>(point_array[i]), get<2>(point_array[i]));
    }

    return point_array;
}

tuple<float,float,float>* generate_sphere2(float radius, int slices, int stacks, int *points_total){
    *points_total = slices*6*(stacks-1);
    float alfa_x = M_PI/stacks;

    float alfa_y = 2*M_PI/slices;

    float pivot_x = 0;
    float pivot_y = radius;
    float pivot_z = 0;

    tuple<float, float, float>* master_line = new tuple<float,float,float>[stacks+1];
    tuple<float,float,float>* points_array = new tuple<float,float,float>[*points_total];

    //generate master line
    int master_line_index = 0;
    int i;
    for (i = 0; i < stacks+1; i++) {

        master_line[master_line_index++] = make_tuple(
                pivot_x,
                pivot_y*cos(i*alfa_x) - pivot_z*sin(i*alfa_x),
                pivot_y*sin(i*alfa_x) + pivot_z*cos(i*alfa_x)
                );
    }
    int index = 0;
    for (int j = 0; j < slices; j++) {
        for (int i = 0; i < stacks-1; i++) {
            
            points_array[index++] = make_tuple(
                    get<0>(master_line[i])*cos(j*alfa_y) + get<2>(master_line[i])*sin(j*alfa_y),
                    get<1>(master_line[i]),
                    -get<0>(master_line[i])*sin(j*alfa_y) + get<2>(master_line[i])*cos(j*alfa_y)
                    );

            points_array[index++] = make_tuple(
                    get<0>(master_line[i+1])*cos(j*alfa_y) + get<2>(master_line[i+1])*sin(j*alfa_y),
                    get<1>(master_line[i+1]),
                    -get<0>(master_line[i+1])*sin(j*alfa_y) + get<2>(master_line[i+1])*cos(j*alfa_y)
                    );

            points_array[index++] = make_tuple(
                    get<0>(master_line[i+1])*cos((j+1)*alfa_y) + get<2>(master_line[i+1])*sin((j+1)*alfa_y),
                    get<1>(master_line[i+1]),
                    -get<0>(master_line[i+1])*sin((j+1)*alfa_y) + get<2>(master_line[i+1])*cos((j+1)*alfa_y)
                    );
        }
    }

    for (int j = 0; j < slices; j++) {
        for (int i = stacks; i > 1; i--) {
            
            points_array[index++] = make_tuple(
                    get<0>(master_line[i])*cos(-j*alfa_y) + get<2>(master_line[i])*sin(-j*alfa_y),
                    get<1>(master_line[i]),
                    -get<0>(master_line[i])*sin(-j*alfa_y) + get<2>(master_line[i])*cos(-j*alfa_y)
                    );

            points_array[index++] = make_tuple(
                    get<0>(master_line[i-1])*cos(-j*alfa_y) + get<2>(master_line[i-1])*sin(-j*alfa_y),
                    get<1>(master_line[i-1]),
                    -get<0>(master_line[i-1])*sin(-j*alfa_y) + get<2>(master_line[i-1])*cos(-j*alfa_y)
                    );

            points_array[index++] = make_tuple(
                    get<0>(master_line[i-1])*cos(-(j+1)*alfa_y) + get<2>(master_line[i-1])*sin(-(j+1)*alfa_y),
                    get<1>(master_line[i-1]),
                    -get<0>(master_line[i-1])*sin(-(j+1)*alfa_y) + get<2>(master_line[i-1])*cos(-(j+1)*alfa_y)
                    );
        }
    }

    return points_array;
}



//i mean fine mas estas a fazer isto muito a C, concordo - assinado carlos
void points_write (const char *filename, int nVertices, tuple<float,float,float> points[]) 
{
  //FILE *fp = fopen (filename, "w");
  //if (!fp)
    //{
      //fprintf (stderr, "failed to open file: %s", filename);
      //exit (1);
    //}

  //fwrite (&nVertices, sizeof (unsigned int), 1, fp);
  //fwrite (points, sizeof(tuple<float,float,float>), nVertices, fp);

    fstream file;
    file.open(filename,ios::out|ios::binary|ios::in|ios::trunc);
    //printf("%d\n", nVertices);
    file.write((char*)&nVertices, sizeof(int));
    //file.read((char*)&n,sizeof(int));
    file.write((char*)points, sizeof(tuple<float,float,float>)*nVertices);

    file.close();
    /*
    fstream f;
    f.open(filename,ios::binary|ios::in|ios::out);
    f.seekg(sizeof(int));
    tuple<float,float,float>* t = new tuple<float,float,float>[nVertices];
    f.read((char*)t, sizeof(tuple<float,float,float>)*nVertices);
    printf("\n");
    for(int i=0; i<nVertices; i++){
        printf("segundo%d: %f %f %f\n", i, get<0>(t[i]),get<1>(t[i]),get<2>(t[i]));
    }
    file.close();
    */
    /*
    FILE *test = fopen("test.txt", "w");
    if (!test) {
        fprintf (stderr, "failed to open file: %s", filename);
        exit (1);
    }

    for (int i = 0; i< nVertices; i++) {
        char s[128];
        sprintf(s, "%d: %f %f %f\n", i, get<0>(points[i]), get<1>(points[i]), get<2>(points[i]));
        fwrite(s,strlen(s),1,test);
    }
    */

  //fclose (fp);
}

int main(int argc, char* argv[]){
    if(!strcmp(argv[1], "sphere")){
        int points_total;
        tuple<float,float,float>* sphere = generate_sphere2(atof(argv[2]), atoi(argv[3]), atoi(argv[4]), &points_total);
        points_write(argv[5], points_total, sphere);
        delete(sphere);
    } else if(!strcmp(argv[1], "box")){
        int points_total;
        tuple<float,float,float>* box = generate_box(atof(argv[2]), atoi(argv[3]), &points_total);
        points_write(argv[4], points_total, box);
        delete(box);
    } else if(!strcmp(argv[1], "plane")){
        int points_total;
        tuple<float,float,float>* plane = generate_plane(atof(argv[2]), atoi(argv[3]), &points_total);
        points_write(argv[4], points_total, plane);
        delete(plane);
    } else if(!strcmp(argv[1], "cone")){
        int points_total;
        tuple<float,float,float>* cone = generate_cone(atof(argv[2]), atof(argv[3]),atoi(argv[4]),atoi(argv[5]), &points_total);
        points_write(argv[6], points_total, cone);
        free(cone);
    }

    return 0;
}
