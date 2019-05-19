#include <GL/glut.h>
#include "GL/freeglut.h"    
#include <iostream>
#include <math.h>
#include "objLoader.cpp"
#include <glm/vec3.hpp>
using namespace std;

#define ESC 27
#define ENTER 13
#define w 119
#define s 115
#define a 97
#define d 100




GLfloat shadowMat[4][4];

std::vector<glm::vec3> vertices;
bool res = objLoader("bunny/bunny.obj", vertices);

// Rotation amounts
static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;
static GLfloat mouseSpeed = 0.05f;
static GLfloat scale = 1.0f;

// mouse cameraX, cameraY, cameraZ, lookX, lookY, lookZ
static GLfloat cameraX = 0.0f;
static GLfloat cameraY = 0.0f;
static GLfloat cameraZ = 0.0f;

static GLfloat lookX = 0.0f;
static GLfloat lookY = 0.0f;
static GLfloat lookZ = 0.0f;


void drawSquare(GLfloat len, GLfloat x, GLfloat y) {
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(x, y);
        glVertex2f(x, y + len);
        glVertex2f(x + len, y + len);
        glVertex2f(x + len, y);
    glEnd();
}

////////////////////////////////////////////////////////////////////////////
// Change viewing volume and viewport.  Called when window is resized
void ChangeSize(int width, int height) {
    GLfloat fAspect;

    // Prevent a divide by zero
    if(height == 0)
        height = 1;

    // Set Viewport to window dimensions
    glViewport(0, 0, width, height);

    fAspect = (GLfloat)width / (GLfloat)height;

    // Reset coordinate system
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Produce the perspective projection
    gluPerspective(35.0f, fAspect, 1, 500.0);
    // glOrtho(-50.0, 50.0, -50.0, 50.0, 100.0, -100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void ReduceToUnit(float vector[3])
	{
	float length;
	
	// Calculate the length of the vector		
	length = (float)sqrt((vector[0]*vector[0]) + 
						(vector[1]*vector[1]) +
						(vector[2]*vector[2]));

	// Keep the program from blowing up by providing an exceptable
	// value for vectors that may calculated too close to zero.
	if(length == 0.0f)
		length = 1.0f;

	// Dividing each element by the length will result in a
	// unit normal vector.
	vector[0] /= length;
	vector[1] /= length;
	vector[2] /= length;
	}


// Points p1, p2, & p3 specified in counter clock-wise order
void calcNormal(float v[3][3], float out[3])
	{
	float v1[3],v2[3];
	static const int x = 0;
	static const int y = 1;
	static const int z = 2;

	// Calculate two vectors from the three points
	v1[x] = v[0][x] - v[1][x];
	v1[y] = v[0][y] - v[1][y];
	v1[z] = v[0][z] - v[1][z];

	v2[x] = v[1][x] - v[2][x];
	v2[y] = v[1][y] - v[2][y];
	v2[z] = v[1][z] - v[2][z];

	// Take the cross product of the two vectors to get
	// the normal vector which will be stored in out
	out[x] = v1[y]*v2[z] - v1[z]*v2[y];
	out[y] = v1[z]*v2[x] - v1[x]*v2[z];
	out[z] = v1[x]*v2[y] - v1[y]*v2[x];

	// Normalize the vector (shorten length to one)
	ReduceToUnit(out);
	}

void MakeShadowMatrix(GLfloat points[3][3], GLfloat lightPos[4], GLfloat destMat[4][4])
	{
	GLfloat planeCoeff[4];
	GLfloat dot;

	// Find the plane equation coefficients
	// Find the first three coefficients the same way we
	// find a normal.
	calcNormal(points,planeCoeff);

	// Find the last coefficient by back substitutions
	planeCoeff[3] = - (
		(planeCoeff[0]*points[2][0]) + (planeCoeff[1]*points[2][1]) +
		(planeCoeff[2]*points[2][2]));


	// Dot product of plane and light position
	dot = planeCoeff[0] * lightPos[0] +
			planeCoeff[1] * lightPos[1] +
			planeCoeff[2] * lightPos[2] +
			planeCoeff[3] * lightPos[3];

	// Now do the projection
	// First column
    	destMat[0][0] = dot - lightPos[0] * planeCoeff[0];
    	destMat[1][0] = 0.0f - lightPos[0] * planeCoeff[1];
    	destMat[2][0] = 0.0f - lightPos[0] * planeCoeff[2];
    	destMat[3][0] = 0.0f - lightPos[0] * planeCoeff[3];

	// Second column
	destMat[0][1] = 0.0f - lightPos[1] * planeCoeff[0];
	destMat[1][1] = dot - lightPos[1] * planeCoeff[1];
	destMat[2][1] = 0.0f - lightPos[1] * planeCoeff[2];
	destMat[3][1] = 0.0f - lightPos[1] * planeCoeff[3];

	// Third Column
	destMat[0][2] = 0.0f - lightPos[2] * planeCoeff[0];
	destMat[1][2] = 0.0f - lightPos[2] * planeCoeff[1];
	destMat[2][2] = dot - lightPos[2] * planeCoeff[2];
	destMat[3][2] = 0.0f - lightPos[2] * planeCoeff[3];

	// Fourth Column
	destMat[0][3] = 0.0f - lightPos[3] * planeCoeff[0];
	destMat[1][3] = 0.0f - lightPos[3] * planeCoeff[1];
	destMat[2][3] = 0.0f - lightPos[3] * planeCoeff[2];
	destMat[3][3] = dot - lightPos[3] * planeCoeff[3];
	}

// This function does any needed initialization on the rendering
// context.  Here it sets up and initializes the lighting for
// the scene.
void SetupRC() {

    // Any three points on the ground (counter clockwise order)
	GLfloat points[3][3] = {{ -30.0f, -149.0f, -20.0f },
							{ -30.0f, -149.0f, 20.0f },
							{ 40.0f, -149.0f, 20.0f }};

    GLfloat amb[] = {0.2f, 0.1f, 0.027f, 0.5f};
    GLfloat diff[] = {0.0f, 0.26f, 0.11f, 0.5f};
    GLfloat spec[] = {0.2f, 0.24f, 0.3f, 1.0f};
    GLfloat shine = 1.9f;  



    // Light values and coordinates
    GLfloat  whiteLight[] = { 0.05f, 0.05f, 0.05f, 0.1f };
    GLfloat  sourceLight[] = { 0.25f, 0.25f, 0.25f, 0.1f };
    GLfloat     lightPos[] = { -10.f, 5.0f, 5.0f, 0.1f };

    glEnable(GL_DEPTH_TEST);    // Hidden surface removal
    glFrontFace(GL_CCW);        // Counter clock-wise polygons face out
    // glEnable(GL_CULL_FACE);           // Do not calculate inside

    // Enable lighting
    glEnable(GL_LIGHTING);

    // Setup and enable light 0
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT,whiteLight);
    glLightfv(GL_LIGHT0,GL_AMBIENT,sourceLight);
    glLightfv(GL_LIGHT0,GL_DIFFUSE,sourceLight);
    glLightfv(GL_LIGHT0,GL_POSITION,lightPos);
    glEnable(GL_LIGHT0);

    // Enable color tracking
    glEnable(GL_COLOR_MATERIAL);
    
    // Set Material properties to follow glColor values
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	
	glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diff);
	glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
	glMaterialf(GL_FRONT, GL_SHININESS, shine);


    // Black blue background
    glClearColor(0.25f, 0.25f, 0.50f, 1.0f);

    // Calculate projection matrix to draw shadow on the ground
	MakeShadowMatrix(points, lightPos, shadowMat);
}

// Respond to arrow keys
void SpecialKeys(int key, int x, int y) {
    if(key == GLUT_KEY_UP)
        xRot-= 5.0f;

    if(key == GLUT_KEY_DOWN)
        xRot += 5.0f;

    if(key == GLUT_KEY_LEFT)
        yRot -= 5.0f;

    if(key == GLUT_KEY_RIGHT)
        yRot += 5.0f;
    
    if(key == GLUT_KEY_PAGE_UP)
        scale += 0.1f;

    if(key == GLUT_KEY_PAGE_DOWN)
        scale -= 0.1f;
                
        xRot = (GLfloat)((const int)xRot % 360);
        yRot = (GLfloat)((const int)yRot % 360);

    // Refresh the Window
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case ESC:     // ESC key
            exit(0);
            break;
        case ENTER:
            exit(0);
            break;
        case w:
            cameraZ += 2.0f;
            // lookZ += 2.0f;
            break;
        case s:
            cameraZ -= 2.0f;
            // lookZ -= 2.0f;
            break;
        // case a:
        //     cameraX += 2.0f;
        //     lookX += 2.0f;
        //     break;
        // case d:
        //     cameraX -= 2.0f;
        //     lookX -= 2.0f;
        //     break;
    }
    // Refresh the Window
    glutPostRedisplay();
}

void motion(int x, int y) {
    static bool wrap = false;

    if(!wrap) {
        int ww = glutGet(GLUT_WINDOW_WIDTH);
        int wh = glutGet(GLUT_WINDOW_HEIGHT);

        int dx = x - ww / 2; // subtracting threshold
        int dy = y - wh / 2;

        // cout << dx << " " << dy << endl;

        xRot += dy/2;
        yRot += dx/2;

        xRot = (GLfloat)((const int)xRot % 360);
        yRot = (GLfloat)((const int)yRot % 360);
        
        glutPostRedisplay();

        wrap = true;
        glutWarpPointer(ww / 2, wh / 2);
    } else {
        wrap = false;
    }
}

glm::vec3 calculateNormal(glm::vec3 triangle[3]) {
	glm::vec3 normal;
    glm::vec3 U, V;
    U.x = triangle[1].x - triangle[0].x;
    U.y = triangle[1].y - triangle[0].y;
    U.z = triangle[1].z - triangle[0].z;

    V.x = triangle[2].x - triangle[0].x;
    V.y = triangle[2].y - triangle[0].y;
    V.z = triangle[2].z - triangle[0].z;
    
    normal.x = U.y * V.z - U.z * V.y;
    normal.y = U.z * V.x - U.x * V.z;
    normal.z = U.x * V.y - U.y * V.x;
	//printf("%f %f %f\n", normal.x, normal.y, normal.z);
	float length = (float) sqrt(normal.x * normal.x +
					normal.y * normal.y +
					normal.z * normal.z);
	
	if(length == 0.0f)
		length = 1.0f;

  	normal.x /= length;
   	normal.y /= length;
	normal.z /= length;	
   

	return normal; 
}

// Called to draw scene
void RenderScene(void) {
    
    GLUquadricObj *pObj;


    // Clear the window with current clearing color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
        gluLookAt(cameraX, cameraY, cameraZ - 150,
                  lookX, lookY, lookZ,
                  0.0, 1.0, 0.0);
        // Move object back and do in place rotation
        glTranslatef(0.0f, 0.0f, -100.0f);
        glRotatef(xRot, 1.0f, 0.0f, 0.0f);
        glRotatef(yRot, 0.0f, 1.0f, 0.0f);


        pObj = gluNewQuadric();
        gluQuadricNormals(pObj, GLU_SMOOTH);


        // Plane
        // Desk
            glPushMatrix();
                glTranslatef(0.0f, -5.0f, 0.0f);
                glRotatef(-90, 1, 0, 0);
                glScalef(100, 100, 100);
                // Desk top
                glColor3f(0, 1, 1); 

                glPushMatrix();
                    glTranslatef(0, 0, -0.5);
                    gluDisk(pObj, 0, 2, 4, 1);
                    glNormal3f(0.0f, 0.0f, 1.0f);
                glPopMatrix();
            glPopMatrix();

        // bunny obj
        glPushMatrix();
            // glScalef(0.25, 0.25, 0.25);
        // glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 15.0f);

            glRotatef(-90, 1, 0, 0);
            glColor3f(0, 1, 0);
            glBegin(GL_TRIANGLES);
			glm::vec3 normal;
            for(unsigned int i = 0; i < vertices.size(); i++) {
				if(i % 3 == 0) {	
					glm::vec3 tempNormal[3];
					tempNormal[0].x = vertices[i].x;
					tempNormal[0].y = vertices[i].y;
					tempNormal[0].z = vertices[i].z;

					tempNormal[1].x = vertices[i + 1].x;
					tempNormal[1].y = vertices[i + 1].y;
					tempNormal[1].z = vertices[i + 1].z;

					tempNormal[2].x = vertices[i + 2].x;
					tempNormal[2].y = vertices[i + 2].y;
					tempNormal[2].z = vertices[i + 2].z;

					normal = calculateNormal(tempNormal);
                    glNormal3f(normal.x, normal.y, normal.y);
				}

                
                glVertex3f(vertices[i].x, vertices[i].y, vertices[i].z);
                 
            }
            glEnd();
        glPopMatrix();

    glPopMatrix();

    // Buffer swap
    glutSwapBuffers();
}

 

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Modeling with Quadrics");
    glutReshapeFunc(ChangeSize);
    glutSpecialFunc(SpecialKeys);
    glutDisplayFunc(RenderScene);
    glutKeyboardFunc(keyboard);
    glutPassiveMotionFunc(motion);
    glutSetCursor(GLUT_CURSOR_NONE);
    SetupRC();
    glutMainLoop();
    
    return 0;
    }

