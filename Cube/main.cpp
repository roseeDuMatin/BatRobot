

/*******************************************************/
/*					cube.c							   */
/*******************************************************/
/*													   */
/*   Didactitiel OpenGL sous Glut                      */
/*   ESGI : 2I année         						   */
/*													   */
/*******************************************************/
/*													   */
/*  Objectif : afficher à l'écran un cube en 3D        */
/*													   */
/*******************************************************/



#include<windows.h>
#include<GL/glut.h>
#include<stdlib.h>
#include<stdio.h>

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
int anglex,angley,x,y,xold,yold;

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

	/* Enregistrement de fonctions de rappel */
    glutDisplayFunc(affichage);
	glutKeyboardFunc(clavier);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutMotionFunc(mousemotion);

	/* Entrée dans la boucle principale de glut */
    glutMainLoop();
    return 0;
}





void affichage(){

	int i,j;

	// effacement de l'image avec la couleur de fond
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	glRotatef(-angley,1.0,0.0,0.0);
	glRotatef(-anglex,0.0,1.0,0.0);

	// dessin du cube
	for (i = 0;i < 6;i++)
	{
		glBegin(GL_POLYGON);
		for (j = 0;j < 4;j++)
		{
			glColor3f(p[f[i][j]].r,p[f[i][j]].g,p[f[i][j]].b);
			glVertex3f(p[f[i][j]].x,p[f[i][j]].y,p[f[i][j]].z);
		}
		glEnd();
	}

	// On force l'affichage du résultat
	glFlush();
	// On échange les buffers
	glutSwapBuffers();
}



/* Evenement du clavier */
void clavier(unsigned char touche,int x,int y){
	switch (touche){

		case 'p':/*affichage du carré plein*/
			glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
			glutPostRedisplay();
			break;

		case 'f':/*affichage en mode fil de fer*/
			glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
			glutPostRedisplay();
			break;

		case 's':/*affichage en mode sommets seuls*/
			glPolygonMode(GL_FRONT_AND_BACK,GL_POINT);
			glutPostRedisplay();
			break;
		case 'd':
			glEnable(GL_DEPTH_TEST);
			glutPostRedisplay();
			break;
		case 'D':
			glDisable(GL_DEPTH_TEST);
			glutPostRedisplay();
			break;
		case 'q':/* Quitter le programme */
			exit(0);
	}
}


void reshape(int x,int y)
{
	if (x < y)
		//glViewport(0,(y-x)/2,x,x);
		glViewport(0,50,54,x);
	else
		glViewport((x-y)/2,0,y,y);
}


void mouse(int button,int state,int x,int y)
{
	// Si on appuie sur le bouton de gauche
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		presse = 1; //le booleen presse passe à 1 (vrai)
		xold = x; //on sauvegardela position de la souris
		yold = y;
	}

	// Si on relache le bouton gauche
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
		presse = 0; //le booleen presse passe à 0 (faux)
}


void mousemotion(int x,int y)
{
	if (presse) // si le bouton de gauche est pressé
	{
		/* On modifie les angles de rotation de l'objet en fonction de la
		position actuelle de la souris et de la dernière position suvegardée*/
		anglex = anglex + (x - xold);
		angley = angley + (y - yold);
		glutPostRedisplay(); // Rafraichissement de l'affichage
	}
	xold = x; //sauvegarde des valeurs courantes de la position de la souris
	yold = y;
}
