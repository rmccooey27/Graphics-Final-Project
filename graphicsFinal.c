/*
Graphics-Final-Project
======================

This is the project I created for CSCI 343. It is a scene of a mountain with a skiing teapot. The teapot is going in and out of ski gates and there is also an ice skating rink. 

*/


/**********************************************************************
 * Graphics Final
 * Author: Regan McCooey
 * Date: 11/17/13
 * Purpose: to draw a mountain with skiing teapots in openGL
 * Input: from the keyboard and from the mouse
 * Output: a mountain with skiing teapots
 ***********************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GLUT/GLUT.h>
#include <time.h>
//#include <GL/glut.h>
//change when I go back into radius

typedef GLfloat point3[3];

void readPPM( char *fname);
void generateTextures(int IDnum );
void hemiSphere(float radx,float rady,float radz);
void keys(unsigned char key, int x, int y);
void DrawMountain();
void SkiingTeapot();
void BuildMountainRange();
void DrawSkiers();
void BuildLift();
void SkiGates();
void SkatingRink();
void SkiandSkate();
void SkatingTeapot();
void TeapotTakeover();
void TeapotCrush();
void makeMenus();
void rightMenu(int id);
void color_menu(int id);

#define PI 3.14159

//images used for texture mapping
#define SKY "winterSky.ppm"
#define SNOW "snow1.ppm"
#define MTREES "snowyTrees.ppm"
#define RANGE "mountainRange.ppm"
#define LTEXT "liftText.ppm"
#define ROAD "road.ppm"
#define BLUE "blueCheck.ppm"
#define RED "redCheck.ppm"
#define ICE "ice.ppm"
#define GRASS "snowGrass.ppm"

GLuint imageID[10];

GLfloat ski[3] = {0.0, 0.0, 0.0};
GLfloat teapot[3] = {0.0, 400.0, 0.0};
GLfloat skate[3] = {0.0, 0.0, 0.0};
int width, height;
GLubyte *imageData;
GLfloat light[4] = {1.0, 2.0, 4.0, 0.0};
GLfloat angle;
GLfloat turnAngle;
GLfloat turn;
GLfloat endTurnAngle;
GLfloat skateAngle = 0.0;
GLfloat skateDist = 0.0;

//globals for the mountain
GLfloat mountainHeight = 110.0;
GLfloat mountainTop = 5.0;
GLfloat mountainBase = 45.0;
GLfloat trailBase = 30.0;
GLfloat gateHeight = 2.5;
GLfloat gateWidth = 0.45;
GLfloat xTrans = 0.0;
GLfloat zRotate = 0.0;
GLfloat skateTurn = 0.0;

GLfloat viewer[] = {0.0, 300.0, 100.0};
GLfloat worldSize = 150.0;

//quadrics
GLUquadric* mountain;
GLUquadric* trail;
GLUquadric* range1;
GLUquadric* range2;
GLUquadric* iceRink;

//variables for animation and different types of viewing
int faceCounter = 0;
int turnCount = 0;
int endTrail;
int takeOver = 0;
int topView = 0;
int rinkView = 0;
int sideView = 0;
int skateCount = 0;
int colorId = 1;

point3 vertices[8] = { {50.0, -50.0, -50.0}, {50.0, 50.0, -50.0},
    {-50.0, 50.0, -50.0}, {-50.0, -50.0, -50.0},
    {50.0, -50.0, 50}, {50, 50, 50},
    {-50.0, 50.0, 50.0}, {-50.0, -50.0, 50.0} };

point3 trailVerts[8] = { {-10.0, -10.0, -10.0}, {10.0, -10.0, -10.0}, {10.0, -10.0, 10.0}, {-10.0, -10.0, 10.0},
    {-10.0, 10.0, -10.0}, {10.0, 10.0, -10.0}, {10.0, 10.0, 10.0}, {-10.0, 10.0, 10.0} };


void myinit(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    gluPerspective(45.0, 1.0, 1.0, 1500.0);
    glMatrixMode(GL_MODELVIEW);
    
    //set up and define quadrics
    mountain = gluNewQuadric();
    gluQuadricNormals(mountain, GLU_SMOOTH);
    trail = gluNewQuadric();
    gluQuadricNormals(trail, GLU_SMOOTH);
    range1 = gluNewQuadric();
    range2 = gluNewQuadric();
    gluQuadricNormals(range1, GLU_SMOOTH);
    gluQuadricNormals(range2, GLU_SMOOTH);
    iceRink = gluNewQuadric();
    gluQuadricNormals(iceRink, GLU_SMOOTH);
    
    GLfloat mat_specular[] = {0.0, 0.0, 0.3, 1.0};
    GLfloat mat_diffuse[] = {0.0, 0.0, 1.0, 1.0};
    GLfloat mat_ambient[] = {0.2, 0.0, 0.2, 1.0};
    GLfloat mat_shininess[] = {0.0};
    
    //white light for a light source
    GLfloat light_specular[] = {1.0, 1.0, 1.0, 1,0};
    GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat light_ambient[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat light0_pos[] = {1.0, 2.0, 4.0, 0.0};
    
    glLightfv(GL_LIGHT0, GL_POSITION, light0_pos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    
    glEnable(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glShadeModel(GL_SMOOTH);
    glColorMaterial ( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );
    glEnable ( GL_COLOR_MATERIAL);
}

void display(void){
		/*Note use of Depth buffer bit for hidden surface removal*/
    
    
    
    point3 backgroundVerts[8] = { {-worldSize, 0.0, -worldSize}, {worldSize, 0.0, -worldSize},
        {worldSize, 0.0, worldSize}, {-worldSize, 0.0, worldSize}, {-worldSize, 2*worldSize, -worldSize}, {worldSize, 2*worldSize, -worldSize}, {worldSize, 2*worldSize, worldSize}, {-worldSize, 2*worldSize, worldSize} };
    
    
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glLoadIdentity();
    
    //setting the viewer different things
    if (rinkView == 1) {
        gluLookAt(0.0, mountainHeight + 50.0, 0.0, -50.0, 0.0, 0.0, -70.0, 1.0, 0.0);
    } else if (topView == 1) {
       gluLookAt(0.0, mountainHeight + 50.0, -70.0, 0.0, 0.0, -70.0, -70.0, 1.0, 0.0);
    } else if (sideView == 1) {
        
        gluLookAt(0.0, mountainHeight + 50.0, 10.0, 0.0, mountainHeight/2, -70.0, 0.0, 1.0, 0.0);
    } else {
        gluLookAt(viewer[0], viewer[1], viewer[2], 0.0, mountainHeight + 50.0, 0.0, 0.0, 1.0, 0.0);
    }
    



    
    //ground
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, imageID[1]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0);
    glVertex3fv(backgroundVerts[0]);
    glTexCoord2f(0.0, 1.0);
    glVertex3fv(backgroundVerts[1]);
    glTexCoord2f(1.0, 0.0);
    glVertex3fv(backgroundVerts[2]);
    glTexCoord2f(1.0, 1.0);
    glVertex3fv(backgroundVerts[3]);
    glEnd();
    
    //side4
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,imageID[3]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0);
    glVertex3fv(backgroundVerts[0]);
    glTexCoord2f(0.0, 1.0);
    glVertex3fv(backgroundVerts[4]);
    glTexCoord2f(1.0, 1.0);
    glVertex3fv(backgroundVerts[5]);
    glTexCoord2f(1.0, 0.0);
    glVertex3fv(backgroundVerts[1]);
    
    //side2
    glTexCoord2f(0.0, 0.0);
    glVertex3fv(backgroundVerts[1]);
    glTexCoord2f(0.0, 1.0);
    glVertex3fv(backgroundVerts[5]);
    glTexCoord2f(1.0, 1.0);
    glVertex3fv(backgroundVerts[6]);
    glTexCoord2f(1.0, 0.0);
    glVertex3fv(backgroundVerts[2]);
    
    //side3
    glTexCoord2f(0.0, 0.0);
    glVertex3fv(backgroundVerts[2]);
    glTexCoord2f(0.0, 1.0);
    glVertex3fv(backgroundVerts[6]);
    glTexCoord2f(1.0, 1.0);
    glVertex3fv(backgroundVerts[7]);
    glTexCoord2f(1.0, 0.0);
    glVertex3fv(backgroundVerts[3]);
    
    //side4
    glTexCoord2f(0.0, 0.0);
    glVertex3fv(backgroundVerts[3]);
    glTexCoord2f(0.0, 1.0);
    glVertex3fv(backgroundVerts[7]);
    glTexCoord2f(1.0, 1.0);
    glVertex3fv(backgroundVerts[4]);
    glTexCoord2f(1.0, 0.0);
    glVertex3fv(backgroundVerts[0]);
    

    glEnd();
    

    DrawMountain();
    
    BuildMountainRange();
    
    DrawSkiers();
    BuildLift();
    SkiGates();
    SkatingRink();
    
    if (takeOver == 1) {
        TeapotTakeover();
    }
  
	glFlush();
	glutSwapBuffers(); /*Display next buffer*/
}


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	/*Note the GLUT_DOUBLE and GLUT_DEPTH here*/
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1000,1000);
	glutInitWindowPosition(50,50);
	glutCreateWindow("McCooey Mountain");
    makeMenus();
    glutDisplayFunc(display);
    glutKeyboardFunc(keys);
    glutIdleFunc(SkiandSkate);
    glEnable(GL_DEPTH_TEST); /*For hidden surface removal*/
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    readPPM(SKY);
    generateTextures(0);
    readPPM(SNOW);
    generateTextures(1);
    readPPM(MTREES);
    generateTextures(2);
    readPPM(RANGE);
    generateTextures(3);
    readPPM(LTEXT);
    generateTextures(4);
    readPPM(ROAD);
    generateTextures(5);
    readPPM(BLUE);
    generateTextures(6);
    readPPM(RED);
    generateTextures(7);
    readPPM(ICE);
    generateTextures(8);
    readPPM(GRASS);
    generateTextures(9);
	
    // cout statements for keyboard functions and menus
    
    printf("Welcome to McCooey Mountain!\n");
    printf("Keyboard Functions:\n");
    printf("*Use the shift of the command to escape from it ex: (R to escape from rink view) \n");
    printf("k: side view\n");
    printf("r: rink view\n");
    printf("v: top view\n");
    printf("t: TEAPOT TAKEOVER\n");
    
    printf("Right click for more menu options!\n");
           
    
    
	myinit();
	glutMainLoop();
	
	return 0;
}

void readPPM( char *fname) {
	FILE *fp;
	char c;
	char b[70];
	int i, n, m, k, nm;
	int count;
	float s;
	GLubyte red, green, blue;
    
    /*Code to read in a PPM file */
	printf("loading PPM textures: %s\n", fname);
	fflush(stdout);
	fp = fopen(fname, "rb");
	if (fp == NULL)
		printf("Cannot open file: %s\n",fname);
    
	fscanf(fp, "%s", b);
	//printf("%s\n", b);
	
	if (b[0] != 'P' || b[1] != '6') {
		printf("%s is not a PPM v. 6 file!\n", fname);
		exit(0);
	}
	fscanf(fp, "%c", &c);
	fscanf(fp, "%c", &c);
	
	while(c == '#') {
		count = 0;
		fscanf(fp, "%c", &c);
		while ((c != '\n') && (count < 100)){
			fscanf(fp, "%c", &c);
			count += 1;
		}
	}
	
	ungetc(c, fp);
	fscanf(fp, "%c", &c);
	fscanf(fp, "%d %d %d", &n, &m, &k);
	printf("n = %d, m = %d, k = %d\n", n, m, k);
	nm = n * m;
	imageData = malloc(3 * sizeof(GLubyte)*nm);
	
	fgetc(fp);
	if (k <= 255) {
		for (i = 0; i < nm; i++) {
			imageData[3*nm - 3*i - 3] = fgetc(fp);
			imageData[3*nm - 3*i - 2] = fgetc(fp);
			imageData[3*nm - 3*i - 1] = fgetc(fp);
		}
	} else {
		printf("Data error. k>255 \n");
	}
    
	width = m;
	height = n;
	fclose(fp);
	printf(".\n");
	/*End code to read in PPM file*/
}

void generateTextures(int IDnum ) {
	glGenTextures(1, &imageID[IDnum]);
	glBindTexture(GL_TEXTURE_2D, imageID[IDnum]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, imageData);
	free(imageData);
    
}

/* hemisphere function which I got from http://www.opengl.org/discussion_boards/showthread.php/138308-Please-help-with-drawing-a-dome
 
 */

void hemiSphere(float radx,float rady,float radz)
{
    float i,j,k;
    float x,y,z,t1x,t1y,t1z,t2x,t2y,t2z;
    float RAD = PI/180;
    float side = 20.0;
    glBegin(GL_QUADS);
    
        for(i=0;i<91;i+=side)
        {
            for(j=0;j<359;j+=side)
            {
                t1x=(sin(j*RAD)*cos(i*RAD) + sin((j+side)*RAD)*cos((i+side)*RAD))/2;
                t1y=(sin(i*RAD) + sin((i+side)*RAD))/2;
                t1z=(cos(j*RAD)*cos(i*RAD) + cos((j+side)*RAD)*cos((i+side)*RAD))/2;
                
                t2x=(sin((j+side)*RAD)*cos(i*RAD) + sin(j*RAD)*cos((i+side)*RAD))/2;
                t2y=(sin(i*RAD) + sin((i+side)*RAD))/2;
                t2z=(cos((j+side)*RAD)*cos(i*RAD) + cos(j*RAD)*cos((i+side)*RAD))/2;
                
                glNormal3f((t1x+t2x)/2,(t1y+t2y)/2,(t1z+t2z)/2);
                glTexCoord2f(0.0, 0.0);
                glVertex3f(radx*sin(j*RAD)*cos(i*RAD),rady*sin(i*RAD),radz*cos(j*RAD)*cos(i*RAD));
                glTexCoord2f(0.0, 1.0);
                glVertex3f(radx*sin((j+side)*RAD)*cos(i*RAD),rady* sin(i*RAD),radz*cos((j+side)*RAD)*cos(i *RAD));
                glTexCoord2f(1.0, 0.0);
                glVertex3f(radx*sin((j+side)*RAD)*cos((i+side)*RAD ),rady*sin((i+side)*RAD),radz*cos((j+side)*RAD)*cos((i+side)*RAD));
                glTexCoord2f(1.0, 1.0);
                glVertex3f(radx*sin(j*RAD)*cos((i+side)*RAD),rady* sin((i+side)*RAD),radz*cos(j*RAD)*cos(( i+side)*RAD));
            }
        }
    
    glEnd();
}

/* Keys function for the user interface */

void keys(unsigned char key, int x, int y)
{
    
    
    
    if (key == 't') {
        takeOver = 1;
    }
    if (key == 'T') {
        takeOver = 0;
    }
    if (key == 'v') {
        topView = 1;
        rinkView = 0;
        sideView = 0;
    }
    if (key == 'V') {
        topView = 0;
    }
    
    if (key == 'r') {
        rinkView = 1;
        topView = 0;
        sideView = 0;
    }
    if (key == 'R') {
        rinkView = 0;
    }
    
    if (key == 'k') {
        sideView = 1;
        rinkView = 0;
        topView = 0;
    }
    if (key == 'K') {
        sideView = 0;
    }
    display();
  

    
}

// sub menu for the color selection of the teapot
void color_menu(int id) {
    switch(id) {
        case 1:
            colorId = 1;
            break;
        case 2:
            colorId = 2;
            break;
        case 3:
            colorId = 3;
            break;
    
    }
    display();
}

//right menu for the right mouse key

void right_menu(int id) {
    if (id == 1) {
        exit(0);
    }
    
} //end

void makeMenus(void) {
    int c_menu;
    c_menu = glutCreateMenu(color_menu);
    glutAddMenuEntry("Blue", 1);
    glutAddMenuEntry("Pink", 2);
    glutAddMenuEntry("Yellow", 3);
    
    glutCreateMenu(right_menu);
    glutAddMenuEntry("Quit", 1);
    glutAddSubMenu("Colors", c_menu);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
} //end


//draws a mountain using a cylinder and a hemsiphere

void DrawMountain() {
    
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, imageID[1]);
    glPushMatrix();
    glScalef(1.0, 1.2, 1.0);
    glTranslatef(0.0, -10.0, -70.0);
    glRotatef(-90.0, 1.0, 0.0, 0.0);
    gluQuadricTexture(mountain, GL_TRUE);
    gluCylinder(mountain, mountainBase + 5.0, mountainTop, mountainHeight, 100.0, 100.0);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0.0, mountainHeight + 7.0, -70.0);
    hemiSphere(mountainTop, mountainTop, mountainTop);
    glPopMatrix();
    
       }


//builds the mountain range behind the skiing mountain using cylinders and texturemapping

void BuildMountainRange() {
    
    
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, imageID[2]);
    glPushMatrix();
    glTranslatef(-70.0, 0.0, -100.0);
    glRotatef(-90.0, 1.0, 0.0, 0.0);
    gluQuadricTexture(range1, GL_TRUE);
    gluCylinder(range1, mountainBase - 10.0, mountainTop, mountainHeight - 10.0, 100.0, 100.0);
    glPopMatrix();
    
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, imageID[2]);
    glPushMatrix();
    glTranslatef(-40.0, 0.0, -125.0);
    glRotatef(-90.0, 1.0, 0.0, 0.0);
    gluQuadricTexture(range1, GL_TRUE);
    gluCylinder(range1, mountainBase - 10.0, mountainTop, mountainHeight - 20.0, 100.0, 100.0);
    glPopMatrix();
    
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, imageID[2]);
    glPushMatrix();
    glTranslatef(-115.0, 0.0, -120.0);
    glRotatef(-90.0, 1.0, 0.0, 0.0);
    gluQuadricTexture(range1, GL_TRUE);
    gluCylinder(range1, mountainBase - 10.0, mountainTop, mountainHeight, 100.0, 100.0);
    glPopMatrix();
    
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, imageID[2]);
    glPushMatrix();
    glTranslatef(40.0, 0.0, -125.0);
    glRotatef(-90.0, 1.0, 0.0, 0.0);
    gluQuadricTexture(range2, GL_TRUE);
    gluCylinder(range2, mountainBase - 10.0, mountainTop, mountainHeight - 20.0, 100.0, 100.0);
    glPopMatrix();
    
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, imageID[2]);
    glPushMatrix();
    glTranslatef(75.0, 0.0, -100.0);
    glRotatef(-90.0, 1.0, 0.0, 0.0);
    gluQuadricTexture(range2, GL_TRUE);
    gluCylinder(range2, mountainBase - 10.0, mountainTop, mountainHeight - 10.0, 100.0, 100.0);
    glPopMatrix();
    
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, imageID[2]);
    glPushMatrix();
    glTranslatef(115.0, 0.0, -120.0);
    glRotatef(-90.0, 1.0, 0.0, 0.0);
    gluQuadricTexture(range1, GL_TRUE);
    gluCylinder(range1, mountainBase - 10.0, mountainTop, mountainHeight, 100.0, 100.0);
    glPopMatrix();
    
}


//builds the ski lift using two rectangular boxes

void BuildLift() {
    
    GLfloat liftHeight = mountainHeight + 10.0;
    GLfloat liftWidth = 6.0;
    GLfloat carpetWidth = liftWidth - 3.0;
    
    point3 magicCarpetVerts[8] = { {-liftWidth, 0.0, -liftWidth}, {liftWidth, liftHeight, -liftWidth}, {liftWidth, 0.0, liftWidth}, {-liftWidth, 0.0, liftWidth},
        {-liftWidth, liftHeight, -liftWidth}, {liftWidth, liftHeight, -liftWidth}, {liftWidth, liftHeight, liftWidth}, {-liftWidth, liftHeight, liftWidth} };
    
    point3 carpetVerts[8] = { {-carpetWidth, 0.0, -carpetWidth}, {carpetWidth, liftHeight, -carpetWidth}, {carpetWidth, 0.0, carpetWidth}, {-carpetWidth, 0.0, carpetWidth},
        {-carpetWidth, liftHeight, -carpetWidth}, {carpetWidth, liftHeight, -carpetWidth}, {carpetWidth, liftHeight, carpetWidth}, {-carpetWidth, liftHeight, carpetWidth} };
    
  
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_TEXTURE_2D);
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, imageID[4]);
    glTranslatef(-mountainBase + 1.0, 0.0, -70.0);
    glRotatef(-20, 0.0, 0.0, 1.0);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0);
    glVertex3fv(magicCarpetVerts[0]);
    glTexCoord2f(0.0, 1.0);
    glVertex3fv(magicCarpetVerts[1]);
    glTexCoord2f(1.0, 0.0);
    glVertex3fv(magicCarpetVerts[2]);
    glTexCoord2f(1.0, 1.0);
    glVertex3fv(magicCarpetVerts[3]);
    
    glTexCoord2f(0.0, 0.0);
    glVertex3fv(magicCarpetVerts[4]);
    glTexCoord2f(0.0, 1.0);
    glVertex3fv(magicCarpetVerts[5]);
    glTexCoord2f(1.0, 0.0);
    glVertex3fv(magicCarpetVerts[6]);
    glTexCoord2f(1.0, 1.0);
    glVertex3fv(magicCarpetVerts[7]);
    
    glTexCoord2f(0.0, 0.0);
    glVertex3fv(magicCarpetVerts[0]);
    glTexCoord2f(0.0, 1.0);
    glVertex3fv(magicCarpetVerts[4]);
    glTexCoord2f(1.0, 0.0);
    glVertex3fv(magicCarpetVerts[5]);
    glTexCoord2f(1.0, 1.0);
    glVertex3fv(magicCarpetVerts[1]);
    
    glTexCoord2f(0.0, 0.0);
    glVertex3fv(magicCarpetVerts[1]);
    glTexCoord2f(0.0, 1.0);
    glVertex3fv(magicCarpetVerts[5]);
    glTexCoord2f(1.0, 0.0);
    glVertex3fv(magicCarpetVerts[6]);
    glTexCoord2f(1.0, 1.0);
    glVertex3fv(magicCarpetVerts[2]);
    
    glTexCoord2f(0.0, 0.0);
    glVertex3fv(magicCarpetVerts[2]);
    glTexCoord2f(0.0, 1.0);
    glVertex3fv(magicCarpetVerts[6]);
    glTexCoord2f(1.0, 0.0);
    glVertex3fv(magicCarpetVerts[7]);
    glTexCoord2f(1.0, 1.0);
    glVertex3fv(magicCarpetVerts[3]);
    
    glTexCoord2f(0.0, 0.0);
    glVertex3fv(magicCarpetVerts[3]);
    glTexCoord2f(0.0, 1.0);
    glVertex3fv(magicCarpetVerts[7]);
    glTexCoord2f(1.0, 0.0);
    glVertex3fv(magicCarpetVerts[4]);
    glTexCoord2f(1.0, 1.0);
    glVertex3fv(magicCarpetVerts[0]);
    
    glEnd();
    glPopMatrix();
    
    glPushMatrix();
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, imageID[5]);
    
    glRotatef(-20, 0.0, 0.0, 1.0);
    glTranslatef(-mountainBase - 0.8, -14.0, -70.0);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0);
    glVertex3fv(carpetVerts[0]);
    glTexCoord2f(0.0, 1.0);
    glVertex3fv(carpetVerts[1]);
    glTexCoord2f(1.0, 0.0);
    glVertex3fv(carpetVerts[2]);
    glTexCoord2f(1.0, 1.0);
    glVertex3fv(carpetVerts[3]);
    
    glTexCoord2f(0.0, 0.0);
    glVertex3fv(carpetVerts[4]);
    glTexCoord2f(0.0, 1.0);
    glVertex3fv(carpetVerts[5]);
    glTexCoord2f(1.0, 0.0);
    glVertex3fv(carpetVerts[6]);
    glTexCoord2f(1.0, 1.0);
    glVertex3fv(carpetVerts[7]);
    
    glTexCoord2f(0.0, 0.0);
    glVertex3fv(carpetVerts[0]);
    glTexCoord2f(0.0, 1.0);
    glVertex3fv(carpetVerts[4]);
    glTexCoord2f(1.0, 0.0);
    glVertex3fv(carpetVerts[5]);
    glTexCoord2f(1.0, 1.0);
    glVertex3fv(carpetVerts[1]);
    
    glTexCoord2f(0.0, 0.0);
    glVertex3fv(carpetVerts[1]);
    glTexCoord2f(0.0, 1.0);
    glVertex3fv(carpetVerts[5]);
    glTexCoord2f(1.0, 0.0);
    glVertex3fv(carpetVerts[6]);
    glTexCoord2f(1.0, 1.0);
    glVertex3fv(carpetVerts[2]);
    
    glTexCoord2f(0.0, 0.0);
    glVertex3fv(carpetVerts[2]);
    glTexCoord2f(0.0, 1.0);
    glVertex3fv(carpetVerts[6]);
    glTexCoord2f(1.0, 0.0);
    glVertex3fv(carpetVerts[7]);
    glTexCoord2f(1.0, 1.0);
    glVertex3fv(carpetVerts[3]);
    
    glTexCoord2f(0.0, 0.0);
    glVertex3fv(carpetVerts[3]);
    glTexCoord2f(0.0, 1.0);
    glVertex3fv(carpetVerts[7]);
    glTexCoord2f(1.0, 0.0);
    glVertex3fv(carpetVerts[4]);
    glTexCoord2f(1.0, 1.0);
    glVertex3fv(carpetVerts[0]);
    
    glEnd();

    glPopMatrix();
    
    

    
    
    
}

//builds the ski gates out of rectangular boxes and places them down the mountain

void SkiGates() {
    point3 gateVerts[8] = {{-gateWidth, -gateHeight, -gateWidth}, {gateWidth, gateHeight, -gateWidth}, {gateWidth, -gateHeight, gateWidth}, {-gateWidth, -gateHeight, gateWidth},
        {-gateWidth, gateHeight, -gateWidth}, {gateWidth, -gateHeight, -gateWidth}, {gateWidth, gateHeight, gateWidth}, {-gateWidth, gateHeight, gateWidth} };
    
    glDisable(GL_TEXTURE_2D);
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, imageID[6]);
    glEnable(GL_AUTO_NORMAL);
    glTranslatef(-1.5, mountainHeight - 5.0, mountainTop - 65.0);
    glRotatef(-90.0, 1.0, 0.0, 0.0);
    glBegin(GL_QUADS);
    
    
    glTexCoord2f(0.0, 0.0);
    glVertex3fv(gateVerts[3]);
    glTexCoord2f(0.0, 1.0);
    glVertex3fv(gateVerts[7]);
    glTexCoord2f(1.0, 0.0);
    glVertex3fv(gateVerts[6]);
    glTexCoord2f(1.0, 1.0);
    glVertex3fv(gateVerts[2]);
    
    glTexCoord2f(0.0, 0.0);
    glVertex3fv(gateVerts[1]);
    glTexCoord2f(0.0, 1.0);
    glVertex3fv(gateVerts[6]);
    glTexCoord2f(1.0, 0.0);
    glVertex3fv(gateVerts[7]);
    glTexCoord2f(1.0, 1.0);
    glVertex3fv(gateVerts[4]);
    
    glTexCoord2f(0.0, 0.0);
    glVertex3fv(gateVerts[0]);
    glTexCoord2f(0.0, 1.0);
    glVertex3fv(gateVerts[4]);
    glTexCoord2f(1.0, 0.0);
    glVertex3fv(gateVerts[7]);
    glTexCoord2f(1.0, 1.0);
    glVertex3fv(gateVerts[3]);
    
    glTexCoord2f(0.0, 0.0);
    glVertex3fv(gateVerts[2]);
    glTexCoord2f(0.0, 1.0);
    glVertex3fv(gateVerts[6]);
    glTexCoord2f(1.0, 0.0);
    glVertex3fv(gateVerts[5]);
    glTexCoord2f(1.0, 1.0);
    glVertex3fv(gateVerts[1]);
    
    glTexCoord2f(0.0, 0.0);
    glVertex3fv(gateVerts[0]);
    glTexCoord2f(0.0, 1.0);
    glVertex3fv(gateVerts[3]);
    glTexCoord2f(1.0, 0.0);
    glVertex3fv(gateVerts[2]);
    glTexCoord2f(1.0, 1.0);
    glVertex3fv(gateVerts[5]);
    
    glTexCoord2f(0.0, 0.0);
    glVertex3fv(gateVerts[0]);
    glTexCoord2f(0.0, 1.0);
    glVertex3fv(gateVerts[5]);
    glTexCoord2f(1.0, 0.0);
    glVertex3fv(gateVerts[1]);
    glTexCoord2f(1.0, 1.0);
    glVertex3fv(gateVerts[4]);
    
    glEnd();

    glPopMatrix();
    
    //gate2
    glPushMatrix();
    glDisable(GL_TEXTURE_2D);
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, imageID[7]);
    glTranslatef(2.5, mountainHeight - 20.0, mountainTop - 60.0);
    glRotatef(-90.0, 1.0, 0.0, 0.0);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0);
    glVertex3fv(gateVerts[3]);
    glTexCoord2f(0.0, 1.0);
    glVertex3fv(gateVerts[7]);
    glTexCoord2f(1.0, 0.0);
    glVertex3fv(gateVerts[6]);
    glTexCoord2f(1.0, 1.0);
    glVertex3fv(gateVerts[2]);
    
    glTexCoord2f(0.0, 0.0);
    glVertex3fv(gateVerts[1]);
    glTexCoord2f(0.0, 1.0);
    glVertex3fv(gateVerts[6]);
    glTexCoord2f(1.0, 0.0);
    glVertex3fv(gateVerts[7]);
    glTexCoord2f(1.0, 1.0);
    glVertex3fv(gateVerts[4]);
    
    glTexCoord2f(0.0, 0.0);
    glVertex3fv(gateVerts[0]);
    glTexCoord2f(0.0, 1.0);
    glVertex3fv(gateVerts[4]);
    glTexCoord2f(1.0, 0.0);
    glVertex3fv(gateVerts[7]);
    glTexCoord2f(1.0, 1.0);
    glVertex3fv(gateVerts[3]);
    
    glTexCoord2f(0.0, 0.0);
    glVertex3fv(gateVerts[2]);
    glTexCoord2f(0.0, 1.0);
    glVertex3fv(gateVerts[6]);
    glTexCoord2f(1.0, 0.0);
    glVertex3fv(gateVerts[5]);
    glTexCoord2f(1.0, 1.0);
    glVertex3fv(gateVerts[1]);
    
    glTexCoord2f(0.0, 0.0);
    glVertex3fv(gateVerts[0]);
    glTexCoord2f(0.0, 1.0);
    glVertex3fv(gateVerts[3]);
    glTexCoord2f(1.0, 0.0);
    glVertex3fv(gateVerts[2]);
    glTexCoord2f(1.0, 1.0);
    glVertex3fv(gateVerts[5]);
    
    glTexCoord2f(0.0, 0.0);
    glVertex3fv(gateVerts[0]);
    glTexCoord2f(0.0, 1.0);
    glVertex3fv(gateVerts[5]);
    glTexCoord2f(1.0, 0.0);
    glVertex3fv(gateVerts[1]);
    glTexCoord2f(1.0, 1.0);
    glVertex3fv(gateVerts[4]);
    
    glEnd();
    glPopMatrix();
    
    //gate 3
    glPushMatrix();
    glDisable(GL_TEXTURE_2D);
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, imageID[6]);
    glTranslatef(-2.5, mountainHeight - 33.0, mountainTop -55.0);
    glRotatef(-90.0, 1.0, 0.0, 0.0);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0);
    glVertex3fv(gateVerts[3]);
    glTexCoord2f(0.0, 1.0);
    glVertex3fv(gateVerts[7]);
    glTexCoord2f(1.0, 0.0);
    glVertex3fv(gateVerts[6]);
    glTexCoord2f(1.0, 1.0);
    glVertex3fv(gateVerts[2]);
    
    glTexCoord2f(0.0, 0.0);
    glVertex3fv(gateVerts[1]);
    glTexCoord2f(0.0, 1.0);
    glVertex3fv(gateVerts[6]);
    glTexCoord2f(1.0, 0.0);
    glVertex3fv(gateVerts[7]);
    glTexCoord2f(1.0, 1.0);
    glVertex3fv(gateVerts[4]);
    
    glTexCoord2f(0.0, 0.0);
    glVertex3fv(gateVerts[0]);
    glTexCoord2f(0.0, 1.0);
    glVertex3fv(gateVerts[4]);
    glTexCoord2f(1.0, 0.0);
    glVertex3fv(gateVerts[7]);
    glTexCoord2f(1.0, 1.0);
    glVertex3fv(gateVerts[3]);
    
    glTexCoord2f(0.0, 0.0);
    glVertex3fv(gateVerts[2]);
    glTexCoord2f(0.0, 1.0);
    glVertex3fv(gateVerts[6]);
    glTexCoord2f(1.0, 0.0);
    glVertex3fv(gateVerts[5]);
    glTexCoord2f(1.0, 1.0);
    glVertex3fv(gateVerts[1]);
    
    glTexCoord2f(0.0, 0.0);
    glVertex3fv(gateVerts[0]);
    glTexCoord2f(0.0, 1.0);
    glVertex3fv(gateVerts[3]);
    glTexCoord2f(1.0, 0.0);
    glVertex3fv(gateVerts[2]);
    glTexCoord2f(1.0, 1.0);
    glVertex3fv(gateVerts[5]);
    
    glTexCoord2f(0.0, 0.0);
    glVertex3fv(gateVerts[0]);
    glTexCoord2f(0.0, 1.0);
    glVertex3fv(gateVerts[5]);
    glTexCoord2f(1.0, 0.0);
    glVertex3fv(gateVerts[1]);
    glTexCoord2f(1.0, 1.0);
    glVertex3fv(gateVerts[4]);
    
    glEnd();
    glPopMatrix();
    
    //gate 4
    glPushMatrix();
    glDisable(GL_TEXTURE_2D);
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, imageID[7]);
    glTranslatef(3.5, mountainHeight - 44.0, mountainTop - 52.0);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0);
    glVertex3fv(gateVerts[3]);
    glTexCoord2f(0.0, 1.0);
    glVertex3fv(gateVerts[7]);
    glTexCoord2f(1.0, 0.0);
    glVertex3fv(gateVerts[6]);
    glTexCoord2f(1.0, 1.0);
    glVertex3fv(gateVerts[2]);
    
    glTexCoord2f(0.0, 0.0);
    glVertex3fv(gateVerts[1]);
    glTexCoord2f(0.0, 1.0);
    glVertex3fv(gateVerts[6]);
    glTexCoord2f(1.0, 0.0);
    glVertex3fv(gateVerts[7]);
    glTexCoord2f(1.0, 1.0);
    glVertex3fv(gateVerts[4]);
    
    glTexCoord2f(0.0, 0.0);
    glVertex3fv(gateVerts[0]);
    glTexCoord2f(0.0, 1.0);
    glVertex3fv(gateVerts[4]);
    glTexCoord2f(1.0, 0.0);
    glVertex3fv(gateVerts[7]);
    glTexCoord2f(1.0, 1.0);
    glVertex3fv(gateVerts[3]);
    
    glTexCoord2f(0.0, 0.0);
    glVertex3fv(gateVerts[2]);
    glTexCoord2f(0.0, 1.0);
    glVertex3fv(gateVerts[6]);
    glTexCoord2f(1.0, 0.0);
    glVertex3fv(gateVerts[5]);
    glTexCoord2f(1.0, 1.0);
    glVertex3fv(gateVerts[1]);
    
    glTexCoord2f(0.0, 0.0);
    glVertex3fv(gateVerts[0]);
    glTexCoord2f(0.0, 1.0);
    glVertex3fv(gateVerts[3]);
    glTexCoord2f(1.0, 0.0);
    glVertex3fv(gateVerts[2]);
    glTexCoord2f(1.0, 1.0);
    glVertex3fv(gateVerts[5]);
    
    glTexCoord2f(0.0, 0.0);
    glVertex3fv(gateVerts[0]);
    glTexCoord2f(0.0, 1.0);
    glVertex3fv(gateVerts[5]);
    glTexCoord2f(1.0, 0.0);
    glVertex3fv(gateVerts[1]);
    glTexCoord2f(1.0, 1.0);
    glVertex3fv(gateVerts[4]);
    
    glEnd();
    glPopMatrix();
    
    //gate5
    glPushMatrix();
    glDisable(GL_TEXTURE_2D);
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, imageID[6]);
    glTranslatef(-3.0, mountainHeight - 55.0, mountainTop - 47.0);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0);
    glVertex3fv(gateVerts[3]);
    glTexCoord2f(0.0, 1.0);
    glVertex3fv(gateVerts[7]);
    glTexCoord2f(1.0, 0.0);
    glVertex3fv(gateVerts[6]);
    glTexCoord2f(1.0, 1.0);
    glVertex3fv(gateVerts[2]);
    
    glTexCoord2f(0.0, 0.0);
    glVertex3fv(gateVerts[1]);
    glTexCoord2f(0.0, 1.0);
    glVertex3fv(gateVerts[6]);
    glTexCoord2f(1.0, 0.0);
    glVertex3fv(gateVerts[7]);
    glTexCoord2f(1.0, 1.0);
    glVertex3fv(gateVerts[4]);
    
    glTexCoord2f(0.0, 0.0);
    glVertex3fv(gateVerts[0]);
    glTexCoord2f(0.0, 1.0);
    glVertex3fv(gateVerts[4]);
    glTexCoord2f(1.0, 0.0);
    glVertex3fv(gateVerts[7]);
    glTexCoord2f(1.0, 1.0);
    glVertex3fv(gateVerts[3]);
    
    glTexCoord2f(0.0, 0.0);
    glVertex3fv(gateVerts[2]);
    glTexCoord2f(0.0, 1.0);
    glVertex3fv(gateVerts[6]);
    glTexCoord2f(1.0, 0.0);
    glVertex3fv(gateVerts[5]);
    glTexCoord2f(1.0, 1.0);
    glVertex3fv(gateVerts[1]);
    
    glTexCoord2f(0.0, 0.0);
    glVertex3fv(gateVerts[0]);
    glTexCoord2f(0.0, 1.0);
    glVertex3fv(gateVerts[3]);
    glTexCoord2f(1.0, 0.0);
    glVertex3fv(gateVerts[2]);
    glTexCoord2f(1.0, 1.0);
    glVertex3fv(gateVerts[5]);
    
    glTexCoord2f(0.0, 0.0);
    glVertex3fv(gateVerts[0]);
    glTexCoord2f(0.0, 1.0);
    glVertex3fv(gateVerts[5]);
    glTexCoord2f(1.0, 0.0);
    glVertex3fv(gateVerts[1]);
    glTexCoord2f(1.0, 1.0);
    glVertex3fv(gateVerts[4]);
    
    glEnd();
    glPopMatrix();
    
    //gate 6
    glPushMatrix();
    glDisable(GL_TEXTURE_2D);
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, imageID[7]);
    glTranslatef(2.5, mountainHeight - 67.0, mountainTop - 43.0);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0);
    glVertex3fv(gateVerts[3]);
    glTexCoord2f(0.0, 1.0);
    glVertex3fv(gateVerts[7]);
    glTexCoord2f(1.0, 0.0);
    glVertex3fv(gateVerts[6]);
    glTexCoord2f(1.0, 1.0);
    glVertex3fv(gateVerts[2]);
    
    glTexCoord2f(0.0, 0.0);
    glVertex3fv(gateVerts[1]);
    glTexCoord2f(0.0, 1.0);
    glVertex3fv(gateVerts[6]);
    glTexCoord2f(1.0, 0.0);
    glVertex3fv(gateVerts[7]);
    glTexCoord2f(1.0, 1.0);
    glVertex3fv(gateVerts[4]);
    
    glTexCoord2f(0.0, 0.0);
    glVertex3fv(gateVerts[0]);
    glTexCoord2f(0.0, 1.0);
    glVertex3fv(gateVerts[4]);
    glTexCoord2f(1.0, 0.0);
    glVertex3fv(gateVerts[7]);
    glTexCoord2f(1.0, 1.0);
    glVertex3fv(gateVerts[3]);
    
    glTexCoord2f(0.0, 0.0);
    glVertex3fv(gateVerts[2]);
    glTexCoord2f(0.0, 1.0);
    glVertex3fv(gateVerts[6]);
    glTexCoord2f(1.0, 0.0);
    glVertex3fv(gateVerts[5]);
    glTexCoord2f(1.0, 1.0);
    glVertex3fv(gateVerts[1]);
    
    glTexCoord2f(0.0, 0.0);
    glVertex3fv(gateVerts[0]);
    glTexCoord2f(0.0, 1.0);
    glVertex3fv(gateVerts[3]);
    glTexCoord2f(1.0, 0.0);
    glVertex3fv(gateVerts[2]);
    glTexCoord2f(1.0, 1.0);
    glVertex3fv(gateVerts[5]);
    
    glTexCoord2f(0.0, 0.0);
    glVertex3fv(gateVerts[0]);
    glTexCoord2f(0.0, 1.0);
    glVertex3fv(gateVerts[5]);
    glTexCoord2f(1.0, 0.0);
    glVertex3fv(gateVerts[1]);
    glTexCoord2f(1.0, 1.0);
    glVertex3fv(gateVerts[4]);
    
    glEnd();
    glPopMatrix();
    
    //gate7
    glPushMatrix();
    glDisable(GL_TEXTURE_2D);
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, imageID[6]);
    glTranslatef(-2.3, mountainHeight - 78.0, mountainTop - 40.0);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0);
    glVertex3fv(gateVerts[3]);
    glTexCoord2f(0.0, 1.0);
    glVertex3fv(gateVerts[7]);
    glTexCoord2f(1.0, 0.0);
    glVertex3fv(gateVerts[6]);
    glTexCoord2f(1.0, 1.0);
    glVertex3fv(gateVerts[2]);
    
    glTexCoord2f(0.0, 0.0);
    glVertex3fv(gateVerts[1]);
    glTexCoord2f(0.0, 1.0);
    glVertex3fv(gateVerts[6]);
    glTexCoord2f(1.0, 0.0);
    glVertex3fv(gateVerts[7]);
    glTexCoord2f(1.0, 1.0);
    glVertex3fv(gateVerts[4]);
    
    glTexCoord2f(0.0, 0.0);
    glVertex3fv(gateVerts[0]);
    glTexCoord2f(0.0, 1.0);
    glVertex3fv(gateVerts[4]);
    glTexCoord2f(1.0, 0.0);
    glVertex3fv(gateVerts[7]);
    glTexCoord2f(1.0, 1.0);
    glVertex3fv(gateVerts[3]);
    
    glTexCoord2f(0.0, 0.0);
    glVertex3fv(gateVerts[2]);
    glTexCoord2f(0.0, 1.0);
    glVertex3fv(gateVerts[6]);
    glTexCoord2f(1.0, 0.0);
    glVertex3fv(gateVerts[5]);
    glTexCoord2f(1.0, 1.0);
    glVertex3fv(gateVerts[1]);
    
    glTexCoord2f(0.0, 0.0);
    glVertex3fv(gateVerts[0]);
    glTexCoord2f(0.0, 1.0);
    glVertex3fv(gateVerts[3]);
    glTexCoord2f(1.0, 0.0);
    glVertex3fv(gateVerts[2]);
    glTexCoord2f(1.0, 1.0);
    glVertex3fv(gateVerts[5]);
    
    glTexCoord2f(0.0, 0.0);
    glVertex3fv(gateVerts[0]);
    glTexCoord2f(0.0, 1.0);
    glVertex3fv(gateVerts[5]);
    glTexCoord2f(1.0, 0.0);
    glVertex3fv(gateVerts[1]);
    glTexCoord2f(1.0, 1.0);
    glVertex3fv(gateVerts[4]);
    
    glEnd();
    glPopMatrix();
    
    //gate 8
    glPushMatrix();
    glDisable(GL_TEXTURE_2D);
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, imageID[7]);
    glTranslatef(3.0, mountainHeight - 90.0, mountainTop - 35.0);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0);
    glVertex3fv(gateVerts[3]);
    glTexCoord2f(0.0, 1.0);
    glVertex3fv(gateVerts[7]);
    glTexCoord2f(1.0, 0.0);
    glVertex3fv(gateVerts[6]);
    glTexCoord2f(1.0, 1.0);
    glVertex3fv(gateVerts[2]);
    
    glTexCoord2f(0.0, 0.0);
    glVertex3fv(gateVerts[1]);
    glTexCoord2f(0.0, 1.0);
    glVertex3fv(gateVerts[6]);
    glTexCoord2f(1.0, 0.0);
    glVertex3fv(gateVerts[7]);
    glTexCoord2f(1.0, 1.0);
    glVertex3fv(gateVerts[4]);
    
    glTexCoord2f(0.0, 0.0);
    glVertex3fv(gateVerts[0]);
    glTexCoord2f(0.0, 1.0);
    glVertex3fv(gateVerts[4]);
    glTexCoord2f(1.0, 0.0);
    glVertex3fv(gateVerts[7]);
    glTexCoord2f(1.0, 1.0);
    glVertex3fv(gateVerts[3]);
    
    glTexCoord2f(0.0, 0.0);
    glVertex3fv(gateVerts[2]);
    glTexCoord2f(0.0, 1.0);
    glVertex3fv(gateVerts[6]);
    glTexCoord2f(1.0, 0.0);
    glVertex3fv(gateVerts[5]);
    glTexCoord2f(1.0, 1.0);
    glVertex3fv(gateVerts[1]);
    
    glTexCoord2f(0.0, 0.0);
    glVertex3fv(gateVerts[0]);
    glTexCoord2f(0.0, 1.0);
    glVertex3fv(gateVerts[3]);
    glTexCoord2f(1.0, 0.0);
    glVertex3fv(gateVerts[2]);
    glTexCoord2f(1.0, 1.0);
    glVertex3fv(gateVerts[5]);
    
    glTexCoord2f(0.0, 0.0);
    glVertex3fv(gateVerts[0]);
    glTexCoord2f(0.0, 1.0);
    glVertex3fv(gateVerts[5]);
    glTexCoord2f(1.0, 0.0);
    glVertex3fv(gateVerts[1]);
    glTexCoord2f(1.0, 1.0);
    glVertex3fv(gateVerts[4]);
    
    glEnd();
    glPopMatrix();
    
    //gate9
    glPushMatrix();
    glDisable(GL_TEXTURE_2D);
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, imageID[6]);
    glTranslatef(-2.5, mountainHeight - 101.0, mountainTop - 31.5);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0);
    glVertex3fv(gateVerts[3]);
    glTexCoord2f(0.0, 1.0);
    glVertex3fv(gateVerts[7]);
    glTexCoord2f(1.0, 0.0);
    glVertex3fv(gateVerts[6]);
    glTexCoord2f(1.0, 1.0);
    glVertex3fv(gateVerts[2]);
    
    glTexCoord2f(0.0, 0.0);
    glVertex3fv(gateVerts[1]);
    glTexCoord2f(0.0, 1.0);
    glVertex3fv(gateVerts[6]);
    glTexCoord2f(1.0, 0.0);
    glVertex3fv(gateVerts[7]);
    glTexCoord2f(1.0, 1.0);
    glVertex3fv(gateVerts[4]);
    
    glTexCoord2f(0.0, 0.0);
    glVertex3fv(gateVerts[0]);
    glTexCoord2f(0.0, 1.0);
    glVertex3fv(gateVerts[4]);
    glTexCoord2f(1.0, 0.0);
    glVertex3fv(gateVerts[7]);
    glTexCoord2f(1.0, 1.0);
    glVertex3fv(gateVerts[3]);
    
    glTexCoord2f(0.0, 0.0);
    glVertex3fv(gateVerts[2]);
    glTexCoord2f(0.0, 1.0);
    glVertex3fv(gateVerts[6]);
    glTexCoord2f(1.0, 0.0);
    glVertex3fv(gateVerts[5]);
    glTexCoord2f(1.0, 1.0);
    glVertex3fv(gateVerts[1]);
    
    glTexCoord2f(0.0, 0.0);
    glVertex3fv(gateVerts[0]);
    glTexCoord2f(0.0, 1.0);
    glVertex3fv(gateVerts[3]);
    glTexCoord2f(1.0, 0.0);
    glVertex3fv(gateVerts[2]);
    glTexCoord2f(1.0, 1.0);
    glVertex3fv(gateVerts[5]);
    
    glTexCoord2f(0.0, 0.0);
    glVertex3fv(gateVerts[0]);
    glTexCoord2f(0.0, 1.0);
    glVertex3fv(gateVerts[5]);
    glTexCoord2f(1.0, 0.0);
    glVertex3fv(gateVerts[1]);
    glTexCoord2f(1.0, 1.0);
    glVertex3fv(gateVerts[4]);
    
    glEnd();
    glPopMatrix();



}


//draws the teapot that's skiing

void DrawSkiers() {
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_AUTO_NORMAL);
    glPushMatrix();
    glTranslatef(ski[0], ski[1], ski[2]);
    glTranslatef(0.0, mountainHeight, mountainTop/2.5 - 60.0);
    glRotatef(45.0, 1.0, 0.0, 0.0);
    glRotatef(-90.0, 0.0, 1.0, 0.0);
    glRotatef(zRotate, 0.0, 0.0, 1.0);
    glRotatef(endTurnAngle, 0.0, 1.0, 0.0);
    glRotatef(angle, 1.0, 0.0, 0.0);
    if (colorId == 1) {
        glColor3f(0.0, 0.0, 1.0);
    } else if (colorId == 2) {
        glColor3f(1.0, 0.0, 1.0);
    } else if (colorId == 3) {
        glColor3f(1.0, 1.0, 0.0);
    }
    glutSolidTeapot(1.0);
    glPopMatrix();
    glPopAttrib();
}


//function that creates the path of the teapot
void SkiingTeapot() {
  
    
    //used to go through the gates
    turn = 4*sin(turnAngle);
    
    if (ski[2] < 33.5 && endTrail < 2) {
        angle = 0.0;
        endTurnAngle = 0.0;
        if (turnCount == 4) {
            ski[0] = turn;
            turnCount = 0;
        } else {
            turnAngle += 25.0;
            turnCount++;
        }
        endTrail = 1;
        ski[2] += 0.125;
        ski[1] -= 0.4;
    } else {
        if (endTrail == 1) {
            turnAngle = 0.0;
            angle = 45.0;
            endTurnAngle = -90.0;
            endTrail = 2;
        } else {
            if (ski[0] >= -51.0 && endTrail < 3) {
                ski[0] -= 0.35;
                ski[2] -= 0.045;
            } else if (endTrail == 2){
                endTurnAngle -= 90.0;
                endTrail = 3;
            } else if (ski[2] > -9.9 && endTrail < 4) {
                ski[2] -= 0.2;
            } else if (endTrail == 3) {
                endTurnAngle += 90.0;
                zRotate = 0.0;
                angle -= 90.0;
                endTrail = 4;
            } else if (ski [1] <= mountainHeight - 100.0 && endTrail < 5) {
                ski[1] += 0.50;
                ski[0] += 0.18;
            } else if (endTrail == 4) {
                angle += 90.0;
                endTurnAngle += 180.0;
                zRotate -= 270.0;
                endTrail = 5;
            } else if (ski[0] < mountainTop - 5.0 && endTrail == 5) {
                ski[1] += 0.05;
                ski[0] += 0.15;

            } else if (endTrail == 5) {
                zRotate -= 90.0;
                endTrail = 6;
            } else if (endTrail < 10 && endTrail > 5) {
                endTrail++;
            } else if (endTrail == 10) {
                ski[0] = 0.0;
                ski[1] = 0.0;
                ski[2] = 0.0;
                zRotate = 0.0;
                angle = 0.0;
                endTurnAngle = 0.0;
                endTrail = 11;
            } else if (endTrail == 11) {
                endTrail = 0.0;
            }
            
            
        }
        
    }
    
    
  
    
}

//combines all of the idle functions

void SkiandSkate() {
    SkiingTeapot();
    SkatingTeapot();
    
    if (takeOver == 1) {
        TeapotCrush();
    } else if (takeOver == 0) {
        teapot[1] = 400.0;
    }
    glutPostRedisplay();
}



//draws the skating rink and the skaters on the rink

void SkatingRink() {
    glEnable(GL_AUTO_NORMAL);
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_TEXTURE_2D);
    gluQuadricTexture(iceRink, GL_TRUE);
    glBindTexture(GL_TEXTURE_2D, imageID[8]);
    glPushMatrix();
    glTranslatef(-65.0, 0.1, -2.0);
    glRotatef(90.0, 1.0, 0.0, 0.0);
    gluDisk(iceRink, 0.0, mountainBase - 15.0, 100.0, 100.0);
    
    glPushMatrix();
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_AUTO_NORMAL);
    glTranslatef(0.0, 0.15, -5.0);
    glRotatef(-90.0, 1.0, 0.0, 0.0);
    glRotatef(skateAngle, 0.0, 1.0, 0.0);
    glColor3f(1.0, 0.0, 0.0);
    glutSolidTeapot(2.5);
    glPopMatrix();
    glPopMatrix();
    glPopAttrib();
    
    glPushMatrix();
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_AUTO_NORMAL);
    glTranslatef(skate[0], skate[1], skate[2]);
    glTranslatef(-87.0, 0.4, 2.0);
    glRotatef(skateTurn, 0.0, 1.0, 0.0);
    glRotatef(90.0, 0.0, 1.0, 0.0);
    glColor3f(0.5, 0.0, 0.5);
    glutSolidTeapot(3.5);
    glPopMatrix();
    
}

//sub idle function for the skating teapots

void SkatingTeapot() {
    
    skateAngle += 1.0;
    
  if (skate[2] > -15 && skateCount == 0) {
        skate[2] -= 0.15;
    } else if (skateCount == 0) {
        skateTurn = -90.0;
        skateCount = 1;
    } else if (skate[0] < 45.0 && skateCount == 1) {
        skate[0] += 0.15;
    } else if (skateCount == 1) {
        skateTurn += -90.0;
        skateCount = 2;
    } else if (skate[2] < 7 && skateCount == 2) {
        skate[2] += 0.15;
    } else if (skateCount == 2) {
        skateTurn -= 90.0;
        skateCount = 3;
    } else if (skateCount == 3 && skate[0] > 0.0) {
        skate[0] -= 0.15;
    } else if (skateCount == 3) {
        skateTurn -= 90.0;
        skateCount = 0;
    }
   
    

}


//function for drawing the giant teapot
void TeapotTakeover() {
    glEnable(GL_AUTO_NORMAL);
    glTranslatef(teapot[0], teapot[1], teapot[2]);
    glTranslatef(0.0, 0.0, -70.0);
    glutSolidTeapot(120.0);
    
}

//idle function for putting teapot takeover into action
void TeapotCrush() {
    if (takeOver == 1 && teapot[1] > 20.0) {
        teapot[1] -= 12.0;
    }
}

