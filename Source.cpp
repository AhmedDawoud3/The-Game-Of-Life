/*
	Conway's Game of Life

	Auther: Ahmed Dawoud
	adawoud1000@hotmail.com

	The Game of Life, is a cellular automaton devised by the British
	mathematician John Horton Conway in 1970. It is a zero-player game,
	meaning that its evolution is determined by its initial state,
	requiring no further input. One interacts with the Game of Life
	by creating an initial configuration and observing how it evolves.
	It is Turing complete and can simulate a universal constructor or any other Turing machine.


	The universe of the Game of Life is an infinite, two-dimensional
	orthogonal grid of square cells, each of which is in one of two possible states,
	live or dead, (or populated and unpopulated, respectively). Every cell interacts
	with its eight neighbours, which are the cells that are horizontally, vertically,
	or diagonally adjacent. At each step in time, the following transitions occur:

	-Any live cell with fewer than two live neighbours dies, as if by underpopulation.
	-Any live cell with two or three live neighbours lives on to the next generation.
	-Any live cell with more than three live neighbours dies, as if by overpopulation.
	-Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
*/

#include <GLFW/glfw3.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <algorithm> 
#include <Math.h>
#include <Windows.h>

void DrawRectangle(float  x, float y, int WIDTH, int HEIGHT);
void BlackCell(float  x, float y, int WIDTH, int HEIGHT);
void WhiteCell(float  x, float y, int WIDTH, int HEIGHT);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

float clip(double n, float lower, float upper) {
	return max(lower, min(n, upper));
}

const int wHeight = 720;
const int wWidth = 1280;

int gameSpeed = 1;

int main(void)
{
	// First, we initialize the window
	GLFWwindow* window;

	// The resolution is the number of squares compared to window resolution
	const int resolution = 10;

	// Calcute the number of rows and colums of cells
	const int rows = wHeight / resolution;
	const int cols = wWidth / resolution;

	// The grid is a 2d array of int (0, 1) where 0 means it's dead and 1 means it's allive
	int grid[cols][rows];
	// neighbors is the number cells in all direction around the cell (above, below, left, right, and diagonals)
	int neighbors;

	// We set all the cell to dead at first
	for (int i = 0; i < cols; i++) {
		for (int j = 0; j < rows; j++) {
			grid[i][j] = 0;
		}
	}

	// The steo contralls the game speed
	int step = 0;
	// when playing is false, you're in edit mode
	bool playing = false;
	double mouseX, mouseY;
	// viewGrid shows the grid in edit mode only
	bool viewGrid = true;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	// Make the window unresizeable
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(wWidth, wHeight, "The Game Of Life", NULL, NULL);

	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	/* Make the window's context current */
	glfwMakeContextCurrent(window);


	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		// SetScrollCallback so that when mouse wheel is used, call the "scroll_callback" function
		glfwSetScrollCallback(window, scroll_callback);

		// Increase the step every frame
		step++;

		//VSync
		glfwSwapInterval(true);

		// Clear the background and start drawing
		glClear(GL_COLOR_BUFFER_BIT);

		// When in edit mode, if the mouse middle button is clicked then start the game
		if (!playing && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS)
		{
			playing = true;
		}

		// Save the mouse X, Y position and clamp it in the window dimentions
		glfwGetCursorPos(window, &mouseX, &mouseY);
		mouseX = clip(mouseX, 0, wWidth);
		mouseY = clip(mouseY, 0, wHeight);

		// Add Cell when mouse left button is clicked
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
			for (int i = 0; i < cols; i++) {
				for (int j = 0; j < rows; j++) {
					// We check it with AABB collision
					if ((i * resolution < mouseX) && (i * resolution + resolution > mouseX) && (j * resolution < mouseY) && (j * resolution + resolution > mouseY) && !playing) {
						// If found then set it to 1 (alive)
						grid[i][j] = 1;
					}
				}
			}
		}

		// Do the same thing but for removing the cell with right click
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
			for (int i = 0; i < cols; i++) {
				for (int j = 0; j < rows; j++) {
					if ((i * resolution < mouseX) && (i * resolution + resolution > mouseX) && (j * resolution < mouseY) && (j * resolution + resolution > mouseY) && !playing) {
						grid[i][j] = 0;
					}
				}
			}
		}

		// If "R" is pressed then clear all cells and go to edit mode
		if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
			for (int i = 0; i < cols; i++) {
				for (int j = 0; j < rows; j++) {
					grid[i][j] = 0;
					playing = false;
				}
			}
		}

		// Turn the grid on with CTRL + V
		if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
			viewGrid = true;
		}
		// Turn the grid on with ALT + V
		if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS) {
			viewGrid = false;
		}

		// Draw the cells
		for (int i = 0; i < cols; i++) {
			for (int j = 0; j < rows; j++) {
				if (grid[i][j] == 1) {
					WhiteCell(i * resolution, j * resolution, resolution, resolution);
				}
				else {
					if (playing)
					{
						glColor3f(0, 0, 0);
						DrawRectangle(i * resolution, j * resolution, resolution, resolution);
					}
					else if (viewGrid) {
						BlackCell(i * resolution, j * resolution, resolution, resolution);
					}
				}
				if ((i * resolution < mouseX) && (i * resolution + resolution > mouseX) && (j * resolution < mouseY) && (j * resolution + resolution > mouseY)) {
					glColor4f(0.15, 0.6, 0.75, 0.3);
					DrawRectangle(i * resolution, j * resolution, resolution, resolution);
				}

			}
		}

		// Make the grid update with game speed
		if (step >= gameSpeed)
		{
			step = 0;
			if (playing) {
				// "next" array hold the gird for the next frame
				int next[cols][rows];

				// Compute Next Based in grid
				for (int i = 0; i < cols; i++) {
					for (int j = 0; j < rows; j++) {
						// We save the state of the cell for later
						int state = grid[i][j];

						// Count Live Neighbors
						neighbors = 0;
						for (int I = -1; I < 2; I++)
						{
							for (int J = -1; J < 2; J++)
							{
								// Here {(i + I + cols) % cols} is for looping over the edges
								int Col = (i + I + cols) % cols;
								int Row = (j + J + rows) % rows;

								neighbors += grid[Col][Row];
							}
						}
						// Remove the cell it self from neighbors 
						neighbors -= grid[i][j];

						// Then We follow the rules:
						if (state == 0 && neighbors == 3)
						{
							next[i][j] = 1;
						}
						else if (state == 1 && (neighbors < 2 || neighbors > 3)) {
							next[i][j] = 0;
						}
						else {
							next[i][j] = state;
						}

						/*
						// For making the cell be allive if the one bellow it is allive
						if (grid[i][j + 1] == 1) {
							next[i][j] = 1;
						}*/

					}
				}

				// Then assign "next" values to grid
				for (int i = 0; i < cols; i++) {
					for (int j = 0; j < rows; j++) {
						grid[i][j] = next[i][j];
					}
				}
			}
		}




		// Swap front and back buffers
		glfwSwapBuffers(window);

		// Poll for and process events
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	// Changes the game speed according to the mouse wheel
	if (yoffset < 0) {
		gameSpeed++;
	}
	else if (yoffset > 0) {
		gameSpeed = max(--gameSpeed, 0);
	}
}

void DrawRectangle(float  x, float y, int WIDTH, int HEIGHT) {
	/// <summary>
	/// Draws a rectanlge to the screen
	/// </summary>
	/// <param name="x"> X for the upper left edge of the rectangle </param>
	/// <param name="y"> Y for the upper left edge of the rectangle </param>
	/// <param name="WIDTH"> The Rectanlges's width </param>
	/// <param name="HEIGHT"> The Rectangle's height </param>
	float startX;
	float startY;
	float width = wWidth;
	float height = wHeight;
	startX = (x - width / 2) / (width / 2);
	startY = -(y - height / 2) / (height / 2);

	float endX = startX + WIDTH / width * 2;
	float endY = startY + -HEIGHT / height * 2;

	glRectf(startX, startY, endX, endY);
}


void WhiteCell(float  x, float y, int WIDTH, int HEIGHT)
{
	// Special function for drawing white cells with black borders
	glColor3f(0, 0, 0);
	DrawRectangle(x, y, WIDTH, HEIGHT);
	glColor3f(1, 1, 1);
	DrawRectangle(x + 0.1, y + 0.1, WIDTH - 0.2, HEIGHT - 0.2);
}
void BlackCell(float  x, float y, int WIDTH, int HEIGHT)
{
	// Special function for drawing black cells with white borders
	glColor3f(1, 1, 1);
	DrawRectangle(x, y, WIDTH, HEIGHT);
	glColor3f(0, 0, 0);
	DrawRectangle(x + 0.0001, y + 0.0001, WIDTH - 0.0002, HEIGHT - 0.0002);
}
