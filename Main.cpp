//This assignment's resources are the labs provided in the course. Learnopengl.com the cherno youtube channel and sometimes stackovoverflow
#include <iostream>
#include <vector>

#define GLEW_STATIC 1   // This allows linking with Static Library on Windows, without DLL
#include <GL/glew.h>    // Include GLEW - OpenGL Extension Wrangler

#include <GLFW/glfw3.h> // cross-platform interface for creating a graphical context,
// initializing OpenGL and binding inputs

#include <glm/glm.hpp>  // GLM is an optimized math library with syntax to similar to OpenGL Shading Language
#include <glm/gtc/matrix_transform.hpp> // include this to create transformation matrices
#include "fragShaders.h"
#include "vertexShaders.h"

using namespace glm;// we do this to call glm functions without using full fucntion name
using namespace std;// we dp this to call std functions without using full fucntion name

void keyCall(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouseButtons(GLFWwindow* window, int button, int action, int mods);


//frag shaders are now provided in a separate file fragShaders.h/fragShaders.cpp



mat4 translationMatrix;
mat4 rotationMatrix;
mat4 scaleMatrix;
mat4 modelMatrix;
mat4 handModelMatrix;
mat4 handTranslation;
mat4 handRotation;
mat4 handScalar;
GLint modelMatrixLocation;

// the following 3 variables hold the values of the scaleMatrix to facilitate modifications later on
float scaleX = 0.5f, scaleY = 0.5f, scaleZ = 0.5f;

// the following 3 variables are for the location of the model on the grid
float locationX = 0.0f, locationY = 0.0f, locationZ = 0.0f;

// the following is the current angle of the hand
float angleOfAllArm = radians(0.0f);


// Camera parameters for view transform
vec3 cameraPosition(0.0f, 15.0f, 15.0f);
vec3 objectPosition = vec3(0.0f, 0.0f, 0.0f); // Position of the object you want to rotate around
vec3 cameraUp(0.0f, 1.0f, 0.0f);

// Other camera parameters
float cameraSpeed = 1.0f;
float cameraHorizontalAngle = radians(90.0f);
float cameraVerticalAngle = radians(30.0f);
float cameraDistance = length(cameraPosition - objectPosition); // Distance of the camera from the object

// this part is for rendering type: triangles, lines, points
bool isTriangles = true;
bool isLines = false;
bool isPoints = false;

bool leftMouseButtonPressed = false;
bool rightMouseButtonPressed = false;
bool middleMouseButtonPressed = false;
double lastMouseX = 0.0;
double lastMouseY = 0.0;

int main(int argc, char* argv[])
{
    // Initialize GLFW and OpenGL version
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);// this tells opengl that the version is 4.1
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Create Window and rendering context using GLFW, resolution is 1024 768
    GLFWwindow* window = glfwCreateWindow(1024, 768, "Comp371 - Assignment1", NULL, NULL);
    if (window == NULL)
    {
        cerr << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        cerr << "Failed to create GLEW" << endl;
        glfwTerminate();
        return -1;
    }

    glClearColor(51 / 255.0f, 76 / 255.0f, 76 / 255.0f, 1.0f); //slate dark gray for assignment background

    int shaderProgram = compileAndLinkShaders();

    glUseProgram(shaderProgram);
    GLuint colorLocation = glGetUniformLocation(shaderProgram, "color");


    // Set projection matrix for shader, this won't change
    mat4 projectionMatrix = perspective(70.0f,            // field of view in degrees
        800.0f / 600.0f,  // aspect ratio
        0.01f, 100.0f);   // near and far (near > 0)

    GLuint projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");
    glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);


    // Define and upload geometry to the GPU here ...
    int vao = createGrid();
    int X = createX();
    int Y = createY();
    int Z = createZ();
    int cube = createCubeCoordinate();

    // Other OpenGL states to set once
    // Enable Backface culling
    glEnable(GL_CULL_FACE);

    glEnable(GL_DEPTH_TEST);

    // Entering Main Loop
    float originalLineWidth = 1.0f;

    while (!glfwWindowShouldClose(window))
    {


        // Calculate the new camera position based on the camera angles
        float cameraX = objectPosition.x + cameraDistance * sin(cameraHorizontalAngle) * cos(cameraVerticalAngle);
        float cameraY = objectPosition.y + cameraDistance * sin(cameraVerticalAngle);
        float cameraZ = objectPosition.z + cameraDistance * cos(cameraHorizontalAngle) * cos(cameraVerticalAngle);
        cameraPosition = vec3(cameraX, cameraY, cameraZ);

        // Set initial view matrix
        mat4 viewMatrix = lookAt(cameraPosition,  // eye
            objectPosition,  // center
            cameraUp); // up

        GLuint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
        glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLineWidth(6.5f);//sets line width 
        glBindVertexArray(X);//draw xyz rgb axis
        vec3 instanceColor(1.0f, 0.0f, 0.0f);
        glUniform3fv(colorLocation, 1, &instanceColor[0]);
        glDrawArrays(GL_LINES, 0, 2);
        glBindVertexArray(Y);//draw xyz rgb axis
        instanceColor = vec3(0.0f, 1.0f, 0.0f);
        glUniform3fv(colorLocation, 1, &instanceColor[0]);
        glDrawArrays(GL_LINES, 0, 2);
        glBindVertexArray(Z);//draw xyz rgb axis
        instanceColor = vec3(0.0f, 0.0f, 1.0f);
        glUniform3fv(colorLocation, 1, &instanceColor[0]);
        glDrawArrays(GL_LINES, 0, 2);

        rotationMatrix = rotate(mat4(1.0f), radians(0.0f), vec3(0.0f, 0.0f, 1.0f));
        translationMatrix = translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.0f));
        scaleMatrix = scale(mat4(1.0f), vec3(0.0f, 0.0f, 0.0f));
        modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;     //base matrix , degrees of rotation changed to radians, then which axis you want to rotate around.


        handTranslation = translate(mat4(1.0f), vec3(locationX, locationY, locationZ));
        handRotation = rotate(mat4(1.0f), angleOfAllArm, vec3(0.0f, 0.0f, 1.0f));
        handScalar = scale(mat4(1.0f), vec3(scaleX, scaleY, scaleZ));


        handModelMatrix = handTranslation * handRotation * handScalar;//moves the arm so in turn moves the whole upper arm and racket.


        modelMatrixLocation = glGetUniformLocation(shaderProgram, "modelMatrix");//model matrix location
        glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);


        GLuint projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");//projection matrix location
        glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);


        glLineWidth(originalLineWidth);//resets line width to original value

        glBindVertexArray(cube);// CUBE BASE

        translationMatrix = translate(mat4(1.0f), vec3(3.0f, 3.0f, -1.0f));
        rotationMatrix = rotate(mat4(1.0f), radians(-45.0f), vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix = scale(mat4(1.0f), vec3(1.0f, 2.0f, 1.0f));
        modelMatrix = handModelMatrix * (translationMatrix * rotationMatrix * scaleMatrix);
        glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);
        //whole hand movement


        // Set the keyboard key and mouse callback functions
        glfwSetKeyCallback(window, keyCall);
        glfwSetMouseButtonCallback(window, mouseButtons);

        instanceColor = vec3(0.6f, 0.5176f, 0.4039f);
        glUniform3fv(colorLocation, 1, &instanceColor[0]);
        // DRAWING LOWER ARM
        if (isTriangles)
            glDrawArrays(GL_TRIANGLES, 0, 36);
        else if (isLines)
            glDrawArrays(GL_LINES, 0, 36);
        else if (isPoints)
            glDrawArrays(GL_POINTS, 0, 36);

        rotationMatrix = rotate(mat4(1.0f), radians(0.0f), vec3(0.0f, 0.0f, 1.0f));
        translationMatrix = translate(mat4(1.0f), vec3(4.33f, 6.29f, -1.0f));
        modelMatrix = handModelMatrix * (translationMatrix * rotationMatrix * scaleMatrix);
        glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);


        // DRAWING UPPER ARM
        if (isTriangles)
            glDrawArrays(GL_TRIANGLES, 0, 36);
        else if (isLines)
            glDrawArrays(GL_LINES, 0, 36);
        else if (isPoints)
            glDrawArrays(GL_POINTS, 0, 36);

        rotationMatrix = rotate(mat4(1.0f), radians(45.0f), vec3(0.0f, 0.0f, 1.0f));
        translationMatrix = translate(mat4(1.0f), vec3(4.33f, 6.29f * 1.34, -1.0f));
        scaleMatrix = scale(mat4(1.0f), vec3(3.0f, 0.5f, 1.0f));
        modelMatrix = handModelMatrix * (translationMatrix * rotationMatrix * scaleMatrix);
        glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);
        // DRAWING PALM
        if (isTriangles)
            glDrawArrays(GL_TRIANGLES, 0, 36);
        else if (isLines)
            glDrawArrays(GL_LINES, 0, 36);
        else if (isPoints)
            glDrawArrays(GL_POINTS, 0, 36);

        //drawing Fingers
        for (int i = 0; i < 5; i++) {
            float angle = 45.0f;
            float translation = i * 0.29;


            rotationMatrix = rotate(mat4(1.0f), radians(angle), vec3(0.0f, 0.0f, 1.0f));
            translationMatrix = translate(mat4(1.0f), vec3(3.4f + translation, (6.1f * 1.4) + translation, -1.0f));
            if (i == 4) {

                rotationMatrix = rotate(mat4(1.0f), radians(-45.0f), vec3(0.0f, 0.0f, 1.0f));
                translationMatrix = translate(mat4(1.0f), vec3(5.1f, (6.1f * 1.5), -1.0f));
            }
            scaleMatrix = scale(mat4(1.0f), vec3(0.50f, 0.5f, 0.5f));
            modelMatrix = handModelMatrix * (translationMatrix * rotationMatrix * scaleMatrix);
            glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);
            // DRAWING Fingers
            if (isTriangles)
                glDrawArrays(GL_TRIANGLES, 0, 36);
            else if (isLines)
                glDrawArrays(GL_LINES, 0, 36);
            else if (isPoints)
                glDrawArrays(GL_POINTS, 0, 36);
        }

        // DRAWING Racket handle
        rotationMatrix = rotate(mat4(1.0f), radians(0.0f), vec3(0.0f, 0.0f, 1.0f));
        translationMatrix = translate(mat4(1.0f), vec3(4.7f, 6.7f * 1.5, -1.0f));
        scaleMatrix = scale(mat4(1.0f), vec3(0.5f, 1.0f, 1.0f));
        modelMatrix = handModelMatrix * (translationMatrix * rotationMatrix * scaleMatrix);
        instanceColor = vec3(1.0f, 0.0f, 0.0f);
        glUniform3fv(colorLocation, 1, &instanceColor[0]);
        glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);
        if (isTriangles)
            glDrawArrays(GL_TRIANGLES, 0, 36);
        else if (isLines)
            glDrawArrays(GL_LINES, 0, 36);
        else if (isPoints)
            glDrawArrays(GL_POINTS, 0, 36);

        // DRAWING right base of racket
        rotationMatrix = rotate(mat4(1.0f), radians(-45.0f), vec3(0.0f, 0.0f, 1.0f));
        translationMatrix = translate(mat4(1.0f), vec3(5.0f, 6.7f * 1.7, -1.0f));
        scaleMatrix = scale(mat4(1.0f), vec3(0.5f, 0.5f, 1.0f));
        modelMatrix = handModelMatrix * (translationMatrix * rotationMatrix * scaleMatrix);
        instanceColor = vec3(1.0f, 1.0f, 1.0f);
        glUniform3fv(colorLocation, 1, &instanceColor[0]);
        glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);
        if (isTriangles)
            glDrawArrays(GL_TRIANGLES, 0, 36);
        else if (isLines)
            glDrawArrays(GL_LINES, 0, 36);
        else if (isPoints)
            glDrawArrays(GL_POINTS, 0, 36);

        // DRAWING top right of racket
        rotationMatrix = rotate(mat4(1.0f), radians(45.0f), vec3(0.0f, 0.0f, 1.0f));
        translationMatrix = translate(mat4(1.0f), vec3(4.95f, 6.7f * 2.08, -1.0f));
        scaleMatrix = scale(mat4(1.0f), vec3(0.5f, 0.55f, 1.0f));
        modelMatrix = handModelMatrix * (translationMatrix * rotationMatrix * scaleMatrix);
        instanceColor = vec3(1.0f, 1.0f, 1.0f);
        glUniform3fv(colorLocation, 1, &instanceColor[0]);
        glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);
        if (isTriangles)
            glDrawArrays(GL_TRIANGLES, 0, 36);
        else if (isLines)
            glDrawArrays(GL_LINES, 0, 36);
        else if (isPoints)
            glDrawArrays(GL_POINTS, 0, 36);

        // DRAWING left base of racket
        rotationMatrix = rotate(mat4(1.0f), radians(45.0f), vec3(0.0f, 0.0f, 1.0f));
        translationMatrix = translate(mat4(1.0f), vec3(4.3f, 6.7f * 1.7, -1.0f));
        scaleMatrix = scale(mat4(1.0f), vec3(0.5f, 0.5f, 1.0f));
        modelMatrix = handModelMatrix * (translationMatrix * rotationMatrix * scaleMatrix);
        instanceColor = vec3(1.0f, 1.0f, 1.0f);
        glUniform3fv(colorLocation, 1, &instanceColor[0]);
        glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);
        if (isTriangles)
            glDrawArrays(GL_TRIANGLES, 0, 36);
        else if (isLines)
            glDrawArrays(GL_LINES, 0, 36);
        else if (isPoints)
            glDrawArrays(GL_POINTS, 0, 36);

        // DRAWING top left of racket
        rotationMatrix = rotate(mat4(1.0f), radians(-45.0f), vec3(0.0f, 0.0f, 1.0f));
        translationMatrix = translate(mat4(1.0f), vec3(4.32f, 6.7f * 2.08, -1.0f));
        scaleMatrix = scale(mat4(1.0f), vec3(0.5f, 0.55f, 1.0f));
        modelMatrix = handModelMatrix * (translationMatrix * rotationMatrix * scaleMatrix);
        instanceColor = vec3(1.0f, 1.0f, 1.0f);
        glUniform3fv(colorLocation, 1, &instanceColor[0]);
        glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);
        if (isTriangles)
            glDrawArrays(GL_TRIANGLES, 0, 36);
        else if (isLines)
            glDrawArrays(GL_LINES, 0, 36);
        else if (isPoints)
            glDrawArrays(GL_POINTS, 0, 36);

        // DRAWING right leg of racket
        rotationMatrix = rotate(mat4(1.0f), radians(0.0f), vec3(0.0f, 0.0f, 1.0f));
        translationMatrix = translate(mat4(1.0f), vec3(5.3f, 6.7f * 1.89, -1.0f));
        scaleMatrix = scale(mat4(1.0f), vec3(0.5f, 1.0f, 1.0f));
        modelMatrix = handModelMatrix * (translationMatrix * rotationMatrix * scaleMatrix);
        instanceColor = vec3(1.0f, 0.0f, 0.0f);
        glUniform3fv(colorLocation, 1, &instanceColor[0]);
        glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);
        if (isTriangles)
            glDrawArrays(GL_TRIANGLES, 0, 36);
        else if (isLines)
            glDrawArrays(GL_LINES, 0, 36);
        else if (isPoints)
            glDrawArrays(GL_POINTS, 0, 36);

        // DRAWING left leg of racket
        rotationMatrix = rotate(mat4(1.0f), radians(0.0f), vec3(0.0f, 0.0f, 1.0f));
        translationMatrix = translate(mat4(1.0f), vec3(4.f, 6.7f * 1.89, -1.0f));
        scaleMatrix = scale(mat4(1.0f), vec3(0.5f, 1.0f, 1.0f));
        modelMatrix = handModelMatrix * (translationMatrix * rotationMatrix * scaleMatrix);
        instanceColor = vec3(1.0f, 0.0f, 0.0f);
        glUniform3fv(colorLocation, 1, &instanceColor[0]);
        glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);
        if (isTriangles)
            glDrawArrays(GL_TRIANGLES, 0, 36);
        else if (isLines)
            glDrawArrays(GL_LINES, 0, 36);
        else if (isPoints)
            glDrawArrays(GL_POINTS, 0, 36);

        // DRAWING right leg of racket
        rotationMatrix = rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 0.0f, 1.0f));
        translationMatrix = translate(mat4(1.0f), vec3(4.6f, 6.7f * 1.72, -1.0f));
        scaleMatrix = scale(mat4(1.0f), vec3(0.1f, 0.4f, 1.0f));
        modelMatrix = handModelMatrix * (translationMatrix * rotationMatrix * scaleMatrix);
        instanceColor = vec3(1.0f, 0.0f, 0.0f);
        glUniform3fv(colorLocation, 1, &instanceColor[0]);
        glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);
        if (isTriangles)
            glDrawArrays(GL_TRIANGLES, 0, 36);
        else if (isLines)
            glDrawArrays(GL_LINES, 0, 36);
        else if (isPoints)
            glDrawArrays(GL_POINTS, 0, 36);

        //drawing racket grid down up
        float space = 0.0f;
        for (int i = 0; i < 8; i++) {
            space += 0.3f;
            rotationMatrix = rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 0.0f, 1.0f));
            translationMatrix = translate(mat4(1.0f), vec3(4.6f, 6.7f * 1.72 + space, -1.0f));
            scaleMatrix = scale(mat4(1.0f), vec3(0.0009f, 0.6f, 1.0f));
            if (i == 7)
                scaleMatrix = scale(mat4(1.0f), vec3(0.0009f, 0.5f, 1.0f));
            modelMatrix = handModelMatrix * (translationMatrix * rotationMatrix * scaleMatrix);
            instanceColor = vec3(0.0f, 1.0f, 0.0f);
            glUniform3fv(colorLocation, 1, &instanceColor[0]);
            glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);
            if (isTriangles)
                glDrawArrays(GL_TRIANGLES, 0, 36);
            else if (isLines)
                glDrawArrays(GL_LINES, 0, 36);
            else if (isPoints)
                glDrawArrays(GL_POINTS, 0, 36);
        }

        //Drawing grid of racket left to right
        float spacing = 0.0f;
        for (int i = 0; i < 6; i++) {
            spacing += 0.2f;
            rotationMatrix = rotate(mat4(1.0f), radians(0.0f), vec3(0.0f, 0.0f, 1.0f));
            translationMatrix = translate(mat4(1.0f), vec3(3.9f + spacing, 6.7f * 1.89, -1.0f));
            scaleMatrix = scale(mat4(1.0f), vec3(0.09f, 1.15f, 1.0f));
            if (i == 1 || i == 2 || i == 3 || i == 4)
                scaleMatrix = scale(mat4(1.0f), vec3(0.09f, 1.25f, 1.0f));
            modelMatrix = handModelMatrix * (translationMatrix * rotationMatrix * scaleMatrix);
            instanceColor = vec3(0.0f, 1.0f, 0.0f);
            glUniform3fv(colorLocation, 1, &instanceColor[0]);
            glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);
            if (isTriangles)
                glDrawArrays(GL_TRIANGLES, 0, 36);
            else if (isLines)
                glDrawArrays(GL_LINES, 0, 36);
            else if (isPoints)
                glDrawArrays(GL_POINTS, 0, 36);
        }

        //EXTRA DESIGN FOR EXTRA GRADES
        // DRAWING W for wilson on tennis racket
        float sp = 0.0f;
        for (int i = 0; i < 3; i++) {
            sp += 0.2;
            rotationMatrix = rotate(mat4(1.0f), radians(0.0f), vec3(0.0f, 0.0f, 1.0f));
            translationMatrix = translate(mat4(1.0f), vec3(4.2f + sp, 6.7f * 1.9, -0.9f));
            scaleMatrix = scale(mat4(1.0f), vec3(0.25f, 0.3f, 1.0f));
            modelMatrix = handModelMatrix * (translationMatrix * rotationMatrix * scaleMatrix);
            instanceColor = vec3(1.0f, 0.0f, 1.0f);
            glUniform3fv(colorLocation, 1, &instanceColor[0]);
            glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);
            if (isTriangles)
                glDrawArrays(GL_TRIANGLES, 0, 36);
            else if (isLines)
                glDrawArrays(GL_LINES, 0, 36);
            else if (isPoints)
                glDrawArrays(GL_POINTS, 0, 36);
        }

        rotationMatrix = rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 0.0f, 1.0f));
        translationMatrix = translate(mat4(1.0f), vec3(4.6f, 6.7f * 1.836, -0.9f));
        scaleMatrix = scale(mat4(1.0f), vec3(0.25f, 0.3f, 1.0f));
        modelMatrix = handModelMatrix * (translationMatrix * rotationMatrix * scaleMatrix);
        instanceColor = vec3(1.0f, 0.0f, 1.0f);
        glUniform3fv(colorLocation, 1, &instanceColor[0]);
        glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);
        if (isTriangles)
            glDrawArrays(GL_TRIANGLES, 0, 36);
        else if (isLines)
            glDrawArrays(GL_LINES, 0, 36);
        else if (isPoints)
            glDrawArrays(GL_POINTS, 0, 36);



        translationMatrix = translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.0f));//important to reset the translation matrix
        rotationMatrix = rotate(mat4(1.0f), radians(0.0f), vec3(0.0f, 1.0f, 0.0f));
        modelMatrix = translationMatrix * rotationMatrix;
        glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);

        glBindVertexArray(vao);//drawing the grid
        instanceColor = vec3(1.0f, 1.0f, 0.0f);
        glUniform3fv(colorLocation, 1, &instanceColor[0]);
        glDrawArrays(GL_LINES, 0, 2); // 3 vertices, starting at index 0
        float i = 0.0f;
        while (i < 20.0f) {//left to right lines
            i += 0.2f;
            translationMatrix = translate(mat4(1.0f), vec3(0.0f + i, 0.0f, 0.0f));
            modelMatrix = translationMatrix * rotationMatrix;
            glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);
            glDrawArrays(GL_LINES, 0, 2);
        }

        translationMatrix = translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.0f));//detranlating the matrix
        modelMatrix = translationMatrix * rotationMatrix;
        glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);

        rotationMatrix = rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 1.0f, 0.0f));
        modelMatrix = translationMatrix * rotationMatrix;
        glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);
        glDrawArrays(GL_LINES, 0, 2);
        i = 0;
        while (i < 20.0f) {//down to up lines
            i += 0.2f;
            translationMatrix = translate(mat4(1.0f), vec3(0.0f, 0.0f, -20.0f + i));
            modelMatrix = translationMatrix * rotationMatrix;
            glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);
            glDrawArrays(GL_LINES, 0, 2);
        }
        //when ledt mouse button is pressed we zoom
        if (leftMouseButtonPressed) {
            double mouseX, mouseY;
            glfwGetCursorPos(window, &mouseX, &mouseY);
            double deltaX = mouseX - lastMouseX;
            double deltaY = mouseY - lastMouseY;
            lastMouseX = mouseX;
            lastMouseY = mouseY;

            // Sensitivity factor for zooming in and out
            float zoomSensitivity = 0.03f;

            // Update camera distance based on mouse movement (zoom in and out)
            cameraDistance += static_cast<float>(deltaY) * zoomSensitivity;

            // Limit the camera distance to avoid zooming in too close or going through the object
            float minDistance = 4.0f;
            float maxDistance = 50.0f;
            cameraDistance = clamp(cameraDistance, minDistance, maxDistance);

            // Update the camera position
            cameraPosition = vec3(cameraX, cameraY, cameraZ);
        }
        //handling mouse buttons
        if (rightMouseButtonPressed || middleMouseButtonPressed)
        {
            double mouseX, mouseY;
            glfwGetCursorPos(window, &mouseX, &mouseY);
            double deltaX = mouseX - lastMouseX;
            double deltaY = mouseY - lastMouseY;
            lastMouseX = mouseX;
            lastMouseY = mouseY;

            // Sensitivity factor for panning and tilting
            float panTiltSensitivity = 0.01f;

            if (rightMouseButtonPressed)
            {
                // Update camera horizontal angle for panning
                cameraHorizontalAngle += static_cast<float>(deltaX) * panTiltSensitivity;
            }

            if (middleMouseButtonPressed)
            {
                // Update camera vertical angle for tilting
                cameraVerticalAngle += static_cast<float>(deltaY) * panTiltSensitivity;

                // Limit the camera vertical angle to avoid over-tilting
                float minVerticalAngle = -pi<float>() / 2.0f + 0.1f;
                float maxVerticalAngle = pi<float>() / 2.0f - 0.1f;
                cameraVerticalAngle = clamp(cameraVerticalAngle, minVerticalAngle, maxVerticalAngle);
            }

            // Recalculate the camera position based on the updated angles
            float cameraX = objectPosition.x + cameraDistance * sin(cameraHorizontalAngle) * cos(cameraVerticalAngle);
            float cameraY = objectPosition.y + cameraDistance * sin(cameraVerticalAngle);
            float cameraZ = objectPosition.z + cameraDistance * cos(cameraHorizontalAngle) * cos(cameraVerticalAngle);
            cameraPosition = vec3(cameraX, cameraY, cameraZ);
        }


        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        // End Frame
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // Shutdown GLFW
    glfwTerminate();

    return 0;
}

void keyCall(GLFWwindow* window, int key, int scancode, int action, int mods) {

    // input handling
    //if space key is pressed, randomly places the model in the grid
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {

        float randomX = ((rand() % 160) / 10) - 8;
        float randomZ = ((rand() % 160) / 10) - 8;

        locationX = randomX;
        locationZ = randomZ;


        handTranslation = translate(mat4(1.0f), vec3(locationX, locationY, locationZ));
        handModelMatrix = handTranslation * handRotation * handScalar;
        modelMatrix = handModelMatrix * (translationMatrix * rotationMatrix * scaleMatrix);
        glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);
    }
    //This button scales up the model
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
    {
        scaleX += 0.1f;
        scaleY += 0.1f;
        scaleZ += 0.1f;

        handScalar = scale(mat4(1.0f), vec3(scaleX, scaleY, scaleZ));
        handModelMatrix = handTranslation * handRotation * handScalar;
        modelMatrix = handModelMatrix * (translationMatrix * rotationMatrix * scaleMatrix);
        glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);
    }
    //this button scales down the model
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
    {
        scaleX -= 0.05f;
        scaleY -= 0.05f;
        scaleZ -= 0.05f;
        handScalar = scale(mat4(1.0f), vec3(scaleX, scaleY, scaleZ));
        handModelMatrix = handTranslation * handRotation * handScalar;
        modelMatrix = handModelMatrix * (translationMatrix * rotationMatrix * scaleMatrix);
        glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);
    }
    //moving the model using A key for left 
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        locationX -= 1.0f;

        handTranslation = translate(mat4(1.0f), vec3(locationX, locationY, locationZ));
        handModelMatrix = handTranslation * handRotation * handScalar;
        modelMatrix = handModelMatrix * (translationMatrix * rotationMatrix * scaleMatrix);
        glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);
    }
    //moving the model using D key for right 
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        locationX += 1.0f;

        handTranslation = translate(mat4(1.0f), vec3(locationX, locationY, locationZ));
        handModelMatrix = handTranslation * handRotation * handScalar;
        modelMatrix = handModelMatrix * (translationMatrix * rotationMatrix * scaleMatrix);
        glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);
    }
    //moving the model using S key for down
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        locationY -= 1.0f;

        handTranslation = translate(mat4(1.0f), vec3(locationX, locationY, locationZ));
        handModelMatrix = handTranslation * handRotation * handScalar;
        modelMatrix = handModelMatrix * (translationMatrix * rotationMatrix * scaleMatrix);
        glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);
    }
    //moving the model using W key for up
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        locationY += 1.0f;

        handTranslation = translate(mat4(1.0f), vec3(locationX, locationY, locationZ));
        handModelMatrix = handTranslation * handRotation * handScalar;
        modelMatrix = handModelMatrix * (translationMatrix * rotationMatrix * scaleMatrix);
        glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);
    }
    //when 1 is pressed the model rotates clockwise
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
        angleOfAllArm += radians(5.0f);
        handRotation = rotate(mat4(1.0f), angleOfAllArm, vec3(1.0f, 0.0f, 0.0f));
        handModelMatrix = handTranslation * handRotation * handScalar;
        modelMatrix = handModelMatrix * (translationMatrix * rotationMatrix * scaleMatrix);
        glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);
    }
    //when 2 is pressed the model rotates anticlockwise 
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
        angleOfAllArm -= radians(5.0f);

        handRotation = rotate(mat4(1.0f), angleOfAllArm, vec3(1.0f, 0.0f, 0.0f));
        handModelMatrix = handTranslation * handRotation * handScalar;
        modelMatrix = handModelMatrix * (translationMatrix * rotationMatrix * scaleMatrix);
        glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) // move camera to the left
    {
        cameraHorizontalAngle += radians(5.0f);
    }
    //moving right key to right
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) // move camera to the right
    {
        cameraHorizontalAngle -= radians(5.0f);
    }
    //moving camera down with down key
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) // move camera up
    {
        cameraVerticalAngle -= radians(5.0f);
    }
    //moving camera up with up key
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) // move camera down
    {
        cameraVerticalAngle += radians(5.0f);
    }
    //when H is pressed the camera resets to original position
    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) {
        cameraHorizontalAngle = radians(90.0f);
        cameraVerticalAngle = radians(30.0f);
    }
    //makes the model as points
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
        isTriangles = false;
        isLines = false;
        isPoints = true;
    }
    //makes the model as lines
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
        isTriangles = false;
        isLines = true;
        isPoints = false;
    }
    //makes the model as triangles
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
        isTriangles = true;
        isLines = false;
        isPoints = false;
    }
    //closes the program using Esc key
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

}
void mouseButtons(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {//if left mouse button is pressed we zoom in out
        if (action == GLFW_PRESS) {
            leftMouseButtonPressed = true;
            glfwGetCursorPos(window, &lastMouseX, &lastMouseY);
        }
        else if (action == GLFW_RELEASE) {
            leftMouseButtonPressed = false;
        }
    }
    // if right mouse button is pressed we pan camera
    if (button == GLFW_MOUSE_BUTTON_RIGHT)
    {
        if (action == GLFW_PRESS)
        {
            rightMouseButtonPressed = true;
            glfwGetCursorPos(window, &lastMouseX, &lastMouseY);
        }
        else if (action == GLFW_RELEASE)
            rightMouseButtonPressed = false;
    }
    else if (button == GLFW_MOUSE_BUTTON_MIDDLE)//if middle mouse button is pressed we tilt camera
    {
        if (action == GLFW_PRESS)
        {
            middleMouseButtonPressed = true;
            glfwGetCursorPos(window, &lastMouseX, &lastMouseY);
        }
        else if (action == GLFW_RELEASE)
            middleMouseButtonPressed = false;
    }
}