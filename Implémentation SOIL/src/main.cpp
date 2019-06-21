#include <stdlib.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include "glut.h"
#include "SOIL.h"

#include "Camera.h"
#include "Map.h"
#include <math.h>


// Objet Camera
Camera *cam = new Camera();
// Objet Scène
Map *m = new Map();

char presse;
int x, y, xold, yold;
double bat = 0;
double move = 0;
double depth = 3;

void finger(GLUquadricObj * arm, int inverse, double radius, double space, double length, int rotation){
	glPushMatrix();
		glRotatef(rotation, 0, 1, 0);
		glTranslatef(0, 0, space);
		gluCylinder(arm, 2.5 * (radius - space),  1.5 * (radius - space), length * 0.4, 30, 30);
		glPushMatrix();

			glTranslatef(0, 0, length * 0.4 + space);
			gluSphere(arm, 1.5 * (radius - space), 40, 30);
			glPushMatrix();
				if(bat >= 0 && bat < M_PI / 2){
					glRotatef(100 * inverse - 100 * inverse * sin(bat), 1, 0, 1);
				}else if(bat >= 3 * M_PI / 2 && bat < 2 * M_PI){
					glRotatef(100 * inverse * sin(bat - (3 * M_PI / 2) ), 1, 0, 1);
				}
				glRotatef(-10, 0, 1, 0);
				glTranslatef(0, 0, space);
				gluCylinder(arm, 1.5 * (radius - space), radius, length * 0.6, 30, 30);

			glPopMatrix();
		glPopMatrix();
	glPopMatrix();
}


void ears(double offset){
    glBegin(GL_POLYGON);
        glVertex3d(offset, 0.3, -0.1);
        glVertex3d(offset, 0.4,-0.13);
        glVertex3d(offset, 0.55, -0.15);
        glVertex3d(offset, 0.65, 0);
        glVertex3d(offset, 0.55, 0.15);
        glVertex3d(offset, 0.4, 0.13);
        glVertex3d(offset, 0.3, 0.1);
    glEnd();
}
void tooth(double offset){
    glBegin(GL_TRIANGLES);
        glVertex3d(offset, -0.2, 0.48);
        glVertex3d(offset+0.05, -0.1,0.48);
        glVertex3d(offset-0.05, -0.1, 0.48);
    glEnd();
}

void eye(int inverse, double headWidth){
	GLUquadricObj *eye = gluNewQuadric();
	glPushMatrix();
		glTranslatef(0, 0, headWidth);
		glTranslatef(0.15 * inverse, 0, 0);
		glTranslatef(0, 0.15 , 0);
		gluSphere(eye, headWidth / 10,  40, 30); // tete
	glPopMatrix();
}

void head(int inverse, double headWidth, double bodyLength){
	glPushMatrix();
		GLUquadricObj *head = gluNewQuadric();
		glPushMatrix();
			glTranslatef(0, 0, bodyLength);
			gluCylinder(head, headWidth, headWidth / 2, 0.25, 30, 30); // cou
			glPushMatrix();
				glTranslatef(0, 0, headWidth + 0.25);
				gluSphere(head, headWidth, 40, 30); // tete
				eye(inverse, headWidth);
				eye(-inverse, headWidth);
				ears(0.3);
				ears(-0.3);
				tooth(0.15);
				tooth(-0.15);
			glPopMatrix();
		glPopMatrix();

	glPopMatrix();

}

void leftArm(int inverse, double bodyLength){
		double radius = 0.25;
		double length = 1;
		double space = 0.05;

		GLfloat matrixMV[16];
		double coords[12];

		// leftArm
		glPushMatrix();
			GLUquadricObj *leftArm = gluNewQuadric();
			glPushMatrix();
                glRotatef(sin(bat) * 20 * inverse, 0, 0, 1);
				glTranslatef(radius * 2 + space, 0, 0.80 * bodyLength);
				gluSphere(leftArm, radius, 40, 30);
				glPushMatrix();
					glRotatef(70 + sin(bat) * 35, 0, 1, 0);
					glRotatef(-70 *inverse, 1, 0, 0);
					glTranslatef(0, 0, (radius / 2) + space);

					length = radius * 3;
					radius = radius - space;
					//premier cylindre (bras)
					gluCylinder(leftArm, radius, radius / 2, length, 30, 30);
					glPushMatrix();
						glTranslatef(0, 0, length + space);

						space = 0.02;
						radius = radius / 2 + space;

						gluSphere(leftArm, radius, 40, 30);
						glPushMatrix();
							if(!(bat >= 0 && bat < M_PI))
							{
								glRotatef(-120 * inverse * sin(bat), 1, 0, 0);
								glRotatef(15 * sin(bat), 0, 1, 0);
							}
							length = length * 2.5;
							radius = radius / 2 + space / 2;

							glTranslatef(0, 0, space);
							gluCylinder(leftArm, 2 * (radius - space), radius - space, length, 30, 30);

							glPushMatrix();

								glRotatef(-25 * inverse - sin(bat) * inverse * 15 , 0, 0, 1);

								glTranslatef(0, 0, length + space);
								gluSphere(leftArm, radius - space, 40, 30);

								space = space / 5;
								radius = radius / 5 + space;

								// Plus près du corps
								finger(leftArm, inverse, radius, space, length, 90);
								// Milieu
								finger(leftArm,inverse, radius, space, length * 1.05, 60);
								// Plus loin du corps
								finger(leftArm,inverse, radius, space, length * 1.65, 20);


							glPopMatrix();
						glPopMatrix();
					glPopMatrix();
				glPopMatrix();
			glPopMatrix();
		glPopMatrix();
}


/* glBegin(GL_TRIANGLES) {
    glPushMatrix();
        glVertex2f(10.0, 50.0);
        glVertex2f(50.0, 70.0);
        glVertex2f(70.0, 10.0);
    glPopMatrix();
}  */

void rightArm(double bodyLength){
	glPushMatrix();
		glRotatef(180, 0, 1, 0);
		glRotatef(180, 1, 0, 0);
		leftArm(-1, bodyLength);
	glPopMatrix();
}


void fingerLeg(GLUquadricObj * leg, int inverse, double radius, double space, double length, int rotation){
	glPushMatrix();
		length = length * 1.5;
		radius = radius * 1.5;
		space = space * 1.5;
		glRotatef(rotation, 0, 1, 0);
		glTranslatef(0, 0, space);
		gluCylinder(leg, 2.5 * (radius - space),  1.5 * (radius - space), length * 0.4, 30, 30);
		glPushMatrix();

			glTranslatef(0, 0, length * 0.4 + space);
			gluSphere(leg, 1.5 * (radius - space), 40, 30);
			glPushMatrix();
				finger(leg, inverse, radius / 1.5, space / 1.5 , length / 3, -20);
				finger(leg, inverse, radius / 1.5, space / 1.5 , length / 3, -50);
				finger(leg, inverse, radius / 1.5, space / 1.5 , length / 3, -80);
			glPopMatrix();
		glPopMatrix();
	glPopMatrix();
}

void leftLeg(int inverse, double bodyLength) {
    double radius = 0.25/3;
    double length = 0.75;
    double space = 0.05;

    // Left Leg
    glPushMatrix();
        GLUquadricObj *leftLeg = gluNewQuadric();
        glPushMatrix();
            glRotatef(150 * inverse, 1, 1, 0);
            glTranslatef(space, 0,  0.25 * bodyLength);//3 * space +
			gluSphere(leftLeg, radius * 1.5, 40, 30);

			length = length/ 1.5;
            gluCylinder(leftLeg, radius, radius / 2, length * 1.05, 30, 30);

			glPushMatrix();
				glRotatef(-25 * inverse  , 0, 0, 1);

				glTranslatef(0, 0, length + space);
				gluSphere(leftLeg, radius - space, 40, 30);

				space = space / 5;
				radius = radius / 5 + space;

				fingerLeg(leftLeg, inverse, radius / 1.5, space / 1.5 , length, -50);
			glPopMatrix();
        glPopMatrix();
    glPopMatrix();
}



void rightLeg(double bodyLength){
	glPushMatrix();
		glRotatef(-45, 1, 0, 0);
		leftLeg(-1, bodyLength);
	glPopMatrix();
}

/** GESTION FENETRE **/
void reshapeWindow(int w, int h)
{
    if (h == 0)
        h = 1;

    float ratio =  w * 1.0 / h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glViewport(0, 0, w, h);
    gluPerspective(45.0f, ratio, 0.1f, 348.0f);
    glMatrixMode(GL_MODELVIEW);
}

/** FONCTIONS DE GESTION CLAVIER **/
void KeyboardDown(unsigned char key, int xx, int yy)
{
    switch(key)
    {
    case 'e': // Unlock Camera
        cam->locked = (cam->locked)?0:1;
        break;
    case 'z':
        cam->deltaForward = 1;
        break;
    case 's':
        cam->deltaForward = -1;
        break;
    case 'd':
        cam->deltaStrafe = -1;
        break;
    case 'q':
        cam->deltaStrafe = 1;
        break;
    case 'b':
        move += 0.2;
        bat += 0.1;
        if(bat > 2 * M_PI){
            bat = 0;
        }
        if(move > 10){
            move = -10;
        }
        break;
    }
}
void KeyboardUp(unsigned char key, int xx, int yy)
{
    switch(key)
    {
    case 'z':
    case 's':
        cam->deltaForward = 0;
        break;
    case 'q':
    case 'd':
        cam->deltaStrafe = 0;
        break;
    }
}
void SpecialDown(int key, int xx, int yy)
{
    switch(key)
    {
    case GLUT_KEY_UP:
        cam->deltaForward = 1;
        break;
    case GLUT_KEY_DOWN:
        cam->deltaForward = -1;
        break;
    case GLUT_KEY_RIGHT:
        cam->deltaStrafe = -1;
        break;
    case GLUT_KEY_LEFT:
        cam->deltaStrafe = 1;
        break;
    }
}
void SpecialUp(int key, int xx, int yy)
{
    switch(key)
    {
    case GLUT_KEY_UP:
    case GLUT_KEY_DOWN:
        cam->deltaForward = 0;
        break;
    case GLUT_KEY_RIGHT:
    case GLUT_KEY_LEFT:
        cam->deltaStrafe = 0;
        break;
    }
}

/** FONCTIONS DE GESTION SOURIS (ORIENTATION CAMERA) **/
void mouseMove(int x, int y)
{
    // Rentres uniquement lors du clic
    cam->orienterCam(x, y);
}
void mouseButton(int button, int state, int x, int y)
{
    // Gestion camera en fonction du clic souris
    if (button == GLUT_LEFT_BUTTON)
    {
        // Relacher la camera
        if (state == GLUT_UP)
        {
            cam->releaseCam();
        }
        // Mise à jour origine du clic
        else
        {
            cam->grabCam(x, y);
        }
    }
}

/** GESTION DEPLACEMENT CAMERA **/
void computePos(int inutile)
{
    cam->updatePos();
    glutTimerFunc(10, computePos, 0);
}

/** AFFICHAGE **/
void renderScene(void)
{
    int i,j;
	double headWidth = 0.5;
	double bodyBottomWidth = 0.4;
	double bodyLength = 1.5;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    // Définition de la caméra
    gluLookAt(  cam->posx, cam->posy, cam->posz,
                cam->posx+cam->dirx, cam->posy+cam->diry,  cam->posz+cam->dirz,
                0.0f, 1.0f,  0.0f
                );

    m->DrawGround();
    m->DrawSkybox(cam);
    glPushMatrix();
		// Déplacement vertical à commenter pour immobiliser
        glScalef(0.2,0.2,0.2);
		glTranslatef(0, -2 *sin(bat), 0); //BAT  est remis à zero
		// Déplacement horizontal
		glTranslatef(0, 0, move);

		// Inclinaison
		glRotatef(-20, 1, 0, 0);

		glColor3f(0.3, 0.3, 0.3);

		// Body
		glPushMatrix();
			GLUquadricObj *body = gluNewQuadric();
			// head
			head(1, headWidth, bodyLength);

			// torso
			gluCylinder(body, bodyBottomWidth, headWidth, bodyLength, 30, 30); // 1.5

			// torso-top
			glPushMatrix();
				glTranslatef(0, 0, bodyLength);
				gluDisk(body, 0, headWidth, 30, 1);
			glPopMatrix();

			// bottom
			glPushMatrix();
				gluSphere(body, bodyBottomWidth, 40, 30);
			glPopMatrix();
		glPopMatrix();

		// Arms
		leftArm(1, bodyLength);
		rightArm(bodyLength);

		// Legs
        leftLeg(1, bodyLength);
        rightLeg(bodyLength);

    glPopMatrix();
    glutSwapBuffers();
}

void LoadTextures()
{
    m->LoadTextures();
}

int main(int argc, char **argv)
{
    /** CREATION FENETRE **/
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(320,320);
    glutCreateWindow("Implémentation :: Textures");

    /** FONCTIONS GLUT **/
    glutDisplayFunc(renderScene);
    glutReshapeFunc(reshapeWindow);
    glutIdleFunc(renderScene);
    glutTimerFunc(10, computePos, 0);

    /** GESTION CLAVIER **/
    glutIgnoreKeyRepeat(1);
    glutKeyboardFunc(KeyboardDown);
    glutKeyboardUpFunc(KeyboardUp);
    glutSpecialFunc(SpecialDown);
    glutSpecialUpFunc(SpecialUp);

    /** GESTION SOURIS **/
    glutMouseFunc(mouseButton);
    glutMotionFunc(mouseMove);

    /** INIT TEXTURES **/
    LoadTextures();

    /** INIT GL STATES **/
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);

    glutMainLoop();

    return (1);
}


