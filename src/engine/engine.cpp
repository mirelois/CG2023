#include "engine.h"

Camera* camera_global;
Group* group_global;

using namespace std;

void drawAxis(){
	glBegin(GL_LINES);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-100.0f, 0.0f, 0.0f);
	glVertex3f( 100.0f, 0.0f, 0.0f);

	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, -100.0f, 0.0f);
	glVertex3f(0.0f, 100.0f, 0.0f);

	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, -100.0f);
	glVertex3f(0.0f, 0.0f, 100.0f);

	glEnd();
}

void draw(){
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_TRIANGLES);
	glColor3f(1.0f,1.0f,1.0f);
	for(int i=0; i<group_global->models.size(); i++){
		for(int j=0; j<group_global->models[i]->size; j++){
			glVertex3f(get<0>(group_global->models[i]->figure[j]), get<1>(group_global->models[i]->figure[j]), get<2>(group_global->models[i]->figure[j]));
			// delete group_global->models[i]->figure[j];
		}
	}
	glEnd();
}

void renderScene(void) {

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(camera_global->position[0], camera_global->position[1], camera_global->position[2],
		camera_global->lookAt[0], camera_global->lookAt[1], camera_global->lookAt[2],
		camera_global->up[0], camera_global->up[1], camera_global->up[2]);

	// Colocar funcoes de desenho aqui
	drawAxis();
	draw();

	// End of frame
	glutSwapBuffers();
}

void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if(h == 0)
		h = 1;

	// compute window's aspect ratio 
	float ratio = w * 1.0 / h;

	// Set the projection matrix as current
	glMatrixMode(GL_PROJECTION);
	// Load Identity Matrix
	glLoadIdentity();
	
	// Set the viewport to be the entire window
    glViewport(0, 0, w, h);

	// Set perspective
	gluPerspective(camera_global->projection[0],ratio, camera_global->projection[1] ,camera_global->projection[2]);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}

void run(Window* window, Camera* camera, Group* group, int argc, char* argv[]) {
    camera_global = camera;
	group_global = group;

	// init GLUT and #include "parser.cpp"the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(window->height,window->width);
	glutCreateWindow("3DEngine");
		
// Required callback registry 
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	
// Callback registration for keyboard processing
//	glutKeyboardFunc(processKeys);
//	glutSpecialFunc(processSpecialKeys);

//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	
// enter GLUT's main cycle
	glutMainLoop();
}
