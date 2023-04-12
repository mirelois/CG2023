#include "engine.h"

Camera* camera_global;
Group* group_global;
float camera_side = 0, camera_up = 0, camera_front = 0, camera_move_delta = 2, 
	look_rotate_delta_up = M_PI/32, look_rotate_delta_right = M_PI / 32, look_rotate_up = 0, look_rotate_right = 0;
float saved[3];
char axis = 1;
char polygon = 1;

using namespace std;

void drawAxis(){
	glBegin(GL_LINES);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-300.0f, 0.0f, 0.0f);
	glVertex3f( 300.0f, 0.0f, 0.0f);

	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, -100.0f, 0.0f);
	glVertex3f(0.0f, 100.0f, 0.0f);

	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, -300.0f);
	glVertex3f(0.0f, 0.0f, 300.0f);

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

void rotate_over_vector(float p[3], float v[3], float angle) {
	//https://en.wikipedia.org/wiki/Rotation_matrix#General_rotations
	float omc = 1 - cos(angle), s = sin(angle), c = cos(angle);
	p[0] = p[0] * (pow(v[0], 2) * omc + c) + p[1] * (v[0] * v[1] * omc - v[2] * s) + p[2] * (v[0] * v[2] * omc + v[1] * s);
	p[1] = p[0] * (v[1] * v[0] * omc + v[2] * s) + p[1] * (pow(v[1], 2) * omc + c) + p[2] * (v[1] * v[2] * omc - v[0] * s);
	p[2] = p[0] * (v[2] * v[0] * omc - v[1] * s) + p[1] * (v[2] * v[1] * omc + v[0] * s) + p[2] * (pow(v[2], 2) * omc + c);
}

float normalize_vector(float p[3]) {
	float norm = sqrt(pow(p[0], 2) + pow(p[1], 2) + pow(p[2], 2));
	p[0] = p[0] / norm;
	p[1] = p[1] / norm;
	p[2] = p[2] / norm;
	return norm;
}

void save_position() {
	//os c�lculos est�o aqui dentro se por ventura nos interessar mudar a dire��o da c�mera (implica recalcular)
	//se estiver demasiado lento p�e-se no in�cio
	float d[3] = { camera_global->lookAt[0] - camera_global->position[0],
		camera_global->lookAt[1] - camera_global->position[1],
		camera_global->lookAt[2] - camera_global->position[2] };

	normalize_vector(d);

	//rota��o de l sobre o eixo up e sobre o eixo r
	//na documenta��o do glut tem a matriz explicitada, ter cuidado que faltam parenteses
	//https://registry.khronos.org/OpenGL-Refpages/gl2.1/xhtml/glRotate.xml

	normalize_vector(camera_global->up);

	rotate_over_vector(d, camera_global->up, look_rotate_right * look_rotate_delta_right);

	//cross product � m�o enquanto n�o descubro como fazer pela fun��o cross (cad�?)
	//a dire��o para a frente � o -z
	float r[3] = { d[1] * camera_global->up[2] - camera_global->up[1] * d[2],
		d[2] * camera_global->up[0] - camera_global->up[2] * d[0],
		d[0] * camera_global->up[1] - camera_global->up[0] * d[1] };

	normalize_vector(r);

	rotate_over_vector(d, r, look_rotate_up * look_rotate_delta_up);

	saved[0] += d[0] * camera_move_delta * camera_front + r[0] * camera_move_delta * camera_side + camera_up * camera_move_delta * camera_global->up[0];
	saved[1] += camera_side * camera_move_delta * r[1] + camera_front * camera_move_delta * d[1] + camera_up * camera_move_delta * camera_global->up[1];
	saved[2] += camera_side * camera_move_delta * r[2] + camera_front * camera_move_delta * d[2] + camera_up * camera_move_delta * camera_global->up[2];
	camera_front = 0;
	camera_up = 0;
	camera_side = 0;
	printf("%f %f %f\n", saved[0], saved[1], saved[2]);
}

void renderScene(void) {

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();


	//os c�lculos est�o aqui dentro se por ventura nos interessar mudar a dire��o da c�mera (implica recalcular)
	//se estiver demasiado lento p�e-se no in�cio
	float d[3] = { camera_global->lookAt[0] - camera_global->position[0],
		camera_global->lookAt[1] - camera_global->position[1],
		camera_global->lookAt[2] - camera_global->position[2] };
	
	float norm = normalize_vector(d);

	//rota��o de l sobre o eixo up e sobre o eixo r
	//na documenta��o do glut tem a matriz explicitada, ter cuidado que faltam parenteses
	//https://registry.khronos.org/OpenGL-Refpages/gl2.1/xhtml/glRotate.xml

	normalize_vector(camera_global->up);
	
	rotate_over_vector(d, camera_global->up, look_rotate_right * look_rotate_delta_right);

	//cross product � m�o enquanto n�o descubro como fazer pela fun��o cross (cad�?)
	//a dire��o para a frente � o -z
	float r[3] = { d[1] * camera_global->up[2] - camera_global->up[1] * d[2],
		d[2] * camera_global->up[0] - camera_global->up[2] * d[0],
		d[0] * camera_global->up[1] - camera_global->up[0] * d[1] };

	normalize_vector(r);

	rotate_over_vector(d, r, look_rotate_up * look_rotate_delta_up);

	float desl[3] = { d[0] * camera_move_delta * camera_front + r[0] * camera_move_delta * camera_side + camera_up * camera_move_delta * camera_global->up[0] ,
		camera_side * camera_move_delta * r[1] + camera_front * camera_move_delta * d[1] + camera_up * camera_move_delta * camera_global->up[1],
		camera_side * camera_move_delta * r[2] + camera_front * camera_move_delta * d[2] + camera_up * camera_move_delta * camera_global->up[2]
	};

	gluLookAt(	desl[0] + saved[0], desl[1] + saved[1], desl[2] + saved[2],
				saved[0] + desl[0] + d[0] * norm, 
				saved[1] + desl[1] + d[1] * norm, 
				saved[2] + desl[2] + d[2] * norm,
				camera_global->up[0], camera_global->up[1], camera_global->up[2]);

	// Colocar funcoes de desenho aqui
	if(axis)
		drawAxis();

	//mudar o mundo

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

	case 'r': {
		look_rotate_right = 0;
		look_rotate_up = 0;
		camera_side = 0;
		camera_up = 0;
		camera_front = 0;
		saved[0] = camera_global->position[0], saved[1] = camera_global->position[1], saved[2] = camera_global->position[2];
		break;
	}

	case 'u': {
		save_position();
		look_rotate_up += 1;
		break;
	}

	case 'j': {
		save_position();
		look_rotate_up -= 1;
		break;
	}

	case 'h': {
		save_position();
		look_rotate_right += 1;
		break;
	}

	case 'k': {
		save_position();
		look_rotate_right -= 1;
		break;
	}

	case ' ': {
		int mod_key = glutGetModifiers();
		if (GLUT_ACTIVE_SHIFT == mod_key)
			camera_up -= 1;
		else 
			camera_up += 1;
		break;
	}

		case 'w': {
			camera_front += 1;
			break;
		}

		case 'a': {
			camera_side -= 1;
			break;
		}

		case 's': {
			camera_front -= 1;
			break;
		}

		case 'd': {
			camera_side += 1;
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

void processSpecialKeys(int key, int xx, int yy) {

	switch (key) {


	default:
		return;
	}
	glutPostRedisplay();

}

void run(Window* window, Camera* camera, Group* group, int argc, char* argv[]) {
    camera_global = camera;
	group_global = group;
	saved[0] = camera_global->position[0];
	saved[1] = camera_global->position[1];
	saved[2] = camera_global->position[2];

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
