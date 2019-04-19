

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
int x,y,xold,yold;
double theta=0,phi=0;
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

	glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

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





void affichage(){

    GLfloat lightColor0[] = {1.0f, 0.1f, 0.1f, 1.0f};    // Color (0.5, 0.5, 0.5)
	GLfloat lightPos0[] = {0.0f, 2.0f, 0.0f, 1.0f};      // Positioned at (4, 0, 8)
	GLfloat ambientColor[] = {0.2f, 0.2f, 0.2f, 1.0f};   // Color (0.2, 0.2, 0.2)

	// effacement de l'image avec la couleur de fond
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);

	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();


	gluLookAt(depth * sin(theta) * cos(phi), depth * sin(phi), depth * cos(theta) * cos(phi),
              0,0,0,
              0,1,0);
    //glTranslatef(0.0f, 0.0f, -depth);

    glPushMatrix();
		glColor3f(0.3, 0.3, 0.3);
		glPushMatrix();
			GLUquadricObj *torso = gluNewQuadric();
			gluCylinder(torso, 0.5, 0.5, 1.0, 30, 30);
			
		glPopMatrix();

		glPushMatrix();
			glTranslatef(0, 0, -0.5);

			GLUquadricObj *bottom = gluNewQuadric();
			gluSphere(bottom, 0.35, 40, 30);

		glPopMatrix();
    glPopMatrix();

	// On force l'affichage du r�sultat
	glFlush();
	// On �change les buffers
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
