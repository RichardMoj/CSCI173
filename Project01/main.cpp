#include <GL/gl.h>
#include <string.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <SOIL.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <ctime>

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

    float Wwidth,Wheight;

float translate_z = 10;
float rotateX = 0;
float rotateY = 0;
float rotateZ = 0;
GLuint tex;

// Controls how far the ball will move each frame
const float slices = 100.0;

/*      BALL VARIABLES          */
float curr_x = 0; float curr_y = 0; float curr_z = 0;
float old_x = 0; float old_y = 0; float old_z = 0;    
float mx = 0; float my = 0; float mz = 0;       // Where ball should move to
float sphere_radius = 0.5;
float t = 0;
float delta_x = 0;
float delta_y = 0;
float delta_z = 0;


/*      CUBE BOUNDS             */
int MAX_X = 18; int MIN_X = -18;
int MAX_Y = 18; int MIN_Y = -18;
int MAX_Z = 18; int MIN_Z = -18;




//Function prototypes
void TLoad(char *, GLuint);
float generate_z_val();


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

    gluLookAt(0,0,translate_z,0.0,0.0,0.0,0.0,1.0,100.0);

    //Move the cube around
    glPushMatrix();
    glDisable(GL_LIGHTING);
    glTranslatef(0,0, 0);
    glRotatef(rotateX, 1.0f, 0.0f, 0.0f);
    glRotatef(rotateY, 0.0f, 1.0f, 0.0f);
    glRotatef(rotateZ, 0.0f, 0.0f, 1.0f);
    glScalef(20.0f, 20.0f, 20.0f);

    // Draw the front face of a cube
    // Order of tex cords: TL -> TR -> BR -> BL
    glBindTexture(GL_TEXTURE_2D, tex);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, -1.0f);
    glTexCoord2f(0.25f, 0.33f); glVertex3f(-1.0f, 1.0f, 1.0f);
    glTexCoord2f(0.50f, 0.33f); glVertex3f(1.0f, 1.0f, 1.0f);
    glTexCoord2f(0.50f, 0.66f); glVertex3f(1.0f, -1.0f, 1.0f);
    glTexCoord2f(0.25f, 0.66f); glVertex3f(-1.0f, -1.0f, 1.0f);
    glEnd();

    //Draw the backface
    // Order: TR -> BR -> BL -> TL
    glBindTexture(GL_TEXTURE_2D, tex);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(1.0f, 0.33f); glVertex3f(-1.0f, 1.0f, -1.0f);
    glTexCoord2f(1.0f, 0.66f); glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f(0.75f, 0.66f); glVertex3f(1.0f, -1.0f, -1.0f);
    glTexCoord2f(0.75f, 0.33f); glVertex3f(1.0f, 1.0f, -1.0f);
    glEnd();




/****************************************************************************/

    //Draw the top face
    // Order: BL -> TL -> TR -> BR
    glBindTexture(GL_TEXTURE_2D, tex);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, -1.0f, 0.0f);
    glTexCoord2f(0.25f, 0.33f); glVertex3f(1.0f, 1.0f, -1.0f);
    glTexCoord2f(0.25f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f);
    glTexCoord2f(0.50f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
    glTexCoord2f(0.50f, 0.33f); glVertex3f(-1.0f, 1.0f, -1.0f);
    glEnd();

    /*  NOT CORRECT? */
    //Draw the bottom face Z
    // Order: TR -> TL -> BL -> BR
    glBindTexture(GL_TEXTURE_2D, tex);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(0.50f, 0.66f); glVertex3f(-1.0f, -1.0f, 1.0f);
    glTexCoord2f(0.25f, 0.66f); glVertex3f(1.0f, -1.0f, 1.0f);
    glTexCoord2f(0.25f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f);
    glTexCoord2f(0.50f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
    glEnd();


/****************************************************************************/


    //Right Face Y
    // Order: TR -> BR -> BL -> TL
    glBindTexture(GL_TEXTURE_2D, tex);
    glBegin(GL_QUADS);
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.75f, 0.33f); glVertex3f(1.0f, 1.0f, -1.0f);
    glTexCoord2f(0.75f, 0.66f); glVertex3f(1.0f, -1.0f, -1.0f);
    glTexCoord2f(0.50f, 0.66f); glVertex3f(1.0f, -1.0f, 1.0f);
    glTexCoord2f(0.50f, 0.33f); glVertex3f(1.0f, 1.0f, 1.0f);
    glEnd();

    //Left Face Y
    glBindTexture(GL_TEXTURE_2D, tex);
    glBegin(GL_QUADS);
    glNormal3f(1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.33f); glVertex3f(-1.0f, 1.0f, -1.0f);
    glTexCoord2f(0.25f, 0.33f); glVertex3f(-1.0f, 1.0f, 1.0f);
    glTexCoord2f(0.25f, 0.66f); glVertex3f(-1.0f, -1.0f, 1.0f);
    glTexCoord2f(0.0f, 0.66f); glVertex3f(-1.0f, -1.0f, -1.0f);
    glEnd();
    glPopMatrix();


    // Draw the sphere
    glPushMatrix();
        glEnable(GL_LIGHTING);
        glTranslatef(curr_x, curr_y, curr_z);
        glutSolidSphere(sphere_radius, 40, 40);
    glPopMatrix();


    if(WireFrame)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);      //Draw Our Mesh In Wireframe Mesh
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);      //Toggle WIRE FRAME

    // your code here
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
    case GLUT_KEY_END:
        translate_z += (float) 1.0f;
        break;
    case GLUT_KEY_HOME:
        translate_z -= (float) 1.0f;
        break;
    case GLUT_KEY_UP:
        rotateX = ( (int)rotateX + 5) % 360;
        break;
    case GLUT_KEY_DOWN:
        rotateX = ( (int)rotateX - 5) % 360;
        break;
    case GLUT_KEY_LEFT:
        rotateY = ( (int)rotateY + 5) % 360;
        break;
    case GLUT_KEY_RIGHT:
        rotateY = ( (int)rotateY - 5) % 360;
        break;
    default:
        break;
   }
  glutPostRedisplay();
}


static void idle(void)
{
    // Use parametric equation with t increment for xpos and y pos
    // Don't need a loop

    // Speed? increase every frame
    t   += 1;

    // calculate the length of each slice - the distance the ball will 
    // travel each frame
    delta_x = (mx - old_x)/slices;
    delta_y = (my - old_y)/slices;
    delta_z = (mz - old_z)/slices;


    // Update balls position
    curr_x = old_x + delta_x * t;
    curr_y = old_y + delta_y * t;
    curr_z = old_z + delta_z * t;
    

    if(curr_y >= MAX_Y){
        curr_y = MAX_Y;
        t = 0;
        //tx = ty = tz = 0;

        my = old_y;
        mx = 2 * (curr_x - old_x);
        mz = 2 * (curr_z - old_z);

        old_x = curr_x;
        old_y = curr_y;
        old_z = curr_z;
    }else if(curr_y <= MIN_Y){
        curr_y = MIN_Y;
        t = 0;
        //tx = ty = tz = 0;

        my = old_y;
        mx = 2 * (curr_x - old_x);
        mz = 2 * (curr_z - old_z);

        old_x = curr_x;
        old_y = curr_y;
        old_z = curr_z;
    }else if(curr_x >= MAX_X){
        curr_x = MAX_X;
        t = 0;
        //tx = ty = tz = 0;

        my = 2 * (curr_y - old_y);
        mx = old_x;
        mz = 2 * (curr_z - old_z);

        old_x = curr_x;
        old_y = curr_y;
        old_z = curr_z;
    }else if(curr_x <= MIN_X){
        curr_x = MIN_X;
        t = 0;
        //tx = ty = tz = 0;

        my = 2 * (curr_y - old_y);
        mx = old_x;
        mz = 2 * (curr_z - old_z);

        old_x = curr_x;
        old_y = curr_y;
        old_z = curr_z;
    }
    else if(curr_z >= MAX_Z){
        curr_z = MAX_Z;
        t = 0;
        //tx = ty = tz = 0;

        mx = 2 * (curr_x - old_x);
        my = 2 * (curr_y - old_y);
        mz = old_z;

        old_x = curr_x;
        old_y = curr_y;
        old_z = curr_z;

    }
    else if(curr_z <= MIN_Z){
        curr_z = MIN_Z;
        t = 0;
        //tx = ty = tz = 0;

        mx = 2 * (curr_x - old_x);
        my = 2 * (curr_y - old_y);
        mz = old_z;

        old_x = curr_x;
        old_y = curr_y;
        old_z = curr_z;

    }
    else{}
    

    glutPostRedisplay();
}



void mouse(int btn, int state, int x, int y){

    float scale = 55*(Wwidth/Wheight);
    
    // Grab mouse coords to calculate new direction
    mx = (float) (x-Wwidth/2)/scale;
    my = (float) (Wheight/2 - y)/scale;
    mz = generate_z_val();

    //cout<<mx<<" | "<<my<<" | "<<mz<<endl;

    switch(btn){
        case GLUT_LEFT_BUTTON:

        if(state==GLUT_DOWN){

                t = 0;
                delta_x = 0;
                delta_y = 0;
                delta_z = 0;

                // save the ball position before update of new direction
                old_x = curr_x; 
                old_y = curr_y;
                old_z = curr_z;
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

    //Enable lightning for objects in display for that specific object
    glEnable(GL_LIGHTING);

    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &tex);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    TLoad("/home/richard/Documents/2020SP/CSCI173/Project01/images/skybox2.png", tex);


}

void TLoad(char *file, GLuint tex){

     glBindTexture(GL_TEXTURE_2D, tex); // images are 2D arrays of pixels, bound to the GL_TEXTURE_2D target.

     int width, height; // width & height for the image reader
     unsigned char* image;

     //change file?
     image = SOIL_load_image(file, &width, &height, 0, SOIL_LOAD_RGB);
     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

    // binding image data
     SOIL_free_image_data(image);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
}


float generate_z_val(){
    float temp = 0;
    int sign = rand();

    temp = (float) (rand() % 301)/100.0;      //value between 0-3;

    //random chance the z value will be positive or negative each click
    if(sign % 2 != 0){
        temp = -1 * temp;
    }
    return temp;
}





/* Program entry point */

int main(int argc, char *argv[])
{
    // To make each mouse click different every time the program
    // is run
    srand(time(0));

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
