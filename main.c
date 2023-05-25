#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


#include <GL/glut.h>

#include "bezier.h"

// Define the pixel array dimensions
#define WIDTH 640
#define HEIGHT 480

// Define the pixel array
unsigned char pixels[WIDTH * HEIGHT * 3];

// Function to display the pixel array
void display()
{
printf("rerender\n");
    glClear(GL_COLOR_BUFFER_BIT);

    // Load the pixel array as a texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);

    // Enable texturing
    glEnable(GL_TEXTURE_2D);

    // Set up the texture parameters
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Render a quad with the texture mapped onto it
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(-1.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(1.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex2f(1.0f, 1.0f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(-1.0f, 1.0f);
    glEnd();

    // Disable texturing
    glDisable(GL_TEXTURE_2D);

    // Swap the buffers
    glutSwapBuffers();
}

void handleMouseEvent(int button, int state, int x, int y){
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        printf("New point: (%d, %d)",x, y);
        printf("\n");
        const int index = (HEIGHT - y - 1)*WIDTH*3 + x*3;
        pixels[index] = 255;
        pixels[index+1] = 255;
        pixels[index+2] = 255;

        glutPostRedisplay();
    }
}

// Initializes GLUT, the display mode, and main window; registers callbacks;
// enters the main event loop.
int main(int argc, char** argv) {

    for(int i = 0; i < WIDTH;i+=3){
    pixels[i] = 255;
    pixels[i+1] = 0;
    pixels[i+2] = 0;
    }

    // Use a single buffered window in RGB mode (as opposed to a double-buffered
    // window or color-index mode).
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    glutInitWindowPosition(80, 80);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Bezier Editor");

    // Tell GLUT that whenever the main window needs to be repainted that it
    // should call the function display().
    glutDisplayFunc(display);

    glutMouseFunc(handleMouseEvent);

    // Tell GLUT to start reading and processing events.  This function
    // never returns; the program only exits when the user closes the main
    // window or kills the process.
    glutMainLoop();
}