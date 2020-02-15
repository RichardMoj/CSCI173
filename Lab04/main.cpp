// (1-t)^2P0 + 2(1-t)P1 + t^2P2


#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <cmath>
#include <string.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <iostream>
#include <math.h>

#define PI 3.14159

using namespace std;

bool WireFrame= false;
float i =0;
const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };

    float xpos =0;
    float ypos =0;
    float Wwidth,Wheight;

float mx;
float my;
float xp[3] = {0.0,0.0,0.0};
float yp[3] = {0.0,0.0,0.0};
float zp[3] = {0.0,0.0,0.0};
int counter = 0;
float  t = 0;                      // For bezier curve
float x = 0;
float y = 0;
float z = 0;

/* GLUT callback Handlers */

static void resize(int width, int height)
{
     double Ratio;

     Wwidth = (float)width;
     Wheight = (float)height;

     Ratio= (double)width /(double)height;

    glViewport(0,0,(GLsizei) width,(GLsizei) height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective (45.0f,Ratio,0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

 }

static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(0,0,10,0.0,0.0,0.0,0.0,1.0,100.0);

    if(WireFrame)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);      //Draw Our Mesh In Wireframe Mesh
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);      //Toggle WIRE FRAME

    // your code here
    // Add z to the equation for 3D, random from -0.5 to 5
    // Add rotation to see the depth


    // Draw the line
    glPushMatrix();
    glPointSize(1);
        for(float j = 0; j <= 1; j+=0.01)
        {
            glBegin(GL_POINTS);
            xpos = (1-j)*(1-j)*xp[0] + 2*(1-j)*j*xp[1] + j*j*xp[2];
            ypos = (1-j)*(1-j)*yp[0] + 2*(1-j)*j*yp[1] + j*j*yp[2];
            glVertex3f(xpos, ypos, -0.5);
            glEnd();
        }
    glPopMatrix();

    // Draw the sphere along the line
        glPushMatrix();
            glTranslatef(x, y, -0.5);
            glutSolidSphere(0.1, 40, 40);
        glPopMatrix();

    // Draw the points
    glPushMatrix();
    glPointSize(10);
        for (int i = 0; i < 3; i++)
        {
            glColor3f(1.0f, 0.0f, 0.0f);
            glBegin(GL_POINTS);
            glVertex3f(xp[i], yp[i], -0.5);
            glEnd();
        }
    glPopMatrix();

    glutSwapBuffers();
}


static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27 :
        case 'q':
            exit(0);
            break;

      case 'w':
        WireFrame =!WireFrame;
           break;
    }
}

void Specialkeys(int key, int x, int y)
{
    switch(key)
    {
    case GLUT_KEY_UP:
    break;
   }
  glutPostRedisplay();
}


static void idle(void)
{
    // Use parametric equation with t increment for xpos and y pos
    // Don't need a loop

    // Reset t when it reaches 1 because j goes to 1
    if(t < 1){
        t += 0.01;
        x = (1-t)*(1-t)*xp[0] + 2*(1-t)*t*xp[1] + t*t*xp[2];
        y = (1-t)*(1-t)*yp[0] + 2*(1-t)*t*yp[1] + t*t*yp[2];
        }
    else{
        t = 0;
    }

    glutPostRedisplay();
}



void mouse(int btn, int state, int x, int y){

    float scale = 55*(Wwidth/Wheight);
    mx = (float) (x-Wwidth/2)/scale;
    my = (float) (Wheight/2-y)/scale;

    switch(btn){
        case GLUT_LEFT_BUTTON:

        if(state==GLUT_DOWN){

            xp[counter] = mx;
            yp[counter] = my;
            counter++;
            counter = counter % 3;                      //Reset counter after 3
            t = 0;
            }
            break;
    }
     glutPostRedisplay();
};



static void init(void)
{
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glClearColor(0.5f, 0.5f, 1.0f, 0.0f);                 // assign a color you like

    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);

    glEnable(GL_DEPTH_TEST);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glShadeModel(GL_SMOOTH);

    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);
}

/* Program entry point */

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);

    glutInitWindowSize(800,600);
    glutInitWindowPosition(0,0);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("Project Assignment 2");
    init();
    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutKeyboardFunc(key);
    glutSpecialFunc(Specialkeys);

    glutIdleFunc(idle);

    glutMainLoop();

    return EXIT_SUCCESS;
}
