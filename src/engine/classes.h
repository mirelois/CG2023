#include <tuple>
#include <vector>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

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


class Transformation{
public:
    void virtual transform() = 0;
};

class Translate: public Transformation{
private:
float arguments[3];
public:
void setArgOne(float x){
    arguments[0] = x;
}

void setArgTwo(float y){
    arguments[1] = y;
}

void setArgThree(float z){
    arguments[2] = z;
}

void transform() override{
    glTranslatef(arguments[0], arguments[1], arguments[2]);
}
};

class Rotate: public Transformation{
private:
float arguments[4];
public:
void setArgOne(float alpha){
    arguments[0] = alpha;
}

void setArgTwo(float x){
    arguments[1] = x;
}

void setArgThree(float y){
    arguments[2] = y;
}

void setArgFour(float z){
    arguments[3] = z;
}

void transform() override{
    glRotatef(arguments[0], arguments[1], arguments[2], arguments[3]);
}
};

class Scale: public Transformation{
private:
float arguments[3];
public:
void setArgOne(float x){
    arguments[0] = x;
}

void setArgTwo(float y){
    arguments[1] = y;
}

void setArgThree(float z){
    arguments[2] = z;
}

void transform() override{
    glScalef(arguments[0], arguments[1], arguments[2]);
}
};

class Group{
public:
    std::vector<Model*> models;


    std::vector<Transformation*> transformations;

    //Group *group = 0;
    std::vector<Group*> subGroups;
};