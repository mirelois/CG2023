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

#include <IL/il.h>

class Camera {
public:
    float position[3] = { 1,2,1 }; // Valor por omiss�o sugerido por n�s
    float lookAt[3] = { 0,0,0 }; // valor por omiss�o sugerido por n�s
    float up[3] = { 0,1,0 }; // Valor Padr�o segundo o ficheiro de exemplo
    float projection[3] = { 60,1,1000 }; // Valor Padr�o segundo o ficheiro de exemplo
};

class Light{
public: // Default settings
    GLuint type;
    virtual void drawLight();
};

class Spotlight : Light{
public:
    float point[4] = { 0,10,0,0 };
    float dir[3] = { 1,1,1 };
    float cutoff = 45;
    void drawLight() override;
};

class Point : Light {
public:
    float point[4] = { 0,10,0,1 };
    void drawLight() override;
};

class Directional : Light {
public:
    float point[4] = { 1,1,1,0 };
    void drawLight() override;
};

class Window {
public:
    // Valores por Omiss�o
    float width = 512;
    float height = 512;
};


class Model {
public:
    unsigned int type;
    unsigned int size;
    unsigned int index = 0;
    

    // Texture
    GLuint texID;
    // Color
    float diffuse[4] = {200, 200, 200, 1.0};
    float ambient[4] = {200, 200, 200, 1.0};
    float specular[4] = {200, 200, 200, 1.0};
    float emissive[4] = {200, 200, 200, 1.0};
    GLfloat shininess = 0;
    Model(unsigned int t) { type = t; }
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
class Rotate : public Transformation {
public:
    float arguments[3];
    void setArgOne(float x);
    void setArgTwo(float y);
    void setArgThree(float z);
};

class Rotate_Alpha: public Rotate{
private:
    float alpha;
public:
    Rotate_Alpha(float a) {
        alpha = a;
    }
    void setAlpha(float a);
    void transform() override;
};

class Rotate_Time : public Rotate {
private:
    float time;
public:
    Rotate_Time(float t) {
        time = t;
    }
    void setTime(float t);
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