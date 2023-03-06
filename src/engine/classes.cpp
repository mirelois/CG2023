

class Camera{
public:
/* Opção 1
    float position[3];
    float lookAt[3];
    float up[3] = {0,1,0}; // Valor Padrão segundo o ficheiro de exemplo
    float projection[3] = {60,1,1000}; // Valor Padrão segundo o ficheiro de exemplo
*/
/*Opção 2  prefiro esta */
    float options[12];
};

class Window{
public:
    // Valores por Omissão
    float width = 512;
    float height = 512; 
};


class Model{
public:
    std::tuple<float, float, float>* figure;
};


class Group{
public:
    std::vector<Model*> models;
    // Futuramente: Transformations
    Group *group = NULL; // Por agora não há aninhamento de grupos
};