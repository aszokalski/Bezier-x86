#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>


#include <GL/glut.h>

#include "bezier.h"

// Define the pixel array dimensions
#define WIDTH 640
#define HEIGHT 480
#define MAX_POINTS 5

// Define the pixel array
unsigned char pixels[WIDTH * HEIGHT * 3];
unsigned int pointsX[5], pointsY[5];
unsigned int n_points = 0;

// Function to display the pixel array
void display()
{
	printf("rerender\n");
    glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0f, 1.0f, 1.0f); // Set color to white
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

	// Draw input points
	const float bigPointSize = 7.0f;
	const float smallPointSize = 5.0f;

	glPointSize(bigPointSize);
	glColor3f(0.0f, 0.0f, 0.0f); // Set color to black
	glBegin(GL_POINTS);
	for (int i = 0; i < 2 && i < n_points; i++) {
		float ndcX = (2.0f * (float) pointsX[i]) / WIDTH - 1.0f;
		float ndcY = 1.0f - (2.0f * (float) pointsY[i]) / HEIGHT;

		// Draw the point
		glVertex2f(ndcX, ndcY);
	}
	glEnd();

	// Set smaller point size for the remaining points
	glPointSize(smallPointSize);

	// Draw the remaining points in different colors
	glBegin(GL_POINTS);
	for (int i = 2; i < n_points; i++) {
		// Set a different color for each point
		if (i == 2)
			glColor3f(1.0f, 0.0f, 0.0f); // Red
		else if (i == 3)
			glColor3f(0.0f, 1.0f, 0.0f); // Green
		else if (i == 4)
			glColor3f(0.0f, 0.0f, 1.0f); // Blue
		else
			glColor3f(1.0f, 1.0f, 1.0f); // White

		float ndcX = (2.0f * (float) pointsX[i]) / WIDTH - 1.0f;
		float ndcY = 1.0f - (2.0f * (float) pointsY[i]) / HEIGHT;

		// Draw the point
		glVertex2f(ndcX, ndcY);
	}
	glEnd();

	glFlush();

    // Swap the buffers
    glutSwapBuffers();
}

void addPoint(int x, int y){
    if(n_points < MAX_POINTS){
	    printf("New point: (%d, %d)",x, y);
	    printf("\n");
        pointsX[n_points] = x;
        pointsY[n_points] = y;
        ++n_points;
        if(n_points == MAX_POINTS){
	        printf("Drawing the curve...\n");
        }

	    glutPostRedisplay();
    }
}

void handleMouseEvent(int button, int state, int x, int y){
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        addPoint(x, y);
    }
}

void handleKeyboardEvent(unsigned char key, int x, int y)
{
	if (key == 8) // Backspace key
	{
		// Remove the last point if there are points available
		if (n_points > 0){
			n_points--;
			printf("Point removed\n");
		}


		// Refresh the display
		glutPostRedisplay();
	}
}

// Initializes GLUT, the display mode, and main window; registers callbacks;
// enters the main event loop.
int main(int argc, char** argv) {
    // Use a single buffered window in RGB mode (as opposed to a double-buffered
    // window or color-index mode).
    for(int i = 0; i < WIDTH * HEIGHT * 3; ++i){
        pixels[i] = 255;
    }
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    glutInitWindowPosition(80, 80);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Bezier Editor");

    // Tell GLUT that whenever the main window needs to be repainted that it
    // should call the function display().
    glutDisplayFunc(display);

    glutMouseFunc(handleMouseEvent);
	glutKeyboardFunc(handleKeyboardEvent);

    // Tell GLUT to start reading and processing events.  This function
    // never returns; the program only exits when the user closes the main
    // window or kills the process.
    glutMainLoop();
}