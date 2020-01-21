/*
 *  UAS CG 
 *  Alif Ramdani
 *  171511036
 *  3B/D3
 *
 *  m             Toggles light (sun) movement
 *  p             Toggles first person/perspective projection
 *  +/-           Change field of view of perspective
 *  x             Toggle axes display
 *  arrow keys    Change view angle
 *  PgDn/PgUp     Zoom in and out (in perspective view)
 *  0             Reset view angle
 *  ESC           Exit
 *
 */
#include "JTKPOLBAN.h"
#include "loadtexbmp.c"
#include "fatal.c"
#include "Errcheck.c"
#include "print.c"
#include "project.c"
#ifndef GL_CLAMP_TO_EDGE
#define GL_CLAMP_TO_EDGE 0x812F
//tambahan
#include <stdbool.h>
#define TURN_NORTHWEST 1
#define TURN_NORTHEAST 2
#define TURN_SOUTHEAST 3
#define TURN_SOUTHWEST 4
#define TURN_LEFT 5
#define TURN_RIGHT 6
#define BLUE_CAR 0
#define RED_CAR 1
#define BLACK_CAR 2
#define GREEN_CAR 3
#define CYAN_CAR 4
#define AMOUNT_OF_CARS 5
#define GAPURA_X 1
#define GAPURA_Y 2
#endif

int axes=0;       //  Display axes
int mode=0;       //  Projection mode
int move=1;       //  Move light
int th=110;         //  Azimuth of view angle
int ph=0;         //  Elevation of view angle
int fov=55;       //  Field of view (for perspective)
int light=1;      //  Lighting
double asp=1.333;  //  Aspect ratio
double dim=8.0;   //  Size of world

// Light values
int one       =   1;  // Unit value
int distance  =   25;  // Light distance
int inc       =  10;  // Ball increment
int smooth    =   1;  // Smooth/Flat shading
int local     =   0;  // Local Viewer Model
int emission  =  30;  // Emission intensity (%)
int ambient   =  80;  // Ambient intensity (%)
int diffuse   = 100;  // Diffuse intensity (%)
int specular  =   0;  // Specular intensity (%)
int shininess =   0;  // Shininess (power of two)
float shiny   =   1;  // Shininess (value)
int zh        =  90;  // Light azimuth
float ylight  = 13;  // Elevation of light

int at0=100;      //  Constant  attenuation %
int at1=20;        //  Linear    attenuation %
int at2=20;        //  Quadratic attenuation %

double fpMoveInc = 0.02; //Multiplier for how much to move each keystroke in FP mode

//First person camera location
double fpX = 0;
double fpY = 0.7;
double fpZ = -0.3;


//x, y, z for refrence point in glLookAt() for FP mode
double refX = 5;
double refY = 0;
double refZ = 0;

//Texture Variables
int tMode = 0;
float texScale = 0.5;
GLuint
_textureAsphalt,
_textureGrass,
_textureSkyboxBack,
_textureSkyboxFront,
_textureSkyboxLeft,
_textureSkyboxRight,
_textureSkyboxTop,
_textureStandWall,
_textureStandSeats,
_textureBasicMetal,
_textureGlass,
_textureCarWheel,
_textureTireTread,
_textureHeadLamp,
_textureCarbonFiber,
_textureCarGrill,
_textureFinishLine,
_textureRacingTrafficLight,
_textureGapuraFinishPilar,
_textureAdsMonster,
_textureAdsPertamina,
_textureAdsMotul

;

//Light Vecotrs
float Ambient[]   = {0.01*80 ,0.01*80 ,0.01*80 ,1.0};
float Diffuse[]   = {1.0,1.0,1.0,1.0};
float Specular[]  = {0.01*0,0.01*0,0.01*0,1.0};

//Car Movements
float xPositionCar[AMOUNT_OF_CARS] = {-1, -4, -6, -1, -11};
float zPositionCar[AMOUNT_OF_CARS] = {2.2, 1.2, 2.2, 1.2, 2.2};
int turnDegree[AMOUNT_OF_CARS] = {0,0,0,0,0};
//float xPositionCar = -1;
//float zPositionCar = 2.2;
//int turnDegree = 0;
int turnDegreeCount[AMOUNT_OF_CARS] = {80, 80, 80, 80, 80};
int turnStatus[AMOUNT_OF_CARS] = {0,0,0,0,0};
bool isTurnNorthwest[AMOUNT_OF_CARS] = {false, false, false, false, false};
bool isTurnNortheast[AMOUNT_OF_CARS] = {false, false, false, false, false};
bool isTurnSouthwest[AMOUNT_OF_CARS] = {false, false, false, false, false};
bool isTurnSoutheast[AMOUNT_OF_CARS] = {false, false, false, false, false};
bool isStright[AMOUNT_OF_CARS] = {true, true, true, true, true};

//UAS Hari H
bool pause = false;

//Supir person camera location
double spX = -4;
double spY = 0;
double spZ = 1.2;

/*
 *  Draw a cube
 *     at (x,y,z)
 *     dimensions (dx,dy,dz)
 *     rotated th about the y axis
 */
static void cube(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
{
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);

   //Texture repitition values
   float texRepX = 1.0;
   float texRepY = 1.0;

   //  Cube
   glBegin(GL_QUADS);
   //  Front
   texRepX = dx/texScale;
   texRepY = dy/texScale;
   glNormal3f( 0, 0, 1);
   glTexCoord2f(0.0,0.0); glVertex3f(-1,-1, 1);
   glTexCoord2f(texRepX,0.0); glVertex3f(+1,-1, 1);
   glTexCoord2f(texRepX,texRepY); glVertex3f(+1,+1, 1);
   glTexCoord2f(0.0,texRepY); glVertex3f(-1,+1, 1);
   //  Back
   texRepX = dx/texScale;
   texRepY = dy/texScale;
   glNormal3f( 0, 0,-1);
   glTexCoord2f(0.0,0.0); glVertex3f(+1,-1,-1);
   glTexCoord2f(texRepX,0.0); glVertex3f(-1,-1,-1);
   glTexCoord2f(texRepX,texRepY); glVertex3f(-1,+1,-1);
   glTexCoord2f(0.0,texRepY); glVertex3f(+1,+1,-1);
   //  Right
   texRepX = dz/texScale;
   texRepY = dy/texScale;
   glNormal3f(+1, 0, 0);
   glTexCoord2f(0.0,0.0); glVertex3f(+1,-1,+1);
   glTexCoord2f(texRepX,0.0); glVertex3f(+1,-1,-1);
   glTexCoord2f(texRepX,texRepY); glVertex3f(+1,+1,-1);
   glTexCoord2f(0.0,texRepY); glVertex3f(+1,+1,+1);
   //  Left
   texRepX = dz/texScale;
   texRepY = dy/texScale;
   glNormal3f(-1, 0, 0);
   glTexCoord2f(0.0,0.0); glVertex3f(-1,-1,-1);
   glTexCoord2f(texRepX,0.0); glVertex3f(-1,-1,+1);
   glTexCoord2f(texRepX,texRepY); glVertex3f(-1,+1,+1);
   glTexCoord2f(0.0,texRepY); glVertex3f(-1,+1,-1);
   //  Top
   texRepX = dx/texScale;
   texRepY = dz/texScale;
   glNormal3f( 0,+1, 0);
   glTexCoord2f(0.0,0.0); glVertex3f(-1,+1,+1);
   glTexCoord2f(texRepX,0.0); glVertex3f(+1,+1,+1);
   glTexCoord2f(texRepX,texRepY); glVertex3f(+1,+1,-1);
   glTexCoord2f(0.0,texRepY); glVertex3f(-1,+1,-1);
   //  Bottom
   texRepX = dx/texScale;
   texRepY = dz/texScale;
   glNormal3f( 0,-one, 0);
   glTexCoord2f(0.0,0.0); glVertex3f(-1,-1,-1);
   glTexCoord2f(texRepX,0.0); glVertex3f(+1,-1,-1);
   glTexCoord2f(texRepX,texRepY); glVertex3f(+1,-1,+1);
   glTexCoord2f(0.0,texRepY); glVertex3f(-1,-1,+1);
   //  End
   glEnd();
   //  Undo transofrmations
   glPopMatrix();
}

/*
 *  Draw a stands (Tribun)
 * 	Modified by Alif Ramdani
 *     at (x,y,z)
 *     dimensions (dx,dy,dz)
 *     rotated th about the y axis
 */
static void stand(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
{
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);

   //Texture repitition values
   float texRepX = 1.0;
   float texRepY = 1.0;
	
	glBindTexture(GL_TEXTURE_2D,_textureStandWall);
   texScale = 0.5;
   glNormal3f( 0, 0, 1);
   //  Cube
   glBegin(GL_QUADS);
   //  Front
   texRepX = dx/texScale;
   texRepY = dy/texScale;
   glColor3f(0.7, 0.7, 0.7);
   
   glTexCoord2f(0.0,0.0); glVertex3f(-1,-1, 1);
   glTexCoord2f(texRepX,0.0); glVertex3f(+1,-1, 1);
   glTexCoord2f(texRepX,texRepY); glVertex3f(+1,+1, 1);
   glTexCoord2f(0.0,texRepY); glVertex3f(-1,+1, 1);
   //  Back
   texRepX = dx/texScale;
   texRepY = dy/texScale;
   glNormal3f( 0, 0,-1);
   glTexCoord2f(0.0,0.0); glVertex3f(+1,-1,-1);
   glTexCoord2f(texRepX,0.0); glVertex3f(-1,-1,-1);
   glTexCoord2f(texRepX,texRepY); glVertex3f(-1,+10,-1);
   glTexCoord2f(0.0,texRepY); glVertex3f(+1,+10,-1);
   //  Right
   texRepX = dz/texScale;
   texRepY = dy/texScale;
   glNormal3f(+1, 0, 0);
   glTexCoord2f(0.0,0.0); glVertex3f(+1,-1,+1);
   glTexCoord2f(texRepX,0.0); glVertex3f(+1,-1,-1);
   glTexCoord2f(texRepX,texRepY); glVertex3f(+1,+10,-1);
   glTexCoord2f(0.0,texRepY); glVertex3f(+1,+1,+1);
   //  Left
   texRepX = dz/texScale;
   texRepY = dy/texScale;
   glNormal3f(-1, 0, 0);
   glTexCoord2f(0.0,0.0); glVertex3f(-1,-1,-1);
   glTexCoord2f(texRepX,0.0); glVertex3f(-1,-1,+1);
   glTexCoord2f(texRepX,texRepY); glVertex3f(-1,+1,+1);
   glTexCoord2f(0.0,texRepY); glVertex3f(-1,+10,-1);
  
   //  Bottom
   texRepX = dx/texScale;
   texRepY = dz/texScale;
   glNormal3f( 0,-one, 0);
   glTexCoord2f(0.0,0.0); glVertex3f(-1,-1,-1);
   glTexCoord2f(texRepX,0.0); glVertex3f(+1,-1,-1);
   glTexCoord2f(texRepX,texRepY); glVertex3f(+1,-1,+1);
   glTexCoord2f(0.0,texRepY); glVertex3f(-1,-1,+1);
   
   //  End
   glEnd();
   
   // Top
   glColor3f(0.7, 0.7, 0.7);
   glBindTexture(GL_TEXTURE_2D,_textureStandSeats);
   texScale = 0.5;
   glBegin(GL_QUADS);
   	
    // -- Seats
   texRepX = dx/texScale;
   texRepY = dz/texScale;
   glNormal3f( 0,+1, 0);
   glTexCoord2f(0.0,0.0); glVertex3f(-1,+1,+1);
   glTexCoord2f(texRepX,0.0); glVertex3f(+1,+1,+1);
   glTexCoord2f(texRepX,texRepY); glVertex3f(+1,+10,-1);
   glTexCoord2f(0.0,texRepY); glVertex3f(-1,+10,-1);
   glEnd();
   
   //  Undo transofrmations
   glPopMatrix();
}

/*
 *  Draw a Pillar (Gapura element)
 *     at (x,y,z)
 *     dimensions (dx,dy,dz)
 *     rotated th about the y axis
 */
static void pillar(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th, int xSlope, int ySlope, int zSlope)
{
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glRotated(th,xSlope,ySlope,zSlope);
   glScaled(dx,dy,dz);

   //Texture repitition values
   float texRepX = 1.0;
   float texRepY = 1.0;

   //  Cube
   glBegin(GL_QUADS);
   //  Front
   texRepX = dx/texScale;
   texRepY = dy/texScale;
   glNormal3f( 0, 0, 1);
   glTexCoord2f(0.0,0.0); glVertex3f(-1,-1, 1);
   glTexCoord2f(texRepX,0.0); glVertex3f(+1,-1, 1);
   glTexCoord2f(texRepX,texRepY); glVertex3f(+1,+1, 1);
   glTexCoord2f(0.0,texRepY); glVertex3f(-1,+1, 1);
   //  Back
   texRepX = dx/texScale;
   texRepY = dy/texScale;
   glNormal3f( 0, 0,-1);
   glTexCoord2f(0.0,0.0); glVertex3f(+1,-1,-1);
   glTexCoord2f(texRepX,0.0); glVertex3f(-1,-1,-1);
   glTexCoord2f(texRepX,texRepY); glVertex3f(-1,+1,-1);
   glTexCoord2f(0.0,texRepY); glVertex3f(+1,+1,-1);
   //  Right
   texRepX = dz/texScale;
   texRepY = dy/texScale;
   glNormal3f(+1, 0, 0);
   glTexCoord2f(0.0,0.0); glVertex3f(+1,-1,+1);
   glTexCoord2f(texRepX,0.0); glVertex3f(+1,-1,-1);
   glTexCoord2f(texRepX,texRepY); glVertex3f(+1,+1,-1);
   glTexCoord2f(0.0,texRepY); glVertex3f(+1,+1,+1);
   //  Left
   texRepX = dz/texScale;
   texRepY = dy/texScale;
   glNormal3f(-1, 0, 0);
   glTexCoord2f(0.0,0.0); glVertex3f(-1,-1,-1);
   glTexCoord2f(texRepX,0.0); glVertex3f(-1,-1,+1);
   glTexCoord2f(texRepX,texRepY); glVertex3f(-1,+1,+1);
   glTexCoord2f(0.0,texRepY); glVertex3f(-1,+1,-1);
   //  Top
   texRepX = dx/texScale;
   texRepY = dz/texScale;
   glNormal3f( 0,+1, 0);
   glTexCoord2f(0.0,0.0); glVertex3f(-1,+1,+1);
   glTexCoord2f(texRepX,0.0); glVertex3f(+1,+1,+1);
   glTexCoord2f(texRepX,texRepY); glVertex3f(+1,+1,-1);
   glTexCoord2f(0.0,texRepY); glVertex3f(-1,+1,-1);
   //  Bottom
   texRepX = dx/texScale;
   texRepY = dz/texScale;
   glNormal3f( 0,-one, 0);
   glTexCoord2f(0.0,0.0); glVertex3f(-1,-1,-1);
   glTexCoord2f(texRepX,0.0); glVertex3f(+1,-1,-1);
   glTexCoord2f(texRepX,texRepY); glVertex3f(+1,-1,+1);
   glTexCoord2f(0.0,texRepY); glVertex3f(-1,-1,+1);
   //  End
   glEnd();
   //  Undo transofrmations
   glPopMatrix();
}

/*
 *  Draw a Gapura
 *     at (x,y,z)
 *     dimensions (dx,dy,dz)
 *     rotated th about the y axis
 */
static void gapura(double xPillar, double zPillar, int direction, 
	GLuint textureSidePillar, GLuint textureTopPillar)
{
  	glColor3f(1, 1, 1);
	texScale = 0.5;
	double yDepthFinish = 1;
	double widthFinish, lengthFinish;
	widthFinish = lengthFinish = 0.25; //niatnya ingin square
	if(direction == GAPURA_X) {
		//bagian penyangganya
		glBindTexture(GL_TEXTURE_2D,textureSidePillar);
			pillar(xPillar,1,zPillar,widthFinish,yDepthFinish,lengthFinish, 90, 0, 1, 0);
		    pillar(xPillar,1,zPillar+4,widthFinish,yDepthFinish,lengthFinish, 90, 0, 1, 0);
	    //bagian atasnya
		glBindTexture(GL_TEXTURE_2D,textureTopPillar);
	   	 	pillar(xPillar,2.25,zPillar+2,widthFinish,yDepthFinish+1.25,lengthFinish, 90, 2, 0, 0);
	}else if(direction == GAPURA_Y) {
		//bagian penyangganya
		glBindTexture(GL_TEXTURE_2D,textureSidePillar);
			pillar(xPillar,1,zPillar,widthFinish,yDepthFinish,lengthFinish, 90, 0, 1, 0);
		    pillar(xPillar+4,1,zPillar,widthFinish,yDepthFinish,lengthFinish, 90, 0, 1, 0);
		//bagian atasnya
		glBindTexture(GL_TEXTURE_2D,textureTopPillar);
	   	 	pillar(xPillar,2.25,zPillar+2,widthFinish,yDepthFinish+1.25,lengthFinish, 90, 0, 0, 20);
	} else {
		printf("Gapura Direction is not recognized\n");
	}

}

/*
 *  Draw a Gapura
 *     at (x,y,z)
 *     dimensions (dx,dy,dz)
 *     rotated th about the y axis
 */
static void tunnel(double xPillar, double zPillar, int direction, 
	GLuint textureSidePillar, GLuint textureTopPillar)
{
  	glColor3f(1, 1, 1);
	texScale = 0.5;
	int amountOfPillars = 5;
	int count;
	double yDepthPillar = 1;
	double widthPillar, lengthPillar;
	widthPillar = lengthPillar = 0.25; //niatnya ingin square
	if(direction == GAPURA_X) {
		//bagian penyangganya
		glBindTexture(GL_TEXTURE_2D,textureSidePillar);
			pillar(xPillar,1,zPillar,widthPillar,yDepthPillar,lengthPillar, 90, 0, 1, 0);
		    pillar(xPillar,1,zPillar+4,widthPillar,yDepthPillar,lengthPillar, 90, 0, 1, 0);
	    //bagian atasnya
		glBindTexture(GL_TEXTURE_2D,textureTopPillar);
	   	 	pillar(xPillar,2.25,zPillar+2,widthPillar,yDepthPillar+1.25,lengthPillar, 90, 2, 0, 0);
	}else if(direction == GAPURA_Y) {
		//bagian atasnya
		glBindTexture(GL_TEXTURE_2D,textureTopPillar);
	   	 	pillar(xPillar+2,2.25,zPillar+8,widthPillar,yDepthPillar+1.25,lengthPillar+9,90, 0, 0, 20);
		//bagian penyangganya
		glBindTexture(GL_TEXTURE_2D,textureSidePillar);
		for(count = 0; count < amountOfPillars; count++) {
			pillar(xPillar,1,zPillar,widthPillar,yDepthPillar,lengthPillar, 90, 0, 1, 0);
		    pillar(xPillar+4,1,zPillar,widthPillar,yDepthPillar,lengthPillar, 90, 0, 1, 0);
		    zPillar += 4;
		}
	
	} else {
		printf("Gapura Direction is not recognized\n");
	}

}

/*
 *  Draw a Gapura
 *     at (x,y,z)
 *     dimensions (dx,dy,dz)
 *     rotated th about the y axis
 */
static void adboard(double xPillar, double zPillar, int direction, 
	GLuint textureSidePillar, GLuint textureTopPillar)
{
  	glColor3f(1, 1, 1);
	texScale = 0.5;
	int amountOfPillars = 5;
	int count;
	double yDepthPillar = 0.5;
	double widthPillarX, widthPillarY, lengthPillarX, lengthPillarY;
	widthPillarX = 10;
	widthPillarY = 0;
	lengthPillarX = 0;
	lengthPillarY = 0; 
	if(direction == GAPURA_X) {
		//bagian penyangganya
		glBindTexture(GL_TEXTURE_2D,textureSidePillar);
			pillar(xPillar,0.5,zPillar,widthPillarX,yDepthPillar,lengthPillarX, 90, 0, 1, 0);
	}else if(direction == GAPURA_Y) {
		glBindTexture(GL_TEXTURE_2D,textureSidePillar);
			pillar(xPillar,0.5,zPillar,widthPillarY,yDepthPillar,lengthPillarY, 90, 0, 1, 0);
	} else {
		printf("AdBoard Direction is not recognized\n");
	}

}



/*
 *  Draw a cube
 *     at (x,y,z)
 *     dimensions (dx,dy,dz)
 *     rotated th about the y axis
 */
static void turnCircuit(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
{
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);

   //Texture repitition values
   float texRepX = 1.0;
   float texRepY = 1.0;

   //  Cube
   glBegin(GL_QUADS);
   //  Front
   texRepX = dx/texScale;
   texRepY = dy/texScale;
   glNormal3f( 0, 0, 1);
   glTexCoord2f(0.0,0.0); glVertex3f(-1,-1, 1);
   glTexCoord2f(texRepX,0.0); glVertex3f(+1,-1, 1);
   glTexCoord2f(texRepX,texRepY); glVertex3f(+1,+1, 1);
   glTexCoord2f(0.0,texRepY); glVertex3f(-1,+1, 1);
   //  Back
   texRepX = dx/texScale;
   texRepY = dy/texScale;
   glNormal3f( 0, 0,-1);
   glTexCoord2f(0.0,0.0); glVertex3f(+1,-1,-1);
   glTexCoord2f(texRepX,0.0); glVertex3f(-1,-1,-1);
   glTexCoord2f(texRepX,texRepY); glVertex3f(-1,+1,-1);
   glTexCoord2f(0.0,texRepY); glVertex3f(+1,+1,-1);
   //  Right
   texRepX = dz/texScale;
   texRepY = dy/texScale;
   glNormal3f(+1, 0, 0);
   glTexCoord2f(0.0,0.0); glVertex3f(+1,-1,+1);
   glTexCoord2f(texRepX,0.0); glVertex3f(+1,-1,-1);
   glTexCoord2f(texRepX,texRepY); glVertex3f(+1,+1,-1);
   glTexCoord2f(0.0,texRepY); glVertex3f(+1,+1,+1);
   //  Left
   texRepX = dz/texScale;
   texRepY = dy/texScale;
   glNormal3f(-1, 0, 0);
   glTexCoord2f(0.0,0.0); glVertex3f(-1,-1,-1);
   glTexCoord2f(texRepX,0.0); glVertex3f(-1,-1,+1);
   glTexCoord2f(texRepX,texRepY); glVertex3f(-1,+1,+1);
   glTexCoord2f(0.0,texRepY); glVertex3f(-1,+1,-1);
   //  Top
   texRepX = dx/texScale;
   texRepY = dz/texScale;
   glNormal3f( 0,+1, 0);
   glTexCoord2f(0.0,0.0); glVertex3f(-1,+1,+3);
   glTexCoord2f(texRepX,0.0); glVertex3f(+3,+1,-2.5);
   glTexCoord2f(texRepX,texRepY); glVertex3f(0,+1,-2.5);
   glTexCoord2f(0.0,texRepY); glVertex3f(-1,+1,-1);
   //  Bottom
   texRepX = dx/texScale;
   texRepY = dz/texScale;
   glNormal3f( 0,-one, 0);
   glTexCoord2f(0.0,0.0); glVertex3f(-1,-2,+3);
   glTexCoord2f(texRepX,0.0); glVertex3f(+3,-2,-2.5);
   glTexCoord2f(texRepX,texRepY); glVertex3f(0,-2,-2.5);
   glTexCoord2f(0.0,texRepY); glVertex3f(-1,-2,-1);
   //  End
   glEnd();
   //  Undo transofrmations
   glPopMatrix();
}


/*
 *  Draw a section of the car body
 *     at (x,y,z)
 *     dimensions (dx,dy,dz)
 *     rotated th about the y axis
 *     w (1 to color windows for car body, 0 otherwise)
 */
static void body(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th,
                 int w)
{
   //  Set specular color to white
   float white[] = {1,1,1,1};
   float black[] = {0,0,0,1};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

   glBindTexture(GL_TEXTURE_2D,_textureBasicMetal);
   texScale = 0.1;

   glEnable(GL_POLYGON_OFFSET_FILL);

   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);

   //Texture repitition values
   float texRepX = 1.0;
   float texRepY = 1.0;

   //  Cube
   glBegin(GL_QUADS);

   //  Top
   texRepX = dx/texScale;
   texRepY = dz/texScale;
   glNormal3f( 0,+1, 0);
   glTexCoord2f(0.0,0.0); glVertex3f(-1,+1,+1);
   glTexCoord2f(texRepX,0.0); glVertex3f(+1,+1,+1);
   glTexCoord2f(texRepX,texRepY); glVertex3f(+1,+1,-1);
   glTexCoord2f(0.0,texRepY); glVertex3f(-1,+1,-1);
   //  Bottom
   texRepX = dx/texScale;
   texRepY = dz/texScale;
   glNormal3f( 0,-one, 0);
   glTexCoord2f(0.0,0.0); glVertex3f(-1,-1,-1);
   glTexCoord2f(texRepX,0.0); glVertex3f(+1,-1,-1);
   glTexCoord2f(texRepX,texRepY); glVertex3f(+1,-1,+1);
   glTexCoord2f(0.0,texRepY); glVertex3f(-1,-1,+1);

   //  Front
   texRepX = dx/texScale;
   texRepY = dy/texScale;
   glNormal3f( 0, 0, 1);
   glTexCoord2f(0.0,0.0); glVertex3f(-1,-1, 1);
   glTexCoord2f(texRepX,0.0); glVertex3f(+1,-1, 1);
   glTexCoord2f(texRepX,texRepY); glVertex3f(+1,+1, 1);
   glTexCoord2f(0.0,texRepY); glVertex3f(-1,+1, 1);
   //  Back
   texRepX = dx/texScale;
   texRepY = dy/texScale;
   glNormal3f( 0, 0,-1);
   glTexCoord2f(0.0,0.0); glVertex3f(+1,-1,-1);
   glTexCoord2f(texRepX,0.0); glVertex3f(-1,-1,-1);
   glTexCoord2f(texRepX,texRepY); glVertex3f(-1,+1,-1);
   glTexCoord2f(0.0,texRepY); glVertex3f(+1,+1,-1);

   //  End
   glEnd();

   glDisable(GL_POLYGON_OFFSET_FILL);
   //Color and texture to add windows
   if(w == 1) {
      glColor3f(0.8, 0.8, 1);
      glBindTexture(GL_TEXTURE_2D,_textureGlass);
      texRepX = 1.0;
      texRepY = 1.0;
      glBegin(GL_QUADS);
      //  Front
      glNormal3f(0, 0, 1);
      glTexCoord2f(0.0,0.0); glVertex3f(-0.8,-1, 1);
      glTexCoord2f(texRepX,0.0); glVertex3f(+0.8,-1, 1);
      glTexCoord2f(texRepX,texRepY); glVertex3f(+0.8,+1, 1);
      glTexCoord2f(0.0,texRepY); glVertex3f(-0.8,+1, 1);
      //  Back
      glNormal3f(0, 0,-1);
      glTexCoord2f(0.0,0.0); glVertex3f(+0.8,-1,-1);
      glTexCoord2f(texRepX,0.0); glVertex3f(-0.8,-1,-1);
      glTexCoord2f(texRepX,texRepY); glVertex3f(-0.8,+1,-1);
      glTexCoord2f(0.0,texRepY); glVertex3f(+0.8,+1,-1);
      glEnd();
   }
   glEnable(GL_POLYGON_OFFSET_FILL);
   
   //  Undo transformations
   glPopMatrix();
}

static void wheel(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th,
                 int d,
                 int s)
{
   //  Set specular color to white
   float white[] = {1,1,1,1};
   float black[] = {0,0,0,1};
   //Turn off shininess for the rubber tire
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,0);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);

   glBindTexture(GL_TEXTURE_2D,_textureCarWheel);

   glColor3f(0.8,0.8,0.8);
   //Tire
   glBegin(GL_TRIANGLE_FAN);
   glNormal3f(0, 0, -1);
   glTexCoord2f(0.5,0.5);
   glVertex3f(0, 0, -0.05);
   for (th=0;th<=360;th+=s)
   {
      double ph = d-90;
      glTexCoord2f(0.5*Cos(th)+0.5,0.5*Sin(th)+0.5);
      glVertex3d(Sin(th)*Cos(ph), Cos(th)*Cos(ph), -0.05);
   }
   glEnd();

   glBegin(GL_TRIANGLE_FAN);
   glNormal3f(0, 0, 1);
   glTexCoord2f(0.5,0.5);
   glVertex3f(0, 0, 0.05);
   for (th=360;th>=0;th-=s)
   {
      double ph = d-90;
      glTexCoord2f(0.5*Cos(th)+0.5,0.5*Sin(th)+0.5);
      glVertex3d(Sin(th)*Cos(ph) , Cos(th)*Cos(ph), 0.05);
   }
   glEnd();

   glBindTexture(GL_TEXTURE_2D,_textureTireTread);

   glColor3f(0.5,0.5,0.55);
   glBegin(GL_QUAD_STRIP);
   for (th=0;th<=360;th+=s)
   {
      double ph = d-90;
      glNormal3f(Sin(th)*Cos(ph), Cos(th)*Cos(ph), 0);
      glTexCoord2f(0,0.1*th); glVertex3d(Sin(th)*Cos(ph), Cos(th)*Cos(ph), -0.05);
      glTexCoord2f(1,0.1*th); glVertex3d(Sin(th)*Cos(ph), Cos(th)*Cos(ph), 0.05);
   }
   // glNormal3f(Sin(0)*Cos(-90), Cos(0)*Cos(-90), 0);
   // glVertex3d(Sin(0)*Cos(-90), Cos(0)*Cos(-90), -0.05);
   // glVertex3d(Sin(0)*Cos(-90), Cos(0)*Cos(-90), 0.05);
   glEnd();

   //  Undo transformations
   glPopMatrix();
}

static void bumper(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th,
                 int m)
{
   //  Set specular color to white
   float white[] = {1,1,1,1};
   float black[] = {0,0,0,1};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);

   //Texture repitition values
   float texRepX = 1.0;
   float texRepY = 1.0;

   glBindTexture(GL_TEXTURE_2D,_textureBasicMetal);
   texScale = 0.1;

   //Offset the bumper so that the lights and grill are drawn directly on the surface
   glEnable(GL_POLYGON_OFFSET_FILL);

   //Bumper

   //Base
   texRepX = dx/texScale;
   texRepY = dy/texScale;
   glBegin(GL_POLYGON);
   glNormal3f(0, -1, 0);
   glTexCoord2f(0.0,0.0); glVertex3f(0,0,0.4);
   glTexCoord2f(texRepX,0.0); glVertex3f(0,0,-0.4);
   glTexCoord2f(texRepX,texRepY); glVertex3f(0.1,0,-0.35);
   glTexCoord2f(0.0,texRepY); glVertex3f(0.1,0,0.35);
   glEnd();

   //Front Panels
   glBegin(GL_QUADS);
   texRepX = dx/texScale;
   texRepY = dy/texScale;
   glNormal3f(0.447214, 0, 0.894427);
   glTexCoord2f(0.0,0.0); glVertex3f(0.1, 0, 0.35);
   glTexCoord2f(texRepX,0.0); glVertex3f(0.1, 0.2, 0.35);
   glTexCoord2f(texRepX,texRepY); glVertex3f(0, 0.2, 0.4);
   glTexCoord2f(0.0,texRepY); glVertex3f(0, 0, 0.4);

   texRepX = dx/texScale;
   texRepY = dy/texScale;
   glNormal3f(1, 0, 0);
   glTexCoord2f(0.0,0.0); glVertex3f(0.1, 0, -0.35);
   glTexCoord2f(texRepX,0.0); glVertex3f(0.1, 0.2, -0.35);
   glTexCoord2f(texRepX,texRepY); glVertex3f(0.1, 0.2, 0.35);
   glTexCoord2f(0.0,texRepY); glVertex3f(0.1, 0, 0.35);

   texRepX = dx/texScale;
   texRepY = dy/texScale;
   glNormal3f(0.447214, 0, -0.894427);
   glTexCoord2f(0.0,0.0); glVertex3f(0.1, 0, -0.35);
   glTexCoord2f(texRepX,0.0); glVertex3f(0, 0, -0.4);
   glTexCoord2f(texRepX,texRepY); glVertex3f(0, 0.2, -0.4);
   glTexCoord2f(0.0,texRepY); glVertex3f(0.1, 0.2, -0.35);
   
   glEnd();

   //Upper Bumper
   glBegin(GL_QUADS);
   texRepX = dx/texScale;
   texRepY = dy/texScale;
   glNormal3f(0.447214, 0.894427, 0);
   glTexCoord2f(0.0,0.0); glVertex3f(0, 0.25, 0.35);
   glTexCoord2f(texRepX,0.0); glVertex3f(0.1, 0.2, 0.35);
   glTexCoord2f(texRepX,texRepY); glVertex3f(0.1, 0.2, -0.35);
   glTexCoord2f(0.0,texRepY); glVertex3f(0, 0.25, -0.35);
   glEnd();

   glBegin(GL_TRIANGLES);
   texRepX = dx/texScale;
   texRepY = dy/texScale;
   glNormal3f(0.333333, 0.666667, 0.666667);
   glTexCoord2f(0.0, 0.0); glVertex3f(0, 0.2, 0.4);
   glTexCoord2f(texRepX/2, texRepY); glVertex3f(0.1, 0.2, 0.35);
   glTexCoord2f(texRepX, 0.0); glVertex3f(0, 0.25, 0.35);

   texRepX = dx/texScale;
   texRepY = dy/texScale;
   glNormal3f(0.333333, 0.666667, -0.666667);
   glTexCoord2f(0.0, 0.0); glVertex3f(0, 0.25, -0.35);
   glTexCoord2f(texRepX/2, texRepY); glVertex3f(0.1, 0.2, -0.35);
   glTexCoord2f(texRepX, 0.0); glVertex3f(0, 0.2, -0.4);
   glEnd();

   //  Disable polygon offset
   glDisable(GL_POLYGON_OFFSET_FILL);

   if (m == 1) {
      glColor3f(0.2,0.2,0.2);
      glBindTexture(GL_TEXTURE_2D,_textureCarGrill);

      //Grill
      glBegin(GL_QUADS);
      glNormal3f(1, 0, 0);
      glTexCoord2f(0.0,0.0); glVertex3f(0.1, 0.15, 0.18);
      glTexCoord2f(0.5,0.0); glVertex3f(0.1, 0.03, 0.18);
      glTexCoord2f(0.5,1.0); glVertex3f(0.1, 0.03, -0.18);
      glTexCoord2f(0.0,1.0); glVertex3f(0.1, 0.15, -0.18);
      glEnd();
   }

   //Lights (taillights or headlights)
   float emColor[4];
   if(m == 1) {
      emColor[0] = 1.0 * emission;
      emColor[1] = 1.0 * emission;
      emColor[2] = 0.8 * emission;
      emColor[3] = 1.0 * emission;
      glColor3f(1, 1, 0.8);
   } else {
      emColor[0] = 0.8 * emission;
      emColor[1] = 0.0 * emission;
      emColor[2] = 0.0 * emission;
      emColor[3] = 1.0 * emission;
      glColor3f(.8, 0, 0);
   }

   glMaterialf(GL_FRONT,GL_SHININESS,0);
   glMaterialfv(GL_FRONT,GL_SPECULAR,emColor);
   glMaterialfv(GL_FRONT,GL_EMISSION,emColor);

   glBindTexture(GL_TEXTURE_2D,_textureHeadLamp);

   glBegin(GL_TRIANGLE_FAN);
   glNormal3f(1, 0, 0);
   glTexCoord2f(0.5,0.5); glVertex3f(0.1, 0.13, -0.25);
   for (th=0;th<=360;th+=10)
   {
      double ph = 3-90;
      glTexCoord2f(0.5*Cos(th)+0.5,0.5*Sin(th)+0.5);
      glVertex3d(0.1, 0.13+(Cos(th)*Cos(ph)), -0.25+(Sin(th)*Cos(ph)));
   }
   glEnd();

   glBegin(GL_TRIANGLE_FAN);
   glNormal3f(1, 0, 0);
   glTexCoord2f(0.5,0.5); glVertex3f(0.1, 0.13, 0.25);
   for (th=0;th<=360;th+=10)
   {
      double ph = 3-90;
      glTexCoord2f(0.5*Cos(th)+0.5,0.5*Sin(th)+0.5);
      glVertex3d(0.1, 0.13+(Cos(th)*Cos(ph)), 0.25+(Sin(th)*Cos(ph)));
   }
   glEnd();

   glEnable(GL_POLYGON_OFFSET_FILL);

   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

   //Undo transformations
   glPopMatrix();  
}


static void car(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th,
                 float cr, float cb, float cg)
{
   //  Set specular color to white
   float white[] = {1,1,1,1};
   float black[] = {0,0,0,1};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);

   glPolygonOffset(1,1);

   wheel(0.6,0,0.4, 1,1,1, 0, 8, 10);
   wheel(-0.6,0,-0.4, 1,1,1, 0, 8, 10);
   wheel(0.6,0,-0.4, 1,1,1, 0, 8, 10);
   wheel(-0.6,0,0.4, 1,1,1, 0, 8, 10);

   glColor3f(cr, cb, cg);

   //Lower Body
   body(0,0.1,0, 0.8,0.1,0.4, 0, 0);
   //Cabin
   body(-0.1,0.3,0, 0.3,0.1,0.35, 0, 1);

   texScale = 1.0;

   glColor3f(cr, cb, cg);

   //Front Bumper
   bumper(0.8,0,0, 1,1,1, 0, 1);

   glColor3f(cr, cb, cg);

   //Rear Bumper
   bumper(-0.8,0,0, 1,1,1, 180, 0);

   //  Set specular color to white
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
   
   glEnable(GL_POLYGON_OFFSET_FILL);

   glColor3f(cr, cb, cg);

   glBindTexture(GL_TEXTURE_2D,_textureBasicMetal);

   //Texture repitition values
   float texRepX = 1.0;
   float texRepY = 1.0;

   //Hood and upper side pannels
   texRepX = dx/texScale;
   texRepY = dz/texScale;
   glBegin(GL_QUADS);
   glNormal3f(0, 0.707107, 0.707107);
   glTexCoord2f(0.0,0.0); glVertex3f(-0.8, 0.25, 0.35);
   glTexCoord2f(texRepX,0.0); glVertex3f(-0.8, 0.2, 0.4);
   glTexCoord2f(texRepX,texRepY); glVertex3f(0.8, 0.2, 0.4);
   glTexCoord2f(0.0,texRepY); glVertex3f(0.8, 0.25, 0.35);

   glNormal3f(0, 1, 0);
   glTexCoord2f(0.0,0.0); glVertex3f(0.4, 0.25, 0.35);
   glTexCoord2f(texRepX,0.0); glVertex3f(0.8, 0.25, 0.35);
   glTexCoord2f(texRepX,texRepY); glVertex3f(0.8, 0.25, -0.35);
   glTexCoord2f(0.0,texRepY); glVertex3f(0.4, 0.25, -0.35);
   
   glNormal3f(0, 0.707107, -0.707107);
   glTexCoord2f(0.0,0.0); glVertex3f(-0.8, 0.2, -0.4);
   glTexCoord2f(texRepX,0.0); glVertex3f(-0.8, 0.25, -0.35);
   glTexCoord2f(texRepX,texRepY); glVertex3f(0.8, 0.25, -0.35);
   glTexCoord2f(0.0,texRepY); glVertex3f(0.8, 0.2, -0.4);
   glEnd();

   //Trunk
   texRepX = dx/texScale;
   texRepY = dz/texScale;
   glBegin(GL_QUADS);
   glNormal3f(0,1,0);
   glTexCoord2f(0.0,0.0); glVertex3f(-0.8, 0.25, -0.35);
   glTexCoord2f(texRepX,0.0); glVertex3f(-0.8, 0.25, 0.35);
   glTexCoord2f(texRepX,texRepY); glVertex3f(-0.6, 0.25, 0.35);
   glTexCoord2f(0.0,texRepY); glVertex3f(-0.6, 0.25, -0.35);
   glEnd();

   glBindTexture(GL_TEXTURE_2D,_textureGlass);

   glColor3f(0.8, 0.8, 1);

   //Windshield
   texRepX = 1.0;
   texRepY = 1.0;
   glBegin(GL_QUADS);
   glNormal3f(0.6, 0.8, 0);
   glTexCoord2f(0.0,0.0); glVertex3f(0.4,0.25,0.35);
   glTexCoord2f(texRepX,0.0); glVertex3f(0.4,0.25,-0.35);
   glTexCoord2f(texRepX,texRepY); glVertex3f(0.2,0.4,-0.35);
   glTexCoord2f(0.0,texRepY); glVertex3f(0.2,0.4,0.35);
   glEnd();

   glBegin(GL_TRIANGLES);
   glNormal3f(0,0,1);
   glTexCoord2f(0.0,0.0); glVertex3f(0.2,0.4,0.35);
   glTexCoord2f(texRepX, 0.0); glVertex3f(0.2,0.25,0.35);
   glTexCoord2f(texRepX, texRepY); glVertex3f(0.4,0.25,0.35);

   glNormal3f(0,0,-1);
   glTexCoord2f(0.0,0.0); glVertex3f(0.4,0.25,-0.35);
   glTexCoord2f(texRepX, 0.0); glVertex3f(0.2,0.25,-0.35);
   glTexCoord2f(texRepX, texRepY); glVertex3f(0.2,0.4,-0.35);
   glEnd();

   //Rear Window
   texRepX = 1.0;
   texRepY = 1.0;
   glBegin(GL_QUADS);
   glNormal3f(-0.6, 0.8, 0);
   glTexCoord2f(0.0,0.0); glVertex3f(-0.6,0.25,-0.35);
   glTexCoord2f(texRepX,0.0); glVertex3f(-0.6,0.25,0.35);
   glTexCoord2f(texRepX,texRepY); glVertex3f(-0.4,0.4,0.35);
   glTexCoord2f(0.0,texRepY); glVertex3f(-0.4,0.4,-0.35);
   glEnd();

   glBegin(GL_TRIANGLES);
   glNormal3f(0,0,1);
   glTexCoord2f(0.0,0.0); glVertex3f(-0.6,0.25,0.35);
   glTexCoord2f(texRepX, 0.0); glVertex3f(-0.4,0.25,0.35);
   glTexCoord2f(texRepX, texRepY); glVertex3f(-0.4,0.4,0.35);
   
   glNormal3f(0,0,-1);
   glTexCoord2f(0.0,0.0); glVertex3f(-0.4,0.4,-0.35);
   glTexCoord2f(texRepX, 0.0); glVertex3f(-0.4,0.25,-0.35);
   glTexCoord2f(texRepX, texRepY); glVertex3f(-0.6,0.25,-0.35);
   glEnd();

   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

   glBindTexture(GL_TEXTURE_2D,_textureCarbonFiber);

   //Spoiler
   glColor3f(0.3,0.3,0.3);
   cube(-0.75,0.28,0.3, 0.02,0.03,0.02, 0);
   cube(-0.75,0.28,-0.3, 0.02,0.03,0.02, 0);

   texRepX = 5.0;
   texRepY = 1.0;

   glBegin(GL_QUADS);
   glNormal3f(0, -1, 0);
   glTexCoord2f(0.0,0.0); glVertex3f(-0.7,0.31,-0.35);
   glTexCoord2f(texRepX,0.0); glVertex3f(-0.7,0.31,0.35);
   glTexCoord2f(texRepX,texRepY); glVertex3f(-0.8,0.31,0.35);
   glTexCoord2f(0.0,texRepY); glVertex3f(-0.8,0.31,-0.35);
   
   glNormal3f(0.196116, 0.980581, 0);
   glTexCoord2f(0.0,0.0); glVertex3f(-0.8,0.33,-0.35);
   glTexCoord2f(texRepX,0.0); glVertex3f(-0.8,0.33,0.35);
   glTexCoord2f(texRepX,texRepY); glVertex3f(-0.7,0.31,0.35);
   glTexCoord2f(0.0,texRepY); glVertex3f(-0.7,0.31,-0.35);

   texRepX = 5.0;
   texRepY = 0.2;
   glNormal3f(-1, 0, 0);
   glTexCoord2f(0.0,0.0); glVertex3f(-0.8,0.33,0.35);
   glTexCoord2f(texRepX,0.0); glVertex3f(-0.8,0.33,-0.35);
   glTexCoord2f(texRepX,texRepY); glVertex3f(-0.8,0.31,-0.35);
   glTexCoord2f(0.0,texRepY); glVertex3f(-0.8,0.31,0.35);
   
   glEnd();

   glBindTexture(GL_TEXTURE_2D,_textureBasicMetal);
   glColor3f(cr, cb, cg);

   //Spoiler Fins
   texRepX = dx/texScale;
   texRepY = dy/texScale;
   glBegin(GL_TRIANGLES);
   glNormal3f(0,0,-1);
   glTexCoord2f(0.0,0.0); glVertex3f(-0.68,0.31,-0.35);
   glTexCoord2f(texRepX, 0.0); glVertex3f(-0.82,0.31,-0.35);
   glTexCoord2f(texRepX, texRepY); glVertex3f(-0.82,0.35,-0.35);

   glNormal3f(0,0,1);
   glTexCoord2f(0.0,0.0); glVertex3f(-0.82,0.35,0.35);
   glTexCoord2f(texRepX, 0.0); glVertex3f(-0.82,0.31,0.35);
   glTexCoord2f(texRepX, texRepY); glVertex3f(-0.68,0.31,0.35);
   
   //Duplicate to draw both sides (with inverted normals) when face culling is on
   glNormal3f(0,0,1);
   glTexCoord2f(0.0,0.0); glVertex3f(-0.68,0.31,-0.35);
   glTexCoord2f(texRepX, 0.0); glVertex3f(-0.82,0.31,-0.35);
   glTexCoord2f(texRepX, texRepY); glVertex3f(-0.82,0.35,-0.35);

   glNormal3f(0,0,-1);
   glTexCoord2f(0.0,0.0); glVertex3f(-0.82,0.35,0.35);
   glTexCoord2f(texRepX, 0.0); glVertex3f(-0.82,0.31,0.35);
   glTexCoord2f(texRepX, texRepY); glVertex3f(-0.68,0.31,0.35);
   
   glEnd();

   //Undo transformations
   glPopMatrix();
}


static void skybox(float dim) {
   glDisable(GL_POLYGON_OFFSET_FILL);

   //  Set specular color to white
   float white[] = {1,1,1,1};
   float black[] = {0,0,0,1};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,0);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

   glColor3f(0.7, 0.7, 0.7);
   glBindTexture(GL_TEXTURE_2D,_textureSkyboxFront);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   glBegin(GL_QUADS);
   glNormal3f(0, 0, -1);
   glTexCoord2f(1.0,0.0); glVertex3f(+dim,-dim, dim);
   glTexCoord2f(0.0,0.0); glVertex3f(-dim,-dim, dim);
   glTexCoord2f(0.0,1.0); glVertex3f(-dim,+dim, dim);
   glTexCoord2f(1.0,1.0); glVertex3f(+dim,+dim, dim);
   glEnd();

   glBindTexture(GL_TEXTURE_2D,_textureSkyboxBack);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   glBegin(GL_QUADS);
   glNormal3f(0, 0, 1);
   glTexCoord2f(1.0,0.0); glVertex3f(-dim,-dim, -dim);
   glTexCoord2f(0.0,0.0); glVertex3f(+dim,-dim, -dim);
   glTexCoord2f(0.0,1.0); glVertex3f(+dim,+dim, -dim);
   glTexCoord2f(1.0,1.0); glVertex3f(-dim,+dim, -dim);
   glEnd();

   glBindTexture(GL_TEXTURE_2D,_textureSkyboxRight);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   glBegin(GL_QUADS);
   glNormal3f(-1, 0, 0);
   glTexCoord2f(1.0,0.0); glVertex3f(dim,-dim, -dim);
   glTexCoord2f(0.0,0.0); glVertex3f(dim,-dim, +dim);
   glTexCoord2f(0.0,1.0); glVertex3f(dim,+dim, +dim);
   glTexCoord2f(1.0,1.0); glVertex3f(dim,+dim, -dim);
   glEnd();

   glBindTexture(GL_TEXTURE_2D,_textureSkyboxLeft);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   glBegin(GL_QUADS);
   glNormal3f(1, 0, 0);
   glTexCoord2f(1.0,0.0); glVertex3f(-dim,-dim, +dim);
   glTexCoord2f(0.0,0.0); glVertex3f(-dim,-dim, -dim);
   glTexCoord2f(0.0,1.0); glVertex3f(-dim,+dim, -dim);
   glTexCoord2f(1.0,1.0); glVertex3f(-dim,+dim, +dim);
   glEnd();

   glBindTexture(GL_TEXTURE_2D,_textureSkyboxTop);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   glBegin(GL_QUADS);
   glNormal3f(0, -1, 0);
   glTexCoord2f(1.0,0.0); glVertex3f(+dim,dim, +dim);
   glTexCoord2f(0.0,0.0); glVertex3f(-dim,dim, +dim);
   glTexCoord2f(0.0,1.0); glVertex3f(-dim,dim, -dim);
   glTexCoord2f(1.0,1.0); glVertex3f(+dim,dim, -dim);
   glEnd();
   
   //percobaan yang bottom
//glBindTexture(GL_TEXTURE_2D,_textureSkyboxFront);
//   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//   glBegin(GL_QUADS);
//   glNormal3f(0, 0, -1);
//   glTexCoord2f(0,0.0); glVertex3f(+dim,-dim, dim);
//   glTexCoord2f(0.0,0.0); glVertex3f(-dim,-dim, dim);
//   glTexCoord2f(0,0); glVertex3f(-dim,+dim, dim);
//   glTexCoord2f(0,0); glVertex3f(+dim,+dim, dim);
//   glEnd();
}


/*
 *  Draw vertex in polar coordinates with normal
 */
static void Vertex(double th,double ph)
{
   double x = Sin(th)*Cos(ph);
   double y = Cos(th)*Cos(ph);
   double z =         Sin(ph);
   //  For a sphere at the origin, the position
   //  and normal vectors are the same
   glNormal3d(x,y,z);
   glVertex3d(x,y,z);
}


/*
 *  Draw a ball
 *     at (x,y,z)
 *     radius (r)
 */
static void ball(double x,double y,double z,double r)
{
   int th,ph;
   float yellow[] = {1.0,1.0,0.0,1.0};
   float Emission[]  = {1.0,1.0,1.0,1.0};
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glScaled(r,r,r);
   //  White ball
   glColor3f(1,1,0.9);
   glMaterialf(GL_FRONT,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT,GL_SPECULAR,yellow);
   glMaterialfv(GL_FRONT,GL_EMISSION,Emission);
   //  Bands of latitude
   for (ph=-90;ph<90;ph+=inc)
   {
      glBegin(GL_QUAD_STRIP);
      for (th=0;th<=360;th+=2*inc)
      {
         Vertex(th,ph);
         Vertex(th,ph+inc);
      }
      glEnd();
   }
   //  Undo transofrmations
   glPopMatrix();
}

/*
 *  Draw a circle
 *     at (x,y,z)
 *     dimensions (dx,dy,dz)
 *     rotated th about the y axis
 */
static void circle(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th, double radius)
{
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);

   //Texture repitition values
   float texRepX = 1.0;
   float texRepY = 1.0;

   texRepX = dx/texScale;
   texRepY = dz/texScale;
	int i;
	float x_now; 
			float z_now;
	const unsigned int triangles = 20; // number of triangles
	const float twoPi = 2.0f * 3.14159f;
	float delta = twoPi / triangles;
	// Top
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0.0,+1.0,0.0); // origin
	for(i = 0; i <= triangles/4; i++){
		x_now =  (radius * cos((i) *  delta));
		z_now =  (radius * sin((i) * delta));
		glTexCoord2f(x_now,z_now); glVertex3f(x_now,+1.0,z_now);
	}
	glEnd();
	
	// Bottom
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0.0,-1.0,0.0); // origin
	for(i = 0; i <= triangles/4; i++){
		x_now =  (radius * cos((i) *  delta));
		z_now =  (radius * sin((i) * delta));
		glTexCoord2f(x_now,z_now); glVertex3f(x_now,-1.0,z_now);
	}
	glEnd();
	
	// Side Circular
	glBegin(GL_TRIANGLE_STRIP);
		for(i = 0; i <= triangles/4;i++) { 
			x_now =  (radius * cos((i) *  delta));
			z_now =  (radius * sin((i) * delta));
			glTexCoord2f(x_now,z_now); glVertex3f(x_now,+1.0,z_now);
			glTexCoord2f(x_now,z_now); glVertex3f(x_now,-1.0,z_now);
		}
	glEnd();
   glPopMatrix();
}


void turnMode(int mode, int turnStatusInput, int car) {
	turnDegreeCount[car] = 0;
	turnStatus[car] = turnStatusInput;
	switch(mode) {
		case TURN_NORTHWEST : {
			turnDegree[car] = 30;
//			turnDegree = 30;
			isStright[car] = false;
			isTurnNorthwest[car] = true;
			isTurnNortheast[car] = isTurnSoutheast[car] = isTurnSouthwest[car] = false;
			printf("1\n");
			break;}
		case TURN_NORTHEAST : {
//			turnDegree[car] = 330;
			turnDegree[car] = 330;
			isStright[car] = false; 
			isTurnNortheast[car] = true;
			isTurnNorthwest[car] = isTurnSoutheast[car] = isTurnSouthwest[car] = false;
			printf("2\n");
			break;}
		case TURN_SOUTHEAST : {
//			turnDegree[car] = 210; 
			turnDegree[car] = 210;
			isStright[car] = false;
			isTurnSoutheast[car] = true;
			isTurnNorthwest[car] = isTurnNortheast[car] = isTurnSouthwest[car] = false;
			printf("3\n");
			break;}
		case TURN_SOUTHWEST : {
//			turnDegree[car] = 120;
			turnDegree[car] = 120;
			isStright[car] = false; 
			isTurnSouthwest[car] = true;
			isTurnNorthwest[car] = isTurnSoutheast[car] = isTurnNortheast[car] = false;
			printf("4\n");
			break;}
	}
}


/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{
   const double len=2.0;  //  Length of axes
   //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   //  Enable Z-buffering in OpenGL
   glEnable(GL_DEPTH_TEST);
   //Enable Face Culling
   glEnable(GL_CULL_FACE);
   //Enable Textures
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , tMode?GL_REPLACE:GL_MODULATE);

   //  Undo previous transformations
   glLoadIdentity();
   //  Perspective - set eye position
   printf("mode : %d\n", mode);
   
   
 spX = xPositionCar[BLACK_CAR];
 spY = 0.5;
 spZ = zPositionCar[BLACK_CAR];
   if (mode == 1)
   {
      double Ex = -2*dim*Sin(th)*Cos(ph);
      double Ey = +2*dim        *Sin(ph);
      double Ez = +2*dim*Cos(th)*Cos(ph);
      gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);
   }
   else if(mode == 2){
   	printf("MODEEEEE 2\n");
   	  refX = (dim * Sin(th)) + fpX;
      refY = (dim * Sin(ph));
      refZ = (dim * -Cos(th)) + fpZ;
      if(spX > 0) {
      	gluLookAt(spX,spY,spZ, spX+2,spY,spZ, 0,1,0);
	  } else if(spX < 0) gluLookAt(spX,spY,spZ, spX-2,spY,spZ, 0,1,0);
      
//       if(spZ > 0) {
//      	gluLookAt(spX,spY,spZ, spX,spY,spZ+2, 0,1,0);
//	  } else gluLookAt(spX,spY,spZ, spX,spY,spZ-2, 0,1,0);
   }
   //  First Person
   else
   {
      refX = (dim * Sin(th)) + fpX;
      refY = (dim * Sin(ph));
      refZ = (dim * -Cos(th)) + fpZ;
      gluLookAt(fpX,fpY,fpZ, refX,refY,refZ, 0,1,0);
   }

   //  Flat or smooth shading
   glShadeModel(smooth ? GL_SMOOTH : GL_FLAT);

   //  Light position
   float Position[]  = {distance*Cos(zh),distance*Sin(zh),0,1.0};
   // float MoonPosition[]  = {-(distance*Cos(zh)),-(distance*Sin(zh)),0,1.0};

   //  Draw light position as ball (still no lighting here)
   glColor3f(1,1,1);
   ball(Position[0],Position[1],Position[2] , 1); //Sun
//    ball(MoonPosition[0],MoonPosition[1],MoonPosition[2], 0.3); //Moon

   //  OpenGL should normalize normal vectors
   glEnable(GL_NORMALIZE);
   //  Enable lighting
   glEnable(GL_LIGHTING);
   //  Location of viewer for specular calculations
   glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,local);
   //  glColor sets ambient and diffuse color materials
   glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
   glEnable(GL_COLOR_MATERIAL);

   //  Enable light 0 - Sun
   glEnable(GL_LIGHT0);
   //  Set ambient, diffuse, specular components and position of light 0 (Sun)
   glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
   glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
   glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
   glLightfv(GL_LIGHT0,GL_POSITION,Position);
   

   //Inital values for texture repitition
   double texRepX = 1.0;
   double texRepY = 1.0;

   //  Set specular color to white
   float white[] = {1,1,1,1};
   float black[] = {0,0,0,1};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

   //Enable offset for windows, doors, and other decals
   glPolygonOffset(1,1);
   glEnable(GL_POLYGON_OFFSET_FILL);

   //Lights for street lamps
   float amb[4] = {1,.5,0,0};
   float dif[4] = {0.8,0.8,.8,0.1,1};
   float spec[4] = {0.8,0.0,0.0,1};

   float posThree[4] = {16,1.1,-3,1.0};
   float posFour[4] = {14,1.1,-3,1.0};
   float posFive[4] = {16,1.1,3,1.0};
   float posSix[4] = {14,1.1,3,1.0};
   //tambahan
   float posSeven[4] = {16,1.1,-8,1.0};
   float posEight[4] = {14,1.1,-8,1.0};
   float posNine[4] = {16,1.1,8,1.0};
   float posTen[4] = {14,1.1,8,1.0};

   if(Sin(zh) <= 0) {
      glEnable(GL_LIGHT3);
      glLightfv(GL_LIGHT3,GL_AMBIENT ,amb);
      glLightfv(GL_LIGHT3,GL_DIFFUSE ,dif);
      glLightfv(GL_LIGHT3,GL_SPECULAR,spec);
      glLightfv(GL_LIGHT3,GL_POSITION,posThree);

      glLightf(GL_LIGHT3,GL_CONSTANT_ATTENUATION ,at0/100.0);
      glLightf(GL_LIGHT3,GL_LINEAR_ATTENUATION   ,at1/100.0);
      glLightf(GL_LIGHT3,GL_QUADRATIC_ATTENUATION,at2/100.0);

      glEnable(GL_LIGHT4);
      glLightfv(GL_LIGHT4,GL_AMBIENT ,amb);
      glLightfv(GL_LIGHT4,GL_DIFFUSE ,dif);
      glLightfv(GL_LIGHT4,GL_SPECULAR,spec);
      glLightfv(GL_LIGHT4,GL_POSITION,posFour);

      glLightf(GL_LIGHT4,GL_CONSTANT_ATTENUATION ,at0/100.0);
      glLightf(GL_LIGHT4,GL_LINEAR_ATTENUATION   ,at1/100.0);
      glLightf(GL_LIGHT4,GL_QUADRATIC_ATTENUATION,at2/100.0);

      glEnable(GL_LIGHT5);
      glLightfv(GL_LIGHT5,GL_AMBIENT ,amb);
      glLightfv(GL_LIGHT5,GL_DIFFUSE ,dif);
      glLightfv(GL_LIGHT5,GL_SPECULAR,spec);
      glLightfv(GL_LIGHT5,GL_POSITION,posFive);

      glLightf(GL_LIGHT5,GL_CONSTANT_ATTENUATION ,at0/100.0);
      glLightf(GL_LIGHT5,GL_LINEAR_ATTENUATION   ,at1/100.0);
      glLightf(GL_LIGHT5,GL_QUADRATIC_ATTENUATION,at2/100.0);

      glEnable(GL_LIGHT6);
      glLightfv(GL_LIGHT6,GL_AMBIENT ,amb);
      glLightfv(GL_LIGHT6,GL_DIFFUSE ,dif);
      glLightfv(GL_LIGHT6,GL_SPECULAR,spec);
      glLightfv(GL_LIGHT6,GL_POSITION,posSix);

      glLightf(GL_LIGHT6,GL_CONSTANT_ATTENUATION ,at0/100.0);
      glLightf(GL_LIGHT6,GL_LINEAR_ATTENUATION   ,at1/100.0);
      glLightf(GL_LIGHT6,GL_QUADRATIC_ATTENUATION,at2/100.0);
      
   } else {
      glDisable(GL_LIGHT3);
      glDisable(GL_LIGHT4);
      glDisable(GL_LIGHT5);
      glDisable(GL_LIGHT6);
      glDisable(GL_LIGHT7);
   }
	

	//Draw scene
	
	//Skybox
	skybox(64);
	
	//finsih line
	glColor3f(1, 1, 1);
	glBindTexture(GL_TEXTURE_2D,_textureFinishLine);
	texScale = 0.5;
	double finsihAngle = 90;
	double xFinish = 1;
	double yFinish = -0.09;
	double zFinish = 0.50;
	double zGapuraFinish = zFinish - 1;
	int i;
	for(i = 0; i < 3; i++) {
		cube(xFinish,yFinish,zFinish, 0.5,0.1,0.5, finsihAngle);
		zFinish = zFinish + 1;
	}
	
	//Gapura Finish
    gapura(xFinish,zGapuraFinish,GAPURA_X, _textureCarGrill, _textureFinishLine);
    
    //Gapura Iklan
    int amountOfAdsGapura = 3;
    GLuint textureAds[3] = 
    	{
    		_textureAdsMonster,
    		_textureAdsPertamina,
    		_textureAdsMotul
		};
    float xAdsGapura = 10;
    float yAdsGapura = 11.25;
    int j;
    for(j = 0;j<amountOfAdsGapura; j++) {
    	gapura(xAdsGapura,yAdsGapura,GAPURA_X, textureAds[j], textureAds[j]);
    	xAdsGapura -= 10; 
	}
	
	//Terowongan
	double xTunnel = 16.25;
	double yTunnel = -8;
	tunnel(xTunnel,yTunnel,GAPURA_Y, _textureCarGrill, _textureCarGrill);
    
	//Street surface - Main Street
	glColor3f(0.4, 0.4, 0.4);
	glBindTexture(GL_TEXTURE_2D,_textureAsphalt);
	texScale = 0.5;
	float xPos = -6;
	for(i = 0; i < 7; i++){
		//Men-skip tile finish-line
		if(xPos != 1) {
			//ruas jalan -- jarak lajur : 1.5
			cube(xPos,-0.1,0.75, 1,0.1,0.75, 0);
			cube(xPos,-0.1,2.25, 1,0.1,0.75, 0);
			xPos += 2;
		} 
	
	}
	
	//turn-1-left
	turnCircuit(xPos,-0.1,0.75, 1,0.1,0.75, 0);
	
	//Street surface - turn left1
	glColor3f(0.4, 0.4, 0.4);
	glBindTexture(GL_TEXTURE_2D,_textureAsphalt);
	texScale = 0.5;
	float yPos = -2;
	float defTurnCoord = 0.7;
	for(i = 0; i < 4; i++){
		cube(xPos+defTurnCoord,-0.1,yPos, 1,0.1,0.75, 90);
		cube(xPos+defTurnCoord+1.5,-0.1,yPos, 1,0.1,0.75, 90);
		yPos -= 2;
	}
	
	//Street surface - turn left1 duplicate in bottom (z coordinate)
	yPos =-8;
	for(i = 0; i < 4; i++){
		cube(-8.80,-0.1,yPos, 1,0.1,0.75, 90);
		cube(-10.30,-0.1,yPos, 1,0.1,0.75, 90);
		yPos += 2;
	}
	
	//turn-8-left
	turnCircuit(-8.80,-0.1,yPos, 1,0.1,0.75,270);
	
	//turn-2-right
	turnCircuit(xPos+2.90,-0.1,-10.7, 1,0.1,0.75,180);
	
	//Street surface - stright1
	glColor3f(0.4, 0.4, 0.4);
	glBindTexture(GL_TEXTURE_2D,_textureAsphalt);
	texScale = 0.5;
	float xPos2 = 13;
	for(i = 0; i < 2; i++){
		//ruas jalan
		cube(xPos2-0.1,-0.1,-12.25, 1,0.1,0.75, 0);
		cube(xPos2-0.1,-0.1,-10.75, 1,0.1,0.75, 0);
		xPos2 += 2;
	}
	
	//Street surface - stright1 duplicate bottom
	xPos2 = -15;
	//turn-6-right
	turnCircuit(xPos2-2.20,-0.1,-10.7, 1,0.1,0.75,180);
	for(i = 0; i < 2; i++){
	//ruas jalan
		cube(xPos2-0.2,-0.1,-12.20, 1,0.1,0.75, 0);
		cube(xPos2-0.2,-0.1,-10.70, 1,0.1,0.75, 0);
		xPos2 += 2;
	}
	//turn-7right
	turnCircuit(xPos2+0.70,-0.1,-10, 1,0.1,0.75,90);
	
	
	//turn-3right
	turnCircuit(xPos+9.75,-0.1,-10.10, 1,0.1,0.75,90);
	
	//Street surface - turn right1
	glColor3f(0.4, 0.4, 0.4);
	glBindTexture(GL_TEXTURE_2D,_textureAsphalt);
	texScale = 0.5;
	float yPos2 = -8.25;
	
	for(i = 0; i < 10; i++){
		cube(17.80,-0.1,yPos2, 1,0.1,0.75, 90);
		cube(19.30,-0.1,yPos2, 1,0.1,0.75, 90);
		yPos2 += 2;
	}
	
	//turn-4-right
	turnCircuit(xPos+9.10,-0.1,12.60, 1,0.1,0.75,0);
	
	//Street surface - stright2
	glColor3f(0.4, 0.4, 0.4);
	glBindTexture(GL_TEXTURE_2D,_textureAsphalt);
	texScale = 0.5;
	float xPos3 = -15;
	//turn-5-left
	turnCircuit(xPos3-2.75,-0.1,12, 1,0.1,0.75,270);
	for(i = 0; i < 16; i++){
	//ruas jalan
		cube(xPos3+0.1,-0.1,12.65, 1,0.1,0.75, 0);
		cube(xPos3+0.1,-0.1,14.15, 1,0.1,0.75, 0);
		xPos3 += 2;
	}
	
	//Street surface - bottom
	glColor3f(0.4, 0.4, 0.4);
	glBindTexture(GL_TEXTURE_2D,_textureAsphalt);
	texScale = 0.5;
	float yPos3 = -8;
	
	for(i = 0; i < 10; i++){
		cube(-17.80,-0.1,yPos3, 1,0.1,0.75, 90);
		cube(-19.30,-0.1,yPos3, 1,0.1,0.75, 90);
		yPos3 += 2;
	}
	
	//Ads board	
	adboard(-20.25, 1, GAPURA_X, _textureAdsMotul, _textureAdsMotul);	
	
	//Main Stand (Tribun)
	stand(4,0.2,-3, 2,0.15,2, 0); //left
	stand(0,0.2,-3, 2,0.15,2, 0); //middle
	stand(-4,0.2,-3, 2,0.15,2, 0); //right
	
	//Stand (Tribun) - 2
	stand(10,0.2,9, 2,0.15,2, 0); //left
	stand(-10,0.2,9, 2,0.15,2, 0); //right
	
	
	// Cover the remain spaces with :
	//Grass Square
	glColor3f(0.7, 0.7, 0.7);
	glBindTexture(GL_TEXTURE_2D,_textureGrass);
	texScale = 0.5;
	cube(0,-0.05,0, 22,0,22, 0);
	
	car(xPositionCar[BLUE_CAR],0.13,zPositionCar[BLUE_CAR], 1,1,1, turnDegree[BLUE_CAR], 0,0,0.8);
	
   //Red Car
   car(xPositionCar[RED_CAR],0.13,zPositionCar[RED_CAR], 1,1,1, turnDegree[RED_CAR], 0.8,0,0);

   //Black Car
   car(xPositionCar[BLACK_CAR],0.13,zPositionCar[BLACK_CAR], 1,1,1, turnDegree[BLACK_CAR], 0,0,0);
   
   //Green Car - Lawan arah
   car(xPositionCar[GREEN_CAR],0.13,zPositionCar[GREEN_CAR], 1,1,1, turnDegree[GREEN_CAR], 0,0.8,0);
   
   //Cyan Car
   car(xPositionCar[CYAN_CAR],0.13,zPositionCar[CYAN_CAR], 1,1,1, turnDegree[CYAN_CAR], 0,0.8,0.8);
//		printf("XPosition : %f, TurnDegree : %d, Zposition : %f\n ",
//		 xPositionCar[1], turnDegree[1], zPositionCar[1]);
	
	//Car Movement
	if(!pause) {
		float speed = 0.2;
	float turnSpeed = speed-0.11;
	int turnCount; 
	int car;
	

	printf("turnCount nya - %d\n", turnCount);
	
	for(car = 0; car < AMOUNT_OF_CARS; car++) {
	//lawan arah (belum beres)
//		if(car == GREEN_CAR) {
//			turnCount = (2/turnSpeed)+5;
//			if(xPositionCar[car] >= -8 && xPositionCar[car] < -6 && zPositionCar[car] < 3 ) {
//		     	turnMode(TURN_SOUTHWEST, TURN_RIGHT, car);  //turn-1
//			} else if(xPositionCar[car] >= -12 && xPositionCar[car] <= -10 & zPositionCar[car] < -8 ) {
//				turnMode(TURN_SOUTHWEST, TURN_LEFT, car); //turn-2
//			} else if(xPositionCar[car] >= -15 && xPositionCar[car] <= -14 && zPositionCar[car] > -15 ) {
//				turnMode(TURN_SOUTHEAST, TURN_LEFT, car); //turn-3
//			} 
			//yang salah, yang diatas udh berner buat lawan arahnya. Kapan ya aku ngebenrin ini. 
//			Ini  di tulis tanggal 21 Januari 2020 3:10 PM. Habis dengerin Arctic Monkey - A Certain Romance
//			else if(zPositionCar[car] >= 10.2 && zPositionCar[car] <= 10.4 && xPositionCar[car] > 15 ) {
//				turnMode(TURN_SOUTHEAST, TURN_RIGHT, car); //turn-4
//			} else if(xPositionCar[car] >= -15.4 && xPositionCar[car] <= -15 && zPositionCar[car] > 12) {
//				turnMode(TURN_SOUTHWEST, TURN_RIGHT, car); //turn-5
//			} else if(zPositionCar[car] >= -8.4 && zPositionCar[car] <= -8 && xPositionCar[car] < -16) {
//				turnMode(TURN_NORTHWEST, TURN_RIGHT, car); //turn-6
//			} else if(xPositionCar[car] >= -12.4 && xPositionCar[car] <= -12 && zPositionCar[car] < -8) {
//				turnMode(TURN_NORTHEAST, TURN_RIGHT, car); //turn-7
//			} else if(zPositionCar[car] >= -1.4 && zPositionCar[car] <= -1 && xPositionCar[car] < -8 && xPositionCar[car] > -10) {
//				turnMode(TURN_NORTHEAST, TURN_LEFT, car), car; //turn-8
//			}
//		} 
//		else {
	//Arah yang benar
		
	   //beda jalur
	   if(car%2 == 0) turnCount = (1.5/turnSpeed)+5;
 	   else turnCount = (2.4/turnSpeed)+5;
		
		if(xPositionCar[car] >= 6 && xPositionCar[car] < 6.4 && zPositionCar[car] < 3 ) {
	     	turnMode(TURN_NORTHWEST, TURN_LEFT, car);  //turn-1
		} else if(zPositionCar[car] >= -10 && zPositionCar[car] <= -8 && xPositionCar[car] > 8 && xPositionCar[car] <= 10 ) {
			turnMode(TURN_NORTHWEST, TURN_RIGHT, car); //turn-2
		} else if(xPositionCar[car] >= 15.4 && xPositionCar[car] <= 15.8 && zPositionCar[car] < -8) {
			turnMode(TURN_NORTHEAST, TURN_RIGHT, car); //turn-3
		} else if(zPositionCar[car] >= 10.2 && zPositionCar[car] <= 10.4 && xPositionCar[car] > 15 ) {
			turnMode(TURN_SOUTHEAST, TURN_RIGHT, car); //turn-4
		} else if(xPositionCar[car] >= -15.4 && xPositionCar[car] <= -15 && zPositionCar[car] > 12) {
			turnMode(TURN_SOUTHWEST, TURN_RIGHT, car); //turn-5
		} else if(zPositionCar[car] >= -8.4 && zPositionCar[car] <= -8 && xPositionCar[car] < -16) {
			turnMode(TURN_NORTHWEST, TURN_RIGHT, car); //turn-6
		} else if(xPositionCar[car] >= -12.4 && xPositionCar[car] <= -12 && zPositionCar[car] < -8) {
			turnMode(TURN_NORTHEAST, TURN_RIGHT, car); //turn-7
		} else if(zPositionCar[car] >= -1.4 && zPositionCar[car] <= -1 && xPositionCar[car] < -8 && xPositionCar[car] > -12) {
			turnMode(TURN_NORTHEAST, TURN_LEFT, car), car; //turn-8
		}
//	}
		
		//Car Movemen - Turn Counter
		if(turnDegreeCount[car] >= turnCount) {
			if(isTurnNorthwest[car]) {
				if(turnStatus[car] == TURN_LEFT) {
					turnDegree[car] = 90;
					isStright[car] = true;	
				} else if (turnStatus[car] == TURN_RIGHT) {
					turnDegree[car] = 0;
					isStright[car] = true;
				}
			}
			
			if(isTurnNortheast[car]) {
				if(turnStatus[car] == TURN_LEFT) {
					turnDegree[car]= 0;
					isStright[car] = true;	
				} else if (turnStatus[car] == TURN_RIGHT) {
					turnDegree[car] = 270;
					isStright[car] = true;
				}
			}
			 
			if(isTurnSoutheast[car]) {
				if(turnStatus[car] == TURN_LEFT) {
					turnDegree[car] = 270;
					isStright[car] = true;	
				} else if (turnStatus[car] == TURN_RIGHT) {
					turnDegree[car] = 180;
					isStright[car] = true;
				}
			}
			
			if(isTurnSouthwest[car]) {
				if(turnStatus[car] == TURN_LEFT) {
					turnDegree[car] = 180;	
					isStright[car] = true;	
				} else if (turnStatus[car] == TURN_RIGHT) {
					turnDegree[car] = 90;
					isStright[car] = true;
				}
			}
		}
		
		//Car Movement : Speed
		if(isStright[car]) {
			//Kalau dia jalan lurus
			if(turnDegree[car] == 90) {
				zPositionCar[car] = zPositionCar[car] - speed;
			} else if(turnDegree[car] == 270) {
				zPositionCar[car] = zPositionCar[car] + speed;
			} else if(turnDegree[car] == 180) {
				xPositionCar[car] = xPositionCar[car] - speed;
			} else {
				xPositionCar[car] = xPositionCar[car] + speed;
			}
		}else {
			//Kalau dia belok
			if(isTurnNorthwest[car]) {
				xPositionCar[car] = xPositionCar[car] + turnSpeed;
				zPositionCar[car] = zPositionCar[car] - turnSpeed;
				turnDegreeCount[car]++;
			} else if(isTurnNortheast[car]) {
				xPositionCar[car] = xPositionCar[car] + turnSpeed;
				zPositionCar[car] = zPositionCar[car] + turnSpeed;
				turnDegreeCount[car]++;
			} else if(isTurnSoutheast[car]) {
				xPositionCar[car] = xPositionCar[car] - turnSpeed;
				zPositionCar[car] = zPositionCar[car] + turnSpeed;
				turnDegreeCount[car]++;
			} else if(isTurnSouthwest[car]) {
				xPositionCar[car] = xPositionCar[car] - turnSpeed;
				zPositionCar[car] = zPositionCar[car] - turnSpeed;
				turnDegreeCount[car]++;
			}
		}
	}

	}
	
   
//   car(-1,0.13,0.8, 1,1,1, 0, 0.8,0,0); 
   
   
//   cube( -6,-0.1,0.75, 1,0.1,0.75, 0);
//
//   //Grass Lawn - Far right
//   glColor3f(0.7, 0.7, 0.7);
//   glBindTexture(GL_TEXTURE_2D,_textureGrass);
//   texScale = 0.5;
//   int zPos = -3;
//
//   for(i = 0; i < 7; i++) {
//      cube(-12,-0.05,zPos, 1,0.15,1, 0);
//      zPos += 2;
//   }

   //  Display parameters
  // glWindowPos2i(5,5);
   Print("Angle=%d,%d  Dim=%.1f  FOV=%d  Projection=%s",th,ph,dim,fov,mode?"Perpective":"FP");

   //  Render the scene and make it visible
   ErrCheck("display");
   glFlush();
   glutSwapBuffers();
}



static void setEnvLighting()
{
   float ambScale = 0.8;

   if(Sin(zh) >= 0.2) {
      Ambient[0] = (255 / 255) * ambScale;
      Ambient[1] = (255 / 255) * ambScale;
      Ambient[2] = (255 / 255) * ambScale;

      Diffuse[0] = 1.0;
      Diffuse[1] = 1.0;
      Diffuse[2] = 1.0;
   } else if (Sin(zh) < 0.4 && Sin(zh) >= 0) {
      Ambient[0] = (255 / 255) * ambScale;
      Ambient[1] = ((60 + (195*Sin(zh)*2.5)) / 255) * ambScale;
      Ambient[2] = ((60 + (195*Sin(zh)*2.5)) / 255) * ambScale;

      Diffuse[0] = Sin(zh)*2.5;
      Diffuse[1] = Sin(zh)*2.5;
      Diffuse[2] = Sin(zh)*2.5;
   } else if (Sin(zh) < 0 && Sin(zh) >= -0.4){
      Ambient[0] = ((255 - (235*Sin(zh)*-2.5)) / 255) * ambScale;
      Ambient[1] = (60 / 255) * ambScale;
      Ambient[2] = ((60 + (120*Sin(zh)*-2.5)) / 255) * ambScale;

      Diffuse[0] = 0;
      Diffuse[1] = 0;
      Diffuse[2] = 0;
   } else {
      Ambient[0] = (20 / 255) * ambScale;
      Ambient[1] = (60 / 255) * ambScale;
      Ambient[2] = (180 / 255) * ambScale;

      Diffuse[0] = 0;
      Diffuse[1] = 0;
      Diffuse[2] = 0;
   }
}


/*
 *  GLUT calls this routine when the window is resized
 */
void idle()
{
   //  Elapsed time in seconds
   double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
   zh = fmod(10*t,360.0);

   setEnvLighting();
   
   //car animation
//   printf("xPositionCar : %d", xPositionCar);

   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
   //  Right arrow key - increase angle by 5 degrees
   if (key == GLUT_KEY_RIGHT)
      th += 5;
   //  Left arrow key - decrease angle by 5 degrees
   else if (key == GLUT_KEY_LEFT)
      th -= 5;
   //  Up arrow key - increase elevation by 5 degrees
   else if (key == GLUT_KEY_UP)
      ph += 5;
   //  Down arrow key - decrease elevation by 5 degrees
   else if (key == GLUT_KEY_DOWN)
      ph -= 5;
   //  PageUp key - increase dim
   else if (key == GLUT_KEY_PAGE_DOWN)
      dim += 0.1;
   //  PageDown key - decrease dim
   th %= 360;
   ph %= 360;
   //  Update projection
   Project(fov,asp,dim);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
   //  Exit on ESC
   if (ch == 27)
      exit(0);
   //  Reset view angle
   else if (ch == '0')
      th = ph = 0;
   //  Toggle axes
   else if (ch == 'x' || ch == 'X')
      axes = 1-axes;
   //  Switch  mode
   else if (ch == 'p' || ch == 'P') {
      if (mode == 0) mode = 1;
      else if(mode == 1) mode = 2;
      else mode = 0;
  }
   //  Toggle light movement
   else if (ch == 'm' || ch == 'M')
      move = 1-move;
   //  Change field of view angle
   else if (ch == '-' && ch>1)
      fov--;
   else if (ch == '+' && ch<179)
      fov++;
   else if (key == GLUT_KEY_PAGE_UP && dim>1)
      dim -= 0.1;
   //Prepektif orang mati
   	   else if (ch == 'W' || ch == 'w')
         if(mode == 0) fpX  += 2;
         else refX += 2;
       else if (ch == 'A' || ch == 'a')
         if(mode == 0) fpZ -= 2;
         else refZ -= 2;
       else if (ch == 'D' || ch == 'd')
         if(mode == 0) fpZ += 2;
         else refZ += 2;
       else if (ch == 'S' || ch == 's')
         if(mode == 0) fpX -= 2;
         else refX -= 2;
       else if (ch == 'Q' || ch == 'q')
         if(mode == 0) fpY += 2;
         else refY += 2;
       else if (ch == 'E' || ch == 'e')
         if(mode == 0) fpY -= 2;
         else refY -= 2;
    //pause - Spacebar
    else if (ch == 32)
    	if(pause == false) pause = true;
    	else pause = false;
    //
    	
   //  Reproject
   Project(fov,asp,dim);
   //  Animate if requested
   glutIdleFunc(move?idle:NULL);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
   //  Ratio of the width to the height of the window
   asp = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, width,height);
   //  Set projection
   Project(fov,asp,dim);
}


/*
 * Init Texture BMP
 */
void init(){
	_textureAsphalt = LoadTexBMP("asphalt.bmp");
	_textureGrass = LoadTexBMP("grass.bmp");
	_textureSkyboxBack = LoadTexBMP("skybox-back.bmp");
	_textureSkyboxFront = LoadTexBMP("skybox-front.bmp");
	_textureSkyboxLeft = LoadTexBMP("skybox-left.bmp");
	_textureSkyboxRight = LoadTexBMP("skybox-right.bmp");
	_textureSkyboxTop = LoadTexBMP("skybox-top.bmp");
	_textureStandWall = LoadTexBMP("stand-wall.bmp");
	_textureStandSeats = LoadTexBMP("stand-seats.bmp");
	_textureBasicMetal = LoadTexBMP("basic-metal.bmp"); 
	_textureGlass = LoadTexBMP("glass.bmp");
	_textureCarWheel = LoadTexBMP("car-wheel.bmp");
	_textureTireTread = LoadTexBMP("tire-tread.bmp");
	_textureHeadLamp = LoadTexBMP("headlamp.bmp");
	_textureCarbonFiber = LoadTexBMP("carbon-fiber.bmp");
	_textureCarGrill = LoadTexBMP("car-grill.bmp");
	_textureFinishLine = LoadTexBMP("finish-line.bmp");
	_textureRacingTrafficLight = LoadTexBMP("racing-traffic-light.bmp");
	_textureGapuraFinishPilar = LoadTexBMP("gapura-finish-pilar.bmp");
	_textureAdsMonster = LoadTexBMP("ad-monster-logo.bmp");
    _textureAdsPertamina = LoadTexBMP("ad-pertamina-logo.bmp");
    _textureAdsMotul = LoadTexBMP("ad-motul-logo.bmp");
}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
   //  Initialize GLUT
   glutInit(&argc,argv);
   //  Request double buffered, true color window with Z buffering at 800x600
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   glutInitWindowSize(800,600);
   glutCreateWindow("171511036_Alif Ramdani - UAS CG");
   //  Set callbacks
   init();
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);
   glutIdleFunc(idle);
   //  Pass control to GLUT so it can interact with the user
   ErrCheck("init");
   glutMainLoop();
   return 0;
}

//archive


//   cube(xPos-0.5,-0.1,1.88, 1,0.1,0.75, 30);
//   cube(xPos,-0.1,1.50, 1,0.1,0.75, 60);
  
  
//  float sizeBlock = 4;
//	float x = -2;
//	float y = 1;
//	float z = 2;
//	float dx = 0.5 * sizeBlock; // biar simetris
//	float dy = 0.1;
//	float dz = 0.5 * sizeBlock;
//	float turn = dx*4/sizeBlock;
   
//   circle(x+(sizeBlock/2),y,z+(sizeBlock/2),dx,dy,dz,180,turn);

//	if(xPositionCar >= 6 && xPositionCar < 6.1 && zPositionCar < 3) {
//     	turnMode(TURN_NORTHWEST, TURN_LEFT);  //turn-1
//	} else if(zPositionCar >= -8.1 && zPositionCar <= -8 && xPositionCar > 8 && xPositionCar <= 9 ) {
//		turnMode(TURN_NORTHWEST, TURN_RIGHT); //turn-2
//	} else if(xPositionCar >= 15.5 && xPositionCar <= 15.6 && zPositionCar < -8) {
//		turnMode(TURN_NORTHEAST, TURN_RIGHT); //turn-3
//	} else if(zPositionCar >= 10.2 && zPositionCar <= 10.3 && xPositionCar > 15 ) {
//		turnMode(TURN_SOUTHEAST, TURN_RIGHT); //turn-4
//	} else if(xPositionCar >= -15.1 && xPositionCar <= -15 && zPositionCar > 12) {
//		turnMode(TURN_SOUTHWEST, TURN_RIGHT); //turn-5
//	} else if(zPositionCar >= -8.1 && zPositionCar <= -8 && xPositionCar < -16) {
//		turnMode(TURN_NORTHWEST, TURN_RIGHT); //turn-6
//	} else if(xPositionCar >= -12.1 && xPositionCar <= -12 && zPositionCar < -8) {
//		turnMode(TURN_NORTHEAST, TURN_RIGHT); //turn-7
//	} else if(zPositionCar >= -1.1 && zPositionCar <= -1 && xPositionCar < -9 && xPositionCar > -10) {
//		turnMode(TURN_NORTHEAST, TURN_LEFT); //turn-8
//	}
//	
//	//Car Movemen - Turn Counter
//	if(turnDegreeCount >= turnCount) {
//		if(isTurnNorthwest) {
//			if(turnStatus == TURN_LEFT) {
//				turnDegree = 90;
//				isStright = true;	
//			} else if (turnStatus == TURN_RIGHT) {
//				turnDegree = 0;
//				isStright = true;
//			}
//		}
//		
//		if(isTurnNortheast) {
//			if(turnStatus == TURN_LEFT) {
//				turnDegree = 0;
//				isStright = true;	
//			} else if (turnStatus == TURN_RIGHT) {
//				turnDegree = 270;
//				isStright = true;
//			}
//		}
//		 
//		if(isTurnSoutheast) {
//			if(turnStatus == TURN_LEFT) {
//				turnDegree = 270;
//				isStright = true;	
//			} else if (turnStatus == TURN_RIGHT) {
//				turnDegree = 180;
//				isStright = true;
//			}
//		}
//		
//		if(isTurnSouthwest) {
//			if(turnStatus == TURN_LEFT) {
//				turnDegree = 180;	
//				isStright = true;	
//			} else if (turnStatus == TURN_RIGHT) {
//				turnDegree = 90;
//				isStright = true;
//			}
//		}
//	}
//	
//	//Car Movement : Speed
//	if(isStright) {
//		//Kalau dia jalan lurus
//		if(turnDegree == 90) {
//			zPositionCar = zPositionCar - speed;
//		} else if(turnDegree == 270) {
//			zPositionCar = zPositionCar + speed;
//		} else if(turnDegree == 180) {
//			xPositionCar = xPositionCar - speed;
//		} else {
//			xPositionCar = xPositionCar + speed;
//		}
//	}else {
//		//Kalau dia belok
//		if(isTurnNorthwest) {
//			xPositionCar = xPositionCar + turnSpeed;
//			zPositionCar = zPositionCar - turnSpeed;
//			turnDegreeCount++;
//		} else if(isTurnNortheast) {
//			xPositionCar = xPositionCar + turnSpeed;
//			zPositionCar = zPositionCar + turnSpeed;
//			turnDegreeCount++;
//		} else if(isTurnSoutheast) {
//			xPositionCar = xPositionCar -turnSpeed;
//			zPositionCar = zPositionCar + turnSpeed;
//			turnDegreeCount++;
//		} else if(isTurnSouthwest) {
//			xPositionCar = xPositionCar - turnSpeed;
//			zPositionCar = zPositionCar - turnSpeed;
//			turnDegreeCount++;
//		}
//	}
