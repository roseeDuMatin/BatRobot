

/*******************************************************/
/*					cube.c							   */
/*******************************************************/
/*													   */
/*   Didactitiel OpenGL sous Glut                      */
/*   ESGI : 2I ann�e         						   */
/*													   */
/*******************************************************/
/*													   */
/*  Objectif : afficher � l'�cran un cube en 3D        */
/*													   */
/*******************************************************/



#include<windows.h>
#include<GL/glut.h>
#include<stdlib.h>
#include<stdio.h>
#include <math.h>

/* Notre structure point */
typedef struct
{
	float x;
	float y;
	float z;
	float r;
	float g;
	float b;
} point;

point p[8]={
		{-0.5,-0.5, 0.5,1.0,0.0,0.0},
		{-0.5, 0.5, 0.5,0.0,1.0,0.0},
		{ 0.5, 0.5, 0.5,0.0,0.0,1.0},
		{ 0.5,-0.5, 0.5,1.0,1.0,1.0},
		{-0.5,-0.5,-0.5,1.0,0.0,0.0},
		{-0.5, 0.5,-0.5,0.0,1.0,0.0},
		{ 0.5, 0.5,-0.5,0.0,0.0,1.0},
		{ 0.5,-0.5,-0.5,1.0,1.0,1.0}};


int f[6][4]={
	{0,1,2,3},
	{3,2,6,7},
	{4,5,6,7},
	{0,1,5,4},
	{1,5,6,2},
	{0,4,7,3}};

char presse;
int x, y, xold, yold;
double theta = 0, phi = 0;
double bat = 0;
double move = 0;
double depth = 3;

/* Prototypes de fonctions */
void affichage();
void clavier(unsigned char touche,int x,int y);
void reshape(int x,int y);
void idle();
void mouse(int bouton,int etat,int x,int y);
void mousemotion(int x,int y);




int main(int argc, char **argv){

	/* Initialisation de glut et creation de la fenetre */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB);
    glutInitWindowSize(500, 500);
	glutInitWindowPosition (100, 100);
    glutCreateWindow("cube");

	/* Initialisation d'OpenGL */
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glColor3f(1.0, 1.0, 1.0);
	glPointSize(2.0);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_COLOR_MATERIAL);
    glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);

	/* Enregistrement de fonctions de rappel */
    glutDisplayFunc(affichage);
	glutKeyboardFunc(clavier);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutMotionFunc(mousemotion);

	/* Entr�e dans la boucle principale de glut */
    glutMainLoop();
    return 0;
}

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

void leftArm(int inverse, double bodyLength){
		double radius = 0.25;
		double length = 1;
		double space = 0.05;
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

void rightArm(double bodyLength){
	glPushMatrix();
		glRotatef(180, 0, 1, 0);
		glRotatef(180, 1, 0, 0);
		leftArm(-1, bodyLength);
	glPopMatrix();
}

void animation(){
	move += 0.2;
	bat += 0.1;
	if(bat > 2 * M_PI){
		bat = 0;
	}
	if(move > 100){
		move = 0;
	}
}

void affichage(){

	int i,j;
	double headWidth = 0.5;
	double bodyBottomWidth = 0.4;
	double bodyLength = 1.5;



    GLfloat lightColor0[] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat lightPos0[] = {0.0f, 1.2f, 1.0f, 1.0f};
	GLfloat ambientColor[] = {0.2f, 0.2f, 0.2f, 1.0f};   // Color (0.2, 0.2, 0.2)
	GLfloat low_shininess[] = {5.0};

	// effacement de l'image avec la couleur de fond
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();


	gluLookAt(depth * sin(theta) * cos(phi), depth * sin(phi), depth * cos(theta) * cos(phi),
              0,0,0,
              0,1,0);
    //glTranslatef(0.0f, 0.0f, -depth);

glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);

	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightColor0);
    glMaterialfv(GL_FRONT, GL_SHININESS, low_shininess);

    glPushMatrix();
		// Déplacement vertical à commenter pour immobiliser
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
			glPushMatrix();
				glTranslatef(0, 0, headWidth + bodyLength + 0.25);
				gluSphere(body, headWidth, 40, 30);
			glPopMatrix();

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
		glPushMatrix();

		glPopMatrix();

    glPopMatrix();

	// On force l'affichage du resultat
	glFlush();
	// On echange les buffers
	glutSwapBuffers();

}



/* Evenement du clavier */
void clavier(unsigned char touche,int x,int y){
	switch (touche){

        case 'z':/*affichage du carr� plein*/
			depth-=0.1;
			glutPostRedisplay();
			break;
        case 's':
            depth+=0.1;
            glutPostRedisplay();
            break;
        case 'b':
			animation();
            glutPostRedisplay();
            break;
		case 'q':/* Quitter le programme */
			exit(0);
	}
}


void reshape(int x,int y)
{
    glViewport(0, 0,(GLsizei) 500, (GLsizei) 500);
    glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    gluPerspective(60.0, (GLfloat) 500 / (GLfloat) 500, 1.0,	200.0);
}


void mouse(int button,int state,int x,int y)
{
	// Si on appuie sur le bouton de gauche
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		presse = 1; //le booleen presse passe � 1 (vrai)
		xold = x; //on sauvegardela position de la souris
		yold = y;
	}
	// Si on relache le bouton gauche
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
		presse = 0; //le booleen presse passe � 0 (faux)
}


void mousemotion(int x,int y)
{
	if (presse) // si le bouton de gauche est press�
	{
		/* On modifie les angles de rotation de l'objet en fonction de la
		position actuelle de la souris et de la derni�re position suvegard�e*/
		theta = theta - (x - xold) * M_PI / 180;
		phi = phi + (y - yold) * M_PI / 180;
		glutPostRedisplay(); // Rafraichissement de l'affichage
	}



	xold = x; //sauvegarde des valeurs courantes de la position de la souris
	yold = y;
}
