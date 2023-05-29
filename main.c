#include <stdio.h>
#include <GL/glut.h>

#include "bezier.h"

#define WIDTH 640
#define HEIGHT 480
#define MAX_POINTS 5

unsigned char pixels[WIDTH * HEIGHT];
unsigned int pointsX[MAX_POINTS], pointsY[MAX_POINTS];
unsigned int n_points = 0;

int selectedPoint = -1; // Index of the selected point (-1 means no point selected)

void display()
{
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

	glPointSize(5.0f);
	// Draw the points in different colors
	glBegin(GL_POINTS);
	for (int i = 0; i < n_points; i++)
	{
		switch (i)
		{
			case 1:
				glColor3f(1.0f, 0.0f, 0.0f); // Red
				break;
			case 2:
				glColor3f(0.0f, 1.0f, 0.0f); // Green
				break;
			case 3:
				glColor3f(0.0f, 0.0f, 1.0f); // Blue
				break;
			default:
				glColor3f(0.0f, 0.0f, 0.0f); // Black
				break;
		}

		float ndcX = (2.0f * (float)pointsX[i]) / WIDTH - 1.0f;
		float ndcY = 1.0f - (2.0f * (float)pointsY[i]) / HEIGHT;

		// Draw the point
		glVertex2f(ndcX, ndcY);
	}
	glEnd();

	glFlush();

	// Swap the buffers
	glutSwapBuffers();
}

void clear()
{
	for (int i = 0; i < WIDTH * HEIGHT; ++i)
	{
		pixels[i] = 255;
	}
}

void update()
{
	clear();
	if (n_points > 1)
	{
		bezier(pixels, pointsX, pointsY, n_points, WIDTH, HEIGHT);
	}

	// Refresh the display
	glutPostRedisplay();
}

void addPoint(int x, int y)
{
	if (n_points < MAX_POINTS)
	{
		pointsX[n_points] = x;
		pointsY[n_points] = y;
		++n_points;
		update();
	}
}

void movePoint(int x, int y)
{
	if (selectedPoint != -1)
	{
		pointsX[selectedPoint] = x;
		pointsY[selectedPoint] = y;
		update();
	}
}

void handleMouseEvent(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		// Check if a point is selected
		for (int i = 0; i < n_points; i++)
		{
			float ndcX = (2.0f * (float)pointsX[i]) / WIDTH - 1.0f;
			float ndcY = 1.0f - (2.0f * (float)pointsY[i]) / HEIGHT;

			// Convert NDC coordinates to screen coordinates
			int screenX = (int)((ndcX + 1.0f) / 2.0f * WIDTH);
			int screenY = (int)((1.0f - ndcY) / 2.0f * HEIGHT);

			// Check if the mouse click is within the range of the point
			if (abs(screenX - x) <= 5 && abs(screenY - y) <= 5)
			{
				selectedPoint = i;
				return;
			}
		}

		// If no point is selected, add a new point
		addPoint(x, y);
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		selectedPoint = -1; // Reset selected point when mouse button is released
	}
}

void handleMotionEvent(int x, int y)
{
	movePoint(x, y);
}

void handleKeyboardEvent(unsigned char key, int x, int y)
{
	if (key == 8) // Backspace key
	{
		if (n_points > 0)
			n_points--;


		update();
	}
}

// Initializes GLUT, the display mode, and main window; registers callbacks;
// enters the main event loop.
int main(int argc, char **argv)
{
	clear();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

	glutInitWindowPosition(80, 80);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("Bezier Editor");

	glutDisplayFunc(display);
	glutMouseFunc(handleMouseEvent);
	glutMotionFunc(handleMotionEvent);
	glutKeyboardFunc(handleKeyboardEvent);

	glutMainLoop();
}
