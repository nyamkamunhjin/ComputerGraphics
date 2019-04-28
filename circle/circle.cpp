

#include <GL/glut.h>
#include "GL/freeglut.h"    
#include <iostream>
#include <math.h>

using namespace std;
#define ESC 27
#define ENTER 13

int sides = 3; // circle sides
int squareX = 1;
int squareY = 0.5;
GLfloat squareLen = 0.3;
GLfloat circleColor[3] = {0, 1, 0};

void drawSquare(GLfloat len, GLfloat x, GLfloat y) {
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(x, y);
        glVertex2f(x, y + len);
        glVertex2f(x + len, y + len);
        glVertex2f(x + len, y);
    glEnd();
}

void drawCircle(GLfloat radius, GLfloat x, GLfloat y, int sides) {
    glBegin(GL_TRIANGLE_FAN);
        for(int i = 0; i < sides; i++) {
            glVertex2f(x + (radius * cos(i * M_PI * 2 / sides)), y + (radius * sin(i * M_PI * 2 / sides)));
        }
    glEnd();
}

void ChangeSize(int w, int h) {
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

    gluOrtho2D(-2.0 * fAspect, 2.0 * fAspect, -2.0, 2.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glutPostRedisplay();
}

// Respond to arrow keys
void SpecialKeys(int key, int x, int y) {
    
    switch(key) {
        case GLUT_KEY_RIGHT:
            if(sides < 30) sides += 2;
            break;
        case GLUT_KEY_LEFT:
            if(sides > 3) sides -= 2;
            break;
        case GLUT_KEY_F1:
            circleColor[0] = 1;
            circleColor[1] = 0;
            circleColor[2] = 0;
            break;
        case GLUT_KEY_F2:
            circleColor[0] = 0;
            circleColor[1] = 1;
            circleColor[2] = 0;
            break;
        case GLUT_KEY_F3:
            circleColor[0] = 0;
            circleColor[1] = 0;
            circleColor[2] = 1;
            break;
    }

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

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) { 
        GLfloat posX = -2.0 + 4.0 * x / glutGet(GLUT_WINDOW_WIDTH);
        GLfloat posY = 2.0 - 4.0 * y / glutGet(GLUT_WINDOW_HEIGHT);
        printf("%f %f\n", posX, posY);
        
        // red 
        if(posX >= squareX && posX <= squareX + squareLen &&
            posY >= squareY && posY <= squareY + squareLen) {
                circleColor[0] = 1;
                circleColor[1] = 0;
                circleColor[2] = 0;
            }
        
        // green 
        if(posX >= squareX && posX <= squareX + squareLen &&
            posY >= squareY + 0.5 && posY <= squareY + 0.5 + squareLen) {
                circleColor[0] = 0;
                circleColor[1] = 1;
                circleColor[2] = 0;
            }

        // blue 
        if(posX >= squareX && posX <= squareX + squareLen &&
            posY >= squareY + 1 && posY <= squareY + 1 + squareLen) {
                circleColor[0] = 0;
                circleColor[1] = 0;
                circleColor[2] = 1;
            }

        glutPostRedisplay();
    }
}

void displayMe(void) {
    glClear(GL_COLOR_BUFFER_BIT);

    // circle
    glPushMatrix();
        glColor3fv(circleColor);
        drawCircle(1, -1, 0, sides);
    glPopMatrix();
    
    // red button
    glPushMatrix();
        glColor3f(1, 0, 0);
        drawSquare(squareLen, squareX, squareY);
    glPopMatrix();

    // green button
    glPushMatrix();
        glColor3f(0, 1, 0);
        drawSquare(squareLen, squareX, squareY + 0.5);
    glPopMatrix();

    // blue button
    glPushMatrix();
        glColor3f(0, 0, 1);
        drawSquare(squareLen, squareX, squareY + 1);
    glPopMatrix();

    
    
    
    
    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Circle");
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutSpecialFunc(SpecialKeys);
    glutReshapeFunc(ChangeSize);
    glutDisplayFunc(displayMe);
    glutMainLoop();
    return 0;
}
