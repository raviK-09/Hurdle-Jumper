#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <ctime>
#include <string>

// Window size
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

// Square parameters
float squareX = 0.0f;
float squareY = 0.0f;
const float SQUARE_SIZE = 50.0f;
const float SQUARE_SPEED = 30.0f;


// Obstacle parameters
const float OBSTACLE_SIZE = 75.0f;
const float OBSTACLE_SIZE1 = 160.0f;
const float OBSTACLE_SPEED = 15.0f;
std::vector<std::pair<float, float>> obstacles;

// Time
clock_t startTime;

// Function prototypes
void init();
void display();
void reshape(int width, int height);
void keyboard(unsigned char key, int x, int y);
void update(int value);
void printTime();

int main(int argc, char** argv) {
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("Avoid Obstacles");

    // Register callbacks
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, update, 0);

    // Initialize game
    init();

    // Start the timer
    startTime = clock();

    // Start the main loop
    glutMainLoop();

    return 0;
}

void init() {
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    

    glLoadIdentity();

    // Draw the square
    glColor3f(0.9f, 0.9f, 0.9f);
    glBegin(GL_QUADS);
    glVertex2f(squareX, squareY);
    glVertex2f(squareX + SQUARE_SIZE, squareY);
    glVertex2f(squareX + SQUARE_SIZE, squareY + SQUARE_SIZE);
    glVertex2f(squareX, squareY + SQUARE_SIZE);
    glEnd();


    // Draw the obstacles
    glColor3f(0.6f, 0.2f, 0.2f);
    for (const auto& obstacle : obstacles) {
        glBegin(GL_QUADS);
        glVertex2f(obstacle.first, obstacle.second);
        glVertex2f(obstacle.first + OBSTACLE_SIZE, obstacle.second);
        glVertex2f(obstacle.first + OBSTACLE_SIZE, obstacle.second + OBSTACLE_SIZE1);
        glVertex2f(obstacle.first, obstacle.second + OBSTACLE_SIZE1);
        glEnd();
    }

    printTime();

    glutSwapBuffers();
}

void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'w':
    case 'W':
        squareY -= SQUARE_SPEED;
        break;
    case 's':
    case 'S':
        squareY += SQUARE_SPEED;
        break;
    case 'a':
    case 'A':
        squareX -= SQUARE_SPEED;
        break;
    case 'd':
    case 'D':
        squareX += SQUARE_SPEED;
        break;
    case 27:  // ESC key
        exit(0);
        break;
    }
}

void update(int value) {
    // Update obstacle positions
    for (auto& obstacle : obstacles) {
        obstacle.first -= OBSTACLE_SPEED;

        // Check collision
        if (squareX + SQUARE_SIZE >= obstacle.first && squareX <= obstacle.first + OBSTACLE_SIZE &&
            squareY + SQUARE_SIZE >= obstacle.second && squareY <= obstacle.second + OBSTACLE_SIZE1) {
            std::cout << std::endl;
            std::cout << "Congrats you scored a Total Points of->" << value / 50 -1<< std::endl;
            std::cout << std::endl;
            std::cout << "Don't cry you can do much better in next game." << std::endl;
            std::cout << std::endl;
            std::cout << "***********************Game Over!****************************" << std::endl;
            std::cout << std::endl;
            printTime();
            exit(0);
        }
    }

    // Remove obstacles that are offscreen
    obstacles.erase(std::remove_if(obstacles.begin(), obstacles.end(),
        [](const auto& obstacle) { return obstacle.first + OBSTACLE_SIZE < 0; }),
        obstacles.end());

    // Add new obstacles periodically
    if (value % 50 == 0) {
        const int x = (WINDOW_HEIGHT - OBSTACLE_SIZE);
        obstacles.emplace_back(WINDOW_WIDTH, rand() % x);
    }

    // Redraw the scene
    glutPostRedisplay();

    // Set the next update
    glutTimerFunc(16, update, value + 1);
}

void printTime() {

    clock_t currentTime = clock();
    double elapsedTime = double(currentTime - startTime) / CLOCKS_PER_SEC;
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(10.0f, 10.0f);
    std::string timeText = "Time: " + std::to_string(elapsedTime) + " seconds";
    for (const char& c : timeText) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);
    }
}
