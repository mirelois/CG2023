#include <tuple>
#include <vector>

class Camera {
public:
    float position[3] = { 1,2,1 }; // Valor por omiss�o sugerido por n�s
    float lookAt[3] = { 0,0,0 }; // valor por omiss�o sugerido por n�s
    float up[3] = { 0,1,0 }; // Valor Padr�o segundo o ficheiro de exemplo
    float projection[3] = { 60,1,1000 }; // Valor Padr�o segundo o ficheiro de exemplo
};

class Window {
public:
    // Valores por Omiss�o
    float width = 512;
    float height = 512;
};


class Model {
public:
    int size;
    std::tuple<float, float, float>* figure;
};


class Group {
public:
    std::vector<Model*> models;
    // Futuramente: Transformations
    //Group *group = NULL; // Por agora n�o h� aninhamento de grupos
};