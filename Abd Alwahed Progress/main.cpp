#include <iostream>
#include <list>
#include <algorithm>

#define GLEW_STATIC 1
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "shadersFunctions.h"
#include "vaos.h"


using namespace glm;
using namespace std;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

void drawingAbdAlwahedRacket(GLint colorLocation, vec3 instanceColor);
void drawingHishamRacket(GLint colorLocation, vec3 instanceColor);


GLuint createVertexBufferObject() {

    vec3 vertexArray[] = {
            vec3(-10.0f,  0.0f, -10.0f), //top center vertex for a line
            vec3(1.0f,  1.0f, 0.0f), //first vertex color
            vec3(-10.0f,  0.0f, 10.0f),
            vec3(1.0f,  1.0f, 0.0f),
    };
    // Create a vertex array
    GLuint vertexArrayObject;
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);


    GLuint vertexBufferObject;
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexArray), vertexArray, GL_STATIC_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,2 * sizeof(vec3),(void*)nullptr);
    glEnableVertexAttribArray(0);

    // attribute 1 matches aColor in Vertex Shader
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,2 * sizeof(vec3),(void*)sizeof(vec3));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0); // VAO already stored the state we just defined, safe to unbind buffer
    glBindVertexArray(0); // Unbind to not modify the VAO

    return vertexArrayObject;
}

////////////////// ABD ALWAHED model variables
mat4 translateMatrix;
mat4 rotateMatrix;
mat4 scaleMatrix;

// upper arm variables
mat4 armModelMatrix;
mat4 armTranslate;
mat4 armRotate;
mat4 armScale;

// lower arm variables
mat4 lowerArmModelMatrix;
mat4 lowerArmRotate;

// hand variables
mat4 AbdHandRotate;
mat4 AbdHandModelMatrix;

// the following 3 variables hold the values of the scaleMatrix to facilitate modifications later on
float AbdScaleX = 0.5f, AbdScaleY = 0.5f, AbdScaleZ = 0.5f;

// the following 3 variables are for the location of the model on the grid
float AbdLocationX = -5.0f, AbdLocationY = 0.0f, AbdLocationZ = 0.0f;

// the following is the current angle of the hand
float AbdAngleOfAllArm = radians(0.0f);

// the angle of rotation for the lower arm
float AbdAngleOfLowerArm = radians(0.0f);

// the angle of rotation for the hand
float AbdAngleOfHand = radians(0.0f);

/////////// HISHAM model variables //////////////

mat4 translationMatrix;
mat4 rotationMatrix;
mat4 scalingMatrix;
mat4 handModelMatrix;
mat4 handTranslation;
mat4 handRotation;
mat4 handScalar;

// lower arm variables
mat4 lowerHandModelMatrix;
mat4 lowerHandRotate;

// hand variables
mat4 HishamHandRotate;
mat4 HishamHandModelMatrix;

// the following 3 variables hold the values of the scaleMatrix to facilitate modifications later on
float HishamScaleX = 0.5f, HishamScaleY = 0.5f, HishamScaleZ = 0.5f;

// the following 3 variables are for the location of the model on the grid
float HishamLocationX = 5.0f, HishamLocationY = 0.0f, HishamLocationZ = 0.0f;

// the following is the current angle of the hand
float HishamAngleOfAllArm = radians(180.0f);
// the angle of rotation for the lower arm
float HishamAngleOfLowerArm = radians(0.0f);

// the angle of rotation for the hand
float HishamAngleOfHand = radians(0.0f);

///////////// General variables //////////////

mat4 modelMatrix = mat4(1.0f);
GLint modelMatrixLocation;

// defining the location matrix for the light shadow
GLint lightSpaceMatrixLocation;


// Camera parameters for view transform
float cameraX, cameraY, cameraZ;
vec3 cameraPosition(0.0f, 15.0f, 15.0f);
vec3 objectPosition = vec3(0.0f, 0.0f, 0.0f); // Position of the object you want to rotate around
vec3 cameraUp(0.0f, 1.0f, 0.0f);

// Other camera parameters
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

vec3 instanceColor;
GLint colorLocation;

GLuint initialCube;
// sphere variables
int resolution = 65;
int vertexCount = 6 * (resolution / 2) * resolution + 1;
int sphere;


void drawScene(int shaderProgram) {

    glBindVertexArray(sphere);
    rotationMatrix = rotate(mat4(1.0f), radians(0.0f), vec3(0.0f, 0.0f, 1.0f));
    translationMatrix = translate(mat4(1.0f), vec3(0.0f, 5.0f, 2.0f));
    scaleMatrix = scale(mat4(1.0f), vec3(1.0f, 1.0f, 1.0f));
    modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);
    instanceColor = vec3(1.0f,1.0f,1.0f);
    glUniform3fv(colorLocation, 1, &instanceColor[0]);
    glDrawElements(GL_TRIANGLES, vertexCount ,GL_UNSIGNED_INT,(void*)0);


    glBindVertexArray(initialCube);// CUBE BASE

    mat4 groundWorldMatrix = translate(mat4(1.0f), vec3(0.0f, -0.01f, 0.0f)) * scale(mat4(1.0f), vec3(100.0f, 0.02f, 100.0f));
    modelMatrixLocation = glGetUniformLocation(shaderProgram, "modelMatrix");
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &groundWorldMatrix[0][0]);

    glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices, starting at index 0

    drawingAbdAlwahedRacket(colorLocation, instanceColor);

    drawingHishamRacket(colorLocation, instanceColor);
}

int main(int argc, char* argv[]) {

    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Create Window and rendering context using GLFW, resolution is 1024 768
    GLFWwindow* window = glfwCreateWindow(1024, 768, "Comp371 - Assignment1", nullptr, nullptr);
    if (window == nullptr) {
        cerr << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glewExperimental = true;
    if (glewInit() != GLEW_OK) {
        cerr << "Failed to create GLEW" << endl;
        glfwTerminate();
        return -1;
    }

    glClearColor(0.0f/255, 18.0f/255, 43.0f/255, 1.0f);

    int shaderProgram = compileAndLinkShaders(getVertexShaderSource(), getFragmentShaderSource());
    int depthShaderProgram = compileAndLinkShaders(getDepthVertexShaderSource(), getDepthFragmentShaderSource());


    ///////////////////////// texture shader program ///////////////////////////

    GLuint brickTextureID = loadTexture("assets/textures/clay.jpg");//load texture

    int texturedShaderProgram = compileAndLinkShaders(getTextureVertexShaderSource(), getTextureFragmentShaderSource());

    glUseProgram(shaderProgram);
    colorLocation = glGetUniformLocation(shaderProgram, "color");

    glUseProgram(texturedShaderProgram);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, brickTextureID);
    GLuint textureLocation = glGetUniformLocation(texturedShaderProgram, "textureSampler");
    glUniform1i(textureLocation, 0);

    // Set projection matrix for shader, this won't change
    mat4 projectionMatrix = perspective(70.0f,800.0f / 600.0f,0.01f, 100.0f);

    GLint projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");
    glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);

    lightSpaceMatrixLocation = glGetUniformLocation(shaderProgram, "lightSpaceMatrix");
    GLint lightSpaceDepthMatrixLocation = glGetUniformLocation(depthShaderProgram, "lightSpaceMatrix");


    /////////////////// setting up the light //////////////////////

    vec3 lightPos(5.0f, 30.0f, 5.0f);
    GLint lightLocation = glGetUniformLocation(shaderProgram, "lightPos");
    glUniform3fv(lightLocation, 1, &lightPos[0]);

    GLuint depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);

    int shadowWidth = 1024;
    int shadowHeight = 1024;

    GLuint depthTexture;
    glGenTextures(1, &depthTexture);
    glBindTexture(GL_TEXTURE_2D, depthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowWidth, shadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture, 0);

    glDrawBuffer(GL_NONE);

    GLint frameBufferWidth, frameBufferHeight;
    glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);

    lowerArmRotate = rotate(mat4(1.0f), AbdAngleOfLowerArm, vec3(0.0f, 0.0f, 1.0f));
    lowerArmModelMatrix = lowerArmRotate ;

    lowerHandRotate = rotate(mat4(1.0f), HishamAngleOfLowerArm, vec3(0.0f, 0.0f, 1.0f));
    lowerHandModelMatrix = lowerHandRotate;

    AbdHandRotate = rotate(mat4(1.0f), AbdAngleOfHand, vec3(1.0f, 0.0f, 0.0f));
    AbdHandModelMatrix = AbdHandRotate;

    HishamHandRotate = rotate(mat4(1.0f), HishamAngleOfHand, vec3(1.0f, 0.0f, 0.0f));
    HishamHandModelMatrix = HishamHandRotate;

    // Define and upload geometry to the GPU here ...
    GLuint VAO = createVertexBufferObject();
    GLuint axisX = createX();
    GLuint axisY = createY();
    GLuint axisZ = createZ();
    initialCube = createCubeCoordinate();
    sphere = createSphere(resolution, 1);
    int ground = createGround();

    // Enable Backface culling and depth test
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);


    while (!glfwWindowShouldClose(window)) {

        // Calculate the new camera position based on the camera angles
        cameraX = objectPosition.x + cameraDistance * sin(cameraHorizontalAngle) * cos(cameraVerticalAngle) ;
        cameraY = objectPosition.y + cameraDistance * sin(cameraVerticalAngle);
        cameraZ = objectPosition.z + cameraDistance * cos(cameraHorizontalAngle) * cos(cameraVerticalAngle);
        cameraPosition = vec3(cameraX, cameraY, cameraZ);


        //////////////////// the first pass to draw the shadows texture ///////////////////
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glViewport(0,0,shadowWidth, shadowHeight);

        glClear(GL_DEPTH_BUFFER_BIT);

        glUseProgram(depthShaderProgram);

        // defining light view and projection matrices
        mat4 lightViewMatrix, lightProjectionMatrix;
        lightViewMatrix = lookAt(lightPos, vec3(0.0f), vec3(0.0f, 1.0f, 0.0f));
        lightProjectionMatrix = ortho(-50.0f, 50.0f, -50.0f, 50.0f, 0.1f, 50.0f);
        mat4 lightSpaceMatrix = lightProjectionMatrix * lightViewMatrix;

        glUniformMatrix4fv(lightSpaceDepthMatrixLocation, 1, GL_FALSE, &lightSpaceMatrix[0][0]);

        drawScene(depthShaderProgram);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glUseProgram(shaderProgram);

        glUniformMatrix4fv(lightSpaceMatrixLocation, 1, GL_FALSE, &lightSpaceMatrix[0][0]);


        glViewport(0,0, frameBufferWidth, frameBufferHeight);

        // Set initial view matrix
        mat4 viewMatrix = lookAt(cameraPosition,objectPosition,cameraUp);
        GLint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
        glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //sets line width
        glLineWidth(6.5f);
        glBindVertexArray(axisX);

        glUniform3fv(colorLocation, 1, &instanceColor[0]);
        glDrawArrays(GL_LINES, 0, 2);


        glBindVertexArray(axisY);
        instanceColor = vec3(0.0f, 1.0f, 0.0f);
        glUniform3fv(colorLocation, 1, &instanceColor[0]);
        glDrawArrays(GL_LINES, 0, 2);


        glBindVertexArray(axisZ);
        instanceColor = vec3(0.0f, 0.0f, 1.0f);
        glUniform3fv(colorLocation, 1, &instanceColor[0]);
        glDrawArrays(GL_LINES, 0, 2);

        //model matrix location
        modelMatrixLocation = glGetUniformLocation(shaderProgram, "modelMatrix");
        glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);

        //projection matrix location
        projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");
        glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);

        glLineWidth(1.0f); //resets line width to original value

        // Set the keyboard key and mouse callback functions
        glfwSetKeyCallback(window, keyCallback);
        glfwSetMouseButtonCallback(window, mouseButtonCallback);

        // this will draw the rackets and the sphere
        drawScene(shaderProgram);


        translateMatrix = translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.0f));//important to reset the translation matrix
        rotateMatrix = rotate(mat4(1.0f), radians(0.0f), vec3(0.0f, 1.0f, 0.0f));
        modelMatrix = translateMatrix * rotateMatrix;
        glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);


        ////////////////////// DRAWING the world's grid /////////////////////////

        translateMatrix = translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.0f));//important to reset the translation matrix
        rotateMatrix = rotate(mat4(1.0f), radians(0.0f), vec3(0.0f, 1.0f, 0.0f));
        modelMatrix = translateMatrix * rotateMatrix;
        glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);

        /*
        glBindVertexArray(VAO);//drawing the grid

        instanceColor = vec3(129.0f/255,174.0f/255,208.0f/255);
        glUniform3fv(colorLocation, 1, &instanceColor[0]);
        glDrawArrays(GL_LINES, 0, 2); // 3 vertices, starting at index 0
        float i = 0.0f;
        while (i < 20.0f) {//left to right lines
            i += 0.2f;
            translateMatrix = translate(mat4(1.0f), vec3(0.0f + i, 0.0f, 0.0f));
            modelMatrix = translateMatrix * rotateMatrix;
            glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);
            glDrawArrays(GL_LINES, 0, 2);
        }

        translateMatrix = translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.0f));
        modelMatrix = translateMatrix * rotateMatrix;
        glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);

        rotateMatrix = rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, 1.0f, 0.0f));
        modelMatrix = translateMatrix * rotateMatrix;
        glUniformMatrix4fv(modelMatrixLocation, 1,GL_FALSE, &modelMatrix[0][0]);
        glDrawArrays(GL_LINES, 0, 2);
        i = 0;
        while (i < 20.0f) {
            i += 0.2f;
            translateMatrix = translate(mat4(1.0f), vec3(0.0f, 0.0f, -20.0f + i));
            modelMatrix = translateMatrix * rotateMatrix;
            glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);
            glDrawArrays(GL_LINES, 0, 2);
        }

         */


        glUseProgram(texturedShaderProgram);
        glBindVertexArray(ground);
        rotationMatrix = rotate(mat4(1.0f), radians(0.0f), vec3(1.0f, 1.0f, 1.0f));
        translationMatrix = translate(mat4(1.0f), vec3(2.0f, 5.0f, 2.0f));
        scaleMatrix = scale(mat4(1.0f), vec3(2.0f, 2.0f, 2.0f));
        modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;
        GLint textureModelMatrixLocation = glGetUniformLocation(texturedShaderProgram, "modelMatrix");
        glUniformMatrix4fv(textureModelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);
        glEnable(GL_TEXTURE_2D);
        glDrawArrays(GL_TRIANGLES, 0, 36);



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
            cameraX = objectPosition.x + cameraDistance * sin(cameraHorizontalAngle) * cos(cameraVerticalAngle);
            cameraY = objectPosition.y + cameraDistance * sin(cameraVerticalAngle);
            cameraZ = objectPosition.z + cameraDistance * cos(cameraHorizontalAngle) * cos(cameraVerticalAngle);
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

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    float movementSpeed = 0.5f; // Adjust the camera movement speed as needed


    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
        isTriangles = false;
        isLines = false;
        isPoints = true;
    }

    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
        isTriangles = false;
        isLines = true;
        isPoints = false;
    }

    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
        isTriangles = true;
        isLines = false;
        isPoints = false;
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {

        float randomX = ((rand() % 160)/10) - 8;
        float randomZ = ((rand() % 160)/10) - 8;

        AbdLocationX = randomX;
        AbdLocationZ = randomZ;

        randomX = ((rand() % 160)/10) - 8;
        randomZ = ((rand() % 160)/10) - 8;

        HishamLocationX = randomX;
        HishamLocationZ = randomZ;

    }

    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
    {
        AbdScaleX += 0.1f;
        AbdScaleY += 0.1f;
        AbdScaleZ += 0.1f;

        HishamScaleX += 0.1f;
        HishamScaleY += 0.1f;
        HishamScaleZ += 0.1f;

    }

    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
        AbdScaleX -= 0.05f;
        AbdScaleY -= 0.05f;
        AbdScaleZ -= 0.05f;

        HishamScaleX -= 0.05f;
        HishamScaleY -= 0.05f;
        HishamScaleZ -= 0.05f;
    }


    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        AbdLocationX-=1.0f;

        HishamLocationX-=1.0f;
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        AbdLocationX+=1.0f;

        HishamLocationX+=1.0f;
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        AbdLocationY-=1.0f;

        HishamLocationY-=1.0f;
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        AbdLocationY+=1.0f;

        HishamLocationY+=1.0f;
    }


    /*
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        cameraPosition -= normalize(cross(cameraUp, objectPosition - cameraPosition)) * movementSpeed;
        cameraDistance = length(cameraPosition - objectPosition);

    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        cameraPosition += normalize(cross(cameraUp, objectPosition - cameraPosition)) * movementSpeed;
        cameraDistance = length(cameraPosition - objectPosition);
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        cameraPosition += normalize(objectPosition - cameraPosition) * movementSpeed;
        cameraDistance = length(cameraPosition - objectPosition);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        cameraPosition -= normalize(objectPosition - cameraPosition) * movementSpeed;
        cameraDistance = length(cameraPosition - objectPosition);
    }
    */

    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
        AbdAngleOfAllArm += radians(5.0f);

        HishamAngleOfAllArm -= radians(5.0f);
    }

    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
        AbdAngleOfAllArm -= radians(5.0f);

        HishamAngleOfAllArm += radians(5.0f);
    }

    // the next 2 if statements are for lower arm

    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) {
        vec3 trans = vec3(4.3f,4.5f,2.0f);
        mat4 translate1 = translate(mat4(1.0f), trans);
        mat4 translate2 = translate(mat4(1.0f), -trans);

        AbdAngleOfLowerArm += radians(5.0f);
        HishamAngleOfLowerArm -= radians(5.0f);

        lowerArmRotate = rotate(mat4(1.0f), AbdAngleOfLowerArm, vec3(0.0f, 0.0f, 1.0f));
        lowerArmModelMatrix = translate1 * lowerArmRotate * translate2;

        lowerHandRotate = rotate(mat4(1.0f), AbdAngleOfLowerArm, vec3(0.0f, 0.0f, 1.0f));
        lowerHandModelMatrix = translate1 * lowerArmRotate * translate2;

    }

    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
        vec3 trans = vec3(4.3f,4.5f,2.0f);
        mat4 translate1 = translate(mat4(1.0f), trans);
        mat4 translate2 = translate(mat4(1.0f), -trans);

        AbdAngleOfLowerArm -= radians(5.0f);
        HishamAngleOfLowerArm += radians(5.0f);

        lowerArmRotate = rotate(mat4(1.0f), AbdAngleOfLowerArm, vec3(0.0f, 0.0f, 1.0f));
        lowerArmModelMatrix = translate1 * lowerArmRotate * translate2;

        lowerHandRotate = rotate(mat4(1.0f), AbdAngleOfLowerArm, vec3(0.0f, 0.0f, 1.0f));
        lowerHandModelMatrix = translate1 * lowerArmRotate * translate2;
    }

    // the next 2 if statements are for the hand

    if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS) {
        vec3 trans = vec3(4.0f,7.5f,2.0f);
        mat4 translate1 = translate(mat4(1.0f), trans);
        mat4 translate2 = translate(mat4(1.0f), -trans);

        AbdAngleOfHand += radians(5.0f);
        HishamAngleOfHand -= radians(5.0f);

        AbdHandRotate = rotate(mat4(1.0f), AbdAngleOfHand, vec3(0.0f, 0.0f, 1.0f));
        AbdHandModelMatrix = translate1 * AbdHandRotate * translate2;

        HishamHandRotate = rotate(mat4(1.0f), HishamAngleOfHand, vec3(0.0f, 0.0f, 1.0f));
        HishamHandModelMatrix = translate1 * HishamHandRotate * translate2;

    }

    if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS) {
        vec3 trans = vec3(4.0f,7.5f,2.0f);
        mat4 translate1 = translate(mat4(1.0f), trans);
        mat4 translate2 = translate(mat4(1.0f), -trans);

        AbdAngleOfHand -= radians(5.0f);
        HishamAngleOfHand += radians(5.0f);

        AbdHandRotate = rotate(mat4(1.0f), AbdAngleOfHand, vec3(0.0f, 0.0f, 1.0f));
        AbdHandModelMatrix = translate1 * AbdHandRotate * translate2;

        HishamHandRotate = rotate(mat4(1.0f), HishamAngleOfHand, vec3(0.0f, 0.0f, 1.0f));
        HishamHandModelMatrix = translate1 * HishamHandRotate * translate2;
    }

    // move camera to the left
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        cameraHorizontalAngle += radians(5.0f);
    }

    // move camera to the right
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        cameraHorizontalAngle -= radians(5.0f);
    }

    // move camera up
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        cameraVerticalAngle -= radians(5.0f);
    }

    // move camera down
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        cameraVerticalAngle += radians(5.0f);
    }

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        cameraHorizontalAngle = radians(90.0f);
        cameraVerticalAngle = radians(30.0f);
    }

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            leftMouseButtonPressed = true;
            glfwGetCursorPos(window, &lastMouseX, &lastMouseY);
        } else if (action == GLFW_RELEASE) {
            leftMouseButtonPressed = false;
        }
    }

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
    else if (button == GLFW_MOUSE_BUTTON_MIDDLE)
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

void drawingAbdAlwahedRacket(GLint colorLocation, vec3 instanceColor) {

    armTranslate = translate(mat4(1.0f), vec3(AbdLocationX, AbdLocationY, AbdLocationZ));
    armRotate = rotate(mat4(1.0f), AbdAngleOfAllArm, vec3(0.0f, 1.0f, 0.0f));
    armScale = scale(mat4(1.0f), vec3(AbdScaleX, AbdScaleY, AbdScaleZ));

    armModelMatrix = armTranslate * armRotate * armScale;

    ////////////////// DRAWING LOWER ARM  ////////////////////////

    translateMatrix = translate(mat4(1.0f), vec3(3.0f, 3.0f, -1.0f));
    rotateMatrix = rotate(mat4(1.0f), radians(-45.0f), vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = scale(mat4(1.0f), vec3(1.0f, 2.0f, 1.0f));
    modelMatrix = armModelMatrix * (translateMatrix * rotateMatrix * scaleMatrix);
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);

    instanceColor=vec3(255.0f/255,238.0f/255,109.0f/255);
    glUniform3fv(colorLocation, 1, &instanceColor[0]);

    if (isTriangles)
        glDrawArrays(GL_TRIANGLES, 0, 36);
    else if (isLines)
        glDrawArrays(GL_LINES, 0, 36);
    else if (isPoints)
        glDrawArrays(GL_POINTS, 0, 36);

    /////////////// DRAWING UPPER ARM //////////////////////

    rotateMatrix = rotate(mat4(1.0f), radians(0.0f), vec3(0.0f, 0.0f, 1.0f));
    translateMatrix = translate(mat4(1.0f), vec3(4.33f, 6.29f, -1.0f));
    modelMatrix = armModelMatrix * lowerArmModelMatrix * (translateMatrix * rotateMatrix * scaleMatrix);
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);

    if (isTriangles)
        glDrawArrays(GL_TRIANGLES, 0, 36);
    else if (isLines)
        glDrawArrays(GL_LINES, 0, 36);
    else if (isPoints)
        glDrawArrays(GL_POINTS, 0, 36);

    ///////////////////// DRAWING HAND ///////////////////

    rotateMatrix = rotate(mat4(1.0f), radians(45.0f), vec3(0.0f, 0.0f, 1.0f));
    translateMatrix = translate(mat4(1.0f), vec3(4.33f, 6.29f * 1.34, -1.0f));
    scaleMatrix = scale(mat4(1.0f), vec3(3.0f, 0.5f, 1.0f));
    modelMatrix = armModelMatrix * lowerArmModelMatrix * AbdHandModelMatrix * (translateMatrix * rotateMatrix * scaleMatrix);
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);


    if (isTriangles)
        glDrawArrays(GL_TRIANGLES, 0, 36);
    else if (isLines)
        glDrawArrays(GL_LINES, 0, 36);
    else if (isPoints)
        glDrawArrays(GL_POINTS, 0, 36);

    ///////////////////// DRAWING FINGERS /////////////////
    for (int i = 0; i <5 ; i++) {
        float translation = i * 0.29f;
        rotateMatrix = rotate(mat4(1.0f), radians( 45.0f), vec3(0.0f, 0.0f, 1.0f));
        translateMatrix = translate(mat4(1.0f), vec3(3.4f + translation, (6.1f * 1.4) + translation, -1.0f));

        if (i == 4) {
            rotateMatrix = rotate(mat4(1.0f), radians(-45.0f), vec3(0.0f, 0.0f, 1.0f));
            translateMatrix = translate(mat4(1.0f), vec3(5.1f , (6.1f * 1.5) , -1.0f));
        }
        scaleMatrix = scale(mat4(1.0f), vec3(0.50f, 0.5f, 0.5f));
        modelMatrix = armModelMatrix * lowerArmModelMatrix *  AbdHandModelMatrix * (translateMatrix * rotateMatrix * scaleMatrix);
        glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);

        if (isTriangles)
            glDrawArrays(GL_TRIANGLES, 0, 36);
        else if (isLines)
            glDrawArrays(GL_LINES, 0, 36);
        else if (isPoints)
            glDrawArrays(GL_POINTS, 0, 36);
    }

    //////////////////// DRAWING RACKET HANDLE ////////////////////

    float defaultRacketOrientation = -20.0f;
    float racketHandleX = 4.7f;
    float racketHandleY = 6.7f * 1.5;
    float racketHandleZ = -1.0f;

    rotateMatrix = rotate(mat4(1.0f), radians(defaultRacketOrientation), vec3(0.0f, 0.0f, 1.0f));
    translateMatrix = translate(mat4(1.0f), vec3(racketHandleX, racketHandleY, racketHandleZ));
    scaleMatrix = scale(mat4(1.0f), vec3(1.0f, 1.0f, 1.0f));
    modelMatrix = armModelMatrix * lowerArmModelMatrix *  AbdHandModelMatrix * (translateMatrix * rotateMatrix * scaleMatrix);

    instanceColor = vec3(253.0f/255.,251.0f/255,236.0f/255); // white
    glUniform3fv(colorLocation, 1, &instanceColor[0]);
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);
    if (isTriangles)
        glDrawArrays(GL_TRIANGLES, 0, 36);
    else if (isLines)
        glDrawArrays(GL_LINES, 0, 36);
    else if (isPoints)
        glDrawArrays(GL_POINTS, 0, 36);

    //////////////// DRAWING right base of racket //////////////////////

    rotateMatrix = rotate(mat4(1.0f), radians(defaultRacketOrientation-45.0f), vec3(0.0f, 0.0f, 1.0f));
    translateMatrix = translate(mat4(1.0f), vec3(racketHandleX + 1.0f, racketHandleY + 1.0f, racketHandleZ));
    scaleMatrix = scale(mat4(1.0f), vec3(1.0f, 1.0f, 1.0f));
    modelMatrix = armModelMatrix * lowerArmModelMatrix *  AbdHandModelMatrix * (translateMatrix * rotateMatrix * scaleMatrix);

    instanceColor = vec3(222.0f/255,62.0f/255,28.0f/255); // red
    glUniform3fv(colorLocation, 1, &instanceColor[0]);
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);
    if (isTriangles)
        glDrawArrays(GL_TRIANGLES, 0, 36);
    else if (isLines)
        glDrawArrays(GL_LINES, 0, 36);
    else if (isPoints)
        glDrawArrays(GL_POINTS, 0, 36);

    ////////////////// DRAWING top right of racket ///////////////////////

    rotateMatrix = rotate(mat4(1.0f), radians(defaultRacketOrientation + 45.0f), vec3(0.0f, 0.0f, 1.0f));
    translateMatrix = translate(mat4(1.0f), vec3(racketHandleX + 2.0f, racketHandleY + 4.0f, racketHandleZ));
    scaleMatrix = scale(mat4(1.0f), vec3(1.0f, 1.0f, 1.0f));
    modelMatrix = armModelMatrix * lowerArmModelMatrix *  AbdHandModelMatrix * (translateMatrix * rotateMatrix * scaleMatrix);

    instanceColor = vec3(222.0f/255,62.0f/255,28.0f/255); // red
    glUniform3fv(colorLocation, 1, &instanceColor[0]);
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);
    if (isTriangles)
        glDrawArrays(GL_TRIANGLES, 0, 36);
    else if (isLines)
        glDrawArrays(GL_LINES, 0, 36);
    else if (isPoints)
        glDrawArrays(GL_POINTS, 0, 36);

    ///////////////// DRAWING left base of racket ////////////////////
    rotateMatrix = rotate(mat4(1.0f), radians(defaultRacketOrientation + 45.0f), vec3(0.0f, 0.0f, 1.0f));
    translateMatrix = translate(mat4(1.0f), vec3(racketHandleX -0.25f, racketHandleY + 1.5f, racketHandleZ));
    scaleMatrix = scale(mat4(1.0f), vec3(1.0f, 1.0f, 1.0f));
    modelMatrix = armModelMatrix * lowerArmModelMatrix *  AbdHandModelMatrix * (translateMatrix * rotateMatrix * scaleMatrix);

    instanceColor = vec3(222.0f/255,62.0f/255,28.0f/255); // red
    glUniform3fv(colorLocation, 1, &instanceColor[0]);
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);
    if (isTriangles)
        glDrawArrays(GL_TRIANGLES, 0, 36);
    else if (isLines)
        glDrawArrays(GL_LINES, 0, 36);
    else if (isPoints)
        glDrawArrays(GL_POINTS, 0, 36);

    /////////////////  DRAWING top left of racket //////////////////////

    rotateMatrix = rotate(mat4(1.0f), radians(defaultRacketOrientation-45.0f), vec3(0.0f, 0.0f, 1.0f));
    translateMatrix = translate(mat4(1.0f), vec3(racketHandleX +0.8f, racketHandleY + 4.6f, racketHandleZ));
    scaleMatrix = scale(mat4(1.0f), vec3(1.0f, 1.0f, 1.0f));
    modelMatrix = armModelMatrix * lowerArmModelMatrix *  AbdHandModelMatrix * (translateMatrix * rotateMatrix * scaleMatrix);

    instanceColor = vec3(222.0f/255,62.0f/255,28.0f/255); // red
    glUniform3fv(colorLocation, 1, &instanceColor[0]);
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);
    if (isTriangles)
        glDrawArrays(GL_TRIANGLES, 0, 36);
    else if (isLines)
        glDrawArrays(GL_LINES, 0, 36);
    else if (isPoints)
        glDrawArrays(GL_POINTS, 0, 36);

    ////////////////// DRAWING right leg of racket ///////////////////

    rotateMatrix = rotate(mat4(1.0f), radians(defaultRacketOrientation), vec3(0.0f, 0.0f, 1.0f));
    translateMatrix = translate(mat4(1.0f), vec3(racketHandleX + 2.15f, racketHandleY + 2.25f, racketHandleZ));
    scaleMatrix = scale(mat4(1.0f), vec3(1.0f, 1.0f, 1.0f));
    modelMatrix = armModelMatrix * lowerArmModelMatrix *  AbdHandModelMatrix * (translateMatrix * rotateMatrix * scaleMatrix);

    instanceColor = vec3(253.0f/255.,251.0f/255,236.0f/255); // white
    glUniform3fv(colorLocation, 1, &instanceColor[0]);
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);
    if (isTriangles)
        glDrawArrays(GL_TRIANGLES, 0, 36);
    else if (isLines)
        glDrawArrays(GL_LINES, 0, 36);
    else if (isPoints)
        glDrawArrays(GL_POINTS, 0, 36);

    //////////////////// DRAWING left leg of racket ////////////////////

    rotateMatrix = rotate(mat4(1.0f), radians(defaultRacketOrientation), vec3(0.0f, 0.0f, 1.0f));
    translateMatrix = translate(mat4(1.0f), vec3(racketHandleX - .35f, racketHandleY + 3.25f, racketHandleZ));
    scaleMatrix = scale(mat4(1.0f), vec3(1.0f, 1.0f, 1.0f));
    modelMatrix = armModelMatrix * lowerArmModelMatrix *  AbdHandModelMatrix * (translateMatrix * rotateMatrix * scaleMatrix);

    instanceColor = vec3(253.0f/255.,251.0f/255,236.0f/255); // white
    glUniform3fv(colorLocation, 1, &instanceColor[0]);
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);
    if (isTriangles)
        glDrawArrays(GL_TRIANGLES, 0, 36);
    else if (isLines)
        glDrawArrays(GL_LINES, 0, 36);
    else if (isPoints)
        glDrawArrays(GL_POINTS, 0, 36);

    //////////////////// DRAWING racket grid down up /////////////////////
    float space = 0.0f;
    for (int i = 0; i < 7; i++) {
        space += 0.5f;
        rotateMatrix = rotate(mat4(1.0f), radians(  defaultRacketOrientation + 90.0f), vec3(0.0f, 0.0f, 1.0f));
        translateMatrix = translate(mat4(1.0f), vec3(4.9f + space*0.3f, 6.3f * 1.72 + space, -1.0f));
        scaleMatrix = scale(mat4(1.0f), vec3(0.01f, 1.2f, 1.0f));
        if(i==6 || i==0)
            scaleMatrix = scale(mat4(1.0f), vec3(0.01f, 0.5f, 1.0f));
        modelMatrix = armModelMatrix * lowerArmModelMatrix * AbdHandModelMatrix *  (translateMatrix * rotateMatrix * scaleMatrix);
        instanceColor = vec3(129.0f/255,174.0f/255,208.0f/255);
        glUniform3fv(colorLocation, 1, &instanceColor[0]);
        glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);
        if (isTriangles)
            glDrawArrays(GL_TRIANGLES, 0, 36);
        else if (isLines)
            glDrawArrays(GL_LINES, 0, 36);
        else if (isPoints)
            glDrawArrays(GL_POINTS, 0, 36);
    }

    ///////////////////// DRAWING grid of racket left to right ///////////////////

    float spacing = 0.0f;
    for (int i = 0; i < 6 ; i++) {
        spacing += 0.5f;
        rotateMatrix = rotate(mat4(1.0f), radians(defaultRacketOrientation), vec3(0.0f, 0.0f, 1.0f));
        translateMatrix = translate(mat4(1.0f), vec3(3.9f + spacing, 7.5f * 1.80 - spacing*0.4f, -1.0f));
        scaleMatrix = scale(mat4(1.0f), vec3(0.09f, 1.15f, 1.0f));
        if(i==1||i==2||i==3||i==4)
            scaleMatrix = scale(mat4(1.0f), vec3(0.09f, 1.55f, 1.0f));
        modelMatrix = armModelMatrix * lowerArmModelMatrix *  AbdHandModelMatrix * (translateMatrix * rotateMatrix * scaleMatrix);

        instanceColor = vec3(129.0f/255,174.0f/255,208.0f/255);
        glUniform3fv(colorLocation, 1, &instanceColor[0]);
        glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);
        if (isTriangles)
            glDrawArrays(GL_TRIANGLES, 0, 36);
        else if (isLines)
            glDrawArrays(GL_LINES, 0, 36);
        else if (isPoints)
            glDrawArrays(GL_POINTS, 0, 36);
    }

    translationMatrix = translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.0f));//important to reset the translation matrix
    rotationMatrix = rotate(mat4(1.0f), radians(0.0f), vec3(0.0f, 1.0f, 0.0f));
    modelMatrix = translationMatrix * rotationMatrix;
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);

}

void drawingHishamRacket(GLint colorLocation, vec3 instanceColor) {

    handTranslation = translate(mat4(1.0f), vec3(HishamLocationX, HishamLocationY, HishamLocationZ));
    handRotation = rotate(mat4(1.0f), HishamAngleOfAllArm, vec3(0.0f, 1.0f, 0.0f));
    handScalar = scale(mat4(1.0f), vec3(HishamScaleX, HishamScaleY, HishamScaleZ));

    handModelMatrix = handTranslation * handRotation * handScalar;

    translationMatrix = translate(mat4(1.0f), vec3(3.0f, 3.0f, -1.0f));
    rotationMatrix = rotate(mat4(1.0f), radians(-45.0f), vec3(0.0f, 0.0f, 1.0f));
    scalingMatrix = scale(mat4(1.0f), vec3(1.0f, 2.0f, 1.0f));
    modelMatrix = handModelMatrix * (translationMatrix * rotationMatrix * scalingMatrix);
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);

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
    modelMatrix = handModelMatrix * lowerHandModelMatrix * (translationMatrix * rotationMatrix * scalingMatrix);
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
    scalingMatrix = scale(mat4(1.0f), vec3(3.0f, 0.5f, 1.0f));
    modelMatrix = handModelMatrix * lowerHandModelMatrix * HishamHandModelMatrix * (translationMatrix * rotationMatrix * scalingMatrix);
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
        scalingMatrix = scale(mat4(1.0f), vec3(0.50f, 0.5f, 0.5f));
        modelMatrix = handModelMatrix * lowerHandModelMatrix * HishamHandModelMatrix * (translationMatrix * rotationMatrix * scalingMatrix);
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
    scalingMatrix = scale(mat4(1.0f), vec3(0.5f, 1.0f, 1.0f));
    modelMatrix = handModelMatrix * lowerHandModelMatrix * HishamHandModelMatrix * (translationMatrix * rotationMatrix * scalingMatrix);
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
    scalingMatrix = scale(mat4(1.0f), vec3(0.5f, 0.5f, 1.0f));
    modelMatrix = handModelMatrix * lowerHandModelMatrix * HishamHandModelMatrix * (translationMatrix * rotationMatrix * scalingMatrix);
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
    scalingMatrix = scale(mat4(1.0f), vec3(0.5f, 0.55f, 1.0f));
    modelMatrix = handModelMatrix * lowerHandModelMatrix * HishamHandModelMatrix * (translationMatrix * rotationMatrix * scalingMatrix);
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
    scalingMatrix = scale(mat4(1.0f), vec3(0.5f, 0.5f, 1.0f));
    modelMatrix = handModelMatrix * lowerHandModelMatrix * HishamHandModelMatrix * (translationMatrix * rotationMatrix * scalingMatrix);
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
    scalingMatrix = scale(mat4(1.0f), vec3(0.5f, 0.55f, 1.0f));
    modelMatrix = handModelMatrix * lowerHandModelMatrix * HishamHandModelMatrix * (translationMatrix * rotationMatrix * scalingMatrix);
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
    scalingMatrix = scale(mat4(1.0f), vec3(0.5f, 1.0f, 1.0f));
    modelMatrix = handModelMatrix * lowerHandModelMatrix * HishamHandModelMatrix * (translationMatrix * rotationMatrix * scalingMatrix);
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
    scalingMatrix = scale(mat4(1.0f), vec3(0.5f, 1.0f, 1.0f));
    modelMatrix = handModelMatrix * lowerHandModelMatrix * HishamHandModelMatrix * (translationMatrix * rotationMatrix * scalingMatrix);
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
    scalingMatrix = scale(mat4(1.0f), vec3(0.1f, 0.4f, 1.0f));
    modelMatrix = handModelMatrix * lowerHandModelMatrix * HishamHandModelMatrix * (translationMatrix * rotationMatrix * scalingMatrix);
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
        scalingMatrix = scale(mat4(1.0f), vec3(0.0009f, 0.6f, 1.0f));
        if (i == 7)
            scalingMatrix = scale(mat4(1.0f), vec3(0.0009f, 0.5f, 1.0f));
        modelMatrix = handModelMatrix * lowerHandModelMatrix * HishamHandModelMatrix * (translationMatrix * rotationMatrix * scalingMatrix);
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
        scalingMatrix = scale(mat4(1.0f), vec3(0.09f, 1.15f, 1.0f));
        if (i == 1 || i == 2 || i == 3 || i == 4)
            scalingMatrix = scale(mat4(1.0f), vec3(0.09f, 1.25f, 1.0f));
        modelMatrix = handModelMatrix * lowerHandModelMatrix * HishamHandModelMatrix * (translationMatrix * rotationMatrix * scalingMatrix);
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
        scalingMatrix = scale(mat4(1.0f), vec3(0.25f, 0.3f, 1.0f));
        modelMatrix = handModelMatrix * lowerHandModelMatrix * HishamHandModelMatrix * (translationMatrix * rotationMatrix * scalingMatrix);
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
    scalingMatrix = scale(mat4(1.0f), vec3(0.25f, 0.3f, 1.0f));
    modelMatrix = handModelMatrix * lowerHandModelMatrix * HishamHandModelMatrix * (translationMatrix * rotationMatrix * scalingMatrix);
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
}
