// Snowman.cpp
// Demonstrates using Quadric Objects
// OpenGL SuperBible
// Richard S. Wright Jr.

#include "GL/freeglut.h"    // OpenGL toolkit
#include <iostream>

using namespace std;
#define ESC 27
#define ENTER 13
// Rotation amounts
static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;
static GLfloat mouseSpeed = 0.05f;
static GLfloat scale = 1.0f;


////////////////////////////////////////////////////////////////////////////
// Change viewing volume and viewport.  Called when window is resized
void ChangeSize(int w, int h)
    {
    GLfloat fAspect;

    // Prevent a divide by zero
    if(h == 0)
        h = 1;

    // Set Viewport to window dimensions
    glViewport(0, 0, w, h);

    fAspect = (GLfloat)w/(GLfloat)h;

    // Reset coordinate system
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Produce the perspective projection
    gluPerspective(35.0f, fAspect, 1.0, 40.0);
    // glOrtho(-5.0, 5.0, -5.0, 5.0, 5.0, -5.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    }
    

// This function does any needed initialization on the rendering
// context.  Here it sets up and initializes the lighting for
// the scene.
void SetupRC()
    {
    
    // Light values and coordinates
    GLfloat  whiteLight[] = { 0.05f, 0.05f, 0.05f, 1.0f };
    GLfloat  sourceLight[] = { 0.25f, 0.25f, 0.25f, 1.0f };
    GLfloat     lightPos[] = { -10.f, 5.0f, 5.0f, 1.0f };

    glEnable(GL_DEPTH_TEST);    // Hidden surface removal
    glFrontFace(GL_CCW);        // Counter clock-wise polygons face out
    // glEnable(GL_CULL_FACE);        // Do not calculate inside

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

    // Black blue background
    glClearColor(0.25f, 0.25f, 0.50f, 1.0f );
    }

// Respond to arrow keys
void SpecialKeys(int key, int x, int y)
    {
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
    }
}

void motion(int x, int y) {
    static bool wrap = false;

    if(!wrap) {
        int ww = glutGet(GLUT_WINDOW_WIDTH);
        int wh = glutGet(GLUT_WINDOW_HEIGHT);

        int dx = x - ww / 2;
        int dy = y - wh / 2;

        // cout << dx << " " << dy << endl;

        xRot += dy;
        yRot += dx;

        xRot = (GLfloat)((const int)xRot % 360);
        yRot = (GLfloat)((const int)yRot % 360);
        glutPostRedisplay();

        wrap = true;
        glutWarpPointer(ww / 2, wh / 2);
    } else {
        wrap = false;
    }
}

// Called to draw scene
void RenderScene(void)
    {
    GLUquadricObj *pObj;    // Quadric Object
    // Clear the window with current clearing color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Save the matrix state and do the rotations
    glPushMatrix();

        // Move object back and do in place rotation
        glTranslatef(0.0f, .3f, -10.0f);
        glRotatef(xRot, 1.0f, 0.0f, 0.0f);
        glRotatef(yRot, 0.0f, 1.0f, 0.0f);

        // Draw something
        pObj = gluNewQuadric();
        gluQuadricNormals(pObj, GLU_SMOOTH);
        
        
        glPushMatrix();
            glScalef(scale, scale, scale);
            
            glRotatef(-90, 1, 0, 0);

            // Desk
            glPushMatrix();
                // Desk top
                glColor3f(0, 1, 1); // blue

                glPushMatrix();
                    glTranslatef(0, 0, -0.5);
                    gluDisk(pObj, 0, 2, 4, 1);
                    glTranslatef(0, 0, -0.1);
                    gluDisk(pObj, 0, 2, 4, 1);
                    gluCylinder(pObj, 2, 2, 0.1, 4, 10);
                glPopMatrix();
                // Legs
                glPushMatrix();
                    glTranslatef(1.8, 0.0, -2.1);
                    gluCylinder(pObj, 0.2, 0.2, 1.5, 4, 1);
                glPopMatrix();

                glPushMatrix();
                    glTranslatef(-1.8, 0.0, -2.1);
                    gluCylinder(pObj, 0.2, 0.2, 1.5, 4, 1);
                glPopMatrix();

                glPushMatrix();
                    glTranslatef(0.0, 1.8, -2.1);
                    gluCylinder(pObj, 0.2, 0.2, 1.5, 4, 1);
                glPopMatrix();

                glPushMatrix();
                    glTranslatef(0.0, -1.8, -2.1);
                    gluCylinder(pObj, 0.2, 0.2, 1.5, 4, 1);
                glPopMatrix();

            glPopMatrix();

            // Lamp
            glPushMatrix();
                // bottom holder
                glPushMatrix();
                    glColor3f(0.35, 0.16, 0.14); // brown
                    glTranslatef(0, 0, -0.5);
                    gluCylinder(pObj, 0.25, 0.05, 0.1, 30, 1);
                glPopMatrix();

                // rod
                glPushMatrix();
                    glColor3f(0.647059, 0.164706, 0.164706); // reddish
                    glTranslatef(0, 0, -0.4);
                    gluCylinder(pObj, 0.05, 0.05, 0.9, 30, 1);
                glPopMatrix();

                // lamp
                glPushMatrix();
                    glColor3f(1, 1, 0); // yellow
                    gluCylinder(pObj, 0.5, 0.25, 0.6, 30, 1);
                glPopMatrix();
            glPopMatrix();

        

        glPopMatrix();
        
        
    // Restore the matrix state
    glPopMatrix();

    // Buffer swap
    glutSwapBuffers();
}

 

int main(int argc, char *argv[])
    {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Modeling with Quadrics");
    glutReshapeFunc(ChangeSize);
    glutSpecialFunc(SpecialKeys);
    glutDisplayFunc(RenderScene);
    glutKeyboardFunc(keyboard);
    // glutPassiveMotionFunc(motion);
    // glutSetCursor(GLUT_CURSOR_NONE);
    SetupRC();
    glutMainLoop();
    
    return 0;
    }
