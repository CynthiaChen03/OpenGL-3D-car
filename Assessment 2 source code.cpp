/*All codes inspired from GitHub or CSDN mean I just drew inspiration from their object shapes but not copyed their code.*/

#define FREEGLUT_STATIC
#include <GL/freeglut.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "vector"
#include <stdlib.h>
#include "vector"
#include <cmath>
#include <time.h>
using namespace std;

// Window size
int intWinWidth = 800; // Initial window width
int intWinHeight = 600;// Initial window height

int menu = 0, submenu1 = 0, submenu2; // Menu variables

// Initial colors for the car body
float carbodyR = 1, carbodyG = 0, carbodyB = 0;

// Animation parameters
float fltAnimationOffset = 0.0;
float fltLampOffset = 0.0;
float fltCarOffset0 = 0.0;
float fltCarOffset1 = 0.0;
float fltmountainOffset = 0.0;
float fltboardOffset = 0.0;
float fltAcceleration = 2.0; //Speed of animation
float TreeOffsetX = 0.0;
GLfloat Wheelangle = 0.0;
float fltCarAngle = 0.0;
float MoonAngle = 0.0;
float fltViewingAngle = 0; //Used for rotating camera for lengthening of vision

/*Translation, scaling, rotation parameters*/
float fltFOV = 70; //Field Of View
float fltZoom = 1.0; //Zoom amount

// Camera parameters
float fltX0 = -200.0;
float fltY0 = 250.0;
float fltZ0 = 500.0;

GLfloat  yangle = 0; //Rotating angle

/*Lighting and color parameters*/
int flag1 = 1;// to switch car driving mode
int flag2 = 0;// to switch day or night
GLint imagewidth0, imagewidth1, imagewidth2, imagewidth3;
GLint imageheight0, imageheight1, imageheight2, imageheight3;
GLint pixellength0, pixellength1, pixellength2, pixellength3;
vector<GLubyte*> p;
GLuint texture[4]; // Increase the array size to accommodate the third texture

/*Bezier Curve point for draw the lamp*/
float P0[] = { 0, 0 };
float P1[] = { 12, 43 };
float P2[] = { 37, 73 };
float P3[] = { 73, 84 };
/*Function to draw a Bezier curve*/
void drawBezierCurve() {
    glBegin(GL_LINE_STRIP);

    for (float t = 0.0; t <= 1.0; t += 0.01) {
        float x = pow(1 - t, 3) * P0[0] + 3 * pow(1 - t, 2) * t * P1[0] + 3 * (1 - t) * pow(t, 2) * P2[0] + pow(t, 3) * P3[0];
        float y = pow(1 - t, 3) * P0[1] + 3 * pow(1 - t, 2) * t * P1[1] + 3 * (1 - t) * pow(t, 2) * P2[1] + pow(t, 3) * P3[1];
        glVertex2f(x, y);
    }

    glEnd();
}

/*Read Images and Set Texture*/
void ReadImage(const char path[256], GLint& imagewidth, GLint& imageheight, GLint& pixellength) {
    GLubyte* pixeldata;
    FILE* pfile;
    fopen_s(&pfile, path, "rb");
    if (pfile == 0) exit(0);
    // Extract image information from the file
    fseek(pfile, 0x0012, SEEK_SET);
    fread(&imagewidth, sizeof(imagewidth), 1, pfile);
    fread(&imageheight, sizeof(imageheight), 1, pfile);
    pixellength = imagewidth * 3;
    // Adjust pixel length to be a multiple of 4
    while (pixellength % 4 != 0)pixellength++;
    pixellength *= imageheight;
    // Read pixel data and store it in the vector
    pixeldata = (GLubyte*)malloc(pixellength);
    if (pixeldata == 0) exit(0);
    fseek(pfile, 54, SEEK_SET);
    fread(pixeldata, pixellength, 1, pfile);
    p.push_back(pixeldata);
    fclose(pfile);
}
/*Function to set up textures*/
void setTexture(void) {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glShadeModel(GL_FLAT);
    glEnable(GL_TEXTURE_2D);

    // Assuming ReadImage function is defined and works correctly
    ReadImage("road.bmp", imagewidth0, imageheight0, pixellength0);
    ReadImage("straight.bmp", imagewidth1, imageheight1, pixellength1);
    ReadImage("moon.bmp", imagewidth2, imageheight2, pixellength2);
    ReadImage("sun2.bmp", imagewidth3, imageheight3, pixellength3);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // set pixel storage modes (in memory)
    glGenTextures(4, &texture[0]); // Generate three texture names

    // Set up texture parameters for each texture
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, imagewidth0, imageheight0, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, p[0]);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, imagewidth1, imageheight1, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, p[1]);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


    glBindTexture(GL_TEXTURE_2D, texture[2]); // Bind the third texture
    glTexImage2D(GL_TEXTURE_2D, 0, 3, imagewidth2, imageheight2, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, p[2]);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glBindTexture(GL_TEXTURE_2D, texture[3]); // Bind the forth texture
    glTexImage2D(GL_TEXTURE_2D, 0, 3, imagewidth3, imageheight3, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, p[3]);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}

/*Set texture of road*/
void setRoad() {
    glColor3f(1.0, 1.0, 1.0);
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    // Draw road with repeated texture along the X-axis
    for (int i = 0; i <= 10000; i += 180) {
        glBegin(GL_QUADS);
        glTexCoord2f(0.0, 0.0); glVertex3f(-90.0 - 5000 + i, 1.0, -90);
        glTexCoord2f(1.0, 0.0); glVertex3f(-90.0 - 5000 + i, 1, 90);
        glTexCoord2f(1.0, 1.0); glVertex3f(90.0 - 5000 + i, 1.0, 90);
        glTexCoord2f(0.0, 1.0); glVertex3f(90.0 - 5000 + i, 1.0, -90);
        glEnd();
    }
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}
/*Set Texture of Sun and Moon*/
/*The moon was inspired from CSDN. https://blog.csdn.net/qq_44704426/article/details/106139763 */
void setSunAndMoon() {
    glColor3f(1, 1, 1);
    glPushMatrix();

    glEnable(GL_TEXTURE_2D);
    glTranslatef(250.0, 350.0, 0.0);
    glRotatef(MoonAngle, 0.0, 1.0, 0.0);
    if (flag2 == 1) //night mode
    {
        glBindTexture(GL_TEXTURE_2D, texture[2]);
        GLUquadric* quadricObj = gluNewQuadric(); // Create a quadric surface object
        gluQuadricTexture(quadricObj, GL_TRUE); // Set texture mode to true

        GLfloat moon_mat_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };  //Define the ambient light color of the moon as 0
        GLfloat moon_mat_diffuse[] = { 0.0f, 0.0f, 0.0f, 1.0f };  //Define the diffuse light color of the moon as 0
        GLfloat moon_mat_specular[] = { 0.0f, 0.0f, 0.0f, 1.0f };   //Define the specular light color of the moon as 0
        GLfloat moon_mat_emission[] = { 1.0f, 1.0f, .7f, 1.0f };   //Define the ambient light color of the moon as yellow
        GLfloat moon_mat_shininess = 0.0f;//Define the shiniess of the moon as 0
        glMaterialfv(GL_FRONT, GL_AMBIENT, moon_mat_ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, moon_mat_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, moon_mat_specular);
        glMaterialfv(GL_FRONT, GL_EMISSION, moon_mat_emission);
        glMaterialf(GL_FRONT, GL_SHININESS, moon_mat_shininess);
        gluSphere(quadricObj, 30, 40, 40);
        gluDeleteQuadric(quadricObj);
        glDisable(GL_TEXTURE_2D);
        //Enable night light
        glEnable(GL_LIGHT1);
        glDisable(GL_LIGHT0);
    }
    else //day mode
    {
        glBindTexture(GL_TEXTURE_2D, texture[3]);
        GLUquadric* quadricObj = gluNewQuadric(); // Create a quadric surface object
        gluQuadricTexture(quadricObj, GL_TRUE); // Set texture mode to true
        // Material properties for the sun
        GLfloat sun_mat_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };  //Define the ambient light color of the sun as 0
        GLfloat sun_mat_diffuse[] = { 0.0f, 0.0f, 0.0f, 1.0f };   //Define the diffuse light color of the sun as 0
        GLfloat sun_mat_specular[] = { 0.0f, 0.0f, 0.0f, 1.0f };   //Define the specular light color of the sun as 0
        GLfloat sun_mat_emission[] = { 0.8f, 0.0f, 0.0f, 1.0f };  //Define the ambient light color of the sun as red
        GLfloat sun_mat_shininess = 0.0f;//Define the shiniess of the sun as 0
        glMaterialfv(GL_FRONT, GL_AMBIENT, sun_mat_ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, sun_mat_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, sun_mat_specular);
        glMaterialfv(GL_FRONT, GL_EMISSION, sun_mat_emission);
        glMaterialf(GL_FRONT, GL_SHININESS, sun_mat_shininess);
        gluSphere(quadricObj, 30, 40, 40);
        gluDeleteQuadric(quadricObj); // object must be deleted or it will be created every call of the 
        glDisable(GL_TEXTURE_2D);
        // Enable day light
        glEnable(GL_LIGHT0);
        glDisable(GL_LIGHT1);
    }
    glPopMatrix();
}
/*Function to set up a textured straight road segment*/
void setStraight() {
    glColor3f(1, 1, 1);
    glPushMatrix();

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[1]);

    GLUquadric* quadricObj = gluNewQuadric(); // Create a quadric object
    gluQuadricTexture(quadricObj, GL_TRUE); // Enable texture mapping
    gluDisk(quadricObj, 0.0, 30.0, 40, 1); // Draw a texture map to the disk
    gluDeleteQuadric(quadricObj); // Delete a quadric object

    glDisable(GL_TEXTURE_2D);

    glPopMatrix();
}

/*Animations in display*/
void animateCar()
{
    if (fltAcceleration < 0)
    {
        fltAcceleration = 0;
    }
    //Increments/decrements animation variables
    Wheelangle += fltAcceleration;

    if (flag1 == 0){
        // Update offsets for various objects in the scene
        fltLampOffset -= fltAcceleration;
        TreeOffsetX -= fltAcceleration;
        fltboardOffset -= fltAcceleration;
        fltmountainOffset -= fltAcceleration * 0.1;
        // Ensure that offsets stay within specified ranges
        if (fltLampOffset < -5000.0)
            fltLampOffset = 0.0;
        if (fltLampOffset > 0.0)
            fltLampOffset = -5000.0;

        if (TreeOffsetX < -5000.0)
            TreeOffsetX = 500.0;
        if (TreeOffsetX > 500.0)
            TreeOffsetX = -5000.0;

        if (fltmountainOffset < -1000.0)
            fltmountainOffset = 500.0;
        if (fltmountainOffset > 500.0)
            fltmountainOffset = -1000.0;

        if (fltboardOffset < -1500.0)
            fltboardOffset = 500.0;
        if (fltboardOffset > 500.0)
            fltboardOffset = -1500.0;
    }

    if (flag1 == 1)
    {
        // Update car offset for animation in flag1 mode
        fltCarOffset1 += fltAcceleration;
        if (fltCarOffset1 < -5000.0)
            fltCarOffset1 = 0.0;
        else if (fltCarOffset1 > 5000.0)
            fltCarOffset1 = 0;
    }
    // Request a redraw to display the updated scene
    glutPostRedisplay();

}

/*Define light and material properties*/
void myinit() {

    // Night Light properties
    GLfloat light_position1[] = { 400.0, 300.0, 0.0 , 0.0 };
    GLfloat light_ambient1[] = { .3, .3, .3, 1.0 };
    GLfloat light_diffuse1[] = { 0.4, 0.4, .4, 1.0 };
    GLfloat light_specular1[] = { .0, .0, .0, 1.0 };

    glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient1);
    glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse1);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular1);
    glEnable(GL_LIGHTING);


    // Day Light properties
    GLfloat light_position0[] = { 400.0, 300.0, 0.0 , 0.0 };
    GLfloat light_ambient0[] = { .4, .4, .4, 1.0 };
    GLfloat light_diffuse0[] = { .8, .8, .8, 1.0 };
    GLfloat light_specular0[] = { 1.0, 1.0, 1.0, 1.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient0);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse0);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular0);

    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    // Specify which material parameters track the current color
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    glShadeModel(GL_SMOOTH);
}

/*Objects in scene*/

/*Car parts*/
/*The car was inspired from GitHub. https://github.com/Rajkumarpaneru18/3D_Car_Animation_CG/tree/main */
void FrontAndBackBody() {
  
    glPointSize(200.0);
    glPushMatrix();
    glTranslated(-100, -40, -40);

    glBegin(GL_QUADS);
    /* top of cube*/
    glNormal3f(0, 1, 0); // Normal vector for the front face
    glColor3f(carbodyR, carbodyG, carbodyB);
    glVertex3f(20, 40, 60);
    glVertex3f(60, 60, 60);
    glVertex3f(60, 60, 20);
    glVertex3f(20, 40, 20);

    /* bottom of cube*/
    glNormal3f(0, -1, 0);
    glVertex3f(20, 40, 60);
    glVertex3f(60, 20, 60);
    glVertex3f(60, 20, 20);
    glVertex3f(20, 20, 20);

    /* front of cube*/
    glNormal3f(-1, 0, 0);
    glVertex3f(20, 20, 60);
    glVertex3f(20, 40, 60);
    glVertex3f(20, 40, 20);
    glVertex3f(20, 20, 20);

    /* back of cube.*/
    glNormal3f(1, 0, 0);
    glVertex3f(60, 20, 60);
    glVertex3f(60, 60, 60);
    glVertex3f(60, 60, 20);
    glVertex3f(60, 20, 20);

    /* left of cube*/
    glNormal3f(0, 0, 1);
    glVertex3f(20, 20, 60);
    glVertex3f(60, 20, 60);
    glVertex3f(60, 60, 60);
    glVertex3f(20, 40, 60);

    /* Right of cube */
    glNormal3f(0, 0, 1);
    glVertex3f(20, 20, 20);
    glVertex3f(60, 20, 20);
    glVertex3f(60, 60, 20);
    glVertex3f(20, 40, 20);

    glEnd();
    glPopMatrix();
}
void Wheel() {
    glPushMatrix();
    glColor3f(0.7, 0.7, 0.7);
    glLineWidth(1);
    glBegin(GL_LINE_STRIP);
    for (float theta = 0; theta < 360; theta = theta + 40)
    {
        glVertex3f(0, 0, 0);
        glVertex3f((12.0 * (cos(((theta + Wheelangle) * 3.14) / 180.0))), (12.0 * (sin(((theta + Wheelangle) * 3.14) / 180.0))), 0.0);
    }
    glEnd();
    glColor3f(0, 0, 0);
    glRotatef(fltCarAngle, 0, 1, 0);
    glutSolidTorus(3, 12, 20, 20);
    glPopMatrix();
}
void FrontAndBackWindows() {
    glPushMatrix();
    glBegin(GL_QUADS);
    glColor3f(0.3, 0.3, 0.3);

    // back side
    glNormal3f(-1, 0, 0);
    glVertex3f(30, 20, -20);
    glVertex3f(30, 20, 20);
    glVertex3f(30, 50, 20);
    glVertex3f(30, 50, -20);

    //front side
    glNormal3f(1.0f, 0.0f, 0.0f);
    glColor3f(0.3, 0.3, 0.3);
    glVertex3f(40, 20, -20);
    glVertex3f(40, 20, 20);
    glVertex3f(30, 50, 20);
    glVertex3f(30, 50, -20);

    //right side 
    glNormal3f(0, 0, 1);
    glVertex3f(30, 20, -20);
    glVertex3f(40, 20, -20);
    glVertex3f(30, 50, -20);

    //left side
    glNormal3f(0, 0, 1);
    glVertex3f(30, 20, 20);
    glVertex3f(40, 20, 20);
    glVertex3f(30, 50, 20);

    //bottom side
    glVertex3f(30, 20, 20);
    glVertex3f(40, 20, 20);
    glVertex3f(40, 20, -20);
    glVertex3f(30, 20, -20);

    glEnd();
    glPopMatrix();
}
void SideWindow() {
    glPushMatrix();
    glNormal3f(1, 0, 0);
    glColor3f(0.3, 0.3, 0.3);
    glTranslatef(10, 7.5, 0);
    glScalef(20, 15, 1);
    glutSolidCube(1);
    glPopMatrix();
}
/*Functions to combine car parts*/

//Function to draw car body
void DrawCarBody() {
    // Code for drawing the car body
    // Bottom part of the body
    glPushMatrix();
    glColor3f(carbodyR, carbodyG, carbodyB);
    glLineWidth(1);
    glScaled(2, 1, 1);
    glutSolidCube(40);
    glPopMatrix();

    //Top part of the body
    glPushMatrix();
    glLineWidth(1);
    glScaled(2, 1, 1.33);
    glTranslated(0, 35, 0);
    glutSolidCube(30);
    glPopMatrix();

    //Middle Body
    FrontAndBackBody();
    glPushMatrix();
    glRotatef(-180, 0, 1, 0);
    glTranslatef(0, 0, 0);
    FrontAndBackBody();
    glPopMatrix();

}
// Function to draw windows
void DrawWindows() {
    //Front and Back Windows 
    glPushMatrix(); //save present MODELVIEW matrix
    FrontAndBackWindows();
    glPushMatrix();
    glRotatef(-180, 0, 1, 0); //incremental change for FrontAndBackWindows 
    FrontAndBackWindows();
    glPopMatrix();
    glPopMatrix(); //recent state recovery

    //Side Windows 
    glPushMatrix();//save present MODELVIEW matrix
    glTranslatef(5, 27.5, -21); //update MODELVIEW matrix for sidewindows
    SideWindow();
    glTranslatef(-30, 0, 0);
    SideWindow();
    glTranslatef(0, 0, 42);
    SideWindow();
    glTranslatef(30, 0, 0);
    SideWindow();
    glPopMatrix(); //recent state recovery
}
// Function to draw wheels
void DrawWheels() {
    // Code for drawing wheels
    glPushMatrix();
    glTranslatef(0, 2, 0);
    glPushMatrix();
    glTranslatef(-40, -20, -20);
    Wheel();
    glTranslatef(0, 0, 40);
    Wheel();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(40, -20, -20);
    Wheel();
    glTranslatef(0, 0, 40);
    Wheel();
    glPopMatrix();
    glPopMatrix();
}
// Function to draw the entire car
void DrawCar() {
    // Material properties
    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat no_shininess[] = { 0.0 };
    GLfloat low_shininess[] = { 5.0 };

    glPushMatrix();
    glScaled(1.1, 0.9, 1); // More coordinated proportion of car  glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);

    /*Whole body of the car*/
       { glPushMatrix();
        //The car body do not have specular light
        glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
        glMaterialfv(GL_FRONT, GL_SHININESS, no_shininess);
        glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);

        DrawCarBody();
        glPopMatrix(); }
   

    /*Windows*/
    {glPushMatrix();
    //Windows have specular light
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, low_shininess);
    glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);

    DrawWindows();
    glPopMatrix(); }
   

    /*No specular light for ignition system and wheels*/
    glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
    glMaterialfv(GL_FRONT, GL_SHININESS, no_shininess);
    glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);

    /*Ignition System*/
    {glPushMatrix();
    glColor3f(0.3, 0.3, 0.7);
    glTranslatef(-85, -20, -12);
    glRotatef(90.0, 0, 1, 0);
    glutSolidCylinder(2.5, 20, 10, 10);
    glPopMatrix();
    }
    /*Wheels*/
    {
        DrawWheels();
    }

    glPopMatrix();
}

/*Tree*/
/*The tree was inspired by GitHub. https://github.com/JohnFNovak/3D_Trees/tree/master */
void tree() {
    glPushMatrix();
    glScalef(10, 10, 10);
    glEnable(GL_NORMALIZE); //Aable automatic normalization
    // Trunk
    glPushMatrix();
    glColor3f(0.7, 0.3, 0.0);
    glScalef(1, 3.6, 1);
    glTranslatef(0, 0, 0);
    glutSolidCube(2);
    glPopMatrix();

    // Leaves
    glPushMatrix();
    glTranslatef(0, 4.5, 0);
    glPushMatrix();
    glColor3f(0.0, 0.8, 0.3);
    glScalef(5, 1, 5);
    glutSolidCube(2);
    glPopMatrix();

    glTranslatef(0, 2, 0);
    glPushMatrix();
    glScalef(4, 1, 4);
    glColor3f(0.3, 0.8, 0.3);
    glutSolidCube(2);
    glPopMatrix();

    glColor3f(0.4, 0.8, 0.3);
    glTranslatef(0, 2, 0);
    glPushMatrix();
    glScalef(3, 1, 1);
    glutSolidCube(2);
    glPopMatrix();
    glPushMatrix();
    glScalef(1, 1, 3);
    glutSolidCube(2);
    glPopMatrix();

    // Create last top cube
    glColor3f(0.5, 0.8, 0.3);
    glTranslatef(0, 2, 0);
    glutSolidCube(2);
    glDisable(GL_NORMALIZE);  // Disable automatic normalization
    glPopMatrix();

    glPopMatrix();
}

/*Clouds*/
void sphere(void) {
    GLfloat cloud_mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };   //Define the specular light color of the cloud as 0
    GLfloat cloud_mat_emission[] = { 1.0f, 1.0f, 1.0f, 1.0f };   //Define the ambient light color of the cloud as white
    GLfloat cloud_mat_shininess = 100.0f;//Define the shiniess of the cloud as 0
    glMaterialfv(GL_FRONT, GL_SPECULAR, cloud_mat_specular);
    glMaterialfv(GL_FRONT, GL_EMISSION, cloud_mat_emission);
    glMaterialf(GL_FRONT, GL_SHININESS, cloud_mat_shininess);
    glPushMatrix();
    glColor4f(1.0, 1.0, 1.0, 0.7);
    glutSolidSphere(20, 20, 20);
    glPopMatrix();
}
void Clouds() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Enable stencil test
    glEnable(GL_STENCIL_TEST);
    glClear(GL_STENCIL_BUFFER_BIT);

    // Draw the union of all spheres into the stencil buffer
    glPushMatrix();
    glStencilFunc(GL_NOTEQUAL, 1, 1);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    // Draw the first sphere (A) and set stencil to 1

    glPushMatrix();
    sphere();
    glPopMatrix();

    // Draw the second sphere (B) only where stencil is not 1
    glTranslatef(20.0, 0.0, 0.0);
    glPushMatrix();
    sphere();
    glPopMatrix();

    // Draw the third sphere (C) only where stencil is not 1
    glTranslatef(-30.0, 15.0, 0.0);
    glPushMatrix();
    sphere();
    glPopMatrix();

    glTranslatef(20.0, 0.0, 0.0);
    // Draw the fourth sphere (D) only where stencil is not 1
    glPushMatrix();
    sphere();
    glPopMatrix();

    glPopMatrix();
    glDisable(GL_STENCIL_TEST);
    glDisable(GL_BLEND);
}

/*Lamp*/
void lamp() {
    glPushMatrix();
    glTranslatef(200, 145, -60);
    glRotatef(90, 0, 1, 0);
    glScalef(0.5, 0.5, 0.5);
    glColor3f(1, 1, 0);
    glPushMatrix();
    glPushMatrix();
    if (flag2 == 1) //Define the light bulb's matertial when in night
    {
        GLfloat mat_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
        GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
        GLfloat mat_emission[] = { 1.0f, 1.0f, .0f, 1.0f };
        glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
        glMaterialf(GL_FRONT, GL_SHININESS, 5.0);
        glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
    }
    else //Define the light bulb's matertial during the day
    {
        GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
        GLfloat mat_emission[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    }
    glutSolidSphere(15, 20, 20); // the bulb
    glPopMatrix();

    //The material of the pillar and light bulb is different
    GLfloat mat_specular2[] = { .0, .0, .0, 1.0 };
    GLfloat mat_emission2[] = { 0.0f, 0.0f, 0.0f, .0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular2);
    glMaterialf(GL_FRONT, GL_SHININESS, .0);
    glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission2);
    glPushMatrix();
    //Bezier Curve as a bracket for light bulbs
    glColor3f(0.5, 0.27, 0.113);
    glLineWidth(4);
    drawBezierCurve();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(70, -70, 0);
    glScalef(1, 55, 1);
    glutSolidCube(8); // the pillar
    glPopMatrix();

    glPopMatrix();

    glPopMatrix();

}

/*mountain*/ 
void mountains(float fltx, float flty, float fltz) {
    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat no_shininess[] = { 0.0 };
    glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
    glMaterialfv(GL_FRONT, GL_SHININESS, no_shininess);
    glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
    glPushMatrix();
    glColor3f(0.4, 0.58, 0.36);
    glTranslatef(fltx, flty, fltz);
    glRotatef(-90, 1, 0, 0);
    glutSolidCone(150, 250, 100, 100);
    glPopMatrix();
}

/*Signpost*/
void Signpost() {
    glPushMatrix();
    // Position and rotate the signpost
    glTranslatef(200, 0, 150);
    glPushMatrix();
    glRotatef(-90, 1, 0, 0);
    //vertical pole of the signpost
    glColor3f(0.7, 0.7, 0.7);
    glutSolidCylinder(2, 120, 150, 150);
    glPopMatrix();
    // Signboard
    glTranslatef(-2.5, 120 + 20, 0);
    glPushMatrix();
    glTranslatef(0, 0, 1);
    glRotatef(90, 0, 1, 0);
    glutSolidCylinder(30, 3, 30, 30);
    glPopMatrix();
    // Arrows on the signpost
    glPushMatrix();
    glTranslatef(-0.5, 0, 0);
    glRotatef(-90, 0, 1, 0);
    setStraight();// This function draws an arrow or straight element
    glPopMatrix();
    glPushMatrix();
    glTranslatef(3.5, 0, 0);
    glRotatef(-90, 0, 1, 0);
    setStraight();// This function draws an arrow or straight element
    glPopMatrix();
    glPopMatrix();
}

/* Ground and Scene Elements */
void Ground() {
    //Road
    glPushMatrix();
    glTranslatef(0, -33, 0);
    glPushMatrix();
    glScalef(10000, 1, 200);
    glColor3f(0.7, 0.7, 0.7);
    glutSolidCube(1);
    glPopMatrix();

    //Ground and Grass 
    glPushMatrix();
    glTranslatef(0, -1, 0);
    glScalef(10000.0, 1.0, 10000.0);
    glColor3f(0.75, 1, 0.8);
    glutSolidCube(1);
    glPopMatrix();

    //Lamps Posts
    glPushMatrix();
    glTranslatef(-5000 + fltLampOffset, 0, 0);
    for (size_t i = 0; i < 10000; i += 350)
    {
        glTranslatef(350, 0, 0);
        lamp();
        glPushMatrix();
        glRotatef(180, 0, 1, 0);
        lamp();
        glPopMatrix();
    }

    glPopMatrix();

    //Trees
    glPushMatrix();
    glTranslatef(-5000 + TreeOffsetX, 35, 250);
    for (size_t i = 0; i < 10000; i += 500)
    {
        glTranslatef(500, 0, 0);
        tree();
        glPushMatrix();
        glTranslatef(0, 0, -500);
        tree();
        glPopMatrix();
    }
    glPopMatrix();

    // Road Texture
    glPushMatrix();
    setRoad();
    glPopMatrix();

    //Clouds
    glPushMatrix();
    glTranslatef(300, 300, -1000);
    for (size_t i = 0; i < 4; i += 1) {
        glTranslatef(60, 10, 400);
        glPushMatrix();
        // glTranslatef(10, 10, 0);
        glRotatef(90, 0, 1, 0);
        Clouds();

        glPopMatrix();
    }
    glPopMatrix();

    glPushMatrix();
    glTranslatef(100, 280, -1000);
    for (size_t i = 0; i < 5; i += 1) {

        glTranslatef(-80, 10, 300);
        glPushMatrix();
        glRotatef(90, 0, 1, 0);
        Clouds();

        glPopMatrix();
    }
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-200, 330, -1000);
    for (size_t i = 0; i < 5; i += 1) {
        glTranslatef(-100, -10, 300);
        glPushMatrix();
        glRotatef(90, 0, 1, 0);
        Clouds();

        glPopMatrix();
    }
    glPopMatrix();

    //Sun and Moon 
    glPushMatrix();
    setSunAndMoon();
    glPopMatrix();

    // Mountains
    mountains(-150 + fltmountainOffset, 0, -800);
    mountains(500 + fltmountainOffset, 0, 700);
    mountains(-200 + fltmountainOffset, 0, 900);
    mountains(400 + fltmountainOffset, 0, -1000);
    mountains(900 + fltmountainOffset, 0, 800);

    // Signpost with Animation
    glPushMatrix();
    glTranslatef(fltboardOffset, 0, 0);
    Signpost();
    glPopMatrix();
    glPopMatrix();
}

/* Draw Ground and Cars */
void groundAndCars() {
    Ground(); // Draw the ground and scene elements

    if (flag1 == 0) { //Single car mode
        glPushMatrix();
        glTranslatef(-100, 0, fltCarOffset0);
        DrawCar();
        glPopMatrix();
    }
    else          // Many cars mode    
    {
        glPushMatrix();
        glTranslatef(0, 0, -60);
        glPushMatrix();
        for (size_t i = 0; i < 15000; i += 1000)
        {
            glPushMatrix();
            glTranslatef(-5000 + fltCarOffset1 + i, 0, 0);
            DrawCar();
            glPopMatrix();
        }
        glPopMatrix();
        glTranslatef(-300, 0, 60);
        glPushMatrix();
        for (size_t i = 0; i < 15000; i += 1200)
        {
            glPushMatrix();
            glTranslatef(-5000 + fltCarOffset1 + i, 0, 0);
            DrawCar();
            glPopMatrix();
        }
        glPopMatrix();
        glTranslatef(-300, 0, 60);
        glPushMatrix();
        for (size_t i = 0; i < 15000; i += 900)
        {
            glPushMatrix();
            glTranslatef(-5000 + fltCarOffset1 + i, 0, 0);
            DrawCar();
            glPopMatrix();
        }
        glPopMatrix();
        glPopMatrix();
    }
    glutPostRedisplay();// Request a redraw
}

/* Display Callback Function */
void displayObject()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fltFOV, 1, 0.1, 5000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(fltX0 * fltZoom + fltViewingAngle, fltY0 * fltZoom, fltZ0 * fltZoom, 0, 0, 0, 0, 1, 0);
    glEnable(GL_DEPTH_TEST);

    // Set background color based on day or night mode
    glClearColor(.73, .88, .96, 1); // Day mode
    if (flag2 == 1) 
    {
        glClearColor(.0, .0, .0, 1);// Night mode
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();
    glRotatef(yangle, 0.0, 1.0, 0.0);
    groundAndCars(); // Draw the ground and cars
    glPopMatrix();

    glutSwapBuffers();// Swap front and back buffers to display the rendered image
}

/* Reshape Callback Function */
void reshapeWindow(GLint intNewWidth, GLint intNewHeight)
{
    glViewport(0, 0, intNewWidth, intNewHeight);
}

/* Adjust Display based on Keyboard Input */
void adjustDisplay(unsigned char key, int x, int y)
{
    switch (key)
    {

    case 'y':
        yangle += 5.0; // Rotate scene to the right
        glutPostRedisplay();
        break;

    case 'Y':
        yangle -= 5.0; // Rotate scene to the left
        glutPostRedisplay();
        break;
    }

    if (flag1 == 0 && (key == 'l' || key == 'L')) {
        fltCarAngle += 3; // Turn car left
        if (fltCarAngle > 12)
        {
            fltCarAngle = 13;
        }
        fltCarOffset0 -= 1; // Move car left
        if (fltCarOffset0 < -60)
        {
            fltCarOffset0 = -60;
        }
        else
        {
            if (fltAcceleration == 0) {
                Wheelangle += 4; // Rotate wheels
            }
        }
    }

    if (flag1 == 0 && (key == 'r' || key == 'R')) {
        fltCarAngle -= 5;// Turn car right
        if (fltCarAngle < -12)
        {
            fltCarAngle = -13;
        }
        fltCarOffset0 += 1; // Move car right
        if (fltCarOffset0 > 60)
        {
            fltCarOffset0 = 60;
        }
        else
        {
            if (fltAcceleration == 0) {
                Wheelangle += 4; // Rotate wheels
            }
        }
    }

    if (key == '-' || key == '_')
        fltAcceleration -= 0.5; //Speed down
    if (key == '=' || key == '+')
        fltAcceleration += 0.5; //Speed up
    if (key == 'f' || key == 'F')
    {
        fltViewingAngle -= 5; // Adjust viewing angle
    }

    if (key == 'd' || key == 'D')
        if (fltY0 > 35.0)
            fltY0 -= 5.0;// Move camera down
    if (key == 'u' || key == 'U')
        if (fltY0 < 500)
            fltY0 += 5; // Move camera up

    if ( key == 'W' || key == 'w')
    {
        carbodyB += 0.1; // Increase blue color component of the car body
        carbodyG  += 0.1;// Increase green color component of the car body
    }

    if (key == 'S' || key == 's')
    {
        carbodyB -= 0.1;  // Decrease blue color component of the car body
        carbodyG -= 0.1;  // Decrease green color component of the car body
    }

    if (key == 'R' || key == 'r')
        MoonAngle += 5;// Rotate the moon

    glutPostRedisplay();// Request a redraw
}

/* Handle Mouse Wheel Input */
void mouseWheel(int wheel, int direction, int x, int y) {
    if (direction == 1) {
        // Scroll Up
        if (fltZoom > 0.2)
            fltZoom -= 0.1; //Zoom in
    }
    else if (direction == -1) {
        // Scroll Down
        if (fltZoom < 1.5)
            fltZoom += 0.1;//Zoom out
        if (fltZoom > 1.5) {
            fltZoom = 1.5;// Set maximum zoom level
        }
    }

    glutPostRedisplay(); // Request a redraw
}

/* Submenu Function for Car Driving Modes */
void submenufunc1(int data) {
    switch (data) {
    case 1:
        flag1 = 1;// Activate Many Cars Mode
        fltAcceleration = 2;// Set initial acceleration
        glutPostRedisplay();// Request a redraw
        break;
    case 2:
        flag1 = 0;// Activate Single Car Mode
        fltAcceleration = 2; // Set initial acceleration
        glutPostRedisplay();// Request a redraw
        break;
    }
}

/* Submenu Function for Light Modes */
void submenufunc2(int data) {
    switch (data) {
    case 1:
        flag2 = 0; // Activate Day Mode
        break;
    case 2:
        flag2 = 1; // Activate Night Mode
        break;
    }
}

/* Menu Function */
void menufunc(int data) {
    // Placeholder for menu functionality
    switch (data) {
    case 1:
        break;
    case 2:
        break;
    }
}

/* Main Function */
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(intWinWidth, intWinHeight);
    glutCreateWindow("Car-based Road Transport System");

    myinit();// Initialize lighting and material properties
    setTexture();// Set texture parameters
    glutDisplayFunc(displayObject);// Set the display callback function
    glutReshapeFunc(reshapeWindow);// Set the window reshape callback function
    glutKeyboardFunc(adjustDisplay);// Set the keyboard callback function
    glutIdleFunc(animateCar); // Set the idle callback function for animation
    glutMouseWheelFunc(mouseWheel); // Register the mouse wheel callback
    // Build the contents of submenu 1 for Car Driving Modes
    submenu1 = glutCreateMenu(submenufunc1);
    glutAddMenuEntry("Many Cars Mode", 1);
    glutAddMenuEntry("Signle Car Mode", 2);

    // Build the contents of submenu 2 for Light Modes
    submenu2 = glutCreateMenu(submenufunc2);
    glutAddMenuEntry("Day Mode", 1);
    glutAddMenuEntry("Night Mode", 2);
    glutAttachMenu(GLUT_RIGHT_BUTTON);// Attach submenu 2 to the right mouse button
    
    // Build the main menu
    menu = glutCreateMenu(menufunc);
    glutAddMenuEntry("Car Transport System", 1);
    glutAddMenuEntry("", 2);// Placeholder entry

    // Attach submenus to the main menu
    glutAddSubMenu("Car Driving Mode", submenu1);
    glutAddSubMenu("Light Mode", submenu2);
    glutAttachMenu(GLUT_RIGHT_BUTTON);// Attach the main menu to the right mouse button

    glutMainLoop();// Enter the GLUT main event loop
    return 0;
}
