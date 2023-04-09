#include "engine.h"

Camera* camera_global;
Group* group_global;
float camera_right = 0, camera_up = 0, camera_front = 0, camera_delta = 0.5f;

char axis = 1;
char polygon = 1;

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

void drawGroup(Group* group){
	glPushMatrix();
	
	for(char transformation: group->transformations){
		switch(transformation){

			case 't': {
				glTranslatef(group->translate[0], group->translate[1], group->translate[2]);
				break;
			}

			case 'r': {
				glRotatef(group->rotate[0], group->rotate[1], group->rotate[2], group->rotate[3]);
				break;
			}

			case 's':{
				glScalef(group->scale[0], group->scale[1], group->scale[2]);
				break;
			}

		}
	}

	glBegin(GL_TRIANGLES);
	for(Model* groupModel: group->models){
		for(int i=0; i<groupModel->size; i++){
			glVertex3f(get<0>(groupModel->figure[i]), get<1>(groupModel->figure[i]), get<2>(groupModel->figure[i]));
		}
	}
	glEnd();
	
	for(Group* groupChild: group->subGroups)
		drawGroup(groupChild);
	
	
	glPopMatrix();
}

void draw(){
	glPolygonMode(GL_FRONT_AND_BACK, polygon ? GL_LINE : GL_FILL);
	glColor3f(1.0f,1.0f,1.0f);
	drawGroup(group_global);
	
}

void renderScene(void) {

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();


	//os cálculos estão aqui dentro se por ventura nos interessar mudar a direção da câmera (implica recalcular)
	//se estiver demasiado lento põe-se no início
	float dx = camera_global->position[0] - camera_global->lookAt[0],
		dy = camera_global->position[1] - camera_global->lookAt[1],
		dz = camera_global->position[2] - camera_global->lookAt[2];

	float norm = sqrt(pow(dx, 2) + pow(dy, 2) + pow(dz, 2));
	dx = dx / norm;
	dy = dy / norm;
	dz = dz / norm;

	//cross product à mão enquanto não descubro como fazer pela função cross (cadê?)
	//trocar o cross
	float rx = dz * camera_global->up[1] - camera_global->up[2] * dy,
		ry = dx * camera_global->up[2] - camera_global->up[0] * dz, 
		rz = dy * camera_global->up[0] - camera_global->up[1] * dx;

	norm = sqrt(pow(rx, 2) + pow(ry, 2) + pow(rz, 2));
	rx = rx / norm;
	//ry = ry / norm;
	rz = rz / norm;

	printf("%f\n", camera_up);

	gluLookAt(	camera_global->position[0] + camera_right * rx + camera_front * dx + camera_up * camera_global->up[0],
				camera_global->position[1] + camera_up * camera_global->up[1],
				camera_global->position[2] + camera_right * rz + camera_front * dz + camera_up * camera_global->up[2],
				camera_global->lookAt[0] + camera_right * rx + camera_front * dx + camera_up * camera_global->up[0],
				camera_global->lookAt[1] + camera_up * camera_global->up[1],
				camera_global->lookAt[2] + camera_right * rz + camera_front * dz + camera_up * camera_global->up[2],
				camera_global->up[0], camera_global->up[1], camera_global->up[2]);

	// Colocar funcoes de desenho aqui
	if(axis)
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

void processKeys(unsigned char key, int xx, int yy) {

	switch(key){

	case 'u': {
		camera_up += camera_delta;
		break;
	}

	case 'j': {
		camera_up -= camera_delta;
		break;
	}

		case 'w': {
			camera_front -= camera_delta;
			break;
		}

		case 'a': {
			camera_right -= camera_delta;
			break;
		}

		case 's': {
			camera_front += camera_delta;
			break;
		}

		case 'd': {
			camera_right += camera_delta;
			break;
		}

		case 'o':{
			axis = !axis;
			break;
		}

		case 'p':{
			polygon = !polygon;
			break;
		}

		default:{
			return;
		}
	}
	glutPostRedisplay();

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
	glutKeyboardFunc(processKeys);
	//glutSpecialFunc(processSpecialKeys);

//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	
// enter GLUT's main cycle
	glutMainLoop();
}
