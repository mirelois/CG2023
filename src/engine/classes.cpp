#include <tuple>
#include <vector>

class Camera{
public:
    float position[3] = {1,2,1}; // Valor por omissão sugerido por nós
    float lookAt[3] = {0,0,0}; // valor por omissão sugerido por nós
    float up[3] = {0,1,0}; // Valor Padrão segundo o ficheiro de exemplo
    float projection[3] = {60,1,1000}; // Valor Padrão segundo o ficheiro de exemplo
};

class Window{
public:
    // Valores por Omissão
    float width = 512;
    float height = 512; 
};


class Model{
public:
    int size;
    std::tuple<float, float, float>* figure;
};


class Group{
public:
    std::vector<Model*> models;


    std::vector<char> transformations;
    float translate[3];
    float rotate[4];
    float scale[3];

    //Group *group = 0;
    std::vector<Group*> subGroups;
};