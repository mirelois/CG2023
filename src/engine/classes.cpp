#include <tuple>
#include <vector>
#include "classes.h"

    void Translate_Catmull::buildRotMatrix(float* x, float* y, float* z, float* m) {

        m[0] = x[0]; m[1] = x[1]; m[2] = x[2]; m[3] = 0;
        m[4] = y[0]; m[5] = y[1]; m[6] = y[2]; m[7] = 0;
        m[8] = z[0]; m[9] = z[1]; m[10] = z[2]; m[11] = 0;
        m[12] = 0; m[13] = 0; m[14] = 0; m[15] = 1;
    }


    void Translate_Catmull::cross(float* a, float* b, float* res) {

        res[0] = a[1] * b[2] - a[2] * b[1];
        res[1] = a[2] * b[0] - a[0] * b[2];
        res[2] = a[0] * b[1] - a[1] * b[0];
    }


    void Translate_Catmull::normalize(float* a) {

        float l = sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
        a[0] = a[0] / l;
        a[1] = a[1] / l;
        a[2] = a[2] / l;
    }

    void Translate_Catmull::multMatrixVector(float* m, float* v, float* res) {

        for (int j = 0; j < 4; ++j) {
            res[j] = 0;
            for (int k = 0; k < 4; ++k) {
                res[j] += v[k] * m[j * 4 + k];
            }
        }

    }

    void Translate_Catmull::getCatmullRomPoint(float t, float* p0, float* p1, float* p2, float* p3, float* pos, float* deriv) {

        // catmull-rom matrix
        float m[4][4] = { {-0.5f,  1.5f, -1.5f,  0.5f},
                            { 1.0f, -2.5f,  2.0f, -0.5f},
                            {-0.5f,  0.0f,  0.5f,  0.0f},
                            { 0.0f,  1.0f,  0.0f,  0.0f} };

        for (int i = 0; i < 3; i++) {
            // Compute A = M * P
            float a[4];
            float p[4] = { p0[i], p1[i], p2[i], p3[i] };
            multMatrixVector(&m[0][0], p, a);
            // Compute pos = T * A
            pos[i] = (t * t * t * a[0] + t * t * a[1] + t * a[2] + a[3]);
            // compute deriv = T' * A
            deriv[i] = (3 * t * t * a[0] + 2 * t * a[1] + a[2]);
        }
    }

    // given  global t, returns the point in the curve
    void Translate_Catmull::getGlobalCatmullRomPoint(float gt, float* pos, float* deriv) {

        int POINT_COUNT = points.size();
        float t = gt * POINT_COUNT; // this is the real global t
        int index = floor(t);  // which segment
        t = t - index; // where within  the segment

        // indices store the points
        int indices[4];
        indices[0] = (index + POINT_COUNT - 1) % POINT_COUNT;
        indices[1] = (indices[0] + 1) % POINT_COUNT;
        indices[2] = (indices[1] + 1) % POINT_COUNT;
        indices[3] = (indices[2] + 1) % POINT_COUNT;

        getCatmullRomPoint(t, points[indices[0]], points[indices[1]], points[indices[2]], points[indices[3]], pos, deriv);
    }

    void Translate_Catmull::setTime(float t) {
        time = t;
    }
    void Translate_Catmull::setAlign(bool a) {
        align = a;
    }
    void Translate_Catmull::addPoint(float p[3]) {
        //TODO quero não usar mallocs
        float* t = (float*)malloc(sizeof(float) * 3);
        t[0] = p[0];
        t[1] = p[1];
        t[2] = p[2];
        points.push_back(t);
    }
    void Translate_Catmull::transform() {
        float timePassed = remainder(glutGet(GLUT_ELAPSED_TIME) / 1000.0f, time);
        float pos[3], x[3], z[3];
        timePassed = timePassed < 0 ? (timePassed + time) / time : timePassed / time;
        getGlobalCatmullRomPoint(timePassed, pos, x);
        glTranslatef(pos[0], pos[1], pos[2]);
        if (align) {
            normalize(x);
            cross(x, y, z);
            normalize(z);
            cross(z, x, y);

            float m[16];
            buildRotMatrix(x, y, z, m);
            glMultMatrixf(m);
        }
    }