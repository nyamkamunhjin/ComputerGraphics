#include <GL/glut.h>
#include "GL/freeglut.h"    
#include <iostream>
#include <math.h>
#include "objLoader.cpp"
#include <glm/vec3.hpp>
using namespace std;
#define ESC 27
#define ENTER 13


std::vector<glm::vec3> vertices;
bool res = objLoader("bunny.obj", vertices);

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


void displayMe(void) {
    glClear(GL_COLOR_BUFFER_BIT); 
    
    glBegin(GL_TRIANGLE_FAN);
        for(unsigned int i = 0; i < vertices.size(); i++) {
            glVertex3f(vertices[i].x, vertices[i].y, vertices[i].z);
        }
    glEnd();

    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Circle");
    glutKeyboardFunc(keyboard);
    glutReshapeFunc(ChangeSize);
    glutDisplayFunc(displayMe);
    glutMainLoop();
    return 0;
}
