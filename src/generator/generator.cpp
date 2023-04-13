#include <cstdlib>
#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <string.h>
#include <tuple>
#include <fstream>
#include <vector>

using namespace std;

tuple<float, float, float>* generate_torus(float inner_radius, float outer_radius, int vertical_divisions, int horizontal_divisions, int* points_total){

    *points_total = 6*vertical_divisions*horizontal_divisions;

    tuple<float, float, float>* point_array = new tuple<float, float, float>[*points_total];

    int index = 0;

    
    tuple<float, float, float>* master_circle = new tuple<float, float, float>[vertical_divisions];
    
    int master_circle_index = 0;


    float delta_x = 2*M_PI/vertical_divisions;

    float delta_y = 2*M_PI/horizontal_divisions;

    float pivot_x = 0;

    float pivot_y = (outer_radius-inner_radius)/2;

    float pivot_z = 0;


    for (int i = 0; i < vertical_divisions; i++) {

        master_circle[master_circle_index++] = make_tuple(
                pivot_x,
                pivot_y*cos(i*delta_x),
                pivot_y*sin(i*delta_x) + (outer_radius+inner_radius)/2
                );

    }

    for (int i = 0; i < horizontal_divisions; i++) {

        for (int j = 0; j < vertical_divisions; j++) {

            point_array[index++] = make_tuple(
                    get<0>(master_circle[j])*cos(i*delta_y) + get<2>(master_circle[j])*sin(i*delta_y),
                    get<1>(master_circle[j]),
                    -get<0>(master_circle[j])*sin(i*delta_y) + get<2>(master_circle[j])*cos(i*delta_y)
                    );
            point_array[index++] = make_tuple(
                    get<0>(master_circle[(j+1)%vertical_divisions])*cos(i*delta_y) + get<2>(master_circle[(j+1)%vertical_divisions])*sin(i*delta_y),
                    get<1>(master_circle[(j+1)%vertical_divisions]),
                    -get<0>(master_circle[(j+1)%vertical_divisions])*sin(i*delta_y) + get<2>(master_circle[(j+1)%vertical_divisions])*cos(i*delta_y)
                    );
            point_array[index++] = make_tuple(
                    get<0>(master_circle[(j+1)%vertical_divisions])*cos(((i+1)%horizontal_divisions)*delta_y) + get<2>(master_circle[(j+1)%vertical_divisions])*sin(((i+1)%horizontal_divisions)*delta_y),
                    get<1>(master_circle[(j+1)%vertical_divisions]),
                    -get<0>(master_circle[(j+1)%vertical_divisions])*sin(((i+1)%horizontal_divisions)*delta_y) + get<2>(master_circle[(j+1)%vertical_divisions])*cos(((i+1)%horizontal_divisions)*delta_y)
                    );


            point_array[index++] = make_tuple(
                    get<0>(master_circle[j])*cos(i*delta_y) + get<2>(master_circle[j])*sin(i*delta_y),
                    get<1>(master_circle[j]),
                    -get<0>(master_circle[j])*sin(i*delta_y) + get<2>(master_circle[j])*cos(i*delta_y)
                    );
            point_array[index++] = make_tuple(
                    get<0>(master_circle[(j+1)%vertical_divisions])*cos(((i+1)%horizontal_divisions)*delta_y) + get<2>(master_circle[(j+1)%vertical_divisions])*sin(((i+1)%horizontal_divisions)*delta_y),
                    get<1>(master_circle[(j+1)%vertical_divisions]),
                    -get<0>(master_circle[(j+1)%vertical_divisions])*sin(((i+1)%horizontal_divisions)*delta_y) + get<2>(master_circle[(j+1)%vertical_divisions])*cos(((i+1)%horizontal_divisions)*delta_y)
                    );
            point_array[index++] = make_tuple(
                    get<0>(master_circle[j])*cos(((i+1)%horizontal_divisions)*delta_y) + get<2>(master_circle[j])*sin(((i+1)%horizontal_divisions)*delta_y),
                    get<1>(master_circle[j]),
                    -get<0>(master_circle[j])*sin(((i+1)%horizontal_divisions)*delta_y) + get<2>(master_circle[j])*cos(((i+1)%horizontal_divisions)*delta_y)
                    );

        }
    }
    return point_array;
}


tuple<float,float,float>* generate_cone(float bottom_radius, float height, int slices, int stacks, int* total_points){
    
    vector<tuple<float,float, float>> point_array;
    int i, j;
	double alfa = 2 * M_PI / slices;
	double division_height_step = height / stacks;
	double division_radius_step = bottom_radius / stacks;
	// Base
	for (i = 0; i < slices; i++) {
		point_array.push_back(make_tuple(bottom_radius * sin(alfa * (i + 1)), 0.0f, bottom_radius * cos(alfa * (i + 1))));
        point_array.push_back(make_tuple(bottom_radius * sin(alfa * i), 0.0f, bottom_radius * cos(alfa * i)));
		point_array.push_back(make_tuple(0.0f, 0.0f, 0.0f));
        
	}

	for (i = 0; i < stacks; i++) {
		for (j = 0; j < slices; j++) {
			double bot_height = division_height_step * i;
			double top_height = bot_height + division_height_step;
			double bot_radius = bottom_radius - division_radius_step * i;
			double top_radius = bot_radius - division_radius_step;

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

vector<tuple<float,float, float>>* generate_cylinder(float radius, float height, int slices){
    vector<tuple<float,float, float>>* point_array = new vector<tuple<float,float,float>>;

    float alfa = 2*M_PI/slices;

    for(int i=0; i<slices; i++){
        point_array->push_back(make_tuple(sin(alfa * i),height/2, cos(alfa * i)));
        point_array->push_back(make_tuple(sin(alfa * (i+1)),height/2, cos(alfa * (i+1))));
        point_array->push_back(make_tuple(0.0f,height/2, 0.0f));

        point_array->push_back(make_tuple(0.0f,-height/2, 0.0f));
        point_array->push_back(make_tuple(sin(alfa * (i+1)),-height/2, cos(alfa * (i+1))));
        point_array->push_back(make_tuple(sin(alfa * i),-height/2, cos(alfa * i)));

        point_array->push_back(make_tuple(sin(alfa * i),-height/2, cos(alfa * i)));
        point_array->push_back(make_tuple(sin(alfa * (i+1)),-height/2, cos(alfa * (i+1))));
        point_array->push_back(make_tuple(sin(alfa * i),height/2, cos(alfa * i)));

        point_array->push_back(make_tuple(sin(alfa * i),height/2, cos(alfa * i)));
        point_array->push_back(make_tuple(sin(alfa * (i+1)),-height/2, cos(alfa * (i+1))));
        point_array->push_back(make_tuple(sin(alfa * (i+1)),height/2, cos(alfa * (i+1))));
    }

    return point_array;
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

tuple<float,float,float>* generate_sphere(float radius, int slices, int stacks, int *points_total){
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
                pivot_y*cos(i*alfa_x),
                pivot_y*sin(i*alfa_x)
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
                    get<0>(master_line[i+1])*cos(((j+1)%slices)*alfa_y) + get<2>(master_line[i+1])*sin(((j+1)%slices)*alfa_y),
                    get<1>(master_line[i+1]),
                    -get<0>(master_line[i+1])*sin(((j+1)%slices)*alfa_y) + get<2>(master_line[i+1])*cos(((j+1)%slices)*alfa_y)
                    );
        }
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
                    get<0>(master_line[i-1])*cos(-((j+1)%slices)*alfa_y) + get<2>(master_line[i-1])*sin(-((j+1)%slices)*alfa_y),
                    get<1>(master_line[i-1]),
                    -get<0>(master_line[i-1])*sin(-((j+1)%slices)*alfa_y) + get<2>(master_line[i-1])*cos(-((j+1)%slices)*alfa_y)
                    );
        }
    }

    return points_array;
}


void points_write (const char *filename, int nVertices, tuple<float,float,float> points[]) 
{
    fstream file;
    file.open(filename,ios::out|ios::binary|ios::in|ios::trunc);

    file.write((char*)&nVertices, sizeof(int));
    file.write((char*)points, sizeof(tuple<float,float,float>)*nVertices);

    file.close();
}

int main(int argc, char* argv[]){
    if(!strcmp(argv[1], "sphere")){
        int points_total;
        tuple<float,float,float>* sphere = generate_sphere(atof(argv[2]), atoi(argv[3]), atoi(argv[4]), &points_total);
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
    } else if(!strcmp(argv[1], "torus")){
        int points_total;
        tuple<float,float,float>* torus = generate_torus(atof(argv[2]), atof(argv[3]),atoi(argv[4]),atoi(argv[5]), &points_total);
        points_write(argv[6], points_total, torus);
        free(torus);
    } else if(!strcmp(argv[1], "cylinder")){
        vector<tuple<float,float, float>>* cylinder = generate_cylinder(atof(argv[2]), atof(argv[3]), atoi(argv[4]));
        points_write(argv[5], cylinder->size(), cylinder->data());
        free(cylinder);
    }else{
        printf("Invalid Model\n");
    }

    return 0;
}
