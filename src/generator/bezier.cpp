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
            patch->push_back(atoi(number.c_str()));
        }

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
            points.push_back(atof(number.c_str()));
        }

        cpoints->push_back(points);
    }

}


void multMatrixVector(float *m, float *v, float *res) {
	for (int j = 0; j < 4; ++j) {
		res[j] = 0;
		for (int k = 0; k < 4; ++k) {
			res[j] += v[k] * m[j * 4 + k];
		}
	}
}

tuple<float,float,float> calculate_square(float u, float v, vector<int>* patch, vector<vector<float>>* cpoints){
    float M[4][4] = { // Matriz de Bezier
    {-1, 3, -3, 1},
    {3, -6, 3, 0},
    {-3, 3, 0, 0},
    {1, 0, 0, 0}
    }; 

    float points[3];
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

    return make_tuple(points[0], points[1], points[2]);
}

vector<tuple<float,float,float>>* generate_bezier(char *file_name, int tessellation_level){
    vector<tuple<float,float,float>>* point_vector = new vector<tuple<float,float,float>>();

    vector<vector<int>*>* patches = new vector<vector<int>*>();
    vector<vector<float>>* cpoints = new vector<vector<float>>();

    parse_bezier(file_name, patches, cpoints);
    
    for(vector<int>* patch: *patches){
        for(float u=0; u<tessellation_level; u++){
            for(float v=0; v<tessellation_level; v++){
                point_vector->push_back(calculate_square(u/tessellation_level,v/tessellation_level, patch, cpoints));
                point_vector->push_back(calculate_square(u/tessellation_level,(v+1)/tessellation_level, patch, cpoints));
                point_vector->push_back(calculate_square((u+1)/tessellation_level,(v+1)/tessellation_level, patch, cpoints));

                point_vector->push_back(calculate_square((u+1)/tessellation_level,(v+1)/tessellation_level, patch, cpoints));
                point_vector->push_back(calculate_square((u+1)/tessellation_level,v/tessellation_level, patch, cpoints));
                point_vector->push_back(calculate_square(u/tessellation_level,v/tessellation_level, patch, cpoints));
                //point_vector->push_back(calculate_square((u+1)/tessellation_level*1.0f,v/tessellation_level*1.0f, patch, cpoints));
                //point_vector->push_back(calculate_square((u+1)/tessellation_level*1.0f,(v+1)/tessellation_level*1.0f, patch, cpoints));
                //point_vector->push_back(calculate_square(u/tessellation_level*1.0f,(v+1)/tessellation_level*1.0f, patch, cpoints));
            }
        }
    }

    for(tuple<float,float,float> tuple: *point_vector){
        printf("%f %f %f\n", get<0>(tuple),get<1>(tuple),get<2>(tuple));
    }

    return point_vector;
}
