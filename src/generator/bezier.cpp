#include "bezier.h"

void parse_bezier(char *fileName, vector<vector<int>*>* patches, vector<vector<float>>* cpoints){
    ifstream file;
    file.open(fileName, ios::in);

    int nPatches, nControlPoints;
    string number;
    
    file >> nPatches;

    getline(file,number,'\n');
    for(int i=0; i<nPatches; i++){
        vector<int>* patch = new vector<int>();
        string line;
        getline(file, line, '\n');
        stringstream lineS (line);
        while(getline(lineS, number, ',')){
            //printf("%s ", number.c_str());
            patch->push_back(atoi(number.c_str()));
        }
        //putchar('\n');
        patches->push_back(patch);
    }
    
    file >> nControlPoints;
    getline(file,number,'\n');
    for(int i=0; i<nControlPoints; i++){
        string line;
        getline(file, line, '\n');
        stringstream lineS (line);
        
        vector<float> points;
        while(getline(lineS, number, ',')){
            //printf("%s ",number.c_str());
            points.push_back(atof(number.c_str()));
        }
        //putchar('\n');
        cpoints->push_back(points);
    }

    
    file.close();
}


void multMatrixVector(float *m, float *v, float *res) {
	for (int j = 0; j < 4; ++j) {
		res[j] = 0;
		for (int k = 0; k < 4; ++k) {
			res[j] += v[k] * m[j * 4 + k];
		}
	}
}

void calculate_point(float u, float v, vector<int>* patch, vector<vector<float>>* cpoints, float* points){
    float M[4][4] = { // Matriz de Bezier
    {-1, 3, -3, 1},
    {3, -6, 3, 0},
    {-3, 3, 0, 0},
    {1, 0, 0, 0}
    }; 

    for(int p=0; p<3; p++){
        float V[4] = {v*v*v, v*v, v, 1};
        float MV[4];
        multMatrixVector(&M[0][0], V, MV);

        float PMV[4];
        float P[4][4] = {{cpoints->at(patch->at(0))[p], cpoints->at(patch->at(1))[p], cpoints->at(patch->at(2))[p], cpoints->at(patch->at(3))[p]},
                        {cpoints->at(patch->at(4))[p], cpoints->at(patch->at(5))[p], cpoints->at(patch->at(6))[p], cpoints->at(patch->at(7))[p]},
                        {cpoints->at(patch->at(8))[p], cpoints->at(patch->at(9))[p], cpoints->at(patch->at(10))[p], cpoints->at(patch->at(11))[p]},
                        {cpoints->at(patch->at(12))[p], cpoints->at(patch->at(13))[p], cpoints->at(patch->at(14))[p], cpoints->at(patch->at(15))[p]}
        };

        multMatrixVector(&P[0][0], MV, PMV);

        float MPMV[4];
        multMatrixVector(&M[0][0], PMV, MPMV);

        points[p] = u*u*u*MPMV[0] + u*u*MPMV[1] + u*MPMV[2] + MPMV[3];
    }
}


void calculate_normal(float u, float v, vector<int>* patch, vector<vector<float>>* cpoints, float** M, float* n){
    float du[3];
    int p;

    float P[4][4] = {{cpoints->at(patch->at(0))[p], cpoints->at(patch->at(1))[p], cpoints->at(patch->at(2))[p], cpoints->at(patch->at(3))[p]},
                        {cpoints->at(patch->at(4))[p], cpoints->at(patch->at(5))[p], cpoints->at(patch->at(6))[p], cpoints->at(patch->at(7))[p]},
                        {cpoints->at(patch->at(8))[p], cpoints->at(patch->at(9))[p], cpoints->at(patch->at(10))[p], cpoints->at(patch->at(11))[p]},
                        {cpoints->at(patch->at(12))[p], cpoints->at(patch->at(13))[p], cpoints->at(patch->at(14))[p], cpoints->at(patch->at(15))[p]}
    };

    for(p=0; p<3; p++){
        float V[4] = {v*v*v, v*v, v, 1};
        float MV[4];
        multMatrixVector(&M[0][0], V, MV);

        float PMV[4];

        multMatrixVector(&P[0][0], MV, PMV);

        float MPMV[4];
        multMatrixVector(&M[0][0], PMV, MPMV);

        du[p] = 3*u*u*MPMV[0] + 2*u*MPMV[1] + MPMV[2];
    }

    float dv[3];
    for(p=0; p<3; p++){
        float V[4] = {3*v*v, 2*v, 1, 0};
        float MV[4];
        multMatrixVector(&M[0][0], V, MV);

        float PMV[4];

        multMatrixVector(&P[0][0], MV, PMV);

        float MPMV[4];
        multMatrixVector(&M[0][0], PMV, MPMV);

        dv[p] = u*u*u*MPMV[0] + u*u*MPMV[1] + u*MPMV[2] + MPMV[3];
    }

    // Cross Product dv x du
    n[0] = dv[1]*du[2] - dv[2]*du[1];
    n[1] = dv[2]*du[0] - dv[0]*du[2];
    n[2] = dv[0]*du[1] - dv[1]*du[0];
}

unsigned int interact(map<tuple<float,float,float>, unsigned int>* map, float* points, vector<unsigned int>* indices, unsigned int* ind, vector<float>* point_vector){
    tuple<float, float, float> item = make_tuple(points[0], points[1], points[2]);
    unsigned int ind_Actual;
    if(map->find(item)==map->end()){
        map->insert(make_pair(item, *ind));
        indices->push_back(*ind);
        ind_Actual = *ind;
        (*ind)++;
        point_vector->push_back(points[0]);point_vector->push_back(points[1]);point_vector->push_back(points[2]);
    } else{
        ind_Actual = map->at(item);
        indices->push_back(ind_Actual);
    }

    return ind_Actual;
}

tuple<vector<float>*, vector<unsigned int>*> generate_bezier(char *file_name, float tessellation_level){

    vector<vector<int>*>* patches = new vector<vector<int>*>();
    vector<vector<float>>* cpoints = new vector<vector<float>>();
    vector<unsigned int>* indices = new vector<unsigned int>();
    vector<float>* point_vector = new vector<float>();

    parse_bezier(file_name, patches, cpoints);

    map<tuple<float,float,float>, unsigned int> map;

    unsigned int ind = 0;

    float points[3];
    unsigned int i1, i2;
    for(vector<int>* patch: *patches){
        for(float u=0; u<tessellation_level; u++){
            for(float v=0; v<tessellation_level; v++){
                calculate_point(u/tessellation_level,v/tessellation_level, patch, cpoints, points);
                i1 = interact(&map, points, indices, &ind, point_vector);

                calculate_point(u/tessellation_level,(v+1)/tessellation_level, patch, cpoints, points);
                interact(&map, points, indices, &ind, point_vector);

                calculate_point((u+1)/tessellation_level,(v+1)/tessellation_level, patch, cpoints, points);
                i2 = interact(&map, points, indices, &ind, point_vector);

                indices->push_back(i2);

                calculate_point((u+1)/tessellation_level,v/tessellation_level, patch, cpoints, points);
                interact(&map, points, indices, &ind, point_vector);

                indices->push_back(i1);
            }
        }
    }
    
    return make_tuple(point_vector, indices);
}
