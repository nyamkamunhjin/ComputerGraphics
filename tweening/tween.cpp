#include <GL/glut.h>
#include "GL/freeglut.h"    
#include <iostream>
#include <math.h>
#include <vector>
using namespace std;
#define ESC 27
#define ENTER 13
#define SPACE 32

class Points {
public:
    GLfloat x;
    GLfloat y;
    
    Points() {}

    Points(GLfloat x, GLfloat y) {
        this->x = x;
        this->y = y;
    }
    
    Points dist(Points points) {
        return Points(this->x - points.x, this->y - points.y);
    }
    void sub(Points points, float rate=0.05) {
        this->x -= points.x * rate;
        this->y -= points.y * rate;
    } 
};

vector<Points> startPoints;
vector<Points> endPoints;
int sides = 3; // circle sides
int squareX = 1;
int squareY = 0.5;
bool tween = false;

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
        case SPACE:
            tween = !tween;
            break;
    }
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) { 
        GLfloat posX = -2.0 + 4.0 * x / glutGet(GLUT_WINDOW_WIDTH);
        GLfloat posY = 2.0 - 4.0 * y / glutGet(GLUT_WINDOW_HEIGHT);

        startPoints.push_back(Points(posX, posY));
        
        printf("added point on %f %f\n", posX, posY);
        
        
    }

    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) { 
        if(endPoints.size() < startPoints.size()) {
            GLfloat posX = -2.0 + 4.0 * x / glutGet(GLUT_WINDOW_WIDTH);
            GLfloat posY = 2.0 - 4.0 * y / glutGet(GLUT_WINDOW_HEIGHT);
            
            endPoints.push_back(Points(posX, posY));

            int count = (int)(startPoints.size() - endPoints.size());
            printf("added shift point on %f %f %d more\n",
                posX, posY, count);
        }
    }

    glutPostRedisplay();
}

void displayMe(void) {
    glClear(GL_COLOR_BUFFER_BIT);

    if(!startPoints.empty()) {
        glBegin(GL_LINE_LOOP);    
            for(int i = 0; i < startPoints.size(); i++) {
                glVertex2f(startPoints[i].x, startPoints[i].y);
            }
        glEnd();
    }

    if(!endPoints.empty()) {
        glBegin(GL_POINTS);    
            for(int i = 0; i < endPoints.size(); i++) {
                glVertex2f(endPoints[i].x, endPoints[i].y);
            }
        glEnd();
    }

    if(tween == true) {
        if(!startPoints.empty() && !endPoints.empty()) {
            for(int i = 0; i < startPoints.size(); i++) {
                startPoints[i].sub(startPoints[i].dist(endPoints[i]));
            }
        }
    }

    // Buffer swap
    glutSwapBuffers();
}

void Timer(int value) {
   glutPostRedisplay();      // Post re-paint request to activate display()
   glutTimerFunc(30, Timer, 0); // next Timer call milliseconds later
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Tweening");
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutTimerFunc(0, Timer, 0);
    glutSpecialFunc(SpecialKeys);
    glutReshapeFunc(ChangeSize);
    glutDisplayFunc(displayMe);
    glutMainLoop();
    return 0;
}
