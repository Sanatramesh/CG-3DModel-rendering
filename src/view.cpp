#include "view.h"
#include "model.h"

int View::h = 0;
int View::w = 0;

extern Model model;

float View::transx = 0.0;
float View::transy = 0.0;
float View::transz = 0.0;
float View::scale = 1;
float View::angle = 0.0;

Trackball View::trackball;

/*
 * draw: Render the axis, model and bounding box
 */
void View::draw(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	float pos = 5.0;
	GLfloat position[] = { pos, pos, pos, 1.0 };	
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	
	gluLookAt
		(
		.05, .1, 4,
		0., 0., 0,
		0, 1, 0
		);

	static float rotation_transform[4][4];
	model.drawAxis();
	
	trackball.rotationMatrix( rotation_transform );
	glLightfv (GL_LIGHT0, GL_POSITION, position);
    glPushMatrix();
	glScalef(scale, scale, scale);
	glTranslatef(transx, transy, transz);
	glRotatef(angle, 1, 0, 0);
	glMultMatrixf( &rotation_transform[0][0] ) ;

	model.drawBoundingBox();
	glEnable (GL_LIGHTING);
	model.drawObject();
	glDisable (GL_LIGHTING);

	glPopMatrix();
	glutSwapBuffers();
}

/*
 * initRendering: Setting up glut window and view
 */
void View::initRendering() {
	w = glutGet( GLUT_WINDOW_WIDTH );
	h = glutGet( GLUT_WINDOW_HEIGHT );
	glViewport(0, 0, w, h);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 50.0 };
	
	GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_position[] = { 2.0, 2.0, 2.0, 1.0 };

	GLfloat spot_direction[] = { 0.0, 0.0, 0.0 };
	GLfloat lmodel_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0, 0.0, .0, 0.0);
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();    
	gluPerspective( 60.0, w / h, 0.1, 10.0 );
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
}

/*
 * handleResize: Setting up glut window and view after window resize
 */
void View::handleResize(int wt, int ht){
	if (ht  == 0)
		ht = 1;
	
	w = wt;
	h = ht;
	glViewport(0,0, wt, ht);
	glMatrixMode(GL_PROJECTION); //Switch to setting the camera perspective
	//Set the camera perspective
	glLoadIdentity(); //Reset the camera
	gluPerspective( 60.0, w / h, 0.1, 10.0 );
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

/*
 * rotate: rotate the object using trackball
 */
void View::rotate(float oldx, float oldy, float x, float y){
	trackball.rotate((2.0*oldx - w)/(float)w,
					 (h - 2.0*oldy)/(float)h,
					 (2.0*x - w)/(float)w,
					 (h - 2.0*y)/(float)h);
}
