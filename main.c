#include <stdio.h>
#include <GL/glut.h>

#include "bezier.h"

#define WIDTH 640
#define HEIGHT 480
#define MAX_POINTS 5

unsigned char pixels[WIDTH * HEIGHT];
unsigned int pointsX[MAX_POINTS], pointsY[MAX_POINTS];
unsigned int n_points = 0;

void display()
{
	printf("rerender\n");
    glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0f, 1.0f, 1.0f); // Set color to white
    // Load the pixel array as a texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, WIDTH, HEIGHT, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, pixels);

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

	glPointSize(smallPointSize);

	// Draw the remaining points in different colors
	glBegin(GL_POINTS);
	for (int i = 2; i < n_points; i++) {
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
	        unsigned int lastX = pointsX[1];
	        unsigned int lastY = pointsY[1];

	        for(int i = 1; i < MAX_POINTS-1; ++i){
	            pointsX[i] = pointsX[i+1];
	            pointsY[i] = pointsY[i+1];
	        }

	        pointsX[MAX_POINTS - 1] = lastX;
	        pointsY[MAX_POINTS - 1] = lastY;
	        bezier(pixels, pointsX, pointsY, MAX_POINTS, WIDTH, HEIGHT);

	        for(int i = 0; i < HEIGHT*WIDTH; ++i){
	            if(pixels[i] == 0){
	                printf("aaa\n");
	            }
	        }

	        for(int i = MAX_POINTS-1; i >= 2; --i){
		        pointsX[i] = pointsX[i-1];
		        pointsY[i] = pointsY[i-1];
	        }

	        pointsX[1] = lastX;
	        pointsY[1] = lastY;

        }

	    // Refresh the display
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
		if (n_points > 0){
			n_points--;
			printf("Point removed\n");
		}

		// Refresh the display
		glutPostRedisplay();
	}
}

void test(){
	pointsX[0] = 27;
	pointsX[1] = 37;
	pointsX[2] = 47;
	pointsX[3] = 57;
	pointsX[4] = 67;
	pointsY[0] = 127;
	pointsY[1] = 137;
	pointsY[2] = 147;
	pointsY[3] = 157;
	pointsY[4] = 167;
	bezier(pixels, pointsX, pointsY, MAX_POINTS, WIDTH, HEIGHT);

}

// Initializes GLUT, the display mode, and main window; registers callbacks;
// enters the main event loop.
int main(int argc, char** argv) {
    for(int i = 0; i < WIDTH * HEIGHT; ++i){
        pixels[i] = 255;
    }
	test();
	return 0;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    glutInitWindowPosition(80, 80);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Bezier Editor");


    glutDisplayFunc(display);

    glutMouseFunc(handleMouseEvent);
	glutKeyboardFunc(handleKeyboardEvent);

    glutMainLoop();
}