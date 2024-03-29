#include "engine.h"
#include <algorithm>

Camera* camera_global;
Group* group_global;
vector<Light*>* lights = new vector<Light*>();
float  look_rotate_delta_up = M_PI / 1024, look_rotate_delta_right = M_PI / 1024;
float startX = 0.0f, startY = 0.0f, tracking = 0;
int camera_side = 0, camera_up = 0, camera_front = 0, look_rotate_up = 0, look_rotate_right = 0;
float last_camera_position[3];
char axis = 1;
char polygon = 0;
GLuint buffer[4];
int timebase = 0;
float frame = 0;

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

	for(Transformation* transformation: group->transformations)
		transformation->transform();

	for (Model* groupModel : group->models) {
		glMaterialfv(GL_FRONT, GL_SPECULAR, groupModel->specular);
		glMaterialfv(GL_FRONT, GL_AMBIENT, groupModel->ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, groupModel->diffuse);
		glMaterialfv(GL_FRONT, GL_EMISSION, groupModel->emissive);
		glMaterialf(GL_FRONT, GL_SHININESS, groupModel->shininess);
		//se texture for null é igual a 0 então o bind não faz nada
		glBindTexture(GL_TEXTURE_2D, groupModel->texID);
		glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
		glVertexPointer(3, GL_FLOAT, 0, 0);

		glBindBuffer(GL_ARRAY_BUFFER, buffer[1]);
		glNormalPointer(GL_FLOAT, 0, 0);

		glBindBuffer(GL_ARRAY_BUFFER, buffer[2]);
		glTexCoordPointer(2, GL_FLOAT, 0, 0);

		glDrawElements(groupModel->type, groupModel->modelInfo->size, GL_UNSIGNED_INT, (void*)(groupModel->modelInfo->index * sizeof(GLuint)));
		glBindTexture(GL_TEXTURE_2D, 0);
	}
		
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
	float q[3];
	float omc = 1 - cos(angle), s = sin(angle), c = cos(angle);

	q[0] = p[0] * (pow(v[0], 2) * omc + c) + p[1] * (v[0] * v[1] * omc - v[2] * s) + p[2] * (v[0] * v[2] * omc + v[1] * s);
	q[1] = p[0] * (v[1] * v[0] * omc + v[2] * s) + p[1] * (pow(v[1], 2) * omc + c) + p[2] * (v[1] * v[2] * omc - v[0] * s);
	q[2] = p[0] * (v[2] * v[0] * omc - v[1] * s) + p[1] * (v[2] * v[1] * omc + v[0] * s) + p[2] * (pow(v[2], 2) * omc + c);

	p[0] = q[0];
	p[1] = q[1];
	p[2] = q[2];
}

float normalize_vector(float p[3]) {
	float norm = sqrt(pow(p[0], 2) + pow(p[1], 2) + pow(p[2], 2));
	p[0] = p[0] / norm;
	p[1] = p[1] / norm;
	p[2] = p[2] / norm;
	return norm;
}


void calculate_displacement(float displacement[3], float distance_to_lookat[3]) {
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

	//normalize_vector(d);

	displacement[0] = d[0] * camera_global->camera_move_delta * camera_front + r[0] * camera_global->camera_move_delta * camera_side,
	displacement[1] = camera_side * camera_global->camera_move_delta * r[1] + camera_front * camera_global->camera_move_delta * d[1] + camera_up * camera_global->camera_move_delta,
	displacement[2] = camera_side * camera_global->camera_move_delta * r[2] + camera_front * camera_global->camera_move_delta * d[2];

	distance_to_lookat[0] = d[0] * norm,
	distance_to_lookat[1] = d[1] * norm,
	distance_to_lookat[2] = d[2] * norm;
}

void save_position() {
	if (camera_front == 0 && camera_up == 0 && camera_side == 0) return;

	float desl[3];
	float dist[3];

	calculate_displacement(desl, dist);

	last_camera_position[0] += desl[0];
	last_camera_position[1] += desl[1];
	last_camera_position[2] += desl[2];
	camera_front = 0;
	camera_up = 0;
	camera_side = 0;
}

void renderScene(void) {

	// clear buffers
	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();

	float desl[3];
	float dist[3];
	calculate_displacement(desl, dist);

	//os c�lculos est�o aqui dentro se por ventura nos interessar mudar a dire��o da c�mera (implica recalcular)
	//se estiver demasiado lento p�e-se no in�cio

	gluLookAt(	desl[0] + last_camera_position[0], desl[1] + last_camera_position[1], desl[2] + last_camera_position[2],
				last_camera_position[0] + desl[0] + dist[0], 
				last_camera_position[1] + desl[1] + dist[1], 
				last_camera_position[2] + desl[2] + dist[2],
				camera_global->up[0], camera_global->up[1], camera_global->up[2]);

	// Colocar funcoes de desenho aqui
	if(axis){
		glEnable(GL_COLOR_MATERIAL);
		drawAxis();
		glDisable(GL_COLOR_MATERIAL);
	}

	//desenhar as luzes
	for(Light* light: *lights){
		light->drawLight();
	}

	//mudar o mundo
	draw();

	float fps;
	int time;
	char s[64];
	frame++;
	time=glutGet(GLUT_ELAPSED_TIME); 
	if (time - timebase > 1000) { 
		fps = frame*1000.0/(time-timebase); 
		timebase = time; 
		frame = 0; 
		sprintf(s, "FPS: %f6.2", fps);
		glutSetWindowTitle(s);
	} 

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
		last_camera_position[0] = camera_global->position[0], last_camera_position[1] = camera_global->position[1], last_camera_position[2] = camera_global->position[2];
		break;
	}

	case 'u': {
		save_position();
		look_rotate_up = (look_rotate_up + 4) % (int)((2 * M_PI) / look_rotate_delta_up + 1);
		break;
	}

	case 'j': {
		save_position();
		look_rotate_up = (look_rotate_up - 4) % (int)((2 * M_PI) / look_rotate_delta_up + 1);
		break;
	}

	case 'h': {
		save_position();
		look_rotate_right = (look_rotate_right + 4) % (int)((2 * M_PI) / look_rotate_delta_right + 1);
		break;
	}

	case 'k': {
		save_position();
		look_rotate_right = (look_rotate_right - 4) % (int)((2 * M_PI) / look_rotate_delta_right + 1);
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
	//glutPostRedisplay();
}

void processSpecialKeys(int key, int xx, int yy) {

	switch (key) {


	default:
		return;
	}
	//glutPostRedisplay();

}

void processMouseButtons(int button, int state, int xx, int yy) {

	if (state == GLUT_DOWN) {
		startX = xx;
		startY = yy;
		tracking = 1;
	}
	else if (state == GLUT_UP) {
		tracking = 0;
	}
}

void processMouseMotion(int xx, int yy) {

	int deltaX, deltaY;

	if (!tracking)
		return;

	deltaX = startX - xx;
	deltaY = startY - yy;

	startX = xx;
	startY = yy;

	if (tracking == 1) {
		save_position();
		look_rotate_right = (look_rotate_right + deltaX) % (int)((2 * M_PI) / look_rotate_delta_right + 1);
		look_rotate_up = (look_rotate_up + deltaY) % (int)((2 * M_PI) / look_rotate_delta_up + 1);
	}
	//glutPostRedisplay();
}

int main(int argc, char* argv[]) {
// Initialize objects
	camera_global = new Camera();
	group_global = new Group();
	Window* window = new Window();
	
// Read Xml file
	vector<float>* points = new vector<float>();
	vector<float>* normals = new vector<float>();
	vector<float>* texCoords = new vector<float>();
	vector<unsigned int>* indices = new vector<unsigned int>();

	// init GLUT and #include "parser.cpp"the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(window->height,window->width);
	glutCreateWindow("3DEngine");
		
// Required callback registry 
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutReshapeFunc(changeSize);
	
// Callback registration for keyboard processing
	glutKeyboardFunc(processKeys);
	glutMouseFunc(processMouseButtons);
	glutMotionFunc(processMouseMotion);
	//glutSpecialFunc(processSpecialKeys);
	
// Init glew
	glewInit();

//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_RESCALE_NORMAL);

	// light colors
	float amb[4] = { 0.5f, 0.5f, 0.5f, 0.5f };
	int amb_active = 1;
	//Parser depois dos inits para se conseguir dar load às texturas e guardar apenas o ID
	parser(argv[1], window, camera_global, lights, amb, &amb_active, group_global, points, normals, texCoords, indices);
	last_camera_position[0] = camera_global->position[0];
	last_camera_position[1] = camera_global->position[1];
	last_camera_position[2] = camera_global->position[2];

	//
	if (!lights->empty()) glEnable(GL_LIGHTING);
	for(Light* light: *lights){
		glEnable(light->getNumber());
		glLightfv(light->getNumber(), GL_AMBIENT, light->ambient);
		glLightfv(light->getNumber(), GL_DIFFUSE, light->diffuse);
		glLightfv(light->getNumber(), GL_SPECULAR, light->specular);
	}
	// controls global ambient light
	if(amb_active){
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb);
	}

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	//glClearColor(0, 0, 0, 0);

	glEnable(GL_TEXTURE_2D);
	// VBO'S
	glGenBuffers(4, buffer);

	glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
	glBufferData(GL_ARRAY_BUFFER, points->size()*sizeof(float), points->data(), GL_STATIC_DRAW);
	delete(points);

	glBindBuffer(GL_ARRAY_BUFFER, buffer[1]);
	glBufferData(GL_ARRAY_BUFFER, normals->size() * sizeof(unsigned int), normals->data(), GL_STATIC_DRAW);
	delete(normals);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, texCoords->size() * sizeof(unsigned int), texCoords->data(), GL_STATIC_DRAW);
	delete(texCoords);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices->size()*sizeof(unsigned int), indices->data(), GL_STATIC_DRAW);
	delete(indices);

	glVertexPointer(3, GL_FLOAT, 0, 0);
// enter GLUT's main cycle
	glutMainLoop();
}
