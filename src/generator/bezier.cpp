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

void matrixMultVector(float m[4][4], float v[4], float result[4]){ // Matrizes 4*4 e vetores 4
    for(int i=0; i<4; i++){
        result[i] = v[i];
        for(int j=0; j<4; j++){
            result[i] *= m[i][j]; 
        }
    }
}

void calculate_points(vector<tuple<float,float,float>>* point_vector, vector<vector<int>*>* patches, vector<vector<float>>* cpoints, int tessellationLevel){
    float MBezier[4][4] = {
    {-1, 3, -3, 1},
    {3, -6, 3, 0},
    {-3, 3, 0, 0},
    {1, 0, 0, 0}
    }; 
    
    for(vector<int>* patch: *patches){
        for(int p = 0; p<4; p++){
            float point[3];
            for(int i=0; i<tessellationLevel; i++){
                for(int j=0; j<tessellationLevel; j++){
                    
                    float MV[4]; // M^T = M no caso de Bezier
                    float v[4] = {j*j*j*1.0f, j*j*1.0f, j*1.0f, 1.0f};
                    
                    matrixMultVector(MBezier, v, MV);
                    
                    float P[4][4] = {{cpoints->at(patch->at(0))[p], cpoints->at(patch->at(1))[p], cpoints->at(patch->at(2))[p], cpoints->at(patch->at(3))[p]},
                                    {cpoints->at(patch->at(4))[p], cpoints->at(patch->at(5))[p], cpoints->at(patch->at(6))[p], cpoints->at(patch->at(7))[p]},
                                    {cpoints->at(patch->at(8))[p], cpoints->at(patch->at(9))[p], cpoints->at(patch->at(10))[p], cpoints->at(patch->at(11))[p]},
                                    {cpoints->at(patch->at(12))[p], cpoints->at(patch->at(13))[p], cpoints->at(patch->at(14))[p], cpoints->at(patch->at(15))[p]}
                    };

                    float PMV[4];
                    matrixMultVector(P, MV, PMV);

                    float MPMV[4];
                    matrixMultVector(MBezier, PMV, MPMV);

                    point[p] = i*i*i*MPMV[0] + i*i*MPMV[1] + i*MPMV[2] + MPMV[3];

                }
            }
            point_vector->push_back(make_tuple(point[0], point[1], point[2]));
        } 
    }
}

vector<tuple<float,float,float>>* generate_bezier(char *fileName, int tessellationLevel){
    vector<tuple<float,float,float>>* point_vector = new vector<tuple<float,float,float>>();

    vector<vector<int>*>* patches = new vector<vector<int>*>();
    vector<vector<float>>* cpoints = new vector<vector<float>>();

    parse_bezier(fileName, patches, cpoints);
    calculate_points(point_vector, patches, cpoints, tessellationLevel);
    
    for(tuple<float,float,float> tuplo: *point_vector){
        printf("%f %f %f\n", get<0>(tuplo),get<1>(tuplo),get<2>(tuplo));
    }
    
    return point_vector;
}