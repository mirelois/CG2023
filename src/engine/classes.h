#define _USE_MATH_DEFINES
#include <tuple>
#include <vector>
#include <math.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
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
    unsigned int size;
    unsigned int index = 0;
};


class Transformation{
public:
    void virtual transform() = 0;
};

class Translate_Catmull: public Transformation {
public:
    std::vector<float*> points;
    float time;
    float x[3] = {0.0f,0.0f,0.0f};
    void multMatrixVector(float* m, float* v, float* res);
    void getCatmullRomPoint(float t, float* p0, float* p1, float* p2, float* p3, float* pos, float* deriv);
    // given  global t, returns the point in the curve
    void getGlobalCatmullRomPoint(float gt, float* pos, float* deriv);
    void setTime(float t);
    void addPoint(float p[3]);
    void transform() override;  
};

class Translate_Catmull_Align : public virtual Translate_Catmull {
public:
    float y[3] = { 0,1,0 };
    void buildRotMatrix(float* x, float* y, float* z, float* m);
    void cross(float* a, float* b, float* res);
    void normalize(float* a);
    void align();
    void transform() override;
};

class Translate_Catmull_Curve : public virtual Translate_Catmull {
public:
    void curve();
    void transform() override;
};

class Translate_Catmull_Curve_Align : public Translate_Catmull_Align, public Translate_Catmull_Curve {
public:
    void transform() override;
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

//desta forma perde-se precisão se o tempo de espera for muito grande
class Rotate: public Transformation{
private:
float arguments[4];
public:
void setArgOne(float alpha){
    arguments[0] = alpha;
}

void virtual setArgTwo(float x){
    arguments[1] = x;
}

void virtual setArgThree(float y){
    arguments[2] = y;
}

void virtual setArgFour(float z){
    arguments[3] = z;
}

void transform() override{
    glRotatef(arguments[0], arguments[1], arguments[2], arguments[3]);
}
};

class Rotate_Time : public Rotate {
private:
    float arguments[3];
    unsigned int time;
public:
    Rotate_Time(unsigned int t) {
        time = t;
    }
    void setTime(unsigned int t) {
        time = t;
    }

    void setArgTwo(float x) override{
        arguments[0] = x;
    }

    void setArgThree(float y) override {
        arguments[1] = y;
    }

    void setArgFour(float z) override {
        arguments[2] = z;
    }

    void transform() override {
        //Conseguir um valor que pertença a [0,1] com base no resto do tempo passado desde o último múltiplo de 
        float timePassed = remainder(glutGet(GLUT_ELAPSED_TIME) / 1000.0f, time);
        timePassed = timePassed < 0 ? (timePassed + time) / time : timePassed / time;
        glRotatef(360.0f * timePassed, arguments[0], arguments[1], arguments[2]);
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