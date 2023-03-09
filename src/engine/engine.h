#define _USE_MATH_DEFINES
#include <math.h>
#include "parser.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

void run(Window* window, Camera* camera, Group* group, int argc, char* argv[]);