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
    void setArgOne(float x);
    void setArgTwo(float y);
    void setArgThree(float z);
    void transform() override;
};

//desta forma perde-se precisão se o tempo de espera for muito grande
class Rotate: public Transformation{
private:
float arguments[4];
public:
    void setArgOne(float alpha);
    void virtual setArgTwo(float x);
    void virtual setArgThree(float y);
    void virtual setArgFour(float z);
    void transform() override;
};

class Rotate_Time : public Rotate {
private:
    float arguments[3];
    unsigned int time;
public:
    Rotate_Time(unsigned int t) {
        time = t;
    }
    void setTime(unsigned int t);
    void setArgTwo(float x) override;
    void setArgThree(float y) override;
    void setArgFour(float z) override;
    void transform() override;
};

class Scale: public Transformation{
private:
float arguments[3];
public:
    void setArgOne(float x);
    void setArgTwo(float y);
    void setArgThree(float z);
    void transform() override;
};

class Group{
public:
    std::vector<Model*> models;
    std::vector<Transformation*> transformations;
    //Group *group = 0;
    std::vector<Group*> subGroups;
};