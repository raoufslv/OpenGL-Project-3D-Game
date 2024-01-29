#define STB_IMAGE_IMPLEMENTATION
#include <iostream>
#include "stb_image/stb_image.h"
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/transform.hpp"
// Include glText
#define GLT_IMPLEMENTATION
#include "gltext/gltext.h"
#include <SFML/Audio.hpp>

int main();
void lostGame();
void wonGame();
void sheepEatTebn();
void wolfeatSheep();
void sheepRunAwayFun();
void checkGameStatus();
void translatingFunction();
void Reshape(GLFWwindow *window, int width, int height);
void InitTexture(void);
void initModels();
int initWindow();
void renderingScene();
void drawtext_func();
void while_func();
void startGame();
static void key_callback_empty(GLFWwindow *window, int key, int scancode, int action, int mods);
static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);

// Structure to store vertex data
struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
    glm::vec3 color; // New attribute for color
};

// Structure to store material data
struct Material
{
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
};

// global variables
sf::SoundBuffer wolfEatBuffer;
sf::Sound wolfEatSound;
GLboolean isPlayingWolf = false;

sf::SoundBuffer sheepEatBuffer;
sf::Sound sheepEatSound;
GLboolean isPlayingSheep = false;

sf::SoundBuffer sheepRunAwayBuffer;
sf::Sound sheepRunAwaySound;

GLboolean GameOver = false;
GLboolean GameStart = false;
GLboolean start_game = true;

// declare the posiition of each model in the first side of the river
glm::vec3 wolfPos2 = glm::vec3(6.0f, 0.8f, -7.0f);
glm::vec3 sheepPos2 = glm::vec3(6.0f, -0.5f, 0.0f);
glm::vec3 tebnPos2 = glm::vec3(6.0f, -0.5f, 7.0f);
glm::vec3 farmerPos2 = glm::vec3(-5.0f, -0.8f, 0.0f);
glm::vec3 boatPos2 = glm::vec3(-6.0f, -1.4f, 0.0f);
glm::vec3 runAwayPos2 = glm::vec3(16.0f, -0.5f, 0.0f);

// declare the posiition of each model in the second side of the river
glm::vec3 wolfPos = glm::vec3(-46.0f, 1.0f, -7.0f);
glm::vec3 sheepPos = glm::vec3(-49.0f, -0.5f, 0.0f);
glm::vec3 tebnPos = glm::vec3(-49.0f, -0.5f, 7.0f);
glm::vec3 farmerPos = glm::vec3(-37.0f, -0.8f, 0.0f);
glm::vec3 boatPos = glm::vec3(-38.0f, -1.5f, 0.0f);
glm::vec3 runAwayPos = glm::vec3(-56.0f, -0.5f, 0.0f);

GLint boatSide = 1;
GLint wolfSide = 1;
GLint sheepSide = 1;
GLint tebnSide = 1;

GLboolean boatEmpty = true;
GLchar onBoat = ' ';

GLboolean isTransitioning = false;
GLfloat transitionStartTime;
GLfloat transitionDuration = 2.0f; // Adjust this as needed

GLboolean SheepIsEating = false;
GLfloat SheepEatingStartTime;
GLfloat SheepEatingDuration = 1.7f; // Adjust this as needed

GLboolean WolfIsEating = false;
GLfloat WolfEatingStartTime;
GLfloat WolfEatingDuration = 2.0f; // Adjust this as needed

GLboolean sheepRunAway = false;
GLfloat sheepRunAwayStartTime;
GLfloat sheepRunAwayDuration = 1.9f; // Adjust this as needed

GLboolean drawText = false;
char text_buffer[80];
char text_buffer2[80];
char text_buffer3[200];

const int nObjects = 8;
const char *filenamesTextures[] = {"textures/wolf.jpg", "textures/sheap.jpg", "textures/tben.png", "textures/farmer.jpg", "textures/boat.PNG", "textures/river.png", "textures/cloud.png", "textures/campfire.jpg"};
const char *filenamesModels[] = {"model/wolf.obj", "model/sheap.obj", "model/tben.obj", "model/farmer.obj", "model/boat.obj", "model/latest_forest2.obj", "model/cloud.obj", "model/campfire.obj"};


GLuint tab_texture[nObjects];
glm::mat4 Model[nObjects];

// shader program
GLuint shaderProgram;

// declare the window
GLFWwindow *window;

void lostGame()
{
    // make the game over
    GameOver = true;
    glfwSetKeyCallback(window, key_callback_empty);

    // print a message to the user
    drawText = true;
    snprintf(text_buffer, sizeof(text_buffer), "You lost!");
    snprintf(text_buffer2, sizeof(text_buffer2), "Press Enter to try again");

    while (GameOver)
    {
        renderingScene();

        if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
        {
            GameOver = false;
        }
        else if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            exit(0);
        }
        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    boatSide = 1;
    wolfSide = 1;
    sheepSide = 1;
    tebnSide = 1;

    boatEmpty = true;
    onBoat = ' ';
    initModels();
    glfwSetKeyCallback(window, key_callback);

    drawText = false;
    while_func();
}

void startGame()
{
    glfwSetKeyCallback(window, key_callback_empty);

    // print a message to the user
    drawText = true;
    snprintf(text_buffer, sizeof(text_buffer), "Press ENTER To start");
    snprintf(text_buffer3, sizeof(text_buffer3), "W: Wolf T: Tben K: Kebch");

    while (start_game)
    {
        renderingScene();

        if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
        {
            start_game = false;
        }
        else if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            exit(0);
        }
        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwSetKeyCallback(window, key_callback);

    drawText = false;
    while_func();
}

void wonGame()
{
    // make the game over
    GameOver = true;
    glfwSetKeyCallback(window, key_callback_empty);

    // print a message to the user
    drawText = true;
    snprintf(text_buffer, sizeof(text_buffer), "You Solved the puzzle!!!");
    snprintf(text_buffer2, sizeof(text_buffer2), "Press Enter to try again");

    while (GameOver)
    {
        renderingScene();

        if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
        {
            GameOver = false;
        }
        else if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            exit(0);
        }
        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    boatSide = 1;
    wolfSide = 1;
    sheepSide = 1;
    tebnSide = 1;

    boatEmpty = true;
    onBoat = ' ';
    initModels();
    glfwSetKeyCallback(window, key_callback);

    drawText = false;
    while_func();
}

static void key_callback_empty(GLFWwindow *window, int key, int scancode, int action, int mods)
{
}

void sheepEatTebn()
{
    // Calculate the elapsed time since the transition started
    float elapsedTime = glfwGetTime() - SheepEatingStartTime;

    // Calculate the interpolation factor (0.0 to 1.0)
    float t = glm::clamp(elapsedTime / SheepEatingDuration, 0.0f, 1.0f);
    glm::vec3 PosSheep;
    if (sheepSide == 1)
    {
        PosSheep = sheepPos;
    }
    else
    {
        PosSheep = sheepPos2;
    }

    // Calculate the new position of the sheep
    glm::vec3 newPos = glm::mix(PosSheep, glm::vec3(Model[2][3]) + glm::vec3(0.0f, 0.0f, -2.0f), t);

    // Update the sheep's model matrix
    Model[1] = glm::translate(glm::mat4(1.0f), newPos);
    Model[1] = glm::rotate(Model[1], glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // rotate sheep around y axis
    Model[1] = glm::scale(Model[1], glm::vec3(1.6f, 1.6f, 1.6f));                       // scale sheep to make it bigger

    if (!isPlayingSheep)
    {
        // check if we are on the 80% of the transition
        if (t >= 0.85f)
        {
            // play the sound
            sheepEatSound.play();
            isPlayingSheep = true;
        }
    }

    // Check if the transition is over
    if (t >= 1.0f)
    {
        // Reset the transition flag
        SheepIsEating = false;
        // Reset the transition start time
        SheepEatingStartTime = 0.0f;
        // make the tebn disappear
        Model[2] = glm::translate(Model[2], glm::vec3(0.0f, -10.0f, 0.0f));

        // game over
        if (!GameOver && !WolfIsEating)
        {
            lostGame();
        }
    }
}

void wolfeatSheep()
{
    // Calculate the elapsed time since the transition started
    float elapsedTime = glfwGetTime() - WolfEatingStartTime;

    // Calculate the interpolation factor (0.0 to 1.0)
    float t = glm::clamp(elapsedTime / WolfEatingDuration, 0.0f, 1.0f);
    glm::vec3 PosWolf;

    // Calculate the new position of the wolf
    if (wolfSide == 1)
    {
        PosWolf = wolfPos;
    }
    else
    {
        PosWolf = wolfPos2;
    }
    glm::vec3 newPos = glm::mix(PosWolf, glm::vec3(Model[1][3]) + glm::vec3(0.0f, 0.0f, -2.0f), t);
    // Update the wolf's model matrix
    Model[0] = glm::translate(glm::mat4(1.0f), newPos);
    Model[0] = glm::rotate(Model[0], glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // rotate wolf around y axis
    Model[0] = glm::scale(Model[0], glm::vec3(0.8f, 0.8f, 0.8f));                       // scale wolf to make it smaller

    if (!isPlayingWolf)
    {
        // check if we are on the 80% of the transition
        if (t >= 0.75f)
        {
            // play the sound
            wolfEatSound.play();
            isPlayingWolf = true;
        }
    }

    // Check if the transition is over
    if (t >= 1.0f)
    {
        // Reset the transition flag
        WolfIsEating = false;
        isPlayingWolf = false;
        // Reset the transition start time
        WolfEatingStartTime = 0.0f;
        // make the sheep disappear
        Model[1] = glm::translate(Model[1], glm::vec3(0.0f, -10.0f, 0.0f));

        // game over
        if (!GameOver)
        {
            lostGame();
        }
    }
}

void sheepRunAwayFun()
{
    // Calculate the elapsed time since the transition started
    float elapsedTime = glfwGetTime() - sheepRunAwayStartTime;

    // Calculate the interpolation factor (0.0 to 1.0)
    float t = glm::clamp(elapsedTime / sheepRunAwayDuration, 0.0f, 1.0f);
    glm::vec3 PosSheep, PosRunAway;
    float angle = 90.0f;
    if (sheepSide == 1)
    {
        PosSheep = sheepPos;
        PosRunAway = runAwayPos;
        angle = -90.0f;
    }
    else
    {
        PosSheep = sheepPos2;
        PosRunAway = runAwayPos2;
    }

    // Calculate the new position of the sheep
    glm::vec3 newPos = glm::mix(PosSheep, PosRunAway, t);

    // Update the sheep's model matrix
    Model[1] = glm::translate(glm::mat4(1.0f), newPos);
    Model[1] = glm::rotate(Model[1], glm::radians(30.0f + angle), glm::vec3(0.0f, 1.0f, 0.0f)); // rotate sheep around y axis
    Model[1] = glm::scale(Model[1], glm::vec3(1.6f, 1.6f, 1.6f));                               // scale sheep to make it bigger

    // Check if the transition is over
    if (t >= 1.0f)
    {
        // Reset the transition flag
        sheepRunAway = false;
        // Reset the transition start time
        sheepRunAwayStartTime = 0.0f;
    }
}

void checkGameStatus()
{
    // check if he lost the game
    if (sheepSide == tebnSide && sheepSide != boatSide)
    {
        SheepIsEating = true;
        SheepEatingStartTime = glfwGetTime();
    }
    if (wolfSide == sheepSide && wolfSide != boatSide)
    {
        WolfIsEating = true;
        WolfEatingStartTime = glfwGetTime();
        if (!SheepIsEating)
        {
            sheepRunAway = true;
            sheepRunAwayStartTime = glfwGetTime();
            // play the sound
            sheepRunAwaySound.play();
        }
    }
    else if (wolfSide == -1 && sheepSide == -1 && tebnSide == -1)
    {
        if (!GameOver)
        {
            wonGame();
        }
    }
}

void initModels()
{
    // Set initial transformations for the model
    Model[0] = glm::translate(glm::mat4(1.0f), wolfPos);                                 // wolf
    Model[0] = glm::rotate(Model[0], glm::radians(120.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // glm::rotate wolf around y axis
    Model[0] = glm::scale(Model[0], glm::vec3(0.8f, 0.8f, 0.8f));                        // glm::scale wolf to make it smaller

    Model[1] = glm::translate(glm::mat4(1.0f), sheepPos);                                // sheep
    Model[1] = glm::rotate(Model[1], glm::radians(120.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // glm::rotate sheep around y axis
    Model[1] = glm::scale(Model[1], glm::vec3(1.6f, 1.6f, 1.6f));                        // glm::scale sheep to make it bigger

    Model[2] = glm::translate(glm::mat4(1.0f), tebnPos);                                // tben
    Model[2] = glm::rotate(Model[2], glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // glm::rotate tben around y axis

    Model[3] = glm::translate(glm::mat4(1.0f), farmerPos);                             // farmer
    Model[3] = glm::rotate(Model[3], glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // glm::rotate tben around y axis
    Model[3] = glm::scale(Model[3], glm::vec3(0.7f, 0.7f, 0.7f));                      // glm::scale farmer to make it smaller

    Model[4] = glm::translate(glm::mat4(1.0f), boatPos);                                // boat
    Model[4] = glm::rotate(Model[4], glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // glm::rotate tben around y axis

    Model[5] = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.5f, 0.0f));            // forest
    Model[5] = glm::rotate(Model[5], glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // glm::rotate tben around y axis
    Model[5] = glm::scale(Model[5], glm::vec3(15.0f, 15.0f, 15.0f));                     // glm::scale forest to make it bigger

    Model[6] = glm::translate(glm::mat4(1.0f), glm::vec3(20.0f, -15.0f, -50.0f));       // cloud
    Model[6] = glm::rotate(Model[6], glm::radians(80.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // glm::rotate tben around y axis
    Model[6] = glm::scale(Model[6], glm::vec3(8.0f, 8.0f, 8.0f));                       // glm::scale cloud to make it bigger

    Model[7] = glm::translate(glm::mat4(1.0f), glm::vec3(9.0f, -0.5f, 15.0f));          // campfire
    Model[7] = glm::rotate(Model[7], glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // glm::rotate tben around y axis
    Model[7] = glm::scale(Model[7], glm::vec3(1.5f, 1.5f, 1.5f));                       // glm::scale campfire to make it bigger
}

void drawtext_func()
{

    gltBeginDraw();

    gltColor(1.0f, 1.0f, 1.0f, 1.0f);
    GLTtext *text = gltCreateText();
    GLTtext *text2 = gltCreateText();
    GLTtext *text3 = gltCreateText();
    gltSetText(text, text_buffer);
    gltSetText(text2, text_buffer2);
    gltSetText(text3, text_buffer3);
    gltDrawText2DAligned(text,
                         960.0f,
                         540.0f,
                         5.0f,
                         GLT_CENTER, GLT_CENTER);

    gltDrawText2DAligned(text2,
                         960.0f,
                         630.0f,
                         3.0f,
                         GLT_CENTER, GLT_CENTER);
    
    gltDrawText2DAligned(text3,
                         960.0f,
                         50.0f,
                         3.0f,
                         GLT_CENTER, GLT_CENTER);

    gltEndDraw();
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS)
        switch (key)
        {
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, GLFW_TRUE);
            break;

        case GLFW_KEY_Z:
            if (!isTransitioning)
            {
                // check if the boat is on the same side of the wolf
                if (boatSide == wolfSide)
                {
                    // check if the boat is empty
                    if (boatEmpty)
                    {
                        // make the wolf coordinates the same as the boat coordinates
                        if (boatSide == 1)
                        {
                            Model[0] = glm::translate(glm::mat4(1.0f), boatPos + glm::vec3(-1.0f, 0.0f, 0.0f));
                        }
                        else
                        {
                            Model[0] = glm::translate(glm::mat4(1.0f), boatPos2 + glm::vec3(-1.0f, 0.0f, 0.0f));
                        }
                        Model[0] = glm::rotate(Model[0], glm::radians(120.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // glm::rotate wolf around y axis
                        Model[0] = glm::scale(Model[0], glm::vec3(0.8f, 0.8f, 0.8f));
                        // make the boat not empty
                        boatEmpty = false;
                        // change the onBoat variable to w
                        onBoat = 'w';
                    }
                    else if (onBoat == 'w')
                    {
                        if (boatSide == 1)
                        {
                            // make the wolf coordinates the same as the first side of the river coordinates
                            Model[0] = glm::translate(glm::mat4(1.0f), wolfPos);
                        }
                        else
                        {
                            // make the wolf coordinates the same as the second side of the river coordinates
                            Model[0] = glm::translate(glm::mat4(1.0f), wolfPos2);
                        }
                        Model[0] = glm::rotate(Model[0], glm::radians(120.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // glm::rotate wolf around y axis
                        Model[0] = glm::scale(Model[0], glm::vec3(0.8f, 0.8f, 0.8f));
                        // make the boat empty
                        boatEmpty = true;
                        // change the onBoat variable to ' '
                        onBoat = ' ';
                    }
                }
            }
            break;

        case GLFW_KEY_K:
            if (!isTransitioning)
            {
                // check if the boat is on the same side of the sheep
                if (boatSide == sheepSide)
                {
                    // check if the boat is empty
                    if (boatEmpty)
                    {
                        // make the sheep coordinates the same as the boat coordinates
                        if (boatSide == 1)
                        {
                            Model[1] = glm::translate(glm::mat4(1.0f), boatPos + glm::vec3(-1.0f, 0.0f, 0.0f));
                        }
                        else
                        {
                            Model[1] = glm::translate(glm::mat4(1.0f), boatPos2 + glm::vec3(-1.0f, 0.0f, 0.0f));
                        }
                        Model[1] = glm::rotate(Model[1], glm::radians(120.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // glm::rotate sheep around y axis
                        Model[1] = glm::scale(Model[1], glm::vec3(1.6f, 1.6f, 1.6f));                        // glm::scale sheep to make it bigger
                        // make the boat not empty
                        boatEmpty = false;
                        // change the onBoat variable to k
                        onBoat = 'k';
                    }
                    else if (onBoat == 'k')
                    {
                        if (boatSide == 1)
                        {
                            // make the sheep coordinates the same as the first side of the river coordinates
                            Model[1] = glm::translate(glm::mat4(1.0f), sheepPos);
                        }
                        else
                        {
                            // make the sheep coordinates the same as the second side of the river coordinates
                            Model[1] = glm::translate(glm::mat4(1.0f), sheepPos2);
                        }
                        Model[1] = glm::rotate(Model[1], glm::radians(120.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // glm::rotate sheep around y axis
                        Model[1] = glm::scale(Model[1], glm::vec3(1.6f, 1.6f, 1.6f));                        // glm::scale sheep to make it bigger
                        // make the boat empty
                        boatEmpty = true;
                        // change the onBoat variable to ' '
                        onBoat = ' ';
                    }
                }
            }
            break;

        case GLFW_KEY_T:
            if (!isTransitioning)
            {
                // check if the boat is on the same side of the tebn
                if (boatSide == tebnSide)
                {
                    // check if the boat is empty
                    if (boatEmpty)
                    {
                        // make the tebn coordinates the same as the boat coordinates
                        if (boatSide == 1)
                        {
                            Model[2] = glm::translate(glm::mat4(1.0f), boatPos + glm::vec3(-1.0f, 0.0f, 0.0f));
                        }
                        else
                        {
                            Model[2] = glm::translate(glm::mat4(1.0f), boatPos2 + glm::vec3(-1.0f, 0.0f, 0.0f));
                        }
                        Model[2] = glm::rotate(Model[2], glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // glm::rotate tben around y axis
                        // make the boat not empty
                        boatEmpty = false;
                        // change the onBoat variable to t
                        onBoat = 't';
                    }
                    else if (onBoat == 't')
                    {
                        if (boatSide == 1)
                        {
                            // make the tebn coordinates the same as the first side of the river coordinates
                            Model[2] = glm::translate(glm::mat4(1.0f), tebnPos);
                        }
                        else
                        {
                            // make the tebn coordinates the same as the second side of the river coordinates
                            Model[2] = glm::translate(glm::mat4(1.0f), tebnPos2);
                        }
                        Model[2] = glm::rotate(Model[2], glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // glm::rotate tben around y axis
                        // make the boat empty
                        boatEmpty = true;
                        // change the onBoat variable to ' '
                        onBoat = ' ';
                    }
                }
            }
            break;

        case GLFW_KEY_SPACE:
            if (!isTransitioning)
            {
                isTransitioning = true;
                transitionStartTime = glfwGetTime();
            }
            break;
        }
}
void translatingFunction()
{
    // Calculate the elapsed time since the transition started
    float elapsedTime = glfwGetTime() - transitionStartTime;

    // Calculate the interpolation factor (0.0 to 1.0)
    float t = glm::clamp(elapsedTime / transitionDuration, 0.0f, 1.0f);

    if (boatSide == 1)
    {
        // Calculate the new position of the boat
        glm::vec3 newPos = glm::mix(boatPos, boatPos2, t);
        // Update the boat's model matrix
        Model[4] = glm::translate(glm::mat4(1.0f), newPos);
        Model[4] = glm::rotate(Model[4], glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // glm::rotate tben around y axis

        // Update the farmer's model matrix
        Model[3] = glm::translate(glm::mat4(1.0f), newPos + glm::vec3(1.0f, 0.0f, 0.0f));
        Model[3] = glm::scale(Model[3], glm::vec3(0.7f, 0.7f, 0.7f)); // glm::scale farmer to make it smaller

        switch (onBoat)
        {
        case 'w':
            // Update the wolf's model matrix
            Model[0] = glm::translate(glm::mat4(1.0f), newPos + glm::vec3(-1.0f, 0.0f, 0.0f));
            Model[0] = glm::rotate(Model[0], glm::radians(120.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // glm::rotate wolf around y axis
            Model[0] = glm::scale(Model[0], glm::vec3(0.8f, 0.8f, 0.8f));                        // glm::scale wolf to make it smaller
            break;
        case 'k':
            // Update the sheep's model matrix
            Model[1] = glm::translate(glm::mat4(1.0f), newPos + glm::vec3(-1.0f, 0.0f, 0.0f));
            Model[1] = glm::rotate(Model[1], glm::radians(120.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // glm::rotate sheep around y axis
            Model[1] = glm::scale(Model[1], glm::vec3(1.6f, 1.6f, 1.6f));                        // glm::scale sheep to make it bigger
            break;
        case 't':
            // Update the tebn's model matrix
            Model[2] = glm::translate(glm::mat4(1.0f), newPos + glm::vec3(-1.0f, 0.0f, 0.0f));
            Model[2] = glm::rotate(Model[2], glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // glm::rotate tben around y axis
            break;
        }
    }
    else if (boatSide == -1)
    {
        // Calculate the new position of the boat
        glm::vec3 newPos = glm::mix(boatPos2, boatPos, t);
        // Update the boat's model matrix
        Model[4] = glm::translate(glm::mat4(1.0f), newPos);
        Model[4] = glm::rotate(Model[4], glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));  // glm::rotate tben around y axis
        Model[4] = glm::rotate(Model[4], glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // glm::rotate tben around y axis
        // Update the farmer's model matrix
        Model[3] = glm::translate(glm::mat4(1.0f), newPos + glm::vec3(1.0f, 0.0f, 0.0f));
        Model[3] = glm::scale(Model[3], glm::vec3(0.7f, 0.7f, 0.7f));                        // glm::scale farmer to make it smaller
        Model[3] = glm::rotate(Model[3], glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // glm::rotate tben around y axis
        switch (onBoat)
        {
        case 'w':
            // Update the wolf's model matrix
            Model[0] = glm::translate(glm::mat4(1.0f), newPos + glm::vec3(-1.0f, 0.0f, 0.0f));
            Model[0] = glm::rotate(Model[0], glm::radians(120.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // glm::rotate wolf around y axis
            Model[0] = glm::scale(Model[0], glm::vec3(0.8f, 0.8f, 0.8f));                        // glm::scale wolf to make it smaller
            break;
        case 'k':
            // Update the sheep's model matrix
            Model[1] = glm::translate(glm::mat4(1.0f), newPos + glm::vec3(-1.0f, 0.0f, 0.0f));
            Model[1] = glm::rotate(Model[1], glm::radians(120.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // glm::rotate sheep around y axis
            Model[1] = glm::scale(Model[1], glm::vec3(1.6f, 1.6f, 1.6f));                        // glm::scale sheep to make it bigger
            break;
        case 't':
            // Update the tebn's model matrix
            Model[2] = glm::translate(glm::mat4(1.0f), newPos + glm::vec3(-1.0f, 0.0f, 0.0f));
            Model[2] = glm::rotate(Model[2], glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // glm::rotate tben around y axis
            break;
        }
    }

    // Check if the transition is over
    if (t >= 1.0f)
    {
        // Reset the transition flag
        isTransitioning = false;
        // Reset the transition start time
        transitionStartTime = 0.0f;
        // Change the boat side
        boatSide *= -1;
        // glm::rotate the boat and the farmer to the other side
        Model[4] = glm::rotate(Model[4], glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // glm::rotate tben around y axis
        Model[3] = glm::rotate(Model[3], glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // glm::rotate tben around y axis
        switch (onBoat)
        {
        case 'w':
            wolfSide *= -1;
            // glm::rotate the wolf to the other side
            Model[0] = glm::rotate(Model[0], glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // glm::rotate wolf around y axis
            break;
        case 'k':
            sheepSide *= -1;
            // glm::rotate the sheep to the other side
            Model[1] = glm::rotate(Model[1], glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // glm::rotate sheep around y axis
            break;
        case 't':
            tebnSide *= -1;
            // glm::rotate the tebn to the other side
            Model[2] = glm::rotate(Model[2], glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // glm::rotate tben around y axis
            break;
        }
        // check the game status
        checkGameStatus();
    }
}

glm::vec3 cameraPos = glm::vec3(-46.0f, 13.0f, 26.0f);
glm::vec3 cameraFront = glm::vec3(-4.0f, -1.0f, -2.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

void processInput(GLFWwindow *window)
{
    const float cameraSpeed = 0.05f;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

float yaw = -90.0f;   // Initial yaw angle
float pitch = 0.0f;   // Initial pitch angle
float lastX = 400.0f; // Initial mouse X position
float lastY = 300.0f; // Initial mouse Y position
bool firstMouse = true;

// Function to handle mouse input
void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // Reversed since y-coordinates go from bottom to top

    const float sensitivity = 0.05f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // Constrain pitch to avoid gimbal lock
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);

    lastX = xpos;
    lastY = ypos;
}

void InitTexture(void)
{

    for (int i = 0; i < nObjects; ++i)
    {
        int width, height, channels;
        unsigned char *image = stbi_load(filenamesTextures[i], &width, &height, &channels, STBI_rgb);

        // Generate texture
        glGenTextures(1, &tab_texture[i]);
        glBindTexture(GL_TEXTURE_2D, tab_texture[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        // Free the image data
        stbi_image_free(image);
    }
}

// Function to load model with Assimp
void loadModel(const std::string &path, std::vector<std::vector<Vertex>> &meshVertices,
               std::vector<std::vector<unsigned int>> &meshIndices, std::vector<Material> &meshMaterials)
{
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(
        path,
        aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices | aiProcess_ForceGenNormals);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cerr << "Error loading model: " << importer.GetErrorString() << std::endl;
        return;
    }

    // Iterate over all meshes in the model
    for (unsigned int meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex)
    {
        aiMesh *mesh = scene->mMeshes[meshIndex];

        // Material information
        Material material;
        if (scene->HasMaterials())
        {
            aiMaterial *aiMat = scene->mMaterials[mesh->mMaterialIndex];

            aiColor3D ambient, diffuse, specular;
            aiMat->Get(AI_MATKEY_COLOR_AMBIENT, ambient);
            aiMat->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
            aiMat->Get(AI_MATKEY_COLOR_SPECULAR, specular);

            material.ambient = glm::vec3(ambient.r, ambient.g, ambient.b);
            material.diffuse = glm::vec3(diffuse.r, diffuse.g, diffuse.b);
            material.specular = glm::vec3(specular.r, specular.g, specular.b);

            aiMat->Get(AI_MATKEY_SHININESS, material.shininess);
        }

        // Containers for the vertices, indices, and materials of the current mesh
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        // Iterate over vertices in the mesh
        for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
        {
            Vertex vertex;
            vertex.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
            vertex.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
            if (mesh->mTextureCoords[0])
            {
                vertex.texCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
            }
            else
            {
                vertex.texCoords = glm::vec2(0.0f, 0.0f);
            }
            vertex.color = material.diffuse; // Use diffuse color as vertex color
            vertices.push_back(vertex);
        }

        // Iterate over faces in the mesh
        for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
        {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; ++j)
            {
                indices.push_back(face.mIndices[j]);
            }
        }

        // Store the vertices, indices, and material for the current mesh
        meshVertices.push_back(vertices);
        meshIndices.push_back(indices);
        meshMaterials.push_back(material);
    }
}

// Function to initialize VBO and VAO
void setupBuffers(const std::vector<std::vector<Vertex>> &meshVertices,
                  const std::vector<std::vector<unsigned int>> &meshIndices,
                  std::vector<GLuint> &VAOs, std::vector<GLuint> &VBOs, std::vector<GLuint> &EBOs)
{
    int numMeshes = meshVertices.size();

    VAOs.resize(numMeshes);
    VBOs.resize(numMeshes);
    EBOs.resize(numMeshes);

    for (int i = 0; i < numMeshes; ++i)
    {
        glGenVertexArrays(1, &VAOs[i]);
        glGenBuffers(1, &VBOs[i]);
        glGenBuffers(1, &EBOs[i]);

        glBindVertexArray(VAOs[i]);

        glBindBuffer(GL_ARRAY_BUFFER, VBOs[i]);
        glBufferData(GL_ARRAY_BUFFER, meshVertices[i].size() * sizeof(Vertex), meshVertices[i].data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[i]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshIndices[i].size() * sizeof(unsigned int), meshIndices[i].data(), GL_STATIC_DRAW);

        // Vertex positions
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
        glEnableVertexAttribArray(0);

        // Vertex normals
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normal));
        glEnableVertexAttribArray(1);

        // Texture coordinates
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, texCoords));
        glEnableVertexAttribArray(2);

        // Vertex color (new attribute)
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, color));
        glEnableVertexAttribArray(3);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
}

// Function to compile and link shaders
void createShaderProgram(const char *vertexShaderSource, const char *fragmentShaderSource)
{
    // Vertex Shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    // Check for vertex shader compilation errors
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cerr << "Error compiling vertex shader:\n"
                  << infoLog << std::endl;
    }

    // Fragment Shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    // Check for fragment shader compilation errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cerr << "Error compiling fragment shader:\n"
                  << infoLog << std::endl;
    }

    // Shader Program
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Check for shader program linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cerr << "Error linking shader program:\n"
                  << infoLog << std::endl;
    }

    // Delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}
// Load 3D model with materials and colors
std::vector<std::vector<Vertex>> meshVertices[nObjects];
std::vector<std::vector<unsigned int>> meshIndices[nObjects];
std::vector<Material> meshMaterials[nObjects];

// Set up VBO and VAO for multiple meshes
std::vector<GLuint> VAOs[nObjects];
std::vector<GLuint> VBOs[nObjects];
std::vector<GLuint> EBOs[nObjects];

GLuint TextureID;
void renderingScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    processInput(window); // Process user input

    // Set shader program
    glUseProgram(shaderProgram);

    // Set transformation matrices
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 projection = glm::perspective(glm::radians(90.0f), 800.0f / 600.0f, 0.1f, 1000.0f);
    glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    // Set material properties
    glUniform3f(glGetUniformLocation(shaderProgram, "material.ambient"), 1.0f, 0.5f, 0.31f);
    glUniform3f(glGetUniformLocation(shaderProgram, "material.diffuse"), 1.0f, 0.5f, 0.31f);
    glUniform3f(glGetUniformLocation(shaderProgram, "material.specular"), 0.5f, 0.5f, 0.5f);
    glUniform1f(glGetUniformLocation(shaderProgram, "material.shininess"), 32.0f);

    // Set light properties
    glUniform3f(glGetUniformLocation(shaderProgram, "light.position"), 0.0f, 5.0f, 2.0f);
    // Set light properties
    glUniform3f(glGetUniformLocation(shaderProgram, "light.ambient"), 1.0f, 1.0f, 1.0f);  // Increase ambient light
    glUniform3f(glGetUniformLocation(shaderProgram, "light.diffuse"), 1.0f, 1.0f, 1.0f);  // Increase diffuse light
    glUniform3f(glGetUniformLocation(shaderProgram, "light.specular"), 1.0f, 1.0f, 1.0f); // Increase specular light

    // Set view position
    glUniform3f(glGetUniformLocation(shaderProgram, "viewPos"), 0.0f, 0.0f, 3.0f);

    // Render each model with its respective texture
    int useTex = 1;
    for (int i = 0; i < nObjects; ++i)
    {
        if (i == 5)
            useTex = 0;
        else
            useTex = 1;
        // Calculate Model-View-Projection matrix
        // Send MVP matrix to the shader
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(Model[i]));
        glUniform1i(glGetUniformLocation(shaderProgram, "useTexture"), useTex);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tab_texture[i]);
        glUniform1i(TextureID, 0);

        // Iterate over meshes and render each one
        for (int j = 0; j < meshVertices[i].size(); ++j)
        {
            glBindVertexArray(VAOs[i][j]);

            // Set transformation matrices, material properties, light properties, etc.

            Material &material = meshMaterials[i][j];

            // glUniform3fv(glGetUniformLocation(shaderProgram, "material.ambient"), 1, glm::value_ptr(material.ambient));
            // glUniform3fv(glGetUniformLocation(shaderProgram, "material.diffuse"), 1, glm::value_ptr(material.diffuse));
            // glUniform3fv(glGetUniformLocation(shaderProgram, "material.specular"), 1, glm::value_ptr(material.specular));
            // glUniform1f(glGetUniformLocation(shaderProgram, "material.shininess"), material.shininess);

            // Render the current mesh
            glDrawElements(GL_TRIANGLES, meshIndices[i][j].size(), GL_UNSIGNED_INT, 0);

            glBindVertexArray(0);
        }
    }

    GLenum error = glGetError();
    if (error != GL_NO_ERROR)
    {
        // Print or log the OpenGL error code
        std::cerr << "OpenGL Error: " << error << std::endl;
    }


    if (isTransitioning)
    {
        translatingFunction();
    }
    if (SheepIsEating)
    {
        sheepEatTebn();
    }
    if (WolfIsEating)
    {
        wolfeatSheep();
    }
    if (sheepRunAway)
    {
        sheepRunAwayFun();
    }
    if (drawText)
    {
        drawtext_func();
    }
}
void while_func()
{
    while (!glfwWindowShouldClose(window))
    {
        renderingScene();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void Reshape(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

int main()
{
    // Initialize GLFW and create a window

    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // GLFW window setup code goes here

    window = glfwCreateWindow(1920, 1080, "River crossing Puzzle", nullptr, nullptr);
    glfwSetFramebufferSizeCallback(window, Reshape);

    // set an icon for the window
    GLFWimage images[1];
    images[0].pixels = stbi_load("/home/rykerdz/Desktop/M2/sim/tp_1/textures/icon.png", &images[0].width, &images[0].height, 0, 4); // rgba channels
    glfwSetWindowIcon(window, 1, images);
    stbi_image_free(images[0].pixels);

    if (!window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // load the sounds
    wolfEatBuffer.loadFromFile("sounds/wolf_eat_sound.wav");
    wolfEatSound.setBuffer(wolfEatBuffer);

    sheepEatBuffer.loadFromFile("sounds/sheep_eat_tben.wav");
    sheepEatSound.setBuffer(sheepEatBuffer);

    sheepRunAwayBuffer.loadFromFile("sounds/goat_scream.wav");
    sheepRunAwaySound.setBuffer(sheepRunAwayBuffer);

    for (int i = 0; i < nObjects; ++i)
    {

        loadModel(filenamesModels[i], meshVertices[i], meshIndices[i], meshMaterials[i]);
        setupBuffers(meshVertices[i], meshIndices[i], VAOs[i], VBOs[i], EBOs[i]);
    }

    initModels();

    // Vertex and fragment shader source code
    const char *vertexShaderSource = R"(
        #version 330 core

        layout(location = 0) in vec3 aPos;
        layout(location = 1) in vec3 aNormal;
        layout(location = 2) in vec2 aTexCoords;
        layout(location = 3) in vec3 aColor;  // New attribute for color

        out vec3 FragPos;
        out vec3 Normal;
        out vec2 TexCoords;
        out vec3 Color;  // New output for color

        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;

        void main() {
            gl_Position = projection * view * model * vec4(aPos, 1.0);
            FragPos = vec3(model * vec4(aPos, 1.0));
            Normal = mat3(transpose(inverse(model))) * aNormal;
            TexCoords = aTexCoords;
            Color = aColor;  // Pass color to fragment shader
        }
    )";

    const char *fragmentShaderSource = R"(
        #version 330 core

        out vec4 FragColor;

        struct Material {
            vec3 ambient;
            vec3 diffuse;
            vec3 specular;
            float shininess;
        };

        struct Light {
            vec3 position;
            vec3 ambient;
            vec3 diffuse;
            vec3 specular;
        };

        uniform Material material;
        uniform Light light;
        uniform vec3 viewPos;
        uniform sampler2D textureSampler;  // Add this uniform
        uniform bool useTexture;

        in vec3 FragPos;
        in vec3 Normal;
        in vec2 TexCoords;
        in vec3 Color;  // New input for color


        void main() {
            // Ambient
            vec4 texColor = useTexture ? texture(textureSampler, TexCoords) : vec4(Color, 1.0f);
            vec3 ambient = light.ambient * material.ambient * texColor.rgb;

            // Diffuse
            vec3 norm = normalize(Normal);
            vec3 lightDir = normalize(light.position - FragPos);
            float diff = max(dot(norm, lightDir), 0.0);
            vec3 diffuse = light.diffuse * (diff * material.diffuse * texColor.rbg);  // Consider material color

            // Specular
            vec3 viewDir = normalize(viewPos - FragPos);
            vec3 reflectDir = reflect(-lightDir, norm);
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
            vec3 specular = light.specular * (spec * material.specular);


            vec3 result = ambient + diffuse + specular;
            FragColor = vec4(result, texColor.a);

        }
    )";

    // Create shader program
    createShaderProgram(vertexShaderSource, fragmentShaderSource);

    // Bind texture to the shader
    glUseProgram(shaderProgram);
    InitTexture();
    TextureID = glGetUniformLocation(shaderProgram, "ourTexture");

    // Set up mouse callback
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glEnable(GL_DEPTH_TEST);
    // glDepthFunc(GL_LESS);
    glClearColor(0.6f, 0.6f, 0.9f, 1.0f);

    if (!gltInit())
    {
        fprintf(stderr, "Failed to initialize glText\n");
        glfwTerminate();
        return EXIT_FAILURE;
    }

    startGame();
    // Main rendering loop
    while_func();

    // Cleanup code
    for (int i = 0; i < nObjects; ++i)
    {
        for (GLuint VAO : VAOs[i])
        {
            glDeleteVertexArrays(1, &VAO);
        }

        for (GLuint VBO : VBOs[i])
        {
            glDeleteBuffers(1, &VBO);
        }

        for (GLuint EBO : EBOs[i])
        {
            glDeleteBuffers(1, &EBO);
        }
    }
    glDeleteProgram(shaderProgram);

    glfwTerminate();

    return 0;
}
