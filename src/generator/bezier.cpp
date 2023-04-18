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

void matrixMultVector(float m[4][4], float v[4][1], float result[4][1]){ // Matrizes 4*4 e vetores 4
    for(int i=0; i<4; i++){
        result[i][0] = 0;
        for(int j=0; j<4; j++){
            result[i][0] += v[i][0]*m[i][j];
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
        printf("patch\n");
            float point[3];
            for(int i=0; i<=tessellationLevel; i++){
                for(int j=0; j<=tessellationLevel; j++){
                    for(int p = 0; p<3; p++){
                    float MV[4][1]; // M^T = M no caso de Bezier
                    float v[4][1] = {{j*j*j*1.0f/(tessellationLevel*tessellationLevel*tessellationLevel)},
                     {j*j*1.0f/(tessellationLevel*tessellationLevel)},
                     {j*1.0f/tessellationLevel},
                      {1.0f}};

                    matrixMultVector(MBezier, v, MV);
                    
                    float P[4][4] = {{cpoints->at(patch->at(0))[p], cpoints->at(patch->at(1))[p], cpoints->at(patch->at(2))[p], cpoints->at(patch->at(3))[p]},
                                    {cpoints->at(patch->at(4))[p], cpoints->at(patch->at(5))[p], cpoints->at(patch->at(6))[p], cpoints->at(patch->at(7))[p]},
                                    {cpoints->at(patch->at(8))[p], cpoints->at(patch->at(9))[p], cpoints->at(patch->at(10))[p], cpoints->at(patch->at(11))[p]},
                                    {cpoints->at(patch->at(12))[p], cpoints->at(patch->at(13))[p], cpoints->at(patch->at(14))[p], cpoints->at(patch->at(15))[p]}
                    };

                    float PMV[4][1];
                    matrixMultVector(P, MV, PMV);

                    //printf("%f %f %f \n", PMV[0], PMV[1], PMV[2]);

                    float MPMV[4][1];
                    matrixMultVector(MBezier, PMV, MPMV);

                    //printf("%f %f %f \n", MPMV[0], MPMV[1], MPMV[2]);

                    point[p] = (i*i*i)/(tessellationLevel*tessellationLevel*tessellationLevel)*MPMV[0][0] + (i*i)/(tessellationLevel*tessellationLevel)*MPMV[1][0] + i/tessellationLevel*MPMV[2][0] + MPMV[3][0];
                    }
                point_vector->push_back(make_tuple(point[0], point[1], point[2]));
                printf("%f %f %f \n", point[0], point[1], point[2]);
                }
            
        } 
    }
}

vector<tuple<float,float,float>>* generate_bezier(char *fileName, int tessellationLevel){
    vector<tuple<float,float,float>>* point_vector = new vector<tuple<float,float,float>>();

    vector<vector<int>*>* patches = new vector<vector<int>*>();
    vector<vector<float>>* cpoints = new vector<vector<float>>();

    parse_bezier(fileName, patches, cpoints);
    calculate_points(point_vector, patches, cpoints, tessellationLevel);
    
    //for(tuple<float,float,float> tuplo: *point_vector){
    //    printf("%f %f %f\n", get<0>(tuplo),get<1>(tuplo),get<2>(tuplo));
    //}
    
    return point_vector;
}